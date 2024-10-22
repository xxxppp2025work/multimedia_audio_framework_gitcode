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

#include <gtest/gtest.h>

#include "audio_service_log.h"
#include "audio_errors.h"
#include "iremote_broker.h"
#include "audio_policy_manager.h"
#include "system_ability_definition.h"
#include "capturer_in_client.h"
#include "../../../client/src/capturer_in_client.cpp"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {
const int32_t CLIENT_ID = 10;

enum {
    STATE_CHANGE_EVENT = 0,
    RENDERER_MARK_REACHED_EVENT,
    RENDERER_PERIOD_REACHED_EVENT,
    CAPTURER_PERIOD_REACHED_EVENT,
    CAPTURER_MARK_REACHED_EVENT,
};

enum : int64_t {
    HANDLER_PARAM_INVALID = -1,
    HANDLER_PARAM_NEW = 0,
    HANDLER_PARAM_PREPARED,
    HANDLER_PARAM_RUNNING,
    HANDLER_PARAM_STOPPED,
    HANDLER_PARAM_RELEASED,
    HANDLER_PARAM_PAUSED,
    HANDLER_PARAM_STOPPING,
    HANDLER_PARAM_RUNNING_FROM_SYSTEM,
    HANDLER_PARAM_PAUSED_FROM_SYSTEM,
};

class CapturerInClientUnitTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

class CapturerPeriodPositionCallbackTest : public CapturerPeriodPositionCallback {
public:
    virtual ~CapturerPeriodPositionCallbackTest() = default;

    /**
     * Called when the requested frame count is read.
     *
     * @param frameCount requested frame frame count for callback.
     * @since 8
     */
    virtual void OnPeriodReached(const int64_t &frameNumber) {};
};

class AudioStreamCallbackTest : public AudioStreamCallback {
public:
    virtual ~AudioStreamCallbackTest() = default;
    /**
     * Called when stream state is updated.
     *
     * @param state Indicates the InterruptEvent information needed by client.
     * For details, refer InterruptEvent struct in audio_info.h
     */
    virtual void OnStateChange(const State state, const StateChangeCmdType cmdType = CMD_FROM_CLIENT) {};
};

class AudioClientTrackerTest : public AudioClientTracker {
public:
    virtual ~AudioClientTrackerTest() = default;
    /**
     * Mute Stream was controlled by system application
     *
     * @param streamSetStateEventInternal Contains the set even information.
     */
    virtual void MuteStreamImpl(const StreamSetStateEventInternal &streamSetStateEventInternal) {};
     /**
     * Unmute Stream was controlled by system application
     *
     * @param streamSetStateEventInternal Contains the set even information.
     */
    virtual void UnmuteStreamImpl(const StreamSetStateEventInternal &streamSetStateEventInternal) {};
    /**
     * Paused Stream was controlled by system application
     *
     * @param streamSetStateEventInternal Contains the set even information.
     */
    virtual void PausedStreamImpl(const StreamSetStateEventInternal &streamSetStateEventInternal) {};
     /**
     * Resumed Stream was controlled by system application
     *
     * @param streamSetStateEventInternal Contains the set even information.
     */
    virtual void ResumeStreamImpl(const StreamSetStateEventInternal &streamSetStateEventInternal) {};
    virtual void SetLowPowerVolumeImpl(float volume) {};
    virtual void GetLowPowerVolumeImpl(float &volume) {};
    virtual void GetSingleStreamVolumeImpl(float &volume) {};
    virtual void SetOffloadModeImpl(int32_t state, bool isAppBack) {};
    virtual void UnsetOffloadModeImpl() {};
};

