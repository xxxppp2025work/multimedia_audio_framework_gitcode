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

#ifndef AUDIO_POLICY_MANAGER_STUB_H
#define AUDIO_POLICY_MANAGER_STUB_H

#include "audio_policy_base.h"

namespace OHOS {
namespace AudioStandard {
const char *g_audioPolicyCodeStrs[] = {
    "GET_MAX_VOLUMELEVEL",
    "GET_MIN_VOLUMELEVEL",
    "SET_SYSTEM_VOLUMELEVEL_LEGACY",
    "SET_SYSTEM_VOLUMELEVEL",
    "SET_APP_VOLUMELEVEL",
    "SET_APP_VOLUME_MUTED",
    "IS_APP_MUTE",
    "SET_SELF_APP_VOLUMELEVEL",
    "SET_SYSTEM_VOLUMELEVEL_WITH_DEVICE",
    "GET_SYSTEM_VOLUMELEVEL",
    "GET_APP_VOLUMELEVEL",
    "GET_SELF_APP_VOLUME_LEVEL",
    "SET_STREAM_MUTE_LEGACY",
    "SET_STREAM_MUTE",
    "GET_STREAM_MUTE",
    "IS_STREAM_ACTIVE",
    "SET_DEVICE_ACTIVE",
    "IS_DEVICE_ACTIVE",
    "GET_ACTIVE_OUTPUT_DEVICE",
    "GET_ACTIVE_INPUT_DEVICE",
    "SET_RINGER_MODE_LEGACY",
    "SET_RINGER_MODE",
    "GET_RINGER_MODE",
    "SET_AUDIO_SCENE",
    "GET_AUDIO_SCENE",
    "SET_MICROPHONE_MUTE",
    "SET_MICROPHONE_MUTE_AUDIO_CONFIG",
    "IS_MICROPHONE_MUTE_LEGACY",
    "IS_MICROPHONE_MUTE",
    "SET_CALLBACK",
    "UNSET_CALLBACK",
    "SET_QUERY_CLIENT_TYPE_CALLBACK",
    "SET_CLIENT_INFO_MGR_CALLBACK",
    "SET_QUERY_BUNDLE_NAME_LIST_CALLBACK",
    "ACTIVATE_INTERRUPT",
    "DEACTIVATE_INTERRUPT",
    "SET_INTERRUPT_CALLBACK",
    "UNSET_INTERRUPT_CALLBACK",
    "REQUEST_AUDIO_FOCUS",
    "ABANDON_AUDIO_FOCUS",
    "GET_STREAM_IN_FOCUS",
    "GET_SESSION_INFO_IN_FOCUS",
    "GET_DEVICES",
    "SELECT_OUTPUT_DEVICE",
    "GET_SELECTED_DEVICE_INFO",
    "SELECT_INPUT_DEVICE",
    "RECONFIGURE_CHANNEL",
    "GET_AUDIO_LATENCY",
    "GET_SINK_LATENCY",
    "GET_PREFERRED_OUTPUT_STREAM_TYPE",
    "GET_PREFERRED_INPUT_STREAM_TYPE",
    "CREATE_RENDERER_CLIENT",
    "CREATE_CAPTURER_CLIENT",
    "REGISTER_TRACKER",
    "UPDATE_TRACKER",
    "GET_RENDERER_CHANGE_INFOS",
    "GET_CAPTURER_CHANGE_INFOS",
    "SET_LOW_POWER_STREM_VOLUME",
    "GET_LOW_POWRR_STREM_VOLUME",
    "UPDATE_STREAM_STATE",
    "GET_SINGLE_STREAM_VOLUME",
    "GET_VOLUME_GROUP_INFO",
    "GET_NETWORKID_BY_GROUP_ID",
#ifdef FEATURE_DTMF_TONE
    "GET_TONEINFO",
    "GET_SUPPORTED_TONES",
#endif
    "IS_AUDIO_RENDER_LOW_LATENCY_SUPPORTED",
    "GET_ACTIVE_OUTPUT_DEVICE_DESCRIPTORS",
    "GET_PREFERRED_INTPUT_DEVICE_DESCRIPTORS",
    "SET_CALLBACKS_ENABLE",
    "GET_AUDIO_FOCUS_INFO_LIST",
    "SET_SYSTEM_SOUND_URI",
    "GET_SYSTEM_SOUND_URI",
    "GET_MIN_VOLUME_STREAM",
    "GET_MAX_VOLUME_STREAM",
    "GET_MAX_RENDERER_INSTANCES",
    "IS_VOLUME_UNADJUSTABLE",
    "ADJUST_VOLUME_BY_STEP",
    "ADJUST_SYSTEM_VOLUME_BY_STEP",
    "GET_SYSTEM_VOLUME_IN_DB",
    "QUERY_EFFECT_SCENEMODE",
    "GET_HARDWARE_OUTPUT_SAMPLING_RATE",
    "GET_AUDIO_CAPTURER_MICROPHONE_DESCRIPTORS",
    "GET_AVAILABLE_MICROPHONE_DESCRIPTORS",
    "SET_DEVICE_ABSOLUTE_VOLUME_SUPPORTED",
    "GET_ABS_VOLUME_SCENE",
    "SET_A2DP_DEVICE_VOLUME",
    "GET_AVAILABLE_DESCRIPTORS",
    "SET_AVAILABLE_DEVICE_CHANGE_CALLBACK",
    "UNSET_AVAILABLE_DEVICE_CHANGE_CALLBACK",
    "IS_SPATIALIZATION_ENABLED",
    "IS_SPATIALIZATION_ENABLED_FOR_DEVICE",
    "SET_SPATIALIZATION_ENABLED",
    "SET_SPATIALIZATION_ENABLED_FOR_DEVICE",
    "IS_HEAD_TRACKING_ENABLED",
    "IS_HEAD_TRACKING_ENABLED_FOR_DEVICE",
    "SET_HEAD_TRACKING_ENABLED",
    "SET_HEAD_TRACKING_ENABLED_FOR_DEVICE",
    "GET_SPATIALIZATION_STATE",
    "IS_SPATIALIZATION_SUPPORTED",
    "IS_SPATIALIZATION_SUPPORTED_FOR_DEVICE",
    "IS_HEAD_TRACKING_SUPPORTED",
    "IS_HEAD_TRACKING_SUPPORTED_FOR_DEVICE",
    "UPDATE_SPATIAL_DEVICE_STATE",
    "REGISTER_SPATIALIZATION_STATE_EVENT",
    "CONFIG_DISTRIBUTED_ROUTING_ROLE",
    "SET_DISTRIBUTED_ROUTING_ROLE_CALLBACK",
    "UNSET_DISTRIBUTED_ROUTING_ROLE_CALLBACK",
    "UNREGISTER_SPATIALIZATION_STATE_EVENT",
    "REGISTER_POLICY_CALLBACK_CLIENT",
    "CREATE_AUDIO_INTERRUPT_ZONE",
    "ADD_AUDIO_INTERRUPT_ZONE_PIDS",
    "REMOVE_AUDIO_INTERRUPT_ZONE_PIDS",
    "RELEASE_AUDIO_INTERRUPT_ZONE",
    "REGISTER_AUDIO_ZONE_CLIENT",
    "CREATE_AUDIO_ZONE",
    "RELEASE_AUDIO_ZONE",
    "GET_ALL_AUDIO_ZONE",
    "GET_AUDIO_ZONE_BY_ID",
    "BIND_AUDIO_ZONE_DEVICE",
    "UNBIND_AUDIO_ZONE_DEVICE",
    "ENABLE_AUDIO_ZONE_REPORT",
    "ENABLE_AUDIO_ZONE_CHANGE_REPORT",
    "ADD_UID_TO_AUDIO_ZONE",
    "REMOVE_UID_FROM_AUDIO_ZONE",
    "ENABLE_SYSTEM_VOLUME_PROXY",
    "SET_SYSTEM_VOLUME_LEVEL_FOR_ZONE",
    "GET_SYSTEM_VOLUME_LEVEL_FOR_ZONE",
    "GET_AUDIO_INTERRUPT_FOR_ZONE",
    "GET_AUDIO_INTERRUPT_OF_DEVICE_FOR_ZONE",
    "ENABLE_AUDIO_ZONE_INTERRUPT_REPORT",
    "INJECT_INTERRUPT_TO_AUDIO_ZONE",
    "INJECT_INTERRUPT_OF_DEVICE_TO_AUDIO_ZONE",
    "SET_CALL_DEVICE_ACTIVE",
    "GET_AUDIO_CONVERTER_CONFIG",
    "GET_ACTIVE_BLUETOOTH_DESCRIPTOR",
    "FETCH_OUTPUT_DEVICE_FOR_TRACK",
    "FETCH_INPUT_DEVICE_FOR_TRACK",
    "IS_HIGH_RESOLUTION_EXIST",
    "SET_HIGH_RESOLUTION_EXIST",
    "GET_SPATIALIZATION_SCENE_TYPE",
    "SET_SPATIALIZATION_SCENE_TYPE",
    "GET_MAX_AMPLITUDE",
    "IS_HEAD_TRACKING_DATA_REQUESTED",
    "SET_AUDIO_DEVICE_REFINER_CALLBACK",
    "UNSET_AUDIO_DEVICE_REFINER_CALLBACK",
    "TRIGGER_FETCH_DEVICE",
    "MOVE_TO_NEW_PIPE",
    "DISABLE_SAFE_MEDIA_VOLUME",
    "GET_DEVICES_INNER",
    "SET_AUDIO_CONCURRENCY_CALLBACK",
    "UNSET_AUDIO_CONCURRENCY_CALLBACK",
    "ACTIVATE_AUDIO_CONCURRENCY",
    "SET_MICROPHONE_MUTE_PERSISTENT",
    "GET_MICROPHONE_MUTE_PERSISTENT",
    "GET_SUPPORT_AUDIO_EFFECT_PROPERTY_V3",
    "GET_AUDIO_EFFECT_PROPERTY_V3",
    "SET_AUDIO_EFFECT_PROPERTY_V3",
    "GET_SUPPORT_AUDIO_ENHANCE_PROPERTY",
    "GET_SUPPORT_AUDIO_EFFECT_PROPERTY",
    "GET_AUDIO_ENHANCE_PROPERTY",
    "GET_AUDIO_EFFECT_PROPERTY",
    "SET_AUDIO_ENHANCE_PROPERTY",
    "SET_AUDIO_EFFECT_PROPERTY",
    "INJECT_INTERRUPTION",
    "ACTIVATE_AUDIO_SESSION",
    "DEACTIVATE_AUDIO_SESSION",
    "IS_AUDIO_SESSION_ACTIVATED",
    "LOAD_SPLIT_MODULE",
    "SET_DEFAULT_OUTPUT_DEVICE",
    "GET_SYSTEM_ACTIVEVOLUME_TYPE",
    "GET_OUTPUT_DEVICE",
    "GET_INPUT_DEVICE",
    "SET_AUDIO_DEVICE_ANAHS_CALLBACK",
    "UNSET_AUDIO_DEVICE_ANAHS_CALLBACK",
    "IS_ALLOWED_PLAYBACK",
    "SET_VOICE_RINGTONE_MUTE",
    "SET_CALLBACK_RENDERER_INFO",
    "SET_CALLBACK_CAPTURER_INFO",
    "GET_STREAM_IN_FOCUS_BY_UID",
    "SET_PREFERRED_DEVICE",
    "SAVE_REMOTE_INFO",
    "SET_VIRTUAL_CALL",
    "SET_DEVICE_CONNECTION_STATUS",
    "EXCLUDE_OUTPUT_DEVICES",
    "UNEXCLUDE_OUTPUT_DEVICES",
    "GET_EXCLUDED_OUTPUT_DEVICES",
    "IS_SPATIALIZATION_ENABLED_FOR_CURRENT_DEVICE",
    "SET_QUERY_ALLOWED_PLAYBACK_CALLBACK",
};

class AudioPolicyManagerStub : public IRemoteStub<IAudioPolicy> {
public:
    virtual int32_t OnRemoteRequest(uint32_t code, MessageParcel &data,
        MessageParcel &reply, MessageOption &option) override;
    virtual bool IsArmUsbDevice(const AudioDeviceDescriptor &desc) = 0;
    virtual void MapExternalToInternalDeviceType(AudioDeviceDescriptor &desc) = 0;

private:
    void GetMaxVolumeLevelInternal(MessageParcel &data, MessageParcel &reply);
    void GetMinVolumeLevelInternal(MessageParcel &data, MessageParcel &reply);
    void SetSystemVolumeLevelLegacyInternal(MessageParcel &data, MessageParcel &reply);
    void SetSystemVolumeLevelInternal(MessageParcel &data, MessageParcel &reply);
    void SetSystemVolumeLevelWithDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void SetAppVolumeLevelInternal(MessageParcel &data, MessageParcel &reply);
    void SetAppVolumeMutedInternal(MessageParcel &data, MessageParcel &reply);
    void SetSelfAppVolumeLevelInternal(MessageParcel &data, MessageParcel &reply);
    void GetSystemActiveVolumeTypeInternal(MessageParcel& data, MessageParcel& reply);
    void GetSystemVolumeLevelInternal(MessageParcel &data, MessageParcel &reply);
    void GetAppVolumeIsMuteInternal(MessageParcel &data, MessageParcel &reply);
    void GetAppVolumeLevelInternal(MessageParcel &data, MessageParcel &reply);
    void GetSelfAppVolumeLevelInternal(MessageParcel &data, MessageParcel &reply);
    void SetStreamMuteLegacyInternal(MessageParcel &data, MessageParcel &reply);
    void SetStreamMuteInternal(MessageParcel &data, MessageParcel &reply);
    void GetStreamMuteInternal(MessageParcel &data, MessageParcel &reply);
    void IsStreamActiveInternal(MessageParcel &data, MessageParcel &reply);
    void SetDeviceActiveInternal(MessageParcel &data, MessageParcel &reply);
    void IsDeviceActiveInternal(MessageParcel &data, MessageParcel &reply);
    void GetActiveOutputDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void GetActiveInputDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void GetOutputDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void GetInputDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void SetRingerModeLegacyInternal(MessageParcel &data, MessageParcel &reply);
    void SetRingerModeInternal(MessageParcel &data, MessageParcel &reply);
    void GetRingerModeInternal(MessageParcel &data, MessageParcel &reply);
    void SetAudioSceneInternal(MessageParcel &data, MessageParcel &reply);
    void GetAudioSceneInternal(MessageParcel &data, MessageParcel &reply);
    void SetMicrophoneMuteInternal(MessageParcel &data, MessageParcel &reply);
    void SetMicrophoneMuteAudioConfigInternal(MessageParcel &data, MessageParcel &reply);
    void IsMicrophoneMuteLegacyInternal(MessageParcel &data, MessageParcel &reply);
    void IsMicrophoneMuteInternal(MessageParcel &data, MessageParcel &reply);
    void SetInterruptCallbackInternal(MessageParcel &data, MessageParcel &reply);
    void UnsetInterruptCallbackInternal(MessageParcel &data, MessageParcel &reply);
    void ActivateInterruptInternal(MessageParcel &data, MessageParcel &reply);
    void DeactivateInterruptInternal(MessageParcel &data, MessageParcel &reply);
    void SetAudioManagerInterruptCbInternal(MessageParcel &data, MessageParcel &reply);
    void UnsetAudioManagerInterruptCbInternal(MessageParcel &data, MessageParcel &reply);
    void RequestAudioFocusInternal(MessageParcel &data, MessageParcel &reply);
    void AbandonAudioFocusInternal(MessageParcel &data, MessageParcel &reply);
    void GetStreamInFocusInternal(MessageParcel &data, MessageParcel &reply);
    void GetStreamInFocusByUidInternal(MessageParcel &data, MessageParcel &reply);
    void GetSessionInfoInFocusInternal(MessageParcel &data, MessageParcel &reply);
    void GetDevicesInternal(MessageParcel &data, MessageParcel &reply);
    void SelectOutputDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void GetSelectedDeviceInfoInternal(MessageParcel &data, MessageParcel &reply);
    void SelectInputDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void ExcludeOutputDevicesInternal(MessageParcel &data, MessageParcel &reply);
    void UnexcludeOutputDevicesInternal(MessageParcel &data, MessageParcel &reply);
    void GetExcludedDevicesInternal(MessageParcel &data, MessageParcel &reply);
    void ReconfigureAudioChannelInternal(MessageParcel &data, MessageParcel &reply);
    void GetPreferredOutputStreamTypeInternal(MessageParcel &data, MessageParcel &reply);
    void GetPreferredInputStreamTypeInternal(MessageParcel &data, MessageParcel &reply);
    void CreateRendererClientInternal(MessageParcel &data, MessageParcel &reply);
    void CreateCapturerClientInternal(MessageParcel &data, MessageParcel &reply);
    void RegisterTrackerInternal(MessageParcel &data, MessageParcel &reply);
    void UpdateTrackerInternal(MessageParcel &data, MessageParcel &reply);
    void GetRendererChangeInfosInternal(MessageParcel &data, MessageParcel &reply);
    void GetCapturerChangeInfosInternal(MessageParcel &data, MessageParcel &reply);
    void SetLowPowerVolumeInternal(MessageParcel &data, MessageParcel &reply);
    void GetLowPowerVolumeInternal(MessageParcel &data, MessageParcel &reply);
    void UpdateStreamStateInternal(MessageParcel& data, MessageParcel& reply);
    void GetSingleStreamVolumeInternal(MessageParcel &data, MessageParcel &reply);
    void GetVolumeGroupInfoInternal(MessageParcel& data, MessageParcel& reply);
    void GetNetworkIdByGroupIdInternal(MessageParcel& data, MessageParcel& reply);
#ifdef FEATURE_DTMF_TONE
    void GetToneInfoInternal(MessageParcel &data, MessageParcel &reply);
    void GetSupportedTonesInternal(MessageParcel &data, MessageParcel &reply);
#endif
    void GetPreferredOutputDeviceDescriptorsInternal(MessageParcel &data, MessageParcel &reply);
    void GetPreferredInputDeviceDescriptorsInternal(MessageParcel &data, MessageParcel &reply);
    void SetClientCallbacksEnableInternal(MessageParcel &data, MessageParcel &reply);
    void SetCallbackRendererInfoInternal(MessageParcel &data, MessageParcel &reply);
    void SetCallbackCapturerInfoInternal(MessageParcel &data, MessageParcel &reply);
    void GetAudioFocusInfoListInternal(MessageParcel &data, MessageParcel &reply);
    void SetSystemSoundUriInternal(MessageParcel &data, MessageParcel &reply);
    void GetSystemSoundUriInternal(MessageParcel &data, MessageParcel &reply);
    void GetMinStreamVolumeInternal(MessageParcel &data, MessageParcel &reply);
    void GetMaxStreamVolumeInternal(MessageParcel &data, MessageParcel &reply);
    void GetMaxRendererInstancesInternal(MessageParcel &data, MessageParcel &reply);
    void IsVolumeUnadjustableInternal(MessageParcel &data, MessageParcel &reply);
    void AdjustVolumeByStepInternal(MessageParcel &data, MessageParcel &reply);
    void AdjustSystemVolumeByStepInternal(MessageParcel &data, MessageParcel &reply);
    void GetSystemVolumeInDbInternal(MessageParcel &data, MessageParcel &reply);
    void QueryEffectSceneModeInternal(MessageParcel &data, MessageParcel &reply);
    void GetHardwareOutputSamplingRateInternal(MessageParcel &data, MessageParcel &reply);
    void GetAudioCapturerMicrophoneDescriptorsInternal(MessageParcel &data, MessageParcel &reply);
    void GetAvailableMicrophonesInternal(MessageParcel &data, MessageParcel &reply);
    void SetDeviceAbsVolumeSupportedInternal(MessageParcel &data, MessageParcel &reply);
    void IsAbsVolumeSceneInternal(MessageParcel &data, MessageParcel &reply);
    void SetA2dpDeviceVolumeInternal(MessageParcel &data, MessageParcel &reply);
    void ReadStreamChangeInfo(MessageParcel &data, const AudioMode &mode, AudioStreamChangeInfo &streamChangeInfo);
    void WriteAudioFocusInfo(MessageParcel &data,
        const std::pair<AudioInterrupt, AudioFocuState> &focusInfo);
    void GetAvailableDevicesInternal(MessageParcel &data, MessageParcel &reply);
    void SetAvailableDeviceChangeCallbackInternal(MessageParcel &data, MessageParcel &reply);
    void UnsetAvailableDeviceChangeCallbackInternal(MessageParcel &data, MessageParcel &reply);
    void IsSpatializationEnabledInternal(MessageParcel &data, MessageParcel &reply);
    void IsSpatializationEnabledForDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void IsSpatializationEnabledForCurrentDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void SetSpatializationEnabledInternal(MessageParcel &data, MessageParcel &reply);
    void SetSpatializationEnabledForDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void IsHeadTrackingEnabledInternal(MessageParcel &data, MessageParcel &reply);
    void IsHeadTrackingEnabledForDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void SetHeadTrackingEnabledInternal(MessageParcel &data, MessageParcel &reply);
    void SetHeadTrackingEnabledForDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void GetSpatializationStateInternal(MessageParcel &data, MessageParcel &reply);
    void IsSpatializationSupportedInternal(MessageParcel &data, MessageParcel &reply);
    void IsSpatializationSupportedForDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void IsHeadTrackingSupportedInternal(MessageParcel &data, MessageParcel &reply);
    void IsHeadTrackingSupportedForDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void UpdateSpatialDeviceStateInternal(MessageParcel &data, MessageParcel &reply);
    void RegisterSpatializationStateEventListenerInternal(MessageParcel &data, MessageParcel &reply);
    void ConfigDistributedRoutingRoleInternal(MessageParcel &data, MessageParcel &reply);
    void SetDistributedRoutingRoleCallbackInternal(MessageParcel &data, MessageParcel &reply);
    void UnsetDistributedRoutingRoleCallbackInternal(MessageParcel &data, MessageParcel &reply);
    void UnregisterSpatializationStateEventListenerInternal(MessageParcel &data, MessageParcel &reply);
    void RegisterPolicyCallbackClientInternal(MessageParcel &data, MessageParcel &reply);
    void CreateAudioInterruptZoneInternal(MessageParcel &data, MessageParcel &reply);
    void AddAudioInterruptZonePidsInternal(MessageParcel &data, MessageParcel &reply);
    void RemoveAudioInterruptZonePidsInternal(MessageParcel &data, MessageParcel &reply);
    void ReleaseAudioInterruptZoneInternal(MessageParcel &data, MessageParcel &reply);

