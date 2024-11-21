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

#include "oh_audio_routing_manager_unit_test.h"


using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

void OHAudioRoutingManagerUnitTest::SetUpTestCase(void) { }

void OHAudioRoutingManagerUnitTest::TearDownTestCase(void) { }

void OHAudioRoutingManagerUnitTest::SetUp(void) { }

void OHAudioRoutingManagerUnitTest::TearDown(void) { }

const int32_t INVALID_VALUIE = -2;
const int32_t MAX_VALID_SIZE = 128;

static int32_t DeviceChangeCallback(OH_AudioDevice_ChangeType type,
    OH_AudioDeviceDescriptorArray *audioDeviceDescriptorArray)
{
    int size = audioDeviceDescriptorArray->size;
    if (type == AUDIO_DEVICE_CHANGE_TYPE_CONNECT) {
        for (int index = 0; index < size; index++) {
            OH_AudioDeviceDescriptor *audioDeviceDescriptor = audioDeviceDescriptorArray->descriptors[index];
            if (audioDeviceDescriptor) {
                OH_AudioDevice_Role deviceRole = AUDIO_DEVICE_ROLE_OUTPUT;
                OH_AudioDeviceDescriptor_GetDeviceRole(audioDeviceDescriptor, &deviceRole);
                OH_AudioDevice_Type deviceType = AUDIO_DEVICE_TYPE_INVALID;
                OH_AudioDeviceDescriptor_GetDeviceType(audioDeviceDescriptor, &deviceType);
            }
        }
    }
    return 0;
}

static int32_t DeviceChangeCallbackMock(OH_AudioDevice_ChangeType type,
    OH_AudioDeviceDescriptorArray *audioDeviceDescriptorArray)
{
    (void)type;

    if (audioDeviceDescriptorArray == nullptr) {
        return -1;
    }

    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    if (audioRoutingManager == nullptr || result != AUDIOCOMMON_RESULT_SUCCESS) {
        return -1;
    }

    OH_AudioRoutingManager_ReleaseDevices(audioRoutingManager, audioDeviceDescriptorArray);
    return 0;
}

static void DeviceBlockStatusCallbackMock(OH_AudioDeviceDescriptorArray *audioDeviceDescriptorArray,
    OH_AudioDevice_BlockStatus status, void *userData)
{
    if (audioDeviceDescriptorArray == nullptr) {
        return;
    }

    (void)status;
    (void)userData;

    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    if (audioRoutingManager == nullptr || result != AUDIOCOMMON_RESULT_SUCCESS) {
        return;
    }

    OH_AudioRoutingManager_ReleaseDevices(audioRoutingManager, audioDeviceDescriptorArray);
}


