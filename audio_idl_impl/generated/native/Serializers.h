
#include "SerializerBase.h"
#include "DeserializerBase.h"
#include "callbacks.h"
#include "ohos_api_generated.h"
#include <string>


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


// Serializers

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

// Deserializers


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

