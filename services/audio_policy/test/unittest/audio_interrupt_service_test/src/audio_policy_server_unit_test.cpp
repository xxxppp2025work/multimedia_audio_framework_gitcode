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

#include "audio_policy_server_unit_test.h"
#include "audio_policy_server.h"
#include "audio_interrupt_unit_test.h"
#include "audio_info.h"
#include "audio_utils.h"
#include "securec.h"
#include "audio_interrupt_service.h"

#include <thread>
#include <memory>
#include <vector>
using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

void AudioPolicyUnitTest::SetUpTestCase(void) {}
void AudioPolicyUnitTest::TearDownTestCase(void) {}
void AudioPolicyUnitTest::SetUp(void) {}

void AudioPolicyUnitTest::TearDown(void) {}

sptr<AudioPolicyServer> GetPolicyServerUnitTest()
{
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server =
        sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    return server;
}

class RemoteObjectTestStub : public IRemoteObject {
public:
    RemoteObjectTestStub() : IRemoteObject(u"IRemoteObject") {}
    int32_t GetObjectRefCount() { return 0; };
    int SendRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option) { return 0; };
    bool AddDeathRecipient(const sptr<DeathRecipient> &recipient) { return true; };
    bool RemoveDeathRecipient(const sptr<DeathRecipient> &recipient) { return true; };
    int Dump(int fd, const std::vector<std::u16string> &args) { return 0; };

    DECLARE_INTERFACE_DESCRIPTOR(u"RemoteObjectTestStub");
};

