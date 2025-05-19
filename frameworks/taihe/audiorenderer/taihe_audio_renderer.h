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

#ifndef TAIHE_AUDIO_RENDERER_H
#define TAIHE_AUDIO_RENDERER_H

#include "audio_renderer.h"
#include "taihe_audio_renderer_device_change_callback.h"

namespace ANI::Audio {
using namespace taihe;
using namespace ohos::multimedia::audio;

class AudioRendererImpl {
public:
AudioRendererImpl();
    explicit AudioRendererImpl(std::unique_ptr<AudioRendererImpl> obj);
    ~AudioRendererImpl();

    static void CreateRendererFailed();
    static std::unique_ptr<AudioRendererImpl> CreateAudioRendererNativeObject();
    static AudioRenderer CreateAudioRendererWrapper(OHOS::AudioStandard::AudioRendererOptions rendererOptions);

#if !defined(ANDROID_PLATFORM) && !defined(IOS_PLATFORM)
    std::shared_ptr<OHOS::AudioStandard::AudioRenderer> audioRenderer_;
#else
    std::unique_ptr<OHOS::AudioStandard::AudioRenderer> audioRenderer_;
#endif

    AudioState GetState();
    void StartSync();
    void StopSync();
    void ReleaseSync();
    int64_t GetBufferSizeSync();
    int64_t GetAudioStreamIdSync();
    void SetVolumeSync(double volume);
    double GetVolume();
    double GetMinStreamVolumeSync();
    double GetMaxStreamVolumeSync();
    int64_t GetUnderflowCountSync();
    AudioStreamInfo GetStreamInfoSync();

    void OnStateChange(callback_view<void(AudioState)> callback);
    void OnAudioInterrupt(callback_view<void(InterruptEvent const&)> callback);
    void OnPeriodReach(double frame, callback_view<void(double)> callback);
    void OnMarkReach(double frame, callback_view<void(double)> callback);
    void OnOutputDeviceChange(callback_view<void(array_view<AudioDeviceDescriptor>)> callback);
    void OnOutputDeviceChangeWithInfo(callback_view<void(AudioStreamDeviceChangeInfo const&)> callback);

    void OffAudioInterrupt(optional_view<callback<void(InterruptEvent const&)>> callback);
    void OffStateChange(optional_view<callback<void(AudioState)>> callback);
    void OffOutputDeviceChange(optional_view<callback<void(array_view<AudioDeviceDescriptor>)>> callback);
    void OffOutputDeviceChangeWithInfo(optional_view<callback<void(AudioStreamDeviceChangeInfo const&)>> callback);
    void OffPeriodReach(optional_view<callback<void(double)>> callback);
    void OffMarkReach(optional_view<callback<void(double)>> callback);

    void DestroyCallbacks();
    void DestroyTaiheCallbacks();

private:
    static void RegisterRendererCallback(std::shared_ptr<uintptr_t> &callback,
        const std::string &cbName, AudioRendererImpl *taiheRenderer);
    static void RegisterRendererDeviceChangeCallback(std::shared_ptr<uintptr_t> &callback,
        const std::string &cbName, AudioRendererImpl *taiheRenderer);
    static void RegisterPositionCallback(double markPosition, std::shared_ptr<uintptr_t> &callback,
        const std::string &cbName, AudioRendererImpl *taiheRenderer);
    static void RegisterPeriodPositionCallback(double frame, std::shared_ptr<uintptr_t> &callback,
        const std::string &cbName, AudioRendererImpl *taiheRenderer);
    static void RegisterRendererOutputDeviceChangeWithInfoCallback(std::shared_ptr<uintptr_t> &callback,
        const std::string &cbName, AudioRendererImpl *taiheRenderer);
    static void UnregisterRendererCallback(std::shared_ptr<uintptr_t> &callback,
        const std::string &cbName, AudioRendererImpl *taiheRenderer);
    static void UnregisterRendererDeviceChangeCallback(std::shared_ptr<uintptr_t> &callback,
        const std::string &cbName, AudioRendererImpl *taiheRenderer);
    static void UnregisterRendererOutputDeviceChangeWithInfoCallback(std::shared_ptr<uintptr_t> &callback,
        const std::string &cbName, AudioRendererImpl *taiheRenderer);
    static void UnregisterPeriodPositionCallback(std::shared_ptr<uintptr_t> &callback,
        const std::string &cbName, AudioRendererImpl *taiheRenderer);
    static void UnregisterPositionCallback(std::shared_ptr<uintptr_t> &callback,
        const std::string &cbName, AudioRendererImpl *taiheRenderer);

    std::shared_ptr<OHOS::AudioStandard::AudioRendererCallback> callbackTaihe_ = nullptr;
    std::shared_ptr<OHOS::AudioStandard::RendererPositionCallback> positionCbTaihe_ = nullptr;
    std::shared_ptr<OHOS::AudioStandard::RendererPeriodPositionCallback> periodPositionCbTaihe_ = nullptr;
    std::shared_ptr<TaiheAudioRendererDeviceChangeCallback> rendererDeviceChangeCallbackTaihe_ = nullptr;
    std::shared_ptr<TaiheAudioRendererOutputDeviceChangeWithInfoCallback>
        rendererOutputDeviceChangeWithInfoCallbackTaihe_ = nullptr;
    std::shared_ptr<OHOS::AudioStandard::AudioRendererPolicyServiceDiedCallback>
        rendererPolicyServiceDiedCallbackTaihe_ = nullptr;

    static std::mutex createMutex_;
    static int32_t isConstructSuccess_;
    static std::unique_ptr<OHOS::AudioStandard::AudioRendererOptions> sRendererOptions_;

    OHOS::AudioStandard::ContentType contentType_;
    OHOS::AudioStandard::StreamUsage streamUsage_;
};
} // namespace ANI::Audio

#endif // TAIHE_AUDIO_RENDERER_H
