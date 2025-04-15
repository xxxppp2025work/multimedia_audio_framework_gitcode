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

#include "audio_device_common_unit_test.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {
static const int32_t GET_RESULT_NO_VALUE = 0;
static const int32_t GET_RESULT_HAS_VALUE = 1;

void AudioDeviceCommonUnitTest::SetUpTestCase(void) {}
void AudioDeviceCommonUnitTest::TearDownTestCase(void) {}
void AudioDeviceCommonUnitTest::SetUp(void) {}
void AudioDeviceCommonUnitTest::TearDown(void) {}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_001
* @tc.desc  : Test GetHasDpFlag interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.audioPolicyServerHandler_ = nullptr;
    AudioDeviceDescriptor deviceDescriptor;
    audioDeviceCommon.OnPreferredOutputDeviceUpdated(deviceDescriptor);
    EXPECT_NE(0, audioDeviceCommon.spatialDeviceMap_.size());

    DeviceType deviceType = DEVICE_TYPE_NONE;
    audioDeviceCommon.OnPreferredInputDeviceUpdated(deviceType, "");
    EXPECT_EQ(nullptr, audioDeviceCommon.audioPolicyServerHandler_);

    AudioRendererInfo rendererInfo;
    rendererInfo.streamUsage = static_cast<StreamUsage>(1000);

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> deviceOutputList =
        audioDeviceCommon.GetPreferredOutputDeviceDescInner(rendererInfo, "");
    EXPECT_EQ(1, deviceOutputList.size());

    AudioCapturerInfo captureInfo;
    captureInfo.sourceType = SOURCE_TYPE_INVALID;
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> deviceInputList =
        audioDeviceCommon.GetPreferredInputDeviceDescInner(captureInfo, "");
    EXPECT_EQ(1, deviceOutputList.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_002
* @tc.desc  : Test IsRingerOrAlarmerDualDevicesRange interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    InternalDeviceType deviceType = DEVICE_TYPE_SPEAKER;
    bool ret = audioDeviceCommon.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(true, ret);

    deviceType = DEVICE_TYPE_WIRED_HEADSET;
    ret = audioDeviceCommon.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(true, ret);

    deviceType = DEVICE_TYPE_WIRED_HEADPHONES;
    ret = audioDeviceCommon.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(true, ret);

    deviceType = DEVICE_TYPE_BLUETOOTH_SCO;
    ret = audioDeviceCommon.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(true, ret);

    deviceType = DEVICE_TYPE_BLUETOOTH_A2DP;
    ret = audioDeviceCommon.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(true, ret);

    deviceType = DEVICE_TYPE_USB_HEADSET;
    ret = audioDeviceCommon.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(true, ret);

    deviceType = DEVICE_TYPE_USB_ARM_HEADSET;
    ret = audioDeviceCommon.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(true, ret);

    deviceType = DEVICE_TYPE_REMOTE_CAST;
    ret = audioDeviceCommon.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(true, ret);

    deviceType = DEVICE_TYPE_EARPIECE;
    ret = audioDeviceCommon.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(false, ret);

    deviceType = DEVICE_TYPE_BLUETOOTH_A2DP_IN;
    ret = audioDeviceCommon.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(false, ret);

    deviceType = DEVICE_TYPE_MIC;
    ret = audioDeviceCommon.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(false, ret);

    deviceType = DEVICE_TYPE_DP;
    ret = audioDeviceCommon.IsRingerOrAlarmerDualDevicesRange(deviceType);
    EXPECT_EQ(false, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_003
* @tc.desc  : Test GetPreferredOutputDeviceDescInner interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_003, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.audioPolicyServerHandler_ = nullptr;
    AudioDeviceDescriptor deviceDescriptor;
    audioDeviceCommon.OnPreferredOutputDeviceUpdated(deviceDescriptor);

    DeviceType deviceType = DEVICE_TYPE_NONE;
    audioDeviceCommon.OnPreferredInputDeviceUpdated(deviceType, "");

    AudioRendererInfo rendererInfo;
    rendererInfo.streamUsage = STREAM_USAGE_MUSIC;
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> ret =
        audioDeviceCommon.GetPreferredOutputDeviceDescInner(rendererInfo, "LocalDevice");
    EXPECT_EQ(GET_RESULT_HAS_VALUE, ret.size());

    AudioCapturerInfo captureInfo;
    captureInfo.sourceType = SOURCE_TYPE_MAX;
    ret = audioDeviceCommon.GetPreferredInputDeviceDescInner(captureInfo, "");
    EXPECT_EQ(GET_RESULT_NO_VALUE, ret.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_004
* @tc.desc  : Test GetPreferredOutputDeviceDescInner interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_004, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.audioPolicyServerHandler_ = nullptr;
    AudioDeviceDescriptor deviceDescriptor;
    audioDeviceCommon.OnPreferredOutputDeviceUpdated(deviceDescriptor);

    DeviceType deviceType = DEVICE_TYPE_NONE;
    audioDeviceCommon.OnPreferredInputDeviceUpdated(deviceType, "");

    AudioRendererInfo rendererInfo;
    rendererInfo.streamUsage = STREAM_USAGE_MUSIC;
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> ret =
        audioDeviceCommon.GetPreferredOutputDeviceDescInner(rendererInfo, "");
    EXPECT_EQ(GET_RESULT_NO_VALUE, ret.size());

    AudioCapturerInfo captureInfo;
    captureInfo.sourceType = SOURCE_TYPE_WAKEUP;
    ret = audioDeviceCommon.GetPreferredInputDeviceDescInner(captureInfo, "");
    EXPECT_EQ(GET_RESULT_HAS_VALUE, ret.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_005
* @tc.desc  : Test GetPreferredInputDeviceDescInner interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_005, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.audioPolicyServerHandler_ = nullptr;
    AudioDeviceDescriptor deviceDescriptor;
    audioDeviceCommon.OnPreferredOutputDeviceUpdated(deviceDescriptor);

    DeviceType deviceType = DEVICE_TYPE_NONE;
    audioDeviceCommon.OnPreferredInputDeviceUpdated(deviceType, "");

    AudioRendererInfo rendererInfo;
    rendererInfo.streamUsage = STREAM_USAGE_MUSIC;
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> ret =
        audioDeviceCommon.GetPreferredOutputDeviceDescInner(rendererInfo, "");
    EXPECT_EQ(GET_RESULT_NO_VALUE, ret.size());

    AudioCapturerInfo captureInfo;
    captureInfo.sourceType = SOURCE_TYPE_MIC;
    ret = audioDeviceCommon.GetPreferredInputDeviceDescInner(captureInfo, "LocalDevice");
    EXPECT_EQ(GET_RESULT_HAS_VALUE, ret.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_006
* @tc.desc  : Test GetPreferredInputDeviceDescInner interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_006, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.audioPolicyServerHandler_ = nullptr;
    AudioDeviceDescriptor deviceDescriptor;
    audioDeviceCommon.OnPreferredOutputDeviceUpdated(deviceDescriptor);

    DeviceType deviceType = DEVICE_TYPE_NONE;
    audioDeviceCommon.OnPreferredInputDeviceUpdated(deviceType, "");

    AudioRendererInfo rendererInfo;
    rendererInfo.streamUsage = STREAM_USAGE_MUSIC;
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> ret =
        audioDeviceCommon.GetPreferredOutputDeviceDescInner(rendererInfo, "");
    EXPECT_EQ(GET_RESULT_NO_VALUE, ret.size());

    AudioCapturerInfo captureInfo;
    captureInfo.sourceType = SOURCE_TYPE_MIC;
    ret = audioDeviceCommon.GetPreferredInputDeviceDescInner(captureInfo, "");
    EXPECT_EQ(GET_RESULT_NO_VALUE, ret.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_010
* @tc.desc  : Test IsRingerOrAlarmerDualDevicesRange interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_010, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    DeviceType deviceType = DEVICE_TYPE_EARPIECE;
    bool isRemote = true;
    bool ret = audioDeviceCommon.HasLowLatencyCapability(deviceType, isRemote);
    EXPECT_EQ(true, ret);

    isRemote = false;
    deviceType = DEVICE_TYPE_EARPIECE;
    ret = audioDeviceCommon.HasLowLatencyCapability(deviceType, isRemote);
    EXPECT_EQ(true, ret);

    isRemote = false;
    deviceType = DEVICE_TYPE_SPEAKER;
    ret = audioDeviceCommon.HasLowLatencyCapability(deviceType, isRemote);
    EXPECT_EQ(true, ret);

    isRemote = false;
    deviceType = DEVICE_TYPE_WIRED_HEADSET;
    ret = audioDeviceCommon.HasLowLatencyCapability(deviceType, isRemote);
    EXPECT_EQ(true, ret);

    isRemote = false;
    deviceType = DEVICE_TYPE_WIRED_HEADPHONES;
    ret = audioDeviceCommon.HasLowLatencyCapability(deviceType, isRemote);
    EXPECT_EQ(true, ret);

    isRemote = false;
    deviceType = DEVICE_TYPE_USB_HEADSET;
    ret = audioDeviceCommon.HasLowLatencyCapability(deviceType, isRemote);
    EXPECT_EQ(true, ret);

    isRemote = false;
    deviceType = DEVICE_TYPE_DP;
    ret = audioDeviceCommon.HasLowLatencyCapability(deviceType, isRemote);
    EXPECT_EQ(true, ret);

    isRemote = false;
    deviceType = DEVICE_TYPE_BLUETOOTH_SCO;
    ret = audioDeviceCommon.HasLowLatencyCapability(deviceType, isRemote);
    EXPECT_EQ(false, ret);

    isRemote = false;
    deviceType = DEVICE_TYPE_BLUETOOTH_A2DP;
    ret = audioDeviceCommon.HasLowLatencyCapability(deviceType, isRemote);
    EXPECT_EQ(false, ret);

    isRemote = false;
    deviceType = DEVICE_TYPE_BLUETOOTH_A2DP_IN;
    ret = audioDeviceCommon.HasLowLatencyCapability(deviceType, isRemote);
    EXPECT_EQ(false, ret);

    isRemote = false;
    deviceType = DEVICE_TYPE_MIC;
    ret = audioDeviceCommon.HasLowLatencyCapability(deviceType, isRemote);
    EXPECT_EQ(false, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_020
* @tc.desc  : Test UpdateDeviceInfo interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_020, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    AudioDeviceDescriptor deviceInfo;
    bool hasBTPermission = true;
    bool hasSystemPermission = true;
    BluetoothOffloadState state = NO_A2DP_DEVICE;
    audioDeviceCommon.audioA2dpOffloadFlag_.SetA2dpOffloadFlag(state);
    audioDeviceCommon.UpdateDeviceInfo(deviceInfo, std::make_shared<AudioDeviceDescriptor>(),
        hasBTPermission, hasSystemPermission);
    EXPECT_EQ(NO_A2DP_DEVICE, deviceInfo.a2dpOffloadFlag_);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_021
* @tc.desc  : Test UpdateDeviceInfo interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_021, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    AudioDeviceDescriptor deviceInfo;
    bool hasBTPermission = false;
    bool hasSystemPermission = true;
    BluetoothOffloadState state = NO_A2DP_DEVICE;
    audioDeviceCommon.audioA2dpOffloadFlag_.SetA2dpOffloadFlag(state);
    audioDeviceCommon.UpdateDeviceInfo(deviceInfo, std::make_shared<AudioDeviceDescriptor>(),
        hasBTPermission, hasSystemPermission);
    EXPECT_EQ(CATEGORY_DEFAULT, deviceInfo.deviceCategory_);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_022
* @tc.desc  : Test UpdateDeviceInfo interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_022, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    AudioDeviceDescriptor deviceInfo;
    bool hasBTPermission = false;
    bool hasSystemPermission = false;
    BluetoothOffloadState state = NO_A2DP_DEVICE;
    audioDeviceCommon.audioA2dpOffloadFlag_.SetA2dpOffloadFlag(state);
    audioDeviceCommon.UpdateDeviceInfo(deviceInfo, std::make_shared<AudioDeviceDescriptor>(),
        hasBTPermission, hasSystemPermission);
    EXPECT_EQ(GROUP_ID_NONE, deviceInfo.volumeGroupId_);
    EXPECT_EQ(GROUP_ID_NONE, deviceInfo.interruptGroupId_);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_023
* @tc.desc  : Test DeviceParamsCheck interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_023, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    DeviceRole targetRole = INPUT_DEVICE;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorSptr = std::make_shared<AudioDeviceDescriptor>();
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorSptrVector;
    audioDeviceDescriptorSptrVector.push_back(audioDeviceDescriptorSptr);
    int32_t ret = audioDeviceCommon.DeviceParamsCheck(targetRole, audioDeviceDescriptorSptrVector);
    EXPECT_EQ(ERR_INVALID_OPERATION, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_024
* @tc.desc  : Test DeviceParamsCheck interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_024, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    DeviceRole targetRole = OUTPUT_DEVICE;
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> sharedAudioDeviceDescriptors;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorSptr = std::make_shared<AudioDeviceDescriptor>();
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorSptrVector;
    audioDeviceDescriptorSptrVector.push_back(audioDeviceDescriptorSptr);
    int32_t ret = audioDeviceCommon.DeviceParamsCheck(targetRole, audioDeviceDescriptorSptrVector);
    EXPECT_EQ(ERR_INVALID_OPERATION, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_026
* @tc.desc  : Test UpdateConnectedDevicesWhenConnecting interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_026, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    AudioDeviceDescriptor updatedDesc;
    updatedDesc.deviceType_ = DEVICE_TYPE_USB_ARM_HEADSET;
    updatedDesc.deviceRole_ = INPUT_DEVICE;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorSptr = std::make_shared<AudioDeviceDescriptor>();
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorSptrVector;
    audioDeviceDescriptorSptrVector.push_back(audioDeviceDescriptorSptr);
    audioDeviceCommon.UpdateConnectedDevicesWhenConnecting(updatedDesc, audioDeviceDescriptorSptrVector);
    audioDeviceCommon.RemoveOfflineDevice(updatedDesc);
    EXPECT_EQ(2, audioDeviceDescriptorSptrVector.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_027
* @tc.desc  : Test UpdateConnectedDevicesWhenConnecting interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_027, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    AudioDeviceDescriptor updatedDesc;
    updatedDesc.deviceType_ = DEVICE_TYPE_USB_ARM_HEADSET;
    updatedDesc.deviceRole_ = OUTPUT_DEVICE;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorSptr = std::make_shared<AudioDeviceDescriptor>();
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorSptrVector;
    audioDeviceDescriptorSptrVector.push_back(audioDeviceDescriptorSptr);
    audioDeviceCommon.UpdateConnectedDevicesWhenConnecting(updatedDesc, audioDeviceDescriptorSptrVector);
    audioDeviceCommon.RemoveOfflineDevice(updatedDesc);
    EXPECT_EQ(2, audioDeviceDescriptorSptrVector.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_028
* @tc.desc  : Test UpdateConnectedDevicesWhenDisconnecting interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_028, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    AudioDeviceDescriptor updatedDesc;
    updatedDesc.deviceType_ = DEVICE_TYPE_USB_ARM_HEADSET;
    updatedDesc.deviceRole_ = OUTPUT_DEVICE;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorSptr = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptorSptr->deviceType_ = DEVICE_TYPE_DP;
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorSptrVector;
    audioDeviceDescriptorSptrVector.push_back(audioDeviceDescriptorSptr);
    audioDeviceCommon.UpdateConnectedDevicesWhenDisconnecting(updatedDesc, audioDeviceDescriptorSptrVector);
    EXPECT_EQ(false, audioDeviceCommon.hasDpDevice_);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_029
* @tc.desc  : Test UpdateConnectedDevicesWhenDisconnecting interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_029, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    AudioDeviceDescriptor updatedDesc;
    updatedDesc.deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    updatedDesc.deviceRole_ = INPUT_DEVICE;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorSptr = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptorSptr->deviceType_ = DEVICE_TYPE_DP;
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorSptrVector;
    audioDeviceDescriptorSptrVector.push_back(audioDeviceDescriptorSptr);
    audioDeviceCommon.UpdateConnectedDevicesWhenDisconnecting(updatedDesc, audioDeviceDescriptorSptrVector);
    EXPECT_EQ(false, audioDeviceCommon.hasDpDevice_);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_030
* @tc.desc  : Test UpdateConnectedDevicesWhenConnectingForInputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_030, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    AudioDeviceDescriptor updatedDesc;
    updatedDesc.deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    updatedDesc.deviceRole_ = INPUT_DEVICE;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorSptr = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptorSptr->deviceType_ = DEVICE_TYPE_DP;
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorSptrVector;
    audioDeviceDescriptorSptrVector.push_back(audioDeviceDescriptorSptr);
    audioDeviceCommon.UpdateConnectedDevicesWhenConnectingForInputDevice(updatedDesc,
        audioDeviceDescriptorSptrVector);
    EXPECT_EQ(2, audioDeviceDescriptorSptrVector.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_031
* @tc.desc  : Test UpdateConnectedDevicesWhenConnectingForInputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_031, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    AudioDeviceDescriptor updatedDesc;
    updatedDesc.deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    updatedDesc.deviceRole_ = INPUT_DEVICE;
    updatedDesc.connectState_ = VIRTUAL_CONNECTED;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorSptr = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptorSptr->deviceType_ = DEVICE_TYPE_DP;
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorSptrVector;
    audioDeviceDescriptorSptrVector.push_back(audioDeviceDescriptorSptr);
    audioDeviceCommon.UpdateConnectedDevicesWhenConnectingForInputDevice(updatedDesc,
        audioDeviceDescriptorSptrVector);
    EXPECT_EQ(2, audioDeviceDescriptorSptrVector.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_032
* @tc.desc  : Test UpdateConnectedDevicesWhenConnectingForOutputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_032, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    AudioDeviceDescriptor updatedDesc;
    updatedDesc.deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    updatedDesc.deviceRole_ = INPUT_DEVICE;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorSptr = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptorSptr->deviceType_ = DEVICE_TYPE_DP;
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorSptrVector;
    audioDeviceDescriptorSptrVector.push_back(audioDeviceDescriptorSptr);
    audioDeviceCommon.UpdateConnectedDevicesWhenConnectingForOutputDevice(updatedDesc,
        audioDeviceDescriptorSptrVector);
    EXPECT_EQ(2, audioDeviceDescriptorSptrVector.size());
}

/**    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorSptrVector;
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_033
* @tc.desc  : Test UpdateConnectedDevicesWhenConnectingForOutputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_033, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    AudioDeviceDescriptor updatedDesc;
    updatedDesc.deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    updatedDesc.deviceRole_ = INPUT_DEVICE;
    updatedDesc.connectState_ = VIRTUAL_CONNECTED;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorSptr = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptorSptr->deviceType_ = DEVICE_TYPE_DP;
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorSptrVector;
    audioDeviceDescriptorSptrVector.push_back(audioDeviceDescriptorSptr);
    audioDeviceCommon.UpdateConnectedDevicesWhenConnectingForOutputDevice(updatedDesc,
        audioDeviceDescriptorSptrVector);
    EXPECT_EQ(2, audioDeviceDescriptorSptrVector.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_034
* @tc.desc  : Test UpdateDualToneState interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_034, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    bool enable = true;
    int32_t sessionId = 0;
    audioDeviceCommon.UpdateDualToneState(enable, sessionId);
    EXPECT_EQ(sessionId, audioDeviceCommon.enableDualHalToneSessionId_);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_035
* @tc.desc  : Test UpdateDualToneState interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_035, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    bool enable = false;
    int32_t sessionId = 0;
    audioDeviceCommon.UpdateDualToneState(enable, sessionId);
    EXPECT_EQ(enable, audioDeviceCommon.enableDualHalToneState_);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_036
* @tc.desc  : Test IsFastFromA2dpToA2dp interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_036, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    rendererChangeInfo->outputDeviceInfo.deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    rendererChangeInfo->rendererInfo.originalFlag = AUDIO_FLAG_MMAP;
    rendererChangeInfo->outputDeviceInfo.deviceId_ = 0;
    desc->deviceId_ = 1;
    bool ret = audioDeviceCommon.IsFastFromA2dpToA2dp(desc, rendererChangeInfo, reason);
    EXPECT_EQ(true, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_037
* @tc.desc  : Test IsFastFromA2dpToA2dp interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_037, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    rendererChangeInfo->outputDeviceInfo.deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    rendererChangeInfo->rendererInfo.originalFlag = AUDIO_FLAG_MMAP;
    rendererChangeInfo->outputDeviceInfo.deviceId_ = 0;
    desc->deviceId_ = 0;
    bool ret = audioDeviceCommon.IsFastFromA2dpToA2dp(desc, rendererChangeInfo, reason);
    EXPECT_EQ(false, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_038
* @tc.desc  : Test NotifyRecreateDirectStream interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_038, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    rendererChangeInfo->rendererInfo.pipeType = PIPE_TYPE_DIRECT_MUSIC;
    rendererChangeInfo->outputDeviceInfo.deviceType_ = DEVICE_TYPE_USB_ARM_HEADSET;
    bool ret = audioDeviceCommon.NotifyRecreateDirectStream(rendererChangeInfo, reason);
    EXPECT_EQ(false, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_039
* @tc.desc  : Test NotifyRecreateDirectStream interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_039, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    rendererChangeInfo->rendererInfo.pipeType = PIPE_TYPE_DIRECT_MUSIC;
    rendererChangeInfo->outputDeviceInfo.deviceType_ = DEVICE_TYPE_MIC;
    bool ret = audioDeviceCommon.NotifyRecreateDirectStream(rendererChangeInfo, reason);
    EXPECT_EQ(true, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_040
* @tc.desc  : Test NotifyRecreateDirectStream interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_040, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    audioDeviceCommon.audioActiveDevice_.SetCurrentOutputDeviceType(DEVICE_TYPE_WIRED_HEADSET);
    rendererChangeInfo->rendererInfo.pipeType = PIPE_TYPE_OFFLOAD;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_MUSIC;
    rendererChangeInfo->rendererInfo.rendererFlags = AUDIO_FLAG_NORMAL;
    rendererChangeInfo->rendererInfo.samplingRate = SAMPLE_RATE_48000;
    rendererChangeInfo->rendererInfo.format = SAMPLE_S24LE;
    bool ret = audioDeviceCommon.NotifyRecreateDirectStream(rendererChangeInfo, reason);
    EXPECT_EQ(true, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_041
* @tc.desc  : Test NotifyRecreateDirectStream interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_041, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    audioDeviceCommon.audioActiveDevice_.SetCurrentOutputDeviceType(DEVICE_TYPE_WIRED_HEADSET);
    rendererChangeInfo->rendererInfo.pipeType = PIPE_TYPE_OFFLOAD;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_MUSIC;
    rendererChangeInfo->rendererInfo.rendererFlags = AUDIO_FLAG_NORMAL;
    rendererChangeInfo->rendererInfo.samplingRate = SAMPLE_RATE_8000;
    rendererChangeInfo->rendererInfo.format = SAMPLE_S24LE;
    bool ret = audioDeviceCommon.NotifyRecreateDirectStream(rendererChangeInfo, reason);
    EXPECT_EQ(false, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_042
* @tc.desc  : Test NotifyRecreateDirectStream interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_042, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    audioDeviceCommon.audioActiveDevice_.SetCurrentOutputDeviceType(DEVICE_TYPE_USB_HEADSET);
    rendererChangeInfo->rendererInfo.pipeType = PIPE_TYPE_OFFLOAD;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_MUSIC;
    rendererChangeInfo->rendererInfo.rendererFlags = AUDIO_FLAG_NORMAL;
    rendererChangeInfo->rendererInfo.samplingRate = SAMPLE_RATE_8000;
    rendererChangeInfo->rendererInfo.format = SAMPLE_S24LE;
    bool ret = audioDeviceCommon.NotifyRecreateDirectStream(rendererChangeInfo, reason);
    EXPECT_EQ(false, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_043
* @tc.desc  : Test MuteSinkForSwitchGeneralDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_043, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorUniqueptr = std::make_shared<AudioDeviceDescriptor>();
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorUniqueptrVector;
    audioDeviceDescriptorUniqueptr->deviceType_ = DEVICE_TYPE_SPEAKER;
    audioDeviceDescriptorUniqueptrVector.push_back(std::move(audioDeviceDescriptorUniqueptr));
    audioDeviceCommon.MuteSinkForSwitchGeneralDevice(rendererChangeInfo,
        audioDeviceDescriptorUniqueptrVector, reason);
    EXPECT_EQ(1, audioDeviceDescriptorUniqueptrVector.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_044
* @tc.desc  : Test MuteSinkForSwitchGeneralDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_044, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorUniqueptr = std::make_shared<AudioDeviceDescriptor>();
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorUniqueptrVector;
    audioDeviceDescriptorUniqueptr->deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    audioDeviceDescriptorUniqueptrVector.push_back(std::move(audioDeviceDescriptorUniqueptr));
    audioDeviceCommon.MuteSinkForSwitchGeneralDevice(rendererChangeInfo,
        audioDeviceDescriptorUniqueptrVector, reason);
    EXPECT_EQ(1, audioDeviceDescriptorUniqueptrVector.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_045
* @tc.desc  : Test MuteSinkForSwitchGeneralDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_045, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorUniqueptr = std::make_shared<AudioDeviceDescriptor>();
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorUniqueptrVector;
    audioDeviceDescriptorUniqueptr->deviceType_ = DEVICE_TYPE_BLUETOOTH_SCO;
    audioDeviceDescriptorUniqueptrVector.push_back(std::move(audioDeviceDescriptorUniqueptr));
    audioDeviceCommon.MuteSinkForSwitchGeneralDevice(rendererChangeInfo,
        audioDeviceDescriptorUniqueptrVector, reason);
    EXPECT_EQ(1, audioDeviceDescriptorUniqueptrVector.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_046
* @tc.desc  : Test MuteSinkForSwitchBluetoothDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_046, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorUniqueptr = std::make_shared<AudioDeviceDescriptor>();
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorUniqueptrVector;
    audioDeviceDescriptorUniqueptr->deviceType_ = DEVICE_TYPE_SPEAKER;
    audioDeviceDescriptorUniqueptrVector.push_back(std::move(audioDeviceDescriptorUniqueptr));
    audioDeviceCommon.MuteSinkForSwitchBluetoothDevice(rendererChangeInfo,
        audioDeviceDescriptorUniqueptrVector, reason);
    EXPECT_EQ(1, audioDeviceDescriptorUniqueptrVector.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_047
* @tc.desc  : Test MuteSinkForSwitchBluetoothDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_047, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorUniqueptr = std::make_shared<AudioDeviceDescriptor>();
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorUniqueptrVector;
    audioDeviceDescriptorUniqueptr->deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    audioDeviceDescriptorUniqueptrVector.push_back(std::move(audioDeviceDescriptorUniqueptr));
    audioDeviceCommon.MuteSinkForSwitchBluetoothDevice(rendererChangeInfo,
        audioDeviceDescriptorUniqueptrVector, reason);
    EXPECT_EQ(1, audioDeviceDescriptorUniqueptrVector.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_048
* @tc.desc  : Test MuteSinkForSwitchBluetoothDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_048, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorUniqueptr = std::make_shared<AudioDeviceDescriptor>();
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorUniqueptrVector;
    audioDeviceDescriptorUniqueptr->deviceType_ = DEVICE_TYPE_BLUETOOTH_SCO;
    audioDeviceDescriptorUniqueptrVector.push_back(std::move(audioDeviceDescriptorUniqueptr));
    audioDeviceCommon.MuteSinkForSwitchBluetoothDevice(rendererChangeInfo,
        audioDeviceDescriptorUniqueptrVector, reason);
    EXPECT_EQ(1, audioDeviceDescriptorUniqueptrVector.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_049
* @tc.desc  : Test MuteSinkForSwitchDistributedDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_049, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReasonExt::ExtEnum::DISTRIBUTED_DEVICE;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptorUniqueptr = std::make_shared<AudioDeviceDescriptor>();
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptorUniqueptrVector;
    audioDeviceDescriptorUniqueptr->deviceType_ = DEVICE_TYPE_SPEAKER;
    audioDeviceDescriptorUniqueptrVector.push_back(std::move(audioDeviceDescriptorUniqueptr));
    audioDeviceCommon.MuteSinkForSwitchDistributedDevice(rendererChangeInfo,
        audioDeviceDescriptorUniqueptrVector, reason);
    EXPECT_EQ(1, audioDeviceDescriptorUniqueptrVector.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_050
* @tc.desc  : Test IsRendererStreamRunning interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_050, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_VOICE_MODEM_COMMUNICATION;
    audioDeviceCommon.audioSceneManager_.SetAudioScenePre(AUDIO_SCENE_RINGING);
    bool ret = audioDeviceCommon.IsRendererStreamRunning(rendererChangeInfo);
    EXPECT_EQ(false, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_051
* @tc.desc  : Test ActivateA2dpDeviceWhenDescEnabled interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_051, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->isEnable_ = false;
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    vector<std::shared_ptr<AudioRendererChangeInfo>> rendererChangeInfos;
    rendererChangeInfos.push_back(std::move(rendererChangeInfo));
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    int32_t ret = audioDeviceCommon.ActivateA2dpDeviceWhenDescEnabled(desc, rendererChangeInfos, reason);
    EXPECT_EQ(SUCCESS, ret);

    desc->isEnable_ = true;
    ret = audioDeviceCommon.ActivateA2dpDeviceWhenDescEnabled(desc, rendererChangeInfos, reason);
    EXPECT_EQ(ERROR, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_052
* @tc.desc  : Test HandleScoOutputDeviceFetched interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_052, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    vector<std::shared_ptr<AudioRendererChangeInfo>> rendererChangeInfos;
    rendererChangeInfos.push_back(std::move(rendererChangeInfo));
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    int32_t ret = audioDeviceCommon.HandleScoOutputDeviceFetched(desc, rendererChangeInfos, reason);
    EXPECT_EQ(ERROR, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_053
* @tc.desc  : Test NotifyRecreateRendererStream interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_053, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    rendererChangeInfo->outputDeviceInfo.deviceType_ = DEVICE_TYPE_SPEAKER;
    desc->deviceType_ = DEVICE_TYPE_EARPIECE;
    rendererChangeInfo->rendererInfo.originalFlag = AUDIO_FLAG_MMAP;
    rendererChangeInfo->rendererInfo.originalFlag = AUDIO_FLAG_VOIP_DIRECT;
    rendererChangeInfo->outputDeviceInfo.deviceType_ = DEVICE_TYPE_USB_ARM_HEADSET;
    desc->deviceType_ = DEVICE_TYPE_USB_ARM_HEADSET;
    bool ret = audioDeviceCommon.NotifyRecreateRendererStream(desc, rendererChangeInfo, reason);
    EXPECT_EQ(false, ret);

    desc->deviceType_ = DEVICE_TYPE_SPEAKER;
    rendererChangeInfo->outputDeviceInfo.networkId_ = "test1";
    desc->networkId_ = "test2";
    ret = audioDeviceCommon.NotifyRecreateRendererStream(desc, rendererChangeInfo, reason);
    EXPECT_EQ(false, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_054
* @tc.desc  : Test NeedRehandleA2DPDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_054, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->deviceType_ = DEVICE_TYPE_SPEAKER;
    bool ret = audioDeviceCommon.NeedRehandleA2DPDevice(desc);
    EXPECT_EQ(false, ret);

    desc->deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    ret = audioDeviceCommon.NeedRehandleA2DPDevice(desc);
    EXPECT_EQ(true, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_055
* @tc.desc  : Test ActivateA2dpDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_055, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    vector<std::shared_ptr<AudioRendererChangeInfo>> rendererChangeInfos;
    rendererChangeInfos.push_back(std::move(rendererChangeInfo));
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    int32_t ret = audioDeviceCommon.ActivateA2dpDevice(desc, rendererChangeInfos, reason);
    EXPECT_EQ(ERROR, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_056
* @tc.desc  : Test TriggerRecreateRendererStreamCallback interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_056, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    int32_t callerPid = 0;
    int32_t sessionId = 0;
    int32_t streamFlag = 0;
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    audioDeviceCommon.audioPolicyServerHandler_ = std::make_shared<AudioPolicyServerHandler>();
    audioDeviceCommon.TriggerRecreateRendererStreamCallback(callerPid, sessionId, streamFlag, reason);
    EXPECT_EQ(true, audioDeviceCommon.audioPolicyServerHandler_ != nullptr);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_057
* @tc.desc  : Test HandleBluetoothInputDeviceFetched interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_057, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->deviceType_ = DEVICE_TYPE_BLUETOOTH_SCO;
    std::shared_ptr<AudioCapturerChangeInfo> captureChangeInfo = std::make_shared<AudioCapturerChangeInfo>();
    vector<std::shared_ptr<AudioCapturerChangeInfo>> captureChangeInfos;
    captureChangeInfos.push_back(std::move(captureChangeInfo));

    SourceType sourceType = SOURCE_TYPE_MIC;
    audioDeviceCommon.HandleBluetoothInputDeviceFetched(desc, captureChangeInfos, sourceType);
    EXPECT_EQ(1, captureChangeInfos.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_058
* @tc.desc  : Test HandleBluetoothInputDeviceFetched interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_058, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP_IN;
    std::shared_ptr<AudioCapturerChangeInfo> captureChangeInfo = std::make_shared<AudioCapturerChangeInfo>();
    vector<std::shared_ptr<AudioCapturerChangeInfo>> captureChangeInfos;
    captureChangeInfos.push_back(std::move(captureChangeInfo));
    SourceType sourceType = SOURCE_TYPE_MIC;
    audioDeviceCommon.HandleBluetoothInputDeviceFetched(desc, captureChangeInfos, sourceType);
    EXPECT_EQ(1, captureChangeInfos.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_059
* @tc.desc  : Test HandleBluetoothInputDeviceFetched interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_059, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->deviceType_ = DEVICE_TYPE_MIC;
    std::shared_ptr<AudioCapturerChangeInfo> captureChangeInfo = std::make_shared<AudioCapturerChangeInfo>();
    vector<std::shared_ptr<AudioCapturerChangeInfo>> captureChangeInfos;
    captureChangeInfos.push_back(std::move(captureChangeInfo));
    SourceType sourceType = SOURCE_TYPE_MIC;
    audioDeviceCommon.HandleBluetoothInputDeviceFetched(desc, captureChangeInfos, sourceType);
    EXPECT_EQ(1, captureChangeInfos.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_060
* @tc.desc  : Test HandleBluetoothInputDeviceFetched interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_060, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->deviceType_ = DEVICE_TYPE_MIC;
    std::shared_ptr<AudioCapturerChangeInfo> captureChangeInfo = std::make_shared<AudioCapturerChangeInfo>();
    vector<std::shared_ptr<AudioCapturerChangeInfo>> captureChangeInfos;
    captureChangeInfos.push_back(std::move(captureChangeInfo));
    SourceType sourceType = SOURCE_TYPE_MIC;
    audioDeviceCommon.HandleBluetoothInputDeviceFetched(desc, captureChangeInfos, sourceType);
    EXPECT_EQ(1, captureChangeInfos.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_061
* @tc.desc  : Test NotifyRecreateCapturerStream interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_061, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    bool isUpdateActiveDevice = true;
    std::shared_ptr<AudioCapturerChangeInfo> capturerChangeInfo = std::make_shared<AudioCapturerChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    capturerChangeInfo->capturerInfo.originalFlag = AUDIO_FLAG_MMAP;
    capturerChangeInfo->inputDeviceInfo.deviceType_ = DEVICE_TYPE_MIC;
    audioDeviceCommon.audioActiveDevice_.SetCurrentInputDeviceType(DEVICE_TYPE_MIC);
    capturerChangeInfo->inputDeviceInfo.networkId_ = "test";
    bool ret = audioDeviceCommon.NotifyRecreateCapturerStream(isUpdateActiveDevice,
        capturerChangeInfo, reason);
    EXPECT_EQ(true, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_062
* @tc.desc  : Test NotifyRecreateCapturerStream interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_062, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    bool isUpdateActiveDevice = true;
    std::shared_ptr<AudioCapturerChangeInfo> capturerChangeInfo = std::make_shared<AudioCapturerChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    capturerChangeInfo->capturerInfo.originalFlag = AUDIO_FLAG_MMAP;
    capturerChangeInfo->inputDeviceInfo.deviceType_ = DEVICE_TYPE_MIC;
    audioDeviceCommon.audioActiveDevice_.SetCurrentInputDeviceType(DEVICE_TYPE_MIC);
    capturerChangeInfo->inputDeviceInfo.networkId_ = "LocalDevice";
    bool ret = audioDeviceCommon.NotifyRecreateCapturerStream(isUpdateActiveDevice,
        capturerChangeInfo, reason);
    EXPECT_EQ(false, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_063
* @tc.desc  : Test ReloadA2dpAudioPort interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_063, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    AudioModuleInfo moduleInfo;
    DeviceType deviceType = DEVICE_TYPE_BLUETOOTH_A2DP;
    AudioStreamInfo audioStreamInfo;
    std::string networkID = "";
    std::string sinkName = "";
    SourceType sourceType = SOURCE_TYPE_MIC;
    int32_t ret = audioDeviceCommon.ReloadA2dpAudioPort(moduleInfo, deviceType, audioStreamInfo, networkID,
        sinkName, sourceType);
    EXPECT_EQ(ERR_OPERATION_FAILED, ret);

    deviceType = DEVICE_TYPE_BLUETOOTH_A2DP_IN;
    ret = audioDeviceCommon.ReloadA2dpAudioPort(moduleInfo, deviceType, audioStreamInfo, networkID,
        sinkName, sourceType);
    EXPECT_EQ(ERR_OPERATION_FAILED, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_064
* @tc.desc  : Test ScoInputDeviceFetchedForRecongnition interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_064, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    bool handleFlag = true;
    std::string address = "";
    ConnectState connectState = CONNECTED;
    int32_t ret = audioDeviceCommon.ScoInputDeviceFetchedForRecongnition(handleFlag, address, connectState);
    EXPECT_EQ(SUCCESS, ret);

    handleFlag = false;
    DeviceType deviceType = DEVICE_TYPE_BLUETOOTH_A2DP_IN;
    ret = audioDeviceCommon.ScoInputDeviceFetchedForRecongnition(handleFlag, address, connectState);
    EXPECT_EQ(ERROR, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_065
* @tc.desc  : Test GetSpatialDeviceType interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_065, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::string macAddress = "F0-FA-C7-8C-46-01";
    DeviceType deviceType = audioDeviceCommon.GetSpatialDeviceType(macAddress);
    EXPECT_EQ(DEVICE_TYPE_NONE, deviceType);

    AudioDeviceDescriptor deviceDescriptor;
    deviceDescriptor.macAddress_ = "F0-FA-C7-8C-46-01";
    deviceDescriptor.deviceType_ = DEVICE_TYPE_SPEAKER;
    audioDeviceCommon.OnPreferredOutputDeviceUpdated(deviceDescriptor);
    deviceType = audioDeviceCommon.GetSpatialDeviceType(macAddress);
    EXPECT_EQ(DEVICE_TYPE_SPEAKER, deviceType);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_066
* @tc.desc  : Test GetDeviceDescriptorInner interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_066, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    VolumeUtils::SetPCVolumeEnable(true);
    audioDeviceCommon.isFirstScreenOn_ = false;
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    vector<std::shared_ptr<AudioDeviceDescriptor>> descs =
        audioDeviceCommon.GetDeviceDescriptorInner(rendererChangeInfo);
    EXPECT_NE(0, descs.size());

    audioDeviceCommon.isFirstScreenOn_ = true;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_ULTRASONIC;
    rendererChangeInfo->clientUID = 0;
    descs = audioDeviceCommon.GetDeviceDescriptorInner(rendererChangeInfo);
    EXPECT_NE(0, descs.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_067
* @tc.desc  : Test FetchOutputEnd interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_067, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    bool isUpdateActiveDevice = true;
    int32_t runningStreamCount = 0;
    audioDeviceCommon.FetchOutputEnd(isUpdateActiveDevice, runningStreamCount);
    EXPECT_NE(0, audioDeviceCommon.spatialDeviceMap_.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_068
* @tc.desc  : Test HandleDeviceChangeForFetchOutputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_068, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    desc->deviceType_ = DEVICE_TYPE_NONE;
    int32_t ret = audioDeviceCommon.HandleDeviceChangeForFetchOutputDevice(desc, rendererChangeInfo);
    EXPECT_EQ(ERR_NEED_NOT_SWITCH_DEVICE, ret);

    desc->deviceType_ = DEVICE_TYPE_EARPIECE;
    rendererChangeInfo->outputDeviceInfo.deviceType_ = DEVICE_TYPE_SPEAKER;
    ret = audioDeviceCommon.HandleDeviceChangeForFetchOutputDevice(desc, rendererChangeInfo);
    EXPECT_EQ(SUCCESS, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_069
* @tc.desc  : Test IsSameDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_069, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    AudioDeviceDescriptor deviceInfo;
    desc->deviceType_ = DEVICE_TYPE_NONE;
    deviceInfo.deviceType_ = DEVICE_TYPE_EARPIECE;
    bool ret = audioDeviceCommon.IsSameDevice(desc, deviceInfo);
    EXPECT_EQ(false, ret);

    desc->networkId_ = "";
    deviceInfo.networkId_ = "";
    desc->macAddress_ = "";
    deviceInfo.macAddress_ = "";
    desc->connectState_ = CONNECTED;
    deviceInfo.connectState_ = CONNECTED;
    desc->deviceType_ = DEVICE_TYPE_USB_HEADSET;
    deviceInfo.deviceType_ = DEVICE_TYPE_USB_HEADSET;
    desc->deviceRole_ = DEVICE_ROLE_NONE;
    deviceInfo.deviceRole_ = DEVICE_ROLE_NONE;
    ret = audioDeviceCommon.IsSameDevice(desc, deviceInfo);
    EXPECT_EQ(true, ret);

    desc->deviceType_ = DEVICE_TYPE_USB_ARM_HEADSET;
    deviceInfo.deviceType_ = DEVICE_TYPE_USB_ARM_HEADSET;
    ret = audioDeviceCommon.IsSameDevice(desc, deviceInfo);
    EXPECT_EQ(true, ret);

    BluetoothOffloadState state = A2DP_NOT_OFFLOAD;
    audioDeviceCommon.audioA2dpOffloadFlag_.SetA2dpOffloadFlag(state);
    desc->deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    deviceInfo.a2dpOffloadFlag_ = A2DP_OFFLOAD;
    ret = audioDeviceCommon.IsSameDevice(desc, deviceInfo);
    EXPECT_EQ(false, ret);

    deviceInfo.a2dpOffloadFlag_ = A2DP_NOT_OFFLOAD;
    state = A2DP_OFFLOAD;
    ret = audioDeviceCommon.IsSameDevice(desc, deviceInfo);
    EXPECT_EQ(false, ret);

    desc->deviceType_ = DEVICE_TYPE_SPEAKER;
    deviceInfo.deviceType_ = DEVICE_TYPE_SPEAKER;
    ret = audioDeviceCommon.IsSameDevice(desc, deviceInfo);
    EXPECT_EQ(true, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_070
* @tc.desc  : Test IsSameDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_070, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->deviceType_ = DEVICE_TYPE_NONE;
    AudioDeviceDescriptor deviceDesc;
    deviceDesc.deviceType_ = DEVICE_TYPE_EARPIECE;
    audioDeviceCommon.audioActiveDevice_.SetCurrentOutputDevice(deviceDesc);
    bool ret = audioDeviceCommon.IsSameDevice(desc, deviceDesc);
    EXPECT_EQ(false, ret);

    desc->networkId_ = "";
    deviceDesc.networkId_ = "";
    desc->macAddress_ = "";
    deviceDesc.macAddress_ = "";
    desc->connectState_ = CONNECTED;
    deviceDesc.connectState_ = CONNECTED;
    desc->deviceType_ = DEVICE_TYPE_SPEAKER;
    deviceDesc.deviceType_ = DEVICE_TYPE_SPEAKER;
    ret = audioDeviceCommon.IsSameDevice(desc, deviceDesc);
    EXPECT_EQ(true, ret);

    desc->deviceType_ = DEVICE_TYPE_USB_ARM_HEADSET;
    deviceDesc.deviceType_ = DEVICE_TYPE_USB_ARM_HEADSET;
    desc->deviceRole_ = DEVICE_ROLE_NONE;
    deviceDesc.deviceRole_ = DEVICE_ROLE_NONE;
    ret = audioDeviceCommon.IsSameDevice(desc, deviceDesc);
    EXPECT_EQ(true, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_071
* @tc.desc  : Test UpdateTracker interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_071, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    AudioMode mode = AUDIO_MODE_PLAYBACK;
    AudioStreamChangeInfo streamChangeInfo;
    RendererState rendererState = RENDERER_RELEASED;
    audioDeviceCommon.UpdateTracker(mode, streamChangeInfo, rendererState);
    bool ret = audioDeviceCommon.audioOffloadStream_.GetOffloadAvailableFromXml();
    EXPECT_EQ(false, ret);

    rendererState = RENDERER_STOPPED;
    audioDeviceCommon.UpdateTracker(mode, streamChangeInfo, rendererState);
    ret = audioDeviceCommon.audioOffloadStream_.GetOffloadAvailableFromXml();
    EXPECT_EQ(false, ret);

    rendererState = RENDERER_PAUSED;
    audioDeviceCommon.UpdateTracker(mode, streamChangeInfo, rendererState);
    ret = audioDeviceCommon.audioOffloadStream_.GetOffloadAvailableFromXml();
    EXPECT_EQ(false, ret);

    rendererState = RENDERER_RELEASED;
    audioDeviceCommon.UpdateTracker(mode, streamChangeInfo, rendererState);
    ret = audioDeviceCommon.audioOffloadStream_.GetOffloadAvailableFromXml();
    EXPECT_EQ(false, ret);

    audioDeviceCommon.enableDualHalToneState_ = true;
    rendererState = RENDERER_STOPPED;
    audioDeviceCommon.UpdateTracker(mode, streamChangeInfo, rendererState);
    ret = audioDeviceCommon.audioOffloadStream_.GetOffloadAvailableFromXml();
    EXPECT_EQ(false, ret);

    rendererState = RENDERER_RELEASED;
    audioDeviceCommon.enableDualHalToneSessionId_ = 0;
    streamChangeInfo.audioRendererChangeInfo.sessionId = 0;
    streamChangeInfo.audioRendererChangeInfo.rendererInfo.streamUsage = STREAM_USAGE_ALARM;
    audioDeviceCommon.UpdateTracker(mode, streamChangeInfo, rendererState);
    ret = audioDeviceCommon.audioOffloadStream_.GetOffloadAvailableFromXml();
    EXPECT_EQ(false, ret);

    streamChangeInfo.audioRendererChangeInfo.rendererInfo.streamUsage = STREAM_USAGE_VOICE_RINGTONE;
    audioDeviceCommon.UpdateTracker(mode, streamChangeInfo, rendererState);
    ret = audioDeviceCommon.audioOffloadStream_.GetOffloadAvailableFromXml();
    EXPECT_EQ(false, ret);

    streamChangeInfo.audioRendererChangeInfo.rendererInfo.streamUsage = STREAM_USAGE_RINGTONE;
    audioDeviceCommon.UpdateTracker(mode, streamChangeInfo, rendererState);
    ret = audioDeviceCommon.audioOffloadStream_.GetOffloadAvailableFromXml();
    EXPECT_EQ(false, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_072
* @tc.desc  : Test CheckAndNotifyUserSelectedDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_072, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceCommon.CheckAndNotifyUserSelectedDevice(desc);
    bool ret = audioDeviceCommon.audioActiveDevice_.CheckActiveOutputDeviceSupportOffload();
    EXPECT_EQ(false, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_073
* @tc.desc  : Test ResetOffloadAndMchMode interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_073, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices;
    std::shared_ptr<AudioDeviceDescriptor> outputDevice = std::make_shared<AudioDeviceDescriptor>();
    outputDevice->networkId_ = "";
    outputDevices.push_back(std::move(outputDevice));
    audioDeviceCommon.ResetOffloadAndMchMode(rendererChangeInfo, outputDevices);
    audioDeviceCommon.SetHasDpFlag(true);
    EXPECT_EQ(true, audioDeviceCommon.GetHasDpFlag());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_074
* @tc.desc  : Test ResetOffloadAndMchMode interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_074, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices;
    std::shared_ptr<AudioDeviceDescriptor> outputDevice = std::make_shared<AudioDeviceDescriptor>();
    outputDevice->networkId_ = "LocalDevice";
    outputDevice->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    outputDevices.push_back(std::move(outputDevice));
    audioDeviceCommon.ResetOffloadAndMchMode(rendererChangeInfo, outputDevices);
    audioDeviceCommon.SetHasDpFlag(true);
    EXPECT_EQ(true, audioDeviceCommon.GetHasDpFlag());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_075
* @tc.desc  : Test ResetOffloadAndMchMode interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_075, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices;
    std::shared_ptr<AudioDeviceDescriptor> outputDevice = std::make_shared<AudioDeviceDescriptor>();
    outputDevice->networkId_ = "LocalDevice";
    outputDevice->deviceType_ = DEVICE_TYPE_DP;
    outputDevices.push_back(std::move(outputDevice));
    audioDeviceCommon.ResetOffloadAndMchMode(rendererChangeInfo, outputDevices);
    audioDeviceCommon.SetHasDpFlag(false);
    EXPECT_EQ(false, audioDeviceCommon.GetHasDpFlag());;
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_076
* @tc.desc  : Test JudgeIfLoadMchModule interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_076, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.JudgeIfLoadMchModule();
    audioDeviceCommon.SetHasDpFlag(false);
    EXPECT_EQ(false, audioDeviceCommon.GetHasDpFlag());

    AudioIOHandle moduleId = 0;
    std::string moduleName = "MCH_Speaker";
    audioDeviceCommon.audioIOHandleMap_.AddIOHandleInfo(moduleName, moduleId);
    audioDeviceCommon.JudgeIfLoadMchModule();
    audioDeviceCommon.SetHasDpFlag(true);
    EXPECT_EQ(true, audioDeviceCommon.GetHasDpFlag());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_077
* @tc.desc  : Test FetchStreamForA2dpMchStream interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_077, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    vector<std::shared_ptr<AudioDeviceDescriptor>> descs;
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    descs.push_back(std::move(desc));
    audioDeviceCommon.FetchStreamForA2dpMchStream(rendererChangeInfo, descs);
    audioDeviceCommon.SetHasDpFlag(true);
    EXPECT_EQ(true, audioDeviceCommon.GetHasDpFlag());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_078
* @tc.desc  : Test FetchStreamForSpkMchStream interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_078, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    vector<std::shared_ptr<AudioDeviceDescriptor>> descs;
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    descs.push_back(std::move(desc));
    audioDeviceCommon.FetchStreamForSpkMchStream(rendererChangeInfo, descs);
    audioDeviceCommon.SetHasDpFlag(true);
    EXPECT_EQ(true, audioDeviceCommon.GetHasDpFlag());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_079
* @tc.desc  : Test HandleDeviceChangeForFetchInputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_079, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioCapturerChangeInfo> capturerChangeInfo = std::make_shared<AudioCapturerChangeInfo>();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->deviceType_ = DEVICE_TYPE_NONE;
    int32_t ret = audioDeviceCommon.HandleDeviceChangeForFetchInputDevice(desc, capturerChangeInfo);
    EXPECT_EQ(ERR_NEED_NOT_SWITCH_DEVICE, ret);

    desc->networkId_ = "";
    capturerChangeInfo->inputDeviceInfo.networkId_ = "";
    desc->macAddress_ = "";
    capturerChangeInfo->inputDeviceInfo.macAddress_ = "";
    desc->connectState_ = CONNECTED;
    capturerChangeInfo->inputDeviceInfo.connectState_ = CONNECTED;
    desc->deviceType_ = DEVICE_TYPE_USB_HEADSET;
    capturerChangeInfo->inputDeviceInfo.deviceType_ = DEVICE_TYPE_USB_HEADSET;
    desc->deviceRole_ = DEVICE_ROLE_NONE;
    capturerChangeInfo->inputDeviceInfo.deviceRole_ = DEVICE_ROLE_NONE;
    ret = audioDeviceCommon.HandleDeviceChangeForFetchInputDevice(desc, capturerChangeInfo);
    EXPECT_EQ(ERR_NEED_NOT_SWITCH_DEVICE, ret);

    desc->deviceType_ = DEVICE_TYPE_SPEAKER;
    desc->connectState_ = DEACTIVE_CONNECTED;
    ret = audioDeviceCommon.HandleDeviceChangeForFetchInputDevice(desc, capturerChangeInfo);
    EXPECT_EQ(SUCCESS, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_080
* @tc.desc  : Test MoveToRemoteInputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_080, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    SourceOutput sourceOutput;
    std::vector<SourceOutput> sourceOutputs;
    sourceOutputs.push_back(sourceOutput);
    std::shared_ptr<AudioDeviceDescriptor> remoteDeviceDescriptor = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceCommon.isOpenRemoteDevice = false;
    int32_t ret = audioDeviceCommon.MoveToRemoteInputDevice(sourceOutputs, remoteDeviceDescriptor);
    EXPECT_EQ(ERR_INVALID_OPERATION, ret);

    audioDeviceCommon.isOpenRemoteDevice = true;
    ret = audioDeviceCommon.MoveToRemoteInputDevice(sourceOutputs, remoteDeviceDescriptor);
    EXPECT_EQ(ERR_INVALID_OPERATION, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_081
* @tc.desc  : Test MuteSinkPort interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_081, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::string oldSinkname = "";
    std::string newSinkName = "Offload_Speaker";
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::OVERRODE;
    audioDeviceCommon.MuteSinkPort(oldSinkname, newSinkName, reason);
    audioDeviceCommon.SetHasDpFlag(false);
    bool ret = audioDeviceCommon.GetHasDpFlag();
    EXPECT_EQ(false, ret);

    oldSinkname = "Offload_Speaker";
    newSinkName = "";
    audioDeviceCommon.MuteSinkPort(oldSinkname, newSinkName, reason);
    audioDeviceCommon.SetHasDpFlag(false);
    ret = audioDeviceCommon.GetHasDpFlag();
    EXPECT_EQ(false, ret);

    reason = AudioStreamDeviceChangeReason::NEW_DEVICE_AVAILABLE;
    newSinkName = "Offload_Speaker";
    oldSinkname = "";
    audioDeviceCommon.MuteSinkPort(oldSinkname, newSinkName, reason);
    audioDeviceCommon.SetHasDpFlag(false);
    ret = audioDeviceCommon.GetHasDpFlag();
    EXPECT_EQ(false, ret);

    newSinkName = "";
    oldSinkname = "Offload_Speaker";
    audioDeviceCommon.MuteSinkPort(oldSinkname, newSinkName, reason);
    audioDeviceCommon.SetHasDpFlag(false);
    ret = audioDeviceCommon.GetHasDpFlag();
    EXPECT_EQ(false, ret);

    reason = AudioStreamDeviceChangeReason::OLD_DEVICE_UNAVALIABLE;
    audioDeviceCommon.audioSceneManager_.SetAudioScenePre(AUDIO_SCENE_DEFAULT);
    audioDeviceCommon.MuteSinkPort(oldSinkname, newSinkName, reason);
    audioDeviceCommon.SetHasDpFlag(false);
    ret = audioDeviceCommon.GetHasDpFlag();
    EXPECT_EQ(false, ret);

    audioDeviceCommon.audioSceneManager_.SetAudioScenePre(AUDIO_SCENE_RINGING);
    audioDeviceCommon.audioPolicyManager_.SetRingerMode(RINGER_MODE_SILENT);
    audioDeviceCommon.MuteSinkPort(oldSinkname, newSinkName, reason);
    audioDeviceCommon.SetHasDpFlag(false);
    ret = audioDeviceCommon.GetHasDpFlag();
    EXPECT_EQ(false, ret);

    reason = AudioStreamDeviceChangeReason::UNKNOWN;
    oldSinkname = "RemoteCastInnerCapturer";
    audioDeviceCommon.MuteSinkPort(oldSinkname, newSinkName, reason);
    audioDeviceCommon.SetHasDpFlag(true);
    ret = audioDeviceCommon.GetHasDpFlag();
    EXPECT_EQ(true, ret);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_082
* @tc.desc  : Test MoveToNewOutputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_082, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->outputDeviceInfo.deviceType_ = DEVICE_TYPE_SPEAKER;
    rendererChangeInfo->outputDeviceInfo.macAddress_ = "";
    rendererChangeInfo->outputDeviceInfo.networkId_ = "";
    rendererChangeInfo->outputDeviceInfo.deviceRole_ = INPUT_DEVICE;
    std::shared_ptr<AudioDeviceDescriptor> outputdevice = std::make_shared<AudioDeviceDescriptor>();
    outputdevice->deviceType_ = DEVICE_TYPE_SPEAKER;
    outputdevice->macAddress_ = "";
    outputdevice->networkId_ = "";
    outputdevice->deviceRole_ = INPUT_DEVICE;
    vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices;
    outputDevices.push_back(std::move(outputdevice));
    std::vector<SinkInput> sinkInputs;
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::OVERRODE;
    audioDeviceCommon.audioConfigManager_.OnUpdateRouteSupport(true);
    audioDeviceCommon.MoveToNewOutputDevice(rendererChangeInfo, outputDevices, sinkInputs, reason);
    EXPECT_EQ(true, audioDeviceCommon.audioConfigManager_.GetUpdateRouteSupport());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_083
* @tc.desc  : Test MoveToNewOutputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_083, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->outputDeviceInfo.deviceType_ = DEVICE_TYPE_SPEAKER;
    rendererChangeInfo->outputDeviceInfo.macAddress_ = "";
    rendererChangeInfo->outputDeviceInfo.networkId_ = "";
    rendererChangeInfo->outputDeviceInfo.deviceRole_ = INPUT_DEVICE;
    std::shared_ptr<AudioDeviceDescriptor> outputdevice = std::make_shared<AudioDeviceDescriptor>();
    outputdevice->deviceType_ = DEVICE_TYPE_SPEAKER;
    outputdevice->macAddress_ = "";
    outputdevice->networkId_ = "LocalDevice";
    outputdevice->deviceRole_ = INPUT_DEVICE;
    vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices;
    outputDevices.push_back(std::move(outputdevice));
    std::vector<SinkInput> sinkInputs;
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::OVERRODE;
    audioDeviceCommon.audioConfigManager_.OnUpdateRouteSupport(true);
    audioDeviceCommon.audioPolicyServerHandler_ = std::make_shared<AudioPolicyServerHandler>();
    audioDeviceCommon.MoveToNewOutputDevice(rendererChangeInfo, outputDevices, sinkInputs, reason);
    EXPECT_EQ(true, audioDeviceCommon.audioConfigManager_.GetUpdateRouteSupport());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_084
* @tc.desc  : Test UpdateRoute interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_084, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_MEDIA;

    std::shared_ptr<AudioDeviceDescriptor> outputdevice = std::make_shared<AudioDeviceDescriptor>();
    vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices;
    outputDevices.push_back(std::move(outputdevice));
    audioDeviceCommon.enableDualHalToneState_ = true;
    audioDeviceCommon.UpdateRoute(rendererChangeInfo, outputDevices);
    EXPECT_EQ(0, audioDeviceCommon.enableDualHalToneSessionId_);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_085
* @tc.desc  : Test UpdateRoute interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_085, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_MEDIA;

    std::shared_ptr<AudioDeviceDescriptor> outputdevice = std::make_shared<AudioDeviceDescriptor>();
    vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices;
    outputDevices.push_back(std::move(outputdevice));
    audioDeviceCommon.enableDualHalToneState_ = false;
    audioDeviceCommon.UpdateRoute(rendererChangeInfo, outputDevices);
    EXPECT_EQ(true, audioDeviceCommon.audioVolumeManager_.IsRingerModeMute());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_086
* @tc.desc  : Test UpdateRoute interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_086, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_ALARM;

    std::shared_ptr<AudioDeviceDescriptor> outputdevice = std::make_shared<AudioDeviceDescriptor>();
    outputdevice->deviceType_ = DEVICE_TYPE_WIRED_HEADSET;
    vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices;
    outputDevices.push_back(std::move(outputdevice));
    VolumeUtils::SetPCVolumeEnable(false);
    audioDeviceCommon.UpdateRoute(rendererChangeInfo, outputDevices);
    EXPECT_EQ(true, audioDeviceCommon.shouldUpdateDeviceDueToDualTone_);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_087
* @tc.desc  : Test UpdateRoute interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_087, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_ALARM;

    std::shared_ptr<AudioDeviceDescriptor> outputdevice = std::make_shared<AudioDeviceDescriptor>();
    outputdevice->deviceType_ = DEVICE_TYPE_SPEAKER;
    vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices;
    outputDevices.push_back(std::move(outputdevice));
    VolumeUtils::SetPCVolumeEnable(false);
    audioDeviceCommon.UpdateRoute(rendererChangeInfo, outputDevices);
    EXPECT_EQ(true, audioDeviceCommon.audioVolumeManager_.IsRingerModeMute());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_088
* @tc.desc  : Test UpdateRoute interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_088, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_MEDIA;
    rendererChangeInfo->sessionId = 1;
    vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices;
    outputDevices.push_back(std::make_shared<AudioDeviceDescriptor>());

    audioDeviceCommon.UpdateRoute(rendererChangeInfo, outputDevices);
    EXPECT_EQ(0, audioDeviceCommon.streamsWhenRingDualOnPrimarySpeaker_.size());

    outputDevices.front()->deviceType_ = DEVICE_TYPE_BLUETOOTH_SCO;
    audioDeviceCommon.UpdateRoute(rendererChangeInfo, outputDevices);
    EXPECT_EQ(0, audioDeviceCommon.streamsWhenRingDualOnPrimarySpeaker_.size());

    audioDeviceCommon.isRingDualToneOnPrimarySpeaker_ = true;
    audioDeviceCommon.UpdateRoute(rendererChangeInfo, outputDevices);
    EXPECT_EQ(1, audioDeviceCommon.streamsWhenRingDualOnPrimarySpeaker_.size());

    outputDevices.front()->deviceType_ = DEVICE_TYPE_INVALID;
    audioDeviceCommon.UpdateRoute(rendererChangeInfo, outputDevices);
    EXPECT_EQ(2, audioDeviceCommon.streamsWhenRingDualOnPrimarySpeaker_.size());
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_089
* @tc.desc  : Test IsDualStreamWhenRingDual interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_089, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();

    EXPECT_TRUE(audioDeviceCommon.IsDualStreamWhenRingDual(STREAM_RING));
    EXPECT_TRUE(audioDeviceCommon.IsDualStreamWhenRingDual(STREAM_ALARM));
    EXPECT_TRUE(audioDeviceCommon.IsDualStreamWhenRingDual(STREAM_ACCESSIBILITY));
    EXPECT_FALSE(audioDeviceCommon.IsDualStreamWhenRingDual(STREAM_MUSIC));
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_090
* @tc.desc  : Test IsRingDualToneOnPrimarySpeaker interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_090, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    vector<std::shared_ptr<AudioDeviceDescriptor>> descs;
    EXPECT_FALSE(audioDeviceCommon.IsRingDualToneOnPrimarySpeaker(descs, 1));

    descs.push_back(std::make_shared<AudioDeviceDescriptor>());
    descs.push_back(std::make_shared<AudioDeviceDescriptor>());
    EXPECT_FALSE(audioDeviceCommon.IsRingDualToneOnPrimarySpeaker(descs, 1));

    descs.front()->deviceType_ = DEVICE_TYPE_EARPIECE;
    EXPECT_FALSE(audioDeviceCommon.IsRingDualToneOnPrimarySpeaker(descs, 1));

    descs.back()->deviceType_ = DEVICE_TYPE_EARPIECE;
    EXPECT_FALSE(audioDeviceCommon.IsRingDualToneOnPrimarySpeaker(descs, 1));

    descs.front()->deviceType_ = DEVICE_TYPE_BLUETOOTH_SCO;
    EXPECT_FALSE(audioDeviceCommon.IsRingDualToneOnPrimarySpeaker(descs, 1));

    descs.back()->deviceType_ = DEVICE_TYPE_SPEAKER;
    EXPECT_TRUE(audioDeviceCommon.IsRingDualToneOnPrimarySpeaker(descs, 1));
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_091
* @tc.desc  : Test IsStopOrReleasePlayback interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_091, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    AudioMode mode = AUDIO_MODE_RECORD;
    RendererState state  = RENDERER_RUNNING;
    EXPECT_FALSE(audioDeviceCommon.IsStopOrReleasePlayback(mode, state));

    mode = AUDIO_MODE_PLAYBACK;
    EXPECT_FALSE(audioDeviceCommon.IsStopOrReleasePlayback(mode, state));

    state = RENDERER_STOPPED;
    EXPECT_TRUE(audioDeviceCommon.IsStopOrReleasePlayback(mode, state));

    state = RENDERER_RELEASED;
    EXPECT_TRUE(audioDeviceCommon.IsStopOrReleasePlayback(mode, state));
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: AudioDeviceCommon_092
* @tc.desc  : Test UpdateTracker interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, AudioDeviceCommon_092, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    AudioMode mode = AUDIO_MODE_RECORD;
    AudioStreamChangeInfo streamChangeInfo;
    streamChangeInfo.audioRendererChangeInfo.rendererInfo.streamUsage = STREAM_USAGE_INVALID;
    RendererState state  = RENDERER_STOPPED;
    audioDeviceCommon.isRingDualToneOnPrimarySpeaker_ = false;
    audioDeviceCommon.UpdateTracker(mode, streamChangeInfo, state);
    EXPECT_FALSE(audioDeviceCommon.isRingDualToneOnPrimarySpeaker_);

    streamChangeInfo.audioRendererChangeInfo.rendererInfo.streamUsage = STREAM_USAGE_VOICE_RINGTONE;
    audioDeviceCommon.UpdateTracker(mode, streamChangeInfo, state);
    EXPECT_FALSE(audioDeviceCommon.isRingDualToneOnPrimarySpeaker_);

    streamChangeInfo.audioRendererChangeInfo.rendererInfo.streamUsage = STREAM_USAGE_RINGTONE;
    audioDeviceCommon.UpdateTracker(mode, streamChangeInfo, state);
    EXPECT_FALSE(audioDeviceCommon.isRingDualToneOnPrimarySpeaker_);

    mode = AUDIO_MODE_PLAYBACK;
    streamChangeInfo.audioRendererChangeInfo.rendererInfo.streamUsage = STREAM_USAGE_INVALID;
    audioDeviceCommon.UpdateTracker(mode, streamChangeInfo, state);
    EXPECT_FALSE(audioDeviceCommon.isRingDualToneOnPrimarySpeaker_);

    streamChangeInfo.audioRendererChangeInfo.rendererInfo.streamUsage = STREAM_USAGE_VOICE_RINGTONE;
    audioDeviceCommon.UpdateTracker(mode, streamChangeInfo, state);
    EXPECT_FALSE(audioDeviceCommon.isRingDualToneOnPrimarySpeaker_);

    streamChangeInfo.audioRendererChangeInfo.rendererInfo.streamUsage = STREAM_USAGE_RINGTONE;
    audioDeviceCommon.UpdateTracker(mode, streamChangeInfo, state);
    EXPECT_FALSE(audioDeviceCommon.isRingDualToneOnPrimarySpeaker_);

    audioDeviceCommon.isRingDualToneOnPrimarySpeaker_ = true;
    audioDeviceCommon.UpdateTracker(mode, streamChangeInfo, state);
    EXPECT_FALSE(audioDeviceCommon.isRingDualToneOnPrimarySpeaker_);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: OnAudioSceneChange_001
* @tc.desc  : Test OnAudioSceneChange interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, OnAudioSceneChange_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioPolicyServerHandler> testHandler = std::make_shared<AudioPolicyServerHandler>();
    audioDeviceCommon.audioPolicyServerHandler_ = testHandler;
    AudioScene testScene = AUDIO_SCENE_RINGING;
    audioDeviceCommon.OnAudioSceneChange(testScene);
    EXPECT_NE(audioDeviceCommon.audioPolicyServerHandler_, nullptr);
}

/**
* @tc.name  : Test AudioDeviceCommon.
* @tc.number: OnAudioSceneChange_002
* @tc.desc  : Test OnAudioSceneChange interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, OnAudioSceneChange_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();
    audioDeviceCommon.audioPolicyServerHandler_ = nullptr;
    AudioScene testScene = AUDIO_SCENE_RINGING;
    audioDeviceCommon.OnAudioSceneChange(testScene);
    EXPECT_EQ(audioDeviceCommon.audioPolicyServerHandler_, nullptr);
}

/**
* @tc.name  : Test GetPreferredInputDeviceDescInner.
* @tc.number: GetPreferredInputDeviceDescInner_001
* @tc.desc  : Test GetPreferredInputDeviceDescInner interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, GetPreferredInputDeviceDescInner_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();
    AudioCapturerInfo captureInfo;
    captureInfo.sourceType = SOURCE_TYPE_PLAYBACK_CAPTURE;
    std::string networkId = LOCAL_NETWORK_ID;
    audioDeviceCommon.audioRouterCenter_.FetchInputDevice(captureInfo.sourceType, -1)->deviceType_ = DEVICE_TYPE_NONE;

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> deviceList =
        audioDeviceCommon.GetPreferredInputDeviceDescInner(captureInfo, networkId);
    EXPECT_NE(deviceList.size(), 0);
}

/**
* @tc.name  : Test GetPreferredInputDeviceDescInner.
* @tc.number: GetPreferredInputDeviceDescInner_002
* @tc.desc  : Test GetPreferredInputDeviceDescInner interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, GetPreferredInputDeviceDescInner_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();
    AudioCapturerInfo captureInfo;
    captureInfo.sourceType = SOURCE_TYPE_PLAYBACK_CAPTURE;
    std::string networkId = REMOTE_NETWORK_ID;

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> deviceList =
        audioDeviceCommon.GetPreferredInputDeviceDescInner(captureInfo, networkId);
    EXPECT_EQ(deviceList.size(), 0);
}

/**
* @tc.name  : Test GetPreferredInputDeviceDescInner.
* @tc.number: GetPreferredInputDeviceDescInner_003
* @tc.desc  : Test GetPreferredInputDeviceDescInner interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, GetPreferredInputDeviceDescInner_003, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();
    AudioCapturerInfo captureInfo;
    captureInfo.sourceType = SOURCE_TYPE_REMOTE_CAST;
    std::string networkId = LOCAL_NETWORK_ID;
    audioDeviceCommon.audioRouterCenter_.FetchInputDevice(captureInfo.sourceType, -1)->deviceType_ = DEVICE_TYPE_NONE;

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> deviceList =
        audioDeviceCommon.GetPreferredInputDeviceDescInner(captureInfo, networkId);
    EXPECT_NE(deviceList.size(), 0);
}

/**
* @tc.name  : Test UpdateDeviceInfo.
* @tc.number: UpdateDeviceInfo_001
* @tc.desc  : Test UpdateDeviceInfo interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, UpdateDeviceInfo_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();
    AudioDeviceDescriptor deviceInfo;
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();

    desc->deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    bool hasBTPermission = true;
    bool hasSystemPermission = true;
    audioDeviceCommon.UpdateDeviceInfo(deviceInfo, desc, hasBTPermission, hasSystemPermission);
    EXPECT_EQ(deviceInfo.deviceType_, DEVICE_TYPE_BLUETOOTH_A2DP);
    EXPECT_EQ(deviceInfo.a2dpOffloadFlag_, audioDeviceCommon.audioA2dpOffloadFlag_.GetA2dpOffloadFlag());
}

/**
* @tc.name  : Test UpdateDeviceInfo.
* @tc.number: UpdateDeviceInfo_002
* @tc.desc  : Test UpdateDeviceInfo interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, UpdateDeviceInfo_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();
    AudioDeviceDescriptor deviceInfo;
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();

    bool hasBTPermission = false;
    bool hasSystemPermission = false;
    audioDeviceCommon.UpdateDeviceInfo(deviceInfo, desc, hasBTPermission, hasSystemPermission);
    EXPECT_EQ(deviceInfo.deviceName_, "");
    EXPECT_EQ(deviceInfo.networkId_, "");
}

/**
* @tc.name  : Test UpdateConnectedDevicesWhenDisconnecting.
* @tc.number: UpdateConnectedDevicesWhenDisconnecting_001
* @tc.desc  : Test UpdateDeviceInfo interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, UpdateConnectedDevicesWhenDisconnecting_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();
    AudioDeviceDescriptor updatedDesc;
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->deviceType_ = DEVICE_TYPE_DP;

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> descForCb;
    descForCb.push_back(desc);

    std::shared_ptr<AudioDeviceDescriptor> preferredMediaRenderDevice = std::make_shared<AudioDeviceDescriptor>();
    preferredMediaRenderDevice->deviceType_ = desc->deviceType_;
    preferredMediaRenderDevice->macAddress_ = desc->macAddress_;
    preferredMediaRenderDevice->deviceRole_ = desc->deviceRole_;
    preferredMediaRenderDevice->networkId_ = desc->networkId_;
    audioDeviceCommon.audioStateManager_.SetPreferredMediaRenderDevice(preferredMediaRenderDevice);

    audioDeviceCommon.UpdateConnectedDevicesWhenDisconnecting(updatedDesc, descForCb);
    EXPECT_EQ(desc->deviceType_, DEVICE_TYPE_DP);
}

/**
* @tc.name  : Test UpdateConnectedDevicesWhenDisconnecting.
* @tc.number: UpdateConnectedDevicesWhenDisconnecting_002
* @tc.desc  : Test UpdateDeviceInfo interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, UpdateConnectedDevicesWhenDisconnecting_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();
    AudioDeviceDescriptor updatedDesc;
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->deviceType_ = DEVICE_TYPE_USB_HEADSET;

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> descForCb;
    descForCb.push_back(desc);

    std::shared_ptr<AudioDeviceDescriptor> preferredCallCaptureDevice = std::make_shared<AudioDeviceDescriptor>();
    preferredCallCaptureDevice->deviceType_ = desc->deviceType_;
    preferredCallCaptureDevice->macAddress_ = desc->macAddress_;
    preferredCallCaptureDevice->deviceRole_ = desc->deviceRole_;
    preferredCallCaptureDevice->networkId_ = desc->networkId_;
    audioDeviceCommon.audioStateManager_.SetPreferredCallCaptureDevice(preferredCallCaptureDevice);

    audioDeviceCommon.UpdateConnectedDevicesWhenDisconnecting(updatedDesc, descForCb);
    EXPECT_EQ(desc->deviceType_, DEVICE_TYPE_USB_HEADSET);
}

/**
* @tc.name  : Test UpdateConnectedDevicesWhenDisconnecting.
* @tc.number: UpdateConnectedDevicesWhenDisconnecting_003
* @tc.desc  : Test UpdateDeviceInfo interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, UpdateConnectedDevicesWhenDisconnecting_003, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();
    AudioDeviceDescriptor updatedDesc;
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->deviceType_ = DEVICE_TYPE_USB_ARM_HEADSET;

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> descForCb;
    descForCb.push_back(desc);

    std::shared_ptr<AudioDeviceDescriptor> preferredRecordCaptureDevice = std::make_shared<AudioDeviceDescriptor>();
    preferredRecordCaptureDevice->deviceType_ = desc->deviceType_;
    preferredRecordCaptureDevice->macAddress_ = desc->macAddress_;
    preferredRecordCaptureDevice->deviceRole_ = desc->deviceRole_;
    preferredRecordCaptureDevice->networkId_ = desc->networkId_;
    audioDeviceCommon.audioStateManager_.SetPreferredRecordCaptureDevice(preferredRecordCaptureDevice);

    audioDeviceCommon.UpdateConnectedDevicesWhenDisconnecting(updatedDesc, descForCb);
    EXPECT_EQ(desc->deviceType_, DEVICE_TYPE_USB_ARM_HEADSET);
}

/**
* @tc.name  : Test NotifyRecreateDirectStream.
* @tc.number: NotifyRecreateDirectStream_001
* @tc.desc  : Test NotifyRecreateDirectStream interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, NotifyRecreateDirectStream_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    rendererChangeInfo->rendererInfo.pipeType = PIPE_TYPE_DIRECT_MUSIC;

    audioDeviceCommon.audioActiveDevice_.SetCurrentOutputDeviceType(DEVICE_TYPE_USB_HEADSET);
    bool ret = audioDeviceCommon.NotifyRecreateDirectStream(rendererChangeInfo, reason);
    EXPECT_EQ(false, ret);
}

/**
* @tc.name  : Test NotifyRecreateDirectStream.
* @tc.number: NotifyRecreateDirectStream_002
* @tc.desc  : Test NotifyRecreateDirectStream interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, NotifyRecreateDirectStream_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    rendererChangeInfo->rendererInfo.pipeType = PIPE_TYPE_SPATIALIZATION;
    rendererChangeInfo->rendererInfo.streamUsage == STREAM_USAGE_MUSIC;
    audioDeviceCommon.audioActiveDevice_.SetCurrentOutputDeviceType(DEVICE_TYPE_USB_HEADSET);
    bool ret = audioDeviceCommon.NotifyRecreateDirectStream(rendererChangeInfo, reason);
    EXPECT_EQ(false, ret);

    rendererChangeInfo->rendererInfo.rendererFlags == AUDIO_FLAG_NORMAL;
    rendererChangeInfo->rendererInfo.samplingRate == SAMPLE_RATE_44100;
    ret = audioDeviceCommon.NotifyRecreateDirectStream(rendererChangeInfo, reason);
    EXPECT_EQ(false, ret);

    rendererChangeInfo->rendererInfo.samplingRate == SAMPLE_RATE_48000;
    rendererChangeInfo->rendererInfo.format == SAMPLE_S16LE;
    ret = audioDeviceCommon.NotifyRecreateDirectStream(rendererChangeInfo, reason);
    EXPECT_EQ(false, ret);
}

/**
* @tc.name  : Test MuteSinkForSwitchDistributedDevice.
* @tc.number: MuteSinkForSwitchDistributedDevice_001
* @tc.desc  : Test MuteSinkForSwitchDistributedDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, MuteSinkForSwitchDistributedDevice_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices;
    std::shared_ptr<AudioDeviceDescriptor> deviceDesc = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc->deviceType_ = DEVICE_TYPE_SPEAKER;
    outputDevices.push_back(deviceDesc);
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReasonExt::ExtEnum::DISTRIBUTED_DEVICE;
    audioDeviceCommon.MuteSinkForSwitchDistributedDevice(rendererChangeInfo, outputDevices, reason);
    EXPECT_NE(outputDevices.front(), nullptr);
}

/**
* @tc.name  : Test MuteSinkForSwitchDistributedDevice.
* @tc.number: MuteSinkForSwitchDistributedDevice_002
* @tc.desc  : Test MuteSinkForSwitchDistributedDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, MuteSinkForSwitchDistributedDevice_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices;
    std::shared_ptr<AudioDeviceDescriptor> deviceDesc = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc->deviceType_ = DEVICE_TYPE_EARPIECE;
    outputDevices.push_back(deviceDesc);
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReasonExt::ExtEnum::DISTRIBUTED_DEVICE;
    audioDeviceCommon.MuteSinkForSwitchDistributedDevice(rendererChangeInfo, outputDevices, reason);
    EXPECT_NE(outputDevices.front(), nullptr);
}

/**
* @tc.name  : Test MuteSinkForSwitchDistributedDevice.
* @tc.number: MuteSinkForSwitchDistributedDevice_003
* @tc.desc  : Test MuteSinkForSwitchDistributedDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, MuteSinkForSwitchDistributedDevice_003, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices;
    std::shared_ptr<AudioDeviceDescriptor> deviceDesc = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc->deviceType_ = DEVICE_TYPE_EARPIECE;
    outputDevices.push_back(deviceDesc);
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReasonExt::ExtEnum::UNKNOWN;
    audioDeviceCommon.MuteSinkForSwitchDistributedDevice(rendererChangeInfo, outputDevices, reason);
    EXPECT_NE(outputDevices.front(), nullptr);
}

/**
* @tc.name  : Test SetDeviceConnectedFlagWhenFetchOutputDevice.
* @tc.number: SetDeviceConnectedFlagWhenFetchOutputDevice_001
* @tc.desc  : Test SetDeviceConnectedFlagWhenFetchOutputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, SetDeviceConnectedFlagWhenFetchOutputDevice_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    AudioDeviceDescriptor deviceDescriptor;
    deviceDescriptor.deviceType_ = DEVICE_TYPE_USB_HEADSET;
    audioDeviceCommon.audioActiveDevice_.SetCurrentOutputDevice(deviceDescriptor);
    audioDeviceCommon.SetDeviceConnectedFlagWhenFetchOutputDevice();
    EXPECT_EQ(deviceDescriptor.deviceType_, DEVICE_TYPE_USB_HEADSET);
}

/**
* @tc.name  : Test SetDeviceConnectedFlagWhenFetchOutputDevice.
* @tc.number: SetDeviceConnectedFlagWhenFetchOutputDevice_002
* @tc.desc  : Test SetDeviceConnectedFlagWhenFetchOutputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, SetDeviceConnectedFlagWhenFetchOutputDevice_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    AudioDeviceDescriptor deviceDescriptor;
    deviceDescriptor.deviceType_ = DEVICE_TYPE_USB_ARM_HEADSET;
    audioDeviceCommon.audioActiveDevice_.SetCurrentOutputDevice(deviceDescriptor);
    audioDeviceCommon.SetDeviceConnectedFlagWhenFetchOutputDevice();
    EXPECT_EQ(deviceDescriptor.deviceType_, DEVICE_TYPE_USB_ARM_HEADSET);
}

/**
* @tc.name  : Test SetDeviceConnectedFlagWhenFetchOutputDevice.
* @tc.number: SetDeviceConnectedFlagWhenFetchOutputDevice_003
* @tc.desc  : Test SetDeviceConnectedFlagWhenFetchOutputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, SetDeviceConnectedFlagWhenFetchOutputDevice_003, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    AudioDeviceDescriptor deviceDescriptor;
    deviceDescriptor.deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP_IN;
    audioDeviceCommon.audioActiveDevice_.SetCurrentOutputDevice(deviceDescriptor);
    audioDeviceCommon.SetDeviceConnectedFlagWhenFetchOutputDevice();
    EXPECT_EQ(deviceDescriptor.deviceType_, DEVICE_TYPE_BLUETOOTH_A2DP_IN);
}

/**
* @tc.name  : Test FetchOutputDevice
* @tc.number: FetchOutputDevice_001
* @tc.desc  : Test FetchOutputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, FetchOutputDevice_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();
    std::vector<std::shared_ptr<AudioRendererChangeInfo>> rendererChangeInfos;
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfos.push_back(rendererChangeInfo);
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_VOICE_MODEM_COMMUNICATION;
    audioDeviceCommon.audioSceneManager_.audioScene_ = AUDIO_SCENE_PHONE_CALL;

    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReasonExt::ExtEnum::UNKNOWN;
    audioDeviceCommon.FetchOutputDevice(rendererChangeInfos, reason);
    EXPECT_NE(rendererChangeInfo, nullptr);
}

/**
* @tc.name  : Test HandleDeviceChangeForFetchOutputDevice
* @tc.number: HandleDeviceChangeForFetchOutputDevice_001
* @tc.desc  : Test HandleDeviceChangeForFetchOutputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, HandleDeviceChangeForFetchOutputDevice_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->deviceType_ = DEVICE_TYPE_SPEAKER;
    desc->connectState_ = CONNECTED;
    desc->descriptorType_ = AudioDeviceDescriptor::AUDIO_DEVICE_DESCRIPTOR;

    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->outputDeviceInfo = AudioDeviceDescriptor(*desc);

    audioDeviceCommon.shouldUpdateDeviceDueToDualTone_ = false;
    audioDeviceCommon.audioSceneManager_.audioScene_ = AUDIO_SCENE_DEFAULT;
    audioDeviceCommon.audioActiveDevice_.currentActiveDevice_ = AudioDeviceDescriptor(*desc);

    int32_t result = audioDeviceCommon.HandleDeviceChangeForFetchOutputDevice(desc, rendererChangeInfo);
    EXPECT_EQ(result, ERR_NEED_NOT_SWITCH_DEVICE);
}

/**
* @tc.name  : Test HandleDeviceChangeForFetchOutputDevice.
* @tc.number: HandleDeviceChangeForFetchOutputDevice_002
* @tc.desc  : Test HandleDeviceChangeForFetchOutputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, HandleDeviceChangeForFetchOutputDevice_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->deviceType_ = DEVICE_TYPE_NONE;
    desc->connectState_ = CONNECTED;

    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->clientUID = 12345;
    audioDeviceCommon.shouldUpdateDeviceDueToDualTone_ = false;
    audioDeviceCommon.audioSceneManager_.audioScene_ = AUDIO_SCENE_DEFAULT;

    AudioDeviceDescriptor tmpOutputDeviceDesc;
    tmpOutputDeviceDesc.deviceType_ = DEVICE_TYPE_SPEAKER;
    audioDeviceCommon.audioActiveDevice_.currentActiveDevice_ = tmpOutputDeviceDesc;

    std::shared_ptr<AudioDeviceDescriptor> preferredDesc = std::make_shared<AudioDeviceDescriptor>();
    preferredDesc->deviceType_ = DEVICE_TYPE_SPEAKER;
    audioDeviceCommon.audioAffinityManager_.activeRendererDeviceMap_[rendererChangeInfo->clientUID] = preferredDesc;

    int32_t result = audioDeviceCommon.HandleDeviceChangeForFetchOutputDevice(desc, rendererChangeInfo);
    EXPECT_EQ(result, ERR_NEED_NOT_SWITCH_DEVICE);
}

/**
* @tc.name  : Test MuteSinkPortForSwitchDevice
* @tc.number: MuteSinkPortForSwitchDevice_001
* @tc.desc  : Test MuteSinkPortForSwitchDevice.
*/
HWTEST_F(AudioDeviceCommonUnitTest, MuteSinkPortForSwitchDevice_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->outputDeviceInfo.deviceType_ = DEVICE_TYPE_SPEAKER;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_VOICE_RINGTONE;
    rendererChangeInfo->sessionId = 12345;

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices;
    std::shared_ptr<AudioDeviceDescriptor> deviceDesc = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc->deviceType_ = DEVICE_TYPE_EARPIECE;
    outputDevices.push_back(deviceDesc);

    audioDeviceCommon.audioSceneManager_.audioScene_ = AUDIO_SCENE_PHONE_CALL;
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReasonExt::ExtEnum::UNKNOWN;
    audioDeviceCommon.MuteSinkPortForSwitchDevice(rendererChangeInfo, outputDevices, reason);
    EXPECT_NE(rendererChangeInfo->outputDeviceInfo.deviceType_, deviceDesc->deviceType_);
}

/**
* @tc.name  : Test MuteSinkPortForSwitchDevice
* @tc.number: MuteSinkPortForSwitchDevice_002
* @tc.desc  : Test MuteSinkPortForSwitchDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, MuteSinkPortForSwitchDevice_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->outputDeviceInfo.deviceType_ = DEVICE_TYPE_SPEAKER;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_VOICE_MODEM_COMMUNICATION;
    rendererChangeInfo->sessionId = 12345;

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices;
    std::shared_ptr<AudioDeviceDescriptor> deviceDesc = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc->deviceType_ = DEVICE_TYPE_EARPIECE;
    outputDevices.push_back(deviceDesc);

    audioDeviceCommon.audioSceneManager_.audioScene_ = AUDIO_SCENE_PHONE_CALL;
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReasonExt::ExtEnum::UNKNOWN;
    audioDeviceCommon.MuteSinkPortForSwitchDevice(rendererChangeInfo, outputDevices, reason);
    EXPECT_NE(rendererChangeInfo->outputDeviceInfo.deviceType_, deviceDesc->deviceType_);
}

/**
* @tc.name  : Test MuteSinkPortForSwitchDevice
* @tc.number: MuteSinkPortForSwitchDevice_003
* @tc.desc  : Test MuteSinkPortForSwitchDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, MuteSinkPortForSwitchDevice_003, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->outputDeviceInfo.deviceType_ = DEVICE_TYPE_SPEAKER;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_VOICE_MODEM_COMMUNICATION;
    rendererChangeInfo->sessionId = 12345;

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices;
    std::shared_ptr<AudioDeviceDescriptor> deviceDesc = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc->deviceType_ = DEVICE_TYPE_EARPIECE;
    outputDevices.push_back(deviceDesc);

    audioDeviceCommon.audioSceneManager_.audioScene_ = AUDIO_SCENE_PHONE_CALL;

    audioDeviceCommon.streamCollector_.audioRendererChangeInfos_.clear();
    std::shared_ptr<AudioRendererChangeInfo> changeInfo = std::make_shared<AudioRendererChangeInfo>();
    changeInfo->rendererState = RENDERER_PREPARED;
    changeInfo->rendererInfo.streamUsage = STREAM_USAGE_VOICE_MODEM_COMMUNICATION;
    changeInfo->sessionId = 12345;
    audioDeviceCommon.streamCollector_.audioRendererChangeInfos_.push_back(changeInfo);

    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReasonExt::ExtEnum::UNKNOWN;
    audioDeviceCommon.MuteSinkPortForSwitchDevice(rendererChangeInfo, outputDevices, reason);
    EXPECT_NE(rendererChangeInfo->outputDeviceInfo.deviceType_, deviceDesc->deviceType_);
}

/**
* @tc.name  : Test SelectRingerOrAlarmDevices
* @tc.number: SelectRingerOrAlarmDevices_001
* @tc.desc  : Test SelectRingerOrAlarmDevices interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, SelectRingerOrAlarmDevices_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->sessionId = 12345;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_RINGTONE;

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> descs;
    std::shared_ptr<AudioDeviceDescriptor> deviceDesc1 = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc1->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    deviceDesc1->networkId_ = "LocalNetworkId1";
    deviceDesc1->deviceRole_ = DeviceRole::OUTPUT_DEVICE;
    descs.push_back(deviceDesc1);

    std::shared_ptr<AudioDeviceDescriptor> deviceDesc2 = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc2->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    deviceDesc2->networkId_ = "LocalNetworkId2";
    deviceDesc1->deviceRole_ = DeviceRole::INPUT_DEVICE;
    descs.push_back(deviceDesc2);

    audioDeviceCommon.enableDualHalToneState_ = true;
    audioDeviceCommon.enableDualHalToneSessionId_ = 54321;
    audioDeviceCommon.audioPolicyManager_.SetRingerMode(RINGER_MODE_SILENT);

    bool result = audioDeviceCommon.SelectRingerOrAlarmDevices(descs, rendererChangeInfo);
    EXPECT_FALSE(result);
}

/**
* @tc.name  : Test SelectRingerOrAlarmDevices
* @tc.number: SelectRingerOrAlarmDevices_002
* @tc.desc  : Test SelectRingerOrAlarmDevices interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, SelectRingerOrAlarmDevices_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->sessionId = 12345;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_RINGTONE;

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> descs;
    std::shared_ptr<AudioDeviceDescriptor> deviceDesc1 = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc1->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    deviceDesc1->networkId_ = "LocalNetworkId1";
    deviceDesc1->deviceRole_ = DeviceRole::OUTPUT_DEVICE;
    descs.push_back(deviceDesc1);

    std::shared_ptr<AudioDeviceDescriptor> deviceDesc2 = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc2->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    deviceDesc2->networkId_ = "LocalNetworkId2";
    deviceDesc1->deviceRole_ = DeviceRole::INPUT_DEVICE;
    descs.push_back(deviceDesc2);

    audioDeviceCommon.enableDualHalToneState_ = false;
    audioDeviceCommon.enableDualHalToneSessionId_ = 54321;
    audioDeviceCommon.audioPolicyManager_.SetRingerMode(RINGER_MODE_NORMAL);

    bool result = audioDeviceCommon.SelectRingerOrAlarmDevices(descs, rendererChangeInfo);
    EXPECT_TRUE(result);
}

/**
* @tc.name  : Test SelectRingerOrAlarmDevices
* @tc.number: SelectRingerOrAlarmDevices_003
* @tc.desc  : Test SelectRingerOrAlarmDevices interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, SelectRingerOrAlarmDevices_003, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->sessionId = 12345;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_ALARM;

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> descs;
    std::shared_ptr<AudioDeviceDescriptor> deviceDesc1 = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc1->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    deviceDesc1->networkId_ = "LocalNetworkId1";
    deviceDesc1->deviceRole_ = DeviceRole::OUTPUT_DEVICE;
    descs.push_back(deviceDesc1);

    std::shared_ptr<AudioDeviceDescriptor> deviceDesc2 = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc2->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    deviceDesc2->networkId_ = "LocalNetworkId2";
    deviceDesc1->deviceRole_ = DeviceRole::INPUT_DEVICE;
    descs.push_back(deviceDesc2);

    audioDeviceCommon.enableDualHalToneState_ = true;
    audioDeviceCommon.enableDualHalToneSessionId_ = 12345;
    audioDeviceCommon.audioPolicyManager_.SetRingerMode(RINGER_MODE_SILENT);

    bool result = audioDeviceCommon.SelectRingerOrAlarmDevices(descs, rendererChangeInfo);
    EXPECT_TRUE(result);
}

/**
* @tc.name  : Test SelectRingerOrAlarmDevices
* @tc.number: SelectRingerOrAlarmDevices_004
* @tc.desc  : Test SelectRingerOrAlarmDevices interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, SelectRingerOrAlarmDevices_004, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->sessionId = 12345;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_ALARM;

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> descs;
    std::shared_ptr<AudioDeviceDescriptor> deviceDesc1 = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc1->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    deviceDesc1->networkId_ = "LocalNetworkId1";
    deviceDesc1->deviceRole_ = DeviceRole::OUTPUT_DEVICE;
    descs.push_back(deviceDesc1);

    std::shared_ptr<AudioDeviceDescriptor> deviceDesc2 = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc2->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    deviceDesc2->networkId_ = "LocalNetworkId1";
    deviceDesc1->deviceRole_ = DeviceRole::OUTPUT_DEVICE;
    descs.push_back(deviceDesc2);

    audioDeviceCommon.enableDualHalToneState_ = true;
    audioDeviceCommon.enableDualHalToneSessionId_ = 12345;
    audioDeviceCommon.audioPolicyManager_.SetRingerMode(RINGER_MODE_SILENT);

    bool result = audioDeviceCommon.SelectRingerOrAlarmDevices(descs, rendererChangeInfo);
    EXPECT_TRUE(result);
}

/**
* @tc.name  : Test SelectRingerOrAlarmDevices
* @tc.number: SelectRingerOrAlarmDevices_005
* @tc.desc  : Test SelectRingerOrAlarmDevices interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, SelectRingerOrAlarmDevices_005, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->sessionId = 12345;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_ALARM;

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> descs;
    std::shared_ptr<AudioDeviceDescriptor> deviceDesc1 = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc1->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    deviceDesc1->networkId_ = "LocalNetworkId1";
    deviceDesc1->deviceRole_ = DeviceRole::OUTPUT_DEVICE;
    descs.push_back(deviceDesc1);

    std::shared_ptr<AudioDeviceDescriptor> deviceDesc2 = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc2->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    deviceDesc2->networkId_ = "LocalNetworkId1";
    deviceDesc1->deviceRole_ = DeviceRole::OUTPUT_DEVICE;
    descs.push_back(deviceDesc2);

    audioDeviceCommon.enableDualHalToneState_ = false;
    audioDeviceCommon.enableDualHalToneSessionId_ = 12345;
    audioDeviceCommon.audioPolicyManager_.SetRingerMode(RINGER_MODE_SILENT);

    bool result = audioDeviceCommon.SelectRingerOrAlarmDevices(descs, rendererChangeInfo);
    EXPECT_TRUE(result);
}

/**
* @tc.name  : Test SelectRingerOrAlarmDevices
* @tc.number: SelectRingerOrAlarmDevices_006
* @tc.desc  : Test SelectRingerOrAlarmDevices interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, SelectRingerOrAlarmDevices_006, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->sessionId = 12345;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_ALARM;

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> descs;
    std::shared_ptr<AudioDeviceDescriptor> deviceDesc1 = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc1->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    deviceDesc1->networkId_ = "LocalNetworkId1";
    deviceDesc1->deviceRole_ = DeviceRole::OUTPUT_DEVICE;
    descs.push_back(deviceDesc1);

    std::shared_ptr<AudioDeviceDescriptor> deviceDesc2 = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc2->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    deviceDesc2->networkId_ = "LocalNetworkId1";
    deviceDesc1->deviceRole_ = DeviceRole::OUTPUT_DEVICE;
    descs.push_back(deviceDesc2);

    audioDeviceCommon.enableDualHalToneState_ = true;
    audioDeviceCommon.enableDualHalToneSessionId_ = 54321;
    audioDeviceCommon.audioPolicyManager_.SetRingerMode(RINGER_MODE_SILENT);

    bool result = audioDeviceCommon.SelectRingerOrAlarmDevices(descs, rendererChangeInfo);
    EXPECT_TRUE(result);
}

/**
* @tc.name  : Test SelectRingerOrAlarmDevices
* @tc.number: SelectRingerOrAlarmDevices_007
* @tc.desc  : Test SelectRingerOrAlarmDevices interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, SelectRingerOrAlarmDevices_007, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->sessionId = 12345;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_ALARM;

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> descs;
    std::shared_ptr<AudioDeviceDescriptor> deviceDesc1 = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc1->deviceType_ = DEVICE_TYPE_LINE_DIGITAL;
    deviceDesc1->networkId_ = "LocalNetworkId1";
    deviceDesc1->deviceRole_ = DeviceRole::OUTPUT_DEVICE;
    descs.push_back(deviceDesc1);

    std::shared_ptr<AudioDeviceDescriptor> deviceDesc2 = std::make_shared<AudioDeviceDescriptor>();
    deviceDesc2->deviceType_ = DEVICE_TYPE_LINE_DIGITAL;
    deviceDesc2->networkId_ = "LocalNetworkId1";
    deviceDesc1->deviceRole_ = DeviceRole::OUTPUT_DEVICE;
    descs.push_back(deviceDesc2);

    bool result = audioDeviceCommon.SelectRingerOrAlarmDevices(descs, rendererChangeInfo);
    EXPECT_FALSE(result);
}

/**
* @tc.name  : Test SelectRingerOrAlarmDevices
* @tc.number: SelectRingerOrAlarmDevices_008
* @tc.desc  : Test SelectRingerOrAlarmDevices interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, SelectRingerOrAlarmDevices_008, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->sessionId = 12345;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_ALARM;
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> descs;

    bool result = audioDeviceCommon.SelectRingerOrAlarmDevices(descs, rendererChangeInfo);
    EXPECT_FALSE(result);
}

/**
* @tc.name  : Test HandleDeviceChangeForFetchInputDevice
* @tc.number: HandleDeviceChangeForFetchInputDevice_001
* @tc.desc  : Test HandleDeviceChangeForFetchInputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, HandleDeviceChangeForFetchInputDevice_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->deviceType_ = DEVICE_TYPE_MIC;
    desc->connectState_ = CONNECTED;
    desc->networkId_ = "LocalNetworkId";
    desc->macAddress_ = "00:11:22:33:44:55";

    std::shared_ptr<AudioCapturerChangeInfo> capturerChangeInfo = std::make_shared<AudioCapturerChangeInfo>();
    capturerChangeInfo->sessionId = 12345;
    capturerChangeInfo->inputDeviceInfo = AudioDeviceDescriptor(*desc);
    capturerChangeInfo->clientUID = 54321;

    AudioDeviceDescriptor currentInputDevice;
    currentInputDevice.deviceType_ = DEVICE_TYPE_MIC;
    currentInputDevice.networkId_ = "LocalNetworkId";
    currentInputDevice.macAddress_ = "00:11:22:33:44:55";
    currentInputDevice.connectState_ = CONNECTED;
    audioDeviceCommon.audioActiveDevice_.currentActiveInputDevice_ = currentInputDevice;

    std::shared_ptr<AudioDeviceDescriptor> preferredDesc = std::make_shared<AudioDeviceDescriptor>();
    preferredDesc->deviceType_ = DEVICE_TYPE_MIC;
    audioDeviceCommon.audioAffinityManager_.activeCapturerDeviceMap_[capturerChangeInfo->clientUID] = preferredDesc;

    int32_t result = audioDeviceCommon.HandleDeviceChangeForFetchInputDevice(desc, capturerChangeInfo);
    EXPECT_EQ(result, ERR_NEED_NOT_SWITCH_DEVICE);
}

/**
* @tc.name  : Test FetchInputDeviceInner.
* @tc.number: FetchInputDeviceInner_001
* @tc.desc  : Test FetchInputDeviceInner interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, FetchInputDeviceInner_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::vector<std::shared_ptr<AudioCapturerChangeInfo>> capturerChangeInfos;
    std::shared_ptr<AudioCapturerChangeInfo> capturerChangeInfo = std::make_shared<AudioCapturerChangeInfo>();
    capturerChangeInfo->capturerInfo.sourceType = SOURCE_TYPE_VIRTUAL_CAPTURE;
    capturerChangeInfo->prerunningState = false;
    capturerChangeInfo->sessionId = 12345;
    capturerChangeInfos.push_back(capturerChangeInfo);

    audioDeviceCommon.audioSceneManager_.SetAudioScenePre(AUDIO_SCENE_DEFAULT);
    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    bool needUpdateActiveDevice = true;
    bool isUpdateActiveDevice = false;
    int32_t runningStreamCount = 0;

    audioDeviceCommon.FetchInputDeviceInner(capturerChangeInfos, reason, needUpdateActiveDevice,
        isUpdateActiveDevice, runningStreamCount);
    EXPECT_EQ(runningStreamCount, 0);
}

/**
* @tc.name  : Test FetchInputDeviceInner.
* @tc.number: FetchInputDeviceInner_002
* @tc.desc  : Test FetchInputDeviceInner interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, FetchInputDeviceInner_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::vector<std::shared_ptr<AudioCapturerChangeInfo>> capturerChangeInfos;
    std::shared_ptr<AudioCapturerChangeInfo> capturerChangeInfo = std::make_shared<AudioCapturerChangeInfo>();
    capturerChangeInfo->capturerInfo.sourceType = SOURCE_TYPE_VIRTUAL_CAPTURE; // Valid source type
    capturerChangeInfo->capturerState = CAPTURER_RUNNING;
    capturerChangeInfo->prerunningState = false;
    capturerChangeInfo->sessionId = 12345;
    capturerChangeInfos.push_back(capturerChangeInfo);

    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->deviceType_ = DEVICE_TYPE_MIC;
    desc->connectState_ = CONNECTED;

    capturerChangeInfo->inputDeviceInfo.deviceType_ = DEVICE_TYPE_MIC;
    capturerChangeInfo->inputDeviceInfo.connectState_ = CONNECTED;

    audioDeviceCommon.audioRouterCenter_.FetchInputDevice(capturerChangeInfo->capturerInfo.sourceType,
        capturerChangeInfo->clientUID, capturerChangeInfo->sessionId);
    audioDeviceCommon.audioSceneManager_.SetAudioScenePre(AUDIO_SCENE_PHONE_CALL);

    AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN;
    bool needUpdateActiveDevice = true;
    bool isUpdateActiveDevice = false;
    int32_t runningStreamCount = 0;

    audioDeviceCommon.FetchInputDeviceInner(capturerChangeInfos, reason, needUpdateActiveDevice,
        isUpdateActiveDevice, runningStreamCount);
    EXPECT_EQ(runningStreamCount, 1);
    EXPECT_FALSE(isUpdateActiveDevice);
}

/**
* @tc.name  : Test FetchInputDeviceInner.
* @tc.number: FetchInputEnd_001
* @tc.desc  : Test FetchInputDeviceInner interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, FetchInputEnd_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    bool isUpdateActiveDevice = true;
    int32_t runningStreamCount = 1;

    audioDeviceCommon.FetchInputEnd(isUpdateActiveDevice, runningStreamCount);
    EXPECT_TRUE(isUpdateActiveDevice);
}

/**
* @tc.name  : Test MoveToNewInputDevice.
* @tc.number: MoveToNewInputDevice_001
* @tc.desc  : Test MoveToNewInputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, MoveToNewInputDevice_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioCapturerChangeInfo> capturerChangeInfo = std::make_shared<AudioCapturerChangeInfo>();
    capturerChangeInfo->sessionId = 12345;
    capturerChangeInfo->inputDeviceInfo.deviceType_ = DEVICE_TYPE_MIC;
    capturerChangeInfo->inputDeviceInfo.macAddress_ = "00:11:22:33:44:55";
    capturerChangeInfo->inputDeviceInfo.networkId_ = LOCAL_NETWORK_ID;

    std::shared_ptr<AudioDeviceDescriptor> inputDevice = std::make_shared<AudioDeviceDescriptor>();
    inputDevice->deviceType_ = DEVICE_TYPE_USB_HEADSET;
    inputDevice->macAddress_ = "66:77:88:99:AA:BB";
    inputDevice->networkId_ = LOCAL_NETWORK_ID;

    audioDeviceCommon.audioConfigManager_.OnUpdateRouteSupport(true);
    audioDeviceCommon.MoveToNewInputDevice(capturerChangeInfo, inputDevice);
    EXPECT_EQ(capturerChangeInfo->inputDeviceInfo.deviceType_, inputDevice->deviceType_);
    EXPECT_EQ(capturerChangeInfo->inputDeviceInfo.macAddress_, inputDevice->macAddress_);
    EXPECT_EQ(capturerChangeInfo->inputDeviceInfo.networkId_, inputDevice->networkId_);
}

/**
* @tc.name  : Test MoveToNewInputDevice.
* @tc.number: MoveToNewInputDevice_002
* @tc.desc  : Test MoveToNewInputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, MoveToNewInputDevice_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioCapturerChangeInfo> capturerChangeInfo = std::make_shared<AudioCapturerChangeInfo>();
    capturerChangeInfo->sessionId = 12345;
    capturerChangeInfo->inputDeviceInfo.deviceType_ = DEVICE_TYPE_MIC;
    capturerChangeInfo->inputDeviceInfo.macAddress_ = "00:11:22:33:44:55";
    capturerChangeInfo->inputDeviceInfo.networkId_ = LOCAL_NETWORK_ID;

    std::shared_ptr<AudioDeviceDescriptor> inputDevice = std::make_shared<AudioDeviceDescriptor>();
    inputDevice->deviceType_ = DEVICE_TYPE_USB_HEADSET;
    inputDevice->macAddress_ = "66:77:88:99:AA:BB";
    inputDevice->networkId_ = LOCAL_NETWORK_ID;

    audioDeviceCommon.audioConfigManager_.OnUpdateRouteSupport(false);
    audioDeviceCommon.MoveToNewInputDevice(capturerChangeInfo, inputDevice);
    EXPECT_EQ(capturerChangeInfo->inputDeviceInfo.deviceType_, inputDevice->deviceType_);
    EXPECT_EQ(capturerChangeInfo->inputDeviceInfo.macAddress_, inputDevice->macAddress_);
    EXPECT_EQ(capturerChangeInfo->inputDeviceInfo.networkId_, inputDevice->networkId_);
}

/**
* @tc.name  : Test MoveToNewInputDevice.
* @tc.number: MoveToNewInputDevice_003
* @tc.desc  : Test MoveToNewInputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, MoveToNewInputDevice_003, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioCapturerChangeInfo> capturerChangeInfo = std::make_shared<AudioCapturerChangeInfo>();
    capturerChangeInfo->sessionId = 12345;
    capturerChangeInfo->inputDeviceInfo.deviceType_ = DEVICE_TYPE_MIC;
    capturerChangeInfo->inputDeviceInfo.macAddress_ = "00:11:22:33:44:55";
    capturerChangeInfo->inputDeviceInfo.networkId_ = LOCAL_NETWORK_ID;

    std::shared_ptr<AudioDeviceDescriptor> inputDevice = std::make_shared<AudioDeviceDescriptor>();
    inputDevice->deviceType_ = DEVICE_TYPE_USB_HEADSET;
    inputDevice->macAddress_ = "66:77:88:99:AA:BB";
    inputDevice->networkId_ = REMOTE_NETWORK_ID;

    audioDeviceCommon.audioConfigManager_.OnUpdateRouteSupport(true);
    audioDeviceCommon.MoveToNewInputDevice(capturerChangeInfo, inputDevice);
    EXPECT_EQ(capturerChangeInfo->inputDeviceInfo.deviceType_, inputDevice->deviceType_);
    EXPECT_EQ(capturerChangeInfo->inputDeviceInfo.macAddress_, inputDevice->macAddress_);
    EXPECT_EQ(capturerChangeInfo->inputDeviceInfo.networkId_, inputDevice->networkId_);
}

/**
* @tc.name  : Test FetchInputDeviceWhenNoRunningStream.
* @tc.number: FetchInputDeviceWhenNoRunningStream_001
* @tc.desc  : Test FetchInputDeviceWhenNoRunningStream interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, FetchInputDeviceWhenNoRunningStream_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    AudioDeviceDescriptor tempDesc;
    tempDesc.deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    audioDeviceCommon.audioActiveDevice_.SetCurrentInputDevice(tempDesc);
    audioDeviceCommon.FetchInputDeviceWhenNoRunningStream();
    EXPECT_EQ(audioDeviceCommon.audioActiveDevice_.currentActiveInputDevice_.deviceType_, DEVICE_TYPE_BLUETOOTH_A2DP);
}

/**
* @tc.name  : Test FetchInputDeviceWhenNoRunningStream.
* @tc.number: FetchInputDeviceWhenNoRunningStream_002
* @tc.desc  : Test FetchInputDeviceWhenNoRunningStream interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, FetchInputDeviceWhenNoRunningStream_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    AudioDeviceDescriptor tempDesc;
    tempDesc.deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    audioDeviceCommon.audioActiveDevice_.SetCurrentInputDevice(tempDesc);
    audioDeviceCommon.FetchInputDeviceWhenNoRunningStream();
    EXPECT_EQ(audioDeviceCommon.audioActiveDevice_.currentActiveInputDevice_.deviceType_, DEVICE_TYPE_BLUETOOTH_A2DP);
}

/**
* @tc.name  : Test BluetoothScoFetch.
* @tc.number: BluetoothScoFetch_001
* @tc.desc  : Test BluetoothScoFetch interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, BluetoothScoFetch_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->deviceType_ = DEVICE_TYPE_BLUETOOTH_SCO;
    desc->macAddress_ = "00:11:22:33:44:55";
    desc->networkId_ = LOCAL_NETWORK_ID;

    std::vector<std::shared_ptr<AudioCapturerChangeInfo>> capturerChangeInfos;
    std::shared_ptr<AudioCapturerChangeInfo> capturerChangeInfo = std::make_shared<AudioCapturerChangeInfo>();
    capturerChangeInfo->capturerInfo.sourceType = SOURCE_TYPE_VOICE_RECOGNITION;
    capturerChangeInfos.push_back(capturerChangeInfo);

    audioDeviceCommon.audioActiveDevice_.SetCurrentInputDevice(*desc);
    audioDeviceCommon.BluetoothScoFetch(desc, capturerChangeInfos, SOURCE_TYPE_VOICE_RECOGNITION);
    const AudioDeviceDescriptor& activeDevice = audioDeviceCommon.audioActiveDevice_.GetCurrentInputDevice();
    EXPECT_EQ(activeDevice.deviceType_, DEVICE_TYPE_BLUETOOTH_SCO);
    EXPECT_EQ(activeDevice.macAddress_, "00:11:22:33:44:55");
    EXPECT_EQ(activeDevice.networkId_, LOCAL_NETWORK_ID);
}

/**
* @tc.name  : Test MoveToRemoteOutputDevice.
* @tc.number: MoveToRemoteOutputDevice_001
* @tc.desc  : Test MoveToRemoteOutputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, MoveToRemoteOutputDevice_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::vector<SinkInput> sinkInputs;
    SinkInput sinkInput = {1};
    sinkInputs.push_back(sinkInput);

    std::shared_ptr<AudioDeviceDescriptor> remoteDevice = std::make_shared<AudioDeviceDescriptor>();
    remoteDevice->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    remoteDevice->macAddress_ = "00:11:22:33:44:55";
    remoteDevice->networkId_ = REMOTE_NETWORK_ID;

    std::string moduleName = "123";
    AudioIOHandle moduleId = 1;
    audioDeviceCommon.audioIOHandleMap_.AddIOHandleInfo(moduleName, moduleId);

    int32_t result = audioDeviceCommon.MoveToRemoteOutputDevice(sinkInputs, remoteDevice);
    EXPECT_EQ(result, ERR_OPERATION_FAILED);
    EXPECT_TRUE(audioDeviceCommon.audioIOHandleMap_.CheckIOHandleExist(moduleName));
}

/**
* @tc.name  : Test MoveToRemoteOutputDevice.
* @tc.number: MoveToRemoteOutputDevice_002
* @tc.desc  : Test MoveToRemoteOutputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, MoveToRemoteOutputDevice_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::vector<SinkInput> sinkInputs;
    SinkInput sinkInput = {1};
    sinkInputs.push_back(sinkInput);

    std::string moduleName = "123";
    std::shared_ptr<AudioDeviceDescriptor> remoteDevice = std::make_shared<AudioDeviceDescriptor>();
    remoteDevice->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    remoteDevice->macAddress_ = "00:11:22:33:44:55";
    remoteDevice->networkId_ = REMOTE_NETWORK_ID;

    audioDeviceCommon.audioIOHandleMap_.IOHandles_.clear();
    audioDeviceCommon.isOpenRemoteDevice = false;

    int32_t result = audioDeviceCommon.MoveToRemoteOutputDevice(sinkInputs, remoteDevice);
    EXPECT_EQ(result, ERR_INVALID_PARAM);
    EXPECT_FALSE(audioDeviceCommon.audioIOHandleMap_.CheckIOHandleExist(moduleName));
}

/**
* @tc.name  : Test MoveToRemoteOutputDevice.
* @tc.number: MoveToRemoteOutputDevice_003
* @tc.desc  : Test MoveToRemoteOutputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, MoveToRemoteOutputDevice_003, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::vector<SinkInput> sinkInputs;
    SinkInput sinkInput = {1};
    sinkInputs.push_back(sinkInput);

    std::string moduleName = "123";
    std::shared_ptr<AudioDeviceDescriptor> remoteDevice = std::make_shared<AudioDeviceDescriptor>();
    remoteDevice->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    remoteDevice->macAddress_ = "00:11:22:33:44:55";
    remoteDevice->networkId_ = REMOTE_NETWORK_ID;

    audioDeviceCommon.audioIOHandleMap_.IOHandles_.clear();
    audioDeviceCommon.isOpenRemoteDevice = true;

    int32_t result = audioDeviceCommon.MoveToRemoteOutputDevice(sinkInputs, remoteDevice);
    EXPECT_EQ(result, SUCCESS);
    EXPECT_FALSE(audioDeviceCommon.audioIOHandleMap_.CheckIOHandleExist(moduleName));
}

/**
* @tc.name  : Test ScoInputDeviceFetchedForRecongnition.
* @tc.number: ScoInputDeviceFetchedForRecongnition_001
* @tc.desc  : Test ScoInputDeviceFetchedForRecongnition interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, ScoInputDeviceFetchedForRecongnition_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    bool handleFlag = false;
    std::string address = "00:11:22:33:44:55";
    ConnectState connectState = DEACTIVE_CONNECTED;

    int32_t result = audioDeviceCommon.ScoInputDeviceFetchedForRecongnition(handleFlag, address, connectState);
    EXPECT_EQ(result, ERROR);
}

/**
* @tc.name  : Test ScoInputDeviceFetchedForRecongnition.
* @tc.number: ScoInputDeviceFetchedForRecongnition_002
* @tc.desc  : Test ScoInputDeviceFetchedForRecongnition interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, ScoInputDeviceFetchedForRecongnition_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    bool handleFlag = true;
    std::string address = "00:11:22:33:44:55";
    ConnectState connectState = DEACTIVE_CONNECTED;

    int32_t result = audioDeviceCommon.ScoInputDeviceFetchedForRecongnition(handleFlag, address, connectState);
    EXPECT_EQ(result, ERROR);
}

/**
* @tc.name  : Test MoveToRemoteInputDevice.
* @tc.number: MoveToRemoteInputDevice_001
* @tc.desc  : Test MoveToRemoteInputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, MoveToRemoteInputDevice_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::vector<SourceOutput> sourceOutputs;
    SourceOutput sourceOutput = {1};
    sourceOutputs.push_back(sourceOutput);

    std::shared_ptr<AudioDeviceDescriptor> remoteDevice = std::make_shared<AudioDeviceDescriptor>();
    remoteDevice->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    remoteDevice->macAddress_ = "00:11:22:33:44:55";
    remoteDevice->networkId_ = REMOTE_NETWORK_ID;

    std::string moduleName = "123";
    AudioIOHandle moduleId = 1;
    audioDeviceCommon.audioIOHandleMap_.AddIOHandleInfo(moduleName, moduleId);

    int32_t result = audioDeviceCommon.MoveToRemoteInputDevice(sourceOutputs, remoteDevice);
    EXPECT_EQ(result, ERR_OPERATION_FAILED);
    EXPECT_TRUE(audioDeviceCommon.audioIOHandleMap_.CheckIOHandleExist(moduleName));
}

/**
* @tc.name  : Test MoveToRemoteInputDevice.
* @tc.number: MoveToRemoteInputDevice_002
* @tc.desc  : Test MoveToRemoteInputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, MoveToRemoteInputDevice_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::vector<SourceOutput> sourceOutputs;
    SourceOutput sourceOutput = {1};
    sourceOutputs.push_back(sourceOutput);

    std::string moduleName = "123";
    std::shared_ptr<AudioDeviceDescriptor> remoteDevice = std::make_shared<AudioDeviceDescriptor>();
    remoteDevice->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    remoteDevice->macAddress_ = "00:11:22:33:44:55";
    remoteDevice->networkId_ = REMOTE_NETWORK_ID;

    audioDeviceCommon.audioIOHandleMap_.IOHandles_.clear();
    audioDeviceCommon.isOpenRemoteDevice = false;

    int32_t result = audioDeviceCommon.MoveToRemoteInputDevice(sourceOutputs, remoteDevice);
    EXPECT_EQ(result, ERR_INVALID_PARAM);
    EXPECT_FALSE(audioDeviceCommon.audioIOHandleMap_.CheckIOHandleExist(moduleName));
}

/**
* @tc.name  : Test MoveToRemoteInputDevice.
* @tc.number: MoveToRemoteInputDevice_003
* @tc.desc  : Test MoveToRemoteInputDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, MoveToRemoteInputDevice_003, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::vector<SourceOutput> sourceOutputs;
    SourceOutput sourceOutput = {1};
    sourceOutputs.push_back(sourceOutput);

    std::string moduleName = "123";
    std::shared_ptr<AudioDeviceDescriptor> remoteDevice = std::make_shared<AudioDeviceDescriptor>();
    remoteDevice->deviceType_ = DEVICE_TYPE_REMOTE_CAST;
    remoteDevice->macAddress_ = "00:11:22:33:44:55";
    remoteDevice->networkId_ = REMOTE_NETWORK_ID;

    audioDeviceCommon.audioIOHandleMap_.IOHandles_.clear();
    audioDeviceCommon.isOpenRemoteDevice = true;

    int32_t result = audioDeviceCommon.MoveToRemoteInputDevice(sourceOutputs, remoteDevice);
    EXPECT_EQ(result, SUCCESS);
    EXPECT_FALSE(audioDeviceCommon.audioIOHandleMap_.CheckIOHandleExist(moduleName));
}

/**
* @tc.name  : Test IsSameDevice
* @tc.number: IsSameDevice_001
* @tc.desc  : Test IsSameDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, IsSameDevice_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->networkId_ = "LocalNetworkId";
    desc->deviceType_ = DEVICE_TYPE_USB_HEADSET;
    desc->macAddress_ = "00:11:22:33:44:55";
    desc->connectState_ = CONNECTED;
    desc->deviceRole_ = INPUT_DEVICE;

    const AudioDeviceDescriptor deviceDesc = AudioDeviceDescriptor(*desc);
    bool result = audioDeviceCommon.IsSameDevice(desc, deviceDesc);
    EXPECT_TRUE(result);
}

/**
* @tc.name  : Test IsSameDevice
* @tc.number: IsSameDevice_002
* @tc.desc  : Test IsSameDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, IsSameDevice_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->networkId_ = "LocalNetworkId";
    desc->deviceType_ = DEVICE_TYPE_NONE;
    desc->macAddress_ = "00:11:22:33:44:55";
    desc->connectState_ = CONNECTED;
    desc->deviceRole_ = INPUT_DEVICE;

    const AudioDeviceDescriptor deviceDesc = AudioDeviceDescriptor(*desc);
    bool result = audioDeviceCommon.IsSameDevice(desc, deviceDesc);
    EXPECT_TRUE(result);
}

/**
* @tc.name  : Test IsSameDevice
* @tc.number: IsSameDevice_003
* @tc.desc  : Test IsSameDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, IsSameDevice_003, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->networkId_ = "LocalNetworkId";
    desc->deviceType_ = DEVICE_TYPE_NONE;
    desc->macAddress_ = "00:11:22:33:44:55";
    desc->connectState_ = CONNECTED;
    desc->deviceRole_ = INPUT_DEVICE;

    const AudioDeviceDescriptor deviceDesc = AudioDeviceDescriptor(*desc);
    const_cast<AudioDeviceDescriptor&>(deviceDesc).deviceRole_ = OUTPUT_DEVICE;
    const_cast<AudioDeviceDescriptor&>(deviceDesc).networkId_ = "RemoteNetworkId";
    bool result = audioDeviceCommon.IsSameDevice(desc, deviceDesc);
    EXPECT_FALSE(result);
}

/**
* @tc.name  : Test IsSameDevice.
* @tc.number: IsSameDevice_004
* @tc.desc  : Test IsSameDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, IsSameDevice_004, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->networkId_ = "LocalNetworkId";
    desc->deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    desc->macAddress_ = "00:11:22:33:44:55";
    desc->connectState_ = CONNECTED;
    desc->a2dpOffloadFlag_ = A2DP_OFFLOAD;

    AudioDeviceDescriptor deviceInfo = AudioDeviceDescriptor(*desc);
    deviceInfo.a2dpOffloadFlag_ = A2DP_OFFLOAD;
    deviceInfo.descriptorType_ = DEVICE_TYPE_BLUETOOTH_A2DP;

    audioDeviceCommon.audioA2dpOffloadFlag_.SetA2dpOffloadFlag(A2DP_NOT_OFFLOAD);
    bool result = audioDeviceCommon.IsSameDevice(desc, deviceInfo);
    EXPECT_FALSE(result);
}

/**
* @tc.name  : Test IsSameDevice.
* @tc.number: IsSameDevice_005
* @tc.desc  : Test IsSameDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, IsSameDevice_005, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->networkId_ = "LocalNetworkId";
    desc->deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    desc->macAddress_ = "00:11:22:33:44:55";
    desc->connectState_ = CONNECTED;
    desc->a2dpOffloadFlag_ = A2DP_NOT_OFFLOAD;

    AudioDeviceDescriptor deviceInfo = AudioDeviceDescriptor(*desc);
    deviceInfo.descriptorType_ = DEVICE_TYPE_BLUETOOTH_A2DP;

    audioDeviceCommon.audioA2dpOffloadFlag_.SetA2dpOffloadFlag(A2DP_OFFLOAD);
    bool result = audioDeviceCommon.IsSameDevice(desc, deviceInfo);
    EXPECT_FALSE(result);
}

/**
* @tc.name  : Test IsSameDevice.
* @tc.number: IsSameDevice_006
* @tc.desc  : Test IsSameDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, IsSameDevice_006, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->networkId_ = "LocalNetworkId";
    desc->deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    desc->macAddress_ = "00:11:22:33:44:55";
    desc->connectState_ = CONNECTED;
    desc->a2dpOffloadFlag_ = A2DP_OFFLOAD;

    AudioDeviceDescriptor deviceInfo = AudioDeviceDescriptor(*desc);
    deviceInfo.descriptorType_ = DEVICE_TYPE_BLUETOOTH_A2DP;

    audioDeviceCommon.audioA2dpOffloadFlag_.SetA2dpOffloadFlag(A2DP_OFFLOAD);
    bool result = audioDeviceCommon.IsSameDevice(desc, deviceInfo);
    EXPECT_TRUE(result);
}

/**
* @tc.name  : Test BluetoothScoDisconectForRecongnition.
* @tc.number: BluetoothScoDisconectForRecongnition_001
* @tc.desc  : Test BluetoothScoDisconectForRecongnition interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, BluetoothScoDisconectForRecongnition_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    AudioDeviceDescriptor deviceInfo;
    deviceInfo.deviceType_ = DEVICE_TYPE_BLUETOOTH_SCO;
    audioDeviceCommon.audioActiveDevice_.SetCurrentInputDevice(deviceInfo);
    audioDeviceCommon.BluetoothScoDisconectForRecongnition();
    DeviceType deviceType = audioDeviceCommon.audioActiveDevice_.currentActiveInputDevice_.deviceType_;
    EXPECT_EQ(deviceType, DEVICE_TYPE_BLUETOOTH_SCO);
}

/**
* @tc.name  : Test ClientDiedDisconnectScoNormal.
* @tc.number: ClientDiedDisconnectScoNormal_001
* @tc.desc  : Test ClientDiedDisconnectScoNormal interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, ClientDiedDisconnectScoNormal_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    AudioDeviceDescriptor outputDevice;
    outputDevice.deviceType_ = DEVICE_TYPE_BLUETOOTH_SCO;
    audioDeviceCommon.audioActiveDevice_.SetCurrentOutputDevice(outputDevice);

    audioDeviceCommon.streamCollector_.audioRendererChangeInfos_.clear();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->rendererState = RENDERER_RUNNING;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_MEDIA;
    audioDeviceCommon.streamCollector_.audioRendererChangeInfos_.push_back(rendererChangeInfo);

    audioDeviceCommon.ClientDiedDisconnectScoNormal();
    EXPECT_EQ(audioDeviceCommon.audioActiveDevice_.GetCurrentOutputDeviceType(), DEVICE_TYPE_BLUETOOTH_SCO);
}

/**
* @tc.name  : Test ClientDiedDisconnectScoNormal.
* @tc.number: ClientDiedDisconnectScoNormal_002
* @tc.desc  : Test ClientDiedDisconnectScoNormal interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, ClientDiedDisconnectScoNormal_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    AudioDeviceDescriptor outputDevice;
    outputDevice.deviceType_ = DEVICE_TYPE_SPEAKER;
    audioDeviceCommon.audioActiveDevice_.SetCurrentOutputDevice(outputDevice);

    audioDeviceCommon.streamCollector_.audioRendererChangeInfos_.clear();
    std::shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    rendererChangeInfo->rendererState = RENDERER_RUNNING;
    rendererChangeInfo->rendererInfo.streamUsage = STREAM_USAGE_MEDIA;
    audioDeviceCommon.streamCollector_.audioRendererChangeInfos_.push_back(rendererChangeInfo);

    audioDeviceCommon.ClientDiedDisconnectScoNormal();
    EXPECT_EQ(audioDeviceCommon.audioActiveDevice_.GetCurrentOutputDeviceType(), DEVICE_TYPE_SPEAKER);
}

/**
* @tc.name  : Test ClientDiedDisconnectScoRecognition.
* @tc.number: ClientDiedDisconnectScoRecognition_001
* @tc.desc  : Test ClientDiedDisconnectScoRecognition interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, ClientDiedDisconnectScoRecognition_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioCapturerChangeInfo> capturerChangeInfo = std::make_shared<AudioCapturerChangeInfo>();
    capturerChangeInfo->capturerState = CAPTURER_RUNNING;
    capturerChangeInfo->capturerInfo.sourceType = SOURCE_TYPE_VOICE_RECOGNITION;
    audioDeviceCommon.streamCollector_.audioCapturerChangeInfos_.push_back(capturerChangeInfo);

    audioDeviceCommon.ClientDiedDisconnectScoRecognition();
    EXPECT_TRUE(audioDeviceCommon.streamCollector_.HasRunningRecognitionCapturerStream());
}

/**
* @tc.name  : Test ClientDiedDisconnectScoRecognition.
* @tc.number: ClientDiedDisconnectScoRecognition_002
* @tc.desc  : Test ClientDiedDisconnectScoRecognition interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, ClientDiedDisconnectScoRecognition_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    AudioDeviceDescriptor inputDevice;
    inputDevice.deviceType_ = DEVICE_TYPE_MIC;
    audioDeviceCommon.audioActiveDevice_.SetCurrentInputDevice(inputDevice);

    audioDeviceCommon.streamCollector_.audioCapturerChangeInfos_.clear();
    audioDeviceCommon.ClientDiedDisconnectScoRecognition();
    EXPECT_EQ(audioDeviceCommon.audioActiveDevice_.GetCurrentInputDeviceType(), DEVICE_TYPE_MIC);
}

/**
* @tc.name  : Test GetA2dpModuleInfo.
* @tc.number: GetA2dpModuleInfo_001
* @tc.desc  : Test GetA2dpModuleInfo function, entering the if branch.
*/
HWTEST_F(AudioDeviceCommonUnitTest, GetA2dpModuleInfo_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    AudioModuleInfo moduleInfo;
    AudioStreamInfo audioStreamInfo;
    audioStreamInfo.samplingRate = SAMPLE_RATE_48000;
    audioStreamInfo.format = SAMPLE_S16LE;
    audioStreamInfo.channels = STEREO;


    moduleInfo.role = "sink";
    audioDeviceCommon.GetA2dpModuleInfo(moduleInfo, audioStreamInfo, SOURCE_TYPE_MIC);
    EXPECT_EQ(moduleInfo.channels, "2");
    EXPECT_EQ(moduleInfo.rate, "48000");
    EXPECT_EQ(moduleInfo.format, "s16le");
    EXPECT_EQ(moduleInfo.renderInIdleState, "1");
    EXPECT_EQ(moduleInfo.sinkLatency, "0");
}

/**
* @tc.name  : Test GetA2dpModuleInfo.
* @tc.number: GetA2dpModuleInfo_002
* @tc.desc  : Test GetA2dpModuleInfo function, not entering the if branch.
*/
HWTEST_F(AudioDeviceCommonUnitTest, GetA2dpModuleInfo_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    AudioModuleInfo moduleInfo;
    AudioStreamInfo audioStreamInfo;
    audioStreamInfo.samplingRate = SAMPLE_RATE_44100;
    audioStreamInfo.format = SAMPLE_S24LE;
    audioStreamInfo.channels = MONO;

    moduleInfo.role = "source";
    audioDeviceCommon.GetA2dpModuleInfo(moduleInfo, audioStreamInfo, SOURCE_TYPE_MIC);
    EXPECT_EQ(moduleInfo.channels, "1");
    EXPECT_EQ(moduleInfo.rate, "44100");
    EXPECT_EQ(moduleInfo.format, "s24le");
    EXPECT_EQ(moduleInfo.renderInIdleState, "");
    EXPECT_EQ(moduleInfo.sinkLatency, "");
}

/**
* @tc.name  : Test LoadA2dpModule
* @tc.number: LoadA2dpModule_001
* @tc.desc  : Test LoadA2dpModule interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, LoadA2dpModule_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    AudioStreamInfo audioStreamInfo;
    audioStreamInfo.samplingRate = SAMPLE_RATE_8000;
    audioStreamInfo.encoding = ENCODING_PCM;
    audioStreamInfo.format = SAMPLE_S24LE;
    audioStreamInfo.channels = STEREO;

    DeviceType deviceType = DEVICE_TYPE_BLUETOOTH_A2DP;
    std::string networkID = "LocalNetworkId";
    std::string sinkName = "bt_a2dp_sink";
    SourceType sourceType = SOURCE_TYPE_MIC;

    AudioModuleInfo moduleInfo;
    moduleInfo.name = "bt_a2dp";
    moduleInfo.role = "sink";
    std::list<AudioModuleInfo> moduleInfoList;
    moduleInfoList.push_back(moduleInfo);

    audioDeviceCommon.audioConfigManager_.deviceClassInfo_[ClassType::TYPE_A2DP] = moduleInfoList;
    int32_t result = audioDeviceCommon.LoadA2dpModule(deviceType, audioStreamInfo, networkID, sinkName, sourceType);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test SwitchActiveA2dpDevice
* @tc.number: SwitchActiveA2dpDevice_001
* @tc.desc  : Test SwitchActiveA2dpDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, SwitchActiveA2dpDevice_001, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioDeviceDescriptor> deviceDescriptor = std::make_shared<AudioDeviceDescriptor>();
    deviceDescriptor->macAddress_ = Bluetooth::AudioA2dpManager::GetActiveA2dpDevice();
    deviceDescriptor->deviceName_ = "TestA2dpDevice";
    deviceDescriptor->deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    audioDeviceCommon.audioA2dpDevice_.connectedA2dpDeviceMap_[deviceDescriptor->macAddress_] = A2dpDeviceConfigInfo();

    audioDeviceCommon.audioIOHandleMap_.IOHandles_[BLUETOOTH_SPEAKER] = 12345;
    int32_t result = audioDeviceCommon.SwitchActiveA2dpDevice(deviceDescriptor);
    EXPECT_EQ(result, SUCCESS);
}

/**
* @tc.name  : Test SwitchActiveA2dpDevice
* @tc.number: SwitchActiveA2dpDevice_002
* @tc.desc  : Test SwitchActiveA2dpDevice interface.
*/
HWTEST_F(AudioDeviceCommonUnitTest, SwitchActiveA2dpDevice_002, TestSize.Level1)
{
    AudioDeviceCommon& audioDeviceCommon = AudioDeviceCommon::GetInstance();
    audioDeviceCommon.DeInit();

    std::shared_ptr<AudioDeviceDescriptor> deviceDescriptor = std::make_shared<AudioDeviceDescriptor>();
    deviceDescriptor->macAddress_ = "00:11:22:33:44:55";
    deviceDescriptor->deviceName_ = "TestA2dpDevice";
    deviceDescriptor->deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    audioDeviceCommon.audioA2dpDevice_.connectedA2dpDeviceMap_[deviceDescriptor->macAddress_] = A2dpDeviceConfigInfo();

    audioDeviceCommon.audioIOHandleMap_.IOHandles_.clear();
    int32_t result = audioDeviceCommon.SwitchActiveA2dpDevice(deviceDescriptor);
    EXPECT_EQ(result, ERROR);
}
} // namespace AudioStandard
} // namespace OHOS