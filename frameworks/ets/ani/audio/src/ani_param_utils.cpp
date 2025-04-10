/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#ifndef LOG_TAG
#define LOG_TAG "AniParamUtils"
#endif

#include "ani_param_utils.h"

#include "audio_ani_log.h"
#include "audio_enum_ani.h"
#include "ani_class_name.h"

namespace OHOS {
namespace AudioStandard {

ani_boolean AniParamUtils::isUndefined(ani_env *env, ani_object object)
{
    ani_boolean isUndefined = ANI_TRUE;
    CHECK_COND_RET(ANI_OK == env->Reference_IsUndefined(object, &isUndefined), ANI_TRUE,
        "Call Reference_IsUndefined failed.");
    return isUndefined;
}
ani_status AniParamUtils::GetInt32(ani_env *env, ani_int arg, int32_t &value)
{
    value = static_cast<int32_t>(arg);
    return ANI_OK;
}

ani_status AniParamUtils::GetInt32(ani_env *env, ani_object arg, int32_t &value)
{
    CHECK_COND_RET(isUndefined(env, arg) != ANI_TRUE, ANI_ERROR, "invalid property.");

    ani_class cls {};
    static const std::string className = "Lstd/core/Int;";
    CHECK_STATUS_RET(env->FindClass(className.c_str(), &cls), "Can't find Lstd/core/Int.");

    ani_method method {};
    CHECK_STATUS_RET(env->Class_FindMethod(cls, "intValue", nullptr, &method),
        "Can't find method intValue in Lstd/core/Int.");

    ani_int result;
    CHECK_STATUS_RET(env->Object_CallMethod_Int(arg, method, &result), "Call method intValue failed.");

    return GetInt32(env, result, value);
}

ani_status AniParamUtils::SetValueEnum(ani_env *env, ani_class cls, ani_object handle,
    const std::string &key, ani_enum_item value)
{
    ani_method setter;
    std::string setterName = "<set>" + key;
    CHECK_STATUS_RET(env->Class_FindMethod(cls, setterName.c_str(), nullptr, &setter),
        "no %{public}s", setterName.c_str());
    CHECK_STATUS_RET(env->Object_CallMethod_Void(handle, setter, value), "%{public}s fail", setterName.c_str());
    return ANI_OK;
}

ani_status AniParamUtils::SetValueInt(ani_env *env, ani_class cls, ani_object handle,
    const std::string &key, int32_t value)
{
    ani_method setter;
    std::string setterName = "<set>" + key;
    CHECK_STATUS_RET(env->Class_FindMethod(cls, setterName.c_str(), nullptr, &setter),
        "no %{public}s", setterName.c_str());
    ani_int aniValue;
    CHECK_STATUS_RET(AniParamUtils::ToAniInt(env, value, aniValue), "int32_t to ani_int fail");
    CHECK_STATUS_RET(env->Object_CallMethod_Void(handle, setter, aniValue), "%{public}s fail", setterName.c_str());
    return ANI_OK;
}

ani_status AniParamUtils::SetValueNumber(ani_env *env, ani_class cls, ani_object handle,
    const std::string &key, int32_t value)
{
    ani_method setter;
    std::string setterName = "<set>" + key;
    CHECK_STATUS_RET(env->Class_FindMethod(cls, setterName.c_str(), nullptr, &setter),
        "no %{public}s", setterName.c_str());
    ani_double aniValue;
    CHECK_STATUS_RET(AniParamUtils::ToAniDouble(env, value, aniValue), "int32_t to number fail");
    CHECK_STATUS_RET(env->Object_CallMethod_Void(handle, setter, aniValue), "%{public}s fail", setterName.c_str());
    return ANI_OK;
}

ani_status AniParamUtils::SetValueBool(ani_env *env, ani_class cls, ani_object handle,
    const std::string &key, bool value)
{
    ani_method setter;
    std::string setterName = "<set>" + key;
    CHECK_STATUS_RET(env->Class_FindMethod(cls, setterName.c_str(), nullptr, &setter),
        "no %{public}s", setterName.c_str());
    ani_boolean aniValue = value;
    CHECK_STATUS_RET(env->Object_CallMethod_Void(handle, setter, aniValue), "%{public}s fail", setterName.c_str());
    return ANI_OK;
}

ani_status AniParamUtils::SetValueString(ani_env *env, ani_class cls, ani_object handle,
    const std::string &key, const std::string &value)
{
    ani_method setter;
    std::string setterName = "<set>" + key;
    CHECK_STATUS_RET(env->Class_FindMethod(cls, setterName.c_str(), nullptr, &setter),
        "no %{public}s", setterName.c_str());
    ani_string aniValue;
    CHECK_STATUS_RET(AniParamUtils::ToAniString(env, value, aniValue), "string to ani_string fail");
    CHECK_STATUS_RET(env->Object_CallMethod_Void(handle, setter, aniValue), "%{public}s fail", setterName.c_str());
    return ANI_OK;
}

ani_status AniParamUtils::SetValueObject(ani_env *env, ani_class cls, ani_object handle,
    const std::string &key, ani_object value)
{
    CHECK_COND_RET(value != nullptr, ANI_ERROR, "invalid property.");

    ani_method setter;
    std::string setterName = "<set>" + key;
    CHECK_STATUS_RET(env->Class_FindMethod(cls, setterName.c_str(), nullptr, &setter),
        "no %{public}s", setterName.c_str());
    CHECK_STATUS_RET(env->Object_CallMethod_Void(handle, setter, value), "%{public}s fail", setterName.c_str());
    return ANI_OK;
}

ani_status AniParamUtils::CreateVolumeEventHandle(ani_env *env, ani_class &cls, ani_object &result)
{
    CHECK_COND_RET(env != nullptr, ANI_ERROR, "env is nullptr");
    static const char *className = ANI_CLASS_VOLUME_EVENT_HANDLE.c_str();
    CHECK_STATUS_RET(env->FindClass(className, &cls), "Can't find class %{public}s", className);
    ani_method ctor;
    CHECK_STATUS_RET(env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor), "Can't find ctor %{public}s", className);
    CHECK_STATUS_RET(env->Object_New(cls, ctor, &result), "Create Object Failed %{public}s", className);
    return ANI_OK;
}

ani_status AniParamUtils::SetValueVolumeEvent(ani_env *env, const VolumeEvent &volumeEvent, ani_object &handle)
{
    CHECK_COND_RET(env != nullptr, ANI_ERROR, "env is nullptr");
    ani_class cls {};
    CHECK_STATUS_RET(CreateVolumeEventHandle(env, cls, handle), "CreateVolumeEventHandle fail");

    ani_enum_item aniVolumeType;
    int32_t volumeType = AudioEnumAni::GetJsAudioVolumeType(static_cast<AudioStreamType>(volumeEvent.volumeType));
    AudioEnumAni::AudioJsVolumeType aniJsVolumeType = static_cast<AudioEnumAni::AudioJsVolumeType>(volumeType);
    AudioEnumAni::ToAniEnum(env, aniJsVolumeType, aniVolumeType);
    SetValueEnum(env, cls, handle, "volumeType", aniVolumeType);
    SetValueNumber(env, cls, handle, "volume", volumeEvent.volume);
    SetValueBool(env, cls, handle, "updateUi", volumeEvent.updateUi);
    SetValueNumber(env, cls, handle, "volumeGroupId", volumeEvent.volumeGroupId);
    SetValueString(env, cls, handle, "networkId", volumeEvent.networkId);
    return ANI_OK;
}

ani_status AniParamUtils::CreateAudioRendererInfoHandle(ani_env *env, ani_class &cls, ani_object &result)
{
    static const char *className = ANI_CLASS_AUDIO_RENDERER_INFO_HANDLE.c_str();
    CHECK_STATUS_RET(env->FindClass(className, &cls), "Can't find class %{public}s", className);
    ani_method ctor;
    CHECK_STATUS_RET(env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor), "Can't find ctor %{public}s", className);
    CHECK_STATUS_RET(env->Object_New(cls, ctor, &result), "Create Object Failed %{public}s", className);
    return ANI_OK;
}

