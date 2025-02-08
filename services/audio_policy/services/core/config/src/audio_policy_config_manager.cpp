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
    bool ret = audioPolicyConfigParser->LoadConfiguration();
    if (!ret) {
        AudioPolicyUtils::GetInstance().WriteServiceStartupError("Audio Policy Config Load Configuration failed");
        AUDIO_ERR_LOG("Audio Policy Config Load Configuration failed");
        return ret;
    }
    ret = audioPolicyConfigParser->Parse();
    if (!ret) {
        AudioPolicyUtils::GetInstance().WriteServiceStartupError("Audio Config Parse failed");
        AUDIO_ERR_LOG("Audio Policy Config Parse Configuration failed");
    }
    return ret;
}

void AudioPolicyConfigManager::OnAudioPolicyConfigXmlParsingCompleted()
{
    AudioPolicyConfigData configData = AudioPolicyConfigData::GetInstance();
    std::unordered_map<AudioAdapterType, PolicyAdapterInfo> adapterInfoMap {};
    configData.GetAdapterInfoMap(adapterInfoMap);
    AUDIO_INFO_LOG("AdapterInfo num [%{public}zu]", adapterInfoMap.size());
    CHECK_AND_RETURN_LOG(!adapterInfoMap.empty(), "Parse audio policy xml failed, received data is empty");

    audioPolicyConfig_ = configData;
    audioPolicyConfig_.Reorganize();
}

void AudioPolicyConfigManager::GetDeviceDescriptorByDeviceType(DeviceType deviceType, AudioDeviceDescriptor &desc)
{
    auto it = audioPolicyConfig_.deviceInfoMap_.find(deviceType);
    if (it != audioPolicyConfig_.deviceInfoMap_.end()) {
        AdapterDeviceInfo deviceInfo = it->second;
        desc.deviceType_ = deviceType;
        desc.deviceRole_ = deviceInfo.role_;
        desc.deviceName_ = deviceInfo.name_;
    }
}

std::string AudioPolicyConfigManager::GetSinkPortName(DeviceType deviceType, std::string pipeName)
{
    std::string portName = PORT_NONE;
    auto deviceIt = audioPolicyConfig_.deviceInfoMap_.find(deviceType);
    CHECK_AND_RETURN_RET_LOG(deviceIt != audioPolicyConfig_.deviceInfoMap_.end(), portName, "Find deviceType failed");
    auto pipeIt = deviceIt->second.supportPipeMap_.find(pipeName);
    CHECK_AND_RETURN_RET_LOG(pipeIt != deviceIt->second.supportPipeMap_.end(), portName, "Find pipeName failed");
    portName = pipeIt->second.paProp_.moduleName_;
    return portName;
}

void AudioPolicyConfigManager::GetStreamPropInfo(std::shared_ptr<AudioStreamDescriptor> desc, PipeStreamPropInfo &info)
{
    // device -> adapter -> flag -> stream
    auto deviceIt = audioPolicyConfig_.deviceInfoMap_.find(desc->deviceDesc_->deviceType_);
    CHECK_AND_RETURN_LOG(deviceIt != audioPolicyConfig_.deviceInfoMap_.end(), "Find deviceType failed");
    auto pipeIt = deviceIt->second.supportPipeMap_.find(desc->audioFlag); // audioFlag? two enum definitions
    if (pipeIt == deviceIt->second.supportPipeMap_.end()) {
        AUDIO_ERR_LOG("Find audioFlag failed");
        AudioFlagType flag = desc->audioMode_ == AUDIO_MODE_PLAYBACK ?
            FLAG_TYPE_OUTPUT_NORMAL : FLAG_TYPE_INPUT_NORMAL;
        pipeIt = deviceIt->second.supportPipeMap_.find(flag);
        CHECK_AND_RETURN_LOG(pipeIt != deviceIt->second.supportPipeMap_.end(), "Find normal flag failed");
    }
    for (auto &streamProp : pipeIt->second.streamPropInfos_) {
        if (streamProp.format_ == desc->audioStreamParams.format &&
            streamProp.sampleRate_ == desc->audioStreamParams.samplingRate &&
            streamProp.channelLayout_ == desc->audioStreamParams.channelLayout) {
            info = streamProp;
            return;
        }
    }
    if (streamProp.format_ == INVALID_WIDTH && streamProp.sampleRate_ == 0 &&
        streamProp.channelLayout_ == CH_LAYOUT_UNKNOWN) {
        AUDIO_ERR_LOG("Find streamPropInfo failed");
    }
}

}
}
