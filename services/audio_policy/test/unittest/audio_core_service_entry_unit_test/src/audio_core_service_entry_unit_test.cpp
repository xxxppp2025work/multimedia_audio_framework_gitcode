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

#include "audio_core_service_entry_unit_test.h"
using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

static constexpr int32_t NUM5 = 5;
/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_001
 * @tc.desc  : Test AudioCoreService::EventEntry::UpdateSessionOperation()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_001, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    uint32_t sessionId = 0;
    SessionOperation operation = SESSION_OPERATION_START;

    auto ret = eventEntry->UpdateSessionOperation(sessionId, operation);
    EXPECT_NE(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_002
 * @tc.desc  : Test AudioCoreService::EventEntry::UpdateSessionOperation()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_002, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    audioCoreService->pipeManager_ = std::make_shared<AudioPipeManager>();
    EXPECT_NE(audioCoreService->pipeManager_, nullptr);
    std::shared_ptr<AudioPipeInfo> audioPipeInfo = std::make_shared<AudioPipeInfo>();
    std::shared_ptr<AudioStreamDescriptor> audioStreamDescriptor = std::make_shared<AudioStreamDescriptor>();
    audioStreamDescriptor->sessionId_ = SESSION_OPERATION_PAUSE;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor = std::make_shared<AudioDeviceDescriptor>();
    audioStreamDescriptor->newDeviceDescs_.push_back(audioDeviceDescriptor);
    audioPipeInfo->streamDescriptors_.push_back(audioStreamDescriptor);
    audioCoreService->pipeManager_->curPipeList_.push_back(audioPipeInfo);
    EXPECT_NE(&(audioCoreService->audioActiveDevice_), nullptr);

    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    uint32_t sessionId = SESSION_OPERATION_PAUSE;
    SessionOperation operation = SESSION_OPERATION_PAUSE;

    auto ret = eventEntry->UpdateSessionOperation(sessionId, operation);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_003
 * @tc.desc  : Test AudioCoreService::EventEntry::UpdateSessionOperation()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_003, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    audioCoreService->pipeManager_ = std::make_shared<AudioPipeManager>();
    EXPECT_NE(audioCoreService->pipeManager_, nullptr);
    std::shared_ptr<AudioPipeInfo> audioPipeInfo = std::make_shared<AudioPipeInfo>();
    std::shared_ptr<AudioStreamDescriptor> audioStreamDescriptor = std::make_shared<AudioStreamDescriptor>();
    audioStreamDescriptor->sessionId_ = SESSION_OPERATION_STOP;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor = std::make_shared<AudioDeviceDescriptor>();
    audioStreamDescriptor->newDeviceDescs_.push_back(audioDeviceDescriptor);
    audioPipeInfo->streamDescriptors_.push_back(audioStreamDescriptor);
    audioCoreService->pipeManager_->curPipeList_.push_back(audioPipeInfo);
    EXPECT_NE(&(audioCoreService->audioActiveDevice_), nullptr);

    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    uint32_t sessionId = SESSION_OPERATION_STOP;
    SessionOperation operation = SESSION_OPERATION_STOP;

    auto ret = eventEntry->UpdateSessionOperation(sessionId, operation);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_004
 * @tc.desc  : Test AudioCoreService::EventEntry::UpdateSessionOperation()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_004, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    audioCoreService->pipeManager_ = std::make_shared<AudioPipeManager>();
    EXPECT_NE(audioCoreService->pipeManager_, nullptr);
    std::shared_ptr<AudioPipeInfo> audioPipeInfo = std::make_shared<AudioPipeInfo>();
    std::shared_ptr<AudioStreamDescriptor> audioStreamDescriptor = std::make_shared<AudioStreamDescriptor>();
    audioStreamDescriptor->sessionId_ = SESSION_OPERATION_RELEASE;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor = std::make_shared<AudioDeviceDescriptor>();
    audioStreamDescriptor->newDeviceDescs_.push_back(audioDeviceDescriptor);
    audioPipeInfo->streamDescriptors_.push_back(audioStreamDescriptor);
    audioCoreService->pipeManager_->curPipeList_.push_back(audioPipeInfo);
    EXPECT_NE(&(audioCoreService->audioActiveDevice_), nullptr);

    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    uint32_t sessionId = SESSION_OPERATION_RELEASE;
    SessionOperation operation = SESSION_OPERATION_RELEASE;

    auto ret = eventEntry->UpdateSessionOperation(sessionId, operation);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_005
 * @tc.desc  : Test AudioCoreService::EventEntry::UpdateSessionOperation()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_005, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    uint32_t sessionId = 0;
    SessionOperation operation = static_cast<SessionOperation>(NUM5);

    auto ret = eventEntry->UpdateSessionOperation(sessionId, operation);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_006
 * @tc.desc  : Test AudioCoreService::EventEntry::OnServiceConnected()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_006, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    AudioServiceIndex serviceIndex = HDI_SERVICE_INDEX;

    eventEntry->OnServiceConnected(serviceIndex);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_007
 * @tc.desc  : Test AudioCoreService::EventEntry::OnServiceDisconnected()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_007, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    AudioServiceIndex serviceIndex = HDI_SERVICE_INDEX;

    eventEntry->OnServiceDisconnected(serviceIndex);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_008
 * @tc.desc  : Test AudioCoreService::EventEntry::createRendererClient()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_008, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    std::shared_ptr<AudioStreamDescriptor> streamDesc = std::make_shared<AudioStreamDescriptor>();
    uint32_t audioFlag = 0;
    uint32_t sessionId = 0;

    auto ret = eventEntry->CreateRendererClient(streamDesc, audioFlag, sessionId);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_009
 * @tc.desc  : Test AudioCoreService::EventEntry::createCapturerClient()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_009, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    std::shared_ptr<AudioStreamDescriptor> streamDesc = std::make_shared<AudioStreamDescriptor>();
    uint32_t audioFlag = 0;
    uint32_t sessionId = 0;

    auto ret = eventEntry->CreateCapturerClient(streamDesc, audioFlag, sessionId);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_010
 * @tc.desc  : Test AudioCoreService::EventEntry::SetDefaultOutputDevice()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_010, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    DeviceType deviceType = DEVICE_TYPE_SPEAKER;
    uint32_t sessionID = 0;
    StreamUsage streamUsage = STREAM_USAGE_MEDIA;
    bool isRunning = true;

    auto ret = eventEntry->SetDefaultOutputDevice(deviceType, sessionID, streamUsage, isRunning);
    EXPECT_NE(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_011
 * @tc.desc  : Test AudioCoreService::EventEntry::GetAdapterNameBySessionId()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_011, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    uint32_t sessionId = 0;

    auto ret = eventEntry->GetAdapterNameBySessionId(sessionId);
    EXPECT_EQ(ret, "");
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_012
 * @tc.desc  : Test AudioCoreService::EventEntry::GetProcessDeviceInfoBySessionId()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_012, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    uint32_t sessionId = 0;
    AudioDeviceDescriptor deviceInfo;

    auto ret = eventEntry->GetProcessDeviceInfoBySessionId(sessionId, deviceInfo);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_013
 * @tc.desc  : Test AudioCoreService::EventEntry::GenerateSessionId()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_013, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    auto ret = eventEntry->GenerateSessionId();
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_014
 * @tc.desc  : Test AudioCoreService::EventEntry::OnDeviceInfoUpdated()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_014, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    AudioDeviceDescriptor desc;
    DeviceInfoUpdateCommand command = CATEGORY_UPDATE;

    eventEntry->OnDeviceInfoUpdated(desc, command);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_015
 * @tc.desc  : Test AudioCoreService::EventEntry::SetAudioScene()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_015, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    AudioScene audioScene = AUDIO_SCENE_RINGING;

    auto ret = eventEntry->SetAudioScene(audioScene);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_016
 * @tc.desc  : Test AudioCoreService::EventEntry::OnDeviceStatusUpdated()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_016, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    AudioDeviceDescriptor desc;
    bool isConnected = true;

    eventEntry->OnDeviceStatusUpdated(desc, isConnected);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_017
 * @tc.desc  : Test AudioCoreService::EventEntry::OnDeviceStatusUpdated()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_017, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    DStatusInfo statusInfo;
    bool isStop = true;

    eventEntry->OnDeviceStatusUpdated(statusInfo, isStop);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_018
 * @tc.desc  : Test AudioCoreService::EventEntry::OnMicrophoneBlockedUpdate()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_018, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    DeviceType devType = DEVICE_TYPE_SPEAKER;
    DeviceBlockStatus status = DEVICE_BLOCKED;

    eventEntry->OnMicrophoneBlockedUpdate(devType, status);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_019
 * @tc.desc  : Test AudioCoreService::EventEntry::OnPnpDeviceStatusUpdated()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_019, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    AudioDeviceDescriptor desc;
    bool isConnected = true;

    eventEntry->OnPnpDeviceStatusUpdated(desc, isConnected);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_020
 * @tc.desc  : Test AudioCoreService::EventEntry::OnServiceConnected()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_020, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    AudioServiceIndex serviceIndex = HDI_SERVICE_INDEX;

    eventEntry->OnServiceConnected(serviceIndex);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_021
 * @tc.desc  : Test AudioCoreService::EventEntry::OnForcedDeviceSelected()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_021, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    DeviceType devType = DEVICE_TYPE_SPEAKER;
    std::string macAddress = "macAddress";

    eventEntry->OnForcedDeviceSelected(devType, macAddress);
}

/**
 * @tc.name  : Test AudioCoreService.
 * @tc.number: AudioCoreService_022
 * @tc.desc  : Test AudioCoreService::EventEntry::SetDefaultOutputDevice()
 */
HWTEST(AudioCoreServiceEntryTest, AudioCoreService_022, TestSize.Level1)
{
    auto audioCoreService = std::make_shared<AudioCoreService>();
    EXPECT_NE(audioCoreService, nullptr);
    audioCoreService->pipeManager_ = std::make_shared<AudioPipeManager>();
    EXPECT_NE(audioCoreService->pipeManager_, nullptr);
    std::shared_ptr<AudioPipeInfo> audioPipeInfo = std::make_shared<AudioPipeInfo>();
    std::shared_ptr<AudioStreamDescriptor> audioStreamDescriptor = std::make_shared<AudioStreamDescriptor>();
    audioStreamDescriptor->sessionId_ = SESSION_OPERATION_PAUSE;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor = std::make_shared<AudioDeviceDescriptor>();
    audioStreamDescriptor->newDeviceDescs_.push_back(audioDeviceDescriptor);
    audioPipeInfo->streamDescriptors_.push_back(audioStreamDescriptor);
    audioCoreService->pipeManager_->curPipeList_.push_back(audioPipeInfo);
    EXPECT_NE(&(audioCoreService->audioActiveDevice_), nullptr);

    auto eventEntry = std::make_shared<AudioCoreService::EventEntry>(audioCoreService);
    EXPECT_NE(eventEntry, nullptr);

    DeviceType deviceType = DEVICE_TYPE_SPEAKER;
    uint32_t sessionID = SESSION_OPERATION_PAUSE;
    StreamUsage streamUsage = STREAM_USAGE_MEDIA;
    bool isRunning = true;

    auto ret = eventEntry->SetDefaultOutputDevice(deviceType, sessionID, streamUsage, isRunning);
    EXPECT_NE(ret, SUCCESS);
}
} // namespace AudioStandard
} // namespace OHOS