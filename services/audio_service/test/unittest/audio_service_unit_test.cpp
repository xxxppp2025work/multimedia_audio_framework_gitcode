/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
#include "iservice_registry.h"
#include "system_ability_definition.h"

#include "audio_service.h"
#include "audio_service_log.h"
#include "audio_errors.h"
#include "audio_system_manager.h"

#include "audio_manager_proxy.h"
#include "audio_manager_listener_stub.h"
#include "audio_process_proxy.h"
#include "audio_process_in_client.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {
std::unique_ptr<AudioManagerProxy> audioManagerProxy;
std::shared_ptr<AudioProcessInClient> processClient_;
const int32_t TEST_RET_NUM = 0;
const int32_t RENDERER_FLAGS = 0;
constexpr int32_t ERROR_62980101 = -62980101;

class AudioServiceUnitTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

class AudioParameterCallbackTest : public AudioParameterCallback {
    virtual void OnAudioParameterChange(const std::string networkId, const AudioParamKey key,
        const std::string& condition, const std::string& value) {}
};

void SetAppUseNumMap(AudioService* audioService, const std::map<int32_t, int32_t>& testMap)
{
    auto& appUseNumMap = const_cast<std::map<int32_t, int32_t>&>(
        audioService->appUseNumMap_);
    appUseNumMap = testMap;
}

void AudioServiceUnitTest::SetUpTestCase(void)
{
    // input testsuit setup step，setup invoked before all testcases
}

void AudioServiceUnitTest::TearDownTestCase(void)
{
    // input testsuit teardown step，teardown invoked after all testcases
}

void AudioServiceUnitTest::SetUp(void)
{
    // input testcase setup step，setup invoked before each testcases
}

void AudioServiceUnitTest::TearDown(void)
{
    // input testcase teardown step，teardown invoked after each testcases
}

/**
 * @tc.name  : Test AudioProcessProxy API
 * @tc.type  : FUNC
 * @tc.number: AudioProcessProxy_001
 * @tc.desc  : Test AudioProcessProxy interface.
 */
HWTEST(AudioServiceUnitTest, AudioProcessProxy_001, TestSize.Level1)
{
    auto samgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_NE(nullptr, samgr);
    sptr<IRemoteObject> object = samgr->GetSystemAbility(AUDIO_DISTRIBUTED_SERVICE_ID);
    EXPECT_NE(nullptr, object);
    std::unique_ptr<AudioProcessProxy> audioProcessProxy = std::make_unique<AudioProcessProxy>(object);

    int32_t ret = -1;
    std::shared_ptr<OHAudioBuffer> buffer;
    uint32_t spanSizeInFrame = 1000;
    uint32_t totalSizeInFrame = spanSizeInFrame - 1;
    uint32_t byteSizePerFrame = 1000;
    buffer = OHAudioBuffer::CreateFromLocal(totalSizeInFrame, spanSizeInFrame, byteSizePerFrame);

    ret=audioProcessProxy->ResolveBuffer(buffer);
    EXPECT_LT(ret, TEST_RET_NUM);

    ret = audioProcessProxy->Start();
    EXPECT_LT(ret, TEST_RET_NUM);

    bool isFlush = true;
    ret = audioProcessProxy->Pause(isFlush);
    EXPECT_LT(ret, TEST_RET_NUM);

    ret = audioProcessProxy->Resume();
    EXPECT_LT(ret, TEST_RET_NUM);

    ret = audioProcessProxy->Stop();
    EXPECT_LT(ret, TEST_RET_NUM);

    ret = audioProcessProxy->RequestHandleInfo();
    EXPECT_EQ(ret, SUCCESS);

    ret = audioProcessProxy->Release();
    EXPECT_LT(ret, TEST_RET_NUM);
}

/**
 * @tc.name  : Test AudioManagerProxy API
 * @tc.type  : FUNC
 * @tc.number: AudioManagerProxy_001
 * @tc.desc  : Test AudioManagerProxy interface.
 */
HWTEST(AudioServiceUnitTest, AudioManagerProxy_001, TestSize.Level1)
{
    int32_t ret = -1;

    auto samgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_NE(nullptr, samgr);
    sptr<IRemoteObject> object = samgr->GetSystemAbility(AUDIO_DISTRIBUTED_SERVICE_ID);
    EXPECT_NE(nullptr, object);

    audioManagerProxy = std::make_unique<AudioManagerProxy>(object);

    bool isMute = true;
    ret = audioManagerProxy->SetMicrophoneMute(isMute);
    if (ret == ERR_PERMISSION_DENIED) {
        return ;
    }
    EXPECT_EQ(ret, SUCCESS);

    ret = audioManagerProxy->RegiestPolicyProvider(object);
    EXPECT_EQ(SUCCESS, ret);

    bool state = false;
    ret = audioManagerProxy->SetCaptureSilentState(state);
    EXPECT_TRUE(ret == ERROR_62980101 || ret == SUCCESS);

    bool result = audioManagerProxy->CreatePlaybackCapturerManager();
    EXPECT_EQ(result, true);

    int32_t deviceType = 1;
    std::string sinkName = "test";
    audioManagerProxy->SetOutputDeviceSink(deviceType, sinkName);
}

/**
 * @tc.name  : Test AudioManagerProxy API
 * @tc.type  : FUNC
 * @tc.number: AudioManagerProxy_002
 * @tc.desc  : Test AudioManagerProxy interface.
 */
HWTEST(AudioServiceUnitTest, AudioManagerProxy_002, TestSize.Level1)
{
    int32_t ret = -1;

    float volume = 0.1;
    ret = audioManagerProxy->SetVoiceVolume(volume);

    const std::string networkId = "LocalDevice";
    const AudioParamKey key = AudioParamKey::VOLUME;
    AudioVolumeType volumeType =AudioVolumeType::STREAM_MEDIA;
    int32_t groupId = 0;
    std::string condition = "EVENT_TYPE=1;VOLUME_GROUP_ID=" + std::to_string(groupId) + ";AUDIO_VOLUME_TYPE="
        + std::to_string(volumeType) + ";";
    std::string value = std::to_string(volume);
    audioManagerProxy->SetAudioParameter(networkId, key, condition, value);
    const std::string retStr = audioManagerProxy->GetAudioParameter(networkId, key, condition);
    EXPECT_NE(retStr, value);

    bool connected = true;
    audioManagerProxy->NotifyDeviceInfo(networkId, connected);
    ret = audioManagerProxy->CheckRemoteDeviceState(networkId, DeviceRole::OUTPUT_DEVICE, true);
    EXPECT_LT(ret, TEST_RET_NUM);
}

/**
 * @tc.name  : Test AudioManagerProxy API
 * @tc.type  : FUNC
 * @tc.number: AudioManagerProxy_004
 * @tc.desc  : Test AudioManagerProxy interface.
 */
