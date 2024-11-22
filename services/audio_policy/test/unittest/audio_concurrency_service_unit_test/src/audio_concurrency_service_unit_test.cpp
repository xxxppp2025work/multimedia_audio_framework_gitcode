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

#include "audio_concurrency_service_unit_test.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

void AudioConcurrencyServiceUnitTest::SetUpTestCase(void) {}
void AudioConcurrencyServiceUnitTest::TearDownTestCase(void) {}
void AudioConcurrencyServiceUnitTest::SetUp(void) {}
void AudioConcurrencyServiceUnitTest::TearDown(void) {}

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

/**
* @tc.name  : Test AudioConcurrencyService.
* @tc.number: AudioEffectConfigParser_001.
* @tc.desc  : Test SetAudioConcurrencyCallback interfaces.
*/
HWTEST(AudioConcurrencyServiceUnitTest, AudioConcurrencyServiceUnitTest_001, TestSize.Level1)
{
    uint32_t sessionID = 0;
    sptr<IRemoteObject> object = new RemoteObjectTestStub();
    auto audioConcurrency_ = std::make_shared<AudioConcurrencyService>();
    int32_t result = audioConcurrency_->SetAudioConcurrencyCallback(sessionID, object);
    EXPECT_EQ(result, SUCCESS);

    auto audioConcurrencyClient_ = std::make_shared<AudioConcurrencyService::AudioConcurrencyClient>(nullptr,
        object, nullptr, sessionID);
    audioConcurrency_->concurrencyClients_.insert({sessionID, audioConcurrencyClient_});
    result = audioConcurrency_->SetAudioConcurrencyCallback(sessionID, object);
    EXPECT_EQ(result, ERR_INVALID_PARAM);
}

