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


// WARNING! THIS FILE IS AUTO-GENERATED, DO NOT MAKE CHANGES, THEY WILL BE LOST ON NEXT GENERATION!

import { TypeChecker } from "#components"
import { Finalizable, isResource, isInstanceOf, runtimeType, RuntimeType, SerializerBase, registerCallback, wrapCallback, KPointer, NativeBuffer } from "@koalaui/interop"
import { MaterializedBase } from "./MaterializedBase"
import { Serializer } from "./peers/Serializer"
import { unsafeCast } from "./shared/generated-utils"
import { CallbackKind } from "./peers/CallbackKind"
import { int32, float32 } from "@koalaui/common"
import { AUDIONativeModule } from "./AUDIONativeModule"
export class GlobalScope_ohos_multimedia_audio {
    public static getAudioManager(): AudioManager {
        return GlobalScope_ohos_multimedia_audio.getAudioManager_serialize()
    }
    public static createAudioCapturer(options: AudioCapturerOptions, callback_?: AsyncCallback_AudioCapturer_Void): AudioCapturer | void {
        const options_type = runtimeType(options)
        const callback__type = runtimeType(callback_)
        if ((((RuntimeType.UNDEFINED == callback__type)))) {
            const options_casted = options as (AudioCapturerOptions)
            return GlobalScope_ohos_multimedia_audio.createAudioCapturer1_serialize(options_casted)
        }
        if ((((RuntimeType.OBJECT == callback__type)))) {
            const options_casted = options as (AudioCapturerOptions)
            const callback__casted = callback_ as (AsyncCallback_AudioCapturer_Void)
            return GlobalScope_ohos_multimedia_audio.createAudioCapturer0_serialize(options_casted, callback__casted)
        }
        throw new Error("Can not select appropriate overload")
    }
    public static createAudioRenderer(options: AudioRendererOptions, callback_?: AsyncCallback_AudioRenderer_Void): AudioRenderer | void {
        const options_type = runtimeType(options)
        const callback__type = runtimeType(callback_)
        if ((((RuntimeType.UNDEFINED == callback__type)))) {
            const options_casted = options as (AudioRendererOptions)
            return GlobalScope_ohos_multimedia_audio.createAudioRenderer1_serialize(options_casted)
        }
        if ((((RuntimeType.OBJECT == callback__type)))) {
            const options_casted = options as (AudioRendererOptions)
            const callback__casted = callback_ as (AsyncCallback_AudioRenderer_Void)
            return GlobalScope_ohos_multimedia_audio.createAudioRenderer0_serialize(options_casted, callback__casted)
        }
        throw new Error("Can not select appropriate overload")
    }
    public static createTonePlayer(options: AudioRendererInfo, callback_?: AsyncCallback_TonePlayer_Void): TonePlayer | void {
        const options_type = runtimeType(options)
        const callback__type = runtimeType(callback_)
        if ((((RuntimeType.UNDEFINED == callback__type)))) {
            const options_casted = options as (AudioRendererInfo)
            return GlobalScope_ohos_multimedia_audio.createTonePlayer1_serialize(options_casted)
        }
        if ((((RuntimeType.OBJECT == callback__type)))) {
            const options_casted = options as (AudioRendererInfo)
            const callback__casted = callback_ as (AsyncCallback_TonePlayer_Void)
            return GlobalScope_ohos_multimedia_audio.createTonePlayer0_serialize(options_casted, callback__casted)
        }
        throw new Error("Can not select appropriate overload")
    }
    public static createAsrProcessingController(audioCapturer: AudioCapturer): AsrProcessingController {
        const audioCapturer_casted = audioCapturer as (AudioCapturer)
        return GlobalScope_ohos_multimedia_audio.createAsrProcessingController_serialize(audioCapturer_casted)
    }
    private static getAudioManager_serialize(): AudioManager {
        const retval  = AUDIONativeModule._GlobalScope_ohos_multimedia_audio_getAudioManager()
        throw new Error("Object deserialization is not implemented.")
    }
    private static createAudioCapturer0_serialize(options: AudioCapturerOptions, callback_: AsyncCallback_AudioCapturer_Void): void {
        const thisSerializer : Serializer = Serializer.hold()
        thisSerializer.writeCustomObject("AudioCapturerOptions", options)
        thisSerializer.writeCustomObject("AsyncCallback_AudioCapturer_Void", callback_)
        AUDIONativeModule._GlobalScope_ohos_multimedia_audio_createAudioCapturer0(thisSerializer.asArray(), thisSerializer.length())
        thisSerializer.release()
    }
    private static createAudioCapturer1_serialize(options: AudioCapturerOptions): AudioCapturer {
        const thisSerializer : Serializer = Serializer.hold()
        thisSerializer.writeCustomObject("AudioCapturerOptions", options)
        const retval  = AUDIONativeModule._GlobalScope_ohos_multimedia_audio_createAudioCapturer1(thisSerializer.asArray(), thisSerializer.length())
        thisSerializer.release()
        throw new Error("Object deserialization is not implemented.")
    }
    private static createAudioRenderer0_serialize(options: AudioRendererOptions, callback_: AsyncCallback_AudioRenderer_Void): void {
        const thisSerializer : Serializer = Serializer.hold()
        thisSerializer.writeCustomObject("AudioRendererOptions", options)
        thisSerializer.writeCustomObject("AsyncCallback_AudioRenderer_Void", callback_)
        AUDIONativeModule._GlobalScope_ohos_multimedia_audio_createAudioRenderer0(thisSerializer.asArray(), thisSerializer.length())
        thisSerializer.release()
    }
    private static createAudioRenderer1_serialize(options: AudioRendererOptions): AudioRenderer {
        const thisSerializer : Serializer = Serializer.hold()
        thisSerializer.writeCustomObject("AudioRendererOptions", options)
        const retval  = AUDIONativeModule._GlobalScope_ohos_multimedia_audio_createAudioRenderer1(thisSerializer.asArray(), thisSerializer.length())
        thisSerializer.release()
        throw new Error("Object deserialization is not implemented.")
    }
    private static createTonePlayer0_serialize(options: AudioRendererInfo, callback_: AsyncCallback_TonePlayer_Void): void {
        const thisSerializer : Serializer = Serializer.hold()
        thisSerializer.writeCustomObject("AudioRendererInfo", options)
        thisSerializer.writeCustomObject("AsyncCallback_TonePlayer_Void", callback_)
        AUDIONativeModule._GlobalScope_ohos_multimedia_audio_createTonePlayer0(thisSerializer.asArray(), thisSerializer.length())
        thisSerializer.release()
    }
    private static createTonePlayer1_serialize(options: AudioRendererInfo): TonePlayer {
        const thisSerializer : Serializer = Serializer.hold()
        thisSerializer.writeCustomObject("AudioRendererInfo", options)
        const retval  = AUDIONativeModule._GlobalScope_ohos_multimedia_audio_createTonePlayer1(thisSerializer.asArray(), thisSerializer.length())
        thisSerializer.release()
        throw new Error("Object deserialization is not implemented.")
    }
    private static createAsrProcessingController_serialize(audioCapturer: AudioCapturer): AsrProcessingController {
        const thisSerializer : Serializer = Serializer.hold()
        thisSerializer.writeCustomObject("AudioCapturer", audioCapturer)
        const retval  = AUDIONativeModule._GlobalScope_ohos_multimedia_audio_createAsrProcessingController(thisSerializer.asArray(), thisSerializer.length())
        thisSerializer.release()
        throw new Error("Object deserialization is not implemented.")
    }
}
