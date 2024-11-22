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

#include "default_router.h"
#include "app_select_router.h"
#include "audio_router_center_unit_test.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

const string MEDIA_RENDER_ROUTERS = "MediaRenderRouters";
const string CALL_RENDER_ROUTERS = "CallRenderRouters";
const string RECORD_CAPTURE_ROUTERS = "RecordCaptureRouters";
const string CALL_CAPTURE_ROUTERS = "CallCaptureRouters";
const string RING_RENDER_ROUTERS = "RingRenderRouters";
const string TONE_RENDER_ROUTERS = "ToneRenderRouters";

void AudioRouterCenterUnitTest::SetUpTestCase(void) {}
void AudioRouterCenterUnitTest::TearDownTestCase(void) {}
void AudioRouterCenterUnitTest::SetUp(void) {}
void AudioRouterCenterUnitTest::TearDown(void) {}

/**
* @tc.name  : Test AudioRouterCenter.
* @tc.number: AudioRouterCenterUnitTest_001.
* @tc.desc  : Test SetLowPowerVolumeInternal interface.
*/
HWTEST(AudioRouterCenterUnitTest, AudioRouterCenterUnitTest_001, TestSize.Level1)
{
    int32_t clientUID = 0;
    RouterType routerType = ROUTER_TYPE_NONE;
    StreamUsage streamUsage = STREAM_USAGE_UNKNOWN;
    const RouterType bypassType = ROUTER_TYPE_APP_SELECT;
    auto audioRouterCenter_ = std::make_shared<AudioRouterCenter>();

    std::unique_ptr<RouterBase> routerBase_ =std::make_unique<AppSelectRouter>();
    audioRouterCenter_->mediaRenderRouters_.push_back(std::move(routerBase_));
    audioRouterCenter_->FetchMediaRenderDevice(streamUsage, clientUID, routerType, bypassType);
    EXPECT_NE(audioRouterCenter_, nullptr);
}

/**
* @tc.name  : Test AudioRouterCenter.
* @tc.number: AudioRouterCenterUnitTest_002.
* @tc.desc  : Test FetchCallRenderDevice interface.
*/
HWTEST(AudioRouterCenterUnitTest, AudioRouterCenterUnitTest_002, TestSize.Level1)
{
    int32_t clientUID = 0;
    RouterType routerType = ROUTER_TYPE_NONE;
    StreamUsage streamUsage = STREAM_USAGE_UNKNOWN;
    const RouterType bypassType = ROUTER_TYPE_APP_SELECT;
    auto audioRouterCenter_ = std::make_shared<AudioRouterCenter>();

    std::unique_ptr<RouterBase> routerBase_ =std::make_unique<AppSelectRouter>();
    audioRouterCenter_->mediaRenderRouters_.push_back(std::move(routerBase_));
    audioRouterCenter_->FetchCallRenderDevice(streamUsage, clientUID, routerType, bypassType);
    EXPECT_NE(audioRouterCenter_, nullptr);
}

/**
* @tc.name  : Test AudioRouterCenter.
* @tc.number: AudioRouterCenterUnitTest_003.
* @tc.desc  : Test FetchRingRenderDevices interface.
*/
HWTEST(AudioRouterCenterUnitTest, AudioRouterCenterUnitTest_003, TestSize.Level1)
{
    int32_t clientUID = 0;
    RouterType routerType;
    StreamUsage streamUsage = STREAM_USAGE_UNKNOWN;
    auto audioRouterCenter_ = std::make_shared<AudioRouterCenter>();
    audioRouterCenter_->FetchRingRenderDevices(streamUsage, clientUID, routerType);
    EXPECT_NE(audioRouterCenter_, nullptr);

    streamUsage = STREAM_USAGE_RINGTONE;
    audioRouterCenter_->FetchRingRenderDevices(streamUsage, clientUID, routerType);
    EXPECT_NE(audioRouterCenter_, nullptr);

    streamUsage = STREAM_USAGE_VOICE_RINGTONE;
    audioRouterCenter_->FetchRingRenderDevices(streamUsage, clientUID, routerType);
    EXPECT_NE(audioRouterCenter_, nullptr);
}

