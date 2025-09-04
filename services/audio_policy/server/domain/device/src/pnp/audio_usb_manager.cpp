/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef LOG_TAG
#define LOG_TAG "AudioUsbManager"
#endif

#include "audio_usb_manager.h"

#include <sstream>
#include <dirent.h>
#include <fstream>

#include "common_event_manager.h"
#include "common_event_support.h"
#include "usb_srv_client.h"

#include "audio_server_proxy.h"

namespace OHOS {
namespace AudioStandard {

using namespace USB;

constexpr int16_t MAX_TRY = 10;
constexpr int16_t DELAY_MS = 100;

template<typename Func__, typename... Args__>
static void RunAsync(Func__&& func, Args__&&... args) {
    thread th(func, args...);
    pthread_setname_np(th.native_handle(), "OS_RUNASYNC");
    th.detach();
}

static string ReadTextFile(const string &file)
{
    string ret;
    ifstream fin;
    fin.open(file.c_str(), ios::binary | ios::in);
    if (fin) {
        int val;
        while ((val = fin.get()) != EOF) {
            ret.push_back(static_cast<char>(val));
        }
        fin.close();
    }
    return ret;
}

static void FillSoundCard(const string &path, SoundCard &card)
{
    DIR *dir = opendir(path.c_str());
    CHECK_AND_RETURN(dir != nullptr);
    struct dirent *tmp;
    while ((tmp = readdir(dir)) != nullptr) {
        string file(tmp->d_name);
        if (file == "usbbus") {
            card.usbBus_ = ReadTextFile(path + "/" + file);
            continue;
        } else if (file.find("pcm", 0) == 0) {
            if (file.back() == 'c') {
                card.isCapturer_ = true;
            } else if (file.back() == 'p') {
                card.isPlayer_ = true;
            }
        }
    }
    closedir(dir);
}

static string Trim(const string &str)
{
    static const set<char> WHITE_SPACE{' ', '\r', '\n', '\t'};
    size_t pos = 0;
    size_t end = str.length();
    for (; pos < end; pos++) {
        if (WHITE_SPACE.find(str[pos]) == WHITE_SPACE.end()) {
            break;
        }
    }
    for (; end > pos; end--) {
        if (WHITE_SPACE.find(str[end - 1]) == WHITE_SPACE.end()) {
            break;
        }
    }
    return str.substr(pos, end - pos);
}

static vector<SoundCard> GetUsbSoundCards()
{
    const string baseDir{"/proc/asound"};
    const string card{"card"};
    vector<SoundCard> soundCards;
    DIR *dir = opendir(baseDir.c_str());
    CHECK_AND_RETURN_RET(dir != nullptr, soundCards);
    struct dirent *tmp;
    int cardNum;
    while ((tmp = readdir(dir)) != nullptr) {
        string file(tmp->d_name);
        if (file.length() <= card.length() || !(file.find(card, 0) == 0)) {continue;}
        string cardNumStr = file.substr(card.length());
        if (!StrToInt(cardNumStr, cardNum)) {continue;}
        SoundCard soundCard{.cardNum_ = static_cast<uint32_t>(cardNum)};
        FillSoundCard(baseDir + "/" + file, soundCard);
        if (soundCard.usbBus_.empty()) {continue;}
        soundCards.push_back(soundCard);
    }
    closedir(dir);
    return soundCards;
}

static string GetDeviceAddr(const uint32_t cardNum)
{
    ostringstream oss;
    oss << "card=" << cardNum << ";device=0";
    return oss.str();
}

static UsbAddr GetUsbAddr(const SoundCard &card)
{
    size_t pos = card.usbBus_.find('/');
    CHECK_AND_RETURN_RET_LOG(pos != string::npos, {}, "Error Parameter: card.usbbus");
    int busNum;
    int devAddr;
    string busNumStr = Trim(card.usbBus_.substr(0, pos));
    string devAddrStr = Trim(card.usbBus_.substr(pos + 1));
    CHECK_AND_RETURN_RET_LOG(StrToInt(busNumStr, busNum) && StrToInt(devAddrStr, devAddr), {}, "StrToInt ERROR");
    return {static_cast<uint8_t>(busNum), static_cast<uint8_t>(devAddr)};
}

static bool IsAudioDevice(USB::UsbDevice &usbDevice)
{
    for (auto &usbConfig : usbDevice.GetConfigs()) {
        for (auto &usbInterface : usbConfig.GetInterfaces()) {
            if (usbInterface.GetClass() == 1 && usbInterface.GetSubClass() == 1) {
                return true;
            }
        }
    }
    return false;
}

static shared_ptr<AudioUsbManager::EventSubscriber> SubscribeCommonEvent()
{
    EventFwk::MatchingSkills matchingSkills;
    matchingSkills.AddEvent(EventFwk::CommonEventSupport::COMMON_EVENT_USB_DEVICE_ATTACHED);
    matchingSkills.AddEvent(EventFwk::CommonEventSupport::COMMON_EVENT_USB_DEVICE_DETACHED);
    EventFwk::CommonEventSubscribeInfo subscribeInfo(matchingSkills);
    subscribeInfo.SetThreadMode(EventFwk::CommonEventSubscribeInfo::COMMON);
    auto subscriber = make_shared<AudioUsbManager::EventSubscriber>(subscribeInfo);
    auto ret = EventFwk::CommonEventManager::NewSubscribeCommonEvent(subscriber);
    CHECK_AND_RETURN_RET_LOG(ret == ERR_OK, nullptr, "NewSubscribeCommonEvent Failed. ret=%{public}d", ret);
    return subscriber;
}

static bool NotSameSoundCard(const UsbAudioDevice &dev1, const UsbAudioDevice &dev2)
{
    return dev1.cardNum_ != dev2.cardNum_ || dev1.isCapturer_ != dev2.isCapturer_ || dev1.isPlayer_ != dev2.isPlayer_;
}

static void RemoveArmModuleInfoCache(const string &address)
{
    string condition = string("address=") + address + " role=" + to_string(DEVICE_ROLE_NONE);
    AudioServerProxy::GetInstance().GetAudioParameterProxy(LOCAL_NETWORK_ID, USB_DEVICE, condition);
}

string EncUsbAddr(const string &src)
{
    const string head("card=");
    auto pos = src.find(';', head.length());
    CHECK_AND_RETURN_RET_LOG(pos != string::npos, "", "Illegal usb address");
    auto num = src.substr(head.length(), pos - head.length());
    return string("c**") + num + "**";
}

AudioUsbManager &AudioUsbManager::GetInstance()
{
    static AudioUsbManager sInstance;
    return sInstance;
}

void AudioUsbManager::Init(InitCtrl initCtrl, shared_ptr<IDeviceStatusObserver> observer)
{
    lock_guard<mutex> lock(mutex_);
    initCtrl_ |= initCtrl;
    CHECK_AND_RETURN_LOG(initCtrl_ == INIT_CTRL_ALL, "Denied. initCtrl_=%{public}d", initCtrl_);
    if (!initialized_) {
#ifdef DETECT_SOUNDBOX
        AUDIO_INFO_LOG("Entry. DETECT_SOUNDBOX=true");
#else
        AUDIO_INFO_LOG("Entry. DETECT_SOUNDBOX=false");
#endif
        observer_ = observer;
        eventSubscriber_ = SubscribeCommonEvent();
        CHECK_AND_RETURN_LOG(eventSubscriber_, "SubscribeCommonEvent Failed");
        RefreshUsbAudioDevices();
        initialized_ = true;
    }
}

void AudioUsbManager::Deinit()
{
    lock_guard<mutex> lock(mutex_);
    if (initialized_) {
        if (eventSubscriber_) {
            EventFwk::CommonEventManager::NewUnSubscribeCommonEvent(eventSubscriber_);
            eventSubscriber_.reset();
        }
        audioDevices_.clear();
        soundCardMap_.clear();
        initialized_ = false;
    }
}

void AudioUsbManager::RefreshUsbAudioDevices()
{
    vector<UsbAudioDevice> devices;
    auto ret = GetUsbAudioDevices(devices);
    CHECK_AND_RETURN_LOG(ret == SUCCESS, "GetUsbAudioDevices Failed. ret=%{public}d", ret);
    vector<UsbAudioDevice> toAdd;
    for (auto &device : devices) {
        auto it = find_if(audioDevices_.cbegin(), audioDevices_.cend(), [&device](auto &item) {
            return device.usbAddr_ == item.usbAddr_ && device.name_ == item.name_;
        });
        if (it == audioDevices_.cend()) {
            toAdd.push_back(device);
        }
    }
    CHECK_AND_RETURN(!toAdd.empty());
    HandleDeviceAttachAsync(std::move(toAdd));
}

void AudioUsbManager::NotifyDevice(const UsbAudioDevice &device, const bool isConnected)
{
    DeviceType devType = device.devType_;
    string macAddress = GetDeviceAddr(device.cardNum_);
    AudioStreamInfo streamInfo{};
    string deviceName = device.name_ + "-" + to_string(device.cardNum_);
    if (device.isPlayer_) {
        AUDIO_INFO_LOG("Usb output, devType=%{public}d, isConnected=%{public}d, "
            "macAddress=%{public}s, deviceName=%{public}s, role=%{public}d", devType, isConnected,
            EncUsbAddr(macAddress).c_str(), deviceName.c_str(), DeviceRole::OUTPUT_DEVICE);
        CHECK_AND_RETURN_LOG(observer_, "observer_ is nullptr");
        observer_->OnDeviceStatusUpdated(devType, isConnected, macAddress,
            deviceName, streamInfo, OUTPUT_DEVICE, device.isCapturer_);
    }
    if (device.isCapturer_) {
        AUDIO_INFO_LOG("Usb input, devType=%{public}d, isConnected=%{public}d, "
            "macAddress=%{public}s, deviceName=%{public}s, role=%{public}d", devType, isConnected,
            EncUsbAddr(macAddress).c_str(), deviceName.c_str(), DeviceRole::INPUT_DEVICE);
        CHECK_AND_RETURN_LOG(observer_, "observer_ is nullptr");
        observer_->OnDeviceStatusUpdated(devType, isConnected, macAddress,
            deviceName, streamInfo, INPUT_DEVICE, device.isPlayer_);
    }
    if (device.devType_ == DEVICE_TYPE_USB_ARM_HEADSET && !isConnected) {
        RemoveArmModuleInfoCache(macAddress);
    }
}

map<UsbAddr, SoundCard> AudioUsbManager::GetUsbSoundCardMap()
{
    map<UsbAddr, SoundCard> cardMap;
    auto cardList = GetUsbSoundCards();
    for (auto &card : cardList) {
        cardMap[GetUsbAddr(card)] = card;
    }
    return cardMap;
}

int32_t AudioUsbManager::GetUsbAudioDevices(vector<UsbAudioDevice> &result)
{
    vector<USB::UsbDevice> deviceList;
    auto ret = UsbSrvClient::GetInstance().GetDevices(deviceList);
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ret,
        "GetDevices failed. ret=%{public}d. size=%{public}zu", ret, deviceList.size());
    for (auto &usbDevice : deviceList) {
        if (IsAudioDevice(usbDevice)) {
            result.push_back({
                {usbDevice.GetBusNum(), usbDevice.GetDevAddr()},
                usbDevice.GetProductName(),
            });
        }
    }
    return SUCCESS;
}

void AudioUsbManager::EventSubscriber::OnReceiveEvent(const EventFwk::CommonEventData &data)
{
    string action = data.GetWant().GetAction();
    AUDIO_INFO_LOG("OnReceiveEvent Entry. action=%{public}s", action.c_str());
    bool isAttach{false};
    if (action == EventFwk::CommonEventSupport::COMMON_EVENT_USB_DEVICE_ATTACHED) {
        isAttach = true;
    } else if (action == EventFwk::CommonEventSupport::COMMON_EVENT_USB_DEVICE_DETACHED) {
        isAttach = false;
    } else {
        return;
    }
    string devStr = data.GetData();
    CHECK_AND_RETURN_LOG(!devStr.empty(), "Error: data.GetData() returns empty");
    auto *devJson = cJSON_Parse(devStr.c_str());
    CHECK_AND_RETURN_LOG(devJson, "Create devJson error");
    USB::UsbDevice usbDevice(devJson);
    cJSON_Delete(devJson);
    CHECK_AND_RETURN(IsAudioDevice(usbDevice));
    UsbAudioDevice device {
        {usbDevice.GetBusNum(), usbDevice.GetDevAddr()},
        usbDevice.GetProductName()
    };
    AudioUsbManager::GetInstance().HandleAudioDeviceEvent(make_pair(device, isAttach));
}

void AudioUsbManager::HandleAudioDeviceEvent(pair<UsbAudioDevice, bool> &&p)
{
    AUDIO_INFO_LOG("Entry. deviceName=%{public}s, busNum=%{public}d, devAddr=%{public}d, isAttach=%{public}d",
        p.first.name_.c_str(), p.first.usbAddr_.busNum_, p.first.usbAddr_.devAddr_, p.second);
    if (p.second) {
        HandleDeviceAttachAsync({p.first});
    } else {
        lock_guard<mutex> lock(mutex_);
        auto it = find(audioDevices_.begin(), audioDevices_.end(), p.first);
        CHECK_AND_RETURN_LOG(it != audioDevices_.end(), "Detached Device does not exist");
        NotifyDevice(*it, false);
        audioDevices_.erase(it);
    }
}

void AudioUsbManager::HandleDeviceAttachAsync(vector<UsbAudioDevice> &&devices)
{
    CHECK_AND_RETURN(!devices.empty());
    RunAsync([this](vector<UsbAudioDevice> &&devices) {
        HandleDeviceAttach(std::move(devices));
    }, devices);
}

void AudioUsbManager::HandleDeviceAttach(vector<UsbAudioDevice> &&devices)
{
    for (int16_t cnt = 0; cnt < MAX_TRY; ++cnt) {
        if (cnt > 0) {
            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        }
        vector<UsbAudioDevice> failList;
        lock_guard<mutex> lg(mutex_);
        soundCardMap_ = GetUsbSoundCardMap();
        for (auto &item : devices) {
            if (FillUsbAudioDevice(item)) {
                UpdateDevice(item);
                NotifyDevice(item, true);
            } else {
                failList.push_back(item);
            }
        }
        CHECK_AND_RETURN(!failList.empty());
        devices.swap(failList);
    }
    AUDIO_ERR_LOG("Attach Device Failed %{public}zu", devices.size());
}

bool AudioUsbManager::FillUsbAudioDevice(UsbAudioDevice &device)
{
    auto it = soundCardMap_.find(device.usbAddr_);
    CHECK_AND_RETURN_RET_LOG(it != soundCardMap_.end(), false,
        "Error: No sound card matches usb device[%{public}s]", device.name_.c_str());
    auto &card = it->second;
    CHECK_AND_RETURN_RET_LOG(card.isPlayer_ || card.isCapturer_, false,
        "Error: Sound card[%{public}d] is not player and not capturer", card.cardNum_);
    device.cardNum_ = card.cardNum_;
    device.devType_ = DetectAudioDeviceType(card.cardNum_);
    device.isCapturer_ = card.isCapturer_;
    device.isPlayer_ = card.isPlayer_;
    return true;
}

DeviceType AudioUsbManager::DetectAudioDeviceType(uint32_t cardNum)
{
    // If has same sound card already, return value of it
    auto it = find_if(audioDevices_.cbegin(), audioDevices_.cend(), [cardNum](auto &item) {
        return item.cardNum_ == cardNum;
    });
    if (it != audioDevices_.cend()) {
        AUDIO_INFO_LOG("Same sound card[%{public}d] exists, type[%{public}d]", it->cardNum_, it->devType_);
        return it->devType_;
    }
    // If has hifi, return arm
    it = find_if(audioDevices_.cbegin(), audioDevices_.cend(), [](auto &item) {
        return item.devType_ == DEVICE_TYPE_USB_HEADSET;
    });
    if (it != audioDevices_.cend()) {
        AUDIO_INFO_LOG("Hifi sound card[%{public}d] exists, return arm", it->cardNum_);
        return DEVICE_TYPE_USB_ARM_HEADSET;
    }
    // Detect by invoke audio hal
    auto key = string("need_change_usb_device#C") + to_string(cardNum) + "D0";
    auto ret = AudioServerProxy::GetInstance().GetAudioParameterProxy(key);
    auto devType = ret == "false" ? DEVICE_TYPE_USB_ARM_HEADSET : DEVICE_TYPE_USB_HEADSET;
    AUDIO_INFO_LOG("key=%{public}s, ret=%{public}s, devType=%{public}d", key.c_str(), ret.c_str(), devType);
    return devType;
}

void AudioUsbManager::UpdateDevice(const UsbAudioDevice &dev)
{
    auto it = find(audioDevices_.begin(), audioDevices_.end(), dev);
    if (it != audioDevices_.end()) {
        if (NotSameSoundCard(dev, *it)) {
            NotifyDevice(*it, false);
        }
        *it = dev;
    } else {
        audioDevices_.push_back(dev);
    }
}
} // namespace AudioStandard
} // namespace OHOS