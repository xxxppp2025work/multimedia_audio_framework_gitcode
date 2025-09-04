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
#ifndef AUDIO_USB_MANAGER_H
#define AUDIO_USB_MANAGER_H

#include <mutex>
#include <map>
#include <vector>
#include "common_event_subscriber.h"
#include "idevice_status_observer.h"

namespace OHOS {
namespace AudioStandard {

using namespace std;

string EncUsbAddr(const string &src);

struct UsbAddr {
    uint8_t busNum_{0};
    uint8_t devAddr_{0};
    inline bool operator==(const UsbAddr &o) const
    {
        return busNum_ == o.busNum_ && devAddr_ == o.devAddr_;
    }
    inline bool operator<(const UsbAddr &o) const
    {
        return busNum_ < o.busNum_ || (busNum_ == o.busNum_ && devAddr_ < o.devAddr_);
    }
};

struct SoundCard {
    uint32_t cardNum_{0};
    string usbBus_;
    bool isCapturer_{false};
    bool isPlayer_{false};
};

struct UsbAudioDevice {
    UsbAddr usbAddr_;
    string name_;
    uint32_t cardNum_{0};
    DeviceType devType_{DEVICE_TYPE_INVALID};
    bool isCapturer_{false};
    bool isPlayer_{false};
    inline bool operator==(const UsbAudioDevice &o) const
    {
        return usbAddr_ == o.usbAddr_;
    }
};

typedef int8_t InitCtrl;
constexpr InitCtrl INIT_CTRL_INVALID{0};
constexpr InitCtrl INIT_CTRL_USB{1};
constexpr InitCtrl INIT_CTRL_CES{2};
constexpr InitCtrl INIT_CTRL_ALL{INIT_CTRL_USB | INIT_CTRL_CES};

class AudioUsbManager {
public:
    class EventSubscriber : public EventFwk::CommonEventSubscriber {
    public:
        explicit EventSubscriber(const EventFwk::CommonEventSubscribeInfo &subscribeInfo)
            : EventFwk::CommonEventSubscriber(subscribeInfo) {}
        void OnReceiveEvent(const EventFwk::CommonEventData &data) override;
    };

    static AudioUsbManager &GetInstance();
    static map<UsbAddr, SoundCard> GetUsbSoundCardMap();
    static int32_t GetUsbAudioDevices(vector<UsbAudioDevice> &result);

    void Init(InitCtrl initCtrl, shared_ptr<IDeviceStatusObserver> observer);
    void Deinit();

private:
    AudioUsbManager() = default;
    void RefreshUsbAudioDevices();
    void NotifyDevice(const UsbAudioDevice &device, const bool isConnected);
    void HandleAudioDeviceEvent(pair<UsbAudioDevice, bool> &&p);
    bool FillUsbAudioDevice(UsbAudioDevice &device);
    // must be called in mutex_ lock
    void UpdateDevice(const UsbAudioDevice &dev);
    void HandleDeviceAttach(vector<UsbAudioDevice> &&devices);
    void HandleDeviceAttachAsync(vector<UsbAudioDevice> &&devices);
    DeviceType DetectAudioDeviceType(uint32_t cardNum);

    std::shared_ptr<IDeviceStatusObserver> observer_{nullptr};
    std::shared_ptr<EventSubscriber> eventSubscriber_{nullptr};
    vector<UsbAudioDevice> audioDevices_;
    map<UsbAddr, SoundCard> soundCardMap_;
    
    bool initialized_{false};
    InitCtrl initCtrl_{INIT_CTRL_INVALID};
    mutex mutex_;
};

} // namespace AudioStandard
} // namespace OHOS

#endif