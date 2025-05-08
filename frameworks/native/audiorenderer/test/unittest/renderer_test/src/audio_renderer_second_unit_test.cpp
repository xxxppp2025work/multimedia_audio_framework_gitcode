/*
 * Copyright (c) 2021-2025 Huawei Device Co., Ltd.
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

#include "audio_renderer_unit_test.h"
#include <chrono>
#include <thread>

#include "audio_errors.h"
#include "audio_info.h"
#include "audio_renderer.h"
#include "audio_renderer_proxy_obj.h"
#include "audio_policy_manager.h"
#include "audio_renderer_private.h"
#include "audio_renderer.cpp"
#include "fast_audio_stream.h"

using namespace std;
using namespace std::chrono;
using namespace testing::ext;
using namespace testing;

namespace OHOS {
namespace AudioStandard {

void AudioRendererUnitTest::SetUpTestCase(void) {}
void AudioRendererUnitTest::TearDownTestCase(void) {}
void AudioRendererUnitTest::SetUp(void) {}
void AudioRendererUnitTest::TearDown(void) {}

/**
 * @tc.name  : Test GetStartStreamResult API.
 * @tc.number: Audio_Renderer_GetStartStreamResult_001
 * @tc.desc  : Test GetStartStreamResult interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetStartStreamResult_001, TestSize.Level2)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    ASSERT_TRUE(audioRendererPrivate != nullptr);

    int32_t ret = audioRendererPrivate->GetStartStreamResult(StateChangeCmdType::CMD_FROM_CLIENT);
    EXPECT_FALSE(ret);
}

/**
 * @tc.name  : Test OnInterrupt API.
 * @tc.number: Audio_Renderer_OnInterrupt_001
 * @tc.desc  : Test OnInterrupt interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_OnInterrupt_001, TestSize.Level2)
{
    AudioStreamParams audioStreamParams;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::FAST_STREAM,
        audioStreamParams, STREAM_DEFAULT, 1);
    AudioInterrupt audioInterrupt;
    auto audioInterruptCallback = std::make_shared<AudioRendererInterruptCallbackImpl>(audioStream, audioInterrupt);
    ASSERT_TRUE(audioInterruptCallback != nullptr);

    audioInterruptCallback->switching_ = true;
    InterruptEventInternal interruptEvent;
    audioInterruptCallback->OnInterrupt(interruptEvent);
}

/**
 * @tc.name  : Test InitTargetStream API.
 * @tc.number: Audio_Renderer_InitTargetStream_001
 * @tc.desc  : Test InitTargetStream interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_InitTargetStream_001, TestSize.Level2)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    ASSERT_TRUE(audioRendererPrivate != nullptr);

    IAudioStream::SwitchInfo info;
    info.rendererInfo.originalFlag = AUDIO_FLAG_INVALID;
    std::shared_ptr<IAudioStream> newAudioStream = nullptr;
    auto ret = audioRendererPrivate->InitTargetStream(info, newAudioStream);
    EXPECT_FALSE(ret);
}

/**
 * @tc.name  : Test InitTargetStream API.
 * @tc.number: Audio_Renderer_InitTargetStream_002
 * @tc.desc  : Test InitTargetStream interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_InitTargetStream_002, TestSize.Level2)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    ASSERT_TRUE(audioRendererPrivate != nullptr);

    IAudioStream::SwitchInfo info;
    info.rendererInfo.originalFlag = AUDIO_FLAG_NORMAL;
    std::shared_ptr<IAudioStream> newAudioStream = nullptr;
    auto ret = audioRendererPrivate->InitTargetStream(info, newAudioStream);
    EXPECT_FALSE(ret);
}

/**
 * @tc.name  : Test FinishOldStream API.
 * @tc.number: Audio_Renderer_FinishOldStream_001
 * @tc.desc  : Test FinishOldStream interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_FinishOldStream_001, TestSize.Level2)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    ASSERT_TRUE(audioRendererPrivate != nullptr);

    std::shared_ptr<FastAudioStream> audioStream = std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK,
        appInfo.appUid);
    ASSERT_TRUE(audioStream != nullptr);
    audioRendererPrivate->audioStream_ = audioStream;

    RestoreInfo restoreInfo;
    RendererState previousState = RENDERER_RUNNING;
    IAudioStream::SwitchInfo switchInfo;
    restoreInfo.restoreReason = SERVER_DIED;
    auto ret = audioRendererPrivate->FinishOldStream(IAudioStream::StreamClass::PA_STREAM, restoreInfo,
        previousState, switchInfo);
    EXPECT_TRUE(ret);
}

/**
 * @tc.name  : Test FinishOldStream API.
 * @tc.number: Audio_Renderer_FinishOldStream_002
 * @tc.desc  : Test FinishOldStream interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_FinishOldStream_002, TestSize.Level2)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    ASSERT_TRUE(audioRendererPrivate != nullptr);

    std::shared_ptr<FastAudioStream> audioStream = std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK,
        appInfo.appUid);
    ASSERT_TRUE(audioStream != nullptr);
    audioRendererPrivate->audioStream_ = audioStream;

    RestoreInfo restoreInfo;
    RendererState previousState = RENDERER_RUNNING;
    IAudioStream::SwitchInfo switchInfo;
    restoreInfo.restoreReason = DEFAULT_REASON;
    auto ret = audioRendererPrivate->FinishOldStream(IAudioStream::StreamClass::PA_STREAM, restoreInfo,
        previousState, switchInfo);
    EXPECT_FALSE(ret);
}

/**
 * @tc.name  : Test ContinueAfterConcede API.
 * @tc.number: Audio_Renderer_ContinueAfterConcede_001
 * @tc.desc  : Test ContinueAfterConcede interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_ContinueAfterConcede_001, TestSize.Level2)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    ASSERT_TRUE(audioRendererPrivate != nullptr);

    AudioStreamParams tempParams = {};
    AudioStreamType audioStreamType = STREAM_MUSIC;
    auto audioStream = IAudioStream::GetPlaybackStream(IAudioStream::PA_STREAM, tempParams, audioStreamType,
        appInfo.appUid);
    ASSERT_TRUE(audioStream != nullptr);
    audioRendererPrivate->audioStream_ = audioStream;

    RestoreInfo restoreInfo;
    IAudioStream::StreamClass targetClass = IAudioStream::StreamClass::PA_STREAM;
    restoreInfo.restoreReason = STREAM_CONCEDED;
    auto ret = audioRendererPrivate->ContinueAfterConcede(targetClass, restoreInfo);
    EXPECT_FALSE(ret);
}

/**
 * @tc.name  : Test HandleAudioInterruptWhenServerDied API.
 * @tc.number: Audio_Renderer_HandleAudioInterruptWhenServerDied_001
 * @tc.desc  : Test HandleAudioInterruptWhenServerDied interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_HandleAudioInterruptWhenServerDied_001, TestSize.Level2)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    ASSERT_TRUE(audioRendererPrivate != nullptr);

    audioRendererPrivate->state_ = RENDERER_STOPPED;
    audioRendererPrivate->rendererInfo_.streamUsage = STREAM_USAGE_VOICE_MODEM_COMMUNICATION;
    audioRendererPrivate->HandleAudioInterruptWhenServerDied();

    audioRendererPrivate->state_ = RENDERER_RUNNING;
    audioRendererPrivate->HandleAudioInterruptWhenServerDied();
}
} // namespace AudioStandard
} // namespace OHOS