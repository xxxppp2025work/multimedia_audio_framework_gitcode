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

#include "audio_volume_manager_unit_test.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {
const int32_t RESTORE_VOLUME_NOTIFICATION_ID = 116000;
const int32_t INCREASE_VOLUME_NOTIFICATION_ID = 116001;
const uint32_t NOTIFICATION_BANNER_FLAG = 1 << 9;

void AudioVolumeManagerUnitTest::SetUpTestCase(void) {}
void AudioVolumeManagerUnitTest::TearDownTestCase(void) {}

void AudioVolumeManagerUnitTest::SetUp(void)
{
    std::shared_ptr<AudioPolicyServerHandler> audioPolicyServerHandler(
        DelayedSingleton<AudioPolicyServerHandler>::GetInstance());
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    audioVolumeManager.Init(audioPolicyServerHandler);
}

void AudioVolumeManagerUnitTest::TearDown(void)
{
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    audioVolumeManager.DeInit();
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_001
* @tc.desc  : Test GetSharedVolume interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_001, TestSize.Level1)
{
    AudioVolumeType streamType = AudioStreamType::STREAM_DEFAULT;
    DeviceType deviceType = DeviceType::DEVICE_TYPE_SPEAKER;
    Volume vol;
    bool bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.GetSharedVolume(streamType, deviceType, vol);
    EXPECT_EQ(bRet, false);

    streamType = AudioStreamType::STREAM_RING;
    bRet = audioVolumeManager.GetSharedVolume(streamType, deviceType, vol);
    EXPECT_EQ(bRet, true);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_002
* @tc.desc  : Test InitSharedVolume interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_002, TestSize.Level1)
{
    std::shared_ptr<AudioSharedMemory> buffer;
    int32_t ret;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    ret = audioVolumeManager.InitSharedVolume(buffer);
    EXPECT_EQ(ret, SUCCESS);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_003
* @tc.desc  : Test SetVoiceRingtoneMute interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_003, TestSize.Level1)
{
    bool isMute = true;
    int32_t ret;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    ret = audioVolumeManager.SetVoiceRingtoneMute(isMute);
    EXPECT_EQ(ret, SUCCESS);

    isMute = false;
    ret = audioVolumeManager.SetVoiceRingtoneMute(isMute);
    EXPECT_EQ(ret, SUCCESS);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_004
* @tc.desc  : Test HandleAbsBluetoothVolume interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_004, TestSize.Level1)
{
    std::string macAddress = "11:22:33:44:55:66";
    int32_t volumeLevel = 0;
    int32_t ret;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    audioVolumeManager.isBtFirstBoot_ = true;
    ret = audioVolumeManager.HandleAbsBluetoothVolume(macAddress, volumeLevel);
    EXPECT_EQ(ret, 0);

    audioVolumeManager.isBtFirstBoot_ = false;
    audioVolumeManager.audioActiveDevice_.currentActiveDevice_.deviceCategory_ = BT_CAR;
    ret = audioVolumeManager.HandleAbsBluetoothVolume(macAddress, volumeLevel);
    EXPECT_EQ(ret, 0);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_005
* @tc.desc  : Test IsWiredHeadSet interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_005, TestSize.Level1)
{
    DeviceType deviceType = DEVICE_TYPE_NONE;
    bool bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.IsWiredHeadSet(deviceType);
    EXPECT_EQ(bRet, false);

    deviceType = DEVICE_TYPE_WIRED_HEADSET;
    bRet = audioVolumeManager.IsWiredHeadSet(deviceType);
    EXPECT_EQ(bRet, true);

    deviceType = DEVICE_TYPE_WIRED_HEADPHONES;
    bRet = audioVolumeManager.IsWiredHeadSet(deviceType);
    EXPECT_EQ(bRet, true);

    deviceType = DEVICE_TYPE_USB_HEADSET;
    bRet = audioVolumeManager.IsWiredHeadSet(deviceType);
    EXPECT_EQ(bRet, true);

    deviceType = DEVICE_TYPE_USB_ARM_HEADSET;
    bRet = audioVolumeManager.IsWiredHeadSet(deviceType);
    EXPECT_EQ(bRet, true);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_006
* @tc.desc  : Test IsBlueTooth interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_006, TestSize.Level1)
{
    DeviceType deviceType = DEVICE_TYPE_NONE;
    bool bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.IsBlueTooth(deviceType);
    EXPECT_EQ(bRet, false);

    audioVolumeManager.audioActiveDevice_.currentActiveDevice_.deviceCategory_ = BT_CAR;
    deviceType = DEVICE_TYPE_BLUETOOTH_A2DP;
    bRet = audioVolumeManager.IsBlueTooth(deviceType);
    EXPECT_EQ(bRet, false);

    audioVolumeManager.audioActiveDevice_.currentActiveDevice_.deviceCategory_ = BT_SOUNDBOX;
    deviceType = DEVICE_TYPE_BLUETOOTH_SCO;
    bRet = audioVolumeManager.IsBlueTooth(deviceType);
    EXPECT_EQ(bRet, false);

    audioVolumeManager.audioActiveDevice_.currentActiveDevice_.deviceCategory_ = CATEGORY_DEFAULT;
    deviceType = DEVICE_TYPE_BLUETOOTH_A2DP;
    bRet = audioVolumeManager.IsBlueTooth(deviceType);
    EXPECT_EQ(bRet, true);

    deviceType = DEVICE_TYPE_BLUETOOTH_SCO;
    bRet = audioVolumeManager.IsBlueTooth(deviceType);
    EXPECT_EQ(bRet, true);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_007
* @tc.desc  : Test CheckMixActiveMusicTime interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_007, TestSize.Level1)
{
    int32_t safeVolume = 0;
    bool bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    audioVolumeManager.activeSafeTimeBt_ = 0;
    audioVolumeManager.activeSafeTime_ = 0;
    bRet = audioVolumeManager.CheckMixActiveMusicTime(safeVolume);
    EXPECT_EQ(bRet, false);

    audioVolumeManager.activeSafeTimeBt_ = 100000;
    audioVolumeManager.activeSafeTime_ = 100000;
    bRet = audioVolumeManager.CheckMixActiveMusicTime(safeVolume);
    EXPECT_EQ(bRet, true);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_008
* @tc.desc  : Test CheckBlueToothActiveMusicTime interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_008, TestSize.Level1)
{
    int32_t safeVolume = 0;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    audioVolumeManager.startSafeTimeBt_ = 0;
    audioVolumeManager.activeSafeTimeBt_ = 10;
    audioVolumeManager.activeSafeTime_ = 100;
    audioVolumeManager.CheckBlueToothActiveMusicTime(safeVolume);
    EXPECT_EQ(audioVolumeManager.startSafeTime_, 0);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_009
* @tc.desc  : Test CheckWiredActiveMusicTime interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_009, TestSize.Level1)
{
    int32_t safeVolume = 0;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    audioVolumeManager.startSafeTime_ = 0;
    audioVolumeManager.activeSafeTimeBt_ = 10;
    audioVolumeManager.activeSafeTime_ = 100;
    audioVolumeManager.CheckWiredActiveMusicTime(safeVolume);
    EXPECT_EQ(audioVolumeManager.startSafeTimeBt_, 0);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_010
* @tc.desc  : Test RestoreSafeVolume interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_010, TestSize.Level1)
{
    AudioStreamType streamType = STREAM_RING;
    int32_t safeVolume = 0;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    audioVolumeManager.RestoreSafeVolume(streamType, safeVolume);
    EXPECT_EQ(audioVolumeManager.GetSystemVolumeLevel(streamType), 0);

    safeVolume = -1;
    audioVolumeManager.RestoreSafeVolume(streamType, safeVolume);
    EXPECT_EQ(audioVolumeManager.GetSystemVolumeLevel(streamType), 0);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_011
* @tc.desc  : Test SetSafeVolumeCallback interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_011, TestSize.Level1)
{
    AudioStreamType streamType = STREAM_MUSIC;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    audioVolumeManager.SetSafeVolumeCallback(streamType);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_012
* @tc.desc  : Test ChangeDeviceSafeStatus interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_012, TestSize.Level1)
{
    SafeStatus safeStatus = SAFE_UNKNOWN;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    audioVolumeManager.ChangeDeviceSafeStatus(safeStatus);
    EXPECT_EQ(audioVolumeManager.safeStatus_, SAFE_UNKNOWN);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_013
* @tc.desc  : Test SetAbsVolumeSceneAsync interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_013, TestSize.Level1)
{
    std::string macAddress = "11:22:33:44:55:66";
    bool support = true;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    audioVolumeManager.audioActiveDevice_.SetActiveBtDeviceMac(macAddress);
    audioVolumeManager.SetAbsVolumeSceneAsync(macAddress, support);
    EXPECT_EQ(audioVolumeManager.audioActiveDevice_.GetActiveBtDeviceMac(), macAddress);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_014
* @tc.desc  : Test DealWithEventVolume interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_014, TestSize.Level1)
{
    int32_t notificationId;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());
    int32_t ret;

    notificationId = RESTORE_VOLUME_NOTIFICATION_ID;
    audioVolumeManager.audioActiveDevice_.currentActiveDevice_.deviceCategory_ = CATEGORY_DEFAULT;
    audioVolumeManager.audioActiveDevice_.currentActiveDevice_.deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    ret = audioVolumeManager.DealWithEventVolume(notificationId);
    EXPECT_NE(ret, 0);

    notificationId = INCREASE_VOLUME_NOTIFICATION_ID;
    ret = audioVolumeManager.DealWithEventVolume(notificationId);
    EXPECT_NE(ret, 0);

    notificationId = NOTIFICATION_BANNER_FLAG;
    ret = audioVolumeManager.DealWithEventVolume(notificationId);
    EXPECT_EQ(ret, 0);

    audioVolumeManager.audioActiveDevice_.currentActiveDevice_.deviceType_ = DEVICE_TYPE_WIRED_HEADSET;
    notificationId = RESTORE_VOLUME_NOTIFICATION_ID;
    ret = audioVolumeManager.DealWithEventVolume(notificationId);
    EXPECT_NE(ret, 0);

    notificationId = INCREASE_VOLUME_NOTIFICATION_ID;
    ret = audioVolumeManager.DealWithEventVolume(notificationId);
    EXPECT_EQ(ret, 0);

    notificationId = NOTIFICATION_BANNER_FLAG;
    ret = audioVolumeManager.DealWithEventVolume(notificationId);
    EXPECT_EQ(ret, 0);

    audioVolumeManager.audioActiveDevice_.currentActiveDevice_.deviceType_ = DEVICE_TYPE_NONE;
    ret = audioVolumeManager.DealWithEventVolume(notificationId);
    EXPECT_NE(ret, 0);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_015
