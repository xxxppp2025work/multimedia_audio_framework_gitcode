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
#ifndef LOG_TAG
#define LOG_TAG "AudioRendererImpl"
#endif

#include "taihe_audio_renderer.h"
#include "errors.h"
#include "audio_log.h"
#include "audio_errors.h"
#include "taihe_audio_enum.h"
#include "taihe_audio_error.h"
#include "taihe_param_utils.h"
#include "taihe_audio_renderer_callback.h"
#include "taihe_renderer_position_callback.h"
#include "taihe_renderer_period_position_callback.h"
#include "taihe_audio_renderer_policy_service_died_callback.h"

using namespace ANI::Audio;

namespace ANI::Audio {
std::unique_ptr<OHOS::AudioStandard::AudioRendererOptions> AudioRendererImpl::sRendererOptions_ = nullptr;
std::mutex AudioRendererImpl::createMutex_;
int32_t AudioRendererImpl::isConstructSuccess_ = OHOS::AudioStandard::SUCCESS;
constexpr double MIN_VOLUME_IN_DOUBLE = 0.0;
constexpr double MAX_VOLUME_IN_DOUBLE = 1.0;

AudioRendererImpl::AudioRendererImpl()
    : audioRenderer_(nullptr), contentType_(OHOS::AudioStandard::ContentType::CONTENT_TYPE_MUSIC),
    streamUsage_(OHOS::AudioStandard::StreamUsage::STREAM_USAGE_MEDIA) {}

AudioRendererImpl::AudioRendererImpl(std::unique_ptr<AudioRendererImpl> obj)
{
    if (obj != nullptr) {
        audioRenderer_ = obj->audioRenderer_;
        contentType_ = obj->contentType_;
        streamUsage_ = obj->streamUsage_;
    }
}
AudioRendererImpl::~AudioRendererImpl()
{
}

void AudioRendererImpl::CreateRendererFailed()
{
    AudioRendererImpl::isConstructSuccess_ = TAIHE_ERR_SYSTEM;
    if (OHOS::AudioStandard::AudioRenderer::CheckMaxRendererInstances() == OHOS::ERR_OVERFLOW) {
        AudioRendererImpl::isConstructSuccess_ = TAIHE_ERR_STREAM_LIMIT;
    }
    AUDIO_ERR_LOG("Renderer Create failed %{public}d", AudioRendererImpl::isConstructSuccess_);
}

std::unique_ptr<AudioRendererImpl> AudioRendererImpl::CreateAudioRendererNativeObject()
{
    std::unique_ptr<AudioRendererImpl> audioRendererImpl = std::make_unique<AudioRendererImpl>();
    if (audioRendererImpl == nullptr) {
        AUDIO_ERR_LOG("No memory");
        return nullptr;
    }
    audioRendererImpl->contentType_ = sRendererOptions_->rendererInfo.contentType;
    audioRendererImpl->streamUsage_ = sRendererOptions_->rendererInfo.streamUsage;

    OHOS::AudioStandard::AudioRendererOptions rendererOptions = *sRendererOptions_;
    /* AudioRenderer not support other rendererFlags, only support flag 0 */
    if (rendererOptions.rendererInfo.rendererFlags != 0) {
        rendererOptions.rendererInfo.rendererFlags = 0;
    }
#if !defined(ANDROID_PLATFORM) && !defined(IOS_PLATFORM)
    audioRendererImpl->audioRenderer_ = OHOS::AudioStandard::AudioRenderer::CreateRenderer(rendererOptions);
#else
    std::string cacheDir = "";
    audioRendererImpl->audioRenderer_ = OHOS::AudioStandard::AudioRenderer::Create(cacheDir, rendererOptions);
#endif
    if (audioRendererImpl->audioRenderer_ == nullptr) {
        AUDIO_ERR_LOG("Renderer Create failed");
        AudioRendererImpl::isConstructSuccess_ = TAIHE_ERR_SYSTEM;
        audioRendererImpl.release();
        return nullptr;
    }

    if (audioRendererImpl->audioRenderer_ == nullptr) {
        CreateRendererFailed();
        audioRendererImpl.release();
        return nullptr;
    }

    if (audioRendererImpl->streamUsage_ == OHOS::AudioStandard::STREAM_USAGE_UNKNOWN) {
        audioRendererImpl->audioRenderer_->SetOffloadAllowed(false);
    }

    if (audioRendererImpl->audioRenderer_ != nullptr && audioRendererImpl->callbackTaihe_ == nullptr) {
        audioRendererImpl->callbackTaihe_ = std::make_shared<TaiheAudioRendererCallback>(get_env());
        CHECK_AND_RETURN_RET_LOG(audioRendererImpl->callbackTaihe_ != nullptr, audioRendererImpl, "No memory");
        int32_t ret = audioRendererImpl->audioRenderer_->SetRendererCallback(audioRendererImpl->callbackTaihe_);
        CHECK_AND_RETURN_RET_LOG(ret == OHOS::AudioStandard::SUCCESS,
            audioRendererImpl, "Construct SetRendererCallback failed");
    }
    return audioRendererImpl;
}

AudioRenderer AudioRendererImpl::CreateAudioRendererWrapper(OHOS::AudioStandard::AudioRendererOptions rendererOptions)
{
    std::lock_guard<std::mutex> lock(createMutex_);
    if (sRendererOptions_ != nullptr) {
        sRendererOptions_.release();
    }
    sRendererOptions_ = std::make_unique<OHOS::AudioStandard::AudioRendererOptions>();
    if (sRendererOptions_ == nullptr) {
        AUDIO_ERR_LOG("sRendererOptions_ create failed");
        return make_holder<AudioRendererImpl, AudioRenderer>(nullptr);
    }
    *sRendererOptions_ = rendererOptions;
    std::unique_ptr<AudioRendererImpl> impl = AudioRendererImpl::CreateAudioRendererNativeObject();
    if (impl == nullptr) {
        AUDIO_ERR_LOG("failed to CreateAudioRendererNativeObject");
        return make_holder<AudioRendererImpl, AudioRenderer>(nullptr);
    }
    return make_holder<AudioRendererImpl, AudioRenderer>(std::move(impl));
}

template <typename T>
static void UnregisterAudioRendererSingletonCallbackTemplate(std::shared_ptr<uintptr_t> &callback,
    const std::string &cbName, std::shared_ptr<T> cb,
    std::function<int32_t(std::shared_ptr<T> callbackPtr,
        std::shared_ptr<uintptr_t> callback)> removeFunction = nullptr)
{
    if (callback != nullptr) {
        CHECK_AND_RETURN_LOG(cb->ContainSameJsCallbackInner(cbName, callback), "callback not exists!");
    }
    cb->RemoveCallbackReference(cbName, callback);

    if (removeFunction == nullptr) {
        return;
    }
    int32_t ret = removeFunction(cb, callback);
    CHECK_AND_RETURN_LOG(ret == OHOS::AudioStandard::SUCCESS, "Unset of Renderer info change call failed");
    return;
}

AudioState AudioRendererImpl::GetState()
{
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, AudioState::key_t::STATE_INVALID, "audioCapturer_ is nullptr");
    OHOS::AudioStandard::RendererState state = audioRenderer_->GetStatus();
    return TaiheAudioEnum::ToTaiheAudioState(state);
}

