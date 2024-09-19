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

#ifndef AUDIO_DEVICE_DESCRIPTOR_H
#define AUDIO_DEVICE_DESCRIPTOR_H

#include <memory>

#include "audio_device_info.h"
#include "audio_info.h"

namespace OHOS {
namespace AudioStandard {
/**
 * @brief The AudioDeviceDescriptor provides
 *         different sets of audio devices and their roles
 */
class AudioDeviceDescriptor : public Parcelable {
public:
    enum {
        AUDIO_DEVICE_DESCRIPTOR,
        DEVICE_INFO,
    };
    int32_t descriptorType_ = AUDIO_DEVICE_DESCRIPTOR;

    bool exceptionFlag_ = false;
    bool isScoRealConnected_ = false;
    bool isEnable_ = true;
    ConnectState connectState_ = CONNECTED;
    DeviceCategory deviceCategory_ = CATEGORY_DEFAULT;
    DeviceType deviceType_ = DEVICE_TYPE_NONE;
    DeviceRole deviceRole_ = DEVICE_ROLE_NONE;
    int32_t deviceId_ = 0;
    int32_t channelMasks_ = 0;
    int32_t channelIndexMasks_ = 0;
    int32_t interruptGroupId_ = 0;
    int32_t volumeGroupId_ = 0;
    int64_t connectTimeStamp_ = 0;
    std::string deviceName_;
    std::string macAddress_;
    std::string networkId_;
    std::string displayName_;
    DeviceStreamInfo audioStreamInfo_ = {};
    std::shared_ptr<AudioDeviceDescriptor> pairDeviceDescriptor_;
    // DeviceInfo
    bool isLowLatencyDevice_ = false;
    int32_t a2dpOffloadFlag_ = 0;

    AudioDeviceDescriptor(int32_t descriptorType = AUDIO_DEVICE_DESCRIPTOR)
        : AudioDeviceDescriptor(DeviceType::DEVICE_TYPE_NONE, DeviceRole::DEVICE_ROLE_NONE)
    {
        descriptorType_ = descriptorType;
    }

    AudioDeviceDescriptor(DeviceType type, DeviceRole role)
        : deviceType_(type), deviceRole_(role)
    {
        exceptionFlag_ = false;
        isScoRealConnected_ = false;
        isEnable_ = true;
        connectState_ = CONNECTED;
        deviceCategory_ = CATEGORY_DEFAULT;
        deviceId_ = 0;
        channelMasks_ = 0;
        channelIndexMasks_ = 0;
        interruptGroupId_ = 0;
        volumeGroupId_ = 0;
        connectTimeStamp_ = 0;
        deviceName_ = "";
        macAddress_ = "";
        networkId_ = LOCAL_NETWORK_ID;
        displayName_ = "";
        audioStreamInfo_ = {};
        pairDeviceDescriptor_ = nullptr;
        descriptorType_ = AUDIO_DEVICE_DESCRIPTOR;
        isLowLatencyDevice_ = false;
        a2dpOffloadFlag_ = 0;
    }

    AudioDeviceDescriptor(DeviceType type, DeviceRole role, int32_t interruptGroupId,
        int32_t volumeGroupId, std::string networkId)
        : deviceType_(type), deviceRole_(role), interruptGroupId_(interruptGroupId), volumeGroupId_(volumeGroupId),
        networkId_(networkId)
    {
        exceptionFlag_ = false;
        isScoRealConnected_ = false;
        isEnable_ = true;
        connectState_ = CONNECTED;
        deviceCategory_ = CATEGORY_DEFAULT;
        deviceId_ = 0;
        channelMasks_ = 0;
        channelIndexMasks_ = 0;
        connectTimeStamp_ = 0;
        deviceName_ = "";
        macAddress_ = "";
        displayName_ = "";
        audioStreamInfo_ = {};
        pairDeviceDescriptor_ = nullptr;
        descriptorType_ = AUDIO_DEVICE_DESCRIPTOR;
        isLowLatencyDevice_ = false;
        a2dpOffloadFlag_ = 0;
    }

    AudioDeviceDescriptor(const AudioDeviceDescriptor &deviceDescriptor)
    {
        descriptorType_ = deviceDescriptor.descriptorType_;
        exceptionFlag_ = deviceDescriptor.exceptionFlag_;
        isScoRealConnected_ = deviceDescriptor.isScoRealConnected_;
        isEnable_ = deviceDescriptor.isEnable_;
        connectState_ = deviceDescriptor.connectState_;
        deviceCategory_ = deviceDescriptor.deviceCategory_;
        deviceType_ = deviceDescriptor.deviceType_;
        deviceRole_ = deviceDescriptor.deviceRole_;
        deviceId_ = deviceDescriptor.deviceId_;
        channelMasks_ = deviceDescriptor.channelMasks_;
        channelIndexMasks_ = deviceDescriptor.channelIndexMasks_;
        interruptGroupId_ = deviceDescriptor.interruptGroupId_;
        volumeGroupId_ = deviceDescriptor.volumeGroupId_;
        connectTimeStamp_ = deviceDescriptor.connectTimeStamp_;
        deviceName_ = deviceDescriptor.deviceName_;
        macAddress_ = deviceDescriptor.macAddress_;
        networkId_ = deviceDescriptor.networkId_;
        displayName_ = deviceDescriptor.displayName_;
        audioStreamInfo_.channels = deviceDescriptor.audioStreamInfo_.channels;
        audioStreamInfo_.encoding = deviceDescriptor.audioStreamInfo_.encoding;
        audioStreamInfo_.format = deviceDescriptor.audioStreamInfo_.format;
        audioStreamInfo_.samplingRate = deviceDescriptor.audioStreamInfo_.samplingRate;
        pairDeviceDescriptor_ = deviceDescriptor.pairDeviceDescriptor_;
        // DeviceInfo
        isLowLatencyDevice_ = deviceDescriptor.isLowLatencyDevice_;
        a2dpOffloadFlag_ = deviceDescriptor.a2dpOffloadFlag_;
    }