#define PRINT_LINE printf("debug __LINE__:%d\n", __LINE__)

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_002
* @tc.desc  : Test LoadSplitModule.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_001, TestSize.Level1)
{
    std::string splitArgs;
    std::string networkId;
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = true;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);

    EXPECT_TRUE(server->LoadSplitModule(splitArgs, networkId));
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_003
* @tc.desc  : Test DeactivateAudioSession.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_002, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = true;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);

    EXPECT_TRUE(server->DeactivateAudioSession());
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_004
* @tc.desc  : Test DeactivateAudioSession.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_003, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = true;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);

    server->interruptService_ =  nullptr;
    EXPECT_TRUE(server->DeactivateAudioSession());
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_005
* @tc.desc  : Test ActivateAudioSession.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_004, TestSize.Level1)
{
    AudioSessionStrategy strategy;
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = true;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);

    EXPECT_TRUE(server->ActivateAudioSession(strategy));
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_006
* @tc.desc  : Test ActivateAudioSession.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_005, TestSize.Level1)
{
    AudioSessionStrategy strategy;
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = true;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);

    server->interruptService_ =  nullptr;
    EXPECT_TRUE(server->ActivateAudioSession(strategy));
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_007
* @tc.desc  : Test ActivateAudioSession.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_006, TestSize.Level1)
{
    AudioSessionStrategy strategy;
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = true;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);

    strategy.concurrencyMode = AudioConcurrencyMode::MIX_WITH_OTHERS;
    EXPECT_TRUE(server->CheckAudioSessionStrategy(strategy));
    EXPECT_TRUE(server->ActivateAudioSession(strategy));
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_008
* @tc.desc  : Test ActivateAudioSession.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_007, TestSize.Level1)
{
    AudioSessionStrategy strategy;
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = true;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);

    EXPECT_TRUE(server->ActivateAudioSession(strategy));
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_009
* @tc.desc  : Test CheckAudioSessionStrategy.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_008, TestSize.Level1)
{
    AudioSessionStrategy strategy;
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);

    strategy.concurrencyMode = AudioConcurrencyMode::DEFAULT;
    EXPECT_TRUE(server->CheckAudioSessionStrategy(strategy));
    strategy.concurrencyMode = AudioConcurrencyMode::MIX_WITH_OTHERS;
    EXPECT_TRUE(server->CheckAudioSessionStrategy(strategy));
    strategy.concurrencyMode = AudioConcurrencyMode::DUCK_OTHERS;
    EXPECT_TRUE(server->CheckAudioSessionStrategy(strategy));
    strategy.concurrencyMode = AudioConcurrencyMode::PAUSE_OTHERS;
    EXPECT_TRUE(server->CheckAudioSessionStrategy(strategy));
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_010
* @tc.desc  : Test CheckAudioSessionStrategy.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_009, TestSize.Level1)
{
    AudioSessionStrategy strategy;
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);

    strategy.concurrencyMode = static_cast<AudioConcurrencyMode>(999); // Invalid mode
    EXPECT_FALSE(server->CheckAudioSessionStrategy(strategy));
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_011
* @tc.desc  : Test InjectInterruption.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_010, TestSize.Level1)
{
    InterruptEvent event;
    std::string networkId;
    auto policyServerTest = GetPolicyServerUnitTest();
    EXPECT_EQ(policyServerTest->InjectInterruption(networkId, event), true);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_012
* @tc.desc  : Test SetCallDeviceActive.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_011, TestSize.Level1)
{
    std::string address;
    bool active = true;
    auto policyServerTest = GetPolicyServerUnitTest();

    EXPECT_EQ(policyServerTest->SetCallDeviceActive(InternalDeviceType::DEVICE_TYPE_EARPIECE,
              active, address), ERR_SYSTEM_PERMISSION_DENIED);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_013
* @tc.desc  : Test SetCallDeviceActive.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_012, TestSize.Level1)
{
    std::string address;
    bool active = true;
    auto policyServerTest = GetPolicyServerUnitTest();

    EXPECT_EQ(policyServerTest->SetCallDeviceActive(InternalDeviceType::DEVICE_TYPE_EARPIECE,
              active, address), ERR_SYSTEM_PERMISSION_DENIED);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_014
* @tc.desc  : Test SetCallDeviceActive.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_013, TestSize.Level1)
{
    std::string address;
    bool active = true;
    auto policyServerTest = GetPolicyServerUnitTest();

    EXPECT_EQ(policyServerTest->SetCallDeviceActive(InternalDeviceType::DEVICE_TYPE_EARPIECE,
              active, address), SUCCESS);
    EXPECT_EQ(policyServerTest->SetCallDeviceActive(InternalDeviceType::DEVICE_TYPE_SPEAKER,
              active, address), SUCCESS);
    EXPECT_EQ(policyServerTest->SetCallDeviceActive(InternalDeviceType::DEVICE_TYPE_BLUETOOTH_SCO,
              active, address), SUCCESS);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_015
* @tc.desc  : Test SetCallDeviceActive.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_014, TestSize.Level1)
{
    std::string address;
    bool active = true;
    auto policyServerTest = GetPolicyServerUnitTest();

    EXPECT_EQ(policyServerTest->SetCallDeviceActive(InternalDeviceType::DEVICE_TYPE_EARPIECE,
              active, address), ERR_NOT_SUPPORTED);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_016
* @tc.desc  : Test CreateAudioInterruptZone.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_015, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    std::set<int32_t> pids = {1, 2, 3};
    int32_t zoneID = 0;

    int32_t result = server->CreateAudioInterruptZone(pids, zoneID);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_021
* @tc.desc  : Test RemoveAudioInterruptZonePids.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_016, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    std::set<int32_t> pids = {1, 2, 3};
    int32_t zoneID = 0;

    server->interruptService_ =  nullptr;
    int32_t result = server->CreateAudioInterruptZone(pids, zoneID);
    result = server->RemoveAudioInterruptZonePids(pids, zoneID);
    EXPECT_EQ(result, ERR_UNKNOWN);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_023
* @tc.desc  : Test ReleaseAudioInterruptZone.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_017, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    std::set<int32_t> pids = {1, 2, 3};
    int32_t zoneID = 0;

    server->interruptService_ =  nullptr;
    int32_t result = server->CreateAudioInterruptZone(pids, zoneID);
    result = server->ReleaseAudioInterruptZone(1);
    EXPECT_EQ(result, ERR_UNKNOWN);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_024
* @tc.desc  : Test SetLowPowerVolume.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_018, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    int32_t result = server->SetLowPowerVolume(1, 0.5f);
    EXPECT_EQ(result, ERROR);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_025
* @tc.desc  : Test SetRingerModeLegacy.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_019, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    int32_t result = server->SetRingerModeLegacy(AudioRingerMode::RINGER_MODE_SILENT);
    EXPECT_EQ(result, SUCCESS);
    result = server->SetRingerModeLegacy(AudioRingerMode::RINGER_MODE_VIBRATE);
    EXPECT_EQ(result, SUCCESS);
    result = server->SetRingerModeLegacy(AudioRingerMode::RINGER_MODE_NORMAL);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_026
* @tc.desc  : Test SetAudioManagerInterruptCallback.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_020, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    int32_t result = server->SetAudioManagerInterruptCallback(0, sptr<RemoteObjectTestStub>::MakeSptr());
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_027
* @tc.desc  : Test SetAudioManagerInterruptCallback.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_021, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    server->interruptService_ = nullptr;
    int32_t result = server->SetAudioManagerInterruptCallback(0, sptr<RemoteObjectTestStub>::MakeSptr());
    EXPECT_EQ(result, ERR_UNKNOWN);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_028
* @tc.desc  : Test UnsetAudioManagerInterruptCallback.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_022, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    int32_t result = server->UnsetAudioManagerInterruptCallback(0);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_029
* @tc.desc  : Test UnsetAudioManagerInterruptCallback.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_023, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    server->interruptService_ = nullptr;
    int32_t result = server->UnsetAudioManagerInterruptCallback(0);
    EXPECT_EQ(result, ERR_UNKNOWN);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_030
* @tc.desc  : Test RequestAudioFocus.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_024, TestSize.Level1)
{
    AudioInterrupt audioInterrupt;
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    int32_t result = server->RequestAudioFocus(0, audioInterrupt);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_031
* @tc.desc  : Test RequestAudioFocus.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_025, TestSize.Level1)
{
    AudioInterrupt audioInterrupt;
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    server->interruptService_ = nullptr;
    int32_t result = server->RequestAudioFocus(0, audioInterrupt);
    EXPECT_EQ(result, ERR_UNKNOWN);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_032
* @tc.desc  : Test AbandonAudioFocus.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_026, TestSize.Level1)
{
    AudioInterrupt audioInterrupt;
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    int32_t result = server->AbandonAudioFocus(0, audioInterrupt);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_033
* @tc.desc  : Test AbandonAudioFocus.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_027, TestSize.Level1)
{
    AudioInterrupt audioInterrupt;
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    server->interruptService_ = nullptr;
    int32_t result = server->AbandonAudioFocus(0, audioInterrupt);
    EXPECT_EQ(result, ERR_UNKNOWN);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_036
* @tc.desc  : Test ReconfigureAudioChannel.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_028, TestSize.Level1)
{
    #ifdef AUDIO_BUILD_VARIANT_ROOT
    #undef AUDIO_BUILD_VARIANT_ROOT
    #endif
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    uint32_t count = 2;
    DeviceType deviceType = DeviceType::DEVICE_TYPE_DEFAULT;
    int32_t result = server->ReconfigureAudioChannel(count, deviceType);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_038
* @tc.desc  : Test ArgInfoDump.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_029, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    std::string dumpString;
    std::queue<std::u16string> argQue;
    argQue.push(u"invalidParam");
    server->ArgInfoDump(dumpString, argQue);
    EXPECT_EQ(dumpString.find("Please input correct param:\n"), 0);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_039
* @tc.desc  : Test ArgInfoDump.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_030, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    std::string dumpString;
    std::queue<std::u16string> argQue;
    argQue.push(u"validParam");
    server->ArgInfoDump(dumpString, argQue);
    EXPECT_NE(dumpString.find("AudioPolicyServer Data Dump:\n\n"), std::string::npos);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_041
* @tc.desc  : Test SetLowPowerVolume.
*/
#define FEATURE_MULTIMODALINPUT_INPUT
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_031, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    int32_t streamId = 2;
    float volume = 0.7f;
    int32_t result = server->SetLowPowerVolume(streamId, volume);
    EXPECT_EQ(result, ERROR);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_042
* @tc.desc  : Test SetStreamMuteLegacy.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_032, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);

    AudioStreamType streamType = STREAM_MUSIC;
    bool mute = false;
    int32_t result = server->SetStreamMuteLegacy(streamType, mute);
    EXPECT_EQ(result, 0);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_043
