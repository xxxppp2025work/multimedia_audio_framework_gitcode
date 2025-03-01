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

    isAdapterInfoMap_.store(true);
}

void AudioPolicyConfigManager::OnXmlParsingCompleted(const std::unordered_map<ClassType, std::list<AudioModuleInfo>> &xmlData)
{
    AUDIO_INFO_LOG("device class num [%{public}zu]", xmlData.size());
    CHECK_AND_RETURN_LOG(!xmlData.empty(), "failed to parse xml file. Received data is empty");

    deviceClassInfo_ = xmlData;
}

void AudioPolicyConfigManager::OnAudioLatencyParsed(uint64_t latency)
{
    audioLatencyInMsec_ = latency;
}

void AudioPolicyConfigManager::OnSinkLatencyParsed(uint32_t latency)
{
    sinkLatencyInMsec_ = latency;
}

void AudioPolicyConfigManager::OnVolumeGroupParsed(std::unordered_map<std::string, std::string>& volumeGroupData)
{
    AUDIO_INFO_LOG("group data num [%{public}zu]", volumeGroupData.size());
    CHECK_AND_RETURN_LOG(!volumeGroupData.empty(), "failed to parse xml file. Received data is empty");

    volumeGroupData_ = volumeGroupData;
}

void AudioPolicyConfigManager::OnInterruptGroupParsed(std::unordered_map<std::string, std::string>& interruptGroupData)
{
    AUDIO_INFO_LOG("group data num [%{public}zu]", interruptGroupData.size());
    CHECK_AND_RETURN_LOG(!interruptGroupData.empty(), "failed to parse xml file. Received data is empty");

    interruptGroupData_ = interruptGroupData;
}

void AudioPolicyConfigManager::OnUpdateRouteSupport(bool isSupported)
{
    isUpdateRouteSupported_ = isSupported;
}

void AudioPolicyConfigManager::OnGlobalConfigsParsed(GlobalConfigs &globalConfigs)
{
    globalConfigs_ = globalConfigs;
}

void AudioPolicyConfigManager::OnVoipConfigParsed(bool enableFastVoip)
{
    enableFastVoip_ = enableFastVoip;
}

void AudioPolicyConfigManager::OnUpdateAnahsSupport(std::string anahsShowType)
{
    AUDIO_INFO_LOG("OnUpdateAnahsSupport show type: %{public}s", anahsShowType.c_str());
    AudioPolicyService::GetAudioPolicyService().OnUpdateAnahsSupport(anahsShowType);
}

void AudioPolicyConfigManager::OnHasEarpiece(std::unordered_map<AudioAdapterType, PolicyAdapterInfo> &adapterInfoMap)
{
    for (const auto &adapterInfo : adapterInfoMap) {
        std::list<AdapterDeviceInfo> deviceInfoList;
        (adapterInfo.second).GetDeviceInfos(deviceInfoList);
        hasEarpiece_ = std::any_of(deviceInfoList.begin(), deviceInfoList.end(),
            [](const auto& deviceInfo) {
                return deviceInfo.type_ == EARPIECE_TYPE_NAME;
            });
        if (hasEarpiece_) {
            break;
        }
    }
    audioDeviceManager_.UpdateEarpieceStatus(hasEarpiece_);
}

void AudioPolicyConfigManager::SetNormalVoipFlag(const bool &normalVoipFlag)
{
    normalVoipFlag_ = normalVoipFlag;
}

bool AudioPolicyConfigManager::GetModuleListByType(ClassType type, std::list<AudioModuleInfo>& moduleList)
{
    auto modulesPos = deviceClassInfo_.find(type);
    if (modulesPos != deviceClassInfo_.end()) {
        moduleList = modulesPos->second;
        return true;
    }
    return false;
}

void AudioPolicyConfigManager::GetDeviceClassInfo(std::unordered_map<ClassType, std::list<AudioModuleInfo>> &deviceClassInfo)
{
    deviceClassInfo = deviceClassInfo_;
}

