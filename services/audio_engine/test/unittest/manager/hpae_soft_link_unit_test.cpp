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
    void OpenAudioPort(bool openSink = true);
    void CloseAudioPort(bool closeSink = true);
    IHpaeManager &hpaeManager_ = IHpaeManager::GetHpaeManager();
    std::shared_ptr<HpaeAudioServiceCallbackUnitTest> callback_ = nullptr;
    std::shared_ptr<HpaeSoftLink> softLink_ = nullptr;
    int32_t sinkId_;
    int32_t sourceId_;
};

static void WaitForMsgProcessing(IHpaeManager &hpaeManager)
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
    callback_ = std::make_shared<HpaeAudioServiceCallbackUnitTest>();
    hpaeManager_.RegisterSerivceCallback(callback_);
    OpenAudioPort();
    OpenAudioPort(false);
}

void HpaeSoftLinkTest::TearDown()
{
    CloseAudioPort();
    CloseAudioPort(false);
    hpaeManager_.DeInit();
}

void HpaeSoftLinkTest::OpenAudioPort(bool openSink)
{
    AudioModuleInfo moduleInfo = openSink ? GetSinkAudioModeInfo() : GetSourceAudioModeInfo();
    EXPECT_EQ(hpaeManager_.OpenAudioPort(moduleInfo), SUCCESS);
    WaitForMsgProcessing(hpaeManager_);
    openSink ? sinkId_ : sourceId_ = callback_->GetPortId();
}

void HpaeSoftLinkTest::CloseAudioPort(bool closeSink)
{
    hpaeManager_.CloseAudioPort(closeSink ? sinkId_ : sourceId_);
    WaitForMsgProcessing(hpaeManager_);
}

TEST_F(HpaeSoftLinkTest, testSoftLink)
{
    softLink_ = std::make_shared<HpaeSoftLink>(sinkId_, sourceId_, SoftLinkMode::HEARING_AID);
    EXPECT_NE(softLink_, nullptr);
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::NEW);

    EXPECT_EQ(softLink_->Init(), SUCCESS);
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::PREPARED);

    EXPECT_EQ(softLink_->Init(), SUCCESS); // init after init
    EXPECT_EQ(softLink_->Stop(), ERR_ILLEGAL_STATE); // stop after init

    EXPECT_EQ(softLink_->Start(), SUCCESS);
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::RUNNING);

    EXPECT_EQ(softLink_->Init(), ERR_ILLEGAL_STATE); // init after start
    EXPECT_EQ(softLink_->Start(), SUCCESS); // start after start

    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // 2s for sleep

    EXPECT_EQ(softLink_->Stop(), SUCCESS);
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::STOPPED);

    EXPECT_EQ(softLink_->Stop(), SUCCESS); // stop after stop

    softLink_->Release();
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::RELEASED);
    softLink_ = nullptr;
}

TEST_F(HpaeSoftLinkTest, testCapturerOverFlow)
{
    softLink_ = std::make_shared<HpaeSoftLink>(sinkId_, sourceId_, SoftLinkMode::HEARING_AID);
    EXPECT_NE(softLink_, nullptr);
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::NEW);
    EXPECT_EQ(softLink_->Init(), SUCCESS);
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::PREPARED);
    EXPECT_EQ(softLink_->Start(), SUCCESS);
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::RUNNING);

    auto &capturerSessionId = softLink_->capturerStreamInfo_.sessionId;
    if (softLink_->state_ == HpaeSoftLinkState::RUNNING &&
        softLink_->streamStateMap_.find(capturerSessionId) != softLink_->streamStateMap_.end()) {
        EXPECT_EQ(softLink_->streamStateMap_[capturerSessionId], HpaeSoftLinkState::RUNNING);
    }

    CloseAudioPort();
    std::this_trhead::sleep_for(std::chrono::milliseconds(2000)); // 2s for sleep
    if (softLink_->state_ == HpaeSoftLinkState::RUNNING &&
        softLink_->streamStateMap_.find(capturerSessionId) != softLink_->streamStateMap_.end()) {
        AUDIO_INFO_LOG("capturer has stopped");
        EXPECT_EQ(softLink_->streamStateMap_[capturerSessionId], HpaeSoftLinkState::STOPPED);
    }
    softLink_->Release();
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::RELEASED);
    softLink_ = nullptr;
}

TEST_F(HpaeSoftLinkTest, testRendererUnderRun)
{
    OpenAudioPort();
    softLink_ = std::make_shared<HpaeSoftLink>(sinkId_, sourceId_, SoftLinkMode::HEARING_AID);
    EXPECT_NE(softLink_, nullptr);
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::NEW);
    EXPECT_EQ(softLink_->Init(), SUCCESS);
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::PREPARED);
    EXPECT_EQ(softLink_->Start(), SUCCESS);
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::RUNNING);

    auto &rendererSessionId = softLink_->rendererStreamInfo_.sessionId;
    if (softLink_->state_ == HpaeSoftLinkState::RUNNING &&
        softLink_->streamStateMap_.find(rendererSessionId) != softLink_->streamStateMap_.end()) {
        EXPECT_EQ(softLink_->streamStateMap_[rendererSessionId], HpaeSoftLinkState::RUNNING);
    }

    CloseAudioPort(false);
    std::this_trhead::sleep_for(std::chrono::milliseconds(2000)); // 2s for sleep
    if (softLink_->state_ == HpaeSoftLinkState::RUNNING &&
        softLink_->streamStateMap_.find(rendererSessionId) != softLink_->streamStateMap_.end()) {
        AUDIO_INFO_LOG("capturer has stopped");
        EXPECT_EQ(softLink_->streamStateMap_[rendererSessionId], HpaeSoftLinkState::STOPPED);
    }
    softLink_->Release();
    EXPECT_EQ(softLink_->state_, HpaeSoftLinkState::RELEASED);
    softLink_ = nullptr;
}

TEST_F(HpaeSoftLinkTest, testStaticFunc)
{
    OpenAudioPort();
    OpenAudioPort(false);
    std::shared_ptr<IHpaeSoftLink> softLink1 = 
        IHpaeSoftLink::CreateSoftLink(sinkId_, sourceId_, SoftLinkMode::HEARING_AID);
    EXPECT_NE(softLink1, nullptr);

    CloseAudioPort(false);
    WaitForMsgProcessing(hpaeManager_);
    std::shared_ptr<IHpaeSoftLink> softLink2 = 
        IHpaeSoftLink::CreateSoftLink(sinkId_, sourceId_, SoftLinkMode::HEARING_AID);
    EXPECT_EQ(softLink2, nullptr);

    std::shared_ptr<IHpaeSoftLink> softLink3 = 
        IHpaeSoftLink::CreateSoftLink(sinkId_, -1, SoftLinkMode::HEARING_AID);
    EXPECT_EQ(softLink3, nullptr);

    HpaeSoftLink::g_sessionId = 99999; // 99999 for max sessionId;
    EXPECT_EQ(HpaeSoftLink::GenerateSessionId(), 99999); // 99999 for max sessionId;
    EXPECT_EQ(HpaeSoftLink::g_sessionId, 90000); // 90000 for min sessionId;
}
} // namespace HPAE
} // namespace AudioStandard
} // namespace OHOS