/**
* @tc.name  : Test AudioConcurrencyService.
* @tc.number: AudioEffectConfigParser_002.
* @tc.desc  : Test UnsetAudioConcurrencyCallback interfaces.
*/
HWTEST(AudioConcurrencyServiceUnitTest, AudioConcurrencyServiceUnitTest_002, TestSize.Level1)
{
    uint32_t sessionID = 0;
    auto audioConcurrency_ = std::make_shared<AudioConcurrencyService>();
    int32_t result = audioConcurrency_->UnsetAudioConcurrencyCallback(sessionID);
    EXPECT_EQ(result, ERR_INVALID_PARAM);

    sptr<IRemoteObject> object = new RemoteObjectTestStub();
    auto audioConcurrencyClient_ = std::make_shared<AudioConcurrencyService::AudioConcurrencyClient>(nullptr,
        object, nullptr, sessionID);
    audioConcurrency_->concurrencyClients_.insert({sessionID, audioConcurrencyClient_});
    result = audioConcurrency_->UnsetAudioConcurrencyCallback(sessionID);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test AudioConcurrencyService.
* @tc.number: AudioEffectConfigParser_003.
* @tc.desc  : Test ActivateAudioConcurrency interfaces.
*/
HWTEST(AudioConcurrencyServiceUnitTest, AudioConcurrencyServiceUnitTest_003, TestSize.Level1)
{
    std::vector<std::shared_ptr<AudioRendererChangeInfo>> audioRendererChangeInfos;
    std::vector<std::shared_ptr<AudioCapturerChangeInfo>> audioCapturerChangeInfos;
    auto audioConcurrency_ = std::make_shared<AudioConcurrencyService>();
    int32_t result = audioConcurrency_->ActivateAudioConcurrency(AudioPipeType::PIPE_TYPE_UNKNOWN,
    audioRendererChangeInfos, audioCapturerChangeInfos);
    EXPECT_EQ(result, SUCCESS);

    std::pair<AudioPipeType, AudioPipeType> dsc = std::make_pair(AudioPipeType::PIPE_TYPE_UNKNOWN,
        AudioPipeType::PIPE_TYPE_UNKNOWN);
    audioConcurrency_->concurrencyCfgMap_.insert({dsc, ConcurrencyAction::PLAY_BOTH});

    std::shared_ptr<AudioRendererChangeInfo> audioRenderer_ = std::make_shared<AudioRendererChangeInfo>();
    audioRenderer_->rendererInfo.pipeType = AudioPipeType::PIPE_TYPE_OFFLOAD;
    audioRendererChangeInfos.push_back(audioRenderer_);

    result = audioConcurrency_->ActivateAudioConcurrency(AudioPipeType::PIPE_TYPE_OFFLOAD, audioRendererChangeInfos,
        audioCapturerChangeInfos);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test AudioConcurrencyService.
* @tc.number: AudioEffectConfigParser_004.
* @tc.desc  : Test ActivateAudioConcurrency interfaces.
*/
HWTEST(AudioConcurrencyServiceUnitTest, AudioConcurrencyServiceUnitTest_004, TestSize.Level1)
{
    std::vector<std::shared_ptr<AudioRendererChangeInfo>> audioRendererChangeInfos;
    std::vector<std::shared_ptr<AudioCapturerChangeInfo>> audioCapturerChangeInfos;
    auto audioConcurrency_ = std::make_shared<AudioConcurrencyService>();

    std::pair<AudioPipeType, AudioPipeType> dsc = std::make_pair(AudioPipeType::PIPE_TYPE_UNKNOWN,
        AudioPipeType::PIPE_TYPE_UNKNOWN);
    audioConcurrency_->concurrencyCfgMap_.insert({dsc, ConcurrencyAction::PLAY_BOTH});

    std::shared_ptr<AudioRendererChangeInfo> audioRenderer_ = std::make_shared<AudioRendererChangeInfo>();
    audioRenderer_->rendererInfo.pipeType = AudioPipeType::PIPE_TYPE_MULTICHANNEL;
    audioRendererChangeInfos.push_back(audioRenderer_);

    int32_t result = audioConcurrency_->ActivateAudioConcurrency(AudioPipeType::PIPE_TYPE_OFFLOAD,
    audioRendererChangeInfos, audioCapturerChangeInfos);
    EXPECT_EQ(result, SUCCESS);

    audioRenderer_->rendererInfo.pipeType = AudioPipeType::PIPE_TYPE_OFFLOAD;
    result = audioConcurrency_->ActivateAudioConcurrency(AudioPipeType::PIPE_TYPE_MULTICHANNEL,
    audioRendererChangeInfos, audioCapturerChangeInfos);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test AudioConcurrencyService.
* @tc.number: AudioEffectConfigParser_005.
* @tc.desc  : Test ActivateAudioConcurrency interfaces.
*/
HWTEST(AudioConcurrencyServiceUnitTest, AudioConcurrencyServiceUnitTest_005, TestSize.Level1)
{
    std::vector<std::shared_ptr<AudioRendererChangeInfo>> audioRendererChangeInfos;
    std::vector<std::shared_ptr<AudioCapturerChangeInfo>> audioCapturerChangeInfos;
    auto audioConcurrency_ = std::make_shared<AudioConcurrencyService>();

    std::pair<AudioPipeType, AudioPipeType> dsc = std::make_pair(AudioPipeType::PIPE_TYPE_UNKNOWN,
        AudioPipeType::PIPE_TYPE_UNKNOWN);
    audioConcurrency_->concurrencyCfgMap_.insert({dsc, ConcurrencyAction::PLAY_BOTH});

    std::shared_ptr<AudioRendererChangeInfo> audioRenderer_ = std::make_shared<AudioRendererChangeInfo>();
    audioRenderer_->rendererInfo.pipeType = AudioPipeType::PIPE_TYPE_OFFLOAD;
    audioRendererChangeInfos.push_back(audioRenderer_);
    int32_t result = audioConcurrency_->ActivateAudioConcurrency(AudioPipeType::PIPE_TYPE_MULTICHANNEL,
    audioRendererChangeInfos, audioCapturerChangeInfos);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test AudioConcurrencyService.
* @tc.number: AudioEffectConfigParser_006.
* @tc.desc  : Test ActivateAudioConcurrency interfaces.
*/
HWTEST(AudioConcurrencyServiceUnitTest, AudioConcurrencyServiceUnitTest_006, TestSize.Level1)
{
    std::vector<std::shared_ptr<AudioRendererChangeInfo>> audioRendererChangeInfos;
    std::vector<std::shared_ptr<AudioCapturerChangeInfo>> audioCapturerChangeInfos;
    auto audioConcurrency_ = std::make_shared<AudioConcurrencyService>();

    std::pair<AudioPipeType, AudioPipeType> dsc = std::make_pair(AudioPipeType::PIPE_TYPE_UNKNOWN,
        AudioPipeType::PIPE_TYPE_UNKNOWN);
    audioConcurrency_->concurrencyCfgMap_.insert({dsc, ConcurrencyAction::PLAY_BOTH});

    std::shared_ptr<AudioRendererChangeInfo> audioRenderer_ = std::make_shared<AudioRendererChangeInfo>();
    audioRenderer_->rendererInfo.pipeType = AudioPipeType::PIPE_TYPE_MULTICHANNEL;
    audioRendererChangeInfos.push_back(audioRenderer_);

    int32_t result = audioConcurrency_->ActivateAudioConcurrency(AudioPipeType::PIPE_TYPE_MULTICHANNEL,
    audioRendererChangeInfos, audioCapturerChangeInfos);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test AudioConcurrencyService.
* @tc.number: AudioEffectConfigParser_007.
* @tc.desc  : Test ActivateAudioConcurrency interfaces.
*/
HWTEST(AudioConcurrencyServiceUnitTest, AudioConcurrencyServiceUnitTest_007, TestSize.Level1)
{
    std::vector<std::shared_ptr<AudioRendererChangeInfo>> audioRendererChangeInfos;
    std::vector<std::shared_ptr<AudioCapturerChangeInfo>> audioCapturerChangeInfos;
    auto audioConcurrency_ = std::make_shared<AudioConcurrencyService>();

    std::shared_ptr<AudioRendererChangeInfo> audioRenderer_ = std::make_shared<AudioRendererChangeInfo>();
    audioRenderer_->rendererInfo.pipeType = AudioPipeType::PIPE_TYPE_OFFLOAD;
    audioRendererChangeInfos.push_back(audioRenderer_);

    std::pair<AudioPipeType, AudioPipeType> dsc = std::make_pair(audioRenderer_->rendererInfo.pipeType,
        AudioPipeType::PIPE_TYPE_MULTICHANNEL);
    audioConcurrency_->concurrencyCfgMap_.insert({dsc, ConcurrencyAction::CONCEDE_EXISTING});

    audioConcurrency_->handler_ = std::make_shared<AudioPolicyServerHandler>();
    int32_t result = audioConcurrency_->ActivateAudioConcurrency(AudioPipeType::PIPE_TYPE_MULTICHANNEL,
    audioRendererChangeInfos, audioCapturerChangeInfos);
    EXPECT_EQ(result, SUCCESS);

    audioConcurrency_->handler_ = nullptr;
    result = audioConcurrency_->ActivateAudioConcurrency(AudioPipeType::PIPE_TYPE_MULTICHANNEL,
    audioRendererChangeInfos, audioCapturerChangeInfos);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test AudioConcurrencyService.
* @tc.number: AudioEffectConfigParser_008.
* @tc.desc  : Test ActivateAudioConcurrency interfaces.
*/
HWTEST(AudioConcurrencyServiceUnitTest, AudioConcurrencyServiceUnitTest_008, TestSize.Level1)
{
    std::vector<std::shared_ptr<AudioRendererChangeInfo>> audioRendererChangeInfos;
    std::vector<std::shared_ptr<AudioCapturerChangeInfo>> audioCapturerChangeInfos;
    auto audioConcurrency_ = std::make_shared<AudioConcurrencyService>();

    std::shared_ptr<AudioRendererChangeInfo> audioRenderer_ = std::make_shared<AudioRendererChangeInfo>();
    audioRenderer_->rendererInfo.pipeType = AudioPipeType::PIPE_TYPE_OFFLOAD;
    audioRendererChangeInfos.push_back(audioRenderer_);

    std::pair<AudioPipeType, AudioPipeType> dsc = std::make_pair(audioRenderer_->rendererInfo.pipeType,
        AudioPipeType::PIPE_TYPE_MULTICHANNEL);
    audioConcurrency_->concurrencyCfgMap_.insert({dsc, ConcurrencyAction::PLAY_BOTH});

    audioConcurrency_->handler_ = std::make_shared<AudioPolicyServerHandler>();
    int32_t result = audioConcurrency_->ActivateAudioConcurrency(AudioPipeType::PIPE_TYPE_MULTICHANNEL,
    audioRendererChangeInfos, audioCapturerChangeInfos);
    EXPECT_EQ(result, SUCCESS);

    audioConcurrency_->handler_ = nullptr;
    result = audioConcurrency_->ActivateAudioConcurrency(AudioPipeType::PIPE_TYPE_MULTICHANNEL,
    audioRendererChangeInfos, audioCapturerChangeInfos);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test AudioConcurrencyService.
* @tc.number: AudioEffectConfigParser_009.
* @tc.desc  : Test ActivateAudioConcurrency interfaces.
*/
HWTEST(AudioConcurrencyServiceUnitTest, AudioConcurrencyServiceUnitTest_009, TestSize.Level1)
{
    std::vector<std::shared_ptr<AudioRendererChangeInfo>> audioRendererChangeInfos;
    std::vector<std::shared_ptr<AudioCapturerChangeInfo>> audioCapturerChangeInfos;
    auto audioConcurrency_ = std::make_shared<AudioConcurrencyService>();

    std::shared_ptr<AudioCapturerChangeInfo> audioCapturer_ = std::make_shared<AudioCapturerChangeInfo>();
    audioCapturer_->capturerInfo.pipeType = AudioPipeType::PIPE_TYPE_UNKNOWN;
    audioCapturerChangeInfos.push_back(audioCapturer_);

    std::pair<AudioPipeType, AudioPipeType> dsc = std::make_pair(audioCapturer_->capturerInfo.pipeType,
        AudioPipeType::PIPE_TYPE_MULTICHANNEL);
    audioConcurrency_->concurrencyCfgMap_.insert({dsc, ConcurrencyAction::CONCEDE_EXISTING});

    audioConcurrency_->handler_ = std::make_shared<AudioPolicyServerHandler>();
    int32_t result = audioConcurrency_->ActivateAudioConcurrency(AudioPipeType::PIPE_TYPE_MULTICHANNEL,
        audioRendererChangeInfos, audioCapturerChangeInfos);
    EXPECT_EQ(result, SUCCESS);

    audioConcurrency_->handler_ = nullptr;
    result = audioConcurrency_->ActivateAudioConcurrency(AudioPipeType::PIPE_TYPE_MULTICHANNEL,
        audioRendererChangeInfos, audioCapturerChangeInfos);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test AudioConcurrencyService.
* @tc.number: AudioEffectConfigParser_010.
* @tc.desc  : Test ActivateAudioConcurrency interfaces.
*/
HWTEST(AudioConcurrencyServiceUnitTest, AudioConcurrencyServiceUnitTest_010, TestSize.Level1)
{
    std::vector<std::shared_ptr<AudioRendererChangeInfo>> audioRendererChangeInfos;
    std::vector<std::shared_ptr<AudioCapturerChangeInfo>> audioCapturerChangeInfos;
    auto audioConcurrency_ = std::make_shared<AudioConcurrencyService>();

    std::shared_ptr<AudioCapturerChangeInfo> audioCapturer_ = std::make_shared<AudioCapturerChangeInfo>();
    audioCapturer_->capturerInfo.pipeType = AudioPipeType::PIPE_TYPE_UNKNOWN;
    audioCapturerChangeInfos.push_back(audioCapturer_);

    std::pair<AudioPipeType, AudioPipeType> dsc = std::make_pair(audioCapturer_->capturerInfo.pipeType,
        AudioPipeType::PIPE_TYPE_MULTICHANNEL);
    audioConcurrency_->concurrencyCfgMap_.insert({dsc, ConcurrencyAction::PLAY_BOTH});

    audioConcurrency_->handler_ = std::make_shared<AudioPolicyServerHandler>();
    int32_t result = audioConcurrency_->ActivateAudioConcurrency(AudioPipeType::PIPE_TYPE_MULTICHANNEL,
        audioRendererChangeInfos, audioCapturerChangeInfos);
    EXPECT_EQ(result, SUCCESS);

    audioConcurrency_->handler_ = nullptr;
    result = audioConcurrency_->ActivateAudioConcurrency(AudioPipeType::PIPE_TYPE_MULTICHANNEL,
        audioRendererChangeInfos, audioCapturerChangeInfos);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test AudioConcurrencyService.
* @tc.number: AudioEffectConfigParser_011.
* @tc.desc  : Test ActivateAudioConcurrency interfaces.
*/
HWTEST(AudioConcurrencyServiceUnitTest, AudioConcurrencyServiceUnitTest_011, TestSize.Level1)
{
    uint32_t sessionID = 0;
    std::shared_ptr<AudioConcurrencyService> service;
    sptr<IRemoteObject> object = new RemoteObjectTestStub();
    auto inAudioConcurrency_ = std::make_shared<AudioConcurrencyService::AudioConcurrencyDeathRecipient>(service,
        sessionID);
    inAudioConcurrency_->OnRemoteDied(object);
    EXPECT_NE(inAudioConcurrency_, nullptr);

    auto audioConcurrencyService_ = std::make_shared<AudioConcurrencyService>();
    service = audioConcurrencyService_;
    inAudioConcurrency_->OnRemoteDied(object);
    EXPECT_NE(inAudioConcurrency_, nullptr);
}
} // namespace AudioStandard
} // namespace OHOS