* @tc.desc  : Test SetAudioInterruptCallback.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_033, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    uint32_t sessionID = 123;
    sptr<IRemoteObject> object = nullptr;
    int32_t zoneID = 456;
    int32_t result = server->SetAudioInterruptCallback(sessionID, object, zoneID);
    EXPECT_EQ(result, ERR_UNKNOWN);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_044
* @tc.desc  : Test UnsetAudioInterruptCallback.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_034, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    uint32_t sessionID = 123;
    int32_t zoneID = 456;
    int32_t result = server->UnsetAudioInterruptCallback(sessionID, zoneID);
    EXPECT_EQ(result, ERR_UNKNOWN);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_045
* @tc.desc  : Test DeactivateAudioInterrupt.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_035, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    AudioInterrupt audioInterrupt;
    int32_t zoneID = 456;
    int32_t result = server->DeactivateAudioInterrupt(audioInterrupt, zoneID);
    EXPECT_EQ(result, ERR_UNKNOWN);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_046
* @tc.desc  : Test GetStreamInFocus.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_036, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    int32_t zoneID = 456;
    server->interruptService_ = nullptr;
    AudioStreamType result = server->GetStreamInFocus(zoneID);
    EXPECT_EQ(result, STREAM_MUSIC);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_047
* @tc.desc  : Test GetSessionInfoInFocus.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_037, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    AudioInterrupt audioInterrupt;
    int32_t zoneID = 456;
    int32_t result = server->GetSessionInfoInFocus(audioInterrupt, zoneID);
    EXPECT_EQ(result, ERR_UNKNOWN);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_048