/**
 * @tc.name  : Test OH_AudioRoutingManager_GetAvailableDevices with null audioRoutingManager.
 * @tc.number: OH_AudioRoutingManager_GetAvailableDevices_001
 * @tc.desc  : Test case for null audioRoutingManager.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_GetAvailableDevices_001, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    OH_AudioDevice_Usage deviceUsage = OH_AudioDevice_Usage(INVALID_VALUIE);
    OH_AudioDeviceDescriptorArray **audioDeviceDescriptorArray = nullptr;

    auto result = OH_AudioRoutingManager_GetAvailableDevices(
        audioRoutingManager, deviceUsage, audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_ERROR_INVALID_PARAM);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_GetAvailableDevices with invalid device usage.
 * @tc.number: OH_AudioRoutingManager_GetAvailableDevices_002
 * @tc.desc  : Test case for invalid device usage.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_GetAvailableDevices_002, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    OH_AudioDevice_Usage deviceUsage = OH_AudioDevice_Usage(INVALID_VALUIE);
    OH_AudioDeviceDescriptorArray **audioDeviceDescriptorArray = nullptr;

    result = OH_AudioRoutingManager_GetAvailableDevices(audioRoutingManager, deviceUsage, audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_ERROR_INVALID_PARAM);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_GetAvailableDevices with null audioRoutingManager.
 * @tc.number: OH_AudioRoutingManager_GetAvailableDevices_003
 * @tc.desc  : Test case for null audioRoutingManager.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_GetAvailableDevices_003, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    OH_AudioDevice_Usage deviceUsage = AUDIO_DEVICE_USAGE_CALL_ALL;
    OH_AudioDeviceDescriptorArray **audioDeviceDescriptorArray = nullptr;

    result = OH_AudioRoutingManager_GetAvailableDevices(audioRoutingManager, deviceUsage, audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_ERROR_INVALID_PARAM);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_GetAvailableDevices with null audioRoutingManager.
 * @tc.number: OH_AudioRoutingManager_GetAvailableDevices_004
 * @tc.desc  : Test case for null audioRoutingManager.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_GetAvailableDevices_004, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    OH_AudioDevice_Usage deviceUsage = AUDIO_DEVICE_USAGE_CALL_ALL;
    OH_AudioDeviceDescriptorArray *audioDeviceDescriptorArray = nullptr;
    OH_AudioDevice_Flag deviceFlag = AUDIO_DEVICE_FLAG_INPUT;
    result = OH_AudioRoutingManager_GetDevices(audioRoutingManager, deviceFlag, &audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioDeviceDescriptorArray, nullptr);

    result = OH_AudioRoutingManager_GetAvailableDevices(
        audioRoutingManager, deviceUsage, &audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    result = OH_AudioRoutingManager_ReleaseDevices(audioRoutingManager, audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_GetPreferredOutputDevice.
 * @tc.number: OH_AudioRoutingManager_GetPreferredOutputDevice_001
 * @tc.desc  : Test OH_AudioRoutingManager_GetPreferredOutputDevice.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_GetPreferredOutputDevice_001, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    OH_AudioStream_Usage streamUsage = (OH_AudioStream_Usage)INVALID_VALUIE;
    OH_AudioDeviceDescriptorArray **audioDeviceDescriptorArray = nullptr;

    auto result = OH_AudioRoutingManager_GetPreferredOutputDevice(
        audioRoutingManager, streamUsage, audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_ERROR_INVALID_PARAM);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_GetPreferredOutputDevice.
 * @tc.number: OH_AudioRoutingManager_GetPreferredOutputDevice_002
 * @tc.desc  : Test OH_AudioRoutingManager_GetPreferredOutputDevice.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_GetPreferredOutputDevice_002, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    OH_AudioStream_Usage streamUsage = (OH_AudioStream_Usage)INVALID_VALUIE;
    OH_AudioDeviceDescriptorArray **audioDeviceDescriptorArray = nullptr;

    result = OH_AudioRoutingManager_GetPreferredOutputDevice(
        audioRoutingManager, streamUsage, audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_ERROR_INVALID_PARAM);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_GetPreferredOutputDevice.
 * @tc.number: OH_AudioRoutingManager_GetPreferredOutputDevice_003
 * @tc.desc  : Test OH_AudioRoutingManager_GetPreferredOutputDevice.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_GetPreferredOutputDevice_003, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    OH_AudioStream_Usage streamUsage = AUDIOSTREAM_USAGE_AUDIOBOOK;
    OH_AudioDeviceDescriptorArray **audioDeviceDescriptorArray = nullptr;

    result = OH_AudioRoutingManager_GetPreferredOutputDevice(
        audioRoutingManager, streamUsage, audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_ERROR_INVALID_PARAM);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_GetPreferredOutputDevice.
 * @tc.number: OH_AudioRoutingManager_GetPreferredOutputDevice_004
 * @tc.desc  : Test OH_AudioRoutingManager_GetPreferredOutputDevice.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_GetPreferredOutputDevice_004, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    OH_AudioStream_Usage streamUsage = AUDIOSTREAM_USAGE_AUDIOBOOK;
    OH_AudioDeviceDescriptorArray *audioDeviceDescriptorArray = nullptr;
    OH_AudioDevice_Flag deviceFlag = AUDIO_DEVICE_FLAG_INPUT;
    result = OH_AudioRoutingManager_GetDevices(audioRoutingManager, deviceFlag, &audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioDeviceDescriptorArray, nullptr);

    result = OH_AudioRoutingManager_GetPreferredOutputDevice(
        audioRoutingManager, streamUsage, &audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    result = OH_AudioRoutingManager_ReleaseDevices(audioRoutingManager, audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_GetPreferredInputDevice.
 * @tc.number: OH_AudioRoutingManager_GetPreferredInputDevice_001
 * @tc.desc  : Test OH_AudioRoutingManager_GetPreferredInputDevice.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_GetPreferredInputDevice_001, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    OH_AudioStream_SourceType sourceType = (OH_AudioStream_SourceType)INVALID_VALUIE;
    OH_AudioDeviceDescriptorArray **audioDeviceDescriptorArray = nullptr;

    auto result = OH_AudioRoutingManager_GetPreferredInputDevice(
        audioRoutingManager, sourceType, audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_ERROR_INVALID_PARAM);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_GetPreferredInputDevice.
 * @tc.number: OH_AudioRoutingManager_GetPreferredInputDevice_002
 * @tc.desc  : Test OH_AudioRoutingManager_GetPreferredInputDevice.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_GetPreferredInputDevice_002, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    OH_AudioStream_SourceType sourceType = (OH_AudioStream_SourceType)INVALID_VALUIE;
    OH_AudioDeviceDescriptorArray **audioDeviceDescriptorArray = nullptr;

    result = OH_AudioRoutingManager_GetPreferredInputDevice(
        audioRoutingManager, sourceType, audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_ERROR_INVALID_PARAM);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_GetPreferredInputDevice.
 * @tc.number: OH_AudioRoutingManager_GetPreferredInputDevice_003
 * @tc.desc  : Test OH_AudioRoutingManager_GetPreferredInputDevice.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_GetPreferredInputDevice_003, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    OH_AudioStream_SourceType sourceType = AUDIOSTREAM_SOURCE_TYPE_VOICE_COMMUNICATION;
    OH_AudioDeviceDescriptorArray **audioDeviceDescriptorArray = nullptr;

    result = OH_AudioRoutingManager_GetPreferredInputDevice(
        audioRoutingManager, sourceType, audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_ERROR_INVALID_PARAM);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_GetPreferredInputDevice.
 * @tc.number: OH_AudioRoutingManager_GetPreferredInputDevice_004
 * @tc.desc  : Test OH_AudioRoutingManager_GetPreferredInputDevice.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_GetPreferredInputDevice_004, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    OH_AudioStream_SourceType sourceType = AUDIOSTREAM_SOURCE_TYPE_VOICE_COMMUNICATION;
    OH_AudioDevice_Flag deviceFlag = AUDIO_DEVICE_FLAG_OUTPUT;
    OH_AudioDeviceDescriptorArray *audioDeviceDescriptorArray = nullptr;
    result = OH_AudioRoutingManager_GetDevices(audioRoutingManager, deviceFlag, &audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioDeviceDescriptorArray, nullptr);

    result = OH_AudioRoutingManager_GetPreferredInputDevice(
        audioRoutingManager, sourceType, &audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    result = OH_AudioRoutingManager_ReleaseDevices(audioRoutingManager, audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_ReleaseDevices.
 * @tc.number: OH_AudioRoutingManager_ReleaseDevices_001
 * @tc.desc  : Test OH_AudioRoutingManager_ReleaseDevices.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_ReleaseDevices_001, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    OH_AudioDeviceDescriptorArray *audioDeviceDescriptorArray = nullptr;

    result = OH_AudioRoutingManager_ReleaseDevices(audioRoutingManager, audioDeviceDescriptorArray);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_ERROR_INVALID_PARAM);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_IsMicBlockDetectionSupported.
 * @tc.number: OH_AudioRoutingManager_IsMicBlockDetectionSupported_001
 * @tc.desc  : Test OH_AudioRoutingManager_IsMicBlockDetectionSupported.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_IsMicBlockDetectionSupported_001, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    bool *supported = nullptr;

    auto result = OH_AudioRoutingManager_IsMicBlockDetectionSupported(audioRoutingManager, supported);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_ERROR_INVALID_PARAM);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_IsMicBlockDetectionSupported.
 * @tc.number: OH_AudioRoutingManager_IsMicBlockDetectionSupported_002
 * @tc.desc  : Test OH_AudioRoutingManager_IsMicBlockDetectionSupported.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_IsMicBlockDetectionSupported_002, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    bool *supported = nullptr;

    result = OH_AudioRoutingManager_IsMicBlockDetectionSupported(audioRoutingManager, supported);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_ERROR_INVALID_PARAM);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_IsMicBlockDetectionSupported.
 * @tc.number: OH_AudioRoutingManager_IsMicBlockDetectionSupported_003
 * @tc.desc  : Test OH_AudioRoutingManager_IsMicBlockDetectionSupported.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_IsMicBlockDetectionSupported_003, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    bool supported = false;

    result = OH_AudioRoutingManager_IsMicBlockDetectionSupported(audioRoutingManager, &supported);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
}

/**
 * @tc.name  : Test SetDeviceChangeCallback.
 * @tc.number: SetDeviceChangeCallback_001
 * @tc.desc  : Test SetDeviceChangeCallback.
 */
