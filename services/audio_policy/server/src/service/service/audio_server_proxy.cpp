
#ifndef LOG_TAG
#define LOG_TAG "AudioServerProxy"
#endif

#include "audio_server_proxy.h"
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

namespace OHOS {
namespace AudioStandard {

const sptr<IStandardAudioService> AudioServerProxy::GetAudioServerProxy()
{
    AUDIO_DEBUG_LOG("[Policy Service] Start get audio policy service proxy.");
    std::lock_guard<std::mutex> lock(adProxyMutex_);
    auto samgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    CHECK_AND_RETURN_RET_LOG(samgr != nullptr, nullptr, "[Policy Service] Get samgr failed.");

    sptr<IRemoteObject> object = samgr->GetSystemAbility(AUDIO_DISTRIBUTED_SERVICE_ID);
    CHECK_AND_RETURN_RET_LOG(object != nullptr, nullptr,
        "[Policy Service] audio service remote object is NULL.");

    const sptr<IStandardAudioService> gsp = iface_cast<IStandardAudioService>(object);
    CHECK_AND_RETURN_RET_LOG(gsp != nullptr, nullptr,
        "[Policy Service] init gsp is NULL.");
    return gsp;
}

int32_t AudioServerProxy::SetAudioSceneProxy(AudioScene audioScene, std::vector<DeviceType> activeOutputDevices,
    DeviceType deviceType, BluetoothOffloadState state)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_RET_LOG(gsp != nullptr, ERR_OPERATION_FAILED, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    int32_t result = gsp->SetAudioScene(audioScene, activeOutputDevices, deviceType, state);
    IPCSkeleton::SetCallingIdentity(identity);
    return result;
}

float AudioServerProxy::GetMaxAmplitudeProxy(bool flag, DeviceType type)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_RET_LOG(gsp != nullptr, 0, "Service proxy unavailable");

    std::string identity = IPCSkeleton::ResetCallingIdentity();
    float maxAmplitude = gsp->GetMaxAmplitude(flag, type);
    IPCSkeleton::SetCallingIdentity(identity);
    return maxAmplitude;
}

void AudioServerProxy::UpdateEffectBtOffloadSupportedProxy(const bool &isSupported)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_LOG(gsp != nullptr, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->UpdateEffectBtOffloadSupported(isSupported);
    IPCSkeleton::SetCallingIdentity(identity);
    return;
}

void AudioServerProxy::SetOutputDeviceSinkProxy(DeviceType deviceType, std::string sinkName)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_LOG(gsp != nullptr, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->SetOutputDeviceSink(deviceType, sinkName);
    IPCSkeleton::SetCallingIdentity(identity);
}

bool AudioServerProxy::GetAudioEffectOffloadFlagProxy()
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_RET_LOG(gsp != nullptr, false, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    bool effectOffloadFlag = gsp->GetEffectOffloadEnabled();
    IPCSkeleton::SetCallingIdentity(identity);
    return effectOffloadFlag;
}

int32_t AudioServerProxy::UpdateActiveDevicesRouteProxy(
    std::vector<std::pair<DeviceType, DeviceFlag>> &activeDevices, BluetoothOffloadState state)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_RET_LOG(gsp != nullptr, ERR_OPERATION_FAILED, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    int32_t ret = gsp->UpdateActiveDevicesRoute(activeDevices, state);
    IPCSkeleton::SetCallingIdentity(identity);
    return ret;
}

int32_t AudioServerProxy::UpdateDualToneStateProxy(const bool &enable, const int32_t &sessionId)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_RET_LOG(gsp != nullptr, ERR_OPERATION_FAILED, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    int32_t ret = gsp->UpdateDualToneState(enable, sessionId);
    IPCSkeleton::SetCallingIdentity(identity);
    return ret;
}

void AudioServerProxy::UpdateSessionConnectionStateProxy(const int32_t &sessionID, const int32_t &state)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_LOG(gsp != nullptr, "Service proxy unavailable");

    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->UpdateSessionConnectionState(sessionID, state);
    IPCSkeleton::SetCallingIdentity(identity);
}

int32_t AudioServerProxy::CheckRemoteDeviceStateProxy(std::string networkId, DeviceRole deviceRole, bool isStartDevice)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_RET_LOG(gsp != nullptr, ERR_OPERATION_FAILED, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    int32_t res = gsp->CheckRemoteDeviceState(networkId, deviceRole, isStartDevice);
    IPCSkeleton::SetCallingIdentity(identity);
    return res;
}

void AudioServerProxy::SetAudioParameterProxy(const std::string &key, const std::string &value)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_LOG(gsp != nullptr, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->SetAudioParameter(key, value);
    IPCSkeleton::SetCallingIdentity(identity);
}