void AudioRendererImpl::StartSync()
{
    CHECK_AND_RETURN_LOG(audioRenderer_ != nullptr, "audioRenderer_ is nullptr");
    bool ret = audioRenderer_->Start();
    CHECK_AND_RETURN_LOG(ret, "StartSync failure!");
}

void AudioRendererImpl::StopSync()
{
    CHECK_AND_RETURN_LOG(audioRenderer_ != nullptr, "audioRenderer_ is nullptr");
    bool ret = audioRenderer_->Stop();
    CHECK_AND_RETURN_LOG(ret, "StopSync failure!");
}

void AudioRendererImpl::ReleaseSync()
{
    CHECK_AND_RETURN_LOG(audioRenderer_ != nullptr, "audioRenderer_ is nullptr");
    bool ret = audioRenderer_->Release();
    CHECK_AND_RETURN_LOG(ret, "ReleaseSync failure!");
}

int64_t AudioRendererImpl::GetBufferSizeSync()
{
    size_t bufferSize = 0;
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, 0, "audioRenderer_ is nullptr");
    if (audioRenderer_->GetBufferSize(bufferSize) != OHOS::AudioStandard::SUCCESS) {
        TaiheAudioError::ThrowError(TAIHE_ERR_SYSTEM);
        AUDIO_ERR_LOG("GetBufferSizeSync failed");
        return 0;
    }
    return static_cast<int64_t>(bufferSize);
}

int64_t AudioRendererImpl::GetAudioStreamIdSync()
{
    uint32_t audioStreamId = 0;
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, audioStreamId, "audioRenderer_ is nullptr");
    int32_t ret = audioRenderer_->GetAudioStreamId(audioStreamId);
    CHECK_AND_RETURN_RET_LOG(ret == OHOS::AudioStandard::SUCCESS, audioStreamId, "GetAudioStreamId failure!");
    return static_cast<int64_t>(audioStreamId);
}

