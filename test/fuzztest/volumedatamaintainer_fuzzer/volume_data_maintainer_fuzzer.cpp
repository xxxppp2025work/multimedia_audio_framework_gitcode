/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
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

#include "audio_info.h"
#include "audio_common_utils.h"
#include "volume_data_maintainer.h"
#include "../fuzz_utils.h"
using namespace std;

namespace OHOS {
namespace AudioStandard {

FuzzUtils &g_fuzzUtils = FuzzUtils::GetInstance();
typedef void (*TestPtr)();

void VolumeDataMaintainerSaveVolumeFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    DeviceType typeRet = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamTypeRet = g_fuzzUtils.GetData<AudioStreamType>();
    int32_t volumeLevelRet = g_fuzzUtils.GetData<int32_t>();
    volumeDataMaintainerRet->SaveVolume(typeRet, streamTypeRet, volumeLevelRet);
}

void VolumeDataMaintainerGetVolumeFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    DeviceType deviceTypeRet = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamTypeRet = g_fuzzUtils.GetData<AudioStreamType>();
    volumeDataMaintainerRet->GetVolume(deviceTypeRet, streamTypeRet);
}

void VolumeDataMaintainerSaveMuteStatusFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    DeviceType deviceTypeRet = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamTypeRet = g_fuzzUtils.GetData<AudioStreamType>();
    bool muteStatusRet = g_fuzzUtils.GetData<bool>();
    volumeDataMaintainerRet->SaveMuteStatus(deviceTypeRet, streamTypeRet, muteStatusRet);
}

void VolumeDataMaintainerGetMuteStatusInternalFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    DeviceType deviceTypeRet = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamTypeRet = g_fuzzUtils.GetData<AudioStreamType>();
    volumeDataMaintainerRet->GetMuteStatusInternal(deviceTypeRet, streamTypeRet);
}

void VolumeDataMaintainerGetMuteTransferStatusFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    int32_t affectedRet = g_fuzzUtils.GetData<int32_t>();
    bool statusRet = g_fuzzUtils.GetData<bool>();
    volumeDataMaintainerRet->GetMuteAffected(affectedRet);
    volumeDataMaintainerRet->GetMuteTransferStatus(statusRet);
}

void VolumeDataMaintainerGetSafeStatusFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    DeviceType deviceTypeRet = g_fuzzUtils.GetData<DeviceType>();
    SafeStatus safeStatusRet = g_fuzzUtils.GetData<SafeStatus>();
    volumeDataMaintainerRet->SaveSafeStatus(deviceTypeRet, safeStatusRet);
    volumeDataMaintainerRet->GetSafeStatus(deviceTypeRet, safeStatusRet);
}

void VolumeDataMaintainerGetSafeVolumeTimeFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    DeviceType deviceTypeRet = g_fuzzUtils.GetData<DeviceType>();
    int64_t timeRet = g_fuzzUtils.GetData<int64_t>();
    volumeDataMaintainerRet->SaveSafeVolumeTime(deviceTypeRet, timeRet);
    volumeDataMaintainerRet->GetSafeVolumeTime(deviceTypeRet, timeRet);
}

void VolumeDataMaintainerRegisterClonedFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    std::string keyRet;
    std::string valueRet;
    volumeDataMaintainerRet->SaveSystemSoundUrl(keyRet, valueRet);
    volumeDataMaintainerRet->GetSystemSoundUrl(keyRet, valueRet);
    volumeDataMaintainerRet->RegisterCloned();
}

void VolumeDataMaintainerGetMicMuteStateFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    bool isMuteRet = g_fuzzUtils.GetData<bool>();
    volumeDataMaintainerRet->SaveMicMuteState(isMuteRet);
    volumeDataMaintainerRet->GetMicMuteState(isMuteRet);
}

void VolumeDataMaintainerGetDeviceTypeNameFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    DeviceType deviceTypeRet = g_fuzzUtils.GetData<DeviceType>();
    volumeDataMaintainerRet->GetDeviceTypeName(deviceTypeRet);
}

void VolumeDataMaintainerGetVolumeKeyForDataShareFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    DeviceType deviceTypeRet = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamTypeRet = g_fuzzUtils.GetData<AudioStreamType>();
    volumeDataMaintainerRet->GetVolumeKeyForDataShare(deviceTypeRet, streamTypeRet);
}

void VolumeDataMaintainerSaveMuteStatusInternalFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainer = std::make_shared<VolumeDataMaintainer>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    bool muteStatus = g_fuzzUtils.GetData<bool>();
    volumeDataMaintainer->SaveMuteStatusInternal(deviceType, streamType, muteStatus);
}

void VolumeDataMaintainerGetAppMuteFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainer = std::make_shared<VolumeDataMaintainer>();
    int32_t appUid = g_fuzzUtils.GetData<int32_t>();
    bool isMute = g_fuzzUtils.GetData<bool>();
    volumeDataMaintainer->appMuteStatusMap_.erase(appUid);

    volumeDataMaintainer->GetAppMute(appUid, isMute);
}

