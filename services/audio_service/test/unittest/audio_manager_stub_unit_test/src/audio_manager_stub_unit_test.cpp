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

#include "audio_manager_stub_unit_test.h"

#include "audio_manager_base.h"
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

#include <locale>
#include <codecvt>

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {
const int32_t SYSTEM_ABILITY_ID = 3001;
const bool RUN_ON_CREATE = false;

void AudioManagerStubUnitTest::SetUpTestCase(void) {}

void AudioManagerStubUnitTest::TearDownTestCase(void) {}

void AudioManagerStubUnitTest::SetUp(void) {}

void AudioManagerStubUnitTest::TearDown(void) {}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleFourthPartCode_011
* @tc.desc  : Test HandleFourthPartCode interface. Set code value to GET_ASR_NOISE_SUPPRESSION_MODE, Set
*             AsrNoiseSuppressionMode value to Outliers(4).
*/
HWTEST_F(AudioManagerStubUnitTest, HandleFourthPartCode_011, TestSize.Level1)
{
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::GET_ASR_NOISE_SUPPRESSION_MODE);
    MessageParcel data;
    data.WriteInt32(1);
    MessageParcel reply;
    MessageOption option;
    std::shared_ptr<AudioServer> audioServerN = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    auto ret = audioServerN ->HandleFourthPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_ERR, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleFourthPartCode_012
* @tc.desc  : Test HandleFourthPartCode interface. Set code value to GET_ASR_NOISE_SUPPRESSION_MODE, Set
*             AsrNoiseSuppressionMode value to Outliers(4).
*/
HWTEST_F(AudioManagerStubUnitTest, HandleFourthPartCode_012, TestSize.Level1)
{
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::GET_ASR_WHISPER_DETECTION_MODE);
    MessageParcel data;
    data.WriteInt32(1);
    MessageParcel reply;
    MessageOption option;
    std::shared_ptr<AudioServer> audioServerN = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    auto ret = audioServerN ->HandleFourthPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_ERR, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleFourthPartCode_001
* @tc.desc  : Test HandleFourthPartCode interface. Set code value to GET_ASR_NOISE_SUPPRESSION_MODE
*/
HWTEST_F(AudioManagerStubUnitTest, HandleFourthPartCode_001, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::GET_ASR_NOISE_SUPPRESSION_MODE);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    AsrNoiseSuppressionMode asrNoiseSuppressionMode = (static_cast<AsrNoiseSuppressionMode>(0));
    auto set = audioServer->SetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
    EXPECT_EQ(AUDIO_OK, set);
    auto ret = audioServer->HandleFourthPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleFourthPartCode_002
* @tc.desc  : Test HandleFourthPartCode interface. Set code value to SET_ASR_WHISPER_DETECTION_MODE
*/
HWTEST_F(AudioManagerStubUnitTest, HandleFourthPartCode_002, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::SET_ASR_WHISPER_DETECTION_MODE);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    AsrNoiseSuppressionMode asrNoiseSuppressionMode = (static_cast<AsrNoiseSuppressionMode>(0));
    auto set = audioServer->SetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
    EXPECT_EQ(AUDIO_OK, set);
    auto ret = audioServer->HandleFourthPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleFourthPartCode_003
* @tc.desc  : Test HandleFourthPartCode interface. Set code value to GET_ASR_WHISPER_DETECTION_MODE
*/
HWTEST_F(AudioManagerStubUnitTest, HandleFourthPartCode_003, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::GET_ASR_WHISPER_DETECTION_MODE);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    AsrNoiseSuppressionMode asrNoiseSuppressionMode = (static_cast<AsrNoiseSuppressionMode>(0));
    auto set = audioServer->SetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
    EXPECT_EQ(AUDIO_OK, set);
    auto ret = audioServer->HandleFourthPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleFourthPartCode_004
* @tc.desc  : Test HandleFourthPartCode interface. Set code value to SET_ASR_VOICE_CONTROL_MODE
*/
HWTEST_F(AudioManagerStubUnitTest, HandleFourthPartCode_004, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::SET_ASR_VOICE_CONTROL_MODE);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    AsrNoiseSuppressionMode asrNoiseSuppressionMode = (static_cast<AsrNoiseSuppressionMode>(0));
    auto set = audioServer->SetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
    EXPECT_EQ(AUDIO_OK, set);
    auto ret = audioServer->HandleFourthPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleFourthPartCode_005
* @tc.desc  : Test HandleFourthPartCode interface. Set code value to SET_ASR_VOICE_MUTE_MODE
*/
HWTEST_F(AudioManagerStubUnitTest, HandleFourthPartCode_005, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::SET_ASR_VOICE_MUTE_MODE);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    AsrNoiseSuppressionMode asrNoiseSuppressionMode = (static_cast<AsrNoiseSuppressionMode>(0));
    auto set = audioServer->SetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
    EXPECT_EQ(AUDIO_OK, set);
    auto ret = audioServer->HandleFourthPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleFourthPartCode_006
* @tc.desc  : Test HandleFourthPartCode interface. Set code value to IS_WHISPERING
*/
HWTEST_F(AudioManagerStubUnitTest, HandleFourthPartCode_006, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::IS_WHISPERING);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    AsrNoiseSuppressionMode asrNoiseSuppressionMode = (static_cast<AsrNoiseSuppressionMode>(0));
    auto set = audioServer->SetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
    EXPECT_EQ(AUDIO_OK, set);
    auto ret = audioServer->HandleFourthPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleFourthPartCode_007
* @tc.desc  : Test HandleFourthPartCode interface. Set code value to GET_EFFECT_OFFLOAD_ENABLED
*/
HWTEST_F(AudioManagerStubUnitTest, HandleFourthPartCode_007, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::GET_EFFECT_OFFLOAD_ENABLED);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    AsrNoiseSuppressionMode asrNoiseSuppressionMode = (static_cast<AsrNoiseSuppressionMode>(0));
    auto set = audioServer->SetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
    EXPECT_EQ(AUDIO_OK, set);
    auto ret = audioServer->HandleFourthPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleFourthPartCode_008
* @tc.desc  : Test HandleFourthPartCode interface. Set code value to SUSPEND_RENDERSINK
*/
HWTEST_F(AudioManagerStubUnitTest, HandleFourthPartCode_008, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::SUSPEND_RENDERSINK);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    AsrNoiseSuppressionMode asrNoiseSuppressionMode = (static_cast<AsrNoiseSuppressionMode>(0));
    auto set = audioServer->SetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
    EXPECT_EQ(AUDIO_OK, set);
    auto ret = audioServer->HandleFourthPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleFourthPartCode_009
* @tc.desc  : Test HandleFourthPartCode interface. Set code value to RESTORE_RENDERSINK
*/
HWTEST_F(AudioManagerStubUnitTest, HandleFourthPartCode_009, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::RESTORE_RENDERSINK);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    AsrNoiseSuppressionMode asrNoiseSuppressionMode = (static_cast<AsrNoiseSuppressionMode>(0));
    auto set = audioServer->SetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
    EXPECT_EQ(AUDIO_OK, set);
    auto ret = audioServer->HandleFourthPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleFourthPartCode_010
* @tc.desc  : Test HandleFourthPartCode interface. Set code value to LOAD_HDI_EFFECT_MODEL
*/
HWTEST_F(AudioManagerStubUnitTest, HandleFourthPartCode_010, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::LOAD_HDI_EFFECT_MODEL);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    AsrNoiseSuppressionMode asrNoiseSuppressionMode = (static_cast<AsrNoiseSuppressionMode>(0));
    auto set = audioServer->SetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
    EXPECT_EQ(AUDIO_OK, set);
    auto ret = audioServer->HandleFourthPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleFourthPartCode_013
* @tc.desc  : Test HandleFourthPartCode interface. Set code value to UPDATE_EFFECT_BT_OFFLOAD_SUPPORTED
*/
HWTEST_F(AudioManagerStubUnitTest, HandleFourthPartCode_013, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::UPDATE_EFFECT_BT_OFFLOAD_SUPPORTED);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    AsrNoiseSuppressionMode asrNoiseSuppressionMode = (static_cast<AsrNoiseSuppressionMode>(0));
    auto set = audioServer->SetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
    EXPECT_EQ(AUDIO_OK, set);
    auto ret = audioServer->HandleFourthPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleFourthPartCode_014
* @tc.desc  : Test HandleFourthPartCode interface. Set code value to SET_SINK_MUTE_FOR_SWITCH_DEVICE
*/
HWTEST_F(AudioManagerStubUnitTest, HandleFourthPartCode_014, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::SET_SINK_MUTE_FOR_SWITCH_DEVICE);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    AsrNoiseSuppressionMode asrNoiseSuppressionMode = (static_cast<AsrNoiseSuppressionMode>(0));
    auto set = audioServer->SetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
    EXPECT_EQ(AUDIO_OK, set);
    auto ret = audioServer->HandleFourthPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleFourthPartCode_015
* @tc.desc  : Test HandleFourthPartCode interface. Set code value to SET_ROTATION_TO_EFFECT
*/
HWTEST_F(AudioManagerStubUnitTest, HandleFourthPartCode_015, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::SET_ROTATION_TO_EFFECT);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    AsrNoiseSuppressionMode asrNoiseSuppressionMode = (static_cast<AsrNoiseSuppressionMode>(0));
    auto set = audioServer->SetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
    EXPECT_EQ(AUDIO_OK, set);
    auto ret = audioServer->HandleFourthPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleFourthPartCode_016
* @tc.desc  : Test HandleFourthPartCode interface. Set code value to UPDATE_SESSION_CONNECTION_STATE
*/
HWTEST_F(AudioManagerStubUnitTest, HandleFourthPartCode_016, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::UPDATE_SESSION_CONNECTION_STATE);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    AsrNoiseSuppressionMode asrNoiseSuppressionMode = (static_cast<AsrNoiseSuppressionMode>(0));
    auto set = audioServer->SetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
    EXPECT_EQ(AUDIO_OK, set);
    auto ret = audioServer->HandleFourthPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleThirdPartCode_001
* @tc.desc  : Test HandleThirdPartCode interface. Set code value to NOTIFY_STREAM_VOLUME_CHANGED
*/
HWTEST_F(AudioManagerStubUnitTest, HandleThirdPartCode_001, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::NOTIFY_STREAM_VOLUME_CHANGED);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    auto ret = audioServer->HandleThirdPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleThirdPartCode_002
* @tc.desc  : Test HandleThirdPartCode interface. Set code value to SET_SPATIALIZATION_SCENE_TYPE
*/
HWTEST_F(AudioManagerStubUnitTest, HandleThirdPartCode_002, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::SET_SPATIALIZATION_SCENE_TYPE);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    auto ret = audioServer->HandleThirdPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleThirdPartCode_003
* @tc.desc  : Test HandleThirdPartCode interface. Set code value to GET_MAX_AMPLITUDE
*/
HWTEST_F(AudioManagerStubUnitTest, HandleThirdPartCode_003, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::GET_MAX_AMPLITUDE);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    auto ret = audioServer->HandleThirdPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleThirdPartCode_004
* @tc.desc  : Test HandleThirdPartCode interface. Set code value to RESET_AUDIO_ENDPOINT
*/
HWTEST_F(AudioManagerStubUnitTest, HandleThirdPartCode_004, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::RESET_AUDIO_ENDPOINT);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    auto ret = audioServer->HandleThirdPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test HandleFourthPartCode API
* @tc.type  : FUNC
* @tc.number: HandleThirdPartCode_005
* @tc.desc  : Test HandleThirdPartCode interface. Set code value to RESET_ROUTE_FOR_DISCONNECT
*/
HWTEST_F(AudioManagerStubUnitTest, HandleThirdPartCode_005, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::RESET_ROUTE_FOR_DISCONNECT);
    MessageParcel data;
    data.WriteInt32(0);
    MessageParcel reply;
    MessageOption option;
    auto ret = audioServer->HandleThirdPartCode(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test OnRemoteRequest API
* @tc.type  : FUNC
* @tc.number: OnRemoteRequest_001
* @tc.desc  : Test OnRemoteRequest interface. Set code value to AUDIO_SERVER_CODE_MAX
*/
HWTEST_F(AudioManagerStubUnitTest, OnRemoteRequest_001, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::AUDIO_SERVER_CODE_MAX);
    MessageParcel data;
    data.WriteInterfaceToken(u"IStandardAudioService");
    MessageParcel reply;
    MessageOption option;
    AsrNoiseSuppressionMode asrNoiseSuppressionMode = (static_cast<AsrNoiseSuppressionMode>(0));
    auto set = audioServer->SetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
    EXPECT_EQ(AUDIO_OK, set);
    auto ret = audioServer->OnRemoteRequest(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

/**
* @tc.name  : Test OnRemoteRequest API
* @tc.type  : FUNC
* @tc.number: OnRemoteRequest_002
* @tc.desc  : Test OnRemoteRequest interface. Set code value to GET_ASR_NOISE_SUPPRESSION_MODE
*/
HWTEST_F(AudioManagerStubUnitTest, OnRemoteRequest_002, TestSize.Level1)
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    uint32_t format = static_cast<uint32_t>(AudioServerInterfaceCode::GET_ASR_NOISE_SUPPRESSION_MODE);
    MessageParcel data;
    data.WriteInterfaceToken(u"IStandardAudioService");
    MessageParcel reply;
    MessageOption option;
    AsrNoiseSuppressionMode asrNoiseSuppressionMode = (static_cast<AsrNoiseSuppressionMode>(0));
    auto set = audioServer->SetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
    EXPECT_EQ(AUDIO_OK, set);
    auto ret = audioServer->OnRemoteRequest(format, data, reply, option);
    EXPECT_EQ(AUDIO_OK, ret);
}

} // namespace AudioStandard
} // namespace OHOS