ani_status AniParamUtils::SetRendererInfo(ani_env *env,
    const AudioRendererInfo &audioRendererInfo, ani_object &handle)
{
    ani_class cls {};
    CHECK_STATUS_RET(CreateAudioRendererInfoHandle(env, cls, handle), "CreateAudioRendererInfoHandle fail");
    
    ani_enum_item aniContent;
    CHECK_STATUS_RET(AudioEnumAni::ToAniEnum(env, audioRendererInfo.contentType, aniContent), "Get volumeType fail");
    SetValueEnum(env, cls, handle, "content", aniContent);
    
    ani_enum_item aniUsage;
    CHECK_STATUS_RET(AudioEnumAni::ToAniEnum(env, audioRendererInfo.streamUsage, aniUsage), "Get usage fail");
    SetValueEnum(env, cls, handle, "usage", aniUsage);
    SetValueNumber(env, cls, handle, "rendererFlags", audioRendererInfo.rendererFlags);
    return ANI_OK;
}

ani_status AniParamUtils::CreateInterruptEventHandle(ani_env *env, ani_class &cls, ani_object &result)
{
    static const char *className = ANI_CLASS_INTERRUPT_EVENT_HANDLE.c_str();
    CHECK_STATUS_RET(env->FindClass(className, &cls), "Can't find class %{public}s", className);
    ani_method ctor;
    CHECK_STATUS_RET(env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor), "Can't find ctor %{public}s", className);
    CHECK_STATUS_RET(env->Object_New(cls, ctor, &result), "Create Object Failed %{public}s", className);
    return ANI_OK;
}