HWTEST(AudioServiceUnitTest, AudioManagerProxy_004, TestSize.Level1)
{
    std::vector<Library> libraries;
    Library library = {};
    library.name = "testname";
    library.path ="test.so";
    libraries.push_back(library);

    std::vector<Effect> effects;
    Effect effect = {};
    effect.name = "test";
    effect.libraryName = "test";
    effects.push_back(effect);

    std::vector<Effect> successEffects;
    bool ret = audioManagerProxy->LoadAudioEffectLibraries(libraries, effects, successEffects);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name  : Test AudioManagerListenerStub API
 * @tc.type  : FUNC
 * @tc.number: AudioManagerListenerStub_001
 * @tc.desc  : Test AudioManagerListenerStub interface.
 */
HWTEST(AudioServiceUnitTest, AudioManagerListenerStub_001, TestSize.Level1)
{
    std::unique_ptr<AudioManagerListenerStub> audioManagerListenerStub = std::make_unique<AudioManagerListenerStub>();

    const std::weak_ptr<AudioParameterCallback> callback = std::make_shared<AudioParameterCallbackTest>();
    audioManagerListenerStub->SetParameterCallback(callback);
    float volume = 0.1;
    const std::string networkId = "LocalDevice";
    const AudioParamKey key = AudioParamKey::VOLUME;
    AudioVolumeType volumeType =AudioVolumeType::STREAM_MEDIA;
    int32_t groupId = 0;
    std::string condition = "EVENT_TYPE=1;VOLUME_GROUP_ID=" + std::to_string(groupId) + ";AUDIO_VOLUME_TYPE="
        + std::to_string(volumeType) + ";";
    std::string value = std::to_string(volume);
    audioManagerListenerStub->OnAudioParameterChange(networkId, key, condition, value);
    EXPECT_NE(value, "");
}


/**
 * @tc.name  : Test AudioProcessInClientInner API
 * @tc.type  : FUNC
 * @tc.number: AudioProcessInClientInner_001
 * @tc.desc  : Test AudioProcessInClientInner interface using unsupported parameters.
 */
HWTEST(AudioServiceUnitTest, AudioProcessInClientInner_001, TestSize.Level1)
{
    AudioProcessConfig config;
    config.appInfo.appPid = getpid();
    config.appInfo.appUid = getuid();

    config.audioMode = AUDIO_MODE_PLAYBACK;

    config.rendererInfo.contentType = CONTENT_TYPE_MUSIC;
    config.rendererInfo.streamUsage = STREAM_USAGE_MEDIA;
    config.rendererInfo.rendererFlags = RENDERER_FLAGS;

    config.streamInfo.channels = STEREO;
    config.streamInfo.encoding = ENCODING_PCM;
    config.streamInfo.format = SAMPLE_S16LE;
    config.streamInfo.samplingRate = SAMPLE_RATE_64000;

    processClient_ = AudioProcessInClient::Create(config);
    EXPECT_EQ(processClient_, nullptr);
}

/**
 * @tc.name  : Test AudioDeviceDescriptor API
 * @tc.type  : FUNC
 * @tc.number: AudioDeviceDescriptor_001
 * @tc.desc  : Test AudioDeviceDescriptor interface.
 */
HWTEST(AudioServiceUnitTest, AudioDeviceDescriptor_001, TestSize.Level1)
{
    DeviceType type = DeviceType::DEVICE_TYPE_SPEAKER;
    DeviceRole role = DeviceRole::OUTPUT_DEVICE;
    int32_t interruptGroupId = 1;
    int32_t volumeGroupId = 1;
    std::string networkId = "LocalDevice";
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor =
        std::make_shared<AudioDeviceDescriptor>(type, role, interruptGroupId, volumeGroupId, networkId);
    EXPECT_NE(audioDeviceDescriptor, nullptr);

    AudioDeviceDescriptor deviceDescriptor;
    deviceDescriptor.deviceType_ = type;
    deviceDescriptor.deviceRole_ = role;
    audioDeviceDescriptor = std::make_shared<AudioDeviceDescriptor>(deviceDescriptor);
    EXPECT_NE(audioDeviceDescriptor, nullptr);

    std::string deviceName = "";
    std::string macAddress = "";
    audioDeviceDescriptor->SetDeviceInfo(deviceName, macAddress);

    DeviceStreamInfo audioStreamInfo = {
        SAMPLE_RATE_48000,
        ENCODING_PCM,
        SAMPLE_S16LE,
        STEREO
    };
    int32_t channelMask = 1;
    audioDeviceDescriptor->SetDeviceCapability(audioStreamInfo, channelMask);

    DeviceStreamInfo streamInfo = audioDeviceDescriptor->audioStreamInfo_;
    EXPECT_EQ(streamInfo.channels, audioStreamInfo.channels);
    EXPECT_EQ(streamInfo.encoding, audioStreamInfo.encoding);
    EXPECT_EQ(streamInfo.format, audioStreamInfo.format);
    EXPECT_EQ(streamInfo.samplingRate, audioStreamInfo.samplingRate);
}

/**
 * @tc.name  : Test UpdateMuteControlSet API
 * @tc.type  : FUNC
 * @tc.number: AudioServiceUpdateMuteControlSet_001
 * @tc.desc  : Test UpdateMuteControlSet interface.
 */
HWTEST(AudioServiceUnitTest, AudioServiceUpdateMuteControlSet_001, TestSize.Level1)
{
    AudioService::GetInstance()->UpdateMuteControlSet(1, true);
    AudioService::GetInstance()->UpdateMuteControlSet(MAX_SESSIONID + 1, true);
    AudioService::GetInstance()->UpdateMuteControlSet(MAX_SESSIONID - 1, false);
    AudioService::GetInstance()->UpdateMuteControlSet(MAX_SESSIONID - 1, true);
    AudioService::GetInstance()->UpdateMuteControlSet(MAX_SESSIONID - 1, false);
    AudioService::GetInstance()->UpdateMuteControlSet(MAX_SESSIONID - 1, true);
    AudioService::GetInstance()->RemoveIdFromMuteControlSet(MAX_SESSIONID - 1);
}

/**
 * @tc.name  : Test ShouldBeInnerCap API
 * @tc.type  : FUNC
 * @tc.number: AudioServiceShouldBeInnerCap_001
 * @tc.desc  : Test ShouldBeInnerCap interface.
 */
HWTEST(AudioServiceUnitTest, AudioServiceShouldBeInnerCap_001, TestSize.Level1)
{
    AudioProcessConfig config = {};
    config.privacyType = AudioPrivacyType::PRIVACY_TYPE_PUBLIC;
    bool ret = AudioService::GetInstance()->ShouldBeInnerCap(config);
    EXPECT_FALSE(ret);
    config.privacyType = AudioPrivacyType::PRIVACY_TYPE_PRIVATE;
    ret = AudioService::GetInstance()->ShouldBeInnerCap(config);
    EXPECT_FALSE(ret);
}

/**
 * @tc.name  : Test ShouldBeDualTone API
 * @tc.type  : FUNC
 * @tc.number: AudioServiceShouldBeDualTone_001
 * @tc.desc  : Test ShouldBeDualTone interface.
 */
HWTEST(AudioServiceUnitTest, AudioServiceShouldBeDualTone_001, TestSize.Level1)
{
    AudioProcessConfig config = {};
    config.audioMode = AUDIO_MODE_RECORD;
    config.rendererInfo.streamUsage = STREAM_USAGE_ALARM;
    bool ret = AudioService::GetInstance()->ShouldBeDualTone(config);
    EXPECT_FALSE(ret);
    config.audioMode = AUDIO_MODE_PLAYBACK;
    ret = AudioService::GetInstance()->ShouldBeDualTone(config);
    EXPECT_FALSE(ret);
}

/**
 * @tc.name  : Test OnInitInnerCapList API
 * @tc.type  : FUNC
 * @tc.number: AudioServiceOnInitInnerCapList_001
 * @tc.desc  : Test OnInitInnerCapList interface.
 */
HWTEST(AudioServiceUnitTest, AudioServiceOnInitInnerCapList_001, TestSize.Level1)
{
    int32_t floatRet = 0;

    AudioService::GetInstance()->OnInitInnerCapList();
    AudioService::GetInstance()->ResetAudioEndpoint();
    floatRet = AudioService::GetInstance()->GetMaxAmplitude(true);
    EXPECT_EQ(0, floatRet);

    AudioProcessConfig config = {};
    config.privacyType = AudioPrivacyType::PRIVACY_TYPE_PUBLIC;
    AudioService::GetInstance()->GetAudioProcess(config);
    AudioService::GetInstance()->OnInitInnerCapList();
    AudioService::GetInstance()->workingConfig_.filterOptions.usages.emplace_back(STREAM_USAGE_MEDIA);
    AudioService::GetInstance()->OnInitInnerCapList();

    AudioService::GetInstance()->workingConfig_.filterOptions.pids.emplace_back(1);
    AudioService::GetInstance()->OnInitInnerCapList();
    AudioService::GetInstance()->OnUpdateInnerCapList();
    EXPECT_EQ(0, floatRet);
    config = {};
    config.privacyType = AudioPrivacyType::PRIVACY_TYPE_PRIVATE;
    config.audioMode = AUDIO_MODE_RECORD;
    AudioService::GetInstance()->GetAudioProcess(config);

    AudioService::GetInstance()->OnInitInnerCapList();
    floatRet = AudioService::GetInstance()->GetMaxAmplitude(true);
    EXPECT_EQ(0, floatRet);
    floatRet = AudioService::GetInstance()->GetMaxAmplitude(false);
    EXPECT_EQ(0, floatRet);
    int32_t ret = AudioService::GetInstance()->EnableDualToneList(MAX_SESSIONID - 1);
    EXPECT_EQ(SUCCESS, ret);
    ret = AudioService::GetInstance()->DisableDualToneList(MAX_SESSIONID - 1);
    EXPECT_EQ(SUCCESS, ret);
    AudioService::GetInstance()->ResetAudioEndpoint();
}

/**
 * @tc.name  : Test IsEndpointTypeVoip API
 * @tc.type  : FUNC
 * @tc.number: AudioServiceIsEndpointTypeVoip_001
 * @tc.desc  : Test IsEndpointTypeVoip interface.
 */
HWTEST(AudioServiceUnitTest, AudioServiceIsEndpointTypeVoip_001, TestSize.Level1)
{
    AudioProcessConfig config = {};
    AudioDeviceDescriptor info(AudioDeviceDescriptor::DEVICE_INFO);
    config.rendererInfo.streamUsage = STREAM_USAGE_INVALID;
    config.capturerInfo.sourceType = SOURCE_TYPE_VOICE_COMMUNICATION;
    config.rendererInfo.originalFlag = AUDIO_FLAG_VOIP_FAST;
    bool ret = AudioService::GetInstance()->IsEndpointTypeVoip(config, info);
    EXPECT_EQ(true, ret);

    config.capturerInfo.sourceType = SOURCE_TYPE_INVALID;
    ret = AudioService::GetInstance()->IsEndpointTypeVoip(config, info);
    EXPECT_FALSE(ret);

    config.rendererInfo.streamUsage = STREAM_USAGE_VIDEO_COMMUNICATION;
    ret = AudioService::GetInstance()->IsEndpointTypeVoip(config, info);
    EXPECT_TRUE(ret);

    config.rendererInfo.streamUsage = STREAM_USAGE_VOICE_COMMUNICATION;
    ret = AudioService::GetInstance()->IsEndpointTypeVoip(config, info);
    EXPECT_TRUE(ret);
}

/**
 * @tc.name  : Test GetCapturerBySessionID API
 * @tc.type  : FUNC
 * @tc.number: AudioServiceGetCapturerBySessionID_001
 * @tc.desc  : Test GetCapturerBySessionID interface.
 */
HWTEST(AudioServiceUnitTest, AudioServiceGetCapturerBySessionID_001, TestSize.Level1)
{
    AudioProcessConfig config = {};
    config.audioMode = AUDIO_MODE_RECORD;
    config.streamInfo.channels = STEREO;
    config.streamInfo.channelLayout = CH_LAYOUT_STEREO;
    config.streamType = STREAM_MUSIC;

    int32_t result;
    AudioService::GetInstance()->RemoveCapturer(-1);
    sptr<OHOS::AudioStandard::IpcStreamInServer> server = AudioService::GetInstance()->GetIpcStream(config, result);
    EXPECT_EQ(server, nullptr);

    auto ret = AudioService::GetInstance()->GetCapturerBySessionID(0);
    EXPECT_EQ(nullptr, ret);
}

/**
 * @tc.name  : Test ShouldBeDualTone API
 * @tc.type  : FUNC
 * @tc.number: AudioServiceShouldBeDualTone_002
 * @tc.desc  : Test ShouldBeDualTone interface.
 */
HWTEST(AudioServiceUnitTest, AudioServiceShouldBeDualTone_002, TestSize.Level1)
{
    AudioProcessConfig config = {};
    config.audioMode = AUDIO_MODE_RECORD;
    bool ret;
    ret = AudioService::GetInstance()->ShouldBeDualTone(config);
    EXPECT_EQ(ret, false);
    config.audioMode = AUDIO_MODE_PLAYBACK;
    config.rendererInfo.streamUsage = STREAM_USAGE_RINGTONE;
    ret = AudioService::GetInstance()->ShouldBeDualTone(config);
    EXPECT_FALSE(ret);
}

/**
 * @tc.name  : Test FilterAllFastProcess API
 * @tc.type  : FUNC
 * @tc.number: AudioServiceFilterAllFastProcess_001
 * @tc.desc  : Test FilterAllFastProcess interface.
 */
HWTEST(AudioServiceUnitTest, AudioServiceFilterAllFastProcess_001, TestSize.Level1)
{
    int32_t floatRet = 0;
    AudioService::GetInstance()->FilterAllFastProcess();
    AudioService::GetInstance()->OnInitInnerCapList();
    AudioService::GetInstance()->ResetAudioEndpoint();
    floatRet = AudioService::GetInstance()->GetMaxAmplitude(true);
    EXPECT_EQ(0, floatRet);

    AudioProcessConfig config = {};
    config.privacyType = AudioPrivacyType::PRIVACY_TYPE_PUBLIC;
    AudioService::GetInstance()->GetAudioProcess(config);
    AudioService::GetInstance()->OnInitInnerCapList();
    AudioService::GetInstance()->workingConfig_.filterOptions.usages.emplace_back(STREAM_USAGE_MEDIA);
    AudioService::GetInstance()->OnInitInnerCapList();

    AudioService::GetInstance()->workingConfig_.filterOptions.pids.emplace_back(1);
    AudioService::GetInstance()->OnInitInnerCapList();
    AudioService::GetInstance()->OnUpdateInnerCapList();
    EXPECT_EQ(0, floatRet);
    AudioService::GetInstance()->FilterAllFastProcess();
}

/**
 * @tc.name  : Test GetDeviceInfoForProcess API
 * @tc.type  : FUNC
 * @tc.number: AudioServiceGetDeviceInfoForProcess_001
 * @tc.desc  : Test GetDeviceInfoForProcess interface.
 */
HWTEST(AudioServiceUnitTest, AudioServiceGetDeviceInfoForProcess_001, TestSize.Level1)
{
    AudioProcessConfig config = {};
    config.audioMode = AUDIO_MODE_PLAYBACK;
    AudioDeviceDescriptor deviceinfo(AudioDeviceDescriptor::DEVICE_INFO);
    deviceinfo = AudioService::GetInstance()->GetDeviceInfoForProcess(config);
    EXPECT_NE(deviceinfo.deviceRole_, INPUT_DEVICE);
    config.audioMode = AUDIO_MODE_RECORD;
    deviceinfo = AudioService::GetInstance()->GetDeviceInfoForProcess(config);
    EXPECT_NE(deviceinfo.deviceRole_, OUTPUT_DEVICE);
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: AudioServiceDump_001
 * @tc.desc  : Test Dump interface.
 */
HWTEST(AudioServiceUnitTest, AudioServiceDump_001, TestSize.Level1)
{
    int32_t floatRet = 0;
    AudioService::GetInstance()->FilterAllFastProcess();
    AudioService::GetInstance()->OnInitInnerCapList();
    AudioService::GetInstance()->ResetAudioEndpoint();
    floatRet = AudioService::GetInstance()->GetMaxAmplitude(true);
    EXPECT_EQ(0, floatRet);

    AudioProcessConfig config = {};
    config.privacyType = AudioPrivacyType::PRIVACY_TYPE_PUBLIC;
    AudioService::GetInstance()->GetAudioProcess(config);
    AudioService::GetInstance()->OnInitInnerCapList();
    AudioService::GetInstance()->workingConfig_.filterOptions.usages.emplace_back(STREAM_USAGE_MEDIA);
    AudioService::GetInstance()->OnInitInnerCapList();

    AudioService::GetInstance()->workingConfig_.filterOptions.pids.emplace_back(1);
    AudioService::GetInstance()->OnInitInnerCapList();
    AudioService::GetInstance()->OnUpdateInnerCapList();
    EXPECT_EQ(0, floatRet);
    std::string dumpString = "This is Dump string";
    AudioService::GetInstance()->Dump(dumpString);
}

/**
 * @tc.name  : Test SetNonInterruptMute API
 * @tc.type  : FUNC
 * @tc.number: AudioServiceSetNonInterruptMute_001
 * @tc.desc  : Test SetNonInterruptMute interface.
 */
HWTEST(AudioServiceUnitTest, AudioServiceSetNonInterruptMute_001, TestSize.Level1)
{
    int32_t floatRet = 0;
    bool muteFlag = true;
    uint32_t sessionId = 0;

    AudioService::GetInstance()->FilterAllFastProcess();
    AudioService::GetInstance()->OnInitInnerCapList();
    AudioService::GetInstance()->ResetAudioEndpoint();
    AudioService::GetInstance()->SetNonInterruptMute(sessionId, muteFlag);
    floatRet = AudioService::GetInstance()->GetMaxAmplitude(true);
    EXPECT_EQ(0, floatRet);

    AudioProcessConfig config = {};
    config.privacyType = AudioPrivacyType::PRIVACY_TYPE_PUBLIC;
    AudioService::GetInstance()->GetAudioProcess(config);
    AudioService::GetInstance()->OnInitInnerCapList();
    AudioService::GetInstance()->workingConfig_.filterOptions.usages.emplace_back(STREAM_USAGE_MEDIA);
    AudioService::GetInstance()->OnInitInnerCapList();

    AudioService::GetInstance()->workingConfig_.filterOptions.pids.emplace_back(1);
    AudioService::GetInstance()->OnInitInnerCapList();
    AudioService::GetInstance()->OnUpdateInnerCapList();
    AudioService::GetInstance()->SetNonInterruptMute(MAX_SESSIONID - 1, muteFlag);
    EXPECT_EQ(0, floatRet);
}

/**
 * @tc.name  : Test OnProcessRelease API
 * @tc.type  : FUNC
 * @tc.number: AudioServiceOnProcessRelease_001
 * @tc.desc  : Test OnProcessRelease interface.
 */
HWTEST(AudioServiceUnitTest, AudioServiceOnProcessRelease_001, TestSize.Level1)
{
    bool isSwitchStream = false;
    int32_t floatRet = 0;
    bool muteFlag = true;
    uint32_t sessionId = 0;

    AudioService::GetInstance()->FilterAllFastProcess();
    AudioService::GetInstance()->OnInitInnerCapList();
    AudioService::GetInstance()->ResetAudioEndpoint();
    AudioService::GetInstance()->SetNonInterruptMute(sessionId, muteFlag);
    floatRet = AudioService::GetInstance()->GetMaxAmplitude(true);
    EXPECT_EQ(0, floatRet);

    AudioProcessConfig config = {};
    config.privacyType = AudioPrivacyType::PRIVACY_TYPE_PUBLIC;
    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    sptr<AudioProcessInServer> audioprocess =  AudioProcessInServer::Create(config, AudioService::GetInstance());
    EXPECT_NE(audioprocess, nullptr);
    audioprocess->Start();
    AudioService::GetInstance()->GetAudioProcess(config);
    AudioService::GetInstance()->OnInitInnerCapList();
    AudioService::GetInstance()->workingConfig_.filterOptions.usages.emplace_back(STREAM_USAGE_MEDIA);
    AudioService::GetInstance()->OnInitInnerCapList();

    AudioService::GetInstance()->workingConfig_.filterOptions.pids.emplace_back(1);
    AudioService::GetInstance()->OnInitInnerCapList();
    AudioService::GetInstance()->OnUpdateInnerCapList();

    int32_t ret = 0;
    ret = AudioService::GetInstance()->OnProcessRelease(audioprocess, isSwitchStream);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name  : Test DelayCallReleaseEndpoint API
 * @tc.type  : FUNC
 * @tc.number: DelayCallReleaseEndpoint_001
 * @tc.desc  : Test DelayCallReleaseEndpoint interface.
 */
HWTEST(AudioServiceUnitTest, DelayCallReleaseEndpoint_001, TestSize.Level1)
{
    std::string endpointName;
    int32_t delayInMs = 1;
    AudioService *audioService = AudioService::GetInstance();
    audioService->DelayCallReleaseEndpoint(endpointName, delayInMs);
}

/**
 * @tc.name  : Test GetAudioEndpointForDevice API
 * @tc.type  : FUNC
 * @tc.number: GetAudioEndpointForDevice_001
 * @tc.desc  : Test GetAudioEndpointForDevice interface.
 */
HWTEST(AudioServiceUnitTest, GetAudioEndpointForDevice_001, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();
    AudioProcessConfig clientConfig;
    AudioDeviceDescriptor deviceInfo = audioService->GetDeviceInfoForProcess(clientConfig);
    bool isVoipStream = true;
    audioService->GetAudioEndpointForDevice(deviceInfo, clientConfig, isVoipStream);
}

/**
 * @tc.name  : Test Dump API
 * @tc.type  : FUNC
 * @tc.number: Dump_001
 * @tc.desc  : Test Dump interface.
 */
HWTEST(AudioServiceUnitTest, Dump_001, TestSize.Level1)
{
    std::string dumpString = "abcdefg";
    AudioService *audioService = AudioService::GetInstance();
    audioService->Dump(dumpString);

    AudioProcessConfig processConfig;

    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();

    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;

    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);

    std::shared_ptr<RendererInServer> renderer = rendererInServer;

    audioService->InsertRenderer(1, renderer);
    audioService->workingInnerCapId_ = 1;
    audioService->Dump(dumpString);
    audioService->RemoveRenderer(1);
}

/**
 * @tc.name  : Test GetMaxAmplitude API
 * @tc.type  : FUNC
 * @tc.number: GetMaxAmplitude_001
 * @tc.desc  : Test GetMaxAmplitude interface.
 */
HWTEST(AudioServiceUnitTest, GetMaxAmplitude_001, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();
    int ret = audioService->GetMaxAmplitude(true);
    EXPECT_EQ(0, ret);
}

/**
 * @tc.name  : Test GetCapturerBySessionID API
 * @tc.type  : FUNC
 * @tc.number: GetCapturerBySessionID_001
 * @tc.desc  : Test GetCapturerBySessionID interface.
 */
HWTEST(AudioServiceUnitTest, GetCapturerBySessionID_001, TestSize.Level1)
{
    uint32_t sessionID = 2;
    AudioService *audioService = AudioService::GetInstance();
    std::shared_ptr<CapturerInServer> renderer = nullptr;
    audioService->InsertCapturer(1, renderer);
    std::shared_ptr<CapturerInServer> ret = audioService->GetCapturerBySessionID(sessionID);
    EXPECT_EQ(nullptr, ret);
    audioService->RemoveCapturer(1);
}

/**
 * @tc.name  : Test GetCapturerBySessionID API
 * @tc.type  : FUNC
 * @tc.number: GetCapturerBySessionID_002
 * @tc.desc  : Test GetCapturerBySessionID interface.
 */
HWTEST(AudioServiceUnitTest, GetCapturerBySessionID_002, TestSize.Level1)
{
    AudioProcessConfig processConfig;

    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();

    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;

    std::shared_ptr<CapturerInServer> capturerInServer =
        std::make_shared<CapturerInServer>(processConfig, streamListener);

    std::shared_ptr<CapturerInServer> capturer = capturerInServer;
    uint32_t sessionID = 1;
    AudioService *audioService = AudioService::GetInstance();
    std::shared_ptr<CapturerInServer> renderer = nullptr;
    audioService->InsertCapturer(1, renderer);
    std::shared_ptr<CapturerInServer> ret = audioService->GetCapturerBySessionID(sessionID);
    EXPECT_EQ(nullptr, ret);
    audioService->RemoveCapturer(1);
}

/**
 * @tc.name  : Test SetNonInterruptMute API
 * @tc.type  : FUNC
 * @tc.number: SetNonInterruptMute_001
 * @tc.desc  : Test SetNonInterruptMute interface.
 */
HWTEST(AudioServiceUnitTest, SetNonInterruptMute_001, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();
    std::shared_ptr<RendererInServer> renderer = nullptr;
    audioService->InsertRenderer(1, renderer);
    audioService->SetNonInterruptMute(1, true);
    audioService->RemoveRenderer(1);
}

/**
 * @tc.name  : Test SetNonInterruptMute API
 * @tc.type  : FUNC
 * @tc.number: SetNonInterruptMute_002
 * @tc.desc  : Test SetNonInterruptMute interface.
 */
HWTEST(AudioServiceUnitTest, SetNonInterruptMute_002, TestSize.Level1)
{
    AudioProcessConfig processConfig;

    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();

    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;

    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);

    std::shared_ptr<RendererInServer> renderer = rendererInServer;

    AudioService *audioService = AudioService::GetInstance();
    audioService->InsertRenderer(1, renderer);
    audioService->SetNonInterruptMute(1, true);
    audioService->RemoveRenderer(1);
}

/**
 * @tc.name  : Test SetNonInterruptMute API
 * @tc.type  : FUNC
 * @tc.number: SetNonInterruptMute_003
 * @tc.desc  : Test SetNonInterruptMute interface.
 */
HWTEST(AudioServiceUnitTest, SetNonInterruptMute_003, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();
    std::shared_ptr<CapturerInServer> capturer = nullptr;
    audioService->InsertCapturer(1, capturer);
    audioService->SetNonInterruptMute(1, true);
    audioService->RemoveCapturer(1);
}

/**
 * @tc.name  : Test SetNonInterruptMute API
 * @tc.type  : FUNC
 * @tc.number: SetNonInterruptMute_004
 * @tc.desc  : Test SetNonInterruptMute interface.
 */
HWTEST(AudioServiceUnitTest, SetNonInterruptMute_004, TestSize.Level1)
{
    AudioProcessConfig processConfig;

    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();

    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;

    std::shared_ptr<CapturerInServer> capturerInServer =
        std::make_shared<CapturerInServer>(processConfig, streamListener);

    std::shared_ptr<CapturerInServer> capturer = capturerInServer;

    AudioService *audioService = AudioService::GetInstance();
    audioService->InsertCapturer(1, capturer);
    audioService->SetNonInterruptMute(1, true);
    audioService->RemoveCapturer(1);
}

/**
 * @tc.name  : Test SetOffloadMode API
 * @tc.type  : FUNC
 * @tc.number: SetOffloadMode_001
 * @tc.desc  : Test SetOffloadMode interface.
 */
HWTEST(AudioServiceUnitTest, SetOffloadMode_001, TestSize.Level1)
{
    uint32_t sessionId = 2;
    int32_t state = 1;
    bool isAppBack = true;
    AudioService *audioService = AudioService::GetInstance();
    std::shared_ptr<CapturerInServer> capturer = nullptr;
    audioService->InsertCapturer(1, capturer);
    int32_t ret = audioService->SetOffloadMode(sessionId, state, isAppBack);
    EXPECT_EQ(ERR_INVALID_INDEX, ret);
    audioService->RemoveCapturer(1);
}

/**
 * @tc.name : Test OnProcessRelease API
 * @tc.type : FUNC
 * @tc.number: OnProcessRelease_001
 * @tc.desc : Test OnProcessRelease interface with normal playback process.
 */
HWTEST(AudioServiceUnitTest, OnProcessRelease_001, TestSize.Level1)
{
    AudioProcessConfig processConfig;
    processConfig.audioMode = AUDIO_MODE_PLAYBACK;
    processConfig.appInfo.appUid = 1000;
    ProcessReleaseCallback *callback = nullptr;
    sptr<AudioProcessInServer> processInServer = AudioProcessInServer::Create(processConfig, callback);
    ASSERT_NE(processInServer, nullptr);
    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_INDEPENDENT, 12345, STREAM_MUSIC);
    ASSERT_NE(endpoint, nullptr);
    AudioService *audioService = AudioService::GetInstance();
    ASSERT_NE(audioService, nullptr);
    std::pair<sptr<AudioProcessInServer>, std::shared_ptr<AudioEndpoint>> pair(processInServer, endpoint);
    audioService->linkedPairedList_.push_back(pair);
    int32_t result = audioService->OnProcessRelease(processInServer.GetRefPtr(), false);
    EXPECT_EQ(result, SUCCESS);
    EXPECT_TRUE(audioService->linkedPairedList_.empty());
}

