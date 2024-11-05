#ifndef ST_AUDIO_POLICY_CONFIG_MANAGER_H
#define ST_AUDIO_POLICY_CONFIG_MANAGER_H

#include <bitset>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include "iport_observer.h"
#include "singleton.h"
#include "audio_group_handle.h"
#include "audio_info.h"
#include "audio_manager_base.h"
#include "audio_module_info.h"
#include "audio_volume_config.h"
#include "audio_utils.h"
#include "audio_errors.h"
#include "audio_policy_parser_factory.h"

namespace OHOS {
namespace AudioStandard {

class AudioPolicyConfigManager : public IPortObserver {
public:
    static AudioPolicyConfigManager& GetInstance()
    {
        static AudioPolicyConfigManager instance;
        return instance;
    }
    bool Init();
    // Audio Policy Parser callbacks
    void OnAudioPolicyXmlParsingCompleted(const std::unordered_map<AdaptersType, AudioAdapterInfo> adapterInfoMap);

    // Parser callbacks
    void OnXmlParsingCompleted(const std::unordered_map<ClassType, std::list<AudioModuleInfo>> &xmldata);

    void OnUpdateRouteSupport(bool isSupported);

    void OnAudioLatencyParsed(uint64_t latency);

    void OnSinkLatencyParsed(uint32_t latency);

    void OnVolumeGroupParsed(std::unordered_map<std::string, std::string>& volumeGroupData);

    void OnInterruptGroupParsed(std::unordered_map<std::string, std::string>& interruptGroupData);

    void OnGlobalConfigsParsed(GlobalConfigs &globalConfigs);

    void OnVoipConfigParsed(bool enableFastVoip);

    bool GetModuleListByType(ClassType type, std::list<AudioModuleInfo>& moduleList);

    void GetDeviceClassInfo(std::unordered_map<ClassType, std::list<AudioModuleInfo>> &deviceClassInfo);

    std::string GetGroupName(const std::string& deviceName, const GroupType type);

    int32_t GetMaxRendererInstances();
    void SetNormalVoipFlag(const bool &normalVoipFlag);

    int32_t GetVoipRendererFlag(const std::string &sinkPortName, const std::string &networkId);
    void GetGlobalConfigs(GlobalConfigs &globalConfigs);

    bool GetVoipConfig();

    int32_t GetAudioLatencyFromXml() const;

    uint32_t GetSinkLatencyFromXml() const;

    void GetAudioAdapterInfos(std::unordered_map<AdaptersType, AudioAdapterInfo> &adapterInfoMap);

    void GetVolumeGroupData(std::unordered_map<std::string, std::string>& volumeGroupData);

    void GetInterruptGroupData(std::unordered_map<std::string, std::string>& interruptGroupData);

    bool GetUpdateRouteSupport();

    bool GetAdapterInfoFlag();

    bool GetAdapterInfoByType(AdaptersType type, AudioAdapterInfo &info);

    bool GetHasEarpiece();
private:
    AudioPolicyConfigManager() : audioPolicyConfigParser_(AudioPolicyParserFactory::GetInstance().CreateParser(*this))
    {
    }
    ~AudioPolicyConfigManager()
    {
    }
private:
    Parser& audioPolicyConfigParser_;

    std::unordered_map<ClassType, std::list<AudioModuleInfo>> deviceClassInfo_ = {};
    std::unordered_map<AdaptersType, AudioAdapterInfo> adapterInfoMap_ {};

    bool hasEarpiece_ = false;
    bool isUpdateRouteSupported_ = true;
    std::unordered_map<std::string, std::string> volumeGroupData_;
    std::unordered_map<std::string, std::string> interruptGroupData_;
    GlobalConfigs globalConfigs_;
    bool enableFastVoip_ = false;
    uint64_t audioLatencyInMsec_ = 50;
    uint32_t sinkLatencyInMsec_ {0};
    bool normalVoipFlag_ = false;

    std::atomic<bool> isAdapterInfoMap_ = false;
};

}
}

#endif