void VolumeDataMaintainerGetAppMuteOwnedFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainer = std::make_shared<VolumeDataMaintainer>();
    int32_t appUid = g_fuzzUtils.GetData<int32_t>();
    bool isMute = g_fuzzUtils.GetData<bool>();
    int32_t callingUid = IPCSkeleton::GetCallingUid();
    volumeDataMaintainer->appMuteStatusMap_[appUid][callingUid] = g_fuzzUtils.GetData<bool>();

    volumeDataMaintainer->GetAppMuteOwned(appUid, isMute);
}

void VolumeDataMaintainerGetDeviceVolumeInternalFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainer = std::make_shared<VolumeDataMaintainer>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    volumeDataMaintainer->GetDeviceVolumeInternal(deviceType, streamType);
}

void VolumeDataMaintainerSetMuteAffectedToMuteStatusDataBaseFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainer = std::make_shared<VolumeDataMaintainer>();
    std::lock_guard<ffrt::mutex> lock(volumeDataMaintainer->volumeMutex_);
    volumeDataMaintainer->volumeLevelMap_.clear();
    volumeDataMaintainer->appVolumeLevelMap_.clear();
    volumeDataMaintainer->appMuteStatusMap_.clear();

    int32_t affected = g_fuzzUtils.GetData<int32_t>();
    volumeDataMaintainer->SetMuteAffectedToMuteStatusDataBase(affected);
}

void VolumeDataMaintainerSetRestoreVolumeLevelFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainer = std::make_shared<VolumeDataMaintainer>();
    std::lock_guard<ffrt::mutex> lock(volumeDataMaintainer->volumeMutex_);
    volumeDataMaintainer->volumeLevelMap_.clear();
    volumeDataMaintainer->appVolumeLevelMap_.clear();
    volumeDataMaintainer->appMuteStatusMap_.clear();

    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    int32_t volume = g_fuzzUtils.GetData<int32_t>();
    volumeDataMaintainer->SetRestoreVolumeLevel(deviceType, volume);
}

void VolumeDataMaintainerGetRestoreVolumeLevelFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainer = std::make_shared<VolumeDataMaintainer>();
    std::lock_guard<ffrt::mutex> lock(volumeDataMaintainer->volumeMutex_);
    volumeDataMaintainer->volumeLevelMap_.clear();
    volumeDataMaintainer->appVolumeLevelMap_.clear();
    volumeDataMaintainer->appMuteStatusMap_.clear();

    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    int32_t volume = g_fuzzUtils.GetData<int32_t>();
    volumeDataMaintainer->GetRestoreVolumeLevel(deviceType, volume);
}

void VolumeDataMaintainerGetRingerModeFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    int32_t affectedRet = g_fuzzUtils.GetData<int32_t>();
    bool statusRet = g_fuzzUtils.GetData<bool>();
    AudioRingerMode ringerModeRet = g_fuzzUtils.GetData<AudioRingerMode>();
    volumeDataMaintainerRet->SetMuteAffectedToMuteStatusDataBase(affectedRet);
    volumeDataMaintainerRet->SaveMuteTransferStatus(statusRet);
    volumeDataMaintainerRet->SaveRingerMode(ringerModeRet);
    volumeDataMaintainerRet->GetRingerMode(ringerModeRet);
}

void VolumeDataMaintainerGetDeviceVolumeFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    volumeDataMaintainerRet->GetDeviceVolume(deviceType, streamType);
}

void VolumeDataMaintainerSetStreamMuteStatusFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    bool muteStatus = g_fuzzUtils.GetData<bool>();
    volumeDataMaintainerRet->SetStreamMuteStatus(streamType, muteStatus);
}

void VolumeDataMaintainerGetMuteStatusFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    volumeDataMaintainerRet->GetMuteStatus(deviceType, streamType);
}

void VolumeDataMaintainerGetStreamMuteFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    volumeDataMaintainerRet->GetStreamMute(streamType);
}

void VolumeDataMaintainerSetAppVolumeFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    int32_t appUid = g_fuzzUtils.GetData<int32_t>();
    int32_t volumeLevel = g_fuzzUtils.GetData<int32_t>();
    volumeDataMaintainerRet->SetAppVolume(appUid, volumeLevel);
}

void VolumeDataMaintainerSetDataShareReadyFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    volumeDataMaintainerRet->SetDataShareReady(std::atomic<bool>(g_fuzzUtils.GetData<bool>()));
}

void VolumeDataMaintainerSaveVolumeWithDatabaseVolumeNameFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    std::string databaseVolumeName = "test_database_volume_name";
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    int32_t volumeLevel = g_fuzzUtils.GetData<int32_t>();
    volumeDataMaintainerRet->SaveVolumeWithDatabaseVolumeName(databaseVolumeName, streamType, volumeLevel);
}

