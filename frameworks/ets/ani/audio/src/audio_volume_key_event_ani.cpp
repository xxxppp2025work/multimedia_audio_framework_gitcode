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

#include "audio_volume_key_event_ani.h"

#include <mutex>
#include <thread>
#include "ani_class_name.h"

using namespace std;
namespace OHOS {
namespace AudioStandard {
mutex AniAudioVolumeKeyEvent::sWorkerMutex_;

AniAudioVolumeKeyEvent::AniAudioVolumeKeyEvent(ani_env *env)
    :env_(env)
{
    ANI_DEBUG_LOG("AniAudioVolumeKeyEvent::Constructor");
}

AniAudioVolumeKeyEvent::~AniAudioVolumeKeyEvent()
{
    ANI_DEBUG_LOG("AniAudioVolumeKeyEvent::Destructor");
}

void AniAudioVolumeKeyEvent::OnVolumeKeyEvent(VolumeEvent volumeEvent)
{
    std::lock_guard<std::mutex> lock(mutex_);
    ANI_INFO_LOG("OnVolumeKeyEvent is called volumeType=%{public}d, volumeLevel=%{public}d,"
        "isUpdateUi=%{public}d", volumeEvent.volumeType, volumeEvent.volume, volumeEvent.updateUi);
    ANI_CHECK_RETURN_LOG(audioVolumeKeyEventJsCallback_ != nullptr,
        "AniAudioVolumeKeyEvent:No JS callback registered return");
    std::unique_ptr<AudioVolumeKeyEventJsCallback> cb = std::make_unique<AudioVolumeKeyEventJsCallback>();
    ANI_CHECK_RETURN_LOG(cb != nullptr, "No memory");
    cb->callback = audioVolumeKeyEventJsCallback_;
    cb->callbackName = VOLUME_KEY_EVENT_CALLBACK_NAME;
    cb->volumeEvent.volumeType = volumeEvent.volumeType;
    cb->volumeEvent.volume = volumeEvent.volume;
    cb->volumeEvent.updateUi = volumeEvent.updateUi;
    cb->volumeEvent.volumeGroupId = volumeEvent.volumeGroupId;
    cb->volumeEvent.networkId = volumeEvent.networkId;

    return OnJsCallbackVolumeEvent(cb);
}

void AniAudioVolumeKeyEvent::SaveCallbackReference(const std::string &callbackName, ani_object object)
{
    std::lock_guard<std::mutex> lock(mutex_);
    ANI_CHECK_RETURN_LOG(object != nullptr, "AniAudioVolumeKeyEvent: creating reference for callback fail");
    ani_ref callback = static_cast<ani_ref>(object);
    callback_ = callback;
    std::shared_ptr<AutoRef> cb = std::make_shared<AutoRef>(env_, callback);
    if (callbackName == VOLUME_KEY_EVENT_CALLBACK_NAME) {
        audioVolumeKeyEventJsCallback_ = cb;
    } else {
        ANI_ERR_LOG("AniAudioVolumeKeyEvent: Unknown callback type: %{public}s", callbackName.c_str());
    }
}

bool AniAudioVolumeKeyEvent::GetVolumeTsfnFlag()
{
    return regVolumeTsfn_;
}

void AniAudioVolumeKeyEvent::CreateVolumeTsfn(ani_env *env)
{
    regVolumeTsfn_ = true;
    SafeJsCallbackVolumeEventWork(env, event_);
}

void AniAudioVolumeKeyEvent::SafeJsCallbackVolumeEventWork(ani_env *env, AudioVolumeKeyEventJsCallback* event)
{
    std::shared_ptr<AudioVolumeKeyEventJsCallback> safeContext(
        static_cast<AudioVolumeKeyEventJsCallback*>(event),
        [](AudioVolumeKeyEventJsCallback *ptr) {
            delete ptr;
    });
    std::string request = event->callbackName;
    ani_object callback = static_cast<ani_object>(event->callback->cb_);
    ANI_INFO_LOG("SafeJsCallbackVolumeEventWork: safe js callback working.");

    do {
        ani_status status;
        ani_object result = nullptr;
        status = AniParamUtils::SetValueVolumeEvent(env, event->volumeEvent, result);
        ANI_CHECK_AND_BREAK_LOG(status == ANI_OK && result != nullptr,
            "%{public}s fail to create volumeChange callback", request.c_str());

        std::thread worker(ExecuteThreadWork, env, result, callback);
        worker.join();
    } while (0);
}

void AniAudioVolumeKeyEvent::ExecuteThreadWork(ani_env *env, ani_object result, ani_object callback)
{
    lock_guard<mutex> lock(sWorkerMutex_);

    ani_vm *etsVm {};
    CHECK_IF_EQUAL(env->GetVM(&etsVm) == ANI_OK, "Get etsVm fail");

    ani_env *etsEnv {};
    ani_option interopEnabled {"--interop=disable", nullptr};
    ani_options aniArgs {1, &interopEnabled};
    CHECK_IF_EQUAL(etsVm->AttachCurrentThread(&aniArgs, ANI_VERSION_1, &etsEnv) == ANI_OK, "AttachCurrentThread fail");

    static const char *className = ANI_CLASS_VOLUME_EVENT_HANDLE.c_str();
    ani_class cls {};
    CHECK_IF_EQUAL(etsEnv->FindClass(className, &cls) == ANI_OK, "Failed to find class: %{public}s", className);

    ani_static_method callMethod {};
    CHECK_IF_EQUAL(etsEnv->Class_FindStaticMethod(cls, "callRegistedCallback", nullptr, &callMethod) == ANI_OK,
        "Failed to find method: callRegistedCallback");

    CHECK_IF_EQUAL(etsEnv->Class_CallStaticMethod_Void(cls, callMethod, result, callback) == ANI_OK,
        "Failed to call method: callRegistedCallback");

    CHECK_IF_EQUAL(etsVm->DetachCurrentThread() == ANI_OK, "DetachCurrentThread fail");
}

void AniAudioVolumeKeyEvent::OnJsCallbackVolumeEvent(std::unique_ptr<AudioVolumeKeyEventJsCallback> &jsCb)
{
    if (jsCb.get() == nullptr) {
        ANI_ERR_LOG("OnJsCallbackVolumeEvent: jsCb.get() is null");
        return;
    }

    AudioVolumeKeyEventJsCallback *event = jsCb.release();
    ANI_CHECK_RETURN_LOG((event != nullptr) && (event->callback != nullptr), "event is nullptr.");
    event_ = event;
}
} // namespace AudioStandard
} // namespace OHOS
