/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define KOALA_INTEROP_MODULE NotSpecifiedInteropModule
#include "audio.h"

OH_AUDIO_AudioManagerHandle AudioManager_constructImpl() {
    return {};
}
void AudioManager_destructImpl(OH_AUDIO_AudioManagerHandle thiz) {
}
void AudioManager_setVolume0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volume, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioManager_setVolume1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volume) {
}
void AudioManager_getVolume0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioManager_getVolume1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType) {
    return {};
}
void AudioManager_getMinVolume0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioManager_getMinVolume1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType) {
    return {};
}
void AudioManager_getMaxVolume0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioManager_getMaxVolume1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType) {
    return {};
}
void AudioManager_getDevices0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag, const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_) {
}
OH_NativePointer AudioManager_getDevices1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag) {
    return {};
}
void AudioManager_mute0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Boolean* mute, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioManager_mute1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Boolean* mute) {
}
void AudioManager_isMute0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Boolean_Void* callback_) {
}
OH_Boolean AudioManager_isMute1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType) {
    return {};
}
void AudioManager_isActive0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Boolean_Void* callback_) {
}
OH_Boolean AudioManager_isActive1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType) {
    return {};
}
void AudioManager_setMicrophoneMute0Impl(OH_NativePointer thisPtr, const OH_Boolean* mute, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioManager_setMicrophoneMute1Impl(OH_NativePointer thisPtr, const OH_Boolean* mute) {
}
void AudioManager_isMicrophoneMute0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Boolean_Void* callback_) {
}
OH_Boolean AudioManager_isMicrophoneMute1Impl(OH_NativePointer thisPtr) {
    return {};
}
void AudioManager_setRingerMode0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRingMode* mode, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioManager_setRingerMode1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRingMode* mode) {
}
void AudioManager_getRingerMode0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioRingMode_Void* callback_) {
}
OH_NativePointer AudioManager_getRingerMode1Impl(OH_NativePointer thisPtr) {
    return {};
}
void AudioManager_setAudioParameter0Impl(OH_NativePointer thisPtr, const OH_String* key, const OH_String* value, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioManager_setAudioParameter1Impl(OH_NativePointer thisPtr, const OH_String* key, const OH_String* value) {
}
void AudioManager_getAudioParameter0Impl(OH_NativePointer thisPtr, const OH_String* key, const AUDIO_AsyncCallback_String_Void* callback_) {
}
OH_String AudioManager_getAudioParameter1Impl(OH_NativePointer thisPtr, const OH_String* key) {
    return {};
}
void AudioManager_setExtraParametersImpl(OH_NativePointer thisPtr, const OH_String* mainKey, const Map_String_String* kvpairs) {
}
OH_NativePointer AudioManager_getExtraParametersImpl(OH_NativePointer thisPtr, const OH_String* mainKey, const Opt_Array_String* subKeys) {
    return {};
}
void AudioManager_setDeviceActive0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_ActiveDeviceType* deviceType, const OH_Boolean* active, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioManager_setDeviceActive1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_ActiveDeviceType* deviceType, const OH_Boolean* active) {
}
void AudioManager_isDeviceActive0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_ActiveDeviceType* deviceType, const AUDIO_AsyncCallback_Boolean_Void* callback_) {
}
OH_Boolean AudioManager_isDeviceActive1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_ActiveDeviceType* deviceType) {
    return {};
}
void AudioManager_onVolumeChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_VolumeEvent_Void* callback_) {
}
void AudioManager_onRingerModeChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioRingMode_Void* callback_) {
}
void AudioManager_setAudioScene0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioScene* scene, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioManager_setAudioScene1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioScene* scene) {
}
void AudioManager_getAudioScene0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioScene_Void* callback_) {
}
OH_NativePointer AudioManager_getAudioScene1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_NativePointer AudioManager_getAudioSceneSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioManager_onDeviceChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_DeviceChangeAction_Void* callback_) {
}
void AudioManager_offDeviceChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_DeviceChangeAction_Void* callback_) {
}
void AudioManager_onInterruptImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioInterrupt* interrupt, const AUDIO_Callback_InterruptAction_Void* callback_) {
}
void AudioManager_offInterruptImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioInterrupt* interrupt, const Opt_AUDIO_Callback_InterruptAction_Void* callback_) {
}
OH_NativePointer AudioManager_getVolumeManagerImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_NativePointer AudioManager_getStreamManagerImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_NativePointer AudioManager_getRoutingManagerImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_NativePointer AudioManager_getSessionManagerImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_NativePointer AudioManager_getSpatializationManagerImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioManager_disableSafeMediaVolumeImpl(OH_NativePointer thisPtr) {
}
OH_AUDIO_AudioRoutingManagerHandle AudioRoutingManager_constructImpl() {
    return {};
}
void AudioRoutingManager_destructImpl(OH_AUDIO_AudioRoutingManagerHandle thiz) {
}
void AudioRoutingManager_getDevices0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag, const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_) {
}
OH_NativePointer AudioRoutingManager_getDevices1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag) {
    return {};
}
OH_NativePointer AudioRoutingManager_getDevicesSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag) {
    return {};
}
void AudioRoutingManager_onDeviceChangeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag, const AUDIO_Callback_DeviceChangeAction_Void* callback_) {
}
void AudioRoutingManager_offDeviceChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_DeviceChangeAction_Void* callback_) {
}
OH_NativePointer AudioRoutingManager_getAvailableDevicesImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceUsage* deviceUsage) {
    return {};
}
void AudioRoutingManager_onAvailableDeviceChangeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceUsage* deviceUsage, const AUDIO_Callback_DeviceChangeAction_Void* callback_) {
}
void AudioRoutingManager_offAvailableDeviceChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_DeviceChangeAction_Void* callback_) {
}
void AudioRoutingManager_setCommunicationDevice0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_CommunicationDeviceType* deviceType, const OH_Boolean* active, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioRoutingManager_setCommunicationDevice1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_CommunicationDeviceType* deviceType, const OH_Boolean* active) {
}
void AudioRoutingManager_isCommunicationDeviceActive0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_CommunicationDeviceType* deviceType, const AUDIO_AsyncCallback_Boolean_Void* callback_) {
}
OH_Boolean AudioRoutingManager_isCommunicationDeviceActive1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_CommunicationDeviceType* deviceType) {
    return {};
}
OH_Boolean AudioRoutingManager_isCommunicationDeviceActiveSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_CommunicationDeviceType* deviceType) {
    return {};
}
void AudioRoutingManager_selectOutputDevice0Impl(OH_NativePointer thisPtr, const Array_CustomObject* outputAudioDevices, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioRoutingManager_selectOutputDevice1Impl(OH_NativePointer thisPtr, const Array_CustomObject* outputAudioDevices) {
}
void AudioRoutingManager_selectOutputDeviceByFilter0Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererFilter* filter, const Array_CustomObject* outputAudioDevices, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioRoutingManager_selectOutputDeviceByFilter1Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererFilter* filter, const Array_CustomObject* outputAudioDevices) {
}
void AudioRoutingManager_selectInputDevice0Impl(OH_NativePointer thisPtr, const Array_CustomObject* inputAudioDevices, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioRoutingManager_selectInputDevice1Impl(OH_NativePointer thisPtr, const Array_CustomObject* inputAudioDevices) {
}
void AudioRoutingManager_selectInputDeviceByFilterImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerFilter* filter, const Array_CustomObject* inputAudioDevices) {
}
void AudioRoutingManager_getPreferOutputDeviceForRendererInfo0Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererInfo* rendererInfo, const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_) {
}
OH_NativePointer AudioRoutingManager_getPreferOutputDeviceForRendererInfo1Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererInfo* rendererInfo) {
    return {};
}
OH_NativePointer AudioRoutingManager_getPreferredOutputDeviceForRendererInfoSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererInfo* rendererInfo) {
    return {};
}
OH_NativePointer AudioRoutingManager_getPreferredOutputDeviceByFilterImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererFilter* filter) {
    return {};
}
void AudioRoutingManager_onPreferOutputDeviceChangeForRendererInfoImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererInfo* rendererInfo, const AUDIO_Callback_AudioDeviceDescriptors_Void* callback_) {
}
void AudioRoutingManager_offPreferOutputDeviceChangeForRendererInfoImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* callback_) {
}
void AudioRoutingManager_getPreferredInputDeviceForCapturerInfo0Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerInfo* capturerInfo, const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_) {
}
OH_NativePointer AudioRoutingManager_getPreferredInputDeviceForCapturerInfo1Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerInfo* capturerInfo) {
    return {};
}
OH_NativePointer AudioRoutingManager_getPreferredInputDeviceByFilterImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerFilter* filter) {
    return {};
}
void AudioRoutingManager_onPreferredInputDeviceChangeForCapturerInfoImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerInfo* capturerInfo, const AUDIO_Callback_AudioDeviceDescriptors_Void* callback_) {
}
void AudioRoutingManager_offPreferredInputDeviceChangeForCapturerInfoImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* callback_) {
}
OH_NativePointer AudioRoutingManager_getPreferredInputDeviceForCapturerInfoSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerInfo* capturerInfo) {
    return {};
}
OH_Boolean AudioRoutingManager_isMicBlockDetectionSupportedImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioRoutingManager_onMicBlockStatusChangedImpl(OH_NativePointer thisPtr, const AUDIO_Callback_DeviceBlockStatusInfo_Void* callback_) {
}
void AudioRoutingManager_offMicBlockStatusChangedImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_DeviceBlockStatusInfo_Void* callback_) {
}
OH_AUDIO_AudioStreamManagerHandle AudioStreamManager_constructImpl() {
    return {};
}
void AudioStreamManager_destructImpl(OH_AUDIO_AudioStreamManagerHandle thiz) {
}
void AudioStreamManager_getCurrentAudioRendererInfoArray0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void* callback_) {
}
OH_NativePointer AudioStreamManager_getCurrentAudioRendererInfoArray1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_NativePointer AudioStreamManager_getCurrentAudioRendererInfoArraySyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioStreamManager_getCurrentAudioCapturerInfoArray0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void* callback_) {
}
OH_NativePointer AudioStreamManager_getCurrentAudioCapturerInfoArray1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_NativePointer AudioStreamManager_getCurrentAudioCapturerInfoArraySyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioStreamManager_getAudioEffectInfoArray0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_StreamUsage* usage, const AUDIO_AsyncCallback_AudioEffectInfoArray_Void* callback_) {
}
OH_NativePointer AudioStreamManager_getAudioEffectInfoArray1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_StreamUsage* usage) {
    return {};
}
OH_NativePointer AudioStreamManager_getAudioEffectInfoArraySyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_StreamUsage* usage) {
    return {};
}
void AudioStreamManager_onAudioRendererChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioRendererChangeInfoArray_Void* callback_) {
}
void AudioStreamManager_offAudioRendererChangeImpl(OH_NativePointer thisPtr) {
}
void AudioStreamManager_onAudioCapturerChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioCapturerChangeInfoArray_Void* callback_) {
}
void AudioStreamManager_offAudioCapturerChangeImpl(OH_NativePointer thisPtr) {
}
void AudioStreamManager_isActive0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Boolean_Void* callback_) {
}
OH_Boolean AudioStreamManager_isActive1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType) {
    return {};
}
OH_Boolean AudioStreamManager_isActiveSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType) {
    return {};
}
OH_AUDIO_AudioSessionManagerHandle AudioSessionManager_constructImpl() {
    return {};
}
void AudioSessionManager_destructImpl(OH_AUDIO_AudioSessionManagerHandle thiz) {
}
void AudioSessionManager_activateAudioSessionImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioSessionStrategy* strategy) {
}
void AudioSessionManager_deactivateAudioSessionImpl(OH_NativePointer thisPtr) {
}
OH_Boolean AudioSessionManager_isAudioSessionActivatedImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioSessionManager_onAudioSessionDeactivatedImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioSessionDeactivatedEvent_Void* callback_) {
}
void AudioSessionManager_offAudioSessionDeactivatedImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioSessionDeactivatedEvent_Void* callback_) {
}
OH_AUDIO_AudioVolumeManagerHandle AudioVolumeManager_constructImpl() {
    return {};
}
void AudioVolumeManager_destructImpl(OH_AUDIO_AudioVolumeManagerHandle thiz) {
}
void AudioVolumeManager_getVolumeGroupInfos0Impl(OH_NativePointer thisPtr, const OH_String* networkId, const AUDIO_AsyncCallback_VolumeGroupInfos_Void* callback_) {
}
OH_NativePointer AudioVolumeManager_getVolumeGroupInfos1Impl(OH_NativePointer thisPtr, const OH_String* networkId) {
    return {};
}
OH_NativePointer AudioVolumeManager_getVolumeGroupInfosSyncImpl(OH_NativePointer thisPtr, const OH_String* networkId) {
    return {};
}
void AudioVolumeManager_getVolumeGroupManager0Impl(OH_NativePointer thisPtr, const OH_Number* groupId, const AUDIO_AsyncCallback_AudioVolumeGroupManager_Void* callback_) {
}
OH_NativePointer AudioVolumeManager_getVolumeGroupManager1Impl(OH_NativePointer thisPtr, const OH_Number* groupId) {
    return {};
}
OH_NativePointer AudioVolumeManager_getVolumeGroupManagerSyncImpl(OH_NativePointer thisPtr, const OH_Number* groupId) {
    return {};
}
void AudioVolumeManager_onVolumeChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_VolumeEvent_Void* callback_) {
}
void AudioVolumeManager_offVolumeChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_VolumeEvent_Void* callback_) {
}
OH_AUDIO_AudioVolumeGroupManagerHandle AudioVolumeGroupManager_constructImpl() {
    return {};
}
void AudioVolumeGroupManager_destructImpl(OH_AUDIO_AudioVolumeGroupManagerHandle thiz) {
}
void AudioVolumeGroupManager_setVolume0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volume, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioVolumeGroupManager_setVolume1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volume) {
}
void AudioVolumeGroupManager_setVolumeWithFlagImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volume, const OH_Number* flags) {
}
OH_NativePointer AudioVolumeGroupManager_getActiveVolumeTypeSyncImpl(OH_NativePointer thisPtr, const OH_Number* uid) {
    return {};
}
void AudioVolumeGroupManager_getVolume0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioVolumeGroupManager_getVolume1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType) {
    return {};
}
OH_Number AudioVolumeGroupManager_getVolumeSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType) {
    return {};
}
void AudioVolumeGroupManager_getMinVolume0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioVolumeGroupManager_getMinVolume1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType) {
    return {};
}
OH_Number AudioVolumeGroupManager_getMinVolumeSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType) {
    return {};
}
void AudioVolumeGroupManager_getMaxVolume0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioVolumeGroupManager_getMaxVolume1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType) {
    return {};
}
OH_Number AudioVolumeGroupManager_getMaxVolumeSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType) {
    return {};
}
void AudioVolumeGroupManager_mute0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Boolean* mute, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioVolumeGroupManager_mute1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Boolean* mute) {
}
void AudioVolumeGroupManager_isMute0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Boolean_Void* callback_) {
}
OH_Boolean AudioVolumeGroupManager_isMute1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType) {
    return {};
}
OH_Boolean AudioVolumeGroupManager_isMuteSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType) {
    return {};
}
void AudioVolumeGroupManager_setRingerMode0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRingMode* mode, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioVolumeGroupManager_setRingerMode1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRingMode* mode) {
}
void AudioVolumeGroupManager_getRingerMode0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioRingMode_Void* callback_) {
}
OH_NativePointer AudioVolumeGroupManager_getRingerMode1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_NativePointer AudioVolumeGroupManager_getRingerModeSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioVolumeGroupManager_onRingerModeChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioRingMode_Void* callback_) {
}
void AudioVolumeGroupManager_setMicrophoneMute0Impl(OH_NativePointer thisPtr, const OH_Boolean* mute, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioVolumeGroupManager_setMicrophoneMute1Impl(OH_NativePointer thisPtr, const OH_Boolean* mute) {
}
void AudioVolumeGroupManager_setMicMuteImpl(OH_NativePointer thisPtr, const OH_Boolean* mute) {
}
void AudioVolumeGroupManager_setMicMutePersistentImpl(OH_NativePointer thisPtr, const OH_Boolean* mute, const OH_AUDIO_audio_PolicyType* type) {
}
OH_Boolean AudioVolumeGroupManager_isPersistentMicMuteImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioVolumeGroupManager_isMicrophoneMute0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Boolean_Void* callback_) {
}
OH_Boolean AudioVolumeGroupManager_isMicrophoneMute1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_Boolean AudioVolumeGroupManager_isMicrophoneMuteSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioVolumeGroupManager_onMicStateChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_MicStateChangeEvent_Void* callback_) {
}
void AudioVolumeGroupManager_offMicStateChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_MicStateChangeEvent_Void* callback_) {
}
OH_Boolean AudioVolumeGroupManager_isVolumeUnadjustableImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioVolumeGroupManager_adjustVolumeByStep0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_VolumeAdjustType* adjustType, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioVolumeGroupManager_adjustVolumeByStep1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_VolumeAdjustType* adjustType) {
}
void AudioVolumeGroupManager_adjustSystemVolumeByStep0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_AUDIO_audio_VolumeAdjustType* adjustType, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioVolumeGroupManager_adjustSystemVolumeByStep1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_AUDIO_audio_VolumeAdjustType* adjustType) {
}
void AudioVolumeGroupManager_getSystemVolumeInDb0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volumeLevel, const OH_AUDIO_audio_DeviceType* device, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioVolumeGroupManager_getSystemVolumeInDb1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volumeLevel, const OH_AUDIO_audio_DeviceType* device) {
    return {};
}
OH_Number AudioVolumeGroupManager_getSystemVolumeInDbSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volumeLevel, const OH_AUDIO_audio_DeviceType* device) {
    return {};
}
OH_Number AudioVolumeGroupManager_getMaxAmplitudeForInputDeviceImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* inputDevice) {
    return {};
}
OH_Number AudioVolumeGroupManager_getMaxAmplitudeForOutputDeviceImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* outputDevice) {
    return {};
}
OH_AUDIO_AudioSpatializationManagerHandle AudioSpatializationManager_constructImpl() {
    return {};
}
void AudioSpatializationManager_destructImpl(OH_AUDIO_AudioSpatializationManagerHandle thiz) {
}
OH_Boolean AudioSpatializationManager_isSpatializationSupportedImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_Boolean AudioSpatializationManager_isSpatializationSupportedForDeviceImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor) {
    return {};
}
OH_Boolean AudioSpatializationManager_isHeadTrackingSupportedImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_Boolean AudioSpatializationManager_isHeadTrackingSupportedForDeviceImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor) {
    return {};
}
void AudioSpatializationManager_setSpatializationEnabled0Impl(OH_NativePointer thisPtr, const OH_Boolean* enable, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioSpatializationManager_setSpatializationEnabled1Impl(OH_NativePointer thisPtr, const OH_Boolean* enable) {
}
void AudioSpatializationManager_setSpatializationEnabled2Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor, const OH_Boolean* enabled) {
}
OH_Boolean AudioSpatializationManager_isSpatializationEnabled0Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_Boolean AudioSpatializationManager_isSpatializationEnabled1Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor) {
    return {};
}
void AudioSpatializationManager_onSpatializationEnabledChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_Boolean_Void* callback_) {
}
void AudioSpatializationManager_onSpatializationEnabledChangeForAnyDeviceImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* callback_) {
}
void AudioSpatializationManager_offSpatializationEnabledChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_Boolean_Void* callback_) {
}
void AudioSpatializationManager_offSpatializationEnabledChangeForAnyDeviceImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* callback_) {
}
void AudioSpatializationManager_setHeadTrackingEnabled0Impl(OH_NativePointer thisPtr, const OH_Boolean* enable, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioSpatializationManager_setHeadTrackingEnabled1Impl(OH_NativePointer thisPtr, const OH_Boolean* enable) {
}
void AudioSpatializationManager_setHeadTrackingEnabled2Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor, const OH_Boolean* enabled) {
}
OH_Boolean AudioSpatializationManager_isHeadTrackingEnabled0Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_Boolean AudioSpatializationManager_isHeadTrackingEnabled1Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor) {
    return {};
}
void AudioSpatializationManager_onHeadTrackingEnabledChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_Boolean_Void* callback_) {
}
void AudioSpatializationManager_onHeadTrackingEnabledChangeForAnyDeviceImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* callback_) {
}
void AudioSpatializationManager_offHeadTrackingEnabledChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_Boolean_Void* callback_) {
}
void AudioSpatializationManager_offHeadTrackingEnabledChangeForAnyDeviceImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* callback_) {
}
void AudioSpatializationManager_updateSpatialDeviceStateImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioSpatialDeviceState* spatialDeviceState) {
}
void AudioSpatializationManager_setSpatializationSceneTypeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioSpatializationSceneType* spatializationSceneType) {
}
OH_NativePointer AudioSpatializationManager_getSpatializationSceneTypeImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_AUDIO_AudioRendererHandle AudioRenderer_constructImpl() {
    return {};
}
void AudioRenderer_destructImpl(OH_AUDIO_AudioRendererHandle thiz) {
}
void AudioRenderer_getRendererInfo0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioRendererInfo_Void* callback_) {
}
OH_NativePointer AudioRenderer_getRendererInfo1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_NativePointer AudioRenderer_getRendererInfoSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioRenderer_getStreamInfo0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioStreamInfo_Void* callback_) {
}
OH_NativePointer AudioRenderer_getStreamInfo1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_NativePointer AudioRenderer_getStreamInfoSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioRenderer_getAudioStreamId0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioRenderer_getAudioStreamId1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_Number AudioRenderer_getAudioStreamIdSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioRenderer_getAudioEffectMode0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioEffectMode_Void* callback_) {
}
OH_NativePointer AudioRenderer_getAudioEffectMode1Impl(OH_NativePointer thisPtr) {
    return {};
}
void AudioRenderer_setAudioEffectMode0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioEffectMode* mode, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioRenderer_setAudioEffectMode1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioEffectMode* mode) {
}
void AudioRenderer_start0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioRenderer_start1Impl(OH_NativePointer thisPtr) {
}
void AudioRenderer_write0Impl(OH_NativePointer thisPtr, const OH_Buffer* buffer, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioRenderer_write1Impl(OH_NativePointer thisPtr, const OH_Buffer* buffer) {
    return {};
}
void AudioRenderer_getAudioTime0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioRenderer_getAudioTime1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_Number AudioRenderer_getAudioTimeSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioRenderer_drain0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioRenderer_drain1Impl(OH_NativePointer thisPtr) {
}
void AudioRenderer_flushImpl(OH_NativePointer thisPtr) {
}
void AudioRenderer_pause0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioRenderer_pause1Impl(OH_NativePointer thisPtr) {
}
void AudioRenderer_stop0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioRenderer_stop1Impl(OH_NativePointer thisPtr) {
}
void AudioRenderer_release0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioRenderer_release1Impl(OH_NativePointer thisPtr) {
}
void AudioRenderer_getBufferSize0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioRenderer_getBufferSize1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_Number AudioRenderer_getBufferSizeSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioRenderer_setRenderRate0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRendererRate* rate, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioRenderer_setRenderRate1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRendererRate* rate) {
}
void AudioRenderer_setSpeedImpl(OH_NativePointer thisPtr, const OH_Number* speed) {
}
void AudioRenderer_getRenderRate0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioRendererRate_Void* callback_) {
}
OH_NativePointer AudioRenderer_getRenderRate1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_NativePointer AudioRenderer_getRenderRateSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_Number AudioRenderer_getSpeedImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioRenderer_setInterruptMode0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_InterruptMode* mode, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioRenderer_setInterruptMode1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_InterruptMode* mode) {
}
void AudioRenderer_setInterruptModeSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_InterruptMode* mode) {
}
void AudioRenderer_setVolume0Impl(OH_NativePointer thisPtr, const OH_Number* volume, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioRenderer_setVolume1Impl(OH_NativePointer thisPtr, const OH_Number* volume) {
}
OH_Number AudioRenderer_getVolumeImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioRenderer_setVolumeWithRampImpl(OH_NativePointer thisPtr, const OH_Number* volume, const OH_Number* duration) {
}
void AudioRenderer_getMinStreamVolume0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioRenderer_getMinStreamVolume1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_Number AudioRenderer_getMinStreamVolumeSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioRenderer_getMaxStreamVolume0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioRenderer_getMaxStreamVolume1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_Number AudioRenderer_getMaxStreamVolumeSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioRenderer_getUnderflowCount0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioRenderer_getUnderflowCount1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_Number AudioRenderer_getUnderflowCountSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioRenderer_getCurrentOutputDevices0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_) {
}
OH_NativePointer AudioRenderer_getCurrentOutputDevices1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_NativePointer AudioRenderer_getCurrentOutputDevicesSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioRenderer_setChannelBlendModeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_ChannelBlendMode* mode) {
}
void AudioRenderer_setSilentModeAndMixWithOthersImpl(OH_NativePointer thisPtr, const OH_Boolean* on) {
}
OH_Boolean AudioRenderer_getSilentModeAndMixWithOthersImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioRenderer_setDefaultOutputDeviceImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceType* deviceType) {
}
void AudioRenderer_onAudioInterruptImpl(OH_NativePointer thisPtr, const AUDIO_Callback_InterruptEvent_Void* callback_) {
}
void AudioRenderer_onMarkReachImpl(OH_NativePointer thisPtr, const OH_Number* frame, const AUDIO_Callback_Number_Void* callback_) {
}
void AudioRenderer_offMarkReachImpl(OH_NativePointer thisPtr) {
}
void AudioRenderer_onPeriodReachImpl(OH_NativePointer thisPtr, const OH_Number* frame, const AUDIO_Callback_Number_Void* callback_) {
}
void AudioRenderer_offPeriodReachImpl(OH_NativePointer thisPtr) {
}
void AudioRenderer_onStateChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioState_Void* callback_) {
}
void AudioRenderer_onOutputDeviceChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioDeviceDescriptors_Void* callback_) {
}
void AudioRenderer_onOutputDeviceChangeWithInfoImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioStreamDeviceChangeInfo_Void* callback_) {
}
void AudioRenderer_offOutputDeviceChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* callback_) {
}
void AudioRenderer_offOutputDeviceChangeWithInfoImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioStreamDeviceChangeInfo_Void* callback_) {
}
void AudioRenderer_onWriteDataImpl(OH_NativePointer thisPtr, const AUDIO_AudioRendererWriteDataCallback* callback_) {
}
void AudioRenderer_offWriteDataImpl(OH_NativePointer thisPtr, const Opt_AUDIO_AudioRendererWriteDataCallback* callback_) {
}
OH_NativePointer AudioRenderer_getStateImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_AUDIO_AudioCapturerHandle AudioCapturer_constructImpl() {
    return {};
}
void AudioCapturer_destructImpl(OH_AUDIO_AudioCapturerHandle thiz) {
}
void AudioCapturer_getCapturerInfo0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioCapturerInfo_Void* callback_) {
}
OH_NativePointer AudioCapturer_getCapturerInfo1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_NativePointer AudioCapturer_getCapturerInfoSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioCapturer_getStreamInfo0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioStreamInfo_Void* callback_) {
}
OH_NativePointer AudioCapturer_getStreamInfo1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_NativePointer AudioCapturer_getStreamInfoSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioCapturer_getAudioStreamId0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioCapturer_getAudioStreamId1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_Number AudioCapturer_getAudioStreamIdSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioCapturer_start0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioCapturer_start1Impl(OH_NativePointer thisPtr) {
}
void AudioCapturer_read0Impl(OH_NativePointer thisPtr, const OH_Number* size, const OH_Boolean* isBlockingRead, const AUDIO_AsyncCallback_Buffer_Void* callback_) {
}
OH_Buffer AudioCapturer_read1Impl(OH_NativePointer thisPtr, const OH_Number* size, const OH_Boolean* isBlockingRead) {
    return {};
}
void AudioCapturer_getAudioTime0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioCapturer_getAudioTime1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_Number AudioCapturer_getAudioTimeSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioCapturer_stop0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioCapturer_stop1Impl(OH_NativePointer thisPtr) {
}
void AudioCapturer_release0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_) {
}
void AudioCapturer_release1Impl(OH_NativePointer thisPtr) {
}
void AudioCapturer_getBufferSize0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_) {
}
OH_Number AudioCapturer_getBufferSize1Impl(OH_NativePointer thisPtr) {
    return {};
}
OH_Number AudioCapturer_getBufferSizeSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_NativePointer AudioCapturer_getCurrentInputDevicesImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_NativePointer AudioCapturer_getCurrentAudioCapturerChangeInfoImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_Number AudioCapturer_getOverflowCountImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_Number AudioCapturer_getOverflowCountSyncImpl(OH_NativePointer thisPtr) {
    return {};
}
void AudioCapturer_onMarkReachImpl(OH_NativePointer thisPtr, const OH_Number* frame, const AUDIO_Callback_Number_Void* callback_) {
}
void AudioCapturer_offMarkReachImpl(OH_NativePointer thisPtr) {
}
void AudioCapturer_onPeriodReachImpl(OH_NativePointer thisPtr, const OH_Number* frame, const AUDIO_Callback_Number_Void* callback_) {
}
void AudioCapturer_offPeriodReachImpl(OH_NativePointer thisPtr) {
}
void AudioCapturer_onStateChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioState_Void* callback_) {
}
void AudioCapturer_onAudioInterruptImpl(OH_NativePointer thisPtr, const AUDIO_Callback_InterruptEvent_Void* callback_) {
}
void AudioCapturer_offAudioInterruptImpl(OH_NativePointer thisPtr) {
}
void AudioCapturer_onInputDeviceChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioDeviceDescriptors_Void* callback_) {
}
void AudioCapturer_offInputDeviceChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* callback_) {
}
void AudioCapturer_onAudioCapturerChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioCapturerChangeInfo_Void* callback_) {
}
void AudioCapturer_offAudioCapturerChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioCapturerChangeInfo_Void* callback_) {
}
void AudioCapturer_onReadDataImpl(OH_NativePointer thisPtr, const AUDIO_Callback_Buffer_Void* callback_) {
}
void AudioCapturer_offReadDataImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_Buffer_Void* callback_) {
}
OH_NativePointer AudioCapturer_getStateImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_AUDIO_AsrProcessingControllerHandle AsrProcessingController_constructImpl() {
    return {};
}
void AsrProcessingController_destructImpl(OH_AUDIO_AsrProcessingControllerHandle thiz) {
}
OH_Boolean AsrProcessingController_setAsrAecModeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AsrAecMode* mode) {
    return {};
}
OH_NativePointer AsrProcessingController_getAsrAecModeImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_Boolean AsrProcessingController_setAsrNoiseSuppressionModeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AsrNoiseSuppressionMode* mode) {
    return {};
}
OH_NativePointer AsrProcessingController_getAsrNoiseSuppressionModeImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_Boolean AsrProcessingController_isWhisperingImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_Boolean AsrProcessingController_setAsrVoiceControlModeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AsrVoiceControlMode* mode, const OH_Boolean* enable) {
    return {};
}
OH_Boolean AsrProcessingController_setAsrVoiceMuteModeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AsrVoiceMuteMode* mode, const OH_Boolean* enable) {
    return {};
}
OH_Boolean AsrProcessingController_setAsrWhisperDetectionModeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AsrWhisperDetectionMode* mode) {
    return {};
}
OH_NativePointer AsrProcessingController_getAsrWhisperDetectionModeImpl(OH_NativePointer thisPtr) {
    return {};
}
OH_AUDIO_TonePlayerHandle TonePlayer_constructImpl() {
    return {};
}
void TonePlayer_destructImpl(OH_AUDIO_TonePlayerHandle thiz) {
}
void TonePlayer_load0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_ToneType* type, const AUDIO_AsyncCallback_Void* callback_) {
}
void TonePlayer_load1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_ToneType* type) {
}
void TonePlayer_start0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_) {
}
void TonePlayer_start1Impl(OH_NativePointer thisPtr) {
}
void TonePlayer_stop0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_) {
}
void TonePlayer_stop1Impl(OH_NativePointer thisPtr) {
}
void TonePlayer_release0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_) {
}
void TonePlayer_release1Impl(OH_NativePointer thisPtr) {
}
OH_NativePointer GlobalScope_ohos_multimedia_audio_getAudioManagerImpl() {
    return {};
}
void GlobalScope_ohos_multimedia_audio_createAudioCapturer0Impl(const OH_CustomObject* options, const OH_CustomObject* callback_) {
}
OH_NativePointer GlobalScope_ohos_multimedia_audio_createAudioCapturer1Impl(const OH_CustomObject* options) {
    return {};
}
void GlobalScope_ohos_multimedia_audio_createAudioRenderer0Impl(const OH_CustomObject* options, const OH_CustomObject* callback_) {
}
OH_NativePointer GlobalScope_ohos_multimedia_audio_createAudioRenderer1Impl(const OH_CustomObject* options) {
    return {};
}
void GlobalScope_ohos_multimedia_audio_createTonePlayer0Impl(const OH_CustomObject* options, const OH_CustomObject* callback_) {
}
OH_NativePointer GlobalScope_ohos_multimedia_audio_createTonePlayer1Impl(const OH_CustomObject* options) {
    return {};
}
OH_NativePointer GlobalScope_ohos_multimedia_audio_createAsrProcessingControllerImpl(const OH_CustomObject* audioCapturer) {
    return {};
}