ani_status AniParamUtils::SetInterruptEvent(ani_env *env,
    const InterruptEvent &interruptEvent, ani_object &handle)
{
    ani_class cls {};
    CHECK_STATUS_RET(CreateInterruptEventHandle(env, cls, handle), "CreateInterruptEventHandle fail");

    ani_enum_item aniEventType;
    CHECK_STATUS_RET(AudioEnumAni::ToAniEnum(env, interruptEvent.eventType, aniEventType), "Get volumeType fail");
    SetValueEnum(env, cls, handle, "eventType", aniEventType);

    ani_enum_item aniForceType;
    CHECK_STATUS_RET(AudioEnumAni::ToAniEnum(env, interruptEvent.forceType, aniForceType), "Get forceType fail");
    SetValueEnum(env, cls, handle, "forceType", aniForceType);

    ani_enum_item aniHintType;
    CHECK_STATUS_RET(AudioEnumAni::ToAniEnum(env, interruptEvent.hintType, aniHintType), "Get hintType fail");
    SetValueEnum(env, cls, handle, "hintType", aniHintType);
    return ANI_OK;
}

void ConvertDeviceInfoToAudioDeviceDescriptor(std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor,
    const AudioDeviceDescriptor &deviceInfo)
{
    CHECK_IF_EQUAL(audioDeviceDescriptor != nullptr, "audioDeviceDescriptor is nullptr");
    audioDeviceDescriptor->deviceRole_ = deviceInfo.deviceRole_;
    audioDeviceDescriptor->deviceType_ = deviceInfo.deviceType_;
    audioDeviceDescriptor->deviceId_ = deviceInfo.deviceId_;
    audioDeviceDescriptor->channelMasks_ = deviceInfo.channelMasks_;
    audioDeviceDescriptor->channelIndexMasks_ = deviceInfo.channelIndexMasks_;
    audioDeviceDescriptor->deviceName_ = deviceInfo.deviceName_;
    audioDeviceDescriptor->macAddress_ = deviceInfo.macAddress_;
    audioDeviceDescriptor->interruptGroupId_ = deviceInfo.interruptGroupId_;
    audioDeviceDescriptor->volumeGroupId_ = deviceInfo.volumeGroupId_;
    audioDeviceDescriptor->networkId_ = deviceInfo.networkId_;
    audioDeviceDescriptor->displayName_ = deviceInfo.displayName_;
    audioDeviceDescriptor->audioStreamInfo_.samplingRate = deviceInfo.audioStreamInfo_.samplingRate;
    audioDeviceDescriptor->audioStreamInfo_.encoding = deviceInfo.audioStreamInfo_.encoding;
    audioDeviceDescriptor->audioStreamInfo_.format = deviceInfo.audioStreamInfo_.format;
    audioDeviceDescriptor->audioStreamInfo_.channels = deviceInfo.audioStreamInfo_.channels;
}

