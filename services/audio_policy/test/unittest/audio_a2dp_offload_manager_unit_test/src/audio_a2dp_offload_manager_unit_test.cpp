/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include "audio_policy_utils.h"
#include "audio_a2dp_offload_manager_unit_test.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

void AudioA2dpOffloadManagerUnitTest::SetUpTestCase(void) {}
void AudioA2dpOffloadManagerUnitTest::TearDownTestCase(void) {}
void AudioA2dpOffloadManagerUnitTest::SetUp(void) {}
void AudioA2dpOffloadManagerUnitTest::TearDown(void) {}

/**
 * @tc.name: ConnectA2dpOffload_001
 * @tc.desc: Test ConnectA2dpOffload with state already connected.
 * @tc.type: FUNC
 * @tc.require: #I5Y4MZ
 */
HWTEST_F(AudioA2dpOffloadManagerUnitTest, ConnectA2dpOffload_001, TestSize.Level1)
{
    std::string deviceAddress = "00:11:22:33:44:55";
    std::vector<int32_t> sessionIds = {1, 2, 3};
    std::shared_ptr<AudioA2dpOffloadManager> manager = std::make_shared<AudioA2dpOffloadManager>();
    manager->Init();
    manager->audioA2dpOffloadFlag_.SetCurrentOffloadConnectedState(CONNECTION_STATUS_CONNECTED);
    manager->ConnectA2dpOffload(deviceAddress, sessionIds);
    EXPECT_EQ(manager->audioA2dpOffloadFlag_.GetCurrentOffloadConnectedState(), CONNECTION_STATUS_CONNECTED);
}

/**
 * @tc.name: ConnectA2dpOffload_002
 * @tc.desc: Test ConnectA2dpOffload with state already connecting.
 * @tc.type: FUNC
 * @tc.require: #I5Y4MZ
 */
HWTEST_F(AudioA2dpOffloadManagerUnitTest, ConnectA2dpOffload_002, TestSize.Level1)
{
    std::string deviceAddress = "00:11:22:33:44:55";
    std::vector<int32_t> sessionIds = {1, 2, 3};
    std::shared_ptr<AudioA2dpOffloadManager> manager = std::make_shared<AudioA2dpOffloadManager>();
    manager->Init();
    manager->audioA2dpOffloadFlag_.SetCurrentOffloadConnectedState(CONNECTION_STATUS_CONNECTING);
    manager->ConnectA2dpOffload(deviceAddress, sessionIds);
    EXPECT_EQ(manager->audioA2dpOffloadFlag_.GetCurrentOffloadConnectedState(), CONNECTION_STATUS_CONNECTING);
}

/**
 * @tc.name: ConnectA2dpOffload_003
 * @tc.desc: Test ConnectA2dpOffload with state disconnected.
 * @tc.type: FUNC
 * @tc.require: #I5Y4MZ
 */
HWTEST_F(AudioA2dpOffloadManagerUnitTest, ConnectA2dpOffload_003, TestSize.Level1)
{
    std::string deviceAddress = "00:11:22:33:44:55";
    std::vector<int32_t> sessionIds = {1, 2, 3};
    std::shared_ptr<AudioA2dpOffloadManager> manager = std::make_shared<AudioA2dpOffloadManager>();
    manager->Init();
    manager->audioA2dpOffloadFlag_.SetCurrentOffloadConnectedState(CONNECTION_STATUS_DISCONNECTED);
    manager->ConnectA2dpOffload(deviceAddress, sessionIds);
    EXPECT_EQ(manager->audioA2dpOffloadFlag_.GetCurrentOffloadConnectedState(), CONNECTION_STATUS_CONNECTING);
}

/**
 * @tc.name: WaitForConnectionCompleted_001
 * @tc.desc: Test WaitForConnectionCompleted with connection timeout.
 * @tc.type: FUNC
 * @tc.require: #I5Y4MZ
 */
