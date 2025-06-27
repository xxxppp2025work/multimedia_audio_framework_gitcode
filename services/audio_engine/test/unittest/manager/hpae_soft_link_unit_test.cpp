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

#include <gtest/gtest.h>
#include "hpae_soft_link.h"
#include "hpae_manager_impl.h"
#include "hpae_audio_service_callback_unit_test.h"
namespace OHOS {
namespace AudioStandard {
namespace HPAE {
static std::string g_rootPath = "/data/";
class HpaeSoftLinkTest : public testing::Test {
public:
    void SetUp();
    void TearDown();
    IHpaeManager &hpaeManager_ = IHpaeManager::GetHpaeManager();
    std::shared_ptr<HpaeSoftLink> softLink_ = nullptr;
    int32_t sinkId_;
    int32_t sourceId_;
};

static void WaitForMsgProcessing(IHpaeManager> &hpaeManager)
{
    int waitCount = 0;
    const int waitCountThd = 5;
    while (hpaeManager.IsMsgProcessing()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20)); // 20 for sleep
        waitCount++;
        if (waitCount >= waitCountThd) {
            break;
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(40)); // 40 for sleep
    EXPECT_EQ(hpaeManager.IsMsgProcessing(), false);
    EXPECT_EQ(waitCount < waitCountThd, true);
}

static AudioModuleInfo GetSinkAudioModeInfo(std::string name = "Speaker_File")
{
    AudioModuleInfo audioModuleInfo;
    audioModuleInfo.lib = "libmodule-hdi-sink.z.so";
    audioModuleInfo.channels = "2";
    audioModuleInfo.rate = "48000";
    audioModuleInfo.name = name;
    audioModuleInfo.adapterName = "file_io";
    audioModuleInfo.className = "file_io";
    audioModuleInfo.bufferSize = "7680";
    audioModuleInfo.format = "s32le";
    audioModuleInfo.fixedLatency = "1";
    audioModuleInfo.offloadEnable = "0";
    audioModuleInfo.networkId = "LocalDevice";
    audioModuleInfo.fileName = g_rootPath + audioModuleInfo.adapterName + "_" + audioModuleInfo.rate + "_" +
                               audioModuleInfo.channels + "_" + audioModuleInfo.format + ".pcm";
    std::stringstream typeValue;
    typeValue << static_cast<int32_t>(DEVICE_TYPE_SPEAKER);
    audioModuleInfo.deviceType = typeValue.str();
    return audioModuleInfo;
}

static AudioModuleInfo GetSourceAudioModeInfo(std::string name = "mic")
{
    AudioModuleInfo audioModuleInfo;
    audioModuleInfo.lib = "libmodule-hdi-source.z.so";
    audioModuleInfo.channels = "2";
    audioModuleInfo.rate = "48000";
    audioModuleInfo.name = name;
    audioModuleInfo.adapterName = "file_io";
    audioModuleInfo.className = "file_io";
    audioModuleInfo.bufferSize = "3840";
    audioModuleInfo.format = "s16le";
    audioModuleInfo.fixedLatency = "1";
    audioModuleInfo.offloadEnable = "0";
    audioModuleInfo.networkId = "LocalDevice";
    audioModuleInfo.fileName = g_rootPath + "source_" + audioModuleInfo.adapterName + "_" + audioModuleInfo.rate + "_" +
                               audioModuleInfo.channels + "_" + audioModuleInfo.format + ".pcm";
    std::stringstream typeValue;
    typeValue << static_cast<int32_t>(DEVICE_TYPE_FILE_SOURCE);
    audioModuleInfo.deviceType = typeValue.str();
    return audioModuleInfo;
}

void HpaeSoftLinkTest::SetUp()
{
    hpaeManager_.Init();
    std::shared_ptr<HpaeAudioServiceCallbackUnitTest> callback = std::make_shared<HpaeAudioServiceCallbackUnitTest>();
    hpaeManager_.RegisterSerivceCallback(callback);
    AudioModuleInfo audioSinkModuleInfo = GetSinkAudioModeInfo();
    EXPECT_EQ(hpaeManager_.OpenAudioPort(audioSinkModuleInfo), SUCCESS);
    WaitForMsgProcessing(hpaeManager_);
    sinkId_ = callback->GetPortId();
    AudioModuleInfo audioSourceModuleInfo = GetSourceAudioModeInfo();
    EXPECT_EQ(hpaeManager_.OpenAudioPort(audioSinkModuleInfo), SUCCESS);
    WaitForMsgProcessing(hpaeManager_);
    sourceId_ = callback->GetPortId();
}

void HpaeSoftLinkTest::TearDown()
{
    hpaeManager_.CloseAudioPort(sinkId_);
    WaitForMsgProcessing(hpaeManager_);
    hpaeManager_.CloseAudioPort(sourceId_);
    WaitForMsgProcessing(hpaeManager_);
    hpaeManager_->DeInit();
}

TEST_F(HpaeSoftLinkTest, testSoftLink)
{
    softLink_ = std::make_shared<HpaeSoftLink>(sinkId_, sourceId_, SoftLinkMode::HEARING_AID);
    EXPECT_NE(softLink_, nullptr);
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::NEW);
    softLink_->Init();
    WaitForMsgProcessing(hpaeManager_);
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::PREPARED);
    softLink_->Start();
    WaitForMsgProcessing(hpaeManager_);
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::RUNNING);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // 2s for sleep
    softLink_->Stop();
    WaitForMsgProcessing(hpaeManager_);
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::STOPPED);
    softLink_->Release();
    WaitForMsgProcessing(hpaeManager_);
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::RELEASED);
}
} // namespace HPAE
} // namespace AudioStandard
} // namespace OHOS
