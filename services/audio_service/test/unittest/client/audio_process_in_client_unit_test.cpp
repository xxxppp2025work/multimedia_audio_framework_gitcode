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
#define private public
#define protected public

#include <gtest/gtest.h>

#include "audio_service_log.h"
#include "audio_errors.h"
#include "audio_process_in_client.h"
#include "i_audio_process.h"
#include "audio_process_in_client.cpp"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

class AudioProcessInClientUnitTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

/**
 * @tc.name  : Test SetPreferredFrameSize API
 * @tc.type  : FUNC
 * @tc.number: SetPreferredFrameSize_001
 * @tc.desc  : Test SetPreferredFrameSize interface.
 */
HWTEST(AudioProcessInClientUnitTest, SetPreferredFrameSize_001, TestSize.Level1)
{
    sptr<IAudioProcess> ipcProxy = nullptr;
    bool ipViopMmap = true;
    std::shared_ptr<AudioProcessInClientInner> audioProcessInClientInner =
        std::make_shared<AudioProcessInClientInner>(ipcProxy, ipViopMmap);
    bool ret = audioProcessInClientInner->Start();
    EXPECT_EQ(ret, SUCCESS);
    int32_t frameSize = 5;
    audioProcessInClientInner->spanSizeInFrame_ = 10;
    audioProcessInClientInner->SetPreferredFrameSize(frameSize);
    frameSize = 1000;
    audioProcessInClientInner->SetPreferredFrameSize(frameSize);
    frameSize = 10;
    audioProcessInClientInner->SetPreferredFrameSize(frameSize);
    frameSize = 30;
    audioProcessInClientInner->SetPreferredFrameSize(frameSize);
    audioProcessInClientInner->spanSizeInFrame_ = 0;
    audioProcessInClientInner->SetPreferredFrameSize(frameSize);
}

/**
 * @tc.name  : Test UpdateLatencyTimestamp API
 * @tc.type  : FUNC
 * @tc.number: UpdateLatencyTimestamp_001
 * @tc.desc  : Test UpdateLatencyTimestamp interface.
 */
HWTEST(AudioProcessInClientUnitTest, UpdateLatencyTimestamp_001, TestSize.Level1)
{
    sptr<IAudioProcess> ipcProxy = nullptr;
    bool ipViopMmap = true;
    std::shared_ptr<AudioProcessInClientInner> audioProcessInClientInner =
        std::make_shared<AudioProcessInClientInner>(ipcProxy, ipViopMmap);
    bool ret = audioProcessInClientInner->Start();
    EXPECT_EQ(ret, SUCCESS);
    std::string timestamp = "20240913";
    bool isRenderer = true;
    audioProcessInClientInner->UpdateLatencyTimestamp(timestamp, isRenderer);
    ret = audioProcessInClientInner->Stop();
    EXPECT_EQ(ret, SUCCESS);
    audioProcessInClientInner->UpdateLatencyTimestamp(timestamp, isRenderer);
}

/**
 * @tc.name  : Test GetFormatSize API
 * @tc.type  : FUNC
 * @tc.number: GetFormatSize_001
 * @tc.desc  : Test GetFormatSize interface.
 */
HWTEST(AudioProcessInClientUnitTest, GetFormatSize_001, TestSize.Level1)
{
    AudioStreamInfo info = AudioStreamInfo();
    info.channels = STEREO;
    sptr<IAudioProcess> ipcProxy = nullptr;
    bool ipViopMmap = true;
    std::shared_ptr<AudioProcessInClientInner> audioProcessInClientInner =
        std::make_shared<AudioProcessInClientInner>(ipcProxy, ipViopMmap);
    size_t ret = GetFormatSize(info);
    EXPECT_EQ(ret, 4);
    info.format = SAMPLE_S24LE;
    ret = GetFormatSize(info);
    EXPECT_EQ(ret, 6);
    info.format = SAMPLE_S32LE;
    ret = GetFormatSize(info);
    EXPECT_EQ(ret, 8);
}

/**
 * @tc.name  : Test S32MonoToS16Stereo API
 * @tc.type  : FUNC
 * @tc.number: S32MonoToS16Stereo_001
 * @tc.desc  : Test S32MonoToS16Stereo interface.
 */
