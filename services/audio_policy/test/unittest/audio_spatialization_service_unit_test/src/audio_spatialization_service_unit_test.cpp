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

#include "audio_spatialization_service_unit_test.h"
#include <thread>
#include <string>
#include <memory>
#include <vector>
using namespace std;
using namespace std::chrono;
using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

void AudioSpatializationServiceUnitTest::SetUpTestCase(void) {}
void AudioSpatializationServiceUnitTest::TearDownTestCase(void) {}
void AudioSpatializationServiceUnitTest::SetUp(void) {}
void AudioSpatializationServiceUnitTest::TearDown(void) {}

#define PRINT_LINE printf("debug __LINE__:%d\n", __LINE__)

/**
* @tc.name  : Test AudioSocketThread.
* @tc.number: AudioSpatializationService_001
* @tc.desc  : Test UpdateCurrentDevice.
*/
HWTEST_F(AudioSpatializationServiceUnitTest, AudioSpatializationService_001, TestSize.Level1)
{
    AudioSpatializationService service;
     // Test case 1: Update with a new device
    std::string newMacAddress = "00:11:22:33:44:55";
    service.UpdateCurrentDevice(newMacAddress);
    EXPECT_EQ(service.GetCurrentDeviceAddress(), newMacAddress);

    // Test case 2: Update with the same device (no change expected)
    service.UpdateCurrentDevice(newMacAddress);
    EXPECT_EQ(service.GetCurrentDeviceAddress(), newMacAddress);

    // Test case 3: Update with an empty address (should not change the current device)
    std::string originalAddress = service.GetCurrentDeviceAddress();
    service.UpdateCurrentDevice("");
    EXPECT_NE(service.GetCurrentDeviceAddress(), originalAddress);

    // Test case 4: Update with a new device that has spatial capabilities
    std::string spatialDeviceAddress = "AA:BB:CC:DD:EE:FF";
    service.addressToSpatialDeviceStateMap_[service.GetSha256EncryptAddress(spatialDeviceAddress)] = {
        spatialDeviceAddress,  // address
        true,                  // isSpatializationSupported
        false,                 // isHeadTrackingSupported
        AudioSpatialDeviceType::EARPHONE_TYPE_HEADPHONE  // spatialDeviceType
    };
    service.UpdateCurrentDevice(spatialDeviceAddress);
    EXPECT_EQ(service.GetCurrentDeviceAddress(), spatialDeviceAddress);
    EXPECT_EQ(service.currSpatialDeviceType_, AudioSpatialDeviceType::EARPHONE_TYPE_HEADPHONE);

    // Test case 5: Update with a device that doesn't have spatial capabilities
    std::string nonSpatialDeviceAddress = "11:22:33:44:55:66";
    service.UpdateCurrentDevice(nonSpatialDeviceAddress);
    EXPECT_EQ(service.GetCurrentDeviceAddress(), nonSpatialDeviceAddress);
    EXPECT_EQ(service.currSpatialDeviceType_, EARPHONE_TYPE_NONE);
}

/**
* @tc.name  : Test AudioSocketThread.
* @tc.number: AudioSpatializationService_002
* @tc.desc  : Test RemoveOldestDevice.
*/
HWTEST_F(AudioSpatializationServiceUnitTest, AudioSpatializationService_002, TestSize.Level1)
{
    //Initialize the data
    service.addressToDeviceSpatialInfoMap_ = {
        {"device1", "info1|1000"},
        {"device2", "info2|2000"},
        {"device3", "info3|1500"}
    };
    service.addressToSpatialEnabledMap_ = {
        {"device1", AudioSpatializationState{true, true}},
        {"device2", AudioSpatializationState{false, false}},
        {"device3", AudioSpatializationState{true, false}}
    };
    service.addressToSpatialDeviceStateMap_ = {
        {"device1", AudioSpatialDeviceState{"device1", true, true, AudioSpatialDeviceType::EARPHONE_TYPE_HEADPHONE}},
        {"device2", AudioSpatialDeviceState{"device2", false, false, AudioSpatialDeviceType::EARPHONE_TYPE_NONE}},
        {"device3", AudioSpatialDeviceState{"device3", true, false, AudioSpatialDeviceType::EARPHONE_TYPE_HEADPHONE}}
    };
    std::string removedDevice = service.RemoveOldestDevice();
    // Verification returns the oldest device address
    EXPECT_EQ(removedDevice, "device1");
    // Verify that the oldest device has been removed from all mappings
    EXPECT_EQ(service.addressToDeviceSpatialInfoMap_.count("device1"), 0);
    EXPECT_EQ(service.addressToSpatialEnabledMap_.count("device1"), 0);
    EXPECT_EQ(service.addressToSpatialDeviceStateMap_.count("device1"), 0);
    // Verify that the other devices are still there
    EXPECT_EQ(service.addressToDeviceSpatialInfoMap_.count("device2"), 1);
    EXPECT_EQ(service.addressToDeviceSpatialInfoMap_.count("device3"), 1);
    EXPECT_EQ(service.addressToSpatialEnabledMap_.count("device2"), 1);
    EXPECT_EQ(service.addressToSpatialEnabledMap_.count("device3"), 1);
    EXPECT_EQ(service.addressToSpatialDeviceStateMap_.count("device2"), 1);
    EXPECT_EQ(service.addressToSpatialDeviceStateMap_.count("device3"), 1);
    // Verify the number of devices remaining
    EXPECT_EQ(service.addressToDeviceSpatialInfoMap_.size(), 2);
    EXPECT_EQ(service.addressToSpatialEnabledMap_.size(), 2);
    EXPECT_EQ(service.addressToSpatialDeviceStateMap_.size(), 2);
    // Verify that the information for the remaining devices remains the same
    EXPECT_EQ(service.addressToDeviceSpatialInfoMap_["device2"], "info2|2000");
    EXPECT_EQ(service.addressToDeviceSpatialInfoMap_["device3"], "info3|1500");
    // Verify the information for the remaining devices in the addressToSpatialDeviceStateMap_
    const auto& device2State = service.addressToSpatialDeviceStateMap_["device2"];
    EXPECT_EQ(device2State.address, "device2");
    EXPECT_FALSE(device2State.isSpatializationSupported);
    EXPECT_FALSE(device2State.isHeadTrackingSupported);
    
    const auto& device3State = service.addressToSpatialDeviceStateMap_["device3"];
    EXPECT_EQ(device3State.address, "device3");
    EXPECT_TRUE(device3State.isSpatializationSupported);
    EXPECT_FALSE(device3State.isHeadTrackingSupported);
    EXPECT_EQ(device3State.spatialDeviceType, AudioSpatialDeviceType::EARPHONE_TYPE_HEADPHONE);
}
} // namespace AudioStandard
} // namespace OHOS

