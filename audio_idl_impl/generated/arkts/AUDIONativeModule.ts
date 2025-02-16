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

import { KInt, KBoolean, KFloat, KUInt, KStringPtr, KPointer, KNativePointer, KInt32ArrayPtr, KUint8ArrayPtr, KFloat32ArrayPtr, pointer, NativeBuffer, loadNativeModuleLibrary } from "@koalaui/interop"
import { int32, float32 } from "@koalaui/common"

export class AUDIONativeModule {
    static {
        loadNativeModuleLibrary("AUDIONativeModule")
    }
    native static _AllocateNativeBuffer(len: int32, data: KUint8ArrayPtr, init: KUint8ArrayPtr): NativeBuffer 
    native static _AudioManager_ctor(): KPointer 
    native static _AudioManager_getFinalizer(): KPointer 
    native static _AudioManager_setVolume0(ptr: KPointer, volumeType: KInt, volume: number, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_setVolume1(ptr: KPointer, volumeType: KInt, volume: number): void 
    native static _AudioManager_getVolume0(ptr: KPointer, volumeType: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_getVolume1(ptr: KPointer, volumeType: KInt): number 
    native static _AudioManager_getMinVolume0(ptr: KPointer, volumeType: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_getMinVolume1(ptr: KPointer, volumeType: KInt): number 
    native static _AudioManager_getMaxVolume0(ptr: KPointer, volumeType: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_getMaxVolume1(ptr: KPointer, volumeType: KInt): number 
    native static _AudioManager_getDevices0(ptr: KPointer, deviceFlag: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_getDevices1(ptr: KPointer, deviceFlag: KInt): KPointer 
    native static _AudioManager_mute0(ptr: KPointer, volumeType: KInt, mute: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_mute1(ptr: KPointer, volumeType: KInt, mute: KInt): void 
    native static _AudioManager_isMute0(ptr: KPointer, volumeType: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_isMute1(ptr: KPointer, volumeType: KInt): boolean 
    native static _AudioManager_isActive0(ptr: KPointer, volumeType: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_isActive1(ptr: KPointer, volumeType: KInt): boolean 
    native static _AudioManager_setMicrophoneMute0(ptr: KPointer, mute: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_setMicrophoneMute1(ptr: KPointer, mute: KInt): void 
    native static _AudioManager_isMicrophoneMute0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_isMicrophoneMute1(ptr: KPointer): boolean 
    native static _AudioManager_setRingerMode0(ptr: KPointer, mode: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_setRingerMode1(ptr: KPointer, mode: KInt): void 
    native static _AudioManager_getRingerMode0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_getRingerMode1(ptr: KPointer): KPointer 
    native static _AudioManager_setAudioParameter0(ptr: KPointer, key: KStringPtr, value: KStringPtr, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_setAudioParameter1(ptr: KPointer, key: KStringPtr, value: KStringPtr): void 
    native static _AudioManager_getAudioParameter0(ptr: KPointer, key: KStringPtr, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_getAudioParameter1(ptr: KPointer, key: KStringPtr): string 
    native static _AudioManager_setExtraParameters(ptr: KPointer, mainKey: KStringPtr, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_getExtraParameters(ptr: KPointer, mainKey: KStringPtr, thisArray: KUint8ArrayPtr, thisLength: int32): KPointer 
    native static _AudioManager_setDeviceActive0(ptr: KPointer, deviceType: KInt, active: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_setDeviceActive1(ptr: KPointer, deviceType: KInt, active: KInt): void 
    native static _AudioManager_isDeviceActive0(ptr: KPointer, deviceType: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_isDeviceActive1(ptr: KPointer, deviceType: KInt): boolean 
    native static _AudioManager_onVolumeChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_onRingerModeChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_setAudioScene0(ptr: KPointer, scene: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_setAudioScene1(ptr: KPointer, scene: KInt): void 
    native static _AudioManager_getAudioScene0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_getAudioScene1(ptr: KPointer): KPointer 
    native static _AudioManager_getAudioSceneSync(ptr: KPointer): KPointer 
    native static _AudioManager_onDeviceChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_offDeviceChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_onInterrupt(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_offInterrupt(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioManager_getVolumeManager(ptr: KPointer): KPointer 
    native static _AudioManager_getStreamManager(ptr: KPointer): KPointer 
    native static _AudioManager_getRoutingManager(ptr: KPointer): KPointer 
    native static _AudioManager_getSessionManager(ptr: KPointer): KPointer 
    native static _AudioManager_getSpatializationManager(ptr: KPointer): KPointer 
    native static _AudioManager_disableSafeMediaVolume(ptr: KPointer): void 
    native static _AudioRoutingManager_ctor(): KPointer 
    native static _AudioRoutingManager_getFinalizer(): KPointer 
    native static _AudioRoutingManager_getDevices0(ptr: KPointer, deviceFlag: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_getDevices1(ptr: KPointer, deviceFlag: KInt): KPointer 
    native static _AudioRoutingManager_getDevicesSync(ptr: KPointer, deviceFlag: KInt): KPointer 
    native static _AudioRoutingManager_onDeviceChange(ptr: KPointer, deviceFlag: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_offDeviceChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_getAvailableDevices(ptr: KPointer, deviceUsage: KInt): KPointer 
    native static _AudioRoutingManager_onAvailableDeviceChange(ptr: KPointer, deviceUsage: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_offAvailableDeviceChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_setCommunicationDevice0(ptr: KPointer, deviceType: KInt, active: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_setCommunicationDevice1(ptr: KPointer, deviceType: KInt, active: KInt): void 
    native static _AudioRoutingManager_isCommunicationDeviceActive0(ptr: KPointer, deviceType: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_isCommunicationDeviceActive1(ptr: KPointer, deviceType: KInt): boolean 
    native static _AudioRoutingManager_isCommunicationDeviceActiveSync(ptr: KPointer, deviceType: KInt): boolean 
    native static _AudioRoutingManager_selectOutputDevice0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_selectOutputDevice1(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_selectOutputDeviceByFilter0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_selectOutputDeviceByFilter1(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_selectInputDevice0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_selectInputDevice1(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_selectInputDeviceByFilter(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_getPreferOutputDeviceForRendererInfo0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_getPreferOutputDeviceForRendererInfo1(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): KPointer 
    native static _AudioRoutingManager_getPreferredOutputDeviceForRendererInfoSync(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): KPointer 
    native static _AudioRoutingManager_getPreferredOutputDeviceByFilter(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): KPointer 
    native static _AudioRoutingManager_onPreferOutputDeviceChangeForRendererInfo(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_offPreferOutputDeviceChangeForRendererInfo(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_getPreferredInputDeviceForCapturerInfo0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_getPreferredInputDeviceForCapturerInfo1(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): KPointer 
    native static _AudioRoutingManager_getPreferredInputDeviceByFilter(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): KPointer 
    native static _AudioRoutingManager_onPreferredInputDeviceChangeForCapturerInfo(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_offPreferredInputDeviceChangeForCapturerInfo(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_getPreferredInputDeviceForCapturerInfoSync(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): KPointer 
    native static _AudioRoutingManager_isMicBlockDetectionSupported(ptr: KPointer): boolean 
    native static _AudioRoutingManager_onMicBlockStatusChanged(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRoutingManager_offMicBlockStatusChanged(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioStreamManager_ctor(): KPointer 
    native static _AudioStreamManager_getFinalizer(): KPointer 
    native static _AudioStreamManager_getCurrentAudioRendererInfoArray0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioStreamManager_getCurrentAudioRendererInfoArray1(ptr: KPointer): KPointer 
    native static _AudioStreamManager_getCurrentAudioRendererInfoArraySync(ptr: KPointer): KPointer 
    native static _AudioStreamManager_getCurrentAudioCapturerInfoArray0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioStreamManager_getCurrentAudioCapturerInfoArray1(ptr: KPointer): KPointer 
    native static _AudioStreamManager_getCurrentAudioCapturerInfoArraySync(ptr: KPointer): KPointer 
    native static _AudioStreamManager_getAudioEffectInfoArray0(ptr: KPointer, usage: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioStreamManager_getAudioEffectInfoArray1(ptr: KPointer, usage: KInt): KPointer 
    native static _AudioStreamManager_getAudioEffectInfoArraySync(ptr: KPointer, usage: KInt): KPointer 
    native static _AudioStreamManager_onAudioRendererChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioStreamManager_offAudioRendererChange(ptr: KPointer): void 
    native static _AudioStreamManager_onAudioCapturerChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioStreamManager_offAudioCapturerChange(ptr: KPointer): void 
    native static _AudioStreamManager_isActive0(ptr: KPointer, volumeType: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioStreamManager_isActive1(ptr: KPointer, volumeType: KInt): boolean 
    native static _AudioStreamManager_isActiveSync(ptr: KPointer, volumeType: KInt): boolean 
    native static _AudioSessionManager_ctor(): KPointer 
    native static _AudioSessionManager_getFinalizer(): KPointer 
    native static _AudioSessionManager_activateAudioSession(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioSessionManager_deactivateAudioSession(ptr: KPointer): void 
    native static _AudioSessionManager_isAudioSessionActivated(ptr: KPointer): boolean 
    native static _AudioSessionManager_onAudioSessionDeactivated(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioSessionManager_offAudioSessionDeactivated(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeManager_ctor(): KPointer 
    native static _AudioVolumeManager_getFinalizer(): KPointer 
    native static _AudioVolumeManager_getVolumeGroupInfos0(ptr: KPointer, networkId: KStringPtr, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeManager_getVolumeGroupInfos1(ptr: KPointer, networkId: KStringPtr): KPointer 
    native static _AudioVolumeManager_getVolumeGroupInfosSync(ptr: KPointer, networkId: KStringPtr): KPointer 
    native static _AudioVolumeManager_getVolumeGroupManager0(ptr: KPointer, groupId: number, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeManager_getVolumeGroupManager1(ptr: KPointer, groupId: number): KPointer 
    native static _AudioVolumeManager_getVolumeGroupManagerSync(ptr: KPointer, groupId: number): KPointer 
    native static _AudioVolumeManager_onVolumeChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeManager_offVolumeChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_ctor(): KPointer 
    native static _AudioVolumeGroupManager_getFinalizer(): KPointer 
    native static _AudioVolumeGroupManager_setVolume0(ptr: KPointer, volumeType: KInt, volume: number, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_setVolume1(ptr: KPointer, volumeType: KInt, volume: number): void 
    native static _AudioVolumeGroupManager_setVolumeWithFlag(ptr: KPointer, volumeType: KInt, volume: number, flags: number): void 
    native static _AudioVolumeGroupManager_getActiveVolumeTypeSync(ptr: KPointer, uid: number): KPointer 
    native static _AudioVolumeGroupManager_getVolume0(ptr: KPointer, volumeType: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_getVolume1(ptr: KPointer, volumeType: KInt): number 
    native static _AudioVolumeGroupManager_getVolumeSync(ptr: KPointer, volumeType: KInt): number 
    native static _AudioVolumeGroupManager_getMinVolume0(ptr: KPointer, volumeType: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_getMinVolume1(ptr: KPointer, volumeType: KInt): number 
    native static _AudioVolumeGroupManager_getMinVolumeSync(ptr: KPointer, volumeType: KInt): number 
    native static _AudioVolumeGroupManager_getMaxVolume0(ptr: KPointer, volumeType: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_getMaxVolume1(ptr: KPointer, volumeType: KInt): number 
    native static _AudioVolumeGroupManager_getMaxVolumeSync(ptr: KPointer, volumeType: KInt): number 
    native static _AudioVolumeGroupManager_mute0(ptr: KPointer, volumeType: KInt, mute: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_mute1(ptr: KPointer, volumeType: KInt, mute: KInt): void 
    native static _AudioVolumeGroupManager_isMute0(ptr: KPointer, volumeType: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_isMute1(ptr: KPointer, volumeType: KInt): boolean 
    native static _AudioVolumeGroupManager_isMuteSync(ptr: KPointer, volumeType: KInt): boolean 
    native static _AudioVolumeGroupManager_setRingerMode0(ptr: KPointer, mode: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_setRingerMode1(ptr: KPointer, mode: KInt): void 
    native static _AudioVolumeGroupManager_getRingerMode0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_getRingerMode1(ptr: KPointer): KPointer 
    native static _AudioVolumeGroupManager_getRingerModeSync(ptr: KPointer): KPointer 
    native static _AudioVolumeGroupManager_onRingerModeChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_setMicrophoneMute0(ptr: KPointer, mute: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_setMicrophoneMute1(ptr: KPointer, mute: KInt): void 
    native static _AudioVolumeGroupManager_setMicMute(ptr: KPointer, mute: KInt): void 
    native static _AudioVolumeGroupManager_setMicMutePersistent(ptr: KPointer, mute: KInt, type: KInt): void 
    native static _AudioVolumeGroupManager_isPersistentMicMute(ptr: KPointer): boolean 
    native static _AudioVolumeGroupManager_isMicrophoneMute0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_isMicrophoneMute1(ptr: KPointer): boolean 
    native static _AudioVolumeGroupManager_isMicrophoneMuteSync(ptr: KPointer): boolean 
    native static _AudioVolumeGroupManager_onMicStateChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_offMicStateChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_isVolumeUnadjustable(ptr: KPointer): boolean 
    native static _AudioVolumeGroupManager_adjustVolumeByStep0(ptr: KPointer, adjustType: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_adjustVolumeByStep1(ptr: KPointer, adjustType: KInt): void 
    native static _AudioVolumeGroupManager_adjustSystemVolumeByStep0(ptr: KPointer, volumeType: KInt, adjustType: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_adjustSystemVolumeByStep1(ptr: KPointer, volumeType: KInt, adjustType: KInt): void 
    native static _AudioVolumeGroupManager_getSystemVolumeInDb0(ptr: KPointer, volumeType: KInt, volumeLevel: number, device: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioVolumeGroupManager_getSystemVolumeInDb1(ptr: KPointer, volumeType: KInt, volumeLevel: number, device: KInt): number 
    native static _AudioVolumeGroupManager_getSystemVolumeInDbSync(ptr: KPointer, volumeType: KInt, volumeLevel: number, device: KInt): number 
    native static _AudioVolumeGroupManager_getMaxAmplitudeForInputDevice(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): number 
    native static _AudioVolumeGroupManager_getMaxAmplitudeForOutputDevice(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): number 
    native static _AudioSpatializationManager_ctor(): KPointer 
    native static _AudioSpatializationManager_getFinalizer(): KPointer 
    native static _AudioSpatializationManager_isSpatializationSupported(ptr: KPointer): boolean 
    native static _AudioSpatializationManager_isSpatializationSupportedForDevice(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): boolean 
    native static _AudioSpatializationManager_isHeadTrackingSupported(ptr: KPointer): boolean 
    native static _AudioSpatializationManager_isHeadTrackingSupportedForDevice(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): boolean 
    native static _AudioSpatializationManager_setSpatializationEnabled0(ptr: KPointer, enable: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioSpatializationManager_setSpatializationEnabled1(ptr: KPointer, enable: KInt): void 
    native static _AudioSpatializationManager_setSpatializationEnabled2(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32, enabled: KInt): void 
    native static _AudioSpatializationManager_isSpatializationEnabled0(ptr: KPointer): boolean 
    native static _AudioSpatializationManager_isSpatializationEnabled1(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): boolean 
    native static _AudioSpatializationManager_onSpatializationEnabledChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioSpatializationManager_onSpatializationEnabledChangeForAnyDevice(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioSpatializationManager_offSpatializationEnabledChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioSpatializationManager_offSpatializationEnabledChangeForAnyDevice(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioSpatializationManager_setHeadTrackingEnabled0(ptr: KPointer, enable: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioSpatializationManager_setHeadTrackingEnabled1(ptr: KPointer, enable: KInt): void 
    native static _AudioSpatializationManager_setHeadTrackingEnabled2(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32, enabled: KInt): void 
    native static _AudioSpatializationManager_isHeadTrackingEnabled0(ptr: KPointer): boolean 
    native static _AudioSpatializationManager_isHeadTrackingEnabled1(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): boolean 
    native static _AudioSpatializationManager_onHeadTrackingEnabledChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioSpatializationManager_onHeadTrackingEnabledChangeForAnyDevice(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioSpatializationManager_offHeadTrackingEnabledChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioSpatializationManager_offHeadTrackingEnabledChangeForAnyDevice(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioSpatializationManager_updateSpatialDeviceState(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioSpatializationManager_setSpatializationSceneType(ptr: KPointer, spatializationSceneType: KInt): void 
    native static _AudioSpatializationManager_getSpatializationSceneType(ptr: KPointer): KPointer 
    native static _AudioRenderer_ctor(): KPointer 
    native static _AudioRenderer_getFinalizer(): KPointer 
    native static _AudioRenderer_getRendererInfo0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_getRendererInfo1(ptr: KPointer): KPointer 
    native static _AudioRenderer_getRendererInfoSync(ptr: KPointer): KPointer 
    native static _AudioRenderer_getStreamInfo0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_getStreamInfo1(ptr: KPointer): KPointer 
    native static _AudioRenderer_getStreamInfoSync(ptr: KPointer): KPointer 
    native static _AudioRenderer_getAudioStreamId0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_getAudioStreamId1(ptr: KPointer): number 
    native static _AudioRenderer_getAudioStreamIdSync(ptr: KPointer): number 
    native static _AudioRenderer_getAudioEffectMode0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_getAudioEffectMode1(ptr: KPointer): KPointer 
    native static _AudioRenderer_setAudioEffectMode0(ptr: KPointer, mode: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_setAudioEffectMode1(ptr: KPointer, mode: KInt): void 
    native static _AudioRenderer_start0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_start1(ptr: KPointer): void 
    native static _AudioRenderer_write0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_write1(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): number 
    native static _AudioRenderer_getAudioTime0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_getAudioTime1(ptr: KPointer): number 
    native static _AudioRenderer_getAudioTimeSync(ptr: KPointer): number 
    native static _AudioRenderer_drain0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_drain1(ptr: KPointer): void 
    native static _AudioRenderer_flush(ptr: KPointer): void 
    native static _AudioRenderer_pause0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_pause1(ptr: KPointer): void 
    native static _AudioRenderer_stop0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_stop1(ptr: KPointer): void 
    native static _AudioRenderer_release0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_release1(ptr: KPointer): void 
    native static _AudioRenderer_getBufferSize0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_getBufferSize1(ptr: KPointer): number 
    native static _AudioRenderer_getBufferSizeSync(ptr: KPointer): number 
    native static _AudioRenderer_setRenderRate0(ptr: KPointer, rate: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_setRenderRate1(ptr: KPointer, rate: KInt): void 
    native static _AudioRenderer_setSpeed(ptr: KPointer, speed: number): void 
    native static _AudioRenderer_getRenderRate0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_getRenderRate1(ptr: KPointer): KPointer 
    native static _AudioRenderer_getRenderRateSync(ptr: KPointer): KPointer 
    native static _AudioRenderer_getSpeed(ptr: KPointer): number 
    native static _AudioRenderer_setInterruptMode0(ptr: KPointer, mode: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_setInterruptMode1(ptr: KPointer, mode: KInt): void 
    native static _AudioRenderer_setInterruptModeSync(ptr: KPointer, mode: KInt): void 
    native static _AudioRenderer_setVolume0(ptr: KPointer, volume: number, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_setVolume1(ptr: KPointer, volume: number): void 
    native static _AudioRenderer_getVolume(ptr: KPointer): number 
    native static _AudioRenderer_setVolumeWithRamp(ptr: KPointer, volume: number, duration: number): void 
    native static _AudioRenderer_getMinStreamVolume0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_getMinStreamVolume1(ptr: KPointer): number 
    native static _AudioRenderer_getMinStreamVolumeSync(ptr: KPointer): number 
    native static _AudioRenderer_getMaxStreamVolume0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_getMaxStreamVolume1(ptr: KPointer): number 
    native static _AudioRenderer_getMaxStreamVolumeSync(ptr: KPointer): number 
    native static _AudioRenderer_getUnderflowCount0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_getUnderflowCount1(ptr: KPointer): number 
    native static _AudioRenderer_getUnderflowCountSync(ptr: KPointer): number 
    native static _AudioRenderer_getCurrentOutputDevices0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_getCurrentOutputDevices1(ptr: KPointer): KPointer 
    native static _AudioRenderer_getCurrentOutputDevicesSync(ptr: KPointer): KPointer 
    native static _AudioRenderer_setChannelBlendMode(ptr: KPointer, mode: KInt): void 
    native static _AudioRenderer_setSilentModeAndMixWithOthers(ptr: KPointer, on: KInt): void 
    native static _AudioRenderer_getSilentModeAndMixWithOthers(ptr: KPointer): boolean 
    native static _AudioRenderer_setDefaultOutputDevice(ptr: KPointer, deviceType: KInt): void 
    native static _AudioRenderer_onAudioInterrupt(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_onMarkReach(ptr: KPointer, frame: number, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_offMarkReach(ptr: KPointer): void 
    native static _AudioRenderer_onPeriodReach(ptr: KPointer, frame: number, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_offPeriodReach(ptr: KPointer): void 
    native static _AudioRenderer_onStateChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_onOutputDeviceChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_onOutputDeviceChangeWithInfo(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_offOutputDeviceChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_offOutputDeviceChangeWithInfo(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_onWriteData(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_offWriteData(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioRenderer_getState(ptr: KPointer): KPointer 
    native static _AudioCapturer_ctor(): KPointer 
    native static _AudioCapturer_getFinalizer(): KPointer 
    native static _AudioCapturer_getCapturerInfo0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_getCapturerInfo1(ptr: KPointer): KPointer 
    native static _AudioCapturer_getCapturerInfoSync(ptr: KPointer): KPointer 
    native static _AudioCapturer_getStreamInfo0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_getStreamInfo1(ptr: KPointer): KPointer 
    native static _AudioCapturer_getStreamInfoSync(ptr: KPointer): KPointer 
    native static _AudioCapturer_getAudioStreamId0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_getAudioStreamId1(ptr: KPointer): number 
    native static _AudioCapturer_getAudioStreamIdSync(ptr: KPointer): number 
    native static _AudioCapturer_start0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_start1(ptr: KPointer): void 
    native static _AudioCapturer_read0(ptr: KPointer, size: number, isBlockingRead: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_read1(ptr: KPointer, size: number, isBlockingRead: KInt): NativeBuffer 
    native static _AudioCapturer_getAudioTime0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_getAudioTime1(ptr: KPointer): number 
    native static _AudioCapturer_getAudioTimeSync(ptr: KPointer): number 
    native static _AudioCapturer_stop0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_stop1(ptr: KPointer): void 
    native static _AudioCapturer_release0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_release1(ptr: KPointer): void 
    native static _AudioCapturer_getBufferSize0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_getBufferSize1(ptr: KPointer): number 
    native static _AudioCapturer_getBufferSizeSync(ptr: KPointer): number 
    native static _AudioCapturer_getCurrentInputDevices(ptr: KPointer): KPointer 
    native static _AudioCapturer_getCurrentAudioCapturerChangeInfo(ptr: KPointer): KPointer 
    native static _AudioCapturer_getOverflowCount(ptr: KPointer): number 
    native static _AudioCapturer_getOverflowCountSync(ptr: KPointer): number 
    native static _AudioCapturer_onMarkReach(ptr: KPointer, frame: number, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_offMarkReach(ptr: KPointer): void 
    native static _AudioCapturer_onPeriodReach(ptr: KPointer, frame: number, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_offPeriodReach(ptr: KPointer): void 
    native static _AudioCapturer_onStateChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_onAudioInterrupt(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_offAudioInterrupt(ptr: KPointer): void 
    native static _AudioCapturer_onInputDeviceChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_offInputDeviceChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_onAudioCapturerChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_offAudioCapturerChange(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_onReadData(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_offReadData(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _AudioCapturer_getState(ptr: KPointer): KPointer 
    native static _AsrProcessingController_ctor(): KPointer 
    native static _AsrProcessingController_getFinalizer(): KPointer 
    native static _AsrProcessingController_setAsrAecMode(ptr: KPointer, mode: KInt): boolean 
    native static _AsrProcessingController_getAsrAecMode(ptr: KPointer): KPointer 
    native static _AsrProcessingController_setAsrNoiseSuppressionMode(ptr: KPointer, mode: KInt): boolean 
    native static _AsrProcessingController_getAsrNoiseSuppressionMode(ptr: KPointer): KPointer 
    native static _AsrProcessingController_isWhispering(ptr: KPointer): boolean 
    native static _AsrProcessingController_setAsrVoiceControlMode(ptr: KPointer, mode: KInt, enable: KInt): boolean 
    native static _AsrProcessingController_setAsrVoiceMuteMode(ptr: KPointer, mode: KInt, enable: KInt): boolean 
    native static _AsrProcessingController_setAsrWhisperDetectionMode(ptr: KPointer, mode: KInt): boolean 
    native static _AsrProcessingController_getAsrWhisperDetectionMode(ptr: KPointer): KPointer 
    native static _TonePlayer_ctor(): KPointer 
    native static _TonePlayer_getFinalizer(): KPointer 
    native static _TonePlayer_load0(ptr: KPointer, type: KInt, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _TonePlayer_load1(ptr: KPointer, type: KInt): void 
    native static _TonePlayer_start0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _TonePlayer_start1(ptr: KPointer): void 
    native static _TonePlayer_stop0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _TonePlayer_stop1(ptr: KPointer): void 
    native static _TonePlayer_release0(ptr: KPointer, thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _TonePlayer_release1(ptr: KPointer): void 
    native static _GlobalScope_ohos_multimedia_audio_getAudioManager(): KPointer 
    native static _GlobalScope_ohos_multimedia_audio_createAudioCapturer0(thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _GlobalScope_ohos_multimedia_audio_createAudioCapturer1(thisArray: KUint8ArrayPtr, thisLength: int32): KPointer 
    native static _GlobalScope_ohos_multimedia_audio_createAudioRenderer0(thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _GlobalScope_ohos_multimedia_audio_createAudioRenderer1(thisArray: KUint8ArrayPtr, thisLength: int32): KPointer 
    native static _GlobalScope_ohos_multimedia_audio_createTonePlayer0(thisArray: KUint8ArrayPtr, thisLength: int32): void 
    native static _GlobalScope_ohos_multimedia_audio_createTonePlayer1(thisArray: KUint8ArrayPtr, thisLength: int32): KPointer 
    native static _GlobalScope_ohos_multimedia_audio_createAsrProcessingController(thisArray: KUint8ArrayPtr, thisLength: int32): KPointer 
}