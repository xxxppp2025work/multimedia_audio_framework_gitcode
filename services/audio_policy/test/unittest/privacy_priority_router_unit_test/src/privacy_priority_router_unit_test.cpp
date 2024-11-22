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

#include "privacy_priority_router_unit_test.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

void PrivacyPriorityRouterUnitTest::SetUpTestCase(void) {}
void PrivacyPriorityRouterUnitTest::TearDownTestCase(void) {}
void PrivacyPriorityRouterUnitTest::SetUp(void) {}
void PrivacyPriorityRouterUnitTest::TearDown(void) {}

/**
* @tc.name  : Test PrivacyPriorityRouter.
* @tc.number: PrivacyPriorityRouterUnitTest_001.
* @tc.desc  : Test GetCallRenderDevice interface.
*/
HWTEST(PrivacyPriorityRouterUnitTest, PrivacyPriorityRouterUnitTest_001, TestSize.Level1)
{
    int32_t clientUID = 0;
    auto arivacyPriorityRouter_ = std::make_shared<PrivacyPriorityRouter>();
    auto result = arivacyPriorityRouter_->GetCallRenderDevice(StreamUsage::STREAM_USAGE_UNKNOWN, clientUID);
    EXPECT_NE(result, nullptr);

    result = arivacyPriorityRouter_->GetCallRenderDevice(
        StreamUsage::STREAM_USAGE_VOICE_MODEM_COMMUNICATION, clientUID);
    EXPECT_NE(result, nullptr);
}

/**
* @tc.name  : Test PrivacyPriorityRouter.
* @tc.number: PrivacyPriorityRouterUnitTest_002.
* @tc.desc  : Test GetRingRenderDevices interface.
*/
HWTEST(PrivacyPriorityRouterUnitTest, PrivacyPriorityRouterUnitTest_002, TestSize.Level1)
{
    int32_t clientUID = 0;
    auto arivacyPriorityRouter_ = std::make_shared<PrivacyPriorityRouter>();
    arivacyPriorityRouter_->GetRingRenderDevices(StreamUsage::STREAM_USAGE_VOICE_RINGTONE, clientUID);
    EXPECT_NE(arivacyPriorityRouter_, nullptr);

    arivacyPriorityRouter_->GetRingRenderDevices(StreamUsage::STREAM_USAGE_RINGTONE, clientUID);
    EXPECT_NE(arivacyPriorityRouter_, nullptr);

    arivacyPriorityRouter_->GetRingRenderDevices(StreamUsage::STREAM_USAGE_UNKNOWN, clientUID);
    EXPECT_NE(arivacyPriorityRouter_, nullptr);
}

/**
* @tc.name  : Test PrivacyPriorityRouter.
* @tc.number: PrivacyPriorityRouterUnitTest_003.
* @tc.desc  : Test GetRingRenderDevices interface.
*/
HWTEST(PrivacyPriorityRouterUnitTest, PrivacyPriorityRouterUnitTest_003, TestSize.Level1)
{
    int32_t clientUID = 0;
    shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor_ = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptor_->deviceType_ = DEVICE_TYPE_NONE;
    AudioDeviceManager::GetAudioDeviceManager().commRenderPrivacyDevices_.push_back(audioDeviceDescriptor_);
    auto arivacyPriorityRouter_ = std::make_shared<PrivacyPriorityRouter>();
    auto result = arivacyPriorityRouter_->GetRingRenderDevices(StreamUsage::STREAM_USAGE_VOICE_RINGTONE, clientUID);
    EXPECT_FALSE(result.empty());
}

/**
* @tc.name  : Test PrivacyPriorityRouter.
* @tc.number: PrivacyPriorityRouterUnitTest_004.
* @tc.desc  : Test GetRingRenderDevices interface.
*/
HWTEST(PrivacyPriorityRouterUnitTest, PrivacyPriorityRouterUnitTest_004, TestSize.Level1)
{
    int32_t clientUID = 0;
    shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor_ = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptor_->deviceType_ = DEVICE_TYPE_WIRED_HEADSET;
    AudioDeviceManager::GetAudioDeviceManager().mediaRenderPrivacyDevices_.push_back(audioDeviceDescriptor_);
    auto arivacyPriorityRouter_ = std::make_shared<PrivacyPriorityRouter>();
    auto result = arivacyPriorityRouter_->GetRingRenderDevices(StreamUsage::STREAM_USAGE_ALARM, clientUID);
    EXPECT_FALSE(result.empty());
}

