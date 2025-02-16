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

import { SerializerBase, Tags, RuntimeType, runtimeType, isResource, isInstanceOf, nullptr, KPointer } from "@koalaui/interop"
import { int32, float32, int64 } from "@koalaui/common"
import { unsafeCast } from "./../shared/generated-utils"
import { MaterializedBase } from "./../MaterializedBase"
import { TypeChecker } from "#components"
import { KUint8ArrayPtr, NativeBuffer, InteropNativeModule } from "@koalaui/interop"
import { audio, audio_SourceType, audio_StreamUsage, audio_ContentType, audio_AudioChannelLayout, audio_AudioEncodingType, audio_AudioSampleFormat, audio_AudioChannel, audio_AudioSamplingRate, audio_AudioSpatialDeviceType, audio_DeviceType, audio_DeviceRole, audio_AudioConcurrencyMode, audio_AudioState, audio_ToneType, audio_AsrWhisperDetectionMode, audio_AsrVoiceMuteMode, audio_AsrVoiceControlMode, audio_AsrNoiseSuppressionMode, audio_AsrAecMode, audio_AudioDataCallbackResult, audio_ChannelBlendMode, audio_InterruptMode, audio_AudioRendererRate, audio_AudioEffectMode, audio_AudioStreamDeviceChangeReason, audio_ConnectType, audio_AudioSpatializationSceneType, audio_AudioVolumeType, audio_VolumeAdjustType, audio_PolicyType, audio_AudioRingMode, audio_AudioSessionDeactivatedReason, audio_CommunicationDeviceType, audio_DeviceUsage, audio_DeviceFlag, audio_DeviceBlockStatus, audio_InterruptRequestResultType, audio_InterruptHint, audio_InterruptType, audio_InterruptActionType, audio_DeviceChangeType, audio_AudioScene, audio_ActiveDeviceType, audio_InterruptForceType, audio_AudioPrivacyType, audio_VolumeFlag, audio_InterruptRequestType, audio_AudioErrors } from "./../OHAudioNamespace"
export class Serializer extends SerializerBase {
    private static pool?: Array<Serializer> | undefined = undefined
    private static poolTop: int32 = -1
    static hold(): Serializer {
        if (!(Serializer.pool != undefined))
        {
            Serializer.pool = new Array<Serializer>(8)
            const pool : Array<Serializer> = (Serializer.pool)!
            for (let idx = 0; idx < 8; idx++) {
                pool[idx] = new Serializer()
            }
        }
        const pool : Array<Serializer> = (Serializer.pool)!
        if (Serializer.poolTop >= pool.length - 1)
        {
            throw new Error("Serializer pool is full. Check if you had released serializers before")
        }
        Serializer.poolTop = Serializer.poolTop + 1
        let serializer  = pool[Serializer.poolTop]
        return serializer
    }
    public release(): void {
        if (Serializer.poolTop == -1)
        {
            throw new Error("Serializer pool is empty. Check if you had hold serializers before")
        }
        const pool : Array<Serializer> = (Serializer.pool)!
        if ((this) == (pool[Serializer.poolTop]))
        {
            Serializer.poolTop = Serializer.poolTop - 1
            super.release()
            return
        }
        throw new Error("Only last serializer should be released")
    }
     constructor() {
        super()
    }
    writeCaptureFilterOptions(value: audio.CaptureFilterOptions): void {
        let valueSerializer : Serializer = this
        const value_usages  = value.usages
        valueSerializer.writeInt32(value_usages.length as int32)
        for (let i = 0; i < value_usages.length; i++) {
            const value_usages_element : audio_StreamUsage = value_usages[i]
            valueSerializer.writeInt32((value_usages_element.valueOf() as int32))
        }
    }
    writeAudioCapturerInfo(value: audio.AudioCapturerInfo): void {
        let valueSerializer : Serializer = this
        const value_source  = value.source
        valueSerializer.writeInt32((value_source.valueOf() as int32))
        const value_capturerFlags  = value.capturerFlags
        valueSerializer.writeNumber(value_capturerFlags)
    }
    writeAudioRendererInfo(value: audio.AudioRendererInfo): void {
        let valueSerializer : Serializer = this
        const value_content  = value.content
        let value_content_type : int32 = RuntimeType.UNDEFINED
        value_content_type = runtimeType(value_content)
        valueSerializer.writeInt8(value_content_type as int32)
        if ((RuntimeType.UNDEFINED) != (value_content_type)) {
            const value_content_value  = (value_content as audio_ContentType)
            valueSerializer.writeInt32((value_content_value.valueOf() as int32))
        }
        const value_usage  = value.usage
        valueSerializer.writeInt32((value_usage.valueOf() as int32))
        const value_rendererFlags  = value.rendererFlags
        valueSerializer.writeNumber(value_rendererFlags)
    }
    writeAudioPlaybackCaptureConfig(value: audio.AudioPlaybackCaptureConfig): void {
        let valueSerializer : Serializer = this
        const value_filterOptions  = value.filterOptions
        valueSerializer.writeCaptureFilterOptions(value_filterOptions)
    }
    writeAudioStreamInfo(value: audio.AudioStreamInfo): void {
        let valueSerializer : Serializer = this
        const value_samplingRate  = value.samplingRate
        valueSerializer.writeInt32((value_samplingRate.valueOf() as int32))
        const value_channels  = value.channels
        valueSerializer.writeInt32((value_channels.valueOf() as int32))
        const value_sampleFormat  = value.sampleFormat
        valueSerializer.writeInt32((value_sampleFormat.valueOf() as int32))
        const value_encodingType  = value.encodingType
        valueSerializer.writeInt32((value_encodingType.valueOf() as int32))
        const value_channelLayout  = value.channelLayout
        let value_channelLayout_type : int32 = RuntimeType.UNDEFINED
        value_channelLayout_type = runtimeType(value_channelLayout)
        valueSerializer.writeInt8(value_channelLayout_type as int32)
        if ((RuntimeType.UNDEFINED) != (value_channelLayout_type)) {
            const value_channelLayout_value  = (value_channelLayout as audio_AudioChannelLayout)
            valueSerializer.writeInt32((value_channelLayout_value.valueOf() as int32))
        }
    }
    writeAudioSpatialDeviceState(value: audio.AudioSpatialDeviceState): void {
        let valueSerializer : Serializer = this
        const value_address  = value.address
        valueSerializer.writeString(value_address)
        const value_isSpatializationSupported  = value.isSpatializationSupported
        valueSerializer.writeBoolean(value_isSpatializationSupported)
        const value_isHeadTrackingSupported  = value.isHeadTrackingSupported
        valueSerializer.writeBoolean(value_isHeadTrackingSupported)
        const value_spatialDeviceType  = value.spatialDeviceType
        valueSerializer.writeInt32((value_spatialDeviceType.valueOf() as int32))
    }
    writeAudioDeviceDescriptor(value: audio.AudioDeviceDescriptor): void {
        let valueSerializer : Serializer = this
        const value_deviceRole  = value.deviceRole
        valueSerializer.writeInt32((value_deviceRole.valueOf() as int32))
        const value_deviceType  = value.deviceType
        valueSerializer.writeInt32((value_deviceType.valueOf() as int32))
        const value_id  = value.id
        valueSerializer.writeNumber(value_id)
        const value_name  = value.name
        valueSerializer.writeString(value_name)
        const value_address  = value.address
        valueSerializer.writeString(value_address)
        const value_sampleRates  = value.sampleRates
        valueSerializer.writeInt32(value_sampleRates.length as int32)
        for (let i = 0; i < value_sampleRates.length; i++) {
            const value_sampleRates_element : number = value_sampleRates[i]
            valueSerializer.writeNumber(value_sampleRates_element)
        }
        const value_channelCounts  = value.channelCounts
        valueSerializer.writeInt32(value_channelCounts.length as int32)
        for (let i = 0; i < value_channelCounts.length; i++) {
            const value_channelCounts_element : number = value_channelCounts[i]
            valueSerializer.writeNumber(value_channelCounts_element)
        }
        const value_channelMasks  = value.channelMasks
        valueSerializer.writeInt32(value_channelMasks.length as int32)
        for (let i = 0; i < value_channelMasks.length; i++) {
            const value_channelMasks_element : number = value_channelMasks[i]
            valueSerializer.writeNumber(value_channelMasks_element)
        }
        const value_networkId  = value.networkId
        valueSerializer.writeString(value_networkId)
        const value_interruptGroupId  = value.interruptGroupId
        valueSerializer.writeNumber(value_interruptGroupId)
        const value_volumeGroupId  = value.volumeGroupId
        valueSerializer.writeNumber(value_volumeGroupId)
        const value_displayName  = value.displayName
        valueSerializer.writeString(value_displayName)
        const value_encodingTypes  = value.encodingTypes
        let value_encodingTypes_type : int32 = RuntimeType.UNDEFINED
        value_encodingTypes_type = runtimeType(value_encodingTypes)
        valueSerializer.writeInt8(value_encodingTypes_type as int32)
        if ((RuntimeType.UNDEFINED) != (value_encodingTypes_type)) {
            const value_encodingTypes_value  = value_encodingTypes!
            valueSerializer.writeInt32(value_encodingTypes_value.length as int32)
            for (let i = 0; i < value_encodingTypes_value.length; i++) {
                const value_encodingTypes_value_element : audio_AudioEncodingType = value_encodingTypes_value[i]
                valueSerializer.writeInt32((value_encodingTypes_value_element.valueOf() as int32))
            }
        }
    }
    writeAudioSessionStrategy(value: audio.AudioSessionStrategy): void {
        let valueSerializer : Serializer = this
        const value_concurrencyMode  = value.concurrencyMode
        valueSerializer.writeInt32((value_concurrencyMode.valueOf() as int32))
    }
    writeAudioCapturerFilter(value: audio.AudioCapturerFilter): void {
        let valueSerializer : Serializer = this
        const value_uid  = value.uid
        let value_uid_type : int32 = RuntimeType.UNDEFINED
        value_uid_type = runtimeType(value_uid)
        valueSerializer.writeInt8(value_uid_type as int32)
        if ((RuntimeType.UNDEFINED) != (value_uid_type)) {
            const value_uid_value  = value_uid!
            valueSerializer.writeNumber(value_uid_value)
        }
        const value_capturerInfo  = value.capturerInfo
        let value_capturerInfo_type : int32 = RuntimeType.UNDEFINED
        value_capturerInfo_type = runtimeType(value_capturerInfo)
        valueSerializer.writeInt8(value_capturerInfo_type as int32)
        if ((RuntimeType.UNDEFINED) != (value_capturerInfo_type)) {
            const value_capturerInfo_value  = value_capturerInfo!
            valueSerializer.writeAudioCapturerInfo(value_capturerInfo_value)
        }
    }
    writeAudioRendererFilter(value: audio.AudioRendererFilter): void {
        let valueSerializer : Serializer = this
        const value_uid  = value.uid
        let value_uid_type : int32 = RuntimeType.UNDEFINED
        value_uid_type = runtimeType(value_uid)
        valueSerializer.writeInt8(value_uid_type as int32)
        if ((RuntimeType.UNDEFINED) != (value_uid_type)) {
            const value_uid_value  = value_uid!
            valueSerializer.writeNumber(value_uid_value)
        }
        const value_rendererInfo  = value.rendererInfo
        let value_rendererInfo_type : int32 = RuntimeType.UNDEFINED
        value_rendererInfo_type = runtimeType(value_rendererInfo)
        valueSerializer.writeInt8(value_rendererInfo_type as int32)
        if ((RuntimeType.UNDEFINED) != (value_rendererInfo_type)) {
            const value_rendererInfo_value  = value_rendererInfo!
            valueSerializer.writeAudioRendererInfo(value_rendererInfo_value)
        }
        const value_rendererId  = value.rendererId
        let value_rendererId_type : int32 = RuntimeType.UNDEFINED
        value_rendererId_type = runtimeType(value_rendererId)
        valueSerializer.writeInt8(value_rendererId_type as int32)
        if ((RuntimeType.UNDEFINED) != (value_rendererId_type)) {
            const value_rendererId_value  = value_rendererId!
            valueSerializer.writeNumber(value_rendererId_value)
        }
    }
    writeAudioInterrupt(value: audio.AudioInterrupt): void {
        let valueSerializer : Serializer = this
        const value_streamUsage  = value.streamUsage
        valueSerializer.writeInt32((value_streamUsage.valueOf() as int32))
        const value_contentType  = value.contentType
        valueSerializer.writeInt32((value_contentType.valueOf() as int32))
        const value_pauseWhenDucked  = value.pauseWhenDucked
        valueSerializer.writeBoolean(value_pauseWhenDucked)
    }
    writeTonePlayer(value: audio.TonePlayer): void {
        let valueSerializer : Serializer = this
        if (TypeChecker.typeInstanceOf<MaterializedBase>(value, "peer"))
        {
            const base : MaterializedBase = TypeChecker.typeCast<MaterializedBase>(value)
            const peer  = base.getPeer()
            let ptr : KPointer = nullptr
            if (peer != undefined)
                ptr = peer.ptr
            valueSerializer.writePointer(ptr)
            return
        }
        else
        {
            throw new Error("Value is not a MaterializedBase instance!")
        }
    }
    writeAudioRenderer(value: audio.AudioRenderer): void {
        let valueSerializer : Serializer = this
        if (TypeChecker.typeInstanceOf<MaterializedBase>(value, "peer"))
        {
            const base : MaterializedBase = TypeChecker.typeCast<MaterializedBase>(value)
            const peer  = base.getPeer()
            let ptr : KPointer = nullptr
            if (peer != undefined)
                ptr = peer.ptr
            valueSerializer.writePointer(ptr)
            return
        }
        else
        {
            throw new Error("Value is not a MaterializedBase instance!")
        }
    }
    writeAudioCapturer(value: audio.AudioCapturer): void {
        let valueSerializer : Serializer = this
        if (TypeChecker.typeInstanceOf<MaterializedBase>(value, "peer"))
        {
            const base : MaterializedBase = TypeChecker.typeCast<MaterializedBase>(value)
            const peer  = base.getPeer()
            let ptr : KPointer = nullptr
            if (peer != undefined)
                ptr = peer.ptr
            valueSerializer.writePointer(ptr)
            return
        }
        else
        {
            throw new Error("Value is not a MaterializedBase instance!")
        }
    }
    writeAudioCapturerOptions(value: audio.AudioCapturerOptions): void {
        let valueSerializer : Serializer = this
        const value_streamInfo  = value.streamInfo
        valueSerializer.writeAudioStreamInfo(value_streamInfo)
        const value_capturerInfo  = value.capturerInfo
        valueSerializer.writeAudioCapturerInfo(value_capturerInfo)
        const value_playbackCaptureConfig  = value.playbackCaptureConfig
        let value_playbackCaptureConfig_type : int32 = RuntimeType.UNDEFINED
        value_playbackCaptureConfig_type = runtimeType(value_playbackCaptureConfig)
        valueSerializer.writeInt8(value_playbackCaptureConfig_type as int32)
        if ((RuntimeType.UNDEFINED) != (value_playbackCaptureConfig_type)) {
            const value_playbackCaptureConfig_value  = value_playbackCaptureConfig!
            valueSerializer.writeAudioPlaybackCaptureConfig(value_playbackCaptureConfig_value)
        }
    }
    writeAudioStreamDeviceChangeInfo(value: audio.AudioStreamDeviceChangeInfo): void {
        let valueSerializer : Serializer = this
        const value_devices  = value.devices
        valueSerializer.writeInt32(value_devices.length as int32)
        for (let i = 0; i < value_devices.length; i++) {
            const value_devices_element : Readonly<AudioDeviceDescriptor> = value_devices[i]
            valueSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", value_devices_element)
        }
        const value_changeReason  = value.changeReason
        valueSerializer.writeInt32((value_changeReason.valueOf() as int32))
    }
    writeAudioCapturerChangeInfo(value: audio.AudioCapturerChangeInfo): void {
        let valueSerializer : Serializer = this
        const value_streamId  = value.streamId
        valueSerializer.writeNumber(value_streamId)
        const value_clientUid  = value.clientUid
        valueSerializer.writeNumber(value_clientUid)
        const value_capturerInfo  = value.capturerInfo
        valueSerializer.writeAudioCapturerInfo(value_capturerInfo)
        const value_capturerState  = value.capturerState
        valueSerializer.writeInt32((value_capturerState.valueOf() as int32))
        const value_deviceDescriptors  = value.deviceDescriptors
        valueSerializer.writeInt32(value_deviceDescriptors.length as int32)
        for (let i = 0; i < value_deviceDescriptors.length; i++) {
            const value_deviceDescriptors_element : Readonly<AudioDeviceDescriptor> = value_deviceDescriptors[i]
            valueSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", value_deviceDescriptors_element)
        }
        const value_muted  = value.muted
        let value_muted_type : int32 = RuntimeType.UNDEFINED
        value_muted_type = runtimeType(value_muted)
        valueSerializer.writeInt8(value_muted_type as int32)
        if ((RuntimeType.UNDEFINED) != (value_muted_type)) {
            const value_muted_value  = value_muted!
            valueSerializer.writeBoolean(value_muted_value)
        }
    }
    writeAudioRendererChangeInfo(value: audio.AudioRendererChangeInfo): void {
        let valueSerializer : Serializer = this
        const value_streamId  = value.streamId
        valueSerializer.writeNumber(value_streamId)
        const value_clientUid  = value.clientUid
        valueSerializer.writeNumber(value_clientUid)
        const value_rendererInfo  = value.rendererInfo
        valueSerializer.writeAudioRendererInfo(value_rendererInfo)
        const value_rendererState  = value.rendererState
        valueSerializer.writeInt32((value_rendererState.valueOf() as int32))
        const value_deviceDescriptors  = value.deviceDescriptors
        valueSerializer.writeInt32(value_deviceDescriptors.length as int32)
        for (let i = 0; i < value_deviceDescriptors.length; i++) {
            const value_deviceDescriptors_element : Readonly<AudioDeviceDescriptor> = value_deviceDescriptors[i]
            valueSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", value_deviceDescriptors_element)
        }
    }
    writeVolumeGroupInfo(value: audio.VolumeGroupInfo): void {
        let valueSerializer : Serializer = this
        const value_networkId  = value.networkId
        valueSerializer.writeString(value_networkId)
        const value_groupId  = value.groupId
        valueSerializer.writeNumber(value_groupId)
        const value_mappingId  = value.mappingId
        valueSerializer.writeNumber(value_mappingId)
        const value_groupName  = value.groupName
        valueSerializer.writeString(value_groupName)
        const value_type  = value.type
        valueSerializer.writeInt32((value_type.valueOf() as int32))
    }
    writeAudioSpatialEnabledStateForDevice(value: audio.AudioSpatialEnabledStateForDevice): void {
        let valueSerializer : Serializer = this
        const value_deviceDescriptor  = value.deviceDescriptor
        valueSerializer.writeAudioDeviceDescriptor(value_deviceDescriptor)
        const value_enabled  = value.enabled
        valueSerializer.writeBoolean(value_enabled)
    }
    writeMicStateChangeEvent(value: audio.MicStateChangeEvent): void {
        let valueSerializer : Serializer = this
        const value_mute  = value.mute
        valueSerializer.writeBoolean(value_mute)
    }
    writeAudioVolumeGroupManager(value: audio.AudioVolumeGroupManager): void {
        let valueSerializer : Serializer = this
        if (TypeChecker.typeInstanceOf<MaterializedBase>(value, "peer"))
        {
            const base : MaterializedBase = TypeChecker.typeCast<MaterializedBase>(value)
            const peer  = base.getPeer()
            let ptr : KPointer = nullptr
            if (peer != undefined)
                ptr = peer.ptr
            valueSerializer.writePointer(ptr)
            return
        }
        else
        {
            throw new Error("Value is not a MaterializedBase instance!")
        }
    }
    writeAudioSessionDeactivatedEvent(value: audio.AudioSessionDeactivatedEvent): void {
        let valueSerializer : Serializer = this
        const value_reason  = value.reason
        valueSerializer.writeInt32((value_reason.valueOf() as int32))
    }
    writeDeviceBlockStatusInfo(value: audio.DeviceBlockStatusInfo): void {
        let valueSerializer : Serializer = this
        const value_blockStatus  = value.blockStatus
        valueSerializer.writeInt32((value_blockStatus.valueOf() as int32))
        const value_devices  = value.devices
        valueSerializer.writeInt32(value_devices.length as int32)
        for (let i = 0; i < value_devices.length; i++) {
            const value_devices_element : Readonly<AudioDeviceDescriptor> = value_devices[i]
            valueSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", value_devices_element)
        }
    }
    writeInterruptResult(value: audio.InterruptResult): void {
        let valueSerializer : Serializer = this
        const value_requestResult  = value.requestResult
        valueSerializer.writeInt32((value_requestResult.valueOf() as int32))
        const value_interruptNode  = value.interruptNode
        valueSerializer.writeNumber(value_interruptNode)
    }
    writeInterruptAction(value: audio.InterruptAction): void {
        let valueSerializer : Serializer = this
        const value_actionType  = value.actionType
        valueSerializer.writeInt32((value_actionType.valueOf() as int32))
        const value_type  = value.type
        let value_type_type : int32 = RuntimeType.UNDEFINED
        value_type_type = runtimeType(value_type)
        valueSerializer.writeInt8(value_type_type as int32)
        if ((RuntimeType.UNDEFINED) != (value_type_type)) {
            const value_type_value  = (value_type as audio_InterruptType)
            valueSerializer.writeInt32((value_type_value.valueOf() as int32))
        }
        const value_hint  = value.hint
        let value_hint_type : int32 = RuntimeType.UNDEFINED
        value_hint_type = runtimeType(value_hint)
        valueSerializer.writeInt8(value_hint_type as int32)
        if ((RuntimeType.UNDEFINED) != (value_hint_type)) {
            const value_hint_value  = (value_hint as audio_InterruptHint)
            valueSerializer.writeInt32((value_hint_value.valueOf() as int32))
        }
        const value_activated  = value.activated
        let value_activated_type : int32 = RuntimeType.UNDEFINED
        value_activated_type = runtimeType(value_activated)
        valueSerializer.writeInt8(value_activated_type as int32)
        if ((RuntimeType.UNDEFINED) != (value_activated_type)) {
            const value_activated_value  = value_activated!
            valueSerializer.writeBoolean(value_activated_value)
        }
    }
    writeDeviceChangeAction(value: audio.DeviceChangeAction): void {
        let valueSerializer : Serializer = this
        const value_type  = value.type
        valueSerializer.writeInt32((value_type.valueOf() as int32))
        const value_deviceDescriptors  = value.deviceDescriptors
        valueSerializer.writeInt32(value_deviceDescriptors.length as int32)
        for (let i = 0; i < value_deviceDescriptors.length; i++) {
            const value_deviceDescriptors_element : Readonly<AudioDeviceDescriptor> = value_deviceDescriptors[i]
            valueSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", value_deviceDescriptors_element)
        }
    }
    writeVolumeEvent(value: audio.VolumeEvent): void {
        let valueSerializer : Serializer = this
        const value_volumeType  = value.volumeType
        valueSerializer.writeInt32((value_volumeType.valueOf() as int32))
        const value_volume  = value.volume
        valueSerializer.writeNumber(value_volume)
        const value_updateUi  = value.updateUi
        valueSerializer.writeBoolean(value_updateUi)
        const value_volumeGroupId  = value.volumeGroupId
        valueSerializer.writeNumber(value_volumeGroupId)
        const value_networkId  = value.networkId
        valueSerializer.writeString(value_networkId)
    }
    writeInterruptEvent(value: audio.InterruptEvent): void {
        let valueSerializer : Serializer = this
        const value_eventType  = value.eventType
        valueSerializer.writeInt32((value_eventType.valueOf() as int32))
        const value_forceType  = value.forceType
        valueSerializer.writeInt32((value_forceType.valueOf() as int32))
        const value_hintType  = value.hintType
        valueSerializer.writeInt32((value_hintType.valueOf() as int32))
    }
    writeAudioRendererOptions(value: audio.AudioRendererOptions): void {
        let valueSerializer : Serializer = this
        const value_streamInfo  = value.streamInfo
        valueSerializer.writeAudioStreamInfo(value_streamInfo)
        const value_rendererInfo  = value.rendererInfo
        valueSerializer.writeAudioRendererInfo(value_rendererInfo)
        const value_privacyType  = value.privacyType
        let value_privacyType_type : int32 = RuntimeType.UNDEFINED
        value_privacyType_type = runtimeType(value_privacyType)
        valueSerializer.writeInt8(value_privacyType_type as int32)
        if ((RuntimeType.UNDEFINED) != (value_privacyType_type)) {
            const value_privacyType_value  = (value_privacyType as audio_AudioPrivacyType)
            valueSerializer.writeInt32((value_privacyType_value.valueOf() as int32))
        }
    }
}