ani_status AniParamUtils::CreateAudioDeviceDescriptorHandle(ani_env *env, ani_class &cls, ani_object &result)
{
    static const char *className = ANI_CLASS_AUDIO_DEVICE_DESCRIPTOR_HANDLE.c_str();
    CHECK_STATUS_RET(env->FindClass(className, &cls), "Can't find class %{public}s", className);
    ani_method ctor;
    CHECK_STATUS_RET(env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor), "Can't find ctor %{public}s", className);
    CHECK_STATUS_RET(env->Object_New(cls, ctor, &result), "Create Object Failed %{public}s", className);
    return ANI_OK;
}

ani_status AniParamUtils::MakeAniArrayRetSetMethod(ani_env *env, size_t size,
    ani_object &aniArray, ani_method &setMethod)
{
    ani_class clas {};
    static const std::string className = "Lescompat/Array;";
    CHECK_STATUS_RET(env->FindClass(className.c_str(), &clas), "Can't find Lescompat/Array.");

    ani_method method {};
    CHECK_STATUS_RET(env->Class_FindMethod(clas, "<ctor>", "I:V", &method),
        "Can't find method <ctor> in Lescompat/Array.");
    CHECK_STATUS_RET(env->Object_New(clas, method, &aniArray, size), "Call method <ctor> failed.");
    CHECK_STATUS_RET(env->Class_FindMethod(clas, "$_set", "ILstd/core/Object;:V", &setMethod),
        "Can't find method set in Lescompat/Array.");
    return ANI_OK;
}

template<class Container>
ani_status AniParamUtils::SetValueElement(ani_env *env, ani_class cls, ani_object &result,
    const std::string &key, const Container &values)
{
    ani_object aniProperty;
    ani_method setMethod;
    CHECK_STATUS_RET(MakeAniArrayRetSetMethod(env, values.size(), aniProperty, setMethod),
        "MakeAniArrayRetSetMethod failed");
    size_t count = 0;
    for (const auto &value : values) {
        ani_object aniValue;
        CHECK_STATUS_RET(ToAniDoubleObject(env, value, aniValue), "SetValueElement ToAniDouble failed");
        CHECK_STATUS_RET(env->Object_CallMethod_Void(aniProperty, setMethod, (ani_int)count, aniValue),
            "SetValueElement array set failed");
        count++;
    }
    CHECK_STATUS_RET(SetValueObject(env, cls, result, key, aniProperty), "SetValueObject failed");
    return ANI_OK;
}

