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

import { CallbackKind } from "./CallbackKind"
import { Deserializer } from "./Deserializer"
import { int32, float32, int64 } from "@koalaui/common"
import { ResourceHolder, KInt, KStringPtr, wrapSystemCallback, KPointer, RuntimeType } from "@koalaui/interop"
import { audio, audio_AudioEffectMode, audio_AudioRendererRate, audio_AudioRingMode, audio_AudioScene, audio_AudioDataCallbackResult, audio_AudioState } from "./../OHAudioNamespace"

export function deserializeAndCallAsyncCallback_AudioCapturer_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: audio.AudioCapturer) => void))
    let result : audio.AudioCapturer = (thisDeserializer.readAudioCapturer() as audio.AudioCapturer)
    _call(result)
}
export function deserializeAndCallAsyncCallback_AudioCapturerChangeInfoArray_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: audio.AudioCapturerChangeInfoArray) => void))
    const result_buf_length : int32 = thisDeserializer.readInt32()
    let result_buf : Array<Readonly<AudioCapturerChangeInfo>> = new Array<Readonly<AudioCapturerChangeInfo>>()
    for (let result_buf_i = 0; result_buf_i < result_buf_length; result_buf_i++) {
        result_buf[result_buf_i] = (thisDeserializer.readCustomObject("Readonly<AudioCapturerChangeInfo>") as Readonly<AudioCapturerChangeInfo>)
    }
    let result : audio.AudioCapturerChangeInfoArray = result_buf
    _call(result)
}
export function deserializeAndCallAsyncCallback_AudioCapturerInfo_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: audio.AudioCapturerInfo) => void))
    let result : audio.AudioCapturerInfo = thisDeserializer.readAudioCapturerInfo()
    _call(result)
}
export function deserializeAndCallAsyncCallback_AudioDeviceDescriptors_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: audio.AudioDeviceDescriptors) => void))
    const result_buf_length : int32 = thisDeserializer.readInt32()
    let result_buf : Array<Readonly<AudioDeviceDescriptor>> = new Array<Readonly<AudioDeviceDescriptor>>()
    for (let result_buf_i = 0; result_buf_i < result_buf_length; result_buf_i++) {
        result_buf[result_buf_i] = (thisDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>") as Readonly<AudioDeviceDescriptor>)
    }
    let result : audio.AudioDeviceDescriptors = result_buf
    _call(result)
}
export function deserializeAndCallAsyncCallback_AudioEffectInfoArray_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: audio.AudioEffectInfoArray) => void))
    const result_buf_length : int32 = thisDeserializer.readInt32()
    let result_buf : Array<Readonly<AudioEffectMode>> = new Array<Readonly<AudioEffectMode>>()
    for (let result_buf_i = 0; result_buf_i < result_buf_length; result_buf_i++) {
        result_buf[result_buf_i] = (thisDeserializer.readCustomObject("Readonly<AudioEffectMode>") as Readonly<AudioEffectMode>)
    }
    let result : audio.AudioEffectInfoArray = result_buf
    _call(result)
}
export function deserializeAndCallAsyncCallback_AudioEffectMode_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: audio_AudioEffectMode) => void))
    let result : audio_AudioEffectMode = (thisDeserializer.readInt32() as audio_AudioEffectMode)
    _call(result)
}
export function deserializeAndCallAsyncCallback_AudioRenderer_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: audio.AudioRenderer) => void))
    let result : audio.AudioRenderer = (thisDeserializer.readAudioRenderer() as audio.AudioRenderer)
    _call(result)
}
export function deserializeAndCallAsyncCallback_AudioRendererChangeInfoArray_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: audio.AudioRendererChangeInfoArray) => void))
    const result_buf_length : int32 = thisDeserializer.readInt32()
    let result_buf : Array<Readonly<AudioRendererChangeInfo>> = new Array<Readonly<AudioRendererChangeInfo>>()
    for (let result_buf_i = 0; result_buf_i < result_buf_length; result_buf_i++) {
        result_buf[result_buf_i] = (thisDeserializer.readCustomObject("Readonly<AudioRendererChangeInfo>") as Readonly<AudioRendererChangeInfo>)
    }
    let result : audio.AudioRendererChangeInfoArray = result_buf
    _call(result)
}
export function deserializeAndCallAsyncCallback_AudioRendererInfo_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: audio.AudioRendererInfo) => void))
    let result : audio.AudioRendererInfo = thisDeserializer.readAudioRendererInfo()
    _call(result)
}
export function deserializeAndCallAsyncCallback_AudioRendererRate_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: audio_AudioRendererRate) => void))
    let result : audio_AudioRendererRate = (thisDeserializer.readInt32() as audio_AudioRendererRate)
    _call(result)
}
export function deserializeAndCallAsyncCallback_AudioRingMode_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: audio_AudioRingMode) => void))
    let result : audio_AudioRingMode = (thisDeserializer.readInt32() as audio_AudioRingMode)
    _call(result)
}
export function deserializeAndCallAsyncCallback_AudioScene_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: audio_AudioScene) => void))
    let result : audio_AudioScene = (thisDeserializer.readInt32() as audio_AudioScene)
    _call(result)
}
export function deserializeAndCallAsyncCallback_AudioStreamInfo_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: audio.AudioStreamInfo) => void))
    let result : audio.AudioStreamInfo = thisDeserializer.readAudioStreamInfo()
    _call(result)
}
export function deserializeAndCallAsyncCallback_AudioVolumeGroupManager_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: audio.AudioVolumeGroupManager) => void))
    let result : audio.AudioVolumeGroupManager = (thisDeserializer.readAudioVolumeGroupManager() as audio.AudioVolumeGroupManager)
    _call(result)
}
export function deserializeAndCallAsyncCallback_Boolean_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: boolean) => void))
    let result : boolean = thisDeserializer.readBoolean()
    _call(result)
}
export function deserializeAndCallAsyncCallback_Buffer_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: NativeBuffer) => void))
    let result : NativeBuffer = (thisDeserializer.readBuffer() as NativeBuffer)
    _call(result)
}
export function deserializeAndCallAsyncCallback_Number_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: number) => void))
    let result : number = (thisDeserializer.readNumber() as number)
    _call(result)
}
export function deserializeAndCallAsyncCallback_String_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: string) => void))
    let result : string = (thisDeserializer.readString() as string)
    _call(result)
}
export function deserializeAndCallAsyncCallback_TonePlayer_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: audio.TonePlayer) => void))
    let result : audio.TonePlayer = (thisDeserializer.readTonePlayer() as audio.TonePlayer)
    _call(result)
}
export function deserializeAndCallAsyncCallback_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as (() => void))
    _call()
}
export function deserializeAndCallAsyncCallback_VolumeGroupInfos_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((result: audio.VolumeGroupInfos) => void))
    const result_buf_length : int32 = thisDeserializer.readInt32()
    let result_buf : Array<Readonly<VolumeGroupInfo>> = new Array<Readonly<VolumeGroupInfo>>()
    for (let result_buf_i = 0; result_buf_i < result_buf_length; result_buf_i++) {
        result_buf[result_buf_i] = (thisDeserializer.readCustomObject("Readonly<VolumeGroupInfo>") as Readonly<VolumeGroupInfo>)
    }
    let result : audio.VolumeGroupInfos = result_buf
    _call(result)
}
export function deserializeAndCallAudioRendererWriteDataCallback(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as audio.AudioRendererWriteDataCallback)
    let data : NativeBuffer = (thisDeserializer.readBuffer() as NativeBuffer)
    const _callResult  = _call(data)
    _continuation(_callResult)
}
export function deserializeAndCallCallback_AudioCapturerChangeInfo_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: audio.AudioCapturerChangeInfo) => void))
    let parameter : audio.AudioCapturerChangeInfo = thisDeserializer.readAudioCapturerChangeInfo()
    _call(parameter)
}
export function deserializeAndCallCallback_AudioCapturerChangeInfoArray_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: audio.AudioCapturerChangeInfoArray) => void))
    const parameter_buf_length : int32 = thisDeserializer.readInt32()
    let parameter_buf : Array<Readonly<AudioCapturerChangeInfo>> = new Array<Readonly<AudioCapturerChangeInfo>>()
    for (let parameter_buf_i = 0; parameter_buf_i < parameter_buf_length; parameter_buf_i++) {
        parameter_buf[parameter_buf_i] = (thisDeserializer.readCustomObject("Readonly<AudioCapturerChangeInfo>") as Readonly<AudioCapturerChangeInfo>)
    }
    let parameter : audio.AudioCapturerChangeInfoArray = parameter_buf
    _call(parameter)
}
export function deserializeAndCallCallback_AudioDeviceDescriptors_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: audio.AudioDeviceDescriptors) => void))
    const parameter_buf_length : int32 = thisDeserializer.readInt32()
    let parameter_buf : Array<Readonly<AudioDeviceDescriptor>> = new Array<Readonly<AudioDeviceDescriptor>>()
    for (let parameter_buf_i = 0; parameter_buf_i < parameter_buf_length; parameter_buf_i++) {
        parameter_buf[parameter_buf_i] = (thisDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>") as Readonly<AudioDeviceDescriptor>)
    }
    let parameter : audio.AudioDeviceDescriptors = parameter_buf
    _call(parameter)
}
export function deserializeAndCallCallback_AudioRendererChangeInfoArray_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: audio.AudioRendererChangeInfoArray) => void))
    const parameter_buf_length : int32 = thisDeserializer.readInt32()
    let parameter_buf : Array<Readonly<AudioRendererChangeInfo>> = new Array<Readonly<AudioRendererChangeInfo>>()
    for (let parameter_buf_i = 0; parameter_buf_i < parameter_buf_length; parameter_buf_i++) {
        parameter_buf[parameter_buf_i] = (thisDeserializer.readCustomObject("Readonly<AudioRendererChangeInfo>") as Readonly<AudioRendererChangeInfo>)
    }
    let parameter : audio.AudioRendererChangeInfoArray = parameter_buf
    _call(parameter)
}
export function deserializeAndCallCallback_AudioRingMode_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: audio_AudioRingMode) => void))
    let parameter : audio_AudioRingMode = (thisDeserializer.readInt32() as audio_AudioRingMode)
    _call(parameter)
}
export function deserializeAndCallCallback_AudioSessionDeactivatedEvent_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: audio.AudioSessionDeactivatedEvent) => void))
    let parameter : audio.AudioSessionDeactivatedEvent = thisDeserializer.readAudioSessionDeactivatedEvent()
    _call(parameter)
}
export function deserializeAndCallCallback_AudioSpatialEnabledStateForDevice_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: audio.AudioSpatialEnabledStateForDevice) => void))
    let parameter : audio.AudioSpatialEnabledStateForDevice = thisDeserializer.readAudioSpatialEnabledStateForDevice()
    _call(parameter)
}
export function deserializeAndCallCallback_AudioState_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: audio_AudioState) => void))
    let parameter : audio_AudioState = (thisDeserializer.readInt32() as audio_AudioState)
    _call(parameter)
}
export function deserializeAndCallCallback_AudioStreamDeviceChangeInfo_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: audio.AudioStreamDeviceChangeInfo) => void))
    let parameter : audio.AudioStreamDeviceChangeInfo = thisDeserializer.readAudioStreamDeviceChangeInfo()
    _call(parameter)
}
export function deserializeAndCallCallback_Boolean_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: boolean) => void))
    let parameter : boolean = thisDeserializer.readBoolean()
    _call(parameter)
}
export function deserializeAndCallCallback_Buffer_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: NativeBuffer) => void))
    let parameter : NativeBuffer = (thisDeserializer.readBuffer() as NativeBuffer)
    _call(parameter)
}
export function deserializeAndCallCallback_DeviceBlockStatusInfo_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: audio.DeviceBlockStatusInfo) => void))
    let parameter : audio.DeviceBlockStatusInfo = thisDeserializer.readDeviceBlockStatusInfo()
    _call(parameter)
}
export function deserializeAndCallCallback_DeviceChangeAction_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: audio.DeviceChangeAction) => void))
    let parameter : audio.DeviceChangeAction = thisDeserializer.readDeviceChangeAction()
    _call(parameter)
}
export function deserializeAndCallCallback_InterruptAction_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: audio.InterruptAction) => void))
    let parameter : audio.InterruptAction = thisDeserializer.readInterruptAction()
    _call(parameter)
}
export function deserializeAndCallCallback_InterruptEvent_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: audio.InterruptEvent) => void))
    let parameter : audio.InterruptEvent = thisDeserializer.readInterruptEvent()
    _call(parameter)
}
export function deserializeAndCallCallback_MicStateChangeEvent_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: audio.MicStateChangeEvent) => void))
    let parameter : audio.MicStateChangeEvent = thisDeserializer.readMicStateChangeEvent()
    _call(parameter)
}
export function deserializeAndCallCallback_Number_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: number) => void))
    let parameter : number = (thisDeserializer.readNumber() as number)
    _call(parameter)
}
export function deserializeAndCallCallback_VolumeEvent_Void(thisDeserializer: Deserializer): void {
    const _resourceId : int32 = thisDeserializer.readInt32()
    const _call  = (ResourceHolder.instance().get(_resourceId) as ((parameter: audio.VolumeEvent) => void))
    let parameter : audio.VolumeEvent = thisDeserializer.readVolumeEvent()
    _call(parameter)
}
export function deserializeAndCallCallback(thisDeserializer: Deserializer): void {
    const kind : int32 = thisDeserializer.readInt32()
    switch (kind) {
        case 103731593/*CallbackKind.Kind_AsyncCallback_AudioCapturer_Void*/: return deserializeAndCallAsyncCallback_AudioCapturer_Void(thisDeserializer);
        case 605900120/*CallbackKind.Kind_AsyncCallback_AudioCapturerChangeInfoArray_Void*/: return deserializeAndCallAsyncCallback_AudioCapturerChangeInfoArray_Void(thisDeserializer);
        case 945856903/*CallbackKind.Kind_AsyncCallback_AudioCapturerInfo_Void*/: return deserializeAndCallAsyncCallback_AudioCapturerInfo_Void(thisDeserializer);
        case 1103131879/*CallbackKind.Kind_AsyncCallback_AudioDeviceDescriptors_Void*/: return deserializeAndCallAsyncCallback_AudioDeviceDescriptors_Void(thisDeserializer);
        case -1801809985/*CallbackKind.Kind_AsyncCallback_AudioEffectInfoArray_Void*/: return deserializeAndCallAsyncCallback_AudioEffectInfoArray_Void(thisDeserializer);
        case -852719253/*CallbackKind.Kind_AsyncCallback_AudioEffectMode_Void*/: return deserializeAndCallAsyncCallback_AudioEffectMode_Void(thisDeserializer);
        case -1795948094/*CallbackKind.Kind_AsyncCallback_AudioRenderer_Void*/: return deserializeAndCallAsyncCallback_AudioRenderer_Void(thisDeserializer);
        case 419082675/*CallbackKind.Kind_AsyncCallback_AudioRendererChangeInfoArray_Void*/: return deserializeAndCallAsyncCallback_AudioRendererChangeInfoArray_Void(thisDeserializer);
        case 721972620/*CallbackKind.Kind_AsyncCallback_AudioRendererInfo_Void*/: return deserializeAndCallAsyncCallback_AudioRendererInfo_Void(thisDeserializer);
        case 147576480/*CallbackKind.Kind_AsyncCallback_AudioRendererRate_Void*/: return deserializeAndCallAsyncCallback_AudioRendererRate_Void(thisDeserializer);
        case -815244914/*CallbackKind.Kind_AsyncCallback_AudioRingMode_Void*/: return deserializeAndCallAsyncCallback_AudioRingMode_Void(thisDeserializer);
        case -854078751/*CallbackKind.Kind_AsyncCallback_AudioScene_Void*/: return deserializeAndCallAsyncCallback_AudioScene_Void(thisDeserializer);
        case -106617875/*CallbackKind.Kind_AsyncCallback_AudioStreamInfo_Void*/: return deserializeAndCallAsyncCallback_AudioStreamInfo_Void(thisDeserializer);
        case 540266043/*CallbackKind.Kind_AsyncCallback_AudioVolumeGroupManager_Void*/: return deserializeAndCallAsyncCallback_AudioVolumeGroupManager_Void(thisDeserializer);
        case 46391693/*CallbackKind.Kind_AsyncCallback_Boolean_Void*/: return deserializeAndCallAsyncCallback_Boolean_Void(thisDeserializer);
        case -1662321143/*CallbackKind.Kind_AsyncCallback_Buffer_Void*/: return deserializeAndCallAsyncCallback_Buffer_Void(thisDeserializer);
        case 1959553162/*CallbackKind.Kind_AsyncCallback_Number_Void*/: return deserializeAndCallAsyncCallback_Number_Void(thisDeserializer);
        case 789188988/*CallbackKind.Kind_AsyncCallback_String_Void*/: return deserializeAndCallAsyncCallback_String_Void(thisDeserializer);
        case 895924586/*CallbackKind.Kind_AsyncCallback_TonePlayer_Void*/: return deserializeAndCallAsyncCallback_TonePlayer_Void(thisDeserializer);
        case 1075219926/*CallbackKind.Kind_AsyncCallback_Void*/: return deserializeAndCallAsyncCallback_Void(thisDeserializer);
        case -801079837/*CallbackKind.Kind_AsyncCallback_VolumeGroupInfos_Void*/: return deserializeAndCallAsyncCallback_VolumeGroupInfos_Void(thisDeserializer);
        case -1508727875/*CallbackKind.Kind_AudioRendererWriteDataCallback*/: return deserializeAndCallAudioRendererWriteDataCallback(thisDeserializer);
        case 300211623/*CallbackKind.Kind_Callback_AudioCapturerChangeInfo_Void*/: return deserializeAndCallCallback_AudioCapturerChangeInfo_Void(thisDeserializer);
        case -788037890/*CallbackKind.Kind_Callback_AudioCapturerChangeInfoArray_Void*/: return deserializeAndCallCallback_AudioCapturerChangeInfoArray_Void(thisDeserializer);
        case -872298751/*CallbackKind.Kind_Callback_AudioDeviceDescriptors_Void*/: return deserializeAndCallCallback_AudioDeviceDescriptors_Void(thisDeserializer);
        case 1609768789/*CallbackKind.Kind_Callback_AudioRendererChangeInfoArray_Void*/: return deserializeAndCallCallback_AudioRendererChangeInfoArray_Void(thisDeserializer);
        case -1700902488/*CallbackKind.Kind_Callback_AudioRingMode_Void*/: return deserializeAndCallCallback_AudioRingMode_Void(thisDeserializer);
        case 1744071031/*CallbackKind.Kind_Callback_AudioSessionDeactivatedEvent_Void*/: return deserializeAndCallCallback_AudioSessionDeactivatedEvent_Void(thisDeserializer);
        case 1057396442/*CallbackKind.Kind_Callback_AudioSpatialEnabledStateForDevice_Void*/: return deserializeAndCallCallback_AudioSpatialEnabledStateForDevice_Void(thisDeserializer);
        case 558001102/*CallbackKind.Kind_Callback_AudioState_Void*/: return deserializeAndCallCallback_AudioState_Void(thisDeserializer);
        case 1823460565/*CallbackKind.Kind_Callback_AudioStreamDeviceChangeInfo_Void*/: return deserializeAndCallCallback_AudioStreamDeviceChangeInfo_Void(thisDeserializer);
        case 313269291/*CallbackKind.Kind_Callback_Boolean_Void*/: return deserializeAndCallCallback_Boolean_Void(thisDeserializer);
        case 908731311/*CallbackKind.Kind_Callback_Buffer_Void*/: return deserializeAndCallCallback_Buffer_Void(thisDeserializer);
        case -761620636/*CallbackKind.Kind_Callback_DeviceBlockStatusInfo_Void*/: return deserializeAndCallCallback_DeviceBlockStatusInfo_Void(thisDeserializer);
        case 892121871/*CallbackKind.Kind_Callback_DeviceChangeAction_Void*/: return deserializeAndCallCallback_DeviceChangeAction_Void(thisDeserializer);
        case -1389231466/*CallbackKind.Kind_Callback_InterruptAction_Void*/: return deserializeAndCallCallback_InterruptAction_Void(thisDeserializer);
        case 638628164/*CallbackKind.Kind_Callback_InterruptEvent_Void*/: return deserializeAndCallCallback_InterruptEvent_Void(thisDeserializer);
        case -1553290571/*CallbackKind.Kind_Callback_MicStateChangeEvent_Void*/: return deserializeAndCallCallback_MicStateChangeEvent_Void(thisDeserializer);
        case 36519084/*CallbackKind.Kind_Callback_Number_Void*/: return deserializeAndCallCallback_Number_Void(thisDeserializer);
        case 311503723/*CallbackKind.Kind_Callback_VolumeEvent_Void*/: return deserializeAndCallCallback_VolumeEvent_Void(thisDeserializer);
    }
    console.log("Unknown callback kind")
}