std::string AudioPolicyConfigManager::GetGroupName(const std::string& deviceName, const GroupType type)
{
    std::string groupName = GROUP_NAME_NONE;
    if (type == VOLUME_TYPE) {
        auto iter = volumeGroupData_.find(deviceName);
        if (iter != volumeGroupData_.end()) {
            groupName = iter->second;
        }
    } else {
        auto iter = interruptGroupData_.find(deviceName);
        if (iter != interruptGroupData_.end()) {
            groupName = iter->second;
        }
    }
    return groupName;
}

int32_t AudioPolicyConfigManager::GetMaxRendererInstances()
{
    auto configIter = std::find_if(globalConfigs_.outputConfigInfos_.begin(), globalConfigs_.outputConfigInfos_.end(),
        [](const auto& configInfo) {
            return configInfo.name_ == "normal" && configInfo.value_ != "";
        });
    if (configIter != globalConfigs_.outputConfigInfos_.end()) {
        AUDIO_INFO_LOG("Max output normal instance is %{public}s", configIter->value_.c_str());
        int32_t convertValue = 0;
        CHECK_AND_RETURN_RET_LOG(StringConverter(configIter->value_, convertValue),
            DEFAULT_MAX_OUTPUT_NORMAL_INSTANCES,
            "convert invalid configInfo.value_: %{public}s", configIter->value_.c_str());
        return convertValue;
    }
    return DEFAULT_MAX_OUTPUT_NORMAL_INSTANCES;
}

int32_t AudioPolicyConfigManager::GetVoipRendererFlag(const std::string &sinkPortName, const std::string &networkId,
    const AudioSamplingRate &samplingRate)
{
    // VoIP stream has three mode for different products.
    if (enableFastVoip_ && (sinkPortName == PRIMARY_SPEAKER && networkId == LOCAL_NETWORK_ID)) {
        if (samplingRate != SAMPLE_RATE_48000 && samplingRate != SAMPLE_RATE_16000) {
            return AUDIO_FLAG_NORMAL;
        }
        return AUDIO_FLAG_VOIP_FAST;
    } else if (!normalVoipFlag_ && (sinkPortName == PRIMARY_SPEAKER) && (networkId == LOCAL_NETWORK_ID)) {
        AUDIO_INFO_LOG("Direct VoIP mode is supported for the device");
        return AUDIO_FLAG_VOIP_DIRECT;
    }

    return AUDIO_FLAG_NORMAL;
}

int32_t AudioPolicyConfigManager::GetAudioLatencyFromXml() const
{
    return audioLatencyInMsec_;
}

uint32_t AudioPolicyConfigManager::GetSinkLatencyFromXml() const
{
    return sinkLatencyInMsec_;
}

void AudioPolicyConfigManager::GetAudioAdapterInfos(std::unordered_map<AdaptersType, AudioAdapterInfo> &adapterInfoMap)
{
    // TODO: no adapterInfoMap
    // adapterInfoMap = adapterInfoMap_;
}

void AudioPolicyConfigManager::GetVolumeGroupData(std::unordered_map<std::string, std::string>& volumeGroupData)
{
    volumeGroupData = volumeGroupData_;
}

void AudioPolicyConfigManager::GetInterruptGroupData(std::unordered_map<std::string, std::string>& interruptGroupData)
{
    interruptGroupData = interruptGroupData_;
}

void AudioPolicyConfigManager::GetGlobalConfigs(GlobalConfigs &globalConfigs)
{
    globalConfigs = globalConfigs_;
}

bool AudioPolicyConfigManager::GetVoipConfig()
{
    return enableFastVoip_;
}

bool AudioPolicyConfigManager::GetUpdateRouteSupport()
{
    return isUpdateRouteSupported_;
}

bool AudioPolicyConfigManager::GetAdapterInfoFlag()
{
    return isAdapterInfoMap_.load();
}

bool AudioPolicyConfigManager::GetAdapterInfoByType(AdaptersType type, AudioAdapterInfo &info)
{
    auto it = adapterInfoMap_.find(type);
    if (it == adapterInfoMap_.end()) {
        AUDIO_ERR_LOG("can not find adapter info");
        return false;
    }
    info = it->second;
    return true;
}

bool AudioPolicyConfigManager::GetHasEarpiece()
{
    return hasEarpiece_;
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
