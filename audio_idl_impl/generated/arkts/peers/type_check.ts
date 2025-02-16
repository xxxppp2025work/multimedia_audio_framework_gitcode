import { KBoolean, KStringPtr, NativeBuffer } from "@koalaui/interop"
import { MaterializedBase } from "./../MaterializedBase"
import { audio_AudioErrors, audio_AudioState, audio_AudioVolumeType, audio_DeviceFlag, audio_DeviceUsage, audio_DeviceRole, audio_DeviceType, audio_ActiveDeviceType, audio_CommunicationDeviceType, audio_AudioRingMode, audio_PolicyType, audio_AudioSampleFormat, audio_AudioChannel, audio_AudioSamplingRate, audio_AudioEncodingType, audio_ContentType, audio_StreamUsage, audio_InterruptRequestType, audio_VolumeFlag, audio, audio_AudioChannelLayout, audio_AudioPrivacyType, audio_InterruptMode, audio_AudioRendererRate, audio_InterruptType, audio_InterruptHint, audio_InterruptForceType, audio_InterruptActionType, audio_DeviceChangeType, audio_AudioScene, audio_VolumeAdjustType, audio_InterruptRequestResultType, audio_DeviceBlockStatus, audio_AudioConcurrencyMode, audio_AudioSessionDeactivatedReason, audio_AudioSpatializationSceneType, audio_ConnectType, audio_ChannelBlendMode, audio_AudioStreamDeviceChangeReason, audio_AudioDataCallbackResult, audio_AudioEffectMode, audio_SourceType, audio_AsrNoiseSuppressionMode, audio_AsrAecMode, audio_AsrVoiceControlMode, audio_AsrVoiceMuteMode, audio_AsrWhisperDetectionMode, audio_ToneType, audio_AudioSpatialDeviceType } from "./../OHAudioNamespace"
export class TypeChecker {
    static typeInstanceOf<T>(value: Object, prop: string): boolean {
        return value instanceof T
    }
    static typeCast<T>(value: Object): T {
        return value as T
    }
    static isaudio_ActiveDeviceType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_ActiveDeviceType
    }
    static isaudio_AsrAecMode(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AsrAecMode
    }
    static isaudio_AsrNoiseSuppressionMode(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AsrNoiseSuppressionMode
    }
    static isaudio_AsrVoiceControlMode(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AsrVoiceControlMode
    }
    static isaudio_AsrVoiceMuteMode(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AsrVoiceMuteMode
    }
    static isaudio_AsrWhisperDetectionMode(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AsrWhisperDetectionMode
    }
    static isaudio_AudioChannel(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioChannel
    }
    static isaudio_AudioChannelLayout(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioChannelLayout
    }
    static isaudio_AudioConcurrencyMode(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioConcurrencyMode
    }
    static isaudio_AudioDataCallbackResult(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioDataCallbackResult
    }
    static isaudio_AudioEffectMode(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioEffectMode
    }
    static isaudio_AudioEncodingType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioEncodingType
    }
    static isaudio_AudioErrors(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioErrors
    }
    static isaudio_AudioPrivacyType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioPrivacyType
    }
    static isaudio_AudioRendererRate(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioRendererRate
    }
    static isaudio_AudioRingMode(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioRingMode
    }
    static isaudio_AudioSampleFormat(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioSampleFormat
    }
    static isaudio_AudioSamplingRate(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioSamplingRate
    }
    static isaudio_AudioScene(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioScene
    }
    static isaudio_AudioSessionDeactivatedReason(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioSessionDeactivatedReason
    }
    static isaudio_AudioSpatialDeviceType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioSpatialDeviceType
    }
    static isaudio_AudioSpatializationSceneType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioSpatializationSceneType
    }
    static isaudio_AudioState(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioState
    }
    static isaudio_AudioStreamDeviceChangeReason(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioStreamDeviceChangeReason
    }
    static isaudio_AudioVolumeType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_AudioVolumeType
    }
    static isaudio_ChannelBlendMode(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_ChannelBlendMode
    }
    static isaudio_CommunicationDeviceType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_CommunicationDeviceType
    }
    static isaudio_ConnectType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_ConnectType
    }
    static isaudio_ContentType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_ContentType
    }
    static isaudio_DeviceBlockStatus(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_DeviceBlockStatus
    }
    static isaudio_DeviceChangeType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_DeviceChangeType
    }
    static isaudio_DeviceFlag(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_DeviceFlag
    }
    static isaudio_DeviceRole(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_DeviceRole
    }
    static isaudio_DeviceType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_DeviceType
    }
    static isaudio_DeviceUsage(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_DeviceUsage
    }
    static isaudio_InterruptActionType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_InterruptActionType
    }
    static isaudio_InterruptForceType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_InterruptForceType
    }
    static isaudio_InterruptHint(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_InterruptHint
    }
    static isaudio_InterruptMode(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_InterruptMode
    }
    static isaudio_InterruptRequestResultType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_InterruptRequestResultType
    }
    static isaudio_InterruptRequestType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_InterruptRequestType
    }
    static isaudio_InterruptType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_InterruptType
    }
    static isaudio_PolicyType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_PolicyType
    }
    static isaudio_SourceType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_SourceType
    }
    static isaudio_StreamUsage(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_StreamUsage
    }
    static isaudio_ToneType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_ToneType
    }
    static isaudio_VolumeAdjustType(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_VolumeAdjustType
    }
    static isaudio_VolumeFlag(value: object|string|number|undefined|null): boolean {
        return value instanceof audio_VolumeFlag
    }
    static isaudioAsrProcessingController(value: object|string|number|undefined|null): boolean {
        return value instanceof audio.AsrProcessingController
    }
    static isaudioAudioCapturer(value: object|string|number|undefined|null, arg0: boolean): boolean {
        return value instanceof audio.AudioCapturer
    }
    static isaudioAudioCapturerChangeInfo(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean, arg2: boolean, arg3: boolean, arg4: boolean, arg5: boolean): boolean {
        return value instanceof audio.AudioCapturerChangeInfo
    }
    static isaudioAudioCapturerFilter(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean): boolean {
        return value instanceof audio.AudioCapturerFilter
    }
    static isaudioAudioCapturerInfo(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean): boolean {
        return value instanceof audio.AudioCapturerInfo
    }
    static isaudioAudioCapturerOptions(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean, arg2: boolean): boolean {
        return value instanceof audio.AudioCapturerOptions
    }
    static isaudioAudioDeviceDescriptor(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean, arg2: boolean, arg3: boolean, arg4: boolean, arg5: boolean, arg6: boolean, arg7: boolean, arg8: boolean, arg9: boolean, arg10: boolean, arg11: boolean, arg12: boolean): boolean {
        return value instanceof audio.AudioDeviceDescriptor
    }
    static isaudioAudioInterrupt(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean, arg2: boolean): boolean {
        return value instanceof audio.AudioInterrupt
    }
    static isaudioAudioManager(value: object|string|number|undefined|null): boolean {
        return value instanceof audio.AudioManager
    }
    static isaudioAudioPlaybackCaptureConfig(value: object|string|number|undefined|null, arg0: boolean): boolean {
        return value instanceof audio.AudioPlaybackCaptureConfig
    }
    static isaudioAudioRenderer(value: object|string|number|undefined|null, arg0: boolean): boolean {
        return value instanceof audio.AudioRenderer
    }
    static isaudioAudioRendererChangeInfo(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean, arg2: boolean, arg3: boolean, arg4: boolean): boolean {
        return value instanceof audio.AudioRendererChangeInfo
    }
    static isaudioAudioRendererFilter(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean, arg2: boolean): boolean {
        return value instanceof audio.AudioRendererFilter
    }
    static isaudioAudioRendererInfo(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean, arg2: boolean): boolean {
        return value instanceof audio.AudioRendererInfo
    }
    static isaudioAudioRendererOptions(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean, arg2: boolean): boolean {
        return value instanceof audio.AudioRendererOptions
    }
    static isaudioAudioRoutingManager(value: object|string|number|undefined|null): boolean {
        return value instanceof audio.AudioRoutingManager
    }
    static isaudioAudioSessionDeactivatedEvent(value: object|string|number|undefined|null, arg0: boolean): boolean {
        return value instanceof audio.AudioSessionDeactivatedEvent
    }
    static isaudioAudioSessionManager(value: object|string|number|undefined|null): boolean {
        return value instanceof audio.AudioSessionManager
    }
    static isaudioAudioSessionStrategy(value: object|string|number|undefined|null, arg0: boolean): boolean {
        return value instanceof audio.AudioSessionStrategy
    }
    static isaudioAudioSpatialDeviceState(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean, arg2: boolean, arg3: boolean): boolean {
        return value instanceof audio.AudioSpatialDeviceState
    }
    static isaudioAudioSpatialEnabledStateForDevice(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean): boolean {
        return value instanceof audio.AudioSpatialEnabledStateForDevice
    }
    static isaudioAudioSpatializationManager(value: object|string|number|undefined|null): boolean {
        return value instanceof audio.AudioSpatializationManager
    }
    static isaudioAudioStreamDeviceChangeInfo(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean): boolean {
        return value instanceof audio.AudioStreamDeviceChangeInfo
    }
    static isaudioAudioStreamInfo(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean, arg2: boolean, arg3: boolean, arg4: boolean): boolean {
        return value instanceof audio.AudioStreamInfo
    }
    static isaudioAudioStreamManager(value: object|string|number|undefined|null): boolean {
        return value instanceof audio.AudioStreamManager
    }
    static isaudioAudioVolumeGroupManager(value: object|string|number|undefined|null): boolean {
        return value instanceof audio.AudioVolumeGroupManager
    }
    static isaudioAudioVolumeManager(value: object|string|number|undefined|null): boolean {
        return value instanceof audio.AudioVolumeManager
    }
    static isaudioCaptureFilterOptions(value: object|string|number|undefined|null, arg0: boolean): boolean {
        return value instanceof audio.CaptureFilterOptions
    }
    static isaudioDeviceBlockStatusInfo(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean): boolean {
        return value instanceof audio.DeviceBlockStatusInfo
    }
    static isaudioDeviceChangeAction(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean): boolean {
        return value instanceof audio.DeviceChangeAction
    }
    static isaudioInterruptAction(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean, arg2: boolean, arg3: boolean): boolean {
        return value instanceof audio.InterruptAction
    }
    static isaudioInterruptEvent(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean, arg2: boolean): boolean {
        return value instanceof audio.InterruptEvent
    }
    static isaudioInterruptResult(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean): boolean {
        return value instanceof audio.InterruptResult
    }
    static isaudioMicStateChangeEvent(value: object|string|number|undefined|null, arg0: boolean): boolean {
        return value instanceof audio.MicStateChangeEvent
    }
    static isaudioTonePlayer(value: object|string|number|undefined|null): boolean {
        return value instanceof audio.TonePlayer
    }
    static isaudioVolumeEvent(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean, arg2: boolean, arg3: boolean, arg4: boolean): boolean {
        return value instanceof audio.VolumeEvent
    }
    static isaudioVolumeGroupInfo(value: object|string|number|undefined|null, arg0: boolean, arg1: boolean, arg2: boolean, arg3: boolean, arg4: boolean): boolean {
        return value instanceof audio.VolumeGroupInfo
    }
    static isArray_String(value: object|string|number|undefined|null): boolean {
        return value instanceof Array<string>
    }
    static isArray_CustomObject<VolumeGroupInfo>(value: object|string|number|undefined|null): boolean {
        return value instanceof Array<Readonly<VolumeGroupInfo>>
    }
    static isArray_Number(value: object|string|number|undefined|null): boolean {
        return value instanceof Array<number>
    }
    static isArray_audio_AudioEncodingType(value: object|string|number|undefined|null): boolean {
        return value instanceof Array<audio_AudioEncodingType>
    }
    static isArray_audio_StreamUsage(value: object|string|number|undefined|null): boolean {
        return value instanceof Array<audio_StreamUsage>
    }
}