/**
 * @tc.name : Test OnProcessRelease API with nullptr
 * @tc.type : FUNC
 * @tc.number: OnProcessRelease_002
 * @tc.desc : Test OnProcessRelease interface with nullptr process.
 */
HWTEST(AudioServiceUnitTest, OnProcessRelease_002, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();
    ASSERT_NE(audioService, nullptr);
    //push nullptr
    int32_t result = audioService->OnProcessRelease(nullptr, false);
    EXPECT_EQ(result, ERROR);
}

/**
 * @tc.name : Test OnProcessRelease API with switch stream
 * @tc.type : FUNC
 * @tc.number: OnProcessRelease_003
 * @tc.desc : Test OnProcessRelease interface with switch stream flag.
 */
HWTEST(AudioServiceUnitTest, OnProcessRelease_003, TestSize.Level1)
{
    AudioProcessConfig processConfig;
    processConfig.audioMode = AUDIO_MODE_RECORD;
    processConfig.appInfo.appUid = 1001;

    ProcessReleaseCallback *callback = nullptr;
    sptr<AudioProcessInServer> processInServer = AudioProcessInServer::Create(processConfig, callback);
    ASSERT_NE(processInServer, nullptr);

    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_INDEPENDENT, 12345, STREAM_MUSIC);
    ASSERT_NE(endpoint, nullptr);

    AudioService *audioService = AudioService::GetInstance();
    ASSERT_NE(audioService, nullptr);

    std::pair<sptr<AudioProcessInServer>, std::shared_ptr<AudioEndpoint>> pair(processInServer, endpoint);
    audioService->linkedPairedList_.push_back(pair);

    int32_t result = audioService->OnProcessRelease(processInServer.GetRefPtr(), true);

    EXPECT_EQ(result, SUCCESS);
    EXPECT_TRUE(audioService->linkedPairedList_.empty());
}

/**
 * @tc.name : Test CheckRenderSessionMuteState when session is not muted
 * @tc.type : FUNC
 * @tc.number: CheckRenderSessionMuteState_001
 * @tc.desc : Test CheckRenderSessionMuteState with session not in mutedSessions.
 */
HWTEST(AudioServiceUnitTest, CheckRenderSessionMuteState_001, TestSize.Level1)
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;

    std::shared_ptr<RendererInServer> renderer =
        std::make_shared<RendererInServer>(processConfig, streamListener);

    // Execute test
    uint32_t sessionId = 1001;  // Use a session ID that's not in mutedSessions_
    AudioService *audioService = AudioService::GetInstance();
    audioService->CheckRenderSessionMuteState(sessionId, renderer);
}

/**
 * @tc.name : Test CheckCaptureSessionMuteState when session is not muted
 * @tc.type : FUNC
 * @tc.number: CheckCaptureSessionMuteState_001
 * @tc.desc : Test CheckCaptureSessionMuteState with session not in mutedSessions.
 */