/**
* @tc.name  : Test AudioRouterCenter.
* @tc.number: AudioRouterCenterUnitTest_004.
* @tc.desc  : Test FetchRingRenderDevices interface.
*/
HWTEST(AudioRouterCenterUnitTest, AudioRouterCenterUnitTest_004, TestSize.Level1)
{
    int32_t clientUID = 0;
    RouterType routerType;
    StreamUsage streamUsage = STREAM_USAGE_UNKNOWN;
    std::unique_ptr<RouterBase> routerBase1_ =std::make_unique<DefaultRouter>();
    std::unique_ptr<RouterBase> routerBase2_ =std::make_unique<CockpitPhoneRouter>();
    auto audioRouterCenter_ = std::make_shared<AudioRouterCenter>();
    audioRouterCenter_->ringRenderRouters_.push_back(std::move(routerBase1_));
    audioRouterCenter_->ringRenderRouters_.push_back(std::move(routerBase2_));
    audioRouterCenter_->FetchRingRenderDevices(streamUsage, clientUID, routerType);
    EXPECT_NE(audioRouterCenter_, nullptr);
}

/**
* @tc.name  : Test AudioRouterCenter.
* @tc.number: AudioRouterCenterUnitTest_005.
* @tc.desc  : Test HasScoDevice interface.
*/
HWTEST(AudioRouterCenterUnitTest, AudioRouterCenterUnitTest_005, TestSize.Level1)
{
    shared_ptr<AudioDeviceDescriptor> AudioDeviceDescriptor_ = std::make_shared<AudioDeviceDescriptor>();
    AudioDeviceManager::GetAudioDeviceManager().commRenderPrivacyDevices_.push_back(AudioDeviceDescriptor_);
    auto audioRouterCenter_ = std::make_shared<AudioRouterCenter>();
    bool result = audioRouterCenter_->HasScoDevice();
    EXPECT_EQ(result, false);
}

/**
* @tc.name  : Test AudioRouterCenter.
* @tc.number: AudioRouterCenterUnitTest_006.
* @tc.desc  : Test FetchOutputDevices interface.
*/
HWTEST(AudioRouterCenterUnitTest, AudioRouterCenterUnitTest_006, TestSize.Level1)
{
    int32_t clientUID = 0;
    const RouterType bypassType = ROUTER_TYPE_NONE;
    StreamUsage streamUsage = STREAM_USAGE_UNKNOWN;
    auto audioRouterCenter_ = std::make_shared<AudioRouterCenter>();
    audioRouterCenter_->audioDeviceRefinerCb_ = nullptr;
    audioRouterCenter_->renderConfigMap_[streamUsage] = RING_RENDER_ROUTERS;
    audioRouterCenter_->FetchOutputDevices(streamUsage, clientUID, bypassType);
    EXPECT_NE(audioRouterCenter_, nullptr);

    audioRouterCenter_->renderConfigMap_[streamUsage] = CALL_RENDER_ROUTERS;
    audioRouterCenter_->FetchOutputDevices(streamUsage, clientUID, bypassType);
    EXPECT_NE(audioRouterCenter_, nullptr);

    audioRouterCenter_->renderConfigMap_[streamUsage] = "test";
    audioRouterCenter_->FetchOutputDevices(streamUsage, clientUID, bypassType);
    EXPECT_NE(audioRouterCenter_, nullptr);
}