HWTEST(OHAudioRoutingManagerUnitTest, SetDeviceChangeCallback_001, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    OH_AudioDevice_Flag deviceFlag = AUDIO_DEVICE_FLAG_ALL;
    OH_AudioRoutingManager_OnDeviceChangedCallback callback = DeviceChangeCallback;

    result = OH_AudioRoutingManager_RegisterDeviceChangeCallback(audioRoutingManager, deviceFlag, callback);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
}

/**
 * @tc.name  : Test SetDeviceChangeCallback.
 * @tc.number: SetDeviceChangeCallback_002
 * @tc.desc  : Test SetDeviceChangeCallback.
 */
HWTEST(OHAudioRoutingManagerUnitTest, SetDeviceChangeCallback_002, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    OH_AudioDevice_Flag deviceFlag = AUDIO_DEVICE_FLAG_INPUT;
    OH_AudioRoutingManager_OnDeviceChangedCallback callback = DeviceChangeCallback;

    result = OH_AudioRoutingManager_RegisterDeviceChangeCallback(audioRoutingManager, deviceFlag, callback);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_UnregisterDeviceChangeCallback.
 * @tc.number: OH_AudioRoutingManager_UnregisterDeviceChangeCallback_001
 * @tc.desc  : Test OH_AudioRoutingManager_UnregisterDeviceChangeCallback.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_UnregisterDeviceChangeCallback_001, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    OH_AudioRoutingManager_OnDeviceChangedCallback callback = DeviceChangeCallback;

    result = OH_AudioRoutingManager_UnregisterDeviceChangeCallback(audioRoutingManager, callback);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_UnregisterDeviceChangeCallback.
 * @tc.number: OH_AudioRoutingManager_UnregisterDeviceChangeCallback_002
 * @tc.desc  : Test OH_AudioRoutingManager_UnregisterDeviceChangeCallback.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_UnregisterDeviceChangeCallback_002, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    OH_AudioRoutingManager_OnDeviceChangedCallback callback = DeviceChangeCallback;

    result = OH_AudioRoutingManager_UnregisterDeviceChangeCallback(audioRoutingManager, callback);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_UnregisterDeviceChangeCallback.
 * @tc.number: OH_AudioRoutingManager_UnregisterDeviceChangeCallback_003
 * @tc.desc  : Test OH_AudioRoutingManager_UnregisterDeviceChangeCallback.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_UnregisterDeviceChangeCallback_003, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    OH_AudioRoutingManager_OnDeviceChangedCallback callback = DeviceChangeCallbackMock;

    result = OH_AudioRoutingManager_UnregisterDeviceChangeCallback(audioRoutingManager, callback);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
}