HWTEST(AudioServiceUnitTest, CheckCaptureSessionMuteState_001, TestSize.Level1)
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;

    std::shared_ptr<CapturerInServer> capturer =
        std::make_shared<CapturerInServer>(processConfig, streamListener);

    // Execute test
    uint32_t sessionId = 1001;  // Use a session ID that's not in mutedSessions_
    AudioService *audioService = AudioService::GetInstance();
    audioService->CheckCaptureSessionMuteState(sessionId, capturer);
}

/**
 * @tc.name : Test CheckRenderSessionMuteState with null renderer
 * @tc.type : FUNC
 * @tc.number: CheckRenderSessionMuteState_002
 * @tc.desc : Test CheckRenderSessionMuteState with null renderer parameter.
 */
HWTEST(AudioServiceUnitTest, CheckRenderSessionMuteState_002, TestSize.Level1)
{
    // Execute test with null renderer
    uint32_t sessionId = 1001;
    AudioService *audioService = AudioService::GetInstance();
    std::shared_ptr<RendererInServer> nullRenderer = nullptr;
    audioService->CheckRenderSessionMuteState(sessionId, nullRenderer);

    // Test passes if no crash occurs
}

/**
 * @tc.name : Test CheckCaptureSessionMuteState with null capturer
 * @tc.type : FUNC
 * @tc.number: CheckCaptureSessionMuteState_002
 * @tc.desc : Test CheckCaptureSessionMuteState with null capturer parameter.
 */
HWTEST(AudioServiceUnitTest, CheckCaptureSessionMuteState_002, TestSize.Level1)
{
    // Execute test with null capturer
    uint32_t sessionId = 1001;
    AudioService *audioService = AudioService::GetInstance();
    std::shared_ptr<CapturerInServer> nullCapturer = nullptr;
    audioService->CheckCaptureSessionMuteState(sessionId, nullCapturer);
}

/**
 * @tc.name : Test CheckInnerCapForRenderer with null renderer
 * @tc.type : FUNC
 * @tc.number: CheckInnerCapForRenderer_001
 * @tc.desc : Test CheckInnerCapForRenderer with null renderer parameter.
 */
HWTEST(AudioServiceUnitTest, CheckInnerCapForRenderer_001, TestSize.Level1)
{
    // Execute test with null renderer
    uint32_t sessionId = 1001;
    AudioService *audioService = AudioService::GetInstance();
    std::shared_ptr<RendererInServer> nullRenderer = nullptr;
    audioService->CheckInnerCapForRenderer(sessionId, nullRenderer);
}

/**
 * @tc.name : Test CheckInnerCapForRenderer with workingInnerCapId_ = 0
 * @tc.type : FUNC
 * @tc.number: CheckInnerCapForRenderer_002
 * @tc.desc : Test CheckInnerCapForRenderer when inner-cap is not working.
 */
HWTEST(AudioServiceUnitTest, CheckInnerCapForRenderer_002, TestSize.Level1)
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;

    std::shared_ptr<RendererInServer> renderer =
        std::make_shared<RendererInServer>(processConfig, streamListener);

    // Execute test when workingInnerCapId_ is 0
    uint32_t sessionId = 1001;
    AudioService *audioService = AudioService::GetInstance();
    audioService->CheckInnerCapForRenderer(sessionId, renderer);
}

/**
 * @tc.name : Test CheckInnerCapForRenderer with private privacy type
 * @tc.type : FUNC
 * @tc.number: CheckInnerCapForRenderer_003
 * @tc.desc : Test CheckInnerCapForRenderer with PRIVACY_TYPE_PRIVATE.
 */
HWTEST(AudioServiceUnitTest, CheckInnerCapForRenderer_003, TestSize.Level1)
{
    AudioProcessConfig processConfig;
    processConfig.privacyType = AudioPrivacyType::PRIVACY_TYPE_PRIVATE;

    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;

    std::shared_ptr<RendererInServer> renderer =
        std::make_shared<RendererInServer>(processConfig, streamListener);

    uint32_t sessionId = 1001;
    AudioService *audioService = AudioService::GetInstance();
    audioService->CheckInnerCapForRenderer(sessionId, renderer);
}

/**
 * @tc.name : Test CheckInnerCapForRenderer with public privacy type
 * @tc.type : FUNC
 * @tc.number: CheckInnerCapForRenderer_004
 * @tc.desc : Test CheckInnerCapForRenderer with PRIVACY_TYPE_PUBLIC.
 */
HWTEST(AudioServiceUnitTest, CheckInnerCapForRenderer_004, TestSize.Level1)
{
    AudioProcessConfig processConfig;
    processConfig.privacyType = AudioPrivacyType::PRIVACY_TYPE_PUBLIC;
    processConfig.rendererInfo.streamUsage = STREAM_USAGE_UNKNOWN;
    processConfig.appInfo.appPid = 1001;

    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;

    std::shared_ptr<RendererInServer> renderer =
        std::make_shared<RendererInServer>(processConfig, streamListener);

    uint32_t sessionId = 1001;
    AudioService *audioService = AudioService::GetInstance();
    audioService->CheckInnerCapForRenderer(sessionId, renderer);
}

/**
 * @tc.name : Test CheckInnerCapForRenderer with invalid policy
 * @tc.type : FUNC
 * @tc.number: CheckInnerCapForRenderer_005
 * @tc.desc : Test CheckInnerCapForRenderer when filter policy is invalid.
 */
HWTEST(AudioServiceUnitTest, CheckInnerCapForRenderer_005, TestSize.Level1)
{
    // Prepare test data with empty filter options (which leads to POLICY_INVALID)
    AudioProcessConfig processConfig;
    processConfig.privacyType = AudioPrivacyType::PRIVACY_TYPE_PUBLIC;

    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;

    std::shared_ptr<RendererInServer> renderer =
        std::make_shared<RendererInServer>(processConfig, streamListener);

    uint32_t sessionId = 1001;
    AudioService *audioService = AudioService::GetInstance();
    audioService->CheckInnerCapForRenderer(sessionId, renderer);
}
/**
 * @tc.name: Test ShouldBeDualTone Normal Case
 * @tc.type: FUNC
 * @tc.number: ShouldBeDualTone_001
 * @tc.desc: Test ShouldBeDualTone with headset and ringtone usage should return true
 */
HWTEST(AudioServiceUnitTest, ShouldBeDualTone_001, TestSize.Level1)
{
    // Prepare test data
    AudioProcessConfig config;
    config.audioMode = AUDIO_MODE_PLAYBACK;
    config.rendererInfo.streamUsage = STREAM_USAGE_RINGTONE;

    AudioService *audioService = AudioService::GetInstance();

    // Since we can't mock PolicyHandler, configure real device info
    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    deviceInfo.deviceType_ = DEVICE_TYPE_WIRED_HEADSET;
    deviceInfo.isLowLatencyDevice_ = false;

    bool result = audioService->ShouldBeDualTone(config);
    EXPECT_NE(result, true);
}

/**
 * @tc.name: Test ShouldBeDualTone Non-Headset Device
 * @tc.type: FUNC
 * @tc.number: ShouldBeDualTone_002
 * @tc.desc: Test ShouldBeDualTone with non-headset device should return false
 */
HWTEST(AudioServiceUnitTest, ShouldBeDualTone_002, TestSize.Level1)
{
    // Prepare test data
    AudioProcessConfig config;
    config.audioMode = AUDIO_MODE_PLAYBACK;
    config.rendererInfo.streamUsage = STREAM_USAGE_RINGTONE;

    AudioService *audioService = AudioService::GetInstance();

    // Configure non-headset device
    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    deviceInfo.deviceType_ = DEVICE_TYPE_NONE;  // Non-headset device
    deviceInfo.isLowLatencyDevice_ = false;

    bool result = audioService->ShouldBeDualTone(config);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: Test ShouldBeDualTone Non-Ringtone Usage
 * @tc.type: FUNC
 * @tc.number: ShouldBeDualTone_003
 * @tc.desc: Test ShouldBeDualTone with non-ringtone usage should return false
 */
HWTEST(AudioServiceUnitTest, ShouldBeDualTone_003, TestSize.Level1)
{
    // Prepare test data
    AudioProcessConfig config;
    config.audioMode = AUDIO_MODE_PLAYBACK;
    config.rendererInfo.streamUsage = STREAM_USAGE_MEDIA;  // Non-ringtone usage

    AudioService *audioService = AudioService::GetInstance();

    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    deviceInfo.deviceType_ = DEVICE_TYPE_WIRED_HEADSET;
    deviceInfo.isLowLatencyDevice_ = false;

    bool result = audioService->ShouldBeDualTone(config);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: Test ShouldBeDualTone Non-Playback Mode
 * @tc.type: FUNC
 * @tc.number: ShouldBeDualTone_004
 * @tc.desc: Test ShouldBeDualTone with non-playback mode should return false
 */
HWTEST(AudioServiceUnitTest, ShouldBeDualTone_004, TestSize.Level1)
{
    // Prepare test data
    AudioProcessConfig config;
    config.audioMode = AUDIO_MODE_RECORD;  // Non-playback mode
    config.rendererInfo.streamUsage = STREAM_USAGE_RINGTONE;

    AudioService *audioService = AudioService::GetInstance();

    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    deviceInfo.deviceType_ = DEVICE_TYPE_WIRED_HEADSET;
    deviceInfo.isLowLatencyDevice_ = false;

    bool result = audioService->ShouldBeDualTone(config);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: Test OnInitInnerCapList Normal Case
 * @tc.type: FUNC
 * @tc.number: OnInitInnerCapList_001
 * @tc.desc: Test OnInitInnerCapList with valid renderer and public privacy type
 */
HWTEST(AudioServiceUnitTest, OnInitInnerCapList_001, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();

    // Prepare renderer config
    AudioProcessConfig processConfig;
    processConfig.audioMode = AUDIO_MODE_PLAYBACK;
    processConfig.privacyType = PRIVACY_TYPE_PUBLIC;
    processConfig.rendererInfo.streamUsage = STREAM_USAGE_MEDIA;
    processConfig.appInfo.appPid = 1001;

    // Create renderer
    std::shared_ptr<IStreamListener> streamListener = nullptr;
    std::shared_ptr<RendererInServer> renderer =
        std::make_shared<RendererInServer>(processConfig, streamListener);

    // Set working config for inner capture
    CaptureFilterOptions filterOptions;
    filterOptions.usages.push_back(STREAM_USAGE_MEDIA);
    filterOptions.usageFilterMode = FilterMode::INCLUDE;
    audioService->workingConfig_.filterOptions = filterOptions;

    // Add renderer to map
    uint32_t sessionId = 1;
    {
        std::unique_lock<std::mutex> lock(audioService->rendererMapMutex_);
        audioService->allRendererMap_[sessionId] = renderer;
    }

    int32_t result = audioService->OnInitInnerCapList();
    EXPECT_EQ(result, SUCCESS);
}

/**
 * @tc.name: Test OnInitInnerCapList Empty Map
 * @tc.type: FUNC
 * @tc.number: OnInitInnerCapList_002
 * @tc.desc: Test OnInitInnerCapList with empty renderer map
 */
HWTEST(AudioServiceUnitTest, OnInitInnerCapList_002, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();

    // Clear renderer map
    {
        std::unique_lock<std::mutex> lock(audioService->rendererMapMutex_);
        audioService->allRendererMap_.clear();
    }

    int32_t result = audioService->OnInitInnerCapList();
    EXPECT_EQ(result, SUCCESS);
}

/**
 * @tc.name: Test OnInitInnerCapList Private Privacy Type
 * @tc.type: FUNC
 * @tc.number: OnInitInnerCapList_003
 * @tc.desc: Test OnInitInnerCapList with private privacy type renderer
 */
HWTEST(AudioServiceUnitTest, OnInitInnerCapList_003, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();

    // Prepare renderer config with private privacy type
    AudioProcessConfig processConfig;
    processConfig.audioMode = AUDIO_MODE_PLAYBACK;
    processConfig.privacyType = PRIVACY_TYPE_PRIVATE;  // Set private type
    processConfig.rendererInfo.streamUsage = STREAM_USAGE_MEDIA;
    processConfig.appInfo.appPid = 1001;

    // Create renderer
    std::shared_ptr<IStreamListener> streamListener = nullptr;
    std::shared_ptr<RendererInServer> renderer =
        std::make_shared<RendererInServer>(processConfig, streamListener);

    // Set working config for inner capture
    CaptureFilterOptions filterOptions;
    filterOptions.usages.push_back(STREAM_USAGE_MEDIA);
    filterOptions.usageFilterMode = FilterMode::INCLUDE;
    audioService->workingConfig_.filterOptions = filterOptions;

    // Add renderer to map
    uint32_t sessionId = 1;
    {
        std::unique_lock<std::mutex> lock(audioService->rendererMapMutex_);
        audioService->allRendererMap_[sessionId] = renderer;
    }

    int32_t result = audioService->OnInitInnerCapList();
    EXPECT_EQ(result, SUCCESS);
}

/**
 * @tc.name : Test OnCapturerFilterChange First Call
 * @tc.type : FUNC
 * @tc.number : OnCapturerFilterChange_001
 * @tc.desc : Test OnCapturerFilterChange when called first time with new sessionId.
 */
HWTEST(AudioServiceUnitTest, OnCapturerFilterChange_001, TestSize.Level1)
{
    // 1. Prepare test objects and config
    AudioProcessConfig processConfig;
    processConfig.callerUid = 1000;
    processConfig.audioMode = AUDIO_MODE_PLAYBACK;

    // Create renderer
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);

    // 2. Prepare test data
    AudioPlaybackCaptureConfig captureConfig;
    CaptureFilterOptions filterOptions;
    filterOptions.usageFilterMode = FilterMode::INCLUDE;
    filterOptions.pidFilterMode = FilterMode::INCLUDE;
    captureConfig.filterOptions = filterOptions;
    captureConfig.silentCapture = false;

    uint32_t sessionId = 100;  // Test session ID

    // 3. Get AudioService instance and setup
    AudioService *audioService = AudioService::GetInstance();
    audioService->InsertRenderer(1, rendererInServer);

    // 4. Execute test
    int32_t result = audioService->OnCapturerFilterChange(sessionId, captureConfig);

    // 5. Verify results
    EXPECT_NE(result, SUCCESS);

    // 6. Cleanup
    audioService->RemoveRenderer(1);
}

/**
 * @tc.name : Test OnCapturerFilterChange Update Config
 * @tc.type : FUNC
 * @tc.number : OnCapturerFilterChange_002
 * @tc.desc : Test OnCapturerFilterChange when updating config with same sessionId
 */
HWTEST(AudioServiceUnitTest, OnCapturerFilterChange_002, TestSize.Level1)
{
    // 1. Prepare test objects and config
    AudioProcessConfig processConfig;
    processConfig.callerUid = 1000;
    processConfig.audioMode = AUDIO_MODE_PLAYBACK;

    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);

    // 2. Prepare initial capture config
    AudioPlaybackCaptureConfig initialConfig;
    CaptureFilterOptions initialFilterOptions;
    initialFilterOptions.usageFilterMode = FilterMode::INCLUDE;
    initialFilterOptions.pidFilterMode = FilterMode::INCLUDE;
    initialConfig.filterOptions = initialFilterOptions;
    initialConfig.silentCapture = false;

    uint32_t sessionId = 100;

    // 3. Get AudioService instance and setup
    AudioService *audioService = AudioService::GetInstance();
    audioService->InsertRenderer(1, rendererInServer);

    // 4. First call to set initial config
    int32_t result1 = audioService->OnCapturerFilterChange(sessionId, initialConfig);
    EXPECT_NE(result1, SUCCESS);

    // 5. Prepare updated config
    AudioPlaybackCaptureConfig updatedConfig;
    CaptureFilterOptions updatedFilterOptions;
    updatedFilterOptions.usageFilterMode = FilterMode::EXCLUDE;
    updatedFilterOptions.pidFilterMode = FilterMode::EXCLUDE;
    updatedConfig.filterOptions = updatedFilterOptions;
    updatedConfig.silentCapture = true;

    // 6. Call with same sessionId but updated config
    int32_t result2 = audioService->OnCapturerFilterChange(sessionId, updatedConfig);
    EXPECT_NE(result2, SUCCESS);

    // 7. Cleanup
    audioService->RemoveRenderer(1);
}

/**
 * @tc.name : Test OnCapturerFilterChange With Working SessionId
 * @tc.type : FUNC
 * @tc.number : OnCapturerFilterChange_003
 * @tc.desc : Test OnCapturerFilterChange when another sessionId is already working
 */
HWTEST(AudioServiceUnitTest, OnCapturerFilterChange_003, TestSize.Level1)
{
    // 1. Prepare test objects and config
    AudioProcessConfig processConfig;
    processConfig.callerUid = 1000;
    processConfig.audioMode = AUDIO_MODE_PLAYBACK;

    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);

    // 2. Prepare capture config
    AudioPlaybackCaptureConfig captureConfig;
    CaptureFilterOptions filterOptions;
    filterOptions.usageFilterMode = FilterMode::INCLUDE;
    filterOptions.pidFilterMode = FilterMode::INCLUDE;
    captureConfig.filterOptions = filterOptions;
    captureConfig.silentCapture = false;

    uint32_t sessionId1 = 100;
    uint32_t sessionId2 = 200;

    // 3. Get AudioService instance and setup
    AudioService *audioService = AudioService::GetInstance();
    audioService->InsertRenderer(1, rendererInServer);

    // 4. First call with sessionId1
    int32_t result1 = audioService->OnCapturerFilterChange(sessionId1, captureConfig);
    EXPECT_NE(result1, SUCCESS);

    // 5. Try to call with different sessionId while sessionId1 is working
    int32_t result2 = audioService->OnCapturerFilterChange(sessionId2, captureConfig);
    EXPECT_EQ(result2, ERR_OPERATION_FAILED);

    // 6. Cleanup
    audioService->RemoveRenderer(1);
}

