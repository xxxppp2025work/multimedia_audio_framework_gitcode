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

#include "audio_volume_manager_ani.h"
#include "ani_class_name.h"
#include "ani_audio_error.h"

namespace OHOS {
namespace AudioStandard {

AudioVolumeManagerAni::AudioVolumeManagerAni()
    : audioSystemMngr_(nullptr), env_(nullptr) {
}

AudioVolumeManagerAni::~AudioVolumeManagerAni() = default;

ani_status AudioVolumeManagerAni::AudioVolumeManagerAniInit(ani_env *env)
{
    static const char *className = ANI_CLASS_AUDIO_VOLUME_MANAGER_HANDLE.c_str();
    ani_class cls;
    ani_status status = env->FindClass(className, &cls);
    if (status != ANI_OK) {
        ANI_ERR_LOG("Failed to find class: %{public}s", className);
        env->ThrowError(reinterpret_cast<ani_error>(status));
    }

    std::array methods = {
        ani_native_function {"onInner", nullptr, reinterpret_cast<void *>(AudioVolumeManagerAni::On) },
    };
    status = env->Class_BindNativeMethods(cls, methods.data(), methods.size());
    if (status != ANI_OK) {
        ANI_ERR_LOG("Failed to bind native methods to: %{public}s", className);
        env->ThrowError(reinterpret_cast<ani_error>(status));
    }
    return ANI_OK;
}

ani_object AudioVolumeManagerAni::CreateVolumeManagerWrapper(ani_env *env)
{
    ani_class clazz = nullptr;
    return Constructor(env, clazz);
}

ani_object AudioVolumeManagerAni::Constructor([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_class clazz)
{
    std::unique_ptr<AudioVolumeManagerAni> audioVolumeManagerAni = std::make_unique<AudioVolumeManagerAni>();
    if (audioVolumeManagerAni != nullptr) {
        audioVolumeManagerAni->env_ = env;
        audioVolumeManagerAni->audioSystemMngr_ = AudioSystemManager::GetInstance();
        audioVolumeManagerAni->cachedClientId_ = getpid();
    }

    static const char *className = ANI_CLASS_AUDIO_VOLUME_MANAGER_HANDLE.c_str();
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        ANI_ERR_LOG("Failed to find class: %{public}s", className);
        ani_object nullobj = nullptr;
        return nullobj;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "J:V", &ctor)) {
        ANI_ERR_LOG("Failed to find method: %{public}s", "ctor");
        ani_object nullobj = nullptr;
        return nullobj;
    }

    ani_object audioManager_object;
    if (ANI_OK !=env->Object_New(cls, ctor, &audioManager_object,
        reinterpret_cast<ani_long>(audioVolumeManagerAni.release()))) {
        ANI_ERR_LOG("New AudioVolumeManager Fail");
    }
    return audioManager_object;
}

AudioVolumeManagerAni* AudioVolumeManagerAni::Unwrap(ani_env *env, ani_object object)
{
    ani_long audioVolumeManager;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeAudioVolumeManagerContext", &audioVolumeManager)) {
        return nullptr;
    }
    return reinterpret_cast<AudioVolumeManagerAni*>(audioVolumeManager);
}

void AudioVolumeManagerAni::RegisterCallback(ani_env *env, ani_object jsThis,
    const std::string &cbName, ani_object callback)
{
    AudioVolumeManagerAni* audioVolumeManagerAni =  AudioVolumeManagerAni::Unwrap(env, jsThis);
    ANI_CHECK_RETURN_RET_LOG(audioVolumeManagerAni != nullptr,
        AniAudioError::ThrowError(env, ANI_ERR_NO_MEMORY), "audioVolumeManagerAni is nullptr");
    ANI_CHECK_RETURN_RET_LOG(audioVolumeManagerAni->audioSystemMngr_ != nullptr,
        AniAudioError::ThrowError(env, ANI_ERR_NO_MEMORY), "audioSystemMngr_ is nullptr");

    if (!cbName.compare(VOLUME_KEY_EVENT_CALLBACK_NAME)) {
        if (audioVolumeManagerAni->volumeKeyEventCallbackAni_ == nullptr) {
            audioVolumeManagerAni->volumeKeyEventCallbackAni_ = std::make_shared<AniAudioVolumeKeyEvent>(env);
            int32_t ret = audioVolumeManagerAni->audioSystemMngr_->RegisterVolumeKeyEventCallback(
                audioVolumeManagerAni->cachedClientId_, audioVolumeManagerAni->volumeKeyEventCallbackAni_);
            audioVolumeManagerAni->volumeKeyEventCallbackAniList_.push_back(
                std::static_pointer_cast<AniAudioVolumeKeyEvent>(audioVolumeManagerAni->volumeKeyEventCallbackAni_));
            if (ret) {
                ANI_ERR_LOG("RegisterVolumeKeyEventCallback Failed");
            }
        }
        std::shared_ptr<AniAudioVolumeKeyEvent> cb =
            std::static_pointer_cast<AniAudioVolumeKeyEvent>(audioVolumeManagerAni->volumeKeyEventCallbackAni_);
        cb->SaveCallbackReference(cbName, callback);
        if (!cb->GetVolumeTsfnFlag()) {
            cb->CreateVolumeTsfn(env);
        }
    } else {
        ANI_ERR_LOG("No such callback supported");
        AniAudioError::ThrowError(env, ANI_ERR_INVALID_PARAM,
            "parameter verification failed: The param of type is not supported");
    }
}

void AudioVolumeManagerAni::On(ani_env *env, ani_object object, ani_string type, ani_object callback)
{
    ANI_CHECK_RETURN_RET_LOG(object != nullptr,
        AniAudioError::ThrowError(env, ANI_ERR_INPUT_INVALID), "object is nullptr");
    std::string callbackName = AniParamUtils::GetStringArgument(env, type);
    ANI_INFO_LOG("On callbackName: %{public}s", callbackName.c_str());

    return RegisterCallback(env, object, callbackName, callback);
}

} // namespace AudioStandard
} // namespace OHOS