ani_status AniParamUtils::SetSetDeviceDescriptorMore(ani_env *env, ani_class cls,
    const AudioDeviceDescriptor &deviceInfo, ani_object &result)
{
    std::vector<int32_t> channelMasks_;
    channelMasks_.push_back(deviceInfo.channelMasks_);
    SetValueElement(env, cls, result, "channelMasks", channelMasks_);
    std::vector<int32_t> channelIndexMasks_;
    channelIndexMasks_.push_back(deviceInfo.channelIndexMasks_);
    SetValueElement(env, cls, result, "channelIndexMasks", channelIndexMasks_);
    std::vector<AudioEncodingType> encoding;
    encoding.push_back(deviceInfo.audioStreamInfo_.encoding);
    ani_object aniProperty;
    ani_method setMethod;
    CHECK_STATUS_RET(MakeAniArrayRetSetMethod(env, encoding.size(), aniProperty, setMethod),
        "MakeAniArrayRetSetMethod failed");
    size_t count = 0;
    for (const auto &value : encoding) {
        ani_enum_item aniValue;
        CHECK_STATUS_RET(AudioEnumAni::ToAniEnum(env, value, aniValue), "SetValueInt32Element ToAniDouble failed");
        env->Object_CallMethod_Void(aniProperty, setMethod, (ani_int)count, aniValue);
        count++;
    }
    SetValueObject(env, cls, result, "encodingTypes", aniProperty);
    return ANI_OK;
}

ani_status AniParamUtils::SetDeviceDescriptor(ani_env *env, const AudioDeviceDescriptor &deviceInfo,
    ani_object &result)
{
    ani_class cls {};
    CHECK_STATUS_RET(CreateAudioDeviceDescriptorHandle(env, cls, result), "CreateInterruptEventHandle fail");
    ani_enum_item aniDeviceRole;
    CHECK_STATUS_RET(AudioEnumAni::ToAniEnum(env, deviceInfo.deviceRole_, aniDeviceRole), "Get deviceRole fail");
    SetValueEnum(env, cls, result, "deviceRole", aniDeviceRole);
    ani_enum_item aniDeviceType;
    CHECK_STATUS_RET(AudioEnumAni::ToAniEnum(env, deviceInfo.deviceType_, aniDeviceType), "Get deviceType fail");
    SetValueEnum(env, cls, result, "deviceType", aniDeviceType);
    SetValueNumber(env, cls, result, "id", deviceInfo.deviceId_);
    SetValueString(env, cls, result, "name", deviceInfo.deviceName_);
    SetValueString(env, cls, result, "address", deviceInfo.macAddress_);
    SetValueString(env, cls, result, "networkId", deviceInfo.networkId_);
    SetValueString(env, cls, result, "displayName", deviceInfo.displayName_);
    SetValueNumber(env, cls, result, "interruptGroupId", deviceInfo.interruptGroupId_);
    SetValueNumber(env, cls, result, "volumeGroupId", deviceInfo.volumeGroupId_);
    SetValueElement(env, cls, result, "sampleRates", deviceInfo.audioStreamInfo_.samplingRate);
    SetValueElement(env, cls, result, "channelCounts", deviceInfo.audioStreamInfo_.channels);
    SetSetDeviceDescriptorMore(env, cls, deviceInfo, result);
    return ANI_OK;
}

ani_status AniParamUtils::SetDeviceDescriptors(ani_env *env, ani_object &aniArray,
    const std::vector<std::shared_ptr<AudioDeviceDescriptor>> &deviceDescriptors)
{
    ani_method method {};
    MakeAniArrayRetSetMethod(env, deviceDescriptors.size(), aniArray, method);
    for (size_t i = 0; i < deviceDescriptors.size(); i++) {
        if (deviceDescriptors[i] != nullptr) {
            ani_object valueParam = nullptr;
            SetDeviceDescriptor(env, deviceDescriptors[i], valueParam);
            CHECK_STATUS_RET(env->Object_CallMethod_Void(aniArray, method, (ani_int)i, valueParam),
                "Call method set failed.");
        }
    }
    return ANI_OK;
}

ani_status AniParamUtils::SetValueDeviceInfo(ani_env *env, const AudioDeviceDescriptor &deviceInfo, ani_object &handle)
{
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> deviceDescriptors;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor = std::make_shared<AudioDeviceDescriptor>();
    CHECK_COND_RET(audioDeviceDescriptor != nullptr, ANI_ERROR,
        "audioDeviceDescriptor malloc failed");
    ConvertDeviceInfoToAudioDeviceDescriptor(audioDeviceDescriptor, deviceInfo);
    deviceDescriptors.push_back(std::move(audioDeviceDescriptor));
    SetDeviceDescriptors(env, handle, deviceDescriptors);
    return ANI_OK;
}

