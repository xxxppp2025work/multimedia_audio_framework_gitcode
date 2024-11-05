#ifndef ST_AUDIO_POLICY_COMMON_H
#define ST_AUDIO_POLICY_COMMON_H

#include <bitset>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include "singleton.h"
#include "audio_group_handle.h"
#include "audio_info.h"
#include "audio_manager_base.h"
#include "audio_module_info.h"
#include "audio_ec_info.h"
#include "datashare_helper.h"
#include "audio_utils.h"
#include "audio_errors.h"

namespace OHOS {
namespace AudioStandard {

using InternalDeviceType = DeviceType;

class AudioPolicyCommon {
public:
    static AudioPolicyCommon& GetInstance()
    {
        static AudioPolicyCommon instance;
        return instance;
    }
    int32_t GetDeviceNameFromDataShareHelper(std::string &deviceName);
    void RegisterNameMonitorHelper();
    bool IsDataShareReady();

    bool IsWiredHeadSet(const DeviceType &deviceType);
    DeviceRole GetDeviceRole(DeviceType deviceType) const;
    DeviceRole GetDeviceRole(const std::string &role);
    DeviceRole GetDeviceRole(AudioPin pin) const;
    std::string ConvertToHDIAudioFormat(AudioSampleFormat sampleFormat);
    uint32_t GetSampleFormatValue(AudioSampleFormat sampleFormat);
    std::string ParseAudioFormat(std::string format);
    int64_t GetCurrentTimeMS();
    uint32_t PcmFormatToBits(AudioSampleFormat format);
    std::string GetPipeNameByDeviceForEc(const std::string &role, const DeviceType deviceType);
    void GetTargetSourceTypeAndMatchingFlag(SourceType source, bool isEcFeatureEnable,
        SourceType &targetSource, bool &useMatchingPropInfo);
    DeviceType GetDeviceTypeFromPin(AudioPin pin);
    std::string GetSourcePortName(InternalDeviceType deviceType);
    bool HasLowLatencyCapability(DeviceType deviceType, bool isRemote);
    void GetUsbModuleInfo(std::string deviceInfo, AudioModuleInfo &moduleInfo);
    void GetDPModuleInfo(AudioModuleInfo &moduleInfo, std::string deviceInfo);
    std::string GetRemoteModuleName(std::string networkId, DeviceRole role);
    EcType GetEcType(const DeviceType inputDevice, const DeviceType outputDevice);
    InternalDeviceType GetDeviceType(const std::string &deviceName);
    void WriteServiceStartupError(std::string reason);
    std::string GetEncryptAddr(const std::string &addr);
    AdaptersType GetAdapterType(std::string sinkPortName);
    std::string GetSinkPortName(DeviceType deviceType, AudioPipeType pipeType = PIPE_TYPE_UNKNOWN);
    int32_t SetPreferredDevice(const PreferredType preferredType, const sptr<AudioDeviceDescriptor> &desc);
    void SetBtConnecting(bool flag);
    void ClearScoDeviceSuspendState(std::string macAddress = "");
    void UpdateDisplayName(sptr<AudioDeviceDescriptor> deviceDescriptor);
    std::vector<std::unique_ptr<AudioDeviceDescriptor>> GetAvailableDevicesInner(AudioDeviceUsage usage);
    void UpdateDescWhenNoBTPermission(std::vector<sptr<AudioDeviceDescriptor>> &deviceDescs);
    AudioModuleInfo ConstructRemoteAudioModuleInfo(std::string networkId, DeviceRole deviceRole,
        DeviceType deviceType);
    std::vector<sptr<AudioDeviceDescriptor>> DeviceFilterByUsageInner(AudioDeviceUsage usage,
        const std::vector<sptr<AudioDeviceDescriptor>>& descs);
public:
    static int32_t startDeviceId;
private:
    std::shared_ptr<DataShare::DataShareHelper> CreateDataShareHelperInstance();
    int32_t ErasePreferredDeviceByType(const PreferredType preferredType);
private:
    AudioPolicyCommon() {}
    ~AudioPolicyCommon() {}
private:
    bool isBTReconnecting_ = true;
};

}
}

#endif