void Init(std::shared_ptr<CapturerInClientInner> capturerInClientInner)
{
    capturerInClientInner->clientPid_ = CLIENT_ID;
    capturerInClientInner->clientUid_ = CLIENT_ID;
    capturerInClientInner->appTokenId_ = CLIENT_ID;
    capturerInClientInner->fullTokenId_ = CLIENT_ID;
    capturerInClientInner->streamParams_.channels = STEREO;
    capturerInClientInner->streamParams_.encoding = AudioEncodingType::ENCODING_AUDIOVIVID;
    capturerInClientInner->streamParams_.format = SAMPLE_S32LE;
    capturerInClientInner->streamParams_.samplingRate = SAMPLE_RATE_48000;
    capturerInClientInner->streamParams_.channelLayout = AudioChannelLayout::CH_LAYOUT_STEREO;
    capturerInClientInner->eStreamType_ = AudioStreamType::STREAM_MUSIC;
    capturerInClientInner->capturerInfo_.sourceType = SOURCE_TYPE_PLAYBACK_CAPTURE;
    size_t size = 8 * 1024;
    capturerInClientInner->clientSpanSizeInByte_ = size;
    capturerInClientInner->ringCache_ = std::make_unique<AudioRingCache>(size);
}

/**
 * @tc.name  : Test OnOperationHandled API
 * @tc.type  : FUNC
 * @tc.number: OnOperationHandled_001
 * @tc.desc  : Test OnOperationHandled interface.
 */