void AudioRendererImpl::SetVolumeSync(double volume)
{
    CHECK_AND_RETURN_LOG(audioRenderer_ != nullptr, "audioRenderer_ is nullptr");
    if (volume < MIN_VOLUME_IN_DOUBLE || volume > MAX_VOLUME_IN_DOUBLE) {
        TaiheAudioError::ThrowError(TAIHE_ERR_UNSUPPORTED);
        AUDIO_ERR_LOG("SetVolume volume unsupported");
        return;
    }
    int32_t ret = audioRenderer_->SetVolume(static_cast<float>(volume));
    CHECK_AND_RETURN_LOG(ret == OHOS::AudioStandard::SUCCESS, "SetVolumeSync failed");
}

double AudioRendererImpl::GetVolume()
{
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, 0, "audioRenderer_ is nullptr");
    double volLevel = audioRenderer_->GetVolume();
    return volLevel;
}

double AudioRendererImpl::GetMinStreamVolumeSync()
{
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, 0, "audioRenderer_ is nullptr");
    double volLevel = audioRenderer_->GetMinStreamVolume();
    return volLevel;
}

double AudioRendererImpl::GetMaxStreamVolumeSync()
{
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, 0, "audioRenderer_ is nullptr");
    double volLevel = audioRenderer_->GetMaxStreamVolume();
    return volLevel;
}

int64_t AudioRendererImpl::GetUnderflowCountSync()
{
    uint32_t underflowCount = 0;
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, underflowCount, "audioRenderer_ is nullptr");
    underflowCount = audioRenderer_->GetUnderflowCount();
    return static_cast<int64_t>(underflowCount);
}

AudioStreamInfo AudioRendererImpl::GetStreamInfoSync()
{
    AudioStreamInfo emptyStreamInfo {
        .samplingRate = ohos::multimedia::audio::AudioSamplingRate::key_t::SAMPLE_RATE_48000,
        .channels = ohos::multimedia::audio::AudioChannel::key_t::CHANNEL_2,
        .sampleFormat = ohos::multimedia::audio::AudioSampleFormat::key_t::SAMPLE_FORMAT_S16LE,
        .encodingType = ohos::multimedia::audio::AudioEncodingType::key_t::ENCODING_TYPE_RAW,
    };
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, emptyStreamInfo, "audioRenderer_ is nullptr");
    OHOS::AudioStandard::AudioStreamInfo streamInfo;
    int32_t ret = audioRenderer_->GetStreamInfo(streamInfo);
    CHECK_AND_RETURN_RET_LOG(ret == OHOS::AudioStandard::SUCCESS, emptyStreamInfo, "GetStreamInfo failure!");
    std::shared_ptr<OHOS::AudioStandard::AudioStreamInfo> streamInfoPtr =
        std::make_shared<OHOS::AudioStandard::AudioStreamInfo>(streamInfo);
    AudioStreamInfo result = TaiheParamUtils::ToTaiheAudioStreamInfo(streamInfoPtr);
    return result;
}

void AudioRendererImpl::RegisterRendererCallback(std::shared_ptr<uintptr_t> &callback,
    const std::string &cbName, AudioRendererImpl *taiheRenderer)
{
    CHECK_AND_RETURN_RET_LOG(taiheRenderer->callbackTaihe_ != nullptr, TaiheAudioError::ThrowErrorAndReturn(
        TAIHE_ERR_NO_MEMORY), "callbackTaihe_ is nullptr");
    std::shared_ptr<TaiheAudioRendererCallback> cb =
        std::static_pointer_cast<TaiheAudioRendererCallback>(taiheRenderer->callbackTaihe_);
    CHECK_AND_RETURN_LOG(cb != nullptr, "cb is nullptr");
    cb->SaveCallbackReference(cbName, callback);
}

void AudioRendererImpl::RegisterPeriodPositionCallback(double frame, std::shared_ptr<uintptr_t> &callback,
    const std::string &cbName, AudioRendererImpl *taiheRenderer)
{
    if (frame > 0) {
        if (taiheRenderer->periodPositionCbTaihe_ == nullptr) {
            taiheRenderer->periodPositionCbTaihe_ = std::make_shared<TaiheRendererPeriodPositionCallback>(get_env());
            CHECK_AND_RETURN_RET_LOG(taiheRenderer->periodPositionCbTaihe_ != nullptr,
                TaiheAudioError::ThrowErrorAndReturn(TAIHE_ERR_NO_MEMORY),
                "periodPositionCbTaihe_ is nullptr, No memery");

            int32_t ret = taiheRenderer->audioRenderer_->SetRendererPeriodPositionCallback(frame,
                taiheRenderer->periodPositionCbTaihe_);
            CHECK_AND_RETURN_RET_LOG(ret == OHOS::AudioStandard::SUCCESS,
                TaiheAudioError::ThrowErrorAndReturn(TAIHE_ERR_SYSTEM),
                "SetRendererPeriodPositionCallback failed");

            std::shared_ptr<TaiheRendererPeriodPositionCallback> cb =
                std::static_pointer_cast<TaiheRendererPeriodPositionCallback>(taiheRenderer->periodPositionCbTaihe_);
            CHECK_AND_RETURN_LOG(cb != nullptr, "cb is nullptr");
            cb->SaveCallbackReference(cbName, callback);
        } else {
            AUDIO_DEBUG_LOG("periodReach already subscribed.");
        }
    } else {
        AUDIO_ERR_LOG("frame value not supported!!");
    }
}