/**
* @tc.name  : Test PrivacyPriorityRouter.
* @tc.number: PrivacyPriorityRouterUnitTest_005.
* @tc.desc  : Test GetRingRenderDevices interface.
*/
HWTEST(PrivacyPriorityRouterUnitTest, PrivacyPriorityRouterUnitTest_005, TestSize.Level1)
{
    int32_t clientUID = 0;
    shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor_ = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptor_->deviceType_ = DEVICE_TYPE_WIRED_HEADPHONES;
    AudioDeviceManager::GetAudioDeviceManager().commRenderPrivacyDevices_.push_back(audioDeviceDescriptor_);
    auto arivacyPriorityRouter_ = std::make_shared<PrivacyPriorityRouter>();
    auto result = arivacyPriorityRouter_->GetRingRenderDevices(StreamUsage::STREAM_USAGE_VOICE_RINGTONE, clientUID);
    EXPECT_FALSE(result.empty());
}

/**
* @tc.name  : Test PrivacyPriorityRouter.
* @tc.number: PrivacyPriorityRouterUnitTest_006.
* @tc.desc  : Test GetRingRenderDevices interface.
*/
HWTEST(PrivacyPriorityRouterUnitTest, PrivacyPriorityRouterUnitTest_006, TestSize.Level1)
{
    int32_t clientUID = 0;
    shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor_ = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptor_->deviceType_ = DEVICE_TYPE_USB_HEADSET;
    AudioDeviceManager::GetAudioDeviceManager().commRenderPrivacyDevices_.push_back(audioDeviceDescriptor_);
    auto arivacyPriorityRouter_ = std::make_shared<PrivacyPriorityRouter>();
    auto result = arivacyPriorityRouter_->GetRingRenderDevices(StreamUsage::STREAM_USAGE_RINGTONE, clientUID);
    EXPECT_FALSE(result.empty());
}

/**
* @tc.name  : Test PrivacyPriorityRouter.
* @tc.number: PrivacyPriorityRouterUnitTest_007.
* @tc.desc  : Test GetRingRenderDevices interface.
*/
HWTEST(PrivacyPriorityRouterUnitTest, PrivacyPriorityRouterUnitTest_007, TestSize.Level1)
{
    int32_t clientUID = 0;
    shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor_ = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptor_->deviceType_ = DEVICE_TYPE_USB_ARM_HEADSET;
    AudioDeviceManager::GetAudioDeviceManager().mediaRenderPrivacyDevices_.push_back(audioDeviceDescriptor_);
    auto arivacyPriorityRouter_ = std::make_shared<PrivacyPriorityRouter>();
    auto result = arivacyPriorityRouter_->GetRingRenderDevices(StreamUsage::STREAM_USAGE_RANGING, clientUID);
    EXPECT_FALSE(result.empty());
}

/**
* @tc.name  : Test PrivacyPriorityRouter.
* @tc.number: PrivacyPriorityRouterUnitTest_008.
* @tc.desc  : Test GetRingRenderDevices interface.
*/
HWTEST(PrivacyPriorityRouterUnitTest, PrivacyPriorityRouterUnitTest_008, TestSize.Level1)
{
    int32_t clientUID = 0;
    shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor_ = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptor_->deviceType_ = DEVICE_TYPE_MAX;
    AudioDeviceManager::GetAudioDeviceManager().commRenderPrivacyDevices_.push_back(audioDeviceDescriptor_);
    auto arivacyPriorityRouter_ = std::make_shared<PrivacyPriorityRouter>();
    auto result = arivacyPriorityRouter_->GetRingRenderDevices(StreamUsage::STREAM_USAGE_RANGING, clientUID);
    EXPECT_FALSE(result.empty());
}

