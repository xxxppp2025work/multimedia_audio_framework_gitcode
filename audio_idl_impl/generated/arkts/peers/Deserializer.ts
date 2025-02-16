
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

import { KPointer, runtimeType, RuntimeType, CallbackResource, DeserializerBase } from "@koalaui/interop"
import { int32, float32, int64 } from "@koalaui/common"
import { Serializer } from "./Serializer"
import { AUDIONativeModule } from "#components"
import { CallbackKind } from "./CallbackKind"
import { TypeChecker } from "#components"
import { KUint8ArrayPtr, NativeBuffer, InteropNativeModule } from "@koalaui/interop"
import { audio, audio_SourceType, audio_StreamUsage, audio_ContentType, audio_AudioChannelLayout, audio_AudioEncodingType, audio_AudioSampleFormat, audio_AudioChannel, audio_AudioSamplingRate, audio_AudioSpatialDeviceType, audio_DeviceType, audio_DeviceRole, audio_AudioConcurrencyMode, audio_AudioState, audio_ToneType, audio_AsrWhisperDetectionMode, audio_AsrVoiceMuteMode, audio_AsrVoiceControlMode, audio_AsrNoiseSuppressionMode, audio_AsrAecMode, audio_AudioDataCallbackResult, audio_ChannelBlendMode, audio_InterruptMode, audio_AudioRendererRate, audio_AudioEffectMode, audio_AudioStreamDeviceChangeReason, audio_ConnectType, audio_AudioSpatializationSceneType, audio_AudioVolumeType, audio_VolumeAdjustType, audio_PolicyType, audio_AudioRingMode, audio_AudioSessionDeactivatedReason, audio_CommunicationDeviceType, audio_DeviceUsage, audio_DeviceFlag, audio_DeviceBlockStatus, audio_InterruptRequestResultType, audio_InterruptHint, audio_InterruptType, audio_InterruptActionType, audio_DeviceChangeType, audio_AudioScene, audio_ActiveDeviceType, audio_InterruptForceType, audio_AudioPrivacyType, audio_VolumeFlag, audio_InterruptRequestType, audio_AudioErrors } from "./../OHAudioNamespace"