void AudioRendererImpl::RegisterPositionCallback(double markPosition, std::shared_ptr<uintptr_t> &callback,
    const std::string &cbName, AudioRendererImpl *taiheRenderer)
{
    CHECK_AND_RETURN_RET_LOG(markPosition > 0, TaiheAudioError::ThrowErrorAndReturn(
        TAIHE_ERR_INPUT_INVALID, "parameter verification failed: The param of frame is not supported"),
        "Mark Position value not supported!!");
    taiheRenderer->positionCbTaihe_ = std::make_shared<TaiheRendererPositionCallback>(get_env());
    CHECK_AND_RETURN_RET_LOG(taiheRenderer->positionCbTaihe_ != nullptr, TaiheAudioError::ThrowErrorAndReturn(
        TAIHE_ERR_NO_MEMORY), "positionCbTaihe_ is nullptr");
    int32_t ret = taiheRenderer->audioRenderer_->SetRendererPositionCallback(markPosition,
        taiheRenderer->positionCbTaihe_);
    CHECK_AND_RETURN_RET_LOG(ret == OHOS::AudioStandard::SUCCESS,
        TaiheAudioError::ThrowErrorAndReturn(TAIHE_ERR_SYSTEM), "SetRendererPositionCallback fail");

    std::shared_ptr<TaiheRendererPositionCallback> cb =
        std::static_pointer_cast<TaiheRendererPositionCallback>(taiheRenderer->positionCbTaihe_);
    CHECK_AND_RETURN_LOG(cb != nullptr, "cb is nullptr");
    cb->SaveCallbackReference(cbName, callback);
}

void AudioRendererImpl::RegisterRendererDeviceChangeCallback(std::shared_ptr<uintptr_t> &callback,
    const std::string &cbName, AudioRendererImpl *taiheRenderer)
{
    if (!taiheRenderer->rendererDeviceChangeCallbackTaihe_) {
        taiheRenderer->rendererDeviceChangeCallbackTaihe_ =
            std::make_shared<TaiheAudioRendererDeviceChangeCallback>(get_env());
        CHECK_AND_RETURN_LOG(taiheRenderer->rendererDeviceChangeCallbackTaihe_ != nullptr,
            "rendererDeviceChangeCallbackTaihe_ is nullptr, No memery");

        int32_t ret = taiheRenderer->audioRenderer_->RegisterOutputDeviceChangeWithInfoCallback(
            taiheRenderer->rendererDeviceChangeCallbackTaihe_);
        CHECK_AND_RETURN_LOG(ret == OHOS::AudioStandard::SUCCESS,
            "Registering of Renderer Device Change Callback Failed");
    }

    if (!taiheRenderer->rendererPolicyServiceDiedCallbackTaihe_) {
        taiheRenderer->rendererPolicyServiceDiedCallbackTaihe_ =
            std::make_shared<TaiheAudioRendererPolicyServiceDiedCallback>(taiheRenderer);
        CHECK_AND_RETURN_LOG(taiheRenderer->rendererPolicyServiceDiedCallbackTaihe_ != nullptr,
            "Registering of Renderer Device Change Callback Failed");

        int32_t ret = taiheRenderer->audioRenderer_->RegisterAudioPolicyServerDiedCb(getpid(),
            taiheRenderer->rendererPolicyServiceDiedCallbackTaihe_);
        CHECK_AND_RETURN_LOG(ret == OHOS::AudioStandard::SUCCESS,
            "Registering of AudioPolicyService Died Change Callback Failed");
    }

    std::shared_ptr<TaiheAudioRendererDeviceChangeCallback> cb =
        std::static_pointer_cast<TaiheAudioRendererDeviceChangeCallback>(
        taiheRenderer->rendererDeviceChangeCallbackTaihe_);
    CHECK_AND_RETURN_LOG(cb != nullptr, "cb is nullptr");
    cb->SaveCallbackReference(cbName, callback);
    AUDIO_INFO_LOG("RegisterRendererStateChangeCallback is successful");
}

