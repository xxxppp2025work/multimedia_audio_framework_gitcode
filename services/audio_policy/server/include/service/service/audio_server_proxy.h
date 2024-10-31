
#ifndef ST_AUDIO_SERVER_PROXY_H
#define ST_AUDIO_SERVER_PROXY_H

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

#include "ipc_skeleton.h"

namespace OHOS {
namespace AudioStandard {
class AudioServerProxy {
public:
    static AudioServerProxy& GetInstance()
    {
        static AudioServerProxy instance;
        return instance;
    }
    const sptr<IStandardAudioService> GetAudioServerProxy();
    int32_t SetAudioSceneProxy(AudioScene audioScene, std::vector<DeviceType> activeOutputDevices,
        DeviceType deviceType, BluetoothOffloadState state);
    float GetMaxAmplitudeProxy(bool flag, DeviceType type);
    void UpdateEffectBtOffloadSupportedProxy(const bool &isSupported);
    void SetOutputDeviceSinkProxy(DeviceType deviceType, std::string sinkName);
    bool GetAudioEffectOffloadFlagProxy();
    int32_t UpdateActiveDevicesRouteProxy(
        std::vector<std::pair<DeviceType, DeviceFlag>> &activeDevices, BluetoothOffloadState state);
    int32_t UpdateDualToneStateProxy(const bool &enable, const int32_t &sessionId);
    void UpdateSessionConnectionStateProxy(const int32_t &sessionID, const int32_t &state);
    int32_t CheckRemoteDeviceStateProxy(std::string networkId, DeviceRole deviceRole, bool isStartDevice);
    void SetAudioParameterProxy(const std::string &key, const std::string &value);
    void ResetAudioEndpointProxy();
    bool NotifyStreamVolumeChangedProxy(AudioStreamType streamType, float volume);
    void OffloadSetVolumeProxy(float volume);
    void SetVoiceVolumeProxy(float volume);
    void UnsetOffloadModeProxy(uint32_t sessionId);
    void SetOffloadModeProxy(uint32_t sessionId, int32_t state, bool isAppBack);
    void RestoreSessionProxy(const int32_t &sessionID, bool isOutput);
    int32_t GetAudioEnhancePropertyProxy(AudioEnhancePropertyArray &propertyArray);
    void SetSinkMuteForSwitchDeviceProxy(const std::string &devceClass, int32_t durationUs, bool mute);
    int32_t SetMicrophoneMuteProxy(bool isMute);
    void SuspendRenderSinkProxy(const std::string &sinkName);
    void RestoreRenderSinkProxy(const std::string &sinkName);
    void LoadHdiEffectModelProxy();
    void NotifyDeviceInfoProxy(std::string networkId, bool connected);
    std::string GetAudioParameterProxy(const std::string &key);
    std::string GetAudioParameterProxy(const std::string& networkId, const AudioParamKey key,
        const std::string& condition);
    void ResetRouteForDisconnectProxy(DeviceType type);
private:
    AudioServerProxy() {}
    ~AudioServerProxy() {}
private:
    std::mutex adProxyMutex_;
};
}
}
#endif