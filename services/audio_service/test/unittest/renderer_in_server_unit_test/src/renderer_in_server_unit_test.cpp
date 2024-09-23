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

#include "renderer_in_server_unit_test.h"

#include "accesstoken_kit.h"
#include "audio_device_info.h"
#include "audio_errors.h"
#include "audio_info.h"
#include "audio_process_config.h"
#include "audio_server.h"
#include "audio_service.h"
#include "audio_stream_info.h"
#include "audio_utils.h"
#include "policy_handler.h"
#include "renderer_in_server.h"
#include "pro_audio_stream_manager.h"
#include "i_renderer_stream.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

const AudioSamplingRate SAMPLING_RATE_ERROR_0 = static_cast<AudioSamplingRate>(0);
const AudioSamplingRate SAMPLING_RATE_ERROR_OVER = static_cast<AudioSamplingRate>(132000000);
const AudioChannel AUDIO_CHANNEL_ERROR_0 = static_cast<AudioChannel>(0);
const AudioChannel AUDIO_CHANNEL_ERROR_1 = static_cast<AudioChannel>(1);
const float OUT_OF_MAX_FLOAT_VOLUME = 2.0f;
const int32_t SAMPLE_RATE_200000 = 200000;
const int32_t OUT_OF_STATUS = 14;
const size_t TEST_SIZE_T = 512;
const uint32_t TEST_UINT32_T = 64;
const int32_t TEST_INT32_T = 2;
const bool TEST_TRUE = true;

static std::shared_ptr<RendererInServer> rendererInServer;
static AudioProcessConfig processConfig;
static std::weak_ptr<IStreamListener> streamListener;
static AudioStreamInfo testStreamInfo(SAMPLE_RATE_48000, ENCODING_INVALID, SAMPLE_S24LE, MONO,
    AudioChannelLayout::CH_LAYOUT_UNKNOWN);

void RendererInServerUnitTest::SetUpTestCase(void) {}

void RendererInServerUnitTest::TearDownTestCase(void) {}

void RendererInServerUnitTest::SetUp(void)
{
    // input testcase setup step，setup invoked before each testcases
    processConfig.deviceType = DEVICE_TYPE_WIRED_HEADSET;
    processConfig.streamInfo = testStreamInfo;
    processConfig.streamType = STREAM_MUSIC;
    processConfig.rendererInfo.pipeType = PIPE_TYPE_DIRECT_MUSIC;
    processConfig.rendererInfo.rendererFlags = AUDIO_FLAG_VOIP_DIRECT;
    rendererInServer = std::make_shared<RendererInServer>(processConfig, streamListener);
}

void RendererInServerUnitTest::TearDown(void) {}