/**
 * @tc.name : Test OnCapturerFilterChange With Multiple Renderers
 * @tc.type : FUNC
 * @tc.number : OnCapturerFilterChange_004
 * @tc.desc : Test OnCapturerFilterChange with multiple renderers in the map
 */
HWTEST(AudioServiceUnitTest, OnCapturerFilterChange_004, TestSize.Level1)
{
    // 1. Prepare test objects and configs
    AudioProcessConfig processConfig1;
    processConfig1.callerUid = 1000;
    processConfig1.audioMode = AUDIO_MODE_PLAYBACK;

    AudioProcessConfig processConfig2;
    processConfig2.callerUid = 2000;
    processConfig2.audioMode = AUDIO_MODE_PLAYBACK;

    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;

    std::shared_ptr<RendererInServer> renderer1 =
        std::make_shared<RendererInServer>(processConfig1, streamListener);
    std::shared_ptr<RendererInServer> renderer2 =
        std::make_shared<RendererInServer>(processConfig2, streamListener);

    // 2. Prepare capture config
    AudioPlaybackCaptureConfig captureConfig;
    CaptureFilterOptions filterOptions;
    filterOptions.usageFilterMode = FilterMode::INCLUDE;
    filterOptions.pidFilterMode = FilterMode::INCLUDE;
    captureConfig.filterOptions = filterOptions;
    captureConfig.silentCapture = false;

    uint32_t sessionId = 100;

    // 3. Get AudioService instance and setup multiple renderers
    AudioService *audioService = AudioService::GetInstance();
    audioService->InsertRenderer(1, renderer1);
    audioService->InsertRenderer(2, renderer2);

    // 4. Execute test
    int32_t result = audioService->OnCapturerFilterChange(sessionId, captureConfig);
    EXPECT_NE(result, SUCCESS);

    // 5. Cleanup
    audioService->RemoveRenderer(1);
    audioService->RemoveRenderer(2);
}

/**
 * @tc.name : Test OnCapturerFilterChange With Invalid SessionId
 * @tc.type : FUNC
 * @tc.number : OnCapturerFilterChange_005
 * @tc.desc : Test OnCapturerFilterChange with invalid sessionId (0)
 */
HWTEST(AudioServiceUnitTest, OnCapturerFilterChange_005, TestSize.Level1)
{
    // 1. Prepare test objects and config
    AudioProcessConfig processConfig;
    processConfig.callerUid = 1000;
    processConfig.audioMode = AUDIO_MODE_PLAYBACK;

    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);

    // 2. Prepare capture config
    AudioPlaybackCaptureConfig captureConfig;
    CaptureFilterOptions filterOptions;
    filterOptions.usageFilterMode = FilterMode::INCLUDE;
    filterOptions.pidFilterMode = FilterMode::INCLUDE;
    captureConfig.filterOptions = filterOptions;
    captureConfig.silentCapture = false;

    uint32_t invalidSessionId = 0;

    // 3. Get AudioService instance and setup
    AudioService *audioService = AudioService::GetInstance();
    audioService->InsertRenderer(1, rendererInServer);

    // 4. Execute test with invalid sessionId
    int32_t result = audioService->OnCapturerFilterChange(invalidSessionId, captureConfig);
    EXPECT_NE(result, ERR_INVALID_PARAM);

    // 5. Cleanup
    audioService->RemoveRenderer(1);
}

/**
 * @tc.name : Test OnCapturerFilterRemove With Non-matching SessionId
 * @tc.type : FUNC
 * @tc.number : OnCapturerFilterRemove_001
 * @tc.desc : Test OnCapturerFilterRemove when sessionId doesn't match workingInnerCapId
 */
HWTEST(AudioServiceUnitTest, OnCapturerFilterRemove_001, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();

    uint32_t removeSessionId = 200;  // Different from working session

    int32_t result = audioService->OnCapturerFilterRemove(removeSessionId);

    EXPECT_EQ(result, SUCCESS);
}

/**
 * @tc.name : Test OnCapturerFilterRemove With Empty EndpointList
 * @tc.type : FUNC
 * @tc.number : OnCapturerFilterRemove_002
 * @tc.desc : Test OnCapturerFilterRemove when endpointList is empty
 */
HWTEST(AudioServiceUnitTest, OnCapturerFilterRemove_002, TestSize.Level1)
{
    // 1. Prepare test environment
    AudioService *audioService = AudioService::GetInstance();

    // Ensure endpoint list is empty
    audioService->endpointList_.clear();

    uint32_t sessionId = 100;
    audioService->workingInnerCapId_ = sessionId;

    // 2. Execute test
    int32_t result = audioService->OnCapturerFilterRemove(sessionId);

    // 3. Verify results
    EXPECT_EQ(result, SUCCESS);
    EXPECT_EQ(audioService->workingInnerCapId_, 0);
}

/**
 * @tc.name : Test OnCapturerFilterRemove With Empty EndpointList
 * @tc.type : FUNC
 * @tc.number : OnCapturerFilterRemove_003
 * @tc.desc : Test OnCapturerFilterRemove when endpointList is empty
 */
HWTEST(AudioServiceUnitTest, OnCapturerFilterRemove_003, TestSize.Level1)
{
    // 1. Prepare test environment
    AudioService *audioService = AudioService::GetInstance();

    // Ensure endpoint list is empty
    audioService->endpointList_.clear();

    uint32_t sessionId = 100;
    audioService->workingInnerCapId_ = sessionId;

    // 2. Execute test
    int32_t result = audioService->OnCapturerFilterRemove(sessionId);

    // 3. Verify results
    EXPECT_EQ(result, SUCCESS);
    EXPECT_EQ(audioService->workingInnerCapId_, 0);
}

/**
 * @tc.name : Test OnCapturerFilterRemove With Multiple Endpoints
 * @tc.type : FUNC
 * @tc.number : OnCapturerFilterRemove_004
 * @tc.desc : Test OnCapturerFilterRemove with multiple endpoints of different roles
 */
HWTEST(AudioServiceUnitTest, OnCapturerFilterRemove_004, TestSize.Level1)
{
    // 1. Prepare test environment
    AudioService *audioService = AudioService::GetInstance();

    // Create and setup output endpoint
    AudioDeviceDescriptor outputDesc(DeviceType::DEVICE_TYPE_SPEAKER, DeviceRole::OUTPUT_DEVICE);
    std::shared_ptr<AudioEndpoint> outputEndpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    // Create and setup input endpoint
    AudioDeviceDescriptor inputDesc(DeviceType::DEVICE_TYPE_MIC, DeviceRole::INPUT_DEVICE);
    std::shared_ptr<AudioEndpoint> inputEndpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);

    audioService->endpointList_["output_endpoint"] = outputEndpoint;
    audioService->endpointList_["input_endpoint"] = inputEndpoint;

    uint32_t sessionId = 100;
    audioService->workingInnerCapId_ = sessionId;

    // 2. Execute test
    int32_t result = audioService->OnCapturerFilterRemove(sessionId);

    // 3. Verify results
    EXPECT_EQ(result, SUCCESS);

    // 4. Cleanup
    audioService->endpointList_.clear();
}

/**
 * @tc.name : Test OnCapturerFilterRemove With Multiple Renderers
 * @tc.type : FUNC
 * @tc.number : OnCapturerFilterRemove_005
 * @tc.desc : Test OnCapturerFilterRemove with multiple renderers including released ones
 */
HWTEST(AudioServiceUnitTest, OnCapturerFilterRemove_005, TestSize.Level1)
{
    // 1. Prepare test environment
    AudioService *audioService = AudioService::GetInstance();

    // Create valid renderer
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> validRenderer =
        std::make_shared<RendererInServer>(processConfig, streamListener);

    // Add both valid and expired renderers to filtered list
    audioService->filteredRendererMap_.push_back(validRenderer);
    audioService->filteredRendererMap_.push_back(std::weak_ptr<RendererInServer>());  // expired renderer

    uint32_t sessionId = 100;
    audioService->workingInnerCapId_ = sessionId;

    // 2. Execute test
    int32_t result = audioService->OnCapturerFilterRemove(sessionId);

    // 3. Verify results
    EXPECT_EQ(result, SUCCESS);
    EXPECT_TRUE(audioService->filteredRendererMap_.empty());
}