/**
 * @tc.name  : Test OH_AudioRoutingManager_SetMicBlockStatusCallback.
 * @tc.number: OH_AudioRoutingManager_SetMicBlockStatusCallback_001
 * @tc.desc  : Test OH_AudioRoutingManager_SetMicBlockStatusCallback.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OH_AudioRoutingManager_SetMicBlockStatusCallback_001, TestSize.Level0)
{
    OH_AudioRoutingManager *audioRoutingManager = nullptr;
    auto result = OH_AudioManager_GetAudioRoutingManager(&audioRoutingManager);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    EXPECT_NE(audioRoutingManager, nullptr);
    OH_AudioRoutingManager_OnDeviceBlockStatusCallback callback = nullptr;
    void *userData = nullptr;

    result = OH_AudioRoutingManager_SetMicBlockStatusCallback(audioRoutingManager, callback, userData);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
}

/**
 * @tc.name  : Test UnsetMicrophoneBlockedCallback.
 * @tc.number: UnsetMicrophoneBlockedCallback001
 * @tc.desc  : Test UnsetMicrophoneBlockedCallback.
 */
HWTEST(OHAudioRoutingManagerUnitTest, UnsetMicrophoneBlockedCallback001, TestSize.Level0)
{
    OHAudioRoutingManager* ohAudioRoutingManager = OHAudioRoutingManager::GetInstance();
    EXPECT_NE(ohAudioRoutingManager, nullptr);
    OH_AudioRoutingManager_OnDeviceBlockStatusCallback callback = DeviceBlockStatusCallbackMock;
    void *userData = nullptr;

    auto result = ohAudioRoutingManager->SetMicrophoneBlockedCallback(callback, userData);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
    result = ohAudioRoutingManager->UnsetMicrophoneBlockedCallback(callback);
    EXPECT_EQ(result, AUDIOCOMMON_RESULT_SUCCESS);
}

/**
 * @tc.name  : Test ConvertDesc.
 * @tc.number: ConvertDesc001
 * @tc.desc  : Test ConvertDesc.
 */
HWTEST(OHAudioRoutingManagerUnitTest, ConvertDesc001, TestSize.Level0)
{
    OHAudioRoutingManager* ohAudioRoutingManager = OHAudioRoutingManager::GetInstance();
    EXPECT_NE(ohAudioRoutingManager, nullptr);

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> desc;
    OH_AudioDeviceDescriptorArray *audioDeviceDescriptorArray = ohAudioRoutingManager->ConvertDesc(desc);
    EXPECT_EQ(audioDeviceDescriptorArray, nullptr);
}