/**
* @tc.name  : Test PrivacyPriorityRouter.
* @tc.number: PrivacyPriorityRouterUnitTest_009.
* @tc.desc  : Test GetRingRenderDevices interface.
*/
HWTEST(PrivacyPriorityRouterUnitTest, PrivacyPriorityRouterUnitTest_009, TestSize.Level1)
{
    int32_t clientUID = 0;
    shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor_ = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptor_->deviceType_ = DEVICE_TYPE_BLUETOOTH_SCO;
    AudioDeviceManager::GetAudioDeviceManager().commRenderPrivacyDevices_.push_back(audioDeviceDescriptor_);
    auto arivacyPriorityRouter_ = std::make_shared<PrivacyPriorityRouter>();
    auto result = arivacyPriorityRouter_->GetRingRenderDevices(StreamUsage::STREAM_USAGE_VOICE_RINGTONE, clientUID);
    EXPECT_FALSE(result.empty());
}

/**
* @tc.name  : Test PrivacyPriorityRouter.
* @tc.number: PrivacyPriorityRouterUnitTest_010.
* @tc.desc  : Test GetRingRenderDevices interface.
*/
HWTEST(PrivacyPriorityRouterUnitTest, PrivacyPriorityRouterUnitTest_010, TestSize.Level1)
{
    int32_t clientUID = 0;
    shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor_ = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptor_->deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    AudioDeviceManager::GetAudioDeviceManager().commRenderPrivacyDevices_.push_back(audioDeviceDescriptor_);
    auto arivacyPriorityRouter_ = std::make_shared<PrivacyPriorityRouter>();
    auto result = arivacyPriorityRouter_->GetRingRenderDevices(StreamUsage::STREAM_USAGE_VOICE_RINGTONE, clientUID);
    EXPECT_FALSE(result.empty());
}

/**
* @tc.name  : Test PrivacyPriorityRouter.
* @tc.number: PrivacyPriorityRouterUnitTest_011.
* @tc.desc  : Test GetRecordCaptureDevice interface.
*/
HWTEST(PrivacyPriorityRouterUnitTest, PrivacyPriorityRouterUnitTest_011, TestSize.Level1)
{
    int32_t clientUID = 0;
    auto arivacyPriorityRouter_ = std::make_shared<PrivacyPriorityRouter>();
    arivacyPriorityRouter_->GetRecordCaptureDevice(SourceType::SOURCE_TYPE_MIC, clientUID);
    EXPECT_NE(arivacyPriorityRouter_, nullptr);

    shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor_ = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptor_->deviceType_ = DEVICE_TYPE_BLUETOOTH_SCO;
    AudioDeviceManager::GetAudioDeviceManager().reconCapturePrivacyDevices_.push_back(audioDeviceDescriptor_);
    arivacyPriorityRouter_->GetRecordCaptureDevice(SourceType::SOURCE_TYPE_VOICE_RECOGNITION, clientUID);
    EXPECT_NE(arivacyPriorityRouter_, nullptr);
}

/**
* @tc.name  : Test PrivacyPriorityRouter.
* @tc.number: PrivacyPriorityRouterUnitTest_012.
* @tc.desc  : Test GetRecordCaptureDevice interface.
*/
HWTEST(PrivacyPriorityRouterUnitTest, PrivacyPriorityRouterUnitTest_012, TestSize.Level1)
{
    int32_t clientUID = 0;
    auto arivacyPriorityRouter_ = std::make_shared<PrivacyPriorityRouter>();
    shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor_ = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptor_->deviceType_ = DEVICE_TYPE_BLUETOOTH_SCO;
    AudioDeviceManager::GetAudioDeviceManager().reconCapturePrivacyDevices_.push_back(audioDeviceDescriptor_);
    arivacyPriorityRouter_->GetRecordCaptureDevice(SourceType::SOURCE_TYPE_VOICE_RECOGNITION, clientUID);
    EXPECT_NE(arivacyPriorityRouter_, nullptr);
}
} // namespace AudioStandard
} // namespace OHOS