void AudioRendererImpl::RegisterRendererOutputDeviceChangeWithInfoCallback(std::shared_ptr<uintptr_t> &callback,
    const std::string &cbName, AudioRendererImpl *taiheRenderer)
{
    if (!taiheRenderer->rendererOutputDeviceChangeWithInfoCallbackTaihe_) {
        taiheRenderer->rendererOutputDeviceChangeWithInfoCallbackTaihe_ =
            std::make_shared<TaiheAudioRendererOutputDeviceChangeWithInfoCallback>(get_env());
        CHECK_AND_RETURN_LOG(taiheRenderer->rendererOutputDeviceChangeWithInfoCallbackTaihe_ != nullptr,
            "rendererOutputDeviceChangeWithInfoCallbackTaihe_ is nullptr, No memery");

        int32_t ret = taiheRenderer->audioRenderer_->RegisterOutputDeviceChangeWithInfoCallback(
            taiheRenderer->rendererOutputDeviceChangeWithInfoCallbackTaihe_);
        CHECK_AND_RETURN_LOG(ret == OHOS::AudioStandard::SUCCESS,
            "Registering of Renderer Device Change Callback Failed");
    }

    if (!taiheRenderer->rendererPolicyServiceDiedCallbackTaihe_) {
        taiheRenderer->rendererPolicyServiceDiedCallbackTaihe_ =
            std::make_shared<TaiheAudioRendererPolicyServiceDiedCallback>(taiheRenderer);
        CHECK_AND_RETURN_LOG(taiheRenderer->rendererPolicyServiceDiedCallbackTaihe_ != nullptr,
            "Registering of Renderer Device Change Callback Failed");

        int32_t ret = taiheRenderer->audioRenderer_->RegisterAudioPolicyServerDiedCb(getpid(),
            taiheRenderer->rendererPolicyServiceDiedCallbackTaihe_);
        CHECK_AND_RETURN_LOG(ret == OHOS::AudioStandard::SUCCESS,
            "Registering of AudioPolicyService Died Change Callback Failed");
    }

    std::shared_ptr<TaiheAudioRendererOutputDeviceChangeWithInfoCallback> cb =
        taiheRenderer->rendererOutputDeviceChangeWithInfoCallbackTaihe_;
    cb->SaveCallbackReference(cbName, callback);
    AUDIO_INFO_LOG("RegisterRendererStateChangeCallback is successful");
}

void AudioRendererImpl::UnregisterRendererCallback(std::shared_ptr<uintptr_t> &callback,
    const std::string &cbName, AudioRendererImpl *taiheRenderer)
{
    CHECK_AND_RETURN_LOG(taiheRenderer->callbackTaihe_ != nullptr, "taiheRendererCallback is nullptr");

    std::shared_ptr<TaiheAudioRendererCallback> cb =
        std::static_pointer_cast<TaiheAudioRendererCallback>(taiheRenderer->callbackTaihe_);
    UnregisterAudioRendererSingletonCallbackTemplate(callback, cbName, cb);
    AUDIO_DEBUG_LOG("UnregisterRendererCallback is successful");
}

void AudioRendererImpl::UnregisterRendererDeviceChangeCallback(std::shared_ptr<uintptr_t> &callback,
    const std::string &cbName, AudioRendererImpl *taiheRenderer)
{
    CHECK_AND_RETURN_LOG(taiheRenderer->rendererDeviceChangeCallbackTaihe_ != nullptr,
        "rendererDeviceChangeCallbackTaihe_ is nullptr, return");

    CHECK_AND_RETURN_LOG(taiheRenderer->rendererPolicyServiceDiedCallbackTaihe_ != nullptr,
        "rendererPolicyServiceDiedCallbackTaihe_ is nullptr, return");

    std::shared_ptr<TaiheAudioRendererDeviceChangeCallback> cb =
        std::static_pointer_cast<TaiheAudioRendererDeviceChangeCallback>(
            taiheRenderer->rendererDeviceChangeCallbackTaihe_);

    std::function<int32_t(std::shared_ptr<TaiheAudioRendererDeviceChangeCallback> callbackPtr,
        std::shared_ptr<uintptr_t> callbackFunction)> removeFunction =
        [&taiheRenderer] (std::shared_ptr<TaiheAudioRendererDeviceChangeCallback> callbackPtr,
            std::shared_ptr<uintptr_t> callbackFunction) {
            if (callbackFunction == nullptr || callbackPtr->GetCallbackListSize() == 0) {
                int32_t ret = taiheRenderer->audioRenderer_->UnregisterOutputDeviceChangeWithInfoCallback(callbackPtr);
                CHECK_AND_RETURN_RET_LOG(ret == OHOS::AudioStandard::SUCCESS,
                    OHOS::AudioStandard::ERR_OPERATION_FAILED,
                    "unregister renderer device change callbackFunction failed");
                ret = taiheRenderer->audioRenderer_->UnregisterAudioPolicyServerDiedCb(getpid());
                CHECK_AND_RETURN_RET_LOG(ret == OHOS::AudioStandard::SUCCESS,
                    OHOS::AudioStandard::ERR_OPERATION_FAILED, "unregister AudioPolicyServerDiedCb failed");
                taiheRenderer->DestroyTaiheCallbacks();
            }
            AUDIO_INFO_LOG("UnregisterRendererDeviceChangeCallback success");
            return OHOS::AudioStandard::SUCCESS;
        };
    UnregisterAudioRendererSingletonCallbackTemplate(callback, cbName, cb, removeFunction);
}