HWTEST(AudioProcessInClientUnitTest, S32MonoToS16Stereo_001, TestSize.Level1)
{
    uint8_t buffer = 14;
    BufferDesc srcDesc = {
        .buffer = nullptr,
        .bufLength = 3,
    };
    BufferDesc dstDesc = {
        .buffer = nullptr,
        .bufLength = 1,
    };
    sptr<IAudioProcess> ipcProxy = nullptr;
    bool ipViopMmap = true;
    AudioProcessInClientInner audioProcessInClientInner =
        AudioProcessInClientInner(ipcProxy, ipViopMmap);
    bool ret = S32MonoToS16Stereo(srcDesc, dstDesc);
    EXPECT_EQ(ret, false);
    srcDesc.bufLength = 4;
    dstDesc.bufLength = 4;
    srcDesc.buffer = &buffer;
    dstDesc.buffer = &buffer;
    ret = S32MonoToS16Stereo(srcDesc, dstDesc);
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name  : Test PrepareCurrent API
 * @tc.type  : FUNC
 * @tc.number: PrepareCurrent_001
 * @tc.desc  : Test PrepareCurrent interface.
 */
HWTEST(AudioProcessInClientUnitTest, PrepareCurrent_001, TestSize.Level1)
{
    sptr<IAudioProcess> ipcProxy = nullptr;
    bool ipViopMmap = true;
    std::shared_ptr<AudioProcessInClientInner> audioProcessInClientInner =
        std::make_shared<AudioProcessInClientInner>(ipcProxy, ipViopMmap);
    uint64_t curWritePos = 1;
    bool ret = audioProcessInClientInner->PrepareCurrent(curWritePos);
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name  : Test S32StereoS16Stereo API
 * @tc.type  : FUNC
 * @tc.number: S32StereoS16Stereo_001
 * @tc.desc  : Test S32StereoS16Stereo interface.
 */
HWTEST(AudioProcessInClientUnitTest, S32StereoS16Stereo_001, TestSize.Level1)
{
    uint8_t buffer = 14;
    BufferDesc srcDesc = {
        .buffer = nullptr,
        .bufLength = 3,
    };
    BufferDesc dstDesc = {
        .buffer = nullptr,
        .bufLength = 1,
    };
    sptr<IAudioProcess> ipcProxy = nullptr;
    bool ipViopMmap = true;
    std::shared_ptr<AudioProcessInClientInner> audioProcessInClientInner =
        std::make_shared<AudioProcessInClientInner>(ipcProxy, ipViopMmap);
    bool ret = S32StereoS16Stereo(srcDesc, dstDesc);
    EXPECT_EQ(ret, false);
    srcDesc.bufLength = 8;
    dstDesc.bufLength = 4;
    srcDesc.buffer = &buffer;
    dstDesc.buffer = &buffer;
    ret = S32MonoToS16Stereo(srcDesc, dstDesc);
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name  : Test GetStatusInfo API
 * @tc.type  : FUNC
 * @tc.number: GetStatusInfo_001
 * @tc.desc  : Test GetStatusInfo interface.
 */
HWTEST(AudioProcessInClientUnitTest, GetStatusInfo_001, TestSize.Level1)
{
    sptr<IAudioProcess> ipcProxy = nullptr;
    bool ipViopMmap = true;
    std::shared_ptr<AudioProcessInClientInner> audioProcessInClientInner =
        std::make_shared<AudioProcessInClientInner>(ipcProxy, ipViopMmap);
    StreamStatus status = STREAM_PAUSING;
    std::string ret = audioProcessInClientInner->GetStatusInfo(status);
    EXPECT_EQ(ret, "STREAM_PAUSING");
    status = STREAM_STOPPING;
    ret = audioProcessInClientInner->GetStatusInfo(status);
    EXPECT_EQ(ret, "STREAM_STOPPING");
    status = STREAM_STOPPED;
    ret = audioProcessInClientInner->GetStatusInfo(status);
    EXPECT_EQ(ret, "STREAM_STOPPED");
    status = STREAM_RELEASED;
    ret = audioProcessInClientInner->GetStatusInfo(status);
    EXPECT_EQ(ret, "STREAM_RELEASED");
    status = STREAM_INVALID;
    ret = audioProcessInClientInner->GetStatusInfo(status);
    EXPECT_EQ(ret, "STREAM_INVALID");
    status = STREAM_STAND_BY;
    ret = audioProcessInClientInner->GetStatusInfo(status);
    EXPECT_EQ(ret, "NO_SUCH_STATUS");
}

/**
 * @tc.name  : Test KeepLoopRunning API
 * @tc.type  : FUNC
 * @tc.number: KeepLoopRunning_001
 * @tc.desc  : Test KeepLoopRunning interface.
 */
HWTEST(AudioProcessInClientUnitTest, KeepLoopRunning_001, TestSize.Level1)
{
    sptr<IAudioProcess> ipcProxy = nullptr;
    bool ipViopMmap = true;
    std::shared_ptr<AudioProcessInClientInner> audioProcessInClientInner =
        std::make_shared<AudioProcessInClientInner>(ipcProxy, ipViopMmap);
    std::atomic<StreamStatus>* statusAtomPtr = new std::atomic<StreamStatus>();
    statusAtomPtr->store(StreamStatus::STREAM_STAND_BY);
    audioProcessInClientInner->streamStatus_ = statusAtomPtr;
    bool ret = audioProcessInClientInner->KeepLoopRunning();
    EXPECT_EQ(ret, true);
    statusAtomPtr->store(StreamStatus::STREAM_PAUSING);
    ret = audioProcessInClientInner->KeepLoopRunning();
    EXPECT_EQ(ret, true);
    statusAtomPtr->store(StreamStatus::STREAM_STOPPING);
    ret = audioProcessInClientInner->KeepLoopRunning();
    EXPECT_EQ(ret, true);
    statusAtomPtr->store(StreamStatus::STREAM_STOPPED);
    ret = audioProcessInClientInner->KeepLoopRunning();
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name  : Test RecordPrepareCurrent API
 * @tc.type  : FUNC
 * @tc.number: RecordPrepareCurrent_001
 * @tc.desc  : Test RecordPrepareCurrent interface.
 */
HWTEST(AudioProcessInClientUnitTest, RecordPrepareCurrent_001, TestSize.Level1)
{
    uint64_t curReadPos = 3;
    sptr<IAudioProcess> ipcProxy = nullptr;
    bool ipViopMmap = true;
    std::shared_ptr<AudioProcessInClientInner> audioProcessInClientInner =
        std::make_shared<AudioProcessInClientInner>(ipcProxy, ipViopMmap);
    int32_t ret = audioProcessInClientInner->RecordPrepareCurrent(curReadPos);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name  : Test PrepareCurrent API
 * @tc.type  : FUNC
 * @tc.number: PrepareCurrent_002
 * @tc.desc  : Test PrepareCurrent interface.
 */
HWTEST(AudioProcessInClientUnitTest, PrepareCurrent_002, TestSize.Level1)
{
    uint64_t curWritePos = 0;
    sptr<IAudioProcess> ipcProxy = nullptr;
    bool ipViopMmap = true;
    std::shared_ptr<AudioProcessInClientInner> audioProcessInClientInner =
        std::make_shared<AudioProcessInClientInner>(ipcProxy, ipViopMmap);
    bool ret = audioProcessInClientInner->PrepareCurrent(curWritePos);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name  : Test FinishHandleCurrent API
 * @tc.type  : FUNC
 * @tc.number: FinishHandleCurrent_001
 * @tc.desc  : Test FinishHandleCurrent interface.
 */
HWTEST(AudioProcessInClientUnitTest, FinishHandleCurrent_001, TestSize.Level1)
{
    uint64_t curWritePos = 0;
    int64_t clientWriteCost =2;
    sptr<IAudioProcess> ipcProxy = nullptr;
    bool ipViopMmap = true;
    std::shared_ptr<AudioProcessInClientInner> audioProcessInClientInner =
        std::make_shared<AudioProcessInClientInner>(ipcProxy, ipViopMmap);
    bool ret = audioProcessInClientInner->FinishHandleCurrent(curWritePos, clientWriteCost);
    EXPECT_EQ(ret, false);
    curWritePos = 1;
    SpanInfo *tempSpan = audioProcessInClientInner->audioBuffer_->GetSpanInfo(curWritePos);
    tempSpan->spanStatus.store(SpanStatus::SPAN_WRITTING);
    ret = audioProcessInClientInner->FinishHandleCurrent(curWritePos, clientWriteCost);
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name  : Test DoFadeInOut API
 * @tc.type  : FUNC
 * @tc.number: DoFadeInOut_001
 * @tc.desc  : Test DoFadeInOut interface.
 */
HWTEST(AudioProcessInClientUnitTest, DoFadeInOut_001, TestSize.Level1)
{
    sptr<IAudioProcess> ipcProxy = nullptr;
    bool ipViopMmap = true;
    std::shared_ptr<AudioProcessInClientInner> audioProcessInClientInner =
        std::make_shared<AudioProcessInClientInner>(ipcProxy, ipViopMmap);
    audioProcessInClientInner->startFadein_.store(true);
    uint64_t curWritePos = 5;
    audioProcessInClientInner->DoFadeInOut(curWritePos);
    audioProcessInClientInner->startFadein_.store(false);
    audioProcessInClientInner->startFadeout_.store(true);
    audioProcessInClientInner->DoFadeInOut(curWritePos);
}

/**
 * @tc.name  : Test FinishHandleCurrent API
 * @tc.type  : FUNC
 * @tc.number: FinishHandleCurrent_002
 * @tc.desc  : Test FinishHandleCurrent interface.
 */
HWTEST(AudioProcessInClientUnitTest, FinishHandleCurrent_002, TestSize.Level1)
{
    sptr<IAudioProcess> ipcProxy = nullptr;
    bool ipViopMmap = true;
    std::shared_ptr<AudioProcessInClientInner> audioProcessInClientInner =
        std::make_shared<AudioProcessInClientInner>(ipcProxy, ipViopMmap);
    uint64_t curWritePos = 5;
    int64_t clientWriteCost = 10;
    SpanInfo *tempSpan = audioProcessInClientInner->audioBuffer_->GetSpanInfo(curWritePos);
    EXPECT_NE(tempSpan, nullptr);
    tempSpan->spanStatus.store(SpanStatus::SPAN_WRITTING);
    bool ret = audioProcessInClientInner->FinishHandleCurrent(curWritePos, clientWriteCost);
    EXPECT_EQ(ret, true);
}
/**
 * @tc.name  : Test ProcessCallbackFuc API
 * @tc.type  : FUNC
 * @tc.number: ProcessCallbackFuc_001
 * @tc.desc  : Test ProcessCallbackFuc interface.
 */
HWTEST(AudioProcessInClientUnitTest, ProcessCallbackFuc_001, TestSize.Level1)
{
    sptr<IAudioProcess> ipcProxy = nullptr;
    bool ipViopMmap = true;
    std::shared_ptr<AudioProcessInClientInner> audioProcessInClientInner =
        std::make_shared<AudioProcessInClientInner>(ipcProxy, ipViopMmap);
    audioProcessInClientInner->ProcessCallbackFuc();
    audioProcessInClientInner->isCallbackLoopEnd_ = true;
    audioProcessInClientInner->ProcessCallbackFuc();
    audioProcessInClientInner->startFadeout_.store(true);
    audioProcessInClientInner->ProcessCallbackFuc();
    std::atomic<StreamStatus>* statusAtomPtr = new std::atomic<StreamStatus>(STREAM_RUNNING);
    audioProcessInClientInner->streamStatus_ = statusAtomPtr;
    audioProcessInClientInner->ProcessCallbackFuc();
}

/**
 * @tc.name  : Test CheckIfWakeUpTooLate API
 * @tc.type  : FUNC
 * @tc.number: CheckIfWakeUpTooLate_001
 * @tc.desc  : Test CheckIfWakeUpTooLate interface.
 */
HWTEST(AudioProcessInClientUnitTest, CheckIfWakeUpTooLate_001, TestSize.Level1)
{
    sptr<IAudioProcess> ipcProxy = nullptr;
    bool ipViopMmap = true;
    std::shared_ptr<AudioProcessInClientInner> audioProcessInClientInner =
        std::make_shared<AudioProcessInClientInner>(ipcProxy, ipViopMmap);
    int64_t curTime = 0;
    int64_t wakeUpTime = 0;
    audioProcessInClientInner->CheckIfWakeUpTooLate(curTime, wakeUpTime);
    wakeUpTime = ClockTime::GetCurNano();
    audioProcessInClientInner->CheckIfWakeUpTooLate(curTime, wakeUpTime);
}

/**
 * @tc.name  : Test CheckIfWakeUpTooLate API
 * @tc.type  : FUNC
 * @tc.number: CheckIfWakeUpTooLate_002
 * @tc.desc  : Test CheckIfWakeUpTooLate interface.
 */
HWTEST(AudioProcessInClientUnitTest, CheckIfWakeUpTooLate_002, TestSize.Level1)
{
    sptr<IAudioProcess> ipcProxy = nullptr;
    bool ipViopMmap = true;
    std::shared_ptr<AudioProcessInClientInner> audioProcessInClientInner =
        std::make_shared<AudioProcessInClientInner>(ipcProxy, ipViopMmap);
    int64_t curTime = 0;
    int64_t wakeUpTime = 1;
    int64_t clientWriteCost = 1;
    audioProcessInClientInner->CheckIfWakeUpTooLate(curTime, wakeUpTime, clientWriteCost);
    wakeUpTime = 255;
    audioProcessInClientInner->CheckIfWakeUpTooLate(curTime, wakeUpTime, clientWriteCost);
}

} // namespace AudioStandard
} // namespace OHOS