ani_status AniParamUtils::CreateAudioStreamDeviceChangeHandle(ani_env *env, ani_class &cls, ani_object &result)
{
    static const char *className = ANI_CLASS_AUDIO_STREAM_DEVICE_CHANGE_HANDLE.c_str();
    CHECK_STATUS_RET(env->FindClass(className, &cls), "Can't find class %{public}s", className);
    ani_method ctor;
    CHECK_STATUS_RET(env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor), "Can't find ctor %{public}s", className);
    CHECK_STATUS_RET(env->Object_New(cls, ctor, &result), "Create Object Failed %{public}s", className);
    return ANI_OK;
}

ani_status AniParamUtils::CreateDeviceBlockStatusInfoHandle(ani_env *env, ani_class &cls, ani_object &result)
{
    static const char *className = ANI_CLASS_DEVICE_BLOCK_STATUS_INFO_HANDLE.c_str();
    CHECK_STATUS_RET(env->FindClass(className, &cls), "Can't find class %{public}s", className);
    ani_method ctor;
    CHECK_STATUS_RET(env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor), "Can't find ctor %{public}s", className);
    CHECK_STATUS_RET(env->Object_New(cls, ctor, &result), "Create Object Failed %{public}s", className);
    return ANI_OK;
}

ani_status AniParamUtils::SetValueBlockedDeviceAction(ani_env *env, const MicrophoneBlockedInfo &action,
    ani_object &handle)
{
    ani_class cls {};
    CHECK_STATUS_RET(CreateDeviceBlockStatusInfoHandle(env, cls, handle), "CreateDeviceBlockStatusInfoHandle fail");

    ani_enum_item aniBlockStatus;
    CHECK_STATUS_RET(AudioEnumAni::ToAniEnum(env, action.blockStatus, aniBlockStatus), "Get blockStatus fail");
    SetValueEnum(env, cls, handle, "blockStatus", aniBlockStatus);
    ani_object aniDevice = nullptr;
    SetDeviceDescriptors(env, aniDevice, action.devices);
    SetValueObject(env, cls, handle, "devices", aniDevice);
    return ANI_OK;
}

std::string AniParamUtils::GetStringArgument(ani_env *env, ani_string aniStr)
{
    ani_size srcSize = 0;
    std::string strValue = "";
    ani_status status = env->String_GetUTF8Size(aniStr, &srcSize);
    if (status == ANI_OK && srcSize > 0 && srcSize < PATH_MAX) {
        std::vector<char> buffer(srcSize + 1);
        ani_size dstSize = 0;
        status = env->String_GetUTF8SubString(aniStr, 0, srcSize, buffer.data(), buffer.size(), &dstSize);
        if (status == ANI_OK) {
            strValue.assign(buffer.data(), dstSize);
            ANI_DEBUG_LOG("argument = %{public}s", strValue.c_str());
        }
    }
    return strValue;
}

ani_status AniParamUtils::GetString(ani_env *env, ani_string arg, std::string &str)
{
    CHECK_COND_RET(arg != nullptr, ANI_INVALID_ARGS, "GetString invalid arg");

    ani_size srcSize = 0;
    CHECK_STATUS_RET(env->String_GetUTF8Size(arg, &srcSize), "String_GetUTF8Size failed");

    std::vector<char> buffer(srcSize + 1);
    ani_size dstSize = 0;
    CHECK_STATUS_RET(env->String_GetUTF8SubString(arg, 0, srcSize, buffer.data(), buffer.size(), &dstSize),
        "String_GetUTF8SubString failed");

    str.assign(buffer.data(), dstSize);
    return ANI_OK;
}

