/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#include <gtest/gtest.h>
#include "pro_renderer_stream_impl.h"
#include "audio_errors.h"
#include "audio_service_log.h"
#include "audio_utils.h"
#include "securec.h"
#include "policy_handler.h"
#include "audio_common_converter.h"

using namespace testing::ext;
namespace OHOS {
namespace AudioStandard {

class ProRendererStreamImplUnitTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void ProRendererStreamImplUnitTest::SetUpTestCase(void)
{
    // input testsuit setup step£¬setup invoked before all testcases
}

void ProRendererStreamImplUnitTest::TearDownTestCase(void)
{
    // input testsuit teardown step£¬teardown invoked after all testcases
}

void ProRendererStreamImplUnitTest::SetUp(void)
{
    // input testcase setup step£¬setup invoked before each testcases
}

void ProRendererStreamImplUnitTest::TearDown(void)
{
    // input testcase teardown step£¬teardown invoked after each testcases
}

constexpr int32_t DEFAULT_STREAM_ID = 10;
constexpr int32_t WRITEINDEX = -1;
constexpr int32_t WRITEINDEXFIVE = 5;

static AudioProcessConfig InitProcessConfig()
{
    AudioProcessConfig config;
    config.appInfo.appUid = DEFAULT_STREAM_ID;
    config.appInfo.appPid = DEFAULT_STREAM_ID;
    config.streamInfo.format = SAMPLE_S32LE;
    config.streamInfo.samplingRate = SAMPLE_RATE_48000;
    config.streamInfo.channels = STEREO;
    config.streamInfo.channelLayout = AudioChannelLayout::CH_LAYOUT_STEREO;
    config.audioMode = AudioMode::AUDIO_MODE_PLAYBACK;
    config.streamType = AudioStreamType::STREAM_MUSIC;
    config.deviceType = DEVICE_TYPE_USB_HEADSET;
    return config;
}

/**
 * @tc.name  : Test ProRendererStreamImpl API
 * @tc.type  : FUNC
 * @tc.number: ProRendererStreamImpl_001
 * @tc.desc  : Test ProRendererStreamImpl::SetRate
 */
HWTEST_F(ProRendererStreamImplUnitTest, ProRendererStreamImpl_001, TestSize.Level1)
{
    AudioProcessConfig processConfig = InitProcessConfig();
    auto ptrProRendererStreamImpl = std::make_shared<ProRendererStreamImpl>(processConfig, true);
    EXPECT_NE(ptrProRendererStreamImpl, nullptr);

    int32_t rate = RENDER_RATE_NORMAL;

    auto ret = ptrProRendererStreamImpl->SetRate(rate);
    EXPECT_EQ(ret, SUCCESS);

    rate = RENDER_RATE_DOUBLE;
    ret = ptrProRendererStreamImpl->SetRate(rate);
    EXPECT_EQ(ret, SUCCESS);

    rate = RENDER_RATE_HALF;
    ret = ptrProRendererStreamImpl->SetRate(rate);
    EXPECT_EQ(ret, SUCCESS);

    rate = 3;
    ret = ptrProRendererStreamImpl->SetRate(rate);
    EXPECT_EQ(ret, ERR_INVALID_PARAM);
}

/**
 * @tc.name  : Test ProRendererStreamImpl API
 * @tc.type  : FUNC
 * @tc.number: ProRendererStreamImpl_002
 * @tc.desc  : Test ProRendererStreamImpl::DequeueBuffer
 */
HWTEST_F(ProRendererStreamImplUnitTest, ProRendererStreamImpl_002, TestSize.Level1)
{
    AudioProcessConfig processConfig = InitProcessConfig();
    auto ptrProRendererStreamImpl = std::make_shared<ProRendererStreamImpl>(processConfig, true);
    EXPECT_NE(ptrProRendererStreamImpl, nullptr);

    ptrProRendererStreamImpl->status_.store(IStatus::I_STATUS_IDLE);
    size_t length = 0;

    ptrProRendererStreamImpl->DequeueBuffer(length);

    ptrProRendererStreamImpl->status_.store(IStatus::I_STATUS_STARTED);
    ptrProRendererStreamImpl->DequeueBuffer(length);
}

/**
 * @tc.name  : Test ProRendererStreamImpl API
 * @tc.type  : FUNC
 * @tc.number: ProRendererStreamImpl_003
 * @tc.desc  : Test ProRendererStreamImpl::EnqueueBuffer
 */
HWTEST_F(ProRendererStreamImplUnitTest, ProRendererStreamImpl_003, TestSize.Level1)
{
    AudioProcessConfig processConfig = InitProcessConfig();
    auto ptrProRendererStreamImpl = std::make_shared<ProRendererStreamImpl>(processConfig, true);
    EXPECT_NE(ptrProRendererStreamImpl, nullptr);

    while (!ptrProRendererStreamImpl->writeQueue_.empty()) {
        ptrProRendererStreamImpl->writeQueue_.pop();
    }
    BufferDesc bufferDesc;
    auto ret = ptrProRendererStreamImpl->EnqueueBuffer(bufferDesc);
    EXPECT_EQ(ret, ERR_WRITE_BUFFER);
}

/**
 * @tc.name  : Test ProRendererStreamImpl API
 * @tc.type  : FUNC
 * @tc.number: ProRendererStreamImpl_004
 * @tc.desc  : Test ProRendererStreamImpl::EnqueueBuffer
 */
HWTEST_F(ProRendererStreamImplUnitTest, ProRendererStreamImpl_004, TestSize.Level1)
{
    AudioProcessConfig processConfig = InitProcessConfig();
    auto ptrProRendererStreamImpl = std::make_shared<ProRendererStreamImpl>(processConfig, true);
    EXPECT_NE(ptrProRendererStreamImpl, nullptr);

    ptrProRendererStreamImpl->writeQueue_.push(5);
    BufferDesc bufferDesc;
    ptrProRendererStreamImpl->isNeedMcr_ = true;
    ptrProRendererStreamImpl->isNeedResample_ = false;

    auto ret = ptrProRendererStreamImpl->EnqueueBuffer(bufferDesc);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test ProRendererStreamImpl API
 * @tc.type  : FUNC
 * @tc.number: ProRendererStreamImpl_005
 * @tc.desc  : Test ProRendererStreamImpl::EnqueueBuffer
 */
HWTEST_F(ProRendererStreamImplUnitTest, ProRendererStreamImpl_005, TestSize.Level1)
{
    AudioProcessConfig processConfig = InitProcessConfig();
    auto ptrProRendererStreamImpl = std::make_shared<ProRendererStreamImpl>(processConfig, true);
    EXPECT_NE(ptrProRendererStreamImpl, nullptr);

    ptrProRendererStreamImpl->writeQueue_.push(5);
    BufferDesc bufferDesc;
    ptrProRendererStreamImpl->isNeedMcr_ = true;
    ptrProRendererStreamImpl->isNeedResample_ = true;

    auto ret = ptrProRendererStreamImpl->EnqueueBuffer(bufferDesc);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test ProRendererStreamImpl API
 * @tc.type  : FUNC
 * @tc.number: ProRendererStreamImpl_006
 * @tc.desc  : Test ProRendererStreamImpl::EnqueueBuffer
 */
HWTEST_F(ProRendererStreamImplUnitTest, ProRendererStreamImpl_006, TestSize.Level1)
{
    AudioProcessConfig processConfig = InitProcessConfig();
    auto ptrProRendererStreamImpl = std::make_shared<ProRendererStreamImpl>(processConfig, true);
    EXPECT_NE(ptrProRendererStreamImpl, nullptr);

    ptrProRendererStreamImpl->writeQueue_.push(5);
    BufferDesc bufferDesc;
    ptrProRendererStreamImpl->isNeedMcr_ = false;
    ptrProRendererStreamImpl->isNeedResample_ = true;
    ptrProRendererStreamImpl->desFormat_ = AudioSampleFormat::SAMPLE_S16LE;

    auto ret = ptrProRendererStreamImpl->EnqueueBuffer(bufferDesc);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test ProRendererStreamImpl API
 * @tc.type  : FUNC
 * @tc.number: ProRendererStreamImpl_007
 * @tc.desc  : Test ProRendererStreamImpl::EnqueueBuffer
 */
HWTEST_F(ProRendererStreamImplUnitTest, ProRendererStreamImpl_007, TestSize.Level1)
{
    AudioProcessConfig processConfig = InitProcessConfig();
    auto ptrProRendererStreamImpl = std::make_shared<ProRendererStreamImpl>(processConfig, true);
    EXPECT_NE(ptrProRendererStreamImpl, nullptr);

    ptrProRendererStreamImpl->writeQueue_.push(5);
    BufferDesc bufferDesc;
    ptrProRendererStreamImpl->isNeedMcr_ = false;
    ptrProRendererStreamImpl->isNeedResample_ = true;
    ptrProRendererStreamImpl->desFormat_ = AudioSampleFormat::SAMPLE_S24LE;
    ptrProRendererStreamImpl->isFirstFrame_ = true;

    auto ret = ptrProRendererStreamImpl->EnqueueBuffer(bufferDesc);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test ProRendererStreamImpl API
 * @tc.type  : FUNC
 * @tc.number: ProRendererStreamImpl_008
 * @tc.desc  : Test ProRendererStreamImpl::EnqueueBuffer
 */
HWTEST_F(ProRendererStreamImplUnitTest, ProRendererStreamImpl_008, TestSize.Level1)
{
    AudioProcessConfig processConfig = InitProcessConfig();
    auto ptrProRendererStreamImpl = std::make_shared<ProRendererStreamImpl>(processConfig, true);
    EXPECT_NE(ptrProRendererStreamImpl, nullptr);

    ptrProRendererStreamImpl->writeQueue_.push(5);
    BufferDesc bufferDesc;
    ptrProRendererStreamImpl->isNeedMcr_ = false;
    ptrProRendererStreamImpl->isNeedResample_ = true;
    ptrProRendererStreamImpl->desFormat_ = AudioSampleFormat::SAMPLE_S24LE;
    ptrProRendererStreamImpl->isFirstFrame_ = false;

    auto ret = ptrProRendererStreamImpl->EnqueueBuffer(bufferDesc);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test ProRendererStreamImpl API
 * @tc.type  : FUNC
 * @tc.number: ProRendererStreamImpl_009
 * @tc.desc  : Test ProRendererStreamImpl::Peek
 */
HWTEST_F(ProRendererStreamImplUnitTest, ProRendererStreamImpl_009, TestSize.Level1)
{
    AudioProcessConfig processConfig = InitProcessConfig();
    auto ptrProRendererStreamImpl = std::make_shared<ProRendererStreamImpl>(processConfig, true);
    EXPECT_NE(ptrProRendererStreamImpl, nullptr);

    std::vector<char> audioBuffer = {'a'};
    int32_t index = 0;
    ptrProRendererStreamImpl->isBlock_ = true;

    auto ret = ptrProRendererStreamImpl->Peek(&audioBuffer, index);
    EXPECT_EQ(ret, ERR_WRITE_BUFFER);
}

/**
 * @tc.name  : Test ProRendererStreamImpl API
 * @tc.type  : FUNC
 * @tc.number: ProRendererStreamImpl_010
 * @tc.desc  : Test ProRendererStreamImpl::Peek
 */
HWTEST_F(ProRendererStreamImplUnitTest, ProRendererStreamImpl_010, TestSize.Level1)
{
    AudioProcessConfig processConfig = InitProcessConfig();
    auto ptrProRendererStreamImpl = std::make_shared<ProRendererStreamImpl>(processConfig, true);
    EXPECT_NE(ptrProRendererStreamImpl, nullptr);

    std::vector<char> audioBuffer = {'a'};
    int32_t index = 0;
    ptrProRendererStreamImpl->isBlock_ = false;
    ptrProRendererStreamImpl->readQueue_.push(5);

    auto ret = ptrProRendererStreamImpl->Peek(&audioBuffer, index);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test ProRendererStreamImpl API
 * @tc.type  : FUNC
 * @tc.number: ProRendererStreamImpl_011
 * @tc.desc  : Test ProRendererStreamImpl::Peek
 */
HWTEST_F(ProRendererStreamImplUnitTest, ProRendererStreamImpl_011, TestSize.Level1)
{
    AudioProcessConfig processConfig = InitProcessConfig();
    auto ptrProRendererStreamImpl = std::make_shared<ProRendererStreamImpl>(processConfig, true);
    EXPECT_NE(ptrProRendererStreamImpl, nullptr);

    std::vector<char> audioBuffer = {'a'};
    int32_t index = 0;
    ptrProRendererStreamImpl->isBlock_ = false;

    ptrProRendererStreamImpl->Peek(&audioBuffer, index);
}

/**
 * @tc.name  : Test ProRendererStreamImpl API
 * @tc.type  : FUNC
 * @tc.number: ProRendererStreamImpl_012
 * @tc.desc  : Test ProRendererStreamImpl::PopWriteBufferIndex
 */
HWTEST_F(ProRendererStreamImplUnitTest, ProRendererStreamImpl_012, TestSize.Level1)
{
    AudioProcessConfig processConfig = InitProcessConfig();
    auto ptrProRendererStreamImpl = std::make_shared<ProRendererStreamImpl>(processConfig, true);
    EXPECT_NE(ptrProRendererStreamImpl, nullptr);

    while (!ptrProRendererStreamImpl->writeQueue_.empty()) {
        ptrProRendererStreamImpl->writeQueue_.pop();
    }

    auto ret = ptrProRendererStreamImpl->PopWriteBufferIndex();
    EXPECT_EQ(ret, WRITEINDEX);
}

/**
 * @tc.name  : Test ProRendererStreamImpl API
 * @tc.type  : FUNC
 * @tc.number: ProRendererStreamImpl_013
 * @tc.desc  : Test ProRendererStreamImpl::PopWriteBufferIndex
 */
HWTEST_F(ProRendererStreamImplUnitTest, ProRendererStreamImpl_013, TestSize.Level1)
{
    AudioProcessConfig processConfig = InitProcessConfig();
    auto ptrProRendererStreamImpl = std::make_shared<ProRendererStreamImpl>(processConfig, true);
    EXPECT_NE(ptrProRendererStreamImpl, nullptr);

    ptrProRendererStreamImpl->writeQueue_.push(5);

    auto ret = ptrProRendererStreamImpl->PopWriteBufferIndex();
    EXPECT_EQ(ret, WRITEINDEXFIVE);
}

/**
 * @tc.name  : Test ProRendererStreamImpl API
 * @tc.type  : FUNC
 * @tc.number: ProRendererStreamImpl_014
 * @tc.desc  : Test ProRendererStreamImpl::PopSinkBuffer
 */
HWTEST_F(ProRendererStreamImplUnitTest, ProRendererStreamImpl_014, TestSize.Level1)
{
    AudioProcessConfig processConfig = InitProcessConfig();
    auto ptrProRendererStreamImpl = std::make_shared<ProRendererStreamImpl>(processConfig, true);
    EXPECT_NE(ptrProRendererStreamImpl, nullptr);

    std::vector<char> audioBuffer = {'a'};
    int32_t index = 0;

    while (!ptrProRendererStreamImpl->readQueue_.empty()) {
        ptrProRendererStreamImpl->readQueue_.pop();
    }
    ptrProRendererStreamImpl->isFirstFrame_ = true;

    ptrProRendererStreamImpl->PopSinkBuffer(&audioBuffer, index);
}

/**
 * @tc.name  : Test ProRendererStreamImpl API
 * @tc.type  : FUNC
 * @tc.number: ProRendererStreamImpl_015
 * @tc.desc  : Test ProRendererStreamImpl::PopSinkBuffer
 */
HWTEST_F(ProRendererStreamImplUnitTest, ProRendererStreamImpl_015, TestSize.Level1)
{
    AudioProcessConfig processConfig = InitProcessConfig();
    auto ptrProRendererStreamImpl = std::make_shared<ProRendererStreamImpl>(processConfig, true);
    EXPECT_NE(ptrProRendererStreamImpl, nullptr);

    std::vector<char> audioBuffer = {'a'};
    int32_t index = 0;

    while (!ptrProRendererStreamImpl->readQueue_.empty()) {
        ptrProRendererStreamImpl->readQueue_.pop();
    }
    ptrProRendererStreamImpl->isFirstFrame_ = false;

    ptrProRendererStreamImpl->PopSinkBuffer(&audioBuffer, index);
}

/**
 * @tc.name  : Test ProRendererStreamImpl API
 * @tc.type  : FUNC
 * @tc.number: ProRendererStreamImpl_016
 * @tc.desc  : Test ProRendererStreamImpl::PopSinkBuffer
 */
HWTEST_F(ProRendererStreamImplUnitTest, ProRendererStreamImpl_016, TestSize.Level1)
{
    AudioProcessConfig processConfig = InitProcessConfig();
    auto ptrProRendererStreamImpl = std::make_shared<ProRendererStreamImpl>(processConfig, true);
    EXPECT_NE(ptrProRendererStreamImpl, nullptr);

    std::vector<char> audioBuffer = {'a'};
    int32_t index = 0;

    ptrProRendererStreamImpl->readQueue_.push(5);
    ptrProRendererStreamImpl->isFirstFrame_ = false;

    ptrProRendererStreamImpl->PopSinkBuffer(&audioBuffer, index);
}
} // namespace AudioStandard
} // namespace OHOS