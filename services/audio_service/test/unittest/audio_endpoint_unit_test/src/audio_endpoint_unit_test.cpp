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

#include "audio_endpoint_unit_test.h"
#include "accesstoken_kit.h"
#include "audio_device_info.h"
#include "audio_endpoint.h"
#include "audio_errors.h"
#include "audio_info.h"
#include "audio_process_config.h"
#include "audio_server.h"
#include "audio_service.h"
#include "audio_stream_info.h"
#include "audio_utils.h"
#include "policy_handler.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {
void AudioEndpointUnitTest::SetUpTestCase(void)
{
    // input testsuit setup step，setup invoked before all testcases
}

void AudioEndpointUnitTest::TearDownTestCase(void)
{
    // input testsuit teardown step，teardown invoked after all testcases
}

void AudioEndpointUnitTest::SetUp(void)
{
    // input testcase setup step，setup invoked before each testcases
}

void AudioEndpointUnitTest::TearDown(void)
{
    // input testcase teardown step，teardown invoked after each testcases
}

/**
 * @tc.name  : Test CreateEndpoint API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointCreateEndpoint_001
 * @tc.desc  : Test CreateEndpoint interface, networkId is LOCAL_NETWORK_ID.
 */
HWTEST_F(AudioEndpointUnitTest, AudioEndpointCreateEndpoint_001, TestSize.Level1)
{
    AudioProcessConfig config = {};
    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    deviceInfo.audioStreamInfo_.samplingRate.insert(SAMPLE_RATE_48000);
    deviceInfo.audioStreamInfo_.channels.insert(STEREO);
    deviceInfo.networkId_ = LOCAL_NETWORK_ID;
    std::shared_ptr<AudioEndpoint> audioEndpoint =
        AudioEndpoint::CreateEndpoint(AudioEndpoint::TYPE_MMAP, 123, config, deviceInfo);
    EXPECT_NE(nullptr, audioEndpoint);
}

/**
 * @tc.name  : Test CreateEndpoint API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointCreateEndpoint_002
 * @tc.desc  : Test CreateEndpoint interface, networkId is LOCAL_NETWORK_ID, deviceRole is INPUT_DEVICE.
 */
HWTEST_F(AudioEndpointUnitTest, AudioEndpointCreateEndpoint_002, TestSize.Level1)
{
    AudioProcessConfig config = {};
    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    deviceInfo.deviceRole_ = DeviceRole::INPUT_DEVICE;
    deviceInfo.audioStreamInfo_.samplingRate.insert(SAMPLE_RATE_48000);
    deviceInfo.audioStreamInfo_.channels.insert(STEREO);
    deviceInfo.networkId_ = LOCAL_NETWORK_ID;
    std::shared_ptr<AudioEndpoint> audioEndpoint =
        AudioEndpoint::CreateEndpoint(AudioEndpoint::TYPE_MMAP, 123, config, deviceInfo);
    EXPECT_NE(nullptr, audioEndpoint);
}

/*
 * @tc.name  : Test CreateEndpoint API
 * @tc.type  : FUNC
 * @tc.number: EnableCreateEndpoint_003
 * @tc.desc  : Test CreateEndpoint interface
 */
HWTEST_F(AudioEndpointUnitTest, EnableCreateEndpoint_003, TestSize.Level1)
{
    AudioProcessConfig config = {};
    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    deviceInfo.deviceRole_ = DeviceRole::OUTPUT_DEVICE;
    deviceInfo.audioStreamInfo_.samplingRate.insert(SAMPLE_RATE_48000);
    deviceInfo.audioStreamInfo_.channels.insert(STEREO);
    deviceInfo.networkId_ = LOCAL_NETWORK_ID;
    std::shared_ptr<AudioEndpoint> audioEndpoint =
        AudioEndpoint::CreateEndpoint(AudioEndpoint::TYPE_INDEPENDENT, 123, config, deviceInfo);
    EXPECT_NE(nullptr, audioEndpoint);

    deviceInfo.networkId_ = REMOTE_NETWORK_ID;
    audioEndpoint = AudioEndpoint::CreateEndpoint(AudioEndpoint::TYPE_INDEPENDENT, 123, config, deviceInfo);
    EXPECT_EQ(nullptr, audioEndpoint);

    deviceInfo.deviceRole_ = DeviceRole::INPUT_DEVICE;
    deviceInfo.networkId_ = LOCAL_NETWORK_ID;
    config.audioMode = AUDIO_MODE_RECORD;
    audioEndpoint = AudioEndpoint::CreateEndpoint(AudioEndpoint::TYPE_INDEPENDENT, 123, config, deviceInfo);
    EXPECT_NE(nullptr, audioEndpoint);
}

/**
 * @tc.name  : Test EnableFastInnerCap API
 * @tc.type  : FUNC
 * @tc.number: AudioEnableFastInnerCap_001
 * @tc.desc  : Test EnableFastInnerCap interface.
 */
HWTEST_F(AudioEndpointUnitTest, AudioEnableFastInnerCap_001, TestSize.Level1)
{
    AudioProcessConfig config = {};
    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    deviceInfo.deviceRole_ = DeviceRole::OUTPUT_DEVICE;
    deviceInfo.audioStreamInfo_.samplingRate.insert(SAMPLE_RATE_48000);
    deviceInfo.audioStreamInfo_.channels.insert(STEREO);
    deviceInfo.networkId_ = LOCAL_NETWORK_ID;
    std::shared_ptr<AudioEndpoint> audioEndpoint =
        AudioEndpoint::CreateEndpoint(AudioEndpoint::TYPE_MMAP, 123, config, deviceInfo);
    EXPECT_NE(nullptr, audioEndpoint);

    int32_t ret = audioEndpoint->EnableFastInnerCap();
    EXPECT_NE(SUCCESS, ret);

    audioEndpoint->Release();
    audioEndpoint->Release();
}
} // namespace AudioStandard
} // namespace OHOS