* @tc.desc  : Test UpdateSafeVolumeByS4 interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_015, TestSize.Level1)
{
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());
    AudioDeviceDescriptor audioDeviceDescriptor;

    audioVolumeManager.UpdateSafeVolumeByS4();
    audioVolumeManager.audioPolicyManager_.SetVolumeForSwitchDevice(audioDeviceDescriptor);
    EXPECT_EQ(audioVolumeManager.isBtFirstBoot_, true);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_016
* @tc.desc  : Test SetAbsVolumeSceneAsync interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_016, TestSize.Level1)
{
    std::string macAddress = "11:22:33:44:55:66";
    bool support = true;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    audioVolumeManager.audioActiveDevice_.SetActiveBtDeviceMac(macAddress);
    auto ret = audioVolumeManager.SetDeviceAbsVolumeSupported(macAddress, support);
    EXPECT_NE(ret, 0);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_017
* @tc.desc  : Test CheckMixActiveMusicTime interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_017, TestSize.Level1)
{
    int32_t safeVolume = 0;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    audioVolumeManager.activeSafeTimeBt_ = 0;
    audioVolumeManager.activeSafeTime_ = 0;
    audioVolumeManager.CheckBlueToothActiveMusicTime(safeVolume);

    audioVolumeManager.activeSafeTimeBt_ = 100000;
    audioVolumeManager.activeSafeTime_ = 100000;
    audioVolumeManager.CheckBlueToothActiveMusicTime(safeVolume);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_018
* @tc.desc  : Test CheckMixActiveMusicTime interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_018, TestSize.Level1)
{
    int32_t safeVolume = 0;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    audioVolumeManager.activeSafeTimeBt_ = 0;
    audioVolumeManager.activeSafeTime_ = 0;
    audioVolumeManager.CheckWiredActiveMusicTime(safeVolume);

    audioVolumeManager.activeSafeTimeBt_ = 100000;
    audioVolumeManager.activeSafeTime_ = 100000;
    audioVolumeManager.CheckWiredActiveMusicTime(safeVolume);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_019
* @tc.desc  : Test CheckMixActiveMusicTime interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_019, TestSize.Level1)
{
    AudioVolumeType streamType = AudioStreamType::STREAM_DEFAULT;
    bool mute = true;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());
    bool bRet;

    bRet = audioVolumeManager.SetStreamMute(streamType, mute);
    EXPECT_TRUE(bRet);

    StreamUsage streamUsage = STREAM_USAGE_MEDIA;
    DeviceType deviceType = DeviceType::DEVICE_TYPE_BLUETOOTH_A2DP;
    bRet = audioVolumeManager.SetStreamMute(streamType, mute, streamUsage, deviceType);
    EXPECT_TRUE(bRet);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_020