void AudioRendererImpl::UnregisterRendererOutputDeviceChangeWithInfoCallback(std::shared_ptr<uintptr_t> &callback,
    const std::string &cbName, AudioRendererImpl *taiheRenderer)
{
    CHECK_AND_RETURN_LOG(taiheRenderer->rendererOutputDeviceChangeWithInfoCallbackTaihe_ != nullptr,
        "rendererOutputDeviceChangeWithInfoCallbackTaihe_ is nullptr, return");

    CHECK_AND_RETURN_LOG(taiheRenderer->rendererPolicyServiceDiedCallbackTaihe_ != nullptr,
        "rendererPolicyServiceDiedCallbackTaihe_ is nullptr, return");

    std::shared_ptr<TaiheAudioRendererOutputDeviceChangeWithInfoCallback> cb =
        taiheRenderer->rendererOutputDeviceChangeWithInfoCallbackTaihe_;
    std::function<int32_t(std::shared_ptr<TaiheAudioRendererOutputDeviceChangeWithInfoCallback> callbackPtr,
        std::shared_ptr<uintptr_t> callbackFunction)> removeFunction = [&taiheRenderer] (
        std::shared_ptr<TaiheAudioRendererOutputDeviceChangeWithInfoCallback> callbackPtr,
        std::shared_ptr<uintptr_t> callbackFunction) {
            if (callbackFunction == nullptr || callbackPtr->GetCallbackListSize() == 0) {
                int32_t ret = taiheRenderer->audioRenderer_->UnregisterOutputDeviceChangeWithInfoCallback(callbackPtr);
                CHECK_AND_RETURN_RET_LOG(ret == OHOS::AudioStandard::SUCCESS,
                    OHOS::AudioStandard::ERR_OPERATION_FAILED,
                    "unregister renderer outputDevice change with info callbackFunction failed");
                ret = taiheRenderer->audioRenderer_->UnregisterAudioPolicyServerDiedCb(getpid());
                CHECK_AND_RETURN_RET_LOG(ret == OHOS::AudioStandard::SUCCESS,
                    OHOS::AudioStandard::ERR_OPERATION_FAILED, "unregister AudioPolicyServerDiedCb failed");
                taiheRenderer->DestroyTaiheCallbacks();
            }
            AUDIO_INFO_LOG("UnregisterRendererDeviceChangeCallback success");
            return OHOS::AudioStandard::SUCCESS;
        };
    UnregisterAudioRendererSingletonCallbackTemplate(callback, cbName, cb, removeFunction);
}

void AudioRendererImpl::UnregisterPeriodPositionCallback(std::shared_ptr<uintptr_t> &callback,
    const std::string &cbName, AudioRendererImpl *taiheRenderer)
{
    CHECK_AND_RETURN_LOG(taiheRenderer->periodPositionCbTaihe_ != nullptr, "periodPositionCbTaihe is nullptr");

    std::shared_ptr<TaiheRendererPeriodPositionCallback> cb =
        std::static_pointer_cast<TaiheRendererPeriodPositionCallback>(taiheRenderer->periodPositionCbTaihe_);
    std::function<int32_t(std::shared_ptr<TaiheRendererPeriodPositionCallback> callbackPtr,
        std::shared_ptr<uintptr_t> callbackFunction)> removeFunction =
        [&taiheRenderer] (std::shared_ptr<TaiheRendererPeriodPositionCallback> callbackPtr,
        std::shared_ptr<uintptr_t> callbackFunction) {
            taiheRenderer->audioRenderer_->UnsetRendererPeriodPositionCallback();
            taiheRenderer->periodPositionCbTaihe_ = nullptr;
            return OHOS::AudioStandard::SUCCESS;
        };
    UnregisterAudioRendererSingletonCallbackTemplate(callback, cbName, cb, removeFunction);
    AUDIO_DEBUG_LOG("UnregisterRendererPeriodPositionCallback is successful");
}