ani_status AniParamUtils::GetString(ani_env *env, ani_object arg, std::string &str)
{
    CHECK_COND_RET(isUndefined(env, arg) != ANI_TRUE, ANI_ERROR, "invalid property.");

    return GetString(env, static_cast<ani_string>(arg), str);
}


ani_status AniParamUtils::ToAniString(ani_env *env, const std::string &str, ani_string &aniStr)
{
    CHECK_STATUS_RET(env->String_NewUTF8(str.c_str(), str.size(), &aniStr), "String_NewUTF8 failed");
    return ANI_OK;
}

ani_status AniParamUtils::ToAniInt(ani_env *env, const std::int32_t &int32, ani_int &aniInt)
{
    aniInt = static_cast<ani_int>(int32);
    return ANI_OK;
}

ani_status AniParamUtils::ToAniLong(ani_env *env, const std::int64_t &int64, ani_long &aniLong)
{
    aniLong = static_cast<ani_long>(int64);
    return ANI_OK;
}

ani_status AniParamUtils::ToAniDouble(ani_env *env, const double &arg, ani_double &aniDouble)
{
    aniDouble = static_cast<ani_double>(arg);
    return ANI_OK;
}

ani_status AniParamUtils::ToAniDoubleObject(ani_env *env, double src, ani_object &aniObj)
{
    static const char *className = "Lstd/core/Double;";
    ani_class cls {};
    CHECK_STATUS_RET(env->FindClass(className, &cls), "Failed to find class: %{public}s", className);

    ani_method ctor {};
    CHECK_STATUS_RET(env->Class_FindMethod(cls, "<ctor>", "D:V", &ctor), "Failed to find method: ctor");

    ani_double aniBool = src;
    CHECK_STATUS_RET(env->Object_New(cls, ctor, &aniObj, aniBool), "New double Object Fail");
    return ANI_OK;
}

ani_status AniParamUtils::ToAniBooleanObject(ani_env *env, bool src, ani_object &aniObj)
{
    static const char *className = "Lstd/core/Boolean;";
    ani_class cls {};
    CHECK_STATUS_RET(env->FindClass(className, &cls), "Failed to find class: %{public}s", className);

    ani_method ctor {};
    CHECK_STATUS_RET(env->Class_FindMethod(cls, "<ctor>", "Z:V", &ctor), "Failed to find method: ctor");

    ani_boolean aniBool = src ? ANI_TRUE : ANI_FALSE;
    CHECK_STATUS_RET(env->Object_New(cls, ctor, &aniObj, aniBool), "New bool Object Fail");
    return ANI_OK;
}

ani_status AniParamUtils::ToAniIntObject(ani_env *env, int32_t src, ani_object &aniObj)
{
    static const char *className = "Lstd/core/Int;";
    ani_class cls {};
    CHECK_STATUS_RET(env->FindClass(className, &cls), "Failed to find class: %{public}s", className);

    ani_method ctor {};
    CHECK_STATUS_RET(env->Class_FindMethod(cls, "<ctor>", "I:V", &ctor), "Failed to find method: ctor");

    CHECK_STATUS_RET(env->Object_New(cls, ctor, &aniObj, static_cast<ani_int>(src)), "New int32 Object Fail");
    return ANI_OK;
}

ani_status AniParamUtils::ToAniLongObject(ani_env *env, int64_t src, ani_object &aniObj)
{
    static const char *className = "Lescompat/BigInt;";
    ani_class cls {};
    CHECK_STATUS_RET(env->FindClass(className, &cls), "Failed to find class: %{public}s", className);

    ani_method ctor {};
    CHECK_STATUS_RET(env->Class_FindMethod(cls, "<ctor>", "J:V", &ctor), "Failed to find method: ctor");

    CHECK_STATUS_RET(env->Object_New(cls, ctor, &aniObj, static_cast<ani_long>(src)), "New int64_t Object Fail");
    return ANI_OK;
}

