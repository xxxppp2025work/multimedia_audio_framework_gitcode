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
#ifndef FRAMEWORKS_ANI_SRC_INCLUDE_AUDIO_VOLUME_KEY_EVENT_ANI_H
#define FRAMEWORKS_ANI_SRC_INCLUDE_AUDIO_VOLUME_KEY_EVENT_ANI_H

#include "ani_param_utils.h"
#include "audio_system_manager.h"
#include "audio_ani_log.h"
#include "ani.h"
#include "ani_work.h"

namespace OHOS {
namespace AudioStandard {

const std::string VOLUME_KEY_EVENT_CALLBACK_NAME = "volumeChange";

class AniAudioVolumeKeyEvent : public VolumeKeyEventCallback {
public:
    explicit AniAudioVolumeKeyEvent(ani_env *env);
    virtual ~AniAudioVolumeKeyEvent();
    void OnVolumeKeyEvent(VolumeEvent volumeEvent) override;
    void SaveCallbackReference(const std::string &callbackName, ani_object object);
    bool GetVolumeTsfnFlag();
    void CreateVolumeTsfn(ani_env *env);
    static void ExecuteThreadWork(ani_env *env, ani_object result, ani_object callback);

private:
    struct AudioVolumeKeyEventJsCallback {
        std::shared_ptr<AutoRef> callback = nullptr;
        std::string callbackName = "unknown";
        VolumeEvent volumeEvent;
    };

    void OnJsCallbackVolumeEvent(std::unique_ptr<AudioVolumeKeyEventJsCallback> &jsCb);
    static void SafeJsCallbackVolumeEventWork(ani_env *env, AudioVolumeKeyEventJsCallback* event);

    std::mutex mutex_;
    ani_env *env_;
    ani_ref callback_ = nullptr;
    std::shared_ptr<AutoRef> audioVolumeKeyEventJsCallback_ = nullptr;
    bool regVolumeTsfn_ = false;
    AudioVolumeKeyEventJsCallback *event_;
    static std::mutex sWorkerMutex_;
};
}  // namespace AudioStandard
}  // namespace OHOS
#endif // FRAMEWORKS_ANI_SRC_INCLUDE_AUDIO_VOLUME_KEY_EVENT_ANI_H
