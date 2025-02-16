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

#include "audio.h"

#define KOALA_INTEROP_MODULE AUDIONativeModule
#include "common-interop.h"
#include "callback-resource.h"
#include "SerializerBase.h"
#include "DeserializerBase.h"
#include <unordered_map>

#if KOALA_USE_PANDA_VM
KOALA_ETS_INTEROP_MODULE_CLASSPATH(KOALA_INTEROP_MODULE, KOALA_QUOTE(ETS_MODULE_CLASSPATH_PREFIX) KOALA_QUOTE(KOALA_INTEROP_MODULE));
#endif

CustomDeserializer * DeserializerBase::customDeserializers = nullptr;

typedef enum CallbackKind {
    Kind_AsyncCallback_AudioCapturer_Void = 103731593,
    Kind_AsyncCallback_AudioCapturerChangeInfoArray_Void = 605900120,
    Kind_AsyncCallback_AudioCapturerInfo_Void = 945856903,
    Kind_AsyncCallback_AudioDeviceDescriptors_Void = 1103131879,
    Kind_AsyncCallback_AudioEffectInfoArray_Void = -1801809985,
    Kind_AsyncCallback_AudioEffectMode_Void = -852719253,
    Kind_AsyncCallback_AudioRenderer_Void = -1795948094,
    Kind_AsyncCallback_AudioRendererChangeInfoArray_Void = 419082675,
    Kind_AsyncCallback_AudioRendererInfo_Void = 721972620,
    Kind_AsyncCallback_AudioRendererRate_Void = 147576480,
    Kind_AsyncCallback_AudioRingMode_Void = -815244914,
    Kind_AsyncCallback_AudioScene_Void = -854078751,
    Kind_AsyncCallback_AudioStreamInfo_Void = -106617875,
    Kind_AsyncCallback_AudioVolumeGroupManager_Void = 540266043,
    Kind_AsyncCallback_Boolean_Void = 46391693,
    Kind_AsyncCallback_Buffer_Void = -1662321143,
    Kind_AsyncCallback_Number_Void = 1959553162,
    Kind_AsyncCallback_String_Void = 789188988,
    Kind_AsyncCallback_TonePlayer_Void = 895924586,
    Kind_AsyncCallback_Void = 1075219926,
    Kind_AsyncCallback_VolumeGroupInfos_Void = -801079837,
    Kind_AudioRendererWriteDataCallback = -1508727875,
    Kind_Callback_AudioCapturerChangeInfo_Void = 300211623,
    Kind_Callback_AudioCapturerChangeInfoArray_Void = -788037890,
    Kind_Callback_AudioDeviceDescriptors_Void = -872298751,
    Kind_Callback_AudioRendererChangeInfoArray_Void = 1609768789,
    Kind_Callback_AudioRingMode_Void = -1700902488,
    Kind_Callback_AudioSessionDeactivatedEvent_Void = 1744071031,
    Kind_Callback_AudioSpatialEnabledStateForDevice_Void = 1057396442,
    Kind_Callback_AudioState_Void = 558001102,
    Kind_Callback_AudioStreamDeviceChangeInfo_Void = 1823460565,
    Kind_Callback_Boolean_Void = 313269291,
    Kind_Callback_Buffer_Void = 908731311,
    Kind_Callback_DeviceBlockStatusInfo_Void = -761620636,
    Kind_Callback_DeviceChangeAction_Void = 892121871,
    Kind_Callback_InterruptAction_Void = -1389231466,
    Kind_Callback_InterruptEvent_Void = 638628164,
    Kind_Callback_MicStateChangeEvent_Void = -1553290571,
    Kind_Callback_Number_Void = 36519084,
    Kind_Callback_VolumeEvent_Void = 311503723,
} CallbackKind;

OH_NativePointer getManagedCallbackCaller(CallbackKind kind);
OH_NativePointer getManagedCallbackCallerSync(CallbackKind kind);

struct Counter {
    int count;
    void* data;
};

static int bufferResourceId = 0;
static std::unordered_map<int, Counter> refCounterMap;

int allocate_buffer(int len, void** mem) {
    char* data = new char[len];
    (*mem) = data;
    int id = ++bufferResourceId;
    refCounterMap[id] = Counter { 1, (void*)data };
    return id;
}

void releaseBuffer(int resourceId) {
    if (refCounterMap.find(resourceId) != refCounterMap.end()) {
        Counter& record = refCounterMap[resourceId];
        --record.count;
        if (record.count <= 0) {
            delete[] (char*)record.data;
        }
    }
}

void holdBuffer(int resourceId) {
    if (refCounterMap.find(resourceId) != refCounterMap.end()) {
        Counter& record = refCounterMap[resourceId];
        ++record.count;
    }
}

void impl_AllocateNativeBuffer(KInt len, KByte* ret, KByte* init) {
    void* mem;
    int resourceId = allocate_buffer(len, &mem);
    memcpy((KByte*)mem, init, len);
    SerializerBase ser { ret, 40 }; // todo check
    ser.writeInt32(resourceId);
    ser.writePointer((void*)&holdBuffer);
    ser.writePointer((void*)&releaseBuffer);
    ser.writePointer(mem);
    ser.writeInt64(len);

}
KOALA_INTEROP_V3(AllocateNativeBuffer, KInt, KByte*, KByte*);
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_Int32& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const Opt_Int32* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_Int32& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Array_audio_StreamUsage& value)
{
    return INTEROP_RUNTIME_OBJECT;
}

template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_StreamUsage value);

inline void WriteToString(std::string* result, const Array_audio_StreamUsage* value) {
    int32_t count = value->length;
    result->append("{.array=allocArray<OH_AUDIO_audio_StreamUsage, " + std::to_string(count) + ">({{");
    for (int i = 0; i < count; i++) {
        if (i > 0) result->append(", ");
        WriteToString(result, value->array[i]);
    }
    result->append("}})");
    result->append(", .length=");
    result->append(std::to_string(value->length));
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_Array_audio_StreamUsage* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_Array_audio_StreamUsage& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_Number& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const Opt_Number* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_Number& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_SourceType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_SourceType value) {
    result->append("OH_AUDIO_audio_SourceType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_SourceType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_SourceType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_StreamUsage& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_StreamUsage value) {
    result->append("OH_AUDIO_audio_StreamUsage(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_StreamUsage* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_StreamUsage& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_ContentType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_ContentType value) {
    result->append("OH_AUDIO_audio_ContentType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_ContentType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_ContentType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_CaptureFilterOptions& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_CaptureFilterOptions* value) {
    result->append("{");
    // Array_audio_StreamUsage usages
    result->append(".usages=");
    WriteToString(result, &value->usages);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_CaptureFilterOptions* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_CaptureFilterOptions& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioChannelLayout& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioChannelLayout value) {
    result->append("OH_AUDIO_audio_AudioChannelLayout(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioChannelLayout* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioChannelLayout& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioEncodingType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioEncodingType value) {
    result->append("OH_AUDIO_audio_AudioEncodingType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioEncodingType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioEncodingType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioSampleFormat& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioSampleFormat value) {
    result->append("OH_AUDIO_audio_AudioSampleFormat(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioSampleFormat* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioSampleFormat& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioChannel& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioChannel value) {
    result->append("OH_AUDIO_audio_AudioChannel(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioChannel* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioChannel& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioSamplingRate& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioSamplingRate value) {
    result->append("OH_AUDIO_audio_AudioSamplingRate(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioSamplingRate* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioSamplingRate& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioSpatialDeviceType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioSpatialDeviceType value) {
    result->append("OH_AUDIO_audio_AudioSpatialDeviceType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioSpatialDeviceType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioSpatialDeviceType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_Boolean& value)
{
    return INTEROP_RUNTIME_BOOLEAN;
}
template <>
inline void WriteToString(std::string* result, const Opt_Boolean* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_Boolean& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_String& value)
{
    return INTEROP_RUNTIME_STRING;
}
template <>
inline void WriteToString(std::string* result, const Opt_String* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_String& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Array_audio_AudioEncodingType& value)
{
    return INTEROP_RUNTIME_OBJECT;
}

template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioEncodingType value);

inline void WriteToString(std::string* result, const Array_audio_AudioEncodingType* value) {
    int32_t count = value->length;
    result->append("{.array=allocArray<OH_AUDIO_audio_AudioEncodingType, " + std::to_string(count) + ">({{");
    for (int i = 0; i < count; i++) {
        if (i > 0) result->append(", ");
        WriteToString(result, value->array[i]);
    }
    result->append("}})");
    result->append(", .length=");
    result->append(std::to_string(value->length));
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_Array_audio_AudioEncodingType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_Array_audio_AudioEncodingType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Array_Number& value)
{
    return INTEROP_RUNTIME_OBJECT;
}

template <>
inline void WriteToString(std::string* result, const OH_Number* value);

inline void WriteToString(std::string* result, const Array_Number* value) {
    int32_t count = value->length;
    result->append("{.array=allocArray<OH_Number, " + std::to_string(count) + ">({{");
    for (int i = 0; i < count; i++) {
        if (i > 0) result->append(", ");
        WriteToString(result, (const OH_Number*)&value->array[i]);
    }
    result->append("}})");
    result->append(", .length=");
    result->append(std::to_string(value->length));
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_Array_Number* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_Array_Number& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_DeviceType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_DeviceType value) {
    result->append("OH_AUDIO_audio_DeviceType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_DeviceType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_DeviceType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_DeviceRole& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_DeviceRole value) {
    result->append("OH_AUDIO_audio_DeviceRole(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_DeviceRole* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_DeviceRole& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioConcurrencyMode& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioConcurrencyMode value) {
    result->append("OH_AUDIO_audio_AudioConcurrencyMode(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioConcurrencyMode* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioConcurrencyMode& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioCapturerInfo& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioCapturerInfo* value) {
    result->append("{");
    // OH_AUDIO_audio_SourceType source
    result->append(".source=");
    WriteToString(result, value->source);
    // OH_Number capturerFlags
    result->append(", ");
    result->append(".capturerFlags=");
    WriteToString(result, &value->capturerFlags);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioCapturerInfo* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioCapturerInfo& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioRendererInfo& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioRendererInfo* value) {
    result->append("{");
    // OH_AUDIO_audio_ContentType content
    result->append(".content=");
    WriteToString(result, &value->content);
    // OH_AUDIO_audio_StreamUsage usage
    result->append(", ");
    result->append(".usage=");
    WriteToString(result, value->usage);
    // OH_Number rendererFlags
    result->append(", ");
    result->append(".rendererFlags=");
    WriteToString(result, &value->rendererFlags);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioRendererInfo* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioRendererInfo& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioState& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioState value) {
    result->append("OH_AUDIO_audio_AudioState(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioState* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioState& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline void WriteToString(std::string* result, const Opt_CustomObject* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_CustomObject& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_ToneType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_ToneType value) {
    result->append("OH_AUDIO_audio_ToneType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_ToneType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_ToneType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AsrWhisperDetectionMode& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AsrWhisperDetectionMode value) {
    result->append("OH_AUDIO_audio_AsrWhisperDetectionMode(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AsrWhisperDetectionMode* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AsrWhisperDetectionMode& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AsrVoiceMuteMode& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AsrVoiceMuteMode value) {
    result->append("OH_AUDIO_audio_AsrVoiceMuteMode(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AsrVoiceMuteMode* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AsrVoiceMuteMode& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AsrVoiceControlMode& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AsrVoiceControlMode value) {
    result->append("OH_AUDIO_audio_AsrVoiceControlMode(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AsrVoiceControlMode* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AsrVoiceControlMode& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AsrNoiseSuppressionMode& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AsrNoiseSuppressionMode value) {
    result->append("OH_AUDIO_audio_AsrNoiseSuppressionMode(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AsrNoiseSuppressionMode* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AsrNoiseSuppressionMode& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AsrAecMode& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AsrAecMode value) {
    result->append("OH_AUDIO_audio_AsrAecMode(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AsrAecMode* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AsrAecMode& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_Buffer_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_Buffer_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_Buffer_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_Buffer_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_AudioCapturerChangeInfo_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_AudioCapturerChangeInfo_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_AudioCapturerChangeInfo_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_AudioCapturerChangeInfo_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_AudioDeviceDescriptors_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_AudioDeviceDescriptors_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_InterruptEvent_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_InterruptEvent_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_InterruptEvent_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_InterruptEvent_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_AudioState_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_AudioState_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_AudioState_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_AudioState_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_Number_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_Number_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_Number_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_Number_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_Number_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_Number_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_Number_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_Number_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_Buffer_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_Buffer_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_Buffer_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_Buffer_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_AudioStreamInfo_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_AudioStreamInfo_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_AudioStreamInfo_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_AudioStreamInfo_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_AudioCapturerInfo_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_AudioCapturerInfo_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_AudioCapturerInfo_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_AudioCapturerInfo_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioPlaybackCaptureConfig& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioPlaybackCaptureConfig* value) {
    result->append("{");
    // OH_AUDIO_CaptureFilterOptions filterOptions
    result->append(".filterOptions=");
    WriteToString(result, &value->filterOptions);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioPlaybackCaptureConfig* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioPlaybackCaptureConfig& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioStreamInfo& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioStreamInfo* value) {
    result->append("{");
    // OH_AUDIO_audio_AudioSamplingRate samplingRate
    result->append(".samplingRate=");
    WriteToString(result, value->samplingRate);
    // OH_AUDIO_audio_AudioChannel channels
    result->append(", ");
    result->append(".channels=");
    WriteToString(result, value->channels);
    // OH_AUDIO_audio_AudioSampleFormat sampleFormat
    result->append(", ");
    result->append(".sampleFormat=");
    WriteToString(result, value->sampleFormat);
    // OH_AUDIO_audio_AudioEncodingType encodingType
    result->append(", ");
    result->append(".encodingType=");
    WriteToString(result, value->encodingType);
    // OH_AUDIO_audio_AudioChannelLayout channelLayout
    result->append(", ");
    result->append(".channelLayout=");
    WriteToString(result, &value->channelLayout);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioStreamInfo* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioStreamInfo& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AudioRendererWriteDataCallback& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AudioRendererWriteDataCallback* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AudioRendererWriteDataCallback* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AudioRendererWriteDataCallback& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_AudioStreamDeviceChangeInfo_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_AudioStreamDeviceChangeInfo_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_AudioStreamDeviceChangeInfo_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_AudioStreamDeviceChangeInfo_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_ChannelBlendMode& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_ChannelBlendMode value) {
    result->append("OH_AUDIO_audio_ChannelBlendMode(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_ChannelBlendMode* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_ChannelBlendMode& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_AudioDeviceDescriptors_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_InterruptMode& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_InterruptMode value) {
    result->append("OH_AUDIO_audio_InterruptMode(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_InterruptMode* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_InterruptMode& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_AudioRendererRate_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_AudioRendererRate_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_AudioRendererRate_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_AudioRendererRate_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioRendererRate& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioRendererRate value) {
    result->append("OH_AUDIO_audio_AudioRendererRate(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioRendererRate* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioRendererRate& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioEffectMode& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioEffectMode value) {
    result->append("OH_AUDIO_audio_AudioEffectMode(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioEffectMode* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioEffectMode& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_AudioEffectMode_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_AudioEffectMode_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_AudioEffectMode_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_AudioEffectMode_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_AudioRendererInfo_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_AudioRendererInfo_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_AudioRendererInfo_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_AudioRendererInfo_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioStreamDeviceChangeReason& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioStreamDeviceChangeReason value) {
    result->append("OH_AUDIO_audio_AudioStreamDeviceChangeReason(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioStreamDeviceChangeReason* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioStreamDeviceChangeReason& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Array_CustomObject& value)
{
    return INTEROP_RUNTIME_OBJECT;
}

template <>
inline void WriteToString(std::string* result, const OH_CustomObject* value);

inline void WriteToString(std::string* result, const Array_CustomObject* value) {
    int32_t count = value->length;
    result->append("{.array=allocArray<OH_CustomObject, " + std::to_string(count) + ">({{");
    for (int i = 0; i < count; i++) {
        if (i > 0) result->append(", ");
        WriteToString(result, (const OH_CustomObject*)&value->array[i]);
    }
    result->append("}})");
    result->append(", .length=");
    result->append(std::to_string(value->length));
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_Array_CustomObject* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_Array_CustomObject& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_ConnectType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_ConnectType value) {
    result->append("OH_AUDIO_audio_ConnectType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_ConnectType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_ConnectType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioSpatializationSceneType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioSpatializationSceneType value) {
    result->append("OH_AUDIO_audio_AudioSpatializationSceneType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioSpatializationSceneType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioSpatializationSceneType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioSpatialDeviceState& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioSpatialDeviceState* value) {
    result->append("{");
    // OH_String address
    result->append(".address=");
    WriteToString(result, &value->address);
    // OH_Boolean isSpatializationSupported
    result->append(", ");
    result->append(".isSpatializationSupported=");
    WriteToString(result, value->isSpatializationSupported);
    // OH_Boolean isHeadTrackingSupported
    result->append(", ");
    result->append(".isHeadTrackingSupported=");
    WriteToString(result, value->isHeadTrackingSupported);
    // OH_AUDIO_audio_AudioSpatialDeviceType spatialDeviceType
    result->append(", ");
    result->append(".spatialDeviceType=");
    WriteToString(result, value->spatialDeviceType);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioSpatialDeviceState* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioSpatialDeviceState& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_Boolean_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_Boolean_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_Boolean_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_Boolean_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioDeviceDescriptor& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioDeviceDescriptor* value) {
    result->append("{");
    // OH_AUDIO_audio_DeviceRole deviceRole
    result->append(".deviceRole=");
    WriteToString(result, value->deviceRole);
    // OH_AUDIO_audio_DeviceType deviceType
    result->append(", ");
    result->append(".deviceType=");
    WriteToString(result, value->deviceType);
    // OH_Number id
    result->append(", ");
    result->append(".id=");
    WriteToString(result, &value->id);
    // OH_String name
    result->append(", ");
    result->append(".name=");
    WriteToString(result, &value->name);
    // OH_String address
    result->append(", ");
    result->append(".address=");
    WriteToString(result, &value->address);
    // Array_Number sampleRates
    result->append(", ");
    result->append(".sampleRates=");
    WriteToString(result, &value->sampleRates);
    // Array_Number channelCounts
    result->append(", ");
    result->append(".channelCounts=");
    WriteToString(result, &value->channelCounts);
    // Array_Number channelMasks
    result->append(", ");
    result->append(".channelMasks=");
    WriteToString(result, &value->channelMasks);
    // OH_String networkId
    result->append(", ");
    result->append(".networkId=");
    WriteToString(result, &value->networkId);
    // OH_Number interruptGroupId
    result->append(", ");
    result->append(".interruptGroupId=");
    WriteToString(result, &value->interruptGroupId);
    // OH_Number volumeGroupId
    result->append(", ");
    result->append(".volumeGroupId=");
    WriteToString(result, &value->volumeGroupId);
    // OH_String displayName
    result->append(", ");
    result->append(".displayName=");
    WriteToString(result, &value->displayName);
    // Array_audio_AudioEncodingType encodingTypes
    result->append(", ");
    result->append(".encodingTypes=");
    WriteToString(result, &value->encodingTypes);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioDeviceDescriptor* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioDeviceDescriptor& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioVolumeType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioVolumeType value) {
    result->append("OH_AUDIO_audio_AudioVolumeType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioVolumeType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioVolumeType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_VolumeAdjustType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_VolumeAdjustType value) {
    result->append("OH_AUDIO_audio_VolumeAdjustType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_VolumeAdjustType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_VolumeAdjustType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_MicStateChangeEvent_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_MicStateChangeEvent_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_MicStateChangeEvent_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_MicStateChangeEvent_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_Boolean_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_Boolean_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_Boolean_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_Boolean_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_PolicyType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_PolicyType value) {
    result->append("OH_AUDIO_audio_PolicyType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_PolicyType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_PolicyType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_AudioRingMode_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_AudioRingMode_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_AudioRingMode_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_AudioRingMode_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_AudioRingMode_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_AudioRingMode_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_AudioRingMode_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_AudioRingMode_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioRingMode& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioRingMode value) {
    result->append("OH_AUDIO_audio_AudioRingMode(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioRingMode* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioRingMode& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_VolumeEvent_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_VolumeEvent_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_VolumeEvent_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_VolumeEvent_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_AudioVolumeGroupManager_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_AudioVolumeGroupManager_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_AudioVolumeGroupManager_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_AudioVolumeGroupManager_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_VolumeGroupInfos_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_VolumeGroupInfos_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_VolumeGroupInfos_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_VolumeGroupInfos_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_AudioSessionDeactivatedEvent_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_AudioSessionDeactivatedEvent_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_AudioSessionDeactivatedEvent_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_AudioSessionDeactivatedEvent_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioSessionStrategy& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioSessionStrategy* value) {
    result->append("{");
    // OH_AUDIO_audio_AudioConcurrencyMode concurrencyMode
    result->append(".concurrencyMode=");
    WriteToString(result, value->concurrencyMode);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioSessionStrategy* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioSessionStrategy& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioSessionDeactivatedReason& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioSessionDeactivatedReason value) {
    result->append("OH_AUDIO_audio_AudioSessionDeactivatedReason(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioSessionDeactivatedReason* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioSessionDeactivatedReason& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_AudioCapturerChangeInfoArray_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_AudioCapturerChangeInfoArray_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_AudioCapturerChangeInfoArray_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_AudioCapturerChangeInfoArray_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_AudioRendererChangeInfoArray_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_AudioRendererChangeInfoArray_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_AudioRendererChangeInfoArray_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_AudioRendererChangeInfoArray_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_AudioEffectInfoArray_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_AudioEffectInfoArray_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_AudioEffectInfoArray_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_AudioEffectInfoArray_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_DeviceBlockStatusInfo_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_DeviceBlockStatusInfo_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_DeviceBlockStatusInfo_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_DeviceBlockStatusInfo_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioCapturerFilter& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioCapturerFilter* value) {
    result->append("{");
    // OH_Number uid
    result->append(".uid=");
    WriteToString(result, &value->uid);
    // OH_AUDIO_AudioCapturerInfo capturerInfo
    result->append(", ");
    result->append(".capturerInfo=");
    WriteToString(result, &value->capturerInfo);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioCapturerFilter* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioCapturerFilter& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioRendererFilter& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioRendererFilter* value) {
    result->append("{");
    // OH_Number uid
    result->append(".uid=");
    WriteToString(result, &value->uid);
    // OH_AUDIO_AudioRendererInfo rendererInfo
    result->append(", ");
    result->append(".rendererInfo=");
    WriteToString(result, &value->rendererInfo);
    // OH_Number rendererId
    result->append(", ");
    result->append(".rendererId=");
    WriteToString(result, &value->rendererId);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioRendererFilter* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioRendererFilter& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_CommunicationDeviceType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_CommunicationDeviceType value) {
    result->append("OH_AUDIO_audio_CommunicationDeviceType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_CommunicationDeviceType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_CommunicationDeviceType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_DeviceChangeAction_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_DeviceChangeAction_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_DeviceChangeAction_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_DeviceChangeAction_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_DeviceUsage& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_DeviceUsage value) {
    result->append("OH_AUDIO_audio_DeviceUsage(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_DeviceUsage* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_DeviceUsage& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_DeviceFlag& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_DeviceFlag value) {
    result->append("OH_AUDIO_audio_DeviceFlag(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_DeviceFlag* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_DeviceFlag& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_DeviceBlockStatus& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_DeviceBlockStatus value) {
    result->append("OH_AUDIO_audio_DeviceBlockStatus(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_DeviceBlockStatus* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_DeviceBlockStatus& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_InterruptRequestResultType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_InterruptRequestResultType value) {
    result->append("OH_AUDIO_audio_InterruptRequestResultType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_InterruptRequestResultType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_InterruptRequestResultType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_Callback_InterruptAction_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_Callback_InterruptAction_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_Callback_InterruptAction_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_Callback_InterruptAction_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioInterrupt& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioInterrupt* value) {
    result->append("{");
    // OH_AUDIO_audio_StreamUsage streamUsage
    result->append(".streamUsage=");
    WriteToString(result, value->streamUsage);
    // OH_AUDIO_audio_ContentType contentType
    result->append(", ");
    result->append(".contentType=");
    WriteToString(result, value->contentType);
    // OH_Boolean pauseWhenDucked
    result->append(", ");
    result->append(".pauseWhenDucked=");
    WriteToString(result, value->pauseWhenDucked);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioInterrupt* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioInterrupt& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_InterruptHint& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_InterruptHint value) {
    result->append("OH_AUDIO_audio_InterruptHint(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_InterruptHint* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_InterruptHint& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_InterruptType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_InterruptType value) {
    result->append("OH_AUDIO_audio_InterruptType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_InterruptType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_InterruptType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_InterruptActionType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_InterruptActionType value) {
    result->append("OH_AUDIO_audio_InterruptActionType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_InterruptActionType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_InterruptActionType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_DeviceChangeType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_DeviceChangeType value) {
    result->append("OH_AUDIO_audio_DeviceChangeType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_DeviceChangeType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_DeviceChangeType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_AudioScene_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_AudioScene_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_AudioScene_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_AudioScene_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioScene& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioScene value) {
    result->append("OH_AUDIO_audio_AudioScene(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioScene* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioScene& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_ActiveDeviceType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_ActiveDeviceType value) {
    result->append("OH_AUDIO_audio_ActiveDeviceType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_ActiveDeviceType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_ActiveDeviceType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_String_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_String_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_String_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_String_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_InterruptForceType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_InterruptForceType value) {
    result->append("OH_AUDIO_audio_InterruptForceType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_InterruptForceType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_InterruptForceType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioPrivacyType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioPrivacyType value) {
    result->append("OH_AUDIO_audio_AudioPrivacyType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioPrivacyType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioPrivacyType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline void WriteToString(std::string* result, const Opt_TonePlayer* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_TonePlayer& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_TonePlayer_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_TonePlayer_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_TonePlayer_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_TonePlayer_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioRenderer* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioRenderer& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_AudioRenderer_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_AudioRenderer_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_AudioRenderer_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_AudioRenderer_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioCapturer* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioCapturer& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const AUDIO_AsyncCallback_AudioCapturer_Void& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const AUDIO_AsyncCallback_AudioCapturer_Void* value) {
    result->append("{");
    result->append(".resource=");
    WriteToString(result, &value->resource);
    result->append(", .call=0");
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AUDIO_AsyncCallback_AudioCapturer_Void* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AUDIO_AsyncCallback_AudioCapturer_Void& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioCapturerOptions& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioCapturerOptions* value) {
    result->append("{");
    // OH_AUDIO_AudioStreamInfo streamInfo
    result->append(".streamInfo=");
    WriteToString(result, &value->streamInfo);
    // OH_AUDIO_AudioCapturerInfo capturerInfo
    result->append(", ");
    result->append(".capturerInfo=");
    WriteToString(result, &value->capturerInfo);
    // OH_AUDIO_AudioPlaybackCaptureConfig playbackCaptureConfig
    result->append(", ");
    result->append(".playbackCaptureConfig=");
    WriteToString(result, &value->playbackCaptureConfig);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioCapturerOptions* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioCapturerOptions& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_Buffer& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const Opt_Buffer* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_Buffer& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioDataCallbackResult& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioDataCallbackResult value) {
    result->append("OH_AUDIO_audio_AudioDataCallbackResult(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioDataCallbackResult* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioDataCallbackResult& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioStreamDeviceChangeInfo& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioStreamDeviceChangeInfo* value) {
    result->append("{");
    // Array_CustomObject devices
    result->append(".devices=");
    WriteToString(result, &value->devices);
    // OH_AUDIO_audio_AudioStreamDeviceChangeReason changeReason
    result->append(", ");
    result->append(".changeReason=");
    WriteToString(result, value->changeReason);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioStreamDeviceChangeInfo* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioStreamDeviceChangeInfo& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioCapturerChangeInfo& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioCapturerChangeInfo* value) {
    result->append("{");
    // OH_Number streamId
    result->append(".streamId=");
    WriteToString(result, &value->streamId);
    // OH_Number clientUid
    result->append(", ");
    result->append(".clientUid=");
    WriteToString(result, &value->clientUid);
    // OH_AUDIO_AudioCapturerInfo capturerInfo
    result->append(", ");
    result->append(".capturerInfo=");
    WriteToString(result, &value->capturerInfo);
    // OH_AUDIO_audio_AudioState capturerState
    result->append(", ");
    result->append(".capturerState=");
    WriteToString(result, value->capturerState);
    // Array_CustomObject deviceDescriptors
    result->append(", ");
    result->append(".deviceDescriptors=");
    WriteToString(result, &value->deviceDescriptors);
    // OH_Boolean muted
    result->append(", ");
    result->append(".muted=");
    WriteToString(result, &value->muted);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioCapturerChangeInfo* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioCapturerChangeInfo& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioRendererChangeInfo& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioRendererChangeInfo* value) {
    result->append("{");
    // OH_Number streamId
    result->append(".streamId=");
    WriteToString(result, &value->streamId);
    // OH_Number clientUid
    result->append(", ");
    result->append(".clientUid=");
    WriteToString(result, &value->clientUid);
    // OH_AUDIO_AudioRendererInfo rendererInfo
    result->append(", ");
    result->append(".rendererInfo=");
    WriteToString(result, &value->rendererInfo);
    // OH_AUDIO_audio_AudioState rendererState
    result->append(", ");
    result->append(".rendererState=");
    WriteToString(result, value->rendererState);
    // Array_CustomObject deviceDescriptors
    result->append(", ");
    result->append(".deviceDescriptors=");
    WriteToString(result, &value->deviceDescriptors);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioRendererChangeInfo* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioRendererChangeInfo& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_VolumeGroupInfo& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_VolumeGroupInfo* value) {
    result->append("{");
    // OH_String networkId
    result->append(".networkId=");
    WriteToString(result, &value->networkId);
    // OH_Number groupId
    result->append(", ");
    result->append(".groupId=");
    WriteToString(result, &value->groupId);
    // OH_Number mappingId
    result->append(", ");
    result->append(".mappingId=");
    WriteToString(result, &value->mappingId);
    // OH_String groupName
    result->append(", ");
    result->append(".groupName=");
    WriteToString(result, &value->groupName);
    // OH_AUDIO_audio_ConnectType type
    result->append(", ");
    result->append(".type=");
    WriteToString(result, value->type);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_VolumeGroupInfo* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_VolumeGroupInfo& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioSpatialEnabledStateForDevice& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioSpatialEnabledStateForDevice* value) {
    result->append("{");
    // OH_AUDIO_AudioDeviceDescriptor deviceDescriptor
    result->append(".deviceDescriptor=");
    WriteToString(result, &value->deviceDescriptor);
    // OH_Boolean enabled
    result->append(", ");
    result->append(".enabled=");
    WriteToString(result, value->enabled);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioSpatialEnabledStateForDevice* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioSpatialEnabledStateForDevice& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_MicStateChangeEvent& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_MicStateChangeEvent* value) {
    result->append("{");
    // OH_Boolean mute
    result->append(".mute=");
    WriteToString(result, value->mute);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_MicStateChangeEvent* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_MicStateChangeEvent& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioVolumeGroupManager* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioVolumeGroupManager& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioSessionDeactivatedEvent& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioSessionDeactivatedEvent* value) {
    result->append("{");
    // OH_AUDIO_audio_AudioSessionDeactivatedReason reason
    result->append(".reason=");
    WriteToString(result, value->reason);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioSessionDeactivatedEvent* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioSessionDeactivatedEvent& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_DeviceBlockStatusInfo& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_DeviceBlockStatusInfo* value) {
    result->append("{");
    // OH_AUDIO_audio_DeviceBlockStatus blockStatus
    result->append(".blockStatus=");
    WriteToString(result, value->blockStatus);
    // Array_CustomObject devices
    result->append(", ");
    result->append(".devices=");
    WriteToString(result, &value->devices);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_DeviceBlockStatusInfo* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_DeviceBlockStatusInfo& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_InterruptResult& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_InterruptResult* value) {
    result->append("{");
    // OH_AUDIO_audio_InterruptRequestResultType requestResult
    result->append(".requestResult=");
    WriteToString(result, value->requestResult);
    // OH_Number interruptNode
    result->append(", ");
    result->append(".interruptNode=");
    WriteToString(result, &value->interruptNode);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_InterruptResult* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_InterruptResult& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_InterruptAction& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_InterruptAction* value) {
    result->append("{");
    // OH_AUDIO_audio_InterruptActionType actionType
    result->append(".actionType=");
    WriteToString(result, value->actionType);
    // OH_AUDIO_audio_InterruptType type
    result->append(", ");
    result->append(".type=");
    WriteToString(result, &value->type);
    // OH_AUDIO_audio_InterruptHint hint
    result->append(", ");
    result->append(".hint=");
    WriteToString(result, &value->hint);
    // OH_Boolean activated
    result->append(", ");
    result->append(".activated=");
    WriteToString(result, &value->activated);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_InterruptAction* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_InterruptAction& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_DeviceChangeAction& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_DeviceChangeAction* value) {
    result->append("{");
    // OH_AUDIO_audio_DeviceChangeType type
    result->append(".type=");
    WriteToString(result, value->type);
    // Array_CustomObject deviceDescriptors
    result->append(", ");
    result->append(".deviceDescriptors=");
    WriteToString(result, &value->deviceDescriptors);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_DeviceChangeAction* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_DeviceChangeAction& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_VolumeEvent& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_VolumeEvent* value) {
    result->append("{");
    // OH_AUDIO_audio_AudioVolumeType volumeType
    result->append(".volumeType=");
    WriteToString(result, value->volumeType);
    // OH_Number volume
    result->append(", ");
    result->append(".volume=");
    WriteToString(result, &value->volume);
    // OH_Boolean updateUi
    result->append(", ");
    result->append(".updateUi=");
    WriteToString(result, value->updateUi);
    // OH_Number volumeGroupId
    result->append(", ");
    result->append(".volumeGroupId=");
    WriteToString(result, &value->volumeGroupId);
    // OH_String networkId
    result->append(", ");
    result->append(".networkId=");
    WriteToString(result, &value->networkId);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_VolumeEvent* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_VolumeEvent& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Array_String& value)
{
    return INTEROP_RUNTIME_OBJECT;
}

template <>
inline void WriteToString(std::string* result, const OH_String* value);

inline void WriteToString(std::string* result, const Array_String* value) {
    int32_t count = value->length;
    result->append("{.array=allocArray<OH_String, " + std::to_string(count) + ">({{");
    for (int i = 0; i < count; i++) {
        if (i > 0) result->append(", ");
        WriteToString(result, (const OH_String*)&value->array[i]);
    }
    result->append("}})");
    result->append(", .length=");
    result->append(std::to_string(value->length));
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_Array_String* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_Array_String& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Map_String_String& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_String* value);
template <>
inline void WriteToString(std::string* result, const OH_String* value);
template <>
inline void WriteToString(std::string* result, const Map_String_String* value) {
    result->append("{");
    int32_t count = value->size;
    for (int i = 0; i < count; i++) {
        if (i > 0) result->append(", ");
        WriteToString(result, (const OH_String*)&value->keys[i]);
        result->append(": ");
        WriteToString(result, (const OH_String*)&value->values[i]);
    }
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_Map_String_String* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_Map_String_String& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_InterruptEvent& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_InterruptEvent* value) {
    result->append("{");
    // OH_AUDIO_audio_InterruptType eventType
    result->append(".eventType=");
    WriteToString(result, value->eventType);
    // OH_AUDIO_audio_InterruptForceType forceType
    result->append(", ");
    result->append(".forceType=");
    WriteToString(result, value->forceType);
    // OH_AUDIO_audio_InterruptHint hintType
    result->append(", ");
    result->append(".hintType=");
    WriteToString(result, value->hintType);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_InterruptEvent* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_InterruptEvent& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_AudioRendererOptions& value)
{
    return INTEROP_RUNTIME_OBJECT;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_AudioRendererOptions* value) {
    result->append("{");
    // OH_AUDIO_AudioStreamInfo streamInfo
    result->append(".streamInfo=");
    WriteToString(result, &value->streamInfo);
    // OH_AUDIO_AudioRendererInfo rendererInfo
    result->append(", ");
    result->append(".rendererInfo=");
    WriteToString(result, &value->rendererInfo);
    // OH_AUDIO_audio_AudioPrivacyType privacyType
    result->append(", ");
    result->append(".privacyType=");
    WriteToString(result, &value->privacyType);
    result->append("}");
}
template <>
inline void WriteToString(std::string* result, const Opt_AudioRendererOptions* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, &value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_AudioRendererOptions& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_VolumeFlag& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_VolumeFlag value) {
    result->append("OH_AUDIO_audio_VolumeFlag(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_VolumeFlag* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_VolumeFlag& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_InterruptRequestType& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_InterruptRequestType value) {
    result->append("OH_AUDIO_audio_InterruptRequestType(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_InterruptRequestType* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_InterruptRequestType& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const OH_AUDIO_audio_AudioErrors& value)
{
    return INTEROP_RUNTIME_NUMBER;
}
template <>
inline void WriteToString(std::string* result, const OH_AUDIO_audio_AudioErrors value) {
    result->append("OH_AUDIO_audio_AudioErrors(");
    WriteToString(result, (OH_Int32) value);
    result->append(")");
}
template <>
inline void WriteToString(std::string* result, const Opt_audio_AudioErrors* value) {
    result->append("{.tag=");
    result->append(tagNameExact((OH_Tag)(value->tag)));
    result->append(", .value=");
    if (value->tag != INTEROP_TAG_UNDEFINED) {
        WriteToString(result, value->value);
    } else {
        OH_Undefined undefined = { 0 };
        WriteToString(result, undefined);
    }
    result->append("}");
}
template <>
inline OH_AUDIO_RuntimeType runtimeType(const Opt_audio_AudioErrors& value)
{
    return (value.tag != INTEROP_TAG_UNDEFINED) ? (INTEROP_RUNTIME_OBJECT) : (INTEROP_RUNTIME_UNDEFINED);
}
class Serializer : public SerializerBase {
    public:
    Serializer(uint8_t* data, OH_UInt32 dataLength = 0, CallbackResourceHolder* resourceHolder = nullptr) : SerializerBase(data, dataLength, resourceHolder) {
    }
    void writeCaptureFilterOptions(OH_AUDIO_CaptureFilterOptions value)
    {
        Serializer& valueSerializer = *this;
        const auto value_usages = value.usages;
        valueSerializer.writeInt32(value_usages.length);
        for (int i = 0; i < value_usages.length; i++) {
            const OH_AUDIO_audio_StreamUsage value_usages_element = value_usages.array[i];
            valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_StreamUsage>(value_usages_element));
        }
    }
    void writeAudioCapturerInfo(OH_AUDIO_AudioCapturerInfo value)
    {
        Serializer& valueSerializer = *this;
        const auto value_source = value.source;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_SourceType>(value_source));
        const auto value_capturerFlags = value.capturerFlags;
        valueSerializer.writeNumber(value_capturerFlags);
    }
    void writeAudioRendererInfo(OH_AUDIO_AudioRendererInfo value)
    {
        Serializer& valueSerializer = *this;
        const auto value_content = value.content;
        OH_Int32 value_content_type = INTEROP_RUNTIME_UNDEFINED;
        value_content_type = runtimeType(value_content);
        valueSerializer.writeInt8(value_content_type);
        if ((INTEROP_RUNTIME_UNDEFINED) != (value_content_type)) {
            const auto value_content_value = value_content.value;
            valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_ContentType>(value_content_value));
        }
        const auto value_usage = value.usage;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_StreamUsage>(value_usage));
        const auto value_rendererFlags = value.rendererFlags;
        valueSerializer.writeNumber(value_rendererFlags);
    }
    void writeAudioPlaybackCaptureConfig(OH_AUDIO_AudioPlaybackCaptureConfig value)
    {
        Serializer& valueSerializer = *this;
        const auto value_filterOptions = value.filterOptions;
        valueSerializer.writeCaptureFilterOptions(value_filterOptions);
    }
    void writeAudioStreamInfo(OH_AUDIO_AudioStreamInfo value)
    {
        Serializer& valueSerializer = *this;
        const auto value_samplingRate = value.samplingRate;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioSamplingRate>(value_samplingRate));
        const auto value_channels = value.channels;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioChannel>(value_channels));
        const auto value_sampleFormat = value.sampleFormat;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioSampleFormat>(value_sampleFormat));
        const auto value_encodingType = value.encodingType;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioEncodingType>(value_encodingType));
        const auto value_channelLayout = value.channelLayout;
        OH_Int32 value_channelLayout_type = INTEROP_RUNTIME_UNDEFINED;
        value_channelLayout_type = runtimeType(value_channelLayout);
        valueSerializer.writeInt8(value_channelLayout_type);
        if ((INTEROP_RUNTIME_UNDEFINED) != (value_channelLayout_type)) {
            const auto value_channelLayout_value = value_channelLayout.value;
            valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioChannelLayout>(value_channelLayout_value));
        }
    }
    void writeAudioSpatialDeviceState(OH_AUDIO_AudioSpatialDeviceState value)
    {
        Serializer& valueSerializer = *this;
        const auto value_address = value.address;
        valueSerializer.writeString(value_address);
        const auto value_isSpatializationSupported = value.isSpatializationSupported;
        valueSerializer.writeBoolean(value_isSpatializationSupported);
        const auto value_isHeadTrackingSupported = value.isHeadTrackingSupported;
        valueSerializer.writeBoolean(value_isHeadTrackingSupported);
        const auto value_spatialDeviceType = value.spatialDeviceType;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioSpatialDeviceType>(value_spatialDeviceType));
    }
    void writeAudioDeviceDescriptor(OH_AUDIO_AudioDeviceDescriptor value)
    {
        Serializer& valueSerializer = *this;
        const auto value_deviceRole = value.deviceRole;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_DeviceRole>(value_deviceRole));
        const auto value_deviceType = value.deviceType;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_DeviceType>(value_deviceType));
        const auto value_id = value.id;
        valueSerializer.writeNumber(value_id);
        const auto value_name = value.name;
        valueSerializer.writeString(value_name);
        const auto value_address = value.address;
        valueSerializer.writeString(value_address);
        const auto value_sampleRates = value.sampleRates;
        valueSerializer.writeInt32(value_sampleRates.length);
        for (int i = 0; i < value_sampleRates.length; i++) {
            const OH_Number value_sampleRates_element = value_sampleRates.array[i];
            valueSerializer.writeNumber(value_sampleRates_element);
        }
        const auto value_channelCounts = value.channelCounts;
        valueSerializer.writeInt32(value_channelCounts.length);
        for (int i = 0; i < value_channelCounts.length; i++) {
            const OH_Number value_channelCounts_element = value_channelCounts.array[i];
            valueSerializer.writeNumber(value_channelCounts_element);
        }
        const auto value_channelMasks = value.channelMasks;
        valueSerializer.writeInt32(value_channelMasks.length);
        for (int i = 0; i < value_channelMasks.length; i++) {
            const OH_Number value_channelMasks_element = value_channelMasks.array[i];
            valueSerializer.writeNumber(value_channelMasks_element);
        }
        const auto value_networkId = value.networkId;
        valueSerializer.writeString(value_networkId);
        const auto value_interruptGroupId = value.interruptGroupId;
        valueSerializer.writeNumber(value_interruptGroupId);
        const auto value_volumeGroupId = value.volumeGroupId;
        valueSerializer.writeNumber(value_volumeGroupId);
        const auto value_displayName = value.displayName;
        valueSerializer.writeString(value_displayName);
        const auto value_encodingTypes = value.encodingTypes;
        OH_Int32 value_encodingTypes_type = INTEROP_RUNTIME_UNDEFINED;
        value_encodingTypes_type = runtimeType(value_encodingTypes);
        valueSerializer.writeInt8(value_encodingTypes_type);
        if ((INTEROP_RUNTIME_UNDEFINED) != (value_encodingTypes_type)) {
            const auto value_encodingTypes_value = value_encodingTypes.value;
            valueSerializer.writeInt32(value_encodingTypes_value.length);
            for (int i = 0; i < value_encodingTypes_value.length; i++) {
                const OH_AUDIO_audio_AudioEncodingType value_encodingTypes_value_element = value_encodingTypes_value.array[i];
                valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioEncodingType>(value_encodingTypes_value_element));
            }
        }
    }
    void writeAudioSessionStrategy(OH_AUDIO_AudioSessionStrategy value)
    {
        Serializer& valueSerializer = *this;
        const auto value_concurrencyMode = value.concurrencyMode;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioConcurrencyMode>(value_concurrencyMode));
    }
    void writeAudioCapturerFilter(OH_AUDIO_AudioCapturerFilter value)
    {
        Serializer& valueSerializer = *this;
        const auto value_uid = value.uid;
        OH_Int32 value_uid_type = INTEROP_RUNTIME_UNDEFINED;
        value_uid_type = runtimeType(value_uid);
        valueSerializer.writeInt8(value_uid_type);
        if ((INTEROP_RUNTIME_UNDEFINED) != (value_uid_type)) {
            const auto value_uid_value = value_uid.value;
            valueSerializer.writeNumber(value_uid_value);
        }
        const auto value_capturerInfo = value.capturerInfo;
        OH_Int32 value_capturerInfo_type = INTEROP_RUNTIME_UNDEFINED;
        value_capturerInfo_type = runtimeType(value_capturerInfo);
        valueSerializer.writeInt8(value_capturerInfo_type);
        if ((INTEROP_RUNTIME_UNDEFINED) != (value_capturerInfo_type)) {
            const auto value_capturerInfo_value = value_capturerInfo.value;
            valueSerializer.writeAudioCapturerInfo(value_capturerInfo_value);
        }
    }
    void writeAudioRendererFilter(OH_AUDIO_AudioRendererFilter value)
    {
        Serializer& valueSerializer = *this;
        const auto value_uid = value.uid;
        OH_Int32 value_uid_type = INTEROP_RUNTIME_UNDEFINED;
        value_uid_type = runtimeType(value_uid);
        valueSerializer.writeInt8(value_uid_type);
        if ((INTEROP_RUNTIME_UNDEFINED) != (value_uid_type)) {
            const auto value_uid_value = value_uid.value;
            valueSerializer.writeNumber(value_uid_value);
        }
        const auto value_rendererInfo = value.rendererInfo;
        OH_Int32 value_rendererInfo_type = INTEROP_RUNTIME_UNDEFINED;
        value_rendererInfo_type = runtimeType(value_rendererInfo);
        valueSerializer.writeInt8(value_rendererInfo_type);
        if ((INTEROP_RUNTIME_UNDEFINED) != (value_rendererInfo_type)) {
            const auto value_rendererInfo_value = value_rendererInfo.value;
            valueSerializer.writeAudioRendererInfo(value_rendererInfo_value);
        }
        const auto value_rendererId = value.rendererId;
        OH_Int32 value_rendererId_type = INTEROP_RUNTIME_UNDEFINED;
        value_rendererId_type = runtimeType(value_rendererId);
        valueSerializer.writeInt8(value_rendererId_type);
        if ((INTEROP_RUNTIME_UNDEFINED) != (value_rendererId_type)) {
            const auto value_rendererId_value = value_rendererId.value;
            valueSerializer.writeNumber(value_rendererId_value);
        }
    }
    void writeAudioInterrupt(OH_AUDIO_AudioInterrupt value)
    {
        Serializer& valueSerializer = *this;
        const auto value_streamUsage = value.streamUsage;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_StreamUsage>(value_streamUsage));
        const auto value_contentType = value.contentType;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_ContentType>(value_contentType));
        const auto value_pauseWhenDucked = value.pauseWhenDucked;
        valueSerializer.writeBoolean(value_pauseWhenDucked);
    }
    void writeTonePlayer(OH_AUDIO_TonePlayer value)
    {
        Serializer& valueSerializer = *this;
        valueSerializer.writePointer(value.ptr);
    }
    void writeAudioRenderer(OH_AUDIO_AudioRenderer value)
    {
        Serializer& valueSerializer = *this;
        valueSerializer.writePointer(value.ptr);
    }
    void writeAudioCapturer(OH_AUDIO_AudioCapturer value)
    {
        Serializer& valueSerializer = *this;
        valueSerializer.writePointer(value.ptr);
    }
    void writeAudioCapturerOptions(OH_AUDIO_AudioCapturerOptions value)
    {
        Serializer& valueSerializer = *this;
        const auto value_streamInfo = value.streamInfo;
        valueSerializer.writeAudioStreamInfo(value_streamInfo);
        const auto value_capturerInfo = value.capturerInfo;
        valueSerializer.writeAudioCapturerInfo(value_capturerInfo);
        const auto value_playbackCaptureConfig = value.playbackCaptureConfig;
        OH_Int32 value_playbackCaptureConfig_type = INTEROP_RUNTIME_UNDEFINED;
        value_playbackCaptureConfig_type = runtimeType(value_playbackCaptureConfig);
        valueSerializer.writeInt8(value_playbackCaptureConfig_type);
        if ((INTEROP_RUNTIME_UNDEFINED) != (value_playbackCaptureConfig_type)) {
            const auto value_playbackCaptureConfig_value = value_playbackCaptureConfig.value;
            valueSerializer.writeAudioPlaybackCaptureConfig(value_playbackCaptureConfig_value);
        }
    }
    void writeAudioStreamDeviceChangeInfo(OH_AUDIO_AudioStreamDeviceChangeInfo value)
    {
        Serializer& valueSerializer = *this;
        const auto value_devices = value.devices;
        valueSerializer.writeInt32(value_devices.length);
        for (int i = 0; i < value_devices.length; i++) {
            const OH_CustomObject value_devices_element = value_devices.array[i];
            valueSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", value_devices_element);
        }
        const auto value_changeReason = value.changeReason;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioStreamDeviceChangeReason>(value_changeReason));
    }
    void writeAudioCapturerChangeInfo(OH_AUDIO_AudioCapturerChangeInfo value)
    {
        Serializer& valueSerializer = *this;
        const auto value_streamId = value.streamId;
        valueSerializer.writeNumber(value_streamId);
        const auto value_clientUid = value.clientUid;
        valueSerializer.writeNumber(value_clientUid);
        const auto value_capturerInfo = value.capturerInfo;
        valueSerializer.writeAudioCapturerInfo(value_capturerInfo);
        const auto value_capturerState = value.capturerState;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioState>(value_capturerState));
        const auto value_deviceDescriptors = value.deviceDescriptors;
        valueSerializer.writeInt32(value_deviceDescriptors.length);
        for (int i = 0; i < value_deviceDescriptors.length; i++) {
            const OH_CustomObject value_deviceDescriptors_element = value_deviceDescriptors.array[i];
            valueSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", value_deviceDescriptors_element);
        }
        const auto value_muted = value.muted;
        OH_Int32 value_muted_type = INTEROP_RUNTIME_UNDEFINED;
        value_muted_type = runtimeType(value_muted);
        valueSerializer.writeInt8(value_muted_type);
        if ((INTEROP_RUNTIME_UNDEFINED) != (value_muted_type)) {
            const auto value_muted_value = value_muted.value;
            valueSerializer.writeBoolean(value_muted_value);
        }
    }
    void writeAudioRendererChangeInfo(OH_AUDIO_AudioRendererChangeInfo value)
    {
        Serializer& valueSerializer = *this;
        const auto value_streamId = value.streamId;
        valueSerializer.writeNumber(value_streamId);
        const auto value_clientUid = value.clientUid;
        valueSerializer.writeNumber(value_clientUid);
        const auto value_rendererInfo = value.rendererInfo;
        valueSerializer.writeAudioRendererInfo(value_rendererInfo);
        const auto value_rendererState = value.rendererState;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioState>(value_rendererState));
        const auto value_deviceDescriptors = value.deviceDescriptors;
        valueSerializer.writeInt32(value_deviceDescriptors.length);
        for (int i = 0; i < value_deviceDescriptors.length; i++) {
            const OH_CustomObject value_deviceDescriptors_element = value_deviceDescriptors.array[i];
            valueSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", value_deviceDescriptors_element);
        }
    }
    void writeVolumeGroupInfo(OH_AUDIO_VolumeGroupInfo value)
    {
        Serializer& valueSerializer = *this;
        const auto value_networkId = value.networkId;
        valueSerializer.writeString(value_networkId);
        const auto value_groupId = value.groupId;
        valueSerializer.writeNumber(value_groupId);
        const auto value_mappingId = value.mappingId;
        valueSerializer.writeNumber(value_mappingId);
        const auto value_groupName = value.groupName;
        valueSerializer.writeString(value_groupName);
        const auto value_type = value.type;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_ConnectType>(value_type));
    }
    void writeAudioSpatialEnabledStateForDevice(OH_AUDIO_AudioSpatialEnabledStateForDevice value)
    {
        Serializer& valueSerializer = *this;
        const auto value_deviceDescriptor = value.deviceDescriptor;
        valueSerializer.writeAudioDeviceDescriptor(value_deviceDescriptor);
        const auto value_enabled = value.enabled;
        valueSerializer.writeBoolean(value_enabled);
    }
    void writeMicStateChangeEvent(OH_AUDIO_MicStateChangeEvent value)
    {
        Serializer& valueSerializer = *this;
        const auto value_mute = value.mute;
        valueSerializer.writeBoolean(value_mute);
    }
    void writeAudioVolumeGroupManager(OH_AUDIO_AudioVolumeGroupManager value)
    {
        Serializer& valueSerializer = *this;
        valueSerializer.writePointer(value.ptr);
    }
    void writeAudioSessionDeactivatedEvent(OH_AUDIO_AudioSessionDeactivatedEvent value)
    {
        Serializer& valueSerializer = *this;
        const auto value_reason = value.reason;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioSessionDeactivatedReason>(value_reason));
    }
    void writeDeviceBlockStatusInfo(OH_AUDIO_DeviceBlockStatusInfo value)
    {
        Serializer& valueSerializer = *this;
        const auto value_blockStatus = value.blockStatus;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_DeviceBlockStatus>(value_blockStatus));
        const auto value_devices = value.devices;
        valueSerializer.writeInt32(value_devices.length);
        for (int i = 0; i < value_devices.length; i++) {
            const OH_CustomObject value_devices_element = value_devices.array[i];
            valueSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", value_devices_element);
        }
    }
    void writeInterruptResult(OH_AUDIO_InterruptResult value)
    {
        Serializer& valueSerializer = *this;
        const auto value_requestResult = value.requestResult;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_InterruptRequestResultType>(value_requestResult));
        const auto value_interruptNode = value.interruptNode;
        valueSerializer.writeNumber(value_interruptNode);
    }
    void writeInterruptAction(OH_AUDIO_InterruptAction value)
    {
        Serializer& valueSerializer = *this;
        const auto value_actionType = value.actionType;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_InterruptActionType>(value_actionType));
        const auto value_type = value.type;
        OH_Int32 value_type_type = INTEROP_RUNTIME_UNDEFINED;
        value_type_type = runtimeType(value_type);
        valueSerializer.writeInt8(value_type_type);
        if ((INTEROP_RUNTIME_UNDEFINED) != (value_type_type)) {
            const auto value_type_value = value_type.value;
            valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_InterruptType>(value_type_value));
        }
        const auto value_hint = value.hint;
        OH_Int32 value_hint_type = INTEROP_RUNTIME_UNDEFINED;
        value_hint_type = runtimeType(value_hint);
        valueSerializer.writeInt8(value_hint_type);
        if ((INTEROP_RUNTIME_UNDEFINED) != (value_hint_type)) {
            const auto value_hint_value = value_hint.value;
            valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_InterruptHint>(value_hint_value));
        }
        const auto value_activated = value.activated;
        OH_Int32 value_activated_type = INTEROP_RUNTIME_UNDEFINED;
        value_activated_type = runtimeType(value_activated);
        valueSerializer.writeInt8(value_activated_type);
        if ((INTEROP_RUNTIME_UNDEFINED) != (value_activated_type)) {
            const auto value_activated_value = value_activated.value;
            valueSerializer.writeBoolean(value_activated_value);
        }
    }
    void writeDeviceChangeAction(OH_AUDIO_DeviceChangeAction value)
    {
        Serializer& valueSerializer = *this;
        const auto value_type = value.type;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_DeviceChangeType>(value_type));
        const auto value_deviceDescriptors = value.deviceDescriptors;
        valueSerializer.writeInt32(value_deviceDescriptors.length);
        for (int i = 0; i < value_deviceDescriptors.length; i++) {
            const OH_CustomObject value_deviceDescriptors_element = value_deviceDescriptors.array[i];
            valueSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", value_deviceDescriptors_element);
        }
    }
    void writeVolumeEvent(OH_AUDIO_VolumeEvent value)
    {
        Serializer& valueSerializer = *this;
        const auto value_volumeType = value.volumeType;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioVolumeType>(value_volumeType));
        const auto value_volume = value.volume;
        valueSerializer.writeNumber(value_volume);
        const auto value_updateUi = value.updateUi;
        valueSerializer.writeBoolean(value_updateUi);
        const auto value_volumeGroupId = value.volumeGroupId;
        valueSerializer.writeNumber(value_volumeGroupId);
        const auto value_networkId = value.networkId;
        valueSerializer.writeString(value_networkId);
    }
    void writeInterruptEvent(OH_AUDIO_InterruptEvent value)
    {
        Serializer& valueSerializer = *this;
        const auto value_eventType = value.eventType;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_InterruptType>(value_eventType));
        const auto value_forceType = value.forceType;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_InterruptForceType>(value_forceType));
        const auto value_hintType = value.hintType;
        valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_InterruptHint>(value_hintType));
    }
    void writeAudioRendererOptions(OH_AUDIO_AudioRendererOptions value)
    {
        Serializer& valueSerializer = *this;
        const auto value_streamInfo = value.streamInfo;
        valueSerializer.writeAudioStreamInfo(value_streamInfo);
        const auto value_rendererInfo = value.rendererInfo;
        valueSerializer.writeAudioRendererInfo(value_rendererInfo);
        const auto value_privacyType = value.privacyType;
        OH_Int32 value_privacyType_type = INTEROP_RUNTIME_UNDEFINED;
        value_privacyType_type = runtimeType(value_privacyType);
        valueSerializer.writeInt8(value_privacyType_type);
        if ((INTEROP_RUNTIME_UNDEFINED) != (value_privacyType_type)) {
            const auto value_privacyType_value = value_privacyType.value;
            valueSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioPrivacyType>(value_privacyType_value));
        }
    }
};

class Deserializer : public DeserializerBase {
    public:
    Deserializer(uint8_t* data, OH_Int32 length) : DeserializerBase(data, length) {
    }
    OH_AUDIO_CaptureFilterOptions readCaptureFilterOptions()
    {
        OH_AUDIO_CaptureFilterOptions value = {};
        Deserializer& valueDeserializer = *this;
        const OH_Int32 usages_buf_length = valueDeserializer.readInt32();
        Array_audio_StreamUsage usages_buf = {};
        valueDeserializer.resizeArray<std::decay<decltype(usages_buf)>::type,
        std::decay<decltype(*usages_buf.array)>::type>(&usages_buf, usages_buf_length);
        for (int usages_buf_i = 0; usages_buf_i < usages_buf_length; usages_buf_i++) {
            usages_buf.array[usages_buf_i] = static_cast<OH_AUDIO_audio_StreamUsage>(valueDeserializer.readInt32());
        }
        value.usages = usages_buf;
        return value;
    }
    OH_AUDIO_AudioCapturerInfo readAudioCapturerInfo()
    {
        OH_AUDIO_AudioCapturerInfo value = {};
        Deserializer& valueDeserializer = *this;
        value.source = static_cast<OH_AUDIO_audio_SourceType>(valueDeserializer.readInt32());
        value.capturerFlags = static_cast<OH_Number>(valueDeserializer.readNumber());
        return value;
    }
    OH_AUDIO_AudioRendererInfo readAudioRendererInfo()
    {
        OH_AUDIO_AudioRendererInfo value = {};
        Deserializer& valueDeserializer = *this;
        const auto content_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(valueDeserializer.readInt8());
        Opt_audio_ContentType content_buf = {};
        content_buf.tag = content_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (content_buf_runtimeType))
        {
            content_buf.value = static_cast<OH_AUDIO_audio_ContentType>(valueDeserializer.readInt32());
        }
        value.content = content_buf;
        value.usage = static_cast<OH_AUDIO_audio_StreamUsage>(valueDeserializer.readInt32());
        value.rendererFlags = static_cast<OH_Number>(valueDeserializer.readNumber());
        return value;
    }
    OH_AUDIO_AudioPlaybackCaptureConfig readAudioPlaybackCaptureConfig()
    {
        OH_AUDIO_AudioPlaybackCaptureConfig value = {};
        Deserializer& valueDeserializer = *this;
        value.filterOptions = valueDeserializer.readCaptureFilterOptions();
        return value;
    }
    OH_AUDIO_AudioStreamInfo readAudioStreamInfo()
    {
        OH_AUDIO_AudioStreamInfo value = {};
        Deserializer& valueDeserializer = *this;
        value.samplingRate = static_cast<OH_AUDIO_audio_AudioSamplingRate>(valueDeserializer.readInt32());
        value.channels = static_cast<OH_AUDIO_audio_AudioChannel>(valueDeserializer.readInt32());
        value.sampleFormat = static_cast<OH_AUDIO_audio_AudioSampleFormat>(valueDeserializer.readInt32());
        value.encodingType = static_cast<OH_AUDIO_audio_AudioEncodingType>(valueDeserializer.readInt32());
        const auto channelLayout_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(valueDeserializer.readInt8());
        Opt_audio_AudioChannelLayout channelLayout_buf = {};
        channelLayout_buf.tag = channelLayout_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (channelLayout_buf_runtimeType))
        {
            channelLayout_buf.value = static_cast<OH_AUDIO_audio_AudioChannelLayout>(valueDeserializer.readInt32());
        }
        value.channelLayout = channelLayout_buf;
        return value;
    }
    OH_AUDIO_AudioSpatialDeviceState readAudioSpatialDeviceState()
    {
        OH_AUDIO_AudioSpatialDeviceState value = {};
        Deserializer& valueDeserializer = *this;
        value.address = static_cast<OH_String>(valueDeserializer.readString());
        value.isSpatializationSupported = valueDeserializer.readBoolean();
        value.isHeadTrackingSupported = valueDeserializer.readBoolean();
        value.spatialDeviceType = static_cast<OH_AUDIO_audio_AudioSpatialDeviceType>(valueDeserializer.readInt32());
        return value;
    }
    OH_AUDIO_AudioDeviceDescriptor readAudioDeviceDescriptor()
    {
        OH_AUDIO_AudioDeviceDescriptor value = {};
        Deserializer& valueDeserializer = *this;
        value.deviceRole = static_cast<OH_AUDIO_audio_DeviceRole>(valueDeserializer.readInt32());
        value.deviceType = static_cast<OH_AUDIO_audio_DeviceType>(valueDeserializer.readInt32());
        value.id = static_cast<OH_Number>(valueDeserializer.readNumber());
        value.name = static_cast<OH_String>(valueDeserializer.readString());
        value.address = static_cast<OH_String>(valueDeserializer.readString());
        const OH_Int32 sampleRates_buf_length = valueDeserializer.readInt32();
        Array_Number sampleRates_buf = {};
        valueDeserializer.resizeArray<std::decay<decltype(sampleRates_buf)>::type,
        std::decay<decltype(*sampleRates_buf.array)>::type>(&sampleRates_buf, sampleRates_buf_length);
        for (int sampleRates_buf_i = 0; sampleRates_buf_i < sampleRates_buf_length; sampleRates_buf_i++) {
            sampleRates_buf.array[sampleRates_buf_i] = static_cast<OH_Number>(valueDeserializer.readNumber());
        }
        value.sampleRates = sampleRates_buf;
        const OH_Int32 channelCounts_buf_length = valueDeserializer.readInt32();
        Array_Number channelCounts_buf = {};
        valueDeserializer.resizeArray<std::decay<decltype(channelCounts_buf)>::type,
        std::decay<decltype(*channelCounts_buf.array)>::type>(&channelCounts_buf, channelCounts_buf_length);
        for (int channelCounts_buf_i = 0; channelCounts_buf_i < channelCounts_buf_length; channelCounts_buf_i++) {
            channelCounts_buf.array[channelCounts_buf_i] = static_cast<OH_Number>(valueDeserializer.readNumber());
        }
        value.channelCounts = channelCounts_buf;
        const OH_Int32 channelMasks_buf_length = valueDeserializer.readInt32();
        Array_Number channelMasks_buf = {};
        valueDeserializer.resizeArray<std::decay<decltype(channelMasks_buf)>::type,
        std::decay<decltype(*channelMasks_buf.array)>::type>(&channelMasks_buf, channelMasks_buf_length);
        for (int channelMasks_buf_i = 0; channelMasks_buf_i < channelMasks_buf_length; channelMasks_buf_i++) {
            channelMasks_buf.array[channelMasks_buf_i] = static_cast<OH_Number>(valueDeserializer.readNumber());
        }
        value.channelMasks = channelMasks_buf;
        value.networkId = static_cast<OH_String>(valueDeserializer.readString());
        value.interruptGroupId = static_cast<OH_Number>(valueDeserializer.readNumber());
        value.volumeGroupId = static_cast<OH_Number>(valueDeserializer.readNumber());
        value.displayName = static_cast<OH_String>(valueDeserializer.readString());
        const auto encodingTypes_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(valueDeserializer.readInt8());
        Opt_Array_audio_AudioEncodingType encodingTypes_buf = {};
        encodingTypes_buf.tag = encodingTypes_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (encodingTypes_buf_runtimeType))
        {
            const OH_Int32 encodingTypes_buf__length = valueDeserializer.readInt32();
            Array_audio_AudioEncodingType encodingTypes_buf_ = {};
            valueDeserializer.resizeArray<std::decay<decltype(encodingTypes_buf_)>::type,
        std::decay<decltype(*encodingTypes_buf_.array)>::type>(&encodingTypes_buf_, encodingTypes_buf__length);
            for (int encodingTypes_buf__i = 0; encodingTypes_buf__i < encodingTypes_buf__length; encodingTypes_buf__i++) {
                encodingTypes_buf_.array[encodingTypes_buf__i] = static_cast<OH_AUDIO_audio_AudioEncodingType>(valueDeserializer.readInt32());
            }
            encodingTypes_buf.value = encodingTypes_buf_;
        }
        value.encodingTypes = encodingTypes_buf;
        return value;
    }
    OH_AUDIO_AudioSessionStrategy readAudioSessionStrategy()
    {
        OH_AUDIO_AudioSessionStrategy value = {};
        Deserializer& valueDeserializer = *this;
        value.concurrencyMode = static_cast<OH_AUDIO_audio_AudioConcurrencyMode>(valueDeserializer.readInt32());
        return value;
    }
    OH_AUDIO_AudioCapturerFilter readAudioCapturerFilter()
    {
        OH_AUDIO_AudioCapturerFilter value = {};
        Deserializer& valueDeserializer = *this;
        const auto uid_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(valueDeserializer.readInt8());
        Opt_Number uid_buf = {};
        uid_buf.tag = uid_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (uid_buf_runtimeType))
        {
            uid_buf.value = static_cast<OH_Number>(valueDeserializer.readNumber());
        }
        value.uid = uid_buf;
        const auto capturerInfo_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(valueDeserializer.readInt8());
        Opt_AudioCapturerInfo capturerInfo_buf = {};
        capturerInfo_buf.tag = capturerInfo_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (capturerInfo_buf_runtimeType))
        {
            capturerInfo_buf.value = valueDeserializer.readAudioCapturerInfo();
        }
        value.capturerInfo = capturerInfo_buf;
        return value;
    }
    OH_AUDIO_AudioRendererFilter readAudioRendererFilter()
    {
        OH_AUDIO_AudioRendererFilter value = {};
        Deserializer& valueDeserializer = *this;
        const auto uid_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(valueDeserializer.readInt8());
        Opt_Number uid_buf = {};
        uid_buf.tag = uid_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (uid_buf_runtimeType))
        {
            uid_buf.value = static_cast<OH_Number>(valueDeserializer.readNumber());
        }
        value.uid = uid_buf;
        const auto rendererInfo_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(valueDeserializer.readInt8());
        Opt_AudioRendererInfo rendererInfo_buf = {};
        rendererInfo_buf.tag = rendererInfo_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (rendererInfo_buf_runtimeType))
        {
            rendererInfo_buf.value = valueDeserializer.readAudioRendererInfo();
        }
        value.rendererInfo = rendererInfo_buf;
        const auto rendererId_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(valueDeserializer.readInt8());
        Opt_Number rendererId_buf = {};
        rendererId_buf.tag = rendererId_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (rendererId_buf_runtimeType))
        {
            rendererId_buf.value = static_cast<OH_Number>(valueDeserializer.readNumber());
        }
        value.rendererId = rendererId_buf;
        return value;
    }
    OH_AUDIO_AudioInterrupt readAudioInterrupt()
    {
        OH_AUDIO_AudioInterrupt value = {};
        Deserializer& valueDeserializer = *this;
        value.streamUsage = static_cast<OH_AUDIO_audio_StreamUsage>(valueDeserializer.readInt32());
        value.contentType = static_cast<OH_AUDIO_audio_ContentType>(valueDeserializer.readInt32());
        value.pauseWhenDucked = valueDeserializer.readBoolean();
        return value;
    }
    OH_AUDIO_TonePlayer readTonePlayer()
    {
        Deserializer& valueDeserializer = *this;
        OH_NativePointer ptr = valueDeserializer.readPointer();
        return { ptr };
    }
    OH_AUDIO_AudioRenderer readAudioRenderer()
    {
        Deserializer& valueDeserializer = *this;
        OH_NativePointer ptr = valueDeserializer.readPointer();
        return { ptr };
    }
    OH_AUDIO_AudioCapturer readAudioCapturer()
    {
        Deserializer& valueDeserializer = *this;
        OH_NativePointer ptr = valueDeserializer.readPointer();
        return { ptr };
    }
    OH_AUDIO_AudioCapturerOptions readAudioCapturerOptions()
    {
        OH_AUDIO_AudioCapturerOptions value = {};
        Deserializer& valueDeserializer = *this;
        value.streamInfo = valueDeserializer.readAudioStreamInfo();
        value.capturerInfo = valueDeserializer.readAudioCapturerInfo();
        const auto playbackCaptureConfig_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(valueDeserializer.readInt8());
        Opt_AudioPlaybackCaptureConfig playbackCaptureConfig_buf = {};
        playbackCaptureConfig_buf.tag = playbackCaptureConfig_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (playbackCaptureConfig_buf_runtimeType))
        {
            playbackCaptureConfig_buf.value = valueDeserializer.readAudioPlaybackCaptureConfig();
        }
        value.playbackCaptureConfig = playbackCaptureConfig_buf;
        return value;
    }
    OH_AUDIO_AudioStreamDeviceChangeInfo readAudioStreamDeviceChangeInfo()
    {
        OH_AUDIO_AudioStreamDeviceChangeInfo value = {};
        Deserializer& valueDeserializer = *this;
        const OH_Int32 devices_buf_length = valueDeserializer.readInt32();
        Array_CustomObject devices_buf = {};
        valueDeserializer.resizeArray<std::decay<decltype(devices_buf)>::type,
        std::decay<decltype(*devices_buf.array)>::type>(&devices_buf, devices_buf_length);
        for (int devices_buf_i = 0; devices_buf_i < devices_buf_length; devices_buf_i++) {
            devices_buf.array[devices_buf_i] = static_cast<OH_CustomObject>(valueDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>"));
        }
        value.devices = devices_buf;
        value.changeReason = static_cast<OH_AUDIO_audio_AudioStreamDeviceChangeReason>(valueDeserializer.readInt32());
        return value;
    }
    OH_AUDIO_AudioCapturerChangeInfo readAudioCapturerChangeInfo()
    {
        OH_AUDIO_AudioCapturerChangeInfo value = {};
        Deserializer& valueDeserializer = *this;
        value.streamId = static_cast<OH_Number>(valueDeserializer.readNumber());
        value.clientUid = static_cast<OH_Number>(valueDeserializer.readNumber());
        value.capturerInfo = valueDeserializer.readAudioCapturerInfo();
        value.capturerState = static_cast<OH_AUDIO_audio_AudioState>(valueDeserializer.readInt32());
        const OH_Int32 deviceDescriptors_buf_length = valueDeserializer.readInt32();
        Array_CustomObject deviceDescriptors_buf = {};
        valueDeserializer.resizeArray<std::decay<decltype(deviceDescriptors_buf)>::type,
        std::decay<decltype(*deviceDescriptors_buf.array)>::type>(&deviceDescriptors_buf, deviceDescriptors_buf_length);
        for (int deviceDescriptors_buf_i = 0; deviceDescriptors_buf_i < deviceDescriptors_buf_length; deviceDescriptors_buf_i++) {
            deviceDescriptors_buf.array[deviceDescriptors_buf_i] = static_cast<OH_CustomObject>(valueDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>"));
        }
        value.deviceDescriptors = deviceDescriptors_buf;
        const auto muted_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(valueDeserializer.readInt8());
        Opt_Boolean muted_buf = {};
        muted_buf.tag = muted_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (muted_buf_runtimeType))
        {
            muted_buf.value = valueDeserializer.readBoolean();
        }
        value.muted = muted_buf;
        return value;
    }
    OH_AUDIO_AudioRendererChangeInfo readAudioRendererChangeInfo()
    {
        OH_AUDIO_AudioRendererChangeInfo value = {};
        Deserializer& valueDeserializer = *this;
        value.streamId = static_cast<OH_Number>(valueDeserializer.readNumber());
        value.clientUid = static_cast<OH_Number>(valueDeserializer.readNumber());
        value.rendererInfo = valueDeserializer.readAudioRendererInfo();
        value.rendererState = static_cast<OH_AUDIO_audio_AudioState>(valueDeserializer.readInt32());
        const OH_Int32 deviceDescriptors_buf_length = valueDeserializer.readInt32();
        Array_CustomObject deviceDescriptors_buf = {};
        valueDeserializer.resizeArray<std::decay<decltype(deviceDescriptors_buf)>::type,
        std::decay<decltype(*deviceDescriptors_buf.array)>::type>(&deviceDescriptors_buf, deviceDescriptors_buf_length);
        for (int deviceDescriptors_buf_i = 0; deviceDescriptors_buf_i < deviceDescriptors_buf_length; deviceDescriptors_buf_i++) {
            deviceDescriptors_buf.array[deviceDescriptors_buf_i] = static_cast<OH_CustomObject>(valueDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>"));
        }
        value.deviceDescriptors = deviceDescriptors_buf;
        return value;
    }
    OH_AUDIO_VolumeGroupInfo readVolumeGroupInfo()
    {
        OH_AUDIO_VolumeGroupInfo value = {};
        Deserializer& valueDeserializer = *this;
        value.networkId = static_cast<OH_String>(valueDeserializer.readString());
        value.groupId = static_cast<OH_Number>(valueDeserializer.readNumber());
        value.mappingId = static_cast<OH_Number>(valueDeserializer.readNumber());
        value.groupName = static_cast<OH_String>(valueDeserializer.readString());
        value.type = static_cast<OH_AUDIO_audio_ConnectType>(valueDeserializer.readInt32());
        return value;
    }
    OH_AUDIO_AudioSpatialEnabledStateForDevice readAudioSpatialEnabledStateForDevice()
    {
        OH_AUDIO_AudioSpatialEnabledStateForDevice value = {};
        Deserializer& valueDeserializer = *this;
        value.deviceDescriptor = valueDeserializer.readAudioDeviceDescriptor();
        value.enabled = valueDeserializer.readBoolean();
        return value;
    }
    OH_AUDIO_MicStateChangeEvent readMicStateChangeEvent()
    {
        OH_AUDIO_MicStateChangeEvent value = {};
        Deserializer& valueDeserializer = *this;
        value.mute = valueDeserializer.readBoolean();
        return value;
    }
    OH_AUDIO_AudioVolumeGroupManager readAudioVolumeGroupManager()
    {
        Deserializer& valueDeserializer = *this;
        OH_NativePointer ptr = valueDeserializer.readPointer();
        return { ptr };
    }
    OH_AUDIO_AudioSessionDeactivatedEvent readAudioSessionDeactivatedEvent()
    {
        OH_AUDIO_AudioSessionDeactivatedEvent value = {};
        Deserializer& valueDeserializer = *this;
        value.reason = static_cast<OH_AUDIO_audio_AudioSessionDeactivatedReason>(valueDeserializer.readInt32());
        return value;
    }
    OH_AUDIO_DeviceBlockStatusInfo readDeviceBlockStatusInfo()
    {
        OH_AUDIO_DeviceBlockStatusInfo value = {};
        Deserializer& valueDeserializer = *this;
        value.blockStatus = static_cast<OH_AUDIO_audio_DeviceBlockStatus>(valueDeserializer.readInt32());
        const OH_Int32 devices_buf_length = valueDeserializer.readInt32();
        Array_CustomObject devices_buf = {};
        valueDeserializer.resizeArray<std::decay<decltype(devices_buf)>::type,
        std::decay<decltype(*devices_buf.array)>::type>(&devices_buf, devices_buf_length);
        for (int devices_buf_i = 0; devices_buf_i < devices_buf_length; devices_buf_i++) {
            devices_buf.array[devices_buf_i] = static_cast<OH_CustomObject>(valueDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>"));
        }
        value.devices = devices_buf;
        return value;
    }
    OH_AUDIO_InterruptResult readInterruptResult()
    {
        OH_AUDIO_InterruptResult value = {};
        Deserializer& valueDeserializer = *this;
        value.requestResult = static_cast<OH_AUDIO_audio_InterruptRequestResultType>(valueDeserializer.readInt32());
        value.interruptNode = static_cast<OH_Number>(valueDeserializer.readNumber());
        return value;
    }
    OH_AUDIO_InterruptAction readInterruptAction()
    {
        OH_AUDIO_InterruptAction value = {};
        Deserializer& valueDeserializer = *this;
        value.actionType = static_cast<OH_AUDIO_audio_InterruptActionType>(valueDeserializer.readInt32());
        const auto type_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(valueDeserializer.readInt8());
        Opt_audio_InterruptType type_buf = {};
        type_buf.tag = type_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (type_buf_runtimeType))
        {
            type_buf.value = static_cast<OH_AUDIO_audio_InterruptType>(valueDeserializer.readInt32());
        }
        value.type = type_buf;
        const auto hint_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(valueDeserializer.readInt8());
        Opt_audio_InterruptHint hint_buf = {};
        hint_buf.tag = hint_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (hint_buf_runtimeType))
        {
            hint_buf.value = static_cast<OH_AUDIO_audio_InterruptHint>(valueDeserializer.readInt32());
        }
        value.hint = hint_buf;
        const auto activated_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(valueDeserializer.readInt8());
        Opt_Boolean activated_buf = {};
        activated_buf.tag = activated_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (activated_buf_runtimeType))
        {
            activated_buf.value = valueDeserializer.readBoolean();
        }
        value.activated = activated_buf;
        return value;
    }
    OH_AUDIO_DeviceChangeAction readDeviceChangeAction()
    {
        OH_AUDIO_DeviceChangeAction value = {};
        Deserializer& valueDeserializer = *this;
        value.type = static_cast<OH_AUDIO_audio_DeviceChangeType>(valueDeserializer.readInt32());
        const OH_Int32 deviceDescriptors_buf_length = valueDeserializer.readInt32();
        Array_CustomObject deviceDescriptors_buf = {};
        valueDeserializer.resizeArray<std::decay<decltype(deviceDescriptors_buf)>::type,
        std::decay<decltype(*deviceDescriptors_buf.array)>::type>(&deviceDescriptors_buf, deviceDescriptors_buf_length);
        for (int deviceDescriptors_buf_i = 0; deviceDescriptors_buf_i < deviceDescriptors_buf_length; deviceDescriptors_buf_i++) {
            deviceDescriptors_buf.array[deviceDescriptors_buf_i] = static_cast<OH_CustomObject>(valueDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>"));
        }
        value.deviceDescriptors = deviceDescriptors_buf;
        return value;
    }
    OH_AUDIO_VolumeEvent readVolumeEvent()
    {
        OH_AUDIO_VolumeEvent value = {};
        Deserializer& valueDeserializer = *this;
        value.volumeType = static_cast<OH_AUDIO_audio_AudioVolumeType>(valueDeserializer.readInt32());
        value.volume = static_cast<OH_Number>(valueDeserializer.readNumber());
        value.updateUi = valueDeserializer.readBoolean();
        value.volumeGroupId = static_cast<OH_Number>(valueDeserializer.readNumber());
        value.networkId = static_cast<OH_String>(valueDeserializer.readString());
        return value;
    }
    OH_AUDIO_InterruptEvent readInterruptEvent()
    {
        OH_AUDIO_InterruptEvent value = {};
        Deserializer& valueDeserializer = *this;
        value.eventType = static_cast<OH_AUDIO_audio_InterruptType>(valueDeserializer.readInt32());
        value.forceType = static_cast<OH_AUDIO_audio_InterruptForceType>(valueDeserializer.readInt32());
        value.hintType = static_cast<OH_AUDIO_audio_InterruptHint>(valueDeserializer.readInt32());
        return value;
    }
    OH_AUDIO_AudioRendererOptions readAudioRendererOptions()
    {
        OH_AUDIO_AudioRendererOptions value = {};
        Deserializer& valueDeserializer = *this;
        value.streamInfo = valueDeserializer.readAudioStreamInfo();
        value.rendererInfo = valueDeserializer.readAudioRendererInfo();
        const auto privacyType_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(valueDeserializer.readInt8());
        Opt_audio_AudioPrivacyType privacyType_buf = {};
        privacyType_buf.tag = privacyType_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (privacyType_buf_runtimeType))
        {
            privacyType_buf.value = static_cast<OH_AUDIO_audio_AudioPrivacyType>(valueDeserializer.readInt32());
        }
        value.privacyType = privacyType_buf;
        return value;
    }
};
OH_AUDIO_AudioManagerHandle AudioManager_constructImpl();
void AudioManager_destructImpl(OH_AUDIO_AudioManagerHandle thiz);
void AudioManager_setVolume0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volume, const AUDIO_AsyncCallback_Void* callback_);
void AudioManager_setVolume1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volume);
void AudioManager_getVolume0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioManager_getVolume1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
void AudioManager_getMinVolume0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioManager_getMinVolume1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
void AudioManager_getMaxVolume0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioManager_getMaxVolume1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
void AudioManager_getDevices0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag, const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_);
OH_NativePointer AudioManager_getDevices1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag);
void AudioManager_mute0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Boolean* mute, const AUDIO_AsyncCallback_Void* callback_);
void AudioManager_mute1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Boolean* mute);
void AudioManager_isMute0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Boolean_Void* callback_);
OH_Boolean AudioManager_isMute1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
void AudioManager_isActive0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Boolean_Void* callback_);
OH_Boolean AudioManager_isActive1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
void AudioManager_setMicrophoneMute0Impl(OH_NativePointer thisPtr, const OH_Boolean* mute, const AUDIO_AsyncCallback_Void* callback_);
void AudioManager_setMicrophoneMute1Impl(OH_NativePointer thisPtr, const OH_Boolean* mute);
void AudioManager_isMicrophoneMute0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Boolean_Void* callback_);
OH_Boolean AudioManager_isMicrophoneMute1Impl(OH_NativePointer thisPtr);
void AudioManager_setRingerMode0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRingMode* mode, const AUDIO_AsyncCallback_Void* callback_);
void AudioManager_setRingerMode1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRingMode* mode);
void AudioManager_getRingerMode0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioRingMode_Void* callback_);
OH_NativePointer AudioManager_getRingerMode1Impl(OH_NativePointer thisPtr);
void AudioManager_setAudioParameter0Impl(OH_NativePointer thisPtr, const OH_String* key, const OH_String* value, const AUDIO_AsyncCallback_Void* callback_);
void AudioManager_setAudioParameter1Impl(OH_NativePointer thisPtr, const OH_String* key, const OH_String* value);
void AudioManager_getAudioParameter0Impl(OH_NativePointer thisPtr, const OH_String* key, const AUDIO_AsyncCallback_String_Void* callback_);
OH_String AudioManager_getAudioParameter1Impl(OH_NativePointer thisPtr, const OH_String* key);
void AudioManager_setExtraParametersImpl(OH_NativePointer thisPtr, const OH_String* mainKey, const Map_String_String* kvpairs);
OH_NativePointer AudioManager_getExtraParametersImpl(OH_NativePointer thisPtr, const OH_String* mainKey, const Opt_Array_String* subKeys);
void AudioManager_setDeviceActive0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_ActiveDeviceType* deviceType, const OH_Boolean* active, const AUDIO_AsyncCallback_Void* callback_);
void AudioManager_setDeviceActive1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_ActiveDeviceType* deviceType, const OH_Boolean* active);
void AudioManager_isDeviceActive0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_ActiveDeviceType* deviceType, const AUDIO_AsyncCallback_Boolean_Void* callback_);
OH_Boolean AudioManager_isDeviceActive1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_ActiveDeviceType* deviceType);
void AudioManager_onVolumeChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_VolumeEvent_Void* callback_);
void AudioManager_onRingerModeChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioRingMode_Void* callback_);
void AudioManager_setAudioScene0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioScene* scene, const AUDIO_AsyncCallback_Void* callback_);
void AudioManager_setAudioScene1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioScene* scene);
void AudioManager_getAudioScene0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioScene_Void* callback_);
OH_NativePointer AudioManager_getAudioScene1Impl(OH_NativePointer thisPtr);
OH_NativePointer AudioManager_getAudioSceneSyncImpl(OH_NativePointer thisPtr);
void AudioManager_onDeviceChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_DeviceChangeAction_Void* callback_);
void AudioManager_offDeviceChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_DeviceChangeAction_Void* callback_);
void AudioManager_onInterruptImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioInterrupt* interrupt, const AUDIO_Callback_InterruptAction_Void* callback_);
void AudioManager_offInterruptImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioInterrupt* interrupt, const Opt_AUDIO_Callback_InterruptAction_Void* callback_);
OH_NativePointer AudioManager_getVolumeManagerImpl(OH_NativePointer thisPtr);
OH_NativePointer AudioManager_getStreamManagerImpl(OH_NativePointer thisPtr);
OH_NativePointer AudioManager_getRoutingManagerImpl(OH_NativePointer thisPtr);
OH_NativePointer AudioManager_getSessionManagerImpl(OH_NativePointer thisPtr);
OH_NativePointer AudioManager_getSpatializationManagerImpl(OH_NativePointer thisPtr);
void AudioManager_disableSafeMediaVolumeImpl(OH_NativePointer thisPtr);
OH_AUDIO_AudioRoutingManagerHandle AudioRoutingManager_constructImpl();
void AudioRoutingManager_destructImpl(OH_AUDIO_AudioRoutingManagerHandle thiz);
void AudioRoutingManager_getDevices0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag, const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_);
OH_NativePointer AudioRoutingManager_getDevices1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag);
OH_NativePointer AudioRoutingManager_getDevicesSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag);
void AudioRoutingManager_onDeviceChangeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceFlag* deviceFlag, const AUDIO_Callback_DeviceChangeAction_Void* callback_);
void AudioRoutingManager_offDeviceChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_DeviceChangeAction_Void* callback_);
OH_NativePointer AudioRoutingManager_getAvailableDevicesImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceUsage* deviceUsage);
void AudioRoutingManager_onAvailableDeviceChangeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceUsage* deviceUsage, const AUDIO_Callback_DeviceChangeAction_Void* callback_);
void AudioRoutingManager_offAvailableDeviceChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_DeviceChangeAction_Void* callback_);
void AudioRoutingManager_setCommunicationDevice0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_CommunicationDeviceType* deviceType, const OH_Boolean* active, const AUDIO_AsyncCallback_Void* callback_);
void AudioRoutingManager_setCommunicationDevice1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_CommunicationDeviceType* deviceType, const OH_Boolean* active);
void AudioRoutingManager_isCommunicationDeviceActive0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_CommunicationDeviceType* deviceType, const AUDIO_AsyncCallback_Boolean_Void* callback_);
OH_Boolean AudioRoutingManager_isCommunicationDeviceActive1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_CommunicationDeviceType* deviceType);
OH_Boolean AudioRoutingManager_isCommunicationDeviceActiveSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_CommunicationDeviceType* deviceType);
void AudioRoutingManager_selectOutputDevice0Impl(OH_NativePointer thisPtr, const Array_CustomObject* outputAudioDevices, const AUDIO_AsyncCallback_Void* callback_);
void AudioRoutingManager_selectOutputDevice1Impl(OH_NativePointer thisPtr, const Array_CustomObject* outputAudioDevices);
void AudioRoutingManager_selectOutputDeviceByFilter0Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererFilter* filter, const Array_CustomObject* outputAudioDevices, const AUDIO_AsyncCallback_Void* callback_);
void AudioRoutingManager_selectOutputDeviceByFilter1Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererFilter* filter, const Array_CustomObject* outputAudioDevices);
void AudioRoutingManager_selectInputDevice0Impl(OH_NativePointer thisPtr, const Array_CustomObject* inputAudioDevices, const AUDIO_AsyncCallback_Void* callback_);
void AudioRoutingManager_selectInputDevice1Impl(OH_NativePointer thisPtr, const Array_CustomObject* inputAudioDevices);
void AudioRoutingManager_selectInputDeviceByFilterImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerFilter* filter, const Array_CustomObject* inputAudioDevices);
void AudioRoutingManager_getPreferOutputDeviceForRendererInfo0Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererInfo* rendererInfo, const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_);
OH_NativePointer AudioRoutingManager_getPreferOutputDeviceForRendererInfo1Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererInfo* rendererInfo);
OH_NativePointer AudioRoutingManager_getPreferredOutputDeviceForRendererInfoSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererInfo* rendererInfo);
OH_NativePointer AudioRoutingManager_getPreferredOutputDeviceByFilterImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererFilter* filter);
void AudioRoutingManager_onPreferOutputDeviceChangeForRendererInfoImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioRendererInfo* rendererInfo, const AUDIO_Callback_AudioDeviceDescriptors_Void* callback_);
void AudioRoutingManager_offPreferOutputDeviceChangeForRendererInfoImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* callback_);
void AudioRoutingManager_getPreferredInputDeviceForCapturerInfo0Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerInfo* capturerInfo, const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_);
OH_NativePointer AudioRoutingManager_getPreferredInputDeviceForCapturerInfo1Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerInfo* capturerInfo);
OH_NativePointer AudioRoutingManager_getPreferredInputDeviceByFilterImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerFilter* filter);
void AudioRoutingManager_onPreferredInputDeviceChangeForCapturerInfoImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerInfo* capturerInfo, const AUDIO_Callback_AudioDeviceDescriptors_Void* callback_);
void AudioRoutingManager_offPreferredInputDeviceChangeForCapturerInfoImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* callback_);
OH_NativePointer AudioRoutingManager_getPreferredInputDeviceForCapturerInfoSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioCapturerInfo* capturerInfo);
OH_Boolean AudioRoutingManager_isMicBlockDetectionSupportedImpl(OH_NativePointer thisPtr);
void AudioRoutingManager_onMicBlockStatusChangedImpl(OH_NativePointer thisPtr, const AUDIO_Callback_DeviceBlockStatusInfo_Void* callback_);
void AudioRoutingManager_offMicBlockStatusChangedImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_DeviceBlockStatusInfo_Void* callback_);
OH_AUDIO_AudioStreamManagerHandle AudioStreamManager_constructImpl();
void AudioStreamManager_destructImpl(OH_AUDIO_AudioStreamManagerHandle thiz);
void AudioStreamManager_getCurrentAudioRendererInfoArray0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void* callback_);
OH_NativePointer AudioStreamManager_getCurrentAudioRendererInfoArray1Impl(OH_NativePointer thisPtr);
OH_NativePointer AudioStreamManager_getCurrentAudioRendererInfoArraySyncImpl(OH_NativePointer thisPtr);
void AudioStreamManager_getCurrentAudioCapturerInfoArray0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void* callback_);
OH_NativePointer AudioStreamManager_getCurrentAudioCapturerInfoArray1Impl(OH_NativePointer thisPtr);
OH_NativePointer AudioStreamManager_getCurrentAudioCapturerInfoArraySyncImpl(OH_NativePointer thisPtr);
void AudioStreamManager_getAudioEffectInfoArray0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_StreamUsage* usage, const AUDIO_AsyncCallback_AudioEffectInfoArray_Void* callback_);
OH_NativePointer AudioStreamManager_getAudioEffectInfoArray1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_StreamUsage* usage);
OH_NativePointer AudioStreamManager_getAudioEffectInfoArraySyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_StreamUsage* usage);
void AudioStreamManager_onAudioRendererChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioRendererChangeInfoArray_Void* callback_);
void AudioStreamManager_offAudioRendererChangeImpl(OH_NativePointer thisPtr);
void AudioStreamManager_onAudioCapturerChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioCapturerChangeInfoArray_Void* callback_);
void AudioStreamManager_offAudioCapturerChangeImpl(OH_NativePointer thisPtr);
void AudioStreamManager_isActive0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Boolean_Void* callback_);
OH_Boolean AudioStreamManager_isActive1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
OH_Boolean AudioStreamManager_isActiveSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
OH_AUDIO_AudioSessionManagerHandle AudioSessionManager_constructImpl();
void AudioSessionManager_destructImpl(OH_AUDIO_AudioSessionManagerHandle thiz);
void AudioSessionManager_activateAudioSessionImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioSessionStrategy* strategy);
void AudioSessionManager_deactivateAudioSessionImpl(OH_NativePointer thisPtr);
OH_Boolean AudioSessionManager_isAudioSessionActivatedImpl(OH_NativePointer thisPtr);
void AudioSessionManager_onAudioSessionDeactivatedImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioSessionDeactivatedEvent_Void* callback_);
void AudioSessionManager_offAudioSessionDeactivatedImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioSessionDeactivatedEvent_Void* callback_);
OH_AUDIO_AudioVolumeManagerHandle AudioVolumeManager_constructImpl();
void AudioVolumeManager_destructImpl(OH_AUDIO_AudioVolumeManagerHandle thiz);
void AudioVolumeManager_getVolumeGroupInfos0Impl(OH_NativePointer thisPtr, const OH_String* networkId, const AUDIO_AsyncCallback_VolumeGroupInfos_Void* callback_);
OH_NativePointer AudioVolumeManager_getVolumeGroupInfos1Impl(OH_NativePointer thisPtr, const OH_String* networkId);
OH_NativePointer AudioVolumeManager_getVolumeGroupInfosSyncImpl(OH_NativePointer thisPtr, const OH_String* networkId);
void AudioVolumeManager_getVolumeGroupManager0Impl(OH_NativePointer thisPtr, const OH_Number* groupId, const AUDIO_AsyncCallback_AudioVolumeGroupManager_Void* callback_);
OH_NativePointer AudioVolumeManager_getVolumeGroupManager1Impl(OH_NativePointer thisPtr, const OH_Number* groupId);
OH_NativePointer AudioVolumeManager_getVolumeGroupManagerSyncImpl(OH_NativePointer thisPtr, const OH_Number* groupId);
void AudioVolumeManager_onVolumeChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_VolumeEvent_Void* callback_);
void AudioVolumeManager_offVolumeChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_VolumeEvent_Void* callback_);
OH_AUDIO_AudioVolumeGroupManagerHandle AudioVolumeGroupManager_constructImpl();
void AudioVolumeGroupManager_destructImpl(OH_AUDIO_AudioVolumeGroupManagerHandle thiz);
void AudioVolumeGroupManager_setVolume0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volume, const AUDIO_AsyncCallback_Void* callback_);
void AudioVolumeGroupManager_setVolume1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volume);
void AudioVolumeGroupManager_setVolumeWithFlagImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volume, const OH_Number* flags);
OH_NativePointer AudioVolumeGroupManager_getActiveVolumeTypeSyncImpl(OH_NativePointer thisPtr, const OH_Number* uid);
void AudioVolumeGroupManager_getVolume0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioVolumeGroupManager_getVolume1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
OH_Number AudioVolumeGroupManager_getVolumeSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
void AudioVolumeGroupManager_getMinVolume0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioVolumeGroupManager_getMinVolume1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
OH_Number AudioVolumeGroupManager_getMinVolumeSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
void AudioVolumeGroupManager_getMaxVolume0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioVolumeGroupManager_getMaxVolume1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
OH_Number AudioVolumeGroupManager_getMaxVolumeSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
void AudioVolumeGroupManager_mute0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Boolean* mute, const AUDIO_AsyncCallback_Void* callback_);
void AudioVolumeGroupManager_mute1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Boolean* mute);
void AudioVolumeGroupManager_isMute0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const AUDIO_AsyncCallback_Boolean_Void* callback_);
OH_Boolean AudioVolumeGroupManager_isMute1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
OH_Boolean AudioVolumeGroupManager_isMuteSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType);
void AudioVolumeGroupManager_setRingerMode0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRingMode* mode, const AUDIO_AsyncCallback_Void* callback_);
void AudioVolumeGroupManager_setRingerMode1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRingMode* mode);
void AudioVolumeGroupManager_getRingerMode0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioRingMode_Void* callback_);
OH_NativePointer AudioVolumeGroupManager_getRingerMode1Impl(OH_NativePointer thisPtr);
OH_NativePointer AudioVolumeGroupManager_getRingerModeSyncImpl(OH_NativePointer thisPtr);
void AudioVolumeGroupManager_onRingerModeChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioRingMode_Void* callback_);
void AudioVolumeGroupManager_setMicrophoneMute0Impl(OH_NativePointer thisPtr, const OH_Boolean* mute, const AUDIO_AsyncCallback_Void* callback_);
void AudioVolumeGroupManager_setMicrophoneMute1Impl(OH_NativePointer thisPtr, const OH_Boolean* mute);
void AudioVolumeGroupManager_setMicMuteImpl(OH_NativePointer thisPtr, const OH_Boolean* mute);
void AudioVolumeGroupManager_setMicMutePersistentImpl(OH_NativePointer thisPtr, const OH_Boolean* mute, const OH_AUDIO_audio_PolicyType* type);
OH_Boolean AudioVolumeGroupManager_isPersistentMicMuteImpl(OH_NativePointer thisPtr);
void AudioVolumeGroupManager_isMicrophoneMute0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Boolean_Void* callback_);
OH_Boolean AudioVolumeGroupManager_isMicrophoneMute1Impl(OH_NativePointer thisPtr);
OH_Boolean AudioVolumeGroupManager_isMicrophoneMuteSyncImpl(OH_NativePointer thisPtr);
void AudioVolumeGroupManager_onMicStateChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_MicStateChangeEvent_Void* callback_);
void AudioVolumeGroupManager_offMicStateChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_MicStateChangeEvent_Void* callback_);
OH_Boolean AudioVolumeGroupManager_isVolumeUnadjustableImpl(OH_NativePointer thisPtr);
void AudioVolumeGroupManager_adjustVolumeByStep0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_VolumeAdjustType* adjustType, const AUDIO_AsyncCallback_Void* callback_);
void AudioVolumeGroupManager_adjustVolumeByStep1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_VolumeAdjustType* adjustType);
void AudioVolumeGroupManager_adjustSystemVolumeByStep0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_AUDIO_audio_VolumeAdjustType* adjustType, const AUDIO_AsyncCallback_Void* callback_);
void AudioVolumeGroupManager_adjustSystemVolumeByStep1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_AUDIO_audio_VolumeAdjustType* adjustType);
void AudioVolumeGroupManager_getSystemVolumeInDb0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volumeLevel, const OH_AUDIO_audio_DeviceType* device, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioVolumeGroupManager_getSystemVolumeInDb1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volumeLevel, const OH_AUDIO_audio_DeviceType* device);
OH_Number AudioVolumeGroupManager_getSystemVolumeInDbSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioVolumeType* volumeType, const OH_Number* volumeLevel, const OH_AUDIO_audio_DeviceType* device);
OH_Number AudioVolumeGroupManager_getMaxAmplitudeForInputDeviceImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* inputDevice);
OH_Number AudioVolumeGroupManager_getMaxAmplitudeForOutputDeviceImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* outputDevice);
OH_AUDIO_AudioSpatializationManagerHandle AudioSpatializationManager_constructImpl();
void AudioSpatializationManager_destructImpl(OH_AUDIO_AudioSpatializationManagerHandle thiz);
OH_Boolean AudioSpatializationManager_isSpatializationSupportedImpl(OH_NativePointer thisPtr);
OH_Boolean AudioSpatializationManager_isSpatializationSupportedForDeviceImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor);
OH_Boolean AudioSpatializationManager_isHeadTrackingSupportedImpl(OH_NativePointer thisPtr);
OH_Boolean AudioSpatializationManager_isHeadTrackingSupportedForDeviceImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor);
void AudioSpatializationManager_setSpatializationEnabled0Impl(OH_NativePointer thisPtr, const OH_Boolean* enable, const AUDIO_AsyncCallback_Void* callback_);
void AudioSpatializationManager_setSpatializationEnabled1Impl(OH_NativePointer thisPtr, const OH_Boolean* enable);
void AudioSpatializationManager_setSpatializationEnabled2Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor, const OH_Boolean* enabled);
OH_Boolean AudioSpatializationManager_isSpatializationEnabled0Impl(OH_NativePointer thisPtr);
OH_Boolean AudioSpatializationManager_isSpatializationEnabled1Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor);
void AudioSpatializationManager_onSpatializationEnabledChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_Boolean_Void* callback_);
void AudioSpatializationManager_onSpatializationEnabledChangeForAnyDeviceImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* callback_);
void AudioSpatializationManager_offSpatializationEnabledChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_Boolean_Void* callback_);
void AudioSpatializationManager_offSpatializationEnabledChangeForAnyDeviceImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* callback_);
void AudioSpatializationManager_setHeadTrackingEnabled0Impl(OH_NativePointer thisPtr, const OH_Boolean* enable, const AUDIO_AsyncCallback_Void* callback_);
void AudioSpatializationManager_setHeadTrackingEnabled1Impl(OH_NativePointer thisPtr, const OH_Boolean* enable);
void AudioSpatializationManager_setHeadTrackingEnabled2Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor, const OH_Boolean* enabled);
OH_Boolean AudioSpatializationManager_isHeadTrackingEnabled0Impl(OH_NativePointer thisPtr);
OH_Boolean AudioSpatializationManager_isHeadTrackingEnabled1Impl(OH_NativePointer thisPtr, const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor);
void AudioSpatializationManager_onHeadTrackingEnabledChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_Boolean_Void* callback_);
void AudioSpatializationManager_onHeadTrackingEnabledChangeForAnyDeviceImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* callback_);
void AudioSpatializationManager_offHeadTrackingEnabledChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_Boolean_Void* callback_);
void AudioSpatializationManager_offHeadTrackingEnabledChangeForAnyDeviceImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* callback_);
void AudioSpatializationManager_updateSpatialDeviceStateImpl(OH_NativePointer thisPtr, const OH_AUDIO_AudioSpatialDeviceState* spatialDeviceState);
void AudioSpatializationManager_setSpatializationSceneTypeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioSpatializationSceneType* spatializationSceneType);
OH_NativePointer AudioSpatializationManager_getSpatializationSceneTypeImpl(OH_NativePointer thisPtr);
OH_AUDIO_AudioRendererHandle AudioRenderer_constructImpl();
void AudioRenderer_destructImpl(OH_AUDIO_AudioRendererHandle thiz);
void AudioRenderer_getRendererInfo0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioRendererInfo_Void* callback_);
OH_NativePointer AudioRenderer_getRendererInfo1Impl(OH_NativePointer thisPtr);
OH_NativePointer AudioRenderer_getRendererInfoSyncImpl(OH_NativePointer thisPtr);
void AudioRenderer_getStreamInfo0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioStreamInfo_Void* callback_);
OH_NativePointer AudioRenderer_getStreamInfo1Impl(OH_NativePointer thisPtr);
OH_NativePointer AudioRenderer_getStreamInfoSyncImpl(OH_NativePointer thisPtr);
void AudioRenderer_getAudioStreamId0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioRenderer_getAudioStreamId1Impl(OH_NativePointer thisPtr);
OH_Number AudioRenderer_getAudioStreamIdSyncImpl(OH_NativePointer thisPtr);
void AudioRenderer_getAudioEffectMode0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioEffectMode_Void* callback_);
OH_NativePointer AudioRenderer_getAudioEffectMode1Impl(OH_NativePointer thisPtr);
void AudioRenderer_setAudioEffectMode0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioEffectMode* mode, const AUDIO_AsyncCallback_Void* callback_);
void AudioRenderer_setAudioEffectMode1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioEffectMode* mode);
void AudioRenderer_start0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
void AudioRenderer_start1Impl(OH_NativePointer thisPtr);
void AudioRenderer_write0Impl(OH_NativePointer thisPtr, const OH_Buffer* buffer, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioRenderer_write1Impl(OH_NativePointer thisPtr, const OH_Buffer* buffer);
void AudioRenderer_getAudioTime0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioRenderer_getAudioTime1Impl(OH_NativePointer thisPtr);
OH_Number AudioRenderer_getAudioTimeSyncImpl(OH_NativePointer thisPtr);
void AudioRenderer_drain0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
void AudioRenderer_drain1Impl(OH_NativePointer thisPtr);
void AudioRenderer_flushImpl(OH_NativePointer thisPtr);
void AudioRenderer_pause0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
void AudioRenderer_pause1Impl(OH_NativePointer thisPtr);
void AudioRenderer_stop0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
void AudioRenderer_stop1Impl(OH_NativePointer thisPtr);
void AudioRenderer_release0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
void AudioRenderer_release1Impl(OH_NativePointer thisPtr);
void AudioRenderer_getBufferSize0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioRenderer_getBufferSize1Impl(OH_NativePointer thisPtr);
OH_Number AudioRenderer_getBufferSizeSyncImpl(OH_NativePointer thisPtr);
void AudioRenderer_setRenderRate0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRendererRate* rate, const AUDIO_AsyncCallback_Void* callback_);
void AudioRenderer_setRenderRate1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AudioRendererRate* rate);
void AudioRenderer_setSpeedImpl(OH_NativePointer thisPtr, const OH_Number* speed);
void AudioRenderer_getRenderRate0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioRendererRate_Void* callback_);
OH_NativePointer AudioRenderer_getRenderRate1Impl(OH_NativePointer thisPtr);
OH_NativePointer AudioRenderer_getRenderRateSyncImpl(OH_NativePointer thisPtr);
OH_Number AudioRenderer_getSpeedImpl(OH_NativePointer thisPtr);
void AudioRenderer_setInterruptMode0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_InterruptMode* mode, const AUDIO_AsyncCallback_Void* callback_);
void AudioRenderer_setInterruptMode1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_InterruptMode* mode);
void AudioRenderer_setInterruptModeSyncImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_InterruptMode* mode);
void AudioRenderer_setVolume0Impl(OH_NativePointer thisPtr, const OH_Number* volume, const AUDIO_AsyncCallback_Void* callback_);
void AudioRenderer_setVolume1Impl(OH_NativePointer thisPtr, const OH_Number* volume);
OH_Number AudioRenderer_getVolumeImpl(OH_NativePointer thisPtr);
void AudioRenderer_setVolumeWithRampImpl(OH_NativePointer thisPtr, const OH_Number* volume, const OH_Number* duration);
void AudioRenderer_getMinStreamVolume0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioRenderer_getMinStreamVolume1Impl(OH_NativePointer thisPtr);
OH_Number AudioRenderer_getMinStreamVolumeSyncImpl(OH_NativePointer thisPtr);
void AudioRenderer_getMaxStreamVolume0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioRenderer_getMaxStreamVolume1Impl(OH_NativePointer thisPtr);
OH_Number AudioRenderer_getMaxStreamVolumeSyncImpl(OH_NativePointer thisPtr);
void AudioRenderer_getUnderflowCount0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioRenderer_getUnderflowCount1Impl(OH_NativePointer thisPtr);
OH_Number AudioRenderer_getUnderflowCountSyncImpl(OH_NativePointer thisPtr);
void AudioRenderer_getCurrentOutputDevices0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_);
OH_NativePointer AudioRenderer_getCurrentOutputDevices1Impl(OH_NativePointer thisPtr);
OH_NativePointer AudioRenderer_getCurrentOutputDevicesSyncImpl(OH_NativePointer thisPtr);
void AudioRenderer_setChannelBlendModeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_ChannelBlendMode* mode);
void AudioRenderer_setSilentModeAndMixWithOthersImpl(OH_NativePointer thisPtr, const OH_Boolean* on);
OH_Boolean AudioRenderer_getSilentModeAndMixWithOthersImpl(OH_NativePointer thisPtr);
void AudioRenderer_setDefaultOutputDeviceImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_DeviceType* deviceType);
void AudioRenderer_onAudioInterruptImpl(OH_NativePointer thisPtr, const AUDIO_Callback_InterruptEvent_Void* callback_);
void AudioRenderer_onMarkReachImpl(OH_NativePointer thisPtr, const OH_Number* frame, const AUDIO_Callback_Number_Void* callback_);
void AudioRenderer_offMarkReachImpl(OH_NativePointer thisPtr);
void AudioRenderer_onPeriodReachImpl(OH_NativePointer thisPtr, const OH_Number* frame, const AUDIO_Callback_Number_Void* callback_);
void AudioRenderer_offPeriodReachImpl(OH_NativePointer thisPtr);
void AudioRenderer_onStateChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioState_Void* callback_);
void AudioRenderer_onOutputDeviceChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioDeviceDescriptors_Void* callback_);
void AudioRenderer_onOutputDeviceChangeWithInfoImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioStreamDeviceChangeInfo_Void* callback_);
void AudioRenderer_offOutputDeviceChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* callback_);
void AudioRenderer_offOutputDeviceChangeWithInfoImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioStreamDeviceChangeInfo_Void* callback_);
void AudioRenderer_onWriteDataImpl(OH_NativePointer thisPtr, const AUDIO_AudioRendererWriteDataCallback* callback_);
void AudioRenderer_offWriteDataImpl(OH_NativePointer thisPtr, const Opt_AUDIO_AudioRendererWriteDataCallback* callback_);
OH_NativePointer AudioRenderer_getStateImpl(OH_NativePointer thisPtr);
OH_AUDIO_AudioCapturerHandle AudioCapturer_constructImpl();
void AudioCapturer_destructImpl(OH_AUDIO_AudioCapturerHandle thiz);
void AudioCapturer_getCapturerInfo0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioCapturerInfo_Void* callback_);
OH_NativePointer AudioCapturer_getCapturerInfo1Impl(OH_NativePointer thisPtr);
OH_NativePointer AudioCapturer_getCapturerInfoSyncImpl(OH_NativePointer thisPtr);
void AudioCapturer_getStreamInfo0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_AudioStreamInfo_Void* callback_);
OH_NativePointer AudioCapturer_getStreamInfo1Impl(OH_NativePointer thisPtr);
OH_NativePointer AudioCapturer_getStreamInfoSyncImpl(OH_NativePointer thisPtr);
void AudioCapturer_getAudioStreamId0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioCapturer_getAudioStreamId1Impl(OH_NativePointer thisPtr);
OH_Number AudioCapturer_getAudioStreamIdSyncImpl(OH_NativePointer thisPtr);
void AudioCapturer_start0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
void AudioCapturer_start1Impl(OH_NativePointer thisPtr);
void AudioCapturer_read0Impl(OH_NativePointer thisPtr, const OH_Number* size, const OH_Boolean* isBlockingRead, const AUDIO_AsyncCallback_Buffer_Void* callback_);
OH_Buffer AudioCapturer_read1Impl(OH_NativePointer thisPtr, const OH_Number* size, const OH_Boolean* isBlockingRead);
void AudioCapturer_getAudioTime0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioCapturer_getAudioTime1Impl(OH_NativePointer thisPtr);
OH_Number AudioCapturer_getAudioTimeSyncImpl(OH_NativePointer thisPtr);
void AudioCapturer_stop0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
void AudioCapturer_stop1Impl(OH_NativePointer thisPtr);
void AudioCapturer_release0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
void AudioCapturer_release1Impl(OH_NativePointer thisPtr);
void AudioCapturer_getBufferSize0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Number_Void* callback_);
OH_Number AudioCapturer_getBufferSize1Impl(OH_NativePointer thisPtr);
OH_Number AudioCapturer_getBufferSizeSyncImpl(OH_NativePointer thisPtr);
OH_NativePointer AudioCapturer_getCurrentInputDevicesImpl(OH_NativePointer thisPtr);
OH_NativePointer AudioCapturer_getCurrentAudioCapturerChangeInfoImpl(OH_NativePointer thisPtr);
OH_Number AudioCapturer_getOverflowCountImpl(OH_NativePointer thisPtr);
OH_Number AudioCapturer_getOverflowCountSyncImpl(OH_NativePointer thisPtr);
void AudioCapturer_onMarkReachImpl(OH_NativePointer thisPtr, const OH_Number* frame, const AUDIO_Callback_Number_Void* callback_);
void AudioCapturer_offMarkReachImpl(OH_NativePointer thisPtr);
void AudioCapturer_onPeriodReachImpl(OH_NativePointer thisPtr, const OH_Number* frame, const AUDIO_Callback_Number_Void* callback_);
void AudioCapturer_offPeriodReachImpl(OH_NativePointer thisPtr);
void AudioCapturer_onStateChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioState_Void* callback_);
void AudioCapturer_onAudioInterruptImpl(OH_NativePointer thisPtr, const AUDIO_Callback_InterruptEvent_Void* callback_);
void AudioCapturer_offAudioInterruptImpl(OH_NativePointer thisPtr);
void AudioCapturer_onInputDeviceChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioDeviceDescriptors_Void* callback_);
void AudioCapturer_offInputDeviceChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* callback_);
void AudioCapturer_onAudioCapturerChangeImpl(OH_NativePointer thisPtr, const AUDIO_Callback_AudioCapturerChangeInfo_Void* callback_);
void AudioCapturer_offAudioCapturerChangeImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_AudioCapturerChangeInfo_Void* callback_);
void AudioCapturer_onReadDataImpl(OH_NativePointer thisPtr, const AUDIO_Callback_Buffer_Void* callback_);
void AudioCapturer_offReadDataImpl(OH_NativePointer thisPtr, const Opt_AUDIO_Callback_Buffer_Void* callback_);
OH_NativePointer AudioCapturer_getStateImpl(OH_NativePointer thisPtr);
OH_AUDIO_AsrProcessingControllerHandle AsrProcessingController_constructImpl();
void AsrProcessingController_destructImpl(OH_AUDIO_AsrProcessingControllerHandle thiz);
OH_Boolean AsrProcessingController_setAsrAecModeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AsrAecMode* mode);
OH_NativePointer AsrProcessingController_getAsrAecModeImpl(OH_NativePointer thisPtr);
OH_Boolean AsrProcessingController_setAsrNoiseSuppressionModeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AsrNoiseSuppressionMode* mode);
OH_NativePointer AsrProcessingController_getAsrNoiseSuppressionModeImpl(OH_NativePointer thisPtr);
OH_Boolean AsrProcessingController_isWhisperingImpl(OH_NativePointer thisPtr);
OH_Boolean AsrProcessingController_setAsrVoiceControlModeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AsrVoiceControlMode* mode, const OH_Boolean* enable);
OH_Boolean AsrProcessingController_setAsrVoiceMuteModeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AsrVoiceMuteMode* mode, const OH_Boolean* enable);
OH_Boolean AsrProcessingController_setAsrWhisperDetectionModeImpl(OH_NativePointer thisPtr, const OH_AUDIO_audio_AsrWhisperDetectionMode* mode);
OH_NativePointer AsrProcessingController_getAsrWhisperDetectionModeImpl(OH_NativePointer thisPtr);
OH_AUDIO_TonePlayerHandle TonePlayer_constructImpl();
void TonePlayer_destructImpl(OH_AUDIO_TonePlayerHandle thiz);
void TonePlayer_load0Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_ToneType* type, const AUDIO_AsyncCallback_Void* callback_);
void TonePlayer_load1Impl(OH_NativePointer thisPtr, const OH_AUDIO_audio_ToneType* type);
void TonePlayer_start0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
void TonePlayer_start1Impl(OH_NativePointer thisPtr);
void TonePlayer_stop0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
void TonePlayer_stop1Impl(OH_NativePointer thisPtr);
void TonePlayer_release0Impl(OH_NativePointer thisPtr, const AUDIO_AsyncCallback_Void* callback_);
void TonePlayer_release1Impl(OH_NativePointer thisPtr);
OH_NativePointer GlobalScope_ohos_multimedia_audio_getAudioManagerImpl();
void GlobalScope_ohos_multimedia_audio_createAudioCapturer0Impl(const OH_CustomObject* options, const OH_CustomObject* callback_);
OH_NativePointer GlobalScope_ohos_multimedia_audio_createAudioCapturer1Impl(const OH_CustomObject* options);
void GlobalScope_ohos_multimedia_audio_createAudioRenderer0Impl(const OH_CustomObject* options, const OH_CustomObject* callback_);
OH_NativePointer GlobalScope_ohos_multimedia_audio_createAudioRenderer1Impl(const OH_CustomObject* options);
void GlobalScope_ohos_multimedia_audio_createTonePlayer0Impl(const OH_CustomObject* options, const OH_CustomObject* callback_);
OH_NativePointer GlobalScope_ohos_multimedia_audio_createTonePlayer1Impl(const OH_CustomObject* options);
OH_NativePointer GlobalScope_ohos_multimedia_audio_createAsrProcessingControllerImpl(const OH_CustomObject* audioCapturer);
const OH_AUDIO_AudioManagerModifier* OH_AUDIO_AudioManagerModifierImpl() {
    const static OH_AUDIO_AudioManagerModifier instance = {
        &AudioManager_constructImpl,
        &AudioManager_destructImpl,
        &AudioManager_setVolume0Impl,
        &AudioManager_setVolume1Impl,
        &AudioManager_getVolume0Impl,
        &AudioManager_getVolume1Impl,
        &AudioManager_getMinVolume0Impl,
        &AudioManager_getMinVolume1Impl,
        &AudioManager_getMaxVolume0Impl,
        &AudioManager_getMaxVolume1Impl,
        &AudioManager_getDevices0Impl,
        &AudioManager_getDevices1Impl,
        &AudioManager_mute0Impl,
        &AudioManager_mute1Impl,
        &AudioManager_isMute0Impl,
        &AudioManager_isMute1Impl,
        &AudioManager_isActive0Impl,
        &AudioManager_isActive1Impl,
        &AudioManager_setMicrophoneMute0Impl,
        &AudioManager_setMicrophoneMute1Impl,
        &AudioManager_isMicrophoneMute0Impl,
        &AudioManager_isMicrophoneMute1Impl,
        &AudioManager_setRingerMode0Impl,
        &AudioManager_setRingerMode1Impl,
        &AudioManager_getRingerMode0Impl,
        &AudioManager_getRingerMode1Impl,
        &AudioManager_setAudioParameter0Impl,
        &AudioManager_setAudioParameter1Impl,
        &AudioManager_getAudioParameter0Impl,
        &AudioManager_getAudioParameter1Impl,
        &AudioManager_setExtraParametersImpl,
        &AudioManager_getExtraParametersImpl,
        &AudioManager_setDeviceActive0Impl,
        &AudioManager_setDeviceActive1Impl,
        &AudioManager_isDeviceActive0Impl,
        &AudioManager_isDeviceActive1Impl,
        &AudioManager_onVolumeChangeImpl,
        &AudioManager_onRingerModeChangeImpl,
        &AudioManager_setAudioScene0Impl,
        &AudioManager_setAudioScene1Impl,
        &AudioManager_getAudioScene0Impl,
        &AudioManager_getAudioScene1Impl,
        &AudioManager_getAudioSceneSyncImpl,
        &AudioManager_onDeviceChangeImpl,
        &AudioManager_offDeviceChangeImpl,
        &AudioManager_onInterruptImpl,
        &AudioManager_offInterruptImpl,
        &AudioManager_getVolumeManagerImpl,
        &AudioManager_getStreamManagerImpl,
        &AudioManager_getRoutingManagerImpl,
        &AudioManager_getSessionManagerImpl,
        &AudioManager_getSpatializationManagerImpl,
        &AudioManager_disableSafeMediaVolumeImpl,
    };
    return &instance;
}
const OH_AUDIO_AudioRoutingManagerModifier* OH_AUDIO_AudioRoutingManagerModifierImpl() {
    const static OH_AUDIO_AudioRoutingManagerModifier instance = {
        &AudioRoutingManager_constructImpl,
        &AudioRoutingManager_destructImpl,
        &AudioRoutingManager_getDevices0Impl,
        &AudioRoutingManager_getDevices1Impl,
        &AudioRoutingManager_getDevicesSyncImpl,
        &AudioRoutingManager_onDeviceChangeImpl,
        &AudioRoutingManager_offDeviceChangeImpl,
        &AudioRoutingManager_getAvailableDevicesImpl,
        &AudioRoutingManager_onAvailableDeviceChangeImpl,
        &AudioRoutingManager_offAvailableDeviceChangeImpl,
        &AudioRoutingManager_setCommunicationDevice0Impl,
        &AudioRoutingManager_setCommunicationDevice1Impl,
        &AudioRoutingManager_isCommunicationDeviceActive0Impl,
        &AudioRoutingManager_isCommunicationDeviceActive1Impl,
        &AudioRoutingManager_isCommunicationDeviceActiveSyncImpl,
        &AudioRoutingManager_selectOutputDevice0Impl,
        &AudioRoutingManager_selectOutputDevice1Impl,
        &AudioRoutingManager_selectOutputDeviceByFilter0Impl,
        &AudioRoutingManager_selectOutputDeviceByFilter1Impl,
        &AudioRoutingManager_selectInputDevice0Impl,
        &AudioRoutingManager_selectInputDevice1Impl,
        &AudioRoutingManager_selectInputDeviceByFilterImpl,
        &AudioRoutingManager_getPreferOutputDeviceForRendererInfo0Impl,
        &AudioRoutingManager_getPreferOutputDeviceForRendererInfo1Impl,
        &AudioRoutingManager_getPreferredOutputDeviceForRendererInfoSyncImpl,
        &AudioRoutingManager_getPreferredOutputDeviceByFilterImpl,
        &AudioRoutingManager_onPreferOutputDeviceChangeForRendererInfoImpl,
        &AudioRoutingManager_offPreferOutputDeviceChangeForRendererInfoImpl,
        &AudioRoutingManager_getPreferredInputDeviceForCapturerInfo0Impl,
        &AudioRoutingManager_getPreferredInputDeviceForCapturerInfo1Impl,
        &AudioRoutingManager_getPreferredInputDeviceByFilterImpl,
        &AudioRoutingManager_onPreferredInputDeviceChangeForCapturerInfoImpl,
        &AudioRoutingManager_offPreferredInputDeviceChangeForCapturerInfoImpl,
        &AudioRoutingManager_getPreferredInputDeviceForCapturerInfoSyncImpl,
        &AudioRoutingManager_isMicBlockDetectionSupportedImpl,
        &AudioRoutingManager_onMicBlockStatusChangedImpl,
        &AudioRoutingManager_offMicBlockStatusChangedImpl,
    };
    return &instance;
}
const OH_AUDIO_AudioStreamManagerModifier* OH_AUDIO_AudioStreamManagerModifierImpl() {
    const static OH_AUDIO_AudioStreamManagerModifier instance = {
        &AudioStreamManager_constructImpl,
        &AudioStreamManager_destructImpl,
        &AudioStreamManager_getCurrentAudioRendererInfoArray0Impl,
        &AudioStreamManager_getCurrentAudioRendererInfoArray1Impl,
        &AudioStreamManager_getCurrentAudioRendererInfoArraySyncImpl,
        &AudioStreamManager_getCurrentAudioCapturerInfoArray0Impl,
        &AudioStreamManager_getCurrentAudioCapturerInfoArray1Impl,
        &AudioStreamManager_getCurrentAudioCapturerInfoArraySyncImpl,
        &AudioStreamManager_getAudioEffectInfoArray0Impl,
        &AudioStreamManager_getAudioEffectInfoArray1Impl,
        &AudioStreamManager_getAudioEffectInfoArraySyncImpl,
        &AudioStreamManager_onAudioRendererChangeImpl,
        &AudioStreamManager_offAudioRendererChangeImpl,
        &AudioStreamManager_onAudioCapturerChangeImpl,
        &AudioStreamManager_offAudioCapturerChangeImpl,
        &AudioStreamManager_isActive0Impl,
        &AudioStreamManager_isActive1Impl,
        &AudioStreamManager_isActiveSyncImpl,
    };
    return &instance;
}
const OH_AUDIO_AudioSessionManagerModifier* OH_AUDIO_AudioSessionManagerModifierImpl() {
    const static OH_AUDIO_AudioSessionManagerModifier instance = {
        &AudioSessionManager_constructImpl,
        &AudioSessionManager_destructImpl,
        &AudioSessionManager_activateAudioSessionImpl,
        &AudioSessionManager_deactivateAudioSessionImpl,
        &AudioSessionManager_isAudioSessionActivatedImpl,
        &AudioSessionManager_onAudioSessionDeactivatedImpl,
        &AudioSessionManager_offAudioSessionDeactivatedImpl,
    };
    return &instance;
}
const OH_AUDIO_AudioVolumeManagerModifier* OH_AUDIO_AudioVolumeManagerModifierImpl() {
    const static OH_AUDIO_AudioVolumeManagerModifier instance = {
        &AudioVolumeManager_constructImpl,
        &AudioVolumeManager_destructImpl,
        &AudioVolumeManager_getVolumeGroupInfos0Impl,
        &AudioVolumeManager_getVolumeGroupInfos1Impl,
        &AudioVolumeManager_getVolumeGroupInfosSyncImpl,
        &AudioVolumeManager_getVolumeGroupManager0Impl,
        &AudioVolumeManager_getVolumeGroupManager1Impl,
        &AudioVolumeManager_getVolumeGroupManagerSyncImpl,
        &AudioVolumeManager_onVolumeChangeImpl,
        &AudioVolumeManager_offVolumeChangeImpl,
    };
    return &instance;
}
const OH_AUDIO_AudioVolumeGroupManagerModifier* OH_AUDIO_AudioVolumeGroupManagerModifierImpl() {
    const static OH_AUDIO_AudioVolumeGroupManagerModifier instance = {
        &AudioVolumeGroupManager_constructImpl,
        &AudioVolumeGroupManager_destructImpl,
        &AudioVolumeGroupManager_setVolume0Impl,
        &AudioVolumeGroupManager_setVolume1Impl,
        &AudioVolumeGroupManager_setVolumeWithFlagImpl,
        &AudioVolumeGroupManager_getActiveVolumeTypeSyncImpl,
        &AudioVolumeGroupManager_getVolume0Impl,
        &AudioVolumeGroupManager_getVolume1Impl,
        &AudioVolumeGroupManager_getVolumeSyncImpl,
        &AudioVolumeGroupManager_getMinVolume0Impl,
        &AudioVolumeGroupManager_getMinVolume1Impl,
        &AudioVolumeGroupManager_getMinVolumeSyncImpl,
        &AudioVolumeGroupManager_getMaxVolume0Impl,
        &AudioVolumeGroupManager_getMaxVolume1Impl,
        &AudioVolumeGroupManager_getMaxVolumeSyncImpl,
        &AudioVolumeGroupManager_mute0Impl,
        &AudioVolumeGroupManager_mute1Impl,
        &AudioVolumeGroupManager_isMute0Impl,
        &AudioVolumeGroupManager_isMute1Impl,
        &AudioVolumeGroupManager_isMuteSyncImpl,
        &AudioVolumeGroupManager_setRingerMode0Impl,
        &AudioVolumeGroupManager_setRingerMode1Impl,
        &AudioVolumeGroupManager_getRingerMode0Impl,
        &AudioVolumeGroupManager_getRingerMode1Impl,
        &AudioVolumeGroupManager_getRingerModeSyncImpl,
        &AudioVolumeGroupManager_onRingerModeChangeImpl,
        &AudioVolumeGroupManager_setMicrophoneMute0Impl,
        &AudioVolumeGroupManager_setMicrophoneMute1Impl,
        &AudioVolumeGroupManager_setMicMuteImpl,
        &AudioVolumeGroupManager_setMicMutePersistentImpl,
        &AudioVolumeGroupManager_isPersistentMicMuteImpl,
        &AudioVolumeGroupManager_isMicrophoneMute0Impl,
        &AudioVolumeGroupManager_isMicrophoneMute1Impl,
        &AudioVolumeGroupManager_isMicrophoneMuteSyncImpl,
        &AudioVolumeGroupManager_onMicStateChangeImpl,
        &AudioVolumeGroupManager_offMicStateChangeImpl,
        &AudioVolumeGroupManager_isVolumeUnadjustableImpl,
        &AudioVolumeGroupManager_adjustVolumeByStep0Impl,
        &AudioVolumeGroupManager_adjustVolumeByStep1Impl,
        &AudioVolumeGroupManager_adjustSystemVolumeByStep0Impl,
        &AudioVolumeGroupManager_adjustSystemVolumeByStep1Impl,
        &AudioVolumeGroupManager_getSystemVolumeInDb0Impl,
        &AudioVolumeGroupManager_getSystemVolumeInDb1Impl,
        &AudioVolumeGroupManager_getSystemVolumeInDbSyncImpl,
        &AudioVolumeGroupManager_getMaxAmplitudeForInputDeviceImpl,
        &AudioVolumeGroupManager_getMaxAmplitudeForOutputDeviceImpl,
    };
    return &instance;
}
const OH_AUDIO_AudioSpatializationManagerModifier* OH_AUDIO_AudioSpatializationManagerModifierImpl() {
    const static OH_AUDIO_AudioSpatializationManagerModifier instance = {
        &AudioSpatializationManager_constructImpl,
        &AudioSpatializationManager_destructImpl,
        &AudioSpatializationManager_isSpatializationSupportedImpl,
        &AudioSpatializationManager_isSpatializationSupportedForDeviceImpl,
        &AudioSpatializationManager_isHeadTrackingSupportedImpl,
        &AudioSpatializationManager_isHeadTrackingSupportedForDeviceImpl,
        &AudioSpatializationManager_setSpatializationEnabled0Impl,
        &AudioSpatializationManager_setSpatializationEnabled1Impl,
        &AudioSpatializationManager_setSpatializationEnabled2Impl,
        &AudioSpatializationManager_isSpatializationEnabled0Impl,
        &AudioSpatializationManager_isSpatializationEnabled1Impl,
        &AudioSpatializationManager_onSpatializationEnabledChangeImpl,
        &AudioSpatializationManager_onSpatializationEnabledChangeForAnyDeviceImpl,
        &AudioSpatializationManager_offSpatializationEnabledChangeImpl,
        &AudioSpatializationManager_offSpatializationEnabledChangeForAnyDeviceImpl,
        &AudioSpatializationManager_setHeadTrackingEnabled0Impl,
        &AudioSpatializationManager_setHeadTrackingEnabled1Impl,
        &AudioSpatializationManager_setHeadTrackingEnabled2Impl,
        &AudioSpatializationManager_isHeadTrackingEnabled0Impl,
        &AudioSpatializationManager_isHeadTrackingEnabled1Impl,
        &AudioSpatializationManager_onHeadTrackingEnabledChangeImpl,
        &AudioSpatializationManager_onHeadTrackingEnabledChangeForAnyDeviceImpl,
        &AudioSpatializationManager_offHeadTrackingEnabledChangeImpl,
        &AudioSpatializationManager_offHeadTrackingEnabledChangeForAnyDeviceImpl,
        &AudioSpatializationManager_updateSpatialDeviceStateImpl,
        &AudioSpatializationManager_setSpatializationSceneTypeImpl,
        &AudioSpatializationManager_getSpatializationSceneTypeImpl,
    };
    return &instance;
}
const OH_AUDIO_AudioRendererModifier* OH_AUDIO_AudioRendererModifierImpl() {
    const static OH_AUDIO_AudioRendererModifier instance = {
        &AudioRenderer_constructImpl,
        &AudioRenderer_destructImpl,
        &AudioRenderer_getRendererInfo0Impl,
        &AudioRenderer_getRendererInfo1Impl,
        &AudioRenderer_getRendererInfoSyncImpl,
        &AudioRenderer_getStreamInfo0Impl,
        &AudioRenderer_getStreamInfo1Impl,
        &AudioRenderer_getStreamInfoSyncImpl,
        &AudioRenderer_getAudioStreamId0Impl,
        &AudioRenderer_getAudioStreamId1Impl,
        &AudioRenderer_getAudioStreamIdSyncImpl,
        &AudioRenderer_getAudioEffectMode0Impl,
        &AudioRenderer_getAudioEffectMode1Impl,
        &AudioRenderer_setAudioEffectMode0Impl,
        &AudioRenderer_setAudioEffectMode1Impl,
        &AudioRenderer_start0Impl,
        &AudioRenderer_start1Impl,
        &AudioRenderer_write0Impl,
        &AudioRenderer_write1Impl,
        &AudioRenderer_getAudioTime0Impl,
        &AudioRenderer_getAudioTime1Impl,
        &AudioRenderer_getAudioTimeSyncImpl,
        &AudioRenderer_drain0Impl,
        &AudioRenderer_drain1Impl,
        &AudioRenderer_flushImpl,
        &AudioRenderer_pause0Impl,
        &AudioRenderer_pause1Impl,
        &AudioRenderer_stop0Impl,
        &AudioRenderer_stop1Impl,
        &AudioRenderer_release0Impl,
        &AudioRenderer_release1Impl,
        &AudioRenderer_getBufferSize0Impl,
        &AudioRenderer_getBufferSize1Impl,
        &AudioRenderer_getBufferSizeSyncImpl,
        &AudioRenderer_setRenderRate0Impl,
        &AudioRenderer_setRenderRate1Impl,
        &AudioRenderer_setSpeedImpl,
        &AudioRenderer_getRenderRate0Impl,
        &AudioRenderer_getRenderRate1Impl,
        &AudioRenderer_getRenderRateSyncImpl,
        &AudioRenderer_getSpeedImpl,
        &AudioRenderer_setInterruptMode0Impl,
        &AudioRenderer_setInterruptMode1Impl,
        &AudioRenderer_setInterruptModeSyncImpl,
        &AudioRenderer_setVolume0Impl,
        &AudioRenderer_setVolume1Impl,
        &AudioRenderer_getVolumeImpl,
        &AudioRenderer_setVolumeWithRampImpl,
        &AudioRenderer_getMinStreamVolume0Impl,
        &AudioRenderer_getMinStreamVolume1Impl,
        &AudioRenderer_getMinStreamVolumeSyncImpl,
        &AudioRenderer_getMaxStreamVolume0Impl,
        &AudioRenderer_getMaxStreamVolume1Impl,
        &AudioRenderer_getMaxStreamVolumeSyncImpl,
        &AudioRenderer_getUnderflowCount0Impl,
        &AudioRenderer_getUnderflowCount1Impl,
        &AudioRenderer_getUnderflowCountSyncImpl,
        &AudioRenderer_getCurrentOutputDevices0Impl,
        &AudioRenderer_getCurrentOutputDevices1Impl,
        &AudioRenderer_getCurrentOutputDevicesSyncImpl,
        &AudioRenderer_setChannelBlendModeImpl,
        &AudioRenderer_setSilentModeAndMixWithOthersImpl,
        &AudioRenderer_getSilentModeAndMixWithOthersImpl,
        &AudioRenderer_setDefaultOutputDeviceImpl,
        &AudioRenderer_onAudioInterruptImpl,
        &AudioRenderer_onMarkReachImpl,
        &AudioRenderer_offMarkReachImpl,
        &AudioRenderer_onPeriodReachImpl,
        &AudioRenderer_offPeriodReachImpl,
        &AudioRenderer_onStateChangeImpl,
        &AudioRenderer_onOutputDeviceChangeImpl,
        &AudioRenderer_onOutputDeviceChangeWithInfoImpl,
        &AudioRenderer_offOutputDeviceChangeImpl,
        &AudioRenderer_offOutputDeviceChangeWithInfoImpl,
        &AudioRenderer_onWriteDataImpl,
        &AudioRenderer_offWriteDataImpl,
        &AudioRenderer_getStateImpl,
    };
    return &instance;
}
const OH_AUDIO_AudioCapturerModifier* OH_AUDIO_AudioCapturerModifierImpl() {
    const static OH_AUDIO_AudioCapturerModifier instance = {
        &AudioCapturer_constructImpl,
        &AudioCapturer_destructImpl,
        &AudioCapturer_getCapturerInfo0Impl,
        &AudioCapturer_getCapturerInfo1Impl,
        &AudioCapturer_getCapturerInfoSyncImpl,
        &AudioCapturer_getStreamInfo0Impl,
        &AudioCapturer_getStreamInfo1Impl,
        &AudioCapturer_getStreamInfoSyncImpl,
        &AudioCapturer_getAudioStreamId0Impl,
        &AudioCapturer_getAudioStreamId1Impl,
        &AudioCapturer_getAudioStreamIdSyncImpl,
        &AudioCapturer_start0Impl,
        &AudioCapturer_start1Impl,
        &AudioCapturer_read0Impl,
        &AudioCapturer_read1Impl,
        &AudioCapturer_getAudioTime0Impl,
        &AudioCapturer_getAudioTime1Impl,
        &AudioCapturer_getAudioTimeSyncImpl,
        &AudioCapturer_stop0Impl,
        &AudioCapturer_stop1Impl,
        &AudioCapturer_release0Impl,
        &AudioCapturer_release1Impl,
        &AudioCapturer_getBufferSize0Impl,
        &AudioCapturer_getBufferSize1Impl,
        &AudioCapturer_getBufferSizeSyncImpl,
        &AudioCapturer_getCurrentInputDevicesImpl,
        &AudioCapturer_getCurrentAudioCapturerChangeInfoImpl,
        &AudioCapturer_getOverflowCountImpl,
        &AudioCapturer_getOverflowCountSyncImpl,
        &AudioCapturer_onMarkReachImpl,
        &AudioCapturer_offMarkReachImpl,
        &AudioCapturer_onPeriodReachImpl,
        &AudioCapturer_offPeriodReachImpl,
        &AudioCapturer_onStateChangeImpl,
        &AudioCapturer_onAudioInterruptImpl,
        &AudioCapturer_offAudioInterruptImpl,
        &AudioCapturer_onInputDeviceChangeImpl,
        &AudioCapturer_offInputDeviceChangeImpl,
        &AudioCapturer_onAudioCapturerChangeImpl,
        &AudioCapturer_offAudioCapturerChangeImpl,
        &AudioCapturer_onReadDataImpl,
        &AudioCapturer_offReadDataImpl,
        &AudioCapturer_getStateImpl,
    };
    return &instance;
}
const OH_AUDIO_AsrProcessingControllerModifier* OH_AUDIO_AsrProcessingControllerModifierImpl() {
    const static OH_AUDIO_AsrProcessingControllerModifier instance = {
        &AsrProcessingController_constructImpl,
        &AsrProcessingController_destructImpl,
        &AsrProcessingController_setAsrAecModeImpl,
        &AsrProcessingController_getAsrAecModeImpl,
        &AsrProcessingController_setAsrNoiseSuppressionModeImpl,
        &AsrProcessingController_getAsrNoiseSuppressionModeImpl,
        &AsrProcessingController_isWhisperingImpl,
        &AsrProcessingController_setAsrVoiceControlModeImpl,
        &AsrProcessingController_setAsrVoiceMuteModeImpl,
        &AsrProcessingController_setAsrWhisperDetectionModeImpl,
        &AsrProcessingController_getAsrWhisperDetectionModeImpl,
    };
    return &instance;
}
const OH_AUDIO_TonePlayerModifier* OH_AUDIO_TonePlayerModifierImpl() {
    const static OH_AUDIO_TonePlayerModifier instance = {
        &TonePlayer_constructImpl,
        &TonePlayer_destructImpl,
        &TonePlayer_load0Impl,
        &TonePlayer_load1Impl,
        &TonePlayer_start0Impl,
        &TonePlayer_start1Impl,
        &TonePlayer_stop0Impl,
        &TonePlayer_stop1Impl,
        &TonePlayer_release0Impl,
        &TonePlayer_release1Impl,
    };
    return &instance;
}
const OH_AUDIO_Modifier* OH_AUDIO_ModifierImpl() {
    const static OH_AUDIO_Modifier instance = {
        &GlobalScope_ohos_multimedia_audio_getAudioManagerImpl,
        &GlobalScope_ohos_multimedia_audio_createAudioCapturer0Impl,
        &GlobalScope_ohos_multimedia_audio_createAudioCapturer1Impl,
        &GlobalScope_ohos_multimedia_audio_createAudioRenderer0Impl,
        &GlobalScope_ohos_multimedia_audio_createAudioRenderer1Impl,
        &GlobalScope_ohos_multimedia_audio_createTonePlayer0Impl,
        &GlobalScope_ohos_multimedia_audio_createTonePlayer1Impl,
        &GlobalScope_ohos_multimedia_audio_createAsrProcessingControllerImpl,
    };
    return &instance;
}
const OH_AUDIO_API* GetAUDIOAPIImpl(int version) {
    const static OH_AUDIO_API api = {
        1, // version
        &OH_AUDIO_AudioManagerModifierImpl,
        &OH_AUDIO_AudioRoutingManagerModifierImpl,
        &OH_AUDIO_AudioStreamManagerModifierImpl,
        &OH_AUDIO_AudioSessionManagerModifierImpl,
        &OH_AUDIO_AudioVolumeManagerModifierImpl,
        &OH_AUDIO_AudioVolumeGroupManagerModifierImpl,
        &OH_AUDIO_AudioSpatializationManagerModifierImpl,
        &OH_AUDIO_AudioRendererModifierImpl,
        &OH_AUDIO_AudioCapturerModifierImpl,
        &OH_AUDIO_AsrProcessingControllerModifierImpl,
        &OH_AUDIO_TonePlayerModifierImpl,
        &OH_AUDIO_ModifierImpl,
    };
    if (version != api.version) return nullptr;
    return &api;
}

// Accessors

OH_NativePointer impl_AudioManager_ctor() {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->construct();
}
KOALA_INTEROP_0(AudioManager_ctor, OH_NativePointer)
OH_NativePointer impl_AudioManager_getFinalizer() {
        return (OH_NativePointer) GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->destruct;
}
KOALA_INTEROP_0(AudioManager_getFinalizer, OH_NativePointer)
void impl_AudioManager_setVolume0(OH_NativePointer thisPtr, OH_Int32 volumeType, KInteropNumber volume, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->setVolume0(thisPtr, &tmp, (const OH_Number*) (&volume), (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V5(AudioManager_setVolume0, OH_NativePointer, OH_Int32, KInteropNumber, uint8_t*, int32_t)
void impl_AudioManager_setVolume1(OH_NativePointer thisPtr, OH_Int32 volumeType, KInteropNumber volume) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);    
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->setVolume1(thisPtr, &tmp, (const OH_Number*) (&volume));
}
KOALA_INTEROP_V3(AudioManager_setVolume1, OH_NativePointer, OH_Int32, KInteropNumber)
void impl_AudioManager_getVolume0(OH_NativePointer thisPtr, OH_Int32 volumeType, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getVolume0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioManager_getVolume0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
OH_Int32 impl_AudioManager_getVolume1(OH_NativePointer thisPtr, OH_Int32 volumeType) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);    
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getVolume1(thisPtr, &tmp).i32;
}
KOALA_INTEROP_2(AudioManager_getVolume1, OH_Int32, OH_NativePointer, OH_Int32)
void impl_AudioManager_getMinVolume0(OH_NativePointer thisPtr, OH_Int32 volumeType, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getMinVolume0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioManager_getMinVolume0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
OH_Int32 impl_AudioManager_getMinVolume1(OH_NativePointer thisPtr, OH_Int32 volumeType) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);    
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getMinVolume1(thisPtr, &tmp).i32;
}
KOALA_INTEROP_2(AudioManager_getMinVolume1, OH_Int32, OH_NativePointer, OH_Int32)
void impl_AudioManager_getMaxVolume0(OH_NativePointer thisPtr, OH_Int32 volumeType, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getMaxVolume0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioManager_getMaxVolume0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
OH_Int32 impl_AudioManager_getMaxVolume1(OH_NativePointer thisPtr, OH_Int32 volumeType) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);    
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getMaxVolume1(thisPtr, &tmp).i32;
}
KOALA_INTEROP_2(AudioManager_getMaxVolume1, OH_Int32, OH_NativePointer, OH_Int32)
void impl_AudioManager_getDevices0(OH_NativePointer thisPtr, OH_Int32 deviceFlag, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_AudioDeviceDescriptors_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioDeviceDescriptors_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioDeviceDescriptors_Void))))};;
        const OH_AUDIO_audio_DeviceFlag tmp = static_cast<OH_AUDIO_audio_DeviceFlag>(deviceFlag);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getDevices0(thisPtr, &tmp, (const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioManager_getDevices0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
OH_NativePointer impl_AudioManager_getDevices1(OH_NativePointer thisPtr, OH_Int32 deviceFlag) {
    const OH_AUDIO_audio_DeviceFlag tmp = static_cast<OH_AUDIO_audio_DeviceFlag>(deviceFlag);    
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getDevices1(thisPtr, &tmp);
}
KOALA_INTEROP_2(AudioManager_getDevices1, OH_NativePointer, OH_NativePointer, OH_Int32)
void impl_AudioManager_mute0(OH_NativePointer thisPtr, OH_Int32 volumeType, OH_Boolean mute, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType); 
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->mute0(thisPtr, &tmp, &mute, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V5(AudioManager_mute0, OH_NativePointer, OH_Int32, OH_Boolean, uint8_t*, int32_t)
void impl_AudioManager_mute1(OH_NativePointer thisPtr, OH_Int32 volumeType, OH_Boolean mute) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);     
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->mute1(thisPtr, &tmp, &mute);
}
KOALA_INTEROP_V3(AudioManager_mute1, OH_NativePointer, OH_Int32, OH_Boolean)
void impl_AudioManager_isMute0(OH_NativePointer thisPtr, OH_Int32 volumeType, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Boolean_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Boolean_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Boolean_Void))))};;
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType); 
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->isMute0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Boolean_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioManager_isMute0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
OH_Boolean impl_AudioManager_isMute1(OH_NativePointer thisPtr, OH_Int32 volumeType) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);     
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->isMute1(thisPtr, &tmp);
}
KOALA_INTEROP_2(AudioManager_isMute1, OH_Boolean, OH_NativePointer, OH_Int32)
void impl_AudioManager_isActive0(OH_NativePointer thisPtr, OH_Int32 volumeType, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Boolean_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Boolean_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Boolean_Void))))};;
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType); 
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->isActive0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Boolean_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioManager_isActive0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
OH_Boolean impl_AudioManager_isActive1(OH_NativePointer thisPtr, OH_Int32 volumeType) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);     
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->isActive1(thisPtr, &tmp);
}
KOALA_INTEROP_2(AudioManager_isActive1, OH_Boolean, OH_NativePointer, OH_Int32)
void impl_AudioManager_setMicrophoneMute0(OH_NativePointer thisPtr, OH_Boolean mute, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->setMicrophoneMute0(thisPtr, &mute, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioManager_setMicrophoneMute0, OH_NativePointer, OH_Boolean, uint8_t*, int32_t)
void impl_AudioManager_setMicrophoneMute1(OH_NativePointer thisPtr, OH_Boolean mute) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->setMicrophoneMute1(thisPtr, &mute);
}
KOALA_INTEROP_V2(AudioManager_setMicrophoneMute1, OH_NativePointer, OH_Boolean)
void impl_AudioManager_isMicrophoneMute0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Boolean_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Boolean_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Boolean_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->isMicrophoneMute0(thisPtr, (const AUDIO_AsyncCallback_Boolean_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioManager_isMicrophoneMute0, OH_NativePointer, uint8_t*, int32_t)
OH_Boolean impl_AudioManager_isMicrophoneMute1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->isMicrophoneMute1(thisPtr);
}
KOALA_INTEROP_1(AudioManager_isMicrophoneMute1, OH_Boolean, OH_NativePointer)
void impl_AudioManager_setRingerMode0(OH_NativePointer thisPtr, OH_Int32 mode, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        const OH_AUDIO_audio_AudioRingMode tmp = static_cast<OH_AUDIO_audio_AudioRingMode>(mode);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->setRingerMode0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioManager_setRingerMode0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
void impl_AudioManager_setRingerMode1(OH_NativePointer thisPtr, OH_Int32 mode) {
    const OH_AUDIO_audio_AudioRingMode tmp = static_cast<OH_AUDIO_audio_AudioRingMode>(mode);    
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->setRingerMode1(thisPtr, &tmp);
}
KOALA_INTEROP_V2(AudioManager_setRingerMode1, OH_NativePointer, OH_Int32)
void impl_AudioManager_getRingerMode0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_AudioRingMode_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioRingMode_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioRingMode_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getRingerMode0(thisPtr, (const AUDIO_AsyncCallback_AudioRingMode_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioManager_getRingerMode0, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioManager_getRingerMode1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getRingerMode1(thisPtr);
}
KOALA_INTEROP_1(AudioManager_getRingerMode1, OH_NativePointer, OH_NativePointer)
void impl_AudioManager_setAudioParameter0(OH_NativePointer thisPtr, const KStringPtr& key, const KStringPtr& value, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->setAudioParameter0(thisPtr, (const OH_String*) (&key), (const OH_String*) (&value), (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V5(AudioManager_setAudioParameter0, OH_NativePointer, KStringPtr, KStringPtr, uint8_t*, int32_t)
void impl_AudioManager_setAudioParameter1(OH_NativePointer thisPtr, const KStringPtr& key, const KStringPtr& value) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->setAudioParameter1(thisPtr, (const OH_String*) (&key), (const OH_String*) (&value));
}
KOALA_INTEROP_V3(AudioManager_setAudioParameter1, OH_NativePointer, KStringPtr, KStringPtr)
void impl_AudioManager_getAudioParameter0(OH_NativePointer thisPtr, const KStringPtr& key, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_String_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_String result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_String_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_String result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_String_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getAudioParameter0(thisPtr, (const OH_String*) (&key), (const AUDIO_AsyncCallback_String_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioManager_getAudioParameter0, OH_NativePointer, KStringPtr, uint8_t*, int32_t)
void impl_AudioManager_getAudioParameter1(OH_NativePointer thisPtr, const KStringPtr& key) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getAudioParameter1(thisPtr, (const OH_String*) (&key));
}
KOALA_INTEROP_V2(AudioManager_getAudioParameter1, OH_NativePointer, KStringPtr)
void impl_AudioManager_setExtraParameters(OH_NativePointer thisPtr, const KStringPtr& mainKey, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const OH_Int32 kvpairs_value_buf_size = thisDeserializer.readInt32();
        Map_String_String kvpairs_value_buf = {};
        thisDeserializer.resizeMap<Map_String_String, OH_String, OH_String>(&kvpairs_value_buf, kvpairs_value_buf_size);
        for (int kvpairs_value_buf_i = 0; kvpairs_value_buf_i < kvpairs_value_buf_size; kvpairs_value_buf_i++) {
            const OH_String kvpairs_value_buf_key = static_cast<OH_String>(thisDeserializer.readString());
            const OH_String kvpairs_value_buf_value = static_cast<OH_String>(thisDeserializer.readString());
            kvpairs_value_buf.keys[kvpairs_value_buf_i] = kvpairs_value_buf_key;
            kvpairs_value_buf.values[kvpairs_value_buf_i] = kvpairs_value_buf_value;
        }
        Map_String_String kvpairs_value = kvpairs_value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->setExtraParameters(thisPtr, (const OH_String*) (&mainKey), (const Map_String_String*)&kvpairs_value);
}
KOALA_INTEROP_V4(AudioManager_setExtraParameters, OH_NativePointer, KStringPtr, uint8_t*, int32_t)
OH_NativePointer impl_AudioManager_getExtraParameters(OH_NativePointer thisPtr, const KStringPtr& mainKey, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto subKeys_value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_Array_String subKeys_value_buf = {};
        subKeys_value_buf.tag = subKeys_value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (subKeys_value_buf_runtimeType))
        {
            const OH_Int32 subKeys_value_buf__length = thisDeserializer.readInt32();
            Array_String subKeys_value_buf_ = {};
            thisDeserializer.resizeArray<std::decay<decltype(subKeys_value_buf_)>::type,
        std::decay<decltype(*subKeys_value_buf_.array)>::type>(&subKeys_value_buf_, subKeys_value_buf__length);
            for (int subKeys_value_buf__i = 0; subKeys_value_buf__i < subKeys_value_buf__length; subKeys_value_buf__i++) {
                subKeys_value_buf_.array[subKeys_value_buf__i] = static_cast<OH_String>(thisDeserializer.readString());
            }
            subKeys_value_buf.value = subKeys_value_buf_;
        }
        Opt_Array_String subKeys_value = subKeys_value_buf;;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getExtraParameters(thisPtr, (const OH_String*) (&mainKey), (const Opt_Array_String*)&subKeys_value);
}
KOALA_INTEROP_4(AudioManager_getExtraParameters, OH_NativePointer, OH_NativePointer, KStringPtr, uint8_t*, int32_t)
void impl_AudioManager_setDeviceActive0(OH_NativePointer thisPtr, OH_Int32 deviceType, OH_Boolean active, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        const OH_AUDIO_audio_ActiveDeviceType tmp = static_cast<OH_AUDIO_audio_ActiveDeviceType>(deviceType);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->setDeviceActive0(thisPtr, &tmp, &active, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V5(AudioManager_setDeviceActive0, OH_NativePointer, OH_Int32, OH_Boolean, uint8_t*, int32_t)
void impl_AudioManager_setDeviceActive1(OH_NativePointer thisPtr, OH_Int32 deviceType, OH_Boolean active) {
    const OH_AUDIO_audio_ActiveDeviceType tmp = static_cast<OH_AUDIO_audio_ActiveDeviceType>(deviceType);    
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->setDeviceActive1(thisPtr, &tmp, &active);
}
KOALA_INTEROP_V3(AudioManager_setDeviceActive1, OH_NativePointer, OH_Int32, OH_Boolean)
void impl_AudioManager_isDeviceActive0(OH_NativePointer thisPtr, OH_Int32 deviceType, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Boolean_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Boolean_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Boolean_Void))))};;
        const OH_AUDIO_audio_ActiveDeviceType tmp = static_cast<OH_AUDIO_audio_ActiveDeviceType>(deviceType);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->isDeviceActive0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Boolean_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioManager_isDeviceActive0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
OH_Boolean impl_AudioManager_isDeviceActive1(OH_NativePointer thisPtr, OH_Int32 deviceType) {
    const OH_AUDIO_audio_ActiveDeviceType tmp = static_cast<OH_AUDIO_audio_ActiveDeviceType>(deviceType);    
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->isDeviceActive1(thisPtr, &tmp);
}
KOALA_INTEROP_2(AudioManager_isDeviceActive1, OH_Boolean, OH_NativePointer, OH_Int32)
void impl_AudioManager_onVolumeChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_VolumeEvent_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_VolumeEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_VolumeEvent_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_VolumeEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_VolumeEvent_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->onVolumeChange(thisPtr, (const AUDIO_Callback_VolumeEvent_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioManager_onVolumeChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioManager_onRingerModeChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_AudioRingMode_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioRingMode_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioRingMode_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->onRingerModeChange(thisPtr, (const AUDIO_Callback_AudioRingMode_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioManager_onRingerModeChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioManager_setAudioScene0(OH_NativePointer thisPtr, OH_Int32 scene, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        const OH_AUDIO_audio_AudioScene tmp = static_cast<OH_AUDIO_audio_AudioScene>(scene);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->setAudioScene0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioManager_setAudioScene0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
void impl_AudioManager_setAudioScene1(OH_NativePointer thisPtr, OH_Int32 scene) {
    const OH_AUDIO_audio_AudioScene tmp = static_cast<OH_AUDIO_audio_AudioScene>(scene);    
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->setAudioScene1(thisPtr, &tmp);
}
KOALA_INTEROP_V2(AudioManager_setAudioScene1, OH_NativePointer, OH_Int32)
void impl_AudioManager_getAudioScene0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_AudioScene_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioScene result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioScene_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, OH_AUDIO_audio_AudioScene result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioScene_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getAudioScene0(thisPtr, (const AUDIO_AsyncCallback_AudioScene_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioManager_getAudioScene0, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioManager_getAudioScene1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getAudioScene1(thisPtr);
}
KOALA_INTEROP_1(AudioManager_getAudioScene1, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioManager_getAudioSceneSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getAudioSceneSync(thisPtr);
}
KOALA_INTEROP_1(AudioManager_getAudioSceneSync, OH_NativePointer, OH_NativePointer)
void impl_AudioManager_onDeviceChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_DeviceChangeAction_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_DeviceChangeAction parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_DeviceChangeAction_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_DeviceChangeAction parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_DeviceChangeAction_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->onDeviceChange(thisPtr, (const AUDIO_Callback_DeviceChangeAction_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioManager_onDeviceChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioManager_offDeviceChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_DeviceChangeAction_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_DeviceChangeAction parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_DeviceChangeAction_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_DeviceChangeAction parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_DeviceChangeAction_Void))))};
        }
        Opt_AUDIO_Callback_DeviceChangeAction_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->offDeviceChange(thisPtr, (const Opt_AUDIO_Callback_DeviceChangeAction_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioManager_offDeviceChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioManager_onInterrupt(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioInterrupt interrupt_value = thisDeserializer.readAudioInterrupt();;
        AUDIO_Callback_InterruptAction_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_InterruptAction parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_InterruptAction_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_InterruptAction parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_InterruptAction_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->onInterrupt(thisPtr, (const OH_AUDIO_AudioInterrupt*)&interrupt_value, (const AUDIO_Callback_InterruptAction_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioManager_onInterrupt, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioManager_offInterrupt(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioInterrupt interrupt_value = thisDeserializer.readAudioInterrupt();;
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_InterruptAction_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_InterruptAction parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_InterruptAction_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_InterruptAction parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_InterruptAction_Void))))};
        }
        Opt_AUDIO_Callback_InterruptAction_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->offInterrupt(thisPtr, (const OH_AUDIO_AudioInterrupt*)&interrupt_value, (const Opt_AUDIO_Callback_InterruptAction_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioManager_offInterrupt, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioManager_getVolumeManager(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getVolumeManager(thisPtr);
}
KOALA_INTEROP_1(AudioManager_getVolumeManager, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioManager_getStreamManager(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getStreamManager(thisPtr);
}
KOALA_INTEROP_1(AudioManager_getStreamManager, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioManager_getRoutingManager(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getRoutingManager(thisPtr);
}
KOALA_INTEROP_1(AudioManager_getRoutingManager, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioManager_getSessionManager(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getSessionManager(thisPtr);
}
KOALA_INTEROP_1(AudioManager_getSessionManager, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioManager_getSpatializationManager(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->getSpatializationManager(thisPtr);
}
KOALA_INTEROP_1(AudioManager_getSpatializationManager, OH_NativePointer, OH_NativePointer)
void impl_AudioManager_disableSafeMediaVolume(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioManager()->disableSafeMediaVolume(thisPtr);
}
KOALA_INTEROP_V1(AudioManager_disableSafeMediaVolume, OH_NativePointer)
OH_NativePointer impl_AudioRoutingManager_ctor() {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->construct();
}
KOALA_INTEROP_0(AudioRoutingManager_ctor, OH_NativePointer)
OH_NativePointer impl_AudioRoutingManager_getFinalizer() {
        return (OH_NativePointer) GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->destruct;
}
KOALA_INTEROP_0(AudioRoutingManager_getFinalizer, OH_NativePointer)
void impl_AudioRoutingManager_getDevices0(OH_NativePointer thisPtr, OH_Int32 deviceFlag, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_AudioDeviceDescriptors_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioDeviceDescriptors_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioDeviceDescriptors_Void))))};;
        const OH_AUDIO_audio_DeviceFlag tmp = static_cast<OH_AUDIO_audio_DeviceFlag>(deviceFlag);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->getDevices0(thisPtr, &tmp, (const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioRoutingManager_getDevices0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
OH_NativePointer impl_AudioRoutingManager_getDevices1(OH_NativePointer thisPtr, OH_Int32 deviceFlag) {
    const OH_AUDIO_audio_DeviceFlag tmp = static_cast<OH_AUDIO_audio_DeviceFlag>(deviceFlag);    
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->getDevices1(thisPtr, &tmp);
}
KOALA_INTEROP_2(AudioRoutingManager_getDevices1, OH_NativePointer, OH_NativePointer, OH_Int32)
OH_NativePointer impl_AudioRoutingManager_getDevicesSync(OH_NativePointer thisPtr, OH_Int32 deviceFlag) {
    const OH_AUDIO_audio_DeviceFlag tmp = static_cast<OH_AUDIO_audio_DeviceFlag>(deviceFlag);    
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->getDevicesSync(thisPtr, &tmp);
}
KOALA_INTEROP_2(AudioRoutingManager_getDevicesSync, OH_NativePointer, OH_NativePointer, OH_Int32)
void impl_AudioRoutingManager_onDeviceChange(OH_NativePointer thisPtr, OH_Int32 deviceFlag, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_DeviceChangeAction_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_DeviceChangeAction parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_DeviceChangeAction_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_DeviceChangeAction parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_DeviceChangeAction_Void))))};;
        const OH_AUDIO_audio_DeviceFlag tmp = static_cast<OH_AUDIO_audio_DeviceFlag>(deviceFlag);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->onDeviceChange(thisPtr, &tmp, (const AUDIO_Callback_DeviceChangeAction_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioRoutingManager_onDeviceChange, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
void impl_AudioRoutingManager_offDeviceChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_DeviceChangeAction_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_DeviceChangeAction parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_DeviceChangeAction_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_DeviceChangeAction parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_DeviceChangeAction_Void))))};
        }
        Opt_AUDIO_Callback_DeviceChangeAction_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->offDeviceChange(thisPtr, (const Opt_AUDIO_Callback_DeviceChangeAction_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRoutingManager_offDeviceChange, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioRoutingManager_getAvailableDevices(OH_NativePointer thisPtr, OH_Int32 deviceUsage) {
    const OH_AUDIO_audio_DeviceUsage tmp = static_cast<OH_AUDIO_audio_DeviceUsage>(deviceUsage);    
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->getAvailableDevices(thisPtr, &tmp);
}
KOALA_INTEROP_2(AudioRoutingManager_getAvailableDevices, OH_NativePointer, OH_NativePointer, OH_Int32)
void impl_AudioRoutingManager_onAvailableDeviceChange(OH_NativePointer thisPtr, OH_Int32 deviceUsage, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_DeviceChangeAction_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_DeviceChangeAction parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_DeviceChangeAction_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_DeviceChangeAction parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_DeviceChangeAction_Void))))};;
        const OH_AUDIO_audio_DeviceUsage tmp = static_cast<OH_AUDIO_audio_DeviceUsage>(deviceUsage); 
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->onAvailableDeviceChange(thisPtr, &tmp, (const AUDIO_Callback_DeviceChangeAction_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioRoutingManager_onAvailableDeviceChange, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
void impl_AudioRoutingManager_offAvailableDeviceChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_DeviceChangeAction_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_DeviceChangeAction parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_DeviceChangeAction_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_DeviceChangeAction parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_DeviceChangeAction_Void))))};
        }
        Opt_AUDIO_Callback_DeviceChangeAction_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->offAvailableDeviceChange(thisPtr, (const Opt_AUDIO_Callback_DeviceChangeAction_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRoutingManager_offAvailableDeviceChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRoutingManager_setCommunicationDevice0(OH_NativePointer thisPtr, OH_Int32 deviceType, OH_Boolean active, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        const OH_AUDIO_audio_CommunicationDeviceType tmp = static_cast<OH_AUDIO_audio_CommunicationDeviceType>(deviceType);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->setCommunicationDevice0(thisPtr, &tmp, &active, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V5(AudioRoutingManager_setCommunicationDevice0, OH_NativePointer, OH_Int32, OH_Boolean, uint8_t*, int32_t)
void impl_AudioRoutingManager_setCommunicationDevice1(OH_NativePointer thisPtr, OH_Int32 deviceType, OH_Boolean active) {
    const OH_AUDIO_audio_CommunicationDeviceType tmp = static_cast<OH_AUDIO_audio_CommunicationDeviceType>(deviceType);    
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->setCommunicationDevice1(thisPtr, &tmp, &active);
}
KOALA_INTEROP_V3(AudioRoutingManager_setCommunicationDevice1, OH_NativePointer, OH_Int32, OH_Boolean)
void impl_AudioRoutingManager_isCommunicationDeviceActive0(OH_NativePointer thisPtr, OH_Int32 deviceType, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Boolean_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Boolean_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Boolean_Void))))};;
        const OH_AUDIO_audio_CommunicationDeviceType tmp = static_cast<OH_AUDIO_audio_CommunicationDeviceType>(deviceType);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->isCommunicationDeviceActive0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Boolean_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioRoutingManager_isCommunicationDeviceActive0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
OH_Boolean impl_AudioRoutingManager_isCommunicationDeviceActive1(OH_NativePointer thisPtr, OH_Int32 deviceType) {
    const OH_AUDIO_audio_CommunicationDeviceType tmp = static_cast<OH_AUDIO_audio_CommunicationDeviceType>(deviceType);    
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->isCommunicationDeviceActive1(thisPtr, &tmp);
}
KOALA_INTEROP_2(AudioRoutingManager_isCommunicationDeviceActive1, OH_Boolean, OH_NativePointer, OH_Int32)
OH_Boolean impl_AudioRoutingManager_isCommunicationDeviceActiveSync(OH_NativePointer thisPtr, OH_Int32 deviceType) {
    const OH_AUDIO_audio_CommunicationDeviceType tmp = static_cast<OH_AUDIO_audio_CommunicationDeviceType>(deviceType);    
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->isCommunicationDeviceActiveSync(thisPtr, &tmp);
}
KOALA_INTEROP_2(AudioRoutingManager_isCommunicationDeviceActiveSync, OH_Boolean, OH_NativePointer, OH_Int32)
void impl_AudioRoutingManager_selectOutputDevice0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const OH_Int32 outputAudioDevices_value_buf_length = thisDeserializer.readInt32();
        Array_CustomObject outputAudioDevices_value_buf = {};
        thisDeserializer.resizeArray<std::decay<decltype(outputAudioDevices_value_buf)>::type,
        std::decay<decltype(*outputAudioDevices_value_buf.array)>::type>(&outputAudioDevices_value_buf, outputAudioDevices_value_buf_length);
        for (int outputAudioDevices_value_buf_i = 0; outputAudioDevices_value_buf_i < outputAudioDevices_value_buf_length; outputAudioDevices_value_buf_i++) {
            outputAudioDevices_value_buf.array[outputAudioDevices_value_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>"));
        }
        Array_CustomObject outputAudioDevices_value = outputAudioDevices_value_buf;;
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->selectOutputDevice0(thisPtr, (const Array_CustomObject*)&outputAudioDevices_value, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRoutingManager_selectOutputDevice0, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRoutingManager_selectOutputDevice1(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const OH_Int32 outputAudioDevices_value_buf_length = thisDeserializer.readInt32();
        Array_CustomObject outputAudioDevices_value_buf = {};
        thisDeserializer.resizeArray<std::decay<decltype(outputAudioDevices_value_buf)>::type,
        std::decay<decltype(*outputAudioDevices_value_buf.array)>::type>(&outputAudioDevices_value_buf, outputAudioDevices_value_buf_length);
        for (int outputAudioDevices_value_buf_i = 0; outputAudioDevices_value_buf_i < outputAudioDevices_value_buf_length; outputAudioDevices_value_buf_i++) {
            outputAudioDevices_value_buf.array[outputAudioDevices_value_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>"));
        }
        Array_CustomObject outputAudioDevices_value = outputAudioDevices_value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->selectOutputDevice1(thisPtr, (const Array_CustomObject*)&outputAudioDevices_value);
}
KOALA_INTEROP_V3(AudioRoutingManager_selectOutputDevice1, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRoutingManager_selectOutputDeviceByFilter0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioRendererFilter filter_value = thisDeserializer.readAudioRendererFilter();;
        const OH_Int32 outputAudioDevices_value_buf_length = thisDeserializer.readInt32();
        Array_CustomObject outputAudioDevices_value_buf = {};
        thisDeserializer.resizeArray<std::decay<decltype(outputAudioDevices_value_buf)>::type,
        std::decay<decltype(*outputAudioDevices_value_buf.array)>::type>(&outputAudioDevices_value_buf, outputAudioDevices_value_buf_length);
        for (int outputAudioDevices_value_buf_i = 0; outputAudioDevices_value_buf_i < outputAudioDevices_value_buf_length; outputAudioDevices_value_buf_i++) {
            outputAudioDevices_value_buf.array[outputAudioDevices_value_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>"));
        }
        Array_CustomObject outputAudioDevices_value = outputAudioDevices_value_buf;;
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->selectOutputDeviceByFilter0(thisPtr, (const OH_AUDIO_AudioRendererFilter*)&filter_value, (const Array_CustomObject*)&outputAudioDevices_value, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRoutingManager_selectOutputDeviceByFilter0, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRoutingManager_selectOutputDeviceByFilter1(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioRendererFilter filter_value = thisDeserializer.readAudioRendererFilter();;
        const OH_Int32 outputAudioDevices_value_buf_length = thisDeserializer.readInt32();
        Array_CustomObject outputAudioDevices_value_buf = {};
        thisDeserializer.resizeArray<std::decay<decltype(outputAudioDevices_value_buf)>::type,
        std::decay<decltype(*outputAudioDevices_value_buf.array)>::type>(&outputAudioDevices_value_buf, outputAudioDevices_value_buf_length);
        for (int outputAudioDevices_value_buf_i = 0; outputAudioDevices_value_buf_i < outputAudioDevices_value_buf_length; outputAudioDevices_value_buf_i++) {
            outputAudioDevices_value_buf.array[outputAudioDevices_value_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>"));
        }
        Array_CustomObject outputAudioDevices_value = outputAudioDevices_value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->selectOutputDeviceByFilter1(thisPtr, (const OH_AUDIO_AudioRendererFilter*)&filter_value, (const Array_CustomObject*)&outputAudioDevices_value);
}
KOALA_INTEROP_V3(AudioRoutingManager_selectOutputDeviceByFilter1, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRoutingManager_selectInputDevice0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const OH_Int32 inputAudioDevices_value_buf_length = thisDeserializer.readInt32();
        Array_CustomObject inputAudioDevices_value_buf = {};
        thisDeserializer.resizeArray<std::decay<decltype(inputAudioDevices_value_buf)>::type,
        std::decay<decltype(*inputAudioDevices_value_buf.array)>::type>(&inputAudioDevices_value_buf, inputAudioDevices_value_buf_length);
        for (int inputAudioDevices_value_buf_i = 0; inputAudioDevices_value_buf_i < inputAudioDevices_value_buf_length; inputAudioDevices_value_buf_i++) {
            inputAudioDevices_value_buf.array[inputAudioDevices_value_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>"));
        }
        Array_CustomObject inputAudioDevices_value = inputAudioDevices_value_buf;;
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->selectInputDevice0(thisPtr, (const Array_CustomObject*)&inputAudioDevices_value, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRoutingManager_selectInputDevice0, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRoutingManager_selectInputDevice1(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const OH_Int32 inputAudioDevices_value_buf_length = thisDeserializer.readInt32();
        Array_CustomObject inputAudioDevices_value_buf = {};
        thisDeserializer.resizeArray<std::decay<decltype(inputAudioDevices_value_buf)>::type,
        std::decay<decltype(*inputAudioDevices_value_buf.array)>::type>(&inputAudioDevices_value_buf, inputAudioDevices_value_buf_length);
        for (int inputAudioDevices_value_buf_i = 0; inputAudioDevices_value_buf_i < inputAudioDevices_value_buf_length; inputAudioDevices_value_buf_i++) {
            inputAudioDevices_value_buf.array[inputAudioDevices_value_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>"));
        }
        Array_CustomObject inputAudioDevices_value = inputAudioDevices_value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->selectInputDevice1(thisPtr, (const Array_CustomObject*)&inputAudioDevices_value);
}
KOALA_INTEROP_V3(AudioRoutingManager_selectInputDevice1, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRoutingManager_selectInputDeviceByFilter(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioCapturerFilter filter_value = thisDeserializer.readAudioCapturerFilter();;
        const OH_Int32 inputAudioDevices_value_buf_length = thisDeserializer.readInt32();
        Array_CustomObject inputAudioDevices_value_buf = {};
        thisDeserializer.resizeArray<std::decay<decltype(inputAudioDevices_value_buf)>::type,
        std::decay<decltype(*inputAudioDevices_value_buf.array)>::type>(&inputAudioDevices_value_buf, inputAudioDevices_value_buf_length);
        for (int inputAudioDevices_value_buf_i = 0; inputAudioDevices_value_buf_i < inputAudioDevices_value_buf_length; inputAudioDevices_value_buf_i++) {
            inputAudioDevices_value_buf.array[inputAudioDevices_value_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>"));
        }
        Array_CustomObject inputAudioDevices_value = inputAudioDevices_value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->selectInputDeviceByFilter(thisPtr, (const OH_AUDIO_AudioCapturerFilter*)&filter_value, (const Array_CustomObject*)&inputAudioDevices_value);
}
KOALA_INTEROP_V3(AudioRoutingManager_selectInputDeviceByFilter, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRoutingManager_getPreferOutputDeviceForRendererInfo0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioRendererInfo rendererInfo_value = thisDeserializer.readAudioRendererInfo();;
        AUDIO_AsyncCallback_AudioDeviceDescriptors_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioDeviceDescriptors_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioDeviceDescriptors_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->getPreferOutputDeviceForRendererInfo0(thisPtr, (const OH_AUDIO_AudioRendererInfo*)&rendererInfo_value, (const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRoutingManager_getPreferOutputDeviceForRendererInfo0, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioRoutingManager_getPreferOutputDeviceForRendererInfo1(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioRendererInfo rendererInfo_value = thisDeserializer.readAudioRendererInfo();;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->getPreferOutputDeviceForRendererInfo1(thisPtr, (const OH_AUDIO_AudioRendererInfo*)&rendererInfo_value);
}
KOALA_INTEROP_3(AudioRoutingManager_getPreferOutputDeviceForRendererInfo1, OH_NativePointer, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioRoutingManager_getPreferredOutputDeviceForRendererInfoSync(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioRendererInfo rendererInfo_value = thisDeserializer.readAudioRendererInfo();;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->getPreferredOutputDeviceForRendererInfoSync(thisPtr, (const OH_AUDIO_AudioRendererInfo*)&rendererInfo_value);
}
KOALA_INTEROP_3(AudioRoutingManager_getPreferredOutputDeviceForRendererInfoSync, OH_NativePointer, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioRoutingManager_getPreferredOutputDeviceByFilter(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioRendererFilter filter_value = thisDeserializer.readAudioRendererFilter();;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->getPreferredOutputDeviceByFilter(thisPtr, (const OH_AUDIO_AudioRendererFilter*)&filter_value);
}
KOALA_INTEROP_3(AudioRoutingManager_getPreferredOutputDeviceByFilter, OH_NativePointer, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRoutingManager_onPreferOutputDeviceChangeForRendererInfo(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioRendererInfo rendererInfo_value = thisDeserializer.readAudioRendererInfo();;
        AUDIO_Callback_AudioDeviceDescriptors_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioDeviceDescriptors_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioDeviceDescriptors_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->onPreferOutputDeviceChangeForRendererInfo(thisPtr, (const OH_AUDIO_AudioRendererInfo*)&rendererInfo_value, (const AUDIO_Callback_AudioDeviceDescriptors_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRoutingManager_onPreferOutputDeviceChangeForRendererInfo, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRoutingManager_offPreferOutputDeviceChangeForRendererInfo(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_AudioDeviceDescriptors_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioDeviceDescriptors_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioDeviceDescriptors_Void))))};
        }
        Opt_AUDIO_Callback_AudioDeviceDescriptors_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->offPreferOutputDeviceChangeForRendererInfo(thisPtr, (const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRoutingManager_offPreferOutputDeviceChangeForRendererInfo, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRoutingManager_getPreferredInputDeviceForCapturerInfo0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioCapturerInfo capturerInfo_value = thisDeserializer.readAudioCapturerInfo();;
        AUDIO_AsyncCallback_AudioDeviceDescriptors_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioDeviceDescriptors_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioDeviceDescriptors_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->getPreferredInputDeviceForCapturerInfo0(thisPtr, (const OH_AUDIO_AudioCapturerInfo*)&capturerInfo_value, (const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRoutingManager_getPreferredInputDeviceForCapturerInfo0, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioRoutingManager_getPreferredInputDeviceForCapturerInfo1(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioCapturerInfo capturerInfo_value = thisDeserializer.readAudioCapturerInfo();;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->getPreferredInputDeviceForCapturerInfo1(thisPtr, (const OH_AUDIO_AudioCapturerInfo*)&capturerInfo_value);
}
KOALA_INTEROP_3(AudioRoutingManager_getPreferredInputDeviceForCapturerInfo1, OH_NativePointer, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioRoutingManager_getPreferredInputDeviceByFilter(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioCapturerFilter filter_value = thisDeserializer.readAudioCapturerFilter();;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->getPreferredInputDeviceByFilter(thisPtr, (const OH_AUDIO_AudioCapturerFilter*)&filter_value);
}
KOALA_INTEROP_3(AudioRoutingManager_getPreferredInputDeviceByFilter, OH_NativePointer, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRoutingManager_onPreferredInputDeviceChangeForCapturerInfo(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioCapturerInfo capturerInfo_value = thisDeserializer.readAudioCapturerInfo();;
        AUDIO_Callback_AudioDeviceDescriptors_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioDeviceDescriptors_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioDeviceDescriptors_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->onPreferredInputDeviceChangeForCapturerInfo(thisPtr, (const OH_AUDIO_AudioCapturerInfo*)&capturerInfo_value, (const AUDIO_Callback_AudioDeviceDescriptors_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRoutingManager_onPreferredInputDeviceChangeForCapturerInfo, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRoutingManager_offPreferredInputDeviceChangeForCapturerInfo(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_AudioDeviceDescriptors_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioDeviceDescriptors_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioDeviceDescriptors_Void))))};
        }
        Opt_AUDIO_Callback_AudioDeviceDescriptors_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->offPreferredInputDeviceChangeForCapturerInfo(thisPtr, (const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRoutingManager_offPreferredInputDeviceChangeForCapturerInfo, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioRoutingManager_getPreferredInputDeviceForCapturerInfoSync(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioCapturerInfo capturerInfo_value = thisDeserializer.readAudioCapturerInfo();;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->getPreferredInputDeviceForCapturerInfoSync(thisPtr, (const OH_AUDIO_AudioCapturerInfo*)&capturerInfo_value);
}
KOALA_INTEROP_3(AudioRoutingManager_getPreferredInputDeviceForCapturerInfoSync, OH_NativePointer, OH_NativePointer, uint8_t*, int32_t)
OH_Boolean impl_AudioRoutingManager_isMicBlockDetectionSupported(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->isMicBlockDetectionSupported(thisPtr);
}
KOALA_INTEROP_1(AudioRoutingManager_isMicBlockDetectionSupported, OH_Boolean, OH_NativePointer)
void impl_AudioRoutingManager_onMicBlockStatusChanged(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_DeviceBlockStatusInfo_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_DeviceBlockStatusInfo parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_DeviceBlockStatusInfo_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_DeviceBlockStatusInfo parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_DeviceBlockStatusInfo_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->onMicBlockStatusChanged(thisPtr, (const AUDIO_Callback_DeviceBlockStatusInfo_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRoutingManager_onMicBlockStatusChanged, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRoutingManager_offMicBlockStatusChanged(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_DeviceBlockStatusInfo_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_DeviceBlockStatusInfo parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_DeviceBlockStatusInfo_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_DeviceBlockStatusInfo parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_DeviceBlockStatusInfo_Void))))};
        }
        Opt_AUDIO_Callback_DeviceBlockStatusInfo_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRoutingManager()->offMicBlockStatusChanged(thisPtr, (const Opt_AUDIO_Callback_DeviceBlockStatusInfo_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRoutingManager_offMicBlockStatusChanged, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioStreamManager_ctor() {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->construct();
}
KOALA_INTEROP_0(AudioStreamManager_ctor, OH_NativePointer)
OH_NativePointer impl_AudioStreamManager_getFinalizer() {
        return (OH_NativePointer) GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->destruct;
}
KOALA_INTEROP_0(AudioStreamManager_getFinalizer, OH_NativePointer)
void impl_AudioStreamManager_getCurrentAudioRendererInfoArray0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioRendererChangeInfoArray_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioRendererChangeInfoArray_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->getCurrentAudioRendererInfoArray0(thisPtr, (const AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioStreamManager_getCurrentAudioRendererInfoArray0, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioStreamManager_getCurrentAudioRendererInfoArray1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->getCurrentAudioRendererInfoArray1(thisPtr);
}
KOALA_INTEROP_1(AudioStreamManager_getCurrentAudioRendererInfoArray1, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioStreamManager_getCurrentAudioRendererInfoArraySync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->getCurrentAudioRendererInfoArraySync(thisPtr);
}
KOALA_INTEROP_1(AudioStreamManager_getCurrentAudioRendererInfoArraySync, OH_NativePointer, OH_NativePointer)
void impl_AudioStreamManager_getCurrentAudioCapturerInfoArray0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioCapturerChangeInfoArray_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioCapturerChangeInfoArray_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->getCurrentAudioCapturerInfoArray0(thisPtr, (const AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioStreamManager_getCurrentAudioCapturerInfoArray0, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioStreamManager_getCurrentAudioCapturerInfoArray1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->getCurrentAudioCapturerInfoArray1(thisPtr);
}
KOALA_INTEROP_1(AudioStreamManager_getCurrentAudioCapturerInfoArray1, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioStreamManager_getCurrentAudioCapturerInfoArraySync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->getCurrentAudioCapturerInfoArraySync(thisPtr);
}
KOALA_INTEROP_1(AudioStreamManager_getCurrentAudioCapturerInfoArraySync, OH_NativePointer, OH_NativePointer)
void impl_AudioStreamManager_getAudioEffectInfoArray0(OH_NativePointer thisPtr, OH_Int32 usage, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_AudioEffectInfoArray_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioEffectInfoArray_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioEffectInfoArray_Void))))};;
        const OH_AUDIO_audio_StreamUsage tmp = static_cast<OH_AUDIO_audio_StreamUsage>(usage);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->getAudioEffectInfoArray0(thisPtr, &tmp, (const AUDIO_AsyncCallback_AudioEffectInfoArray_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioStreamManager_getAudioEffectInfoArray0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
OH_NativePointer impl_AudioStreamManager_getAudioEffectInfoArray1(OH_NativePointer thisPtr, OH_Int32 usage) {
    const OH_AUDIO_audio_StreamUsage tmp = static_cast<OH_AUDIO_audio_StreamUsage>(usage);    
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->getAudioEffectInfoArray1(thisPtr, &tmp);
}
KOALA_INTEROP_2(AudioStreamManager_getAudioEffectInfoArray1, OH_NativePointer, OH_NativePointer, OH_Int32)
OH_NativePointer impl_AudioStreamManager_getAudioEffectInfoArraySync(OH_NativePointer thisPtr, OH_Int32 usage) {
    const OH_AUDIO_audio_StreamUsage tmp = static_cast<OH_AUDIO_audio_StreamUsage>(usage);    
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->getAudioEffectInfoArraySync(thisPtr, &tmp);
}
KOALA_INTEROP_2(AudioStreamManager_getAudioEffectInfoArraySync, OH_NativePointer, OH_NativePointer, OH_Int32)
void impl_AudioStreamManager_onAudioRendererChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_AudioRendererChangeInfoArray_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioRendererChangeInfoArray_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioRendererChangeInfoArray_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->onAudioRendererChange(thisPtr, (const AUDIO_Callback_AudioRendererChangeInfoArray_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioStreamManager_onAudioRendererChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioStreamManager_offAudioRendererChange(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->offAudioRendererChange(thisPtr);
}
KOALA_INTEROP_V1(AudioStreamManager_offAudioRendererChange, OH_NativePointer)
void impl_AudioStreamManager_onAudioCapturerChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_AudioCapturerChangeInfoArray_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioCapturerChangeInfoArray_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioCapturerChangeInfoArray_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->onAudioCapturerChange(thisPtr, (const AUDIO_Callback_AudioCapturerChangeInfoArray_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioStreamManager_onAudioCapturerChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioStreamManager_offAudioCapturerChange(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->offAudioCapturerChange(thisPtr);
}
KOALA_INTEROP_V1(AudioStreamManager_offAudioCapturerChange, OH_NativePointer)
void impl_AudioStreamManager_isActive0(OH_NativePointer thisPtr, OH_Int32 volumeType, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Boolean_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Boolean_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Boolean_Void))))};;
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType); 
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->isActive0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Boolean_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioStreamManager_isActive0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
OH_Boolean impl_AudioStreamManager_isActive1(OH_NativePointer thisPtr, OH_Int32 volumeType) {
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType); 
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->isActive1(thisPtr, &tmp);
}
KOALA_INTEROP_2(AudioStreamManager_isActive1, OH_Boolean, OH_NativePointer, OH_Int32)
OH_Boolean impl_AudioStreamManager_isActiveSync(OH_NativePointer thisPtr, OH_Int32 volumeType) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);     
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioStreamManager()->isActiveSync(thisPtr, &tmp);
}
KOALA_INTEROP_2(AudioStreamManager_isActiveSync, OH_Boolean, OH_NativePointer, OH_Int32)
OH_NativePointer impl_AudioSessionManager_ctor() {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSessionManager()->construct();
}
KOALA_INTEROP_0(AudioSessionManager_ctor, OH_NativePointer)
OH_NativePointer impl_AudioSessionManager_getFinalizer() {
        return (OH_NativePointer) GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSessionManager()->destruct;
}
KOALA_INTEROP_0(AudioSessionManager_getFinalizer, OH_NativePointer)
void impl_AudioSessionManager_activateAudioSession(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioSessionStrategy strategy_value = thisDeserializer.readAudioSessionStrategy();;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSessionManager()->activateAudioSession(thisPtr, (const OH_AUDIO_AudioSessionStrategy*)&strategy_value);
}
KOALA_INTEROP_V3(AudioSessionManager_activateAudioSession, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioSessionManager_deactivateAudioSession(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSessionManager()->deactivateAudioSession(thisPtr);
}
KOALA_INTEROP_V1(AudioSessionManager_deactivateAudioSession, OH_NativePointer)
OH_Boolean impl_AudioSessionManager_isAudioSessionActivated(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSessionManager()->isAudioSessionActivated(thisPtr);
}
KOALA_INTEROP_1(AudioSessionManager_isAudioSessionActivated, OH_Boolean, OH_NativePointer)
void impl_AudioSessionManager_onAudioSessionDeactivated(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_AudioSessionDeactivatedEvent_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioSessionDeactivatedEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioSessionDeactivatedEvent_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioSessionDeactivatedEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioSessionDeactivatedEvent_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSessionManager()->onAudioSessionDeactivated(thisPtr, (const AUDIO_Callback_AudioSessionDeactivatedEvent_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioSessionManager_onAudioSessionDeactivated, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioSessionManager_offAudioSessionDeactivated(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_AudioSessionDeactivatedEvent_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioSessionDeactivatedEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioSessionDeactivatedEvent_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioSessionDeactivatedEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioSessionDeactivatedEvent_Void))))};
        }
        Opt_AUDIO_Callback_AudioSessionDeactivatedEvent_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSessionManager()->offAudioSessionDeactivated(thisPtr, (const Opt_AUDIO_Callback_AudioSessionDeactivatedEvent_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioSessionManager_offAudioSessionDeactivated, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioVolumeManager_ctor() {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeManager()->construct();
}
KOALA_INTEROP_0(AudioVolumeManager_ctor, OH_NativePointer)
OH_NativePointer impl_AudioVolumeManager_getFinalizer() {
        return (OH_NativePointer) GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeManager()->destruct;
}
KOALA_INTEROP_0(AudioVolumeManager_getFinalizer, OH_NativePointer)
void impl_AudioVolumeManager_getVolumeGroupInfos0(OH_NativePointer thisPtr, const KStringPtr& networkId, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_VolumeGroupInfos_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_VolumeGroupInfos_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_VolumeGroupInfos_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeManager()->getVolumeGroupInfos0(thisPtr, (const OH_String*) (&networkId), (const AUDIO_AsyncCallback_VolumeGroupInfos_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioVolumeManager_getVolumeGroupInfos0, OH_NativePointer, KStringPtr, uint8_t*, int32_t)
OH_NativePointer impl_AudioVolumeManager_getVolumeGroupInfos1(OH_NativePointer thisPtr, const KStringPtr& networkId) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeManager()->getVolumeGroupInfos1(thisPtr, (const OH_String*) (&networkId));
}
KOALA_INTEROP_2(AudioVolumeManager_getVolumeGroupInfos1, OH_NativePointer, OH_NativePointer, KStringPtr)
OH_NativePointer impl_AudioVolumeManager_getVolumeGroupInfosSync(OH_NativePointer thisPtr, const KStringPtr& networkId) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeManager()->getVolumeGroupInfosSync(thisPtr, (const OH_String*) (&networkId));
}
KOALA_INTEROP_2(AudioVolumeManager_getVolumeGroupInfosSync, OH_NativePointer, OH_NativePointer, KStringPtr)
void impl_AudioVolumeManager_getVolumeGroupManager0(OH_NativePointer thisPtr, KInteropNumber groupId, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_AudioVolumeGroupManager_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioVolumeGroupManager result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioVolumeGroupManager_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioVolumeGroupManager result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioVolumeGroupManager_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeManager()->getVolumeGroupManager0(thisPtr, (const OH_Number*) (&groupId), (const AUDIO_AsyncCallback_AudioVolumeGroupManager_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioVolumeManager_getVolumeGroupManager0, OH_NativePointer, KInteropNumber, uint8_t*, int32_t)
OH_NativePointer impl_AudioVolumeManager_getVolumeGroupManager1(OH_NativePointer thisPtr, KInteropNumber groupId) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeManager()->getVolumeGroupManager1(thisPtr, (const OH_Number*) (&groupId));
}
KOALA_INTEROP_2(AudioVolumeManager_getVolumeGroupManager1, OH_NativePointer, OH_NativePointer, KInteropNumber)
OH_NativePointer impl_AudioVolumeManager_getVolumeGroupManagerSync(OH_NativePointer thisPtr, KInteropNumber groupId) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeManager()->getVolumeGroupManagerSync(thisPtr, (const OH_Number*) (&groupId));
}
KOALA_INTEROP_2(AudioVolumeManager_getVolumeGroupManagerSync, OH_NativePointer, OH_NativePointer, KInteropNumber)
void impl_AudioVolumeManager_onVolumeChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_VolumeEvent_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_VolumeEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_VolumeEvent_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_VolumeEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_VolumeEvent_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeManager()->onVolumeChange(thisPtr, (const AUDIO_Callback_VolumeEvent_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioVolumeManager_onVolumeChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioVolumeManager_offVolumeChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_VolumeEvent_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_VolumeEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_VolumeEvent_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_VolumeEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_VolumeEvent_Void))))};
        }
        Opt_AUDIO_Callback_VolumeEvent_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeManager()->offVolumeChange(thisPtr, (const Opt_AUDIO_Callback_VolumeEvent_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioVolumeManager_offVolumeChange, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioVolumeGroupManager_ctor() {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->construct();
}
KOALA_INTEROP_0(AudioVolumeGroupManager_ctor, OH_NativePointer)
OH_NativePointer impl_AudioVolumeGroupManager_getFinalizer() {
        return (OH_NativePointer) GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->destruct;
}
KOALA_INTEROP_0(AudioVolumeGroupManager_getFinalizer, OH_NativePointer)
void impl_AudioVolumeGroupManager_setVolume0(OH_NativePointer thisPtr, OH_Int32 volumeType, KInteropNumber volume, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType); 
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->setVolume0(thisPtr, &tmp, (const OH_Number*) (&volume), (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V5(AudioVolumeGroupManager_setVolume0, OH_NativePointer, OH_Int32, KInteropNumber, uint8_t*, int32_t)
void impl_AudioVolumeGroupManager_setVolume1(OH_NativePointer thisPtr, OH_Int32 volumeType, KInteropNumber volume) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);     
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->setVolume1(thisPtr, &tmp, (const OH_Number*) (&volume));
}
KOALA_INTEROP_V3(AudioVolumeGroupManager_setVolume1, OH_NativePointer, OH_Int32, KInteropNumber)
void impl_AudioVolumeGroupManager_setVolumeWithFlag(OH_NativePointer thisPtr, OH_Int32 volumeType, KInteropNumber volume, KInteropNumber flags) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);     
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->setVolumeWithFlag(thisPtr, &tmp, (const OH_Number*) (&volume), (const OH_Number*) (&flags));
}
KOALA_INTEROP_V4(AudioVolumeGroupManager_setVolumeWithFlag, OH_NativePointer, OH_Int32, KInteropNumber, KInteropNumber)
OH_NativePointer impl_AudioVolumeGroupManager_getActiveVolumeTypeSync(OH_NativePointer thisPtr, KInteropNumber uid) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getActiveVolumeTypeSync(thisPtr, (const OH_Number*) (&uid));
}
KOALA_INTEROP_2(AudioVolumeGroupManager_getActiveVolumeTypeSync, OH_NativePointer, OH_NativePointer, KInteropNumber)
void impl_AudioVolumeGroupManager_getVolume0(OH_NativePointer thisPtr, OH_Int32 volumeType, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType); 
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getVolume0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioVolumeGroupManager_getVolume0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
OH_Int32 impl_AudioVolumeGroupManager_getVolume1(OH_NativePointer thisPtr, OH_Int32 volumeType) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);     
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getVolume1(thisPtr, &tmp).i32;
}
KOALA_INTEROP_2(AudioVolumeGroupManager_getVolume1, OH_Int32, OH_NativePointer, OH_Int32)
OH_Int32 impl_AudioVolumeGroupManager_getVolumeSync(OH_NativePointer thisPtr, OH_Int32 volumeType) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);     
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getVolumeSync(thisPtr, &tmp).i32;
}
KOALA_INTEROP_2(AudioVolumeGroupManager_getVolumeSync, OH_Int32, OH_NativePointer, OH_Int32)
void impl_AudioVolumeGroupManager_getMinVolume0(OH_NativePointer thisPtr, OH_Int32 volumeType, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType); 
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getMinVolume0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioVolumeGroupManager_getMinVolume0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
OH_Int32 impl_AudioVolumeGroupManager_getMinVolume1(OH_NativePointer thisPtr, OH_Int32 volumeType) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);     
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getMinVolume1(thisPtr, &tmp).i32;
}
KOALA_INTEROP_2(AudioVolumeGroupManager_getMinVolume1, OH_Int32, OH_NativePointer, OH_Int32)
OH_Int32 impl_AudioVolumeGroupManager_getMinVolumeSync(OH_NativePointer thisPtr, OH_Int32 volumeType) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);     
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getMinVolumeSync(thisPtr, &tmp).i32;
}
KOALA_INTEROP_2(AudioVolumeGroupManager_getMinVolumeSync, OH_Int32, OH_NativePointer, OH_Int32)
void impl_AudioVolumeGroupManager_getMaxVolume0(OH_NativePointer thisPtr, OH_Int32 volumeType, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType); 
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getMaxVolume0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioVolumeGroupManager_getMaxVolume0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
OH_Int32 impl_AudioVolumeGroupManager_getMaxVolume1(OH_NativePointer thisPtr, OH_Int32 volumeType) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);     
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getMaxVolume1(thisPtr, &tmp).i32;
}
KOALA_INTEROP_2(AudioVolumeGroupManager_getMaxVolume1, OH_Int32, OH_NativePointer, OH_Int32)
OH_Int32 impl_AudioVolumeGroupManager_getMaxVolumeSync(OH_NativePointer thisPtr, OH_Int32 volumeType) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);     
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getMaxVolumeSync(thisPtr, &tmp).i32;
}
KOALA_INTEROP_2(AudioVolumeGroupManager_getMaxVolumeSync, OH_Int32, OH_NativePointer, OH_Int32)
void impl_AudioVolumeGroupManager_mute0(OH_NativePointer thisPtr, OH_Int32 volumeType, OH_Boolean mute, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType); 
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->mute0(thisPtr, &tmp, &mute, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V5(AudioVolumeGroupManager_mute0, OH_NativePointer, OH_Int32, OH_Boolean, uint8_t*, int32_t)
void impl_AudioVolumeGroupManager_mute1(OH_NativePointer thisPtr, OH_Int32 volumeType, OH_Boolean mute) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);     
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->mute1(thisPtr, &tmp, &mute);
}
KOALA_INTEROP_V3(AudioVolumeGroupManager_mute1, OH_NativePointer, OH_Int32, OH_Boolean)
void impl_AudioVolumeGroupManager_isMute0(OH_NativePointer thisPtr, OH_Int32 volumeType, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Boolean_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Boolean_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Boolean_Void))))};;
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType); 
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->isMute0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Boolean_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioVolumeGroupManager_isMute0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
OH_Boolean impl_AudioVolumeGroupManager_isMute1(OH_NativePointer thisPtr, OH_Int32 volumeType) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);     
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->isMute1(thisPtr, &tmp);
}
KOALA_INTEROP_2(AudioVolumeGroupManager_isMute1, OH_Boolean, OH_NativePointer, OH_Int32)
OH_Boolean impl_AudioVolumeGroupManager_isMuteSync(OH_NativePointer thisPtr, OH_Int32 volumeType) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);     
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->isMuteSync(thisPtr, &tmp);
}
KOALA_INTEROP_2(AudioVolumeGroupManager_isMuteSync, OH_Boolean, OH_NativePointer, OH_Int32)
void impl_AudioVolumeGroupManager_setRingerMode0(OH_NativePointer thisPtr, OH_Int32 mode, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        const OH_AUDIO_audio_AudioRingMode tmp = static_cast<OH_AUDIO_audio_AudioRingMode>(mode);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->setRingerMode0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioVolumeGroupManager_setRingerMode0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
void impl_AudioVolumeGroupManager_setRingerMode1(OH_NativePointer thisPtr, OH_Int32 mode) {
    const OH_AUDIO_audio_AudioRingMode tmp = static_cast<OH_AUDIO_audio_AudioRingMode>(mode);    
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->setRingerMode1(thisPtr, &tmp);
}
KOALA_INTEROP_V2(AudioVolumeGroupManager_setRingerMode1, OH_NativePointer, OH_Int32)
void impl_AudioVolumeGroupManager_getRingerMode0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_AudioRingMode_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioRingMode_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioRingMode_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getRingerMode0(thisPtr, (const AUDIO_AsyncCallback_AudioRingMode_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioVolumeGroupManager_getRingerMode0, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioVolumeGroupManager_getRingerMode1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getRingerMode1(thisPtr);
}
KOALA_INTEROP_1(AudioVolumeGroupManager_getRingerMode1, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioVolumeGroupManager_getRingerModeSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getRingerModeSync(thisPtr);
}
KOALA_INTEROP_1(AudioVolumeGroupManager_getRingerModeSync, OH_NativePointer, OH_NativePointer)
void impl_AudioVolumeGroupManager_onRingerModeChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_AudioRingMode_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioRingMode_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioRingMode_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->onRingerModeChange(thisPtr, (const AUDIO_Callback_AudioRingMode_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioVolumeGroupManager_onRingerModeChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioVolumeGroupManager_setMicrophoneMute0(OH_NativePointer thisPtr, OH_Boolean mute, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->setMicrophoneMute0(thisPtr, &mute, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioVolumeGroupManager_setMicrophoneMute0, OH_NativePointer, OH_Boolean, uint8_t*, int32_t)
void impl_AudioVolumeGroupManager_setMicrophoneMute1(OH_NativePointer thisPtr, OH_Boolean mute) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->setMicrophoneMute1(thisPtr, &mute);
}
KOALA_INTEROP_V2(AudioVolumeGroupManager_setMicrophoneMute1, OH_NativePointer, OH_Boolean)
void impl_AudioVolumeGroupManager_setMicMute(OH_NativePointer thisPtr, OH_Boolean mute) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->setMicMute(thisPtr, &mute);
}
KOALA_INTEROP_V2(AudioVolumeGroupManager_setMicMute, OH_NativePointer, OH_Boolean)
void impl_AudioVolumeGroupManager_setMicMutePersistent(OH_NativePointer thisPtr, OH_Boolean mute, OH_Int32 type) {
    const OH_AUDIO_audio_PolicyType tmp = static_cast<OH_AUDIO_audio_PolicyType>(type);
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->setMicMutePersistent(thisPtr, &mute, &tmp);
}
KOALA_INTEROP_V3(AudioVolumeGroupManager_setMicMutePersistent, OH_NativePointer, OH_Boolean, OH_Int32)
OH_Boolean impl_AudioVolumeGroupManager_isPersistentMicMute(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->isPersistentMicMute(thisPtr);
}
KOALA_INTEROP_1(AudioVolumeGroupManager_isPersistentMicMute, OH_Boolean, OH_NativePointer)
void impl_AudioVolumeGroupManager_isMicrophoneMute0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Boolean_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Boolean_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Boolean_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->isMicrophoneMute0(thisPtr, (const AUDIO_AsyncCallback_Boolean_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioVolumeGroupManager_isMicrophoneMute0, OH_NativePointer, uint8_t*, int32_t)
OH_Boolean impl_AudioVolumeGroupManager_isMicrophoneMute1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->isMicrophoneMute1(thisPtr);
}
KOALA_INTEROP_1(AudioVolumeGroupManager_isMicrophoneMute1, OH_Boolean, OH_NativePointer)
OH_Boolean impl_AudioVolumeGroupManager_isMicrophoneMuteSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->isMicrophoneMuteSync(thisPtr);
}
KOALA_INTEROP_1(AudioVolumeGroupManager_isMicrophoneMuteSync, OH_Boolean, OH_NativePointer)
void impl_AudioVolumeGroupManager_onMicStateChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_MicStateChangeEvent_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_MicStateChangeEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_MicStateChangeEvent_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_MicStateChangeEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_MicStateChangeEvent_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->onMicStateChange(thisPtr, (const AUDIO_Callback_MicStateChangeEvent_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioVolumeGroupManager_onMicStateChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioVolumeGroupManager_offMicStateChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_MicStateChangeEvent_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_MicStateChangeEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_MicStateChangeEvent_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_MicStateChangeEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_MicStateChangeEvent_Void))))};
        }
        Opt_AUDIO_Callback_MicStateChangeEvent_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->offMicStateChange(thisPtr, (const Opt_AUDIO_Callback_MicStateChangeEvent_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioVolumeGroupManager_offMicStateChange, OH_NativePointer, uint8_t*, int32_t)
OH_Boolean impl_AudioVolumeGroupManager_isVolumeUnadjustable(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->isVolumeUnadjustable(thisPtr);
}
KOALA_INTEROP_1(AudioVolumeGroupManager_isVolumeUnadjustable, OH_Boolean, OH_NativePointer)
void impl_AudioVolumeGroupManager_adjustVolumeByStep0(OH_NativePointer thisPtr, OH_Int32 adjustType, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        const OH_AUDIO_audio_VolumeAdjustType tmp = static_cast<OH_AUDIO_audio_VolumeAdjustType>(adjustType);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->adjustVolumeByStep0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioVolumeGroupManager_adjustVolumeByStep0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
void impl_AudioVolumeGroupManager_adjustVolumeByStep1(OH_NativePointer thisPtr, OH_Int32 adjustType) {
    const OH_AUDIO_audio_VolumeAdjustType tmp = static_cast<OH_AUDIO_audio_VolumeAdjustType>(adjustType);    
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->adjustVolumeByStep1(thisPtr, &tmp);
}
KOALA_INTEROP_V2(AudioVolumeGroupManager_adjustVolumeByStep1, OH_NativePointer, OH_Int32)
void impl_AudioVolumeGroupManager_adjustSystemVolumeByStep0(OH_NativePointer thisPtr, OH_Int32 volumeType, OH_Int32 adjustType, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType); 
        const OH_AUDIO_audio_VolumeAdjustType tmp2 = static_cast<OH_AUDIO_audio_VolumeAdjustType>(adjustType);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->adjustSystemVolumeByStep0(thisPtr, &tmp, &tmp2, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V5(AudioVolumeGroupManager_adjustSystemVolumeByStep0, OH_NativePointer, OH_Int32, OH_Int32, uint8_t*, int32_t)
void impl_AudioVolumeGroupManager_adjustSystemVolumeByStep1(OH_NativePointer thisPtr, OH_Int32 volumeType, OH_Int32 adjustType) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);     
    const OH_AUDIO_audio_VolumeAdjustType tmp2 = static_cast<OH_AUDIO_audio_VolumeAdjustType>(adjustType);
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->adjustSystemVolumeByStep1(thisPtr, &tmp, &tmp2);
}
KOALA_INTEROP_V3(AudioVolumeGroupManager_adjustSystemVolumeByStep1, OH_NativePointer, OH_Int32, OH_Int32)
void impl_AudioVolumeGroupManager_getSystemVolumeInDb0(OH_NativePointer thisPtr, OH_Int32 volumeType, KInteropNumber volumeLevel, OH_Int32 device, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType); 
        const OH_AUDIO_audio_DeviceType tmp2 = static_cast<OH_AUDIO_audio_DeviceType>(device);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getSystemVolumeInDb0(thisPtr, &tmp, (const OH_Number*) (&volumeLevel), &tmp2, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V6(AudioVolumeGroupManager_getSystemVolumeInDb0, OH_NativePointer, OH_Int32, KInteropNumber, OH_Int32, uint8_t*, int32_t)
OH_Int32 impl_AudioVolumeGroupManager_getSystemVolumeInDb1(OH_NativePointer thisPtr, OH_Int32 volumeType, KInteropNumber volumeLevel, OH_Int32 device) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);
    const OH_AUDIO_audio_DeviceType tmp2 = static_cast<OH_AUDIO_audio_DeviceType>(device);
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getSystemVolumeInDb1(thisPtr, &tmp, (const OH_Number*) (&volumeLevel), &tmp2).i32;
}
KOALA_INTEROP_4(AudioVolumeGroupManager_getSystemVolumeInDb1, OH_Int32, OH_NativePointer, OH_Int32, KInteropNumber, OH_Int32)
OH_Int32 impl_AudioVolumeGroupManager_getSystemVolumeInDbSync(OH_NativePointer thisPtr, OH_Int32 volumeType, KInteropNumber volumeLevel, OH_Int32 device) {
    const OH_AUDIO_audio_AudioVolumeType tmp = static_cast<OH_AUDIO_audio_AudioVolumeType>(volumeType);
    const OH_AUDIO_audio_DeviceType tmp2 = static_cast<OH_AUDIO_audio_DeviceType>(device);  
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getSystemVolumeInDbSync(thisPtr, &tmp, (const OH_Number*) (&volumeLevel), &tmp2).i32;
}
KOALA_INTEROP_4(AudioVolumeGroupManager_getSystemVolumeInDbSync, OH_Int32, OH_NativePointer, OH_Int32, KInteropNumber, OH_Int32)
OH_Int32 impl_AudioVolumeGroupManager_getMaxAmplitudeForInputDevice(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioDeviceDescriptor inputDevice_value = thisDeserializer.readAudioDeviceDescriptor();;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getMaxAmplitudeForInputDevice(thisPtr, (const OH_AUDIO_AudioDeviceDescriptor*)&inputDevice_value).i32;
}
KOALA_INTEROP_3(AudioVolumeGroupManager_getMaxAmplitudeForInputDevice, OH_Int32, OH_NativePointer, uint8_t*, int32_t)
OH_Int32 impl_AudioVolumeGroupManager_getMaxAmplitudeForOutputDevice(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioDeviceDescriptor outputDevice_value = thisDeserializer.readAudioDeviceDescriptor();;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioVolumeGroupManager()->getMaxAmplitudeForOutputDevice(thisPtr, (const OH_AUDIO_AudioDeviceDescriptor*)&outputDevice_value).i32;
}
KOALA_INTEROP_3(AudioVolumeGroupManager_getMaxAmplitudeForOutputDevice, OH_Int32, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioSpatializationManager_ctor() {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->construct();
}
KOALA_INTEROP_0(AudioSpatializationManager_ctor, OH_NativePointer)
OH_NativePointer impl_AudioSpatializationManager_getFinalizer() {
        return (OH_NativePointer) GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->destruct;
}
KOALA_INTEROP_0(AudioSpatializationManager_getFinalizer, OH_NativePointer)
OH_Boolean impl_AudioSpatializationManager_isSpatializationSupported(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->isSpatializationSupported(thisPtr);
}
KOALA_INTEROP_1(AudioSpatializationManager_isSpatializationSupported, OH_Boolean, OH_NativePointer)
OH_Boolean impl_AudioSpatializationManager_isSpatializationSupportedForDevice(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioDeviceDescriptor deviceDescriptor_value = thisDeserializer.readAudioDeviceDescriptor();;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->isSpatializationSupportedForDevice(thisPtr, (const OH_AUDIO_AudioDeviceDescriptor*)&deviceDescriptor_value);
}
KOALA_INTEROP_3(AudioSpatializationManager_isSpatializationSupportedForDevice, OH_Boolean, OH_NativePointer, uint8_t*, int32_t)
OH_Boolean impl_AudioSpatializationManager_isHeadTrackingSupported(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->isHeadTrackingSupported(thisPtr);
}
KOALA_INTEROP_1(AudioSpatializationManager_isHeadTrackingSupported, OH_Boolean, OH_NativePointer)
OH_Boolean impl_AudioSpatializationManager_isHeadTrackingSupportedForDevice(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioDeviceDescriptor deviceDescriptor_value = thisDeserializer.readAudioDeviceDescriptor();;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->isHeadTrackingSupportedForDevice(thisPtr, (const OH_AUDIO_AudioDeviceDescriptor*)&deviceDescriptor_value);
}
KOALA_INTEROP_3(AudioSpatializationManager_isHeadTrackingSupportedForDevice, OH_Boolean, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioSpatializationManager_setSpatializationEnabled0(OH_NativePointer thisPtr, OH_Boolean enable, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->setSpatializationEnabled0(thisPtr, &enable, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioSpatializationManager_setSpatializationEnabled0, OH_NativePointer, OH_Boolean, uint8_t*, int32_t)
void impl_AudioSpatializationManager_setSpatializationEnabled1(OH_NativePointer thisPtr, OH_Boolean enable) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->setSpatializationEnabled1(thisPtr, &enable);
}
KOALA_INTEROP_V2(AudioSpatializationManager_setSpatializationEnabled1, OH_NativePointer, OH_Boolean)
void impl_AudioSpatializationManager_setSpatializationEnabled2(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength, OH_Boolean enabled) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioDeviceDescriptor deviceDescriptor_value = thisDeserializer.readAudioDeviceDescriptor();;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->setSpatializationEnabled2(thisPtr, (const OH_AUDIO_AudioDeviceDescriptor*)&deviceDescriptor_value, &enabled);
}
KOALA_INTEROP_V4(AudioSpatializationManager_setSpatializationEnabled2, OH_NativePointer, uint8_t*, int32_t, OH_Boolean)
OH_Boolean impl_AudioSpatializationManager_isSpatializationEnabled0(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->isSpatializationEnabled0(thisPtr);
}
KOALA_INTEROP_1(AudioSpatializationManager_isSpatializationEnabled0, OH_Boolean, OH_NativePointer)
OH_Boolean impl_AudioSpatializationManager_isSpatializationEnabled1(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioDeviceDescriptor deviceDescriptor_value = thisDeserializer.readAudioDeviceDescriptor();;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->isSpatializationEnabled1(thisPtr, (const OH_AUDIO_AudioDeviceDescriptor*)&deviceDescriptor_value);
}
KOALA_INTEROP_3(AudioSpatializationManager_isSpatializationEnabled1, OH_Boolean, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioSpatializationManager_onSpatializationEnabledChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_Boolean_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Boolean parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_Boolean_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Boolean parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_Boolean_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->onSpatializationEnabledChange(thisPtr, (const AUDIO_Callback_Boolean_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioSpatializationManager_onSpatializationEnabledChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioSpatializationManager_onSpatializationEnabledChangeForAnyDevice(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioSpatialEnabledStateForDevice parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioSpatialEnabledStateForDevice_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioSpatialEnabledStateForDevice parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioSpatialEnabledStateForDevice_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->onSpatializationEnabledChangeForAnyDevice(thisPtr, (const AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioSpatializationManager_onSpatializationEnabledChangeForAnyDevice, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioSpatializationManager_offSpatializationEnabledChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_Boolean_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Boolean parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_Boolean_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Boolean parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_Boolean_Void))))};
        }
        Opt_AUDIO_Callback_Boolean_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->offSpatializationEnabledChange(thisPtr, (const Opt_AUDIO_Callback_Boolean_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioSpatializationManager_offSpatializationEnabledChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioSpatializationManager_offSpatializationEnabledChangeForAnyDevice(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioSpatialEnabledStateForDevice parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioSpatialEnabledStateForDevice_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioSpatialEnabledStateForDevice parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioSpatialEnabledStateForDevice_Void))))};
        }
        Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->offSpatializationEnabledChangeForAnyDevice(thisPtr, (const Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioSpatializationManager_offSpatializationEnabledChangeForAnyDevice, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioSpatializationManager_setHeadTrackingEnabled0(OH_NativePointer thisPtr, OH_Boolean enable, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->setHeadTrackingEnabled0(thisPtr, &enable, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioSpatializationManager_setHeadTrackingEnabled0, OH_NativePointer, OH_Boolean, uint8_t*, int32_t)
void impl_AudioSpatializationManager_setHeadTrackingEnabled1(OH_NativePointer thisPtr, OH_Boolean enable) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->setHeadTrackingEnabled1(thisPtr, &enable);
}
KOALA_INTEROP_V2(AudioSpatializationManager_setHeadTrackingEnabled1, OH_NativePointer, OH_Boolean)
void impl_AudioSpatializationManager_setHeadTrackingEnabled2(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength, OH_Boolean enabled) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioDeviceDescriptor deviceDescriptor_value = thisDeserializer.readAudioDeviceDescriptor();;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->setHeadTrackingEnabled2(thisPtr, (const OH_AUDIO_AudioDeviceDescriptor*)&deviceDescriptor_value, &enabled);
}
KOALA_INTEROP_V4(AudioSpatializationManager_setHeadTrackingEnabled2, OH_NativePointer, uint8_t*, int32_t, OH_Boolean)
OH_Boolean impl_AudioSpatializationManager_isHeadTrackingEnabled0(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->isHeadTrackingEnabled0(thisPtr);
}
KOALA_INTEROP_1(AudioSpatializationManager_isHeadTrackingEnabled0, OH_Boolean, OH_NativePointer)
OH_Boolean impl_AudioSpatializationManager_isHeadTrackingEnabled1(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioDeviceDescriptor deviceDescriptor_value = thisDeserializer.readAudioDeviceDescriptor();;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->isHeadTrackingEnabled1(thisPtr, (const OH_AUDIO_AudioDeviceDescriptor*)&deviceDescriptor_value);
}
KOALA_INTEROP_3(AudioSpatializationManager_isHeadTrackingEnabled1, OH_Boolean, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioSpatializationManager_onHeadTrackingEnabledChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_Boolean_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Boolean parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_Boolean_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Boolean parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_Boolean_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->onHeadTrackingEnabledChange(thisPtr, (const AUDIO_Callback_Boolean_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioSpatializationManager_onHeadTrackingEnabledChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioSpatializationManager_onHeadTrackingEnabledChangeForAnyDevice(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioSpatialEnabledStateForDevice parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioSpatialEnabledStateForDevice_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioSpatialEnabledStateForDevice parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioSpatialEnabledStateForDevice_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->onHeadTrackingEnabledChangeForAnyDevice(thisPtr, (const AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioSpatializationManager_onHeadTrackingEnabledChangeForAnyDevice, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioSpatializationManager_offHeadTrackingEnabledChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_Boolean_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Boolean parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_Boolean_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Boolean parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_Boolean_Void))))};
        }
        Opt_AUDIO_Callback_Boolean_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->offHeadTrackingEnabledChange(thisPtr, (const Opt_AUDIO_Callback_Boolean_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioSpatializationManager_offHeadTrackingEnabledChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioSpatializationManager_offHeadTrackingEnabledChangeForAnyDevice(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioSpatialEnabledStateForDevice parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioSpatialEnabledStateForDevice_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioSpatialEnabledStateForDevice parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioSpatialEnabledStateForDevice_Void))))};
        }
        Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->offHeadTrackingEnabledChangeForAnyDevice(thisPtr, (const Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioSpatializationManager_offHeadTrackingEnabledChangeForAnyDevice, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioSpatializationManager_updateSpatialDeviceState(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_AUDIO_AudioSpatialDeviceState spatialDeviceState_value = thisDeserializer.readAudioSpatialDeviceState();;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->updateSpatialDeviceState(thisPtr, (const OH_AUDIO_AudioSpatialDeviceState*)&spatialDeviceState_value);
}
KOALA_INTEROP_V3(AudioSpatializationManager_updateSpatialDeviceState, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioSpatializationManager_setSpatializationSceneType(OH_NativePointer thisPtr, OH_Int32 spatializationSceneType) {
    const OH_AUDIO_audio_AudioSpatializationSceneType tmp = static_cast<OH_AUDIO_audio_AudioSpatializationSceneType>(spatializationSceneType);    
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->setSpatializationSceneType(thisPtr, &tmp);
}
KOALA_INTEROP_V2(AudioSpatializationManager_setSpatializationSceneType, OH_NativePointer, OH_Int32)
OH_NativePointer impl_AudioSpatializationManager_getSpatializationSceneType(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioSpatializationManager()->getSpatializationSceneType(thisPtr);
}
KOALA_INTEROP_1(AudioSpatializationManager_getSpatializationSceneType, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioRenderer_ctor() {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->construct();
}
KOALA_INTEROP_0(AudioRenderer_ctor, OH_NativePointer)
OH_NativePointer impl_AudioRenderer_getFinalizer() {
        return (OH_NativePointer) GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->destruct;
}
KOALA_INTEROP_0(AudioRenderer_getFinalizer, OH_NativePointer)
void impl_AudioRenderer_getRendererInfo0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_AudioRendererInfo_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioRendererInfo result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioRendererInfo_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioRendererInfo result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioRendererInfo_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getRendererInfo0(thisPtr, (const AUDIO_AsyncCallback_AudioRendererInfo_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_getRendererInfo0, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioRenderer_getRendererInfo1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getRendererInfo1(thisPtr);
}
KOALA_INTEROP_1(AudioRenderer_getRendererInfo1, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioRenderer_getRendererInfoSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getRendererInfoSync(thisPtr);
}
KOALA_INTEROP_1(AudioRenderer_getRendererInfoSync, OH_NativePointer, OH_NativePointer)
void impl_AudioRenderer_getStreamInfo0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_AudioStreamInfo_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioStreamInfo result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioStreamInfo_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioStreamInfo result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioStreamInfo_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getStreamInfo0(thisPtr, (const AUDIO_AsyncCallback_AudioStreamInfo_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_getStreamInfo0, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioRenderer_getStreamInfo1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getStreamInfo1(thisPtr);
}
KOALA_INTEROP_1(AudioRenderer_getStreamInfo1, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioRenderer_getStreamInfoSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getStreamInfoSync(thisPtr);
}
KOALA_INTEROP_1(AudioRenderer_getStreamInfoSync, OH_NativePointer, OH_NativePointer)
void impl_AudioRenderer_getAudioStreamId0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getAudioStreamId0(thisPtr, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_getAudioStreamId0, OH_NativePointer, uint8_t*, int32_t)
OH_Int32 impl_AudioRenderer_getAudioStreamId1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getAudioStreamId1(thisPtr).i32;
}
KOALA_INTEROP_1(AudioRenderer_getAudioStreamId1, OH_Int32, OH_NativePointer)
OH_Int32 impl_AudioRenderer_getAudioStreamIdSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getAudioStreamIdSync(thisPtr).i32;
}
KOALA_INTEROP_1(AudioRenderer_getAudioStreamIdSync, OH_Int32, OH_NativePointer)
void impl_AudioRenderer_getAudioEffectMode0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_AudioEffectMode_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioEffectMode result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioEffectMode_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, OH_AUDIO_audio_AudioEffectMode result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioEffectMode_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getAudioEffectMode0(thisPtr, (const AUDIO_AsyncCallback_AudioEffectMode_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_getAudioEffectMode0, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioRenderer_getAudioEffectMode1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getAudioEffectMode1(thisPtr);
}
KOALA_INTEROP_1(AudioRenderer_getAudioEffectMode1, OH_NativePointer, OH_NativePointer)
void impl_AudioRenderer_setAudioEffectMode0(OH_NativePointer thisPtr, OH_Int32 mode, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        const OH_AUDIO_audio_AudioEffectMode tmp = static_cast<OH_AUDIO_audio_AudioEffectMode>(mode);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->setAudioEffectMode0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioRenderer_setAudioEffectMode0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
void impl_AudioRenderer_setAudioEffectMode1(OH_NativePointer thisPtr, OH_Int32 mode) {
    const OH_AUDIO_audio_AudioEffectMode tmp = static_cast<OH_AUDIO_audio_AudioEffectMode>(mode);    
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->setAudioEffectMode1(thisPtr, &tmp);
}
KOALA_INTEROP_V2(AudioRenderer_setAudioEffectMode1, OH_NativePointer, OH_Int32)
void impl_AudioRenderer_start0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->start0(thisPtr, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_start0, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRenderer_start1(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->start1(thisPtr);
}
KOALA_INTEROP_V1(AudioRenderer_start1, OH_NativePointer)
void impl_AudioRenderer_write0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_Buffer buffer_value = static_cast<OH_Buffer>(thisDeserializer.readBuffer());;
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->write0(thisPtr, (const OH_Buffer*)&buffer_value, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_write0, OH_NativePointer, uint8_t*, int32_t)
OH_Int32 impl_AudioRenderer_write1(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_Buffer buffer_value = static_cast<OH_Buffer>(thisDeserializer.readBuffer());;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->write1(thisPtr, (const OH_Buffer*)&buffer_value).i32;
}
KOALA_INTEROP_3(AudioRenderer_write1, OH_Int32, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRenderer_getAudioTime0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getAudioTime0(thisPtr, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_getAudioTime0, OH_NativePointer, uint8_t*, int32_t)
OH_Int32 impl_AudioRenderer_getAudioTime1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getAudioTime1(thisPtr).i32;
}
KOALA_INTEROP_1(AudioRenderer_getAudioTime1, OH_Int32, OH_NativePointer)
OH_Int32 impl_AudioRenderer_getAudioTimeSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getAudioTimeSync(thisPtr).i32;
}
KOALA_INTEROP_1(AudioRenderer_getAudioTimeSync, OH_Int32, OH_NativePointer)
void impl_AudioRenderer_drain0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->drain0(thisPtr, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_drain0, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRenderer_drain1(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->drain1(thisPtr);
}
KOALA_INTEROP_V1(AudioRenderer_drain1, OH_NativePointer)
void impl_AudioRenderer_flush(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->flush(thisPtr);
}
KOALA_INTEROP_V1(AudioRenderer_flush, OH_NativePointer)
void impl_AudioRenderer_pause0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->pause0(thisPtr, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_pause0, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRenderer_pause1(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->pause1(thisPtr);
}
KOALA_INTEROP_V1(AudioRenderer_pause1, OH_NativePointer)
void impl_AudioRenderer_stop0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->stop0(thisPtr, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_stop0, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRenderer_stop1(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->stop1(thisPtr);
}
KOALA_INTEROP_V1(AudioRenderer_stop1, OH_NativePointer)
void impl_AudioRenderer_release0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->release0(thisPtr, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_release0, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRenderer_release1(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->release1(thisPtr);
}
KOALA_INTEROP_V1(AudioRenderer_release1, OH_NativePointer)
void impl_AudioRenderer_getBufferSize0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getBufferSize0(thisPtr, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_getBufferSize0, OH_NativePointer, uint8_t*, int32_t)
OH_Int32 impl_AudioRenderer_getBufferSize1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getBufferSize1(thisPtr).i32;
}
KOALA_INTEROP_1(AudioRenderer_getBufferSize1, OH_Int32, OH_NativePointer)
OH_Int32 impl_AudioRenderer_getBufferSizeSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getBufferSizeSync(thisPtr).i32;
}
KOALA_INTEROP_1(AudioRenderer_getBufferSizeSync, OH_Int32, OH_NativePointer)
void impl_AudioRenderer_setRenderRate0(OH_NativePointer thisPtr, OH_Int32 rate, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        const OH_AUDIO_audio_AudioRendererRate tmp = static_cast<OH_AUDIO_audio_AudioRendererRate>(rate);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->setRenderRate0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioRenderer_setRenderRate0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
void impl_AudioRenderer_setRenderRate1(OH_NativePointer thisPtr, OH_Int32 rate) {
    const OH_AUDIO_audio_AudioRendererRate tmp = static_cast<OH_AUDIO_audio_AudioRendererRate>(rate);    
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->setRenderRate1(thisPtr, &tmp);
}
KOALA_INTEROP_V2(AudioRenderer_setRenderRate1, OH_NativePointer, OH_Int32)
void impl_AudioRenderer_setSpeed(OH_NativePointer thisPtr, KInteropNumber speed) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->setSpeed(thisPtr, (const OH_Number*) (&speed));
}
KOALA_INTEROP_V2(AudioRenderer_setSpeed, OH_NativePointer, KInteropNumber)
void impl_AudioRenderer_getRenderRate0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_AudioRendererRate_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioRendererRate result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioRendererRate_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, OH_AUDIO_audio_AudioRendererRate result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioRendererRate_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getRenderRate0(thisPtr, (const AUDIO_AsyncCallback_AudioRendererRate_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_getRenderRate0, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioRenderer_getRenderRate1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getRenderRate1(thisPtr);
}
KOALA_INTEROP_1(AudioRenderer_getRenderRate1, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioRenderer_getRenderRateSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getRenderRateSync(thisPtr);
}
KOALA_INTEROP_1(AudioRenderer_getRenderRateSync, OH_NativePointer, OH_NativePointer)
OH_Int32 impl_AudioRenderer_getSpeed(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getSpeed(thisPtr).i32;
}
KOALA_INTEROP_1(AudioRenderer_getSpeed, OH_Int32, OH_NativePointer)
void impl_AudioRenderer_setInterruptMode0(OH_NativePointer thisPtr, OH_Int32 mode, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        const OH_AUDIO_audio_InterruptMode tmp = static_cast<OH_AUDIO_audio_InterruptMode>(mode);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->setInterruptMode0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioRenderer_setInterruptMode0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
void impl_AudioRenderer_setInterruptMode1(OH_NativePointer thisPtr, OH_Int32 mode) {
    const OH_AUDIO_audio_InterruptMode tmp = static_cast<OH_AUDIO_audio_InterruptMode>(mode);    
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->setInterruptMode1(thisPtr, &tmp);
}
KOALA_INTEROP_V2(AudioRenderer_setInterruptMode1, OH_NativePointer, OH_Int32)
void impl_AudioRenderer_setInterruptModeSync(OH_NativePointer thisPtr, OH_Int32 mode) {
    const OH_AUDIO_audio_InterruptMode tmp = static_cast<OH_AUDIO_audio_InterruptMode>(mode);    
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->setInterruptModeSync(thisPtr, &tmp);
}
KOALA_INTEROP_V2(AudioRenderer_setInterruptModeSync, OH_NativePointer, OH_Int32)
void impl_AudioRenderer_setVolume0(OH_NativePointer thisPtr, KInteropNumber volume, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->setVolume0(thisPtr, (const OH_Number*) (&volume), (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioRenderer_setVolume0, OH_NativePointer, KInteropNumber, uint8_t*, int32_t)
void impl_AudioRenderer_setVolume1(OH_NativePointer thisPtr, KInteropNumber volume) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->setVolume1(thisPtr, (const OH_Number*) (&volume));
}
KOALA_INTEROP_V2(AudioRenderer_setVolume1, OH_NativePointer, KInteropNumber)
OH_Int32 impl_AudioRenderer_getVolume(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getVolume(thisPtr).i32;
}
KOALA_INTEROP_1(AudioRenderer_getVolume, OH_Int32, OH_NativePointer)
void impl_AudioRenderer_setVolumeWithRamp(OH_NativePointer thisPtr, KInteropNumber volume, KInteropNumber duration) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->setVolumeWithRamp(thisPtr, (const OH_Number*) (&volume), (const OH_Number*) (&duration));
}
KOALA_INTEROP_V3(AudioRenderer_setVolumeWithRamp, OH_NativePointer, KInteropNumber, KInteropNumber)
void impl_AudioRenderer_getMinStreamVolume0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getMinStreamVolume0(thisPtr, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_getMinStreamVolume0, OH_NativePointer, uint8_t*, int32_t)
OH_Int32 impl_AudioRenderer_getMinStreamVolume1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getMinStreamVolume1(thisPtr).i32;
}
KOALA_INTEROP_1(AudioRenderer_getMinStreamVolume1, OH_Int32, OH_NativePointer)
OH_Int32 impl_AudioRenderer_getMinStreamVolumeSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getMinStreamVolumeSync(thisPtr).i32;
}
KOALA_INTEROP_1(AudioRenderer_getMinStreamVolumeSync, OH_Int32, OH_NativePointer)
void impl_AudioRenderer_getMaxStreamVolume0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getMaxStreamVolume0(thisPtr, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_getMaxStreamVolume0, OH_NativePointer, uint8_t*, int32_t)
OH_Int32 impl_AudioRenderer_getMaxStreamVolume1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getMaxStreamVolume1(thisPtr).i32;
}
KOALA_INTEROP_1(AudioRenderer_getMaxStreamVolume1, OH_Int32, OH_NativePointer)
OH_Int32 impl_AudioRenderer_getMaxStreamVolumeSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getMaxStreamVolumeSync(thisPtr).i32;
}
KOALA_INTEROP_1(AudioRenderer_getMaxStreamVolumeSync, OH_Int32, OH_NativePointer)
void impl_AudioRenderer_getUnderflowCount0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getUnderflowCount0(thisPtr, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_getUnderflowCount0, OH_NativePointer, uint8_t*, int32_t)
OH_Int32 impl_AudioRenderer_getUnderflowCount1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getUnderflowCount1(thisPtr).i32;
}
KOALA_INTEROP_1(AudioRenderer_getUnderflowCount1, OH_Int32, OH_NativePointer)
OH_Int32 impl_AudioRenderer_getUnderflowCountSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getUnderflowCountSync(thisPtr).i32;
}
KOALA_INTEROP_1(AudioRenderer_getUnderflowCountSync, OH_Int32, OH_NativePointer)
void impl_AudioRenderer_getCurrentOutputDevices0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_AudioDeviceDescriptors_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioDeviceDescriptors_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioDeviceDescriptors_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getCurrentOutputDevices0(thisPtr, (const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_getCurrentOutputDevices0, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioRenderer_getCurrentOutputDevices1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getCurrentOutputDevices1(thisPtr);
}
KOALA_INTEROP_1(AudioRenderer_getCurrentOutputDevices1, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioRenderer_getCurrentOutputDevicesSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getCurrentOutputDevicesSync(thisPtr);
}
KOALA_INTEROP_1(AudioRenderer_getCurrentOutputDevicesSync, OH_NativePointer, OH_NativePointer)
void impl_AudioRenderer_setChannelBlendMode(OH_NativePointer thisPtr, OH_Int32 mode) {
    const OH_AUDIO_audio_ChannelBlendMode tmp = static_cast<OH_AUDIO_audio_ChannelBlendMode>(mode);
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->setChannelBlendMode(thisPtr, &tmp);
}
KOALA_INTEROP_V2(AudioRenderer_setChannelBlendMode, OH_NativePointer, OH_Int32)
void impl_AudioRenderer_setSilentModeAndMixWithOthers(OH_NativePointer thisPtr, OH_Boolean on) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->setSilentModeAndMixWithOthers(thisPtr, &on);
}
KOALA_INTEROP_V2(AudioRenderer_setSilentModeAndMixWithOthers, OH_NativePointer, OH_Boolean)
OH_Boolean impl_AudioRenderer_getSilentModeAndMixWithOthers(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getSilentModeAndMixWithOthers(thisPtr);
}
KOALA_INTEROP_1(AudioRenderer_getSilentModeAndMixWithOthers, OH_Boolean, OH_NativePointer)
void impl_AudioRenderer_setDefaultOutputDevice(OH_NativePointer thisPtr, OH_Int32 deviceType) {
    const OH_AUDIO_audio_DeviceType tmp = static_cast<OH_AUDIO_audio_DeviceType>(deviceType);
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->setDefaultOutputDevice(thisPtr, &tmp);
}
KOALA_INTEROP_V2(AudioRenderer_setDefaultOutputDevice, OH_NativePointer, OH_Int32)
void impl_AudioRenderer_onAudioInterrupt(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_InterruptEvent_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_InterruptEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_InterruptEvent_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_InterruptEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_InterruptEvent_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->onAudioInterrupt(thisPtr, (const AUDIO_Callback_InterruptEvent_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_onAudioInterrupt, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRenderer_onMarkReach(OH_NativePointer thisPtr, KInteropNumber frame, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_Number_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->onMarkReach(thisPtr, (const OH_Number*) (&frame), (const AUDIO_Callback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioRenderer_onMarkReach, OH_NativePointer, KInteropNumber, uint8_t*, int32_t)
void impl_AudioRenderer_offMarkReach(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->offMarkReach(thisPtr);
}
KOALA_INTEROP_V1(AudioRenderer_offMarkReach, OH_NativePointer)
void impl_AudioRenderer_onPeriodReach(OH_NativePointer thisPtr, KInteropNumber frame, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_Number_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->onPeriodReach(thisPtr, (const OH_Number*) (&frame), (const AUDIO_Callback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioRenderer_onPeriodReach, OH_NativePointer, KInteropNumber, uint8_t*, int32_t)
void impl_AudioRenderer_offPeriodReach(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->offPeriodReach(thisPtr);
}
KOALA_INTEROP_V1(AudioRenderer_offPeriodReach, OH_NativePointer)
void impl_AudioRenderer_onStateChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_AudioState_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioState parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioState_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, OH_AUDIO_audio_AudioState parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioState_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->onStateChange(thisPtr, (const AUDIO_Callback_AudioState_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_onStateChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRenderer_onOutputDeviceChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_AudioDeviceDescriptors_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioDeviceDescriptors_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioDeviceDescriptors_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->onOutputDeviceChange(thisPtr, (const AUDIO_Callback_AudioDeviceDescriptors_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_onOutputDeviceChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRenderer_onOutputDeviceChangeWithInfo(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_AudioStreamDeviceChangeInfo_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioStreamDeviceChangeInfo parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioStreamDeviceChangeInfo_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioStreamDeviceChangeInfo parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioStreamDeviceChangeInfo_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->onOutputDeviceChangeWithInfo(thisPtr, (const AUDIO_Callback_AudioStreamDeviceChangeInfo_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_onOutputDeviceChangeWithInfo, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRenderer_offOutputDeviceChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_AudioDeviceDescriptors_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioDeviceDescriptors_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioDeviceDescriptors_Void))))};
        }
        Opt_AUDIO_Callback_AudioDeviceDescriptors_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->offOutputDeviceChange(thisPtr, (const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_offOutputDeviceChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRenderer_offOutputDeviceChangeWithInfo(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_AudioStreamDeviceChangeInfo_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioStreamDeviceChangeInfo parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioStreamDeviceChangeInfo_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioStreamDeviceChangeInfo parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioStreamDeviceChangeInfo_Void))))};
        }
        Opt_AUDIO_Callback_AudioStreamDeviceChangeInfo_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->offOutputDeviceChangeWithInfo(thisPtr, (const Opt_AUDIO_Callback_AudioStreamDeviceChangeInfo_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_offOutputDeviceChangeWithInfo, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRenderer_onWriteData(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AudioRendererWriteDataCallback callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Buffer data, const OH_CustomObject continuation)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AudioRendererWriteDataCallback)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Buffer data, const OH_CustomObject continuation)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AudioRendererWriteDataCallback))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->onWriteData(thisPtr, (const AUDIO_AudioRendererWriteDataCallback*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_onWriteData, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioRenderer_offWriteData(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_AudioRendererWriteDataCallback callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Buffer data, const OH_CustomObject continuation)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AudioRendererWriteDataCallback)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Buffer data, const OH_CustomObject continuation)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AudioRendererWriteDataCallback))))};
        }
        Opt_AUDIO_AudioRendererWriteDataCallback callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->offWriteData(thisPtr, (const Opt_AUDIO_AudioRendererWriteDataCallback*)&callback__value);
}
KOALA_INTEROP_V3(AudioRenderer_offWriteData, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioRenderer_getState(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioRenderer()->getState(thisPtr);
}
KOALA_INTEROP_1(AudioRenderer_getState, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioCapturer_ctor() {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->construct();
}
KOALA_INTEROP_0(AudioCapturer_ctor, OH_NativePointer)
OH_NativePointer impl_AudioCapturer_getFinalizer() {
        return (OH_NativePointer) GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->destruct;
}
KOALA_INTEROP_0(AudioCapturer_getFinalizer, OH_NativePointer)
void impl_AudioCapturer_getCapturerInfo0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_AudioCapturerInfo_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioCapturerInfo result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioCapturerInfo_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioCapturerInfo result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioCapturerInfo_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getCapturerInfo0(thisPtr, (const AUDIO_AsyncCallback_AudioCapturerInfo_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioCapturer_getCapturerInfo0, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioCapturer_getCapturerInfo1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getCapturerInfo1(thisPtr);
}
KOALA_INTEROP_1(AudioCapturer_getCapturerInfo1, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioCapturer_getCapturerInfoSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getCapturerInfoSync(thisPtr);
}
KOALA_INTEROP_1(AudioCapturer_getCapturerInfoSync, OH_NativePointer, OH_NativePointer)
void impl_AudioCapturer_getStreamInfo0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_AudioStreamInfo_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioStreamInfo result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_AudioStreamInfo_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioStreamInfo result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_AudioStreamInfo_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getStreamInfo0(thisPtr, (const AUDIO_AsyncCallback_AudioStreamInfo_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioCapturer_getStreamInfo0, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioCapturer_getStreamInfo1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getStreamInfo1(thisPtr);
}
KOALA_INTEROP_1(AudioCapturer_getStreamInfo1, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioCapturer_getStreamInfoSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getStreamInfoSync(thisPtr);
}
KOALA_INTEROP_1(AudioCapturer_getStreamInfoSync, OH_NativePointer, OH_NativePointer)
void impl_AudioCapturer_getAudioStreamId0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getAudioStreamId0(thisPtr, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioCapturer_getAudioStreamId0, OH_NativePointer, uint8_t*, int32_t)
OH_Int32 impl_AudioCapturer_getAudioStreamId1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getAudioStreamId1(thisPtr).i32;
}
KOALA_INTEROP_1(AudioCapturer_getAudioStreamId1, OH_Int32, OH_NativePointer)
OH_Int32 impl_AudioCapturer_getAudioStreamIdSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getAudioStreamIdSync(thisPtr).i32;
}
KOALA_INTEROP_1(AudioCapturer_getAudioStreamIdSync, OH_Int32, OH_NativePointer)
void impl_AudioCapturer_start0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->start0(thisPtr, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioCapturer_start0, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioCapturer_start1(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->start1(thisPtr);
}
KOALA_INTEROP_V1(AudioCapturer_start1, OH_NativePointer)
void impl_AudioCapturer_read0(OH_NativePointer thisPtr, KInteropNumber size, OH_Boolean isBlockingRead, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Buffer_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Buffer result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Buffer_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Buffer result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Buffer_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->read0(thisPtr, (const OH_Number*) (&size), &isBlockingRead, (const AUDIO_AsyncCallback_Buffer_Void*)&callback__value);
}
KOALA_INTEROP_V5(AudioCapturer_read0, OH_NativePointer, KInteropNumber, OH_Boolean, uint8_t*, int32_t)
void impl_AudioCapturer_read1(OH_NativePointer thisPtr, KInteropNumber size, OH_Boolean isBlockingRead) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->read1(thisPtr, (const OH_Number*) (&size), &isBlockingRead);
}
KOALA_INTEROP_V3(AudioCapturer_read1, OH_NativePointer, KInteropNumber, OH_Boolean)
void impl_AudioCapturer_getAudioTime0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getAudioTime0(thisPtr, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioCapturer_getAudioTime0, OH_NativePointer, uint8_t*, int32_t)
OH_Int32 impl_AudioCapturer_getAudioTime1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getAudioTime1(thisPtr).i32;
}
KOALA_INTEROP_1(AudioCapturer_getAudioTime1, OH_Int32, OH_NativePointer)
OH_Int32 impl_AudioCapturer_getAudioTimeSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getAudioTimeSync(thisPtr).i32;
}
KOALA_INTEROP_1(AudioCapturer_getAudioTimeSync, OH_Int32, OH_NativePointer)
void impl_AudioCapturer_stop0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->stop0(thisPtr, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioCapturer_stop0, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioCapturer_stop1(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->stop1(thisPtr);
}
KOALA_INTEROP_V1(AudioCapturer_stop1, OH_NativePointer)
void impl_AudioCapturer_release0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->release0(thisPtr, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioCapturer_release0, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioCapturer_release1(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->release1(thisPtr);
}
KOALA_INTEROP_V1(AudioCapturer_release1, OH_NativePointer)
void impl_AudioCapturer_getBufferSize0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Number_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getBufferSize0(thisPtr, (const AUDIO_AsyncCallback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioCapturer_getBufferSize0, OH_NativePointer, uint8_t*, int32_t)
OH_Int32 impl_AudioCapturer_getBufferSize1(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getBufferSize1(thisPtr).i32;
}
KOALA_INTEROP_1(AudioCapturer_getBufferSize1, OH_Int32, OH_NativePointer)
OH_Int32 impl_AudioCapturer_getBufferSizeSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getBufferSizeSync(thisPtr).i32;
}
KOALA_INTEROP_1(AudioCapturer_getBufferSizeSync, OH_Int32, OH_NativePointer)
OH_NativePointer impl_AudioCapturer_getCurrentInputDevices(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getCurrentInputDevices(thisPtr);
}
KOALA_INTEROP_1(AudioCapturer_getCurrentInputDevices, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AudioCapturer_getCurrentAudioCapturerChangeInfo(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getCurrentAudioCapturerChangeInfo(thisPtr);
}
KOALA_INTEROP_1(AudioCapturer_getCurrentAudioCapturerChangeInfo, OH_NativePointer, OH_NativePointer)
OH_Int32 impl_AudioCapturer_getOverflowCount(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getOverflowCount(thisPtr).i32;
}
KOALA_INTEROP_1(AudioCapturer_getOverflowCount, OH_Int32, OH_NativePointer)
OH_Int32 impl_AudioCapturer_getOverflowCountSync(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getOverflowCountSync(thisPtr).i32;
}
KOALA_INTEROP_1(AudioCapturer_getOverflowCountSync, OH_Int32, OH_NativePointer)
void impl_AudioCapturer_onMarkReach(OH_NativePointer thisPtr, KInteropNumber frame, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_Number_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->onMarkReach(thisPtr, (const OH_Number*) (&frame), (const AUDIO_Callback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioCapturer_onMarkReach, OH_NativePointer, KInteropNumber, uint8_t*, int32_t)
void impl_AudioCapturer_offMarkReach(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->offMarkReach(thisPtr);
}
KOALA_INTEROP_V1(AudioCapturer_offMarkReach, OH_NativePointer)
void impl_AudioCapturer_onPeriodReach(OH_NativePointer thisPtr, KInteropNumber frame, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_Number_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_Number_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_Number_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->onPeriodReach(thisPtr, (const OH_Number*) (&frame), (const AUDIO_Callback_Number_Void*)&callback__value);
}
KOALA_INTEROP_V4(AudioCapturer_onPeriodReach, OH_NativePointer, KInteropNumber, uint8_t*, int32_t)
void impl_AudioCapturer_offPeriodReach(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->offPeriodReach(thisPtr);
}
KOALA_INTEROP_V1(AudioCapturer_offPeriodReach, OH_NativePointer)
void impl_AudioCapturer_onStateChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_AudioState_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioState parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioState_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, OH_AUDIO_audio_AudioState parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioState_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->onStateChange(thisPtr, (const AUDIO_Callback_AudioState_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioCapturer_onStateChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioCapturer_onAudioInterrupt(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_InterruptEvent_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_InterruptEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_InterruptEvent_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_InterruptEvent parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_InterruptEvent_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->onAudioInterrupt(thisPtr, (const AUDIO_Callback_InterruptEvent_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioCapturer_onAudioInterrupt, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioCapturer_offAudioInterrupt(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->offAudioInterrupt(thisPtr);
}
KOALA_INTEROP_V1(AudioCapturer_offAudioInterrupt, OH_NativePointer)
void impl_AudioCapturer_onInputDeviceChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_AudioDeviceDescriptors_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioDeviceDescriptors_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioDeviceDescriptors_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->onInputDeviceChange(thisPtr, (const AUDIO_Callback_AudioDeviceDescriptors_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioCapturer_onInputDeviceChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioCapturer_offInputDeviceChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_AudioDeviceDescriptors_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioDeviceDescriptors_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioDeviceDescriptors_Void))))};
        }
        Opt_AUDIO_Callback_AudioDeviceDescriptors_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->offInputDeviceChange(thisPtr, (const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioCapturer_offInputDeviceChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioCapturer_onAudioCapturerChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_AudioCapturerChangeInfo_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioCapturerChangeInfo parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioCapturerChangeInfo_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioCapturerChangeInfo parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioCapturerChangeInfo_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->onAudioCapturerChange(thisPtr, (const AUDIO_Callback_AudioCapturerChangeInfo_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioCapturer_onAudioCapturerChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioCapturer_offAudioCapturerChange(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_AudioCapturerChangeInfo_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioCapturerChangeInfo parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_AudioCapturerChangeInfo_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioCapturerChangeInfo parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_AudioCapturerChangeInfo_Void))))};
        }
        Opt_AUDIO_Callback_AudioCapturerChangeInfo_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->offAudioCapturerChange(thisPtr, (const Opt_AUDIO_Callback_AudioCapturerChangeInfo_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioCapturer_offAudioCapturerChange, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioCapturer_onReadData(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_Callback_Buffer_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Buffer parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_Buffer_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Buffer parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_Buffer_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->onReadData(thisPtr, (const AUDIO_Callback_Buffer_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioCapturer_onReadData, OH_NativePointer, uint8_t*, int32_t)
void impl_AudioCapturer_offReadData(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        const auto callback__value_buf_runtimeType = static_cast<OH_AUDIO_RuntimeType>(thisDeserializer.readInt8());
        Opt_AUDIO_Callback_Buffer_Void callback__value_buf = {};
        callback__value_buf.tag = callback__value_buf_runtimeType == INTEROP_RUNTIME_UNDEFINED ? INTEROP_TAG_UNDEFINED : INTEROP_TAG_OBJECT;
        if ((INTEROP_RUNTIME_UNDEFINED) != (callback__value_buf_runtimeType))
        {
            callback__value_buf.value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Buffer parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_Callback_Buffer_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Buffer parameter)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_Callback_Buffer_Void))))};
        }
        Opt_AUDIO_Callback_Buffer_Void callback__value = callback__value_buf;;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->offReadData(thisPtr, (const Opt_AUDIO_Callback_Buffer_Void*)&callback__value);
}
KOALA_INTEROP_V3(AudioCapturer_offReadData, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_AudioCapturer_getState(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AudioCapturer()->getState(thisPtr);
}
KOALA_INTEROP_1(AudioCapturer_getState, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_AsrProcessingController_ctor() {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AsrProcessingController()->construct();
}
KOALA_INTEROP_0(AsrProcessingController_ctor, OH_NativePointer)
OH_NativePointer impl_AsrProcessingController_getFinalizer() {
        return (OH_NativePointer) GetAUDIOAPIImpl(AUDIO_API_VERSION)->AsrProcessingController()->destruct;
}
KOALA_INTEROP_0(AsrProcessingController_getFinalizer, OH_NativePointer)
OH_Boolean impl_AsrProcessingController_setAsrAecMode(OH_NativePointer thisPtr, OH_Int32 mode) {
    const OH_AUDIO_audio_AsrAecMode tmp = static_cast<OH_AUDIO_audio_AsrAecMode>(mode);
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AsrProcessingController()->setAsrAecMode(thisPtr, &tmp);
}
KOALA_INTEROP_2(AsrProcessingController_setAsrAecMode, OH_Boolean, OH_NativePointer, OH_Int32)
OH_NativePointer impl_AsrProcessingController_getAsrAecMode(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AsrProcessingController()->getAsrAecMode(thisPtr);
}
KOALA_INTEROP_1(AsrProcessingController_getAsrAecMode, OH_NativePointer, OH_NativePointer)
OH_Boolean impl_AsrProcessingController_setAsrNoiseSuppressionMode(OH_NativePointer thisPtr, OH_Int32 mode) {
    const OH_AUDIO_audio_AsrNoiseSuppressionMode tmp = static_cast<OH_AUDIO_audio_AsrNoiseSuppressionMode>(mode);
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AsrProcessingController()->setAsrNoiseSuppressionMode(thisPtr, &tmp);
}
KOALA_INTEROP_2(AsrProcessingController_setAsrNoiseSuppressionMode, OH_Boolean, OH_NativePointer, OH_Int32)
OH_NativePointer impl_AsrProcessingController_getAsrNoiseSuppressionMode(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AsrProcessingController()->getAsrNoiseSuppressionMode(thisPtr);
}
KOALA_INTEROP_1(AsrProcessingController_getAsrNoiseSuppressionMode, OH_NativePointer, OH_NativePointer)
OH_Boolean impl_AsrProcessingController_isWhispering(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AsrProcessingController()->isWhispering(thisPtr);
}
KOALA_INTEROP_1(AsrProcessingController_isWhispering, OH_Boolean, OH_NativePointer)
OH_Boolean impl_AsrProcessingController_setAsrVoiceControlMode(OH_NativePointer thisPtr, OH_Int32 mode, OH_Boolean enable) {
    const OH_AUDIO_audio_AsrVoiceControlMode tmp = static_cast<OH_AUDIO_audio_AsrVoiceControlMode>(mode);
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AsrProcessingController()->setAsrVoiceControlMode(thisPtr, &tmp, &enable);
}
KOALA_INTEROP_3(AsrProcessingController_setAsrVoiceControlMode, OH_Boolean, OH_NativePointer, OH_Int32, OH_Boolean)
OH_Boolean impl_AsrProcessingController_setAsrVoiceMuteMode(OH_NativePointer thisPtr, OH_Int32 mode, OH_Boolean enable) {
    const OH_AUDIO_audio_AsrVoiceMuteMode tmp = static_cast<OH_AUDIO_audio_AsrVoiceMuteMode>(mode);
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AsrProcessingController()->setAsrVoiceMuteMode(thisPtr, &tmp, &enable);
}
KOALA_INTEROP_3(AsrProcessingController_setAsrVoiceMuteMode, OH_Boolean, OH_NativePointer, OH_Int32, OH_Boolean)
OH_Boolean impl_AsrProcessingController_setAsrWhisperDetectionMode(OH_NativePointer thisPtr, OH_Int32 mode) {
    const OH_AUDIO_audio_AsrWhisperDetectionMode tmp = static_cast<OH_AUDIO_audio_AsrWhisperDetectionMode>(mode);
    return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AsrProcessingController()->setAsrWhisperDetectionMode(thisPtr, &tmp);
}
KOALA_INTEROP_2(AsrProcessingController_setAsrWhisperDetectionMode, OH_Boolean, OH_NativePointer, OH_Int32)
OH_NativePointer impl_AsrProcessingController_getAsrWhisperDetectionMode(OH_NativePointer thisPtr) {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AsrProcessingController()->getAsrWhisperDetectionMode(thisPtr);
}
KOALA_INTEROP_1(AsrProcessingController_getAsrWhisperDetectionMode, OH_NativePointer, OH_NativePointer)
OH_NativePointer impl_TonePlayer_ctor() {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->TonePlayer()->construct();
}
KOALA_INTEROP_0(TonePlayer_ctor, OH_NativePointer)
OH_NativePointer impl_TonePlayer_getFinalizer() {
        return (OH_NativePointer) GetAUDIOAPIImpl(AUDIO_API_VERSION)->TonePlayer()->destruct;
}
KOALA_INTEROP_0(TonePlayer_getFinalizer, OH_NativePointer)
void impl_TonePlayer_load0(OH_NativePointer thisPtr, OH_Int32 type, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        const OH_AUDIO_audio_ToneType tmp = static_cast<OH_AUDIO_audio_ToneType>(type);
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->TonePlayer()->load0(thisPtr, &tmp, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V4(TonePlayer_load0, OH_NativePointer, OH_Int32, uint8_t*, int32_t)
void impl_TonePlayer_load1(OH_NativePointer thisPtr, OH_Int32 type) {
    const OH_AUDIO_audio_ToneType tmp = static_cast<OH_AUDIO_audio_ToneType>(type);
    GetAUDIOAPIImpl(AUDIO_API_VERSION)->TonePlayer()->load1(thisPtr, &tmp);
}
KOALA_INTEROP_V2(TonePlayer_load1, OH_NativePointer, OH_Int32)
void impl_TonePlayer_start0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->TonePlayer()->start0(thisPtr, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V3(TonePlayer_start0, OH_NativePointer, uint8_t*, int32_t)
void impl_TonePlayer_start1(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->TonePlayer()->start1(thisPtr);
}
KOALA_INTEROP_V1(TonePlayer_start1, OH_NativePointer)
void impl_TonePlayer_stop0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->TonePlayer()->stop0(thisPtr, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V3(TonePlayer_stop0, OH_NativePointer, uint8_t*, int32_t)
void impl_TonePlayer_stop1(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->TonePlayer()->stop1(thisPtr);
}
KOALA_INTEROP_V1(TonePlayer_stop1, OH_NativePointer)
void impl_TonePlayer_release0(OH_NativePointer thisPtr, uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        AUDIO_AsyncCallback_Void callback__value = {thisDeserializer.readCallbackResource(), reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCaller(Kind_AsyncCallback_Void)))), reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointerOrDefault(reinterpret_cast<OH_NativePointer>(getManagedCallbackCallerSync(Kind_AsyncCallback_Void))))};;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->TonePlayer()->release0(thisPtr, (const AUDIO_AsyncCallback_Void*)&callback__value);
}
KOALA_INTEROP_V3(TonePlayer_release0, OH_NativePointer, uint8_t*, int32_t)
void impl_TonePlayer_release1(OH_NativePointer thisPtr) {
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->TonePlayer()->release1(thisPtr);
}
KOALA_INTEROP_V1(TonePlayer_release1, OH_NativePointer)
OH_NativePointer impl_GlobalScope_ohos_multimedia_audio_getAudioManager() {
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AUDIO()->getAudioManager();
}
KOALA_INTEROP_0(GlobalScope_ohos_multimedia_audio_getAudioManager, OH_NativePointer)
void impl_GlobalScope_ohos_multimedia_audio_createAudioCapturer0(uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_CustomObject options_value = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("AudioCapturerOptions"));;
        OH_CustomObject callback__value = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("AsyncCallback_AudioCapturer_Void"));;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AUDIO()->createAudioCapturer0((const OH_CustomObject*)&options_value, (const OH_CustomObject*)&callback__value);
}
KOALA_INTEROP_V2(GlobalScope_ohos_multimedia_audio_createAudioCapturer0, uint8_t*, int32_t)
OH_NativePointer impl_GlobalScope_ohos_multimedia_audio_createAudioCapturer1(uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_CustomObject options_value = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("AudioCapturerOptions"));;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AUDIO()->createAudioCapturer1((const OH_CustomObject*)&options_value);
}
KOALA_INTEROP_2(GlobalScope_ohos_multimedia_audio_createAudioCapturer1, OH_NativePointer, uint8_t*, int32_t)
void impl_GlobalScope_ohos_multimedia_audio_createAudioRenderer0(uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_CustomObject options_value = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("AudioRendererOptions"));;
        OH_CustomObject callback__value = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("AsyncCallback_AudioRenderer_Void"));;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AUDIO()->createAudioRenderer0((const OH_CustomObject*)&options_value, (const OH_CustomObject*)&callback__value);
}
KOALA_INTEROP_V2(GlobalScope_ohos_multimedia_audio_createAudioRenderer0, uint8_t*, int32_t)
OH_NativePointer impl_GlobalScope_ohos_multimedia_audio_createAudioRenderer1(uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_CustomObject options_value = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("AudioRendererOptions"));;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AUDIO()->createAudioRenderer1((const OH_CustomObject*)&options_value);
}
KOALA_INTEROP_2(GlobalScope_ohos_multimedia_audio_createAudioRenderer1, OH_NativePointer, uint8_t*, int32_t)
void impl_GlobalScope_ohos_multimedia_audio_createTonePlayer0(uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_CustomObject options_value = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("AudioRendererInfo"));;
        OH_CustomObject callback__value = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("AsyncCallback_TonePlayer_Void"));;
        GetAUDIOAPIImpl(AUDIO_API_VERSION)->AUDIO()->createTonePlayer0((const OH_CustomObject*)&options_value, (const OH_CustomObject*)&callback__value);
}
KOALA_INTEROP_V2(GlobalScope_ohos_multimedia_audio_createTonePlayer0, uint8_t*, int32_t)
OH_NativePointer impl_GlobalScope_ohos_multimedia_audio_createTonePlayer1(uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_CustomObject options_value = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("AudioRendererInfo"));;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AUDIO()->createTonePlayer1((const OH_CustomObject*)&options_value);
}
KOALA_INTEROP_2(GlobalScope_ohos_multimedia_audio_createTonePlayer1, OH_NativePointer, uint8_t*, int32_t)
OH_NativePointer impl_GlobalScope_ohos_multimedia_audio_createAsrProcessingController(uint8_t* thisArray, int32_t thisLength) {
        Deserializer thisDeserializer(thisArray, thisLength);
        OH_CustomObject audioCapturer_value = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("AudioCapturer"));;
        return GetAUDIOAPIImpl(AUDIO_API_VERSION)->AUDIO()->createAsrProcessingController((const OH_CustomObject*)&audioCapturer_value);
}
KOALA_INTEROP_2(GlobalScope_ohos_multimedia_audio_createAsrProcessingController, OH_NativePointer, uint8_t*, int32_t)
void deserializeAndCallAsyncCallback_AudioCapturer_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioCapturer result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_AudioCapturer result = static_cast<OH_AUDIO_AudioCapturer>(thisDeserializer.readAudioCapturer());
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_AudioCapturer_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioCapturer result)>(thisDeserializer.readPointer());
    OH_AUDIO_AudioCapturer result = static_cast<OH_AUDIO_AudioCapturer>(thisDeserializer.readAudioCapturer());
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_AudioCapturerChangeInfoArray_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    const OH_Int32 result_buf_length = thisDeserializer.readInt32();
    Array_CustomObject result_buf = {};
    thisDeserializer.resizeArray<std::decay<decltype(result_buf)>::type,
        std::decay<decltype(*result_buf.array)>::type>(&result_buf, result_buf_length);
    for (int result_buf_i = 0; result_buf_i < result_buf_length; result_buf_i++) {
        result_buf.array[result_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioCapturerChangeInfo>"));
    }
    Array_CustomObject result = result_buf;
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_AudioCapturerChangeInfoArray_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointer());
    const OH_Int32 result_buf_length = thisDeserializer.readInt32();
    Array_CustomObject result_buf = {};
    thisDeserializer.resizeArray<std::decay<decltype(result_buf)>::type,
        std::decay<decltype(*result_buf.array)>::type>(&result_buf, result_buf_length);
    for (int result_buf_i = 0; result_buf_i < result_buf_length; result_buf_i++) {
        result_buf.array[result_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioCapturerChangeInfo>"));
    }
    Array_CustomObject result = result_buf;
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_AudioCapturerInfo_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioCapturerInfo result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_AudioCapturerInfo result = thisDeserializer.readAudioCapturerInfo();
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_AudioCapturerInfo_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioCapturerInfo result)>(thisDeserializer.readPointer());
    OH_AUDIO_AudioCapturerInfo result = thisDeserializer.readAudioCapturerInfo();
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_AudioDeviceDescriptors_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    const OH_Int32 result_buf_length = thisDeserializer.readInt32();
    Array_CustomObject result_buf = {};
    thisDeserializer.resizeArray<std::decay<decltype(result_buf)>::type,
        std::decay<decltype(*result_buf.array)>::type>(&result_buf, result_buf_length);
    for (int result_buf_i = 0; result_buf_i < result_buf_length; result_buf_i++) {
        result_buf.array[result_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>"));
    }
    Array_CustomObject result = result_buf;
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_AudioDeviceDescriptors_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointer());
    const OH_Int32 result_buf_length = thisDeserializer.readInt32();
    Array_CustomObject result_buf = {};
    thisDeserializer.resizeArray<std::decay<decltype(result_buf)>::type,
        std::decay<decltype(*result_buf.array)>::type>(&result_buf, result_buf_length);
    for (int result_buf_i = 0; result_buf_i < result_buf_length; result_buf_i++) {
        result_buf.array[result_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>"));
    }
    Array_CustomObject result = result_buf;
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_AudioEffectInfoArray_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    const OH_Int32 result_buf_length = thisDeserializer.readInt32();
    Array_CustomObject result_buf = {};
    thisDeserializer.resizeArray<std::decay<decltype(result_buf)>::type,
        std::decay<decltype(*result_buf.array)>::type>(&result_buf, result_buf_length);
    for (int result_buf_i = 0; result_buf_i < result_buf_length; result_buf_i++) {
        result_buf.array[result_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioEffectMode>"));
    }
    Array_CustomObject result = result_buf;
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_AudioEffectInfoArray_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointer());
    const OH_Int32 result_buf_length = thisDeserializer.readInt32();
    Array_CustomObject result_buf = {};
    thisDeserializer.resizeArray<std::decay<decltype(result_buf)>::type,
        std::decay<decltype(*result_buf.array)>::type>(&result_buf, result_buf_length);
    for (int result_buf_i = 0; result_buf_i < result_buf_length; result_buf_i++) {
        result_buf.array[result_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioEffectMode>"));
    }
    Array_CustomObject result = result_buf;
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_AudioEffectMode_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioEffectMode result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_audio_AudioEffectMode result = static_cast<OH_AUDIO_audio_AudioEffectMode>(thisDeserializer.readInt32());
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_AudioEffectMode_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, OH_AUDIO_audio_AudioEffectMode result)>(thisDeserializer.readPointer());
    OH_AUDIO_audio_AudioEffectMode result = static_cast<OH_AUDIO_audio_AudioEffectMode>(thisDeserializer.readInt32());
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_AudioRenderer_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioRenderer result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_AudioRenderer result = static_cast<OH_AUDIO_AudioRenderer>(thisDeserializer.readAudioRenderer());
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_AudioRenderer_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioRenderer result)>(thisDeserializer.readPointer());
    OH_AUDIO_AudioRenderer result = static_cast<OH_AUDIO_AudioRenderer>(thisDeserializer.readAudioRenderer());
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_AudioRendererChangeInfoArray_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    const OH_Int32 result_buf_length = thisDeserializer.readInt32();
    Array_CustomObject result_buf = {};
    thisDeserializer.resizeArray<std::decay<decltype(result_buf)>::type,
        std::decay<decltype(*result_buf.array)>::type>(&result_buf, result_buf_length);
    for (int result_buf_i = 0; result_buf_i < result_buf_length; result_buf_i++) {
        result_buf.array[result_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioRendererChangeInfo>"));
    }
    Array_CustomObject result = result_buf;
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_AudioRendererChangeInfoArray_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointer());
    const OH_Int32 result_buf_length = thisDeserializer.readInt32();
    Array_CustomObject result_buf = {};
    thisDeserializer.resizeArray<std::decay<decltype(result_buf)>::type,
        std::decay<decltype(*result_buf.array)>::type>(&result_buf, result_buf_length);
    for (int result_buf_i = 0; result_buf_i < result_buf_length; result_buf_i++) {
        result_buf.array[result_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioRendererChangeInfo>"));
    }
    Array_CustomObject result = result_buf;
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_AudioRendererInfo_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioRendererInfo result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_AudioRendererInfo result = thisDeserializer.readAudioRendererInfo();
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_AudioRendererInfo_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioRendererInfo result)>(thisDeserializer.readPointer());
    OH_AUDIO_AudioRendererInfo result = thisDeserializer.readAudioRendererInfo();
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_AudioRendererRate_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioRendererRate result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_audio_AudioRendererRate result = static_cast<OH_AUDIO_audio_AudioRendererRate>(thisDeserializer.readInt32());
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_AudioRendererRate_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, OH_AUDIO_audio_AudioRendererRate result)>(thisDeserializer.readPointer());
    OH_AUDIO_audio_AudioRendererRate result = static_cast<OH_AUDIO_audio_AudioRendererRate>(thisDeserializer.readInt32());
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_AudioRingMode_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_audio_AudioRingMode result = static_cast<OH_AUDIO_audio_AudioRingMode>(thisDeserializer.readInt32());
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_AudioRingMode_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode result)>(thisDeserializer.readPointer());
    OH_AUDIO_audio_AudioRingMode result = static_cast<OH_AUDIO_audio_AudioRingMode>(thisDeserializer.readInt32());
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_AudioScene_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioScene result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_audio_AudioScene result = static_cast<OH_AUDIO_audio_AudioScene>(thisDeserializer.readInt32());
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_AudioScene_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, OH_AUDIO_audio_AudioScene result)>(thisDeserializer.readPointer());
    OH_AUDIO_audio_AudioScene result = static_cast<OH_AUDIO_audio_AudioScene>(thisDeserializer.readInt32());
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_AudioStreamInfo_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioStreamInfo result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_AudioStreamInfo result = thisDeserializer.readAudioStreamInfo();
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_AudioStreamInfo_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioStreamInfo result)>(thisDeserializer.readPointer());
    OH_AUDIO_AudioStreamInfo result = thisDeserializer.readAudioStreamInfo();
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_AudioVolumeGroupManager_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioVolumeGroupManager result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_AudioVolumeGroupManager result = static_cast<OH_AUDIO_AudioVolumeGroupManager>(thisDeserializer.readAudioVolumeGroupManager());
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_AudioVolumeGroupManager_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioVolumeGroupManager result)>(thisDeserializer.readPointer());
    OH_AUDIO_AudioVolumeGroupManager result = static_cast<OH_AUDIO_AudioVolumeGroupManager>(thisDeserializer.readAudioVolumeGroupManager());
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_Boolean_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_Boolean result = thisDeserializer.readBoolean();
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_Boolean_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Boolean result)>(thisDeserializer.readPointer());
    OH_Boolean result = thisDeserializer.readBoolean();
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_Buffer_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Buffer result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_Buffer result = static_cast<OH_Buffer>(thisDeserializer.readBuffer());
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_Buffer_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Buffer result)>(thisDeserializer.readPointer());
    OH_Buffer result = static_cast<OH_Buffer>(thisDeserializer.readBuffer());
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_Number_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_Number result = static_cast<OH_Number>(thisDeserializer.readNumber());
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_Number_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number result)>(thisDeserializer.readPointer());
    OH_Number result = static_cast<OH_Number>(thisDeserializer.readNumber());
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_String_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_String result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_String result = static_cast<OH_String>(thisDeserializer.readString());
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_String_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_String result)>(thisDeserializer.readPointer());
    OH_String result = static_cast<OH_String>(thisDeserializer.readString());
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_TonePlayer_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_TonePlayer result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_TonePlayer result = static_cast<OH_AUDIO_TonePlayer>(thisDeserializer.readTonePlayer());
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_TonePlayer_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_TonePlayer result)>(thisDeserializer.readPointer());
    OH_AUDIO_TonePlayer result = static_cast<OH_AUDIO_TonePlayer>(thisDeserializer.readTonePlayer());
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAsyncCallback_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    _call(_resourceId);
}
void deserializeAndCallSyncAsyncCallback_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId)>(thisDeserializer.readPointer());
    _callSync(vmContext, _resourceId);
}
void deserializeAndCallAsyncCallback_VolumeGroupInfos_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    const OH_Int32 result_buf_length = thisDeserializer.readInt32();
    Array_CustomObject result_buf = {};
    thisDeserializer.resizeArray<std::decay<decltype(result_buf)>::type,
        std::decay<decltype(*result_buf.array)>::type>(&result_buf, result_buf_length);
    for (int result_buf_i = 0; result_buf_i < result_buf_length; result_buf_i++) {
        result_buf.array[result_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<VolumeGroupInfo>"));
    }
    Array_CustomObject result = result_buf;
    _call(_resourceId, result);
}
void deserializeAndCallSyncAsyncCallback_VolumeGroupInfos_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject result)>(thisDeserializer.readPointer());
    const OH_Int32 result_buf_length = thisDeserializer.readInt32();
    Array_CustomObject result_buf = {};
    thisDeserializer.resizeArray<std::decay<decltype(result_buf)>::type,
        std::decay<decltype(*result_buf.array)>::type>(&result_buf, result_buf_length);
    for (int result_buf_i = 0; result_buf_i < result_buf_length; result_buf_i++) {
        result_buf.array[result_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<VolumeGroupInfo>"));
    }
    Array_CustomObject result = result_buf;
    _callSync(vmContext, _resourceId, result);
}
void deserializeAndCallAudioRendererWriteDataCallback(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Buffer data, const OH_CustomObject continuation)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_Buffer data = static_cast<OH_Buffer>(thisDeserializer.readBuffer());
    OH_CustomObject _continuation = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Callback_AudioDataCallbackResult_Void"));
    _call(_resourceId, data, _continuation);
}
void deserializeAndCallSyncAudioRendererWriteDataCallback(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Buffer data, const OH_CustomObject continuation)>(thisDeserializer.readPointer());
    OH_Buffer data = static_cast<OH_Buffer>(thisDeserializer.readBuffer());
    OH_CustomObject _continuation = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Callback_AudioDataCallbackResult_Void"));
    _callSync(vmContext, _resourceId, data, _continuation);
}
void deserializeAndCallCallback_AudioCapturerChangeInfo_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioCapturerChangeInfo parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_AudioCapturerChangeInfo parameter = thisDeserializer.readAudioCapturerChangeInfo();
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_AudioCapturerChangeInfo_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioCapturerChangeInfo parameter)>(thisDeserializer.readPointer());
    OH_AUDIO_AudioCapturerChangeInfo parameter = thisDeserializer.readAudioCapturerChangeInfo();
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_AudioCapturerChangeInfoArray_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    const OH_Int32 parameter_buf_length = thisDeserializer.readInt32();
    Array_CustomObject parameter_buf = {};
    thisDeserializer.resizeArray<std::decay<decltype(parameter_buf)>::type,
        std::decay<decltype(*parameter_buf.array)>::type>(&parameter_buf, parameter_buf_length);
    for (int parameter_buf_i = 0; parameter_buf_i < parameter_buf_length; parameter_buf_i++) {
        parameter_buf.array[parameter_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioCapturerChangeInfo>"));
    }
    Array_CustomObject parameter = parameter_buf;
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_AudioCapturerChangeInfoArray_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointer());
    const OH_Int32 parameter_buf_length = thisDeserializer.readInt32();
    Array_CustomObject parameter_buf = {};
    thisDeserializer.resizeArray<std::decay<decltype(parameter_buf)>::type,
        std::decay<decltype(*parameter_buf.array)>::type>(&parameter_buf, parameter_buf_length);
    for (int parameter_buf_i = 0; parameter_buf_i < parameter_buf_length; parameter_buf_i++) {
        parameter_buf.array[parameter_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioCapturerChangeInfo>"));
    }
    Array_CustomObject parameter = parameter_buf;
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_AudioDeviceDescriptors_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    const OH_Int32 parameter_buf_length = thisDeserializer.readInt32();
    Array_CustomObject parameter_buf = {};
    thisDeserializer.resizeArray<std::decay<decltype(parameter_buf)>::type,
        std::decay<decltype(*parameter_buf.array)>::type>(&parameter_buf, parameter_buf_length);
    for (int parameter_buf_i = 0; parameter_buf_i < parameter_buf_length; parameter_buf_i++) {
        parameter_buf.array[parameter_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>"));
    }
    Array_CustomObject parameter = parameter_buf;
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_AudioDeviceDescriptors_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointer());
    const OH_Int32 parameter_buf_length = thisDeserializer.readInt32();
    Array_CustomObject parameter_buf = {};
    thisDeserializer.resizeArray<std::decay<decltype(parameter_buf)>::type,
        std::decay<decltype(*parameter_buf.array)>::type>(&parameter_buf, parameter_buf_length);
    for (int parameter_buf_i = 0; parameter_buf_i < parameter_buf_length; parameter_buf_i++) {
        parameter_buf.array[parameter_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioDeviceDescriptor>"));
    }
    Array_CustomObject parameter = parameter_buf;
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_AudioRendererChangeInfoArray_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    const OH_Int32 parameter_buf_length = thisDeserializer.readInt32();
    Array_CustomObject parameter_buf = {};
    thisDeserializer.resizeArray<std::decay<decltype(parameter_buf)>::type,
        std::decay<decltype(*parameter_buf.array)>::type>(&parameter_buf, parameter_buf_length);
    for (int parameter_buf_i = 0; parameter_buf_i < parameter_buf_length; parameter_buf_i++) {
        parameter_buf.array[parameter_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioRendererChangeInfo>"));
    }
    Array_CustomObject parameter = parameter_buf;
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_AudioRendererChangeInfoArray_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const Array_CustomObject parameter)>(thisDeserializer.readPointer());
    const OH_Int32 parameter_buf_length = thisDeserializer.readInt32();
    Array_CustomObject parameter_buf = {};
    thisDeserializer.resizeArray<std::decay<decltype(parameter_buf)>::type,
        std::decay<decltype(*parameter_buf.array)>::type>(&parameter_buf, parameter_buf_length);
    for (int parameter_buf_i = 0; parameter_buf_i < parameter_buf_length; parameter_buf_i++) {
        parameter_buf.array[parameter_buf_i] = static_cast<OH_CustomObject>(thisDeserializer.readCustomObject("Readonly<AudioRendererChangeInfo>"));
    }
    Array_CustomObject parameter = parameter_buf;
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_AudioRingMode_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_audio_AudioRingMode parameter = static_cast<OH_AUDIO_audio_AudioRingMode>(thisDeserializer.readInt32());
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_AudioRingMode_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode parameter)>(thisDeserializer.readPointer());
    OH_AUDIO_audio_AudioRingMode parameter = static_cast<OH_AUDIO_audio_AudioRingMode>(thisDeserializer.readInt32());
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_AudioSessionDeactivatedEvent_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioSessionDeactivatedEvent parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_AudioSessionDeactivatedEvent parameter = thisDeserializer.readAudioSessionDeactivatedEvent();
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_AudioSessionDeactivatedEvent_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioSessionDeactivatedEvent parameter)>(thisDeserializer.readPointer());
    OH_AUDIO_AudioSessionDeactivatedEvent parameter = thisDeserializer.readAudioSessionDeactivatedEvent();
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_AudioSpatialEnabledStateForDevice_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioSpatialEnabledStateForDevice parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_AudioSpatialEnabledStateForDevice parameter = thisDeserializer.readAudioSpatialEnabledStateForDevice();
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_AudioSpatialEnabledStateForDevice_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioSpatialEnabledStateForDevice parameter)>(thisDeserializer.readPointer());
    OH_AUDIO_AudioSpatialEnabledStateForDevice parameter = thisDeserializer.readAudioSpatialEnabledStateForDevice();
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_AudioState_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, OH_AUDIO_audio_AudioState parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_audio_AudioState parameter = static_cast<OH_AUDIO_audio_AudioState>(thisDeserializer.readInt32());
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_AudioState_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, OH_AUDIO_audio_AudioState parameter)>(thisDeserializer.readPointer());
    OH_AUDIO_audio_AudioState parameter = static_cast<OH_AUDIO_audio_AudioState>(thisDeserializer.readInt32());
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_AudioStreamDeviceChangeInfo_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_AudioStreamDeviceChangeInfo parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_AudioStreamDeviceChangeInfo parameter = thisDeserializer.readAudioStreamDeviceChangeInfo();
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_AudioStreamDeviceChangeInfo_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_AudioStreamDeviceChangeInfo parameter)>(thisDeserializer.readPointer());
    OH_AUDIO_AudioStreamDeviceChangeInfo parameter = thisDeserializer.readAudioStreamDeviceChangeInfo();
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_Boolean_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Boolean parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_Boolean parameter = thisDeserializer.readBoolean();
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_Boolean_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Boolean parameter)>(thisDeserializer.readPointer());
    OH_Boolean parameter = thisDeserializer.readBoolean();
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_Buffer_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Buffer parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_Buffer parameter = static_cast<OH_Buffer>(thisDeserializer.readBuffer());
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_Buffer_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Buffer parameter)>(thisDeserializer.readPointer());
    OH_Buffer parameter = static_cast<OH_Buffer>(thisDeserializer.readBuffer());
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_DeviceBlockStatusInfo_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_DeviceBlockStatusInfo parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_DeviceBlockStatusInfo parameter = thisDeserializer.readDeviceBlockStatusInfo();
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_DeviceBlockStatusInfo_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_DeviceBlockStatusInfo parameter)>(thisDeserializer.readPointer());
    OH_AUDIO_DeviceBlockStatusInfo parameter = thisDeserializer.readDeviceBlockStatusInfo();
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_DeviceChangeAction_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_DeviceChangeAction parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_DeviceChangeAction parameter = thisDeserializer.readDeviceChangeAction();
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_DeviceChangeAction_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_DeviceChangeAction parameter)>(thisDeserializer.readPointer());
    OH_AUDIO_DeviceChangeAction parameter = thisDeserializer.readDeviceChangeAction();
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_InterruptAction_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_InterruptAction parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_InterruptAction parameter = thisDeserializer.readInterruptAction();
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_InterruptAction_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_InterruptAction parameter)>(thisDeserializer.readPointer());
    OH_AUDIO_InterruptAction parameter = thisDeserializer.readInterruptAction();
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_InterruptEvent_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_InterruptEvent parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_InterruptEvent parameter = thisDeserializer.readInterruptEvent();
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_InterruptEvent_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_InterruptEvent parameter)>(thisDeserializer.readPointer());
    OH_AUDIO_InterruptEvent parameter = thisDeserializer.readInterruptEvent();
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_MicStateChangeEvent_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_MicStateChangeEvent parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_MicStateChangeEvent parameter = thisDeserializer.readMicStateChangeEvent();
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_MicStateChangeEvent_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_MicStateChangeEvent parameter)>(thisDeserializer.readPointer());
    OH_AUDIO_MicStateChangeEvent parameter = thisDeserializer.readMicStateChangeEvent();
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_Number_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_Number parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_Number parameter = static_cast<OH_Number>(thisDeserializer.readNumber());
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_Number_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_Number parameter)>(thisDeserializer.readPointer());
    OH_Number parameter = static_cast<OH_Number>(thisDeserializer.readNumber());
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback_VolumeEvent_Void(uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    const auto _call = reinterpret_cast<void(*)(const OH_Int32 resourceId, const OH_AUDIO_VolumeEvent parameter)>(thisDeserializer.readPointer());
    thisDeserializer.readPointer();
    OH_AUDIO_VolumeEvent parameter = thisDeserializer.readVolumeEvent();
    _call(_resourceId, parameter);
}
void deserializeAndCallSyncCallback_VolumeEvent_Void(OH_AUDIO_VMContext vmContext, uint8_t* thisArray, OH_Int32 thisLength)
{
    Deserializer thisDeserializer = Deserializer(thisArray, thisLength);
    const OH_Int32 _resourceId = thisDeserializer.readInt32();
    thisDeserializer.readPointer();
    const auto _callSync = reinterpret_cast<void(*)(OH_AUDIO_VMContext vmContext, const OH_Int32 resourceId, const OH_AUDIO_VolumeEvent parameter)>(thisDeserializer.readPointer());
    OH_AUDIO_VolumeEvent parameter = thisDeserializer.readVolumeEvent();
    _callSync(vmContext, _resourceId, parameter);
}
void deserializeAndCallCallback(OH_Int32 kind, uint8_t* thisArray, OH_Int32 thisLength)
{
    switch (kind) {
        case 103731593/*Kind_AsyncCallback_AudioCapturer_Void*/: return deserializeAndCallAsyncCallback_AudioCapturer_Void(thisArray, thisLength);
        case 605900120/*Kind_AsyncCallback_AudioCapturerChangeInfoArray_Void*/: return deserializeAndCallAsyncCallback_AudioCapturerChangeInfoArray_Void(thisArray, thisLength);
        case 945856903/*Kind_AsyncCallback_AudioCapturerInfo_Void*/: return deserializeAndCallAsyncCallback_AudioCapturerInfo_Void(thisArray, thisLength);
        case 1103131879/*Kind_AsyncCallback_AudioDeviceDescriptors_Void*/: return deserializeAndCallAsyncCallback_AudioDeviceDescriptors_Void(thisArray, thisLength);
        case -1801809985/*Kind_AsyncCallback_AudioEffectInfoArray_Void*/: return deserializeAndCallAsyncCallback_AudioEffectInfoArray_Void(thisArray, thisLength);
        case -852719253/*Kind_AsyncCallback_AudioEffectMode_Void*/: return deserializeAndCallAsyncCallback_AudioEffectMode_Void(thisArray, thisLength);
        case -1795948094/*Kind_AsyncCallback_AudioRenderer_Void*/: return deserializeAndCallAsyncCallback_AudioRenderer_Void(thisArray, thisLength);
        case 419082675/*Kind_AsyncCallback_AudioRendererChangeInfoArray_Void*/: return deserializeAndCallAsyncCallback_AudioRendererChangeInfoArray_Void(thisArray, thisLength);
        case 721972620/*Kind_AsyncCallback_AudioRendererInfo_Void*/: return deserializeAndCallAsyncCallback_AudioRendererInfo_Void(thisArray, thisLength);
        case 147576480/*Kind_AsyncCallback_AudioRendererRate_Void*/: return deserializeAndCallAsyncCallback_AudioRendererRate_Void(thisArray, thisLength);
        case -815244914/*Kind_AsyncCallback_AudioRingMode_Void*/: return deserializeAndCallAsyncCallback_AudioRingMode_Void(thisArray, thisLength);
        case -854078751/*Kind_AsyncCallback_AudioScene_Void*/: return deserializeAndCallAsyncCallback_AudioScene_Void(thisArray, thisLength);
        case -106617875/*Kind_AsyncCallback_AudioStreamInfo_Void*/: return deserializeAndCallAsyncCallback_AudioStreamInfo_Void(thisArray, thisLength);
        case 540266043/*Kind_AsyncCallback_AudioVolumeGroupManager_Void*/: return deserializeAndCallAsyncCallback_AudioVolumeGroupManager_Void(thisArray, thisLength);
        case 46391693/*Kind_AsyncCallback_Boolean_Void*/: return deserializeAndCallAsyncCallback_Boolean_Void(thisArray, thisLength);
        case -1662321143/*Kind_AsyncCallback_Buffer_Void*/: return deserializeAndCallAsyncCallback_Buffer_Void(thisArray, thisLength);
        case 1959553162/*Kind_AsyncCallback_Number_Void*/: return deserializeAndCallAsyncCallback_Number_Void(thisArray, thisLength);
        case 789188988/*Kind_AsyncCallback_String_Void*/: return deserializeAndCallAsyncCallback_String_Void(thisArray, thisLength);
        case 895924586/*Kind_AsyncCallback_TonePlayer_Void*/: return deserializeAndCallAsyncCallback_TonePlayer_Void(thisArray, thisLength);
        case 1075219926/*Kind_AsyncCallback_Void*/: return deserializeAndCallAsyncCallback_Void(thisArray, thisLength);
        case -801079837/*Kind_AsyncCallback_VolumeGroupInfos_Void*/: return deserializeAndCallAsyncCallback_VolumeGroupInfos_Void(thisArray, thisLength);
        case -1508727875/*Kind_AudioRendererWriteDataCallback*/: return deserializeAndCallAudioRendererWriteDataCallback(thisArray, thisLength);
        case 300211623/*Kind_Callback_AudioCapturerChangeInfo_Void*/: return deserializeAndCallCallback_AudioCapturerChangeInfo_Void(thisArray, thisLength);
        case -788037890/*Kind_Callback_AudioCapturerChangeInfoArray_Void*/: return deserializeAndCallCallback_AudioCapturerChangeInfoArray_Void(thisArray, thisLength);
        case -872298751/*Kind_Callback_AudioDeviceDescriptors_Void*/: return deserializeAndCallCallback_AudioDeviceDescriptors_Void(thisArray, thisLength);
        case 1609768789/*Kind_Callback_AudioRendererChangeInfoArray_Void*/: return deserializeAndCallCallback_AudioRendererChangeInfoArray_Void(thisArray, thisLength);
        case -1700902488/*Kind_Callback_AudioRingMode_Void*/: return deserializeAndCallCallback_AudioRingMode_Void(thisArray, thisLength);
        case 1744071031/*Kind_Callback_AudioSessionDeactivatedEvent_Void*/: return deserializeAndCallCallback_AudioSessionDeactivatedEvent_Void(thisArray, thisLength);
        case 1057396442/*Kind_Callback_AudioSpatialEnabledStateForDevice_Void*/: return deserializeAndCallCallback_AudioSpatialEnabledStateForDevice_Void(thisArray, thisLength);
        case 558001102/*Kind_Callback_AudioState_Void*/: return deserializeAndCallCallback_AudioState_Void(thisArray, thisLength);
        case 1823460565/*Kind_Callback_AudioStreamDeviceChangeInfo_Void*/: return deserializeAndCallCallback_AudioStreamDeviceChangeInfo_Void(thisArray, thisLength);
        case 313269291/*Kind_Callback_Boolean_Void*/: return deserializeAndCallCallback_Boolean_Void(thisArray, thisLength);
        case 908731311/*Kind_Callback_Buffer_Void*/: return deserializeAndCallCallback_Buffer_Void(thisArray, thisLength);
        case -761620636/*Kind_Callback_DeviceBlockStatusInfo_Void*/: return deserializeAndCallCallback_DeviceBlockStatusInfo_Void(thisArray, thisLength);
        case 892121871/*Kind_Callback_DeviceChangeAction_Void*/: return deserializeAndCallCallback_DeviceChangeAction_Void(thisArray, thisLength);
        case -1389231466/*Kind_Callback_InterruptAction_Void*/: return deserializeAndCallCallback_InterruptAction_Void(thisArray, thisLength);
        case 638628164/*Kind_Callback_InterruptEvent_Void*/: return deserializeAndCallCallback_InterruptEvent_Void(thisArray, thisLength);
        case -1553290571/*Kind_Callback_MicStateChangeEvent_Void*/: return deserializeAndCallCallback_MicStateChangeEvent_Void(thisArray, thisLength);
        case 36519084/*Kind_Callback_Number_Void*/: return deserializeAndCallCallback_Number_Void(thisArray, thisLength);
        case 311503723/*Kind_Callback_VolumeEvent_Void*/: return deserializeAndCallCallback_VolumeEvent_Void(thisArray, thisLength);
    }
    printf("Unknown callback kind\n");
}
void deserializeAndCallCallbackSync(OH_AUDIO_VMContext vmContext, OH_Int32 kind, uint8_t* thisArray, OH_Int32 thisLength)
{
    switch (kind) {
        case 103731593/*Kind_AsyncCallback_AudioCapturer_Void*/: return deserializeAndCallSyncAsyncCallback_AudioCapturer_Void(vmContext, thisArray, thisLength);
        case 605900120/*Kind_AsyncCallback_AudioCapturerChangeInfoArray_Void*/: return deserializeAndCallSyncAsyncCallback_AudioCapturerChangeInfoArray_Void(vmContext, thisArray, thisLength);
        case 945856903/*Kind_AsyncCallback_AudioCapturerInfo_Void*/: return deserializeAndCallSyncAsyncCallback_AudioCapturerInfo_Void(vmContext, thisArray, thisLength);
        case 1103131879/*Kind_AsyncCallback_AudioDeviceDescriptors_Void*/: return deserializeAndCallSyncAsyncCallback_AudioDeviceDescriptors_Void(vmContext, thisArray, thisLength);
        case -1801809985/*Kind_AsyncCallback_AudioEffectInfoArray_Void*/: return deserializeAndCallSyncAsyncCallback_AudioEffectInfoArray_Void(vmContext, thisArray, thisLength);
        case -852719253/*Kind_AsyncCallback_AudioEffectMode_Void*/: return deserializeAndCallSyncAsyncCallback_AudioEffectMode_Void(vmContext, thisArray, thisLength);
        case -1795948094/*Kind_AsyncCallback_AudioRenderer_Void*/: return deserializeAndCallSyncAsyncCallback_AudioRenderer_Void(vmContext, thisArray, thisLength);
        case 419082675/*Kind_AsyncCallback_AudioRendererChangeInfoArray_Void*/: return deserializeAndCallSyncAsyncCallback_AudioRendererChangeInfoArray_Void(vmContext, thisArray, thisLength);
        case 721972620/*Kind_AsyncCallback_AudioRendererInfo_Void*/: return deserializeAndCallSyncAsyncCallback_AudioRendererInfo_Void(vmContext, thisArray, thisLength);
        case 147576480/*Kind_AsyncCallback_AudioRendererRate_Void*/: return deserializeAndCallSyncAsyncCallback_AudioRendererRate_Void(vmContext, thisArray, thisLength);
        case -815244914/*Kind_AsyncCallback_AudioRingMode_Void*/: return deserializeAndCallSyncAsyncCallback_AudioRingMode_Void(vmContext, thisArray, thisLength);
        case -854078751/*Kind_AsyncCallback_AudioScene_Void*/: return deserializeAndCallSyncAsyncCallback_AudioScene_Void(vmContext, thisArray, thisLength);
        case -106617875/*Kind_AsyncCallback_AudioStreamInfo_Void*/: return deserializeAndCallSyncAsyncCallback_AudioStreamInfo_Void(vmContext, thisArray, thisLength);
        case 540266043/*Kind_AsyncCallback_AudioVolumeGroupManager_Void*/: return deserializeAndCallSyncAsyncCallback_AudioVolumeGroupManager_Void(vmContext, thisArray, thisLength);
        case 46391693/*Kind_AsyncCallback_Boolean_Void*/: return deserializeAndCallSyncAsyncCallback_Boolean_Void(vmContext, thisArray, thisLength);
        case -1662321143/*Kind_AsyncCallback_Buffer_Void*/: return deserializeAndCallSyncAsyncCallback_Buffer_Void(vmContext, thisArray, thisLength);
        case 1959553162/*Kind_AsyncCallback_Number_Void*/: return deserializeAndCallSyncAsyncCallback_Number_Void(vmContext, thisArray, thisLength);
        case 789188988/*Kind_AsyncCallback_String_Void*/: return deserializeAndCallSyncAsyncCallback_String_Void(vmContext, thisArray, thisLength);
        case 895924586/*Kind_AsyncCallback_TonePlayer_Void*/: return deserializeAndCallSyncAsyncCallback_TonePlayer_Void(vmContext, thisArray, thisLength);
        case 1075219926/*Kind_AsyncCallback_Void*/: return deserializeAndCallSyncAsyncCallback_Void(vmContext, thisArray, thisLength);
        case -801079837/*Kind_AsyncCallback_VolumeGroupInfos_Void*/: return deserializeAndCallSyncAsyncCallback_VolumeGroupInfos_Void(vmContext, thisArray, thisLength);
        case -1508727875/*Kind_AudioRendererWriteDataCallback*/: return deserializeAndCallSyncAudioRendererWriteDataCallback(vmContext, thisArray, thisLength);
        case 300211623/*Kind_Callback_AudioCapturerChangeInfo_Void*/: return deserializeAndCallSyncCallback_AudioCapturerChangeInfo_Void(vmContext, thisArray, thisLength);
        case -788037890/*Kind_Callback_AudioCapturerChangeInfoArray_Void*/: return deserializeAndCallSyncCallback_AudioCapturerChangeInfoArray_Void(vmContext, thisArray, thisLength);
        case -872298751/*Kind_Callback_AudioDeviceDescriptors_Void*/: return deserializeAndCallSyncCallback_AudioDeviceDescriptors_Void(vmContext, thisArray, thisLength);
        case 1609768789/*Kind_Callback_AudioRendererChangeInfoArray_Void*/: return deserializeAndCallSyncCallback_AudioRendererChangeInfoArray_Void(vmContext, thisArray, thisLength);
        case -1700902488/*Kind_Callback_AudioRingMode_Void*/: return deserializeAndCallSyncCallback_AudioRingMode_Void(vmContext, thisArray, thisLength);
        case 1744071031/*Kind_Callback_AudioSessionDeactivatedEvent_Void*/: return deserializeAndCallSyncCallback_AudioSessionDeactivatedEvent_Void(vmContext, thisArray, thisLength);
        case 1057396442/*Kind_Callback_AudioSpatialEnabledStateForDevice_Void*/: return deserializeAndCallSyncCallback_AudioSpatialEnabledStateForDevice_Void(vmContext, thisArray, thisLength);
        case 558001102/*Kind_Callback_AudioState_Void*/: return deserializeAndCallSyncCallback_AudioState_Void(vmContext, thisArray, thisLength);
        case 1823460565/*Kind_Callback_AudioStreamDeviceChangeInfo_Void*/: return deserializeAndCallSyncCallback_AudioStreamDeviceChangeInfo_Void(vmContext, thisArray, thisLength);
        case 313269291/*Kind_Callback_Boolean_Void*/: return deserializeAndCallSyncCallback_Boolean_Void(vmContext, thisArray, thisLength);
        case 908731311/*Kind_Callback_Buffer_Void*/: return deserializeAndCallSyncCallback_Buffer_Void(vmContext, thisArray, thisLength);
        case -761620636/*Kind_Callback_DeviceBlockStatusInfo_Void*/: return deserializeAndCallSyncCallback_DeviceBlockStatusInfo_Void(vmContext, thisArray, thisLength);
        case 892121871/*Kind_Callback_DeviceChangeAction_Void*/: return deserializeAndCallSyncCallback_DeviceChangeAction_Void(vmContext, thisArray, thisLength);
        case -1389231466/*Kind_Callback_InterruptAction_Void*/: return deserializeAndCallSyncCallback_InterruptAction_Void(vmContext, thisArray, thisLength);
        case 638628164/*Kind_Callback_InterruptEvent_Void*/: return deserializeAndCallSyncCallback_InterruptEvent_Void(vmContext, thisArray, thisLength);
        case -1553290571/*Kind_Callback_MicStateChangeEvent_Void*/: return deserializeAndCallSyncCallback_MicStateChangeEvent_Void(vmContext, thisArray, thisLength);
        case 36519084/*Kind_Callback_Number_Void*/: return deserializeAndCallSyncCallback_Number_Void(vmContext, thisArray, thisLength);
        case 311503723/*Kind_Callback_VolumeEvent_Void*/: return deserializeAndCallSyncCallback_VolumeEvent_Void(vmContext, thisArray, thisLength);
    }
    printf("Unknown callback kind\n");
}
void callManagedAsyncCallback_AudioCapturer_Void(OH_Int32 resourceId, OH_AUDIO_AudioCapturer result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioCapturer_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioCapturer(result);
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_AudioCapturer_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_AudioCapturer result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioCapturer_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioCapturer(result);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_AudioCapturerChangeInfoArray_Void(OH_Int32 resourceId, Array_CustomObject result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioCapturerChangeInfoArray_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(result.length);
    for (int i = 0; i < result.length; i++) {
        const OH_CustomObject result_element = result.array[i];
        argsSerializer.writeCustomObject("Readonly<AudioCapturerChangeInfo>", result_element);
    }
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_AudioCapturerChangeInfoArray_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, Array_CustomObject result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioCapturerChangeInfoArray_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(result.length);
    for (int i = 0; i < result.length; i++) {
        const OH_CustomObject result_element = result.array[i];
        argsSerializer.writeCustomObject("Readonly<AudioCapturerChangeInfo>", result_element);
    }
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_AudioCapturerInfo_Void(OH_Int32 resourceId, OH_AUDIO_AudioCapturerInfo result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioCapturerInfo_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioCapturerInfo(result);
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_AudioCapturerInfo_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_AudioCapturerInfo result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioCapturerInfo_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioCapturerInfo(result);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_AudioDeviceDescriptors_Void(OH_Int32 resourceId, Array_CustomObject result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioDeviceDescriptors_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(result.length);
    for (int i = 0; i < result.length; i++) {
        const OH_CustomObject result_element = result.array[i];
        argsSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", result_element);
    }
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_AudioDeviceDescriptors_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, Array_CustomObject result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioDeviceDescriptors_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(result.length);
    for (int i = 0; i < result.length; i++) {
        const OH_CustomObject result_element = result.array[i];
        argsSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", result_element);
    }
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_AudioEffectInfoArray_Void(OH_Int32 resourceId, Array_CustomObject result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioEffectInfoArray_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(result.length);
    for (int i = 0; i < result.length; i++) {
        const OH_CustomObject result_element = result.array[i];
        argsSerializer.writeCustomObject("Readonly<AudioEffectMode>", result_element);
    }
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_AudioEffectInfoArray_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, Array_CustomObject result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioEffectInfoArray_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(result.length);
    for (int i = 0; i < result.length; i++) {
        const OH_CustomObject result_element = result.array[i];
        argsSerializer.writeCustomObject("Readonly<AudioEffectMode>", result_element);
    }
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_AudioEffectMode_Void(OH_Int32 resourceId, OH_AUDIO_audio_AudioEffectMode result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioEffectMode_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioEffectMode>(result));
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_AudioEffectMode_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_audio_AudioEffectMode result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioEffectMode_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioEffectMode>(result));
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_AudioRenderer_Void(OH_Int32 resourceId, OH_AUDIO_AudioRenderer result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioRenderer_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioRenderer(result);
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_AudioRenderer_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_AudioRenderer result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioRenderer_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioRenderer(result);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_AudioRendererChangeInfoArray_Void(OH_Int32 resourceId, Array_CustomObject result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioRendererChangeInfoArray_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(result.length);
    for (int i = 0; i < result.length; i++) {
        const OH_CustomObject result_element = result.array[i];
        argsSerializer.writeCustomObject("Readonly<AudioRendererChangeInfo>", result_element);
    }
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_AudioRendererChangeInfoArray_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, Array_CustomObject result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioRendererChangeInfoArray_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(result.length);
    for (int i = 0; i < result.length; i++) {
        const OH_CustomObject result_element = result.array[i];
        argsSerializer.writeCustomObject("Readonly<AudioRendererChangeInfo>", result_element);
    }
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_AudioRendererInfo_Void(OH_Int32 resourceId, OH_AUDIO_AudioRendererInfo result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioRendererInfo_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioRendererInfo(result);
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_AudioRendererInfo_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_AudioRendererInfo result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioRendererInfo_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioRendererInfo(result);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_AudioRendererRate_Void(OH_Int32 resourceId, OH_AUDIO_audio_AudioRendererRate result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioRendererRate_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioRendererRate>(result));
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_AudioRendererRate_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_audio_AudioRendererRate result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioRendererRate_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioRendererRate>(result));
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_AudioRingMode_Void(OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioRingMode_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioRingMode>(result));
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_AudioRingMode_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioRingMode_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioRingMode>(result));
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_AudioScene_Void(OH_Int32 resourceId, OH_AUDIO_audio_AudioScene result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioScene_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioScene>(result));
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_AudioScene_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_audio_AudioScene result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioScene_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioScene>(result));
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_AudioStreamInfo_Void(OH_Int32 resourceId, OH_AUDIO_AudioStreamInfo result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioStreamInfo_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioStreamInfo(result);
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_AudioStreamInfo_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_AudioStreamInfo result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioStreamInfo_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioStreamInfo(result);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_AudioVolumeGroupManager_Void(OH_Int32 resourceId, OH_AUDIO_AudioVolumeGroupManager result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioVolumeGroupManager_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioVolumeGroupManager(result);
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_AudioVolumeGroupManager_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_AudioVolumeGroupManager result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_AudioVolumeGroupManager_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioVolumeGroupManager(result);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_Boolean_Void(OH_Int32 resourceId, OH_Boolean result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_Boolean_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeBoolean(result);
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_Boolean_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_Boolean result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_Boolean_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeBoolean(result);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_Buffer_Void(OH_Int32 resourceId, OH_Buffer result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_Buffer_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeBuffer(result);
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_Buffer_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_Buffer result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_Buffer_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeBuffer(result);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_Number_Void(OH_Int32 resourceId, OH_Number result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_Number_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeNumber(result);
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_Number_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_Number result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_Number_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeNumber(result);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_String_Void(OH_Int32 resourceId, OH_String result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_String_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeString(result);
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_String_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_String result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_String_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeString(result);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_TonePlayer_Void(OH_Int32 resourceId, OH_AUDIO_TonePlayer result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_TonePlayer_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeTonePlayer(result);
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_TonePlayer_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_TonePlayer result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_TonePlayer_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeTonePlayer(result);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_Void(OH_Int32 resourceId)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_Void);
    argsSerializer.writeInt32(resourceId);
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_Void);
    argsSerializer.writeInt32(resourceId);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAsyncCallback_VolumeGroupInfos_Void(OH_Int32 resourceId, Array_CustomObject result)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AsyncCallback_VolumeGroupInfos_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(result.length);
    for (int i = 0; i < result.length; i++) {
        const OH_CustomObject result_element = result.array[i];
        argsSerializer.writeCustomObject("Readonly<VolumeGroupInfo>", result_element);
    }
    enqueueCallback(&__buffer);
}
void callManagedAsyncCallback_VolumeGroupInfos_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, Array_CustomObject result)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AsyncCallback_VolumeGroupInfos_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(result.length);
    for (int i = 0; i < result.length; i++) {
        const OH_CustomObject result_element = result.array[i];
        argsSerializer.writeCustomObject("Readonly<VolumeGroupInfo>", result_element);
    }
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedAudioRendererWriteDataCallback(OH_Int32 resourceId, OH_Buffer data, OH_CustomObject continuation)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_AudioRendererWriteDataCallback);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeBuffer(data);
    argsSerializer.writeCustomObject("Callback_AudioDataCallbackResult_Void", continuation);
    enqueueCallback(&__buffer);
}
void callManagedAudioRendererWriteDataCallbackSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_Buffer data, OH_CustomObject continuation)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_AudioRendererWriteDataCallback);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeBuffer(data);
    argsSerializer.writeCustomObject("Callback_AudioDataCallbackResult_Void", continuation);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_AudioCapturerChangeInfo_Void(OH_Int32 resourceId, OH_AUDIO_AudioCapturerChangeInfo parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_AudioCapturerChangeInfo_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioCapturerChangeInfo(parameter);
    enqueueCallback(&__buffer);
}
void callManagedCallback_AudioCapturerChangeInfo_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_AudioCapturerChangeInfo parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_AudioCapturerChangeInfo_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioCapturerChangeInfo(parameter);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_AudioCapturerChangeInfoArray_Void(OH_Int32 resourceId, Array_CustomObject parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_AudioCapturerChangeInfoArray_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(parameter.length);
    for (int i = 0; i < parameter.length; i++) {
        const OH_CustomObject parameter_element = parameter.array[i];
        argsSerializer.writeCustomObject("Readonly<AudioCapturerChangeInfo>", parameter_element);
    }
    enqueueCallback(&__buffer);
}
void callManagedCallback_AudioCapturerChangeInfoArray_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, Array_CustomObject parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_AudioCapturerChangeInfoArray_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(parameter.length);
    for (int i = 0; i < parameter.length; i++) {
        const OH_CustomObject parameter_element = parameter.array[i];
        argsSerializer.writeCustomObject("Readonly<AudioCapturerChangeInfo>", parameter_element);
    }
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_AudioDeviceDescriptors_Void(OH_Int32 resourceId, Array_CustomObject parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_AudioDeviceDescriptors_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(parameter.length);
    for (int i = 0; i < parameter.length; i++) {
        const OH_CustomObject parameter_element = parameter.array[i];
        argsSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", parameter_element);
    }
    enqueueCallback(&__buffer);
}
void callManagedCallback_AudioDeviceDescriptors_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, Array_CustomObject parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_AudioDeviceDescriptors_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(parameter.length);
    for (int i = 0; i < parameter.length; i++) {
        const OH_CustomObject parameter_element = parameter.array[i];
        argsSerializer.writeCustomObject("Readonly<AudioDeviceDescriptor>", parameter_element);
    }
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_AudioRendererChangeInfoArray_Void(OH_Int32 resourceId, Array_CustomObject parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_AudioRendererChangeInfoArray_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(parameter.length);
    for (int i = 0; i < parameter.length; i++) {
        const OH_CustomObject parameter_element = parameter.array[i];
        argsSerializer.writeCustomObject("Readonly<AudioRendererChangeInfo>", parameter_element);
    }
    enqueueCallback(&__buffer);
}
void callManagedCallback_AudioRendererChangeInfoArray_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, Array_CustomObject parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_AudioRendererChangeInfoArray_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(parameter.length);
    for (int i = 0; i < parameter.length; i++) {
        const OH_CustomObject parameter_element = parameter.array[i];
        argsSerializer.writeCustomObject("Readonly<AudioRendererChangeInfo>", parameter_element);
    }
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_AudioRingMode_Void(OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_AudioRingMode_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioRingMode>(parameter));
    enqueueCallback(&__buffer);
}
void callManagedCallback_AudioRingMode_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_audio_AudioRingMode parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_AudioRingMode_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioRingMode>(parameter));
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_AudioSessionDeactivatedEvent_Void(OH_Int32 resourceId, OH_AUDIO_AudioSessionDeactivatedEvent parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_AudioSessionDeactivatedEvent_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioSessionDeactivatedEvent(parameter);
    enqueueCallback(&__buffer);
}
void callManagedCallback_AudioSessionDeactivatedEvent_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_AudioSessionDeactivatedEvent parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_AudioSessionDeactivatedEvent_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioSessionDeactivatedEvent(parameter);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_AudioSpatialEnabledStateForDevice_Void(OH_Int32 resourceId, OH_AUDIO_AudioSpatialEnabledStateForDevice parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_AudioSpatialEnabledStateForDevice_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioSpatialEnabledStateForDevice(parameter);
    enqueueCallback(&__buffer);
}
void callManagedCallback_AudioSpatialEnabledStateForDevice_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_AudioSpatialEnabledStateForDevice parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_AudioSpatialEnabledStateForDevice_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioSpatialEnabledStateForDevice(parameter);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_AudioState_Void(OH_Int32 resourceId, OH_AUDIO_audio_AudioState parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_AudioState_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioState>(parameter));
    enqueueCallback(&__buffer);
}
void callManagedCallback_AudioState_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_audio_AudioState parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_AudioState_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInt32(static_cast<OH_AUDIO_audio_AudioState>(parameter));
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_AudioStreamDeviceChangeInfo_Void(OH_Int32 resourceId, OH_AUDIO_AudioStreamDeviceChangeInfo parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_AudioStreamDeviceChangeInfo_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioStreamDeviceChangeInfo(parameter);
    enqueueCallback(&__buffer);
}
void callManagedCallback_AudioStreamDeviceChangeInfo_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_AudioStreamDeviceChangeInfo parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_AudioStreamDeviceChangeInfo_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeAudioStreamDeviceChangeInfo(parameter);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_Boolean_Void(OH_Int32 resourceId, OH_Boolean parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_Boolean_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeBoolean(parameter);
    enqueueCallback(&__buffer);
}
void callManagedCallback_Boolean_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_Boolean parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_Boolean_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeBoolean(parameter);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_Buffer_Void(OH_Int32 resourceId, OH_Buffer parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_Buffer_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeBuffer(parameter);
    enqueueCallback(&__buffer);
}
void callManagedCallback_Buffer_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_Buffer parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_Buffer_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeBuffer(parameter);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_DeviceBlockStatusInfo_Void(OH_Int32 resourceId, OH_AUDIO_DeviceBlockStatusInfo parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_DeviceBlockStatusInfo_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeDeviceBlockStatusInfo(parameter);
    enqueueCallback(&__buffer);
}
void callManagedCallback_DeviceBlockStatusInfo_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_DeviceBlockStatusInfo parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_DeviceBlockStatusInfo_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeDeviceBlockStatusInfo(parameter);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_DeviceChangeAction_Void(OH_Int32 resourceId, OH_AUDIO_DeviceChangeAction parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_DeviceChangeAction_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeDeviceChangeAction(parameter);
    enqueueCallback(&__buffer);
}
void callManagedCallback_DeviceChangeAction_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_DeviceChangeAction parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_DeviceChangeAction_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeDeviceChangeAction(parameter);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_InterruptAction_Void(OH_Int32 resourceId, OH_AUDIO_InterruptAction parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_InterruptAction_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInterruptAction(parameter);
    enqueueCallback(&__buffer);
}
void callManagedCallback_InterruptAction_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_InterruptAction parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_InterruptAction_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInterruptAction(parameter);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_InterruptEvent_Void(OH_Int32 resourceId, OH_AUDIO_InterruptEvent parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_InterruptEvent_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInterruptEvent(parameter);
    enqueueCallback(&__buffer);
}
void callManagedCallback_InterruptEvent_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_InterruptEvent parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_InterruptEvent_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeInterruptEvent(parameter);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_MicStateChangeEvent_Void(OH_Int32 resourceId, OH_AUDIO_MicStateChangeEvent parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_MicStateChangeEvent_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeMicStateChangeEvent(parameter);
    enqueueCallback(&__buffer);
}
void callManagedCallback_MicStateChangeEvent_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_MicStateChangeEvent parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_MicStateChangeEvent_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeMicStateChangeEvent(parameter);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_Number_Void(OH_Int32 resourceId, OH_Number parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_Number_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeNumber(parameter);
    enqueueCallback(&__buffer);
}
void callManagedCallback_Number_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_Number parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_Number_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeNumber(parameter);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
void callManagedCallback_VolumeEvent_Void(OH_Int32 resourceId, OH_AUDIO_VolumeEvent parameter)
{
    CallbackBuffer __buffer = {{}, {}};
    const OH_AUDIO_CallbackResource __callbackResource = {resourceId, holdManagedCallbackResource, releaseManagedCallbackResource};
    __buffer.resourceHolder.holdCallbackResource(&__callbackResource);
    Serializer argsSerializer = Serializer(__buffer.buffer, sizeof(__buffer.buffer), &(__buffer.resourceHolder));
    argsSerializer.writeInt32(Kind_Callback_VolumeEvent_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeVolumeEvent(parameter);
    enqueueCallback(&__buffer);
}
void callManagedCallback_VolumeEvent_VoidSync(OH_AUDIO_VMContext vmContext, OH_Int32 resourceId, OH_AUDIO_VolumeEvent parameter)
{
    uint8_t __buffer[60 * 4];
    Serializer argsSerializer = Serializer(__buffer, sizeof(__buffer), nullptr);
    argsSerializer.writeInt32(Kind_Callback_VolumeEvent_Void);
    argsSerializer.writeInt32(resourceId);
    argsSerializer.writeVolumeEvent(parameter);
    KOALA_INTEROP_CALL_VOID(vmContext, 1, sizeof(__buffer), __buffer);
}
OH_NativePointer getManagedCallbackCaller(CallbackKind kind)
{
    switch (kind) {
        case Kind_AsyncCallback_AudioCapturer_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioCapturer_Void);
        case Kind_AsyncCallback_AudioCapturerChangeInfoArray_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioCapturerChangeInfoArray_Void);
        case Kind_AsyncCallback_AudioCapturerInfo_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioCapturerInfo_Void);
        case Kind_AsyncCallback_AudioDeviceDescriptors_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioDeviceDescriptors_Void);
        case Kind_AsyncCallback_AudioEffectInfoArray_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioEffectInfoArray_Void);
        case Kind_AsyncCallback_AudioEffectMode_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioEffectMode_Void);
        case Kind_AsyncCallback_AudioRenderer_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioRenderer_Void);
        case Kind_AsyncCallback_AudioRendererChangeInfoArray_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioRendererChangeInfoArray_Void);
        case Kind_AsyncCallback_AudioRendererInfo_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioRendererInfo_Void);
        case Kind_AsyncCallback_AudioRendererRate_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioRendererRate_Void);
        case Kind_AsyncCallback_AudioRingMode_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioRingMode_Void);
        case Kind_AsyncCallback_AudioScene_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioScene_Void);
        case Kind_AsyncCallback_AudioStreamInfo_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioStreamInfo_Void);
        case Kind_AsyncCallback_AudioVolumeGroupManager_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioVolumeGroupManager_Void);
        case Kind_AsyncCallback_Boolean_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_Boolean_Void);
        case Kind_AsyncCallback_Buffer_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_Buffer_Void);
        case Kind_AsyncCallback_Number_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_Number_Void);
        case Kind_AsyncCallback_String_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_String_Void);
        case Kind_AsyncCallback_TonePlayer_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_TonePlayer_Void);
        case Kind_AsyncCallback_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_Void);
        case Kind_AsyncCallback_VolumeGroupInfos_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_VolumeGroupInfos_Void);
        case Kind_AudioRendererWriteDataCallback: return reinterpret_cast<OH_NativePointer>(callManagedAudioRendererWriteDataCallback);
        case Kind_Callback_AudioCapturerChangeInfo_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioCapturerChangeInfo_Void);
        case Kind_Callback_AudioCapturerChangeInfoArray_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioCapturerChangeInfoArray_Void);
        case Kind_Callback_AudioDeviceDescriptors_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioDeviceDescriptors_Void);
        case Kind_Callback_AudioRendererChangeInfoArray_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioRendererChangeInfoArray_Void);
        case Kind_Callback_AudioRingMode_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioRingMode_Void);
        case Kind_Callback_AudioSessionDeactivatedEvent_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioSessionDeactivatedEvent_Void);
        case Kind_Callback_AudioSpatialEnabledStateForDevice_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioSpatialEnabledStateForDevice_Void);
        case Kind_Callback_AudioState_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioState_Void);
        case Kind_Callback_AudioStreamDeviceChangeInfo_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioStreamDeviceChangeInfo_Void);
        case Kind_Callback_Boolean_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_Boolean_Void);
        case Kind_Callback_Buffer_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_Buffer_Void);
        case Kind_Callback_DeviceBlockStatusInfo_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_DeviceBlockStatusInfo_Void);
        case Kind_Callback_DeviceChangeAction_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_DeviceChangeAction_Void);
        case Kind_Callback_InterruptAction_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_InterruptAction_Void);
        case Kind_Callback_InterruptEvent_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_InterruptEvent_Void);
        case Kind_Callback_MicStateChangeEvent_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_MicStateChangeEvent_Void);
        case Kind_Callback_Number_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_Number_Void);
        case Kind_Callback_VolumeEvent_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_VolumeEvent_Void);
    }
    return nullptr;
}
OH_NativePointer getManagedCallbackCallerSync(CallbackKind kind)
{
    switch (kind) {
        case Kind_AsyncCallback_AudioCapturer_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioCapturer_VoidSync);
        case Kind_AsyncCallback_AudioCapturerChangeInfoArray_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioCapturerChangeInfoArray_VoidSync);
        case Kind_AsyncCallback_AudioCapturerInfo_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioCapturerInfo_VoidSync);
        case Kind_AsyncCallback_AudioDeviceDescriptors_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioDeviceDescriptors_VoidSync);
        case Kind_AsyncCallback_AudioEffectInfoArray_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioEffectInfoArray_VoidSync);
        case Kind_AsyncCallback_AudioEffectMode_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioEffectMode_VoidSync);
        case Kind_AsyncCallback_AudioRenderer_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioRenderer_VoidSync);
        case Kind_AsyncCallback_AudioRendererChangeInfoArray_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioRendererChangeInfoArray_VoidSync);
        case Kind_AsyncCallback_AudioRendererInfo_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioRendererInfo_VoidSync);
        case Kind_AsyncCallback_AudioRendererRate_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioRendererRate_VoidSync);
        case Kind_AsyncCallback_AudioRingMode_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioRingMode_VoidSync);
        case Kind_AsyncCallback_AudioScene_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioScene_VoidSync);
        case Kind_AsyncCallback_AudioStreamInfo_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioStreamInfo_VoidSync);
        case Kind_AsyncCallback_AudioVolumeGroupManager_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_AudioVolumeGroupManager_VoidSync);
        case Kind_AsyncCallback_Boolean_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_Boolean_VoidSync);
        case Kind_AsyncCallback_Buffer_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_Buffer_VoidSync);
        case Kind_AsyncCallback_Number_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_Number_VoidSync);
        case Kind_AsyncCallback_String_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_String_VoidSync);
        case Kind_AsyncCallback_TonePlayer_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_TonePlayer_VoidSync);
        case Kind_AsyncCallback_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_VoidSync);
        case Kind_AsyncCallback_VolumeGroupInfos_Void: return reinterpret_cast<OH_NativePointer>(callManagedAsyncCallback_VolumeGroupInfos_VoidSync);
        case Kind_AudioRendererWriteDataCallback: return reinterpret_cast<OH_NativePointer>(callManagedAudioRendererWriteDataCallbackSync);
        case Kind_Callback_AudioCapturerChangeInfo_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioCapturerChangeInfo_VoidSync);
        case Kind_Callback_AudioCapturerChangeInfoArray_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioCapturerChangeInfoArray_VoidSync);
        case Kind_Callback_AudioDeviceDescriptors_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioDeviceDescriptors_VoidSync);
        case Kind_Callback_AudioRendererChangeInfoArray_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioRendererChangeInfoArray_VoidSync);
        case Kind_Callback_AudioRingMode_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioRingMode_VoidSync);
        case Kind_Callback_AudioSessionDeactivatedEvent_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioSessionDeactivatedEvent_VoidSync);
        case Kind_Callback_AudioSpatialEnabledStateForDevice_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioSpatialEnabledStateForDevice_VoidSync);
        case Kind_Callback_AudioState_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioState_VoidSync);
        case Kind_Callback_AudioStreamDeviceChangeInfo_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_AudioStreamDeviceChangeInfo_VoidSync);
        case Kind_Callback_Boolean_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_Boolean_VoidSync);
        case Kind_Callback_Buffer_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_Buffer_VoidSync);
        case Kind_Callback_DeviceBlockStatusInfo_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_DeviceBlockStatusInfo_VoidSync);
        case Kind_Callback_DeviceChangeAction_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_DeviceChangeAction_VoidSync);
        case Kind_Callback_InterruptAction_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_InterruptAction_VoidSync);
        case Kind_Callback_InterruptEvent_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_InterruptEvent_VoidSync);
        case Kind_Callback_MicStateChangeEvent_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_MicStateChangeEvent_VoidSync);
        case Kind_Callback_Number_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_Number_VoidSync);
        case Kind_Callback_VolumeEvent_Void: return reinterpret_cast<OH_NativePointer>(callManagedCallback_VolumeEvent_VoidSync);
    }
    return nullptr;
}
const OH_AnyAPI* impls[16] = { 0 };


const OH_AnyAPI* GetAnyAPIImpl(int kind, int version) {
    switch (kind) {
        case OH_AUDIO_API_KIND:
            return reinterpret_cast<const OH_AnyAPI*>(GetAUDIOAPIImpl(version));
        default:
            return nullptr;
    }
}

extern "C" const OH_AnyAPI* GetAnyAPI(int kind, int version) {
    if (kind < 0 || kind > 15) return nullptr;
    if (!impls[kind]) {
        impls[kind] = GetAnyAPIImpl(kind, version);
    }
    return impls[kind];
}