ani_status AniParamUtils::GetOptionalEnumInt32Field(ani_env *env, ani_object src, const std::string &fieldName,
    int32_t &value)
{
    ani_ref field_ref;
    if (ANI_OK != env->Object_GetPropertyByName_Ref(src, fieldName.c_str(), &field_ref)) {
        ANI_ERR_LOG("Object_GetPropertyByName_Ref %{public}s Failed", fieldName.c_str());
        return ANI_INVALID_ARGS;
    }

    ani_boolean isUndefined;
    env->Reference_IsUndefined(field_ref, &isUndefined);
    if (isUndefined) {
        ANI_ERR_LOG("%{public}s is undefined", fieldName.c_str());
        return ANI_NOT_FOUND;
    }

    ani_int enum_value {};
    CHECK_STATUS_RET(env->EnumItem_GetValue_Int(static_cast<ani_enum_item>(field_ref), &enum_value),
        "EnumItem_GetValue_Int failed");
    CHECK_STATUS_RET(GetInt32(env, enum_value, value), "GetInt32 failed");
    ANI_INFO_LOG("%{public}s Get %{public}s: %{public}d", __func__, fieldName.c_str(), value);
    return ANI_OK;
}

ani_status AniParamUtils::GetOptionalInt32Field(ani_env *env, ani_object src,
    const std::string &fieldName, int32_t &value)
{
    ani_double field_double;
    if (ANI_OK != env->Object_GetPropertyByName_Double(src, fieldName.c_str(), &field_double)) {
        ANI_ERR_LOG("Object_GetPropertyByName_Ref %{public}s Failed", fieldName.c_str());
        return ANI_INVALID_ARGS;
    }
    value = static_cast<int32_t>(field_double);
    return ANI_OK;
}

ani_status AniParamUtils::GetRendererInfo(ani_env *env, ani_object arg, AudioRendererInfo *rendererInfo)
{
    CHECK_COND_RET(env != nullptr, ANI_ERROR, "env is nullptr");
    CHECK_COND_RET(arg != nullptr, ANI_ERROR, "arg is nullptr");

    int32_t intValue = {0};
    ani_status status = GetOptionalEnumInt32Field(env, arg, "content", intValue);
    if (status == ANI_OK) {
        rendererInfo->contentType = static_cast<ContentType>(intValue);
    }

    status = GetOptionalEnumInt32Field(env, arg, "usage", intValue);
    if (status == ANI_OK) {
        if (AudioEnumAni::IsLegalInputArgumentStreamUsage(intValue)) {
            rendererInfo->streamUsage = static_cast<StreamUsage>(intValue);
        } else {
            rendererInfo->streamUsage = StreamUsage::STREAM_USAGE_INVALID;
        }
    }
    GetOptionalInt32Field(env, arg, "rendererFlags", rendererInfo->rendererFlags);

    return ANI_OK;
}

void AniParamUtils::CreateAniErrorObject(ani_env *env, ani_object &errorObj, const int32_t errCode,
    const std::string &errMsg)
{
    static const std::string className = "L@ohos/multimedia/audio/audio/AudioAniError;";
    ani_class cls;
    ani_status status = env->FindClass(className.c_str(), &cls);
    if (status != ANI_OK) {
        ANI_ERR_LOG("Can't find class %{public}s", className.c_str());
        return;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "DLstd/core/String;:V", &ctor)) {
        ANI_ERR_LOG("Can't find <ctor> from class %{public}s", className.c_str());
        return;
    }
    ani_string error_msg;
    if (ANI_OK != AniParamUtils::ToAniString(env, errMsg, error_msg)) {
        ANI_ERR_LOG("Call ToAniString function failed.");
        return;
    }

    if (ANI_OK != env->Object_New(cls, ctor, &errorObj, (ani_double)errCode, error_msg)) {
        ANI_ERR_LOG("New MediaLibraryAniError object failed.");
        return;
    }
    return;
}

} // namespace AudioStandard
} // namespace OHOS
