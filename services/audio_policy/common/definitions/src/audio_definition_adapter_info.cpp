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
#define LOG_TAG "AudioDefinitionAdapterInfo"
#endif

#include "audio_definition_adapter_info.h"
#include "audio_effect.h"

namespace OHOS {
namespace AudioStandard {
AudioPolicyConfigData& AudioPolicyConfigData::GetInstance()
{
    static AudioPolicyConfigData instance;
    return instance;
}

void AudioPolicyConfigData::SetDeviceMaps(std::list<AdapterDeviceInfo> &deviceInfos)
{
    for (AdapterDeviceInfo &deviceInfo : deviceInfos) {
        deviceInfoMap_.insert({deviceInfo.type_, std::make_shared<AdapterDeviceInfo>(deviceInfo)});
    }
}

void AudioPolicyConfigData::SetPipeMaps(std::list<AdapterPipeInfo> &pipeInfos)
{
    for (AdapterPipeInfo &pipeInfo : pipeInfos) {
        pipeInfoMap_.insert({pipeInfo.name_, std::make_shared<AdapterPipeInfo>(pipeInfo)});
    }
}

void AudioPolicyConfigData::SetSupportDeviceAndPipeMaps(std::shared_ptr<AdapterPipeInfo> pipeInfo)
{
    for (PipeStreamPropInfo &streamPropInfo : pipeInfo->streamPropInfos_) {
        for (DeviceType &supportDevice : streamPropInfo.supportDevices_) {
            std::shared_ptr<AdapterDeviceInfo> deviceInfo = deviceInfoMap_.find(supportDevice)->second;

            streamPropInfo.supportDeviceMap_.insert({supportDevice, deviceInfo});
            for (auto &supportFlag : pipeInfo->supportFlags_) {
                deviceInfo->supportPipeMap_.insert({supportFlag, pipeInfo});
            }
        }
    }
}

void AudioPolicyConfigData::Reorganize()
{
    for (auto &pair : adapterInfoMap_) {
        std::list<AdapterDeviceInfo> deviceInfos {};
        std::list<AdapterPipeInfo> pipeInfos {};
        pair.second.GetDeviceInfos(deviceInfos);
        SetDeviceMaps(deviceInfos);
        pair.second.GetPipeInfos(pipeInfos);
        SetPipeMaps(pipeInfos);
    }

    for (auto &pipePair : pipeInfoMap_) {
        SetSupportDeviceAndPipeMaps(pipePair.second);
    }
}

void AudioPolicyConfigData::SetVersion(const std::string version)
{
    if (!version.empty()) {
        version_ = version;
    } else {
        AUDIO_ERR_LOG("Set version failed, data is empty");
    }
}

void AudioPolicyConfigData::SetAdapterInfoMap(std::unordered_map<AudioAdapterType, PolicyAdapterInfo> &adapterInfoMap)
{
    if (!adapterInfoMap.empty()) {
        adapterInfoMap_ = std::move(adapterInfoMap);
    } else {
        AUDIO_ERR_LOG("Set adapterInfoMap failed, data is empty");
    }
}

std::string AudioPolicyConfigData::GetVersion()
{
    return version_;
}

void AudioPolicyConfigData::GetAdapterInfoMap(std::unordered_map<AudioAdapterType, PolicyAdapterInfo> &adapterInfoMap)
{
    adapterInfoMap = adapterInfoMap_;
}

void AudioPolicyConfigData::GetDeviceInfoMap(std::unordered_map<DeviceType,
    std::shared_ptr<AdapterDeviceInfo>> &deviceInfoMap)
{
    deviceInfoMap = deviceInfoMap_;
}

void AudioPolicyConfigData::GetPipeInfoMap(std::unordered_map<std::string,
    std::shared_ptr<AdapterPipeInfo>> &pipeInfoMap)
{
    pipeInfoMap = pipeInfoMap_;
}

AudioAdapterType PolicyAdapterInfo::GetTypeEnum()
{
    return GetAdapterType(adapterName_);
}

AudioAdapterType PolicyAdapterInfo::GetAdapterType(const std::string &adapterName)
{
    if (adapterName == ADAPTER_TYPE_PRIMARY) {
        return AudioAdapterType::TYPE_PRIMARY;
    } else if (adapterName == ADAPTER_TYPE_A2DP) {
        return AudioAdapterType::TYPE_A2DP;
    } else if (adapterName == ADAPTER_TYPE_REMOTE) {
        return AudioAdapterType::TYPE_REMOTE_AUDIO;
    } else if (adapterName == ADAPTER_TYPE_FILE) {
        return AudioAdapterType::TYPE_FILE_IO;
    } else if (adapterName == ADAPTER_TYPE_USB) {
        return AudioAdapterType::TYPE_USB;
    } else if (adapterName == ADAPTER_TYPE_DP) {
        return AudioAdapterType::TYPE_DP;
    } else if (adapterName == ADAPTER_TYPE_SLE) {
        return AudioAdapterType::TYPE_SLE;
    } else {
        return AudioAdapterType::TYPE_INVALID;
    }
}

std::shared_ptr<AdapterPipeInfo> PolicyAdapterInfo::GetPipeInfoByName(const std::string &pipeName)
{
    for (auto &pipeInfo : pipeInfos_) {
        if (pipeInfo.name_ == pipeName) {
            return std::make_shared<AdapterPipeInfo>(pipeInfo);
        }
    }
    return nullptr;
}

std::shared_ptr<AdapterDeviceInfo> PolicyAdapterInfo::GetDeviceInfoByType(DeviceType deviceType)
{
    for (auto &deviceInfo : deviceInfos_) {
        auto device = SUPPORTED_DEVICE_TYPE.find(deviceType);
        if (device != SUPPORTED_DEVICE_TYPE.end()) {
            return std::make_shared<AdapterDeviceInfo>(deviceInfo);
        }
    }
    return nullptr;
}

void PolicyAdapterInfo::SetAdapterName(const std::string adapterName)
{
    if (!adapterName.empty()) {
        adapterName_ = adapterName;
    } else {
        AUDIO_ERR_LOG("Set adapterName failed, data is empty");
    }
}

void PolicyAdapterInfo::SetAdapterSupportScene(const std::string adapterSupportScene)
{
    if (!adapterSupportScene.empty()) {
        adapterSupportScene_ = adapterSupportScene;
    } else {
        AUDIO_ERR_LOG("Set adapterSupportScene failed, data is empty");
    }
}

void PolicyAdapterInfo::SetDeviceInfos(std::list<AdapterDeviceInfo> &deviceInfos)
{
    if (!deviceInfos.empty()) {
        deviceInfos_ = std::move(deviceInfos);
    } else {
        AUDIO_ERR_LOG("Set deviceInfos failed, data is empty");
    }
}

void PolicyAdapterInfo::SetPipeInfos(std::list<AdapterPipeInfo> &pipeInfos)
{
    if (!pipeInfos.empty()) {
        pipeInfos_ = std::move(pipeInfos);
    } else {
        AUDIO_ERR_LOG("Set pipeInfos failed, data is empty");
    }
}

std::string PolicyAdapterInfo::GetAdapterName()
{
    return adapterName_;
}

std::string PolicyAdapterInfo::GetAdapterSupportScene()
{
    return adapterSupportScene_;
}

void PolicyAdapterInfo::GetDeviceInfos(std::list<AdapterDeviceInfo> &deviceInfos) const
{
    deviceInfos = deviceInfos_;
}

void PolicyAdapterInfo::GetPipeInfos(std::list<AdapterPipeInfo> &pipeInfos)
{
    pipeInfos = pipeInfos_;
}

bool AdapterPipeInfo::IsSupportFlag(AudioFlag flag_)
{
    for (auto supportedFlag : supportFlags_) {
        if (flag_ & supportedFlag) {
            return true;
        }
    }
    return false;
}

}
}