/**
 * @tc.name  : Test ConvertDesc.
 * @tc.number: ConvertDesc002
 * @tc.desc  : Test ConvertDesc.
 */
HWTEST(OHAudioRoutingManagerUnitTest, ConvertDesc002, TestSize.Level0)
{
    OHAudioRoutingManager* ohAudioRoutingManager = OHAudioRoutingManager::GetInstance();
    EXPECT_NE(ohAudioRoutingManager, nullptr);

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> desc(MAX_VALID_SIZE, nullptr);

    OH_AudioDeviceDescriptorArray *audioDeviceDescriptorArray = ohAudioRoutingManager->ConvertDesc(desc);
    EXPECT_EQ(audioDeviceDescriptorArray, nullptr);
}

/**
 * @tc.name  : Test GetDevices.
 * @tc.number: GetDevices001
 * @tc.desc  : Test GetDevices.
 */
HWTEST(OHAudioRoutingManagerUnitTest, GetDevices001, TestSize.Level0)
{
    OHAudioRoutingManager* ohAudioRoutingManager = OHAudioRoutingManager::GetInstance();
    EXPECT_NE(ohAudioRoutingManager, nullptr);
    DeviceFlag deviceFlag = (DeviceFlag)INVALID_VALUIE;

    OH_AudioDeviceDescriptorArray *audioDeviceDescriptorArray = ohAudioRoutingManager->GetDevices(deviceFlag);
    EXPECT_EQ(audioDeviceDescriptorArray, nullptr);
}

/**
 * @tc.name  : Test GetAvailableDevices.
 * @tc.number: GetAvailableDevices001
 * @tc.desc  : Test GetAvailableDevices.
 */
HWTEST(OHAudioRoutingManagerUnitTest, GetAvailableDevices001, TestSize.Level0)
{
    OHAudioRoutingManager* ohAudioRoutingManager = OHAudioRoutingManager::GetInstance();
    EXPECT_NE(ohAudioRoutingManager, nullptr);
    AudioDeviceUsage deviceUsage = (AudioDeviceUsage)INVALID_VALUIE;

    OH_AudioDeviceDescriptorArray *audioDeviceDescriptorArray = ohAudioRoutingManager->GetAvailableDevices(deviceUsage);
    EXPECT_EQ(audioDeviceDescriptorArray, nullptr);
}

/**
 * @tc.name  : Test OnDeviceChange.
 * @tc.number: OnDeviceChange001
 * @tc.desc  : Test OnDeviceChange.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OnDeviceChange001, TestSize.Level0)
{
    auto ohAudioOnDeviceChangedCallback = std::make_shared<OHAudioDeviceChangedCallback>(DeviceChangeCallbackMock);
    EXPECT_NE(ohAudioOnDeviceChangedCallback, nullptr);

    DeviceChangeAction deviceChangeAction;
    deviceChangeAction.type = CONNECT;

    ohAudioOnDeviceChangedCallback->OnDeviceChange(deviceChangeAction);

    auto dec = std::make_shared<AudioDeviceDescriptor>();
    EXPECT_NE(dec, nullptr);
    deviceChangeAction.deviceDescriptors.push_back(dec);

    ohAudioOnDeviceChangedCallback->OnDeviceChange(deviceChangeAction);
}

/**
 * @tc.name  : Test OnMicrophoneBlocked.
 * @tc.number: OnMicrophoneBlocked001
 * @tc.desc  : Test OnMicrophoneBlocked.
 */
HWTEST(OHAudioRoutingManagerUnitTest, OnMicrophoneBlocked001, TestSize.Level0)
{
    void *userData = nullptr;
    OH_AudioRoutingManager_OnDeviceBlockStatusCallback callback = DeviceBlockStatusCallbackMock;

    auto ohMicrophoneBlockCallback = std::make_shared<OHMicrophoneBlockCallback>(callback, userData);
    EXPECT_NE(ohMicrophoneBlockCallback, nullptr);

    MicrophoneBlockedInfo microphoneBlockedInfo;
    microphoneBlockedInfo.blockStatus = DEVICE_UNBLOCKED;

    ohMicrophoneBlockCallback->OnMicrophoneBlocked(microphoneBlockedInfo);

    auto dec = std::make_shared<AudioDeviceDescriptor>();
    EXPECT_NE(dec, nullptr);
    microphoneBlockedInfo.devices.push_back(dec);

    ohMicrophoneBlockCallback->OnMicrophoneBlocked(microphoneBlockedInfo);
}
} // namespace AudioStandard
} // namespace OHOS