    AudioDeviceDescriptor(const sptr<AudioDeviceDescriptor> &deviceDescriptor)
    {
        if (deviceDescriptor == nullptr) {
            return;
        }
        descriptorType_ = deviceDescriptor->descriptorType_;
        exceptionFlag_ = deviceDescriptor->exceptionFlag_;
        isScoRealConnected_ = deviceDescriptor->isScoRealConnected_;
        isEnable_ = deviceDescriptor->isEnable_;
        connectState_ = deviceDescriptor->connectState_;
        deviceCategory_ = deviceDescriptor->deviceCategory_;
        deviceType_ = deviceDescriptor->deviceType_;
        deviceRole_ = deviceDescriptor->deviceRole_;
        deviceId_ = deviceDescriptor->deviceId_;
        channelMasks_ = deviceDescriptor->channelMasks_;
        channelIndexMasks_ = deviceDescriptor->channelIndexMasks_;
        interruptGroupId_ = deviceDescriptor->interruptGroupId_;
        volumeGroupId_ = deviceDescriptor->volumeGroupId_;
        connectTimeStamp_ = deviceDescriptor->connectTimeStamp_;
        deviceName_ = deviceDescriptor->deviceName_;
        macAddress_ = deviceDescriptor->macAddress_;
        networkId_ = deviceDescriptor->networkId_;
        displayName_ = deviceDescriptor->displayName_;
        audioStreamInfo_.channels = deviceDescriptor->audioStreamInfo_.channels;
        audioStreamInfo_.encoding = deviceDescriptor->audioStreamInfo_.encoding;
        audioStreamInfo_.format = deviceDescriptor->audioStreamInfo_.format;
        audioStreamInfo_.samplingRate = deviceDescriptor->audioStreamInfo_.samplingRate;
        pairDeviceDescriptor_ = deviceDescriptor->pairDeviceDescriptor_;
        // DeviceInfo
        isLowLatencyDevice_ = deviceDescriptor->isLowLatencyDevice_;
        a2dpOffloadFlag_ = deviceDescriptor->a2dpOffloadFlag_;
    }

    virtual ~AudioDeviceDescriptor()
    {
        pairDeviceDescriptor_ = nullptr;
    }

    DeviceType getType()
    {
        return deviceType_;
    }

    DeviceRole getRole() const
    {
        return deviceRole_;
    }

    bool isAudioDeviceDescriptor() const
    {
        return descriptorType_ == AUDIO_DEVICE_DESCRIPTOR;
    }

    bool Marshalling(Parcel &parcel) const override
    {
        DeviceType deviceType = deviceType_;
        if (descriptorType_ == AUDIO_DEVICE_DESCRIPTOR && deviceType_ == DEVICE_TYPE_USB_ARM_HEADSET) {
            deviceType = DEVICE_TYPE_USB_HEADSET;
        }
        return parcel.WriteInt32(static_cast<int32_t>(deviceCategory_))
            && parcel.WriteInt32(static_cast<int32_t>(deviceType))
            && parcel.WriteInt32(static_cast<int32_t>(deviceRole_))
            && parcel.WriteInt32(deviceId_)
            && parcel.WriteInt32(channelMasks_)
            && parcel.WriteInt32(channelIndexMasks_)
            && parcel.WriteInt32(interruptGroupId_)
            && parcel.WriteInt32(volumeGroupId_)
            && parcel.WriteString(deviceName_)
            && parcel.WriteString(macAddress_)
            && parcel.WriteString(networkId_)
            && parcel.WriteString(displayName_)
            && audioStreamInfo_.Marshalling(parcel)
            && parcel.WriteBool(isLowLatencyDevice_)
            && parcel.WriteInt32(a2dpOffloadFlag_);
    }

