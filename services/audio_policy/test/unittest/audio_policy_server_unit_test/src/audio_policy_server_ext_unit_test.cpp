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
#include "audio_policy_server_ext_unit_test.h"
#include "audio_info.h"
#include "audio_policy_server.h"
#include "audio_policy_service.h"
#include "audio_session_info.h"
#include <memory>
#include <thread>
#include <vector>

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {
const int32_t SYSTEM_ABILITY_ID = 3009;
const bool RUN_ON_CREATE = false;

void AudioPolicyServerExtUnitTest::SetUpTestCase(void) {}
void AudioPolicyServerExtUnitTest::TearDownTestCase(void) {}
void AudioPolicyServerExtUnitTest::SetUp(void) {}
void AudioPolicyServerExtUnitTest::TearDown(void) {}

bool g_isInit = false;
AudioPolicyServer *GetServerSptr()
{
    static AudioPolicyServer server(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (!g_isInit) {
        server.OnStart();
        server.OnAddSystemAbility(DISTRIBUTED_KV_DATA_SERVICE_ABILITY_ID, "");
        server.OnAddSystemAbility(DISTRIBUTED_HARDWARE_DEVICEMANAGER_SA_ID, "");
        server.OnAddSystemAbility(ACCESSIBILITY_MANAGER_SERVICE_ID, "AccessibilityManagerService");
        server.OnAddSystemAbility(COMMON_EVENT_SERVICE_ID, "");
        server.OnAddSystemAbility(AUDIO_DISTRIBUTED_SERVICE_ID, "");
        server.OnAddSystemAbility(MULTIMODAL_INPUT_SERVICE_ID, "");
        server.OnAddSystemAbility(BLUETOOTH_HOST_SYS_ABILITY_ID, "");
        server.OnAddSystemAbility(POWER_MANAGER_SERVICE_ID, "");
        server.OnAddSystemAbility(SUBSYS_ACCOUNT_SYS_ABILITY_ID_BEGIN, "");
        server.audioPolicyService_.SetDefaultDeviceLoadFlag(true);
        g_isInit = true;
    }
    return &server;
}

/**
 * @tc.name  : Test StreamVolumesDump.
 * @tc.number: StreamVolumesDump_001
 * @tc.desc  : Test StreamVolumesDump interfaces.
 */
HWTEST(AudioPolicyServerExtUnitTest, StreamVolumesDump_001, TestSize.Level1)
{
    auto server = GetServerSptr();
    std::string dumpString = "666";
    server->AudioVolumeDump(dumpString);
    EXPECT_NE(dumpString, "666");
}

/**
 * @tc.name  : Test AudioStreamDump.
 * @tc.number: AudioStreamDump_001
 * @tc.desc  : Test AudioStreamDump interfaces.
 */
HWTEST(AudioPolicyServerExtUnitTest, AudioStreamDump_001, TestSize.Level1)
{
    auto server = GetServerSptr();
    std::string dumpString = "666";
    server->AudioStreamDump(dumpString);
    EXPECT_NE(dumpString, "666");
}

/**
 * @tc.name  : Test CheckAudioSessionStrategy.
 * @tc.number: CheckAudioSessionStrategy_001
 * @tc.desc  : Test CheckAudioSessionStrategy interfaces.
 */
HWTEST(AudioPolicyServerExtUnitTest, CheckAudioSessionStrategy_001, TestSize.Level1)
{
    auto server = GetServerSptr();
    AudioSessionStrategy sessionStrategy;
    sessionStrategy.concurrencyMode = AudioConcurrencyMode::DEFAULT;
    bool ret = server->CheckAudioSessionStrategy(sessionStrategy);
    EXPECT_EQ(ret, true);

    sessionStrategy.concurrencyMode = (AudioConcurrencyMode)666;
    ret = server->CheckAudioSessionStrategy(sessionStrategy);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name  : Test LoadSplitModule.
 * @tc.number: LoadSplitModule_001
 * @tc.desc  : Test LoadSplitModule interfaces.
 */
HWTEST(AudioPolicyServerExtUnitTest, LoadSplitModule_001, TestSize.Level1)
{
    auto server = GetServerSptr();
    std::string splitArgs = "";
    std::string networkId = "";
    int32_t ret = server->audioPolicyService_.LoadSplitModule(splitArgs, networkId);
    EXPECT_EQ(ret, ERR_INVALID_PARAM);

    splitArgs = "test";
    networkId = LOCAL_NETWORK_ID;
    ret = server->audioPolicyService_.LoadSplitModule(splitArgs, networkId);
    EXPECT_EQ(ret, ERR_INVALID_HANDLE);
}

/**
 * @tc.name  : Test HandleA2dpDeviceInOffload.
 * @tc.number: HandleA2dpDeviceInOffload_001
 * @tc.desc  : Test HandleA2dpDeviceInOffload interfaces.
 */
HWTEST(AudioPolicyServerExtUnitTest, HandleA2dpDeviceInOffload_001, TestSize.Level1)
{
    auto server = GetServerSptr();
    BluetoothOffloadState a2dpOffloadFlag = A2DP_NOT_OFFLOAD;
    int32_t ret = server->audioPolicyService_.HandleA2dpDeviceInOffload(a2dpOffloadFlag);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test ReconfigureAudioChannel.
 * @tc.number: ReconfigureAudioChannel_001
 * @tc.desc  : Test ReconfigureAudioChannel interfaces.
 */
HWTEST(AudioPolicyServerExtUnitTest, ReconfigureAudioChannel_001, TestSize.Level1)
{
    auto server = GetServerSptr();
    uint32_t channelCount = 1;
    DeviceType deviceType = DeviceType::DEVICE_TYPE_SPEAKER;
    int32_t ret = server->audioPolicyService_.ReconfigureAudioChannel(channelCount, deviceType);
    EXPECT_EQ(ret, ERROR);
}

/**
 * @tc.name  : Test GetSinkIOHandle.
 * @tc.number: GetSinkIOHandle_001
 * @tc.desc  : Test GetSinkIOHandle interfaces.
 */
HWTEST(AudioPolicyServerExtUnitTest, GetSinkIOHandle_001, TestSize.Level1)
{
    auto server = GetServerSptr();
    InternalDeviceType deviceType = DeviceType::DEVICE_TYPE_SPEAKER;
    int32_t ret = server->audioPolicyService_.GetSinkIOHandle(deviceType);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test GetSourceIOHandle.
 * @tc.number: GetSourceIOHandle_001
 * @tc.desc  : Test GetSourceIOHandle interfaces.
 */
HWTEST(AudioPolicyServerExtUnitTest, GetSourceIOHandle_001, TestSize.Level1)
{
    auto server = GetServerSptr();
    InternalDeviceType deviceType = DeviceType::DEVICE_TYPE_SPEAKER;
    int32_t ret = server->audioPolicyService_.GetSourceIOHandle(deviceType);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test SetAbsVolumeSceneAsync.
 * @tc.number: SetAbsVolumeSceneAsync_001
 * @tc.desc  : Test SetAbsVolumeSceneAsync interfaces.
 */
HWTEST(AudioPolicyServerExtUnitTest, SetAbsVolumeSceneAsync_001, TestSize.Level1)
{
    auto server = GetServerSptr();
    std::string macAddress = "";
    bool support = false;
    server->audioPolicyService_.SetAbsVolumeSceneAsync(macAddress, support);
    EXPECT_EQ(support, false);
}

/**
 * @tc.name  : Test SetDeviceAbsVolumeSupported.
 * @tc.number: SetDeviceAbsVolumeSupported_001
 * @tc.desc  : Test SetDeviceAbsVolumeSupported interfaces.
 */
HWTEST(AudioPolicyServerExtUnitTest, SetDeviceAbsVolumeSupported_001, TestSize.Level1)
{
    auto server = GetServerSptr();
    std::string macAddress = "";
    bool support = false;
    int32_t ret = server->audioPolicyService_.SetDeviceAbsVolumeSupported(macAddress, support);
    EXPECT_EQ(ret, ERROR);
}

/**
 * @tc.name  : Test IsWiredHeadSet.
 * @tc.number: IsWiredHeadSet_001
 * @tc.desc  : Test IsWiredHeadSet interfaces.
 */
HWTEST(AudioPolicyServerExtUnitTest, IsWiredHeadSet_001, TestSize.Level1)
{
    auto server = GetServerSptr();
    DeviceType deviceType = DeviceType::DEVICE_TYPE_SPEAKER;
    int32_t ret = server->audioPolicyService_.IsWiredHeadSet(deviceType);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test IsBlueTooth.
 * @tc.number: IsBlueTooth_001
 * @tc.desc  : Test IsBlueTooth interfaces.
 */
HWTEST(AudioPolicyServerExtUnitTest, IsBlueTooth_001, TestSize.Level1)
{
    auto server = GetServerSptr();
    DeviceType deviceType = DeviceType::DEVICE_TYPE_SPEAKER;
    int32_t ret = server->audioPolicyService_.IsBlueTooth(deviceType);
    EXPECT_EQ(ret, SUCCESS);
}

} // namespace AudioStandard
} // namespace OHOS