* @tc.desc  : Test GetSharedVolume interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_020, TestSize.Level1)
{
    AudioVolumeType streamType = AudioStreamType::STREAM_ALL;
    int32_t bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.GetMaxVolumeLevel(streamType);
    EXPECT_EQ(bRet, 15);

    streamType = AudioStreamType::STREAM_MUSIC;
    bRet = audioVolumeManager.GetMaxVolumeLevel(streamType);
    EXPECT_EQ(bRet, 15);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_021
* @tc.desc  : Test GetSharedVolume interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_021, TestSize.Level1)
{
    AudioVolumeType streamType = AudioStreamType::STREAM_ALL;
    int32_t bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.GetMinVolumeLevel(streamType);
    EXPECT_EQ(bRet, 0);

    streamType = AudioStreamType::STREAM_MUSIC;
    bRet = audioVolumeManager.GetMinVolumeLevel(streamType);
    EXPECT_EQ(bRet, 0);
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_022
* @tc.desc  : Test GetAllDeviceVolumeInfo interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_022, TestSize.Level1)
{
    auto audioVolumeManager = std::make_shared<AudioVolumeManager>();
    ASSERT_TRUE(audioVolumeManager != nullptr);

    std::shared_ptr<AudioDeviceDescriptor> remoteDeviceDescriptor = std::make_shared<AudioDeviceDescriptor>(
        DeviceType::DEVICE_TYPE_EARPIECE, DeviceRole::OUTPUT_DEVICE);
    audioVolumeManager->audioConnectedDevice_.AddConnectedDevice(remoteDeviceDescriptor);
    audioVolumeManager->GetAllDeviceVolumeInfo();
}