void AudioServerProxy::ResetAudioEndpointProxy()
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_LOG(gsp != nullptr, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->ResetAudioEndpoint();
    IPCSkeleton::SetCallingIdentity(identity);
}

bool AudioServerProxy::NotifyStreamVolumeChangedProxy(AudioStreamType streamType, float volume)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_RET_LOG(gsp != nullptr, false, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->NotifyStreamVolumeChanged(streamType, volume);
    IPCSkeleton::SetCallingIdentity(identity);
    return true;
}

void AudioServerProxy::OffloadSetVolumeProxy(float volume)
{
    const sptr <IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_LOG(gsp != nullptr, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->OffloadSetVolume(volume);
    IPCSkeleton::SetCallingIdentity(identity);
}

void AudioServerProxy::SetVoiceVolumeProxy(float volume)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_LOG(gsp != nullptr, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->SetVoiceVolume(volume);
    IPCSkeleton::SetCallingIdentity(identity);
}

void AudioServerProxy::UnsetOffloadModeProxy(uint32_t sessionId)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_LOG(gsp != nullptr, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->UnsetOffloadMode(sessionId);
    IPCSkeleton::SetCallingIdentity(identity);
}

void AudioServerProxy::SetOffloadModeProxy(uint32_t sessionId, int32_t state, bool isAppBack)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_LOG(gsp != nullptr, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->SetOffloadMode(sessionId, state, isAppBack);
    IPCSkeleton::SetCallingIdentity(identity);
}
void AudioServerProxy::RestoreSessionProxy(const int32_t &sessionID, bool isOutput)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_LOG(gsp != nullptr, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->RestoreSession(sessionID, isOutput);
    IPCSkeleton::SetCallingIdentity(identity);
}

int32_t AudioServerProxy::GetAudioEnhancePropertyProxy(AudioEnhancePropertyArray &propertyArray)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_RET_LOG(gsp != nullptr, ERR_INVALID_HANDLE, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    int32_t ret = gsp->GetAudioEnhanceProperty(propertyArray);
    IPCSkeleton::SetCallingIdentity(identity);
    return ret;
}

void AudioServerProxy::SetSinkMuteForSwitchDeviceProxy(const std::string &devceClass, int32_t durationUs, bool mute)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_LOG(gsp != nullptr, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->SetSinkMuteForSwitchDevice(devceClass, durationUs, mute);
    IPCSkeleton::SetCallingIdentity(identity);
}

int32_t AudioServerProxy::SetMicrophoneMuteProxy(bool isMute)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_RET_LOG(gsp != nullptr, ERR_OPERATION_FAILED, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    int32_t ret = gsp->SetMicrophoneMute(isMute);
    IPCSkeleton::SetCallingIdentity(identity);
    return ret;
}

void AudioServerProxy::SuspendRenderSinkProxy(const std::string &sinkName)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_LOG(gsp != nullptr, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->SuspendRenderSink(sinkName);
    IPCSkeleton::SetCallingIdentity(identity);
}

void AudioServerProxy::RestoreRenderSinkProxy(const std::string &sinkName)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_LOG(gsp != nullptr, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->RestoreRenderSink(sinkName);
    IPCSkeleton::SetCallingIdentity(identity);
}

void AudioServerProxy::LoadHdiEffectModelProxy()
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_LOG(gsp != nullptr, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->LoadHdiEffectModel();
    IPCSkeleton::SetCallingIdentity(identity);
}

void AudioServerProxy::NotifyDeviceInfoProxy(std::string networkId, bool connected)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_LOG(gsp != nullptr, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->NotifyDeviceInfo(networkId, connected);
    IPCSkeleton::SetCallingIdentity(identity);  
}

std::string AudioServerProxy::GetAudioParameterProxy(const std::string &key)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_RET_LOG(gsp != nullptr, "", "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    std::string result = gsp->GetAudioParameter(key);
    IPCSkeleton::SetCallingIdentity(identity);
    return result;
}

std::string AudioServerProxy::GetAudioParameterProxy(const std::string& networkId, const AudioParamKey key,
    const std::string& condition)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_RET_LOG(gsp != nullptr, "", "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    std::string result = gsp->GetAudioParameter(networkId, key, condition);
    IPCSkeleton::SetCallingIdentity(identity);
    return result;
}

void AudioServerProxy::ResetRouteForDisconnectProxy(DeviceType type)
{
    const sptr<IStandardAudioService> gsp = GetAudioServerProxy();
    CHECK_AND_RETURN_LOG(gsp != nullptr, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    gsp->ResetRouteForDisconnect(type);
    IPCSkeleton::SetCallingIdentity(identity);
}



}
}
