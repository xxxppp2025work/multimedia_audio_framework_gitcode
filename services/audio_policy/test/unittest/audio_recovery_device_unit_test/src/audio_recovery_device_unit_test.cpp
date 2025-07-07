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

#include "audio_recovery_device_unit_test.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

void AudioRecoveryDeviceUnitTest::SetUpTestCase(void) {}
void AudioRecoveryDeviceUnitTest::TearDownTestCase(void) {}
void AudioRecoveryDeviceUnitTest::SetUp(void) {}
void AudioRecoveryDeviceUnitTest::TearDown(void) {}

/**
* @tc.name  : Test AudioRecoveryDevice.
* @tc.number: AudioRecoveryDeviceUnitTest_001.
* @tc.desc  : Test RecoveryPreferredDevices.
*/
HWTEST_F(AudioRecoveryDeviceUnitTest, AudioRecoveryDeviceUnitTest_001, TestSize.Level1)
{
    auto audioRecoveryDevice = std::make_shared<AudioRecoveryDevice>();
    audioRecoveryDevice->RecoveryPreferredDevices();
    EXPECT_NE(audioRecoveryDevice, nullptr);
}

/**
* @tc.name  : Test AudioRecoveryDevice.
* @tc.number: AudioRecoveryDeviceUnitTest_002.
* @tc.desc  : Test RecoverExcludedOutputDevices.
*/
HWTEST_F(AudioRecoveryDeviceUnitTest, AudioRecoveryDeviceUnitTest_002, TestSize.Level1)
{
    auto audioRecoveryDevice = std::make_shared<AudioRecoveryDevice>();
    audioRecoveryDevice->RecoverExcludedOutputDevices();
    EXPECT_NE(audioRecoveryDevice, nullptr);
}

/**
* @tc.name  : Test AudioRecoveryDevice.
* @tc.number: AudioRecoveryDeviceUnitTest_003.
* @tc.desc  : Test HandleExcludedOutputDevicesRecovery.
*/
HWTEST_F(AudioRecoveryDeviceUnitTest, AudioRecoveryDeviceUnitTest_003, TestSize.Level1)
{
    auto audioRecoveryDevice = std::make_shared<AudioRecoveryDevice>();
    std::vector<std::shared_ptr<Media::MediaMonitor::MonitorDeviceInfo>> excludedDevices;
    auto mediaMonitor = std::make_shared<Media::MediaMonitor::MonitorDeviceInfo>();
    excludedDevices.push_back(mediaMonitor);
    auto result = audioRecoveryDevice->HandleExcludedOutputDevicesRecovery(AudioDeviceUsage::ALL_CALL_DEVICES,
        excludedDevices);
    EXPECT_EQ(result, ERROR);
}

/**
* @tc.name  : Test SelectOutputDevice.
* @tc.number: SelectOutputDevice_001.
* @tc.desc  : Test SelectOutputDevice.
*/
HWTEST_F(AudioRecoveryDeviceUnitTest, SelectOutputDevice_001, TestSize.Level1)
{
    auto audioRendererFilter = std::make_shared<AudioRecoveryDevice>();
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> selectedDesc;
    std::shared_ptr<AudioDeviceDescriptor> deviceDesc = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc->deviceRole_ == DeviceRole::OUTPUT_DEVICE, ERR_INVALID_OPERATION
    outputDevices.push_back(deviceDesc);
    audioRendererFilter->uid = -1;
    audioRendererFilter->rendererInfo.rendererFlags = AUDIO_FLAG_NORMAL;
    
    auto result = audioRecoveryDevice->SelectOutputDevice(audioRendererFilter,
        selectedDesc);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test ConnectVirtualDevice.
* @tc.number: ConnectVirtualDevice_001.
* @tc.desc  : Test ConnectVirtualDevice.
*/
HWTEST_F(AudioRecoveryDeviceUnitTest, ConnectVirtualDevice_001, TestSize.Level1)
{
    auto audioRendererFilter = std::make_shared<AudioRecoveryDevice>();
    std::shared_ptr<AudioDeviceDescriptor> selectedDesc = nullptr;
    
    auto result = audioRecoveryDevice->ConnectVirtualDevice(audioRendererFilter);
    EXPECT_EQ(result, ERROR_INVALID_PARAM);
}

/**
* @tc.name  : Test ConnectVirtualDevice.
* @tc.number: ConnectVirtualDevice_002.
* @tc.desc  : Test ConnectVirtualDevice.
*/
HWTEST_F(AudioRecoveryDeviceUnitTest, ConnectVirtualDevice_001, TestSize.Level1)
{
    auto audioRendererFilter = std::make_shared<AudioRecoveryDevice>();
    std::shared_ptr<AudioDeviceDescriptor> selectedDesc = nullptr;
    
    auto result = audioRecoveryDevice->ConnectVirtualDevice(audioRendererFilter);
    EXPECT_EQ(result, ERROR_INVALID_PARAM);
}
} // namespace AudioStandard
} // namespace OHOS