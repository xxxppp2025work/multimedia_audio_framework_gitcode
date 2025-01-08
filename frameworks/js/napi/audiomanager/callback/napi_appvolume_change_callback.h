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

#ifndef NAPI_APPVOLUME_CHANGE_CALLBACK_H
#define NAPI_APPVOLUME_CHANGE_CALLBACK_H

#include <uv.h>
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "napi_async_work.h"
#include "napi_audio_manager.h"
#include "audio_system_manager.h"

namespace OHOS {
namespace AudioStandard {
const std::string APP_VOLUME_CHANGE_CALLBACK_NAME_FOR_UID = "appVolumeChangeForUid";
const std::string APP_VOLUME_CHANGE_CALLBACK_NAME = "appVolumeChange";
class NapiAudioManagerAppVolumeChangeCallback : public AudioManagerAppVolumeChangeCallback {
public:
    explicit NapiAudioManagerAppVolumeChangeCallback(napi_env env);
    virtual ~NapiAudioManagerAppVolumeChangeCallback();
    void SaveCallbackReference(const std::string &callbackName, napi_value args);
    bool IsSameCallback(const napi_value args);
    void RemoveCallbackReference(const napi_value args);
    void OnAppVolumeChangedForUid(const VolumeEvent &event) override;
    void OnSelfAppVolumeChanged(const VolumeEvent &event) override;
    void CreateManagerAppVolumeChangeTsfn(napi_env env);
    bool GetManagerAppVolumeChangeTsfnFlag();

private:
    struct AudioManagerAppVolumeChangeJsCallback {
        std::shared_ptr<AutoRef> callback = nullptr;
        std::string callbackName = "unknown";
        VolumeEvent appVolumeChangeEvent;
    };

    void OnJsCallbackAppVolumeChange(std::unique_ptr<AudioManagerAppVolumeChangeJsCallback> &jsCb);
    static void AppVolumeChangeTsfnFinalize(napi_env env, void *data, void *hint);
    static void SafeJsCallbackAppVolumeChangeWork(napi_env env, napi_value js_cb, void *context, void *data);

    std::mutex mutex_;
    napi_env env_ = nullptr;
    std::shared_ptr<AutoRef> appVolumeChangeCallback_ = nullptr;
    bool regAmAppVolumeChgTsfn_ = false;
    napi_threadsafe_function amAppVolumeChgTsfn_ = nullptr;
};
}
}

#endif