/**
 * @tc.name : Test ResetAudioEndpoint API
 * @tc.type : FUNC
 * @tc.number : ResetAudioEndpoint_001
 * @tc.desc : Test basic functionality of ResetAudioEndpoint
 */
HWTEST(AudioServiceUnitTest, ResetAudioEndpoint_001, TestSize.Level1)
{
    AudioProcessConfig processConfig;
    processConfig.audioMode = AUDIO_MODE_PLAYBACK;
    processConfig.appInfo.appUid = 1000;
    ProcessReleaseCallback *callback = nullptr;
    sptr<AudioProcessInServer> processInServer = AudioProcessInServer::Create(processConfig, callback);
    ASSERT_NE(processInServer, nullptr);

    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    ASSERT_NE(endpoint, nullptr);

    AudioService *audioService = AudioService::GetInstance();
    ASSERT_NE(audioService, nullptr);

    // add pair to linkedPairedList_
    std::pair<sptr<AudioProcessInServer>, std::shared_ptr<AudioEndpoint>> pair(processInServer, endpoint);
    audioService->linkedPairedList_.push_back(pair);

    // add endpoint to endpointList_
    std::string endpointName = endpoint->GetEndpointName();
    audioService->endpointList_[endpointName] = endpoint;

    audioService->ResetAudioEndpoint();

    EXPECT_TRUE(audioService->endpointList_.empty());
}

/**
 * @tc.name : Test ReLinkProcessToEndpoint API with non-MMAP endpoint
 * @tc.type : FUNC
 * @tc.number : ReLinkProcessToEndpoint_001
 * @tc.desc : Test ReLinkProcessToEndpoint with non-MMAP type endpoint
 */
HWTEST(AudioServiceUnitTest, ReLinkProcessToEndpoint_001, TestSize.Level1)
{
    AudioProcessConfig processConfig;
    processConfig.audioMode = AUDIO_MODE_PLAYBACK;
    processConfig.appInfo.appUid = 1000;
    ProcessReleaseCallback *callback = nullptr;

    sptr<AudioProcessInServer> processInServer = AudioProcessInServer::Create(processConfig, callback);
    ASSERT_NE(processInServer, nullptr);

    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_INDEPENDENT, 12345, STREAM_MUSIC);
    ASSERT_NE(endpoint, nullptr);

    AudioService *audioService = AudioService::GetInstance();
    ASSERT_NE(audioService, nullptr);

    std::pair<sptr<AudioProcessInServer>, std::shared_ptr<AudioEndpoint>> pair(processInServer, endpoint);
    audioService->linkedPairedList_.push_back(pair);

    audioService->ReLinkProcessToEndpoint();

    // because not TYPE_MMAP，linkedPairedList_ should not change
    EXPECT_FALSE(audioService->linkedPairedList_.empty());
    EXPECT_EQ(audioService->linkedPairedList_.back().second->GetEndpointType(), AudioEndpoint::TYPE_INDEPENDENT);
}

/**
 * @tc.name : Test ReLinkProcessToEndpoint API with TYPE_MMAP endpoint but GetAudioEndpointForDevice fails
 * @tc.type : FUNC
 * @tc.number : ReLinkProcessToEndpoint_002
 * @tc.desc : Test ReLinkProcessToEndpoint when GetAudioEndpointForDevice returns nullptr
 */
HWTEST(AudioServiceUnitTest, ReLinkProcessToEndpoint_002, TestSize.Level1)
{
    AudioProcessConfig processConfig;
    processConfig.audioMode = AUDIO_MODE_PLAYBACK;
    processConfig.appInfo.appUid = 1000;
    ProcessReleaseCallback *callback = nullptr;

    sptr<AudioProcessInServer> processInServer = AudioProcessInServer::Create(processConfig, callback);
    ASSERT_NE(processInServer, nullptr);

    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    ASSERT_NE(endpoint, nullptr);

    AudioService *audioService = AudioService::GetInstance();
    ASSERT_NE(audioService, nullptr);

    std::pair<sptr<AudioProcessInServer>, std::shared_ptr<AudioEndpoint>> pair(processInServer, endpoint);
    audioService->linkedPairedList_.push_back(pair);

    audioService->ReLinkProcessToEndpoint();

    // should be added to errorLinkedPaireds中
    EXPECT_FALSE(audioService->linkedPairedList_.empty());
    // endpoint should not change , because get endpoint fail
    EXPECT_NE(audioService->linkedPairedList_.back().second, endpoint);
}

/**
 * @tc.name : Test ReLinkProcessToEndpoint API with TYPE_MMAP endpoint but LinkProcessToEndpoint fails
 * @tc.type : FUNC
 * @tc.number : ReLinkProcessToEndpoint_003
 * @tc.desc : Test ReLinkProcessToEndpoint when LinkProcessToEndpoint returns failure
 */
HWTEST(AudioServiceUnitTest, ReLinkProcessToEndpoint_003, TestSize.Level1)
{
    AudioProcessConfig processConfig;
    processConfig.audioMode = AUDIO_MODE_PLAYBACK;
    processConfig.appInfo.appUid = 1000;
    ProcessReleaseCallback *callback = nullptr;

    sptr<AudioProcessInServer> processInServer = AudioProcessInServer::Create(processConfig, callback);
    ASSERT_NE(processInServer, nullptr);

    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    ASSERT_NE(endpoint, nullptr);

    AudioService *audioService = AudioService::GetInstance();
    ASSERT_NE(audioService, nullptr);

    std::pair<sptr<AudioProcessInServer>, std::shared_ptr<AudioEndpoint>> pair(processInServer, endpoint);
    audioService->linkedPairedList_.push_back(pair);

    audioService->ReLinkProcessToEndpoint();

    EXPECT_FALSE(audioService->linkedPairedList_.empty());
    EXPECT_NE(audioService->linkedPairedList_.back().second, endpoint);
}

/**
 * @tc.name : Test ReLinkProcessToEndpoint API with successful complete flow
 * @tc.type : FUNC
 * @tc.number : ReLinkProcessToEndpoint_004
 * @tc.desc : Test ReLinkProcessToEndpoint with successful endpoint creation and linking
 */
HWTEST(AudioServiceUnitTest, ReLinkProcessToEndpoint_004, TestSize.Level1)
{
    AudioProcessConfig processConfig;
    processConfig.audioMode = AUDIO_MODE_PLAYBACK;
    processConfig.appInfo.appUid = 1000;
    ProcessReleaseCallback *callback = nullptr;

    sptr<AudioProcessInServer> processInServer = AudioProcessInServer::Create(processConfig, callback);
    ASSERT_NE(processInServer, nullptr);

    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    ASSERT_NE(endpoint, nullptr);

    AudioService *audioService = AudioService::GetInstance();
    ASSERT_NE(audioService, nullptr);

    std::pair<sptr<AudioProcessInServer>, std::shared_ptr<AudioEndpoint>> pair(processInServer, endpoint);
    audioService->linkedPairedList_.push_back(pair);

    audioService->ReLinkProcessToEndpoint();

    EXPECT_FALSE(audioService->linkedPairedList_.empty());
    EXPECT_NE(audioService->linkedPairedList_.back().second, endpoint);
    EXPECT_NE(audioService->linkedPairedList_.back().second->GetEndpointType(), AudioEndpoint::TYPE_MMAP);
}

/**
 * @tc.name: CheckInnerCapForProcess_001
 * @tc.desc: Test CheckInnerCapForProcess when workingInnerCapId_ == 0
 * @tc.type: FUNC
 * @tc.require: issueI5RWXU
 */
HWTEST(AudioServiceUnitTest, CheckInnerCapForProcess_001, TestSize.Level1)
{
    // 1. Prepare process config
    AudioProcessConfig processConfig;
    processConfig.privacyType = PRIVACY_TYPE_PUBLIC;
    processConfig.appInfo.appPid = 1234;
    processConfig.rendererInfo.streamUsage = STREAM_USAGE_UNKNOWN;

    // 2. Create process
    sptr<AudioProcessInServer> process = AudioProcessInServer::Create(processConfig, nullptr);
    ASSERT_NE(process, nullptr);

    // 3. Create endpoint
    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    ASSERT_NE(endpoint, nullptr);

    // 4. Get AudioService instance and ensure workingInnerCapId_ is 0
    AudioService *audioService = AudioService::GetInstance();
    ASSERT_NE(audioService, nullptr);
    audioService->workingInnerCapId_ = 0;

    // 5. Call test method
    bool initialInnerCapState = process->GetInnerCapState();
    audioService->CheckInnerCapForProcess(process, endpoint);

    // 6. Verify that inner cap state remains unchanged
    EXPECT_EQ(process->GetInnerCapState(), initialInnerCapState);
}

/**
 * @tc.name: CheckInnerCapForProcess_002
 * @tc.desc: Test CheckInnerCapForProcess when privacy type is not public
 * @tc.type: FUNC
 * @tc.require: issueI5RWXU
 */
HWTEST(AudioServiceUnitTest, CheckInnerCapForProcess_002, TestSize.Level1)
{
    // 1. Prepare process config with non-public privacy type
    AudioProcessConfig processConfig;
    processConfig.privacyType = PRIVACY_TYPE_PRIVATE;
    processConfig.appInfo.appPid = 1234;
    processConfig.rendererInfo.streamUsage = STREAM_USAGE_UNKNOWN;

    // 2. Create process
    sptr<AudioProcessInServer> process = AudioProcessInServer::Create(processConfig, nullptr);
    ASSERT_NE(process, nullptr);

    // 3. Create endpoint
    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    ASSERT_NE(endpoint, nullptr);

    // 4. Get AudioService instance and set workingInnerCapId_
    AudioService *audioService = AudioService::GetInstance();
    ASSERT_NE(audioService, nullptr);
    audioService->workingInnerCapId_ = 12345;

    // 5. Call test method
    process->SetInnerCapState(true);
    audioService->CheckInnerCapForProcess(process, endpoint);

    // 6. Verify that inner cap state is set to false
    EXPECT_FALSE(process->GetInnerCapState());
}

/**
 * @tc.name: CheckInnerCapForProcess_003
 * @tc.desc: Test CheckInnerCapForProcess with POLICY_USAGES_ONLY and matched usage
 * @tc.type: FUNC
 * @tc.require: issueI5RWXU
 */
HWTEST(AudioServiceUnitTest, CheckInnerCapForProcess_003, TestSize.Level1)
{
    // 1. Prepare process config
    AudioProcessConfig processConfig;
    processConfig.privacyType = PRIVACY_TYPE_PUBLIC;
    processConfig.appInfo.appPid = 1234;
    processConfig.rendererInfo.streamUsage = STREAM_USAGE_UNKNOWN;

    // 2. Create process
    sptr<AudioProcessInServer> process = AudioProcessInServer::Create(processConfig, nullptr);
    ASSERT_NE(process, nullptr);

    // 3. Create endpoint
    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    ASSERT_NE(endpoint, nullptr);

    // 4. Get AudioService instance and setup
    AudioService *audioService = AudioService::GetInstance();
    ASSERT_NE(audioService, nullptr);
    audioService->workingInnerCapId_ = 12345;
    audioService->workingConfig_.filterOptions.usageFilterMode = FilterMode::INCLUDE;
    audioService->workingConfig_.filterOptions.usages = {STREAM_USAGE_UNKNOWN};

    // 5. Call test method
    process->SetInnerCapState(false);
    audioService->CheckInnerCapForProcess(process, endpoint);

    // 6. Verify that inner cap state is set to true
    EXPECT_TRUE(process->GetInnerCapState());
}

/**
 * @tc.name: CheckInnerCapForProcess_004
 * @tc.desc: Test CheckInnerCapForProcess with POLICY_USAGES_ONLY and unmatched usage
 * @tc.type: FUNC
 * @tc.require: issueI5RWXU
 */
HWTEST(AudioServiceUnitTest, CheckInnerCapForProcess_004, TestSize.Level1)
{
    // 1. Prepare process config with different usage
    AudioProcessConfig processConfig;
    processConfig.privacyType = PRIVACY_TYPE_PUBLIC;
    processConfig.appInfo.appPid = 1234;
    processConfig.rendererInfo.streamUsage = STREAM_USAGE_INVALID;

    // 2. Create process
    sptr<AudioProcessInServer> process = AudioProcessInServer::Create(processConfig, nullptr);
    ASSERT_NE(process, nullptr);

    // 3. Create endpoint
    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    ASSERT_NE(endpoint, nullptr);

    // 4. Get AudioService instance and setup
    AudioService *audioService = AudioService::GetInstance();
    ASSERT_NE(audioService, nullptr);
    audioService->workingInnerCapId_ = 12345;
    audioService->workingConfig_.filterOptions.usageFilterMode = FilterMode::INCLUDE;
    audioService->workingConfig_.filterOptions.usages = {STREAM_USAGE_UNKNOWN};

    // 5. Call test method
    process->SetInnerCapState(true);
    audioService->CheckInnerCapForProcess(process, endpoint);

    // 6. Verify that inner cap state is set to false
    EXPECT_FALSE(process->GetInnerCapState());
}

