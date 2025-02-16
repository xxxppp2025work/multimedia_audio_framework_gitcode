/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifndef OH_AUDIO_H
#define OH_AUDIO_H

#ifndef _INTEROP_TYPES_H_
#define _INTEROP_TYPES_H_

#include <stdint.h>

#define INTEROP_FATAL(msg, ...) fprintf(stderr, msg "\n", ##__VA_ARGS__); abort();

typedef enum InteropTag
{
  INTEROP_TAG_UNDEFINED = 101,
  INTEROP_TAG_INT32 = 102,
  INTEROP_TAG_FLOAT32 = 103,
  INTEROP_TAG_STRING = 104,
  INTEROP_TAG_LENGTH = 105,
  INTEROP_TAG_RESOURCE = 106,
  INTEROP_TAG_OBJECT = 107,
} InteropTag;

typedef enum InteropRuntimeType
{
  INTEROP_RUNTIME_UNEXPECTED = -1,
  INTEROP_RUNTIME_NUMBER = 1,
  INTEROP_RUNTIME_STRING = 2,
  INTEROP_RUNTIME_OBJECT = 3,
  INTEROP_RUNTIME_BOOLEAN = 4,
  INTEROP_RUNTIME_UNDEFINED = 5,
  INTEROP_RUNTIME_BIGINT = 6,
  INTEROP_RUNTIME_FUNCTION = 7,
  INTEROP_RUNTIME_SYMBOL = 8,
  INTEROP_RUNTIME_MATERIALIZED = 9,
} InteropRuntimeType;

typedef float InteropFloat32;
typedef double InteropFloat64;
typedef int32_t InteropInt32;
typedef unsigned int InteropUInt32; // TODO: update unsigned int
typedef int64_t InteropInt64;
typedef int8_t InteropInt8;
typedef uint8_t InteropUInt8;
typedef int64_t InteropDate;
typedef int8_t InteropBoolean;
typedef const char* InteropCharPtr;
typedef void* InteropNativePointer;

struct _InteropVMContext;
typedef struct _InteropVMContext* InteropVMContext;
struct _InteropPipelineContext;
typedef struct _InteropPipelineContext* InteropPipelineContext;
struct _InteropVMObject;
typedef struct _InteropVMObject* InteropVMObject;
struct _InteropNode;
typedef struct _InteropNode* InteropNodeHandle;
typedef struct InteropDeferred {
    void* handler;
    void* context;
    void (*resolve)(struct InteropDeferred* thiz, uint8_t* data, int32_t length);
    void (*reject)(struct InteropDeferred* thiz, const char* message);
} InteropDeferred;

// Binary layout of InteropString must match that of KStringPtrImpl.
typedef struct InteropString {
  const char* chars;
  InteropInt32 length;
} InteropString;

typedef struct InteropEmpty {
  InteropInt32 dummy; // Empty structs are forbidden in C.
} InteropEmpty;

typedef struct InteropNumber {
  InteropInt8 tag;
  union {
    InteropFloat32 f32;
    InteropInt32 i32;
  };
} InteropNumber;

// Binary layout of InteropLength must match that of KLength.
typedef struct InteropLength
{
  InteropInt8 type;
  InteropFloat32 value;
  InteropInt32 unit;
  InteropInt32 resource;
} InteropLength;

typedef struct InteropCustomObject {
  char kind[20];
  InteropInt32 id;
  // Data of custom object.
  union {
    InteropInt32 ints[4];
    InteropFloat32 floats[4];
    void* pointers[4];
    InteropString string;
  };
} InteropCustomObject;

typedef struct InteropUndefined {
  InteropInt32 dummy; // Empty structs are forbidden in C.
} InteropUndefined;

typedef struct InteropVoid {
  InteropInt32 dummy; // Empty structs are forbidden in C.
} InteropVoid;

typedef struct InteropFunction {
  InteropInt32 id;
} InteropFunction;
typedef InteropFunction InteropCallback;
typedef InteropFunction InteropErrorCallback;

typedef struct InteropMaterialized {
  InteropNativePointer ptr;
} InteropMaterialized;

typedef struct InteropCallbackResource {
  InteropInt32 resourceId;
  void (*hold)(InteropInt32 resourceId);
  void (*release)(InteropInt32 resourceId);
} InteropCallbackResource;

typedef struct InteropBuffer {
  InteropCallbackResource resource;
  InteropNativePointer data;
  InteropInt64 length;
} InteropBuffer;

#endif // _INTEROP_TYPES_H_


#define AUDIO_API_VERSION 1

#include <stdint.h>

/* clang-format off */