    void HandleRegisterAudioZoneClient(MessageParcel &data, MessageParcel &reply);
    void HandleCreateAudioZone(MessageParcel &data, MessageParcel &reply);
    void HandleReleaseAudioZone(MessageParcel &data, MessageParcel &reply);
    void HandleGetAllAudioZone(MessageParcel &data, MessageParcel &reply);
    void HandleGetAudioZone(MessageParcel &data, MessageParcel &reply);
    void HandleBindAudioZoneDevice(MessageParcel &data, MessageParcel &reply);
    void HandleUnBindAudioZoneDevice(MessageParcel &data, MessageParcel &reply);
    void HandleEnableAudioZoneReport (MessageParcel &data, MessageParcel &reply);
    void HandleEnableAudioZoneChangeReport(MessageParcel &data, MessageParcel &reply);
    void HandleAddUidToAudioZone(MessageParcel &data, MessageParcel &reply);
    void HandleRemoveUidFromAudioZone(MessageParcel &data, MessageParcel &reply);
    void HandleEnableSystemVolumeProxy(MessageParcel &data, MessageParcel &reply);
    void HandleSetSystemVolumeLevelForZone(MessageParcel &data, MessageParcel &reply);
    void HandleGetSystemVolumeLevelForZone(MessageParcel &data, MessageParcel &reply);
    void HandleGetAudioInterruptForZone(MessageParcel &data, MessageParcel &reply);
    void HandleGetAudioInterruptForZoneDevice(MessageParcel &data, MessageParcel &reply);
    void HandleEnableAudioZoneInterruptReport(MessageParcel &data, MessageParcel &reply);
    void HandleInjectInterruptToAudioZone(MessageParcel &data, MessageParcel &reply);
    void HandleInjectInterruptToAudioZoneDevice(MessageParcel &data, MessageParcel &reply);

