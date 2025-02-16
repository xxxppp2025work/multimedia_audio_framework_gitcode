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
export namespace audio {
    export class AudioManagerInternal implements MaterializedBase,AudioManager {
        peer?: Finalizable | undefined
        public getPeer(): Finalizable | undefined {
            return this.peer
        }
        static ctor(): KPointer {
            const retval  = AUDIONativeModule._AudioManager_ctor()
            return retval
        }
         constructor() {
            const ctorPtr : KPointer = AudioManagerInternal.ctor()
            this.peer = new Finalizable(ctorPtr, AudioManagerInternal.getFinalizer())
        }
        static getFinalizer(): KPointer {
            return AUDIONativeModule._AudioManager_getFinalizer()
        }
        public setVolume(volumeType: audio_AudioVolumeType, volume: number, callback_?: (() => void)): void {
            const volumeType_type = runtimeType(volumeType)
            const volume_type = runtimeType(volume)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const volume_casted = volume as (number)
                this.setVolume1_serialize(volumeType_casted, volume_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const volume_casted = volume as (number)
                const callback__casted = callback_ as ((() => void))
                this.setVolume0_serialize(volumeType_casted, volume_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public getVolume(volumeType: audio_AudioVolumeType, callback_?: ((result: number) => void)): number | void {
            const volumeType_type = runtimeType(volumeType)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                return this.getVolume1_serialize(volumeType_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const callback__casted = callback_ as (((result: number) => void))
                return this.getVolume0_serialize(volumeType_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getMinVolume(volumeType: audio_AudioVolumeType, callback_?: ((result: number) => void)): number | void {
            const volumeType_type = runtimeType(volumeType)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                return this.getMinVolume1_serialize(volumeType_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const callback__casted = callback_ as (((result: number) => void))
                return this.getMinVolume0_serialize(volumeType_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getMaxVolume(volumeType: audio_AudioVolumeType, callback_?: ((result: number) => void)): number | void {
            const volumeType_type = runtimeType(volumeType)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                return this.getMaxVolume1_serialize(volumeType_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const callback__casted = callback_ as (((result: number) => void))
                return this.getMaxVolume0_serialize(volumeType_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getDevices(deviceFlag: audio_DeviceFlag, callback_?: ((result: audio.AudioDeviceDescriptors) => void)): audio.AudioDeviceDescriptors | void {
            const deviceFlag_type = runtimeType(deviceFlag)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const deviceFlag_casted = deviceFlag as (audio_DeviceFlag)
                return this.getDevices1_serialize(deviceFlag_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const deviceFlag_casted = deviceFlag as (audio_DeviceFlag)
                const callback__casted = callback_ as (((result: audio.AudioDeviceDescriptors) => void))
                return this.getDevices0_serialize(deviceFlag_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public mute(volumeType: audio_AudioVolumeType, mute: boolean, callback_?: (() => void)): void {
            const volumeType_type = runtimeType(volumeType)
            const mute_type = runtimeType(mute)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const mute_casted = mute as (boolean)
                this.mute1_serialize(volumeType_casted, mute_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const mute_casted = mute as (boolean)
                const callback__casted = callback_ as ((() => void))
                this.mute0_serialize(volumeType_casted, mute_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public isMute(volumeType: audio_AudioVolumeType, callback_?: ((result: boolean) => void)): boolean | void {
            const volumeType_type = runtimeType(volumeType)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                return this.isMute1_serialize(volumeType_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const callback__casted = callback_ as (((result: boolean) => void))
                return this.isMute0_serialize(volumeType_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public isActive(volumeType: audio_AudioVolumeType, callback_?: ((result: boolean) => void)): boolean | void {
            const volumeType_type = runtimeType(volumeType)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                return this.isActive1_serialize(volumeType_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const callback__casted = callback_ as (((result: boolean) => void))
                return this.isActive0_serialize(volumeType_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public setMicrophoneMute(mute: boolean, callback_?: (() => void)): void {
            const mute_type = runtimeType(mute)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const mute_casted = mute as (boolean)
                this.setMicrophoneMute1_serialize(mute_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const mute_casted = mute as (boolean)
                const callback__casted = callback_ as ((() => void))
                this.setMicrophoneMute0_serialize(mute_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public isMicrophoneMute(callback_?: ((result: boolean) => void)): boolean | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.isMicrophoneMute1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: boolean) => void))
                return this.isMicrophoneMute0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public setRingerMode(mode: audio_AudioRingMode, callback_?: (() => void)): void {
            const mode_type = runtimeType(mode)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const mode_casted = mode as (audio_AudioRingMode)
                this.setRingerMode1_serialize(mode_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const mode_casted = mode as (audio_AudioRingMode)
                const callback__casted = callback_ as ((() => void))
                this.setRingerMode0_serialize(mode_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public getRingerMode(callback_?: ((result: audio_AudioRingMode) => void)): audio_AudioRingMode | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getRingerMode1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: audio_AudioRingMode) => void))
                return this.getRingerMode0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public setAudioParameter(key: string, value: string, callback_?: (() => void)): void {
            const key_type = runtimeType(key)
            const value_type = runtimeType(value)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const key_casted = key as (string)
                const value_casted = value as (string)
                this.setAudioParameter1_serialize(key_casted, value_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const key_casted = key as (string)
                const value_casted = value as (string)
                const callback__casted = callback_ as ((() => void))
                this.setAudioParameter0_serialize(key_casted, value_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public getAudioParameter(key: string, callback_?: ((result: string) => void)): string | void {
            const key_type = runtimeType(key)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const key_casted = key as (string)
                return this.getAudioParameter1_serialize(key_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const key_casted = key as (string)
                const callback__casted = callback_ as (((result: string) => void))
                return this.getAudioParameter0_serialize(key_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public setExtraParameters(mainKey: string, kvpairs: Map<string, string>): void {
            const mainKey_casted = mainKey as (string)
            const kvpairs_casted = kvpairs as (Map<string, string>)
            this.setExtraParameters_serialize(mainKey_casted, kvpairs_casted)
            return
        }
        public getExtraParameters(mainKey: string, subKeys?: Array<string>): Map<string, string> {
            const mainKey_casted = mainKey as (string)
            const subKeys_casted = subKeys as (Array<string> | undefined)
            return this.getExtraParameters_serialize(mainKey_casted, subKeys_casted)
        }
        public setDeviceActive(deviceType: audio_ActiveDeviceType, active: boolean, callback_?: (() => void)): void {
            const deviceType_type = runtimeType(deviceType)
            const active_type = runtimeType(active)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const deviceType_casted = deviceType as (audio_ActiveDeviceType)
                const active_casted = active as (boolean)
                this.setDeviceActive1_serialize(deviceType_casted, active_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const deviceType_casted = deviceType as (audio_ActiveDeviceType)
                const active_casted = active as (boolean)
                const callback__casted = callback_ as ((() => void))
                this.setDeviceActive0_serialize(deviceType_casted, active_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public isDeviceActive(deviceType: audio_ActiveDeviceType, callback_?: ((result: boolean) => void)): boolean | void {
            const deviceType_type = runtimeType(deviceType)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const deviceType_casted = deviceType as (audio_ActiveDeviceType)
                return this.isDeviceActive1_serialize(deviceType_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const deviceType_casted = deviceType as (audio_ActiveDeviceType)
                const callback__casted = callback_ as (((result: boolean) => void))
                return this.isDeviceActive0_serialize(deviceType_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public onVolumeChange(callback_: ((parameter: audio.VolumeEvent) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.VolumeEvent) => void))
            this.onVolumeChange_serialize(callback__casted)
            return
        }
        public onRingerModeChange(callback_: ((parameter: audio_AudioRingMode) => void)): void {
            const callback__casted = callback_ as (((parameter: audio_AudioRingMode) => void))
            this.onRingerModeChange_serialize(callback__casted)
            return
        }
        public setAudioScene(scene: audio_AudioScene, callback_?: (() => void)): void {
            const scene_type = runtimeType(scene)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const scene_casted = scene as (audio_AudioScene)
                this.setAudioScene1_serialize(scene_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const scene_casted = scene as (audio_AudioScene)
                const callback__casted = callback_ as ((() => void))
                this.setAudioScene0_serialize(scene_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public getAudioScene(callback_?: ((result: audio_AudioScene) => void)): audio_AudioScene | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getAudioScene1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: audio_AudioScene) => void))
                return this.getAudioScene0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getAudioSceneSync(): audio_AudioScene {
            return this.getAudioSceneSync_serialize()
        }
        public onDeviceChange(callback_: ((parameter: audio.DeviceChangeAction) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.DeviceChangeAction) => void))
            this.onDeviceChange_serialize(callback__casted)
            return
        }
        public offDeviceChange(callback_?: ((parameter: audio.DeviceChangeAction) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.DeviceChangeAction) => void) | undefined)
            this.offDeviceChange_serialize(callback__casted)
            return
        }
        public onInterrupt(interrupt: audio.AudioInterrupt, callback_: ((parameter: audio.InterruptAction) => void)): void {
            const interrupt_casted = interrupt as (audio.AudioInterrupt)
            const callback__casted = callback_ as (((parameter: audio.InterruptAction) => void))
            this.onInterrupt_serialize(interrupt_casted, callback__casted)
            return
        }
        public offInterrupt(interrupt: audio.AudioInterrupt, callback_?: ((parameter: audio.InterruptAction) => void)): void {
            const interrupt_casted = interrupt as (audio.AudioInterrupt)
            const callback__casted = callback_ as (((parameter: audio.InterruptAction) => void) | undefined)
            this.offInterrupt_serialize(interrupt_casted, callback__casted)
            return
        }
        public getVolumeManager(): audio.AudioVolumeManager {
            return this.getVolumeManager_serialize()
        }
        public getStreamManager(): audio.AudioStreamManager {
            return this.getStreamManager_serialize()
        }
        public getRoutingManager(): audio.AudioRoutingManager {
            return this.getRoutingManager_serialize()
        }
        public getSessionManager(): audio.AudioSessionManager {
            return this.getSessionManager_serialize()
        }
        public getSpatializationManager(): audio.AudioSpatializationManager {
            return this.getSpatializationManager_serialize()
        }
        public disableSafeMediaVolume(): void {
            this.disableSafeMediaVolume_serialize()
            return
        }
        on(type: string, callback_: ((parameter: audio.VolumeEvent) => void)): void {
            throw new Error("TBD")
        }
        off(type: string, callback_: ((parameter: audio.DeviceChangeAction) => void)): void {
            throw new Error("TBD")
        }
        private setVolume0_serialize(volumeType: audio_AudioVolumeType, volume: number, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_setVolume0(this.peer!.ptr, (volumeType.valueOf() as int32), volume, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setVolume1_serialize(volumeType: audio_AudioVolumeType, volume: number): void {
            AUDIONativeModule._AudioManager_setVolume1(this.peer!.ptr, (volumeType.valueOf() as int32), volume)
        }
        private getVolume0_serialize(volumeType: audio_AudioVolumeType, callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_getVolume0(this.peer!.ptr, (volumeType.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getVolume1_serialize(volumeType: audio_AudioVolumeType): number {
            const retval  = AUDIONativeModule._AudioManager_getVolume1(this.peer!.ptr, (volumeType.valueOf() as int32))
            return retval
        }
        private getMinVolume0_serialize(volumeType: audio_AudioVolumeType, callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_getMinVolume0(this.peer!.ptr, (volumeType.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getMinVolume1_serialize(volumeType: audio_AudioVolumeType): number {
            const retval  = AUDIONativeModule._AudioManager_getMinVolume1(this.peer!.ptr, (volumeType.valueOf() as int32))
            return retval
        }
        private getMaxVolume0_serialize(volumeType: audio_AudioVolumeType, callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_getMaxVolume0(this.peer!.ptr, (volumeType.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getMaxVolume1_serialize(volumeType: audio_AudioVolumeType): number {
            const retval  = AUDIONativeModule._AudioManager_getMaxVolume1(this.peer!.ptr, (volumeType.valueOf() as int32))
            return retval
        }
        private getDevices0_serialize(deviceFlag: audio_DeviceFlag, callback_: ((result: audio.AudioDeviceDescriptors) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_getDevices0(this.peer!.ptr, (deviceFlag.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getDevices1_serialize(deviceFlag: audio_DeviceFlag): audio.AudioDeviceDescriptors {
            const retval  = AUDIONativeModule._AudioManager_getDevices1(this.peer!.ptr, (deviceFlag.valueOf() as int32))
            throw new Error("Object deserialization is not implemented.")
        }
        private mute0_serialize(volumeType: audio_AudioVolumeType, mute: boolean, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_mute0(this.peer!.ptr, (volumeType.valueOf() as int32), mute ? 1 : 0, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private mute1_serialize(volumeType: audio_AudioVolumeType, mute: boolean): void {
            AUDIONativeModule._AudioManager_mute1(this.peer!.ptr, (volumeType.valueOf() as int32), mute ? 1 : 0)
        }
        private isMute0_serialize(volumeType: audio_AudioVolumeType, callback_: ((result: boolean) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_isMute0(this.peer!.ptr, (volumeType.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private isMute1_serialize(volumeType: audio_AudioVolumeType): boolean {
            const retval  = AUDIONativeModule._AudioManager_isMute1(this.peer!.ptr, (volumeType.valueOf() as int32))
            return retval
        }
        private isActive0_serialize(volumeType: audio_AudioVolumeType, callback_: ((result: boolean) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_isActive0(this.peer!.ptr, (volumeType.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private isActive1_serialize(volumeType: audio_AudioVolumeType): boolean {
            const retval  = AUDIONativeModule._AudioManager_isActive1(this.peer!.ptr, (volumeType.valueOf() as int32))
            return retval
        }
        private setMicrophoneMute0_serialize(mute: boolean, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_setMicrophoneMute0(this.peer!.ptr, mute ? 1 : 0, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setMicrophoneMute1_serialize(mute: boolean): void {
            AUDIONativeModule._AudioManager_setMicrophoneMute1(this.peer!.ptr, mute ? 1 : 0)
        }
        private isMicrophoneMute0_serialize(callback_: ((result: boolean) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_isMicrophoneMute0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private isMicrophoneMute1_serialize(): boolean {
            const retval  = AUDIONativeModule._AudioManager_isMicrophoneMute1(this.peer!.ptr)
            return retval
        }
        private setRingerMode0_serialize(mode: audio_AudioRingMode, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_setRingerMode0(this.peer!.ptr, (mode.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setRingerMode1_serialize(mode: audio_AudioRingMode): void {
            AUDIONativeModule._AudioManager_setRingerMode1(this.peer!.ptr, (mode.valueOf() as int32))
        }
        private getRingerMode0_serialize(callback_: ((result: audio_AudioRingMode) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_getRingerMode0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getRingerMode1_serialize(): audio_AudioRingMode {
            const retval  = AUDIONativeModule._AudioManager_getRingerMode1(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private setAudioParameter0_serialize(key: string, value: string, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_setAudioParameter0(this.peer!.ptr, key, value, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setAudioParameter1_serialize(key: string, value: string): void {
            AUDIONativeModule._AudioManager_setAudioParameter1(this.peer!.ptr, key, value)
        }
        private getAudioParameter0_serialize(key: string, callback_: ((result: string) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_getAudioParameter0(this.peer!.ptr, key, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getAudioParameter1_serialize(key: string): string {
            const retval  = AUDIONativeModule._AudioManager_getAudioParameter1(this.peer!.ptr, key)
            return retval
        }
        private setExtraParameters_serialize(mainKey: string, kvpairs: Map<string, string>): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeInt32(kvpairs.size as int32)
            // TODO: map serialization not implemented
            AUDIONativeModule._AudioManager_setExtraParameters(this.peer!.ptr, mainKey, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getExtraParameters_serialize(mainKey: string, subKeys?: Array<string>): Map<string, string> {
            const thisSerializer : Serializer = Serializer.hold()
            let subKeys_type : int32 = RuntimeType.UNDEFINED
            subKeys_type = runtimeType(subKeys)
            thisSerializer.writeInt8(subKeys_type as int32)
            if ((RuntimeType.UNDEFINED) != (subKeys_type)) {
                const subKeys_value  = subKeys!
                thisSerializer.writeInt32(subKeys_value.length as int32)
                for (let i = 0; i < subKeys_value.length; i++) {
                    const subKeys_value_element : string = subKeys_value[i]
                    thisSerializer.writeString(subKeys_value_element)
                }
            }
            const retval  = AUDIONativeModule._AudioManager_getExtraParameters(this.peer!.ptr, mainKey, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
            throw new Error("Object deserialization is not implemented.")
        }
        private setDeviceActive0_serialize(deviceType: audio_ActiveDeviceType, active: boolean, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_setDeviceActive0(this.peer!.ptr, (deviceType.valueOf() as int32), active ? 1 : 0, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setDeviceActive1_serialize(deviceType: audio_ActiveDeviceType, active: boolean): void {
            AUDIONativeModule._AudioManager_setDeviceActive1(this.peer!.ptr, (deviceType.valueOf() as int32), active ? 1 : 0)
        }
        private isDeviceActive0_serialize(deviceType: audio_ActiveDeviceType, callback_: ((result: boolean) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_isDeviceActive0(this.peer!.ptr, (deviceType.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private isDeviceActive1_serialize(deviceType: audio_ActiveDeviceType): boolean {
            const retval  = AUDIONativeModule._AudioManager_isDeviceActive1(this.peer!.ptr, (deviceType.valueOf() as int32))
            return retval
        }
        private onVolumeChange_serialize(callback_: ((parameter: audio.VolumeEvent) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_onVolumeChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private onRingerModeChange_serialize(callback_: ((parameter: audio_AudioRingMode) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_onRingerModeChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setAudioScene0_serialize(scene: audio_AudioScene, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_setAudioScene0(this.peer!.ptr, (scene.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setAudioScene1_serialize(scene: audio_AudioScene): void {
            AUDIONativeModule._AudioManager_setAudioScene1(this.peer!.ptr, (scene.valueOf() as int32))
        }
        private getAudioScene0_serialize(callback_: ((result: audio_AudioScene) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_getAudioScene0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getAudioScene1_serialize(): audio_AudioScene {
            const retval  = AUDIONativeModule._AudioManager_getAudioScene1(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getAudioSceneSync_serialize(): audio_AudioScene {
            const retval  = AUDIONativeModule._AudioManager_getAudioSceneSync(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private onDeviceChange_serialize(callback_: ((parameter: audio.DeviceChangeAction) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_onDeviceChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offDeviceChange_serialize(callback_?: ((parameter: audio.DeviceChangeAction) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioManager_offDeviceChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private onInterrupt_serialize(interrupt: audio.AudioInterrupt, callback_: ((parameter: audio.InterruptAction) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioInterrupt(interrupt)
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioManager_onInterrupt(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offInterrupt_serialize(interrupt: audio.AudioInterrupt, callback_?: ((parameter: audio.InterruptAction) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioInterrupt(interrupt)
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioManager_offInterrupt(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getVolumeManager_serialize(): audio.AudioVolumeManager {
            const retval  = AUDIONativeModule._AudioManager_getVolumeManager(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getStreamManager_serialize(): audio.AudioStreamManager {
            const retval  = AUDIONativeModule._AudioManager_getStreamManager(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getRoutingManager_serialize(): audio.AudioRoutingManager {
            const retval  = AUDIONativeModule._AudioManager_getRoutingManager(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getSessionManager_serialize(): audio.AudioSessionManager {
            const retval  = AUDIONativeModule._AudioManager_getSessionManager(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getSpatializationManager_serialize(): audio.AudioSpatializationManager {
            const retval  = AUDIONativeModule._AudioManager_getSpatializationManager(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private disableSafeMediaVolume_serialize(): void {
            AUDIONativeModule._AudioManager_disableSafeMediaVolume(this.peer!.ptr)
        }
        public static fromPtr(ptr: KPointer): AudioManagerInternal {
            const obj : AudioManagerInternal = new AudioManagerInternal()
            obj.peer = new Finalizable(ptr, AudioManagerInternal.getFinalizer())
            return obj
        }
    }
}
export namespace audio {
    export class AudioRoutingManagerInternal implements MaterializedBase,AudioRoutingManager {
        peer?: Finalizable | undefined
        public getPeer(): Finalizable | undefined {
            return this.peer
        }
        static ctor(): KPointer {
            const retval  = AUDIONativeModule._AudioRoutingManager_ctor()
            return retval
        }
         constructor() {
            const ctorPtr : KPointer = AudioRoutingManagerInternal.ctor()
            this.peer = new Finalizable(ctorPtr, AudioRoutingManagerInternal.getFinalizer())
        }
        static getFinalizer(): KPointer {
            return AUDIONativeModule._AudioRoutingManager_getFinalizer()
        }
        public getDevices(deviceFlag: audio_DeviceFlag, callback_?: ((result: audio.AudioDeviceDescriptors) => void)): audio.AudioDeviceDescriptors | void {
            const deviceFlag_type = runtimeType(deviceFlag)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const deviceFlag_casted = deviceFlag as (audio_DeviceFlag)
                return this.getDevices1_serialize(deviceFlag_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const deviceFlag_casted = deviceFlag as (audio_DeviceFlag)
                const callback__casted = callback_ as (((result: audio.AudioDeviceDescriptors) => void))
                return this.getDevices0_serialize(deviceFlag_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getDevicesSync(deviceFlag: audio_DeviceFlag): audio.AudioDeviceDescriptors {
            const deviceFlag_casted = deviceFlag as (audio_DeviceFlag)
            return this.getDevicesSync_serialize(deviceFlag_casted)
        }
        public onDeviceChange(deviceFlag: audio_DeviceFlag, callback_: ((parameter: audio.DeviceChangeAction) => void)): void {
            const deviceFlag_casted = deviceFlag as (audio_DeviceFlag)
            const callback__casted = callback_ as (((parameter: audio.DeviceChangeAction) => void))
            this.onDeviceChange_serialize(deviceFlag_casted, callback__casted)
            return
        }
        public offDeviceChange(callback_?: ((parameter: audio.DeviceChangeAction) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.DeviceChangeAction) => void) | undefined)
            this.offDeviceChange_serialize(callback__casted)
            return
        }
        public getAvailableDevices(deviceUsage: audio_DeviceUsage): audio.AudioDeviceDescriptors {
            const deviceUsage_casted = deviceUsage as (audio_DeviceUsage)
            return this.getAvailableDevices_serialize(deviceUsage_casted)
        }
        public onAvailableDeviceChange(deviceUsage: audio_DeviceUsage, callback_: ((parameter: audio.DeviceChangeAction) => void)): void {
            const deviceUsage_casted = deviceUsage as (audio_DeviceUsage)
            const callback__casted = callback_ as (((parameter: audio.DeviceChangeAction) => void))
            this.onAvailableDeviceChange_serialize(deviceUsage_casted, callback__casted)
            return
        }
        public offAvailableDeviceChange(callback_?: ((parameter: audio.DeviceChangeAction) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.DeviceChangeAction) => void) | undefined)
            this.offAvailableDeviceChange_serialize(callback__casted)
            return
        }
        public setCommunicationDevice(deviceType: audio_CommunicationDeviceType, active: boolean, callback_?: (() => void)): void {
            const deviceType_type = runtimeType(deviceType)
            const active_type = runtimeType(active)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const deviceType_casted = deviceType as (audio_CommunicationDeviceType)
                const active_casted = active as (boolean)
                this.setCommunicationDevice1_serialize(deviceType_casted, active_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const deviceType_casted = deviceType as (audio_CommunicationDeviceType)
                const active_casted = active as (boolean)
                const callback__casted = callback_ as ((() => void))
                this.setCommunicationDevice0_serialize(deviceType_casted, active_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public isCommunicationDeviceActive(deviceType: audio_CommunicationDeviceType, callback_?: ((result: boolean) => void)): boolean | void {
            const deviceType_type = runtimeType(deviceType)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const deviceType_casted = deviceType as (audio_CommunicationDeviceType)
                return this.isCommunicationDeviceActive1_serialize(deviceType_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const deviceType_casted = deviceType as (audio_CommunicationDeviceType)
                const callback__casted = callback_ as (((result: boolean) => void))
                return this.isCommunicationDeviceActive0_serialize(deviceType_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public isCommunicationDeviceActiveSync(deviceType: audio_CommunicationDeviceType): boolean {
            const deviceType_casted = deviceType as (audio_CommunicationDeviceType)
            return this.isCommunicationDeviceActiveSync_serialize(deviceType_casted)
        }
        public selectOutputDevice(outputAudioDevices: audio.AudioDeviceDescriptors, callback_?: (() => void)): void {
            const outputAudioDevices_type = runtimeType(outputAudioDevices)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const outputAudioDevices_casted = outputAudioDevices as (audio.AudioDeviceDescriptors)
                this.selectOutputDevice1_serialize(outputAudioDevices_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const outputAudioDevices_casted = outputAudioDevices as (audio.AudioDeviceDescriptors)
                const callback__casted = callback_ as ((() => void))
                this.selectOutputDevice0_serialize(outputAudioDevices_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public selectOutputDeviceByFilter(filter: audio.AudioRendererFilter, outputAudioDevices: audio.AudioDeviceDescriptors, callback_?: (() => void)): void {
            const filter_type = runtimeType(filter)
            const outputAudioDevices_type = runtimeType(outputAudioDevices)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const filter_casted = filter as (audio.AudioRendererFilter)
                const outputAudioDevices_casted = outputAudioDevices as (audio.AudioDeviceDescriptors)
                this.selectOutputDeviceByFilter1_serialize(filter_casted, outputAudioDevices_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const filter_casted = filter as (audio.AudioRendererFilter)
                const outputAudioDevices_casted = outputAudioDevices as (audio.AudioDeviceDescriptors)
                const callback__casted = callback_ as ((() => void))
                this.selectOutputDeviceByFilter0_serialize(filter_casted, outputAudioDevices_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public selectInputDevice(inputAudioDevices: audio.AudioDeviceDescriptors, callback_?: (() => void)): void {
            const inputAudioDevices_type = runtimeType(inputAudioDevices)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const inputAudioDevices_casted = inputAudioDevices as (audio.AudioDeviceDescriptors)
                this.selectInputDevice1_serialize(inputAudioDevices_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const inputAudioDevices_casted = inputAudioDevices as (audio.AudioDeviceDescriptors)
                const callback__casted = callback_ as ((() => void))
                this.selectInputDevice0_serialize(inputAudioDevices_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public selectInputDeviceByFilter(filter: audio.AudioCapturerFilter, inputAudioDevices: audio.AudioDeviceDescriptors): void {
            const filter_casted = filter as (audio.AudioCapturerFilter)
            const inputAudioDevices_casted = inputAudioDevices as (audio.AudioDeviceDescriptors)
            this.selectInputDeviceByFilter_serialize(filter_casted, inputAudioDevices_casted)
            return
        }
        public getPreferOutputDeviceForRendererInfo(rendererInfo: audio.AudioRendererInfo, callback_?: ((result: audio.AudioDeviceDescriptors) => void)): audio.AudioDeviceDescriptors | void {
            const rendererInfo_type = runtimeType(rendererInfo)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const rendererInfo_casted = rendererInfo as (audio.AudioRendererInfo)
                return this.getPreferOutputDeviceForRendererInfo1_serialize(rendererInfo_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const rendererInfo_casted = rendererInfo as (audio.AudioRendererInfo)
                const callback__casted = callback_ as (((result: audio.AudioDeviceDescriptors) => void))
                return this.getPreferOutputDeviceForRendererInfo0_serialize(rendererInfo_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getPreferredOutputDeviceForRendererInfoSync(rendererInfo: audio.AudioRendererInfo): audio.AudioDeviceDescriptors {
            const rendererInfo_casted = rendererInfo as (audio.AudioRendererInfo)
            return this.getPreferredOutputDeviceForRendererInfoSync_serialize(rendererInfo_casted)
        }
        public getPreferredOutputDeviceByFilter(filter: audio.AudioRendererFilter): audio.AudioDeviceDescriptors {
            const filter_casted = filter as (audio.AudioRendererFilter)
            return this.getPreferredOutputDeviceByFilter_serialize(filter_casted)
        }
        public onPreferOutputDeviceChangeForRendererInfo(rendererInfo: audio.AudioRendererInfo, callback_: ((parameter: audio.AudioDeviceDescriptors) => void)): void {
            const rendererInfo_casted = rendererInfo as (audio.AudioRendererInfo)
            const callback__casted = callback_ as (((parameter: audio.AudioDeviceDescriptors) => void))
            this.onPreferOutputDeviceChangeForRendererInfo_serialize(rendererInfo_casted, callback__casted)
            return
        }
        public offPreferOutputDeviceChangeForRendererInfo(callback_?: ((parameter: audio.AudioDeviceDescriptors) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioDeviceDescriptors) => void) | undefined)
            this.offPreferOutputDeviceChangeForRendererInfo_serialize(callback__casted)
            return
        }
        public getPreferredInputDeviceForCapturerInfo(capturerInfo: audio.AudioCapturerInfo, callback_?: ((result: audio.AudioDeviceDescriptors) => void)): audio.AudioDeviceDescriptors | void {
            const capturerInfo_type = runtimeType(capturerInfo)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const capturerInfo_casted = capturerInfo as (audio.AudioCapturerInfo)
                return this.getPreferredInputDeviceForCapturerInfo1_serialize(capturerInfo_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const capturerInfo_casted = capturerInfo as (audio.AudioCapturerInfo)
                const callback__casted = callback_ as (((result: audio.AudioDeviceDescriptors) => void))
                return this.getPreferredInputDeviceForCapturerInfo0_serialize(capturerInfo_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getPreferredInputDeviceByFilter(filter: audio.AudioCapturerFilter): audio.AudioDeviceDescriptors {
            const filter_casted = filter as (audio.AudioCapturerFilter)
            return this.getPreferredInputDeviceByFilter_serialize(filter_casted)
        }
        public onPreferredInputDeviceChangeForCapturerInfo(capturerInfo: audio.AudioCapturerInfo, callback_: ((parameter: audio.AudioDeviceDescriptors) => void)): void {
            const capturerInfo_casted = capturerInfo as (audio.AudioCapturerInfo)
            const callback__casted = callback_ as (((parameter: audio.AudioDeviceDescriptors) => void))
            this.onPreferredInputDeviceChangeForCapturerInfo_serialize(capturerInfo_casted, callback__casted)
            return
        }
        public offPreferredInputDeviceChangeForCapturerInfo(callback_?: ((parameter: audio.AudioDeviceDescriptors) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioDeviceDescriptors) => void) | undefined)
            this.offPreferredInputDeviceChangeForCapturerInfo_serialize(callback__casted)
            return
        }
        public getPreferredInputDeviceForCapturerInfoSync(capturerInfo: audio.AudioCapturerInfo): audio.AudioDeviceDescriptors {
            const capturerInfo_casted = capturerInfo as (audio.AudioCapturerInfo)
            return this.getPreferredInputDeviceForCapturerInfoSync_serialize(capturerInfo_casted)
        }
        public isMicBlockDetectionSupported(): boolean {
            return this.isMicBlockDetectionSupported_serialize()
        }
        public onMicBlockStatusChanged(callback_: ((parameter: audio.DeviceBlockStatusInfo) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.DeviceBlockStatusInfo) => void))
            this.onMicBlockStatusChanged_serialize(callback__casted)
            return
        }
        public offMicBlockStatusChanged(callback_?: ((parameter: audio.DeviceBlockStatusInfo) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.DeviceBlockStatusInfo) => void) | undefined)
            this.offMicBlockStatusChanged_serialize(callback__casted)
            return
        }
        on(type: string, deviceFlag: audio_DeviceFlag, callback_: ((parameter: audio.DeviceChangeAction) => void)): void {
            throw new Error("TBD")
        }
        off(type: string, callback_: ((parameter: audio.DeviceChangeAction) => void)): void {
            throw new Error("TBD")
        }
        private getDevices0_serialize(deviceFlag: audio_DeviceFlag, callback_: ((result: audio.AudioDeviceDescriptors) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRoutingManager_getDevices0(this.peer!.ptr, (deviceFlag.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getDevices1_serialize(deviceFlag: audio_DeviceFlag): audio.AudioDeviceDescriptors {
            const retval  = AUDIONativeModule._AudioRoutingManager_getDevices1(this.peer!.ptr, (deviceFlag.valueOf() as int32))
            throw new Error("Object deserialization is not implemented.")
        }
        private getDevicesSync_serialize(deviceFlag: audio_DeviceFlag): audio.AudioDeviceDescriptors {
            const retval  = AUDIONativeModule._AudioRoutingManager_getDevicesSync(this.peer!.ptr, (deviceFlag.valueOf() as int32))
            throw new Error("Object deserialization is not implemented.")
        }
        private onDeviceChange_serialize(deviceFlag: audio_DeviceFlag, callback_: ((parameter: audio.DeviceChangeAction) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRoutingManager_onDeviceChange(this.peer!.ptr, (deviceFlag.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offDeviceChange_serialize(callback_?: ((parameter: audio.DeviceChangeAction) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioRoutingManager_offDeviceChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getAvailableDevices_serialize(deviceUsage: audio_DeviceUsage): audio.AudioDeviceDescriptors {
            const retval  = AUDIONativeModule._AudioRoutingManager_getAvailableDevices(this.peer!.ptr, (deviceUsage.valueOf() as int32))
            throw new Error("Object deserialization is not implemented.")
        }
        private onAvailableDeviceChange_serialize(deviceUsage: audio_DeviceUsage, callback_: ((parameter: audio.DeviceChangeAction) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRoutingManager_onAvailableDeviceChange(this.peer!.ptr, (deviceUsage.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offAvailableDeviceChange_serialize(callback_?: ((parameter: audio.DeviceChangeAction) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioRoutingManager_offAvailableDeviceChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setCommunicationDevice0_serialize(deviceType: audio_CommunicationDeviceType, active: boolean, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRoutingManager_setCommunicationDevice0(this.peer!.ptr, (deviceType.valueOf() as int32), active ? 1 : 0, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setCommunicationDevice1_serialize(deviceType: audio_CommunicationDeviceType, active: boolean): void {
            AUDIONativeModule._AudioRoutingManager_setCommunicationDevice1(this.peer!.ptr, (deviceType.valueOf() as int32), active ? 1 : 0)
        }
        private isCommunicationDeviceActive0_serialize(deviceType: audio_CommunicationDeviceType, callback_: ((result: boolean) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRoutingManager_isCommunicationDeviceActive0(this.peer!.ptr, (deviceType.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private isCommunicationDeviceActive1_serialize(deviceType: audio_CommunicationDeviceType): boolean {
            const retval  = AUDIONativeModule._AudioRoutingManager_isCommunicationDeviceActive1(this.peer!.ptr, (deviceType.valueOf() as int32))
            return retval
        }
        private isCommunicationDeviceActiveSync_serialize(deviceType: audio_CommunicationDeviceType): boolean {
            const retval  = AUDIONativeModule._AudioRoutingManager_isCommunicationDeviceActiveSync(this.peer!.ptr, (deviceType.valueOf() as int32))
            return retval
        }
        private selectOutputDevice0_serialize(outputAudioDevices: audio.AudioDeviceDescriptors, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeInt32(outputAudioDevices.length as int32)
            for (let i = 0; i < outputAudioDevices.length; i++) {
                const outputAudioDevices_element : Readonly<AudioDeviceDescriptor> = outputAudioDevices[i]
                thisSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", outputAudioDevices_element)
            }
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRoutingManager_selectOutputDevice0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private selectOutputDevice1_serialize(outputAudioDevices: audio.AudioDeviceDescriptors): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeInt32(outputAudioDevices.length as int32)
            for (let i = 0; i < outputAudioDevices.length; i++) {
                const outputAudioDevices_element : Readonly<AudioDeviceDescriptor> = outputAudioDevices[i]
                thisSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", outputAudioDevices_element)
            }
            AUDIONativeModule._AudioRoutingManager_selectOutputDevice1(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private selectOutputDeviceByFilter0_serialize(filter: audio.AudioRendererFilter, outputAudioDevices: audio.AudioDeviceDescriptors, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioRendererFilter(filter)
            thisSerializer.writeInt32(outputAudioDevices.length as int32)
            for (let i = 0; i < outputAudioDevices.length; i++) {
                const outputAudioDevices_element : Readonly<AudioDeviceDescriptor> = outputAudioDevices[i]
                thisSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", outputAudioDevices_element)
            }
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRoutingManager_selectOutputDeviceByFilter0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private selectOutputDeviceByFilter1_serialize(filter: audio.AudioRendererFilter, outputAudioDevices: audio.AudioDeviceDescriptors): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioRendererFilter(filter)
            thisSerializer.writeInt32(outputAudioDevices.length as int32)
            for (let i = 0; i < outputAudioDevices.length; i++) {
                const outputAudioDevices_element : Readonly<AudioDeviceDescriptor> = outputAudioDevices[i]
                thisSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", outputAudioDevices_element)
            }
            AUDIONativeModule._AudioRoutingManager_selectOutputDeviceByFilter1(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private selectInputDevice0_serialize(inputAudioDevices: audio.AudioDeviceDescriptors, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeInt32(inputAudioDevices.length as int32)
            for (let i = 0; i < inputAudioDevices.length; i++) {
                const inputAudioDevices_element : Readonly<AudioDeviceDescriptor> = inputAudioDevices[i]
                thisSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", inputAudioDevices_element)
            }
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRoutingManager_selectInputDevice0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private selectInputDevice1_serialize(inputAudioDevices: audio.AudioDeviceDescriptors): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeInt32(inputAudioDevices.length as int32)
            for (let i = 0; i < inputAudioDevices.length; i++) {
                const inputAudioDevices_element : Readonly<AudioDeviceDescriptor> = inputAudioDevices[i]
                thisSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", inputAudioDevices_element)
            }
            AUDIONativeModule._AudioRoutingManager_selectInputDevice1(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private selectInputDeviceByFilter_serialize(filter: audio.AudioCapturerFilter, inputAudioDevices: audio.AudioDeviceDescriptors): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioCapturerFilter(filter)
            thisSerializer.writeInt32(inputAudioDevices.length as int32)
            for (let i = 0; i < inputAudioDevices.length; i++) {
                const inputAudioDevices_element : Readonly<AudioDeviceDescriptor> = inputAudioDevices[i]
                thisSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", inputAudioDevices_element)
            }
            AUDIONativeModule._AudioRoutingManager_selectInputDeviceByFilter(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getPreferOutputDeviceForRendererInfo0_serialize(rendererInfo: audio.AudioRendererInfo, callback_: ((result: audio.AudioDeviceDescriptors) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioRendererInfo(rendererInfo)
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRoutingManager_getPreferOutputDeviceForRendererInfo0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getPreferOutputDeviceForRendererInfo1_serialize(rendererInfo: audio.AudioRendererInfo): audio.AudioDeviceDescriptors {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioRendererInfo(rendererInfo)
            const retval  = AUDIONativeModule._AudioRoutingManager_getPreferOutputDeviceForRendererInfo1(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
            throw new Error("Object deserialization is not implemented.")
        }
        private getPreferredOutputDeviceForRendererInfoSync_serialize(rendererInfo: audio.AudioRendererInfo): audio.AudioDeviceDescriptors {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioRendererInfo(rendererInfo)
            const retval  = AUDIONativeModule._AudioRoutingManager_getPreferredOutputDeviceForRendererInfoSync(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
            throw new Error("Object deserialization is not implemented.")
        }
        private getPreferredOutputDeviceByFilter_serialize(filter: audio.AudioRendererFilter): audio.AudioDeviceDescriptors {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioRendererFilter(filter)
            const retval  = AUDIONativeModule._AudioRoutingManager_getPreferredOutputDeviceByFilter(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
            throw new Error("Object deserialization is not implemented.")
        }
        private onPreferOutputDeviceChangeForRendererInfo_serialize(rendererInfo: audio.AudioRendererInfo, callback_: ((parameter: audio.AudioDeviceDescriptors) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioRendererInfo(rendererInfo)
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRoutingManager_onPreferOutputDeviceChangeForRendererInfo(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offPreferOutputDeviceChangeForRendererInfo_serialize(callback_?: ((parameter: audio.AudioDeviceDescriptors) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioRoutingManager_offPreferOutputDeviceChangeForRendererInfo(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getPreferredInputDeviceForCapturerInfo0_serialize(capturerInfo: audio.AudioCapturerInfo, callback_: ((result: audio.AudioDeviceDescriptors) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioCapturerInfo(capturerInfo)
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRoutingManager_getPreferredInputDeviceForCapturerInfo0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getPreferredInputDeviceForCapturerInfo1_serialize(capturerInfo: audio.AudioCapturerInfo): audio.AudioDeviceDescriptors {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioCapturerInfo(capturerInfo)
            const retval  = AUDIONativeModule._AudioRoutingManager_getPreferredInputDeviceForCapturerInfo1(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
            throw new Error("Object deserialization is not implemented.")
        }
        private getPreferredInputDeviceByFilter_serialize(filter: audio.AudioCapturerFilter): audio.AudioDeviceDescriptors {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioCapturerFilter(filter)
            const retval  = AUDIONativeModule._AudioRoutingManager_getPreferredInputDeviceByFilter(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
            throw new Error("Object deserialization is not implemented.")
        }
        private onPreferredInputDeviceChangeForCapturerInfo_serialize(capturerInfo: audio.AudioCapturerInfo, callback_: ((parameter: audio.AudioDeviceDescriptors) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioCapturerInfo(capturerInfo)
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRoutingManager_onPreferredInputDeviceChangeForCapturerInfo(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offPreferredInputDeviceChangeForCapturerInfo_serialize(callback_?: ((parameter: audio.AudioDeviceDescriptors) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioRoutingManager_offPreferredInputDeviceChangeForCapturerInfo(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getPreferredInputDeviceForCapturerInfoSync_serialize(capturerInfo: audio.AudioCapturerInfo): audio.AudioDeviceDescriptors {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioCapturerInfo(capturerInfo)
            const retval  = AUDIONativeModule._AudioRoutingManager_getPreferredInputDeviceForCapturerInfoSync(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
            throw new Error("Object deserialization is not implemented.")
        }
        private isMicBlockDetectionSupported_serialize(): boolean {
            const retval  = AUDIONativeModule._AudioRoutingManager_isMicBlockDetectionSupported(this.peer!.ptr)
            return retval
        }
        private onMicBlockStatusChanged_serialize(callback_: ((parameter: audio.DeviceBlockStatusInfo) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRoutingManager_onMicBlockStatusChanged(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offMicBlockStatusChanged_serialize(callback_?: ((parameter: audio.DeviceBlockStatusInfo) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioRoutingManager_offMicBlockStatusChanged(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        public static fromPtr(ptr: KPointer): AudioRoutingManagerInternal {
            const obj : AudioRoutingManagerInternal = new AudioRoutingManagerInternal()
            obj.peer = new Finalizable(ptr, AudioRoutingManagerInternal.getFinalizer())
            return obj
        }
    }
}
export namespace audio {
    export class AudioStreamManagerInternal implements MaterializedBase,AudioStreamManager {
        peer?: Finalizable | undefined
        public getPeer(): Finalizable | undefined {
            return this.peer
        }
        static ctor(): KPointer {
            const retval  = AUDIONativeModule._AudioStreamManager_ctor()
            return retval
        }
         constructor() {
            const ctorPtr : KPointer = AudioStreamManagerInternal.ctor()
            this.peer = new Finalizable(ctorPtr, AudioStreamManagerInternal.getFinalizer())
        }
        static getFinalizer(): KPointer {
            return AUDIONativeModule._AudioStreamManager_getFinalizer()
        }
        public getCurrentAudioRendererInfoArray(callback_?: ((result: audio.AudioRendererChangeInfoArray) => void)): audio.AudioRendererChangeInfoArray | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getCurrentAudioRendererInfoArray1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: audio.AudioRendererChangeInfoArray) => void))
                return this.getCurrentAudioRendererInfoArray0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getCurrentAudioRendererInfoArraySync(): audio.AudioRendererChangeInfoArray {
            return this.getCurrentAudioRendererInfoArraySync_serialize()
        }
        public getCurrentAudioCapturerInfoArray(callback_?: ((result: audio.AudioCapturerChangeInfoArray) => void)): audio.AudioCapturerChangeInfoArray | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getCurrentAudioCapturerInfoArray1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: audio.AudioCapturerChangeInfoArray) => void))
                return this.getCurrentAudioCapturerInfoArray0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getCurrentAudioCapturerInfoArraySync(): audio.AudioCapturerChangeInfoArray {
            return this.getCurrentAudioCapturerInfoArraySync_serialize()
        }
        public getAudioEffectInfoArray(usage: audio_StreamUsage, callback_?: ((result: audio.AudioEffectInfoArray) => void)): audio.AudioEffectInfoArray | void {
            const usage_type = runtimeType(usage)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const usage_casted = usage as (audio_StreamUsage)
                return this.getAudioEffectInfoArray1_serialize(usage_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const usage_casted = usage as (audio_StreamUsage)
                const callback__casted = callback_ as (((result: audio.AudioEffectInfoArray) => void))
                return this.getAudioEffectInfoArray0_serialize(usage_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getAudioEffectInfoArraySync(usage: audio_StreamUsage): audio.AudioEffectInfoArray {
            const usage_casted = usage as (audio_StreamUsage)
            return this.getAudioEffectInfoArraySync_serialize(usage_casted)
        }
        public onAudioRendererChange(callback_: ((parameter: audio.AudioRendererChangeInfoArray) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioRendererChangeInfoArray) => void))
            this.onAudioRendererChange_serialize(callback__casted)
            return
        }
        public offAudioRendererChange(): void {
            this.offAudioRendererChange_serialize()
            return
        }
        public onAudioCapturerChange(callback_: ((parameter: audio.AudioCapturerChangeInfoArray) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioCapturerChangeInfoArray) => void))
            this.onAudioCapturerChange_serialize(callback__casted)
            return
        }
        public offAudioCapturerChange(): void {
            this.offAudioCapturerChange_serialize()
            return
        }
        public isActive(volumeType: audio_AudioVolumeType, callback_?: ((result: boolean) => void)): boolean | void {
            const volumeType_type = runtimeType(volumeType)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                return this.isActive1_serialize(volumeType_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const callback__casted = callback_ as (((result: boolean) => void))
                return this.isActive0_serialize(volumeType_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public isActiveSync(volumeType: audio_AudioVolumeType): boolean {
            const volumeType_casted = volumeType as (audio_AudioVolumeType)
            return this.isActiveSync_serialize(volumeType_casted)
        }
        on(type: string, callback_: ((parameter: audio.AudioRendererChangeInfoArray) => void)): void {
            throw new Error("TBD")
        }
        off(type: string): void {
            throw new Error("TBD")
        }
        private getCurrentAudioRendererInfoArray0_serialize(callback_: ((result: audio.AudioRendererChangeInfoArray) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioStreamManager_getCurrentAudioRendererInfoArray0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getCurrentAudioRendererInfoArray1_serialize(): audio.AudioRendererChangeInfoArray {
            const retval  = AUDIONativeModule._AudioStreamManager_getCurrentAudioRendererInfoArray1(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getCurrentAudioRendererInfoArraySync_serialize(): audio.AudioRendererChangeInfoArray {
            const retval  = AUDIONativeModule._AudioStreamManager_getCurrentAudioRendererInfoArraySync(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getCurrentAudioCapturerInfoArray0_serialize(callback_: ((result: audio.AudioCapturerChangeInfoArray) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioStreamManager_getCurrentAudioCapturerInfoArray0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getCurrentAudioCapturerInfoArray1_serialize(): audio.AudioCapturerChangeInfoArray {
            const retval  = AUDIONativeModule._AudioStreamManager_getCurrentAudioCapturerInfoArray1(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getCurrentAudioCapturerInfoArraySync_serialize(): audio.AudioCapturerChangeInfoArray {
            const retval  = AUDIONativeModule._AudioStreamManager_getCurrentAudioCapturerInfoArraySync(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getAudioEffectInfoArray0_serialize(usage: audio_StreamUsage, callback_: ((result: audio.AudioEffectInfoArray) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioStreamManager_getAudioEffectInfoArray0(this.peer!.ptr, (usage.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getAudioEffectInfoArray1_serialize(usage: audio_StreamUsage): audio.AudioEffectInfoArray {
            const retval  = AUDIONativeModule._AudioStreamManager_getAudioEffectInfoArray1(this.peer!.ptr, (usage.valueOf() as int32))
            throw new Error("Object deserialization is not implemented.")
        }
        private getAudioEffectInfoArraySync_serialize(usage: audio_StreamUsage): audio.AudioEffectInfoArray {
            const retval  = AUDIONativeModule._AudioStreamManager_getAudioEffectInfoArraySync(this.peer!.ptr, (usage.valueOf() as int32))
            throw new Error("Object deserialization is not implemented.")
        }
        private onAudioRendererChange_serialize(callback_: ((parameter: audio.AudioRendererChangeInfoArray) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioStreamManager_onAudioRendererChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offAudioRendererChange_serialize(): void {
            AUDIONativeModule._AudioStreamManager_offAudioRendererChange(this.peer!.ptr)
        }
        private onAudioCapturerChange_serialize(callback_: ((parameter: audio.AudioCapturerChangeInfoArray) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioStreamManager_onAudioCapturerChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offAudioCapturerChange_serialize(): void {
            AUDIONativeModule._AudioStreamManager_offAudioCapturerChange(this.peer!.ptr)
        }
        private isActive0_serialize(volumeType: audio_AudioVolumeType, callback_: ((result: boolean) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioStreamManager_isActive0(this.peer!.ptr, (volumeType.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private isActive1_serialize(volumeType: audio_AudioVolumeType): boolean {
            const retval  = AUDIONativeModule._AudioStreamManager_isActive1(this.peer!.ptr, (volumeType.valueOf() as int32))
            return retval
        }
        private isActiveSync_serialize(volumeType: audio_AudioVolumeType): boolean {
            const retval  = AUDIONativeModule._AudioStreamManager_isActiveSync(this.peer!.ptr, (volumeType.valueOf() as int32))
            return retval
        }
        public static fromPtr(ptr: KPointer): AudioStreamManagerInternal {
            const obj : AudioStreamManagerInternal = new AudioStreamManagerInternal()
            obj.peer = new Finalizable(ptr, AudioStreamManagerInternal.getFinalizer())
            return obj
        }
    }
}
export namespace audio {
    export class AudioSessionManagerInternal implements MaterializedBase,AudioSessionManager {
        peer?: Finalizable | undefined
        public getPeer(): Finalizable | undefined {
            return this.peer
        }
        static ctor(): KPointer {
            const retval  = AUDIONativeModule._AudioSessionManager_ctor()
            return retval
        }
         constructor() {
            const ctorPtr : KPointer = AudioSessionManagerInternal.ctor()
            this.peer = new Finalizable(ctorPtr, AudioSessionManagerInternal.getFinalizer())
        }
        static getFinalizer(): KPointer {
            return AUDIONativeModule._AudioSessionManager_getFinalizer()
        }
        public activateAudioSession(strategy: audio.AudioSessionStrategy): void {
            const strategy_casted = strategy as (audio.AudioSessionStrategy)
            this.activateAudioSession_serialize(strategy_casted)
            return
        }
        public deactivateAudioSession(): void {
            this.deactivateAudioSession_serialize()
            return
        }
        public isAudioSessionActivated(): boolean {
            return this.isAudioSessionActivated_serialize()
        }
        public onAudioSessionDeactivated(callback_: ((parameter: audio.AudioSessionDeactivatedEvent) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioSessionDeactivatedEvent) => void))
            this.onAudioSessionDeactivated_serialize(callback__casted)
            return
        }
        public offAudioSessionDeactivated(callback_?: ((parameter: audio.AudioSessionDeactivatedEvent) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioSessionDeactivatedEvent) => void) | undefined)
            this.offAudioSessionDeactivated_serialize(callback__casted)
            return
        }
        on(type: string, callback_: ((parameter: audio.AudioSessionDeactivatedEvent) => void)): void {
            throw new Error("TBD")
        }
        off(type: string, callback_: ((parameter: audio.AudioSessionDeactivatedEvent) => void)): void {
            throw new Error("TBD")
        }
        private activateAudioSession_serialize(strategy: audio.AudioSessionStrategy): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioSessionStrategy(strategy)
            AUDIONativeModule._AudioSessionManager_activateAudioSession(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private deactivateAudioSession_serialize(): void {
            AUDIONativeModule._AudioSessionManager_deactivateAudioSession(this.peer!.ptr)
        }
        private isAudioSessionActivated_serialize(): boolean {
            const retval  = AUDIONativeModule._AudioSessionManager_isAudioSessionActivated(this.peer!.ptr)
            return retval
        }
        private onAudioSessionDeactivated_serialize(callback_: ((parameter: audio.AudioSessionDeactivatedEvent) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioSessionManager_onAudioSessionDeactivated(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offAudioSessionDeactivated_serialize(callback_?: ((parameter: audio.AudioSessionDeactivatedEvent) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioSessionManager_offAudioSessionDeactivated(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        public static fromPtr(ptr: KPointer): AudioSessionManagerInternal {
            const obj : AudioSessionManagerInternal = new AudioSessionManagerInternal()
            obj.peer = new Finalizable(ptr, AudioSessionManagerInternal.getFinalizer())
            return obj
        }
    }
}
export namespace audio {
    export class AudioVolumeManagerInternal implements MaterializedBase,AudioVolumeManager {
        peer?: Finalizable | undefined
        public getPeer(): Finalizable | undefined {
            return this.peer
        }
        static ctor(): KPointer {
            const retval  = AUDIONativeModule._AudioVolumeManager_ctor()
            return retval
        }
         constructor() {
            const ctorPtr : KPointer = AudioVolumeManagerInternal.ctor()
            this.peer = new Finalizable(ctorPtr, AudioVolumeManagerInternal.getFinalizer())
        }
        static getFinalizer(): KPointer {
            return AUDIONativeModule._AudioVolumeManager_getFinalizer()
        }
        public getVolumeGroupInfos(networkId: string, callback_?: ((result: audio.VolumeGroupInfos) => void)): audio.VolumeGroupInfos | void {
            const networkId_type = runtimeType(networkId)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const networkId_casted = networkId as (string)
                return this.getVolumeGroupInfos1_serialize(networkId_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const networkId_casted = networkId as (string)
                const callback__casted = callback_ as (((result: audio.VolumeGroupInfos) => void))
                return this.getVolumeGroupInfos0_serialize(networkId_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getVolumeGroupInfosSync(networkId: string): audio.VolumeGroupInfos {
            const networkId_casted = networkId as (string)
            return this.getVolumeGroupInfosSync_serialize(networkId_casted)
        }
        public getVolumeGroupManager(groupId: number, callback_?: ((result: audio.AudioVolumeGroupManager) => void)): audio.AudioVolumeGroupManager | void {
            const groupId_type = runtimeType(groupId)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const groupId_casted = groupId as (number)
                return this.getVolumeGroupManager1_serialize(groupId_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const groupId_casted = groupId as (number)
                const callback__casted = callback_ as (((result: audio.AudioVolumeGroupManager) => void))
                return this.getVolumeGroupManager0_serialize(groupId_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getVolumeGroupManagerSync(groupId: number): audio.AudioVolumeGroupManager {
            const groupId_casted = groupId as (number)
            return this.getVolumeGroupManagerSync_serialize(groupId_casted)
        }
        public onVolumeChange(callback_: ((parameter: audio.VolumeEvent) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.VolumeEvent) => void))
            this.onVolumeChange_serialize(callback__casted)
            return
        }
        public offVolumeChange(callback_?: ((parameter: audio.VolumeEvent) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.VolumeEvent) => void) | undefined)
            this.offVolumeChange_serialize(callback__casted)
            return
        }
        on(type: string, callback_: ((parameter: audio.VolumeEvent) => void)): void {
            throw new Error("TBD")
        }
        off(type: string, callback_: ((parameter: audio.VolumeEvent) => void)): void {
            throw new Error("TBD")
        }
        private getVolumeGroupInfos0_serialize(networkId: string, callback_: ((result: audio.VolumeGroupInfos) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeManager_getVolumeGroupInfos0(this.peer!.ptr, networkId, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getVolumeGroupInfos1_serialize(networkId: string): audio.VolumeGroupInfos {
            const retval  = AUDIONativeModule._AudioVolumeManager_getVolumeGroupInfos1(this.peer!.ptr, networkId)
            throw new Error("Object deserialization is not implemented.")
        }
        private getVolumeGroupInfosSync_serialize(networkId: string): audio.VolumeGroupInfos {
            const retval  = AUDIONativeModule._AudioVolumeManager_getVolumeGroupInfosSync(this.peer!.ptr, networkId)
            throw new Error("Object deserialization is not implemented.")
        }
        private getVolumeGroupManager0_serialize(groupId: number, callback_: ((result: audio.AudioVolumeGroupManager) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeManager_getVolumeGroupManager0(this.peer!.ptr, groupId, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getVolumeGroupManager1_serialize(groupId: number): audio.AudioVolumeGroupManager {
            const retval  = AUDIONativeModule._AudioVolumeManager_getVolumeGroupManager1(this.peer!.ptr, groupId)
            throw new Error("Object deserialization is not implemented.")
        }
        private getVolumeGroupManagerSync_serialize(groupId: number): audio.AudioVolumeGroupManager {
            const retval  = AUDIONativeModule._AudioVolumeManager_getVolumeGroupManagerSync(this.peer!.ptr, groupId)
            throw new Error("Object deserialization is not implemented.")
        }
        private onVolumeChange_serialize(callback_: ((parameter: audio.VolumeEvent) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeManager_onVolumeChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offVolumeChange_serialize(callback_?: ((parameter: audio.VolumeEvent) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioVolumeManager_offVolumeChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        public static fromPtr(ptr: KPointer): AudioVolumeManagerInternal {
            const obj : AudioVolumeManagerInternal = new AudioVolumeManagerInternal()
            obj.peer = new Finalizable(ptr, AudioVolumeManagerInternal.getFinalizer())
            return obj
        }
    }
}
export namespace audio {
    export class AudioVolumeGroupManagerInternal implements MaterializedBase,AudioVolumeGroupManager {
        peer?: Finalizable | undefined
        public getPeer(): Finalizable | undefined {
            return this.peer
        }
        static ctor(): KPointer {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_ctor()
            return retval
        }
         constructor() {
            const ctorPtr : KPointer = AudioVolumeGroupManagerInternal.ctor()
            this.peer = new Finalizable(ctorPtr, AudioVolumeGroupManagerInternal.getFinalizer())
        }
        static getFinalizer(): KPointer {
            return AUDIONativeModule._AudioVolumeGroupManager_getFinalizer()
        }
        public setVolume(volumeType: audio_AudioVolumeType, volume: number, callback_?: (() => void)): void {
            const volumeType_type = runtimeType(volumeType)
            const volume_type = runtimeType(volume)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const volume_casted = volume as (number)
                this.setVolume1_serialize(volumeType_casted, volume_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const volume_casted = volume as (number)
                const callback__casted = callback_ as ((() => void))
                this.setVolume0_serialize(volumeType_casted, volume_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public setVolumeWithFlag(volumeType: audio_AudioVolumeType, volume: number, flags: number): void {
            const volumeType_casted = volumeType as (audio_AudioVolumeType)
            const volume_casted = volume as (number)
            const flags_casted = flags as (number)
            this.setVolumeWithFlag_serialize(volumeType_casted, volume_casted, flags_casted)
            return
        }
        public getActiveVolumeTypeSync(uid: number): audio_AudioVolumeType {
            const uid_casted = uid as (number)
            return this.getActiveVolumeTypeSync_serialize(uid_casted)
        }
        public getVolume(volumeType: audio_AudioVolumeType, callback_?: ((result: number) => void)): number | void {
            const volumeType_type = runtimeType(volumeType)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                return this.getVolume1_serialize(volumeType_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const callback__casted = callback_ as (((result: number) => void))
                return this.getVolume0_serialize(volumeType_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getVolumeSync(volumeType: audio_AudioVolumeType): number {
            const volumeType_casted = volumeType as (audio_AudioVolumeType)
            return this.getVolumeSync_serialize(volumeType_casted)
        }
        public getMinVolume(volumeType: audio_AudioVolumeType, callback_?: ((result: number) => void)): number | void {
            const volumeType_type = runtimeType(volumeType)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                return this.getMinVolume1_serialize(volumeType_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const callback__casted = callback_ as (((result: number) => void))
                return this.getMinVolume0_serialize(volumeType_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getMinVolumeSync(volumeType: audio_AudioVolumeType): number {
            const volumeType_casted = volumeType as (audio_AudioVolumeType)
            return this.getMinVolumeSync_serialize(volumeType_casted)
        }
        public getMaxVolume(volumeType: audio_AudioVolumeType, callback_?: ((result: number) => void)): number | void {
            const volumeType_type = runtimeType(volumeType)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                return this.getMaxVolume1_serialize(volumeType_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const callback__casted = callback_ as (((result: number) => void))
                return this.getMaxVolume0_serialize(volumeType_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getMaxVolumeSync(volumeType: audio_AudioVolumeType): number {
            const volumeType_casted = volumeType as (audio_AudioVolumeType)
            return this.getMaxVolumeSync_serialize(volumeType_casted)
        }
        public mute(volumeType: audio_AudioVolumeType, mute: boolean, callback_?: (() => void)): void {
            const volumeType_type = runtimeType(volumeType)
            const mute_type = runtimeType(mute)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const mute_casted = mute as (boolean)
                this.mute1_serialize(volumeType_casted, mute_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const mute_casted = mute as (boolean)
                const callback__casted = callback_ as ((() => void))
                this.mute0_serialize(volumeType_casted, mute_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public isMute(volumeType: audio_AudioVolumeType, callback_?: ((result: boolean) => void)): boolean | void {
            const volumeType_type = runtimeType(volumeType)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                return this.isMute1_serialize(volumeType_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const callback__casted = callback_ as (((result: boolean) => void))
                return this.isMute0_serialize(volumeType_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public isMuteSync(volumeType: audio_AudioVolumeType): boolean {
            const volumeType_casted = volumeType as (audio_AudioVolumeType)
            return this.isMuteSync_serialize(volumeType_casted)
        }
        public setRingerMode(mode: audio_AudioRingMode, callback_?: (() => void)): void {
            const mode_type = runtimeType(mode)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const mode_casted = mode as (audio_AudioRingMode)
                this.setRingerMode1_serialize(mode_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const mode_casted = mode as (audio_AudioRingMode)
                const callback__casted = callback_ as ((() => void))
                this.setRingerMode0_serialize(mode_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public getRingerMode(callback_?: ((result: audio_AudioRingMode) => void)): audio_AudioRingMode | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getRingerMode1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: audio_AudioRingMode) => void))
                return this.getRingerMode0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getRingerModeSync(): audio_AudioRingMode {
            return this.getRingerModeSync_serialize()
        }
        public onRingerModeChange(callback_: ((parameter: audio_AudioRingMode) => void)): void {
            const callback__casted = callback_ as (((parameter: audio_AudioRingMode) => void))
            this.onRingerModeChange_serialize(callback__casted)
            return
        }
        public setMicrophoneMute(mute: boolean, callback_?: (() => void)): void {
            const mute_type = runtimeType(mute)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const mute_casted = mute as (boolean)
                this.setMicrophoneMute1_serialize(mute_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const mute_casted = mute as (boolean)
                const callback__casted = callback_ as ((() => void))
                this.setMicrophoneMute0_serialize(mute_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public setMicMute(mute: boolean): void {
            const mute_casted = mute as (boolean)
            this.setMicMute_serialize(mute_casted)
            return
        }
        public setMicMutePersistent(mute: boolean, type: audio_PolicyType): void {
            const mute_casted = mute as (boolean)
            const type_casted = type as (audio_PolicyType)
            this.setMicMutePersistent_serialize(mute_casted, type_casted)
            return
        }
        public isPersistentMicMute(): boolean {
            return this.isPersistentMicMute_serialize()
        }
        public isMicrophoneMute(callback_?: ((result: boolean) => void)): boolean | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.isMicrophoneMute1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: boolean) => void))
                return this.isMicrophoneMute0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public isMicrophoneMuteSync(): boolean {
            return this.isMicrophoneMuteSync_serialize()
        }
        public onMicStateChange(callback_: ((parameter: audio.MicStateChangeEvent) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.MicStateChangeEvent) => void))
            this.onMicStateChange_serialize(callback__casted)
            return
        }
        public offMicStateChange(callback_?: ((parameter: audio.MicStateChangeEvent) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.MicStateChangeEvent) => void) | undefined)
            this.offMicStateChange_serialize(callback__casted)
            return
        }
        public isVolumeUnadjustable(): boolean {
            return this.isVolumeUnadjustable_serialize()
        }
        public adjustVolumeByStep(adjustType: audio_VolumeAdjustType, callback_?: (() => void)): void {
            const adjustType_type = runtimeType(adjustType)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const adjustType_casted = adjustType as (audio_VolumeAdjustType)
                this.adjustVolumeByStep1_serialize(adjustType_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const adjustType_casted = adjustType as (audio_VolumeAdjustType)
                const callback__casted = callback_ as ((() => void))
                this.adjustVolumeByStep0_serialize(adjustType_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public adjustSystemVolumeByStep(volumeType: audio_AudioVolumeType, adjustType: audio_VolumeAdjustType, callback_?: (() => void)): void {
            const volumeType_type = runtimeType(volumeType)
            const adjustType_type = runtimeType(adjustType)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const adjustType_casted = adjustType as (audio_VolumeAdjustType)
                this.adjustSystemVolumeByStep1_serialize(volumeType_casted, adjustType_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const adjustType_casted = adjustType as (audio_VolumeAdjustType)
                const callback__casted = callback_ as ((() => void))
                this.adjustSystemVolumeByStep0_serialize(volumeType_casted, adjustType_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public getSystemVolumeInDb(volumeType: audio_AudioVolumeType, volumeLevel: number, device: audio_DeviceType, callback_?: ((result: number) => void)): number | void {
            const volumeType_type = runtimeType(volumeType)
            const volumeLevel_type = runtimeType(volumeLevel)
            const device_type = runtimeType(device)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const volumeLevel_casted = volumeLevel as (number)
                const device_casted = device as (audio_DeviceType)
                return this.getSystemVolumeInDb1_serialize(volumeType_casted, volumeLevel_casted, device_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volumeType_casted = volumeType as (audio_AudioVolumeType)
                const volumeLevel_casted = volumeLevel as (number)
                const device_casted = device as (audio_DeviceType)
                const callback__casted = callback_ as (((result: number) => void))
                return this.getSystemVolumeInDb0_serialize(volumeType_casted, volumeLevel_casted, device_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getSystemVolumeInDbSync(volumeType: audio_AudioVolumeType, volumeLevel: number, device: audio_DeviceType): number {
            const volumeType_casted = volumeType as (audio_AudioVolumeType)
            const volumeLevel_casted = volumeLevel as (number)
            const device_casted = device as (audio_DeviceType)
            return this.getSystemVolumeInDbSync_serialize(volumeType_casted, volumeLevel_casted, device_casted)
        }
        public getMaxAmplitudeForInputDevice(inputDevice: audio.AudioDeviceDescriptor): number {
            const inputDevice_casted = inputDevice as (audio.AudioDeviceDescriptor)
            return this.getMaxAmplitudeForInputDevice_serialize(inputDevice_casted)
        }
        public getMaxAmplitudeForOutputDevice(outputDevice: audio.AudioDeviceDescriptor): number {
            const outputDevice_casted = outputDevice as (audio.AudioDeviceDescriptor)
            return this.getMaxAmplitudeForOutputDevice_serialize(outputDevice_casted)
        }
        on(type: string, callback_: ((parameter: audio_AudioRingMode) => void)): void {
            throw new Error("TBD")
        }
        off(type: string, callback_: ((parameter: audio.MicStateChangeEvent) => void)): void {
            throw new Error("TBD")
        }
        private setVolume0_serialize(volumeType: audio_AudioVolumeType, volume: number, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeGroupManager_setVolume0(this.peer!.ptr, (volumeType.valueOf() as int32), volume, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setVolume1_serialize(volumeType: audio_AudioVolumeType, volume: number): void {
            AUDIONativeModule._AudioVolumeGroupManager_setVolume1(this.peer!.ptr, (volumeType.valueOf() as int32), volume)
        }
        private setVolumeWithFlag_serialize(volumeType: audio_AudioVolumeType, volume: number, flags: number): void {
            AUDIONativeModule._AudioVolumeGroupManager_setVolumeWithFlag(this.peer!.ptr, (volumeType.valueOf() as int32), volume, flags)
        }
        private getActiveVolumeTypeSync_serialize(uid: number): audio_AudioVolumeType {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_getActiveVolumeTypeSync(this.peer!.ptr, uid)
            throw new Error("Object deserialization is not implemented.")
        }
        private getVolume0_serialize(volumeType: audio_AudioVolumeType, callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeGroupManager_getVolume0(this.peer!.ptr, (volumeType.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getVolume1_serialize(volumeType: audio_AudioVolumeType): number {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_getVolume1(this.peer!.ptr, (volumeType.valueOf() as int32))
            return retval
        }
        private getVolumeSync_serialize(volumeType: audio_AudioVolumeType): number {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_getVolumeSync(this.peer!.ptr, (volumeType.valueOf() as int32))
            return retval
        }
        private getMinVolume0_serialize(volumeType: audio_AudioVolumeType, callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeGroupManager_getMinVolume0(this.peer!.ptr, (volumeType.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getMinVolume1_serialize(volumeType: audio_AudioVolumeType): number {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_getMinVolume1(this.peer!.ptr, (volumeType.valueOf() as int32))
            return retval
        }
        private getMinVolumeSync_serialize(volumeType: audio_AudioVolumeType): number {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_getMinVolumeSync(this.peer!.ptr, (volumeType.valueOf() as int32))
            return retval
        }
        private getMaxVolume0_serialize(volumeType: audio_AudioVolumeType, callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeGroupManager_getMaxVolume0(this.peer!.ptr, (volumeType.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getMaxVolume1_serialize(volumeType: audio_AudioVolumeType): number {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_getMaxVolume1(this.peer!.ptr, (volumeType.valueOf() as int32))
            return retval
        }
        private getMaxVolumeSync_serialize(volumeType: audio_AudioVolumeType): number {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_getMaxVolumeSync(this.peer!.ptr, (volumeType.valueOf() as int32))
            return retval
        }
        private mute0_serialize(volumeType: audio_AudioVolumeType, mute: boolean, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeGroupManager_mute0(this.peer!.ptr, (volumeType.valueOf() as int32), mute ? 1 : 0, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private mute1_serialize(volumeType: audio_AudioVolumeType, mute: boolean): void {
            AUDIONativeModule._AudioVolumeGroupManager_mute1(this.peer!.ptr, (volumeType.valueOf() as int32), mute ? 1 : 0)
        }
        private isMute0_serialize(volumeType: audio_AudioVolumeType, callback_: ((result: boolean) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeGroupManager_isMute0(this.peer!.ptr, (volumeType.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private isMute1_serialize(volumeType: audio_AudioVolumeType): boolean {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_isMute1(this.peer!.ptr, (volumeType.valueOf() as int32))
            return retval
        }
        private isMuteSync_serialize(volumeType: audio_AudioVolumeType): boolean {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_isMuteSync(this.peer!.ptr, (volumeType.valueOf() as int32))
            return retval
        }
        private setRingerMode0_serialize(mode: audio_AudioRingMode, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeGroupManager_setRingerMode0(this.peer!.ptr, (mode.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setRingerMode1_serialize(mode: audio_AudioRingMode): void {
            AUDIONativeModule._AudioVolumeGroupManager_setRingerMode1(this.peer!.ptr, (mode.valueOf() as int32))
        }
        private getRingerMode0_serialize(callback_: ((result: audio_AudioRingMode) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeGroupManager_getRingerMode0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getRingerMode1_serialize(): audio_AudioRingMode {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_getRingerMode1(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getRingerModeSync_serialize(): audio_AudioRingMode {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_getRingerModeSync(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private onRingerModeChange_serialize(callback_: ((parameter: audio_AudioRingMode) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeGroupManager_onRingerModeChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setMicrophoneMute0_serialize(mute: boolean, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeGroupManager_setMicrophoneMute0(this.peer!.ptr, mute ? 1 : 0, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setMicrophoneMute1_serialize(mute: boolean): void {
            AUDIONativeModule._AudioVolumeGroupManager_setMicrophoneMute1(this.peer!.ptr, mute ? 1 : 0)
        }
        private setMicMute_serialize(mute: boolean): void {
            AUDIONativeModule._AudioVolumeGroupManager_setMicMute(this.peer!.ptr, mute ? 1 : 0)
        }
        private setMicMutePersistent_serialize(mute: boolean, type: audio_PolicyType): void {
            AUDIONativeModule._AudioVolumeGroupManager_setMicMutePersistent(this.peer!.ptr, mute ? 1 : 0, (type.valueOf() as int32))
        }
        private isPersistentMicMute_serialize(): boolean {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_isPersistentMicMute(this.peer!.ptr)
            return retval
        }
        private isMicrophoneMute0_serialize(callback_: ((result: boolean) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeGroupManager_isMicrophoneMute0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private isMicrophoneMute1_serialize(): boolean {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_isMicrophoneMute1(this.peer!.ptr)
            return retval
        }
        private isMicrophoneMuteSync_serialize(): boolean {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_isMicrophoneMuteSync(this.peer!.ptr)
            return retval
        }
        private onMicStateChange_serialize(callback_: ((parameter: audio.MicStateChangeEvent) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeGroupManager_onMicStateChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offMicStateChange_serialize(callback_?: ((parameter: audio.MicStateChangeEvent) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioVolumeGroupManager_offMicStateChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private isVolumeUnadjustable_serialize(): boolean {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_isVolumeUnadjustable(this.peer!.ptr)
            return retval
        }
        private adjustVolumeByStep0_serialize(adjustType: audio_VolumeAdjustType, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeGroupManager_adjustVolumeByStep0(this.peer!.ptr, (adjustType.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private adjustVolumeByStep1_serialize(adjustType: audio_VolumeAdjustType): void {
            AUDIONativeModule._AudioVolumeGroupManager_adjustVolumeByStep1(this.peer!.ptr, (adjustType.valueOf() as int32))
        }
        private adjustSystemVolumeByStep0_serialize(volumeType: audio_AudioVolumeType, adjustType: audio_VolumeAdjustType, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeGroupManager_adjustSystemVolumeByStep0(this.peer!.ptr, (volumeType.valueOf() as int32), (adjustType.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private adjustSystemVolumeByStep1_serialize(volumeType: audio_AudioVolumeType, adjustType: audio_VolumeAdjustType): void {
            AUDIONativeModule._AudioVolumeGroupManager_adjustSystemVolumeByStep1(this.peer!.ptr, (volumeType.valueOf() as int32), (adjustType.valueOf() as int32))
        }
        private getSystemVolumeInDb0_serialize(volumeType: audio_AudioVolumeType, volumeLevel: number, device: audio_DeviceType, callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioVolumeGroupManager_getSystemVolumeInDb0(this.peer!.ptr, (volumeType.valueOf() as int32), volumeLevel, (device.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getSystemVolumeInDb1_serialize(volumeType: audio_AudioVolumeType, volumeLevel: number, device: audio_DeviceType): number {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_getSystemVolumeInDb1(this.peer!.ptr, (volumeType.valueOf() as int32), volumeLevel, (device.valueOf() as int32))
            return retval
        }
        private getSystemVolumeInDbSync_serialize(volumeType: audio_AudioVolumeType, volumeLevel: number, device: audio_DeviceType): number {
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_getSystemVolumeInDbSync(this.peer!.ptr, (volumeType.valueOf() as int32), volumeLevel, (device.valueOf() as int32))
            return retval
        }
        private getMaxAmplitudeForInputDevice_serialize(inputDevice: audio.AudioDeviceDescriptor): number {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioDeviceDescriptor(inputDevice)
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_getMaxAmplitudeForInputDevice(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
            return retval
        }
        private getMaxAmplitudeForOutputDevice_serialize(outputDevice: audio.AudioDeviceDescriptor): number {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioDeviceDescriptor(outputDevice)
            const retval  = AUDIONativeModule._AudioVolumeGroupManager_getMaxAmplitudeForOutputDevice(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
            return retval
        }
        public static fromPtr(ptr: KPointer): AudioVolumeGroupManagerInternal {
            const obj : AudioVolumeGroupManagerInternal = new AudioVolumeGroupManagerInternal()
            obj.peer = new Finalizable(ptr, AudioVolumeGroupManagerInternal.getFinalizer())
            return obj
        }
    }
}
export namespace audio {
    export class AudioSpatializationManagerInternal implements MaterializedBase,AudioSpatializationManager {
        peer?: Finalizable | undefined
        public getPeer(): Finalizable | undefined {
            return this.peer
        }
        static ctor(): KPointer {
            const retval  = AUDIONativeModule._AudioSpatializationManager_ctor()
            return retval
        }
         constructor() {
            const ctorPtr : KPointer = AudioSpatializationManagerInternal.ctor()
            this.peer = new Finalizable(ctorPtr, AudioSpatializationManagerInternal.getFinalizer())
        }
        static getFinalizer(): KPointer {
            return AUDIONativeModule._AudioSpatializationManager_getFinalizer()
        }
        public isSpatializationSupported(): boolean {
            return this.isSpatializationSupported_serialize()
        }
        public isSpatializationSupportedForDevice(deviceDescriptor: audio.AudioDeviceDescriptor): boolean {
            const deviceDescriptor_casted = deviceDescriptor as (audio.AudioDeviceDescriptor)
            return this.isSpatializationSupportedForDevice_serialize(deviceDescriptor_casted)
        }
        public isHeadTrackingSupported(): boolean {
            return this.isHeadTrackingSupported_serialize()
        }
        public isHeadTrackingSupportedForDevice(deviceDescriptor: audio.AudioDeviceDescriptor): boolean {
            const deviceDescriptor_casted = deviceDescriptor as (audio.AudioDeviceDescriptor)
            return this.isHeadTrackingSupportedForDevice_serialize(deviceDescriptor_casted)
        }
        public setSpatializationEnabled(enable: boolean | audio.AudioDeviceDescriptor, callback_?: (() => void) | boolean): void {
            const enable_type = runtimeType(enable)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.BOOLEAN == enable_type))) && (((RuntimeType.UNDEFINED == callback__type)))) {
                const enable_casted = enable as (boolean)
                this.setSpatializationEnabled1_serialize(enable_casted)
                return
            }
            if ((((RuntimeType.BOOLEAN == enable_type))) && (((RuntimeType.FUNCTION == callback__type)))) {
                const enable_casted = enable as (boolean)
                const callback__casted = callback_ as ((() => void))
                this.setSpatializationEnabled0_serialize(enable_casted, callback__casted)
                return
            }
            if ((((RuntimeType.OBJECT == enable_type))) && (((RuntimeType.BOOLEAN == callback__type)))) {
                const deviceDescriptor_casted = enable as (audio.AudioDeviceDescriptor)
                const enabled_casted = callback_ as (boolean)
                this.setSpatializationEnabled2_serialize(deviceDescriptor_casted, enabled_casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public isSpatializationEnabled(deviceDescriptor?: audio.AudioDeviceDescriptor): boolean {
            const deviceDescriptor_type = runtimeType(deviceDescriptor)
            if ((((RuntimeType.UNDEFINED == deviceDescriptor_type)))) {
                return this.isSpatializationEnabled0_serialize()
            }
            if ((((RuntimeType.OBJECT == deviceDescriptor_type)))) {
                const deviceDescriptor_casted = deviceDescriptor as (audio.AudioDeviceDescriptor)
                return this.isSpatializationEnabled1_serialize(deviceDescriptor_casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public onSpatializationEnabledChange(callback_: ((parameter: boolean) => void)): void {
            const callback__casted = callback_ as (((parameter: boolean) => void))
            this.onSpatializationEnabledChange_serialize(callback__casted)
            return
        }
        public onSpatializationEnabledChangeForAnyDevice(callback_: ((parameter: audio.AudioSpatialEnabledStateForDevice) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioSpatialEnabledStateForDevice) => void))
            this.onSpatializationEnabledChangeForAnyDevice_serialize(callback__casted)
            return
        }
        public offSpatializationEnabledChange(callback_?: ((parameter: boolean) => void)): void {
            const callback__casted = callback_ as (((parameter: boolean) => void) | undefined)
            this.offSpatializationEnabledChange_serialize(callback__casted)
            return
        }
        public offSpatializationEnabledChangeForAnyDevice(callback_?: ((parameter: audio.AudioSpatialEnabledStateForDevice) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioSpatialEnabledStateForDevice) => void) | undefined)
            this.offSpatializationEnabledChangeForAnyDevice_serialize(callback__casted)
            return
        }
        public setHeadTrackingEnabled(enable: boolean | audio.AudioDeviceDescriptor, callback_?: (() => void) | boolean): void {
            const enable_type = runtimeType(enable)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.BOOLEAN == enable_type))) && (((RuntimeType.UNDEFINED == callback__type)))) {
                const enable_casted = enable as (boolean)
                this.setHeadTrackingEnabled1_serialize(enable_casted)
                return
            }
            if ((((RuntimeType.BOOLEAN == enable_type))) && (((RuntimeType.FUNCTION == callback__type)))) {
                const enable_casted = enable as (boolean)
                const callback__casted = callback_ as ((() => void))
                this.setHeadTrackingEnabled0_serialize(enable_casted, callback__casted)
                return
            }
            if ((((RuntimeType.OBJECT == enable_type))) && (((RuntimeType.BOOLEAN == callback__type)))) {
                const deviceDescriptor_casted = enable as (audio.AudioDeviceDescriptor)
                const enabled_casted = callback_ as (boolean)
                this.setHeadTrackingEnabled2_serialize(deviceDescriptor_casted, enabled_casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public isHeadTrackingEnabled(deviceDescriptor?: audio.AudioDeviceDescriptor): boolean {
            const deviceDescriptor_type = runtimeType(deviceDescriptor)
            if ((((RuntimeType.UNDEFINED == deviceDescriptor_type)))) {
                return this.isHeadTrackingEnabled0_serialize()
            }
            if ((((RuntimeType.OBJECT == deviceDescriptor_type)))) {
                const deviceDescriptor_casted = deviceDescriptor as (audio.AudioDeviceDescriptor)
                return this.isHeadTrackingEnabled1_serialize(deviceDescriptor_casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public onHeadTrackingEnabledChange(callback_: ((parameter: boolean) => void)): void {
            const callback__casted = callback_ as (((parameter: boolean) => void))
            this.onHeadTrackingEnabledChange_serialize(callback__casted)
            return
        }
        public onHeadTrackingEnabledChangeForAnyDevice(callback_: ((parameter: audio.AudioSpatialEnabledStateForDevice) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioSpatialEnabledStateForDevice) => void))
            this.onHeadTrackingEnabledChangeForAnyDevice_serialize(callback__casted)
            return
        }
        public offHeadTrackingEnabledChange(callback_?: ((parameter: boolean) => void)): void {
            const callback__casted = callback_ as (((parameter: boolean) => void) | undefined)
            this.offHeadTrackingEnabledChange_serialize(callback__casted)
            return
        }
        public offHeadTrackingEnabledChangeForAnyDevice(callback_?: ((parameter: audio.AudioSpatialEnabledStateForDevice) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioSpatialEnabledStateForDevice) => void) | undefined)
            this.offHeadTrackingEnabledChangeForAnyDevice_serialize(callback__casted)
            return
        }
        public updateSpatialDeviceState(spatialDeviceState: audio.AudioSpatialDeviceState): void {
            const spatialDeviceState_casted = spatialDeviceState as (audio.AudioSpatialDeviceState)
            this.updateSpatialDeviceState_serialize(spatialDeviceState_casted)
            return
        }
        public setSpatializationSceneType(spatializationSceneType: audio_AudioSpatializationSceneType): void {
            const spatializationSceneType_casted = spatializationSceneType as (audio_AudioSpatializationSceneType)
            this.setSpatializationSceneType_serialize(spatializationSceneType_casted)
            return
        }
        public getSpatializationSceneType(): audio_AudioSpatializationSceneType {
            return this.getSpatializationSceneType_serialize()
        }
        on(type: string, callback_: ((parameter: boolean) => void)): void {
            throw new Error("TBD")
        }
        off(type: string, callback_: ((parameter: boolean) => void)): void {
            throw new Error("TBD")
        }
        private isSpatializationSupported_serialize(): boolean {
            const retval  = AUDIONativeModule._AudioSpatializationManager_isSpatializationSupported(this.peer!.ptr)
            return retval
        }
        private isSpatializationSupportedForDevice_serialize(deviceDescriptor: audio.AudioDeviceDescriptor): boolean {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioDeviceDescriptor(deviceDescriptor)
            const retval  = AUDIONativeModule._AudioSpatializationManager_isSpatializationSupportedForDevice(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
            return retval
        }
        private isHeadTrackingSupported_serialize(): boolean {
            const retval  = AUDIONativeModule._AudioSpatializationManager_isHeadTrackingSupported(this.peer!.ptr)
            return retval
        }
        private isHeadTrackingSupportedForDevice_serialize(deviceDescriptor: audio.AudioDeviceDescriptor): boolean {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioDeviceDescriptor(deviceDescriptor)
            const retval  = AUDIONativeModule._AudioSpatializationManager_isHeadTrackingSupportedForDevice(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
            return retval
        }
        private setSpatializationEnabled0_serialize(enable: boolean, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioSpatializationManager_setSpatializationEnabled0(this.peer!.ptr, enable ? 1 : 0, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setSpatializationEnabled1_serialize(enable: boolean): void {
            AUDIONativeModule._AudioSpatializationManager_setSpatializationEnabled1(this.peer!.ptr, enable ? 1 : 0)
        }
        private setSpatializationEnabled2_serialize(deviceDescriptor: audio.AudioDeviceDescriptor, enabled: boolean): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioDeviceDescriptor(deviceDescriptor)
            AUDIONativeModule._AudioSpatializationManager_setSpatializationEnabled2(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length(), enabled ? 1 : 0)
            thisSerializer.release()
        }
        private isSpatializationEnabled0_serialize(): boolean {
            const retval  = AUDIONativeModule._AudioSpatializationManager_isSpatializationEnabled0(this.peer!.ptr)
            return retval
        }
        private isSpatializationEnabled1_serialize(deviceDescriptor: audio.AudioDeviceDescriptor): boolean {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioDeviceDescriptor(deviceDescriptor)
            const retval  = AUDIONativeModule._AudioSpatializationManager_isSpatializationEnabled1(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
            return retval
        }
        private onSpatializationEnabledChange_serialize(callback_: ((parameter: boolean) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioSpatializationManager_onSpatializationEnabledChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private onSpatializationEnabledChangeForAnyDevice_serialize(callback_: ((parameter: audio.AudioSpatialEnabledStateForDevice) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioSpatializationManager_onSpatializationEnabledChangeForAnyDevice(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offSpatializationEnabledChange_serialize(callback_?: ((parameter: boolean) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioSpatializationManager_offSpatializationEnabledChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offSpatializationEnabledChangeForAnyDevice_serialize(callback_?: ((parameter: audio.AudioSpatialEnabledStateForDevice) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioSpatializationManager_offSpatializationEnabledChangeForAnyDevice(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setHeadTrackingEnabled0_serialize(enable: boolean, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioSpatializationManager_setHeadTrackingEnabled0(this.peer!.ptr, enable ? 1 : 0, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setHeadTrackingEnabled1_serialize(enable: boolean): void {
            AUDIONativeModule._AudioSpatializationManager_setHeadTrackingEnabled1(this.peer!.ptr, enable ? 1 : 0)
        }
        private setHeadTrackingEnabled2_serialize(deviceDescriptor: audio.AudioDeviceDescriptor, enabled: boolean): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioDeviceDescriptor(deviceDescriptor)
            AUDIONativeModule._AudioSpatializationManager_setHeadTrackingEnabled2(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length(), enabled ? 1 : 0)
            thisSerializer.release()
        }
        private isHeadTrackingEnabled0_serialize(): boolean {
            const retval  = AUDIONativeModule._AudioSpatializationManager_isHeadTrackingEnabled0(this.peer!.ptr)
            return retval
        }
        private isHeadTrackingEnabled1_serialize(deviceDescriptor: audio.AudioDeviceDescriptor): boolean {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioDeviceDescriptor(deviceDescriptor)
            const retval  = AUDIONativeModule._AudioSpatializationManager_isHeadTrackingEnabled1(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
            return retval
        }
        private onHeadTrackingEnabledChange_serialize(callback_: ((parameter: boolean) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioSpatializationManager_onHeadTrackingEnabledChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private onHeadTrackingEnabledChangeForAnyDevice_serialize(callback_: ((parameter: audio.AudioSpatialEnabledStateForDevice) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioSpatializationManager_onHeadTrackingEnabledChangeForAnyDevice(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offHeadTrackingEnabledChange_serialize(callback_?: ((parameter: boolean) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioSpatializationManager_offHeadTrackingEnabledChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offHeadTrackingEnabledChangeForAnyDevice_serialize(callback_?: ((parameter: audio.AudioSpatialEnabledStateForDevice) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioSpatializationManager_offHeadTrackingEnabledChangeForAnyDevice(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private updateSpatialDeviceState_serialize(spatialDeviceState: audio.AudioSpatialDeviceState): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeAudioSpatialDeviceState(spatialDeviceState)
            AUDIONativeModule._AudioSpatializationManager_updateSpatialDeviceState(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setSpatializationSceneType_serialize(spatializationSceneType: audio_AudioSpatializationSceneType): void {
            AUDIONativeModule._AudioSpatializationManager_setSpatializationSceneType(this.peer!.ptr, (spatializationSceneType.valueOf() as int32))
        }
        private getSpatializationSceneType_serialize(): audio_AudioSpatializationSceneType {
            const retval  = AUDIONativeModule._AudioSpatializationManager_getSpatializationSceneType(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        public static fromPtr(ptr: KPointer): AudioSpatializationManagerInternal {
            const obj : AudioSpatializationManagerInternal = new AudioSpatializationManagerInternal()
            obj.peer = new Finalizable(ptr, AudioSpatializationManagerInternal.getFinalizer())
            return obj
        }
    }
}
export namespace audio {
    export class AudioRendererInternal implements MaterializedBase,AudioRenderer {
        peer?: Finalizable | undefined
        public getPeer(): Finalizable | undefined {
            return this.peer
        }
        get state(): audio_AudioState {
            return this.getState()
        }
        static ctor(): KPointer {
            const retval  = AUDIONativeModule._AudioRenderer_ctor()
            return retval
        }
         constructor() {
            const ctorPtr : KPointer = AudioRendererInternal.ctor()
            this.peer = new Finalizable(ctorPtr, AudioRendererInternal.getFinalizer())
        }
        static getFinalizer(): KPointer {
            return AUDIONativeModule._AudioRenderer_getFinalizer()
        }
        public getRendererInfo(callback_?: ((result: audio.AudioRendererInfo) => void)): audio.AudioRendererInfo | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getRendererInfo1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: audio.AudioRendererInfo) => void))
                return this.getRendererInfo0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getRendererInfoSync(): audio.AudioRendererInfo {
            return this.getRendererInfoSync_serialize()
        }
        public getStreamInfo(callback_?: ((result: audio.AudioStreamInfo) => void)): audio.AudioStreamInfo | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getStreamInfo1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: audio.AudioStreamInfo) => void))
                return this.getStreamInfo0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getStreamInfoSync(): audio.AudioStreamInfo {
            return this.getStreamInfoSync_serialize()
        }
        public getAudioStreamId(callback_?: ((result: number) => void)): number | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getAudioStreamId1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: number) => void))
                return this.getAudioStreamId0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getAudioStreamIdSync(): number {
            return this.getAudioStreamIdSync_serialize()
        }
        public getAudioEffectMode(callback_?: ((result: audio_AudioEffectMode) => void)): audio_AudioEffectMode | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getAudioEffectMode1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: audio_AudioEffectMode) => void))
                return this.getAudioEffectMode0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public setAudioEffectMode(mode: audio_AudioEffectMode, callback_?: (() => void)): void {
            const mode_type = runtimeType(mode)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const mode_casted = mode as (audio_AudioEffectMode)
                this.setAudioEffectMode1_serialize(mode_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const mode_casted = mode as (audio_AudioEffectMode)
                const callback__casted = callback_ as ((() => void))
                this.setAudioEffectMode0_serialize(mode_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public start(callback_?: (() => void)): void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                this.start1_serialize()
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as ((() => void))
                this.start0_serialize(callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public write(buffer: NativeBuffer, callback_?: ((result: number) => void)): number | void {
            const buffer_type = runtimeType(buffer)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const buffer_casted = buffer as (NativeBuffer)
                return this.write1_serialize(buffer_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const buffer_casted = buffer as (NativeBuffer)
                const callback__casted = callback_ as (((result: number) => void))
                return this.write0_serialize(buffer_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getAudioTime(callback_?: ((result: number) => void)): number | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getAudioTime1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: number) => void))
                return this.getAudioTime0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getAudioTimeSync(): number {
            return this.getAudioTimeSync_serialize()
        }
        public drain(callback_?: (() => void)): void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                this.drain1_serialize()
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as ((() => void))
                this.drain0_serialize(callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public flush(): void {
            this.flush_serialize()
            return
        }
        public pause(callback_?: (() => void)): void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                this.pause1_serialize()
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as ((() => void))
                this.pause0_serialize(callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public stop(callback_?: (() => void)): void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                this.stop1_serialize()
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as ((() => void))
                this.stop0_serialize(callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public release(callback_?: (() => void)): void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                this.release1_serialize()
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as ((() => void))
                this.release0_serialize(callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public getBufferSize(callback_?: ((result: number) => void)): number | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getBufferSize1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: number) => void))
                return this.getBufferSize0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getBufferSizeSync(): number {
            return this.getBufferSizeSync_serialize()
        }
        public setRenderRate(rate: audio_AudioRendererRate, callback_?: (() => void)): void {
            const rate_type = runtimeType(rate)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const rate_casted = rate as (audio_AudioRendererRate)
                this.setRenderRate1_serialize(rate_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const rate_casted = rate as (audio_AudioRendererRate)
                const callback__casted = callback_ as ((() => void))
                this.setRenderRate0_serialize(rate_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public setSpeed(speed: number): void {
            const speed_casted = speed as (number)
            this.setSpeed_serialize(speed_casted)
            return
        }
        public getRenderRate(callback_?: ((result: audio_AudioRendererRate) => void)): audio_AudioRendererRate | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getRenderRate1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: audio_AudioRendererRate) => void))
                return this.getRenderRate0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getRenderRateSync(): audio_AudioRendererRate {
            return this.getRenderRateSync_serialize()
        }
        public getSpeed(): number {
            return this.getSpeed_serialize()
        }
        public setInterruptMode(mode: audio_InterruptMode, callback_?: (() => void)): void {
            const mode_type = runtimeType(mode)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const mode_casted = mode as (audio_InterruptMode)
                this.setInterruptMode1_serialize(mode_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const mode_casted = mode as (audio_InterruptMode)
                const callback__casted = callback_ as ((() => void))
                this.setInterruptMode0_serialize(mode_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public setInterruptModeSync(mode: audio_InterruptMode): void {
            const mode_casted = mode as (audio_InterruptMode)
            this.setInterruptModeSync_serialize(mode_casted)
            return
        }
        public setVolume(volume: number, callback_?: (() => void)): void {
            const volume_type = runtimeType(volume)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const volume_casted = volume as (number)
                this.setVolume1_serialize(volume_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const volume_casted = volume as (number)
                const callback__casted = callback_ as ((() => void))
                this.setVolume0_serialize(volume_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public getVolume(): number {
            return this.getVolume_serialize()
        }
        public setVolumeWithRamp(volume: number, duration: number): void {
            const volume_casted = volume as (number)
            const duration_casted = duration as (number)
            this.setVolumeWithRamp_serialize(volume_casted, duration_casted)
            return
        }
        public getMinStreamVolume(callback_?: ((result: number) => void)): number | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getMinStreamVolume1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: number) => void))
                return this.getMinStreamVolume0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getMinStreamVolumeSync(): number {
            return this.getMinStreamVolumeSync_serialize()
        }
        public getMaxStreamVolume(callback_?: ((result: number) => void)): number | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getMaxStreamVolume1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: number) => void))
                return this.getMaxStreamVolume0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getMaxStreamVolumeSync(): number {
            return this.getMaxStreamVolumeSync_serialize()
        }
        public getUnderflowCount(callback_?: ((result: number) => void)): number | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getUnderflowCount1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: number) => void))
                return this.getUnderflowCount0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getUnderflowCountSync(): number {
            return this.getUnderflowCountSync_serialize()
        }
        public getCurrentOutputDevices(callback_?: ((result: audio.AudioDeviceDescriptors) => void)): audio.AudioDeviceDescriptors | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getCurrentOutputDevices1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: audio.AudioDeviceDescriptors) => void))
                return this.getCurrentOutputDevices0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getCurrentOutputDevicesSync(): audio.AudioDeviceDescriptors {
            return this.getCurrentOutputDevicesSync_serialize()
        }
        public setChannelBlendMode(mode: audio_ChannelBlendMode): void {
            const mode_casted = mode as (audio_ChannelBlendMode)
            this.setChannelBlendMode_serialize(mode_casted)
            return
        }
        public setSilentModeAndMixWithOthers(on: boolean): void {
            const on_casted = on as (boolean)
            this.setSilentModeAndMixWithOthers_serialize(on_casted)
            return
        }
        public getSilentModeAndMixWithOthers(): boolean {
            return this.getSilentModeAndMixWithOthers_serialize()
        }
        public setDefaultOutputDevice(deviceType: audio_DeviceType): void {
            const deviceType_casted = deviceType as (audio_DeviceType)
            this.setDefaultOutputDevice_serialize(deviceType_casted)
            return
        }
        public onAudioInterrupt(callback_: ((parameter: audio.InterruptEvent) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.InterruptEvent) => void))
            this.onAudioInterrupt_serialize(callback__casted)
            return
        }
        public onMarkReach(frame: number, callback_: ((parameter: number) => void)): void {
            const frame_casted = frame as (number)
            const callback__casted = callback_ as (((parameter: number) => void))
            this.onMarkReach_serialize(frame_casted, callback__casted)
            return
        }
        public offMarkReach(): void {
            this.offMarkReach_serialize()
            return
        }
        public onPeriodReach(frame: number, callback_: ((parameter: number) => void)): void {
            const frame_casted = frame as (number)
            const callback__casted = callback_ as (((parameter: number) => void))
            this.onPeriodReach_serialize(frame_casted, callback__casted)
            return
        }
        public offPeriodReach(): void {
            this.offPeriodReach_serialize()
            return
        }
        public onStateChange(callback_: ((parameter: audio_AudioState) => void)): void {
            const callback__casted = callback_ as (((parameter: audio_AudioState) => void))
            this.onStateChange_serialize(callback__casted)
            return
        }
        public onOutputDeviceChange(callback_: ((parameter: audio.AudioDeviceDescriptors) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioDeviceDescriptors) => void))
            this.onOutputDeviceChange_serialize(callback__casted)
            return
        }
        public onOutputDeviceChangeWithInfo(callback_: ((parameter: audio.AudioStreamDeviceChangeInfo) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioStreamDeviceChangeInfo) => void))
            this.onOutputDeviceChangeWithInfo_serialize(callback__casted)
            return
        }
        public offOutputDeviceChange(callback_?: ((parameter: audio.AudioDeviceDescriptors) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioDeviceDescriptors) => void) | undefined)
            this.offOutputDeviceChange_serialize(callback__casted)
            return
        }
        public offOutputDeviceChangeWithInfo(callback_?: ((parameter: audio.AudioStreamDeviceChangeInfo) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioStreamDeviceChangeInfo) => void) | undefined)
            this.offOutputDeviceChangeWithInfo_serialize(callback__casted)
            return
        }
        public onWriteData(callback_: audio.AudioRendererWriteDataCallback): void {
            const callback__casted = callback_ as (audio.AudioRendererWriteDataCallback)
            this.onWriteData_serialize(callback__casted)
            return
        }
        public offWriteData(callback_?: audio.AudioRendererWriteDataCallback): void {
            const callback__casted = callback_ as (audio.AudioRendererWriteDataCallback | undefined)
            this.offWriteData_serialize(callback__casted)
            return
        }
        private getState(): audio_AudioState {
            return this.getState_serialize()
        }
        on(type: string, callback_: ((parameter: audio.InterruptEvent) => void)): void {
            throw new Error("TBD")
        }
        off(type: string): void {
            throw new Error("TBD")
        }
        private getRendererInfo0_serialize(callback_: ((result: audio.AudioRendererInfo) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_getRendererInfo0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getRendererInfo1_serialize(): audio.AudioRendererInfo {
            const retval  = AUDIONativeModule._AudioRenderer_getRendererInfo1(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getRendererInfoSync_serialize(): audio.AudioRendererInfo {
            const retval  = AUDIONativeModule._AudioRenderer_getRendererInfoSync(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getStreamInfo0_serialize(callback_: ((result: audio.AudioStreamInfo) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_getStreamInfo0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getStreamInfo1_serialize(): audio.AudioStreamInfo {
            const retval  = AUDIONativeModule._AudioRenderer_getStreamInfo1(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getStreamInfoSync_serialize(): audio.AudioStreamInfo {
            const retval  = AUDIONativeModule._AudioRenderer_getStreamInfoSync(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getAudioStreamId0_serialize(callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_getAudioStreamId0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getAudioStreamId1_serialize(): number {
            const retval  = AUDIONativeModule._AudioRenderer_getAudioStreamId1(this.peer!.ptr)
            return retval
        }
        private getAudioStreamIdSync_serialize(): number {
            const retval  = AUDIONativeModule._AudioRenderer_getAudioStreamIdSync(this.peer!.ptr)
            return retval
        }
        private getAudioEffectMode0_serialize(callback_: ((result: audio_AudioEffectMode) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_getAudioEffectMode0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getAudioEffectMode1_serialize(): audio_AudioEffectMode {
            const retval  = AUDIONativeModule._AudioRenderer_getAudioEffectMode1(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private setAudioEffectMode0_serialize(mode: audio_AudioEffectMode, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_setAudioEffectMode0(this.peer!.ptr, (mode.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setAudioEffectMode1_serialize(mode: audio_AudioEffectMode): void {
            AUDIONativeModule._AudioRenderer_setAudioEffectMode1(this.peer!.ptr, (mode.valueOf() as int32))
        }
        private start0_serialize(callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_start0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private start1_serialize(): void {
            AUDIONativeModule._AudioRenderer_start1(this.peer!.ptr)
        }
        private write0_serialize(buffer: NativeBuffer, callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeBuffer(buffer)
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_write0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private write1_serialize(buffer: NativeBuffer): number {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.writeBuffer(buffer)
            const retval  = AUDIONativeModule._AudioRenderer_write1(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
            return retval
        }
        private getAudioTime0_serialize(callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_getAudioTime0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getAudioTime1_serialize(): number {
            const retval  = AUDIONativeModule._AudioRenderer_getAudioTime1(this.peer!.ptr)
            return retval
        }
        private getAudioTimeSync_serialize(): number {
            const retval  = AUDIONativeModule._AudioRenderer_getAudioTimeSync(this.peer!.ptr)
            return retval
        }
        private drain0_serialize(callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_drain0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private drain1_serialize(): void {
            AUDIONativeModule._AudioRenderer_drain1(this.peer!.ptr)
        }
        private flush_serialize(): void {
            AUDIONativeModule._AudioRenderer_flush(this.peer!.ptr)
        }
        private pause0_serialize(callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_pause0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private pause1_serialize(): void {
            AUDIONativeModule._AudioRenderer_pause1(this.peer!.ptr)
        }
        private stop0_serialize(callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_stop0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private stop1_serialize(): void {
            AUDIONativeModule._AudioRenderer_stop1(this.peer!.ptr)
        }
        private release0_serialize(callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_release0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private release1_serialize(): void {
            AUDIONativeModule._AudioRenderer_release1(this.peer!.ptr)
        }
        private getBufferSize0_serialize(callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_getBufferSize0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getBufferSize1_serialize(): number {
            const retval  = AUDIONativeModule._AudioRenderer_getBufferSize1(this.peer!.ptr)
            return retval
        }
        private getBufferSizeSync_serialize(): number {
            const retval  = AUDIONativeModule._AudioRenderer_getBufferSizeSync(this.peer!.ptr)
            return retval
        }
        private setRenderRate0_serialize(rate: audio_AudioRendererRate, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_setRenderRate0(this.peer!.ptr, (rate.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setRenderRate1_serialize(rate: audio_AudioRendererRate): void {
            AUDIONativeModule._AudioRenderer_setRenderRate1(this.peer!.ptr, (rate.valueOf() as int32))
        }
        private setSpeed_serialize(speed: number): void {
            AUDIONativeModule._AudioRenderer_setSpeed(this.peer!.ptr, speed)
        }
        private getRenderRate0_serialize(callback_: ((result: audio_AudioRendererRate) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_getRenderRate0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getRenderRate1_serialize(): audio_AudioRendererRate {
            const retval  = AUDIONativeModule._AudioRenderer_getRenderRate1(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getRenderRateSync_serialize(): audio_AudioRendererRate {
            const retval  = AUDIONativeModule._AudioRenderer_getRenderRateSync(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getSpeed_serialize(): number {
            const retval  = AUDIONativeModule._AudioRenderer_getSpeed(this.peer!.ptr)
            return retval
        }
        private setInterruptMode0_serialize(mode: audio_InterruptMode, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_setInterruptMode0(this.peer!.ptr, (mode.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setInterruptMode1_serialize(mode: audio_InterruptMode): void {
            AUDIONativeModule._AudioRenderer_setInterruptMode1(this.peer!.ptr, (mode.valueOf() as int32))
        }
        private setInterruptModeSync_serialize(mode: audio_InterruptMode): void {
            AUDIONativeModule._AudioRenderer_setInterruptModeSync(this.peer!.ptr, (mode.valueOf() as int32))
        }
        private setVolume0_serialize(volume: number, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_setVolume0(this.peer!.ptr, volume, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private setVolume1_serialize(volume: number): void {
            AUDIONativeModule._AudioRenderer_setVolume1(this.peer!.ptr, volume)
        }
        private getVolume_serialize(): number {
            const retval  = AUDIONativeModule._AudioRenderer_getVolume(this.peer!.ptr)
            return retval
        }
        private setVolumeWithRamp_serialize(volume: number, duration: number): void {
            AUDIONativeModule._AudioRenderer_setVolumeWithRamp(this.peer!.ptr, volume, duration)
        }
        private getMinStreamVolume0_serialize(callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_getMinStreamVolume0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getMinStreamVolume1_serialize(): number {
            const retval  = AUDIONativeModule._AudioRenderer_getMinStreamVolume1(this.peer!.ptr)
            return retval
        }
        private getMinStreamVolumeSync_serialize(): number {
            const retval  = AUDIONativeModule._AudioRenderer_getMinStreamVolumeSync(this.peer!.ptr)
            return retval
        }
        private getMaxStreamVolume0_serialize(callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_getMaxStreamVolume0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getMaxStreamVolume1_serialize(): number {
            const retval  = AUDIONativeModule._AudioRenderer_getMaxStreamVolume1(this.peer!.ptr)
            return retval
        }
        private getMaxStreamVolumeSync_serialize(): number {
            const retval  = AUDIONativeModule._AudioRenderer_getMaxStreamVolumeSync(this.peer!.ptr)
            return retval
        }
        private getUnderflowCount0_serialize(callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_getUnderflowCount0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getUnderflowCount1_serialize(): number {
            const retval  = AUDIONativeModule._AudioRenderer_getUnderflowCount1(this.peer!.ptr)
            return retval
        }
        private getUnderflowCountSync_serialize(): number {
            const retval  = AUDIONativeModule._AudioRenderer_getUnderflowCountSync(this.peer!.ptr)
            return retval
        }
        private getCurrentOutputDevices0_serialize(callback_: ((result: audio.AudioDeviceDescriptors) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_getCurrentOutputDevices0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getCurrentOutputDevices1_serialize(): audio.AudioDeviceDescriptors {
            const retval  = AUDIONativeModule._AudioRenderer_getCurrentOutputDevices1(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getCurrentOutputDevicesSync_serialize(): audio.AudioDeviceDescriptors {
            const retval  = AUDIONativeModule._AudioRenderer_getCurrentOutputDevicesSync(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private setChannelBlendMode_serialize(mode: audio_ChannelBlendMode): void {
            AUDIONativeModule._AudioRenderer_setChannelBlendMode(this.peer!.ptr, (mode.valueOf() as int32))
        }
        private setSilentModeAndMixWithOthers_serialize(on: boolean): void {
            AUDIONativeModule._AudioRenderer_setSilentModeAndMixWithOthers(this.peer!.ptr, on ? 1 : 0)
        }
        private getSilentModeAndMixWithOthers_serialize(): boolean {
            const retval  = AUDIONativeModule._AudioRenderer_getSilentModeAndMixWithOthers(this.peer!.ptr)
            return retval
        }
        private setDefaultOutputDevice_serialize(deviceType: audio_DeviceType): void {
            AUDIONativeModule._AudioRenderer_setDefaultOutputDevice(this.peer!.ptr, (deviceType.valueOf() as int32))
        }
        private onAudioInterrupt_serialize(callback_: ((parameter: audio.InterruptEvent) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_onAudioInterrupt(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private onMarkReach_serialize(frame: number, callback_: ((parameter: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_onMarkReach(this.peer!.ptr, frame, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offMarkReach_serialize(): void {
            AUDIONativeModule._AudioRenderer_offMarkReach(this.peer!.ptr)
        }
        private onPeriodReach_serialize(frame: number, callback_: ((parameter: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_onPeriodReach(this.peer!.ptr, frame, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offPeriodReach_serialize(): void {
            AUDIONativeModule._AudioRenderer_offPeriodReach(this.peer!.ptr)
        }
        private onStateChange_serialize(callback_: ((parameter: audio_AudioState) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_onStateChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private onOutputDeviceChange_serialize(callback_: ((parameter: audio.AudioDeviceDescriptors) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_onOutputDeviceChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private onOutputDeviceChangeWithInfo_serialize(callback_: ((parameter: audio.AudioStreamDeviceChangeInfo) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_onOutputDeviceChangeWithInfo(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offOutputDeviceChange_serialize(callback_?: ((parameter: audio.AudioDeviceDescriptors) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioRenderer_offOutputDeviceChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offOutputDeviceChangeWithInfo_serialize(callback_?: ((parameter: audio.AudioStreamDeviceChangeInfo) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioRenderer_offOutputDeviceChangeWithInfo(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private onWriteData_serialize(callback_: audio.AudioRendererWriteDataCallback): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioRenderer_onWriteData(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offWriteData_serialize(callback_?: audio.AudioRendererWriteDataCallback): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioRenderer_offWriteData(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getState_serialize(): audio_AudioState {
            const retval  = AUDIONativeModule._AudioRenderer_getState(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        public static fromPtr(ptr: KPointer): AudioRendererInternal {
            const obj : AudioRendererInternal = new AudioRendererInternal()
            obj.peer = new Finalizable(ptr, AudioRendererInternal.getFinalizer())
            return obj
        }
    }
}
export namespace audio {
    export class AudioCapturerInternal implements MaterializedBase,AudioCapturer {
        peer?: Finalizable | undefined
        public getPeer(): Finalizable | undefined {
            return this.peer
        }
        get state(): audio_AudioState {
            return this.getState()
        }
        static ctor(): KPointer {
            const retval  = AUDIONativeModule._AudioCapturer_ctor()
            return retval
        }
         constructor() {
            const ctorPtr : KPointer = AudioCapturerInternal.ctor()
            this.peer = new Finalizable(ctorPtr, AudioCapturerInternal.getFinalizer())
        }
        static getFinalizer(): KPointer {
            return AUDIONativeModule._AudioCapturer_getFinalizer()
        }
        public getCapturerInfo(callback_?: ((result: audio.AudioCapturerInfo) => void)): audio.AudioCapturerInfo | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getCapturerInfo1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: audio.AudioCapturerInfo) => void))
                return this.getCapturerInfo0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getCapturerInfoSync(): audio.AudioCapturerInfo {
            return this.getCapturerInfoSync_serialize()
        }
        public getStreamInfo(callback_?: ((result: audio.AudioStreamInfo) => void)): audio.AudioStreamInfo | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getStreamInfo1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: audio.AudioStreamInfo) => void))
                return this.getStreamInfo0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getStreamInfoSync(): audio.AudioStreamInfo {
            return this.getStreamInfoSync_serialize()
        }
        public getAudioStreamId(callback_?: ((result: number) => void)): number | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getAudioStreamId1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: number) => void))
                return this.getAudioStreamId0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getAudioStreamIdSync(): number {
            return this.getAudioStreamIdSync_serialize()
        }
        public start(callback_?: (() => void)): void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                this.start1_serialize()
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as ((() => void))
                this.start0_serialize(callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public read(size: number, isBlockingRead: boolean, callback_?: ((result: NativeBuffer) => void)): NativeBuffer | void {
            const size_type = runtimeType(size)
            const isBlockingRead_type = runtimeType(isBlockingRead)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const size_casted = size as (number)
                const isBlockingRead_casted = isBlockingRead as (boolean)
                return this.read1_serialize(size_casted, isBlockingRead_casted)
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const size_casted = size as (number)
                const isBlockingRead_casted = isBlockingRead as (boolean)
                const callback__casted = callback_ as (((result: NativeBuffer) => void))
                return this.read0_serialize(size_casted, isBlockingRead_casted, callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getAudioTime(callback_?: ((result: number) => void)): number | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getAudioTime1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: number) => void))
                return this.getAudioTime0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getAudioTimeSync(): number {
            return this.getAudioTimeSync_serialize()
        }
        public stop(callback_?: (() => void)): void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                this.stop1_serialize()
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as ((() => void))
                this.stop0_serialize(callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public release(callback_?: (() => void)): void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                this.release1_serialize()
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as ((() => void))
                this.release0_serialize(callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public getBufferSize(callback_?: ((result: number) => void)): number | void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                return this.getBufferSize1_serialize()
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as (((result: number) => void))
                return this.getBufferSize0_serialize(callback__casted)
            }
            throw new Error("Can not select appropriate overload")
        }
        public getBufferSizeSync(): number {
            return this.getBufferSizeSync_serialize()
        }
        public getCurrentInputDevices(): audio.AudioDeviceDescriptors {
            return this.getCurrentInputDevices_serialize()
        }
        public getCurrentAudioCapturerChangeInfo(): audio.AudioCapturerChangeInfo {
            return this.getCurrentAudioCapturerChangeInfo_serialize()
        }
        public getOverflowCount(): number {
            return this.getOverflowCount_serialize()
        }
        public getOverflowCountSync(): number {
            return this.getOverflowCountSync_serialize()
        }
        public onMarkReach(frame: number, callback_: ((parameter: number) => void)): void {
            const frame_casted = frame as (number)
            const callback__casted = callback_ as (((parameter: number) => void))
            this.onMarkReach_serialize(frame_casted, callback__casted)
            return
        }
        public offMarkReach(): void {
            this.offMarkReach_serialize()
            return
        }
        public onPeriodReach(frame: number, callback_: ((parameter: number) => void)): void {
            const frame_casted = frame as (number)
            const callback__casted = callback_ as (((parameter: number) => void))
            this.onPeriodReach_serialize(frame_casted, callback__casted)
            return
        }
        public offPeriodReach(): void {
            this.offPeriodReach_serialize()
            return
        }
        public onStateChange(callback_: ((parameter: audio_AudioState) => void)): void {
            const callback__casted = callback_ as (((parameter: audio_AudioState) => void))
            this.onStateChange_serialize(callback__casted)
            return
        }
        public onAudioInterrupt(callback_: ((parameter: audio.InterruptEvent) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.InterruptEvent) => void))
            this.onAudioInterrupt_serialize(callback__casted)
            return
        }
        public offAudioInterrupt(): void {
            this.offAudioInterrupt_serialize()
            return
        }
        public onInputDeviceChange(callback_: ((parameter: audio.AudioDeviceDescriptors) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioDeviceDescriptors) => void))
            this.onInputDeviceChange_serialize(callback__casted)
            return
        }
        public offInputDeviceChange(callback_?: ((parameter: audio.AudioDeviceDescriptors) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioDeviceDescriptors) => void) | undefined)
            this.offInputDeviceChange_serialize(callback__casted)
            return
        }
        public onAudioCapturerChange(callback_: ((parameter: audio.AudioCapturerChangeInfo) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioCapturerChangeInfo) => void))
            this.onAudioCapturerChange_serialize(callback__casted)
            return
        }
        public offAudioCapturerChange(callback_?: ((parameter: audio.AudioCapturerChangeInfo) => void)): void {
            const callback__casted = callback_ as (((parameter: audio.AudioCapturerChangeInfo) => void) | undefined)
            this.offAudioCapturerChange_serialize(callback__casted)
            return
        }
        public onReadData(callback_: ((parameter: NativeBuffer) => void)): void {
            const callback__casted = callback_ as (((parameter: NativeBuffer) => void))
            this.onReadData_serialize(callback__casted)
            return
        }
        public offReadData(callback_?: ((parameter: NativeBuffer) => void)): void {
            const callback__casted = callback_ as (((parameter: NativeBuffer) => void) | undefined)
            this.offReadData_serialize(callback__casted)
            return
        }
        private getState(): audio_AudioState {
            return this.getState_serialize()
        }
        on(type: string, frame: number, callback_: ((parameter: number) => void)): void {
            throw new Error("TBD")
        }
        off(type: string): void {
            throw new Error("TBD")
        }
        private getCapturerInfo0_serialize(callback_: ((result: audio.AudioCapturerInfo) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioCapturer_getCapturerInfo0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getCapturerInfo1_serialize(): audio.AudioCapturerInfo {
            const retval  = AUDIONativeModule._AudioCapturer_getCapturerInfo1(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getCapturerInfoSync_serialize(): audio.AudioCapturerInfo {
            const retval  = AUDIONativeModule._AudioCapturer_getCapturerInfoSync(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getStreamInfo0_serialize(callback_: ((result: audio.AudioStreamInfo) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioCapturer_getStreamInfo0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getStreamInfo1_serialize(): audio.AudioStreamInfo {
            const retval  = AUDIONativeModule._AudioCapturer_getStreamInfo1(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getStreamInfoSync_serialize(): audio.AudioStreamInfo {
            const retval  = AUDIONativeModule._AudioCapturer_getStreamInfoSync(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getAudioStreamId0_serialize(callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioCapturer_getAudioStreamId0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getAudioStreamId1_serialize(): number {
            const retval  = AUDIONativeModule._AudioCapturer_getAudioStreamId1(this.peer!.ptr)
            return retval
        }
        private getAudioStreamIdSync_serialize(): number {
            const retval  = AUDIONativeModule._AudioCapturer_getAudioStreamIdSync(this.peer!.ptr)
            return retval
        }
        private start0_serialize(callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioCapturer_start0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private start1_serialize(): void {
            AUDIONativeModule._AudioCapturer_start1(this.peer!.ptr)
        }
        private read0_serialize(size: number, isBlockingRead: boolean, callback_: ((result: NativeBuffer) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioCapturer_read0(this.peer!.ptr, size, isBlockingRead ? 1 : 0, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private read1_serialize(size: number, isBlockingRead: boolean): NativeBuffer {
            const retval  = AUDIONativeModule._AudioCapturer_read1(this.peer!.ptr, size, isBlockingRead ? 1 : 0)
            return retval
        }
        private getAudioTime0_serialize(callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioCapturer_getAudioTime0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getAudioTime1_serialize(): number {
            const retval  = AUDIONativeModule._AudioCapturer_getAudioTime1(this.peer!.ptr)
            return retval
        }
        private getAudioTimeSync_serialize(): number {
            const retval  = AUDIONativeModule._AudioCapturer_getAudioTimeSync(this.peer!.ptr)
            return retval
        }
        private stop0_serialize(callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioCapturer_stop0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private stop1_serialize(): void {
            AUDIONativeModule._AudioCapturer_stop1(this.peer!.ptr)
        }
        private release0_serialize(callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioCapturer_release0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private release1_serialize(): void {
            AUDIONativeModule._AudioCapturer_release1(this.peer!.ptr)
        }
        private getBufferSize0_serialize(callback_: ((result: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioCapturer_getBufferSize0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getBufferSize1_serialize(): number {
            const retval  = AUDIONativeModule._AudioCapturer_getBufferSize1(this.peer!.ptr)
            return retval
        }
        private getBufferSizeSync_serialize(): number {
            const retval  = AUDIONativeModule._AudioCapturer_getBufferSizeSync(this.peer!.ptr)
            return retval
        }
        private getCurrentInputDevices_serialize(): audio.AudioDeviceDescriptors {
            const retval  = AUDIONativeModule._AudioCapturer_getCurrentInputDevices(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getCurrentAudioCapturerChangeInfo_serialize(): audio.AudioCapturerChangeInfo {
            const retval  = AUDIONativeModule._AudioCapturer_getCurrentAudioCapturerChangeInfo(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private getOverflowCount_serialize(): number {
            const retval  = AUDIONativeModule._AudioCapturer_getOverflowCount(this.peer!.ptr)
            return retval
        }
        private getOverflowCountSync_serialize(): number {
            const retval  = AUDIONativeModule._AudioCapturer_getOverflowCountSync(this.peer!.ptr)
            return retval
        }
        private onMarkReach_serialize(frame: number, callback_: ((parameter: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioCapturer_onMarkReach(this.peer!.ptr, frame, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offMarkReach_serialize(): void {
            AUDIONativeModule._AudioCapturer_offMarkReach(this.peer!.ptr)
        }
        private onPeriodReach_serialize(frame: number, callback_: ((parameter: number) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioCapturer_onPeriodReach(this.peer!.ptr, frame, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offPeriodReach_serialize(): void {
            AUDIONativeModule._AudioCapturer_offPeriodReach(this.peer!.ptr)
        }
        private onStateChange_serialize(callback_: ((parameter: audio_AudioState) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioCapturer_onStateChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private onAudioInterrupt_serialize(callback_: ((parameter: audio.InterruptEvent) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioCapturer_onAudioInterrupt(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offAudioInterrupt_serialize(): void {
            AUDIONativeModule._AudioCapturer_offAudioInterrupt(this.peer!.ptr)
        }
        private onInputDeviceChange_serialize(callback_: ((parameter: audio.AudioDeviceDescriptors) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioCapturer_onInputDeviceChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offInputDeviceChange_serialize(callback_?: ((parameter: audio.AudioDeviceDescriptors) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioCapturer_offInputDeviceChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private onAudioCapturerChange_serialize(callback_: ((parameter: audio.AudioCapturerChangeInfo) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioCapturer_onAudioCapturerChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offAudioCapturerChange_serialize(callback_?: ((parameter: audio.AudioCapturerChangeInfo) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioCapturer_offAudioCapturerChange(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private onReadData_serialize(callback_: ((parameter: NativeBuffer) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._AudioCapturer_onReadData(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private offReadData_serialize(callback_?: ((parameter: NativeBuffer) => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            let callback__type : int32 = RuntimeType.UNDEFINED
            callback__type = runtimeType(callback_)
            thisSerializer.writeInt8(callback__type as int32)
            if ((RuntimeType.UNDEFINED) != (callback__type)) {
                const callback__value  = callback_!
                thisSerializer.holdAndWriteCallback(callback__value)
            }
            AUDIONativeModule._AudioCapturer_offReadData(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private getState_serialize(): audio_AudioState {
            const retval  = AUDIONativeModule._AudioCapturer_getState(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        public static fromPtr(ptr: KPointer): AudioCapturerInternal {
            const obj : AudioCapturerInternal = new AudioCapturerInternal()
            obj.peer = new Finalizable(ptr, AudioCapturerInternal.getFinalizer())
            return obj
        }
    }
}
export namespace audio {
    export class AsrProcessingControllerInternal implements MaterializedBase,AsrProcessingController {
        peer?: Finalizable | undefined
        public getPeer(): Finalizable | undefined {
            return this.peer
        }
        static ctor(): KPointer {
            const retval  = AUDIONativeModule._AsrProcessingController_ctor()
            return retval
        }
         constructor() {
            const ctorPtr : KPointer = AsrProcessingControllerInternal.ctor()
            this.peer = new Finalizable(ctorPtr, AsrProcessingControllerInternal.getFinalizer())
        }
        static getFinalizer(): KPointer {
            return AUDIONativeModule._AsrProcessingController_getFinalizer()
        }
        public setAsrAecMode(mode: audio_AsrAecMode): boolean {
            const mode_casted = mode as (audio_AsrAecMode)
            return this.setAsrAecMode_serialize(mode_casted)
        }
        public getAsrAecMode(): audio_AsrAecMode {
            return this.getAsrAecMode_serialize()
        }
        public setAsrNoiseSuppressionMode(mode: audio_AsrNoiseSuppressionMode): boolean {
            const mode_casted = mode as (audio_AsrNoiseSuppressionMode)
            return this.setAsrNoiseSuppressionMode_serialize(mode_casted)
        }
        public getAsrNoiseSuppressionMode(): audio_AsrNoiseSuppressionMode {
            return this.getAsrNoiseSuppressionMode_serialize()
        }
        public isWhispering(): boolean {
            return this.isWhispering_serialize()
        }
        public setAsrVoiceControlMode(mode: audio_AsrVoiceControlMode, enable: boolean): boolean {
            const mode_casted = mode as (audio_AsrVoiceControlMode)
            const enable_casted = enable as (boolean)
            return this.setAsrVoiceControlMode_serialize(mode_casted, enable_casted)
        }
        public setAsrVoiceMuteMode(mode: audio_AsrVoiceMuteMode, enable: boolean): boolean {
            const mode_casted = mode as (audio_AsrVoiceMuteMode)
            const enable_casted = enable as (boolean)
            return this.setAsrVoiceMuteMode_serialize(mode_casted, enable_casted)
        }
        public setAsrWhisperDetectionMode(mode: audio_AsrWhisperDetectionMode): boolean {
            const mode_casted = mode as (audio_AsrWhisperDetectionMode)
            return this.setAsrWhisperDetectionMode_serialize(mode_casted)
        }
        public getAsrWhisperDetectionMode(): audio_AsrWhisperDetectionMode {
            return this.getAsrWhisperDetectionMode_serialize()
        }
        private setAsrAecMode_serialize(mode: audio_AsrAecMode): boolean {
            const retval  = AUDIONativeModule._AsrProcessingController_setAsrAecMode(this.peer!.ptr, (mode.valueOf() as int32))
            return retval
        }
        private getAsrAecMode_serialize(): audio_AsrAecMode {
            const retval  = AUDIONativeModule._AsrProcessingController_getAsrAecMode(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private setAsrNoiseSuppressionMode_serialize(mode: audio_AsrNoiseSuppressionMode): boolean {
            const retval  = AUDIONativeModule._AsrProcessingController_setAsrNoiseSuppressionMode(this.peer!.ptr, (mode.valueOf() as int32))
            return retval
        }
        private getAsrNoiseSuppressionMode_serialize(): audio_AsrNoiseSuppressionMode {
            const retval  = AUDIONativeModule._AsrProcessingController_getAsrNoiseSuppressionMode(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        private isWhispering_serialize(): boolean {
            const retval  = AUDIONativeModule._AsrProcessingController_isWhispering(this.peer!.ptr)
            return retval
        }
        private setAsrVoiceControlMode_serialize(mode: audio_AsrVoiceControlMode, enable: boolean): boolean {
            const retval  = AUDIONativeModule._AsrProcessingController_setAsrVoiceControlMode(this.peer!.ptr, (mode.valueOf() as int32), enable ? 1 : 0)
            return retval
        }
        private setAsrVoiceMuteMode_serialize(mode: audio_AsrVoiceMuteMode, enable: boolean): boolean {
            const retval  = AUDIONativeModule._AsrProcessingController_setAsrVoiceMuteMode(this.peer!.ptr, (mode.valueOf() as int32), enable ? 1 : 0)
            return retval
        }
        private setAsrWhisperDetectionMode_serialize(mode: audio_AsrWhisperDetectionMode): boolean {
            const retval  = AUDIONativeModule._AsrProcessingController_setAsrWhisperDetectionMode(this.peer!.ptr, (mode.valueOf() as int32))
            return retval
        }
        private getAsrWhisperDetectionMode_serialize(): audio_AsrWhisperDetectionMode {
            const retval  = AUDIONativeModule._AsrProcessingController_getAsrWhisperDetectionMode(this.peer!.ptr)
            throw new Error("Object deserialization is not implemented.")
        }
        public static fromPtr(ptr: KPointer): AsrProcessingControllerInternal {
            const obj : AsrProcessingControllerInternal = new AsrProcessingControllerInternal()
            obj.peer = new Finalizable(ptr, AsrProcessingControllerInternal.getFinalizer())
            return obj
        }
    }
}
export namespace audio {
    export class TonePlayerInternal implements MaterializedBase,TonePlayer {
        peer?: Finalizable | undefined
        public getPeer(): Finalizable | undefined {
            return this.peer
        }
        static ctor(): KPointer {
            const retval  = AUDIONativeModule._TonePlayer_ctor()
            return retval
        }
         constructor() {
            const ctorPtr : KPointer = TonePlayerInternal.ctor()
            this.peer = new Finalizable(ctorPtr, TonePlayerInternal.getFinalizer())
        }
        static getFinalizer(): KPointer {
            return AUDIONativeModule._TonePlayer_getFinalizer()
        }
        public load(type: audio_ToneType, callback_?: (() => void)): void {
            const type_type = runtimeType(type)
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                const type_casted = type as (audio_ToneType)
                this.load1_serialize(type_casted)
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const type_casted = type as (audio_ToneType)
                const callback__casted = callback_ as ((() => void))
                this.load0_serialize(type_casted, callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public start(callback_?: (() => void)): void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                this.start1_serialize()
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as ((() => void))
                this.start0_serialize(callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public stop(callback_?: (() => void)): void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                this.stop1_serialize()
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as ((() => void))
                this.stop0_serialize(callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        public release(callback_?: (() => void)): void {
            const callback__type = runtimeType(callback_)
            if ((((RuntimeType.UNDEFINED == callback__type)))) {
                this.release1_serialize()
                return
            }
            if ((((RuntimeType.FUNCTION == callback__type)))) {
                const callback__casted = callback_ as ((() => void))
                this.release0_serialize(callback__casted)
                return
            }
            throw new Error("Can not select appropriate overload")
        }
        private load0_serialize(type: audio_ToneType, callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._TonePlayer_load0(this.peer!.ptr, (type.valueOf() as int32), thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private load1_serialize(type: audio_ToneType): void {
            AUDIONativeModule._TonePlayer_load1(this.peer!.ptr, (type.valueOf() as int32))
        }
        private start0_serialize(callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._TonePlayer_start0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private start1_serialize(): void {
            AUDIONativeModule._TonePlayer_start1(this.peer!.ptr)
        }
        private stop0_serialize(callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._TonePlayer_stop0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private stop1_serialize(): void {
            AUDIONativeModule._TonePlayer_stop1(this.peer!.ptr)
        }
        private release0_serialize(callback_: (() => void)): void {
            const thisSerializer : Serializer = Serializer.hold()
            thisSerializer.holdAndWriteCallback(callback_)
            AUDIONativeModule._TonePlayer_release0(this.peer!.ptr, thisSerializer.asArray(), thisSerializer.length())
            thisSerializer.release()
        }
        private release1_serialize(): void {
            AUDIONativeModule._TonePlayer_release1(this.peer!.ptr)
        }
        public static fromPtr(ptr: KPointer): TonePlayerInternal {
            const obj : TonePlayerInternal = new TonePlayerInternal()
            obj.peer = new Finalizable(ptr, TonePlayerInternal.getFinalizer())
            return obj
        }
    }
}
export enum audio_AudioErrors {
    ERROR_INVALID_PARAM = 6800101,
    ERROR_NO_MEMORY = 6800102,
    ERROR_ILLEGAL_STATE = 6800103,
    ERROR_UNSUPPORTED = 6800104,
    ERROR_TIMEOUT = 6800105,
    ERROR_STREAM_LIMIT = 6800201,
    ERROR_SYSTEM = 6800301
}
export enum audio_AudioState {
    STATE_INVALID = -1,
    STATE_NEW = 0,
    STATE_PREPARED = 1,
    STATE_RUNNING = 2,
    STATE_STOPPED = 3,
    STATE_RELEASED = 4,
    STATE_PAUSED = 5
}
export enum audio_AudioVolumeType {
    VOICE_CALL = 0,
    RINGTONE = 2,
    MEDIA = 3,
    ALARM = 4,
    ACCESSIBILITY = 5,
    VOICE_ASSISTANT = 9,
    ULTRASONIC = 10,
    ALL = 100
}
export enum audio_DeviceFlag {
    NONE_DEVICES_FLAG = 0,
    OUTPUT_DEVICES_FLAG = 1,
    INPUT_DEVICES_FLAG = 2,
    ALL_DEVICES_FLAG = 3,
    DISTRIBUTED_OUTPUT_DEVICES_FLAG = 4,
    DISTRIBUTED_INPUT_DEVICES_FLAG = 8,
    ALL_DISTRIBUTED_DEVICES_FLAG = 12
}
export enum audio_DeviceUsage {
    MEDIA_OUTPUT_DEVICES = 1,
    MEDIA_INPUT_DEVICES = 2,
    ALL_MEDIA_DEVICES = 3,
    CALL_OUTPUT_DEVICES = 4,
    CALL_INPUT_DEVICES = 8,
    ALL_CALL_DEVICES = 12
}
export enum audio_DeviceRole {
    INPUT_DEVICE = 1,
    OUTPUT_DEVICE = 2
}
export enum audio_DeviceType {
    INVALID = 0,
    EARPIECE = 1,
    SPEAKER = 2,
    WIRED_HEADSET = 3,
    WIRED_HEADPHONES = 4,
    BLUETOOTH_SCO = 7,
    BLUETOOTH_A2DP = 8,
    MIC = 15,
    USB_HEADSET = 22,
    DISPLAY_PORT = 23,
    REMOTE_CAST = 24,
    DEFAULT = 1000
}
export enum audio_ActiveDeviceType {
    SPEAKER = 2,
    BLUETOOTH_SCO = 7
}
export enum audio_CommunicationDeviceType {
    SPEAKER = 2
}
export enum audio_AudioRingMode {
    RINGER_MODE_SILENT = 0,
    RINGER_MODE_VIBRATE = 1,
    RINGER_MODE_NORMAL = 2
}
export enum audio_PolicyType {
    EDM = 0,
    PRIVACY = 1
}
export enum audio_AudioSampleFormat {
    SAMPLE_FORMAT_INVALID = -1,
    SAMPLE_FORMAT_U8 = 0,
    SAMPLE_FORMAT_S16LE = 1,
    SAMPLE_FORMAT_S24LE = 2,
    SAMPLE_FORMAT_S32LE = 3,
    SAMPLE_FORMAT_F32LE = 4
}
export enum audio_AudioChannel {
    CHANNEL_1 = 1,
    CHANNEL_2 = 2,
    CHANNEL_3 = 3,
    CHANNEL_4 = 4,
    CHANNEL_5 = 5,
    CHANNEL_6 = 6,
    CHANNEL_7 = 7,
    CHANNEL_8 = 8,
    CHANNEL_9 = 9,
    CHANNEL_10 = 10,
    CHANNEL_12 = 12,
    CHANNEL_14 = 14,
    CHANNEL_16 = 16
}
export enum audio_AudioSamplingRate {
    SAMPLE_RATE_8000 = 8000,
    SAMPLE_RATE_11025 = 11025,
    SAMPLE_RATE_12000 = 12000,
    SAMPLE_RATE_16000 = 16000,
    SAMPLE_RATE_22050 = 22050,
    SAMPLE_RATE_24000 = 24000,
    SAMPLE_RATE_32000 = 32000,
    SAMPLE_RATE_44100 = 44100,
    SAMPLE_RATE_48000 = 48000,
    SAMPLE_RATE_64000 = 64000,
    SAMPLE_RATE_88200 = 88200,
    SAMPLE_RATE_96000 = 96000,
    SAMPLE_RATE_176400 = 176400,
    SAMPLE_RATE_192000 = 192000
}
export enum audio_AudioEncodingType {
    ENCODING_TYPE_INVALID = -1,
    ENCODING_TYPE_RAW = 0
}
export enum audio_ContentType {
    CONTENT_TYPE_UNKNOWN = 0,
    CONTENT_TYPE_SPEECH = 1,
    CONTENT_TYPE_MUSIC = 2,
    CONTENT_TYPE_MOVIE = 3,
    CONTENT_TYPE_SONIFICATION = 4,
    CONTENT_TYPE_RINGTONE = 5
}
export enum audio_StreamUsage {
    STREAM_USAGE_UNKNOWN = 0,
    STREAM_USAGE_MEDIA = 1,
    STREAM_USAGE_MUSIC = 1,
    STREAM_USAGE_VOICE_COMMUNICATION = 2,
    STREAM_USAGE_VOICE_ASSISTANT = 3,
    STREAM_USAGE_ALARM = 4,
    STREAM_USAGE_VOICE_MESSAGE = 5,
    STREAM_USAGE_NOTIFICATION_RINGTONE = 6,
    STREAM_USAGE_RINGTONE = 6,
    STREAM_USAGE_NOTIFICATION = 7,
    STREAM_USAGE_ACCESSIBILITY = 8,
    STREAM_USAGE_SYSTEM = 9,
    STREAM_USAGE_MOVIE = 10,
    STREAM_USAGE_GAME = 11,
    STREAM_USAGE_AUDIOBOOK = 12,
    STREAM_USAGE_NAVIGATION = 13,
    STREAM_USAGE_DTMF = 14,
    STREAM_USAGE_ENFORCED_TONE = 15,
    STREAM_USAGE_ULTRASONIC = 16,
    STREAM_USAGE_VIDEO_COMMUNICATION = 17,
    STREAM_USAGE_VOICE_CALL_ASSISTANT = 21
}
export enum audio_InterruptRequestType {
    INTERRUPT_REQUEST_TYPE_DEFAULT = 0
}
export enum audio_VolumeFlag {
    FLAG_SHOW_SYSTEM_UI = 1
}
export namespace audio {
    export interface AudioStreamInfo {
        samplingRate: audio_AudioSamplingRate
        channels: audio_AudioChannel
        sampleFormat: audio_AudioSampleFormat
        encodingType: audio_AudioEncodingType
        channelLayout?: audio_AudioChannelLayout
    }
}
export namespace audio {
    export interface AudioRendererInfo {
        content?: audio_ContentType
        usage: audio_StreamUsage
        rendererFlags: number
    }
}
export namespace audio {
    export interface AudioRendererFilter {
        uid?: number
        rendererInfo?: audio.AudioRendererInfo
        rendererId?: number
    }
}
export namespace audio {
    export interface AudioCapturerFilter {
        uid?: number
        capturerInfo?: audio.AudioCapturerInfo
    }
}
export namespace audio {
    export interface AudioRendererOptions {
        streamInfo: audio.AudioStreamInfo
        rendererInfo: audio.AudioRendererInfo
        privacyType?: audio_AudioPrivacyType
    }
}
export enum audio_AudioPrivacyType {
    PRIVACY_TYPE_PUBLIC = 0,
    PRIVACY_TYPE_PRIVATE = 1
}
export enum audio_InterruptMode {
    SHARE_MODE = 0,
    INDEPENDENT_MODE = 1
}
export enum audio_AudioRendererRate {
    RENDER_RATE_NORMAL = 0,
    RENDER_RATE_DOUBLE = 1,
    RENDER_RATE_HALF = 2
}
export enum audio_InterruptType {
    INTERRUPT_TYPE_BEGIN = 1,
    INTERRUPT_TYPE_END = 2
}
export enum audio_InterruptHint {
    INTERRUPT_HINT_NONE = 0,
    INTERRUPT_HINT_RESUME = 1,
    INTERRUPT_HINT_PAUSE = 2,
    INTERRUPT_HINT_STOP = 3,
    INTERRUPT_HINT_DUCK = 4,
    INTERRUPT_HINT_UNDUCK = 5
}
export enum audio_InterruptForceType {
    INTERRUPT_FORCE = 0,
    INTERRUPT_SHARE = 1
}
export namespace audio {
    export interface InterruptEvent {
        eventType: audio_InterruptType
        forceType: audio_InterruptForceType
        hintType: audio_InterruptHint
    }
}
export enum audio_InterruptActionType {
    TYPE_ACTIVATED = 0,
    TYPE_INTERRUPT = 1
}
export enum audio_DeviceChangeType {
    CONNECT = 0,
    DISCONNECT = 1
}
export enum audio_AudioScene {
    AUDIO_SCENE_DEFAULT = 0,
    AUDIO_SCENE_RINGING = 1,
    AUDIO_SCENE_PHONE_CALL = 2,
    AUDIO_SCENE_VOICE_CHAT = 3
}
export enum audio_VolumeAdjustType {
    VOLUME_UP = 0,
    VOLUME_DOWN = 1
}
export namespace audio {
    export interface AudioManager {
        setVolume(volumeType: audio_AudioVolumeType, volume: number, callback_: (() => void)): void 
        setVolume(volumeType: audio_AudioVolumeType, volume: number): void 
        getVolume(volumeType: audio_AudioVolumeType, callback_: ((result: number) => void)): void 
        getVolume(volumeType: audio_AudioVolumeType): number 
        getMinVolume(volumeType: audio_AudioVolumeType, callback_: ((result: number) => void)): void 
        getMinVolume(volumeType: audio_AudioVolumeType): number 
        getMaxVolume(volumeType: audio_AudioVolumeType, callback_: ((result: number) => void)): void 
        getMaxVolume(volumeType: audio_AudioVolumeType): number 
        getDevices(deviceFlag: audio_DeviceFlag, callback_: ((result: audio.AudioDeviceDescriptors) => void)): void 
        getDevices(deviceFlag: audio_DeviceFlag): audio.AudioDeviceDescriptors 
        mute(volumeType: audio_AudioVolumeType, mute: boolean, callback_: (() => void)): void 
        mute(volumeType: audio_AudioVolumeType, mute: boolean): void 
        isMute(volumeType: audio_AudioVolumeType, callback_: ((result: boolean) => void)): void 
        isMute(volumeType: audio_AudioVolumeType): boolean 
        isActive(volumeType: audio_AudioVolumeType, callback_: ((result: boolean) => void)): void 
        isActive(volumeType: audio_AudioVolumeType): boolean 
        setMicrophoneMute(mute: boolean, callback_: (() => void)): void 
        setMicrophoneMute(mute: boolean): void 
        isMicrophoneMute(callback_: ((result: boolean) => void)): void 
        isMicrophoneMute(): boolean 
        setRingerMode(mode: audio_AudioRingMode, callback_: (() => void)): void 
        setRingerMode(mode: audio_AudioRingMode): void 
        getRingerMode(callback_: ((result: audio_AudioRingMode) => void)): void 
        getRingerMode(): audio_AudioRingMode 
        setAudioParameter(key: string, value: string, callback_: (() => void)): void 
        setAudioParameter(key: string, value: string): void 
        getAudioParameter(key: string, callback_: ((result: string) => void)): void 
        getAudioParameter(key: string): string 
        setExtraParameters(mainKey: string, kvpairs: Map<string, string>): void 
        getExtraParameters(mainKey: string, subKeys?: Array<string>): Map<string, string> 
        setDeviceActive(deviceType: audio_ActiveDeviceType, active: boolean, callback_: (() => void)): void 
        setDeviceActive(deviceType: audio_ActiveDeviceType, active: boolean): void 
        isDeviceActive(deviceType: audio_ActiveDeviceType, callback_: ((result: boolean) => void)): void 
        isDeviceActive(deviceType: audio_ActiveDeviceType): boolean 
        onVolumeChange(callback_: ((parameter: audio.VolumeEvent) => void)): void 
        onRingerModeChange(callback_: ((parameter: audio_AudioRingMode) => void)): void 
        setAudioScene(scene: audio_AudioScene, callback_: (() => void)): void 
        setAudioScene(scene: audio_AudioScene): void 
        getAudioScene(callback_: ((result: audio_AudioScene) => void)): void 
        getAudioScene(): audio_AudioScene 
        getAudioSceneSync(): audio_AudioScene 
        onDeviceChange(callback_: ((parameter: audio.DeviceChangeAction) => void)): void 
        offDeviceChange(callback_?: ((parameter: audio.DeviceChangeAction) => void)): void 
        onInterrupt(interrupt: audio.AudioInterrupt, callback_: ((parameter: audio.InterruptAction) => void)): void 
        offInterrupt(interrupt: audio.AudioInterrupt, callback_?: ((parameter: audio.InterruptAction) => void)): void 
        getVolumeManager(): audio.AudioVolumeManager 
        getStreamManager(): audio.AudioStreamManager 
        getRoutingManager(): audio.AudioRoutingManager 
        getSessionManager(): audio.AudioSessionManager 
        getSpatializationManager(): audio.AudioSpatializationManager 
        disableSafeMediaVolume(): void 
    }
}
export enum audio_InterruptRequestResultType {
    INTERRUPT_REQUEST_GRANT = 0,
    INTERRUPT_REQUEST_REJECT = 1
}
export namespace audio {
    export interface InterruptResult {
        requestResult: audio_InterruptRequestResultType
        interruptNode: number
    }
}
export enum audio_DeviceBlockStatus {
    UNBLOCKED = 0,
    BLOCKED = 1
}
export namespace audio {
    export interface DeviceBlockStatusInfo {
        blockStatus: audio_DeviceBlockStatus
        devices: audio.AudioDeviceDescriptors
    }
}
export namespace audio {
    export interface AudioRoutingManager {
        getDevices(deviceFlag: audio_DeviceFlag, callback_: ((result: audio.AudioDeviceDescriptors) => void)): void 
        getDevices(deviceFlag: audio_DeviceFlag): audio.AudioDeviceDescriptors 
        getDevicesSync(deviceFlag: audio_DeviceFlag): audio.AudioDeviceDescriptors 
        onDeviceChange(deviceFlag: audio_DeviceFlag, callback_: ((parameter: audio.DeviceChangeAction) => void)): void 
        offDeviceChange(callback_?: ((parameter: audio.DeviceChangeAction) => void)): void 
        getAvailableDevices(deviceUsage: audio_DeviceUsage): audio.AudioDeviceDescriptors 
        onAvailableDeviceChange(deviceUsage: audio_DeviceUsage, callback_: ((parameter: audio.DeviceChangeAction) => void)): void 
        offAvailableDeviceChange(callback_?: ((parameter: audio.DeviceChangeAction) => void)): void 
        setCommunicationDevice(deviceType: audio_CommunicationDeviceType, active: boolean, callback_: (() => void)): void 
        setCommunicationDevice(deviceType: audio_CommunicationDeviceType, active: boolean): void 
        isCommunicationDeviceActive(deviceType: audio_CommunicationDeviceType, callback_: ((result: boolean) => void)): void 
        isCommunicationDeviceActive(deviceType: audio_CommunicationDeviceType): boolean 
        isCommunicationDeviceActiveSync(deviceType: audio_CommunicationDeviceType): boolean 
        selectOutputDevice(outputAudioDevices: audio.AudioDeviceDescriptors, callback_: (() => void)): void 
        selectOutputDevice(outputAudioDevices: audio.AudioDeviceDescriptors): void 
        selectOutputDeviceByFilter(filter: audio.AudioRendererFilter, outputAudioDevices: audio.AudioDeviceDescriptors, callback_: (() => void)): void 
        selectOutputDeviceByFilter(filter: audio.AudioRendererFilter, outputAudioDevices: audio.AudioDeviceDescriptors): void 
        selectInputDevice(inputAudioDevices: audio.AudioDeviceDescriptors, callback_: (() => void)): void 
        selectInputDevice(inputAudioDevices: audio.AudioDeviceDescriptors): void 
        selectInputDeviceByFilter(filter: audio.AudioCapturerFilter, inputAudioDevices: audio.AudioDeviceDescriptors): void 
        getPreferOutputDeviceForRendererInfo(rendererInfo: audio.AudioRendererInfo, callback_: ((result: audio.AudioDeviceDescriptors) => void)): void 
        getPreferOutputDeviceForRendererInfo(rendererInfo: audio.AudioRendererInfo): audio.AudioDeviceDescriptors 
        getPreferredOutputDeviceForRendererInfoSync(rendererInfo: audio.AudioRendererInfo): audio.AudioDeviceDescriptors 
        getPreferredOutputDeviceByFilter(filter: audio.AudioRendererFilter): audio.AudioDeviceDescriptors 
        onPreferOutputDeviceChangeForRendererInfo(rendererInfo: audio.AudioRendererInfo, callback_: ((parameter: audio.AudioDeviceDescriptors) => void)): void 
        offPreferOutputDeviceChangeForRendererInfo(callback_?: ((parameter: audio.AudioDeviceDescriptors) => void)): void 
        getPreferredInputDeviceForCapturerInfo(capturerInfo: audio.AudioCapturerInfo, callback_: ((result: audio.AudioDeviceDescriptors) => void)): void 
        getPreferredInputDeviceForCapturerInfo(capturerInfo: audio.AudioCapturerInfo): audio.AudioDeviceDescriptors 
        getPreferredInputDeviceByFilter(filter: audio.AudioCapturerFilter): audio.AudioDeviceDescriptors 
        onPreferredInputDeviceChangeForCapturerInfo(capturerInfo: audio.AudioCapturerInfo, callback_: ((parameter: audio.AudioDeviceDescriptors) => void)): void 
        offPreferredInputDeviceChangeForCapturerInfo(callback_?: ((parameter: audio.AudioDeviceDescriptors) => void)): void 
        getPreferredInputDeviceForCapturerInfoSync(capturerInfo: audio.AudioCapturerInfo): audio.AudioDeviceDescriptors 
        isMicBlockDetectionSupported(): boolean 
        onMicBlockStatusChanged(callback_: ((parameter: audio.DeviceBlockStatusInfo) => void)): void 
        offMicBlockStatusChanged(callback_?: ((parameter: audio.DeviceBlockStatusInfo) => void)): void 
    }
}
export namespace audio {
    export interface AudioStreamManager {
        getCurrentAudioRendererInfoArray(callback_: ((result: audio.AudioRendererChangeInfoArray) => void)): void 
        getCurrentAudioRendererInfoArray(): audio.AudioRendererChangeInfoArray 
        getCurrentAudioRendererInfoArraySync(): audio.AudioRendererChangeInfoArray 
        getCurrentAudioCapturerInfoArray(callback_: ((result: audio.AudioCapturerChangeInfoArray) => void)): void 
        getCurrentAudioCapturerInfoArray(): audio.AudioCapturerChangeInfoArray 
        getCurrentAudioCapturerInfoArraySync(): audio.AudioCapturerChangeInfoArray 
        getAudioEffectInfoArray(usage: audio_StreamUsage, callback_: ((result: audio.AudioEffectInfoArray) => void)): void 
        getAudioEffectInfoArray(usage: audio_StreamUsage): audio.AudioEffectInfoArray 
        getAudioEffectInfoArraySync(usage: audio_StreamUsage): audio.AudioEffectInfoArray 
        onAudioRendererChange(callback_: ((parameter: audio.AudioRendererChangeInfoArray) => void)): void 
        offAudioRendererChange(): void 
        onAudioCapturerChange(callback_: ((parameter: audio.AudioCapturerChangeInfoArray) => void)): void 
        offAudioCapturerChange(): void 
        isActive(volumeType: audio_AudioVolumeType, callback_: ((result: boolean) => void)): void 
        isActive(volumeType: audio_AudioVolumeType): boolean 
        isActiveSync(volumeType: audio_AudioVolumeType): boolean 
    }
}
export enum audio_AudioConcurrencyMode {
    CONCURRENCY_DEFAULT = 0,
    CONCURRENCY_MIX_WITH_OTHERS = 1,
    CONCURRENCY_DUCK_OTHERS = 2,
    CONCURRENCY_PAUSE_OTHERS = 3
}
export enum audio_AudioSessionDeactivatedReason {
    DEACTIVATED_LOWER_PRIORITY = 0,
    DEACTIVATED_TIMEOUT = 1
}
export namespace audio {
    export interface AudioSessionStrategy {
        concurrencyMode: audio_AudioConcurrencyMode
    }
}
export namespace audio {
    export interface AudioSessionDeactivatedEvent {
        reason: audio_AudioSessionDeactivatedReason
    }
}
export namespace audio {
    export interface AudioSessionManager {
        activateAudioSession(strategy: audio.AudioSessionStrategy): void 
        deactivateAudioSession(): void 
        isAudioSessionActivated(): boolean 
        onAudioSessionDeactivated(callback_: ((parameter: audio.AudioSessionDeactivatedEvent) => void)): void 
        offAudioSessionDeactivated(callback_?: ((parameter: audio.AudioSessionDeactivatedEvent) => void)): void 
    }
}
export namespace audio {
    export interface AudioVolumeManager {
        getVolumeGroupInfos(networkId: string, callback_: ((result: audio.VolumeGroupInfos) => void)): void 
        getVolumeGroupInfos(networkId: string): audio.VolumeGroupInfos 
        getVolumeGroupInfosSync(networkId: string): audio.VolumeGroupInfos 
        getVolumeGroupManager(groupId: number, callback_: ((result: audio.AudioVolumeGroupManager) => void)): void 
        getVolumeGroupManager(groupId: number): audio.AudioVolumeGroupManager 
        getVolumeGroupManagerSync(groupId: number): audio.AudioVolumeGroupManager 
        onVolumeChange(callback_: ((parameter: audio.VolumeEvent) => void)): void 
        offVolumeChange(callback_?: ((parameter: audio.VolumeEvent) => void)): void 
    }
}
export namespace audio {
    export interface AudioVolumeGroupManager {
        setVolume(volumeType: audio_AudioVolumeType, volume: number, callback_: (() => void)): void 
        setVolume(volumeType: audio_AudioVolumeType, volume: number): void 
        setVolumeWithFlag(volumeType: audio_AudioVolumeType, volume: number, flags: number): void 
        getActiveVolumeTypeSync(uid: number): audio_AudioVolumeType 
        getVolume(volumeType: audio_AudioVolumeType, callback_: ((result: number) => void)): void 
        getVolume(volumeType: audio_AudioVolumeType): number 
        getVolumeSync(volumeType: audio_AudioVolumeType): number 
        getMinVolume(volumeType: audio_AudioVolumeType, callback_: ((result: number) => void)): void 
        getMinVolume(volumeType: audio_AudioVolumeType): number 
        getMinVolumeSync(volumeType: audio_AudioVolumeType): number 
        getMaxVolume(volumeType: audio_AudioVolumeType, callback_: ((result: number) => void)): void 
        getMaxVolume(volumeType: audio_AudioVolumeType): number 
        getMaxVolumeSync(volumeType: audio_AudioVolumeType): number 
        mute(volumeType: audio_AudioVolumeType, mute: boolean, callback_: (() => void)): void 
        mute(volumeType: audio_AudioVolumeType, mute: boolean): void 
        isMute(volumeType: audio_AudioVolumeType, callback_: ((result: boolean) => void)): void 
        isMute(volumeType: audio_AudioVolumeType): boolean 
        isMuteSync(volumeType: audio_AudioVolumeType): boolean 
        setRingerMode(mode: audio_AudioRingMode, callback_: (() => void)): void 
        setRingerMode(mode: audio_AudioRingMode): void 
        getRingerMode(callback_: ((result: audio_AudioRingMode) => void)): void 
        getRingerMode(): audio_AudioRingMode 
        getRingerModeSync(): audio_AudioRingMode 
        onRingerModeChange(callback_: ((parameter: audio_AudioRingMode) => void)): void 
        setMicrophoneMute(mute: boolean, callback_: (() => void)): void 
        setMicrophoneMute(mute: boolean): void 
        setMicMute(mute: boolean): void 
        setMicMutePersistent(mute: boolean, type: audio_PolicyType): void 
        isPersistentMicMute(): boolean 
        isMicrophoneMute(callback_: ((result: boolean) => void)): void 
        isMicrophoneMute(): boolean 
        isMicrophoneMuteSync(): boolean 
        onMicStateChange(callback_: ((parameter: audio.MicStateChangeEvent) => void)): void 
        offMicStateChange(callback_?: ((parameter: audio.MicStateChangeEvent) => void)): void 
        isVolumeUnadjustable(): boolean 
        adjustVolumeByStep(adjustType: audio_VolumeAdjustType, callback_: (() => void)): void 
        adjustVolumeByStep(adjustType: audio_VolumeAdjustType): void 
        adjustSystemVolumeByStep(volumeType: audio_AudioVolumeType, adjustType: audio_VolumeAdjustType, callback_: (() => void)): void 
        adjustSystemVolumeByStep(volumeType: audio_AudioVolumeType, adjustType: audio_VolumeAdjustType): void 
        getSystemVolumeInDb(volumeType: audio_AudioVolumeType, volumeLevel: number, device: audio_DeviceType, callback_: ((result: number) => void)): void 
        getSystemVolumeInDb(volumeType: audio_AudioVolumeType, volumeLevel: number, device: audio_DeviceType): number 
        getSystemVolumeInDbSync(volumeType: audio_AudioVolumeType, volumeLevel: number, device: audio_DeviceType): number 
        getMaxAmplitudeForInputDevice(inputDevice: audio.AudioDeviceDescriptor): number 
        getMaxAmplitudeForOutputDevice(outputDevice: audio.AudioDeviceDescriptor): number 
    }
}
export namespace audio {
    export interface AudioSpatialEnabledStateForDevice {
        deviceDescriptor: audio.AudioDeviceDescriptor
        enabled: boolean
    }
}
export namespace audio {
    export interface AudioSpatializationManager {
        isSpatializationSupported(): boolean 
        isSpatializationSupportedForDevice(deviceDescriptor: audio.AudioDeviceDescriptor): boolean 
        isHeadTrackingSupported(): boolean 
        isHeadTrackingSupportedForDevice(deviceDescriptor: audio.AudioDeviceDescriptor): boolean 
        setSpatializationEnabled(enable: boolean, callback_: (() => void)): void 
        setSpatializationEnabled(enable: boolean): void 
        setSpatializationEnabled(deviceDescriptor: audio.AudioDeviceDescriptor, enabled: boolean): void 
        isSpatializationEnabled(): boolean 
        isSpatializationEnabled(deviceDescriptor: audio.AudioDeviceDescriptor): boolean 
        onSpatializationEnabledChange(callback_: ((parameter: boolean) => void)): void 
        onSpatializationEnabledChangeForAnyDevice(callback_: ((parameter: audio.AudioSpatialEnabledStateForDevice) => void)): void 
        offSpatializationEnabledChange(callback_?: ((parameter: boolean) => void)): void 
        offSpatializationEnabledChangeForAnyDevice(callback_?: ((parameter: audio.AudioSpatialEnabledStateForDevice) => void)): void 
        setHeadTrackingEnabled(enable: boolean, callback_: (() => void)): void 
        setHeadTrackingEnabled(enable: boolean): void 
        setHeadTrackingEnabled(deviceDescriptor: audio.AudioDeviceDescriptor, enabled: boolean): void 
        isHeadTrackingEnabled(): boolean 
        isHeadTrackingEnabled(deviceDescriptor: audio.AudioDeviceDescriptor): boolean 
        onHeadTrackingEnabledChange(callback_: ((parameter: boolean) => void)): void 
        onHeadTrackingEnabledChangeForAnyDevice(callback_: ((parameter: audio.AudioSpatialEnabledStateForDevice) => void)): void 
        offHeadTrackingEnabledChange(callback_?: ((parameter: boolean) => void)): void 
        offHeadTrackingEnabledChangeForAnyDevice(callback_?: ((parameter: audio.AudioSpatialEnabledStateForDevice) => void)): void 
        updateSpatialDeviceState(spatialDeviceState: audio.AudioSpatialDeviceState): void 
        setSpatializationSceneType(spatializationSceneType: audio_AudioSpatializationSceneType): void 
        getSpatializationSceneType(): audio_AudioSpatializationSceneType 
    }
}
export enum audio_ConnectType {
    CONNECT_TYPE_LOCAL = 1,
    CONNECT_TYPE_DISTRIBUTED = 2
}
export namespace audio {
    export interface VolumeGroupInfo {
        readonly networkId: string
        readonly groupId: number
        readonly mappingId: number
        readonly groupName: string
        readonly type: audio_ConnectType
    }
}
export namespace audio {
    export interface AudioRendererChangeInfo {
        readonly streamId: number
        readonly clientUid: number
        readonly rendererInfo: audio.AudioRendererInfo
        readonly rendererState: audio_AudioState
        readonly deviceDescriptors: audio.AudioDeviceDescriptors
    }
}
export namespace audio {
    export interface AudioCapturerChangeInfo {
        readonly streamId: number
        readonly clientUid: number
        readonly capturerInfo: audio.AudioCapturerInfo
        readonly capturerState: audio_AudioState
        readonly deviceDescriptors: audio.AudioDeviceDescriptors
        readonly muted?: boolean
    }
}
export namespace audio {
    export interface AudioDeviceDescriptor {
        readonly deviceRole: audio_DeviceRole
        readonly deviceType: audio_DeviceType
        readonly id: number
        readonly name: string
        readonly address: string
        readonly sampleRates: Array<number>
        readonly channelCounts: Array<number>
        readonly channelMasks: Array<number>
        readonly networkId: string
        readonly interruptGroupId: number
        readonly volumeGroupId: number
        readonly displayName: string
        readonly encodingTypes?: Array<audio_AudioEncodingType>
    }
}
export namespace audio {
    export interface VolumeEvent {
        volumeType: audio_AudioVolumeType
        volume: number
        updateUi: boolean
        volumeGroupId: number
        networkId: string
    }
}
export namespace audio {
    export interface InterruptAction {
        actionType: audio_InterruptActionType
        type?: audio_InterruptType
        hint?: audio_InterruptHint
        activated?: boolean
    }
}
export namespace audio {
    export interface AudioInterrupt {
        streamUsage: audio_StreamUsage
        contentType: audio_ContentType
        pauseWhenDucked: boolean
    }
}
export namespace audio {
    export interface MicStateChangeEvent {
        mute: boolean
    }
}
export namespace audio {
    export interface DeviceChangeAction {
        type: audio_DeviceChangeType
        deviceDescriptors: audio.AudioDeviceDescriptors
    }
}
export enum audio_ChannelBlendMode {
    MODE_DEFAULT = 0,
    MODE_BLEND_LR = 1,
    MODE_ALL_LEFT = 2,
    MODE_ALL_RIGHT = 3
}
export enum audio_AudioStreamDeviceChangeReason {
    REASON_UNKNOWN = 0,
    REASON_NEW_DEVICE_AVAILABLE = 1,
    REASON_OLD_DEVICE_UNAVAILABLE = 2,
    REASON_OVERRODE = 3
}
export namespace audio {
    export interface AudioStreamDeviceChangeInfo {
        devices: audio.AudioDeviceDescriptors
        changeReason: audio_AudioStreamDeviceChangeReason
    }
}
export enum audio_AudioDataCallbackResult {
    INVALID = -1,
    VALID = 0
}
export namespace audio {
    export interface AudioRenderer {
        readonly state: audio_AudioState
        getRendererInfo(callback_: ((result: audio.AudioRendererInfo) => void)): void 
        getRendererInfo(): audio.AudioRendererInfo 
        getRendererInfoSync(): audio.AudioRendererInfo 
        getStreamInfo(callback_: ((result: audio.AudioStreamInfo) => void)): void 
        getStreamInfo(): audio.AudioStreamInfo 
        getStreamInfoSync(): audio.AudioStreamInfo 
        getAudioStreamId(callback_: ((result: number) => void)): void 
        getAudioStreamId(): number 
        getAudioStreamIdSync(): number 
        getAudioEffectMode(callback_: ((result: audio_AudioEffectMode) => void)): void 
        getAudioEffectMode(): audio_AudioEffectMode 
        setAudioEffectMode(mode: audio_AudioEffectMode, callback_: (() => void)): void 
        setAudioEffectMode(mode: audio_AudioEffectMode): void 
        start(callback_: (() => void)): void 
        start(): void 
        write(buffer: NativeBuffer, callback_: ((result: number) => void)): void 
        write(buffer: NativeBuffer): number 
        getAudioTime(callback_: ((result: number) => void)): void 
        getAudioTime(): number 
        getAudioTimeSync(): number 
        drain(callback_: (() => void)): void 
        drain(): void 
        flush(): void 
        pause(callback_: (() => void)): void 
        pause(): void 
        stop(callback_: (() => void)): void 
        stop(): void 
        release(callback_: (() => void)): void 
        release(): void 
        getBufferSize(callback_: ((result: number) => void)): void 
        getBufferSize(): number 
        getBufferSizeSync(): number 
        setRenderRate(rate: audio_AudioRendererRate, callback_: (() => void)): void 
        setRenderRate(rate: audio_AudioRendererRate): void 
        setSpeed(speed: number): void 
        getRenderRate(callback_: ((result: audio_AudioRendererRate) => void)): void 
        getRenderRate(): audio_AudioRendererRate 
        getRenderRateSync(): audio_AudioRendererRate 
        getSpeed(): number 
        setInterruptMode(mode: audio_InterruptMode, callback_: (() => void)): void 
        setInterruptMode(mode: audio_InterruptMode): void 
        setInterruptModeSync(mode: audio_InterruptMode): void 
        setVolume(volume: number, callback_: (() => void)): void 
        setVolume(volume: number): void 
        getVolume(): number 
        setVolumeWithRamp(volume: number, duration: number): void 
        getMinStreamVolume(callback_: ((result: number) => void)): void 
        getMinStreamVolume(): number 
        getMinStreamVolumeSync(): number 
        getMaxStreamVolume(callback_: ((result: number) => void)): void 
        getMaxStreamVolume(): number 
        getMaxStreamVolumeSync(): number 
        getUnderflowCount(callback_: ((result: number) => void)): void 
        getUnderflowCount(): number 
        getUnderflowCountSync(): number 
        getCurrentOutputDevices(callback_: ((result: audio.AudioDeviceDescriptors) => void)): void 
        getCurrentOutputDevices(): audio.AudioDeviceDescriptors 
        getCurrentOutputDevicesSync(): audio.AudioDeviceDescriptors 
        setChannelBlendMode(mode: audio_ChannelBlendMode): void 
        setSilentModeAndMixWithOthers(on: boolean): void 
        getSilentModeAndMixWithOthers(): boolean 
        setDefaultOutputDevice(deviceType: audio_DeviceType): void 
        onAudioInterrupt(callback_: ((parameter: audio.InterruptEvent) => void)): void 
        onMarkReach(frame: number, callback_: ((parameter: number) => void)): void 
        offMarkReach(): void 
        onPeriodReach(frame: number, callback_: ((parameter: number) => void)): void 
        offPeriodReach(): void 
        onStateChange(callback_: ((parameter: audio_AudioState) => void)): void 
        onOutputDeviceChange(callback_: ((parameter: audio.AudioDeviceDescriptors) => void)): void 
        onOutputDeviceChangeWithInfo(callback_: ((parameter: audio.AudioStreamDeviceChangeInfo) => void)): void 
        offOutputDeviceChange(callback_?: ((parameter: audio.AudioDeviceDescriptors) => void)): void 
        offOutputDeviceChangeWithInfo(callback_?: ((parameter: audio.AudioStreamDeviceChangeInfo) => void)): void 
        onWriteData(callback_: audio.AudioRendererWriteDataCallback): void 
        offWriteData(callback_?: audio.AudioRendererWriteDataCallback): void 
    }
}
export enum audio_SourceType {
    SOURCE_TYPE_INVALID = -1,
    SOURCE_TYPE_MIC = 0,
    SOURCE_TYPE_VOICE_RECOGNITION = 1,
    SOURCE_TYPE_PLAYBACK_CAPTURE = 2,
    SOURCE_TYPE_WAKEUP = 3,
    SOURCE_TYPE_VOICE_CALL = 4,
    SOURCE_TYPE_VOICE_COMMUNICATION = 7,
    SOURCE_TYPE_VOICE_MESSAGE = 10,
    SOURCE_TYPE_CAMCORDER = 13
}
export namespace audio {
    export interface AudioCapturerInfo {
        source: audio_SourceType
        capturerFlags: number
    }
}
export namespace audio {
    export interface AudioCapturerOptions {
        streamInfo: audio.AudioStreamInfo
        capturerInfo: audio.AudioCapturerInfo
        playbackCaptureConfig?: audio.AudioPlaybackCaptureConfig
    }
}
export namespace audio {
    export interface CaptureFilterOptions {
        usages: Array<audio_StreamUsage>
    }
}
export namespace audio {
    export interface AudioPlaybackCaptureConfig {
        filterOptions: audio.CaptureFilterOptions
    }
}
export namespace audio {
    export interface AudioCapturer {
        readonly state: audio_AudioState
        getCapturerInfo(callback_: ((result: audio.AudioCapturerInfo) => void)): void 
        getCapturerInfo(): audio.AudioCapturerInfo 
        getCapturerInfoSync(): audio.AudioCapturerInfo 
        getStreamInfo(callback_: ((result: audio.AudioStreamInfo) => void)): void 
        getStreamInfo(): audio.AudioStreamInfo 
        getStreamInfoSync(): audio.AudioStreamInfo 
        getAudioStreamId(callback_: ((result: number) => void)): void 
        getAudioStreamId(): number 
        getAudioStreamIdSync(): number 
        start(callback_: (() => void)): void 
        start(): void 
        read(size: number, isBlockingRead: boolean, callback_: ((result: NativeBuffer) => void)): void 
        read(size: number, isBlockingRead: boolean): NativeBuffer 
        getAudioTime(callback_: ((result: number) => void)): void 
        getAudioTime(): number 
        getAudioTimeSync(): number 
        stop(callback_: (() => void)): void 
        stop(): void 
        release(callback_: (() => void)): void 
        release(): void 
        getBufferSize(callback_: ((result: number) => void)): void 
        getBufferSize(): number 
        getBufferSizeSync(): number 
        getCurrentInputDevices(): audio.AudioDeviceDescriptors 
        getCurrentAudioCapturerChangeInfo(): audio.AudioCapturerChangeInfo 
        getOverflowCount(): number 
        getOverflowCountSync(): number 
        onMarkReach(frame: number, callback_: ((parameter: number) => void)): void 
        offMarkReach(): void 
        onPeriodReach(frame: number, callback_: ((parameter: number) => void)): void 
        offPeriodReach(): void 
        onStateChange(callback_: ((parameter: audio_AudioState) => void)): void 
        onAudioInterrupt(callback_: ((parameter: audio.InterruptEvent) => void)): void 
        offAudioInterrupt(): void 
        onInputDeviceChange(callback_: ((parameter: audio.AudioDeviceDescriptors) => void)): void 
        offInputDeviceChange(callback_?: ((parameter: audio.AudioDeviceDescriptors) => void)): void 
        onAudioCapturerChange(callback_: ((parameter: audio.AudioCapturerChangeInfo) => void)): void 
        offAudioCapturerChange(callback_?: ((parameter: audio.AudioCapturerChangeInfo) => void)): void 
        onReadData(callback_: ((parameter: NativeBuffer) => void)): void 
        offReadData(callback_?: ((parameter: NativeBuffer) => void)): void 
    }
}
export enum audio_AsrNoiseSuppressionMode {
    BYPASS = 0,
    STANDARD = 1,
    NEAR_FIELD = 2,
    FAR_FIELD = 3
}
export enum audio_AsrAecMode {
    BYPASS = 0,
    STANDARD = 1
}
export enum audio_AsrVoiceControlMode {
    AUDIO_2_VOICE_TX = 0,
    AUDIO_MIX_2_VOICE_TX = 1,
    AUDIO_2_VOICE_TX_EX = 2,
    AUDIO_MIX_2_VOICE_TX_EX = 3
}
export enum audio_AsrVoiceMuteMode {
    OUTPUT_MUTE = 0,
    INPUT_MUTE = 1,
    TTS_MUTE = 2,
    CALL_MUTE = 3,
    OUTPUT_MUTE_EX = 4
}
export enum audio_AsrWhisperDetectionMode {
    BYPASS = 0,
    STANDARD = 1
}
export namespace audio {
    export interface AsrProcessingController {
        setAsrAecMode(mode: audio_AsrAecMode): boolean 
        getAsrAecMode(): audio_AsrAecMode 
        setAsrNoiseSuppressionMode(mode: audio_AsrNoiseSuppressionMode): boolean 
        getAsrNoiseSuppressionMode(): audio_AsrNoiseSuppressionMode 
        isWhispering(): boolean 
        setAsrVoiceControlMode(mode: audio_AsrVoiceControlMode, enable: boolean): boolean 
        setAsrVoiceMuteMode(mode: audio_AsrVoiceMuteMode, enable: boolean): boolean 
        setAsrWhisperDetectionMode(mode: audio_AsrWhisperDetectionMode): boolean 
        getAsrWhisperDetectionMode(): audio_AsrWhisperDetectionMode 
    }
}
export enum audio_ToneType {
    TONE_TYPE_DIAL_0 = 0,
    TONE_TYPE_DIAL_1 = 1,
    TONE_TYPE_DIAL_2 = 2,
    TONE_TYPE_DIAL_3 = 3,
    TONE_TYPE_DIAL_4 = 4,
    TONE_TYPE_DIAL_5 = 5,
    TONE_TYPE_DIAL_6 = 6,
    TONE_TYPE_DIAL_7 = 7,
    TONE_TYPE_DIAL_8 = 8,
    TONE_TYPE_DIAL_9 = 9,
    TONE_TYPE_DIAL_S = 10,
    TONE_TYPE_DIAL_P = 11,
    TONE_TYPE_DIAL_A = 12,
    TONE_TYPE_DIAL_B = 13,
    TONE_TYPE_DIAL_C = 14,
    TONE_TYPE_DIAL_D = 15,
    TONE_TYPE_COMMON_SUPERVISORY_DIAL = 100,
    TONE_TYPE_COMMON_SUPERVISORY_BUSY = 101,
    TONE_TYPE_COMMON_SUPERVISORY_CONGESTION = 102,
    TONE_TYPE_COMMON_SUPERVISORY_RADIO_ACK = 103,
    TONE_TYPE_COMMON_SUPERVISORY_RADIO_NOT_AVAILABLE = 104,
    TONE_TYPE_COMMON_SUPERVISORY_CALL_WAITING = 106,
    TONE_TYPE_COMMON_SUPERVISORY_RINGTONE = 107,
    TONE_TYPE_COMMON_PROPRIETARY_BEEP = 200,
    TONE_TYPE_COMMON_PROPRIETARY_ACK = 201,
    TONE_TYPE_COMMON_PROPRIETARY_PROMPT = 203,
    TONE_TYPE_COMMON_PROPRIETARY_DOUBLE_BEEP = 204
}
export namespace audio {
    export interface TonePlayer {
        load(type: audio_ToneType, callback_: (() => void)): void 
        load(type: audio_ToneType): void 
        start(callback_: (() => void)): void 
        start(): void 
        stop(callback_: (() => void)): void 
        stop(): void 
        release(callback_: (() => void)): void 
        release(): void 
    }
}
export enum audio_AudioEffectMode {
    EFFECT_NONE = 0,
    EFFECT_DEFAULT = 1
}
export namespace audio {
    export interface AudioSpatialDeviceState {
        address: string
        isSpatializationSupported: boolean
        isHeadTrackingSupported: boolean
        spatialDeviceType: audio_AudioSpatialDeviceType
    }
}
export enum audio_AudioSpatialDeviceType {
    SPATIAL_DEVICE_TYPE_NONE = 0,
    SPATIAL_DEVICE_TYPE_IN_EAR_HEADPHONE = 1,
    SPATIAL_DEVICE_TYPE_HALF_IN_EAR_HEADPHONE = 2,
    SPATIAL_DEVICE_TYPE_OVER_EAR_HEADPHONE = 3,
    SPATIAL_DEVICE_TYPE_GLASSES = 4,
    SPATIAL_DEVICE_TYPE_OTHERS = 5
}
export enum audio_AudioSpatializationSceneType {
    DEFAULT = 0,
    MUSIC = 1,
    MOVIE = 2,
    AUDIOBOOK = 3
}
export enum audio_AudioChannelLayout {
    CH_LAYOUT_UNKNOWN = 0,
    CH_LAYOUT_MONO = 4,
    CH_LAYOUT_STEREO = 3,
    CH_LAYOUT_STEREO_DOWNMIX = 1610612736,
    CH_LAYOUT_2POINT1 = 11,
    CH_LAYOUT_3POINT0 = 259,
    CH_LAYOUT_SURROUND = 7,
    CH_LAYOUT_3POINT1 = 15,
    CH_LAYOUT_4POINT0 = 263,
    CH_LAYOUT_QUAD = 51,
    CH_LAYOUT_QUAD_SIDE = 1539,
    CH_LAYOUT_2POINT0POINT2 = 206158430211,
    CH_LAYOUT_AMB_ORDER1_ACN_N3D = 17592186044417,
    CH_LAYOUT_AMB_ORDER1_ACN_SN3D = 17592186048513,
    CH_LAYOUT_AMB_ORDER1_FUMA = 17592186044673,
    CH_LAYOUT_4POINT1 = 271,
    CH_LAYOUT_5POINT0 = 1543,
    CH_LAYOUT_5POINT0_BACK = 55,
    CH_LAYOUT_2POINT1POINT2 = 206158430219,
    CH_LAYOUT_3POINT0POINT2 = 206158430215,
    CH_LAYOUT_5POINT1 = 1551,
    CH_LAYOUT_5POINT1_BACK = 63,
    CH_LAYOUT_6POINT0 = 1799,
    CH_LAYOUT_HEXAGONAL = 311,
    CH_LAYOUT_3POINT1POINT2 = 20495,
    CH_LAYOUT_6POINT0_FRONT = 1731,
    CH_LAYOUT_6POINT1 = 1807,
    CH_LAYOUT_6POINT1_BACK = 319,
    CH_LAYOUT_6POINT1_FRONT = 1739,
    CH_LAYOUT_7POINT0 = 1591,
    CH_LAYOUT_7POINT0_FRONT = 1735,
    CH_LAYOUT_7POINT1 = 1599,
    CH_LAYOUT_OCTAGONAL = 1847,
    CH_LAYOUT_5POINT1POINT2 = 206158431759,
    CH_LAYOUT_7POINT1_WIDE = 1743,
    CH_LAYOUT_7POINT1_WIDE_BACK = 255,
    CH_LAYOUT_AMB_ORDER2_ACN_N3D = 17592186044418,
    CH_LAYOUT_AMB_ORDER2_ACN_SN3D = 17592186048514,
    CH_LAYOUT_AMB_ORDER2_FUMA = 17592186044674,
    CH_LAYOUT_5POINT1POINT4 = 185871,
    CH_LAYOUT_7POINT1POINT2 = 206158431807,
    CH_LAYOUT_7POINT1POINT4 = 185919,
    CH_LAYOUT_10POINT2 = 6442473271,
    CH_LAYOUT_9POINT1POINT4 = 6442636863,
    CH_LAYOUT_9POINT1POINT6 = 212601067071,
    CH_LAYOUT_HEXADECAGONAL = 6442710839,
    CH_LAYOUT_AMB_ORDER3_ACN_N3D = 17592186044419,
    CH_LAYOUT_AMB_ORDER3_ACN_SN3D = 17592186048515,
    CH_LAYOUT_AMB_ORDER3_FUMA = 17592186044675
}
