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
#define LOG_TAG "AudioPolicyConfigManager"
#endif

#include "audio_policy_config_manager.h"
#include "audio_policy_config_parser.h"
#include "audio_policy_utils.h"

namespace OHOS {
namespace AudioStandard {
bool AudioPolicyConfigManager::Init()
{
    std::unique_ptr<AudioPolicyConfigParser> audioPolicyConfigParser = make_unique<AudioPolicyConfigParser>(this);
    CHECK_AND_RETURN_RET_LOG(audioPolicyConfigParser != nullptr, false, "AudioPolicyConfigParser create failed");
    bool ret = audioPolicyConfigParser->LoadConfiguration();
    if (!ret) {
        AudioPolicyUtils::GetInstance().WriteServiceStartupError("Audio Policy Config Load Configuration failed");
        AUDIO_ERR_LOG("Audio Policy Config Load Configuration failed");
        return ret;
    }
    return ret;
}

void AudioPolicyConfigManager::OnAudioPolicyConfigXmlParsingCompleted()
{
    AudioPolicyConfigData &configData = AudioPolicyConfigData::GetInstance();
    std::unordered_map<AudioAdapterType, PolicyAdapterInfo> adapterInfoMap {};
    configData.GetAdapterInfoMap(adapterInfoMap);
    AUDIO_INFO_LOG("AdapterInfo num [%{public}zu]", adapterInfoMap.size());
    CHECK_AND_RETURN_LOG(!adapterInfoMap.empty(), "Parse audio policy xml failed, received data is empty");

    audioPolicyConfig_.Reorganize();
}

void AudioPolicyConfigManager::GetDeviceDescriptorByDeviceType(DeviceType deviceType, AudioDeviceDescriptor &desc)
{
    std::unordered_map<DeviceType, std::shared_ptr<AdapterDeviceInfo>> deviceInfoMap;
    audioPolicyConfig_.GetDeviceInfoMap(deviceInfoMap);
    auto it = deviceInfoMap.find(deviceType);
    if (it != deviceInfoMap.end()) {
        std::shared_ptr<AdapterDeviceInfo> deviceInfo = it->second;
        desc.deviceType_ = deviceType;
        desc.deviceRole_ = deviceInfo->role_;
        desc.deviceName_ = deviceInfo->name_;
    }
}

std::string AudioPolicyConfigManager::GetSinkPortName(DeviceType deviceType, AudioFlag flagType)
{
    std::string portName = PORT_NONE;
    std::unordered_map<DeviceType, std::shared_ptr<AdapterDeviceInfo>> deviceInfoMap;
    audioPolicyConfig_.GetDeviceInfoMap(deviceInfoMap);
    auto deviceIt = deviceInfoMap.find(deviceType);
    CHECK_AND_RETURN_RET_LOG(deviceIt != deviceInfoMap.end(), portName, "Find deviceType failed");
    auto pipeIt = deviceIt->second->supportPipeMap_.find(flagType);
    CHECK_AND_RETURN_RET_LOG(pipeIt != deviceIt->second->supportPipeMap_.end(), portName, "Find flagType failed");
    portName = pipeIt->second->paProp_.moduleName_;
    return portName;
}

AudioFlag AudioPolicyConfigManager::GetRouteFlag(std::shared_ptr<AudioStreamDescriptor> desc)
{
    // device -> adapter -> flag -> stream
    AudioFlag flag = AUDIO_OUTPUT_FLAG_NONE; // input or output? default?
    std::unordered_map<DeviceType, std::shared_ptr<AdapterDeviceInfo>> deviceInfoMap;
    audioPolicyConfig_.GetDeviceInfoMap(deviceInfoMap);
    auto deviceIt = deviceInfoMap.find(desc->newDeviceDescs_.back()->deviceType_);
    CHECK_AND_RETURN_RET_LOG(deviceIt != deviceInfoMap.end(), flag, "Find deviceType failed");

    for (auto pipeIt : deviceIt->second->supportPipeMap_) {
        if (desc->audioFlag_ & pipeIt.first) {
            flag = pipeIt.first;
            break;
        }
    }
    if (flag == AUDIO_OUTPUT_FLAG_NONE) {
        AUDIO_INFO_LOG("Find audioFlag failed, choose normal flag");
        flag = desc->audioMode_ == AUDIO_MODE_PLAYBACK ?
            AUDIO_OUTPUT_FLAG_NORMAL : AUDIO_INPUT_FLAG_NORMAL;
    }
    return flag;
}

void AudioPolicyConfigManager::GetStreamPropInfo(std::shared_ptr<AudioStreamDescriptor> &desc, PipeStreamPropInfo &info)
{
    std::unordered_map<DeviceType, std::shared_ptr<AdapterDeviceInfo>> deviceInfoMap;
    audioPolicyConfig_.GetDeviceInfoMap(deviceInfoMap);
    auto deviceIt = deviceInfoMap.find(desc->newDeviceDescs_.back()->deviceType_);
    CHECK_AND_RETURN_LOG(deviceIt != deviceInfoMap.end(), "Find deviceType failed");
    auto pipeIt = deviceIt->second->supportPipeMap_.find(desc->routeFlag_);
    CHECK_AND_RETURN_LOG(pipeIt != deviceIt->second->supportPipeMap_.end(), "Find routeFlag failed");

    for (auto &streamProp : pipeIt->second->streamPropInfos_) {
        if (streamProp.format_ == desc->streamInfo_.format &&
            streamProp.sampleRate_ == desc->streamInfo_.samplingRate &&
            streamProp.channelLayout_ == desc->streamInfo_.channelLayout) {
            info = streamProp;
            return;
        }
    }
    if (info.format_ == INVALID_WIDTH && info.sampleRate_ == 0 && info.channelLayout_ == CH_LAYOUT_UNKNOWN &&
        desc->routeFlag_ != (AUDIO_OUTPUT_FLAG_NORMAL || AUDIO_INPUT_FLAG_NORMAL)) {
        AUDIO_INFO_LOG("Find streamPropInfo failed, choose normal flag");
        desc->routeFlag_ = desc->audioMode_ == AUDIO_MODE_PLAYBACK ?
            AUDIO_OUTPUT_FLAG_NORMAL : AUDIO_INPUT_FLAG_NORMAL;
        auto pipeIt = deviceIt->second->supportPipeMap_.find(desc->routeFlag_);
        for (auto &streamProp : pipeIt->second->streamPropInfos_) {
            if (streamProp.format_ == desc->streamInfo_.format &&
                streamProp.sampleRate_ == desc->streamInfo_.samplingRate &&
                streamProp.channelLayout_ == desc->streamInfo_.channelLayout) {
                info = streamProp;
                return;
            }
        }
    }
    if (info.format_ == INVALID_WIDTH && info.sampleRate_ == 0 && info.channelLayout_ == CH_LAYOUT_UNKNOWN &&
        desc->routeFlag_ == (AUDIO_OUTPUT_FLAG_NORMAL || AUDIO_INPUT_FLAG_NORMAL) &&
        !pipeIt->second->streamPropInfos_.empty()) {
        info = pipeIt->second->streamPropInfos_.front(); // if not match, choose first?
    }
}

}
}