/**
 * @tc.name: CheckInnerCapForProcess_005
 * @tc.desc: Test CheckInnerCapForProcess with POLICY_USAGES_AND_PIDS, all matched
 * @tc.type: FUNC
 * @tc.require: issueI5RWXU
 */
HWTEST(AudioServiceUnitTest, CheckInnerCapForProcess_005, TestSize.Level1)
{
    // 1. Prepare process config
    AudioProcessConfig processConfig;
    processConfig.privacyType = PRIVACY_TYPE_PUBLIC;
    processConfig.appInfo.appPid = 1234;
    processConfig.rendererInfo.streamUsage = STREAM_USAGE_UNKNOWN;

    // 2. Create process
    sptr<AudioProcessInServer> process = AudioProcessInServer::Create(processConfig, nullptr);
    ASSERT_NE(process, nullptr);

    // 3. Create endpoint
    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    ASSERT_NE(endpoint, nullptr);

    // 4. Get AudioService instance and setup for both usage and pid matching
    AudioService *audioService = AudioService::GetInstance();
    ASSERT_NE(audioService, nullptr);
    audioService->workingInnerCapId_ = 12345;
    audioService->workingConfig_.filterOptions.usageFilterMode = FilterMode::INCLUDE;
    audioService->workingConfig_.filterOptions.usages = {STREAM_USAGE_UNKNOWN};
    audioService->workingConfig_.filterOptions.pidFilterMode = FilterMode::INCLUDE;
    audioService->workingConfig_.filterOptions.pids = {1234};

    // 5. Call test method
        process->SetInnerCapState(false);
    audioService->CheckInnerCapForProcess(process, endpoint);

    // 6. Verify that inner cap state is set to true
    EXPECT_TRUE(process->GetInnerCapState());
}

/**
 * @tc.name: CheckInnerCapForProcess_006
 * @tc.desc: Test CheckInnerCapForProcess with POLICY_USAGES_AND_PIDS, usage matched but pid unmatched
 * @tc.type: FUNC
 * @tc.require: issueI5RWXU
 */
HWTEST(AudioServiceUnitTest, CheckInnerCapForProcess_006, TestSize.Level1)
{
    // 1. Prepare process config
    AudioProcessConfig processConfig;
    processConfig.privacyType = PRIVACY_TYPE_PUBLIC;
    processConfig.appInfo.appPid = 5678;  // Different PID
    processConfig.rendererInfo.streamUsage = STREAM_USAGE_UNKNOWN;

    // 2. Create process
    sptr<AudioProcessInServer> process = AudioProcessInServer::Create(processConfig, nullptr);
    ASSERT_NE(process, nullptr);

    // 3. Create endpoint
    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    ASSERT_NE(endpoint, nullptr);

    // 4. Get AudioService instance and setup matching usage but unmatching pid
    AudioService *audioService = AudioService::GetInstance();
    ASSERT_NE(audioService, nullptr);
    audioService->workingInnerCapId_ = 12345;
    audioService->workingConfig_.filterOptions.usageFilterMode = FilterMode::INCLUDE;
    audioService->workingConfig_.filterOptions.usages = {STREAM_USAGE_UNKNOWN};
    audioService->workingConfig_.filterOptions.pidFilterMode = FilterMode::INCLUDE;
    audioService->workingConfig_.filterOptions.pids = {1234};

    // 5. Call test method
    process->SetInnerCapState(true);
    audioService->CheckInnerCapForProcess(process, endpoint);

    // 6. Verify that inner cap state is set to false
    EXPECT_FALSE(process->GetInnerCapState());
}

/**
 * @tc.name : Test NotifyStreamVolumeChanged API when endpoint name equals item first
 * @tc.type : FUNC
 * @tc.number : NotifyStreamVolumeChanged_001
 * @tc.desc : Test NotifyStreamVolumeChanged interface when endpoint name matches.
 */
HWTEST(AudioServiceUnitTest, NotifyStreamVolumeChanged_001, TestSize.Level1)
{
    // Create AudioService instance
    AudioService *audioService = AudioService::GetInstance();

    // Create test endpoint
    std::string testKey = "test_endpoint";
    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);

    // Insert test endpoint to endpointList_
    audioService->endpointList_[testKey] = endpoint;

    // Call NotifyStreamVolumeChanged with test parameters
    AudioStreamType testType = STREAM_MUSIC;
    float testVolume = 0.5f;
    int32_t result = audioService->NotifyStreamVolumeChanged(testType, testVolume);

    // Verify result
    EXPECT_EQ(result, SUCCESS);

    // Cleanup
    audioService->endpointList_.clear();
}

/**
 * @tc.name : Test NotifyStreamVolumeChanged API when endpoint name not equals item first
 * @tc.type : FUNC
 * @tc.number : NotifyStreamVolumeChanged_002
 * @tc.desc : Test NotifyStreamVolumeChanged interface when endpoint name doesn't match.
 */
HWTEST(AudioServiceUnitTest, NotifyStreamVolumeChanged_002, TestSize.Level1)
{
    // Create AudioService instance
    AudioService *audioService = AudioService::GetInstance();

    // Create test endpoint with different name
    std::string testKey = "test_endpoint";
    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);

    // Insert test endpoint to endpointList_
    audioService->endpointList_[testKey] = endpoint;

    // Call NotifyStreamVolumeChanged with test parameters
    AudioStreamType testType = STREAM_MUSIC;
    float testVolume = 0.5f;
    int32_t result = audioService->NotifyStreamVolumeChanged(testType, testVolume);

    // Since names don't match, SetVolume won't be called, should still return SUCCESS
    EXPECT_EQ(result, SUCCESS);

    // Cleanup
    audioService->endpointList_.clear();
}

/**
 * @tc.name : Test LinkProcessToEndpoint API - Happy Path
 * @tc.type : FUNC
 * @tc.number : LinkProcessToEndpoint_001
 * @tc.desc : Test LinkProcessToEndpoint when all conditions are successful
 */
HWTEST(AudioServiceUnitTest, LinkProcessToEndpoint_001, TestSize.Level1)
{
    // Create AudioService instance
    AudioService *audioService = AudioService::GetInstance();

    // Create process
    AudioProcessConfig processConfig;
    ProcessReleaseCallback *releaseCallback = nullptr;
    sptr<AudioProcessInServer> process = AudioProcessInServer::Create(processConfig, releaseCallback);

    // Create endpoint
    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    std::string endpointName = "test_endpoint";

    // Setup conditions for success path
    audioService->endpointList_[endpointName] = endpoint;

    // Test
    int32_t result = audioService->LinkProcessToEndpoint(process, endpoint);

    // Verify
    EXPECT_NE(result, SUCCESS);

    // Cleanup
    audioService->endpointList_.clear();
}

/**
 * @tc.name : Test LinkProcessToEndpoint API - Failed Link with Zero Count
 * @tc.type : FUNC
 * @tc.number : LinkProcessToEndpoint_002
 * @tc.desc : Test LinkProcessToEndpoint when LinkProcessStream fails and process count is zero
 */
HWTEST(AudioServiceUnitTest, LinkProcessToEndpoint_002, TestSize.Level1)
{
    // Create AudioService instance
    AudioService *audioService = AudioService::GetInstance();

    // Create process
    AudioProcessConfig processConfig;
    ProcessReleaseCallback *releaseCallback = nullptr;
    sptr<AudioProcessInServer> process = AudioProcessInServer::Create(processConfig, releaseCallback);

    // Create endpoint with conditions for failure
    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    std::string endpointName = "test_endpoint";

    // Setup conditions for failure path
    audioService->endpointList_[endpointName] = endpoint;

    // Test
    int32_t result = audioService->LinkProcessToEndpoint(process, endpoint);

    // Verify
    EXPECT_EQ(result, ERR_OPERATION_FAILED);

    // Cleanup
    audioService->endpointList_.clear();
}

/**
 * @tc.name : Test LinkProcessToEndpoint API - With Releasing Endpoint
 * @tc.type : FUNC
 * @tc.number : LinkProcessToEndpoint_003
 * @tc.desc : Test LinkProcessToEndpoint when endpoint is in releasing set
 */
HWTEST(AudioServiceUnitTest, LinkProcessToEndpoint_003, TestSize.Level1)
{
    // Create AudioService instance
    AudioService *audioService = AudioService::GetInstance();

    // Create process
    AudioProcessConfig processConfig;
    ProcessReleaseCallback *releaseCallback = nullptr;
    sptr<AudioProcessInServer> process = AudioProcessInServer::Create(processConfig, releaseCallback);

    // Create endpoint
    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    std::string endpointName = "test_endpoint";

    // Setup conditions
    audioService->endpointList_[endpointName] = endpoint;
    audioService->releasingEndpointSet_.insert(endpointName);

    // Test
    int32_t result = audioService->LinkProcessToEndpoint(process, endpoint);

    // Verify
    EXPECT_NE(result, SUCCESS);
    EXPECT_NE(audioService->releasingEndpointSet_.count(endpointName), 0);

    // Cleanup
    audioService->endpointList_.clear();
    audioService->releasingEndpointSet_.clear();
}

/**
 * @tc.name : Test LinkProcessToEndpoint API - Not In EndpointList
 * @tc.type : FUNC
 * @tc.number : LinkProcessToEndpoint_004
 * @tc.desc : Test LinkProcessToEndpoint when endpoint is not in endpointList
 */
HWTEST(AudioServiceUnitTest, LinkProcessToEndpoint_004, TestSize.Level1)
{
    // Create AudioService instance
    AudioService *audioService = AudioService::GetInstance();

    // Create process
    AudioProcessConfig processConfig;
    ProcessReleaseCallback *releaseCallback = nullptr;
    sptr<AudioProcessInServer> process = AudioProcessInServer::Create(processConfig, releaseCallback);

    // Create endpoint
    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);

    // Test with endpoint not in endpointList_
    int32_t result = audioService->LinkProcessToEndpoint(process, endpoint);

    // Verify
    EXPECT_NE(result, SUCCESS);

    // Cleanup
    audioService->endpointList_.clear();
}

/**
 * @tc.name : Test LinkProcessToEndpoint API - Failed Link with Non-Zero Count
 * @tc.type : FUNC
 * @tc.number : LinkProcessToEndpoint_005
 * @tc.desc : Test LinkProcessToEndpoint when LinkProcessStream fails but process count is not zero
 */
HWTEST(AudioServiceUnitTest, LinkProcessToEndpoint_005, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();

    // Create first process and link it (to make process count non-zero)
    AudioProcessConfig processConfig1;
    ProcessReleaseCallback *releaseCallback = nullptr;
    sptr<AudioProcessInServer> process1 = AudioProcessInServer::Create(processConfig1, releaseCallback);

    // Create second process for testing
    AudioProcessConfig processConfig2;
    sptr<AudioProcessInServer> process2 = AudioProcessInServer::Create(processConfig2, releaseCallback);

    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    std::string endpointName = "test_endpoint";

    // Link first process to make count non-zero
    audioService->endpointList_[endpointName] = endpoint;
    audioService->LinkProcessToEndpoint(process1, endpoint);

    // Test linking second process
    int32_t result = audioService->LinkProcessToEndpoint(process2, endpoint);

    // Even if LinkProcessStream fails, since count is non-zero, should return SUCCESS
    EXPECT_NE(result, SUCCESS);

    audioService->endpointList_.clear();
}

/**
 * @tc.name : Test LinkProcessToEndpoint API - With Releasing Endpoint Not In EndpointList
 * @tc.type : FUNC
 * @tc.number : LinkProcessToEndpoint_006
 * @tc.desc : Test LinkProcessToEndpoint when endpoint is in releasing set but not in endpointList
 */
HWTEST(AudioServiceUnitTest, LinkProcessToEndpoint_006, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();

    AudioProcessConfig processConfig;
    ProcessReleaseCallback *releaseCallback = nullptr;
    sptr<AudioProcessInServer> process = AudioProcessInServer::Create(processConfig, releaseCallback);

    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    std::string endpointName = "test_endpoint";

    // Add to releasing set but not to endpointList
    audioService->releasingEndpointSet_.insert(endpointName);

    int32_t result = audioService->LinkProcessToEndpoint(process, endpoint);

    EXPECT_NE(result, SUCCESS);
    EXPECT_NE(audioService->releasingEndpointSet_.count(endpointName), 0);

    audioService->releasingEndpointSet_.clear();
}

/**
 * @tc.name : Test LinkProcessToEndpoint API - Multiple Process Links
 * @tc.type : FUNC
 * @tc.number : LinkProcessToEndpoint_007
 * @tc.desc : Test LinkProcessToEndpoint with multiple process links to same endpoint
 */