#ifdef __cplusplus
extern "C" {
#endif

typedef InteropTag OH_Tag;
typedef InteropRuntimeType OH_AUDIO_RuntimeType;

typedef InteropFloat32 OH_Float32;
typedef InteropFloat64 OH_Float64;
typedef InteropInt32 OH_Int32;
typedef InteropUInt32 OH_UInt32;
typedef InteropInt64 OH_Int64;
typedef InteropInt8 OH_Int8;
typedef InteropBoolean OH_Boolean;
typedef InteropCharPtr OH_CharPtr;
typedef InteropNativePointer OH_NativePointer;
typedef InteropString OH_String;
typedef InteropCallbackResource OH_AUDIO_CallbackResource;
typedef InteropNumber OH_Number;
typedef InteropMaterialized OH_Materialized;
typedef InteropCustomObject OH_CustomObject;
typedef InteropUndefined OH_Undefined;
// typedef InteropAPIKind OH_APIKind;
typedef InteropVMContext OH_AUDIO_VMContext;
typedef InteropBuffer OH_Buffer;
typedef InteropLength OH_Length;
typedef InteropFunction OH_Function;

typedef enum OH_APIKind {
    OH_AUDIO_API_KIND = 100
} OH_APIKind;

typedef struct OH_AnyAPI {
    OH_Int32 version;
} OH_AnyAPI;

typedef struct Array_audio_StreamUsage Array_audio_StreamUsage;
typedef struct Opt_Array_audio_StreamUsage Opt_Array_audio_StreamUsage;
typedef struct OH_AUDIO_CaptureFilterOptions OH_AUDIO_CaptureFilterOptions;
typedef struct Opt_CaptureFilterOptions Opt_CaptureFilterOptions;
typedef struct Array_audio_AudioEncodingType Array_audio_AudioEncodingType;
typedef struct Opt_Array_audio_AudioEncodingType Opt_Array_audio_AudioEncodingType;
typedef struct Array_Number Array_Number;
typedef struct Opt_Array_Number Opt_Array_Number;
typedef struct OH_AUDIO_AudioCapturerInfo OH_AUDIO_AudioCapturerInfo;
typedef struct Opt_AudioCapturerInfo Opt_AudioCapturerInfo;
typedef struct OH_AUDIO_AudioRendererInfo OH_AUDIO_AudioRendererInfo;
typedef struct Opt_AudioRendererInfo Opt_AudioRendererInfo;
typedef struct AUDIO_AsyncCallback_Void AUDIO_AsyncCallback_Void;
typedef struct Opt_AUDIO_AsyncCallback_Void Opt_AUDIO_AsyncCallback_Void;
typedef struct AUDIO_Callback_Buffer_Void AUDIO_Callback_Buffer_Void;
typedef struct Opt_AUDIO_Callback_Buffer_Void Opt_AUDIO_Callback_Buffer_Void;
typedef struct AUDIO_Callback_AudioCapturerChangeInfo_Void AUDIO_Callback_AudioCapturerChangeInfo_Void;
typedef struct Opt_AUDIO_Callback_AudioCapturerChangeInfo_Void Opt_AUDIO_Callback_AudioCapturerChangeInfo_Void;
typedef struct AUDIO_Callback_AudioDeviceDescriptors_Void AUDIO_Callback_AudioDeviceDescriptors_Void;
typedef struct Opt_AUDIO_Callback_AudioDeviceDescriptors_Void Opt_AUDIO_Callback_AudioDeviceDescriptors_Void;
typedef struct AUDIO_Callback_InterruptEvent_Void AUDIO_Callback_InterruptEvent_Void;
typedef struct Opt_AUDIO_Callback_InterruptEvent_Void Opt_AUDIO_Callback_InterruptEvent_Void;
typedef struct AUDIO_Callback_AudioState_Void AUDIO_Callback_AudioState_Void;
typedef struct Opt_AUDIO_Callback_AudioState_Void Opt_AUDIO_Callback_AudioState_Void;
typedef struct AUDIO_Callback_Number_Void AUDIO_Callback_Number_Void;
typedef struct Opt_AUDIO_Callback_Number_Void Opt_AUDIO_Callback_Number_Void;
typedef struct AUDIO_AsyncCallback_Number_Void AUDIO_AsyncCallback_Number_Void;
typedef struct Opt_AUDIO_AsyncCallback_Number_Void Opt_AUDIO_AsyncCallback_Number_Void;
typedef struct AUDIO_AsyncCallback_Buffer_Void AUDIO_AsyncCallback_Buffer_Void;
typedef struct Opt_AUDIO_AsyncCallback_Buffer_Void Opt_AUDIO_AsyncCallback_Buffer_Void;
typedef struct AUDIO_AsyncCallback_AudioStreamInfo_Void AUDIO_AsyncCallback_AudioStreamInfo_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioStreamInfo_Void Opt_AUDIO_AsyncCallback_AudioStreamInfo_Void;
typedef struct AUDIO_AsyncCallback_AudioCapturerInfo_Void AUDIO_AsyncCallback_AudioCapturerInfo_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioCapturerInfo_Void Opt_AUDIO_AsyncCallback_AudioCapturerInfo_Void;
typedef struct OH_AUDIO_AudioPlaybackCaptureConfig OH_AUDIO_AudioPlaybackCaptureConfig;
typedef struct Opt_AudioPlaybackCaptureConfig Opt_AudioPlaybackCaptureConfig;
typedef struct OH_AUDIO_AudioStreamInfo OH_AUDIO_AudioStreamInfo;
typedef struct Opt_AudioStreamInfo Opt_AudioStreamInfo;
typedef struct AUDIO_AudioRendererWriteDataCallback AUDIO_AudioRendererWriteDataCallback;
typedef struct Opt_AUDIO_AudioRendererWriteDataCallback Opt_AUDIO_AudioRendererWriteDataCallback;
typedef struct AUDIO_Callback_AudioStreamDeviceChangeInfo_Void AUDIO_Callback_AudioStreamDeviceChangeInfo_Void;
typedef struct Opt_AUDIO_Callback_AudioStreamDeviceChangeInfo_Void Opt_AUDIO_Callback_AudioStreamDeviceChangeInfo_Void;
typedef struct AUDIO_AsyncCallback_AudioDeviceDescriptors_Void AUDIO_AsyncCallback_AudioDeviceDescriptors_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioDeviceDescriptors_Void Opt_AUDIO_AsyncCallback_AudioDeviceDescriptors_Void;
typedef struct AUDIO_AsyncCallback_AudioRendererRate_Void AUDIO_AsyncCallback_AudioRendererRate_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioRendererRate_Void Opt_AUDIO_AsyncCallback_AudioRendererRate_Void;
typedef struct AUDIO_AsyncCallback_AudioEffectMode_Void AUDIO_AsyncCallback_AudioEffectMode_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioEffectMode_Void Opt_AUDIO_AsyncCallback_AudioEffectMode_Void;
typedef struct AUDIO_AsyncCallback_AudioRendererInfo_Void AUDIO_AsyncCallback_AudioRendererInfo_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioRendererInfo_Void Opt_AUDIO_AsyncCallback_AudioRendererInfo_Void;
typedef struct Array_CustomObject Array_CustomObject;
typedef struct Opt_Array_CustomObject Opt_Array_CustomObject;
typedef struct OH_AUDIO_AudioSpatialDeviceState OH_AUDIO_AudioSpatialDeviceState;
typedef struct Opt_AudioSpatialDeviceState Opt_AudioSpatialDeviceState;
typedef struct AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void;
typedef struct Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void;
typedef struct AUDIO_Callback_Boolean_Void AUDIO_Callback_Boolean_Void;
typedef struct Opt_AUDIO_Callback_Boolean_Void Opt_AUDIO_Callback_Boolean_Void;
typedef struct OH_AUDIO_AudioDeviceDescriptor OH_AUDIO_AudioDeviceDescriptor;
typedef struct Opt_AudioDeviceDescriptor Opt_AudioDeviceDescriptor;
typedef struct AUDIO_Callback_MicStateChangeEvent_Void AUDIO_Callback_MicStateChangeEvent_Void;
typedef struct Opt_AUDIO_Callback_MicStateChangeEvent_Void Opt_AUDIO_Callback_MicStateChangeEvent_Void;
typedef struct AUDIO_AsyncCallback_Boolean_Void AUDIO_AsyncCallback_Boolean_Void;
typedef struct Opt_AUDIO_AsyncCallback_Boolean_Void Opt_AUDIO_AsyncCallback_Boolean_Void;
typedef struct AUDIO_Callback_AudioRingMode_Void AUDIO_Callback_AudioRingMode_Void;
typedef struct Opt_AUDIO_Callback_AudioRingMode_Void Opt_AUDIO_Callback_AudioRingMode_Void;
typedef struct AUDIO_AsyncCallback_AudioRingMode_Void AUDIO_AsyncCallback_AudioRingMode_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioRingMode_Void Opt_AUDIO_AsyncCallback_AudioRingMode_Void;
typedef struct AUDIO_Callback_VolumeEvent_Void AUDIO_Callback_VolumeEvent_Void;
typedef struct Opt_AUDIO_Callback_VolumeEvent_Void Opt_AUDIO_Callback_VolumeEvent_Void;
typedef struct AUDIO_AsyncCallback_AudioVolumeGroupManager_Void AUDIO_AsyncCallback_AudioVolumeGroupManager_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioVolumeGroupManager_Void Opt_AUDIO_AsyncCallback_AudioVolumeGroupManager_Void;
typedef struct AUDIO_AsyncCallback_VolumeGroupInfos_Void AUDIO_AsyncCallback_VolumeGroupInfos_Void;
typedef struct Opt_AUDIO_AsyncCallback_VolumeGroupInfos_Void Opt_AUDIO_AsyncCallback_VolumeGroupInfos_Void;
typedef struct AUDIO_Callback_AudioSessionDeactivatedEvent_Void AUDIO_Callback_AudioSessionDeactivatedEvent_Void;
typedef struct Opt_AUDIO_Callback_AudioSessionDeactivatedEvent_Void Opt_AUDIO_Callback_AudioSessionDeactivatedEvent_Void;
typedef struct OH_AUDIO_AudioSessionStrategy OH_AUDIO_AudioSessionStrategy;
typedef struct Opt_AudioSessionStrategy Opt_AudioSessionStrategy;
typedef struct AUDIO_Callback_AudioCapturerChangeInfoArray_Void AUDIO_Callback_AudioCapturerChangeInfoArray_Void;
typedef struct Opt_AUDIO_Callback_AudioCapturerChangeInfoArray_Void Opt_AUDIO_Callback_AudioCapturerChangeInfoArray_Void;
typedef struct AUDIO_Callback_AudioRendererChangeInfoArray_Void AUDIO_Callback_AudioRendererChangeInfoArray_Void;
typedef struct Opt_AUDIO_Callback_AudioRendererChangeInfoArray_Void Opt_AUDIO_Callback_AudioRendererChangeInfoArray_Void;
typedef struct AUDIO_AsyncCallback_AudioEffectInfoArray_Void AUDIO_AsyncCallback_AudioEffectInfoArray_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioEffectInfoArray_Void Opt_AUDIO_AsyncCallback_AudioEffectInfoArray_Void;
typedef struct AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void Opt_AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void;
typedef struct AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void Opt_AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void;
typedef struct AUDIO_Callback_DeviceBlockStatusInfo_Void AUDIO_Callback_DeviceBlockStatusInfo_Void;
typedef struct Opt_AUDIO_Callback_DeviceBlockStatusInfo_Void Opt_AUDIO_Callback_DeviceBlockStatusInfo_Void;
typedef struct OH_AUDIO_AudioCapturerFilter OH_AUDIO_AudioCapturerFilter;
typedef struct Opt_AudioCapturerFilter Opt_AudioCapturerFilter;
typedef struct OH_AUDIO_AudioRendererFilter OH_AUDIO_AudioRendererFilter;
typedef struct Opt_AudioRendererFilter Opt_AudioRendererFilter;
typedef struct AUDIO_Callback_DeviceChangeAction_Void AUDIO_Callback_DeviceChangeAction_Void;
typedef struct Opt_AUDIO_Callback_DeviceChangeAction_Void Opt_AUDIO_Callback_DeviceChangeAction_Void;
typedef struct AUDIO_Callback_InterruptAction_Void AUDIO_Callback_InterruptAction_Void;
typedef struct Opt_AUDIO_Callback_InterruptAction_Void Opt_AUDIO_Callback_InterruptAction_Void;
typedef struct OH_AUDIO_AudioInterrupt OH_AUDIO_AudioInterrupt;
typedef struct Opt_AudioInterrupt Opt_AudioInterrupt;
typedef struct AUDIO_AsyncCallback_AudioScene_Void AUDIO_AsyncCallback_AudioScene_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioScene_Void Opt_AUDIO_AsyncCallback_AudioScene_Void;
typedef struct AUDIO_AsyncCallback_String_Void AUDIO_AsyncCallback_String_Void;
typedef struct Opt_AUDIO_AsyncCallback_String_Void Opt_AUDIO_AsyncCallback_String_Void;
typedef OH_Materialized OH_AUDIO_TonePlayer;
typedef struct Opt_TonePlayer Opt_TonePlayer;
typedef struct AUDIO_AsyncCallback_TonePlayer_Void AUDIO_AsyncCallback_TonePlayer_Void;
typedef struct Opt_AUDIO_AsyncCallback_TonePlayer_Void Opt_AUDIO_AsyncCallback_TonePlayer_Void;
typedef OH_Materialized OH_AUDIO_AudioRenderer;
typedef struct Opt_AudioRenderer Opt_AudioRenderer;
typedef struct AUDIO_AsyncCallback_AudioRenderer_Void AUDIO_AsyncCallback_AudioRenderer_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioRenderer_Void Opt_AUDIO_AsyncCallback_AudioRenderer_Void;
typedef OH_Materialized OH_AUDIO_AudioCapturer;
typedef struct Opt_AudioCapturer Opt_AudioCapturer;
typedef struct AUDIO_AsyncCallback_AudioCapturer_Void AUDIO_AsyncCallback_AudioCapturer_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioCapturer_Void Opt_AUDIO_AsyncCallback_AudioCapturer_Void;
typedef struct OH_AUDIO_AudioCapturerOptions OH_AUDIO_AudioCapturerOptions;
typedef struct Opt_AudioCapturerOptions Opt_AudioCapturerOptions;
typedef struct OH_AUDIO_AudioStreamDeviceChangeInfo OH_AUDIO_AudioStreamDeviceChangeInfo;
typedef struct Opt_AudioStreamDeviceChangeInfo Opt_AudioStreamDeviceChangeInfo;
typedef struct OH_AUDIO_AudioCapturerChangeInfo OH_AUDIO_AudioCapturerChangeInfo;
typedef struct Opt_AudioCapturerChangeInfo Opt_AudioCapturerChangeInfo;
typedef struct OH_AUDIO_AudioRendererChangeInfo OH_AUDIO_AudioRendererChangeInfo;
typedef struct Opt_AudioRendererChangeInfo Opt_AudioRendererChangeInfo;
typedef struct OH_AUDIO_VolumeGroupInfo OH_AUDIO_VolumeGroupInfo;
typedef struct Opt_VolumeGroupInfo Opt_VolumeGroupInfo;
typedef struct OH_AUDIO_AudioSpatialEnabledStateForDevice OH_AUDIO_AudioSpatialEnabledStateForDevice;
typedef struct Opt_AudioSpatialEnabledStateForDevice Opt_AudioSpatialEnabledStateForDevice;
typedef struct OH_AUDIO_MicStateChangeEvent OH_AUDIO_MicStateChangeEvent;
typedef struct Opt_MicStateChangeEvent Opt_MicStateChangeEvent;
typedef OH_Materialized OH_AUDIO_AudioVolumeGroupManager;
typedef struct Opt_AudioVolumeGroupManager Opt_AudioVolumeGroupManager;
typedef struct OH_AUDIO_AudioSessionDeactivatedEvent OH_AUDIO_AudioSessionDeactivatedEvent;
typedef struct Opt_AudioSessionDeactivatedEvent Opt_AudioSessionDeactivatedEvent;
typedef struct OH_AUDIO_DeviceBlockStatusInfo OH_AUDIO_DeviceBlockStatusInfo;
typedef struct Opt_DeviceBlockStatusInfo Opt_DeviceBlockStatusInfo;
typedef struct OH_AUDIO_InterruptResult OH_AUDIO_InterruptResult;
typedef struct Opt_InterruptResult Opt_InterruptResult;
typedef struct OH_AUDIO_InterruptAction OH_AUDIO_InterruptAction;
typedef struct Opt_InterruptAction Opt_InterruptAction;
typedef struct OH_AUDIO_DeviceChangeAction OH_AUDIO_DeviceChangeAction;
typedef struct Opt_DeviceChangeAction Opt_DeviceChangeAction;
typedef struct OH_AUDIO_VolumeEvent OH_AUDIO_VolumeEvent;
typedef struct Opt_VolumeEvent Opt_VolumeEvent;
typedef struct Array_String Array_String;
typedef struct Opt_Array_String Opt_Array_String;
typedef struct Map_String_String Map_String_String;
typedef struct Opt_Map_String_String Opt_Map_String_String;
typedef struct OH_AUDIO_InterruptEvent OH_AUDIO_InterruptEvent;
typedef struct Opt_InterruptEvent Opt_InterruptEvent;
typedef struct OH_AUDIO_AudioRendererOptions OH_AUDIO_AudioRendererOptions;
typedef struct Opt_AudioRendererOptions Opt_AudioRendererOptions;
typedef enum OH_AUDIO_audio_SourceType {
    OH_AUDIO_AUDIO_SOURCE_TYPE_SOURCE_TYPE_INVALID = -1,
    OH_AUDIO_AUDIO_SOURCE_TYPE_SOURCE_TYPE_MIC = 0,
    OH_AUDIO_AUDIO_SOURCE_TYPE_SOURCE_TYPE_VOICE_RECOGNITION = 1,
    OH_AUDIO_AUDIO_SOURCE_TYPE_SOURCE_TYPE_PLAYBACK_CAPTURE = 2,
    OH_AUDIO_AUDIO_SOURCE_TYPE_SOURCE_TYPE_WAKEUP = 3,
    OH_AUDIO_AUDIO_SOURCE_TYPE_SOURCE_TYPE_VOICE_CALL = 4,
    OH_AUDIO_AUDIO_SOURCE_TYPE_SOURCE_TYPE_VOICE_COMMUNICATION = 7,
    OH_AUDIO_AUDIO_SOURCE_TYPE_SOURCE_TYPE_VOICE_MESSAGE = 10,
    OH_AUDIO_AUDIO_SOURCE_TYPE_SOURCE_TYPE_CAMCORDER = 13,
} OH_AUDIO_audio_SourceType;
typedef struct Opt_audio_SourceType {
    OH_Tag tag;
    OH_AUDIO_audio_SourceType value;
} Opt_audio_SourceType;
typedef enum OH_AUDIO_audio_StreamUsage {
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_UNKNOWN = 0,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_MEDIA = 1,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_MUSIC = 1,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_VOICE_COMMUNICATION = 2,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_VOICE_ASSISTANT = 3,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_ALARM = 4,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_VOICE_MESSAGE = 5,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_NOTIFICATION_RINGTONE = 6,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_RINGTONE = 6,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_NOTIFICATION = 7,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_ACCESSIBILITY = 8,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_SYSTEM = 9,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_MOVIE = 10,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_GAME = 11,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_AUDIOBOOK = 12,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_NAVIGATION = 13,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_DTMF = 14,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_ENFORCED_TONE = 15,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_ULTRASONIC = 16,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_VIDEO_COMMUNICATION = 17,
    OH_AUDIO_AUDIO_STREAM_USAGE_STREAM_USAGE_VOICE_CALL_ASSISTANT = 21,
} OH_AUDIO_audio_StreamUsage;
typedef struct Opt_audio_StreamUsage {
    OH_Tag tag;
    OH_AUDIO_audio_StreamUsage value;
} Opt_audio_StreamUsage;
typedef enum OH_AUDIO_audio_ContentType {
    OH_AUDIO_AUDIO_CONTENT_TYPE_CONTENT_TYPE_UNKNOWN = 0,
    OH_AUDIO_AUDIO_CONTENT_TYPE_CONTENT_TYPE_SPEECH = 1,
    OH_AUDIO_AUDIO_CONTENT_TYPE_CONTENT_TYPE_MUSIC = 2,
    OH_AUDIO_AUDIO_CONTENT_TYPE_CONTENT_TYPE_MOVIE = 3,
    OH_AUDIO_AUDIO_CONTENT_TYPE_CONTENT_TYPE_SONIFICATION = 4,
    OH_AUDIO_AUDIO_CONTENT_TYPE_CONTENT_TYPE_RINGTONE = 5,
} OH_AUDIO_audio_ContentType;
typedef struct Opt_audio_ContentType {
    OH_Tag tag;
    OH_AUDIO_audio_ContentType value;
} Opt_audio_ContentType;
typedef enum OH_AUDIO_audio_AudioChannelLayout {
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_UNKNOWN = 0,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_MONO = 4,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_STEREO = 3,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_STEREO_DOWNMIX = 1610612736,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_2POINT1 = 11,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_3POINT0 = 259,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_SURROUND = 7,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_3POINT1 = 15,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_4POINT0 = 263,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_QUAD = 51,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_QUAD_SIDE = 1539,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_2POINT0POINT2 = 206158430211,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_AMB_ORDER1_ACN_N3D = 17592186044417,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_AMB_ORDER1_ACN_SN3D = 17592186048513,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_AMB_ORDER1_FUMA = 17592186044673,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_4POINT1 = 271,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_5POINT0 = 1543,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_5POINT0_BACK = 55,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_2POINT1POINT2 = 206158430219,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_3POINT0POINT2 = 206158430215,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_5POINT1 = 1551,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_5POINT1_BACK = 63,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_6POINT0 = 1799,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_HEXAGONAL = 311,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_3POINT1POINT2 = 20495,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_6POINT0_FRONT = 1731,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_6POINT1 = 1807,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_6POINT1_BACK = 319,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_6POINT1_FRONT = 1739,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_7POINT0 = 1591,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_7POINT0_FRONT = 1735,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_7POINT1 = 1599,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_OCTAGONAL = 1847,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_5POINT1POINT2 = 206158431759,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_7POINT1_WIDE = 1743,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_7POINT1_WIDE_BACK = 255,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_AMB_ORDER2_ACN_N3D = 17592186044418,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_AMB_ORDER2_ACN_SN3D = 17592186048514,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_AMB_ORDER2_FUMA = 17592186044674,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_5POINT1POINT4 = 185871,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_7POINT1POINT2 = 206158431807,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_7POINT1POINT4 = 185919,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_10POINT2 = 6442473271,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_9POINT1POINT4 = 6442636863,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_9POINT1POINT6 = 212601067071,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_HEXADECAGONAL = 6442710839,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_AMB_ORDER3_ACN_N3D = 17592186044419,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_AMB_ORDER3_ACN_SN3D = 17592186048515,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_LAYOUT_CH_LAYOUT_AMB_ORDER3_FUMA = 17592186044675,
} OH_AUDIO_audio_AudioChannelLayout;
typedef struct Opt_audio_AudioChannelLayout {
    OH_Tag tag;
    OH_AUDIO_audio_AudioChannelLayout value;
} Opt_audio_AudioChannelLayout;
typedef enum OH_AUDIO_audio_AudioEncodingType {
    OH_AUDIO_AUDIO_AUDIO_ENCODING_TYPE_ENCODING_TYPE_INVALID = -1,
    OH_AUDIO_AUDIO_AUDIO_ENCODING_TYPE_ENCODING_TYPE_RAW = 0,
} OH_AUDIO_audio_AudioEncodingType;
typedef struct Opt_audio_AudioEncodingType {
    OH_Tag tag;
    OH_AUDIO_audio_AudioEncodingType value;
} Opt_audio_AudioEncodingType;
typedef enum OH_AUDIO_audio_AudioSampleFormat {
    OH_AUDIO_AUDIO_AUDIO_SAMPLE_FORMAT_SAMPLE_FORMAT_INVALID = -1,
    OH_AUDIO_AUDIO_AUDIO_SAMPLE_FORMAT_SAMPLE_FORMAT_U8 = 0,
    OH_AUDIO_AUDIO_AUDIO_SAMPLE_FORMAT_SAMPLE_FORMAT_S16LE = 1,
    OH_AUDIO_AUDIO_AUDIO_SAMPLE_FORMAT_SAMPLE_FORMAT_S24LE = 2,
    OH_AUDIO_AUDIO_AUDIO_SAMPLE_FORMAT_SAMPLE_FORMAT_S32LE = 3,
    OH_AUDIO_AUDIO_AUDIO_SAMPLE_FORMAT_SAMPLE_FORMAT_F32LE = 4,
} OH_AUDIO_audio_AudioSampleFormat;
typedef struct Opt_audio_AudioSampleFormat {
    OH_Tag tag;
    OH_AUDIO_audio_AudioSampleFormat value;
} Opt_audio_AudioSampleFormat;
typedef enum OH_AUDIO_audio_AudioChannel {
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_CHANNEL_1 = 1,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_CHANNEL_2 = 2,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_CHANNEL_3 = 3,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_CHANNEL_4 = 4,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_CHANNEL_5 = 5,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_CHANNEL_6 = 6,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_CHANNEL_7 = 7,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_CHANNEL_8 = 8,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_CHANNEL_9 = 9,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_CHANNEL_10 = 10,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_CHANNEL_12 = 12,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_CHANNEL_14 = 14,
    OH_AUDIO_AUDIO_AUDIO_CHANNEL_CHANNEL_16 = 16,
} OH_AUDIO_audio_AudioChannel;
typedef struct Opt_audio_AudioChannel {
    OH_Tag tag;
    OH_AUDIO_audio_AudioChannel value;
} Opt_audio_AudioChannel;
typedef enum OH_AUDIO_audio_AudioSamplingRate {
    OH_AUDIO_AUDIO_AUDIO_SAMPLING_RATE_SAMPLE_RATE_8000 = 8000,
    OH_AUDIO_AUDIO_AUDIO_SAMPLING_RATE_SAMPLE_RATE_11025 = 11025,
    OH_AUDIO_AUDIO_AUDIO_SAMPLING_RATE_SAMPLE_RATE_12000 = 12000,
    OH_AUDIO_AUDIO_AUDIO_SAMPLING_RATE_SAMPLE_RATE_16000 = 16000,
    OH_AUDIO_AUDIO_AUDIO_SAMPLING_RATE_SAMPLE_RATE_22050 = 22050,
    OH_AUDIO_AUDIO_AUDIO_SAMPLING_RATE_SAMPLE_RATE_24000 = 24000,
    OH_AUDIO_AUDIO_AUDIO_SAMPLING_RATE_SAMPLE_RATE_32000 = 32000,
    OH_AUDIO_AUDIO_AUDIO_SAMPLING_RATE_SAMPLE_RATE_44100 = 44100,
    OH_AUDIO_AUDIO_AUDIO_SAMPLING_RATE_SAMPLE_RATE_48000 = 48000,
    OH_AUDIO_AUDIO_AUDIO_SAMPLING_RATE_SAMPLE_RATE_64000 = 64000,
    OH_AUDIO_AUDIO_AUDIO_SAMPLING_RATE_SAMPLE_RATE_88200 = 88200,
    OH_AUDIO_AUDIO_AUDIO_SAMPLING_RATE_SAMPLE_RATE_96000 = 96000,
    OH_AUDIO_AUDIO_AUDIO_SAMPLING_RATE_SAMPLE_RATE_176400 = 176400,
    OH_AUDIO_AUDIO_AUDIO_SAMPLING_RATE_SAMPLE_RATE_192000 = 192000,
} OH_AUDIO_audio_AudioSamplingRate;
typedef struct Opt_audio_AudioSamplingRate {
    OH_Tag tag;
    OH_AUDIO_audio_AudioSamplingRate value;
} Opt_audio_AudioSamplingRate;
typedef enum OH_AUDIO_audio_AudioSpatialDeviceType {
    OH_AUDIO_AUDIO_AUDIO_SPATIAL_DEVICE_TYPE_SPATIAL_DEVICE_TYPE_NONE = 0,
    OH_AUDIO_AUDIO_AUDIO_SPATIAL_DEVICE_TYPE_SPATIAL_DEVICE_TYPE_IN_EAR_HEADPHONE = 1,
    OH_AUDIO_AUDIO_AUDIO_SPATIAL_DEVICE_TYPE_SPATIAL_DEVICE_TYPE_HALF_IN_EAR_HEADPHONE = 2,
    OH_AUDIO_AUDIO_AUDIO_SPATIAL_DEVICE_TYPE_SPATIAL_DEVICE_TYPE_OVER_EAR_HEADPHONE = 3,
    OH_AUDIO_AUDIO_AUDIO_SPATIAL_DEVICE_TYPE_SPATIAL_DEVICE_TYPE_GLASSES = 4,
    OH_AUDIO_AUDIO_AUDIO_SPATIAL_DEVICE_TYPE_SPATIAL_DEVICE_TYPE_OTHERS = 5,
} OH_AUDIO_audio_AudioSpatialDeviceType;
typedef struct Opt_audio_AudioSpatialDeviceType {
    OH_Tag tag;
    OH_AUDIO_audio_AudioSpatialDeviceType value;
} Opt_audio_AudioSpatialDeviceType;
typedef enum OH_AUDIO_audio_DeviceType {
    OH_AUDIO_AUDIO_DEVICE_TYPE_INVALID = 0,
    OH_AUDIO_AUDIO_DEVICE_TYPE_EARPIECE = 1,
    OH_AUDIO_AUDIO_DEVICE_TYPE_SPEAKER = 2,
    OH_AUDIO_AUDIO_DEVICE_TYPE_WIRED_HEADSET = 3,
    OH_AUDIO_AUDIO_DEVICE_TYPE_WIRED_HEADPHONES = 4,
    OH_AUDIO_AUDIO_DEVICE_TYPE_BLUETOOTH_SCO = 7,
    OH_AUDIO_AUDIO_DEVICE_TYPE_BLUETOOTH_A2DP = 8,
    OH_AUDIO_AUDIO_DEVICE_TYPE_MIC = 15,
    OH_AUDIO_AUDIO_DEVICE_TYPE_USB_HEADSET = 22,
    OH_AUDIO_AUDIO_DEVICE_TYPE_DISPLAY_PORT = 23,
    OH_AUDIO_AUDIO_DEVICE_TYPE_REMOTE_CAST = 24,
    OH_AUDIO_AUDIO_DEVICE_TYPE_DEFAULT = 1000,
} OH_AUDIO_audio_DeviceType;
typedef struct Opt_audio_DeviceType {
    OH_Tag tag;
    OH_AUDIO_audio_DeviceType value;
} Opt_audio_DeviceType;
typedef enum OH_AUDIO_audio_DeviceRole {
    OH_AUDIO_AUDIO_DEVICE_ROLE_INPUT_DEVICE = 1,
    OH_AUDIO_AUDIO_DEVICE_ROLE_OUTPUT_DEVICE = 2,
} OH_AUDIO_audio_DeviceRole;
typedef struct Opt_audio_DeviceRole {
    OH_Tag tag;
    OH_AUDIO_audio_DeviceRole value;
} Opt_audio_DeviceRole;
typedef enum OH_AUDIO_audio_AudioConcurrencyMode {
    OH_AUDIO_AUDIO_AUDIO_CONCURRENCY_MODE_CONCURRENCY_DEFAULT = 0,
    OH_AUDIO_AUDIO_AUDIO_CONCURRENCY_MODE_CONCURRENCY_MIX_WITH_OTHERS = 1,
    OH_AUDIO_AUDIO_AUDIO_CONCURRENCY_MODE_CONCURRENCY_DUCK_OTHERS = 2,
    OH_AUDIO_AUDIO_AUDIO_CONCURRENCY_MODE_CONCURRENCY_PAUSE_OTHERS = 3,
} OH_AUDIO_audio_AudioConcurrencyMode;
typedef struct Opt_audio_AudioConcurrencyMode {
    OH_Tag tag;
    OH_AUDIO_audio_AudioConcurrencyMode value;
} Opt_audio_AudioConcurrencyMode;
typedef enum OH_AUDIO_audio_AudioState {
    OH_AUDIO_AUDIO_AUDIO_STATE_STATE_INVALID = -1,
    OH_AUDIO_AUDIO_AUDIO_STATE_STATE_NEW = 0,
    OH_AUDIO_AUDIO_AUDIO_STATE_STATE_PREPARED = 1,
    OH_AUDIO_AUDIO_AUDIO_STATE_STATE_RUNNING = 2,
    OH_AUDIO_AUDIO_AUDIO_STATE_STATE_STOPPED = 3,
    OH_AUDIO_AUDIO_AUDIO_STATE_STATE_RELEASED = 4,
    OH_AUDIO_AUDIO_AUDIO_STATE_STATE_PAUSED = 5,
} OH_AUDIO_audio_AudioState;
typedef struct Opt_audio_AudioState {
    OH_Tag tag;
    OH_AUDIO_audio_AudioState value;
} Opt_audio_AudioState;
typedef enum OH_AUDIO_audio_ToneType {
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_DIAL_0 = 0,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_DIAL_1 = 1,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_DIAL_2 = 2,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_DIAL_3 = 3,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_DIAL_4 = 4,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_DIAL_5 = 5,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_DIAL_6 = 6,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_DIAL_7 = 7,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_DIAL_8 = 8,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_DIAL_9 = 9,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_DIAL_S = 10,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_DIAL_P = 11,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_DIAL_A = 12,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_DIAL_B = 13,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_DIAL_C = 14,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_DIAL_D = 15,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_COMMON_SUPERVISORY_DIAL = 100,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_COMMON_SUPERVISORY_BUSY = 101,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_COMMON_SUPERVISORY_CONGESTION = 102,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_COMMON_SUPERVISORY_RADIO_ACK = 103,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_COMMON_SUPERVISORY_RADIO_NOT_AVAILABLE = 104,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_COMMON_SUPERVISORY_CALL_WAITING = 106,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_COMMON_SUPERVISORY_RINGTONE = 107,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_COMMON_PROPRIETARY_BEEP = 200,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_COMMON_PROPRIETARY_ACK = 201,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_COMMON_PROPRIETARY_PROMPT = 203,
    OH_AUDIO_AUDIO_TONE_TYPE_TONE_TYPE_COMMON_PROPRIETARY_DOUBLE_BEEP = 204,
} OH_AUDIO_audio_ToneType;
typedef struct Opt_audio_ToneType {
    OH_Tag tag;
    OH_AUDIO_audio_ToneType value;
} Opt_audio_ToneType;
typedef enum OH_AUDIO_audio_AsrWhisperDetectionMode {
    OH_AUDIO_AUDIO_ASR_WHISPER_DETECTION_MODE_BYPASS = 0,
    OH_AUDIO_AUDIO_ASR_WHISPER_DETECTION_MODE_STANDARD = 1,
} OH_AUDIO_audio_AsrWhisperDetectionMode;
typedef struct Opt_audio_AsrWhisperDetectionMode {
    OH_Tag tag;
    OH_AUDIO_audio_AsrWhisperDetectionMode value;
} Opt_audio_AsrWhisperDetectionMode;
typedef enum OH_AUDIO_audio_AsrVoiceMuteMode {
    OH_AUDIO_AUDIO_ASR_VOICE_MUTE_MODE_OUTPUT_MUTE = 0,
    OH_AUDIO_AUDIO_ASR_VOICE_MUTE_MODE_INPUT_MUTE = 1,
    OH_AUDIO_AUDIO_ASR_VOICE_MUTE_MODE_TTS_MUTE = 2,
    OH_AUDIO_AUDIO_ASR_VOICE_MUTE_MODE_CALL_MUTE = 3,
    OH_AUDIO_AUDIO_ASR_VOICE_MUTE_MODE_OUTPUT_MUTE_EX = 4,
} OH_AUDIO_audio_AsrVoiceMuteMode;
typedef struct Opt_audio_AsrVoiceMuteMode {
    OH_Tag tag;
    OH_AUDIO_audio_AsrVoiceMuteMode value;
} Opt_audio_AsrVoiceMuteMode;
typedef enum OH_AUDIO_audio_AsrVoiceControlMode {
    OH_AUDIO_AUDIO_ASR_VOICE_CONTROL_MODE_AUDIO_2_VOICE_TX = 0,
    OH_AUDIO_AUDIO_ASR_VOICE_CONTROL_MODE_AUDIO_MIX_2_VOICE_TX = 1,
    OH_AUDIO_AUDIO_ASR_VOICE_CONTROL_MODE_AUDIO_2_VOICE_TX_EX = 2,
    OH_AUDIO_AUDIO_ASR_VOICE_CONTROL_MODE_AUDIO_MIX_2_VOICE_TX_EX = 3,
} OH_AUDIO_audio_AsrVoiceControlMode;
typedef struct Opt_audio_AsrVoiceControlMode {
    OH_Tag tag;
    OH_AUDIO_audio_AsrVoiceControlMode value;
} Opt_audio_AsrVoiceControlMode;
typedef enum OH_AUDIO_audio_AsrNoiseSuppressionMode {
    OH_AUDIO_AUDIO_ASR_NOISE_SUPPRESSION_MODE_BYPASS = 0,
    OH_AUDIO_AUDIO_ASR_NOISE_SUPPRESSION_MODE_STANDARD = 1,
    OH_AUDIO_AUDIO_ASR_NOISE_SUPPRESSION_MODE_NEAR_FIELD = 2,
    OH_AUDIO_AUDIO_ASR_NOISE_SUPPRESSION_MODE_FAR_FIELD = 3,
} OH_AUDIO_audio_AsrNoiseSuppressionMode;
typedef struct Opt_audio_AsrNoiseSuppressionMode {
    OH_Tag tag;
    OH_AUDIO_audio_AsrNoiseSuppressionMode value;
} Opt_audio_AsrNoiseSuppressionMode;
typedef enum OH_AUDIO_audio_AsrAecMode {
    OH_AUDIO_AUDIO_ASR_AEC_MODE_BYPASS = 0,
    OH_AUDIO_AUDIO_ASR_AEC_MODE_STANDARD = 1,
} OH_AUDIO_audio_AsrAecMode;
typedef struct Opt_audio_AsrAecMode {
    OH_Tag tag;
    OH_AUDIO_audio_AsrAecMode value;
} Opt_audio_AsrAecMode;
typedef enum OH_AUDIO_audio_ChannelBlendMode {
    OH_AUDIO_AUDIO_CHANNEL_BLEND_MODE_MODE_DEFAULT = 0,
    OH_AUDIO_AUDIO_CHANNEL_BLEND_MODE_MODE_BLEND_LR = 1,
    OH_AUDIO_AUDIO_CHANNEL_BLEND_MODE_MODE_ALL_LEFT = 2,
    OH_AUDIO_AUDIO_CHANNEL_BLEND_MODE_MODE_ALL_RIGHT = 3,
} OH_AUDIO_audio_ChannelBlendMode;
typedef struct Opt_audio_ChannelBlendMode {
    OH_Tag tag;
    OH_AUDIO_audio_ChannelBlendMode value;
} Opt_audio_ChannelBlendMode;
typedef enum OH_AUDIO_audio_InterruptMode {
    OH_AUDIO_AUDIO_INTERRUPT_MODE_SHARE_MODE = 0,
    OH_AUDIO_AUDIO_INTERRUPT_MODE_INDEPENDENT_MODE = 1,
} OH_AUDIO_audio_InterruptMode;
typedef struct Opt_audio_InterruptMode {
    OH_Tag tag;
    OH_AUDIO_audio_InterruptMode value;
} Opt_audio_InterruptMode;
typedef enum OH_AUDIO_audio_AudioRendererRate {
    OH_AUDIO_AUDIO_AUDIO_RENDERER_RATE_RENDER_RATE_NORMAL = 0,
    OH_AUDIO_AUDIO_AUDIO_RENDERER_RATE_RENDER_RATE_DOUBLE = 1,
    OH_AUDIO_AUDIO_AUDIO_RENDERER_RATE_RENDER_RATE_HALF = 2,
} OH_AUDIO_audio_AudioRendererRate;
typedef struct Opt_audio_AudioRendererRate {
    OH_Tag tag;
    OH_AUDIO_audio_AudioRendererRate value;
} Opt_audio_AudioRendererRate;
typedef enum OH_AUDIO_audio_AudioEffectMode {
    OH_AUDIO_AUDIO_AUDIO_EFFECT_MODE_EFFECT_NONE = 0,
    OH_AUDIO_AUDIO_AUDIO_EFFECT_MODE_EFFECT_DEFAULT = 1,
} OH_AUDIO_audio_AudioEffectMode;
typedef struct Opt_audio_AudioEffectMode {
    OH_Tag tag;
    OH_AUDIO_audio_AudioEffectMode value;
} Opt_audio_AudioEffectMode;
typedef enum OH_AUDIO_audio_AudioStreamDeviceChangeReason {
    OH_AUDIO_AUDIO_AUDIO_STREAM_DEVICE_CHANGE_REASON_REASON_UNKNOWN = 0,
    OH_AUDIO_AUDIO_AUDIO_STREAM_DEVICE_CHANGE_REASON_REASON_NEW_DEVICE_AVAILABLE = 1,
    OH_AUDIO_AUDIO_AUDIO_STREAM_DEVICE_CHANGE_REASON_REASON_OLD_DEVICE_UNAVAILABLE = 2,
    OH_AUDIO_AUDIO_AUDIO_STREAM_DEVICE_CHANGE_REASON_REASON_OVERRODE = 3,
} OH_AUDIO_audio_AudioStreamDeviceChangeReason;
typedef struct Opt_audio_AudioStreamDeviceChangeReason {
    OH_Tag tag;
    OH_AUDIO_audio_AudioStreamDeviceChangeReason value;
} Opt_audio_AudioStreamDeviceChangeReason;
typedef enum OH_AUDIO_audio_ConnectType {
    OH_AUDIO_AUDIO_CONNECT_TYPE_CONNECT_TYPE_LOCAL = 1,
    OH_AUDIO_AUDIO_CONNECT_TYPE_CONNECT_TYPE_DISTRIBUTED = 2,
} OH_AUDIO_audio_ConnectType;
typedef struct Opt_audio_ConnectType {
    OH_Tag tag;
    OH_AUDIO_audio_ConnectType value;
} Opt_audio_ConnectType;
typedef enum OH_AUDIO_audio_AudioSpatializationSceneType {
    OH_AUDIO_AUDIO_AUDIO_SPATIALIZATION_SCENE_TYPE_DEFAULT = 0,
    OH_AUDIO_AUDIO_AUDIO_SPATIALIZATION_SCENE_TYPE_MUSIC = 1,
    OH_AUDIO_AUDIO_AUDIO_SPATIALIZATION_SCENE_TYPE_MOVIE = 2,
    OH_AUDIO_AUDIO_AUDIO_SPATIALIZATION_SCENE_TYPE_AUDIOBOOK = 3,
} OH_AUDIO_audio_AudioSpatializationSceneType;
typedef struct Opt_audio_AudioSpatializationSceneType {
    OH_Tag tag;
    OH_AUDIO_audio_AudioSpatializationSceneType value;
} Opt_audio_AudioSpatializationSceneType;
typedef enum OH_AUDIO_audio_AudioVolumeType {
    OH_AUDIO_AUDIO_AUDIO_VOLUME_TYPE_VOICE_CALL = 0,
    OH_AUDIO_AUDIO_AUDIO_VOLUME_TYPE_RINGTONE = 2,
    OH_AUDIO_AUDIO_AUDIO_VOLUME_TYPE_MEDIA = 3,
    OH_AUDIO_AUDIO_AUDIO_VOLUME_TYPE_ALARM = 4,
    OH_AUDIO_AUDIO_AUDIO_VOLUME_TYPE_ACCESSIBILITY = 5,
    OH_AUDIO_AUDIO_AUDIO_VOLUME_TYPE_VOICE_ASSISTANT = 9,
    OH_AUDIO_AUDIO_AUDIO_VOLUME_TYPE_ULTRASONIC = 10,
    OH_AUDIO_AUDIO_AUDIO_VOLUME_TYPE_ALL = 100,
} OH_AUDIO_audio_AudioVolumeType;
typedef struct Opt_audio_AudioVolumeType {
    OH_Tag tag;
    OH_AUDIO_audio_AudioVolumeType value;
} Opt_audio_AudioVolumeType;
typedef enum OH_AUDIO_audio_VolumeAdjustType {
    OH_AUDIO_AUDIO_VOLUME_ADJUST_TYPE_VOLUME_UP = 0,
    OH_AUDIO_AUDIO_VOLUME_ADJUST_TYPE_VOLUME_DOWN = 1,
} OH_AUDIO_audio_VolumeAdjustType;
typedef struct Opt_audio_VolumeAdjustType {
    OH_Tag tag;
    OH_AUDIO_audio_VolumeAdjustType value;
} Opt_audio_VolumeAdjustType;
typedef enum OH_AUDIO_audio_PolicyType {
    OH_AUDIO_AUDIO_POLICY_TYPE_EDM = 0,
    OH_AUDIO_AUDIO_POLICY_TYPE_PRIVACY = 1,
} OH_AUDIO_audio_PolicyType;
typedef struct Opt_audio_PolicyType {
    OH_Tag tag;
    OH_AUDIO_audio_PolicyType value;
} Opt_audio_PolicyType;
typedef enum OH_AUDIO_audio_AudioRingMode {
    OH_AUDIO_AUDIO_AUDIO_RING_MODE_RINGER_MODE_SILENT = 0,
    OH_AUDIO_AUDIO_AUDIO_RING_MODE_RINGER_MODE_VIBRATE = 1,
    OH_AUDIO_AUDIO_AUDIO_RING_MODE_RINGER_MODE_NORMAL = 2,
} OH_AUDIO_audio_AudioRingMode;
typedef struct Opt_audio_AudioRingMode {
    OH_Tag tag;
    OH_AUDIO_audio_AudioRingMode value;
} Opt_audio_AudioRingMode;
typedef enum OH_AUDIO_audio_AudioSessionDeactivatedReason {
    OH_AUDIO_AUDIO_AUDIO_SESSION_DEACTIVATED_REASON_DEACTIVATED_LOWER_PRIORITY = 0,
    OH_AUDIO_AUDIO_AUDIO_SESSION_DEACTIVATED_REASON_DEACTIVATED_TIMEOUT = 1,
} OH_AUDIO_audio_AudioSessionDeactivatedReason;
typedef struct Opt_audio_AudioSessionDeactivatedReason {
    OH_Tag tag;
    OH_AUDIO_audio_AudioSessionDeactivatedReason value;
} Opt_audio_AudioSessionDeactivatedReason;
typedef enum OH_AUDIO_audio_CommunicationDeviceType {
    OH_AUDIO_AUDIO_COMMUNICATION_DEVICE_TYPE_SPEAKER = 2,
} OH_AUDIO_audio_CommunicationDeviceType;
typedef struct Opt_audio_CommunicationDeviceType {
    OH_Tag tag;
    OH_AUDIO_audio_CommunicationDeviceType value;
} Opt_audio_CommunicationDeviceType;
typedef enum OH_AUDIO_audio_DeviceUsage {
    OH_AUDIO_AUDIO_DEVICE_USAGE_MEDIA_OUTPUT_DEVICES = 1,
    OH_AUDIO_AUDIO_DEVICE_USAGE_MEDIA_INPUT_DEVICES = 2,
    OH_AUDIO_AUDIO_DEVICE_USAGE_ALL_MEDIA_DEVICES = 3,
    OH_AUDIO_AUDIO_DEVICE_USAGE_CALL_OUTPUT_DEVICES = 4,
    OH_AUDIO_AUDIO_DEVICE_USAGE_CALL_INPUT_DEVICES = 8,
    OH_AUDIO_AUDIO_DEVICE_USAGE_ALL_CALL_DEVICES = 12,
} OH_AUDIO_audio_DeviceUsage;
typedef struct Opt_audio_DeviceUsage {
    OH_Tag tag;
    OH_AUDIO_audio_DeviceUsage value;
} Opt_audio_DeviceUsage;
typedef enum OH_AUDIO_audio_DeviceFlag {
    OH_AUDIO_AUDIO_DEVICE_FLAG_NONE_DEVICES_FLAG = 0,
    OH_AUDIO_AUDIO_DEVICE_FLAG_OUTPUT_DEVICES_FLAG = 1,
    OH_AUDIO_AUDIO_DEVICE_FLAG_INPUT_DEVICES_FLAG = 2,
    OH_AUDIO_AUDIO_DEVICE_FLAG_ALL_DEVICES_FLAG = 3,
    OH_AUDIO_AUDIO_DEVICE_FLAG_DISTRIBUTED_OUTPUT_DEVICES_FLAG = 4,
    OH_AUDIO_AUDIO_DEVICE_FLAG_DISTRIBUTED_INPUT_DEVICES_FLAG = 8,
    OH_AUDIO_AUDIO_DEVICE_FLAG_ALL_DISTRIBUTED_DEVICES_FLAG = 12,
} OH_AUDIO_audio_DeviceFlag;
typedef struct Opt_audio_DeviceFlag {
    OH_Tag tag;
    OH_AUDIO_audio_DeviceFlag value;
} Opt_audio_DeviceFlag;
typedef enum OH_AUDIO_audio_DeviceBlockStatus {
    OH_AUDIO_AUDIO_DEVICE_BLOCK_STATUS_UNBLOCKED = 0,
    OH_AUDIO_AUDIO_DEVICE_BLOCK_STATUS_BLOCKED = 1,
} OH_AUDIO_audio_DeviceBlockStatus;
typedef struct Opt_audio_DeviceBlockStatus {
    OH_Tag tag;
    OH_AUDIO_audio_DeviceBlockStatus value;
} Opt_audio_DeviceBlockStatus;
typedef enum OH_AUDIO_audio_InterruptRequestResultType {
    OH_AUDIO_AUDIO_INTERRUPT_REQUEST_RESULT_TYPE_INTERRUPT_REQUEST_GRANT = 0,
    OH_AUDIO_AUDIO_INTERRUPT_REQUEST_RESULT_TYPE_INTERRUPT_REQUEST_REJECT = 1,
} OH_AUDIO_audio_InterruptRequestResultType;
typedef struct Opt_audio_InterruptRequestResultType {
    OH_Tag tag;
    OH_AUDIO_audio_InterruptRequestResultType value;
} Opt_audio_InterruptRequestResultType;
typedef enum OH_AUDIO_audio_InterruptHint {
    OH_AUDIO_AUDIO_INTERRUPT_HINT_INTERRUPT_HINT_NONE = 0,
    OH_AUDIO_AUDIO_INTERRUPT_HINT_INTERRUPT_HINT_RESUME = 1,
    OH_AUDIO_AUDIO_INTERRUPT_HINT_INTERRUPT_HINT_PAUSE = 2,
    OH_AUDIO_AUDIO_INTERRUPT_HINT_INTERRUPT_HINT_STOP = 3,
    OH_AUDIO_AUDIO_INTERRUPT_HINT_INTERRUPT_HINT_DUCK = 4,
    OH_AUDIO_AUDIO_INTERRUPT_HINT_INTERRUPT_HINT_UNDUCK = 5,
} OH_AUDIO_audio_InterruptHint;
typedef struct Opt_audio_InterruptHint {
    OH_Tag tag;
    OH_AUDIO_audio_InterruptHint value;
} Opt_audio_InterruptHint;
typedef enum OH_AUDIO_audio_InterruptType {
    OH_AUDIO_AUDIO_INTERRUPT_TYPE_INTERRUPT_TYPE_BEGIN = 1,
    OH_AUDIO_AUDIO_INTERRUPT_TYPE_INTERRUPT_TYPE_END = 2,
} OH_AUDIO_audio_InterruptType;
typedef struct Opt_audio_InterruptType {
    OH_Tag tag;
    OH_AUDIO_audio_InterruptType value;
} Opt_audio_InterruptType;
typedef enum OH_AUDIO_audio_InterruptActionType {
    OH_AUDIO_AUDIO_INTERRUPT_ACTION_TYPE_TYPE_ACTIVATED = 0,
    OH_AUDIO_AUDIO_INTERRUPT_ACTION_TYPE_TYPE_INTERRUPT = 1,
} OH_AUDIO_audio_InterruptActionType;
typedef struct Opt_audio_InterruptActionType {
    OH_Tag tag;
    OH_AUDIO_audio_InterruptActionType value;
} Opt_audio_InterruptActionType;
typedef enum OH_AUDIO_audio_DeviceChangeType {
    OH_AUDIO_AUDIO_DEVICE_CHANGE_TYPE_CONNECT = 0,
    OH_AUDIO_AUDIO_DEVICE_CHANGE_TYPE_DISCONNECT = 1,
} OH_AUDIO_audio_DeviceChangeType;
typedef struct Opt_audio_DeviceChangeType {
    OH_Tag tag;
    OH_AUDIO_audio_DeviceChangeType value;
} Opt_audio_DeviceChangeType;
typedef enum OH_AUDIO_audio_AudioScene {
    OH_AUDIO_AUDIO_AUDIO_SCENE_AUDIO_SCENE_DEFAULT = 0,
    OH_AUDIO_AUDIO_AUDIO_SCENE_AUDIO_SCENE_RINGING = 1,
    OH_AUDIO_AUDIO_AUDIO_SCENE_AUDIO_SCENE_PHONE_CALL = 2,
    OH_AUDIO_AUDIO_AUDIO_SCENE_AUDIO_SCENE_VOICE_CHAT = 3,
} OH_AUDIO_audio_AudioScene;
typedef struct Opt_audio_AudioScene {
    OH_Tag tag;
    OH_AUDIO_audio_AudioScene value;
} Opt_audio_AudioScene;
typedef enum OH_AUDIO_audio_ActiveDeviceType {
    OH_AUDIO_AUDIO_ACTIVE_DEVICE_TYPE_SPEAKER = 2,
    OH_AUDIO_AUDIO_ACTIVE_DEVICE_TYPE_BLUETOOTH_SCO = 7,
} OH_AUDIO_audio_ActiveDeviceType;
typedef struct Opt_audio_ActiveDeviceType {
    OH_Tag tag;
    OH_AUDIO_audio_ActiveDeviceType value;
} Opt_audio_ActiveDeviceType;
typedef enum OH_AUDIO_audio_InterruptForceType {
    OH_AUDIO_AUDIO_INTERRUPT_FORCE_TYPE_INTERRUPT_FORCE = 0,
    OH_AUDIO_AUDIO_INTERRUPT_FORCE_TYPE_INTERRUPT_SHARE = 1,
} OH_AUDIO_audio_InterruptForceType;
typedef struct Opt_audio_InterruptForceType {
    OH_Tag tag;
    OH_AUDIO_audio_InterruptForceType value;
} Opt_audio_InterruptForceType;
typedef enum OH_AUDIO_audio_AudioPrivacyType {
    OH_AUDIO_AUDIO_AUDIO_PRIVACY_TYPE_PRIVACY_TYPE_PUBLIC = 0,
    OH_AUDIO_AUDIO_AUDIO_PRIVACY_TYPE_PRIVACY_TYPE_PRIVATE = 1,
} OH_AUDIO_audio_AudioPrivacyType;
typedef struct Opt_audio_AudioPrivacyType {
    OH_Tag tag;
    OH_AUDIO_audio_AudioPrivacyType value;
} Opt_audio_AudioPrivacyType;
typedef enum OH_AUDIO_audio_AudioDataCallbackResult {
    OH_AUDIO_AUDIO_AUDIO_DATA_CALLBACK_RESULT_INVALID = -1,
    OH_AUDIO_AUDIO_AUDIO_DATA_CALLBACK_RESULT_VALID = 0,
} OH_AUDIO_audio_AudioDataCallbackResult;
typedef struct Opt_audio_AudioDataCallbackResult {
    OH_Tag tag;
    OH_AUDIO_audio_AudioDataCallbackResult value;
} Opt_audio_AudioDataCallbackResult;
typedef enum OH_AUDIO_audio_VolumeFlag {
    OH_AUDIO_AUDIO_VOLUME_FLAG_FLAG_SHOW_SYSTEM_UI = 1,
} OH_AUDIO_audio_VolumeFlag;
typedef struct Opt_audio_VolumeFlag {
    OH_Tag tag;
    OH_AUDIO_audio_VolumeFlag value;
} Opt_audio_VolumeFlag;
typedef enum OH_AUDIO_audio_InterruptRequestType {
    OH_AUDIO_AUDIO_INTERRUPT_REQUEST_TYPE_INTERRUPT_REQUEST_TYPE_DEFAULT = 0,
} OH_AUDIO_audio_InterruptRequestType;
typedef struct Opt_audio_InterruptRequestType {
    OH_Tag tag;
    OH_AUDIO_audio_InterruptRequestType value;
} Opt_audio_InterruptRequestType;
typedef enum OH_AUDIO_audio_AudioErrors {
    OH_AUDIO_AUDIO_AUDIO_ERRORS_ERROR_INVALID_PARAM = 6800101,
    OH_AUDIO_AUDIO_AUDIO_ERRORS_ERROR_NO_MEMORY = 6800102,
    OH_AUDIO_AUDIO_AUDIO_ERRORS_ERROR_ILLEGAL_STATE = 6800103,
    OH_AUDIO_AUDIO_AUDIO_ERRORS_ERROR_UNSUPPORTED = 6800104,
    OH_AUDIO_AUDIO_AUDIO_ERRORS_ERROR_TIMEOUT = 6800105,
    OH_AUDIO_AUDIO_AUDIO_ERRORS_ERROR_STREAM_LIMIT = 6800201,
    OH_AUDIO_AUDIO_AUDIO_ERRORS_ERROR_SYSTEM = 6800301,
} OH_AUDIO_audio_AudioErrors;
typedef struct Opt_audio_AudioErrors {
    OH_Tag tag;
    OH_AUDIO_audio_AudioErrors value;
} Opt_audio_AudioErrors;
typedef struct Opt_Int32 {
    OH_Tag tag;
    OH_Int32 value;
} Opt_Int32;
typedef struct Array_audio_StreamUsage {
    OH_AUDIO_audio_StreamUsage* array;
    OH_Int32 length;
} Array_audio_StreamUsage;
typedef struct Opt_Array_audio_StreamUsage {
    OH_Tag tag;
    Array_audio_StreamUsage value;
} Opt_Array_audio_StreamUsage;
typedef struct Opt_Number {
    OH_Tag tag;
    OH_Number value;
} Opt_Number;
typedef struct OH_AUDIO_CaptureFilterOptions {
    Array_audio_StreamUsage usages;
} OH_AUDIO_CaptureFilterOptions;
typedef struct Opt_CaptureFilterOptions {
    OH_Tag tag;
    OH_AUDIO_CaptureFilterOptions value;
} Opt_CaptureFilterOptions;
typedef struct Opt_Boolean {
    OH_Tag tag;
    OH_Boolean value;
} Opt_Boolean;
typedef struct Opt_String {
    OH_Tag tag;
    OH_String value;
} Opt_String;
typedef struct Array_audio_AudioEncodingType {
    OH_AUDIO_audio_AudioEncodingType* array;
    OH_Int32 length;
} Array_audio_AudioEncodingType;
typedef struct Opt_Array_audio_AudioEncodingType {
    OH_Tag tag;
    Array_audio_AudioEncodingType value;
} Opt_Array_audio_AudioEncodingType;
typedef struct Array_Number {
    OH_Number* array;
    OH_Int32 length;
} Array_Number;
typedef struct Opt_Array_Number {
    OH_Tag tag;
    Array_Number value;
} Opt_Array_Number;
typedef struct OH_AUDIO_AudioCapturerInfo {
    OH_AUDIO_audio_SourceType source;
    OH_Number capturerFlags;
} OH_AUDIO_AudioCapturerInfo;
typedef struct Opt_AudioCapturerInfo {
    OH_Tag tag;
    OH_AUDIO_AudioCapturerInfo value;
} Opt_AudioCapturerInfo;
typedef struct OH_AUDIO_AudioRendererInfo {
    Opt_audio_ContentType content;
    OH_AUDIO_audio_StreamUsage usage;
    OH_Number rendererFlags;
} OH_AUDIO_AudioRendererInfo;
typedef struct Opt_AudioRendererInfo {
    OH_Tag tag;
    OH_AUDIO_AudioRendererInfo value;
} Opt_AudioRendererInfo;
typedef struct Opt_CustomObject {
    OH_Tag tag;
    OH_CustomObject value;
} Opt_CustomObject;
typedef struct AUDIO_AsyncCallback_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId);
} AUDIO_AsyncCallback_Void;
typedef struct Opt_AUDIO_AsyncCallback_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_Void value;
} Opt_AUDIO_AsyncCallback_Void;
typedef struct AUDIO_Callback_Buffer_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_Buffer parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_Buffer parameter);
} AUDIO_Callback_Buffer_Void;
typedef struct Opt_AUDIO_Callback_Buffer_Void {
    OH_Tag tag;
    AUDIO_Callback_Buffer_Void value;
} Opt_AUDIO_Callback_Buffer_Void;
typedef struct AUDIO_Callback_AudioCapturerChangeInfo_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_AudioCapturerChangeInfo parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_AudioCapturerChangeInfo parameter);
} AUDIO_Callback_AudioCapturerChangeInfo_Void;
typedef struct Opt_AUDIO_Callback_AudioCapturerChangeInfo_Void {
    OH_Tag tag;
    AUDIO_Callback_AudioCapturerChangeInfo_Void value;
} Opt_AUDIO_Callback_AudioCapturerChangeInfo_Void;
typedef struct AUDIO_Callback_AudioDeviceDescriptors_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const Array_CustomObject parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const Array_CustomObject parameter);
} AUDIO_Callback_AudioDeviceDescriptors_Void;
typedef struct Opt_AUDIO_Callback_AudioDeviceDescriptors_Void {
    OH_Tag tag;
    AUDIO_Callback_AudioDeviceDescriptors_Void value;
} Opt_AUDIO_Callback_AudioDeviceDescriptors_Void;
typedef struct AUDIO_Callback_InterruptEvent_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_InterruptEvent parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_InterruptEvent parameter);
} AUDIO_Callback_InterruptEvent_Void;
typedef struct Opt_AUDIO_Callback_InterruptEvent_Void {
    OH_Tag tag;
    AUDIO_Callback_InterruptEvent_Void value;
} Opt_AUDIO_Callback_InterruptEvent_Void;
typedef struct AUDIO_Callback_AudioState_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioState parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, OH_AUDIO_audio_AudioState parameter);
} AUDIO_Callback_AudioState_Void;
typedef struct Opt_AUDIO_Callback_AudioState_Void {
    OH_Tag tag;
    AUDIO_Callback_AudioState_Void value;
} Opt_AUDIO_Callback_AudioState_Void;
typedef struct AUDIO_Callback_Number_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_Number parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_Number parameter);
} AUDIO_Callback_Number_Void;
typedef struct Opt_AUDIO_Callback_Number_Void {
    OH_Tag tag;
    AUDIO_Callback_Number_Void value;
} Opt_AUDIO_Callback_Number_Void;
typedef struct AUDIO_AsyncCallback_Number_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_Number result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_Number result);
} AUDIO_AsyncCallback_Number_Void;
typedef struct Opt_AUDIO_AsyncCallback_Number_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_Number_Void value;
} Opt_AUDIO_AsyncCallback_Number_Void;
typedef struct AUDIO_AsyncCallback_Buffer_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_Buffer result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_Buffer result);
} AUDIO_AsyncCallback_Buffer_Void;
typedef struct Opt_AUDIO_AsyncCallback_Buffer_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_Buffer_Void value;
} Opt_AUDIO_AsyncCallback_Buffer_Void;
typedef struct AUDIO_AsyncCallback_AudioStreamInfo_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_AudioStreamInfo result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_AudioStreamInfo result);
} AUDIO_AsyncCallback_AudioStreamInfo_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioStreamInfo_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_AudioStreamInfo_Void value;
} Opt_AUDIO_AsyncCallback_AudioStreamInfo_Void;
typedef struct AUDIO_AsyncCallback_AudioCapturerInfo_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_AudioCapturerInfo result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_AudioCapturerInfo result);
} AUDIO_AsyncCallback_AudioCapturerInfo_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioCapturerInfo_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_AudioCapturerInfo_Void value;
} Opt_AUDIO_AsyncCallback_AudioCapturerInfo_Void;
typedef struct OH_AUDIO_AudioPlaybackCaptureConfig {
    OH_AUDIO_CaptureFilterOptions filterOptions;
} OH_AUDIO_AudioPlaybackCaptureConfig;
typedef struct Opt_AudioPlaybackCaptureConfig {
    OH_Tag tag;
    OH_AUDIO_AudioPlaybackCaptureConfig value;
} Opt_AudioPlaybackCaptureConfig;
typedef struct OH_AUDIO_AudioStreamInfo {
    OH_AUDIO_audio_AudioSamplingRate samplingRate;
    OH_AUDIO_audio_AudioChannel channels;
    OH_AUDIO_audio_AudioSampleFormat sampleFormat;
    OH_AUDIO_audio_AudioEncodingType encodingType;
    Opt_audio_AudioChannelLayout channelLayout;
} OH_AUDIO_AudioStreamInfo;
typedef struct Opt_AudioStreamInfo {
    OH_Tag tag;
    OH_AUDIO_AudioStreamInfo value;
} Opt_AudioStreamInfo;
typedef struct AUDIO_AudioRendererWriteDataCallback {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_Buffer data, const OH_CustomObject continuation);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_Buffer data, const OH_CustomObject continuation);
} AUDIO_AudioRendererWriteDataCallback;
typedef struct Opt_AUDIO_AudioRendererWriteDataCallback {
    OH_Tag tag;
    AUDIO_AudioRendererWriteDataCallback value;
} Opt_AUDIO_AudioRendererWriteDataCallback;
typedef struct AUDIO_Callback_AudioStreamDeviceChangeInfo_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_AudioStreamDeviceChangeInfo parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_AudioStreamDeviceChangeInfo parameter);
} AUDIO_Callback_AudioStreamDeviceChangeInfo_Void;
typedef struct Opt_AUDIO_Callback_AudioStreamDeviceChangeInfo_Void {
    OH_Tag tag;
    AUDIO_Callback_AudioStreamDeviceChangeInfo_Void value;
} Opt_AUDIO_Callback_AudioStreamDeviceChangeInfo_Void;
typedef struct AUDIO_AsyncCallback_AudioDeviceDescriptors_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const Array_CustomObject result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const Array_CustomObject result);
} AUDIO_AsyncCallback_AudioDeviceDescriptors_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioDeviceDescriptors_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_AudioDeviceDescriptors_Void value;
} Opt_AUDIO_AsyncCallback_AudioDeviceDescriptors_Void;
typedef struct AUDIO_AsyncCallback_AudioRendererRate_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioRendererRate result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, OH_AUDIO_audio_AudioRendererRate result);
} AUDIO_AsyncCallback_AudioRendererRate_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioRendererRate_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_AudioRendererRate_Void value;
} Opt_AUDIO_AsyncCallback_AudioRendererRate_Void;
typedef struct AUDIO_AsyncCallback_AudioEffectMode_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioEffectMode result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, OH_AUDIO_audio_AudioEffectMode result);
} AUDIO_AsyncCallback_AudioEffectMode_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioEffectMode_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_AudioEffectMode_Void value;
} Opt_AUDIO_AsyncCallback_AudioEffectMode_Void;
typedef struct AUDIO_AsyncCallback_AudioRendererInfo_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_AudioRendererInfo result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_AudioRendererInfo result);
} AUDIO_AsyncCallback_AudioRendererInfo_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioRendererInfo_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_AudioRendererInfo_Void value;
} Opt_AUDIO_AsyncCallback_AudioRendererInfo_Void;
typedef struct Array_CustomObject {
    OH_CustomObject* array;
    OH_Int32 length;
} Array_CustomObject;
typedef struct Opt_Array_CustomObject {
    OH_Tag tag;
    Array_CustomObject value;
} Opt_Array_CustomObject;
typedef struct OH_AUDIO_AudioSpatialDeviceState {
    OH_String address;
    OH_Boolean isSpatializationSupported;
    OH_Boolean isHeadTrackingSupported;
    OH_AUDIO_audio_AudioSpatialDeviceType spatialDeviceType;
} OH_AUDIO_AudioSpatialDeviceState;
typedef struct Opt_AudioSpatialDeviceState {
    OH_Tag tag;
    OH_AUDIO_AudioSpatialDeviceState value;
} Opt_AudioSpatialDeviceState;
typedef struct AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_AudioSpatialEnabledStateForDevice parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_AudioSpatialEnabledStateForDevice parameter);
} AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void;
typedef struct Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void {
    OH_Tag tag;
    AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void value;
} Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void;
typedef struct AUDIO_Callback_Boolean_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_Boolean parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_Boolean parameter);
} AUDIO_Callback_Boolean_Void;
typedef struct Opt_AUDIO_Callback_Boolean_Void {
    OH_Tag tag;
    AUDIO_Callback_Boolean_Void value;
} Opt_AUDIO_Callback_Boolean_Void;
typedef struct OH_AUDIO_AudioDeviceDescriptor {
    OH_AUDIO_audio_DeviceRole deviceRole;
    OH_AUDIO_audio_DeviceType deviceType;
    OH_Number id;
    OH_String name;
    OH_String address;
    Array_Number sampleRates;
    Array_Number channelCounts;
    Array_Number channelMasks;
    OH_String networkId;
    OH_Number interruptGroupId;
    OH_Number volumeGroupId;
    OH_String displayName;
    Opt_Array_audio_AudioEncodingType encodingTypes;
} OH_AUDIO_AudioDeviceDescriptor;
typedef struct Opt_AudioDeviceDescriptor {
    OH_Tag tag;
    OH_AUDIO_AudioDeviceDescriptor value;
} Opt_AudioDeviceDescriptor;
typedef struct AUDIO_Callback_MicStateChangeEvent_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_MicStateChangeEvent parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_MicStateChangeEvent parameter);
} AUDIO_Callback_MicStateChangeEvent_Void;
typedef struct Opt_AUDIO_Callback_MicStateChangeEvent_Void {
    OH_Tag tag;
    AUDIO_Callback_MicStateChangeEvent_Void value;
} Opt_AUDIO_Callback_MicStateChangeEvent_Void;
typedef struct AUDIO_AsyncCallback_Boolean_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_Boolean result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_Boolean result);
} AUDIO_AsyncCallback_Boolean_Void;
typedef struct Opt_AUDIO_AsyncCallback_Boolean_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_Boolean_Void value;
} Opt_AUDIO_AsyncCallback_Boolean_Void;
typedef struct AUDIO_Callback_AudioRingMode_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode parameter);
} AUDIO_Callback_AudioRingMode_Void;
typedef struct Opt_AUDIO_Callback_AudioRingMode_Void {
    OH_Tag tag;
    AUDIO_Callback_AudioRingMode_Void value;
} Opt_AUDIO_Callback_AudioRingMode_Void;
typedef struct AUDIO_AsyncCallback_AudioRingMode_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode result);
} AUDIO_AsyncCallback_AudioRingMode_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioRingMode_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_AudioRingMode_Void value;
} Opt_AUDIO_AsyncCallback_AudioRingMode_Void;
typedef struct AUDIO_Callback_VolumeEvent_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_VolumeEvent parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_VolumeEvent parameter);
} AUDIO_Callback_VolumeEvent_Void;
typedef struct Opt_AUDIO_Callback_VolumeEvent_Void {
    OH_Tag tag;
    AUDIO_Callback_VolumeEvent_Void value;
} Opt_AUDIO_Callback_VolumeEvent_Void;
typedef struct AUDIO_AsyncCallback_AudioVolumeGroupManager_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_AudioVolumeGroupManager result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_AudioVolumeGroupManager result);
} AUDIO_AsyncCallback_AudioVolumeGroupManager_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioVolumeGroupManager_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_AudioVolumeGroupManager_Void value;
} Opt_AUDIO_AsyncCallback_AudioVolumeGroupManager_Void;
typedef struct AUDIO_AsyncCallback_VolumeGroupInfos_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const Array_CustomObject result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const Array_CustomObject result);
} AUDIO_AsyncCallback_VolumeGroupInfos_Void;
typedef struct Opt_AUDIO_AsyncCallback_VolumeGroupInfos_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_VolumeGroupInfos_Void value;
} Opt_AUDIO_AsyncCallback_VolumeGroupInfos_Void;
typedef struct AUDIO_Callback_AudioSessionDeactivatedEvent_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_AudioSessionDeactivatedEvent parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_AudioSessionDeactivatedEvent parameter);
} AUDIO_Callback_AudioSessionDeactivatedEvent_Void;
typedef struct Opt_AUDIO_Callback_AudioSessionDeactivatedEvent_Void {
    OH_Tag tag;
    AUDIO_Callback_AudioSessionDeactivatedEvent_Void value;
} Opt_AUDIO_Callback_AudioSessionDeactivatedEvent_Void;
typedef struct OH_AUDIO_AudioSessionStrategy {
    OH_AUDIO_audio_AudioConcurrencyMode concurrencyMode;
} OH_AUDIO_AudioSessionStrategy;
typedef struct Opt_AudioSessionStrategy {
    OH_Tag tag;
    OH_AUDIO_AudioSessionStrategy value;
} Opt_AudioSessionStrategy;
typedef struct AUDIO_Callback_AudioCapturerChangeInfoArray_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const Array_CustomObject parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const Array_CustomObject parameter);
} AUDIO_Callback_AudioCapturerChangeInfoArray_Void;
typedef struct Opt_AUDIO_Callback_AudioCapturerChangeInfoArray_Void {
    OH_Tag tag;
    AUDIO_Callback_AudioCapturerChangeInfoArray_Void value;
} Opt_AUDIO_Callback_AudioCapturerChangeInfoArray_Void;
typedef struct AUDIO_Callback_AudioRendererChangeInfoArray_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const Array_CustomObject parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const Array_CustomObject parameter);
} AUDIO_Callback_AudioRendererChangeInfoArray_Void;
typedef struct Opt_AUDIO_Callback_AudioRendererChangeInfoArray_Void {
    OH_Tag tag;
    AUDIO_Callback_AudioRendererChangeInfoArray_Void value;
} Opt_AUDIO_Callback_AudioRendererChangeInfoArray_Void;
typedef struct AUDIO_AsyncCallback_AudioEffectInfoArray_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const Array_CustomObject result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const Array_CustomObject result);
} AUDIO_AsyncCallback_AudioEffectInfoArray_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioEffectInfoArray_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_AudioEffectInfoArray_Void value;
} Opt_AUDIO_AsyncCallback_AudioEffectInfoArray_Void;
typedef struct AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const Array_CustomObject result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const Array_CustomObject result);
} AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void value;
} Opt_AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void;
typedef struct AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const Array_CustomObject result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const Array_CustomObject result);
} AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void value;
} Opt_AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void;
typedef struct AUDIO_Callback_DeviceBlockStatusInfo_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_DeviceBlockStatusInfo parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_DeviceBlockStatusInfo parameter);
} AUDIO_Callback_DeviceBlockStatusInfo_Void;
typedef struct Opt_AUDIO_Callback_DeviceBlockStatusInfo_Void {
    OH_Tag tag;
    AUDIO_Callback_DeviceBlockStatusInfo_Void value;
} Opt_AUDIO_Callback_DeviceBlockStatusInfo_Void;
typedef struct OH_AUDIO_AudioCapturerFilter {
    Opt_Number uid;
    Opt_AudioCapturerInfo capturerInfo;
} OH_AUDIO_AudioCapturerFilter;
typedef struct Opt_AudioCapturerFilter {
    OH_Tag tag;
    OH_AUDIO_AudioCapturerFilter value;
} Opt_AudioCapturerFilter;
typedef struct OH_AUDIO_AudioRendererFilter {
    Opt_Number uid;
    Opt_AudioRendererInfo rendererInfo;
    Opt_Number rendererId;
} OH_AUDIO_AudioRendererFilter;
typedef struct Opt_AudioRendererFilter {
    OH_Tag tag;
    OH_AUDIO_AudioRendererFilter value;
} Opt_AudioRendererFilter;
typedef struct AUDIO_Callback_DeviceChangeAction_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_DeviceChangeAction parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_DeviceChangeAction parameter);
} AUDIO_Callback_DeviceChangeAction_Void;
typedef struct Opt_AUDIO_Callback_DeviceChangeAction_Void {
    OH_Tag tag;
    AUDIO_Callback_DeviceChangeAction_Void value;
} Opt_AUDIO_Callback_DeviceChangeAction_Void;
typedef struct AUDIO_Callback_InterruptAction_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_InterruptAction parameter);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_InterruptAction parameter);
} AUDIO_Callback_InterruptAction_Void;
typedef struct Opt_AUDIO_Callback_InterruptAction_Void {
    OH_Tag tag;
    AUDIO_Callback_InterruptAction_Void value;
} Opt_AUDIO_Callback_InterruptAction_Void;
typedef struct OH_AUDIO_AudioInterrupt {
    OH_AUDIO_audio_StreamUsage streamUsage;
    OH_AUDIO_audio_ContentType contentType;
    OH_Boolean pauseWhenDucked;
} OH_AUDIO_AudioInterrupt;
typedef struct Opt_AudioInterrupt {
    OH_Tag tag;
    OH_AUDIO_AudioInterrupt value;
} Opt_AudioInterrupt;
typedef struct AUDIO_AsyncCallback_AudioScene_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioScene result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, OH_AUDIO_audio_AudioScene result);
} AUDIO_AsyncCallback_AudioScene_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioScene_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_AudioScene_Void value;
} Opt_AUDIO_AsyncCallback_AudioScene_Void;
typedef struct AUDIO_AsyncCallback_String_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_String result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_String result);
} AUDIO_AsyncCallback_String_Void;
typedef struct Opt_AUDIO_AsyncCallback_String_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_String_Void value;
} Opt_AUDIO_AsyncCallback_String_Void;
typedef struct Opt_TonePlayer {
    OH_Tag tag;
    OH_AUDIO_TonePlayer value;
} Opt_TonePlayer;
typedef struct AUDIO_AsyncCallback_TonePlayer_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_TonePlayer result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_TonePlayer result);
} AUDIO_AsyncCallback_TonePlayer_Void;
typedef struct Opt_AUDIO_AsyncCallback_TonePlayer_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_TonePlayer_Void value;
} Opt_AUDIO_AsyncCallback_TonePlayer_Void;
typedef struct Opt_AudioRenderer {
    OH_Tag tag;
    OH_AUDIO_AudioRenderer value;
} Opt_AudioRenderer;
typedef struct AUDIO_AsyncCallback_AudioRenderer_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_AudioRenderer result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_AudioRenderer result);
} AUDIO_AsyncCallback_AudioRenderer_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioRenderer_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_AudioRenderer_Void value;
} Opt_AUDIO_AsyncCallback_AudioRenderer_Void;
typedef struct Opt_AudioCapturer {
    OH_Tag tag;
    OH_AUDIO_AudioCapturer value;
} Opt_AudioCapturer;
typedef struct AUDIO_AsyncCallback_AudioCapturer_Void {
    OH_AUDIO_CallbackResource resource;
    void (*call)(const OH_Int32 resourceId, const OH_AUDIO_AudioCapturer result);
    void (*callSync)(OH_AUDIO_VMContext context, const OH_Int32 resourceId, const OH_AUDIO_AudioCapturer result);
} AUDIO_AsyncCallback_AudioCapturer_Void;
typedef struct Opt_AUDIO_AsyncCallback_AudioCapturer_Void {
    OH_Tag tag;
    AUDIO_AsyncCallback_AudioCapturer_Void value;
} Opt_AUDIO_AsyncCallback_AudioCapturer_Void;
typedef struct OH_AUDIO_AudioCapturerOptions {
    OH_AUDIO_AudioStreamInfo streamInfo;
    OH_AUDIO_AudioCapturerInfo capturerInfo;
    Opt_AudioPlaybackCaptureConfig playbackCaptureConfig;
} OH_AUDIO_AudioCapturerOptions;
typedef struct Opt_AudioCapturerOptions {
    OH_Tag tag;
    OH_AUDIO_AudioCapturerOptions value;
} Opt_AudioCapturerOptions;
typedef struct Opt_Buffer {
    OH_Tag tag;
    OH_Buffer value;
} Opt_Buffer;
typedef struct OH_AUDIO_AudioStreamDeviceChangeInfo {
    Array_CustomObject devices;
    OH_AUDIO_audio_AudioStreamDeviceChangeReason changeReason;
} OH_AUDIO_AudioStreamDeviceChangeInfo;
typedef struct Opt_AudioStreamDeviceChangeInfo {
    OH_Tag tag;
    OH_AUDIO_AudioStreamDeviceChangeInfo value;
} Opt_AudioStreamDeviceChangeInfo;
typedef struct OH_AUDIO_AudioCapturerChangeInfo {
    OH_Number streamId;
    OH_Number clientUid;
    OH_AUDIO_AudioCapturerInfo capturerInfo;
    OH_AUDIO_audio_AudioState capturerState;
    Array_CustomObject deviceDescriptors;
    Opt_Boolean muted;
} OH_AUDIO_AudioCapturerChangeInfo;
typedef struct Opt_AudioCapturerChangeInfo {
    OH_Tag tag;
    OH_AUDIO_AudioCapturerChangeInfo value;
} Opt_AudioCapturerChangeInfo;
typedef struct OH_AUDIO_AudioRendererChangeInfo {
    OH_Number streamId;
    OH_Number clientUid;
    OH_AUDIO_AudioRendererInfo rendererInfo;
    OH_AUDIO_audio_AudioState rendererState;
    Array_CustomObject deviceDescriptors;
} OH_AUDIO_AudioRendererChangeInfo;
typedef struct Opt_AudioRendererChangeInfo {
    OH_Tag tag;
    OH_AUDIO_AudioRendererChangeInfo value;
} Opt_AudioRendererChangeInfo;
typedef struct OH_AUDIO_VolumeGroupInfo {
    OH_String networkId;
    OH_Number groupId;
    OH_Number mappingId;
    OH_String groupName;
    OH_AUDIO_audio_ConnectType type;
} OH_AUDIO_VolumeGroupInfo;
typedef struct Opt_VolumeGroupInfo {
    OH_Tag tag;
    OH_AUDIO_VolumeGroupInfo value;
} Opt_VolumeGroupInfo;
typedef struct OH_AUDIO_AudioSpatialEnabledStateForDevice {
    OH_AUDIO_AudioDeviceDescriptor deviceDescriptor;
    OH_Boolean enabled;
} OH_AUDIO_AudioSpatialEnabledStateForDevice;
typedef struct Opt_AudioSpatialEnabledStateForDevice {
    OH_Tag tag;
    OH_AUDIO_AudioSpatialEnabledStateForDevice value;
} Opt_AudioSpatialEnabledStateForDevice;
typedef struct OH_AUDIO_MicStateChangeEvent {
    OH_Boolean mute;
} OH_AUDIO_MicStateChangeEvent;
typedef struct Opt_MicStateChangeEvent {
    OH_Tag tag;
    OH_AUDIO_MicStateChangeEvent value;
} Opt_MicStateChangeEvent;
typedef struct Opt_AudioVolumeGroupManager {
    OH_Tag tag;
    OH_AUDIO_AudioVolumeGroupManager value;
} Opt_AudioVolumeGroupManager;
typedef struct OH_AUDIO_AudioSessionDeactivatedEvent {
    OH_AUDIO_audio_AudioSessionDeactivatedReason reason;
} OH_AUDIO_AudioSessionDeactivatedEvent;
typedef struct Opt_AudioSessionDeactivatedEvent {
    OH_Tag tag;
    OH_AUDIO_AudioSessionDeactivatedEvent value;
} Opt_AudioSessionDeactivatedEvent;
typedef struct OH_AUDIO_DeviceBlockStatusInfo {
    OH_AUDIO_audio_DeviceBlockStatus blockStatus;
    Array_CustomObject devices;
} OH_AUDIO_DeviceBlockStatusInfo;
typedef struct Opt_DeviceBlockStatusInfo {
    OH_Tag tag;
    OH_AUDIO_DeviceBlockStatusInfo value;
} Opt_DeviceBlockStatusInfo;
typedef struct OH_AUDIO_InterruptResult {
    OH_AUDIO_audio_InterruptRequestResultType requestResult;
    OH_Number interruptNode;
} OH_AUDIO_InterruptResult;
typedef struct Opt_InterruptResult {
    OH_Tag tag;
    OH_AUDIO_InterruptResult value;
} Opt_InterruptResult;
typedef struct OH_AUDIO_InterruptAction {
    OH_AUDIO_audio_InterruptActionType actionType;
    Opt_audio_InterruptType type;
    Opt_audio_InterruptHint hint;
    Opt_Boolean activated;
} OH_AUDIO_InterruptAction;
typedef struct Opt_InterruptAction {
    OH_Tag tag;
    OH_AUDIO_InterruptAction value;
} Opt_InterruptAction;
typedef struct OH_AUDIO_DeviceChangeAction {
    OH_AUDIO_audio_DeviceChangeType type;
    Array_CustomObject deviceDescriptors;
} OH_AUDIO_DeviceChangeAction;
typedef struct Opt_DeviceChangeAction {
    OH_Tag tag;
    OH_AUDIO_DeviceChangeAction value;
} Opt_DeviceChangeAction;
typedef struct OH_AUDIO_VolumeEvent {
    OH_AUDIO_audio_AudioVolumeType volumeType;
    OH_Number volume;
    OH_Boolean updateUi;
    OH_Number volumeGroupId;
    OH_String networkId;
} OH_AUDIO_VolumeEvent;
typedef struct Opt_VolumeEvent {
    OH_Tag tag;
    OH_AUDIO_VolumeEvent value;
} Opt_VolumeEvent;
typedef struct Array_String {
    OH_String* array;
    OH_Int32 length;
} Array_String;
typedef struct Opt_Array_String {
    OH_Tag tag;
    Array_String value;
} Opt_Array_String;
typedef struct Map_String_String {
    OH_Int32 size;
    OH_String* keys;
    OH_String* values;
} Map_String_String;
typedef struct Opt_Map_String_String {
    OH_Tag tag;
    Map_String_String value;
} Opt_Map_String_String;
typedef struct OH_AUDIO_InterruptEvent {
    OH_AUDIO_audio_InterruptType eventType;
    OH_AUDIO_audio_InterruptForceType forceType;
    OH_AUDIO_audio_InterruptHint hintType;
} OH_AUDIO_InterruptEvent;
typedef struct Opt_InterruptEvent {
    OH_Tag tag;
    OH_AUDIO_InterruptEvent value;
} Opt_InterruptEvent;
typedef struct OH_AUDIO_AudioRendererOptions {
    OH_AUDIO_AudioStreamInfo streamInfo;
    OH_AUDIO_AudioRendererInfo rendererInfo;
    Opt_audio_AudioPrivacyType privacyType;
} OH_AUDIO_AudioRendererOptions;
typedef struct Opt_AudioRendererOptions {
    OH_Tag tag;
    OH_AUDIO_AudioRendererOptions value;
} Opt_AudioRendererOptions;
struct OH_AUDIO_AudioManagerHandleOpaque;
typedef struct OH_AUDIO_AudioManagerHandleOpaque* OH_AUDIO_AudioManagerHandle;
typedef struct OH_AUDIO_AudioManagerModifier {
    OH_AUDIO_AudioManagerHandle (*construct)();
    void (*destruct)(OH_AUDIO_AudioManagerHandle thiz);
    void (*setVolume0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volume, const AUDIO_AsyncCallback_Void* callback_);
    void (*setVolume1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volume);
    void (*getVolume0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*getVolume1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
    void (*getMinVolume0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*getMinVolume1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
    void (*getMaxVolume0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*getMaxVolume1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
    void (*getDevices0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag, const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_);
    OH_NativePointer (*getDevices1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag);
    void (*mute0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Boolean* mute, const AUDIO_AsyncCallback_Void* callback_);
    void (*mute1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Boolean* mute);
    void (*isMute0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Boolean_Void* callback_);
    OH_Boolean (*isMute1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
    void (*isActive0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Boolean_Void* callback_);
    OH_Boolean (*isActive1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
    void (*setMicrophoneMute0)(OH_NativePointer thisPtr, const OH_Boolean* mute, const AUDIO_AsyncCallback_Void* callback_);
    void (*setMicrophoneMute1)(OH_NativePointer thisPtr, const OH_Boolean* mute);
    void (*isMicrophoneMute0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Boolean_Void* callback_);
    OH_Boolean (*isMicrophoneMute1)(OH_NativePointer thisPtr);
    void (*setRingerMode0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRingMode* mode, const AUDIO_AsyncCallback_Void* callback_);
    void (*setRingerMode1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRingMode* mode);
    void (*getRingerMode0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioRingMode_Void* callback_);
    OH_NativePointer (*getRingerMode1)(OH_NativePointer thisPtr);
    void (*setAudioParameter0)(OH_NativePointer thisPtr, const OH_String* key, const OH_String* value, const AUDIO_AsyncCallback_Void* callback_);
    void (*setAudioParameter1)(OH_NativePointer thisPtr, const OH_String* key, const OH_String* value);
    void (*getAudioParameter0)(OH_NativePointer thisPtr, const OH_String* key, const AUDIO_AsyncCallback_String_Void* callback_);
    OH_String (*getAudioParameter1)(OH_NativePointer thisPtr, const OH_String* key);
    void (*setExtraParameters)(OH_NativePointer thisPtr, const OH_String* mainKey, const Map_String_String* kvpairs);
    OH_NativePointer (*getExtraParameters)(OH_NativePointer thisPtr, const OH_String* mainKey, const Opt_Array_String* subKeys);
    void (*setDeviceActive0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_ActiveDeviceType* deviceType, const OH_Boolean* active, const AUDIO_AsyncCallback_Void* callback_);
    void (*setDeviceActive1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_ActiveDeviceType* deviceType, const OH_Boolean* active);
    void (*isDeviceActive0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_ActiveDeviceType* deviceType, const AUDIO_AsyncCallback_Boolean_Void* callback_);
    OH_Boolean (*isDeviceActive1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_ActiveDeviceType* deviceType);
    void (*onVolumeChange)(OH_NativePointer thisPtr, const AUDIO_Callback_VolumeEvent_Void* callback_);
    void (*onRingerModeChange)(OH_NativePointer thisPtr, const AUDIO_Callback_AudioRingMode_Void* callback_);
    void (*setAudioScene0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioScene* scene, const AUDIO_AsyncCallback_Void* callback_);
    void (*setAudioScene1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioScene* scene);
    void (*getAudioScene0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioScene_Void* callback_);
    OH_NativePointer (*getAudioScene1)(OH_NativePointer thisPtr);
    OH_NativePointer (*getAudioSceneSync)(OH_NativePointer thisPtr);
    void (*onDeviceChange)(OH_NativePointer thisPtr, const AUDIO_Callback_DeviceChangeAction_Void* callback_);
    void (*offDeviceChange)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_DeviceChangeAction_Void* callback_);
    void (*onInterrupt)(OH_NativePointer thisPtr, const OH_AUDIO_AudioInterrupt* interrupt, const AUDIO_Callback_InterruptAction_Void* callback_);
    void (*offInterrupt)(OH_NativePointer thisPtr, const OH_AUDIO_AudioInterrupt* interrupt, const Opt_AUDIO_Callback_InterruptAction_Void* callback_);
    OH_NativePointer (*getVolumeManager)(OH_NativePointer thisPtr);
    OH_NativePointer (*getStreamManager)(OH_NativePointer thisPtr);
    OH_NativePointer (*getRoutingManager)(OH_NativePointer thisPtr);
    OH_NativePointer (*getSessionManager)(OH_NativePointer thisPtr);
    OH_NativePointer (*getSpatializationManager)(OH_NativePointer thisPtr);
    void (*disableSafeMediaVolume)(OH_NativePointer thisPtr);
} OH_AUDIO_AudioManagerModifier;
struct OH_AUDIO_AudioRoutingManagerHandleOpaque;
typedef struct OH_AUDIO_AudioRoutingManagerHandleOpaque* OH_AUDIO_AudioRoutingManagerHandle;
typedef struct OH_AUDIO_AudioRoutingManagerModifier {
    OH_AUDIO_AudioRoutingManagerHandle (*construct)();
    void (*destruct)(OH_AUDIO_AudioRoutingManagerHandle thiz);
    void (*getDevices0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag, const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_);
    OH_NativePointer (*getDevices1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag);
    OH_NativePointer (*getDevicesSync)(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag);
    void (*onDeviceChange)(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag, const AUDIO_Callback_DeviceChangeAction_Void* callback_);
    void (*offDeviceChange)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_DeviceChangeAction_Void* callback_);
    OH_NativePointer (*getAvailableDevices)(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceUsage* deviceUsage);
    void (*onAvailableDeviceChange)(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceUsage* deviceUsage, const AUDIO_Callback_DeviceChangeAction_Void* callback_);
    void (*offAvailableDeviceChange)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_DeviceChangeAction_Void* callback_);
    void (*setCommunicationDevice0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_CommunicationDeviceType* deviceType, const OH_Boolean* active, const AUDIO_AsyncCallback_Void* callback_);
    void (*setCommunicationDevice1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_CommunicationDeviceType* deviceType, const OH_Boolean* active);
    void (*isCommunicationDeviceActive0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_CommunicationDeviceType* deviceType, const AUDIO_AsyncCallback_Boolean_Void* callback_);
    OH_Boolean (*isCommunicationDeviceActive1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_CommunicationDeviceType* deviceType);
    OH_Boolean (*isCommunicationDeviceActiveSync)(OH_NativePointer thisPtr, const OH_AUDIO_audio_CommunicationDeviceType* deviceType);
    void (*selectOutputDevice0)(OH_NativePointer thisPtr, const Array_CustomObject* outputAudioDevices, const AUDIO_AsyncCallback_Void* callback_);
    void (*selectOutputDevice1)(OH_NativePointer thisPtr, const Array_CustomObject* outputAudioDevices);
    void (*selectOutputDeviceByFilter0)(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererFilter* filter, const Array_CustomObject* outputAudioDevices, const AUDIO_AsyncCallback_Void* callback_);
    void (*selectOutputDeviceByFilter1)(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererFilter* filter, const Array_CustomObject* outputAudioDevices);
    void (*selectInputDevice0)(OH_NativePointer thisPtr, const Array_CustomObject* inputAudioDevices, const AUDIO_AsyncCallback_Void* callback_);
    void (*selectInputDevice1)(OH_NativePointer thisPtr, const Array_CustomObject* inputAudioDevices);
    void (*selectInputDeviceByFilter)(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerFilter* filter, const Array_CustomObject* inputAudioDevices);
    void (*getPreferOutputDeviceForRendererInfo0)(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererInfo* rendererInfo, const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_);
    OH_NativePointer (*getPreferOutputDeviceForRendererInfo1)(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererInfo* rendererInfo);
    OH_NativePointer (*getPreferredOutputDeviceForRendererInfoSync)(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererInfo* rendererInfo);
    OH_NativePointer (*getPreferredOutputDeviceByFilter)(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererFilter* filter);
    void (*onPreferOutputDeviceChangeForRendererInfo)(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererInfo* rendererInfo, const AUDIO_Callback_AudioDeviceDescriptors_Void* callback_);
    void (*offPreferOutputDeviceChangeForRendererInfo)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* callback_);
    void (*getPreferredInputDeviceForCapturerInfo0)(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerInfo* capturerInfo, const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_);
    OH_NativePointer (*getPreferredInputDeviceForCapturerInfo1)(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerInfo* capturerInfo);
    OH_NativePointer (*getPreferredInputDeviceByFilter)(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerFilter* filter);
    void (*onPreferredInputDeviceChangeForCapturerInfo)(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerInfo* capturerInfo, const AUDIO_Callback_AudioDeviceDescriptors_Void* callback_);
    void (*offPreferredInputDeviceChangeForCapturerInfo)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* callback_);
    OH_NativePointer (*getPreferredInputDeviceForCapturerInfoSync)(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerInfo* capturerInfo);
    OH_Boolean (*isMicBlockDetectionSupported)(OH_NativePointer thisPtr);
    void (*onMicBlockStatusChanged)(OH_NativePointer thisPtr, const AUDIO_Callback_DeviceBlockStatusInfo_Void* callback_);
    void (*offMicBlockStatusChanged)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_DeviceBlockStatusInfo_Void* callback_);
} OH_AUDIO_AudioRoutingManagerModifier;
struct OH_AUDIO_AudioStreamManagerHandleOpaque;
typedef struct OH_AUDIO_AudioStreamManagerHandleOpaque* OH_AUDIO_AudioStreamManagerHandle;
typedef struct OH_AUDIO_AudioStreamManagerModifier {
    OH_AUDIO_AudioStreamManagerHandle (*construct)();
    void (*destruct)(OH_AUDIO_AudioStreamManagerHandle thiz);
    void (*getCurrentAudioRendererInfoArray0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void* callback_);
    OH_NativePointer (*getCurrentAudioRendererInfoArray1)(OH_NativePointer thisPtr);
    OH_NativePointer (*getCurrentAudioRendererInfoArraySync)(OH_NativePointer thisPtr);
    void (*getCurrentAudioCapturerInfoArray0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void* callback_);
    OH_NativePointer (*getCurrentAudioCapturerInfoArray1)(OH_NativePointer thisPtr);
    OH_NativePointer (*getCurrentAudioCapturerInfoArraySync)(OH_NativePointer thisPtr);
    void (*getAudioEffectInfoArray0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_StreamUsage* usage, const AUDIO_AsyncCallback_AudioEffectInfoArray_Void* callback_);
    OH_NativePointer (*getAudioEffectInfoArray1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_StreamUsage* usage);
    OH_NativePointer (*getAudioEffectInfoArraySync)(OH_NativePointer thisPtr, const OH_AUDIO_audio_StreamUsage* usage);
    void (*onAudioRendererChange)(OH_NativePointer thisPtr, const AUDIO_Callback_AudioRendererChangeInfoArray_Void* callback_);
    void (*offAudioRendererChange)(OH_NativePointer thisPtr);
    void (*onAudioCapturerChange)(OH_NativePointer thisPtr, const AUDIO_Callback_AudioCapturerChangeInfoArray_Void* callback_);
    void (*offAudioCapturerChange)(OH_NativePointer thisPtr);
    void (*isActive0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Boolean_Void* callback_);
    OH_Boolean (*isActive1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
    OH_Boolean (*isActiveSync)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
} OH_AUDIO_AudioStreamManagerModifier;
struct OH_AUDIO_AudioSessionManagerHandleOpaque;
typedef struct OH_AUDIO_AudioSessionManagerHandleOpaque* OH_AUDIO_AudioSessionManagerHandle;
typedef struct OH_AUDIO_AudioSessionManagerModifier {
    OH_AUDIO_AudioSessionManagerHandle (*construct)();
    void (*destruct)(OH_AUDIO_AudioSessionManagerHandle thiz);
    void (*activateAudioSession)(OH_NativePointer thisPtr, const OH_AUDIO_AudioSessionStrategy* strategy);
    void (*deactivateAudioSession)(OH_NativePointer thisPtr);
    OH_Boolean (*isAudioSessionActivated)(OH_NativePointer thisPtr);
    void (*onAudioSessionDeactivated)(OH_NativePointer thisPtr, const AUDIO_Callback_AudioSessionDeactivatedEvent_Void* callback_);
    void (*offAudioSessionDeactivated)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioSessionDeactivatedEvent_Void* callback_);
} OH_AUDIO_AudioSessionManagerModifier;
struct OH_AUDIO_AudioVolumeManagerHandleOpaque;
typedef struct OH_AUDIO_AudioVolumeManagerHandleOpaque* OH_AUDIO_AudioVolumeManagerHandle;
typedef struct OH_AUDIO_AudioVolumeManagerModifier {
    OH_AUDIO_AudioVolumeManagerHandle (*construct)();
    void (*destruct)(OH_AUDIO_AudioVolumeManagerHandle thiz);
    void (*getVolumeGroupInfos0)(OH_NativePointer thisPtr, const OH_String* networkId, const AUDIO_AsyncCallback_VolumeGroupInfos_Void* callback_);
    OH_NativePointer (*getVolumeGroupInfos1)(OH_NativePointer thisPtr, const OH_String* networkId);
    OH_NativePointer (*getVolumeGroupInfosSync)(OH_NativePointer thisPtr, const OH_String* networkId);
    void (*getVolumeGroupManager0)(OH_NativePointer thisPtr, const OH_Number* groupId, const AUDIO_AsyncCallback_AudioVolumeGroupManager_Void* callback_);
    OH_NativePointer (*getVolumeGroupManager1)(OH_NativePointer thisPtr, const OH_Number* groupId);
    OH_NativePointer (*getVolumeGroupManagerSync)(OH_NativePointer thisPtr, const OH_Number* groupId);
    void (*onVolumeChange)(OH_NativePointer thisPtr, const AUDIO_Callback_VolumeEvent_Void* callback_);
    void (*offVolumeChange)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_VolumeEvent_Void* callback_);
} OH_AUDIO_AudioVolumeManagerModifier;
struct OH_AUDIO_AudioVolumeGroupManagerHandleOpaque;
typedef struct OH_AUDIO_AudioVolumeGroupManagerHandleOpaque* OH_AUDIO_AudioVolumeGroupManagerHandle;
typedef struct OH_AUDIO_AudioVolumeGroupManagerModifier {
    OH_AUDIO_AudioVolumeGroupManagerHandle (*construct)();
    void (*destruct)(OH_AUDIO_AudioVolumeGroupManagerHandle thiz);
    void (*setVolume0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volume, const AUDIO_AsyncCallback_Void* callback_);
    void (*setVolume1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volume);
    void (*setVolumeWithFlag)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volume, const OH_Number* flags);
    OH_NativePointer (*getActiveVolumeTypeSync)(OH_NativePointer thisPtr, const OH_Number* uid);
    void (*getVolume0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*getVolume1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
    OH_Number (*getVolumeSync)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
    void (*getMinVolume0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*getMinVolume1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
    OH_Number (*getMinVolumeSync)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
    void (*getMaxVolume0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*getMaxVolume1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
    OH_Number (*getMaxVolumeSync)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
    void (*mute0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Boolean* mute, const AUDIO_AsyncCallback_Void* callback_);
    void (*mute1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Boolean* mute);
    void (*isMute0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Boolean_Void* callback_);
    OH_Boolean (*isMute1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
    OH_Boolean (*isMuteSync)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
    void (*setRingerMode0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRingMode* mode, const AUDIO_AsyncCallback_Void* callback_);
    void (*setRingerMode1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRingMode* mode);
    void (*getRingerMode0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioRingMode_Void* callback_);
    OH_NativePointer (*getRingerMode1)(OH_NativePointer thisPtr);
    OH_NativePointer (*getRingerModeSync)(OH_NativePointer thisPtr);
    void (*onRingerModeChange)(OH_NativePointer thisPtr, const AUDIO_Callback_AudioRingMode_Void* callback_);
    void (*setMicrophoneMute0)(OH_NativePointer thisPtr, const OH_Boolean* mute, const AUDIO_AsyncCallback_Void* callback_);
    void (*setMicrophoneMute1)(OH_NativePointer thisPtr, const OH_Boolean* mute);
    void (*setMicMute)(OH_NativePointer thisPtr, const OH_Boolean* mute);
    void (*setMicMutePersistent)(OH_NativePointer thisPtr, const OH_Boolean* mute, const OH_AUDIO_audio_PolicyType* type);
    OH_Boolean (*isPersistentMicMute)(OH_NativePointer thisPtr);
    void (*isMicrophoneMute0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Boolean_Void* callback_);
    OH_Boolean (*isMicrophoneMute1)(OH_NativePointer thisPtr);
    OH_Boolean (*isMicrophoneMuteSync)(OH_NativePointer thisPtr);
    void (*onMicStateChange)(OH_NativePointer thisPtr, const AUDIO_Callback_MicStateChangeEvent_Void* callback_);
    void (*offMicStateChange)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_MicStateChangeEvent_Void* callback_);
    OH_Boolean (*isVolumeUnadjustable)(OH_NativePointer thisPtr);
    void (*adjustVolumeByStep0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_VolumeAdjustType* adjustType, const AUDIO_AsyncCallback_Void* callback_);
    void (*adjustVolumeByStep1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_VolumeAdjustType* adjustType);
    void (*adjustSystemVolumeByStep0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_AUDIO_audio_VolumeAdjustType* adjustType, const AUDIO_AsyncCallback_Void* callback_);
    void (*adjustSystemVolumeByStep1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_AUDIO_audio_VolumeAdjustType* adjustType);
    void (*getSystemVolumeInDb0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volumeLevel, const OH_AUDIO_audio_DeviceType* device, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*getSystemVolumeInDb1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volumeLevel, const OH_AUDIO_audio_DeviceType* device);
    OH_Number (*getSystemVolumeInDbSync)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volumeLevel, const OH_AUDIO_audio_DeviceType* device);
    OH_Number (*getMaxAmplitudeForInputDevice)(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* inputDevice);
    OH_Number (*getMaxAmplitudeForOutputDevice)(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* outputDevice);
} OH_AUDIO_AudioVolumeGroupManagerModifier;
struct OH_AUDIO_AudioSpatializationManagerHandleOpaque;
typedef struct OH_AUDIO_AudioSpatializationManagerHandleOpaque* OH_AUDIO_AudioSpatializationManagerHandle;
typedef struct OH_AUDIO_AudioSpatializationManagerModifier {
    OH_AUDIO_AudioSpatializationManagerHandle (*construct)();
    void (*destruct)(OH_AUDIO_AudioSpatializationManagerHandle thiz);
    OH_Boolean (*isSpatializationSupported)(OH_NativePointer thisPtr);
    OH_Boolean (*isSpatializationSupportedForDevice)(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor);
    OH_Boolean (*isHeadTrackingSupported)(OH_NativePointer thisPtr);
    OH_Boolean (*isHeadTrackingSupportedForDevice)(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor);
    void (*setSpatializationEnabled0)(OH_NativePointer thisPtr, const OH_Boolean* enable, const AUDIO_AsyncCallback_Void* callback_);
    void (*setSpatializationEnabled1)(OH_NativePointer thisPtr, const OH_Boolean* enable);
    void (*setSpatializationEnabled2)(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor, const OH_Boolean* enabled);
    OH_Boolean (*isSpatializationEnabled0)(OH_NativePointer thisPtr);
    OH_Boolean (*isSpatializationEnabled1)(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor);
    void (*onSpatializationEnabledChange)(OH_NativePointer thisPtr, const AUDIO_Callback_Boolean_Void* callback_);
    void (*onSpatializationEnabledChangeForAnyDevice)(OH_NativePointer thisPtr, const AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* callback_);
    void (*offSpatializationEnabledChange)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_Boolean_Void* callback_);
    void (*offSpatializationEnabledChangeForAnyDevice)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* callback_);
    void (*setHeadTrackingEnabled0)(OH_NativePointer thisPtr, const OH_Boolean* enable, const AUDIO_AsyncCallback_Void* callback_);
    void (*setHeadTrackingEnabled1)(OH_NativePointer thisPtr, const OH_Boolean* enable);
    void (*setHeadTrackingEnabled2)(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor, const OH_Boolean* enabled);
    OH_Boolean (*isHeadTrackingEnabled0)(OH_NativePointer thisPtr);
    OH_Boolean (*isHeadTrackingEnabled1)(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor);
    void (*onHeadTrackingEnabledChange)(OH_NativePointer thisPtr, const AUDIO_Callback_Boolean_Void* callback_);
    void (*onHeadTrackingEnabledChangeForAnyDevice)(OH_NativePointer thisPtr, const AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* callback_);
    void (*offHeadTrackingEnabledChange)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_Boolean_Void* callback_);
    void (*offHeadTrackingEnabledChangeForAnyDevice)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* callback_);
    void (*updateSpatialDeviceState)(OH_NativePointer thisPtr, const OH_AUDIO_AudioSpatialDeviceState* spatialDeviceState);
    void (*setSpatializationSceneType)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioSpatializationSceneType* spatializationSceneType);
    OH_NativePointer (*getSpatializationSceneType)(OH_NativePointer thisPtr);
} OH_AUDIO_AudioSpatializationManagerModifier;
struct OH_AUDIO_AudioRendererHandleOpaque;
typedef struct OH_AUDIO_AudioRendererHandleOpaque* OH_AUDIO_AudioRendererHandle;
typedef struct OH_AUDIO_AudioRendererModifier {
    OH_AUDIO_AudioRendererHandle (*construct)();
    void (*destruct)(OH_AUDIO_AudioRendererHandle thiz);
    void (*getRendererInfo0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioRendererInfo_Void* callback_);
    OH_NativePointer (*getRendererInfo1)(OH_NativePointer thisPtr);
    OH_NativePointer (*getRendererInfoSync)(OH_NativePointer thisPtr);
    void (*getStreamInfo0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioStreamInfo_Void* callback_);
    OH_NativePointer (*getStreamInfo1)(OH_NativePointer thisPtr);
    OH_NativePointer (*getStreamInfoSync)(OH_NativePointer thisPtr);
    void (*getAudioStreamId0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*getAudioStreamId1)(OH_NativePointer thisPtr);
    OH_Number (*getAudioStreamIdSync)(OH_NativePointer thisPtr);
    void (*getAudioEffectMode0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioEffectMode_Void* callback_);
    OH_NativePointer (*getAudioEffectMode1)(OH_NativePointer thisPtr);
    void (*setAudioEffectMode0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioEffectMode* mode, const AUDIO_AsyncCallback_Void* callback_);
    void (*setAudioEffectMode1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioEffectMode* mode);
    void (*start0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
    void (*start1)(OH_NativePointer thisPtr);
    void (*write0)(OH_NativePointer thisPtr, const OH_Buffer* buffer, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*write1)(OH_NativePointer thisPtr, const OH_Buffer* buffer);
    void (*getAudioTime0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*getAudioTime1)(OH_NativePointer thisPtr);
    OH_Number (*getAudioTimeSync)(OH_NativePointer thisPtr);
    void (*drain0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
    void (*drain1)(OH_NativePointer thisPtr);
    void (*flush)(OH_NativePointer thisPtr);
    void (*pause0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
    void (*pause1)(OH_NativePointer thisPtr);
    void (*stop0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
    void (*stop1)(OH_NativePointer thisPtr);
    void (*release0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
    void (*release1)(OH_NativePointer thisPtr);
    void (*getBufferSize0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*getBufferSize1)(OH_NativePointer thisPtr);
    OH_Number (*getBufferSizeSync)(OH_NativePointer thisPtr);
    void (*setRenderRate0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRendererRate* rate, const AUDIO_AsyncCallback_Void* callback_);
    void (*setRenderRate1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRendererRate* rate);
    void (*setSpeed)(OH_NativePointer thisPtr, const OH_Number* speed);
    void (*getRenderRate0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioRendererRate_Void* callback_);
    OH_NativePointer (*getRenderRate1)(OH_NativePointer thisPtr);
    OH_NativePointer (*getRenderRateSync)(OH_NativePointer thisPtr);
    OH_Number (*getSpeed)(OH_NativePointer thisPtr);
    void (*setInterruptMode0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_InterruptMode* mode, const AUDIO_AsyncCallback_Void* callback_);
    void (*setInterruptMode1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_InterruptMode* mode);
    void (*setInterruptModeSync)(OH_NativePointer thisPtr, const OH_AUDIO_audio_InterruptMode* mode);
    void (*setVolume0)(OH_NativePointer thisPtr, const OH_Number* volume, const AUDIO_AsyncCallback_Void* callback_);
    void (*setVolume1)(OH_NativePointer thisPtr, const OH_Number* volume);
    OH_Number (*getVolume)(OH_NativePointer thisPtr);
    void (*setVolumeWithRamp)(OH_NativePointer thisPtr, const OH_Number* volume, const OH_Number* duration);
    void (*getMinStreamVolume0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*getMinStreamVolume1)(OH_NativePointer thisPtr);
    OH_Number (*getMinStreamVolumeSync)(OH_NativePointer thisPtr);
    void (*getMaxStreamVolume0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*getMaxStreamVolume1)(OH_NativePointer thisPtr);
    OH_Number (*getMaxStreamVolumeSync)(OH_NativePointer thisPtr);
    void (*getUnderflowCount0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*getUnderflowCount1)(OH_NativePointer thisPtr);
    OH_Number (*getUnderflowCountSync)(OH_NativePointer thisPtr);
    void (*getCurrentOutputDevices0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_);
    OH_NativePointer (*getCurrentOutputDevices1)(OH_NativePointer thisPtr);
    OH_NativePointer (*getCurrentOutputDevicesSync)(OH_NativePointer thisPtr);
    void (*setChannelBlendMode)(OH_NativePointer thisPtr, const OH_AUDIO_audio_ChannelBlendMode* mode);
    void (*setSilentModeAndMixWithOthers)(OH_NativePointer thisPtr, const OH_Boolean* on);
    OH_Boolean (*getSilentModeAndMixWithOthers)(OH_NativePointer thisPtr);
    void (*setDefaultOutputDevice)(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceType* deviceType);
    void (*onAudioInterrupt)(OH_NativePointer thisPtr, const AUDIO_Callback_InterruptEvent_Void* callback_);
    void (*onMarkReach)(OH_NativePointer thisPtr, const OH_Number* frame, const AUDIO_Callback_Number_Void* callback_);
    void (*offMarkReach)(OH_NativePointer thisPtr);
    void (*onPeriodReach)(OH_NativePointer thisPtr, const OH_Number* frame, const AUDIO_Callback_Number_Void* callback_);
    void (*offPeriodReach)(OH_NativePointer thisPtr);
    void (*onStateChange)(OH_NativePointer thisPtr, const AUDIO_Callback_AudioState_Void* callback_);
    void (*onOutputDeviceChange)(OH_NativePointer thisPtr, const AUDIO_Callback_AudioDeviceDescriptors_Void* callback_);
    void (*onOutputDeviceChangeWithInfo)(OH_NativePointer thisPtr, const AUDIO_Callback_AudioStreamDeviceChangeInfo_Void* callback_);
    void (*offOutputDeviceChange)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* callback_);
    void (*offOutputDeviceChangeWithInfo)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioStreamDeviceChangeInfo_Void* callback_);
    void (*onWriteData)(OH_NativePointer thisPtr, const AUDIO_AudioRendererWriteDataCallback* callback_);
    void (*offWriteData)(OH_NativePointer thisPtr, const Opt_AUDIO_AudioRendererWriteDataCallback* callback_);
    OH_NativePointer (*getState)(OH_NativePointer thisPtr);
} OH_AUDIO_AudioRendererModifier;
struct OH_AUDIO_AudioCapturerHandleOpaque;
typedef struct OH_AUDIO_AudioCapturerHandleOpaque* OH_AUDIO_AudioCapturerHandle;
typedef struct OH_AUDIO_AudioCapturerModifier {
    OH_AUDIO_AudioCapturerHandle (*construct)();
    void (*destruct)(OH_AUDIO_AudioCapturerHandle thiz);
    void (*getCapturerInfo0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioCapturerInfo_Void* callback_);
    OH_NativePointer (*getCapturerInfo1)(OH_NativePointer thisPtr);
    OH_NativePointer (*getCapturerInfoSync)(OH_NativePointer thisPtr);
    void (*getStreamInfo0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioStreamInfo_Void* callback_);
    OH_NativePointer (*getStreamInfo1)(OH_NativePointer thisPtr);
    OH_NativePointer (*getStreamInfoSync)(OH_NativePointer thisPtr);
    void (*getAudioStreamId0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*getAudioStreamId1)(OH_NativePointer thisPtr);
    OH_Number (*getAudioStreamIdSync)(OH_NativePointer thisPtr);
    void (*start0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
    void (*start1)(OH_NativePointer thisPtr);
    void (*read0)(OH_NativePointer thisPtr, const OH_Number* size, const OH_Boolean* isBlockingRead, const AUDIO_AsyncCallback_Buffer_Void* callback_);
    OH_Buffer (*read1)(OH_NativePointer thisPtr, const OH_Number* size, const OH_Boolean* isBlockingRead);
    void (*getAudioTime0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*getAudioTime1)(OH_NativePointer thisPtr);
    OH_Number (*getAudioTimeSync)(OH_NativePointer thisPtr);
    void (*stop0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
    void (*stop1)(OH_NativePointer thisPtr);
    void (*release0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
    void (*release1)(OH_NativePointer thisPtr);
    void (*getBufferSize0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
    OH_Number (*getBufferSize1)(OH_NativePointer thisPtr);
    OH_Number (*getBufferSizeSync)(OH_NativePointer thisPtr);
    OH_NativePointer (*getCurrentInputDevices)(OH_NativePointer thisPtr);
    OH_NativePointer (*getCurrentAudioCapturerChangeInfo)(OH_NativePointer thisPtr);
    OH_Number (*getOverflowCount)(OH_NativePointer thisPtr);
    OH_Number (*getOverflowCountSync)(OH_NativePointer thisPtr);
    void (*onMarkReach)(OH_NativePointer thisPtr, const OH_Number* frame, const AUDIO_Callback_Number_Void* callback_);
    void (*offMarkReach)(OH_NativePointer thisPtr);
    void (*onPeriodReach)(OH_NativePointer thisPtr, const OH_Number* frame, const AUDIO_Callback_Number_Void* callback_);
    void (*offPeriodReach)(OH_NativePointer thisPtr);
    void (*onStateChange)(OH_NativePointer thisPtr, const AUDIO_Callback_AudioState_Void* callback_);
    void (*onAudioInterrupt)(OH_NativePointer thisPtr, const AUDIO_Callback_InterruptEvent_Void* callback_);
    void (*offAudioInterrupt)(OH_NativePointer thisPtr);
    void (*onInputDeviceChange)(OH_NativePointer thisPtr, const AUDIO_Callback_AudioDeviceDescriptors_Void* callback_);
    void (*offInputDeviceChange)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* callback_);
    void (*onAudioCapturerChange)(OH_NativePointer thisPtr, const AUDIO_Callback_AudioCapturerChangeInfo_Void* callback_);
    void (*offAudioCapturerChange)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioCapturerChangeInfo_Void* callback_);
    void (*onReadData)(OH_NativePointer thisPtr, const AUDIO_Callback_Buffer_Void* callback_);
    void (*offReadData)(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_Buffer_Void* callback_);
    OH_NativePointer (*getState)(OH_NativePointer thisPtr);
} OH_AUDIO_AudioCapturerModifier;
struct OH_AUDIO_AsrProcessingControllerHandleOpaque;
typedef struct OH_AUDIO_AsrProcessingControllerHandleOpaque* OH_AUDIO_AsrProcessingControllerHandle;
typedef struct OH_AUDIO_AsrProcessingControllerModifier {
    OH_AUDIO_AsrProcessingControllerHandle (*construct)();
    void (*destruct)(OH_AUDIO_AsrProcessingControllerHandle thiz);
    OH_Boolean (*setAsrAecMode)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AsrAecMode* mode);
    OH_NativePointer (*getAsrAecMode)(OH_NativePointer thisPtr);
    OH_Boolean (*setAsrNoiseSuppressionMode)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AsrNoiseSuppressionMode* mode);
    OH_NativePointer (*getAsrNoiseSuppressionMode)(OH_NativePointer thisPtr);
    OH_Boolean (*isWhispering)(OH_NativePointer thisPtr);
    OH_Boolean (*setAsrVoiceControlMode)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AsrVoiceControlMode* mode, const OH_Boolean* enable);
    OH_Boolean (*setAsrVoiceMuteMode)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AsrVoiceMuteMode* mode, const OH_Boolean* enable);
    OH_Boolean (*setAsrWhisperDetectionMode)(OH_NativePointer thisPtr, const OH_AUDIO_audio_AsrWhisperDetectionMode* mode);
    OH_NativePointer (*getAsrWhisperDetectionMode)(OH_NativePointer thisPtr);
} OH_AUDIO_AsrProcessingControllerModifier;
struct OH_AUDIO_TonePlayerHandleOpaque;
typedef struct OH_AUDIO_TonePlayerHandleOpaque* OH_AUDIO_TonePlayerHandle;
typedef struct OH_AUDIO_TonePlayerModifier {
    OH_AUDIO_TonePlayerHandle (*construct)();
    void (*destruct)(OH_AUDIO_TonePlayerHandle thiz);
    void (*load0)(OH_NativePointer thisPtr, const OH_AUDIO_audio_ToneType* type, const AUDIO_AsyncCallback_Void* callback_);
    void (*load1)(OH_NativePointer thisPtr, const OH_AUDIO_audio_ToneType* type);
    void (*start0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
    void (*start1)(OH_NativePointer thisPtr);
    void (*stop0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
    void (*stop1)(OH_NativePointer thisPtr);
    void (*release0)(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
    void (*release1)(OH_NativePointer thisPtr);
} OH_AUDIO_TonePlayerModifier;
struct OH_AUDIO_GlobalScope_ohos_multimedia_audioHandleOpaque;
typedef struct OH_AUDIO_GlobalScope_ohos_multimedia_audioHandleOpaque* OH_AUDIO_GlobalScope_ohos_multimedia_audioHandle;
typedef struct OH_AUDIO_Modifier {
    OH_NativePointer (*getAudioManager)();
    void (*createAudioCapturer0)(const OH_CustomObject* options, const OH_CustomObject* callback_);
    OH_NativePointer (*createAudioCapturer1)(const OH_CustomObject* options);
    void (*createAudioRenderer0)(const OH_CustomObject* options, const OH_CustomObject* callback_);
    OH_NativePointer (*createAudioRenderer1)(const OH_CustomObject* options);
    void (*createTonePlayer0)(const OH_CustomObject* options, const OH_CustomObject* callback_);
    OH_NativePointer (*createTonePlayer1)(const OH_CustomObject* options);
    OH_NativePointer (*createAsrProcessingController)(const OH_CustomObject* audioCapturer);
} OH_AUDIO_Modifier;
typedef struct OH_AUDIO_API {
    OH_Int32 version;
    const OH_AUDIO_AudioManagerModifier* (*AudioManager)();
    const OH_AUDIO_AudioRoutingManagerModifier* (*AudioRoutingManager)();
    const OH_AUDIO_AudioStreamManagerModifier* (*AudioStreamManager)();
    const OH_AUDIO_AudioSessionManagerModifier* (*AudioSessionManager)();
    const OH_AUDIO_AudioVolumeManagerModifier* (*AudioVolumeManager)();
    const OH_AUDIO_AudioVolumeGroupManagerModifier* (*AudioVolumeGroupManager)();
    const OH_AUDIO_AudioSpatializationManagerModifier* (*AudioSpatializationManager)();
    const OH_AUDIO_AudioRendererModifier* (*AudioRenderer)();
    const OH_AUDIO_AudioCapturerModifier* (*AudioCapturer)();
    const OH_AUDIO_AsrProcessingControllerModifier* (*AsrProcessingController)();
    const OH_AUDIO_TonePlayerModifier* (*TonePlayer)();
    const OH_AUDIO_Modifier* (*AUDIO)();
} OH_AUDIO_API;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // OH_AUDIO_H
/* clang-format on */