/**
* @tc.name  : Test AudioVolumeManager.
* @tc.number: AudioVolumeManager_023
* @tc.desc  : Test SetSharedVolume interface.
*/
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_023, TestSize.Level1)
{
    AudioVolumeType streamType = AudioStreamType::STREAM_ALL;
    DeviceType deviceType = DeviceType::DEVICE_TYPE_SPEAKER;
    Volume vol;
    bool bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.SetSharedVolume(streamType, deviceType, vol);
    EXPECT_NE(bRet, false);

    streamType = AudioStreamType::STREAM_RING;
    bRet = audioVolumeManager.SetSharedVolume(streamType, deviceType, vol);
    EXPECT_EQ(bRet, true);
}

/**
 * @tc.name  : Test AudioVolumeManager.
 * @tc.number: AudioVolumeManager_024
 * @tc.desc  : Test GetAppVolumeLevel interface.
 */
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_024, TestSize.Level1)
{
    int32_t appUid = 0;
    int32_t bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.GetAppVolumeLevel(appUid);
    EXPECT_EQ(bRet, 0);
}

/**
 * @tc.name  : Test AudioVolumeManager.
 * @tc.number: AudioVolumeManager_025
 * @tc.desc  : Test GetSystemVolumeLevelNoMuteState interface.
 */
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_025, TestSize.Level1)
{
    AudioStreamType streamType = AudioStreamType::STREAM_ALL;
    int32_t bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.GetSystemVolumeLevelNoMuteState(streamType);
    EXPECT_EQ(bRet, 0);

    streamType = AudioStreamType::STREAM_MUSIC;
    bRet = audioVolumeManager.GetSystemVolumeLevelNoMuteState(streamType);
    EXPECT_EQ(bRet, 0);
}

