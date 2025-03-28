/*
 * Copyright (C) 2025 Huawei Device Co., Ltd.
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

import { AsyncCallback, Callback } from '@ohos.base';

export namespace audio {
  loadLibrary("audio_ani");

  export native function getAudioManager(): AudioManager;

  export enum AudioVolumeType {
    VOICE_CALL = 0,
    RINGTONE = 2,
    MEDIA = 3,
    ALARM = 4,
    ACCESSIBILITY = 5,
    VOICE_ASSISTANT = 9,
    ULTRASONIC = 10,
    ALL = 100,
  }

  export enum DeviceRole {
    INPUT_DEVICE = 1,
    OUTPUT_DEVICE = 2,
  }

  export enum DeviceType {
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
    DEFAULT = 1000,
  }

  export enum ActiveDeviceType {
    SPEAKER = 2,
    BLUETOOTH_SCO = 7,
  }

  export enum CommunicationDeviceType {
    SPEAKER = 2
  }

  enum AudioEncodingType {
    ENCODING_TYPE_INVALID = -1,
    ENCODING_TYPE_RAW = 0
  }

  export enum ContentType {
    CONTENT_TYPE_UNKNOWN = 0,
    CONTENT_TYPE_SPEECH = 1,
    CONTENT_TYPE_MUSIC = 2,
    CONTENT_TYPE_MOVIE = 3,
    CONTENT_TYPE_SONIFICATION = 4,
    CONTENT_TYPE_RINGTONE = 5,
  }

  export enum StreamUsage {
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
    STREAM_USAGE_VOICE_CALL_ASSISTANT = 21,
  }

  export interface AudioRendererInfo {
    content?: ContentType;
    usage: StreamUsage;
    rendererFlags: number;
  }

  class AudioRendererInfoHandle implements AudioRendererInfo {
    constructor() {}
    content?: ContentType;
    usage: StreamUsage;
    rendererFlags: number = 0;
  }

  export enum InterruptMode {
    SHARE_MODE = 0,
    INDEPENDENT_MODE = 1
  }

  export enum InterruptType {
    INTERRUPT_TYPE_BEGIN = 1,
    INTERRUPT_TYPE_END = 2
  }

  export enum InterruptHint {
    INTERRUPT_HINT_NONE = 0,
    INTERRUPT_HINT_RESUME = 1,
    INTERRUPT_HINT_PAUSE = 2,
    INTERRUPT_HINT_STOP = 3,
    INTERRUPT_HINT_DUCK = 4,
    INTERRUPT_HINT_UNDUCK = 5,
  }

  export enum InterruptForceType {
    INTERRUPT_FORCE = 0,
    INTERRUPT_SHARE = 1
  }

  export interface InterruptEvent {
    eventType: InterruptType;
    forceType: InterruptForceType;
    hintType: InterruptHint;
  }

  class InterruptEventHandle implements InterruptEvent {
    eventType: InterruptType;
    forceType: InterruptForceType;
    hintType: InterruptHint;
  }

  export interface AudioManager {
    getVolumeManager(): AudioVolumeManager;
  }

  class AudioManagerHandle implements AudioManager {
    private nativeAudioManagerContext: long = 0;

    constructor(nativePtr: long) {
      if (this.nativeAudioManagerContext == 0) {
        this.nativeAudioManagerContext = nativePtr;
      }
    }
    private native onInner<T, O>(type: string, callback: Callback<T>, obj?: O): void;
    native getVolumeManager(): AudioVolumeManager;
  }

  enum DeviceBlockStatus{
    UNBLOCKED = 0,
    BLOCKED = 1,
  }

  export interface DeviceBlockStatusInfo {
    blockStatus: DeviceBlockStatus;
    devices: AudioDeviceDescriptors;
  }

  class DeviceBlockStatusInfoHandle implements DeviceBlockStatusInfo {
    constructor() {}
    blockStatus: DeviceBlockStatus;
    devices: AudioDeviceDescriptors = new Array<Readonly<AudioDeviceDescriptor>>();
  }

  export interface AudioVolumeManager {
  }

  class AudioVolumeManagerHandle implements AudioVolumeManager {
    private nativeAudioVolumeManagerContext: long = 0;

    constructor(nativePtr: long) {
      if (this.nativeAudioVolumeManagerContext == 0) {
        this.nativeAudioVolumeManagerContext = nativePtr;
      }
    }
    private native onInner<T, O>(type: string, callback: Callback<T>, obj?: O): void;
  }

  export interface AudioDeviceDescriptor {
    readonly deviceRole: DeviceRole;
    readonly deviceType: DeviceType;
    readonly id: number;
    readonly name: string;
    readonly address: string;
    readonly sampleRates: Array<number>;
    readonly channelCounts: Array<number>;
    readonly channelMasks: Array<number>;
    readonly networkId: string;
    readonly interruptGroupId: number;
    readonly volumeGroupId: number;
    readonly displayName: string;
    readonly encodingTypes?: Array<AudioEncodingType>;
  }

  class AudioDeviceDescriptorHandle implements AudioDeviceDescriptor {
    constructor() {}
    readonly deviceRole: DeviceRole;
    readonly deviceType: DeviceType;
    readonly id: number = 0;
    readonly name: string = "";
    readonly address: string = "";
    readonly sampleRates: Array<number> = new Array<number>();
    readonly channelCounts: Array<number> = new Array<number>();
    readonly channelMasks: Array<number> = new Array<number>();
    readonly networkId: string = "";
    readonly interruptGroupId: number = 0;
    readonly volumeGroupId: number = 0;
    readonly displayName: string = "";
    readonly encodingTypes?: Array<AudioEncodingType>;
  }

  export type AudioDeviceDescriptors = Array<Readonly<AudioDeviceDescriptor>>;

  export interface VolumeEvent {
    volumeType: AudioVolumeType;
    volume: number;
    updateUi: boolean;
    volumeGroupId: number;
    networkId: string;
  }

  class VolumeEventHandle implements VolumeEvent {
    constructor() {}
    volumeType: AudioVolumeType;
    volume: number = 0;
    updateUi: boolean = false;
    volumeGroupId: number = 0;
    networkId: string = "";
  }

  export enum AudioStreamDeviceChangeReason {
    REASON_UNKNOWN = 0,
    REASON_NEW_DEVICE_AVAILABLE = 1,
    REASON_OLD_DEVICE_UNAVAILABLE = 2,
    REASON_OVERRODE = 3,
  }

  export interface AudioStreamDeviceChangeInfo {
    devices: AudioDeviceDescriptors;
    changeReason: AudioStreamDeviceChangeReason;
  }

  class AudioStreamDeviceChangeHandle implements AudioStreamDeviceChangeInfo {
    constructor() {}
    devices: AudioDeviceDescriptors = new Array<AudioDeviceDescriptor>();
    changeReason: AudioStreamDeviceChangeReason;
  }
}