HWTEST(CapturerInClientUnitTest, OnOperationHandled_001, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Operation operation = UPDATE_STREAM;
    int64_t result = 1;
    int32_t ret = capturerInClientInner_->OnOperationHandled(operation, result);
    EXPECT_EQ(ret, SUCCESS);

    operation = BUFFER_OVERFLOW;
    ret = capturerInClientInner_->OnOperationHandled(operation, result);
    EXPECT_EQ(ret, SUCCESS);

    operation = RESTORE_SESSION;
    ret = capturerInClientInner_->OnOperationHandled(operation, result);
    EXPECT_EQ(ret, SUCCESS);

    operation = START_STREAM;
    ret = capturerInClientInner_->OnOperationHandled(operation, result);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test RegisterTracker API
 * @tc.type  : FUNC
 * @tc.number: RegisterTracker_001
 * @tc.desc  : Test RegisterTracker interface.
 */
HWTEST(CapturerInClientUnitTest, RegisterTracker_001, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    capturerInClientInner_->streamTrackerRegistered_ = false;
    std::shared_ptr<AudioClientTracker> proxyObj;
    capturerInClientInner_->RegisterTracker(proxyObj);
    EXPECT_NE(capturerInClientInner_, nullptr);
}

/**
 * @tc.name  : Test RegisterTracker API
 * @tc.type  : FUNC
 * @tc.number: RegisterTracker_002
 * @tc.desc  : Test RegisterTracker interface.
 */
HWTEST(CapturerInClientUnitTest, RegisterTracker_002, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    capturerInClientInner_->audioStreamTracker_ = nullptr;
    capturerInClientInner_->streamTrackerRegistered_ = true;
    std::shared_ptr<AudioClientTracker> proxyObj;
    capturerInClientInner_->RegisterTracker(proxyObj);
    EXPECT_NE(capturerInClientInner_, nullptr);
}

/**
 * @tc.name  : Test RegisterTracker API
 * @tc.type  : FUNC
 * @tc.number: RegisterTracker_003
 * @tc.desc  : Test RegisterTracker interface.
 */
HWTEST(CapturerInClientUnitTest, RegisterTracker_003, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    capturerInClientInner_->audioStreamTracker_ = nullptr;
    capturerInClientInner_->streamTrackerRegistered_ = false;
    std::shared_ptr<AudioClientTracker> proxyObj;
    capturerInClientInner_->RegisterTracker(proxyObj);
    EXPECT_NE(capturerInClientInner_, nullptr);
}

/**
 * @tc.name  : Test UpdateTracker API
 * @tc.type  : FUNC
 * @tc.number: UpdateTracker_001
 * @tc.desc  : Test UpdateTracker interface.
 */
HWTEST(CapturerInClientUnitTest, UpdateTracker_001, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    capturerInClientInner_->audioStreamTracker_ = nullptr;
    std::string updateCase = " ";
    capturerInClientInner_->UpdateTracker(updateCase);
    EXPECT_NE(capturerInClientInner_, nullptr);
}

/**
 * @tc.name  : Test SetAudioStreamInfo API
 * @tc.type  : FUNC
 * @tc.number: SetAudioStreamInfo_001
 * @tc.desc  : Test SetAudioStreamInfo interface.
 */
HWTEST(CapturerInClientUnitTest, SetAudioStreamInfo_001, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Init(capturerInClientInner_);
    std::shared_ptr<AudioClientTracker> proxyObj;
    AudioStreamParams info;
    info.format = AudioSampleFormat::SAMPLE_U8;
    info.encoding = AudioEncodingType::ENCODING_AUDIOVIVID;
    info.samplingRate = AudioSamplingRate::SAMPLE_RATE_8000;
    info.channels = AudioChannel::MONO;
    info.channelLayout = AudioChannelLayout::CH_LAYOUT_MONO;
    capturerInClientInner_->state_ = NEW;
    int32_t ret = capturerInClientInner_->SetAudioStreamInfo(info, proxyObj);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test SetAudioStreamInfo API
 * @tc.type  : FUNC
 * @tc.number: SetAudioStreamInfo_002
 * @tc.desc  : Test SetAudioStreamInfo interface.
 */
HWTEST(CapturerInClientUnitTest, SetAudioStreamInfo_002, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    std::shared_ptr<AudioClientTracker> proxyObj;
    AudioStreamParams info;
    info.format = AudioSampleFormat::SAMPLE_F32LE;
    info.encoding = AudioEncodingType::ENCODING_AUDIOVIVID;
    info.samplingRate = AudioSamplingRate::SAMPLE_RATE_8000;
    info.channels = AudioChannel::MONO;
    info.channelLayout = AudioChannelLayout::CH_LAYOUT_MONO;
    capturerInClientInner_->state_ = NEW;
    int32_t ret = capturerInClientInner_->SetAudioStreamInfo(info, proxyObj);
    EXPECT_EQ(ret, ERR_NOT_SUPPORTED);
}

/**
 * @tc.name  : Test SetAudioStreamInfo API
 * @tc.type  : FUNC
 * @tc.number: SetAudioStreamInfo_003
 * @tc.desc  : Test SetAudioStreamInfo interface.
 */
HWTEST(CapturerInClientUnitTest, SetAudioStreamInfo_003, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    std::shared_ptr<AudioClientTracker> proxyObj;
    AudioStreamParams info;
    info.format = AudioSampleFormat::SAMPLE_F32LE;
    info.encoding = AudioEncodingType::ENCODING_INVALID;
    info.samplingRate = AudioSamplingRate::SAMPLE_RATE_8000;
    info.channels = AudioChannel::MONO;
    info.channelLayout = AudioChannelLayout::CH_LAYOUT_MONO;
    capturerInClientInner_->state_ = NEW;
    int32_t ret = capturerInClientInner_->SetAudioStreamInfo(info, proxyObj);
    EXPECT_EQ(ret, ERR_NOT_SUPPORTED);
}

/**
 * @tc.name  : Test SetAudioStreamInfo API
 * @tc.type  : FUNC
 * @tc.number: SetAudioStreamInfo_004
 * @tc.desc  : Test SetAudioStreamInfo interface.
 */
HWTEST(CapturerInClientUnitTest, SetAudioStreamInfo_004, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    std::shared_ptr<AudioClientTracker> proxyObj;
    AudioStreamParams info;
    info.format = AudioSampleFormat::SAMPLE_F32LE;
    info.encoding = AudioEncodingType::ENCODING_INVALID;
    info.samplingRate = static_cast<AudioSamplingRate>(600);
    info.channels = AudioChannel::MONO;
    info.channelLayout = AudioChannelLayout::CH_LAYOUT_MONO;
    capturerInClientInner_->state_ = NEW;
    int32_t ret = capturerInClientInner_->SetAudioStreamInfo(info, proxyObj);
    EXPECT_EQ(ret, ERR_NOT_SUPPORTED);
}

/**
 * @tc.name  : Test SetAudioStreamInfo API
 * @tc.type  : FUNC
 * @tc.number: SetAudioStreamInfo_005
 * @tc.desc  : Test SetAudioStreamInfo interface.
 */
HWTEST(CapturerInClientUnitTest, SetAudioStreamInfo_005, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    std::shared_ptr<AudioClientTracker> proxyObj;
    AudioStreamParams info;
    info.format = AudioSampleFormat::SAMPLE_F32LE;
    info.encoding = AudioEncodingType::ENCODING_AUDIOVIVID;
    info.samplingRate = static_cast<AudioSamplingRate>(600);
    info.channels = AudioChannel::MONO;
    info.channelLayout = AudioChannelLayout::CH_LAYOUT_MONO;
    capturerInClientInner_->state_ = NEW;
    int32_t ret = capturerInClientInner_->SetAudioStreamInfo(info, proxyObj);
    EXPECT_EQ(ret, ERR_NOT_SUPPORTED);
}

/**
 * @tc.name  : Test SetAudioStreamInfo API
 * @tc.type  : FUNC
 * @tc.number: SetAudioStreamInfo_006
 * @tc.desc  : Test SetAudioStreamInfo interface.
 */
HWTEST(CapturerInClientUnitTest, SetAudioStreamInfo_006, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    std::shared_ptr<AudioClientTracker> proxyObj;
    AudioStreamParams info;
    info.format = AudioSampleFormat::SAMPLE_U8;
    info.encoding = AudioEncodingType::ENCODING_INVALID;
    info.samplingRate = static_cast<AudioSamplingRate>(600);
    info.channels = AudioChannel::MONO;
    info.channelLayout = AudioChannelLayout::CH_LAYOUT_MONO;
    capturerInClientInner_->state_ = NEW;
    int32_t ret = capturerInClientInner_->SetAudioStreamInfo(info, proxyObj);
    EXPECT_EQ(ret, ERR_NOT_SUPPORTED);
}

/**
 * @tc.name  : Test SetAudioStreamInfo API
 * @tc.type  : FUNC
 * @tc.number: SetAudioStreamInfo_007
 * @tc.desc  : Test SetAudioStreamInfo interface.
 */
HWTEST(CapturerInClientUnitTest, SetAudioStreamInfo_007, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    std::shared_ptr<AudioClientTracker> proxyObj;
    AudioStreamParams info;
    info.format = AudioSampleFormat::SAMPLE_U8;
    info.encoding = AudioEncodingType::ENCODING_INVALID;
    info.samplingRate = AudioSamplingRate::SAMPLE_RATE_8000;
    info.channels = AudioChannel::MONO;
    info.channelLayout = AudioChannelLayout::CH_LAYOUT_MONO;
    capturerInClientInner_->state_ = NEW;
    int32_t ret = capturerInClientInner_->SetAudioStreamInfo(info, proxyObj);
    EXPECT_EQ(ret, ERR_NOT_SUPPORTED);
}

/**
 * @tc.name  : Test SetAudioStreamInfo API
 * @tc.type  : FUNC
 * @tc.number: SetAudioStreamInfo_008
 * @tc.desc  : Test SetAudioStreamInfo interface.
 */
HWTEST(CapturerInClientUnitTest, SetAudioStreamInfo_008, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    std::shared_ptr<AudioClientTracker> proxyObj;
    AudioStreamParams info;
    info.format = AudioSampleFormat::SAMPLE_U8;
    info.encoding = AudioEncodingType::ENCODING_AUDIOVIVID;
    info.samplingRate = static_cast<AudioSamplingRate>(600);
    info.channels = AudioChannel::MONO;
    info.channelLayout = AudioChannelLayout::CH_LAYOUT_MONO;
    capturerInClientInner_->state_ = NEW;
    int32_t ret = capturerInClientInner_->SetAudioStreamInfo(info, proxyObj);
    EXPECT_EQ(ret, ERR_NOT_SUPPORTED);
}

/**
 * @tc.name  : Test SetAudioStreamInfo API
 * @tc.type  : FUNC
 * @tc.number: SetAudioStreamInfo_009
 * @tc.desc  : Test SetAudioStreamInfo interface.
 */
HWTEST(CapturerInClientUnitTest, SetAudioStreamInfo_009, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    std::shared_ptr<AudioClientTracker> proxyObj;
    AudioStreamParams info;
    info.format = AudioSampleFormat::SAMPLE_U8;
    info.encoding = AudioEncodingType::ENCODING_AUDIOVIVID;
    info.samplingRate = AudioSamplingRate::SAMPLE_RATE_8000;
    info.channels = AudioChannel::CHANNEL_7;
    info.channelLayout = static_cast<AudioChannelLayout>(99999);
    capturerInClientInner_->state_ = NEW;
    int32_t ret = capturerInClientInner_->SetAudioStreamInfo(info, proxyObj);
    EXPECT_EQ(ret, ERR_NOT_SUPPORTED);
}

/**
 * @tc.name  : Test SetAudioStreamInfo API
 * @tc.type  : FUNC
 * @tc.number: SetAudioStreamInfo_010
 * @tc.desc  : Test SetAudioStreamInfo interface.
 */
HWTEST(CapturerInClientUnitTest, SetAudioStreamInfo_010, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Init(capturerInClientInner_);
    std::shared_ptr<AudioClientTracker> proxyObj;
    AudioStreamParams info;
    info.format = AudioSampleFormat::SAMPLE_U8;
    info.encoding = AudioEncodingType::ENCODING_AUDIOVIVID;
    info.samplingRate = AudioSamplingRate::SAMPLE_RATE_8000;
    info.channels = AudioChannel::MONO;
    info.channelLayout = AudioChannelLayout::CH_LAYOUT_MONO;
    capturerInClientInner_->state_ = INVALID;
    int32_t ret = capturerInClientInner_->SetAudioStreamInfo(info, proxyObj);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test GetAudioServerProxy API
 * @tc.type  : FUNC
 * @tc.number: GetAudioServerProxy_001
 * @tc.desc  : Test GetAudioServerProxy interface.
 */
HWTEST(CapturerInClientUnitTest, GetAudioServerProxy_001, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Init(capturerInClientInner_);
    capturerInClientInner_->GetAudioServerProxy();
    capturerInClientInner_->GetAudioServerProxy();
    EXPECT_NE(capturerInClientInner_, nullptr);
}

/**
 * @tc.name  : Test OnHandle API
 * @tc.type  : FUNC
 * @tc.number: OnHandle_001
 * @tc.desc  : Test OnHandle interface.
 */
HWTEST(CapturerInClientUnitTest, OnHandle_001, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Init(capturerInClientInner_);
    uint32_t code = RENDERER_PERIOD_REACHED_EVENT;
    int64_t data = 0;
    capturerInClientInner_->OnHandle(code, data);
    EXPECT_NE(capturerInClientInner_, nullptr);

    code = CAPTURER_PERIOD_REACHED_EVENT;
    capturerInClientInner_->OnHandle(code, data);
    EXPECT_NE(capturerInClientInner_, nullptr);
}

/**
 * @tc.name  : Test HandleCapturerMarkReachedEvent API
 * @tc.type  : FUNC
 * @tc.number: HandleCapturerMarkReachedEvent_001
 * @tc.desc  : Test HandleCapturerMarkReachedEvent interface.
 */
HWTEST(CapturerInClientUnitTest, HandleCapturerMarkReachedEvent_001, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Init(capturerInClientInner_);
    capturerInClientInner_->capturerPositionCallback_ = nullptr;
    int64_t data = 16;
    capturerInClientInner_->HandleCapturerMarkReachedEvent(data);
    EXPECT_NE(capturerInClientInner_, nullptr);
}

/**
 * @tc.name  : Test HandleCapturerPeriodReachedEvent API
 * @tc.type  : FUNC
 * @tc.number: HandleCapturerPeriodReachedEvent_001
 * @tc.desc  : Test HandleCapturerPeriodReachedEvent interface.
 */
HWTEST(CapturerInClientUnitTest, HandleCapturerPeriodReachedEvent_001, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Init(capturerInClientInner_);
    capturerInClientInner_->capturerPeriodPositionCallback_ = nullptr;
    int64_t data = 16;
    capturerInClientInner_->HandleCapturerPeriodReachedEvent(data);
    EXPECT_NE(capturerInClientInner_, nullptr);
}

/**
 * @tc.name  : Test HandleCapturerPeriodReachedEvent API
 * @tc.type  : FUNC
 * @tc.number: HandleCapturerPeriodReachedEvent_002
 * @tc.desc  : Test HandleCapturerPeriodReachedEvent interface.
 */
HWTEST(CapturerInClientUnitTest, HandleCapturerPeriodReachedEvent_002, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Init(capturerInClientInner_);
    auto callback = std::make_shared<CapturerPeriodPositionCallbackTest>();
    int64_t data = 16;
    capturerInClientInner_->SetCapturerPeriodPositionCallback(data, callback);
    capturerInClientInner_->HandleCapturerPeriodReachedEvent(data);
    EXPECT_NE(capturerInClientInner_, nullptr);
}

/**
 * @tc.name  : Test ParamsToStateCmdType API
 * @tc.type  : FUNC
 * @tc.number: ParamsToStateCmdType_001
 * @tc.desc  : Test ParamsToStateCmdType interface.
 */
HWTEST(CapturerInClientUnitTest, ParamsToStateCmdType_001, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Init(capturerInClientInner_);
    int64_t params = HANDLER_PARAM_NEW;
    State state;
    StateChangeCmdType cmdType;
    int32_t ret = capturerInClientInner_->ParamsToStateCmdType(params, state, cmdType);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test ParamsToStateCmdType API
 * @tc.type  : FUNC
 * @tc.number: ParamsToStateCmdType_002
 * @tc.desc  : Test ParamsToStateCmdType interface.
 */
HWTEST(CapturerInClientUnitTest, ParamsToStateCmdType_002, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Init(capturerInClientInner_);
    int64_t params = HANDLER_PARAM_RELEASED;
    State state;
    StateChangeCmdType cmdType;
    int32_t ret = capturerInClientInner_->ParamsToStateCmdType(params, state, cmdType);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test ParamsToStateCmdType API
 * @tc.type  : FUNC
 * @tc.number: ParamsToStateCmdType_003
 * @tc.desc  : Test ParamsToStateCmdType interface.
 */
HWTEST(CapturerInClientUnitTest, ParamsToStateCmdType_003, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Init(capturerInClientInner_);
    int64_t params = HANDLER_PARAM_STOPPING;
    State state;
    StateChangeCmdType cmdType;
    int32_t ret = capturerInClientInner_->ParamsToStateCmdType(params, state, cmdType);
    EXPECT_EQ(ret, SUCCESS);

    params = HANDLER_PARAM_RUNNING_FROM_SYSTEM;
    ret = capturerInClientInner_->ParamsToStateCmdType(params, state, cmdType);
    EXPECT_EQ(ret, SUCCESS);

    params = HANDLER_PARAM_PAUSED_FROM_SYSTEM;
    ret = capturerInClientInner_->ParamsToStateCmdType(params, state, cmdType);
    EXPECT_EQ(ret, SUCCESS);

    params = HANDLER_PARAM_INVALID;
    ret = capturerInClientInner_->ParamsToStateCmdType(params, state, cmdType);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test StateCmdTypeToParams API
 * @tc.type  : FUNC
 * @tc.number: StateCmdTypeToParams_001
 * @tc.desc  : Test StateCmdTypeToParams interface.
 */
HWTEST(CapturerInClientUnitTest, StateCmdTypeToParams_001, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Init(capturerInClientInner_);
    int64_t params;
    State state = State::RUNNING;
    StateChangeCmdType cmdType = CMD_FROM_SYSTEM;
    int32_t ret = capturerInClientInner_->StateCmdTypeToParams(params, state, cmdType);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test StateCmdTypeToParams API
 * @tc.type  : FUNC
 * @tc.number: StateCmdTypeToParams_001
 * @tc.desc  : Test StateCmdTypeToParams interface.
 */
HWTEST(CapturerInClientUnitTest, StateCmdTypeToParams_002, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Init(capturerInClientInner_);
    int64_t params;
    State state = State::PAUSED;
    StateChangeCmdType cmdType = CMD_FROM_SYSTEM;
    int32_t ret = capturerInClientInner_->StateCmdTypeToParams(params, state, cmdType);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test StateCmdTypeToParams API
 * @tc.type  : FUNC
 * @tc.number: StateCmdTypeToParams_001
 * @tc.desc  : Test StateCmdTypeToParams interface.
 */
HWTEST(CapturerInClientUnitTest, StateCmdTypeToParams_003, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Init(capturerInClientInner_);
    int64_t params;
    State state = State::NEW;
    StateChangeCmdType cmdType = CMD_FROM_SYSTEM;
    int32_t ret = capturerInClientInner_->StateCmdTypeToParams(params, state, cmdType);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test ConstructConfig API
 * @tc.type  : FUNC
 * @tc.number: ConstructConfig_001
 * @tc.desc  : Test ConstructConfig interface.
 */
HWTEST(CapturerInClientUnitTest, ConstructConfig_001, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Init(capturerInClientInner_);
    capturerInClientInner_->capturerInfo_.capturerFlags = 1;
    AudioProcessConfig ret = capturerInClientInner_->ConstructConfig();
    EXPECT_NE(capturerInClientInner_, nullptr);
}

/**
 * @tc.name  : Test InitCacheBuffer API
 * @tc.type  : FUNC
 * @tc.number: InitCacheBuffer_001
 * @tc.desc  : Test InitCacheBuffer interface.
 */
HWTEST(CapturerInClientUnitTest, InitCacheBuffer_001, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Init(capturerInClientInner_);
    size_t size = 32 * 1024 * 1024;
    int32_t ret = capturerInClientInner_->InitCacheBuffer(size);
    EXPECT_EQ(ret, ERR_OPERATION_FAILED);
}


/**
 * @tc.name  : Test GetBufferSize API
 * @tc.type  : FUNC
 * @tc.number: GetBufferSize_001
 * @tc.desc  : Test GetBufferSize interface.
 */
HWTEST(CapturerInClientUnitTest, GetBufferSize_001, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    Init(capturerInClientInner_);
    capturerInClientInner_->state_ = State::NEW;
    capturerInClientInner_->capturerMode_ = CAPTURE_MODE_CALLBACK;
    size_t bufferSize = 0;
    int32_t ret = capturerInClientInner_->GetBufferSize(bufferSize);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test SetStreamCallback API
 * @tc.type  : FUNC
 * @tc.number: SetStreamCallback_001
 * @tc.desc  : Test SetStreamCallback interface.
 */
HWTEST(CapturerInClientUnitTest, SetStreamCallback_001, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    auto callback = std::make_shared<AudioStreamCallbackTest>();
    callback = nullptr;
    int32_t ret = capturerInClientInner_->SetStreamCallback(callback);
    EXPECT_EQ(ret, ERR_INVALID_PARAM);
}

/**
 * @tc.name  : Test SetStreamCallback API
 * @tc.type  : FUNC
 * @tc.number: SetStreamCallback_002
 * @tc.desc  : Test SetStreamCallback interface.
 */
HWTEST(CapturerInClientUnitTest, SetStreamCallback_002, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    auto callback = std::make_shared<AudioStreamCallbackTest>();
    capturerInClientInner_->state_ = State::RUNNING;
    int32_t ret = capturerInClientInner_->SetStreamCallback(callback);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test InitCallbackBuffer API
 * @tc.type  : FUNC
 * @tc.number: InitCallbackBuffer_001
 * @tc.desc  : Test InitCallbackBuffer interface.
 */
HWTEST(CapturerInClientUnitTest, InitCallbackBuffer_001, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    u_int64_t bufferDurationInUs = 3000000;
    capturerInClientInner_->InitCallbackBuffer(bufferDurationInUs);
    EXPECT_NE(capturerInClientInner_, nullptr);
}

/**
 * @tc.name  : Test SetCaptureMode API
 * @tc.type  : FUNC
 * @tc.number: SetCaptureMode_001
 * @tc.desc  : Test SetCaptureMode interface.
 */
HWTEST(CapturerInClientUnitTest, SetCaptureMode_001, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    capturerInClientInner_->capturerMode_ = AudioCaptureMode::CAPTURE_MODE_NORMAL;
    AudioCaptureMode captureMode = AudioCaptureMode::CAPTURE_MODE_NORMAL;
    int32_t ret = capturerInClientInner_->SetCaptureMode(captureMode);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test SetCaptureMode API
 * @tc.type  : FUNC
 * @tc.number: SetCaptureMode_002
 * @tc.desc  : Test SetCaptureMode interface.
 */
HWTEST(CapturerInClientUnitTest, SetCaptureMode_002, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    capturerInClientInner_->state_ = State::INVALID;
    capturerInClientInner_->capturerMode_ = AudioCaptureMode::CAPTURE_MODE_NORMAL;
    AudioCaptureMode captureMode = AudioCaptureMode::CAPTURE_MODE_CALLBACK;
    int32_t ret = capturerInClientInner_->SetCaptureMode(captureMode);
    EXPECT_EQ(ret, ERR_ILLEGAL_STATE);

    capturerInClientInner_->capturerMode_ = AudioCaptureMode::CAPTURE_MODE_CALLBACK;
    captureMode = AudioCaptureMode::CAPTURE_MODE_NORMAL;
    ret = capturerInClientInner_->SetCaptureMode(captureMode);
    EXPECT_EQ(ret, ERR_INCORRECT_MODE);
}

/**
 * @tc.name  : Test SetCaptureMode API
 * @tc.type  : FUNC
 * @tc.number: SetCaptureMode_003
 * @tc.desc  : Test SetCaptureMode interface.
 */
HWTEST(CapturerInClientUnitTest, SetCaptureMode_003, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    capturerInClientInner_->capturerMode_ = AudioCaptureMode::CAPTURE_MODE_NORMAL;
    AudioCaptureMode captureMode = AudioCaptureMode::CAPTURE_MODE_CALLBACK;
    capturerInClientInner_->state_ = State::NEW;
    capturerInClientInner_->streamParams_.samplingRate = SAMPLE_RATE_11025;
    capturerInClientInner_->spanSizeInFrame_ = 1;
    int32_t ret = capturerInClientInner_->SetCaptureMode(captureMode);
    EXPECT_EQ(ret, SUCCESS);

    capturerInClientInner_->state_ = State::PREPARED;
    ret = capturerInClientInner_->SetCaptureMode(captureMode);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test SetCaptureMode API
 * @tc.type  : FUNC
 * @tc.number: SetCaptureMode_004
 * @tc.desc  : Test SetCaptureMode interface.
 */
HWTEST(CapturerInClientUnitTest, SetCaptureMode_004, TestSize.Level1)
{
    std::shared_ptr<CapturerInClientInner> capturerInClientInner_ =
        std::make_shared<CapturerInClientInner>(STREAM_MUSIC, getpid());
    capturerInClientInner_->capturerMode_ = AudioCaptureMode::CAPTURE_MODE_NORMAL;
    AudioCaptureMode captureMode = AudioCaptureMode::CAPTURE_MODE_CALLBACK;
    capturerInClientInner_->state_ = State::NEW;
    capturerInClientInner_->streamParams_.samplingRate = SAMPLE_RATE_11025;
    capturerInClientInner_->cbThreadReleased_ = true;
    capturerInClientInner_->spanSizeInFrame_ = 1;
    int32_t ret = capturerInClientInner_->SetCaptureMode(captureMode);
    EXPECT_EQ(ret, SUCCESS);
}

} // namespace AudioStandard
} // namespace OHOS