void AudioRendererImpl::UnregisterPositionCallback(std::shared_ptr<uintptr_t> &callback,
    const std::string &cbName, AudioRendererImpl *taiheRenderer)
{
    CHECK_AND_RETURN_LOG(taiheRenderer->positionCbTaihe_ != nullptr, "rendererCallbackTaihe is nullptr");

    std::shared_ptr<TaiheRendererPositionCallback> cb =
        std::static_pointer_cast<TaiheRendererPositionCallback>(taiheRenderer->positionCbTaihe_);
    std::function<int32_t(std::shared_ptr<TaiheRendererPositionCallback> callbackPtr,
        std::shared_ptr<uintptr_t> callbackFunction)> removeFunction =
        [&taiheRenderer] (std::shared_ptr<TaiheRendererPositionCallback> callbackPtr,
        std::shared_ptr<uintptr_t> callbackFunction) {
            taiheRenderer->audioRenderer_->UnsetRendererPositionCallback();
            taiheRenderer->positionCbTaihe_ = nullptr;
            return OHOS::AudioStandard::SUCCESS;
        };
    UnregisterAudioRendererSingletonCallbackTemplate(callback, cbName, cb, removeFunction);
    AUDIO_DEBUG_LOG("UnregisterRendererPositionCallback is successful");
}

void AudioRendererImpl::OnStateChange(callback_view<void(AudioState)> callback)
{
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, TaiheAudioError::ThrowErrorAndReturn(
        TAIHE_ERROR_INVALID_PARAM), "audioRenderer_ is nullptr");
    auto cacheCallback = TaiheParamUtils::TypeCallback(callback);
    RegisterRendererCallback(cacheCallback, STATE_CHANGE_CALLBACK_NAME, this);
}

void AudioRendererImpl::OnAudioInterrupt(callback_view<void(InterruptEvent const&)> callback)
{
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, TaiheAudioError::ThrowErrorAndReturn(
        TAIHE_ERROR_INVALID_PARAM), "audioRenderer_ is nullptr");
    auto cacheCallback = TaiheParamUtils::TypeCallback(callback);
    RegisterRendererCallback(cacheCallback, AUDIO_INTERRUPT_CALLBACK_NAME, this);
}

void AudioRendererImpl::OnOutputDeviceChange(callback_view<void(array_view<AudioDeviceDescriptor>)> callback)
{
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, TaiheAudioError::ThrowErrorAndReturn(
        TAIHE_ERROR_INVALID_PARAM), "audioRenderer_ is nullptr");
    auto cacheCallback = TaiheParamUtils::TypeCallback(callback);
    RegisterRendererDeviceChangeCallback(cacheCallback, DEVICECHANGE_CALLBACK_NAME, this);
}

void AudioRendererImpl::OnOutputDeviceChangeWithInfo(callback_view<void(AudioStreamDeviceChangeInfo const&)> callback)
{
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, TaiheAudioError::ThrowErrorAndReturn(
        TAIHE_ERROR_INVALID_PARAM), "audioRenderer_ is nullptr");
    auto cacheCallback = TaiheParamUtils::TypeCallback(callback);
    RegisterRendererOutputDeviceChangeWithInfoCallback(cacheCallback, OUTPUT_DEVICECHANGE_WITH_INFO, this);
}

void AudioRendererImpl::OnPeriodReach(double frame, callback_view<void(double)> callback)
{
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, TaiheAudioError::ThrowErrorAndReturn(
        TAIHE_ERROR_INVALID_PARAM), "audioRenderer_ is nullptr");
    auto cacheCallback = TaiheParamUtils::TypeCallback(callback);
    RegisterPeriodPositionCallback(frame, cacheCallback, AUDIO_INTERRUPT_CALLBACK_NAME, this);
}

void AudioRendererImpl::OnMarkReach(double frame, callback_view<void(double)> callback)
{
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, TaiheAudioError::ThrowErrorAndReturn(
        TAIHE_ERROR_INVALID_PARAM), "audioRenderer_ is nullptr");
    auto cacheCallback = TaiheParamUtils::TypeCallback(callback);
    RegisterPositionCallback(frame, cacheCallback, MARK_REACH_CALLBACK_NAME, this);
}

void AudioRendererImpl::OffAudioInterrupt(optional_view<callback<void(InterruptEvent const&)>> callback)
{
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, TaiheAudioError::ThrowErrorAndReturn(
        TAIHE_ERROR_INVALID_PARAM), "audioRenderer_ is nullptr");
    std::shared_ptr<uintptr_t> cacheCallback;
    if (callback.has_value()) {
        cacheCallback = TaiheParamUtils::TypeCallback(callback.value());
    }
    UnregisterRendererCallback(cacheCallback, AUDIO_INTERRUPT_CALLBACK_NAME, this);
}