    void SetCallDeviceActiveInternal(MessageParcel &data, MessageParcel &reply);
    void GetConverterConfigInternal(MessageParcel &data, MessageParcel &reply);
    void GetActiveBluetoothDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void FetchOutputDeviceForTrackInternal(MessageParcel &data, MessageParcel &reply);
    void FetchInputDeviceForTrackInternal(MessageParcel &data, MessageParcel &reply);
    void IsHighResolutionExistInternal(MessageParcel &data, MessageParcel &reply);
    void SetHighResolutionExistInternal(MessageParcel &data, MessageParcel &reply);
    void GetSpatializationSceneTypeInternal(MessageParcel &data, MessageParcel &reply);
    void SetSpatializationSceneTypeInternal(MessageParcel &data, MessageParcel &reply);
    void GetMaxAmplitudeInternal(MessageParcel &data, MessageParcel &reply);
    void DisableSafeMediaVolumeInternal(MessageParcel &data, MessageParcel &reply);
    void IsHeadTrackingDataRequestedInternal(MessageParcel &data, MessageParcel &reply);
    void SetAudioDeviceRefinerCallbackInternal(MessageParcel &data, MessageParcel &reply);
    void UnsetAudioDeviceRefinerCallbackInternal(MessageParcel &data, MessageParcel &reply);
    void TriggerFetchDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void SetPreferredDeviceInternal(MessageParcel &data, MessageParcel &reply);
    void SaveRemoteInfoInternal(MessageParcel &data, MessageParcel &reply);
    void SetAudioDeviceAnahsCallbackInternal(MessageParcel &data, MessageParcel &reply);
    void UnsetAudioDeviceAnahsCallbackInternal(MessageParcel &data, MessageParcel &reply);
    void MoveToNewTypeInternal(MessageParcel &data, MessageParcel &reply);
    void GetDevicesInnerInternal(MessageParcel &data, MessageParcel &reply);
    void SetConcurrencyCallbackInternal(MessageParcel &data, MessageParcel &reply);
    void UnsetConcurrencyCallbackInternal(MessageParcel &data, MessageParcel &reply);
    void ActivateAudioConcurrencyInternal(MessageParcel &data, MessageParcel &reply);
    void SetRingerStreamMuteInternal(MessageParcel &data, MessageParcel &reply);
    void SetMicrophoneMutePersistentInternal(MessageParcel &data, MessageParcel &reply);
    void GetMicrophoneMutePersistentInternal(MessageParcel &data, MessageParcel &reply);
    void GetSupportedAudioEffectPropertyV3Internal(MessageParcel &data, MessageParcel &reply);
    void SetAudioEffectPropertyV3Internal(MessageParcel &data, MessageParcel &reply);
    void GetAudioEffectPropertyV3Internal(MessageParcel &data, MessageParcel &reply);
    void GetSupportedAudioEnhancePropertyInternal(MessageParcel &data, MessageParcel &reply);
    void GetSupportedAudioEffectPropertyInternal(MessageParcel &data, MessageParcel &reply);
    void SetAudioEffectPropertyInternal(MessageParcel &data, MessageParcel &reply);
    void GetAudioEffectPropertyInternal(MessageParcel &data, MessageParcel &reply);
    void SetAudioEnhancePropertyInternal(MessageParcel &data, MessageParcel &reply);
    void GetAudioEnhancePropertyInternal(MessageParcel &data, MessageParcel &reply);
    void InjectInterruptionInternal(MessageParcel &data, MessageParcel &reply);
    void ActivateAudioSessionInternal(MessageParcel &data, MessageParcel &reply);
    void DeactivateAudioSessionInternal(MessageParcel &data, MessageParcel &reply);
    void IsAudioSessionActivatedInternal(MessageParcel &data, MessageParcel &reply);
    void LoadSplitModuleInternal(MessageParcel &data, MessageParcel &reply);
    void IsAllowedPlaybackInternal(MessageParcel &data, MessageParcel &reply);
    void SetVoiceRingtoneMuteInternal(MessageParcel &data, MessageParcel &reply);
    void SetQueryClientTypeCallbackInternal(MessageParcel &data, MessageParcel &reply);
    void SetAudioClientInfoMgrCallbackInternal(MessageParcel &data, MessageParcel &reply);
    void SetVirtualCallInternal(MessageParcel &data, MessageParcel &reply);
    void SetDeviceConnectionStatusInternal(MessageParcel &data, MessageParcel &reply);
    void SetQueryAllowedPlaybackCallbackInternal(MessageParcel &data, MessageParcel &reply);
    void SetQueryBundleNameListCallbackInternal(MessageParcel &data, MessageParcel &reply);

    void OnMiddleEleRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option);
    void OnMiddleTenRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option);
    void OnMiddleNinRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option);
    void OnMiddleEigRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option);
    void OnMiddleSevRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option);
    void OnMiddleSixRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option);
    void OnMiddleFifRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option);
    void OnMiddleFouRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option);
    void OnMiddleTirRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option);
    void OnMiddleSecRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option);
    void OnMiddleFirRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option);
    void OnMiddlesRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option);
    void OnMidRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option);
    void OnAudioZoneRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option);
    void OnAudioZoneRemoteRequestExt(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option);
};
} // namespace AudioStandard
} // namespace OHOS
#endif // AUDIO_POLICY_MANAGER_STUB_H
