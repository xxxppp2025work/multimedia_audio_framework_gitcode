
#ifndef LOG_TAG
#define LOG_TAG "AudioPolicyConfigManager"
#endif

#include "audio_policy_config_manager.h"
#include <ability_manager_client.h>
#include "iservice_registry.h"
#include "parameter.h"
#include "parameters.h"
#include "audio_utils.h"
#include "audio_log.h"
#include "audio_utils.h"
#include "audio_manager_listener_stub.h"
#include "audio_inner_call.h"
#include "media_monitor_manager.h"
#include "audio_device_manager.h"

#include "audio_policy_common.h"


namespace OHOS {
namespace AudioStandard {

const int32_t DEFAULT_MAX_OUTPUT_NORMAL_INSTANCES = 128;
static const std::string EARPIECE_TYPE_NAME = "DEVICE_TYPE_EARPIECE";

bool AudioPolicyConfigManager::Init()
{
    bool ret = audioPolicyConfigParser_.LoadConfiguration();
    if (!ret) {
        AudioPolicyCommon::GetInstance().WriteServiceStartupError("Audio Policy Config Load Configuration failed");
        AUDIO_ERR_LOG("Audio Policy Config Load Configuration failed");
        return ret;
    }
    ret = audioPolicyConfigParser_.Parse();
    if (!ret) {
        AudioPolicyCommon::GetInstance().WriteServiceStartupError("Audio Config Parse failed");
        AUDIO_ERR_LOG("Audio Policy Config Parse Configuration failed");
    }
    return ret;
}

void AudioPolicyConfigManager::OnAudioPolicyXmlParsingCompleted(
    const std::unordered_map<AdaptersType, AudioAdapterInfo> adapterInfoMap)
{
    AUDIO_INFO_LOG("adapterInfo num [%{public}zu]", adapterInfoMap.size());
    CHECK_AND_RETURN_LOG(!adapterInfoMap.empty(), "failed to parse audiopolicy xml file. Received data is empty");
    adapterInfoMap_ = adapterInfoMap;

    for (auto &adapterInfo : adapterInfoMap_) {
        for (auto &deviceInfos : (adapterInfo.second).deviceInfos_) {
            if (deviceInfos.type_ == EARPIECE_TYPE_NAME) {
                AUDIO_INFO_LOG("Has earpiece");
                hasEarpiece_ = true;
                break;
            }
        }
        if (hasEarpiece_) {
            break;
        }
    }
    isAdapterInfoMap_.store(true);

    AudioDeviceManager::GetAudioDeviceManager().UpdateEarpieceStatus(hasEarpiece_);
}

bool AudioPolicyConfigManager::GetHasEarpiece()
{
    return hasEarpiece_;
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

// Parser callbacks
void AudioPolicyConfigManager::OnXmlParsingCompleted(const std::unordered_map<ClassType, std::list<AudioModuleInfo>> &xmlData)
{
    AUDIO_INFO_LOG("device class num [%{public}zu]", xmlData.size());
    CHECK_AND_RETURN_LOG(!xmlData.empty(), "failed to parse xml file. Received data is empty");

    deviceClassInfo_ = xmlData;
}

void AudioPolicyConfigManager::GetDeviceClassInfo(std::unordered_map<ClassType, std::list<AudioModuleInfo>> &deviceClassInfo)
{
    deviceClassInfo = deviceClassInfo_;
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

void AudioPolicyConfigManager::OnUpdateRouteSupport(bool isSupported)
{
    isUpdateRouteSupported_ = isSupported;
}

bool AudioPolicyConfigManager::GetUpdateRouteSupport()
{
    return isUpdateRouteSupported_;
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

void AudioPolicyConfigManager::OnGlobalConfigsParsed(GlobalConfigs &globalConfigs)
{
    globalConfigs_ = globalConfigs;
}

int32_t AudioPolicyConfigManager::GetMaxRendererInstances()
{
    for (auto &configInfo : globalConfigs_.outputConfigInfos_) {
        if (configInfo.name_ == "normal" && configInfo.value_ != "") {
            AUDIO_INFO_LOG("Max output normal instance is %{public}s", configInfo.value_.c_str());
            return (int32_t)std::stoi(configInfo.value_);
        }
    }
    return DEFAULT_MAX_OUTPUT_NORMAL_INSTANCES;
}

void AudioPolicyConfigManager::OnVoipConfigParsed(bool enableFastVoip)
{
    enableFastVoip_ = enableFastVoip;
}


void AudioPolicyConfigManager::SetNormalVoipFlag(const bool &normalVoipFlag)
{
    normalVoipFlag_ = normalVoipFlag;
}

int32_t AudioPolicyConfigManager::GetVoipRendererFlag(const std::string &sinkPortName, const std::string &networkId)
{
    // VoIP stream has three mode for different products.
    if (enableFastVoip_ && (sinkPortName == PRIMARY_SPEAKER || networkId != LOCAL_NETWORK_ID)) {
        return AUDIO_FLAG_VOIP_FAST;
    } else if (!normalVoipFlag_ && (sinkPortName == PRIMARY_SPEAKER) && (networkId == LOCAL_NETWORK_ID)) {
        AUDIO_INFO_LOG("Direct VoIP mode is supported for the device");
        return AUDIO_FLAG_VOIP_DIRECT;
    }

    return AUDIO_FLAG_NORMAL;
}

void AudioPolicyConfigManager::OnAudioLatencyParsed(uint64_t latency)
{
    audioLatencyInMsec_ = latency;
}

void AudioPolicyConfigManager::OnSinkLatencyParsed(uint32_t latency)
{
    sinkLatencyInMsec_ = latency;
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
    adapterInfoMap = adapterInfoMap_;
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

}
}