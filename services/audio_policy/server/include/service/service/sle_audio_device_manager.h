/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#ifndef SLE_AUDIO_DEVICE_MANAGER_H
#define SLE_AUDIO_DEVICE_MANAGER_H

#include "audio_general_manager.h"
#include "i_standard_sle_audio_operation_callback.h"

namespace OHOS {
namespace AudioStandard {
struct SleVolumeConfigInfo {
    AudioVolumeType volumeType = STREAM_DEFAULT;
    int32_t volumeLevel = -1;
    bool isMute = false;

    SleVolumeConfigInfo() = default;
    SleVolumeConfigInfo(AudioVolumeType volumeType) : volumeType(volumeType) {}
};

class SleAudioDeviceManager : public SleAudioOperationCallback {
public:
    static SleAudioDeviceManager &GetInstance()
    {
        static SleAudioDeviceManager instance;
        return instance;
    }

    int32_t SetSleAudioOperationCallback(const sptr<IStandardSleAudioOperationCallback> &callback);

    // Callback Interface Implementations
    void GetSleAudioDeviceList(std::vector<AudioDeviceDescriptor> &devices) override;
    void GetSleVirtualAudioDeviceList(std::vector<AudioDeviceDescriptor> &devices) override;
    bool IsInBandRingOpen(const std::string &device) const override;
    uint32_t GetSupportStreamType(const std::string &device) const override;
    int32_t SetActiveSinkDevice(const std::string &device, uint32_t streamType) override;
    int32_t StartPlaying(const std::string &device, uint32_t streamType) override;
    int32_t StopPlaying(const std::string &device, uint32_t streamType) override;
    int32_t ConnectAllowedProfiles(const std::string &remoteAddr) const override;
    int32_t SetDeviceAbsVolume(const std::string &remoteAddr, uint32_t volume, uint32_t streamType) override;
    int32_t SendUserSelection(const std::string &device, uint32_t streamType) override;

    // Parameter Conversion Interface
    int32_t SetActiveDevice(const std::string &device, StreamUsage streamUsage);
    int32_t SetActiveDevice(const std::string &device, SourceType sourceType);
    int32_t StartPlaying(const AudioDeviceDescriptor &deviceDesc, StreamUsage streamUsage);
    int32_t StopPlaying(const AudioDeviceDescriptor &deviceDesc, StreamUsage streamUsage);
    int32_t StartPlaying(const AudioDeviceDescriptor &deviceDesc, SourceType sourceType);
    int32_t StopPlaying(const AudioDeviceDescriptor &deviceDesc, SourceType sourceType);
    int32_t SendUserSelection(const AudioDeviceDescriptor &deviceDesc, StreamUsage streamUsage);
    int32_t SendUserSelection(const AudioDeviceDescriptor &deviceDesc, SourceType sourceType);
    int32_t SetDeviceAbsVolume(const std::string &device, AudioStreamType streamType, int32_t volume);

    // Core Device Management Methods
    int32_t AddNearlinkDevice(const AudioDeviceDescriptor &deviceDesc);
    int32_t RemoveNearlinkDevice(const AudioDeviceDescriptor &deviceDesc);
    int32_t SetNearlinkDeviceMute(const std::string &device, AudioStreamType streamType, bool isMute);
    int32_t SetNearlinkDeviceVolumeLevel(const std::string &device, AudioStreamType streamType,
        const int32_t volumeLevel);

private:
    SleAudioDeviceManager() = default;
    virtual ~SleAudioDeviceManager() = default;

    uint32_t GetSleStreamTypeByStreamUsage(StreamUsage streamUsage) const;
    uint32_t GetSleStreamTypeBySourceType(SourceType sourceType) const;

    sptr<IStandardSleAudioOperationCallback> callback_ = nullptr;

    std::mutex deviceVolumeConfigMutex_;
    std::unordered_map<std::string, std::pair<SleVolumeConfigInfo, SleVolumeConfigInfo>> deviceVolumeConfigInfo_;
};
} // namespace AudioStandard
} // namespace OHOS
#endif // SLE_AUDIO_DEVICE_MANAGER_H