/**
* @tc.name  : Test AudioRouterCenter.
* @tc.number: AudioRouterCenterUnitTest_007.
* @tc.desc  : Test FetchOutputDevices interface.
*/
HWTEST(AudioRouterCenterUnitTest, AudioRouterCenterUnitTest_007, TestSize.Level1)
{
    int32_t clientUID = 0;
    const RouterType bypassType = ROUTER_TYPE_NONE;
    StreamUsage streamUsage = STREAM_USAGE_UNKNOWN;
    auto audioRouterCenter_ = std::make_shared<AudioRouterCenter>();
    audioRouterCenter_->renderConfigMap_[streamUsage] = MEDIA_RENDER_ROUTERS;
    audioRouterCenter_->FetchOutputDevices(streamUsage, clientUID, bypassType);
    EXPECT_NE(audioRouterCenter_, nullptr);

    std::unique_ptr<RouterBase> routerBase_ =std::make_unique<AppSelectRouter>();
    routerBase_->name_ = "package_filter_router";
    audioRouterCenter_->mediaRenderRouters_.push_back(std::move(routerBase_));
    audioRouterCenter_->FetchOutputDevices(streamUsage, clientUID, bypassType);
    EXPECT_NE(audioRouterCenter_, nullptr);
}

/**
* @tc.name  : Test AudioRouterCenter.
* @tc.number: AudioRouterCenterUnitTest_008.
* @tc.desc  : Test DealRingRenderRouters interface.
*/
HWTEST(AudioRouterCenterUnitTest, AudioRouterCenterUnitTest_008, TestSize.Level1)
{
    int32_t clientUID = 0;
    RouterType routerType = ROUTER_TYPE_NONE;
    StreamUsage streamUsage = STREAM_USAGE_UNKNOWN;
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> descs;
    auto audioRouterCenter_ = std::make_shared<AudioRouterCenter>();
    std::unique_ptr<RouterBase> routerBase_ =std::make_unique<AppSelectRouter>();
    routerBase_->name_ = "package_filter_router";
    audioRouterCenter_->mediaRenderRouters_.push_back(std::move(routerBase_));
    audioRouterCenter_->DealRingRenderRouters(descs, streamUsage, clientUID, routerType);
    EXPECT_NE(audioRouterCenter_, nullptr);
}

/**
* @tc.name  : Test AudioRouterCenter.
* @tc.number: AudioRouterCenterUnitTest_009.
* @tc.desc  : Test FetchInputDevice interface.
*/
HWTEST(AudioRouterCenterUnitTest, AudioRouterCenterUnitTest_009, TestSize.Level1)
{
    int32_t clientUID = 0;
    SourceType sourceType = SOURCE_TYPE_MIC;
    auto audioRouterCenter_ = std::make_shared<AudioRouterCenter>();\
    std::unique_ptr<RouterBase> routerBase_ =std::make_unique<AppSelectRouter>();
    audioRouterCenter_->voiceMessageRouters_.push_back(std::move(routerBase_));
    audioRouterCenter_->capturerConfigMap_[sourceType] = "VoiceMessages";
    audioRouterCenter_->FetchInputDevice(sourceType, clientUID);
    EXPECT_NE(audioRouterCenter_, nullptr);
}

/**
* @tc.name  : Test AudioRouterCenter.
* @tc.number: AudioRouterCenterUnitTest_010.
* @tc.desc  : Test HasScoDevice interface.
*/
HWTEST(AudioRouterCenterUnitTest, AudioRouterCenterUnitTest_010, TestSize.Level1)
{
    shared_ptr<AudioDeviceDescriptor> AudioDeviceDescriptor_ = std::make_shared<AudioDeviceDescriptor>();
    AudioDeviceDescriptor_->deviceType_ = DEVICE_TYPE_BLUETOOTH_SCO;
    AudioDeviceManager::GetAudioDeviceManager().commRenderPrivacyDevices_.push_back(AudioDeviceDescriptor_);
    auto audioRouterCenter_ = std::make_shared<AudioRouterCenter>();
    bool result = audioRouterCenter_->HasScoDevice();
    EXPECT_EQ(result, true);
}
} // namespace AudioStandard
} // namespace OHOS