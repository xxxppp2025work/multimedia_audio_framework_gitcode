/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <sstream>
#include <dirent.h>
#include <fstream>
#include <thread>
#include "common_event_manager.h"
#include "common_event_support.h"
#include "usb_srv_client.h"
#include "audio_usb_manager.h"
#include "audio_policy_log.h"
#include "audio_utils.h"

namespace OHOS {
namespace AudioStandard {

using namespace USB;

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
    CHECK_AND_RETURN_RET(dir != nullptr,);
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
        SoundCard card = {.cardNum_ = static_cast<uint32_t>(cardNum)};
        FillSoundCard(baseDir + "/" + file, card);
        if (card.usbBus_.empty()) {continue;}
        soundCards.push_back(card);
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
    int busNum, devAddr;
    string busNumStr = Trim(card.usbBus_.substr(0, pos));
    string devAddrStr = Trim(card.usbBus_.substr(pos + 1));
    CHECK_AND_RETURN_RET_LOG(StrToInt(busNumStr, busNum) && StrToInt(devAddrStr, devAddr), {}, "StrToInt ERROR");
    return {static_cast<uint8_t>(busNum), static_cast<uint8_t>(devAddr)};
}

static bool IsAudioDevice(UsbDevice &usbDevice)
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

static inline bool HasDeviceAttach(list<pair<UsbAudioDevice, bool>> &messages)
{
    return find_if(messages.cbegin(), messages.cend(), [](auto &item) {
        return item.second;
    }) != messages.cend();
}

string EncUsbAddr(const string &src)
{
    const string head("card=");
    auto pos = src.find(';', head.length());
    CHECK_AND_RETURN_RET_LOG(pos != string::npos, "", "Illegal usb address");
    auto num = src.substr(head.length(), pos - head.length());
    return string("c**") + num + "**";
}

AudioUsbManager& AudioUsbManager::GetInstance()
{
    static AudioUsbManager sAudioUsbManager;
    return sAudioUsbManager;
}

void AudioUsbManager::Init(IDeviceStatusObserver *observer)
{
    lock_guard<mutex> lock(mutex_);
    if (!initialized_) {
        AUDIO_INFO_LOG("Entry");
        observer_ = observer;
        RefreshUsbAudioDevices();
        initialized_ = true;
        StartNotifyThread();
    }
}

void AudioUsbManager::StartNotifyThread()
{
    thread th([this] {
        while (initialized_) {
            auto origin = WaitMessageQueue();
            if (origin.empty()) { continue; }
            NotifyDevicesLoop(origin);
        }
    });
    pthread_setname_np(th.native_handle(), "OS_AUD_NOTI_USB");
    th.detach();
}

list<pair<UsbAudioDevice, bool>> AudioUsbManager::WaitMessageQueue()
{
    unique_lock<mutex> uLock(mqMutex_);
    ntCondition_.wait(uLock, [this] { return !messageQueue_.empty(); });
    list<pair<UsbAudioDevice, bool>> c;
    messageQueue_.swap(c);
    uLock.unlock();
    return c;
}

void AudioUsbManager::Deinit()
{
    lock_guard<mutex> lock(mutex_);
    if (initialized_) {
        observer_ = nullptr;
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
    auto devices = GetUsbAudioDevices();
    vector<UsbAudioDevice> toAdd;
    for (auto &device : devices) {
        auto it = find_if(audioDevices_.cbegin(), audioDevices_.cend(), [&device](auto &item) {
            return device.usbAddr_ == item.usbAddr_ && device.name_ == item.name_;
        });
        if (it == audioDevices_.cend()) {
            toAdd.push_back(device);
        }
    }
    CHECK_AND_RETURN_RET(!toAdd.empty(),);
    soundCardMap_ = GetUsbSoundCardMap();
    for (auto &device : toAdd) {
        if (!FillUsbAudioDevice(device)) { continue; }
        audioDevices_.push_back(device);
        NotifyDevice(device, true);
    }
}

void AudioUsbManager::SubscribeEvent()
{
    AUDIO_INFO_LOG("Entry");
    CHECK_AND_RETURN_LOG(eventSubscriber_ == nullptr, "feventSubscriber_ already exists");
    eventSubscriber_ = SubscribeCommonEvent();
    lock_guard<mutex> lock(mutex_);
    RefreshUsbAudioDevices();
}

void AudioUsbManager::NotifyDevice(const UsbAudioDevice &device, const bool isConnected)
{
    CHECK_AND_RETURN_LOG(observer_ != nullptr, "observer_ is nullptr");
    DeviceType devType = DeviceType::DEVICE_TYPE_USB_HEADSET;
    string macAddress = GetDeviceAddr(device.cardNum_);
    AudioStreamInfo streamInfo{};
    string deviceName = device.name_ + "-" + to_string(device.cardNum_);
    if (device.isPlayer_) {
        AUDIO_INFO_LOG("Call observer_->OnDeviceStatusUpdated. devType=%{public}d, isConnected=%{public}d, "
            "macAddress=%{public}s, deviceName=%{public}s, role=%{public}d", devType, isConnected,
            EncUsbAddr(macAddress).c_str(), deviceName.c_str(), DeviceRole::OUTPUT_DEVICE);
        observer_->OnDeviceStatusUpdated(devType, isConnected, macAddress,
            deviceName, streamInfo, OUTPUT_DEVICE);
    }
    if (device.isCapturer_) {
        AUDIO_INFO_LOG("Call observer_->OnDeviceStatusUpdated. devType=%{public}d, isConnected=%{public}d, "
            "macAddress=%{public}s, deviceName=%{public}s, role=%{public}d", devType, isConnected,
            EncUsbAddr(macAddress).c_str(), deviceName.c_str(), DeviceRole::INPUT_DEVICE);
        observer_->OnDeviceStatusUpdated(devType, isConnected, macAddress,
            deviceName, streamInfo, INPUT_DEVICE);
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

vector<UsbAudioDevice> AudioUsbManager::GetUsbAudioDevices()
{
    vector<UsbDevice> deviceList;
    vector<UsbAudioDevice> result;
    auto ret = UsbSrvClient::GetInstance().GetDevices(deviceList);
    if (ret != ERR_OK) {
        AUDIO_ERR_LOG("GetDevices failed. ret=%{public}d. size=%{public}zu", ret, deviceList.size());
        return result;
    }
    for (auto &usbDevice : deviceList) {
        if (IsAudioDevice(usbDevice)) {
            result.push_back({
                {usbDevice.GetBusNum(), usbDevice.GetDevAddr()},
                usbDevice.GetProductName(),
            });
        }
    }
    return result;
}

void AudioUsbManager::EventSubscriber::OnReceiveEvent(const EventFwk::CommonEventData &data)
{
    string action = data.GetWant().GetAction();
    AUDIO_INFO_LOG("OnReceiveEvent Entry. action=%{public}s", action.c_str());
    string devStr;
    bool isAttach{false};
    if (action == EventFwk::CommonEventSupport::COMMON_EVENT_USB_DEVICE_ATTACHED) {
        devStr = data.GetData();
        isAttach = true;
    } else if (action == EventFwk::CommonEventSupport::COMMON_EVENT_USB_DEVICE_DETACHED) {
        devStr = data.GetData();
        isAttach = false;
    } else {
        return;
    }
    if (devStr.empty()) {
        AUDIO_ERR_LOG("Error: data.GetData() returns empty");
        return;
    }
    auto devJson = cJSON_Parse(devStr.c_str());
    if (devJson == nullptr) {
        cJSON_Delete(devJson);
        AUDIO_ERR_LOG("Create devJson error");
        return;
    }
    UsbDevice usbDevice(devJson);
    cJSON_Delete(devJson);
    if (!IsAudioDevice(usbDevice)) {
        return;
    }
    UsbAudioDevice device = {
        {usbDevice.GetBusNum(), usbDevice.GetDevAddr()},
        usbDevice.GetProductName()
    };
    AudioUsbManager::GetInstance().PushMessageQueue(device, isAttach);
}

void AudioUsbManager::PushMessageQueue(const UsbAudioDevice &device, const bool isAttach)
{
    AUDIO_INFO_LOG("Entry. deviceName=%{public}s, isAttach=%{public}d", device.name_.c_str(), isAttach);
    unique_lock<mutex> uLock(mqMutex_);
    messageQueue_.push_back(make_pair(device, isAttach));
    ntCondition_.notify_one();
    uLock.unlock();
}

void AudioUsbManager::NotifyDevicesLoop(list<pair<UsbAudioDevice, bool>> &origin)
{
    list<pair<UsbAudioDevice, bool>> merged;
    set<UsbAddr> toDelete;
    for (auto &qItem : origin) {
        auto it = find_if(merged.begin(), merged.end(), [&qItem](auto &item) {
            return qItem.first.usbAddr_ == item.first.usbAddr_;
        });
        if (it != merged.end()) {
            FillToDelete(qItem, it->second, toDelete);
            merged.erase(it);
        }
        merged.push_back(qItem);
    }
    if (!merged.empty()) {
        for (auto item : toDelete) {
            PreDeleteDevice(item);
        }
        lock_guard<mutex> lock(mutex_);
        if (HasDeviceAttach(merged)) {
            soundCardMap_ = GetUsbSoundCardMap();
        }
        for (auto &item : merged) {
            HandleDeviceUpdate(item);
        }
    }
}

void AudioUsbManager::FillToDelete(pair<UsbAudioDevice, bool> &curr, bool IsPrevAttach, set<UsbAddr> &toDelete)
{
    if (curr.second) {
        if (!IsPrevAttach) {
            toDelete.insert(curr.first.usbAddr_);
        }
    } else {
        toDelete.erase(curr.first.usbAddr_);
    }
}

void AudioUsbManager::PreDeleteDevice(const UsbAddr addr)
{
    auto it = find_if(audioDevices_.cbegin(), audioDevices_.cend(), [addr](auto &item) {
        return addr == item.usbAddr_;
    });
    if (it != audioDevices_.cend()) {
        NotifyDevice(*it, false);
    }
}

void AudioUsbManager::HandleDeviceUpdate(pair<UsbAudioDevice, bool> &p)
{
    if (p.second) {
        CHECK_AND_RETURN_RET(FillUsbAudioDevice(p.first),);
        auto it = find(audioDevices_.begin(), audioDevices_.end(), p.first);
        if (it == audioDevices_.end()) {
            audioDevices_.push_back(p.first);
        } else {
            *it = p.first;
        }
        NotifyDevice(p.first, true);
    } else {
        auto it = find(audioDevices_.begin(), audioDevices_.end(), p.first);
        if (it != audioDevices_.end()) {
            NotifyDevice(*it, false);
            audioDevices_.erase(it);
        }
    }
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
    device.isCapturer_ = card.isCapturer_;
    device.isPlayer_ = card.isPlayer_;
    return true;
}

} // namespace AudioStandard
} // namespace OHOS