export class Deserializer extends DeserializerBase {
     constructor(data: KUint8ArrayPtr, length: int32) {
        super(data, length)
    }
    readCaptureFilterOptions(): audio.CaptureFilterOptions {
        let valueDeserializer : Deserializer = this
        const usages_buf_length : int32 = valueDeserializer.readInt32()
        let usages_buf : Array<audio_StreamUsage> = new Array<audio_StreamUsage>()
        for (let usages_buf_i = 0; usages_buf_i < usages_buf_length; usages_buf_i++) {
            usages_buf[usages_buf_i] = (valueDeserializer.readInt32() as audio_StreamUsage)
        }
        const usages_result : Array<audio_StreamUsage> = usages_buf
        let value : audio.CaptureFilterOptions = ({usages: usages_result} as audio.CaptureFilterOptions)
        return value
    }
    readAudioCapturerInfo(): audio.AudioCapturerInfo {
        let valueDeserializer : Deserializer = this
        const source_result : audio_SourceType = (valueDeserializer.readInt32() as audio_SourceType)
        const capturerFlags_result : number = (valueDeserializer.readNumber() as number)
        let value : audio.AudioCapturerInfo = ({source: source_result,capturerFlags: capturerFlags_result} as audio.AudioCapturerInfo)
        return value
    }
    readAudioRendererInfo(): audio.AudioRendererInfo {
        let valueDeserializer : Deserializer = this
        const content_buf_runtimeType  = (valueDeserializer.readInt8() as int32)
        let content_buf : audio_ContentType | undefined
        if ((RuntimeType.UNDEFINED) != (content_buf_runtimeType))
        {
            content_buf = (valueDeserializer.readInt32() as audio_ContentType)
        }
        const content_result : audio_ContentType | undefined = content_buf
        const usage_result : audio_StreamUsage = (valueDeserializer.readInt32() as audio_StreamUsage)
        const rendererFlags_result : number = (valueDeserializer.readNumber() as number)
        let value : audio.AudioRendererInfo = ({content: content_result,usage: usage_result,rendererFlags: rendererFlags_result} as audio.AudioRendererInfo)
        return value
    }
    readAUDIO_AsyncCallback_Void(isSync: boolean = false): (() => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return ():void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
(isSync) ? (InteropNativeModule._CallCallbackSync(1075219926, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(1075219926, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_Callback_Buffer_Void(isSync: boolean = false): ((parameter: NativeBuffer) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: NativeBuffer):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeBuffer(parameter);
(isSync) ? (InteropNativeModule._CallCallbackSync(908731311, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(908731311, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_Callback_AudioCapturerChangeInfo_Void(isSync: boolean = false): ((parameter: audio.AudioCapturerChangeInfo) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: audio.AudioCapturerChangeInfo):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeAudioCapturerChangeInfo(parameter);
(isSync) ? (InteropNativeModule._CallCallbackSync(300211623, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(300211623, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_Callback_AudioDeviceDescriptors_Void(isSync: boolean = false): ((parameter: audio.AudioDeviceDescriptors) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: audio.AudioDeviceDescriptors):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeInt32(parameter.length as int32);
for (let i = 0; i < parameter.length; i++) {
    const parameter_element : Readonly<AudioDeviceDescriptor> = parameter[i];
    _argsSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", parameter_element);
}
(isSync) ? (InteropNativeModule._CallCallbackSync(-872298751, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(-872298751, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_Callback_InterruptEvent_Void(isSync: boolean = false): ((parameter: audio.InterruptEvent) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: audio.InterruptEvent):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeInterruptEvent(parameter);
(isSync) ? (InteropNativeModule._CallCallbackSync(638628164, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(638628164, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_Callback_AudioState_Void(isSync: boolean = false): ((parameter: audio_AudioState) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: audio_AudioState):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeInt32((parameter.valueOf() as int32));
(isSync) ? (InteropNativeModule._CallCallbackSync(558001102, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(558001102, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_Callback_Number_Void(isSync: boolean = false): ((parameter: number) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: number):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeNumber(parameter);
(isSync) ? (InteropNativeModule._CallCallbackSync(36519084, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(36519084, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_AsyncCallback_Number_Void(isSync: boolean = false): ((result: number) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: number):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeNumber(result);
(isSync) ? (InteropNativeModule._CallCallbackSync(1959553162, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(1959553162, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_AsyncCallback_Buffer_Void(isSync: boolean = false): ((result: NativeBuffer) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: NativeBuffer):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeBuffer(result);
(isSync) ? (InteropNativeModule._CallCallbackSync(-1662321143, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(-1662321143, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_AsyncCallback_AudioStreamInfo_Void(isSync: boolean = false): ((result: audio.AudioStreamInfo) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: audio.AudioStreamInfo):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeAudioStreamInfo(result);
(isSync) ? (InteropNativeModule._CallCallbackSync(-106617875, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(-106617875, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_AsyncCallback_AudioCapturerInfo_Void(isSync: boolean = false): ((result: audio.AudioCapturerInfo) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: audio.AudioCapturerInfo):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeAudioCapturerInfo(result);
(isSync) ? (InteropNativeModule._CallCallbackSync(945856903, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(945856903, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAudioPlaybackCaptureConfig(): audio.AudioPlaybackCaptureConfig {
        let valueDeserializer : Deserializer = this
        const filterOptions_result : audio.CaptureFilterOptions = valueDeserializer.readCaptureFilterOptions()
        let value : audio.AudioPlaybackCaptureConfig = ({filterOptions: filterOptions_result} as audio.AudioPlaybackCaptureConfig)
        return value
    }
    readAudioStreamInfo(): audio.AudioStreamInfo {
        let valueDeserializer : Deserializer = this
        const samplingRate_result : audio_AudioSamplingRate = (valueDeserializer.readInt32() as audio_AudioSamplingRate)
        const channels_result : audio_AudioChannel = (valueDeserializer.readInt32() as audio_AudioChannel)
        const sampleFormat_result : audio_AudioSampleFormat = (valueDeserializer.readInt32() as audio_AudioSampleFormat)
        const encodingType_result : audio_AudioEncodingType = (valueDeserializer.readInt32() as audio_AudioEncodingType)
        const channelLayout_buf_runtimeType  = (valueDeserializer.readInt8() as int32)
        let channelLayout_buf : audio_AudioChannelLayout | undefined
        if ((RuntimeType.UNDEFINED) != (channelLayout_buf_runtimeType))
        {
            channelLayout_buf = (valueDeserializer.readInt32() as audio_AudioChannelLayout)
        }
        const channelLayout_result : audio_AudioChannelLayout | undefined = channelLayout_buf
        let value : audio.AudioStreamInfo = ({samplingRate: samplingRate_result,channels: channels_result,sampleFormat: sampleFormat_result,encodingType: encodingType_result,channelLayout: channelLayout_result} as audio.AudioStreamInfo)
        return value
    }
    readAUDIO_AudioRendererWriteDataCallback(isSync: boolean = false): audio.AudioRendererWriteDataCallback {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (data: NativeBuffer):audio_AudioDataCallbackResult => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeBuffer(data);
let _continuationValue : audio_AudioDataCallbackResult | undefined;
const _continuationCallback : Callback_AudioDataCallbackResult_Void = (value: audio_AudioDataCallbackResult):void => { _continuationValue = value; }
_argsSerializer.writeCustomObject("Callback_AudioDataCallbackResult_Void", _continuationCallback);
(isSync) ? (InteropNativeModule._CallCallbackSync(-1508727875, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(-1508727875, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return (_continuationValue as audio_AudioDataCallbackResult); }
    }
    readAUDIO_Callback_AudioStreamDeviceChangeInfo_Void(isSync: boolean = false): ((parameter: audio.AudioStreamDeviceChangeInfo) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: audio.AudioStreamDeviceChangeInfo):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeAudioStreamDeviceChangeInfo(parameter);
(isSync) ? (InteropNativeModule._CallCallbackSync(1823460565, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(1823460565, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_AsyncCallback_AudioDeviceDescriptors_Void(isSync: boolean = false): ((result: audio.AudioDeviceDescriptors) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: audio.AudioDeviceDescriptors):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeInt32(result.length as int32);
for (let i = 0; i < result.length; i++) {
    const result_element : Readonly<AudioDeviceDescriptor> = result[i];
    _argsSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", result_element);
}
(isSync) ? (InteropNativeModule._CallCallbackSync(1103131879, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(1103131879, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_AsyncCallback_AudioRendererRate_Void(isSync: boolean = false): ((result: audio_AudioRendererRate) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: audio_AudioRendererRate):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeInt32((result.valueOf() as int32));
(isSync) ? (InteropNativeModule._CallCallbackSync(147576480, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(147576480, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_AsyncCallback_AudioEffectMode_Void(isSync: boolean = false): ((result: audio_AudioEffectMode) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: audio_AudioEffectMode):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeInt32((result.valueOf() as int32));
(isSync) ? (InteropNativeModule._CallCallbackSync(-852719253, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(-852719253, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_AsyncCallback_AudioRendererInfo_Void(isSync: boolean = false): ((result: audio.AudioRendererInfo) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: audio.AudioRendererInfo):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeAudioRendererInfo(result);
(isSync) ? (InteropNativeModule._CallCallbackSync(721972620, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(721972620, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAudioSpatialDeviceState(): audio.AudioSpatialDeviceState {
        let valueDeserializer : Deserializer = this
        const address_result : string = (valueDeserializer.readString() as string)
        const isSpatializationSupported_result : boolean = valueDeserializer.readBoolean()
        const isHeadTrackingSupported_result : boolean = valueDeserializer.readBoolean()
        const spatialDeviceType_result : audio_AudioSpatialDeviceType = (valueDeserializer.readInt32() as audio_AudioSpatialDeviceType)
        let value : audio.AudioSpatialDeviceState = ({address: address_result,isSpatializationSupported: isSpatializationSupported_result,isHeadTrackingSupported: isHeadTrackingSupported_result,spatialDeviceType: spatialDeviceType_result} as audio.AudioSpatialDeviceState)
        return value
    }
    readAUDIO_Callback_AudioSpatialEnabledStateForDevice_Void(isSync: boolean = false): ((parameter: audio.AudioSpatialEnabledStateForDevice) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: audio.AudioSpatialEnabledStateForDevice):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeAudioSpatialEnabledStateForDevice(parameter);
(isSync) ? (InteropNativeModule._CallCallbackSync(1057396442, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(1057396442, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_Callback_Boolean_Void(isSync: boolean = false): ((parameter: boolean) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: boolean):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeBoolean(parameter);
(isSync) ? (InteropNativeModule._CallCallbackSync(313269291, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(313269291, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAudioDeviceDescriptor(): audio.AudioDeviceDescriptor {
        let valueDeserializer : Deserializer = this
        const deviceRole_result : audio_DeviceRole = (valueDeserializer.readInt32() as audio_DeviceRole)
        const deviceType_result : audio_DeviceType = (valueDeserializer.readInt32() as audio_DeviceType)
        const id_result : number = (valueDeserializer.readNumber() as number)
        const name_result : string = (valueDeserializer.readString() as string)
        const address_result : string = (valueDeserializer.readString() as string)
        const sampleRates_buf_length : int32 = valueDeserializer.readInt32()
        let sampleRates_buf : Array<number> = new Array<number>()
        for (let sampleRates_buf_i = 0; sampleRates_buf_i < sampleRates_buf_length; sampleRates_buf_i++) {
            sampleRates_buf[sampleRates_buf_i] = (valueDeserializer.readNumber() as number)
        }
        const sampleRates_result : Array<number> = sampleRates_buf
        const channelCounts_buf_length : int32 = valueDeserializer.readInt32()
        let channelCounts_buf : Array<number> = new Array<number>()
        for (let channelCounts_buf_i = 0; channelCounts_buf_i < channelCounts_buf_length; channelCounts_buf_i++) {
            channelCounts_buf[channelCounts_buf_i] = (valueDeserializer.readNumber() as number)
        }
        const channelCounts_result : Array<number> = channelCounts_buf
        const channelMasks_buf_length : int32 = valueDeserializer.readInt32()
        let channelMasks_buf : Array<number> = new Array<number>()
        for (let channelMasks_buf_i = 0; channelMasks_buf_i < channelMasks_buf_length; channelMasks_buf_i++) {
            channelMasks_buf[channelMasks_buf_i] = (valueDeserializer.readNumber() as number)
        }
        const channelMasks_result : Array<number> = channelMasks_buf
        const networkId_result : string = (valueDeserializer.readString() as string)
        const interruptGroupId_result : number = (valueDeserializer.readNumber() as number)
        const volumeGroupId_result : number = (valueDeserializer.readNumber() as number)
        const displayName_result : string = (valueDeserializer.readString() as string)
        const encodingTypes_buf_runtimeType  = (valueDeserializer.readInt8() as int32)
        let encodingTypes_buf : Array<audio_AudioEncodingType> | undefined
        if ((RuntimeType.UNDEFINED) != (encodingTypes_buf_runtimeType))
        {
            const encodingTypes_buf__length : int32 = valueDeserializer.readInt32()
            let encodingTypes_buf_ : Array<audio_AudioEncodingType> = new Array<audio_AudioEncodingType>()
            for (let encodingTypes_buf__i = 0; encodingTypes_buf__i < encodingTypes_buf__length; encodingTypes_buf__i++) {
                encodingTypes_buf_[encodingTypes_buf__i] = (valueDeserializer.readInt32() as audio_AudioEncodingType)
            }
            encodingTypes_buf = encodingTypes_buf_
        }
        const encodingTypes_result : Array<audio_AudioEncodingType> | undefined = encodingTypes_buf
        let value : audio.AudioDeviceDescriptor = ({deviceRole: deviceRole_result,deviceType: deviceType_result,id: id_result,name: name_result,address: address_result,sampleRates: sampleRates_result,channelCounts: channelCounts_result,channelMasks: channelMasks_result,networkId: networkId_result,interruptGroupId: interruptGroupId_result,volumeGroupId: volumeGroupId_result,displayName: displayName_result,encodingTypes: encodingTypes_result} as audio.AudioDeviceDescriptor)
        return value
    }
    readAUDIO_Callback_MicStateChangeEvent_Void(isSync: boolean = false): ((parameter: audio.MicStateChangeEvent) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: audio.MicStateChangeEvent):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeMicStateChangeEvent(parameter);
(isSync) ? (InteropNativeModule._CallCallbackSync(-1553290571, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(-1553290571, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_AsyncCallback_Boolean_Void(isSync: boolean = false): ((result: boolean) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: boolean):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeBoolean(result);
(isSync) ? (InteropNativeModule._CallCallbackSync(46391693, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(46391693, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_Callback_AudioRingMode_Void(isSync: boolean = false): ((parameter: audio_AudioRingMode) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: audio_AudioRingMode):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeInt32((parameter.valueOf() as int32));
(isSync) ? (InteropNativeModule._CallCallbackSync(-1700902488, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(-1700902488, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_AsyncCallback_AudioRingMode_Void(isSync: boolean = false): ((result: audio_AudioRingMode) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: audio_AudioRingMode):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeInt32((result.valueOf() as int32));
(isSync) ? (InteropNativeModule._CallCallbackSync(-815244914, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(-815244914, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_Callback_VolumeEvent_Void(isSync: boolean = false): ((parameter: audio.VolumeEvent) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: audio.VolumeEvent):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeVolumeEvent(parameter);
(isSync) ? (InteropNativeModule._CallCallbackSync(311503723, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(311503723, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_AsyncCallback_AudioVolumeGroupManager_Void(isSync: boolean = false): ((result: audio.AudioVolumeGroupManager) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: audio.AudioVolumeGroupManager):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeAudioVolumeGroupManager(result);
(isSync) ? (InteropNativeModule._CallCallbackSync(540266043, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(540266043, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_AsyncCallback_VolumeGroupInfos_Void(isSync: boolean = false): ((result: audio.VolumeGroupInfos) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: audio.VolumeGroupInfos):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeInt32(result.length as int32);
for (let i = 0; i < result.length; i++) {
    const result_element : Readonly<VolumeGroupInfo> = result[i];
    _argsSerializer.writeCustomObject("Readonly<VolumeGroupInfo>", result_element);
}
(isSync) ? (InteropNativeModule._CallCallbackSync(-801079837, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(-801079837, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_Callback_AudioSessionDeactivatedEvent_Void(isSync: boolean = false): ((parameter: audio.AudioSessionDeactivatedEvent) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: audio.AudioSessionDeactivatedEvent):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeAudioSessionDeactivatedEvent(parameter);
(isSync) ? (InteropNativeModule._CallCallbackSync(1744071031, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(1744071031, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAudioSessionStrategy(): audio.AudioSessionStrategy {
        let valueDeserializer : Deserializer = this
        const concurrencyMode_result : audio_AudioConcurrencyMode = (valueDeserializer.readInt32() as audio_AudioConcurrencyMode)
        let value : audio.AudioSessionStrategy = ({concurrencyMode: concurrencyMode_result} as audio.AudioSessionStrategy)
        return value
    }
    readAUDIO_Callback_AudioCapturerChangeInfoArray_Void(isSync: boolean = false): ((parameter: audio.AudioCapturerChangeInfoArray) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: audio.AudioCapturerChangeInfoArray):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeInt32(parameter.length as int32);
for (let i = 0; i < parameter.length; i++) {
    const parameter_element : Readonly<AudioCapturerChangeInfo> = parameter[i];
    _argsSerializer.writeCustomObject("Readonly<AudioCapturerChangeInfo>", parameter_element);
}
(isSync) ? (InteropNativeModule._CallCallbackSync(-788037890, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(-788037890, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_Callback_AudioRendererChangeInfoArray_Void(isSync: boolean = false): ((parameter: audio.AudioRendererChangeInfoArray) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: audio.AudioRendererChangeInfoArray):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeInt32(parameter.length as int32);
for (let i = 0; i < parameter.length; i++) {
    const parameter_element : Readonly<AudioRendererChangeInfo> = parameter[i];
    _argsSerializer.writeCustomObject("Readonly<AudioRendererChangeInfo>", parameter_element);
}
(isSync) ? (InteropNativeModule._CallCallbackSync(1609768789, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(1609768789, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_AsyncCallback_AudioEffectInfoArray_Void(isSync: boolean = false): ((result: audio.AudioEffectInfoArray) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: audio.AudioEffectInfoArray):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeInt32(result.length as int32);
for (let i = 0; i < result.length; i++) {
    const result_element : Readonly<AudioEffectMode> = result[i];
    _argsSerializer.writeCustomObject("Readonly<AudioEffectMode>", result_element);
}
(isSync) ? (InteropNativeModule._CallCallbackSync(-1801809985, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(-1801809985, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void(isSync: boolean = false): ((result: audio.AudioCapturerChangeInfoArray) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: audio.AudioCapturerChangeInfoArray):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeInt32(result.length as int32);
for (let i = 0; i < result.length; i++) {
    const result_element : Readonly<AudioCapturerChangeInfo> = result[i];
    _argsSerializer.writeCustomObject("Readonly<AudioCapturerChangeInfo>", result_element);
}
(isSync) ? (InteropNativeModule._CallCallbackSync(605900120, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(605900120, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void(isSync: boolean = false): ((result: audio.AudioRendererChangeInfoArray) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: audio.AudioRendererChangeInfoArray):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeInt32(result.length as int32);
for (let i = 0; i < result.length; i++) {
    const result_element : Readonly<AudioRendererChangeInfo> = result[i];
    _argsSerializer.writeCustomObject("Readonly<AudioRendererChangeInfo>", result_element);
}
(isSync) ? (InteropNativeModule._CallCallbackSync(419082675, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(419082675, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_Callback_DeviceBlockStatusInfo_Void(isSync: boolean = false): ((parameter: audio.DeviceBlockStatusInfo) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: audio.DeviceBlockStatusInfo):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeDeviceBlockStatusInfo(parameter);
(isSync) ? (InteropNativeModule._CallCallbackSync(-761620636, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(-761620636, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAudioCapturerFilter(): audio.AudioCapturerFilter {
        let valueDeserializer : Deserializer = this
        const uid_buf_runtimeType  = (valueDeserializer.readInt8() as int32)
        let uid_buf : number | undefined
        if ((RuntimeType.UNDEFINED) != (uid_buf_runtimeType))
        {
            uid_buf = (valueDeserializer.readNumber() as number)
        }
        const uid_result : number | undefined = uid_buf
        const capturerInfo_buf_runtimeType  = (valueDeserializer.readInt8() as int32)
        let capturerInfo_buf : audio.AudioCapturerInfo | undefined
        if ((RuntimeType.UNDEFINED) != (capturerInfo_buf_runtimeType))
        {
            capturerInfo_buf = valueDeserializer.readAudioCapturerInfo()
        }
        const capturerInfo_result : audio.AudioCapturerInfo | undefined = capturerInfo_buf
        let value : audio.AudioCapturerFilter = ({uid: uid_result,capturerInfo: capturerInfo_result} as audio.AudioCapturerFilter)
        return value
    }
    readAudioRendererFilter(): audio.AudioRendererFilter {
        let valueDeserializer : Deserializer = this
        const uid_buf_runtimeType  = (valueDeserializer.readInt8() as int32)
        let uid_buf : number | undefined
        if ((RuntimeType.UNDEFINED) != (uid_buf_runtimeType))
        {
            uid_buf = (valueDeserializer.readNumber() as number)
        }
        const uid_result : number | undefined = uid_buf
        const rendererInfo_buf_runtimeType  = (valueDeserializer.readInt8() as int32)
        let rendererInfo_buf : audio.AudioRendererInfo | undefined
        if ((RuntimeType.UNDEFINED) != (rendererInfo_buf_runtimeType))
        {
            rendererInfo_buf = valueDeserializer.readAudioRendererInfo()
        }
        const rendererInfo_result : audio.AudioRendererInfo | undefined = rendererInfo_buf
        const rendererId_buf_runtimeType  = (valueDeserializer.readInt8() as int32)
        let rendererId_buf : number | undefined
        if ((RuntimeType.UNDEFINED) != (rendererId_buf_runtimeType))
        {
            rendererId_buf = (valueDeserializer.readNumber() as number)
        }
        const rendererId_result : number | undefined = rendererId_buf
        let value : audio.AudioRendererFilter = ({uid: uid_result,rendererInfo: rendererInfo_result,rendererId: rendererId_result} as audio.AudioRendererFilter)
        return value
    }
    readAUDIO_Callback_DeviceChangeAction_Void(isSync: boolean = false): ((parameter: audio.DeviceChangeAction) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: audio.DeviceChangeAction):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeDeviceChangeAction(parameter);
(isSync) ? (InteropNativeModule._CallCallbackSync(892121871, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(892121871, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_Callback_InterruptAction_Void(isSync: boolean = false): ((parameter: audio.InterruptAction) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (parameter: audio.InterruptAction):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeInterruptAction(parameter);
(isSync) ? (InteropNativeModule._CallCallbackSync(-1389231466, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(-1389231466, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAudioInterrupt(): audio.AudioInterrupt {
        let valueDeserializer : Deserializer = this
        const streamUsage_result : audio_StreamUsage = (valueDeserializer.readInt32() as audio_StreamUsage)
        const contentType_result : audio_ContentType = (valueDeserializer.readInt32() as audio_ContentType)
        const pauseWhenDucked_result : boolean = valueDeserializer.readBoolean()
        let value : audio.AudioInterrupt = ({streamUsage: streamUsage_result,contentType: contentType_result,pauseWhenDucked: pauseWhenDucked_result} as audio.AudioInterrupt)
        return value
    }
    readAUDIO_AsyncCallback_AudioScene_Void(isSync: boolean = false): ((result: audio_AudioScene) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: audio_AudioScene):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeInt32((result.valueOf() as int32));
(isSync) ? (InteropNativeModule._CallCallbackSync(-854078751, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(-854078751, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAUDIO_AsyncCallback_String_Void(isSync: boolean = false): ((result: string) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: string):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeString(result);
(isSync) ? (InteropNativeModule._CallCallbackSync(789188988, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(789188988, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readTonePlayer(): audio.TonePlayer {
        let valueDeserializer : Deserializer = this
        let ptr : KPointer = valueDeserializer.readPointer()
        return audio.TonePlayerInternal.fromPtr(ptr)
    }
    readAUDIO_AsyncCallback_TonePlayer_Void(isSync: boolean = false): ((result: audio.TonePlayer) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: audio.TonePlayer):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeTonePlayer(result);
(isSync) ? (InteropNativeModule._CallCallbackSync(895924586, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(895924586, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAudioRenderer(): audio.AudioRenderer {
        let valueDeserializer : Deserializer = this
        let ptr : KPointer = valueDeserializer.readPointer()
        return audio.AudioRendererInternal.fromPtr(ptr)
    }
    readAUDIO_AsyncCallback_AudioRenderer_Void(isSync: boolean = false): ((result: audio.AudioRenderer) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: audio.AudioRenderer):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeAudioRenderer(result);
(isSync) ? (InteropNativeModule._CallCallbackSync(-1795948094, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(-1795948094, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAudioCapturer(): audio.AudioCapturer {
        let valueDeserializer : Deserializer = this
        let ptr : KPointer = valueDeserializer.readPointer()
        return audio.AudioCapturerInternal.fromPtr(ptr)
    }
    readAUDIO_AsyncCallback_AudioCapturer_Void(isSync: boolean = false): ((result: audio.AudioCapturer) => void) {
        const _resource : CallbackResource = this.readCallbackResource()
        const _call : KPointer = this.readPointer()
        const _callSync : KPointer = this.readPointer()
        return (result: audio.AudioCapturer):void => { const _argsSerializer : Serializer = Serializer.hold();
_argsSerializer.writeInt32(_resource.resourceId);
_argsSerializer.writePointer(_call);
_argsSerializer.writePointer(_callSync);
_argsSerializer.writeAudioCapturer(result);
(isSync) ? (InteropNativeModule._CallCallbackSync(103731593, _argsSerializer.asArray(), _argsSerializer.length())) : (InteropNativeModule._CallCallback(103731593, _argsSerializer.asArray(), _argsSerializer.length()));
_argsSerializer.release();
return; }
    }
    readAudioCapturerOptions(): audio.AudioCapturerOptions {
        let valueDeserializer : Deserializer = this
        const streamInfo_result : audio.AudioStreamInfo = valueDeserializer.readAudioStreamInfo()
        const capturerInfo_result : audio.AudioCapturerInfo = valueDeserializer.readAudioCapturerInfo()
        const playbackCaptureConfig_buf_runtimeType  = (valueDeserializer.readInt8() as int32)
        let playbackCaptureConfig_buf : audio.AudioPlaybackCaptureConfig | undefined
        if ((RuntimeType.UNDEFINED) != (playbackCaptureConfig_buf_runtimeType))
        {
            playbackCaptureConfig_buf = valueDeserializer.readAudioPlaybackCaptureConfig()
        }
        const playbackCaptureConfig_result : audio.AudioPlaybackCaptureConfig | undefined = playbackCaptureConfig_buf
        let value : audio.AudioCapturerOptions = ({streamInfo: streamInfo_result,capturerInfo: capturerInfo_result,playbackCaptureConfig: playbackCaptureConfig_result} as audio.AudioCapturerOptions)
        return value
    }
    readAudioStreamDeviceChangeInfo(): audio.AudioStreamDeviceChangeInfo {
        let valueDeserializer : Deserializer = this
        const devices_buf_length : int32 = valueDeserializer.readInt32()
        let devices_buf : Array<Readonly<AudioDeviceDescriptor>> = new Array<Readonly<AudioDeviceDescriptor>>()
        for (let devices_buf_i = 0; devices_buf_i < devices_buf_length; devices_buf_i++) {
            devices_buf[devices_buf_i] = (valueDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>") as Readonly<AudioDeviceDescriptor>)
        }
        const devices_result : audio.AudioDeviceDescriptors = devices_buf
        const changeReason_result : audio_AudioStreamDeviceChangeReason = (valueDeserializer.readInt32() as audio_AudioStreamDeviceChangeReason)
        let value : audio.AudioStreamDeviceChangeInfo = ({devices: devices_result,changeReason: changeReason_result} as audio.AudioStreamDeviceChangeInfo)
        return value
    }
    readAudioCapturerChangeInfo(): audio.AudioCapturerChangeInfo {
        let valueDeserializer : Deserializer = this
        const streamId_result : number = (valueDeserializer.readNumber() as number)
        const clientUid_result : number = (valueDeserializer.readNumber() as number)
        const capturerInfo_result : audio.AudioCapturerInfo = valueDeserializer.readAudioCapturerInfo()
        const capturerState_result : audio_AudioState = (valueDeserializer.readInt32() as audio_AudioState)
        const deviceDescriptors_buf_length : int32 = valueDeserializer.readInt32()
        let deviceDescriptors_buf : Array<Readonly<AudioDeviceDescriptor>> = new Array<Readonly<AudioDeviceDescriptor>>()
        for (let deviceDescriptors_buf_i = 0; deviceDescriptors_buf_i < deviceDescriptors_buf_length; deviceDescriptors_buf_i++) {
            deviceDescriptors_buf[deviceDescriptors_buf_i] = (valueDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>") as Readonly<AudioDeviceDescriptor>)
        }
        const deviceDescriptors_result : audio.AudioDeviceDescriptors = deviceDescriptors_buf
        const muted_buf_runtimeType  = (valueDeserializer.readInt8() as int32)
        let muted_buf : boolean | undefined
        if ((RuntimeType.UNDEFINED) != (muted_buf_runtimeType))
        {
            muted_buf = valueDeserializer.readBoolean()
        }
        const muted_result : boolean | undefined = muted_buf
        let value : audio.AudioCapturerChangeInfo = ({streamId: streamId_result,clientUid: clientUid_result,capturerInfo: capturerInfo_result,capturerState: capturerState_result,deviceDescriptors: deviceDescriptors_result,muted: muted_result} as audio.AudioCapturerChangeInfo)
        return value
    }
    readAudioRendererChangeInfo(): audio.AudioRendererChangeInfo {
        let valueDeserializer : Deserializer = this
        const streamId_result : number = (valueDeserializer.readNumber() as number)
        const clientUid_result : number = (valueDeserializer.readNumber() as number)
        const rendererInfo_result : audio.AudioRendererInfo = valueDeserializer.readAudioRendererInfo()
        const rendererState_result : audio_AudioState = (valueDeserializer.readInt32() as audio_AudioState)
        const deviceDescriptors_buf_length : int32 = valueDeserializer.readInt32()
        let deviceDescriptors_buf : Array<Readonly<AudioDeviceDescriptor>> = new Array<Readonly<AudioDeviceDescriptor>>()
        for (let deviceDescriptors_buf_i = 0; deviceDescriptors_buf_i < deviceDescriptors_buf_length; deviceDescriptors_buf_i++) {
            deviceDescriptors_buf[deviceDescriptors_buf_i] = (valueDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>") as Readonly<AudioDeviceDescriptor>)
        }
        const deviceDescriptors_result : audio.AudioDeviceDescriptors = deviceDescriptors_buf
        let value : audio.AudioRendererChangeInfo = ({streamId: streamId_result,clientUid: clientUid_result,rendererInfo: rendererInfo_result,rendererState: rendererState_result,deviceDescriptors: deviceDescriptors_result} as audio.AudioRendererChangeInfo)
        return value
    }
    readVolumeGroupInfo(): audio.VolumeGroupInfo {
        let valueDeserializer : Deserializer = this
        const networkId_result : string = (valueDeserializer.readString() as string)
        const groupId_result : number = (valueDeserializer.readNumber() as number)
        const mappingId_result : number = (valueDeserializer.readNumber() as number)
        const groupName_result : string = (valueDeserializer.readString() as string)
        const type_result : audio_ConnectType = (valueDeserializer.readInt32() as audio_ConnectType)
        let value : audio.VolumeGroupInfo = ({networkId: networkId_result,groupId: groupId_result,mappingId: mappingId_result,groupName: groupName_result,type: type_result} as audio.VolumeGroupInfo)
        return value
    }
    readAudioSpatialEnabledStateForDevice(): audio.AudioSpatialEnabledStateForDevice {
        let valueDeserializer : Deserializer = this
        const deviceDescriptor_result : audio.AudioDeviceDescriptor = valueDeserializer.readAudioDeviceDescriptor()
        const enabled_result : boolean = valueDeserializer.readBoolean()
        let value : audio.AudioSpatialEnabledStateForDevice = ({deviceDescriptor: deviceDescriptor_result,enabled: enabled_result} as audio.AudioSpatialEnabledStateForDevice)
        return value
    }
    readMicStateChangeEvent(): audio.MicStateChangeEvent {
        let valueDeserializer : Deserializer = this
        const mute_result : boolean = valueDeserializer.readBoolean()
        let value : audio.MicStateChangeEvent = ({mute: mute_result} as audio.MicStateChangeEvent)
        return value
    }
    readAudioVolumeGroupManager(): audio.AudioVolumeGroupManager {
        let valueDeserializer : Deserializer = this
        let ptr : KPointer = valueDeserializer.readPointer()
        return audio.AudioVolumeGroupManagerInternal.fromPtr(ptr)
    }
    readAudioSessionDeactivatedEvent(): audio.AudioSessionDeactivatedEvent {
        let valueDeserializer : Deserializer = this
        const reason_result : audio_AudioSessionDeactivatedReason = (valueDeserializer.readInt32() as audio_AudioSessionDeactivatedReason)
        let value : audio.AudioSessionDeactivatedEvent = ({reason: reason_result} as audio.AudioSessionDeactivatedEvent)
        return value
    }
    readDeviceBlockStatusInfo(): audio.DeviceBlockStatusInfo {
        let valueDeserializer : Deserializer = this
        const blockStatus_result : audio_DeviceBlockStatus = (valueDeserializer.readInt32() as audio_DeviceBlockStatus)
        const devices_buf_length : int32 = valueDeserializer.readInt32()
        let devices_buf : Array<Readonly<AudioDeviceDescriptor>> = new Array<Readonly<AudioDeviceDescriptor>>()
        for (let devices_buf_i = 0; devices_buf_i < devices_buf_length; devices_buf_i++) {
            devices_buf[devices_buf_i] = (valueDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>") as Readonly<AudioDeviceDescriptor>)
        }
        const devices_result : audio.AudioDeviceDescriptors = devices_buf
        let value : audio.DeviceBlockStatusInfo = ({blockStatus: blockStatus_result,devices: devices_result} as audio.DeviceBlockStatusInfo)
        return value
    }
    readInterruptResult(): audio.InterruptResult {
        let valueDeserializer : Deserializer = this
        const requestResult_result : audio_InterruptRequestResultType = (valueDeserializer.readInt32() as audio_InterruptRequestResultType)
        const interruptNode_result : number = (valueDeserializer.readNumber() as number)
        let value : audio.InterruptResult = ({requestResult: requestResult_result,interruptNode: interruptNode_result} as audio.InterruptResult)
        return value
    }
    readInterruptAction(): audio.InterruptAction {
        let valueDeserializer : Deserializer = this
        const actionType_result : audio_InterruptActionType = (valueDeserializer.readInt32() as audio_InterruptActionType)
        const type_buf_runtimeType  = (valueDeserializer.readInt8() as int32)
        let type_buf : audio_InterruptType | undefined
        if ((RuntimeType.UNDEFINED) != (type_buf_runtimeType))
        {
            type_buf = (valueDeserializer.readInt32() as audio_InterruptType)
        }
        const type_result : audio_InterruptType | undefined = type_buf
        const hint_buf_runtimeType  = (valueDeserializer.readInt8() as int32)
        let hint_buf : audio_InterruptHint | undefined
        if ((RuntimeType.UNDEFINED) != (hint_buf_runtimeType))
        {
            hint_buf = (valueDeserializer.readInt32() as audio_InterruptHint)
        }
        const hint_result : audio_InterruptHint | undefined = hint_buf
        const activated_buf_runtimeType  = (valueDeserializer.readInt8() as int32)
        let activated_buf : boolean | undefined
        if ((RuntimeType.UNDEFINED) != (activated_buf_runtimeType))
        {
            activated_buf = valueDeserializer.readBoolean()
        }
        const activated_result : boolean | undefined = activated_buf
        let value : audio.InterruptAction = ({actionType: actionType_result,type: type_result,hint: hint_result,activated: activated_result} as audio.InterruptAction)
        return value
    }
    readDeviceChangeAction(): audio.DeviceChangeAction {
        let valueDeserializer : Deserializer = this
        const type_result : audio_DeviceChangeType = (valueDeserializer.readInt32() as audio_DeviceChangeType)
        const deviceDescriptors_buf_length : int32 = valueDeserializer.readInt32()
        let deviceDescriptors_buf : Array<Readonly<AudioDeviceDescriptor>> = new Array<Readonly<AudioDeviceDescriptor>>()
        for (let deviceDescriptors_buf_i = 0; deviceDescriptors_buf_i < deviceDescriptors_buf_length; deviceDescriptors_buf_i++) {
            deviceDescriptors_buf[deviceDescriptors_buf_i] = (valueDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>") as Readonly<AudioDeviceDescriptor>)
        }
        const deviceDescriptors_result : audio.AudioDeviceDescriptors = deviceDescriptors_buf
        let value : audio.DeviceChangeAction = ({type: type_result,deviceDescriptors: deviceDescriptors_result} as audio.DeviceChangeAction)
        return value
    }
    readVolumeEvent(): audio.VolumeEvent {
        let valueDeserializer : Deserializer = this
        const volumeType_result : audio_AudioVolumeType = (valueDeserializer.readInt32() as audio_AudioVolumeType)
        const volume_result : number = (valueDeserializer.readNumber() as number)
        const updateUi_result : boolean = valueDeserializer.readBoolean()
        const volumeGroupId_result : number = (valueDeserializer.readNumber() as number)
        const networkId_result : string = (valueDeserializer.readString() as string)
        let value : audio.VolumeEvent = ({volumeType: volumeType_result,volume: volume_result,updateUi: updateUi_result,volumeGroupId: volumeGroupId_result,networkId: networkId_result} as audio.VolumeEvent)
        return value
    }
    readInterruptEvent(): audio.InterruptEvent {
        let valueDeserializer : Deserializer = this
        const eventType_result : audio_InterruptType = (valueDeserializer.readInt32() as audio_InterruptType)
        const forceType_result : audio_InterruptForceType = (valueDeserializer.readInt32() as audio_InterruptForceType)
        const hintType_result : audio_InterruptHint = (valueDeserializer.readInt32() as audio_InterruptHint)
        let value : audio.InterruptEvent = ({eventType: eventType_result,forceType: forceType_result,hintType: hintType_result} as audio.InterruptEvent)
        return value
    }
    readAudioRendererOptions(): audio.AudioRendererOptions {
        let valueDeserializer : Deserializer = this
        const streamInfo_result : audio.AudioStreamInfo = valueDeserializer.readAudioStreamInfo()
        const rendererInfo_result : audio.AudioRendererInfo = valueDeserializer.readAudioRendererInfo()
        const privacyType_buf_runtimeType  = (valueDeserializer.readInt8() as int32)
        let privacyType_buf : audio_AudioPrivacyType | undefined
        if ((RuntimeType.UNDEFINED) != (privacyType_buf_runtimeType))
        {
            privacyType_buf = (valueDeserializer.readInt32() as audio_AudioPrivacyType)
        }
        const privacyType_result : audio_AudioPrivacyType | undefined = privacyType_buf
        let value : audio.AudioRendererOptions = ({streamInfo: streamInfo_result,rendererInfo: rendererInfo_result,privacyType: privacyType_result} as audio.AudioRendererOptions)
        return value
    }
}
