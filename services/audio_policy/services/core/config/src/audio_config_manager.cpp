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
#define LOG_TAG "AudioConfigManager"
#endif

#include "audio_config_manager.h"

#include "audio_policy_utils.h"
#include "audio_policy_service.h"

namespace OHOS {
namespace AudioStandard {
bool AudioConfigManager::Init()
{
    std::unique_ptr<AudioPolicyParser> audioPolicyConfigParser = make_unique<AudioPolicyParser>(*this);
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

void AudioConfigManager::OnAudioPolicyXmlParsingCompleted()
{
    AudioPolicyConfigData configData = AudioPolicyConfigData::GetInstance();
    AUDIO_INFO_LOG("AdapterInfo num [%{public}zu]", configData.adapterInfoMap_.size());
    CHECK_AND_RETURN_LOG(!configData.adapterInfoMap_.empty(), "Parse audio policy xml failed, received data is empty");

    audioPolicyConfig_ = configData;
    audioPolicyConfig_.Reorganize();
}

void AudioConfigManager::GetDeviceDescriptorByDeviceType(DeviceType deviceType, AudioDeviceDescriptor &desc)
{
    auto it = audioPolicyConfig_.deviceInfoMap_.find(deviceType);
    if (it != audioPolicyConfig_.deviceInfoMap_.end()) {
        AdapterDeviceInfo deviceInfo = it->second;
        desc.deviceType_ = deviceType;
        desc.deviceRole_ = deviceInfo.role_;
        desc.deviceName_ = deviceInfo.name_;
    }
}

std::string AudioConfigManager::GetSinkPortName(DeviceType deviceType, std::string pipeName)
{
    std::string portName = PORT_NONE;
    auto deviceIt = audioPolicyConfig_.deviceInfoMap_.find(deviceType);
    CHECK_AND_RETURN_RET_LOG(deviceIt != audioPolicyConfig_.deviceInfoMap_.end(), portName, "Find deviceType failed");
    auto pipeIt = deviceIt->second.supportPipeMap_.find(pipeName);
    CHECK_AND_RETURN_RET_LOG(pipeIt != deviceIt->second.supportPipeMap_.end(), portName, "Find pipeName failed");
    portName = pipeIt->second.paProp_.moduleName_;
    return portName;
}

}
}