/**
 * @tc.name  : Test AudioVolumeManager.
 * @tc.number: AudioVolumeManager_026
 * @tc.desc  : Test SetSystemVolumeLevel interface.
 */
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_026, TestSize.Level1)
{
    AudioStreamType streamType = AudioStreamType::STREAM_ALL;
    int32_t volumeLevel = 0;
    int32_t bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.SetSystemVolumeLevel(streamType, volumeLevel);
    EXPECT_EQ(bRet, 0);

    streamType = AudioStreamType::STREAM_MUSIC;
    bRet = audioVolumeManager.SetSystemVolumeLevel(streamType, volumeLevel);
    EXPECT_EQ(bRet, 0);
}

/**
 * @tc.name  : Test AudioVolumeManager.
 * @tc.number: AudioVolumeManager_027
 * @tc.desc  : Test SetSystemVolumeLevelWithDevice interface.
 */
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_027, TestSize.Level1)
{
    AudioStreamType streamType = AudioStreamType::STREAM_ALL;
    int32_t volumeLevel = 0;
    DeviceType deviceType = DeviceType::DEVICE_TYPE_SPEAKER;
    int32_t bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.SetSystemVolumeLevelWithDevice(streamType, volumeLevel, deviceType);
    EXPECT_NE(bRet, 0);
}

/**
 * @tc.name  : Test AudioVolumeManager.
 * @tc.number: AudioVolumeManager_028
 * @tc.desc  : Test SetAppVolumeMuted interface.
 */
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_028, TestSize.Level1)
{
    int32_t appUid = 0;
    bool muted = true;
    int32_t bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.SetAppVolumeMuted(appUid, muted);
    EXPECT_EQ(bRet, 0);
}

/**
 * @tc.name  : Test AudioVolumeManager.
 * @tc.number: AudioVolumeManager_029
 * @tc.desc  : Test IsAppVolumeMute interface.
 */
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_029, TestSize.Level1)
{
    int32_t appUid = 0;
    bool owned = true;
    bool bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.IsAppVolumeMute(appUid, owned);
    EXPECT_EQ(bRet, true);
}

/**
 * @tc.name  : Test AudioVolumeManager.
 * @tc.number: AudioVolumeManager_030
 * @tc.desc  : Test SetAppVolumeLevel interface.
 */
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_030, TestSize.Level1)
{
    int32_t appUid = 0;
    int32_t volumeLevel = 0;
    int32_t bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.SetAppVolumeLevel(appUid, volumeLevel);
    EXPECT_EQ(bRet, 0);
}

/**
 * @tc.name  : Test AudioVolumeManager.
 * @tc.number: AudioVolumeManager_031
 * @tc.desc  : Test DisableSafeMediaVolume interface.
 */
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_031, TestSize.Level1)
{
    int32_t bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.DisableSafeMediaVolume();
    EXPECT_EQ(bRet, 0);
}

