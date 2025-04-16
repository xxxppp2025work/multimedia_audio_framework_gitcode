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

#ifndef REMOTE_DEVICE_MANAGER_H
#define REMOTE_DEVICE_MANAGER_H

#include <iostream>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <v1_0/iaudio_manager.h>
#include <v1_0/iaudio_callback.h>
#include <v1_0/audio_types.h>
#include "audio_info.h"
#include "adapter/i_device_manager.h"
#include "adapter/i_device_adapter.h"

namespace OHOS {
namespace AudioStandard {
typedef OHOS::HDI::DistributedAudio::Audio::V1_0::IAudioAdapter RemoteIAudioAdapter;
typedef struct OHOS::HDI::DistributedAudio::Audio::V1_0::AudioAdapterDescriptor RemoteAudioAdapterDescriptor;
typedef OHOS::HDI::DistributedAudio::Audio::V1_0::IAudioCallback RemoteIAudioCallback;
typedef OHOS::HDI::DistributedAudio::Audio::V1_0::AudioCallbackType RemoteAudioCallbackType;
typedef OHOS::HDI::DistributedAudio::Audio::V1_0::AudioExtParamKey RemoteAudioExtParamKey;
typedef enum OHOS::HDI::DistributedAudio::Audio::V1_0::AudioPortDirection RemoteAudioPortDirection;
typedef OHOS::HDI::DistributedAudio::Audio::V1_0::IAudioManager RemoteIAudioManager;
typedef OHOS::HDI::DistributedAudio::Audio::V1_0::AudioRouteNode RemoteAudioRouteNode;

class RemoteDeviceManager : public IDeviceManager {
public:
    RemoteDeviceManager() = default;
    ~RemoteDeviceManager() = default;

    std::shared_ptr<IDeviceAdapter> LoadAdapter(const std::string &adapterName, bool needReInitManager) override;
    void UnloadAdapter(std::shared_ptr<IDeviceAdapter> deviceAdapter) override;

private:
    void InitAudioManager(void);
    int32_t SwitchAdapterDesc(const std::vector<RemoteAudioAdapterDescriptor> &descs, const std::string &adapterName);

private:
    static constexpr uint32_t MAX_AUDIO_ADAPTER_NUM = 5;
    sptr<RemoteIAudioManager> audioManager_ = nullptr;
};

} // namespace AudioStandard
} // namespace OHOS

#endif // REMOTE_DEVICE_MANAGER_H
