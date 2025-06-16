/*
 * Copyright (c) 2021-2024 Huawei Device Co., Ltd.
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

#ifndef I_AUDIO_POLICY_BASE_H
#define I_AUDIO_POLICY_BASE_H

#include "audio_interrupt_callback.h"
#include "audio_policy_ipc_interface_code.h"
#include "ipc_types.h"
#include "iremote_broker.h"
#include "iremote_proxy.h"
#include "iremote_stub.h"
#include "audio_system_manager.h"
#include "audio_effect.h"
#include "microphone_descriptor.h"
#include "audio_zone_manager.h"
#include "audio_stream_descriptor.h"

namespace OHOS {
namespace AudioStandard {
using InternalDeviceType = DeviceType;
using InternalAudioCapturerOptions = AudioCapturerOptions;

class IAudioPolicy : public IRemoteBroker {
public:

    virtual int32_t GetMaxVolumeLevel(int32_t volumeType) = 0;

    virtual int32_t GetMinVolumeLevel(int32_t volumeType) = 0;

    virtual int32_t SetSystemVolumeLevelLegacy(int32_t volumeType, int32_t volumeLevel) = 0;

    virtual int32_t SetSystemVolumeLevel(int32_t volumeType, int32_t volumeLevel, int32_t volumeFlag) = 0;

    virtual int32_t SetSystemVolumeLevelWithDevice(int32_t volumeType, int32_t volumeLevel,
        int32_t deviceType, int32_t volumeFlag) = 0;

    virtual int32_t GetAppVolumeLevel(int32_t appUid, int32_t &volumeLevel) = 0;

    virtual int32_t GetSelfAppVolumeLevel(int32_t &volumeLevel) = 0;

    virtual int32_t SetAppVolumeLevel(int32_t appUid, int32_t volumeLevel, int32_t volumeFlag) = 0;

    virtual int32_t IsAppVolumeMute(int32_t appUid, bool muted, bool &isMute) = 0;

    virtual int32_t SetAppVolumeMuted(int32_t appUid, bool muted, int32_t volumeFlag) = 0;

    virtual int32_t SetSelfAppVolumeLevel(int32_t volumeLevel, int32_t volumeFlag) = 0;

    virtual int32_t GetSystemActiveVolumeType(int32_t clientUid, int32_t &streamType) = 0;

    virtual int32_t GetSystemVolumeLevel(int32_t volumeType) = 0;

    virtual int32_t SetLowPowerVolume(int32_t streamId, float volume) = 0;

    virtual int32_t GetLowPowerVolume(int32_t streamId, float &volume) = 0;

    virtual int32_t GetSingleStreamVolume(int32_t streamId, float &volume) = 0;

    virtual int32_t SetStreamMuteLegacy(int32_t volumeType, bool mute,
        int32_t deviceType) = 0;

    virtual int32_t SetStreamMute(int32_t volumeType, bool mute,
        int32_t deviceType) = 0;

    virtual int32_t GetStreamMute(int32_t volumeType, bool &mute) = 0;

    virtual int32_t IsStreamActive(int32_t volumeType, bool &active) = 0;

    virtual int32_t IsFastPlaybackSupported(int32_t &streamInfo, int32_t usage, bool &support) = 0;
    virtual int32_t IsFastRecordingSupported(int32_t &streamInfo, int32_t source, bool &support) = 0;

    virtual int32_t GetDevices(int32_t deviceFlag,
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> &deviceDescs) = 0;

    virtual int32_t GetDevicesInner(int32_t deviceFlag,
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> &deviceDescs) = 0;

    virtual int32_t GetOutputDevice(
        sptr<AudioRendererFilter> audioRendererFilter,
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> &deviceDescs) = 0;

    virtual int32_t GetInputDevice(
        sptr<AudioCapturerFilter> audioCapturerFilter,
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> &deviceDescs) = 0;

    virtual int32_t SetDeviceActive(int32_t deviceType, bool active, int32_t uid) = 0;

    virtual int32_t IsDeviceActive(int32_t deviceType, bool &active) = 0;

    virtual int32_t GetActiveOutputDevice(int32_t &deviceType) = 0;

    virtual int32_t GetDmDeviceType(uint16_t &deviceType) = 0;

    virtual int32_t GetActiveInputDevice(int32_t &deviceType) = 0;

#ifdef FEATURE_DTMF_TONE
    virtual int32_t GetToneConfig(int32_t ltonetype, const std::string &countryCode,
        std::shared_ptr<ToneInfo> &config) = 0;

    virtual int32_t GetSupportedTones(const std::string &countryCode,
        std::vector<int32_t> &tones) = 0;
#endif

    virtual int32_t SetRingerModeLegacy(int32_t ringMode) = 0;

    virtual int32_t SetRingerMode(int32_t ringMode) = 0;

    virtual int32_t GetRingerMode(int32_t &ringerMode) = 0;

    virtual int32_t SetAudioScene(int32_t scene) = 0;

    virtual int32_t SetMicrophoneMute(bool isMute) = 0;

    virtual int32_t SetMicrophoneMuteAudioConfig(bool isMute) = 0;

    virtual int32_t SetMicrophoneMutePersistent(bool isMute, int32_t type) = 0;

    virtual int32_t GetPersistentMicMuteState(bool &mute) = 0;

    virtual int32_t IsMicrophoneMuteLegacy(bool &mute) = 0;

    virtual int32_t IsMicrophoneMute(bool &mute) = 0;

    virtual int32_t GetAudioScene(int32_t &scene) = 0;

    virtual int32_t ActivateAudioSession(int32_t strategy) = 0;

    virtual int32_t DeactivateAudioSession() = 0;

    virtual int32_t IsAudioSessionActivated(int32_t &scene) = 0;

    virtual int32_t SetAudioInterruptCallback(uint32_t sessionID, const sptr<IRemoteObject> &object,
        uint32_t clientUid, int32_t zoneID) = 0;

    virtual int32_t UnsetAudioInterruptCallback(uint32_t sessionID,
        int32_t zoneID) = 0;

    virtual int32_t ActivateAudioInterrupt(const AudioInterrupt &audioInterrupt,
        int32_t zoneID,
        bool isUpdatedAudioStrategy) = 0;

    virtual int32_t DeactivateAudioInterrupt(const AudioInterrupt &audioInterrupt,
        int32_t zoneID) = 0;

    virtual int32_t ActivatePreemptMode(void) = 0;

    virtual int32_t DeactivatePreemptMode(void) = 0;

    virtual int32_t SetAudioManagerInterruptCallback(int32_t clientId, const sptr<IRemoteObject> &object) = 0;

    virtual int32_t UnsetAudioManagerInterruptCallback(int32_t clientId) = 0;

    virtual int32_t SetQueryClientTypeCallback(const sptr<IRemoteObject> &object) = 0;

    virtual int32_t SetAudioClientInfoMgrCallback(const sptr<IRemoteObject> &object) = 0;

    virtual int32_t SetQueryBundleNameListCallback(const sptr<IRemoteObject> &object) = 0;

    virtual int32_t RequestAudioFocus(int32_t clientId, const AudioInterrupt &audioInterrupt) = 0;

    virtual int32_t AbandonAudioFocus(int32_t clientId, const AudioInterrupt &audioInterrupt) = 0;

    virtual int32_t GetStreamInFocus( int32_t zoneID, int32_t &streamType) = 0;

    virtual int32_t GetStreamInFocusByUid(
        int32_t uid, int32_t zoneID, int32_t &streamType) = 0;

    virtual int32_t GetSessionInfoInFocus(AudioInterrupt &audioInterrupt, int32_t zoneID) = 0;

    virtual int32_t ReconfigureAudioChannel(uint32_t &count, int32_t deviceType) = 0;

    virtual int32_t GetPreferredOutputStreamType(AudioRendererInfo &rendererInfo) = 0;

    virtual int32_t GetPreferredInputStreamType(AudioCapturerInfo &capturerInfo) = 0;

    virtual int32_t CreateRendererClient(
        const std::shared_ptr<AudioStreamDescriptor> &streamDesc, uint32_t &flag, uint32_t &sessionId) = 0;

    virtual int32_t CreateCapturerClient(
        const std::shared_ptr<AudioStreamDescriptor> &streamDesc, uint32_t &flag, uint32_t &sessionId) = 0;

    virtual int32_t RegisterTracker(AudioMode &mode,
        AudioStreamChangeInfo &streamChangeInfo, const sptr<IRemoteObject> &object) = 0;

    virtual int32_t UpdateTracker(AudioMode &mode, AudioStreamChangeInfo &streamChangeInfo) = 0;

    virtual int32_t GetCurrentRendererChangeInfos(
        std::vector<std::shared_ptr<AudioRendererChangeInfo>> &audioRendererChangeInfos) = 0;

    virtual int32_t GetCurrentCapturerChangeInfos(
        std::vector<std::shared_ptr<AudioCapturerChangeInfo>> &audioCapturerChangeInfos) = 0;

    virtual int32_t UpdateStreamState(int32_t clientUid, int32_t streamSetState, int32_t streamUsage) = 0;

    virtual int32_t SelectOutputDevice(sptr<AudioRendererFilter> audioRendererFilter,
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptors) = 0;

    virtual int32_t GetSelectedDeviceInfo(int32_t uid, int32_t pid, int32_t streamType, std::string &info) = 0;

    virtual int32_t SelectInputDevice(const sptr<AudioCapturerFilter> &audioCapturerFilter,
        const std::vector<std::shared_ptr<AudioDeviceDescriptor>> &audioDeviceDescriptors) = 0;

    virtual int32_t ExcludeOutputDevices(int32_t audioDevUsage,
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> &audioDeviceDescriptors) = 0;

    virtual int32_t UnexcludeOutputDevices(int32_t audioDevUsage,
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> &audioDeviceDescriptors) = 0;

    virtual int32_t GetExcludedDevices(int32_t audioDevUsage,
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> &audioDeviceDescriptors) = 0;

    virtual int32_t GetVolumeGroupInfos(const std::string networkId, std::vector<sptr<VolumeGroupInfo>> &infos) = 0;

    virtual int32_t GetNetworkIdByGroupId(int32_t groupId, std::string &networkId) = 0;

    virtual int32_t GetPreferredOutputDeviceDescriptors(AudioRendererInfo &rendererInfo, bool forceNoBTPermission,
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> &deviceDescs) = 0;

    virtual int32_t GetPreferredInputDeviceDescriptors(
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> &deviceDescs) = 0;

    virtual int32_t SetClientCallbacksEnable(const CallbackChange &callbackchange, bool &enable) = 0;

    virtual int32_t SetCallbackRendererInfo(const AudioRendererInfo &rendererInfo) = 0;

    virtual int32_t SetCallbackCapturerInfo(const AudioCapturerInfo &capturerInfo) = 0;

    virtual int32_t GetAudioFocusInfoList(std::vector<std::map<AudioInterrupt, int32_t>> &focusInfoList,
        int32_t zoneID) = 0;

    virtual int32_t SetSystemSoundUri(const std::string &key, const std::string &uri) = 0;

    virtual int32_t GetSystemSoundUri(const std::string &key, std::string &uri) = 0;

    virtual int32_t GetMinStreamVolume(float &volume) = 0;

    virtual int32_t GetMaxStreamVolume(float &volume) = 0;

    virtual int32_t GetMaxRendererInstances() = 0;

    virtual int32_t IsVolumeUnadjustable(bool &unadjustable) = 0;

    virtual int32_t AdjustVolumeByStep(int32_t adjustType) = 0;

    virtual int32_t AdjustSystemVolumeByStep(int32_t volumeType, int32_t adjustType) = 0;

    virtual int32_t GetSystemVolumeInDb(int32_t volumeType, int32_t volumeLevel,
        int32_t deviceType, float &volume) = 0;

    virtual int32_t QueryEffectSceneMode(SupportedEffectConfig &supportedEffectConfig) = 0;

    virtual int32_t GetHardwareOutputSamplingRate(const std::shared_ptr<AudioDeviceDescriptor> &desc) = 0;

    virtual int32_t GetAudioCapturerMicrophoneDescriptors(int32_t sessionId,
        vector<sptr<MicrophoneDescriptor>> &micDescs) = 0;

    virtual int32_t GetAvailableMicrophones(std::vector<sptr<MicrophoneDescriptor>> &retMicList) = 0;

    virtual int32_t SetDeviceAbsVolumeSupported(const std::string &macAddress, bool support) = 0;

    virtual int32_t IsAbsVolumeScene(bool &ret) = 0;

    virtual int32_t SetA2dpDeviceVolume(const std::string &macAddress, int32_t volume, bool updateUi) = 0;

    virtual int32_t SetNearlinkDeviceVolume(const std::string &macAddress, int32_t volumeType,
        int32_t volume, bool updateUi) = 0;

    virtual int32_t GetAvailableDevices(AudioDeviceUsage usage,
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> &descs) = 0;

    virtual int32_t SetAvailableDeviceChangeCallback(int32_t clientId, int32_t usage,
        const sptr<IRemoteObject> &object) = 0;

    virtual int32_t UnsetAvailableDeviceChangeCallback(int32_t clientId, int32_t usage) = 0;

    virtual int32_t ConfigDistributedRoutingRole(
        const std::shared_ptr<AudioDeviceDescriptor> descriptor, int32_t type) = 0;

    virtual int32_t SetDistributedRoutingRoleCallback(const sptr<IRemoteObject> &object) = 0;

    virtual int32_t UnsetDistributedRoutingRoleCallback() = 0;

    virtual int32_t IsSpatializationEnabled(bool &ret) = 0;

    virtual int32_t IsSpatializationEnabled(const std::string address, bool &ret) = 0;

    virtual int32_t IsSpatializationEnabledForCurrentDevice() = 0;

    virtual int32_t SetSpatializationEnabled(bool enable) = 0;

    virtual int32_t SetSpatializationEnabled(const std::shared_ptr<AudioDeviceDescriptor> &selectedAudioDevice,
        bool enable) = 0;

    virtual int32_t IsHeadTrackingEnabled(bool &ret) = 0;

    virtual int32_t IsHeadTrackingEnabled(const std::string address, bool &ret) = 0;

    virtual int32_t SetHeadTrackingEnabled(bool enable) = 0;

    virtual int32_t SetHeadTrackingEnabled(const std::shared_ptr<AudioDeviceDescriptor> &selectedAudioDevice,
        bool enable) = 0;

    virtual int32_t GetSpatializationState(int32_t streamUsage, AudioSpatializationState &state) = 0;

    virtual int32_t IsSpatializationSupported(bool &ret) = 0;

    virtual int32_t IsSpatializationSupportedForDevice(const std::string &address, bool &ret) = 0;

    virtual int32_t IsHeadTrackingSupported(bool &ret) = 0;

    virtual int32_t IsHeadTrackingSupportedForDevice(const std::string address, bool &ret) = 0;

    virtual int32_t UpdateSpatialDeviceState(const AudioSpatialDeviceState &audioSpatialDeviceState) = 0;

    virtual int32_t RegisterSpatializationStateEventListener(uint32_t sessionID, int32_t streamUsage,
        const sptr<IRemoteObject> &object) = 0;

    virtual int32_t UnregisterSpatializationStateEventListener(uint32_t sessionID) = 0;

    virtual int32_t RegisterPolicyCallbackClient(const sptr<IRemoteObject> &object,
        int32_t zoneID) = 0;

    virtual int32_t CreateAudioInterruptZone(const std::set<int32_t> &pids,
        int32_t zoneID) = 0;

    virtual int32_t AddAudioInterruptZonePids(const std::set<int32_t> &pids,
        int32_t zoneID) = 0;

    virtual int32_t RemoveAudioInterruptZonePids(const std::set<int32_t> &pids,
        int32_t zoneID) = 0;

    virtual int32_t ReleaseAudioInterruptZone(int32_t zoneID) = 0;

    virtual int32_t RegisterAudioZoneClient(const sptr<IRemoteObject>& object) = 0;

    virtual int32_t CreateAudioZone(const std::string &name, const AudioZoneContext &context) = 0;

    virtual int32_t ReleaseAudioZone(int32_t zoneId) = 0;

    virtual int32_t GetAllAudioZone(std::vector<std::shared_ptr<AudioZoneDescriptor>> &descs) = 0;

    virtual int32_t GetAudioZone(int32_t zoneId, std::shared_ptr<AudioZoneDescriptor> &desc) = 0;

    virtual int32_t BindDeviceToAudioZone(int32_t zoneId,
        const std::vector<std::shared_ptr<AudioDeviceDescriptor>> &devices) = 0;

    virtual int32_t UnBindDeviceToAudioZone(int32_t zoneId,
        const std::vector<std::shared_ptr<AudioDeviceDescriptor>> &devices) = 0;

    virtual int32_t EnableAudioZoneReport(bool enable) = 0;

    virtual int32_t EnableAudioZoneChangeReport(int32_t zoneId, bool enable) = 0;

    virtual int32_t AddUidToAudioZone(int32_t zoneId, int32_t uid) = 0;

    virtual int32_t RemoveUidFromAudioZone(int32_t zoneId, int32_t uid) = 0;

    virtual int32_t EnableSystemVolumeProxy(int32_t zoneId, bool enable) = 0;

    virtual int32_t GetAudioInterruptForZone(int32_t zoneId,
        std::vector<std::map<AudioInterrupt, int32_t>> &retList) = 0;

    virtual int32_t GetAudioInterruptForZone(
        int32_t zoneId, const std::string &deviceTag, std::vector<std::map<AudioInterrupt, int32_t>> &retList) = 0;

    virtual int32_t EnableAudioZoneInterruptReport(int32_t zoneId, const std::string &deviceTag, bool enable) = 0;

    virtual int32_t InjectInterruptToAudioZone(int32_t zoneId,
        const std::vector<std::map<AudioInterrupt, int32_t>> &interrupts) = 0;
    
    virtual int32_t InjectInterruptToAudioZone(int32_t zoneId, const std::string &deviceTag,
        const std::vector<std::map<AudioInterrupt, int32_t>> &interrupts) = 0;

    virtual int32_t SetCallDeviceActive(InternalDeviceType deviceType, bool active, std::string address,
        int32_t uid) = 0;

    virtual int32_t GetActiveBluetoothDevice(std::shared_ptr<AudioDeviceDescriptor> &descs) = 0;

    virtual int32_t GetConverterConfig(ConverterConfig &config) = 0;

    virtual int32_t FetchOutputDeviceForTrack(AudioStreamChangeInfo &streamChangeInfo,
        const AudioStreamDeviceChangeReasonExt &reason) = 0;

    virtual int32_t FetchInputDeviceForTrack(AudioStreamChangeInfo &streamChangeInfo) = 0;

    virtual int32_t IsHighResolutionExist(bool& ret) = 0;

    virtual int32_t SetHighResolutionExist(bool highResExist) = 0;

    virtual int32_t GetSpatializationSceneType(int32_t &type) = 0;

    virtual int32_t SetSpatializationSceneType(const AudioSpatializationSceneType spatializationSceneType) = 0;

    virtual int32_t GetMaxAmplitude(int32_t deviceId, float &ret) = 0;

    virtual int32_t DisableSafeMediaVolume() = 0;

    virtual int32_t IsHeadTrackingDataRequested(const std::string &macAddress, bool& ret) = 0;

    virtual int32_t SetAudioDeviceRefinerCallback(const sptr<IRemoteObject> &object) = 0;

    virtual int32_t UnsetAudioDeviceRefinerCallback() = 0;

    virtual int32_t TriggerFetchDevice(AudioStreamDeviceChangeReasonExt reason) = 0;

    virtual int32_t SetPreferredDevice(const PreferredType preferredType,
        const std::shared_ptr<AudioDeviceDescriptor> &desc, int32_t uid = INVALID_UID) = 0;

    virtual int32_t SetAudioDeviceAnahsCallback(const sptr<IRemoteObject> &object) = 0;

    virtual int32_t UnsetAudioDeviceAnahsCallback() = 0;

    virtual int32_t SetAudioConcurrencyCallback(uint32_t sessionID, const sptr<IRemoteObject> &object) = 0;

    virtual int32_t UnsetAudioConcurrencyCallback(uint32_t sessionID) = 0;

    virtual int32_t ActivateAudioConcurrency(const AudioPipeType &pipeType) = 0;

    virtual int32_t MoveToNewPipe(uint32_t sessionId, const AudioPipeType pipeType) = 0;

    virtual int32_t InjectInterruption(const std::string networkId, InterruptEvent &event) = 0;

    virtual int32_t LoadSplitModule(const std::string &splitArgs, const std::string &networkId) = 0;

    virtual int32_t SetInputDevice(int32_t deviceType, uint32_t session_ID,
        int32_t sourceType, bool isRunning) = 0;

    virtual bool IsAllowedPlayback(int32_t &uid, int32_t &pid) = 0;

    virtual int32_t SetVoiceRingtoneMute(bool isMute) = 0;

    virtual int32_t NotifySessionStateChange(int32_t uid, int32_t pid, bool hasSession) = 0;

    virtual int32_t NotifyFreezeStateChange(const std::set<int32_t> &pidList, bool isFreeze) = 0;

    virtual int32_t ResetAllProxy() = 0;

    virtual int32_t SaveRemoteInfo(const std::string &networkId, int32_t deviceType) = 0;

    virtual int32_t SetDeviceConnectionStatus(const std::shared_ptr<AudioDeviceDescriptor> &desc,
        bool isConnected) = 0;

    virtual int32_t GetSupportedAudioEffectProperty(AudioEffectPropertyArrayV3 &propertyArray) = 0;

    virtual int32_t SetAudioEffectProperty(const AudioEffectPropertyArrayV3 &propertyArray) = 0;

    virtual int32_t GetAudioEffectProperty(AudioEffectPropertyArrayV3 &propertyArray) = 0;

    virtual int32_t GetSupportedAudioEffectProperty(AudioEffectPropertyArray &propertyArray) = 0;

    virtual int32_t GetSupportedAudioEnhanceProperty(AudioEnhancePropertyArray &propertyArray) = 0;

    virtual int32_t SetAudioEffectProperty(const AudioEffectPropertyArray &propertyArray) = 0;

    virtual int32_t GetAudioEffectProperty(AudioEffectPropertyArray &propertyArray) = 0;

    virtual int32_t SetAudioEnhanceProperty(const AudioEnhancePropertyArray &propertyArray) = 0;

    virtual int32_t GetAudioEnhanceProperty(AudioEnhancePropertyArray &propertyArray) = 0;

    virtual int32_t SetVirtualCall(bool isVirtual) = 0;

    virtual int32_t SetQueryAllowedPlaybackCallback(const sptr<IRemoteObject> &object) = 0;

    virtual int32_t SetBackgroundMuteCallback(const sptr<IRemoteObject> &object) = 0;

    virtual int32_t GetDirectPlaybackSupport(const AudioStreamInfo &streamInfo,
        int32_t streamUsage, int32_t &retMod) = 0;
    
    virtual int32_t IsAcousticEchoCancelerSupported(int32_t sourceType, bool &ret) = 0;

    virtual int32_t ForceStopAudioStream(StopAudioType audioType) = 0;

    virtual int32_t IsCapturerFocusAvailable(const AudioCapturerChangeInfo &capturerInfo, bool &ret) = 0;

    virtual int32_t GetMaxVolumeLevelByUsage(int32_t streamUsage) = 0;

    virtual int32_t GetMinVolumeLevelByUsage(int32_t streamUsage) = 0;

    virtual int32_t GetVolumeLevelByUsage(int32_t streamUsage) = 0;

    virtual int32_t GetStreamMuteByUsage(int32_t streamUsage, bool &isMute) = 0;

    virtual int32_t SetCallbackStreamUsageInfo(const std::set<int32_t> &streamUsages) = 0;

    virtual int32_t UpdateDeviceInfo(const std::shared_ptr<AudioDeviceDescriptor> &deviceDesc,
        const DeviceInfoUpdateCommand command) = 0;
    virtual int32_t SetSleAudioOperationCallback(const sptr<IRemoteObject> &object) = 0;
public:
    DECLARE_INTERFACE_DESCRIPTOR(u"IAudioPolicy");
};
} // namespace AudioStandard
} // namespace OHOS
#endif // I_AUDIO_POLICY_BASE_H