/**
 * @tc.name  : Test Init API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerInit_001
 * @tc.desc  : Test Init API when managerType_ is VOIP_PLAYBACK.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerInit_001, TestSize.Level1)
{
    AudioStreamInfo testStreamInfo(SAMPLE_RATE_48000, ENCODING_INVALID, SAMPLE_S24LE, MONO,
        AudioChannelLayout::CH_LAYOUT_UNKNOWN);
    processConfig.streamInfo = testStreamInfo;
    rendererInServer = std::make_shared<RendererInServer>(processConfig, streamListener);
    EXPECT_NE(nullptr, rendererInServer);

    int32_t ret = rendererInServer->Init();
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test Init API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerInit_002
 * @tc.desc  : Test Init API when managerType_ is DIRECT_PLAYBACK.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerInit_002, TestSize.Level1)
{
    AudioStreamInfo testStreamInfo(SAMPLE_RATE_48000, ENCODING_INVALID, SAMPLE_S24LE, MONO,
        AudioChannelLayout::CH_LAYOUT_UNKNOWN);
    processConfig.streamInfo = testStreamInfo;
    processConfig.rendererInfo.rendererFlags = AUDIO_FLAG_NORMAL;
    rendererInServer = std::make_shared<RendererInServer>(processConfig, streamListener);
    EXPECT_NE(nullptr, rendererInServer);

    int32_t ret = rendererInServer->Init();
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test Init API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerInit_003
 * @tc.desc  : Test Init API when managerType_ is PLAYBACK.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerInit_003, TestSize.Level1)
{
    AudioStreamInfo testStreamInfo(SAMPLE_RATE_48000, ENCODING_INVALID, SAMPLE_S24LE, MONO,
        AudioChannelLayout::CH_LAYOUT_UNKNOWN);
    processConfig.streamInfo = testStreamInfo;
    processConfig.deviceType = DEVICE_TYPE_USB_HEADSET;
    processConfig.rendererInfo.rendererFlags = AUDIO_FLAG_NORMAL;
    rendererInServer = std::make_shared<RendererInServer>(processConfig, streamListener);
    EXPECT_NE(nullptr, rendererInServer);

    int32_t ret = rendererInServer->Init();
    ret = rendererInServer->Init();
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test Init API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerInit_004
 * @tc.desc  : Test Init API when ConfigServerBuffer return Error with spanSizeInFrame_ is 0.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerInit_004, TestSize.Level1)
{
    AudioStreamInfo testStreamInfo(SAMPLING_RATE_ERROR_0, ENCODING_INVALID, SAMPLE_S24LE, MONO,
        AudioChannelLayout::CH_LAYOUT_UNKNOWN);
    processConfig.streamInfo = testStreamInfo;
    rendererInServer = std::make_shared<RendererInServer>(processConfig, streamListener);
    EXPECT_NE(nullptr, rendererInServer);

    int32_t ret = rendererInServer->Init();
    EXPECT_EQ(ERR_OPERATION_FAILED, ret);
}

/**
 * @tc.name  : Test ConfigServerBuffer API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerInit_005
 * @tc.desc  : Test Init API when ConfigServerBuffer return Error with byteSizePerFrame_ is 0.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerInit_005, TestSize.Level1)
{
    AudioStreamInfo testStreamInfo(SAMPLE_RATE_48000, ENCODING_INVALID, SAMPLE_S24LE, AUDIO_CHANNEL_ERROR_0,
        AudioChannelLayout::CH_LAYOUT_UNKNOWN);
    processConfig.streamInfo = testStreamInfo;
    rendererInServer = std::make_shared<RendererInServer>(processConfig, streamListener);
    EXPECT_NE(nullptr, rendererInServer);

    int32_t ret = rendererInServer->Init();
    EXPECT_EQ(ERR_OPERATION_FAILED, ret);
}

/**
 * @tc.name  : Test ConfigServerBuffer API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerInit_006
 * @tc.desc  : Test ConfigServerBuffer when audioServerBuffer_ create failed.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerInit_006, TestSize.Level1)
{
    AudioStreamInfo testStreamInfo(SAMPLING_RATE_ERROR_OVER, ENCODING_INVALID, SAMPLE_S24LE, AUDIO_CHANNEL_ERROR_1,
        AudioChannelLayout::CH_LAYOUT_UNKNOWN);
    processConfig.streamInfo = testStreamInfo;
    rendererInServer = std::make_shared<RendererInServer>(processConfig, streamListener);
    EXPECT_NE(nullptr, rendererInServer);

    int32_t ret = rendererInServer->Init();
    EXPECT_EQ(ERR_OPERATION_FAILED, ret);
}

/**
 * @tc.name  : Test ConfigServerBuffer API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerConfigServerBuffer_001
 * @tc.desc  : Test ConfigServerBuffer when audioServerBuffer_ is not nullptr.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerConfigServerBuffer_001, TestSize.Level1)
{
    AudioStreamInfo testStreamInfo(SAMPLE_RATE_48000, ENCODING_INVALID, SAMPLE_S24LE, MONO,
        AudioChannelLayout::CH_LAYOUT_UNKNOWN);
    processConfig.streamInfo = testStreamInfo;
    rendererInServer = std::make_shared<RendererInServer>(processConfig, streamListener);
    EXPECT_NE(nullptr, rendererInServer);

    int32_t ret = rendererInServer->Init();
    ret = rendererInServer->ConfigServerBuffer();
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test InitDualToneStream API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerInitDualToneStream_001
 * @tc.desc  : Test InitDualToneStream interface, Set dualToneStream_ is nullptr.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerInitDualToneStream_001, TestSize.Level1)
{
    AudioProcessConfig tempProcessConfig;
    std::shared_ptr<RendererInServer> tempRendererInServer;
    tempRendererInServer = std::make_shared<RendererInServer>(tempProcessConfig, streamListener);
    EXPECT_NE(nullptr, tempRendererInServer);

    tempRendererInServer->dualToneStream_ = nullptr;
    int32_t ret = tempRendererInServer->InitDualToneStream();

    EXPECT_EQ(ERR_OPERATION_FAILED, ret);
}

/**
 * @tc.name  : Test InitDualToneStream API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerInitDualToneStream_002
 * @tc.desc  : Test InitDualToneStream interface.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerInitDualToneStream_002, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    rendererInServer->status_ = I_STATUS_STARTED;
    int32_t ret = rendererInServer->InitDualToneStream();

    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test OnWriteData API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerOnWriteData_001
 * @tc.desc  : Test OnWriteData interface.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerOnWriteData_001, TestSize.Level1)
{
    std::shared_ptr<StreamCallbacks> streamCallbacks;
    streamCallbacks = std::make_shared<StreamCallbacks>(TEST_UINT32_T);
    EXPECT_NE(nullptr, streamCallbacks);

    int32_t ret = streamCallbacks->OnWriteData(TEST_SIZE_T);

    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test SetOffloadMode API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerSetOffloadMode_001
 * @tc.desc  : Test SetOffloadMode interface.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerSetOffloadMode_001, TestSize.Level1)
{
    AudioProcessConfig tempProcessConfig;
    std::shared_ptr<RendererInServer> tempRendererInServer;

    tempProcessConfig.streamInfo = testStreamInfo;
    tempProcessConfig.streamType = STREAM_MUSIC;
    tempProcessConfig.rendererInfo.pipeType = PIPE_TYPE_DIRECT_MUSIC;
    tempProcessConfig.deviceType = DEVICE_TYPE_INVALID;
    tempProcessConfig.rendererInfo.rendererFlags = AUDIO_FLAG_MMAP;
    tempRendererInServer = std::make_shared<RendererInServer>(tempProcessConfig, streamListener);
    EXPECT_NE(nullptr, tempRendererInServer);

    tempRendererInServer->managerType_ = DIRECT_PLAYBACK;
    tempRendererInServer->Init();
    int32_t ret = tempRendererInServer->SetOffloadMode(TEST_INT32_T, TEST_TRUE);

    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test SetOffloadMode API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerSetOffloadMode_002
 * @tc.desc  : Test SetOffloadMode interface, dupStream_ and dualToneStream_ is not nullptr.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerSetOffloadMode_002, TestSize.Level1)
{
    AudioProcessConfig tempProcessConfig;
    std::shared_ptr<RendererInServer> tempRendererInServer;

    tempProcessConfig.streamInfo = testStreamInfo;
    tempProcessConfig.streamType = STREAM_MUSIC;
    tempProcessConfig.rendererInfo.pipeType = PIPE_TYPE_DIRECT_MUSIC;
    tempProcessConfig.deviceType = DEVICE_TYPE_INVALID;
    tempProcessConfig.rendererInfo.rendererFlags = AUDIO_FLAG_MMAP;
    tempRendererInServer = std::make_shared<RendererInServer>(tempProcessConfig, streamListener);
    EXPECT_NE(nullptr, tempRendererInServer);

    tempRendererInServer->managerType_ = DIRECT_PLAYBACK;
    tempRendererInServer->Init();
    tempRendererInServer->InitDupStream();
    tempRendererInServer->InitDualToneStream();
    int32_t ret = tempRendererInServer->SetOffloadMode(TEST_INT32_T, TEST_TRUE);

    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test UnsetOffloadMode API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerUnsetOffloadMode_001
 * @tc.desc  : Test UnsetOffloadMode interface.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerUnsetOffloadMode_001, TestSize.Level1)
{
    AudioProcessConfig tempProcessConfig;
    std::shared_ptr<RendererInServer> tempRendererInServer;

    tempProcessConfig.streamInfo = testStreamInfo;
    tempProcessConfig.streamType = STREAM_MUSIC;
    tempProcessConfig.rendererInfo.pipeType = PIPE_TYPE_DIRECT_MUSIC;
    tempProcessConfig.deviceType = DEVICE_TYPE_INVALID;
    tempProcessConfig.rendererInfo.rendererFlags = AUDIO_FLAG_MMAP;
    tempRendererInServer = std::make_shared<RendererInServer>(tempProcessConfig, streamListener);
    EXPECT_NE(nullptr, tempRendererInServer);

    tempRendererInServer->managerType_ = DIRECT_PLAYBACK;
    tempRendererInServer->Init();
    int32_t ret = tempRendererInServer->UnsetOffloadMode();

    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test UnsetOffloadMode API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerUnsetOffloadMode_002
 * @tc.desc  : Test UnsetOffloadMode interface, dupStream_ and dualToneStream_ is not nullptr.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerUnsetOffloadMode_002, TestSize.Level1)
{
    AudioProcessConfig tempProcessConfig;
    std::shared_ptr<RendererInServer> tempRendererInServer;

    tempProcessConfig.streamInfo = testStreamInfo;
    tempProcessConfig.streamType = STREAM_MUSIC;
    tempProcessConfig.rendererInfo.pipeType = PIPE_TYPE_DIRECT_MUSIC;
    tempProcessConfig.deviceType = DEVICE_TYPE_INVALID;
    tempProcessConfig.rendererInfo.rendererFlags = AUDIO_FLAG_MMAP;
    tempRendererInServer = std::make_shared<RendererInServer>(tempProcessConfig, streamListener);
    EXPECT_NE(nullptr, tempRendererInServer);

    tempRendererInServer->managerType_ = DIRECT_PLAYBACK;
    tempRendererInServer->Init();
    tempRendererInServer->InitDupStream();
    tempRendererInServer->InitDualToneStream();
    int32_t ret = tempRendererInServer->UnsetOffloadMode();

    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test OffloadSetVolume API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerOffloadSetVolume_001
 * @tc.desc  : Test OffloadSetVolume interface, volume is OUT_OF_MAX_FLOAT_VOLUME.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerOffloadSetVolume_001, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    float volume = OUT_OF_MAX_FLOAT_VOLUME;
    int32_t ret = rendererInServer->OffloadSetVolume(volume);

    EXPECT_EQ(ERR_INVALID_PARAM, ret);
}

/**
 * @tc.name  : Test UpdateSpatializationState API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerUpdateSpatializationState_001
 * @tc.desc  : Test UpdateSpatializationState interface, Set managerType_ is DIRECT_PLAYBACK.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerUpdateSpatializationState_001, TestSize.Level1)
{
    AudioProcessConfig tempProcessConfig;
    std::shared_ptr<RendererInServer> tempRendererInServer;

    tempProcessConfig.streamInfo = testStreamInfo;
    tempProcessConfig.streamType = STREAM_MUSIC;
    tempProcessConfig.rendererInfo.pipeType = PIPE_TYPE_DIRECT_MUSIC;
    tempProcessConfig.deviceType = DEVICE_TYPE_INVALID;
    tempProcessConfig.rendererInfo.rendererFlags = AUDIO_FLAG_MMAP;
    tempRendererInServer = std::make_shared<RendererInServer>(tempProcessConfig, streamListener);
    EXPECT_NE(nullptr, tempRendererInServer);

    tempRendererInServer->managerType_ = DIRECT_PLAYBACK;
    tempRendererInServer->Init();
    bool spatializationEnabled = true;
    bool headTrackingEnabled = true;
    int32_t ret = tempRendererInServer->UpdateSpatializationState(spatializationEnabled, headTrackingEnabled);

    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test GetStreamManagerType API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerGetStreamManagerType_001
 * @tc.desc  : Test GetStreamManagerType interface, Set managerType_ is DIRECT_PLAYBACK.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerGetStreamManagerType_001, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    rendererInServer->managerType_ = DIRECT_PLAYBACK;
    int32_t ret = rendererInServer->GetStreamManagerType();

    EXPECT_EQ(AUDIO_DIRECT_MANAGER_TYPE, ret);
}

/**
 * @tc.name  : Test GetStreamManagerType API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerGetStreamManagerType_002
 * @tc.desc  : Test GetStreamManagerType interface, Set managerType_ is VOIP_PLAYBACK.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerGetStreamManagerType_002, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    rendererInServer->managerType_ = VOIP_PLAYBACK;
    int32_t ret = rendererInServer->GetStreamManagerType();

    EXPECT_EQ(AUDIO_NORMAL_MANAGER_TYPE, ret);
}

/**
 * @tc.name  : Test IsHighResolution API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerIsHighResolution_001
 * @tc.desc  : Test IsHighResolution interface, Set deviceType is DEVICE_TYPE_INVALID.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerIsHighResolution_001, TestSize.Level1)
{
    AudioProcessConfig tempProcessConfig;
    std::shared_ptr<RendererInServer> tempRendererInServer;

    tempProcessConfig.deviceType = DEVICE_TYPE_INVALID;
    tempRendererInServer = std::make_shared<RendererInServer>(tempProcessConfig, streamListener);
    EXPECT_NE(nullptr, tempRendererInServer);

    EXPECT_FALSE(tempRendererInServer->IsHighResolution());
}

/**
 * @tc.name  : Test IsHighResolution API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerIsHighResolution_002
 * @tc.desc  : Test IsHighResolution interface, Set deviceType is DEVICE_TYPE_USB_HEADSET.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerIsHighResolution_002, TestSize.Level1)
{
    AudioProcessConfig tempProcessConfig;
    std::shared_ptr<RendererInServer> tempRendererInServer;

    tempProcessConfig.deviceType = DEVICE_TYPE_USB_HEADSET;
    tempRendererInServer = std::make_shared<RendererInServer>(tempProcessConfig, streamListener);
    EXPECT_NE(nullptr, tempRendererInServer);

    EXPECT_FALSE(tempRendererInServer->IsHighResolution());
}

/**
 * @tc.name  : Test IsHighResolution API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerIsHighResolution_003
 * @tc.desc  : Test IsHighResolution interface, Set deviceType is DEVICE_TYPE_WIRED_HEADSET, streamType is STREAM_RING.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerIsHighResolution_003, TestSize.Level1)
{
    AudioProcessConfig tempProcessConfig;
    std::shared_ptr<RendererInServer> tempRendererInServer;

    tempProcessConfig.deviceType = DEVICE_TYPE_WIRED_HEADSET;
    tempProcessConfig.streamType = STREAM_RING;
    tempRendererInServer = std::make_shared<RendererInServer>(tempProcessConfig, streamListener);
    EXPECT_NE(nullptr, tempRendererInServer);

    EXPECT_FALSE(tempRendererInServer->IsHighResolution());
}

/**
 * @tc.name  : Test IsHighResolution API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerIsHighResolution_004
 * @tc.desc  : Test IsHighResolution interface, Set deviceType is DEVICE_TYPE_WIRED_HEADSET, streamType is STREAM_MUSIC,
 *             processConfig.streamInfo.samplingRate is SAMPLE_RATE_200000,
 *             processConfig.streamInfo.format is SAMPLE_S32LE,
 *             processConfig.rendererInfo.pipeType is PIPE_TYPE_DIRECT_MUSIC.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerIsHighResolution_004, TestSize.Level1)
{
    AudioProcessConfig tempProcessConfig;
    std::shared_ptr<RendererInServer> tempRendererInServer;

    tempProcessConfig.deviceType = DEVICE_TYPE_WIRED_HEADSET;
    tempProcessConfig.streamType = STREAM_MUSIC;
    tempProcessConfig.streamInfo.samplingRate = static_cast<AudioSamplingRate>(SAMPLE_RATE_200000);
    tempProcessConfig.streamInfo.format = SAMPLE_S32LE;
    tempProcessConfig.rendererInfo.pipeType = PIPE_TYPE_DIRECT_MUSIC;
    tempRendererInServer = std::make_shared<RendererInServer>(tempProcessConfig, streamListener);
    EXPECT_NE(nullptr, tempRendererInServer);

    EXPECT_FALSE(tempRendererInServer->IsHighResolution());
}

/**
 * @tc.name  : Test SetSilentModeAndMixWithOthers API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerSetSilentModeAndMixWithOthers_001
 * @tc.desc  : Test SetSilentModeAndMixWithOthers interface.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerSetSilentModeAndMixWithOthers_001, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    bool on = true;
    int32_t ret = rendererInServer->SetSilentModeAndMixWithOthers(on);

    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_001
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is PLAYBACK.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_001, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = PLAYBACK;

    EXPECT_FALSE(rendererInServer->Dump(dump));
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_002
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is DIRECT_PLAYBACK, status_ is I_STATUS_INVALID.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_002, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = DIRECT_PLAYBACK;
    rendererInServer->status_ = I_STATUS_INVALID;

    EXPECT_TRUE(rendererInServer->Dump(dump));
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_003
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is DIRECT_PLAYBACK, status_ is I_STATUS_IDLE.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_003, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = DIRECT_PLAYBACK;
    rendererInServer->status_ = I_STATUS_IDLE;

    EXPECT_TRUE(rendererInServer->Dump(dump));
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_004
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is DIRECT_PLAYBACK, status_ is I_STATUS_STARTING.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_004, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = DIRECT_PLAYBACK;
    rendererInServer->status_ = I_STATUS_STARTING;

    EXPECT_TRUE(rendererInServer->Dump(dump));
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_005
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is DIRECT_PLAYBACK, status_ is I_STATUS_STARTED.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_005, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = DIRECT_PLAYBACK;
    rendererInServer->status_ = I_STATUS_STARTED;

    EXPECT_TRUE(rendererInServer->Dump(dump));
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_006
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is DIRECT_PLAYBACK, status_ is I_STATUS_PAUSING.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_006, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = DIRECT_PLAYBACK;
    rendererInServer->status_ = I_STATUS_PAUSING;

    EXPECT_TRUE(rendererInServer->Dump(dump));
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_007
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is DIRECT_PLAYBACK, status_ is I_STATUS_PAUSED.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_007, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = DIRECT_PLAYBACK;
    rendererInServer->status_ = I_STATUS_PAUSED;

    EXPECT_TRUE(rendererInServer->Dump(dump));
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_008
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is DIRECT_PLAYBACK,
 *             status_ is I_STATUS_FLUSHING_WHEN_STARTED.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_008, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = DIRECT_PLAYBACK;
    rendererInServer->status_ = I_STATUS_FLUSHING_WHEN_STARTED;

    EXPECT_TRUE(rendererInServer->Dump(dump));
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_009
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is DIRECT_PLAYBACK,
 *             status_ is I_STATUS_FLUSHING_WHEN_PAUSED.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_009, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = DIRECT_PLAYBACK;
    rendererInServer->status_ = I_STATUS_FLUSHING_WHEN_PAUSED;

    EXPECT_TRUE(rendererInServer->Dump(dump));
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_010
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is VOIP_PLAYBACK,
 *             status_ is I_STATUS_FLUSHING_WHEN_STOPPED.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_010, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = VOIP_PLAYBACK;
    rendererInServer->status_ = I_STATUS_FLUSHING_WHEN_STOPPED;

    EXPECT_TRUE(rendererInServer->Dump(dump));
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_011
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is VOIP_PLAYBACK, status_ is I_STATUS_DRAINING.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_011, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = VOIP_PLAYBACK;
    rendererInServer->status_ = I_STATUS_DRAINING;

    EXPECT_TRUE(rendererInServer->Dump(dump));
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_012
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is VOIP_PLAYBACK, status_ is I_STATUS_DRAINED.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_012, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = VOIP_PLAYBACK;
    rendererInServer->status_ = I_STATUS_DRAINED;

    EXPECT_TRUE(rendererInServer->Dump(dump));
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_013
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is VOIP_PLAYBACK, status_ is I_STATUS_STOPPING.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_013, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = VOIP_PLAYBACK;
    rendererInServer->status_ = I_STATUS_STOPPING;

    EXPECT_TRUE(rendererInServer->Dump(dump));
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_014
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is VOIP_PLAYBACK, status_ is I_STATUS_STOPPED.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_014, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = VOIP_PLAYBACK;
    rendererInServer->status_ = I_STATUS_STOPPED;

    EXPECT_TRUE(rendererInServer->Dump(dump));
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_015
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is VOIP_PLAYBACK, status_ is I_STATUS_RELEASING.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_015, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = VOIP_PLAYBACK;
    rendererInServer->status_ = I_STATUS_RELEASING;

    EXPECT_TRUE(rendererInServer->Dump(dump));
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_016
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is VOIP_PLAYBACK, status_ is I_STATUS_RELEASED.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_016, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = VOIP_PLAYBACK;
    rendererInServer->status_ = I_STATUS_RELEASED;

    EXPECT_TRUE(rendererInServer->Dump(dump));
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: RendererInServerDump_017
 * @tc.desc  : Test Dump interface. Set dumpString is "", managerType_ is VOIP_PLAYBACK, status_ is OUT_OF_STATUS.
 */
HWTEST_F(RendererInServerUnitTest, RendererInServerDump_017, TestSize.Level1)
{
    EXPECT_NE(nullptr, rendererInServer);

    std::string dump="";
    rendererInServer->managerType_ = VOIP_PLAYBACK;
    rendererInServer->status_ = static_cast<IStatus>(OUT_OF_STATUS);

    EXPECT_TRUE(rendererInServer->Dump(dump));
}
} // namespace AudioStandard
} // namespace OHOS