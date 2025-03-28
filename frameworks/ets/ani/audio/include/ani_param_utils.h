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
#ifndef FRAMEWORKS_ANI_SRC_INCLUDE_ANI_PARAM_UTILS_H
#define FRAMEWORKS_ANI_SRC_INCLUDE_ANI_PARAM_UTILS_H

#include "ani.h"
#include "audio_system_manager.h"
#include "audio_ani_log.h"

namespace OHOS {
namespace AudioStandard {
const int ARGS_ONE = 1;
const int ARGS_TWO = 2;
const int ARGS_THREE = 3;

const int PARAM0 = 0;
const int PARAM1 = 1;
const int PARAM2 = 2;

const std::vector<DeviceRole> DEVICE_ROLE_SET = {
    DEVICE_ROLE_NONE,
    INPUT_DEVICE,
    OUTPUT_DEVICE
};

const std::vector<DeviceType> DEVICE_TYPE_SET = {
    DEVICE_TYPE_NONE,
    DEVICE_TYPE_INVALID,
    DEVICE_TYPE_EARPIECE,
    DEVICE_TYPE_SPEAKER,
    DEVICE_TYPE_WIRED_HEADSET,
    DEVICE_TYPE_WIRED_HEADPHONES,
    DEVICE_TYPE_BLUETOOTH_SCO,
    DEVICE_TYPE_BLUETOOTH_A2DP,
    DEVICE_TYPE_MIC,
    DEVICE_TYPE_WAKEUP,
    DEVICE_TYPE_USB_HEADSET,
    DEVICE_TYPE_DP,
    DEVICE_TYPE_REMOTE_CAST,
    DEVICE_TYPE_USB_ARM_HEADSET,
    DEVICE_TYPE_FILE_SINK,
    DEVICE_TYPE_FILE_SOURCE,
    DEVICE_TYPE_EXTERN_CABLE,
    DEVICE_TYPE_DEFAULT
};


#define CHECK_ARGS_WITH_MESSAGE(env, cond, msg)                                                 \
    do {                                                                                        \
        if (!(cond)) {                                                                          \
            AniError::ThrowError(env, JS_ERR_PARAMETER_INVALID, __FUNCTION__, __LINE__, msg);   \
            return nullptr;                                                                     \
        }                                                                                       \
    } while (0)

#define CHECK_COND_WITH_MESSAGE(env, cond, msg)                                                 \
    do {                                                                                        \
        if (!(cond)) {                                                                          \
            AniError::ThrowError(env, OHOS_INVALID_PARAM_CODE, __FUNCTION__, __LINE__, msg);    \
            return nullptr;                                                                     \
        }                                                                                       \
    } while (0)

#define CHECK_COND_WITH_RET_MESSAGE(env, cond, ret, msg)                                        \
    do {                                                                                        \
        if (!(cond)) {                                                                          \
            AniError::ThrowError(env, OHOS_INVALID_PARAM_CODE, __FUNCTION__, __LINE__, msg);    \
            return ret;                                                                         \
        }                                                                                       \
    } while (0)

#define ANI_ASSERT(env, cond, msg) CHECK_ARGS_WITH_MESSAGE(env, cond, msg)

#define ASSERT_NULLPTR_CHECK(env, result)       \
    do {                                        \
        if ((result) == nullptr) {              \
            GetUndefiend(env, &(result));       \
            return result;                      \
        }                                       \
    } while (0)

#define CHECK_NULL_PTR_RETURN_UNDEFINED(env, ptr, ret, message)     \
    do {                                                            \
        if ((ptr) == nullptr) {                                     \
            ANI_ERR_LOG(message);                                   \
            GetUndefiend(env, &(ret));                              \
            return ret;                                             \
        }                                                           \
    } while (0)

#define CHECK_NULL_PTR_RETURN_VOID(ptr, message)    \
    do {                                            \
        if ((ptr) == nullptr) {                     \
            ANI_ERR_LOG(message);                   \
            return;                                 \
        }                                           \
    } while (0)
#define CHECK_IF_EQUAL(condition, errMsg, ...)      \
    do {                                            \
        if (!(condition)) {                         \
            ANI_ERR_LOG(errMsg, ##__VA_ARGS__);     \
            return;                                 \
        }                                           \
    } while (0)

#define CHECK_COND_RET(cond, ret, message, ...)                     \
    do {                                                            \
        if (!(cond)) {                                              \
            ANI_ERR_LOG(message, ##__VA_ARGS__);                    \
            return ret;                                             \
        }                                                           \
    } while (0)

#define CHECK_STATUS_RET(cond, message, ...)                        \
    do {                                                            \
        ani_status __ret = (cond);                                  \
        if (__ret != ANI_OK) {                                      \
            ANI_ERR_LOG(message, ##__VA_ARGS__);                    \
            return __ret;                                           \
        }                                                           \
    } while (0)

#define CHECK_NULLPTR_RET(ret)                                      \
    do {                                                            \
        if ((ret) == nullptr) {                                     \
            return nullptr;                                         \
        }                                                           \
    } while (0)

#define CHECK_ARGS_BASE(env, cond, err, retVal)                     \
    do {                                                            \
        if ((cond) != ANI_OK) {                                    \
            AniError::ThrowError(env, err, __FUNCTION__, __LINE__); \
            return retVal;                                          \
        }                                                           \
    } while (0)

#define CHECK_ARGS(env, cond, err) CHECK_ARGS_BASE(env, cond, err, nullptr)

#define CHECK_ARGS_THROW_INVALID_PARAM(env, cond) CHECK_ARGS(env, cond, OHOS_INVALID_PARAM_CODE)

#define CHECK_ARGS_RET_VOID(env, cond, err)                         \
    do {                                                            \
        if ((cond) != ANI_OK) {                                     \
            AniError::ThrowError(env, err, __FUNCTION__, __LINE__); \
            return;                                                 \
        }                                                           \
    } while (0)

#define CHECK_COND(env, cond, err)                                  \
    do {                                                            \
        if (!(cond)) {                                              \
            AniError::ThrowError(env, err, __FUNCTION__, __LINE__); \
            return nullptr;                                         \
        }                                                           \
    } while (0)

#define RETURN_ANI_UNDEFINED(env)                                         \
    do {                                                                  \
        ani_ref result = nullptr;                                         \
        ani_status __ret = ((env)->GetUndefined(&result));                \
        CHECK_ARGS((env), __ret, JS_INNER_FAIL);                          \
        return static_cast<ani_object>(result);                           \
    } while (0)

/* check condition related to argc/argv, return and logging. */
#define ANI_CHECK_ARGS_RETURN_VOID(context, condition, message, code)               \
    do {                                                               \
        if (!(condition)) {                                            \
            (context)->status = ANI_INVALID_ARGS;                         \
            (context)->errMessage = std::string(message);                      \
            (context)->errCode = code;                      \
            ANI_ERR_LOG("test (" #condition ") failed: " message);           \
            return;                                                    \
        }                                                              \
    } while (0)

#define ANI_CHECK_STATUS_RETURN_VOID(context, message, code)                        \
    do {                                                               \
        if ((context)->status != ANI_OK) {                               \
            (context)->errMessage = std::string(message);                      \
            (context)->errCode = code;                      \
            ANI_ERR_LOG("test (context->status == ANI_OK) failed: " message);  \
            return;                                                    \
        }                                                              \
    } while (0)

#define ANI_CHECK_AND_BREAK_LOG(cond, fmt, ...)            \
    if (1) {                                           \
        if (!(cond)) {                                 \
            ANI_ERR_LOG(fmt, ##__VA_ARGS__);         \
            break;                                     \
        }                                              \
    } else void (0)

class AniParamUtils {
public:
    static ani_boolean isUndefined(ani_env *env, ani_object object);
    static ani_status GetInt32(ani_env *env, ani_int arg, int32_t &value);
    static ani_status GetInt32(ani_env *env, ani_object arg, int32_t &value);
    static ani_status SetValueEnum(ani_env *env, ani_class cls, ani_object handle,
        const std::string &key, ani_enum_item value);
    static ani_status SetValueInt(ani_env *env, ani_class cls, ani_object handle,
        const std::string &key, int32_t value);
    static ani_status SetValueNumber(ani_env *env, ani_class cls, ani_object handle,
        const std::string &key, int32_t value);
    static ani_status SetValueBool(ani_env *env, ani_class cls, ani_object handle,
    const std::string &key, bool value);
    static ani_status SetValueString(ani_env *env, ani_class cls, ani_object handle,
        const std::string &key, const std::string &value);
    static ani_status SetValueObject(ani_env *env, ani_class cls, ani_object handle,
        const std::string &key, ani_object value);
    static ani_status CreateVolumeEventHandle(ani_env *env, ani_class &cls, ani_object &result);
    static ani_status SetValueVolumeEvent(ani_env *env, const VolumeEvent &volumeEvent, ani_object handle);
    static ani_status CreateAudioRendererInfoHandle(ani_env *env, ani_class &cls, ani_object &result);
    static ani_status SetRendererInfo(ani_env *env,
        const AudioRendererInfo &audioRendererInfo, ani_object handle);
    static ani_status CreateInterruptEventHandle(ani_env *env, ani_class &cls, ani_object &result);
    static ani_status SetInterruptEvent(ani_env *env,
        const InterruptEvent &interruptEvent, ani_object handle);
    static ani_status CreateAudioDeviceDescriptorHandle(ani_env *env, ani_class &cls, ani_object &result);
    static ani_status MakeAniArrayRetSetMethod(ani_env *env, size_t size, ani_object &aniArray, ani_method &setMethod);
    static ani_status SetValueInt32Element(ani_env *env, ani_class cls, ani_object &result,
        const std::string &key, const std::vector<int32_t> &values);
    static ani_status SetSetDeviceDescriptorMore(ani_env *env, ani_class cls,
        const AudioDeviceDescriptor &deviceInfo, ani_object &result);
    static ani_status SetDeviceDescriptor(ani_env *env, const AudioDeviceDescriptor &deviceInfo, ani_object &result);
    static ani_status SetDeviceDescriptors(ani_env *env, ani_object &aniArray,
        const std::vector<sptr<AudioDeviceDescriptor>> &deviceDescriptors);
    static ani_status SetValueDeviceInfo(ani_env *env, const AudioDeviceDescriptor &deviceInfo, ani_object handle);
    static std::string GetStringArgument(ani_env *env, ani_string aniStr);
    static ani_status GetString(ani_env *env, ani_string arg, std::string &str);
    static ani_status GetString(ani_env *env, ani_object arg, std::string &str);
    static ani_status ToAniString(ani_env *env, const std::string &str, ani_string &aniStr);
    static ani_status ToAniInt(ani_env *env, const std::int32_t &int32, ani_int &aniInt);
    static ani_status ToAniLong(ani_env *env, const std::int64_t &int64, ani_long &aniLong);
    static ani_status ToAniDouble(ani_env *env, const double &arg, ani_double &aniDouble);
    static ani_status ToAniBooleanObject(ani_env *env, bool src, ani_object &aniObj);
    static ani_status ToAniIntObject(ani_env *env, int32_t src, ani_object &aniObj);
    static ani_status ToAniLongObject(ani_env *env, int64_t src, ani_object &aniObj);
    static ani_status GetOptionalEnumInt32Field(ani_env *env, ani_object src,
        const std::string &fieldName, int32_t &value);
    static ani_status GetOptionalInt32Field(ani_env *env, ani_object src,
        const std::string &fieldName, int32_t &value);
    static ani_status GetRendererInfo(ani_env *env, ani_object arg, AudioRendererInfo *rendererInfo);
};
} // namespace AudioStandard
} // namespace OHOS
#endif // FRAMEWORKS_ANI_SRC_INCLUDE_ANI_PARAM_UTILS_H