void AudioRendererImpl::OffStateChange(optional_view<callback<void(AudioState)>> callback)
{
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, TaiheAudioError::ThrowErrorAndReturn(
        TAIHE_ERROR_INVALID_PARAM), "audioRenderer_ is nullptr");
    std::shared_ptr<uintptr_t> cacheCallback;
    if (callback.has_value()) {
        cacheCallback = TaiheParamUtils::TypeCallback(callback.value());
    }
    UnregisterRendererCallback(cacheCallback, STATE_CHANGE_CALLBACK_NAME, this);
}

void AudioRendererImpl::OffOutputDeviceChange(optional_view<callback<void(array_view<AudioDeviceDescriptor>)>> callback)
{
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, TaiheAudioError::ThrowErrorAndReturn(
        TAIHE_ERROR_INVALID_PARAM), "audioRenderer_ is nullptr");
    std::shared_ptr<uintptr_t> cacheCallback;
    if (callback.has_value()) {
        cacheCallback = TaiheParamUtils::TypeCallback(callback.value());
    }
    UnregisterRendererDeviceChangeCallback(cacheCallback, DEVICECHANGE_CALLBACK_NAME, this);
}

void AudioRendererImpl::OffOutputDeviceChangeWithInfo(
    optional_view<callback<void(AudioStreamDeviceChangeInfo const&)>> callback)
{
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, TaiheAudioError::ThrowErrorAndReturn(
        TAIHE_ERROR_INVALID_PARAM), "audioRenderer_ is nullptr");
    std::shared_ptr<uintptr_t> cacheCallback;
    if (callback.has_value()) {
        cacheCallback = TaiheParamUtils::TypeCallback(callback.value());
    }
    UnregisterRendererOutputDeviceChangeWithInfoCallback(cacheCallback, OUTPUT_DEVICECHANGE_WITH_INFO, this);
}

void AudioRendererImpl::OffPeriodReach(optional_view<callback<void(double)>> callback)
{
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, TaiheAudioError::ThrowErrorAndReturn(
        TAIHE_ERROR_INVALID_PARAM), "audioRenderer_ is nullptr");
    std::shared_ptr<uintptr_t> cacheCallback;
    if (callback.has_value()) {
        cacheCallback = TaiheParamUtils::TypeCallback(callback.value());
    }
    UnregisterPeriodPositionCallback(cacheCallback, PERIOD_REACH_CALLBACK_NAME, this);
}

void AudioRendererImpl::OffMarkReach(optional_view<callback<void(double)>> callback)
{
    CHECK_AND_RETURN_RET_LOG(audioRenderer_ != nullptr, TaiheAudioError::ThrowErrorAndReturn(
        TAIHE_ERROR_INVALID_PARAM), "audioRenderer_ is nullptr");
    std::shared_ptr<uintptr_t> cacheCallback;
    if (callback.has_value()) {
        cacheCallback = TaiheParamUtils::TypeCallback(callback.value());
    }
    UnregisterPositionCallback(cacheCallback, MARK_REACH_CALLBACK_NAME, this);
}

void AudioRendererImpl::DestroyCallbacks()
{
    CHECK_AND_RETURN_LOG(rendererDeviceChangeCallbackTaihe_ != nullptr,
        "rendererDeviceChangeCallbackTaihe_ is nullptr");
    rendererDeviceChangeCallbackTaihe_->RemoveAllCallbacks();
    DestroyTaiheCallbacks();
}

void AudioRendererImpl::DestroyTaiheCallbacks()
{
    if (rendererDeviceChangeCallbackTaihe_ != nullptr) {
        rendererDeviceChangeCallbackTaihe_.reset();
        rendererDeviceChangeCallbackTaihe_ = nullptr;
    }

    if (rendererPolicyServiceDiedCallbackTaihe_ != nullptr) {
        rendererPolicyServiceDiedCallbackTaihe_.reset();
        rendererPolicyServiceDiedCallbackTaihe_ = nullptr;
    }
}

AudioRenderer CreateAudioRendererSync(AudioRendererOptions const& options)
{
    OHOS::AudioStandard::AudioRendererOptions rendererOptions;
    if (TaiheParamUtils::GetRendererOptions(&rendererOptions, options) != AUDIO_OK) {
        TaiheAudioError::ThrowErrorAndReturn(TAIHE_ERR_INPUT_INVALID,
            "parameter verification failed: The param of options must be interface AudioRendererOptions");
        AUDIO_ERR_LOG("get rendererOptions failed");
        return make_holder<AudioRendererImpl, AudioRenderer>(nullptr);
    }
    return AudioRendererImpl::CreateAudioRendererWrapper(rendererOptions);
}

} // namespace ANI::Audio

TH_EXPORT_CPP_API_CreateAudioRendererSync(CreateAudioRendererSync);
