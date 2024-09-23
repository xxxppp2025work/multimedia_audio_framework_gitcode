/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "audio_errors.h"
#include "audio_service_log.h"
#include "audio_info.h"
#include "audio_ring_cache.h"
#include "audio_process_config.h"
#include "linear_pos_time_model.h"
#include "oh_audio_buffer.h"
#include <gtest/gtest.h>
#include "pa_renderer_stream_impl.h"
#include "policy_handler.h"
#include "pa_adapter_manager.h"
#include "audio_capturer_private.h"
#include "audio_system_manager.h"

using namespace testing::ext;
namespace OHOS {
namespace AudioStandard {
    const int32_t CAPTURER_FLAG = 0;
class PaRendererStreamUnitTest : public ::testing::Test {
public:
    void SetUp();
    void TearDown();
    std::shared_ptr<PaRendererStreamImpl> CreatePaRendererStreamImpl(pa_stream *paStream);
};
void PaRendererStreamUnitTest::SetUp(void)
{
    // input testcase setup step，setup invoked before each testcases
}

void PaRendererStreamUnitTest::TearDown(void)
{
    // input testcase teardown step，teardown invoked after each testcases
}

AudioProcessConfig GetInnerCapConfig()
{
    AudioProcessConfig processConfig;

    processConfig.appInfo.appPid = static_cast<int32_t>(getpid());
    processConfig.appInfo.appUid = static_cast<int32_t>(getuid());

    AudioCapturerOptions capturerOptions;
    capturerOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_96000;
    capturerOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    capturerOptions.streamInfo.format = AudioSampleFormat::SAMPLE_U8;
    capturerOptions.streamInfo.channels = AudioChannel::MONO;
    capturerOptions.capturerInfo.sourceType = SourceType::SOURCE_TYPE_MIC;
    capturerOptions.capturerInfo.capturerFlags = CAPTURER_FLAG;
    std::unique_ptr<AudioCapturer> audioCapturer = AudioCapturer::Create(capturerOptions);
    AudioStreamInfo streamInfo;
    audioCapturer->GetStreamInfo(streamInfo);
    processConfig.streamInfo = streamInfo;
    processConfig.audioMode = AUDIO_MODE_PLAYBACK;
    processConfig.streamType = STREAM_MUSIC;

    return processConfig;
}

std::shared_ptr<PaRendererStreamImpl> PaRendererStreamUnitTest::CreatePaRendererStreamImpl(pa_stream *paStream)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *stream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(stream,
        processConfig, mainLoop);
    unit->paStream_ = paStream; // Set the stream pointer to the provided value
    return unit;
}

/**
 * @tc.name  : Test AudioProcessProxy API
 * @tc.type  : FUNC
 * @tc.number: AudioProcessProxy_001
 * @tc.desc  : Test AudioProcessProxy interface.
 */
HWTEST_F(PaRendererStreamUnitTest, GetCurrentTimeStamp_001, TestSize.Level1)
{
    // Create a PaRendererStreamImpl instance with a null paStream_
    auto unit = CreatePaRendererStreamImpl(nullptr); // Pass nullptr to simulate an invalid stream
    uint64_t timestamp = 0;
    int32_t ret = unit->GetCurrentTimeStamp(timestamp);
    // Verify the return value
    EXPECT_EQ(ret, ERR_ILLEGAL_STATE);
}

/**
 * @tc.name  : Test GetCurrentTimeStamp
 * @tc.type  : FUNC
 * @tc.number: AudioProcessProxy_001
 * @tc.desc  : Test GetCurrentTimeStamp.
 */
HWTEST_F(PaRendererStreamUnitTest, GetCurrentTimeStamp_002, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    unit->paStream_ = paStream;
    uint64_t timestamp = 0;
    int32_t ret = unit->GetCurrentTimeStamp(timestamp);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test GetEffectModeName
 * @tc.type  : FUNC
 * @tc.number: AudioProcessProxy_001
 * @tc.desc  : Test GetEffectModeName.
 */
HWTEST_F(PaRendererStreamUnitTest, GetEffectModeName_003, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    EXPECT_EQ("EFFECT_NONE", unit->GetEffectModeName(0));
}

/**
 * @tc.name  : Test GetEffectModeName
 * @tc.type  : FUNC
 * @tc.number: AudioProcessProxy_001
 * @tc.desc  : Test GetEffectModeName.
 */
HWTEST_F(PaRendererStreamUnitTest, GetEffectModeName_004, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    EXPECT_EQ("EFFECT_DEFAULT", unit->GetEffectModeName(0));
}

/**
 * @tc.name  : Test AudioProcessProxy API
 * @tc.type  : FUNC
 * @tc.number: AudioProcessProxy_001
 * @tc.desc  : Test AudioProcessProxy interface.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_006, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    int32_t rate = RENDER_RATE_NORMAL;
    EXPECT_TRUE(unit->SetRate(rate));
}

/**
 * @tc.name  : Test AudioProcessProxy API
 * @tc.type  : FUNC
 * @tc.number: AudioProcessProxy_001
 * @tc.desc  : Test AudioProcessProxy interface.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_007, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    int32_t rate = RENDER_RATE_DOUBLE;
    EXPECT_TRUE(unit->SetRate(rate));
}


/**
 * @tc.name  : Test AudioProcessProxy API
 * @tc.type  : FUNC
 * @tc.number: AudioProcessProxy_001
 * @tc.desc  : Test AudioProcessProxy interface.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_008, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    int32_t rate = RENDER_RATE_NORMAL;
    EXPECT_TRUE(unit->SetRate(rate));
}

/**
 * @tc.name  : Test AudioProcessProxy API
 * @tc.type  : FUNC
 * @tc.number: AudioProcessProxy_001
 * @tc.desc  : Test AudioProcessProxy interface.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_009, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    int32_t rate = 999;
    EXPECT_EQ(unit->SetRate(rate), ERR_INVALID_PARAM);
}

/**
 * @tc.name  : Test GetCurrentPosition
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_010
 * @tc.desc  : Test GetCurrentPosition.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_010, TestSize.Level1)
{
    auto unit = CreatePaRendererStreamImpl(nullptr);
    uint64_t timestamp = 0;
    uint64_t framePosition = 0;
    int32_t ret = unit->GetCurrentPosition(framePosition, timestamp);
    // Verify the return value
    EXPECT_EQ(ret, ERR_ILLEGAL_STATE);
}

/**
 * @tc.name  : Test GetCurrentPosition.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_011
 * @tc.desc  : Test GetCurrentPosition.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_011, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    unit->paStream_ = paStream;
    unit->firstGetLatency_ = true;
    EXPECT_FALSE(unit->firstGetLatency_);
}

/**
 * @tc.name  : Test OffloadSetVolume.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_013
 * @tc.desc  : Test OffloadSetVolume.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_013, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    unit->offloadEnable_ = false;
    float volume = 0.0f;
    auto ret = unit->OffloadSetVolume(volume);
    EXPECT_EQ(ret, ERR_OPERATION_FAILED);
}

/**
 * @tc.name  : Test OffloadSetVolume.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_014
 * @tc.desc  : Test OffloadSetVolume.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_014, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    unit->offloadEnable_ = true;
    float volume = 0.0f;
    auto ret = unit->OffloadSetVolume(volume);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test UpdateSpatializationState.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_015
 * @tc.desc  : Test UpdateSpatializationState.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_015, TestSize.Level1)
{
    auto unit = CreatePaRendererStreamImpl(nullptr);
    bool spatializationEnabled = false;
    bool headTrackingEnabled = false;
    int32_t ret = unit->UpdateSpatializationState(spatializationEnabled, headTrackingEnabled);
    EXPECT_EQ(ret, ERR_ILLEGAL_STATE);
}

/**
 * @tc.name  : Test UpdateSpatializationState.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_016
 * @tc.desc  : Test UpdateSpatializationState.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_016, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    unit->paStream_ = paStream;
    bool spatializationEnabled = false;
    bool headTrackingEnabled = false;
    int32_t ret = unit->UpdateSpatializationState(spatializationEnabled, headTrackingEnabled);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test GetOffloadApproximatelyCacheTime.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_017
 * @tc.desc  : Test GetOffloadApproximatelyCacheTime.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_017, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    unit->offloadEnable_ = false;
    uint64_t timestamp = 0;
    uint64_t paWriteIndex = 0;
    uint64_t cacheTimeDsp = 0;
    uint64_t cacheTimePa = 0;
    int32_t result = unit->GetOffloadApproximatelyCacheTime(timestamp, paWriteIndex, cacheTimeDsp, cacheTimePa);
    EXPECT_EQ(result, ERR_OPERATION_FAILED);
    EXPECT_EQ(timestamp, 0);
    EXPECT_EQ(paWriteIndex, 0);
    EXPECT_EQ(cacheTimeDsp, 0);
    EXPECT_EQ(cacheTimePa, 0);
}

/**
 * @tc.name  : Test GetOffloadApproximatelyCacheTime.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_018
 * @tc.desc  : Test GetOffloadApproximatelyCacheTime.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_018, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    unit->offloadEnable_ = true;
    unit->paStream_ = nullptr;
    uint64_t timestamp = 0;
    uint64_t paWriteIndex = 0;
    uint64_t cacheTimeDsp = 0;
    uint64_t cacheTimePa = 0;
    int32_t ret = unit->GetOffloadApproximatelyCacheTime(timestamp, paWriteIndex, cacheTimeDsp, cacheTimePa);
    EXPECT_EQ(ret, ERR_OPERATION_FAILED);
    EXPECT_EQ(timestamp, 0);
    EXPECT_EQ(paWriteIndex, 0);
    EXPECT_EQ(cacheTimeDsp, 0);
    EXPECT_EQ(cacheTimePa, 0);
}

/**
 * @tc.name  : Test GetOffloadApproximatelyCacheTime.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_019
 * @tc.desc  : Test GetOffloadApproximatelyCacheTime.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_019, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    unit->offloadEnable_ = true;
    unit->paStream_ = paStream;
    uint64_t timestamp = 0;
    uint64_t paWriteIndex = 0;
    uint64_t cacheTimeDsp = 0;
    uint64_t cacheTimePa = 0;
    int32_t ret = unit->GetOffloadApproximatelyCacheTime(timestamp, paWriteIndex, cacheTimeDsp, cacheTimePa);
    EXPECT_EQ(ret,  SUCCESS);
}

/**
 * @tc.name  : Test OffloadUpdatePolicyInWrite.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_020
 * @tc.desc  : Test OffloadUpdatePolicyInWrite.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_020, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    unit->offloadEnable_ = true;
    unit->paStream_ = paStream;
    unit->lastOffloadUpdateFinishTime_ = 1;
    int32_t ret = unit->OffloadUpdatePolicyInWrite();
    EXPECT_EQ(ret, -1);
}

/**
 * @tc.name  : Test OffloadUpdatePolicyInWrite.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_021
 * @tc.desc  : Test OffloadUpdatePolicyInWrite.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_021, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    unit->offloadEnable_ = true;
    unit->paStream_ = paStream;
    unit->lastOffloadUpdateFinishTime_ = 0;
    int32_t ret = unit->OffloadUpdatePolicyInWrite();
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name  : Test OffloadUpdatePolicy.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_022
 * @tc.desc  : Test OffloadUpdatePolicy.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_022, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    AudioOffloadType statePolicy = OFFLOAD_ACTIVE_FOREGROUND;
    bool force = true;
    unit->offloadStatePolicy_ = OFFLOAD_ACTIVE_FOREGROUND;
    unit->lastOffloadUpdateFinishTime_ = 1;
    unit->OffloadUpdatePolicy(statePolicy, force);
    EXPECT_EQ(0, unit->lastOffloadUpdateFinishTime_);
}

/**
 * @tc.name  : Test OffloadUpdatePolicy.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_023
 * @tc.desc  : Test OffloadUpdatePolicy.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_023, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    AudioOffloadType statePolicy = OFFLOAD_ACTIVE_FOREGROUND;
    bool force = false;
    unit->offloadStatePolicy_ = OFFLOAD_DEFAULT;
    unit->lastOffloadUpdateFinishTime_ = 1;
    unit->OffloadUpdatePolicy(statePolicy, force);
    EXPECT_EQ(0, unit->lastOffloadUpdateFinishTime_);
}

/**
 * @tc.name  : Test OffloadUpdatePolicy.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_024
 * @tc.desc  : Test OffloadUpdatePolicy.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_024, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    AudioOffloadType statePolicy = OFFLOAD_DEFAULT;
    bool force = false;
    unit->offloadStatePolicy_ = OFFLOAD_ACTIVE_FOREGROUND;
    unit->lastOffloadUpdateFinishTime_ = 1;
    unit->OffloadUpdatePolicy(statePolicy, force);
    EXPECT_EQ(0, unit->lastOffloadUpdateFinishTime_);
}

/**
 * @tc.name  : Test OffloadUpdatePolicy.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_025
 * @tc.desc  : Test OffloadUpdatePolicy.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_025, TestSize.Level1)
{
    auto unit = CreatePaRendererStreamImpl(nullptr);
    AudioOffloadType statePolicy = OFFLOAD_ACTIVE_FOREGROUND;
    bool force = true;
    unit->offloadStatePolicy_ = OFFLOAD_ACTIVE_FOREGROUND;
    int32_t ret = unit->OffloadUpdatePolicy(statePolicy, force);
    EXPECT_EQ(ret, ERR_ILLEGAL_STATE);
}

/**
 * @tc.name  : Test SetClientVolume.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_026
 * @tc.desc  : Test SetClientVolume.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_026, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    float clientVolume = -1;
    int32_t ret = unit->SetClientVolume(clientVolume);
    EXPECT_EQ(ret, ERR_INVALID_PARAM);
}

/**
 * @tc.name  : Test SetClientVolume.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_027
 * @tc.desc  : Test SetClientVolume.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_027, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    float clientVolume = 1.5;
    int32_t ret = unit->SetClientVolume(clientVolume);
    EXPECT_EQ(ret, ERR_INVALID_PARAM);
}

/**
 * @tc.name  : Test SetClientVolume.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_028
 * @tc.desc  : Test SetClientVolume.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_028, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    float clientVolume = 0.5;
    int32_t ret = unit->SetClientVolume(clientVolume);
    EXPECT_EQ(ret, ERR_INVALID_PARAM);
}

/**
 * @tc.name  : Test GetWritableSize.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_029
 * @tc.desc  : Test GetWritableSize.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_029, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    unit->paStream_ = nullptr;
    int32_t ret = unit->GetWritableSize();
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name  : Test GetWritableSize.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_030
 * @tc.desc  : Test GetWritableSize.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_030, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    unit->paStream_ = paStream;
    int32_t ret = unit->GetWritableSize();
    EXPECT_EQ(ret, pa_stream_writable_size(unit->paStream_));
}

/**
 * @tc.name  : Test EnqueueBuffer.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_031
 * @tc.desc  : Test EnqueueBuffer.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_031, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    BufferDesc bufferDesc = {
        .buffer = nullptr,
        .bufLength = 0,
        .dataLength = 0,
        .metaBuffer = nullptr,
        .metaLength = 0
    };
    unit->paStream_ = nullptr;
    int32_t ret = unit->EnqueueBuffer(bufferDesc);
    EXPECT_EQ(ret, ERR_ILLEGAL_STATE);
}

/**
 * @tc.name  : Test EnqueueBuffer.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_032
 * @tc.desc  : Test EnqueueBuffer.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_032, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    BufferDesc bufferDesc = {
        .buffer = nullptr,
        .bufLength = 0,
        .dataLength = 0,
        .metaBuffer = nullptr,
        .metaLength = 0
    };
    unit->paStream_ = paStream;
    int32_t ret = unit->EnqueueBuffer(bufferDesc);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test SetAudioEffectMode.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_033
 * @tc.desc  : Test SetAudioEffectMode.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_033, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    int32_t effectMode = 0;
    unit->paStream_ = nullptr;
    int32_t ret = unit->SetAudioEffectMode(effectMode);
    EXPECT_EQ(ret, ERR_ILLEGAL_STATE);
}

/**
 * @tc.name  : Test SetAudioEffectMode.
 * @tc.type  : FUNC
 * @tc.number: PaRenderer_034
 * @tc.desc  : Test SetAudioEffectMode.
 */
HWTEST_F(PaRendererStreamUnitTest, PaRenderer_034, TestSize.Level1)
{
    void *userdata = nullptr;
    pa_threaded_mainloop *mainLoop = reinterpret_cast<pa_threaded_mainloop *>(userdata);
    PaAdapterManager adapterManager = PaAdapterManager(DUP_PLAYBACK);
    AudioProcessConfig processConfig = GetInnerCapConfig();
    uint32_t sessionId = PolicyHandler::GetInstance().GenerateSessionId(processConfig.appInfo.appUid);
    pa_stream *paStream = adapterManager.InitPaStream(processConfig, sessionId, false);
    std::shared_ptr<PaRendererStreamImpl> unit = std::make_shared<PaRendererStreamImpl>(paStream,
        processConfig, mainLoop);
    int32_t effectMode = 0;
    unit->paStream_ = paStream;
    int32_t ret = unit->SetAudioEffectMode(effectMode);
    EXPECT_EQ(ret, SUCCESS);
}
}
}