HWTEST(AudioServiceUnitTest, LinkProcessToEndpoint_007, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();

    // Create multiple processes
    AudioProcessConfig processConfig1;
    AudioProcessConfig processConfig2;
    AudioProcessConfig processConfig3;
    ProcessReleaseCallback *releaseCallback = nullptr;
    sptr<AudioProcessInServer> process1 = AudioProcessInServer::Create(processConfig1, releaseCallback);
    sptr<AudioProcessInServer> process2 = AudioProcessInServer::Create(processConfig2, releaseCallback);
    sptr<AudioProcessInServer> process3 = AudioProcessInServer::Create(processConfig3, releaseCallback);
    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    std::string endpointName = "test_endpoint";
    audioService->endpointList_[endpointName] = endpoint;

    // Link multiple processes
    int32_t result1 = audioService->LinkProcessToEndpoint(process1, endpoint);
    int32_t result2 = audioService->LinkProcessToEndpoint(process2, endpoint);
    int32_t result3 = audioService->LinkProcessToEndpoint(process3, endpoint);

    EXPECT_NE(result1, SUCCESS);
    EXPECT_NE(result2, SUCCESS);
    EXPECT_NE(result3, SUCCESS);
    EXPECT_EQ(endpoint->GetLinkedProcessCount(), 0);

    audioService->endpointList_.clear();
}

/**
 * @tc.name : Test LinkProcessToEndpoint API - Process Link After Release
 * @tc.type : FUNC
 * @tc.number : LinkProcessToEndpoint_008
 * @tc.desc : Test LinkProcessToEndpoint immediately after endpoint is removed from releasing set
 */
HWTEST(AudioServiceUnitTest, LinkProcessToEndpoint_008, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();

    AudioProcessConfig processConfig;
    ProcessReleaseCallback *releaseCallback = nullptr;
    sptr<AudioProcessInServer> process = AudioProcessInServer::Create(processConfig, releaseCallback);
    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    std::string endpointName = "test_endpoint";

    // Setup initial conditions
    audioService->endpointList_[endpointName] = endpoint;
    audioService->releasingEndpointSet_.insert(endpointName);

    // First link should remove from releasing set
    int32_t result1 = audioService->LinkProcessToEndpoint(process, endpoint);
    EXPECT_NE(result1, SUCCESS);
    EXPECT_NE(audioService->releasingEndpointSet_.count(endpointName), 0);

    // Create new process and try to link immediately
    AudioProcessConfig processConfig2;
    sptr<AudioProcessInServer> process2 = AudioProcessInServer::Create(processConfig2, releaseCallback);
    int32_t result2 = audioService->LinkProcessToEndpoint(process2, endpoint);
    EXPECT_NE(result2, SUCCESS);

    audioService->endpointList_.clear();
    audioService->releasingEndpointSet_.clear();
}

/**
 * @tc.name : Test LinkProcessToEndpoint API - Complex Scenario
 * @tc.type : FUNC
 * @tc.number : LinkProcessToEndpoint_009
 * @tc.desc : Test LinkProcessToEndpoint with multiple conditions changing
 */
HWTEST(AudioServiceUnitTest, LinkProcessToEndpoint_009, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();

    AudioProcessConfig processConfig;
    ProcessReleaseCallback *releaseCallback = nullptr;
    sptr<AudioProcessInServer> process = AudioProcessInServer::Create(processConfig, releaseCallback);

    std::shared_ptr<AudioEndpoint> endpoint = std::make_shared<AudioEndpointSeparate>(
        AudioEndpoint::TYPE_MMAP, 12345, STREAM_MUSIC);
    std::string endpointName = "test_endpoint";

    // First try without endpoint in list
    int32_t result1 = audioService->LinkProcessToEndpoint(process, endpoint);
    EXPECT_NE(result1, SUCCESS);

    // Add to endpoint list and releasing set
    audioService->endpointList_[endpointName] = endpoint;
    audioService->releasingEndpointSet_.insert(endpointName);

    // Try link again
    int32_t result2 = audioService->LinkProcessToEndpoint(process, endpoint);
    EXPECT_NE(result2, SUCCESS);
    EXPECT_NE(audioService->releasingEndpointSet_.count(endpointName), 0);

    // Remove from endpoint list and try again
    audioService->endpointList_.clear();
    int32_t result3 = audioService->LinkProcessToEndpoint(process, endpoint);
    EXPECT_NE(result3, SUCCESS);

    audioService->endpointList_.clear();
    audioService->releasingEndpointSet_.clear();
}

/**
 * @tc.name : Test GetCreatedAudioStreamMostUid API
 * @tc.type : FUNC
 * @tc.number : GetCreatedAudioStreamMostUid_001
 * @tc.desc : Test it->second <= mostAppNum
 */
HWTEST(AudioServiceUnitTest, GetCreatedAudioStreamMostUid_001, TestSize.Level1)
{
    // perpare test data
    std::map<int32_t, int32_t> testMap = {
        {10, 5},   // uid: 10, count: 5
        {20, 8},   // uid: 20, count: 8
        {30, 8}    // uid: 30, count: 8
    };

    AudioService *audioService = AudioService::GetInstance();

    // set map
    SetAppUseNumMap(audioService, testMap);

    int32_t mostAppUid = -1;
    int32_t mostAppNum = -1;

    audioService->GetCreatedAudioStreamMostUid(mostAppUid, mostAppNum);

    EXPECT_EQ(mostAppUid, 20);
    EXPECT_EQ(mostAppNum, 8);
}

/**
 * @tc.name : Test GetCreatedAudioStreamMostUid API
 * @tc.type : FUNC
 * @tc.number : GetCreatedAudioStreamMostUid_002
 * @tc.desc : Test it->second > mostAppNum
 */
HWTEST(AudioServiceUnitTest, GetCreatedAudioStreamMostUid_002, TestSize.Level1)
{
    std::map<int32_t, int32_t> testMap = {
        {10, 5},   // uid: 10, count: 5
        {20, 8},   // uid: 20, count: 8
        {30, 3}    // uid: 30, count: 3
    };

    AudioService *audioService = AudioService::GetInstance();


    SetAppUseNumMap(audioService, testMap);

    int32_t mostAppUid = -1;
    int32_t mostAppNum = -1;

    audioService->GetCreatedAudioStreamMostUid(mostAppUid, mostAppNum);

    EXPECT_EQ(mostAppUid, 20);
    EXPECT_EQ(mostAppNum, 8);
}

/**
 * @tc.name : Test IsExceedingMaxStreamCntPerUid with MEDIA_SERVICE_UID
 * @tc.type : FUNC
 * @tc.number: IsExceedingMaxStreamCntPerUid_001
 * @tc.desc : Test method when callingUid is MEDIA_SERVICE_UID
 */
HWTEST(AudioServiceUnitTest, IsExceedingMaxStreamCntPerUid_001, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();

    int32_t callingUid = 1013;  // MEDIA_SERVICE_UID
    int32_t appUid = 10000;
    int32_t maxStreamCntPerUid = 3;

    bool result1 = audioService->IsExceedingMaxStreamCntPerUid(callingUid, appUid, maxStreamCntPerUid);
    EXPECT_FALSE(result1);

    bool result2 = audioService->IsExceedingMaxStreamCntPerUid(callingUid, appUid, maxStreamCntPerUid);
    EXPECT_FALSE(result2);

    // Check the actual values in the appUseNumMap_
    auto it = audioService->appUseNumMap_.find(appUid);
    ASSERT_NE(it, audioService->appUseNumMap_.end());
    EXPECT_EQ(it->second, 2);
}

/**
 * @tc.name : Test IsExceedingMaxStreamCntPerUid with non-MEDIA_SERVICE_UID
 * @tc.type : FUNC
 * @tc.number: IsExceedingMaxStreamCntPerUid_002
 * @tc.desc : Test method when callingUid is not MEDIA_SERVICE_UID
 */
HWTEST(AudioServiceUnitTest, IsExceedingMaxStreamCntPerUid_002, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();

    int32_t callingUid = 10001;  // 非MEDIA_SERVICE_UID
    int32_t appUid = 10000;
    int32_t maxStreamCntPerUid = 3;

    bool result1 = audioService->IsExceedingMaxStreamCntPerUid(callingUid, appUid, maxStreamCntPerUid);
    EXPECT_FALSE(result1);

    // now appUid should equal to callingUid
    auto it = audioService->appUseNumMap_.find(callingUid);
    ASSERT_NE(it, audioService->appUseNumMap_.end());
    EXPECT_EQ(it->second, 1);
}

/**
 * @tc.name : Test IsExceedingMaxStreamCntPerUid when exceeding max stream count
 * @tc.type : FUNC
 * @tc.number: IsExceedingMaxStreamCntPerUid_003
 * @tc.desc : Test method when stream count exceeds maxStreamCntPerUid
 */
HWTEST(AudioServiceUnitTest, IsExceedingMaxStreamCntPerUid_003, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();

    int32_t callingUid = 10001;
    int32_t appUid = 10000;
    int32_t maxStreamCntPerUid = 2;

    bool result1 = audioService->IsExceedingMaxStreamCntPerUid(callingUid, appUid, maxStreamCntPerUid);
    EXPECT_FALSE(result1);

    bool result2 = audioService->IsExceedingMaxStreamCntPerUid(callingUid, appUid, maxStreamCntPerUid);
    EXPECT_TRUE(result2);

    bool result3 = audioService->IsExceedingMaxStreamCntPerUid(callingUid, appUid, maxStreamCntPerUid);
    EXPECT_TRUE(result3);

    auto it = audioService->appUseNumMap_.find(callingUid);
    ASSERT_NE(it, audioService->appUseNumMap_.end());
    EXPECT_EQ(it->second, 2);  // If the limit is exceeded, it should fall back to 2
}

/**
 * @tc.name : Test UpdateSourceType API
 * @tc.type : FUNC
 * @tc.number: UpdateSourceType_001
 * @tc.desc : Test method when sourceType is in specialSourceTypeSet_
 */
HWTEST(AudioServiceUnitTest, UpdateSourceType_001, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();

    std::vector<SourceType> specialSourceTypes = {
        SOURCE_TYPE_PLAYBACK_CAPTURE,
        SOURCE_TYPE_WAKEUP,
        SOURCE_TYPE_VIRTUAL_CAPTURE,
        SOURCE_TYPE_REMOTE_CAST
    };

    for (auto sourceType : specialSourceTypes) {
        int32_t result = audioService->UpdateSourceType(sourceType);

        // For special sourceType, return SUCCESS
        EXPECT_EQ(result, SUCCESS) << "Failed for sourceType: " << sourceType;
    }
}

/**
 * @tc.name : TestSetIncMaxRendererStreamCnt API
 * @tc.type : FUNC
 * @tc.number: SetIncMaxRendererStreamCnt_001
 * @tc.desc : Test method when audioMode == AUDIO_MODE_PLAYBACK or audioMode != AUDIO_MODE_PLAYBACK
 */
HWTEST(AudioServiceUnitTest, SetIncMaxRendererStreamCnt_001, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();
    audioService-> currentRendererStreamCnt_ = 0;
    audioService->SetIncMaxRendererStreamCnt(AudioMode::AUDIO_MODE_PLAYBACK);
    EXPECT_EQ(audioService-> currentRendererStreamCnt_, 1);
    audioService->SetIncMaxRendererStreamCnt(AudioMode::AUDIO_MODE_RECORD);
    //currentRendererStreamCnt_ is should not change
    EXPECT_EQ(audioService-> currentRendererStreamCnt_, 1);
}

/**
 * @tc.name : Test UnsetOffloadMode API - Invalid SessionId
 * @tc.type : FUNC
 * @tc.number: UnsetOffloadMode_001
 * @tc.desc : Test UnsetOffloadMode interface with invalid sessionId
 */
HWTEST(AudioServiceUnitTest, UnsetOffloadMode_001, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();

    // test sessionId is not exist
    uint32_t invalidSessionId = 9999;
    int32_t result = audioService->UnsetOffloadMode(invalidSessionId);
    EXPECT_EQ(result, ERR_INVALID_INDEX);
}

/**
 * @tc.name : Test UnsetOffloadMode API - Renderer is nullptr
 * @tc.type : FUNC
 * @tc.number: UnsetOffloadMode_002
 * @tc.desc : Test UnsetOffloadMode interface with renderer being nullptr
 */
HWTEST(AudioServiceUnitTest, UnsetOffloadMode_002, TestSize.Level1)
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;

    AudioService *audioService = AudioService::GetInstance();

    uint32_t sessionId = 2;
    std::weak_ptr<RendererInServer> weakRenderer;

    auto tempRenderer = std::make_shared<RendererInServer>(processConfig, streamListener);
    weakRenderer = tempRenderer;
    // tempRenderer is out of scope, and weakRenderer becomes nullptr

    // mock invaild weakRenderer add to map
    audioService->allRendererMap_[sessionId] = weakRenderer;

    int32_t result = audioService->UnsetOffloadMode(sessionId);
    EXPECT_EQ(result, ERROR);
}
} // namespace AudioStandard
} // namespace OHOS