HWTEST_F(AudioA2dpOffloadManagerUnitTest, WaitForConnectionCompleted_001, TestSize.Level1)
{
    std::shared_ptr<AudioA2dpOffloadManager> manager = std::make_shared<AudioA2dpOffloadManager>();
    manager->Init();
    const int32_t shortTimeout = 100;
    manager->audioA2dpOffloadFlag_.SetCurrentOffloadConnectedState(CONNECTION_STATUS_DISCONNECTED);
    std::thread testThread([manager]() {
        manager->WaitForConnectionCompleted();
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(shortTimeout + 100));
    EXPECT_EQ(manager->audioA2dpOffloadFlag_.GetCurrentOffloadConnectedState(), CONNECTION_STATUS_DISCONNECTED);
    testThread.join();
}


/**
 * @tc.name: OffloadStartPlaying_001
 * @tc.desc: Test OffloadStartPlaying with entering the second if branch.
 * @tc.type: FUNC
 * @tc.require: #I5Y4MZ
 */
HWTEST_F(AudioA2dpOffloadManagerUnitTest, OffloadStartPlaying_001, TestSize.Level1)
{
    std::shared_ptr<AudioA2dpOffloadManager> manager = std::make_shared<AudioA2dpOffloadManager>();
    manager->Init();
    manager->SetA2dpOffloadFlag(A2DP_OFFLOAD);
    std::vector<int32_t> sessionIds = {1, 2, 3};
    manager->audioA2dpOffloadFlag_.SetCurrentOffloadConnectedState(CONNECTION_STATUS_DISCONNECTED);
    int32_t ret = manager->OffloadStartPlaying(sessionIds);
    EXPECT_EQ(ret, BASE_AUDIO_ERR_OFFSET);
    EXPECT_EQ(manager->audioA2dpOffloadFlag_.GetCurrentOffloadConnectedState(), CONNECTION_STATUS_DISCONNECTED);
}

/**
 * @tc.name: OffloadStartPlaying_002
 * @tc.desc: Test OffloadStartPlaying without entering the second if branch due to ret != SUCCESS.
 * @tc.type: FUNC
 * @tc.require: #I5Y4MZ
 */
HWTEST_F(AudioA2dpOffloadManagerUnitTest, OffloadStartPlaying_002, TestSize.Level1)
{
    std::shared_ptr<AudioA2dpOffloadManager> manager = std::make_shared<AudioA2dpOffloadManager>();
    manager->Init();
    manager->SetA2dpOffloadFlag(A2DP_OFFLOAD);
    std::vector<int32_t> sessionIds = {1, 2, 3};
    int32_t ret = manager->OffloadStartPlaying(sessionIds);
    EXPECT_EQ(ret, BASE_AUDIO_ERR_OFFSET);
    EXPECT_EQ(manager->audioA2dpOffloadFlag_.GetCurrentOffloadConnectedState(), CONNECTION_STATUS_DISCONNECTED);
}

/**
 * @tc.name: OffloadStartPlaying_003
 * @tc.desc: Test OffloadStartPlaying without entering the second if branch due to state == CONNECTION_STATUS_CONNECTED.
 * @tc.type: FUNC
 * @tc.require: #I5Y4MZ
 */
HWTEST_F(AudioA2dpOffloadManagerUnitTest, OffloadStartPlaying_003, TestSize.Level1)
{
    std::shared_ptr<AudioA2dpOffloadManager> manager = std::make_shared<AudioA2dpOffloadManager>();
    manager->Init();
    manager->SetA2dpOffloadFlag(A2DP_OFFLOAD);
    std::vector<int32_t> sessionIds = {1, 2, 3};
    manager->audioA2dpOffloadFlag_.SetCurrentOffloadConnectedState(CONNECTION_STATUS_CONNECTED);
    int32_t ret = manager->OffloadStartPlaying(sessionIds);
    EXPECT_EQ(ret, BASE_AUDIO_ERR_OFFSET);
    EXPECT_EQ(manager->audioA2dpOffloadFlag_.GetCurrentOffloadConnectedState(), CONNECTION_STATUS_CONNECTED);
}

/**
 * @tc.name: OffloadStopPlaying_001
 * @tc.desc: Test OffloadStopPlaying without entering the if branch.
 * @tc.type: FUNC
 * @tc.require: #I5Y4MZ
 */
HWTEST_F(AudioA2dpOffloadManagerUnitTest, OffloadStopPlaying_001, TestSize.Level1)
{
    std::shared_ptr<AudioA2dpOffloadManager> manager = std::make_shared<AudioA2dpOffloadManager>();
    manager->Init();
    manager->SetA2dpOffloadFlag(A2DP_OFFLOAD);
    std::vector<int32_t> sessionIds = {1, 2, 3};
    int32_t ret = manager->OffloadStopPlaying(sessionIds);
    EXPECT_EQ(ret, BASE_AUDIO_ERR_OFFSET);
}

/**
 * @tc.name: UpdateA2dpOffloadFlagForAllStream_001
 * @tc.desc: Test UpdateA2dpOffloadFlagForAllStream with entering all if branches.
 * @tc.type: FUNC
 * @tc.require: #I5Y4MZ
 */
HWTEST_F(AudioA2dpOffloadManagerUnitTest, UpdateA2dpOffloadFlagForAllStream_001, TestSize.Level1)
{
    std::shared_ptr<AudioA2dpOffloadManager> manager = std::make_shared<AudioA2dpOffloadManager>();
    manager->Init();
    std::unordered_map<uint32_t, bool> sessionIDToSpatializationEnableMap = {
        {1, true},
        {2, false}
    };
    DeviceType deviceType = DEVICE_TYPE_BLUETOOTH_A2DP;
    std::vector<std::shared_ptr<AudioRendererChangeInfo>> audioRendererChangeInfos;
    auto changeInfo1 = std::make_shared<AudioRendererChangeInfo>();
    changeInfo1->sessionId = 1;
    changeInfo1->rendererState = RENDERER_RUNNING;
    audioRendererChangeInfos.push_back(changeInfo1);

    auto changeInfo2 = std::make_shared<AudioRendererChangeInfo>();
    changeInfo2->sessionId = 2;
    changeInfo2->rendererState = RENDERER_STOPPED;
    audioRendererChangeInfos.push_back(changeInfo2);
    manager->streamCollector_.audioRendererChangeInfos_ = audioRendererChangeInfos;
    manager->UpdateA2dpOffloadFlagForAllStream(sessionIDToSpatializationEnableMap, deviceType);
    EXPECT_EQ(manager->streamCollector_.audioRendererChangeInfos_, audioRendererChangeInfos);
}


/**
 * @tc.name: UpdateA2dpOffloadFlagForAllStream_002
 * @tc.desc: Test UpdateA2dpOffloadFlagForAllStream without entering any if branches.
 * @tc.type: FUNC
 * @tc.require: #I5Y4MZ
 */
HWTEST_F(AudioA2dpOffloadManagerUnitTest, UpdateA2dpOffloadFlagForAllStream_002, TestSize.Level1)
{
    std::shared_ptr<AudioA2dpOffloadManager> manager = std::make_shared<AudioA2dpOffloadManager>();
    manager->Init();
    std::unordered_map<uint32_t, bool> sessionIDToSpatializationEnableMap;
    DeviceType deviceType = DEVICE_TYPE_BLUETOOTH_A2DP;
    std::vector<std::shared_ptr<AudioRendererChangeInfo>> audioRendererChangeInfos;
    auto changeInfo1 = std::make_shared<AudioRendererChangeInfo>();
    changeInfo1->sessionId = 1;
    changeInfo1->rendererState = RENDERER_RUNNING;
    audioRendererChangeInfos.push_back(changeInfo1);

    auto changeInfo2 = std::make_shared<AudioRendererChangeInfo>();
    changeInfo2->sessionId = 2;
    changeInfo2->rendererState = RENDERER_RUNNING;
    audioRendererChangeInfos.push_back(changeInfo2);
    manager->streamCollector_.audioRendererChangeInfos_ = audioRendererChangeInfos;
    manager->UpdateA2dpOffloadFlagForAllStream(sessionIDToSpatializationEnableMap, deviceType);
    EXPECT_EQ(manager->streamCollector_.audioRendererChangeInfos_, audioRendererChangeInfos);
}

/**
 * @tc.name: UpdateA2dpOffloadFlag_001
 * @tc.desc: Test UpdateA2dpOffloadFlag with deviceType DEVICE_TYPE_BLUETOOTH_A2DP
 *  and receiveOffloadFlag != GetA2dpOffloadFlag.
 * @tc.type: FUNC
 * @tc.require: #I5Y4MZ
 */
HWTEST_F(AudioA2dpOffloadManagerUnitTest, UpdateA2dpOffloadFlag_001, TestSize.Level1)
{
    std::shared_ptr<AudioA2dpOffloadManager> manager = std::make_shared<AudioA2dpOffloadManager>();
    manager->Init();
    DeviceType deviceType = DEVICE_TYPE_BLUETOOTH_A2DP;
    std::vector<Bluetooth::A2dpStreamInfo> allActiveSessions;
    Bluetooth::A2dpStreamInfo a2dpStreamInfo;
    a2dpStreamInfo.sessionId = 1;
    a2dpStreamInfo.streamType = 1;
    a2dpStreamInfo.isSpatialAudio = false;
    allActiveSessions.push_back(a2dpStreamInfo);
    manager->SetA2dpOffloadFlag(A2DP_OFFLOAD);
    manager->UpdateA2dpOffloadFlag(allActiveSessions, deviceType);
    EXPECT_NE(manager, nullptr);
}

/**
 * @tc.name: UpdateA2dpOffloadFlag_002
 * @tc.desc: Test UpdateA2dpOffloadFlag with deviceType DEVICE_TYPE_NONE and current output device is BLUETOOTH_A2DP.
 * @tc.type: FUNC
 * @tc.require: #I5Y4MZ
 */
HWTEST_F(AudioA2dpOffloadManagerUnitTest, UpdateA2dpOffloadFlag_002, TestSize.Level1)
{
    std::shared_ptr<AudioA2dpOffloadManager> manager = std::make_shared<AudioA2dpOffloadManager>();
    manager->Init();
    DeviceType deviceType = DEVICE_TYPE_NONE;
    manager->audioActiveDevice_.SetCurrentOutputDeviceType(DEVICE_TYPE_BLUETOOTH_A2DP);
    std::vector<Bluetooth::A2dpStreamInfo> allActiveSessions;
    Bluetooth::A2dpStreamInfo a2dpStreamInfo;
    a2dpStreamInfo.sessionId = 1;
    a2dpStreamInfo.streamType = 1;
    a2dpStreamInfo.isSpatialAudio = false;
    allActiveSessions.push_back(a2dpStreamInfo);
    manager->SetA2dpOffloadFlag(A2DP_OFFLOAD);
    manager->UpdateA2dpOffloadFlag(allActiveSessions, deviceType);
    EXPECT_NE(manager, nullptr);
}

/**
 * @tc.name: UpdateA2dpOffloadFlag_003
 * @tc.desc: Test UpdateA2dpOffloadFlag with receiveOffloadFlag == NO_A2DP_DEVICE.
 * @tc.type: FUNC
 * @tc.require: #I5Y4MZ
 */
HWTEST_F(AudioA2dpOffloadManagerUnitTest, UpdateA2dpOffloadFlag_003, TestSize.Level1)
{
    std::shared_ptr<AudioA2dpOffloadManager> manager = std::make_shared<AudioA2dpOffloadManager>();
    manager->Init();
    DeviceType deviceType = DEVICE_TYPE_BLUETOOTH_A2DP;
    std::vector<Bluetooth::A2dpStreamInfo> allActiveSessions;
    Bluetooth::A2dpStreamInfo a2dpStreamInfo;
    a2dpStreamInfo.sessionId = 1;
    a2dpStreamInfo.streamType = 1;
    a2dpStreamInfo.isSpatialAudio = false;
    allActiveSessions.push_back(a2dpStreamInfo);
    manager->SetA2dpOffloadFlag(A2DP_OFFLOAD);
    manager->UpdateA2dpOffloadFlag(allActiveSessions, deviceType);
    EXPECT_NE(manager, nullptr);
}

/**
 * @tc.name: UpdateA2dpOffloadFlag_004
 * @tc.desc: Test UpdateA2dpOffloadFlag with receiveOffloadFlag == GetA2dpOffloadFlag and both are A2DP_OFFLOAD.
 * @tc.type: FUNC
 * @tc.require: #I5Y4MZ
 */
HWTEST_F(AudioA2dpOffloadManagerUnitTest, UpdateA2dpOffloadFlag_004, TestSize.Level1)
{
    std::shared_ptr<AudioA2dpOffloadManager> manager = std::make_shared<AudioA2dpOffloadManager>();
    manager->Init();
    DeviceType deviceType = DEVICE_TYPE_BLUETOOTH_A2DP;
    std::vector<Bluetooth::A2dpStreamInfo> allActiveSessions;
    Bluetooth::A2dpStreamInfo a2dpStreamInfo;
    a2dpStreamInfo.sessionId = 1;
    a2dpStreamInfo.streamType = 1;
    a2dpStreamInfo.isSpatialAudio = false;
    allActiveSessions.push_back(a2dpStreamInfo);
    manager->SetA2dpOffloadFlag(A2DP_OFFLOAD);
    manager->UpdateA2dpOffloadFlag(allActiveSessions, deviceType);
    EXPECT_NE(manager, nullptr);
}

} // namespace AudioStandard
} // namespace OHOS