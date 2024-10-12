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

#include "audio_policy_service_one_unit_test.h"

#include <thread>
#include <memory>
#include <vector>
using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

const int32_t SYSTEM_ABILITY_ID = 3009;
const bool RUN_ON_CREATE = false;
bool g_hasServerInit = false;
const int32_t TEST_SESSIONID = MIN_SESSIONID + 1010;
const int32_t A2DP_STOPPED = 1;
const int32_t A2DP_PLAYING = 2;
const int32_t A2DP_INVALID = 3;
const uint32_t ROTATE = 1;
const int32_t SESSION_ID = 1000001;
const int32_t STATE = 1;

void AudioPolicyServiceOneUnitTest::SetUpTestCase(void)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest::SetUpTestCase start-end");
}
void AudioPolicyServiceOneUnitTest::TearDownTestCase(void)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest::TearDownTestCase start-end");
}
void AudioPolicyServiceOneUnitTest::SetUp(void)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest::SetUp start-end");
}
void AudioPolicyServiceOneUnitTest::TearDown(void)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest::TearDown start-end");
}

AudioPolicyServer* GetServerPtr()
{
    static AudioPolicyServer server(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (!g_hasServerInit) {
        server.OnStart();
        server.OnAddSystemAbility(AUDIO_DISTRIBUTED_SERVICE_ID, "");
#ifdef FEATURE_MULTIMODALINPUT_INPUT
        server.OnAddSystemAbility(MULTIMODAL_INPUT_SERVICE_ID, "");
#endif
        server.OnAddSystemAbility(DISTRIBUTED_KV_DATA_SERVICE_ABILITY_ID, "");
        server.OnAddSystemAbility(BLUETOOTH_HOST_SYS_ABILITY_ID, "");
        server.OnAddSystemAbility(ACCESSIBILITY_MANAGER_SERVICE_ID, "");
        server.OnAddSystemAbility(POWER_MANAGER_SERVICE_ID, "");
        server.OnAddSystemAbility(SUBSYS_ACCOUNT_SYS_ABILITY_ID_BEGIN, "");
        server.audioPolicyService_.SetDefaultDeviceLoadFlag(true);
        g_hasServerInit = true;
    }
    return &server;
}

static const std::vector<DeviceType> deviceTypes = {
    DEVICE_TYPE_NONE,
    DEVICE_TYPE_INVALID,
    DEVICE_TYPE_EARPIECE,
    DEVICE_TYPE_SPEAKER,
    DEVICE_TYPE_WIRED_HEADSET,
    DEVICE_TYPE_WIRED_HEADPHONES,
    DEVICE_TYPE_BLUETOOTH_SCO,
    DEVICE_TYPE_BLUETOOTH_A2DP,
    DEVICE_TYPE_MIC,
    DEVICE_TYPE_WAKEUP,
    DEVICE_TYPE_USB_HEADSET,
    DEVICE_TYPE_DP,
    DEVICE_TYPE_REMOTE_CAST,
    DEVICE_TYPE_FILE_SINK,
    DEVICE_TYPE_FILE_SOURCE,
    DEVICE_TYPE_EXTERN_CABLE,
    DEVICE_TYPE_DEFAULT,
    DEVICE_TYPE_USB_ARM_HEADSET
};
static const std::vector<bool>isConnecteds = {
    true,
    false
};
static const std::vector<BluetoothOffloadState>flags = {
    NO_A2DP_DEVICE,
    A2DP_NOT_OFFLOAD,
    A2DP_OFFLOAD
};
static const std::vector<AudioPipeType>pipeTypes = {
    PIPE_TYPE_UNKNOWN,
    PIPE_TYPE_NORMAL_OUT,
    PIPE_TYPE_NORMAL_IN,
    PIPE_TYPE_OFFLOAD,
    PIPE_TYPE_MULTICHANNEL
};
static const std::vector<StreamUsage>streamUsages = {
    STREAM_USAGE_INVALID,
    STREAM_USAGE_UNKNOWN,
    STREAM_USAGE_MEDIA,
    STREAM_USAGE_MUSIC,
    STREAM_USAGE_VOICE_COMMUNICATION,
    STREAM_USAGE_VOICE_ASSISTANT,
    STREAM_USAGE_ALARM,
    STREAM_USAGE_VOICE_MESSAGE,
    STREAM_USAGE_NOTIFICATION_RINGTONE,
    STREAM_USAGE_RINGTONE,
    STREAM_USAGE_NOTIFICATION,
    STREAM_USAGE_ACCESSIBILITY,
    STREAM_USAGE_SYSTEM,
    STREAM_USAGE_MOVIE,
    STREAM_USAGE_GAME,
    STREAM_USAGE_AUDIOBOOK,
    STREAM_USAGE_NAVIGATION,
    STREAM_USAGE_DTMF,
    STREAM_USAGE_ENFORCED_TONE,
    STREAM_USAGE_ULTRASONIC,
    STREAM_USAGE_VIDEO_COMMUNICATION,
    STREAM_USAGE_RANGING,
    STREAM_USAGE_VOICE_MODEM_COMMUNICATION,
    STREAM_USAGE_VOICE_RINGTONE,
    STREAM_USAGE_VOICE_CALL_ASSISTANT,
    STREAM_USAGE_MAX
};
static const std::vector<AudioStreamType>audioStreamTypes = {
    STREAM_DEFAULT,
    STREAM_VOICE_CALL,
    STREAM_MUSIC,
    STREAM_RING,
    STREAM_MEDIA,
    STREAM_VOICE_ASSISTANT,
    STREAM_SYSTEM,
    STREAM_ALARM,
    STREAM_NOTIFICATION,
    STREAM_BLUETOOTH_SCO,
    STREAM_ENFORCED_AUDIBLE,
    STREAM_DTMF,
    STREAM_TTS,
    STREAM_ACCESSIBILITY,
    STREAM_RECORDING,
    STREAM_MOVIE,
    STREAM_GAME,
    STREAM_SPEECH,
    STREAM_SYSTEM_ENFORCED,
    STREAM_ULTRASONIC,
    STREAM_WAKEUP,
    STREAM_VOICE_MESSAGE,
    STREAM_NAVIGATION,
    STREAM_INTERNAL_FORCE_STOP,
    STREAM_SOURCE_VOICE_CALL,
    STREAM_VOICE_COMMUNICATION,
    STREAM_VOICE_RING,
    STREAM_VOICE_CALL_ASSISTANT,
    STREAM_TYPE_MAX,
    STREAM_ALL
};
static const std::vector<bool>isMutes = {
    true,
    false
};
static const std::vector<DeviceRole>deviceRoles = {
    DEVICE_ROLE_NONE,
    INPUT_DEVICE,
    OUTPUT_DEVICE,
    DEVICE_ROLE_MAX
};
static const std::vector<DeviceFlag>deviceFlags = {
    NONE_DEVICES_FLAG,
    OUTPUT_DEVICES_FLAG,
    INPUT_DEVICES_FLAG,
    ALL_DEVICES_FLAG,
    DISTRIBUTED_OUTPUT_DEVICES_FLAG,
    DISTRIBUTED_INPUT_DEVICES_FLAG,
    ALL_DISTRIBUTED_DEVICES_FLAG,
    ALL_L_D_DEVICES_FLAG,
    DEVICE_FLAG_MAX
};
static const std::vector<AudioSampleFormat>audioSampleFormats = {
    SAMPLE_U8,
    SAMPLE_S16LE,
    SAMPLE_S24LE,
    SAMPLE_S32LE,
    SAMPLE_F32LE,
    INVALID_WIDTH
};
static const std::vector<AudioScene>audioScenes = {
    AUDIO_SCENE_INVALID,
    AUDIO_SCENE_DEFAULT,
    AUDIO_SCENE_RINGING,
    AUDIO_SCENE_PHONE_CALL,
    AUDIO_SCENE_PHONE_CHAT,
    AUDIO_SCENE_CALL_START,
    AUDIO_SCENE_CALL_END,
    AUDIO_SCENE_VOICE_RINGING,
    AUDIO_SCENE_MAX
};
static const std::vector<AudioRingerMode>audioRingerModes = {
    RINGER_MODE_SILENT,
    RINGER_MODE_VIBRATE,
    RINGER_MODE_NORMAL
};

/**
* @tc.name  : Test GetSupportedAudioEffectProperty.
* @tc.number: GetSupportedAudioEffectProperty_001
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, GetSupportedAudioEffectProperty_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest GetSupportedAudioEffectProperty_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    AudioEffectPropertyArray propertyArray;
    GetServerPtr()->audioPolicyService_.GetSupportedAudioEffectProperty(propertyArray);
}

/**
* @tc.name  : Test LoadHdiEffectModel.
* @tc.number: LoadHdiEffectModel_001
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, LoadHdiEffectModel_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest LoadHdiEffectModel_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    GetServerPtr()->audioPolicyService_.LoadHdiEffectModel();
}

/**
* @tc.name  : Test OnReceiveBluetoothEvent.
* @tc.number: OnReceiveBluetoothEvent_001
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, OnReceiveBluetoothEvent_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest OnReceiveBluetoothEvent_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    const std::string macAddress = "11-11-11-11-11-11";
    const std::string deviceName = "deviceName";
    GetServerPtr()->audioPolicyService_.OnReceiveBluetoothEvent(macAddress, deviceName);
}

/**
* @tc.name  : Test WaitForConnectionCompleted.
* @tc.number: WaitForConnectionCompleted_001
* @tc.desc  : Test AudioPolicyServic interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, WaitForConnectionCompleted_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest WaitForConnectionCompleted_001 start");
    EXPECT_NE(nullptr, GetServerPtr());
    AudioA2dpOffloadManager audioA2dpOffloadManager_(&(GetServerPtr())->audioPolicyService_);

    audioA2dpOffloadManager_.currentOffloadConnectionState_ = CONNECTION_STATUS_CONNECTED;
    audioA2dpOffloadManager_.WaitForConnectionCompleted();
    EXPECT_FALSE(!(audioA2dpOffloadManager_.currentOffloadConnectionState_ = CONNECTION_STATUS_CONNECTED));
}

/**
* @tc.name  : Test WaitForConnectionCompleted.
* @tc.number: WaitForConnectionCompleted_002
* @tc.desc  : Test AudioPolicyServic interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, WaitForConnectionCompleted_002, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest WaitForConnectionCompleted_001 start");
    EXPECT_NE(nullptr, GetServerPtr());
    AudioA2dpOffloadManager audioA2dpOffloadManager_(&(GetServerPtr())->audioPolicyService_);

    audioA2dpOffloadManager_.currentOffloadConnectionState_ = CONNECTION_STATUS_CONNECTING;
    audioA2dpOffloadManager_.WaitForConnectionCompleted();
    EXPECT_FALSE(!(audioA2dpOffloadManager_.currentOffloadConnectionState_ = CONNECTION_STATUS_CONNECTED));
}

/**
* @tc.name  : Test IsA2dpOffloadConnecting.
* @tc.number: IsA2dpOffloadConnecting_001
* @tc.desc  : Test AudioPolicyServic interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, IsA2dpOffloadConnecting_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest IsA2dpOffloadConnecting_001 start");
    EXPECT_NE(nullptr, GetServerPtr());
    AudioA2dpOffloadManager audioA2dpOffloadManager_(&(GetServerPtr())->audioPolicyService_);

    audioA2dpOffloadManager_.currentOffloadConnectionState_ = CONNECTION_STATUS_CONNECTING;
    audioA2dpOffloadManager_.connectionTriggerSessionIds_ = {0};

    bool ret = audioA2dpOffloadManager_.IsA2dpOffloadConnecting(0);
    EXPECT_TRUE(ret);
}

/**
* @tc.name  : Test IsA2dpOffloadConnecting.
* @tc.number: IsA2dpOffloadConnecting_002
* @tc.desc  : Test AudioPolicyServic interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, IsA2dpOffloadConnecting_002, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest IsA2dpOffloadConnecting_001 start");
    EXPECT_NE(nullptr, GetServerPtr());
    AudioA2dpOffloadManager audioA2dpOffloadManager_(&(GetServerPtr())->audioPolicyService_);

    audioA2dpOffloadManager_.currentOffloadConnectionState_ = CONNECTION_STATUS_CONNECTING;
    audioA2dpOffloadManager_.connectionTriggerSessionIds_ = {0};

    bool ret = audioA2dpOffloadManager_.IsA2dpOffloadConnecting(1);
    EXPECT_FALSE(ret);
}

/**
* @tc.name  : Test IsA2dpOffloadConnecting.
* @tc.number: IsA2dpOffloadConnecting_003
* @tc.desc  : Test AudioPolicyServic interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, IsA2dpOffloadConnecting_003, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest IsA2dpOffloadConnecting_001 start");
    EXPECT_NE(nullptr, GetServerPtr());
    AudioA2dpOffloadManager audioA2dpOffloadManager_(&(GetServerPtr())->audioPolicyService_);

    audioA2dpOffloadManager_.currentOffloadConnectionState_ = CONNECTION_STATUS_CONNECTED;
    audioA2dpOffloadManager_.connectionTriggerSessionIds_ = {0};

    bool ret = audioA2dpOffloadManager_.IsA2dpOffloadConnecting(0);
    EXPECT_FALSE(ret);
}

/**
* @tc.name  : Test GetAudioEffectOffloadFlag.
* @tc.number: GetAudioEffectOffloadFlag_001
* @tc.desc  : Test AudioPolicyServic interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, GetAudioEffectOffloadFlag_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest GetAudioEffectOffloadFlag_001 start");
    auto server = GetServerPtr();
    EXPECT_NE(nullptr, server);

    bool ret = server->audioPolicyService_.GetAudioEffectOffloadFlag();
    EXPECT_FALSE(ret);
}

/**
* @tc.name  : Test CheckSpatializationAndEffectState.
* @tc.number: CheckSpatializationAndEffectState_001
* @tc.desc  : Test AudioPolicyServic interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, CheckSpatializationAndEffectState_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest CheckSpatializationAndEffectState_001 start");
    auto server = GetServerPtr();
    EXPECT_NE(nullptr, server);

    bool ret = server->audioPolicyService_.CheckSpatializationAndEffectState();
    EXPECT_FALSE(ret);
}

/**
* @tc.name  : Test UpdateRoute.
* @tc.number: UpdateRoute_001
* @tc.desc  : Test AudioPolicyServic interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, UpdateRoute_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest UpdateRoute_001 start");
    auto server = GetServerPtr();
    EXPECT_NE(nullptr, server);
    vector<std::unique_ptr<AudioDeviceDescriptor>> outputDevices;
    std::unique_ptr<AudioDeviceDescriptor> audioDeviceDescriptor = std::make_unique<AudioDeviceDescriptor>();
    audioDeviceDescriptor->deviceType_ = DEVICE_TYPE_SPEAKER;
    outputDevices.push_back(std::move(audioDeviceDescriptor));
    unique_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_unique<AudioRendererChangeInfo>();
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_ALARM;
    server->audioPolicyService_.UpdateRoute(rendererChangeInfo, outputDevices);
    EXPECT_EQ(true, server->audioPolicyService_.ringerModeMute_);

    std::unique_ptr<AudioDeviceDescriptor> audioDeviceDescriptor1 = std::make_unique<AudioDeviceDescriptor>();
    audioDeviceDescriptor1->deviceType_ = DEVICE_TYPE_WIRED_HEADSET;
    outputDevices.push_back(std::move(audioDeviceDescriptor1));
    std::unique_ptr<AudioDeviceDescriptor> audioDeviceDescriptor2 = std::make_unique<AudioDeviceDescriptor>();
    audioDeviceDescriptor2->deviceType_ = DEVICE_TYPE_WIRED_HEADPHONES;
    outputDevices.push_back(std::move(audioDeviceDescriptor2));
    server->audioPolicyService_.UpdateRoute(rendererChangeInfo, outputDevices);
    EXPECT_EQ(true, server->audioPolicyService_.ringerModeMute_);
    audioDeviceDescriptor.reset();
    audioDeviceDescriptor1.reset();
    audioDeviceDescriptor2.reset();
    rendererChangeInfo.reset();
}

/**
* @tc.name  : Test UpdateRoute.
* @tc.number: UpdateRoute_002
* @tc.desc  : Test AudioPolicyServic interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, UpdateRoute_002, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest UpdateRoute_001 start");
    auto server = GetServerPtr();
    EXPECT_NE(nullptr, server);
    vector<std::unique_ptr<AudioDeviceDescriptor>> outputDevices;
    std::unique_ptr<AudioDeviceDescriptor> audioDeviceDescriptor = std::make_unique<AudioDeviceDescriptor>();
    audioDeviceDescriptor->deviceType_ = DEVICE_TYPE_DP;
    outputDevices.push_back(std::move(audioDeviceDescriptor));
    unique_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_unique<AudioRendererChangeInfo>();
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_MUSIC;
    server->audioPolicyService_.UpdateRoute(rendererChangeInfo, outputDevices);
    EXPECT_EQ(true, server->audioPolicyService_.ringerModeMute_);

    server->audioPolicyService_.enableDualHalToneState_ = true;
    server->audioPolicyService_.UpdateRoute(rendererChangeInfo, outputDevices);
    EXPECT_EQ(true, server->audioPolicyService_.ringerModeMute_);
    audioDeviceDescriptor.reset();
    rendererChangeInfo.reset();
}

/**
* @tc.name  : Test LoadSplitModule.
* @tc.number: LoadSplitModule_001
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, LoadSplitModule_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest LoadSplitModule_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    const std::string networkIdOne = "";
    const std::string splitArgOne = "";
    int32_t result = GetServerPtr()->audioPolicyService_.LoadSplitModule(splitArgOne, networkIdOne);
    EXPECT_EQ(ERR_INVALID_PARAM, result);

    const std::string networkIdTwo = LOCAL_NETWORK_ID;
    const std::string splitArgTwo = "11.22";
    result = GetServerPtr()->audioPolicyService_.LoadSplitModule(splitArgTwo, networkIdTwo);
    EXPECT_EQ(ERR_INVALID_HANDLE, result);
}

/**
* @tc.name  : Test SetDefaultOutputDevice.
* @tc.number: SetDefaultOutputDevice_001
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, SetDefaultOutputDevice_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest SetDefaultOutputDevice_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    DeviceType deviceType = DeviceType::DEVICE_TYPE_WIRED_HEADSET;
    bool isRunning = true;
    const StreamUsage streamUsage = STREAM_USAGE_VOICE_MESSAGE;
    const uint32_t sessionID = 0;

    int32_t result =
        GetServerPtr()->audioPolicyService_.SetDefaultOutputDevice(deviceType, sessionID, streamUsage, isRunning);
    EXPECT_EQ(ERR_NOT_SUPPORTED, result);
    GetServerPtr()->audioPolicyService_.hasEarpiece_ = true;
    result =
        GetServerPtr()->audioPolicyService_.SetDefaultOutputDevice(deviceType, sessionID, streamUsage, isRunning);
    EXPECT_EQ(SUCCESS, result);
}

/**
* @tc.name  : Test UpdateDefaultOutputDeviceWhenStopping.
* @tc.number: UpdateDefaultOutputDeviceWhenStopping_001
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, UpdateDefaultOutputDeviceWhenStopping_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest UpdateDefaultOutputDeviceWhenStopping_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    int32_t uid = getuid();
    GetServerPtr()->audioPolicyService_.UpdateDefaultOutputDeviceWhenStopping(uid);
    EXPECT_EQ(SUCCESS, uid);
}

/**
* @tc.name  : Test OnA2dpPlayingStateChanged.
* @tc.number: OnA2dpPlayingStateChanged_001
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, OnA2dpPlayingStateChanged_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest OnA2dpPlayingStateChanged_001 start");
    ASSERT_NE(nullptr, GetServerPtr());
    AudioA2dpOffloadManager audioA2dpOffloadManager_(&(GetServerPtr())->audioPolicyService_);

    const std::string deviceAddress = "test";
    int32_t playingState = A2DP_STOPPED;
    audioA2dpOffloadManager_.currentOffloadConnectionState_ = CONNECTION_STATUS_CONNECTED;
    audioA2dpOffloadManager_.OnA2dpPlayingStateChanged(deviceAddress, playingState);
    EXPECT_EQ(A2DP_STOPPED, playingState);
    EXPECT_EQ(CONNECTION_STATUS_CONNECTED, audioA2dpOffloadManager_.currentOffloadConnectionState_);
}

/**
* @tc.name  : Test OnA2dpPlayingStateChanged.
* @tc.number: OnA2dpPlayingStateChanged_002
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, OnA2dpPlayingStateChanged_002, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest OnA2dpPlayingStateChanged_002 start");
    ASSERT_NE(nullptr, GetServerPtr());
    AudioA2dpOffloadManager audioA2dpOffloadManager_(&(GetServerPtr())->audioPolicyService_);

    const std::string deviceAddress = "test";
    int32_t playingState = A2DP_STOPPED;
    audioA2dpOffloadManager_.currentOffloadConnectionState_ = CONNECTION_STATUS_DISCONNECTED;
    audioA2dpOffloadManager_.OnA2dpPlayingStateChanged(deviceAddress, playingState);
    EXPECT_EQ(CONNECTION_STATUS_DISCONNECTED, audioA2dpOffloadManager_.currentOffloadConnectionState_);
}

/**
* @tc.name  : Test OnA2dpPlayingStateChanged.
* @tc.number: OnA2dpPlayingStateChanged_003
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, OnA2dpPlayingStateChanged_003, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest OnA2dpPlayingStateChanged_003 start");
    ASSERT_NE(nullptr, GetServerPtr());
    AudioA2dpOffloadManager audioA2dpOffloadManager_(&(GetServerPtr())->audioPolicyService_);

    const std::string deviceAddress = "test";
    int32_t playingState = A2DP_PLAYING;
    audioA2dpOffloadManager_.currentOffloadConnectionState_ = CONNECTION_STATUS_CONNECTED;
    audioA2dpOffloadManager_.OnA2dpPlayingStateChanged(deviceAddress, playingState);
    EXPECT_EQ(CONNECTION_STATUS_DISCONNECTED, audioA2dpOffloadManager_.currentOffloadConnectionState_);
}

/**
* @tc.name  : Test OnA2dpPlayingStateChanged.
* @tc.number: OnA2dpPlayingStateChanged_004
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, OnA2dpPlayingStateChanged_004, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest OnA2dpPlayingStateChanged_004 start");
    ASSERT_NE(nullptr, GetServerPtr());
    AudioA2dpOffloadManager audioA2dpOffloadManager_(&(GetServerPtr())->audioPolicyService_);

    const std::string deviceAddress = "test";
    int32_t playingState = A2DP_PLAYING;
    audioA2dpOffloadManager_.currentOffloadConnectionState_ = CONNECTION_STATUS_DISCONNECTED;
    audioA2dpOffloadManager_.OnA2dpPlayingStateChanged(deviceAddress, playingState);
    EXPECT_EQ(CONNECTION_STATUS_DISCONNECTED, audioA2dpOffloadManager_.currentOffloadConnectionState_);
}

/**
* @tc.name  : Test OnA2dpPlayingStateChanged.
* @tc.number: OnA2dpPlayingStateChanged_005
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, OnA2dpPlayingStateChanged_005, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest OnA2dpPlayingStateChanged_005 start");
    ASSERT_NE(nullptr, GetServerPtr());
    AudioA2dpOffloadManager audioA2dpOffloadManager_(&(GetServerPtr())->audioPolicyService_);

    const std::string deviceAddress = "";
    int32_t playingState = A2DP_PLAYING;
    audioA2dpOffloadManager_.currentOffloadConnectionState_ = CONNECTION_STATUS_CONNECTING;
    audioA2dpOffloadManager_.OnA2dpPlayingStateChanged(deviceAddress, playingState);
    EXPECT_EQ(CONNECTION_STATUS_CONNECTED, audioA2dpOffloadManager_.currentOffloadConnectionState_);
}

/**
* @tc.name  : Test OnA2dpPlayingStateChanged.
* @tc.number: OnA2dpPlayingStateChanged_006
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, OnA2dpPlayingStateChanged_006, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest OnA2dpPlayingStateChanged_006 start");
    ASSERT_NE(nullptr, GetServerPtr());
    AudioA2dpOffloadManager audioA2dpOffloadManager_(&(GetServerPtr())->audioPolicyService_);

    const std::string deviceAddress = "";
    int32_t playingState = A2DP_PLAYING;
    audioA2dpOffloadManager_.currentOffloadConnectionState_ = CONNECTION_STATUS_CONNECTED;
    audioA2dpOffloadManager_.OnA2dpPlayingStateChanged(deviceAddress, playingState);
    EXPECT_EQ(CONNECTION_STATUS_CONNECTED, audioA2dpOffloadManager_.currentOffloadConnectionState_);
}

/**
* @tc.name  : Test OnA2dpPlayingStateChanged.
* @tc.number: OnA2dpPlayingStateChanged_007
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, OnA2dpPlayingStateChanged_007, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest OnA2dpPlayingStateChanged_007 start");
    ASSERT_NE(nullptr, GetServerPtr());
    AudioA2dpOffloadManager audioA2dpOffloadManager_(&(GetServerPtr())->audioPolicyService_);

    const std::string deviceAddress = "";
    int32_t playingState = A2DP_STOPPED;
    audioA2dpOffloadManager_.OnA2dpPlayingStateChanged(deviceAddress, playingState);
    EXPECT_EQ(CONNECTION_STATUS_DISCONNECTED, audioA2dpOffloadManager_.currentOffloadConnectionState_);
}

/**
* @tc.name  : Test OnA2dpPlayingStateChanged.
* @tc.number: OnA2dpPlayingStateChanged_008
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, OnA2dpPlayingStateChanged_008, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest OnA2dpPlayingStateChanged_008 start");
    ASSERT_NE(nullptr, GetServerPtr());
    AudioA2dpOffloadManager audioA2dpOffloadManager_(&(GetServerPtr())->audioPolicyService_);

    const std::string deviceAddress = "";
    int32_t playingState = A2DP_INVALID;
    audioA2dpOffloadManager_.OnA2dpPlayingStateChanged(deviceAddress, playingState);
    EXPECT_NE(A2DP_STOPPED, playingState);
    EXPECT_NE(A2DP_PLAYING, playingState);
}

/**
* @tc.name  : Test ConnectA2dpOffload.
* @tc.number: ConnectA2dpOffload_001
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, ConnectA2dpOffload_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest ConnectA2dpOffload_001 start");
    ASSERT_NE(nullptr, GetServerPtr());
    AudioA2dpOffloadManager audioA2dpOffloadManager_(&(GetServerPtr())->audioPolicyService_);

    const std::string deviceAddress = "";
    const vector<int32_t> sessionIds = {};
    audioA2dpOffloadManager_.currentOffloadConnectionState_ = CONNECTION_STATUS_CONNECTED;
    audioA2dpOffloadManager_.ConnectA2dpOffload(deviceAddress, sessionIds);
    EXPECT_EQ(CONNECTION_STATUS_CONNECTED, audioA2dpOffloadManager_.currentOffloadConnectionState_);
}

/**
* @tc.name  : Test ConnectA2dpOffload.
* @tc.number: ConnectA2dpOffload_002
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, ConnectA2dpOffload_002, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest ConnectA2dpOffload_002 start");
    ASSERT_NE(nullptr, GetServerPtr());
    AudioA2dpOffloadManager audioA2dpOffloadManager_(&(GetServerPtr())->audioPolicyService_);

    const std::string deviceAddress = "";
    const vector<int32_t> sessionIds = {};
    audioA2dpOffloadManager_.currentOffloadConnectionState_ = CONNECTION_STATUS_CONNECTING;
    audioA2dpOffloadManager_.ConnectA2dpOffload(deviceAddress, sessionIds);
    EXPECT_EQ(CONNECTION_STATUS_CONNECTING, audioA2dpOffloadManager_.currentOffloadConnectionState_);
}

/**
* @tc.name  : Test ConnectA2dpOffload.
* @tc.number: ConnectA2dpOffload_003
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, ConnectA2dpOffload_003, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest ConnectA2dpOffload_002 start");
    ASSERT_NE(nullptr, GetServerPtr());
    AudioA2dpOffloadManager audioA2dpOffloadManager_(&(GetServerPtr())->audioPolicyService_);

    const std::string deviceAddress = "";
    const vector<int32_t> sessionIds = {};
    audioA2dpOffloadManager_.currentOffloadConnectionState_ = CONNECTION_STATUS_DISCONNECTED;
    audioA2dpOffloadManager_.ConnectA2dpOffload(deviceAddress, sessionIds);
    EXPECT_EQ(CONNECTION_STATUS_CONNECTING, audioA2dpOffloadManager_.currentOffloadConnectionState_);
}

/**
* @tc.name  : Test GetSupportedAudioEnhanceProperty.
* @tc.number: GetSupportedAudioEnhanceProperty_001
* @tc.desc  : Test GetSupportedAudioEnhanceProperty interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, GetSupportedAudioEnhanceProperty_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest GetSupportedAudioEnhanceProperty_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    AudioEnhancePropertyArray propertyArrayTest;
    GetServerPtr()->GetSupportedAudioEnhanceProperty(propertyArrayTest);
}

/**
* @tc.name  : Test SetAudioEffectProperty.
* @tc.number: SetAudioEffectProperty_001
* @tc.desc  : Test SetAudioEffectProperty interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, SetAudioEffectProperty_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest SetAudioEffectProperty_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    AudioEffectPropertyArray propertyArrayTest;
    GetServerPtr()->SetAudioEffectProperty(propertyArrayTest);
}

/**
* @tc.name  : Test GetAudioEffectProperty.
* @tc.number: GetAudioEffectProperty_001
* @tc.desc  : Test GetAudioEffectProperty interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, GetAudioEffectProperty_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest GetAudioEffectProperty_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    AudioEffectPropertyArray propertyArrayTest;
    GetServerPtr()->GetAudioEffectProperty(propertyArrayTest);
}

/**
* @tc.name  : Test SetAudioEnhanceProperty.
* @tc.number: SetAudioEnhanceProperty_001
* @tc.desc  : Test SetAudioEnhanceProperty interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, SetAudioEnhanceProperty_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest SetAudioEnhanceProperty_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    AudioEnhancePropertyArray propertyArrayTest;
    GetServerPtr()->SetAudioEnhanceProperty(propertyArrayTest);
}

/**
* @tc.name  : Test GetAudioEnhanceProperty.
* @tc.number: GetAudioEnhanceProperty_001
* @tc.desc  : Test GetAudioEnhanceProperty interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, GetAudioEnhanceProperty_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest GetAudioEnhanceProperty_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    AudioEnhancePropertyArray propertyArrayTest;
    GetServerPtr()->GetAudioEnhanceProperty(propertyArrayTest);
}

/**
* @tc.name  : Test GetCurActivateCount.
* @tc.number: GetCurActivateCount_001
* @tc.desc  : Test GetCurActivateCount interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, GetCurActivateCount_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest GetCurActivateCount_001 start");
    std::shared_ptr<AudioPolicyService> service = std::make_shared<AudioPolicyService>();
    ASSERT_NE(nullptr, service);

    service->GetCurActivateCount();
    service.reset();
}

/**
* @tc.name  : Test WriteServiceStartupError.
* @tc.number: WriteServiceStartupError_001
* @tc.desc  : Test WriteServiceStartupError interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, WriteServiceStartupError_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest WriteServiceStartupError_001 start");
    std::shared_ptr<AudioPolicyService> service = std::make_shared<AudioPolicyService>();
    ASSERT_NE(nullptr, service);

    std::string reasonTest = "Reason for test";
    service->WriteServiceStartupError(reasonTest);
    service.reset();
}

/**
* @tc.name  : Test LoadToneDtmfConfig.
* @tc.number: LoadToneDtmfConfig_001
* @tc.desc  : Test LoadToneDtmfConfig interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, LoadToneDtmfConfig_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest LoadToneDtmfConfig_001 start");
    std::shared_ptr<AudioPolicyService> service = std::make_shared<AudioPolicyService>();
    ASSERT_NE(nullptr, service);

    bool ret = service->LoadToneDtmfConfig();
    ASSERT_FALSE(ret);
    service.reset();
}

/**
* @tc.name  : Test SetAudioConcurrencyCallback.
* @tc.number: SetAudioConcurrencyCallback_001
* @tc.desc  : Test SetAudioConcurrencyCallback interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, SetAudioConcurrencyCallback_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest SetAudioConcurrencyCallback_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    sptr<IRemoteObject> objectSptrTest = nullptr;
    GetServerPtr()->SetAudioConcurrencyCallback(TEST_SESSIONID, objectSptrTest);
}

/**
* @tc.name  : Test UnsetAudioConcurrencyCallback.
* @tc.number: UnsetAudioConcurrencyCallback_001
* @tc.desc  : Test UnsetAudioConcurrencyCallback interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, UnsetAudioConcurrencyCallback_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest UnsetAudioConcurrencyCallback_001 start");
    AudioPolicyServer* server = GetServerPtr();
    ASSERT_NE(nullptr, server);

    sptr<IRemoteObject> objectSptrTest = nullptr;
    server->SetAudioConcurrencyCallback(TEST_SESSIONID, objectSptrTest);
    server->UnsetAudioConcurrencyCallback(TEST_SESSIONID);
}

/**
* @tc.name  : Test ActivateAudioConcurrency.
* @tc.number: ActivateAudioConcurrency_001
* @tc.desc  : Test ActivateAudioConcurrency interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, ActivateAudioConcurrency_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest ActivateAudioConcurrency_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    GetServerPtr()->ActivateAudioConcurrency(PIPE_TYPE_UNKNOWN);
}

/**
* @tc.name  : Test ResetRingerModeMute.
* @tc.number: ResetRingerModeMute_001
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, ResetRingerModeMute_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest ResetRingerModeMute_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    GetServerPtr()->audioPolicyService_.ringerModeMute_ = true;
    auto ret = GetServerPtr()->audioPolicyService_.ResetRingerModeMute();
    EXPECT_EQ(SUCCESS, ret);
}

/**
* @tc.name  : Test ResetRingerModeMute.
* @tc.number: ResetRingerModeMute_002
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, ResetRingerModeMute_002, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest ResetRingerModeMute_002 start");
    ASSERT_NE(nullptr, GetServerPtr());

    GetServerPtr()->audioPolicyService_.ringerModeMute_ = false;
    auto ret = GetServerPtr()->audioPolicyService_.ResetRingerModeMute();
    EXPECT_EQ(SUCCESS, ret);
}

/**
* @tc.name  : Test ErasePreferredDeviceByType.
* @tc.number: ErasePreferredDeviceByType_001
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, ErasePreferredDeviceByType_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest ErasePreferredDeviceByType_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    GetServerPtr()->audioPolicyService_.isBTReconnecting_ = true;
    const PreferredType preferredType = AUDIO_MEDIA_RENDER;
    auto ret = GetServerPtr()->audioPolicyService_.ErasePreferredDeviceByType(preferredType);
    EXPECT_EQ(SUCCESS, ret);
}

/**
* @tc.name  : Test ErasePreferredDeviceByType.
* @tc.number: ErasePreferredDeviceByType_002
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, ErasePreferredDeviceByType_002, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest ErasePreferredDeviceByType_002 start");
    ASSERT_NE(nullptr, GetServerPtr());

    const PreferredType preferredType = AUDIO_MEDIA_RENDER;
    auto ret = GetServerPtr()->audioPolicyService_.ErasePreferredDeviceByType(preferredType);
    EXPECT_EQ(SUCCESS, ret);
}

/**
* @tc.name  : Test UpdateSessionConnectionState.
* @tc.number: UpdateSessionConnectionState_001
* @tc.desc  : Test UpdateSessionConnectionState interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, UpdateSessionConnectionState_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest UpdateSessionConnectionState_001 start");
    auto server = GetServerPtr();
    EXPECT_NE(nullptr, server);
    int32_t sessionID = SESSION_ID;
    int32_t state = STATE;
    server->audioPolicyService_.UpdateSessionConnectionState(sessionID, state);
    EXPECT_NE(nullptr, server->audioPolicyService_.GetAudioServerProxy());
}

/**
* @tc.name  : Test IsRingerOrAlarmerDualDevicesRange.
* @tc.number: IsRingerOrAlarmerDualDevicesRange_001
* @tc.desc  : Test AudioPolicyServic interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, IsRingerOrAlarmerDualDevicesRange_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest IsRingerOrAlarmerDualDevicesRange_001 start");
    auto server = GetServerPtr();
    EXPECT_NE(nullptr, server);

    InternalDeviceType deviceType = DEVICE_TYPE_SPEAKER;
    bool ret = server->audioPolicyService_.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(true, ret);

    deviceType = DEVICE_TYPE_WIRED_HEADSET;
    ret = server->audioPolicyService_.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(true, ret);

    deviceType = DEVICE_TYPE_WIRED_HEADPHONES;
    ret = server->audioPolicyService_.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(true, ret);

    deviceType = DEVICE_TYPE_BLUETOOTH_SCO;
    ret = server->audioPolicyService_.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(true, ret);
}

/**
* @tc.name  : Test IsRingerOrAlarmerDualDevicesRange.
* @tc.number: IsRingerOrAlarmerDualDevicesRange_002
* @tc.desc  : Test AudioPolicyServic interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, IsRingerOrAlarmerDualDevicesRange_002, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest IsRingerOrAlarmerDualDevicesRange_002 start");
    auto server = GetServerPtr();
    EXPECT_NE(nullptr, server);

    InternalDeviceType deviceType = DEVICE_TYPE_BLUETOOTH_A2DP;
    bool ret = server->audioPolicyService_.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(true, ret);

    deviceType = DEVICE_TYPE_USB_HEADSET;
    ret = server->audioPolicyService_.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(true, ret);

    deviceType = DEVICE_TYPE_USB_ARM_HEADSET;
    ret = server->audioPolicyService_.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(true, ret);

    deviceType = DEVICE_TYPE_DP;
    ret = server->audioPolicyService_.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(false, ret);
}

/**
* @tc.name  : Test IsA2dpOrArmUsbDevice.
* @tc.number: IsA2dpOrArmUsbDevice_001
* @tc.desc  : Test AudioPolicyServic interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, IsA2dpOrArmUsbDevice_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest IsA2dpOrArmUsbDevice_001 start");
    auto server = GetServerPtr();
    EXPECT_NE(nullptr, server);

    InternalDeviceType deviceType = DEVICE_TYPE_BLUETOOTH_A2DP;
    bool ret = server->audioPolicyService_.IsA2dpOrArmUsbDevice(deviceType);
    EXPECT_EQ(true, ret);

    deviceType = DEVICE_TYPE_USB_ARM_HEADSET;
    ret = server->audioPolicyService_.IsA2dpOrArmUsbDevice(deviceType);
    EXPECT_EQ(true, ret);

    deviceType = DEVICE_TYPE_DP;
    ret = server->audioPolicyService_.IsA2dpOrArmUsbDevice(deviceType);
    EXPECT_EQ(false, ret);
}

/**
* @tc.name  : Test UpdateEffectBtOffloadSupported.
* @tc.number: UpdateEffectBtOffloadSupported_001
* @tc.desc  : Test AudioPolicyServic interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, UpdateEffectBtOffloadSupported_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest UpdateEffectBtOffloadSupported_001 start");
    auto server = GetServerPtr();
    EXPECT_NE(nullptr, server);
    bool isSupported = false;
    server->audioPolicyService_.UpdateEffectBtOffloadSupported(isSupported);
    EXPECT_NE(nullptr, server->audioPolicyService_.GetAudioServerProxy());
}

/**
* @tc.name  : Test ScoInputDeviceFetchedForRecongnition.
* @tc.number: ScoInputDeviceFetchedForRecongnition_001
* @tc.desc  : Test ScoInputDeviceFetchedForRecongnition interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, ScoInputDeviceFetchedForRecongnition_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest ScoInputDeviceFetchedForRecongnition_001 start");
    auto server = GetServerPtr();
    EXPECT_NE(nullptr, server);
    std::string address = "";

    bool handleFlag = false;
    ConnectState connectState = DEACTIVE_CONNECTED;
    int32_t result =
        server->audioPolicyService_.ScoInputDeviceFetchedForRecongnition(handleFlag, address, connectState);
    EXPECT_EQ(ERROR, result);

    handleFlag = true;
    connectState = VIRTUAL_CONNECTED;
    result = server->audioPolicyService_.ScoInputDeviceFetchedForRecongnition(handleFlag, address, connectState);
    EXPECT_EQ(SUCCESS, result);
}

/**
* @tc.name  : Test SetRotationToEffect.
* @tc.number: SetRotationToEffect_001
* @tc.desc  : Test AudioPolicyServic interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, SetRotationToEffect_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest SetRotationToEffect_001 start");
    auto server = GetServerPtr();
    EXPECT_NE(nullptr, server);
    uint32_t rotate = ROTATE;
    server->audioPolicyService_.SetRotationToEffect(rotate);
    EXPECT_NE(nullptr, server->audioPolicyService_.GetAudioServerProxy());
}

/**
* @tc.name  : Test DealAudioSceneOutputDevices.
* @tc.number: DealAudioSceneOutputDevices_001
* @tc.desc  : Test IsA2dpOffloadConnected interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, DealAudioSceneOutputDevices_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest DealAudioSceneOutputDevices_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    const AudioScene audioScene = AUDIO_SCENE_RINGING;
    std::vector<DeviceType> activeOutputDevices;
    bool haveArmUsbDevice = false;
    GetServerPtr()->audioPolicyService_.DealAudioSceneOutputDevices(audioScene, activeOutputDevices, haveArmUsbDevice);
    EXPECT_EQ(false, haveArmUsbDevice);

    const AudioScene audioScene2 = AUDIO_SCENE_VOICE_RINGING;
    haveArmUsbDevice = false;
    GetServerPtr()->audioPolicyService_.DealAudioSceneOutputDevices(audioScene2, activeOutputDevices, haveArmUsbDevice);
    EXPECT_EQ(false, haveArmUsbDevice);

    const AudioScene audioScene3 = AUDIO_SCENE_DEFAULT;
    vector<std::unique_ptr<AudioDeviceDescriptor>> descs {};
    haveArmUsbDevice = false;
    GetServerPtr()->audioPolicyService_.DealAudioSceneOutputDevices(audioScene3, activeOutputDevices, haveArmUsbDevice);
    EXPECT_TRUE(descs.empty());
    EXPECT_EQ(false, haveArmUsbDevice);
}

/**
* @tc.name  : Test IsA2dpOffloadConnected.
* @tc.number: IsA2dpOffloadConnected_001
* @tc.desc  : Test IsA2dpOffloadConnected interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, IsA2dpOffloadConnected_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest IsA2dpOffloadConnected_001 start");
    std::shared_ptr<AudioPolicyService> server = std::make_unique<AudioPolicyService>();
    EXPECT_NE(nullptr, server);
    bool ret = server->IsA2dpOffloadConnected();
    EXPECT_EQ(true, ret);

    server->Init();
    ret = server->IsA2dpOffloadConnected();
    EXPECT_EQ(false, ret);
}

/**
* @tc.name  : Test SelectRingerOrAlarmDevices.
* @tc.number: SelectRingerOrAlarmDevices_001
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, SelectRingerOrAlarmDevices_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest SelectRingerOrAlarmDevices_001 start");
    ASSERT_NE(nullptr, GetServerPtr());
    vector<std::unique_ptr<AudioDeviceDescriptor>> descs1;
    unique_ptr<AudioRendererChangeInfo> rendererChangeInfo1 = std::make_unique<AudioRendererChangeInfo>();
    bool result =GetServerPtr()->audioPolicyService_.SelectRingerOrAlarmDevices(descs1, rendererChangeInfo1);
    EXPECT_EQ(false, result);

    vector<std::unique_ptr<AudioDeviceDescriptor>> descs2;
    std::unique_ptr<AudioDeviceDescriptor> audioDeviceDescriptor2 = std::make_unique<AudioDeviceDescriptor>();
    audioDeviceDescriptor2->deviceType_ = DEVICE_TYPE_SPEAKER;
    descs2.push_back(std::move(audioDeviceDescriptor2));
    unique_ptr<AudioRendererChangeInfo> rendererChangeInfo2 = std::make_unique<AudioRendererChangeInfo>();
    rendererChangeInfo2->rendererInfo.streamUsage = STREAM_USAGE_ALARM;
    rendererChangeInfo2->sessionId = TEST_SESSIONID;
    result =GetServerPtr()->audioPolicyService_.SelectRingerOrAlarmDevices(descs2, rendererChangeInfo2);
    EXPECT_EQ(true, result);

    vector<std::unique_ptr<AudioDeviceDescriptor>> descs3;
    std::unique_ptr<AudioDeviceDescriptor> audioDeviceDescriptor3 = std::make_unique<AudioDeviceDescriptor>();
    audioDeviceDescriptor3->deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    descs3.push_back(std::move(audioDeviceDescriptor3));
    unique_ptr<AudioRendererChangeInfo> rendererChangeInfo3 = std::make_unique<AudioRendererChangeInfo>();
    rendererChangeInfo3->rendererInfo.streamUsage = STREAM_USAGE_VOICE_MESSAGE;
    rendererChangeInfo3->sessionId = TEST_SESSIONID;
    GetServerPtr()->audioPolicyService_.enableDualHalToneState_ = true;
    GetServerPtr()->audioPolicyService_.audioPolicyManager_.SetRingerMode(RINGER_MODE_VIBRATE);
    result =GetServerPtr()->audioPolicyService_.SelectRingerOrAlarmDevices(descs3, rendererChangeInfo3);
    EXPECT_EQ(false, result);

    vector<std::unique_ptr<AudioDeviceDescriptor>> descs4;
    std::unique_ptr<AudioDeviceDescriptor> audioDeviceDescriptor4 = std::make_unique<AudioDeviceDescriptor>();
    audioDeviceDescriptor4->deviceType_ = DEVICE_TYPE_EXTERN_CABLE;
    descs3.push_back(std::move(audioDeviceDescriptor4));
    unique_ptr<AudioRendererChangeInfo> rendererChangeInfo4 = std::make_unique<AudioRendererChangeInfo>();
    rendererChangeInfo4->rendererInfo.streamUsage = STREAM_USAGE_ALARM;
    rendererChangeInfo4->sessionId = TEST_SESSIONID;
    GetServerPtr()->audioPolicyService_.enableDualHalToneState_ = true;
    result =GetServerPtr()->audioPolicyService_.SelectRingerOrAlarmDevices(descs4, rendererChangeInfo4);
    EXPECT_EQ(false, result);

    rendererChangeInfo1.reset();
    rendererChangeInfo2.reset();
    rendererChangeInfo3.reset();
    rendererChangeInfo4.reset();
    audioDeviceDescriptor2.reset();
    audioDeviceDescriptor3.reset();
    audioDeviceDescriptor4.reset();
}

/**
* @tc.name  : Test SetPreferredDevice.
* @tc.number: SetPreferredDevice_001
* @tc.desc  : Test AudioPolicyService interfaces.
*/
HWTEST_F(AudioPolicyServiceOneUnitTest, SetPreferredDevice_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyServiceOneUnitTest SetPreferredDevice_001 start");
    ASSERT_NE(nullptr, GetServerPtr());

    sptr<AudioDeviceDescriptor> audioDeviceDescriptorSptr1 = nullptr;
    int32_t result =
        GetServerPtr()->audioPolicyService_.SetPreferredDevice(AUDIO_MEDIA_RENDER, audioDeviceDescriptorSptr1);
    EXPECT_EQ(SUCCESS, result);

    sptr<AudioDeviceDescriptor> audioDeviceDescriptorSptr2 = new AudioDeviceDescriptor();
    audioDeviceDescriptorSptr2->deviceType_ = DEVICE_TYPE_NONE;
    result = GetServerPtr()->audioPolicyService_.SetPreferredDevice(AUDIO_CALL_RENDER, audioDeviceDescriptorSptr2);
    EXPECT_EQ(SUCCESS, result);

    result = GetServerPtr()->audioPolicyService_.SetPreferredDevice(AUDIO_CALL_CAPTURE, audioDeviceDescriptorSptr2);
    EXPECT_EQ(SUCCESS, result);

    result = GetServerPtr()->audioPolicyService_.SetPreferredDevice(AUDIO_RECORD_CAPTURE, audioDeviceDescriptorSptr2);
    EXPECT_EQ(SUCCESS, result);

    result = GetServerPtr()->audioPolicyService_.SetPreferredDevice(AUDIO_RING_RENDER, audioDeviceDescriptorSptr2);
    EXPECT_EQ(ERR_INVALID_PARAM, result);

    result = GetServerPtr()->audioPolicyService_.SetPreferredDevice(AUDIO_TONE_RENDER, audioDeviceDescriptorSptr2);
    EXPECT_EQ(ERR_INVALID_PARAM, result);

    uint32_t preferredType = 6;
    PreferredType ERR_PFTYPE = static_cast<PreferredType>(preferredType);
    result = GetServerPtr()->audioPolicyService_.SetPreferredDevice(ERR_PFTYPE, audioDeviceDescriptorSptr2);
    EXPECT_EQ(ERR_INVALID_PARAM, result);
    delete audioDeviceDescriptorSptr2;
    audioDeviceDescriptorSptr2 = nullptr;
}
} // namespace AudioStandard
} // namespace OHOS