    bool Marshalling(Parcel &parcel, bool hasBTPermission, bool hasSystemPermission, int32_t apiVersion) const
    {
        DeviceType devType = deviceType_;
        int32_t devId = deviceId_;
        DeviceStreamInfo streamInfo = audioStreamInfo_;

        // If api target version < 11 && does not set deviceType, fix api compatibility.
        if (apiVersion < API_11 && (deviceType_ == DEVICE_TYPE_NONE || deviceType_ == DEVICE_TYPE_INVALID)) {
            // DeviceType use speaker or mic instead.
            if (deviceRole_ == OUTPUT_DEVICE) {
                devType = DEVICE_TYPE_SPEAKER;
                devId = 1; // 1 default speaker device id.
            } else if (deviceRole_ == INPUT_DEVICE) {
                devType = DEVICE_TYPE_MIC;
                devId = 2; // 2 default mic device id.
            }

            //If does not set sampleRates use SAMPLE_RATE_44100 instead.
            if (streamInfo.samplingRate.empty()) {
                streamInfo.samplingRate.insert(SAMPLE_RATE_44100);
            }
            // If does not set channelCounts use STEREO instead.
            if (streamInfo.channels.empty()) {
                streamInfo.channels.insert(STEREO);
            }
        }

        return parcel.WriteInt32(static_cast<int32_t>(deviceCategory_))
            && parcel.WriteInt32(static_cast<int32_t>(devType))
            && parcel.WriteInt32(static_cast<int32_t>(deviceRole_))
            && parcel.WriteInt32(devId)
            && parcel.WriteInt32(channelMasks_)
            && parcel.WriteInt32(channelIndexMasks_)
            && parcel.WriteInt32(hasSystemPermission ? interruptGroupId_ : INVALID_GROUP_ID)
            && parcel.WriteInt32(hasSystemPermission ? volumeGroupId_ : INVALID_GROUP_ID)
            && parcel.WriteString((!hasBTPermission && (deviceType_ == DEVICE_TYPE_BLUETOOTH_A2DP
                || deviceType_ == DEVICE_TYPE_BLUETOOTH_SCO)) ? "" : deviceName_)
            && parcel.WriteString((!hasBTPermission && (deviceType_ == DEVICE_TYPE_BLUETOOTH_A2DP
                || deviceType_ == DEVICE_TYPE_BLUETOOTH_SCO)) ? "" : macAddress_)
            && parcel.WriteString(hasSystemPermission ? networkId_ : "")
            && parcel.WriteString(displayName_)
            && audioStreamInfo_.Marshalling(parcel)
            && parcel.WriteBool(isLowLatencyDevice_)
            && parcel.WriteInt32(a2dpOffloadFlag_);
    }

    void Unmarshalling(Parcel &parcel)
    {
        deviceCategory_ = static_cast<DeviceCategory>(parcel.ReadInt32());
        deviceType_ = static_cast<DeviceType>(parcel.ReadInt32());
        deviceRole_ = static_cast<DeviceRole>(parcel.ReadInt32());
        deviceId_ = parcel.ReadInt32();
        channelMasks_ = parcel.ReadInt32();
        channelIndexMasks_ = parcel.ReadInt32();
        interruptGroupId_ = parcel.ReadInt32();
        volumeGroupId_ = parcel.ReadInt32();
        deviceName_ = parcel.ReadString();
        macAddress_ = parcel.ReadString();
        networkId_ = parcel.ReadString();
        displayName_ = parcel.ReadString();
        audioStreamInfo_.Unmarshalling(parcel);
        isLowLatencyDevice_ = parcel.ReadBool();
        a2dpOffloadFlag_ = parcel.ReadInt32();
    }

    static sptr<AudioDeviceDescriptor> UnmarshallingToPtr(Parcel &parcel)
    {
        sptr<AudioDeviceDescriptor> audioDeviceDescriptor = new(std::nothrow) AudioDeviceDescriptor();
        if (audioDeviceDescriptor == nullptr) {
            return nullptr;
        }

        audioDeviceDescriptor->Unmarshalling(parcel);
        return audioDeviceDescriptor;
    }

    void SetDeviceInfo(std::string deviceName, std::string macAddress)
    {
        deviceName_ = deviceName;
        macAddress_ = macAddress;
    }

    void SetDeviceCapability(const DeviceStreamInfo &audioStreamInfo, int32_t channelMask,
        int32_t channelIndexMasks = 0)
    {
        audioStreamInfo_.channels = audioStreamInfo.channels;
        audioStreamInfo_.encoding = audioStreamInfo.encoding;
        audioStreamInfo_.format = audioStreamInfo.format;
        audioStreamInfo_.samplingRate = audioStreamInfo.samplingRate;
        channelMasks_ = channelMask;
        channelIndexMasks_ = channelIndexMasks;
    }

    bool isSameDevice(const AudioDeviceDescriptor &deviceDescriptor)
    {
        return deviceDescriptor.deviceType_ == deviceType_ &&
            deviceDescriptor.macAddress_ == macAddress_ &&
            deviceDescriptor.networkId_ == networkId_;
    }

    bool isSameDeviceDesc(const std::unique_ptr<AudioDeviceDescriptor> &deviceDescriptor)
    {
        if (deviceDescriptor == nullptr) {
            return false;
        }
        return isSameDevice(*deviceDescriptor);
    }
};
} // namespace AudioStandard
} // namespace OHOS
#endif // AUDIO_DEVICE_DESCRIPTOR_H