void VolumeDataMaintainerGetVolumeWithDatabaseVolumeNameFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    std::string databaseVolumeName = "test_database_volume_name";
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    volumeDataMaintainerRet->GetVolumeWithDatabaseVolumeName(databaseVolumeName, streamType);
}

void VolumeDataMaintainerSaveMuteStatusWithDatabaseVolumeNameFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    std::string databaseVolumeName = "test_database_volume_name";
    AudioStreamType streamType = STREAM_DEFAULT;
    bool muteStatus = g_fuzzUtils.GetData<bool>();
    VolumeUtils::SetPCVolumeEnable(false);
    volumeDataMaintainerRet->SaveMuteStatusWithDatabaseVolumeName(databaseVolumeName, streamType, muteStatus);
}

void VolumeDataMaintainerGetMuteStatusWithDatabaseVolumeNameFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    std::string databaseVolumeName = "test_database_volume_name";
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    volumeDataMaintainerRet->GetMuteStatusWithDatabaseVolumeName(databaseVolumeName, streamType);
}

void VolumeDataMaintainerSetAppVolumeMutedFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    int32_t appUid = g_fuzzUtils.GetData<int32_t>();
    bool muted = g_fuzzUtils.GetData<bool>();
    volumeDataMaintainerRet->SetAppVolumeMuted(appUid, muted);
}

void VolumeDataMaintainerGetVolumeKeyForDatabaseVolumeNameFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    std::string databaseVolumeName = "test_database_volume_name";
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    volumeDataMaintainerRet->GetVolumeKeyForDatabaseVolumeName(databaseVolumeName, streamType);
}

void VolumeDataMaintainerGetMuteKeyForDatabaseVolumeNameFuzzTest()
{
    std::shared_ptr<VolumeDataMaintainer> volumeDataMaintainerRet = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainerRet != nullptr);
    std::string databaseVolumeName = "test_database_volume_name";
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    volumeDataMaintainerRet->GetMuteKeyForDatabaseVolumeName(databaseVolumeName, streamType);
}

vector<TestFuncs> g_testFuncs = {
    VolumeDataMaintainerSaveVolumeFuzzTest,
    VolumeDataMaintainerGetVolumeFuzzTest,
    VolumeDataMaintainerSaveMuteStatusFuzzTest,
    VolumeDataMaintainerGetMuteStatusInternalFuzzTest,
    VolumeDataMaintainerGetMuteTransferStatusFuzzTest,
    VolumeDataMaintainerGetSafeStatusFuzzTest,
    VolumeDataMaintainerGetSafeVolumeTimeFuzzTest,
    VolumeDataMaintainerRegisterClonedFuzzTest,
    VolumeDataMaintainerGetMicMuteStateFuzzTest,
    VolumeDataMaintainerGetDeviceTypeNameFuzzTest,
    VolumeDataMaintainerGetVolumeKeyForDataShareFuzzTest,
    VolumeDataMaintainerSaveMuteStatusInternalFuzzTest,
    VolumeDataMaintainerGetAppMuteFuzzTest,
    VolumeDataMaintainerGetAppMuteOwnedFuzzTest,
    VolumeDataMaintainerGetDeviceVolumeInternalFuzzTest,
    VolumeDataMaintainerSetMuteAffectedToMuteStatusDataBaseFuzzTest,
    VolumeDataMaintainerSetRestoreVolumeLevelFuzzTest,
    VolumeDataMaintainerGetRestoreVolumeLevelFuzzTest,
    VolumeDataMaintainerGetRingerModeFuzzTest,
    VolumeDataMaintainerGetDeviceVolumeFuzzTest,
    VolumeDataMaintainerSetStreamMuteStatusFuzzTest,
    VolumeDataMaintainerGetMuteStatusFuzzTest,
    VolumeDataMaintainerGetStreamMuteFuzzTest,
    VolumeDataMaintainerSetAppVolumeFuzzTest,
    VolumeDataMaintainerSetDataShareReadyFuzzTest,
    VolumeDataMaintainerSaveVolumeWithDatabaseVolumeNameFuzzTest,
    VolumeDataMaintainerGetVolumeWithDatabaseVolumeNameFuzzTest,
    VolumeDataMaintainerSaveMuteStatusWithDatabaseVolumeNameFuzzTest,
    VolumeDataMaintainerGetMuteStatusWithDatabaseVolumeNameFuzzTest,
    VolumeDataMaintainerSetAppVolumeMutedFuzzTest,
    VolumeDataMaintainerGetVolumeKeyForDatabaseVolumeNameFuzzTest,
    VolumeDataMaintainerGetMuteKeyForDatabaseVolumeNameFuzzTest,
};

} // namespace AudioStandard
} // namesapce OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    OHOS::AudioStandard::g_fuzzUtils.fuzzTest(data, size, OHOS::AudioStandard::g_testFuncs);
    return 0;
}