/**
 * @tc.name  : Test AudioVolumeManager.
 * @tc.number: AudioVolumeManager_032
 * @tc.desc  : Test SetDeviceAbsVolumeSupported interface.
 */
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_032, TestSize.Level1)
{
    std::string macAddress = "11:22:33:44:55:66";
    bool support = true;
    int32_t bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.SetDeviceAbsVolumeSupported(macAddress, support);
    EXPECT_NE(bRet, 0);
}

/**
 * @tc.name  : Test AudioVolumeManager.
 * @tc.number: AudioVolumeManager_033
 * @tc.desc  : Test GetStreamMute interface.
 */
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_033, TestSize.Level1)
{
    AudioStreamType streamType = AudioStreamType::STREAM_ALL;
    bool bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.GetStreamMute(streamType);
    EXPECT_EQ(bRet, false);

    streamType = AudioStreamType::STREAM_MUSIC;
    bRet = audioVolumeManager.GetStreamMute(streamType);
    EXPECT_EQ(bRet, true);
}

/**
 * @tc.name  : Test AudioVolumeManager.
 * @tc.number: AudioVolumeManager_034
 * @tc.desc  : Test SetA2dpDeviceVolume interface.
 */
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_034, TestSize.Level1)
{
    std::string macAddress = "11:22:33:44:55:66";
    int32_t volumeLevel = 0;
    int32_t bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.SetA2dpDeviceVolume(macAddress, volumeLevel);
    EXPECT_NE(bRet, 0);
}

/**
 * @tc.name  : Test AudioVolumeManager.
 * @tc.number: AudioVolumeManager_035
 * @tc.desc  : Test IsRingerModeMute interface.
 */
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_035, TestSize.Level1)
{
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());
    bool bRet;

    bRet = audioVolumeManager.IsRingerModeMute();
    EXPECT_EQ(bRet, true);

    audioVolumeManager.SetRingerModeMute(false);
    bRet = audioVolumeManager.IsRingerModeMute();
    EXPECT_EQ(bRet, false);
}

/**
 * @tc.name  : Test AudioVolumeManager.
 * @tc.number: AudioVolumeManager_036
 * @tc.desc  : Test ResetRingerModeMute interface.
 */
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_036, TestSize.Level1)
{
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());
    bool bRet;

    audioVolumeManager.SetRingerModeMute(true);
    bRet = audioVolumeManager.ResetRingerModeMute();
    EXPECT_EQ(bRet, false);

    bRet = audioVolumeManager.IsRingerModeMute();
    EXPECT_EQ(bRet, true);
}

/**
 * @tc.name  : Test AudioVolumeManager.
 * @tc.number: AudioVolumeManager_037
 * @tc.desc  : Test GetVolumeGroupInfosNotWait interface.
 */
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_037, TestSize.Level1)
{
    std::vector<sptr<VolumeGroupInfo>> infos;
    bool bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.GetVolumeGroupInfosNotWait(infos);
    EXPECT_EQ(bRet, false);

    audioVolumeManager.SetDefaultDeviceLoadFlag(true);
    bRet = audioVolumeManager.GetVolumeGroupInfosNotWait(infos);
    EXPECT_EQ(bRet, true);
}

/**
 * @tc.name  : Test AudioVolumeManager.
 * @tc.number: AudioVolumeManager_038
 * @tc.desc  : Test GetLoadFlag interface.
 */
HWTEST_F(AudioVolumeManagerUnitTest, AudioVolumeManager_038, TestSize.Level1)
{
    bool bRet;
    AudioVolumeManager& audioVolumeManager(AudioVolumeManager::GetInstance());

    bRet = audioVolumeManager.GetLoadFlag();
    EXPECT_EQ(bRet, true);

    audioVolumeManager.SetDefaultDeviceLoadFlag(false);
    bRet = audioVolumeManager.GetLoadFlag();
    EXPECT_EQ(bRet, false);
}
} // namespace AudioStandard
} // namespace OHOS
