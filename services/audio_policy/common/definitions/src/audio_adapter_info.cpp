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
#ifndef LOG_TAG
#define LOG_TAG "AudioAdapterInfo"
#endif

#include "audio_adapter_info.h"
#include "audio_effect.h"

namespace OHOS {
namespace AudioStandard {
void AudioPolicyConfigData::SetDeviceMaps(std::list<AdapterDeviceInfo> &deviceInfos)
{
    for (AdapterDeviceInfo &deviceInfo : deviceInfos) {
        deviceInfoMap_.insert({deviceInfo.type_, deviceInfo});

        if (deviceInfo.role_ == OUTPUT_DEVICE) {
            outputDeviceMap_.insert({deviceInfo.type_, deviceInfo});
        } else if (deviceInfo.role_ == INPUT_DEVICE) {
            inputDeviceMap_.insert({deviceInfo.type_, deviceInfo});
        }
    }
}

void AudioPolicyConfigData::SetPipeMaps(std::list<PipeInfo> &pipeInfos)
{
    for (PipeInfo &pipeInfo : pipeInfos) {
        pipeInfoMap_.insert({pipeInfo.name_, pipeInfo});

        if (pipeInfo.pipeRole_ == PIPE_ROLE_OUT) {
            outputPipeMap_.insert({pipeInfo.name_, pipeInfo});
        } else if (pipeInfo.pipeRole_ == PIPE_ROLE_IN) {
            inputPipeMap_.insert({pipeInfo.name_, pipeInfo});
        }
    }
}

void AudioPolicyConfigData::SetSupportDeviceAndPipeMaps(PipeInfo &pipeInfo)
{
    for (StreamPropInfo &streamPropInfo : pipeInfo.streamPropInfos_) {
        for (DeviceType &supportDevice : streamPropInfo.supportDevices_) {
            AdapterDeviceInfo &deviceInfo = deviceInfoMap_.find(supportDevice)->second;

            pipeInfo.supportDeviceMap_.insert({supportDevice, deviceInfo});
            streamPropInfo.supportDeviceMap_.insert({supportDevice, deviceInfo});
            deviceInfo.supportPipeMap_.insert({pipeInfo.name_, pipeInfo});
        }
    }
}

void AudioPolicyConfigData::Reorganize()
{
    for (auto &pair : adapterInfoMap_) {
        SetDeviceMaps(pair.second.deviceInfos_);
        SetPipeMaps(pair.second.pipeInfos_);
    }

    for (auto &pair : pipeInfoMap_) {
        SetSupportDeviceAndPipeMaps(pair.second);
    }
}

AdapterType AudioAdapterInfo::GetTypeEnum()
{
    return GetAdapterType(adapterName_);
}

AdapterType AudioAdapterInfo::GetAdapterType(const std::string &adapterName)
{
    if (adapterName == ADAPTER_TYPE_PRIMARY) {
        return AdapterType::TYPE_PRIMARY;
    } else if (adapterName == ADAPTER_TYPE_A2DP) {
        return AdapterType::TYPE_A2DP;
    } else if (adapterName == ADAPTER_TYPE_REMOTE) {
        return AdapterType::TYPE_REMOTE_AUDIO;
    } else if (adapterName == ADAPTER_TYPE_FILE) {
        return AdapterType::TYPE_FILE_IO;
    } else if (adapterName == ADAPTER_TYPE_USB) {
        return AdapterType::TYPE_USB;
    } else if (adapterName == ADAPTER_TYPE_DP) {
        return AdapterType::TYPE_DP;
    } else if (adapterName == ADAPTER_TYPE_SLE) {
        return AdapterType::TYPE_SLE;
    } else {
        return AdapterType::TYPE_INVALID;
    }
}

PipeInfo* AudioAdapterInfo::GetPipeInfoByName(const std::string &pipeName)
{
    for (auto &pipeInfo : pipeInfos_) {
        if (pipeInfo.name_ == pipeName) {
            return &pipeInfo;
        }
    }
    return nullptr;
}

AdapterDeviceInfo* AudioAdapterInfo::GetDeviceInfoByType(DeviceType deviceType)
{
    for (auto &deviceInfo : deviceInfos_) {
        auto device = SUPPORTED_DEVICE_TYPE.find(deviceType);
        if (device != SUPPORTED_DEVICE_TYPE.end()) {
            return &deviceInfo;
        }
    }
    return nullptr;
}
}
}