* @tc.desc  : Test OnAudioParameterChange.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_038, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    std::string networkId;
    AudioParamKey key = VOLUME;
    std::string condition;
    std::string value;

    auto callback = std::make_shared<AudioPolicyServer::RemoteParameterCallback>(server);
    callback->OnAudioParameterChange(networkId, key, condition, value);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_049
* @tc.desc  : Test OnAudioParameterChange.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_039, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    std::string networkId;
    AudioParamKey key = INTERRUPT;
    std::string condition;
    std::string value;

    auto callback = std::make_shared<AudioPolicyServer::RemoteParameterCallback>(server);
    callback->OnAudioParameterChange(networkId, key, condition, value);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_050
* @tc.desc  : Test OnAudioParameterChange.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_040, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    std::string networkId;
    AudioParamKey key = PARAM_KEY_STATE;
    std::string condition;
    std::string value;
    auto callback = std::make_shared<AudioPolicyServer::RemoteParameterCallback>(server);
    callback->OnAudioParameterChange(networkId, key, condition, value);
}

/**
* @tc.name  : Test AudioPolicyServer.
* @tc.number: AudioPolicyServer_051
* @tc.desc  : Test OnAudioParameterChange.
*/
HWTEST(AudioPolicyUnitTest, AudioPolicyServer_041, TestSize.Level1)
{
    auto policyServerTest = GetPolicyServerUnitTest();
    int32_t systemAbilityId = 3009;
    bool runOnCreate = false;
    sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(systemAbilityId, runOnCreate);
    
    std::string networkId;
    AudioParamKey key = static_cast<AudioParamKey>(100);
    std::string condition;
    std::string value;
    auto callback = std::make_shared<AudioPolicyServer::RemoteParameterCallback>(server);
    callback->OnAudioParameterChange(networkId, key, condition, value);
}
}
}