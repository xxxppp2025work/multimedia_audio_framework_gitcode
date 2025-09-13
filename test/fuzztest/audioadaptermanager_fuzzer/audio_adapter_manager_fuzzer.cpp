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

#include "audio_log.h"
#include "audio_adapter_manager.h"
#include "audio_server_proxy.h"
#include "audio_zone_service.h"
#include "../fuzz_utils.h"
using namespace std;

namespace OHOS {
namespace AudioStandard {

static shared_ptr<AudioAdapterManager> audioAdapterManager_ = make_shared<AudioAdapterManager>();
static bool g_isInit = false;
FuzzUtils &g_fuzzUtils = FuzzUtils::GetInstance();
const int32_t DEFAULT_TEST_VOLUME_LEVEL = 10;
typedef void (*TestPtr)();

void AudioAdapterManagerIsAppVolumeMuteFuzzTest()
{
    int32_t appUid = g_fuzzUtils.GetData<int32_t>();
    bool owned = g_fuzzUtils.GetData<bool>();
    bool isMute = g_fuzzUtils.GetData<bool>();
    AudioAdapterManager::GetInstance().IsAppVolumeMute(appUid, owned, isMute);
}

void AudioAdapterManagerSaveSpecifiedDeviceVolumeFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    audioAdapterManager_->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    int32_t volumeLevel = g_fuzzUtils.GetData<int32_t>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager_->SaveSpecifiedDeviceVolume(streamType, volumeLevel, deviceType);
}

void AudioAdapterManagerHandleStreamMuteStatusFuzzTest()
{
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    bool mute = g_fuzzUtils.GetData<bool>();
    StreamUsage streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    AudioAdapterManager::GetInstance().HandleStreamMuteStatus(streamType, mute, streamUsage, deviceType);
}

void AudioAdapterManagerSetOffloadVolumeFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    float volumeDb = g_fuzzUtils.GetData<float>();
    audioAdapterManager_->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager_->SetOffloadVolume(streamType, volumeDb, "offload");
}

void AudioAdapterManagerSetOffloadSessionIdFuzzTest()
{
    uint32_t sessionId = g_fuzzUtils.GetData<uint32_t>();
    AudioAdapterManager::GetInstance().SetOffloadSessionId(sessionId);
}

void AudioAdapterManagerHandleKvDataFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    audioAdapterManager_->isNeedCopySystemUrlData_ = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->isNeedCopyVolumeData_ = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->isNeedCopyMuteData_ = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->isNeedCopyRingerModeData_ = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->isNeedCopySystemUrlData_ = g_fuzzUtils.GetData<bool>();
    bool isFirstBoot = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->HandleKvData(isFirstBoot);
}

void AudioAdapterManagerReInitKVStoreFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    audioAdapterManager_->ReInitKVStore();
}

void AudioAdapterManagerSaveRingtoneVolumeToLocalFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    int32_t volumeLevel = g_fuzzUtils.GetData<int32_t>();
    AudioVolumeType volumeType = g_fuzzUtils.GetData<AudioVolumeType>();
    audioAdapterManager_->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager_->SaveRingtoneVolumeToLocal(volumeType, volumeLevel);
}

void AudioAdapterManagerUpdateSafeVolumeByS4FuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    audioAdapterManager_->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager_->UpdateSafeVolumeByS4();
}

void AudioAdapterManagerSetAppVolumeMutedFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    bool muted = g_fuzzUtils.GetData<bool>();
    int32_t appUid = g_fuzzUtils.GetData<int32_t>();
    audioAdapterManager_->SetAppVolumeLevel(appUid, randIntValue);
    audioAdapterManager_->SetAppVolumeMuted(randIntValue, muted);
}

void AudioAdapterManagerSetSystemVolumeLevelFuzzTest()
{
    static const vector<string> testNetworkIds = {
        "LocalDevice",
        "RemoteDevice",
        "NetworkDevice1",
    };
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioDeviceDescriptor deviceDescriptor;
    deviceDescriptor.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    deviceDescriptor.networkId_ = testNetworkIds[index % testNetworkIds.size()];
    int32_t testVolumeLevel = DEFAULT_TEST_VOLUME_LEVEL;
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    audioAdapterManager->SetActiveDeviceDescriptor(deviceDescriptor);
    audioAdapterManager->SetSystemVolumeLevel(STREAM_MUSIC, testVolumeLevel);
}

void AudioAdapterManagerSetDoubleRingVolumeDbFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    audioAdapterManager_->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    int32_t volumeLevel = g_fuzzUtils.GetData<int32_t>();
    audioAdapterManager_->useNonlinearAlgo_ = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetDoubleRingVolumeDb(streamType, volumeLevel);
}

void AudioAdapterManagerSetInnerStreamMuteFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    StreamUsage streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    bool mute = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetInnerStreamMute(streamType, mute, streamUsage);
}

void AudioAdapterManagerSetStreamMuteFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    StreamUsage streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    bool mute = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetStreamMute(streamType, mute, streamUsage, deviceType);
}

void AudioAdapterManagerSetSinkMuteFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    static const vector<string> sinkNames = {
        "Speaker",
        "Offload_Speaker",
        "Bt_Speaker",
        "MCH_Speaker",
        "Usb_arm_speaker",
        "DP_speaker",
        "file_sink",
        "RemoteCastInnerCapturer",
        "test",
    };
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    string sinkName = sinkNames[index % sinkNames.size()];
    bool isMute = g_fuzzUtils.GetData<bool>();
    bool isSync = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetSinkMute(sinkName, isMute, isSync);
}

void AudioAdapterManagerSelectDeviceFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    DeviceRole deviceRole = g_fuzzUtils.GetData<DeviceRole>();
    InternalDeviceType deviceType = g_fuzzUtils.GetData<InternalDeviceType>();
    audioAdapterManager_->SelectDevice(deviceRole, deviceType, "test");
}

void AudioAdapterManagerSetDeviceActiveFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    InternalDeviceType deviceType = g_fuzzUtils.GetData<InternalDeviceType>();
    bool active = g_fuzzUtils.GetData<bool>();
    DeviceFlag flag = g_fuzzUtils.GetData<DeviceFlag>();
    audioAdapterManager_->SetDeviceActive(deviceType, "test", active, flag);
}

void AudioAdapterManagerAdjustBluetoothVoiceAssistantVolumeFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    InternalDeviceType deviceType = g_fuzzUtils.GetData<InternalDeviceType>();
    bool isA2dpSwitchToSco = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->AdjustBluetoothVoiceAssistantVolume(deviceType, isA2dpSwitchToSco);
}

void AudioAdapterManagerSetVolumeForSwitchDeviceFuzzTest()
{
    static const vector<string> testNetworkIds = {
        "LocalDevice",
        "RemoteDevice",
        "NetworkDevice1",
    };
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioDeviceDescriptor deviceDescriptor;
    deviceDescriptor.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    deviceDescriptor.networkId_ = testNetworkIds[index % testNetworkIds.size()];

    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    audioAdapterManager->SetVolumeForSwitchDevice(deviceDescriptor);
}

void AudioAdapterManagerSaveRingerModeInfoFuzzTest()
{
    AudioRingerMode ringMode = g_fuzzUtils.GetData<AudioRingerMode>();

    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    audioAdapterManager->SaveRingerModeInfo(ringMode, "test", "invocationTimeTest");
}

void AudioAdapterManagerGetSinkIdInfoAndIdTypeFuzzTest()
{
    static const vector<string> testAdapterNames = {
        "primary",
        "a2dp",
        "testAdapter",
    };
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    std::shared_ptr<AudioPipeInfo> pipeInfo = std::make_shared<AudioPipeInfo>();
    CHECK_AND_RETURN(pipeInfo != nullptr);
    pipeInfo->adapterName_ = testAdapterNames[index % testAdapterNames.size()];
    pipeInfo->routeFlag_ = g_fuzzUtils.GetData<uint32_t>();
    std::string idInfo;
    HdiIdType idType;

    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    audioAdapterManager->GetSinkIdInfoAndIdType(pipeInfo, idInfo, idType);
}

void AudioAdapterManagerOpenNotPaAudioPortFuzzTest()
{
    std::shared_ptr<AudioPipeInfo> pipeInfo = std::make_shared<AudioPipeInfo>();
    CHECK_AND_RETURN(pipeInfo != nullptr);
    pipeInfo->pipeRole_ = g_fuzzUtils.GetData<AudioPipeRole>();
    pipeInfo->routeFlag_ = g_fuzzUtils.GetData<uint32_t>();
    uint32_t paIndex = 0;

    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    audioAdapterManager->audioServerProxy_ = AudioServerProxy::GetInstance().GetAudioServerProxy();
    audioAdapterManager->OpenNotPaAudioPort(pipeInfo, paIndex);
}

void AudioAdapterManagerSetAudioVolumeFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    float volumeDb = g_fuzzUtils.GetData<float>();
    std::shared_ptr<AudioDeviceDescriptor> device = std::make_shared<AudioDeviceDescriptor>();
    CHECK_AND_RETURN(device != nullptr);
    device->deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    device->networkId_ = "testNetworkId";
    audioAdapterManager_->volumeDataExtMaintainer_.clear();
    audioAdapterManager_->volumeDataExtMaintainer_.insert({device->GetKey(), std::make_shared<VolumeDataMaintainer>()});
    audioAdapterManager_->SetAudioVolume(streamType, volumeDb);
    audioAdapterManager_->SetAudioVolume(device, streamType, volumeDb);
}

void AudioAdapterManagerNotifyAccountsChangedFuzzTest()
{
    int id = g_fuzzUtils.GetData<int>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->NotifyAccountsChanged(id);
}

void AudioAdapterManagerDoRestoreDataFuzzTest()
{
    AudioAdapterManager::GetInstance().DoRestoreData();
    AudioAdapterManager::GetInstance().GetSafeVolumeLevel();
    AudioAdapterManager::GetInstance().GetSafeVolumeTimeout();
}

void AudioAdapterManagerSetFirstBootFuzzTest()
{
    bool isFirst = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->SetFirstBoot(isFirst);
}

void AudioAdapterManagerSafeVolumeDumpFuzzTest()
{
    std::string dumpString = "test";
    AudioAdapterManager::GetInstance().SafeVolumeDump(dumpString);
}

void AudioAdapterManagerSetVgsVolumeSupportedFuzzTest()
{
    bool isVgsSupported = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager->SetVgsVolumeSupported(isVgsSupported);
    audioAdapterManager->IsVgsVolumeSupported();
}

void AudioAdapterManagerUpdateVolumeForLowLatencyFuzzTest()
{
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager->UpdateVolumeForLowLatency();
}

void AudioAdapterManagerUpdateSinkArgsFuzzTest()
{
    AudioModuleInfo info;
    info.name = "hello";
    info.adapterName = "world";
    info.className = "CALSS";
    info.fileName = "sink.so";
    info.sinkLatency = "300ms";
    info.networkId = "ASD**G124";
    info.deviceType = "AE00";
    info.extra = "1:13:2";
    info.needEmptyChunk = g_fuzzUtils.GetData<bool>();
    std::string ret {};
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->UpdateSinkArgs(info, ret);
}

void AudioAdapterManagerGetModuleArgsFuzzTest()
{
    vector<std::string_view> testLib = {
        AudioAdapterManager::HDI_SINK,
        AudioAdapterManager::SPLIT_STREAM_SINK,
        AudioAdapterManager::HDI_SOURCE,
        AudioAdapterManager::PIPE_SINK,
        AudioAdapterManager::PIPE_SOURCE,
        AudioAdapterManager::CLUSTER_SINK,
        AudioAdapterManager::EFFECT_SINK,
        AudioAdapterManager::INNER_CAPTURER_SINK,
        AudioAdapterManager::RECEIVER_SINK,
    };
    AudioModuleInfo info;
    info.name = "hello";
    info.adapterName = "world";
    info.className = "CALSS";
    info.fileName = "sink.so";
    info.sinkLatency = "300ms";
    info.networkId = "ASD**G124";
    info.deviceType = "AE00";
    info.extra = "1:13:2";
    info.lib = testLib[g_fuzzUtils.GetData<uint32_t>() % testLib.size()];
    info.needEmptyChunk = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetModuleArgs(info);
}

void AudioAdapterManagerGetHdiSinkIdInfoFuzzTest()
{
    vector<string> testClassName = {
        "CALSS",
        "remote",
    };
    AudioModuleInfo info;
    info.name = "hello";
    info.adapterName = "world";
    info.className = testClassName[g_fuzzUtils.GetData<uint32_t>() % testClassName.size()];
    info.fileName = "sink.so";
    info.sinkLatency = "300ms";
    info.networkId = "ASD**G124";
    info.deviceType = "AE00";
    info.extra = "1:13:2";
    info.needEmptyChunk = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetHdiSinkIdInfo(info);
}

void AudioAdapterManagerGetHdiSourceIdInfoFuzzTest()
{
    vector<string> testClassName = {
        "primary",
        "CALSS",
        "remote",
    };
    AudioModuleInfo info;
    info.name = "hello";
    info.adapterName = "world";
    info.className = testClassName[g_fuzzUtils.GetData<uint32_t>() % testClassName.size()];
    info.fileName = "sink.so";
    info.sinkLatency = "300ms";
    info.networkId = "ASD**G124";
    info.deviceType = "AE00";
    info.extra = "1:13:2";
    info.sourceType = "SOURCE_TYPE_WAKEUP";
    info.needEmptyChunk = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetHdiSourceIdInfo(info);
}

void AudioAdapterManagerGetVolumeKeyForKvStoreFuzzTest()
{
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetVolumeKeyForKvStore(deviceType, streamType);
}

void AudioAdapterManagerGetStreamIDByTypeFuzzTest()
{
    vector<string> testStreamTypes = {
        "music",
        "ring",
        "voice_call",
        "system",
        "notification",
        "alarm",
        "voice_assistant",
        "accessibility",
        "ultrasonic",
        "camcorder",
    };
    std::string streamType = testStreamTypes[g_fuzzUtils.GetData<uint32_t>() % testStreamTypes.size()];
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetStreamIDByType(streamType);
}

void AudioAdapterManagerGetDeviceCategoryFuzzTest()
{
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetDeviceCategory(deviceType);
}

void AudioAdapterManagerDeleteAudioPolicyKvStoreFuzzTest()
{
    bool isFirstBoot = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->InitAudioPolicyKvStore(isFirstBoot);
    audioAdapterManager->DeleteAudioPolicyKvStore();
}

void AudioAdapterManagerUpdateSafeVolumeFuzzTest()
{
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager->UpdateSafeVolume();
}

void AudioAdapterManagerInitVolumeMapFuzzTest()
{
    bool isFirstBoot = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->InitVolumeMap(isFirstBoot);
}

void AudioAdapterManagerGetDefaultVolumeLevelFuzzTest()
{
    std::unordered_map<AudioStreamType, int32_t> volumeLevelMapTemp;
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    int32_t volumeLevel = g_fuzzUtils.GetData<int32_t>();
    volumeLevelMapTemp.insert({streamType, volumeLevel});
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    AudioVolumeType volumeType = g_fuzzUtils.GetData<AudioVolumeType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetDefaultVolumeLevel(volumeLevelMapTemp, volumeType, deviceType);
}

void AudioAdapterManagerResetRemoteCastDeviceVolumeFuzzTest()
{
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->defaultVolumeTypeList_.push_back(streamType);
    audioAdapterManager->ResetRemoteCastDeviceVolume();
}

void AudioAdapterManagerInitRingerModeFuzzTest()
{
    bool isFirstBoot = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->isNeedCopyRingerModeData_ = g_fuzzUtils.GetData<bool>();
    audioAdapterManager->ReInitKVStore();
    audioAdapterManager->InitRingerMode(isFirstBoot);
}

void AudioAdapterManagerHandleDistributedVolumeFuzzTest()
{
    AudioDeviceDescriptor deviceDescriptor;
    deviceDescriptor.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    deviceDescriptor.networkId_ = "testNetworkId";
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->HandleDistributedVolume(streamType);
}

void AudioAdapterManagerInitMuteStatusMapFuzzTest()
{
    bool isFirstBoot = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->InitMuteStatusMap(isFirstBoot);
}

void AudioAdapterManagerCheckAndDealMuteStatusFuzzTest()
{
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->currentActiveDevice_.deviceType_ = deviceType;
    audioAdapterManager->CheckAndDealMuteStatus(deviceType, streamType);
}

void AudioAdapterManagerSetVolumeCallbackAfterCloneFuzzTest()
{
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->defaultVolumeTypeList_.push_back(streamType);
    audioAdapterManager->SetVolumeCallbackAfterClone();
}

void AudioAdapterManagerCloneMuteStatusMapFuzzTest()
{
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager->CloneMuteStatusMap();
}

void AudioAdapterManagerLoadMuteStatusMapFuzzTest()
{
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->isNeedCopyMuteData_ = g_fuzzUtils.GetData<bool>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    audioAdapterManager->defaultVolumeTypeList_.push_back(streamType);
    audioAdapterManager->LoadMuteStatusMap();
}

void AudioAdapterManagerInitSafeStatusFuzzTest()
{
    bool isFirstBoot = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->InitSafeStatus(isFirstBoot);
}

void AudioAdapterManagerInitSafeTimeFuzzTest()
{
    bool isFirstBoot = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->InitSafeTime(isFirstBoot);
}

void AudioAdapterManagerConvertSafeTimeFuzzTest()
{
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->safeActiveTime_ = g_fuzzUtils.GetData<int64_t>();
    audioAdapterManager->safeActiveBtTime_ = g_fuzzUtils.GetData<int64_t>();
    audioAdapterManager->ConvertSafeTime();
}

void AudioAdapterManagerGetCurrentDeviceSafeStatusFuzzTest()
{
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetCurrentDeviceSafeStatus(deviceType);
}

void AudioAdapterManagerGetCurentDeviceSafeTimeFuzzTest()
{
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetCurentDeviceSafeTime(deviceType);
}

void AudioAdapterManagerGetRestoreVolumeLevelFuzzTest()
{
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetRestoreVolumeLevel(deviceType);
}

void AudioAdapterManagerSetDeviceSafeStatusFuzzTest()
{
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    SafeStatus status = g_fuzzUtils.GetData<SafeStatus>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->SetDeviceSafeStatus(deviceType, status);
}

void AudioAdapterManagerSetDeviceSafeTimeFuzzTest()
{
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    int64_t time = g_fuzzUtils.GetData<int64_t>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->SetDeviceSafeTime(deviceType, time);
}

void AudioAdapterManagerSetRestoreVolumeLevelFuzzTest()
{
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    int32_t volume = g_fuzzUtils.GetData<int32_t>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->SetRestoreVolumeLevel(deviceType, volume);
}

void AudioAdapterManagerGetMuteKeyForKvStoreFuzzTest()
{
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetMuteKeyForKvStore(deviceType, streamType);
}

void AudioAdapterManagerSetSystemSoundUriFuzzTest()
{
    static const std::vector<std::string> testSystemSoundKey = {
        "ringtone_for_sim_card_0",
        "ringtone_for_sim_card_1",
        "system_tone_for_sim_card_0",
        "system_tone_for_sim_card_1",
        "system_tone_for_notification"
    };
    std::string key = testSystemSoundKey[g_fuzzUtils.GetData<uint32_t>() % testSystemSoundKey.size()];
    std::string uri = "test";
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->systemSoundUriMap_.insert({key, uri});
    audioAdapterManager->SetSystemSoundUri(key, uri);
}

void AudioAdapterManagerGetSystemSoundUriFuzzTest()
{
    static const std::vector<std::string> testSystemSoundKey = {
        "ringtone_for_sim_card_0",
        "ringtone_for_sim_card_1",
        "system_tone_for_sim_card_0",
        "system_tone_for_sim_card_1",
        "system_tone_for_notification"
    };
    std::string key = testSystemSoundKey[g_fuzzUtils.GetData<uint32_t>() % testSystemSoundKey.size()];
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->systemSoundUriMap_.insert({key, "testUri"});
    audioAdapterManager->GetSystemSoundUri(key);
}

void AudioAdapterManagerGetSystemVolumeInDbFuzzTest()
{
    int32_t volumeLevel = g_fuzzUtils.GetData<int32_t>();
    AudioVolumeType volumeType =  g_fuzzUtils.GetData<AudioVolumeType>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    audioAdapterManager->useNonlinearAlgo_ = g_fuzzUtils.GetData<bool>();
    audioAdapterManager->GetSystemVolumeInDb(volumeType, volumeLevel, deviceType);
}

void AudioAdapterManagerGetPositionInVolumePointsFuzzTest()
{
    VolumePoint volumePoint1;
    volumePoint1.index = g_fuzzUtils.GetData<uint32_t>();
    volumePoint1.dbValue = g_fuzzUtils.GetData<int32_t>();
    VolumePoint volumePoint2;
    volumePoint2.index = g_fuzzUtils.GetData<uint32_t>();
    volumePoint2.dbValue = g_fuzzUtils.GetData<uint32_t>();
    std::vector<VolumePoint> volumePoints;
    volumePoints.push_back(volumePoint1);
    volumePoints.push_back(volumePoint2);
    int32_t idx = 0;
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetPositionInVolumePoints(volumePoints, idx);
}

void AudioAdapterManagerInitVolumeMapIndexFuzzTest()
{
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->defaultVolumeTypeList_.push_back(streamType);
    audioAdapterManager->InitVolumeMapIndex();
}

void AudioAdapterManagerUpdateVolumeMapIndexFuzzTest()
{
    VolumePoint volumePoint;
    volumePoint.index = g_fuzzUtils.GetData<uint32_t>();
    volumePoint.dbValue = g_fuzzUtils.GetData<uint32_t>();
    std::vector<VolumePoint> volumePoints;
    volumePoints.push_back(volumePoint);
    std::shared_ptr<DeviceVolumeInfo> deviceVolumeInfoPtr = std::make_shared<DeviceVolumeInfo>();
    CHECK_AND_RETURN(deviceVolumeInfoPtr != nullptr);
    deviceVolumeInfoPtr->deviceType = g_fuzzUtils.GetData<DeviceVolumeType>();
    deviceVolumeInfoPtr->volumePoints = volumePoints;
    DeviceVolumeInfoMap deviceVolumeInfoMap;
    deviceVolumeInfoMap.insert({deviceVolumeInfoPtr->deviceType, deviceVolumeInfoPtr});

    std::shared_ptr<StreamVolumeInfo> streamVolumeInfoPtr = std::make_shared<StreamVolumeInfo>();
    CHECK_AND_RETURN(streamVolumeInfoPtr != nullptr);
    streamVolumeInfoPtr->streamType = g_fuzzUtils.GetData<AudioStreamType>();
    streamVolumeInfoPtr->maxLevel = g_fuzzUtils.GetData<int>();
    streamVolumeInfoPtr->minLevel = g_fuzzUtils.GetData<int>();
    streamVolumeInfoPtr->defaultLevel = g_fuzzUtils.GetData<int>();
    streamVolumeInfoPtr->deviceVolumeInfos = deviceVolumeInfoMap;
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->streamVolumeInfos_.insert({streamVolumeInfoPtr->streamType, streamVolumeInfoPtr});
    audioAdapterManager->UpdateVolumeMapIndex();
}

void AudioAdapterManagerSetAbsVolumeSceneFuzzTest()
{
    bool isAbsVolumeScene = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager->SetAbsVolumeScene(isAbsVolumeScene);
}

void AudioAdapterManagerSetAbsVolumeMuteFuzzTest()
{
    bool mute = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager->SetAbsVolumeMute(mute);
}

void AudioAdapterManagerSetAppVolumeLevelFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    audioAdapterManager_->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    int32_t appUid = g_fuzzUtils.GetData<int32_t>();
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    audioAdapterManager_->SetAppVolumeLevel(appUid, randIntValue);
}

void AudioAdapterManagerSetAdjustVolumeForZoneFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    int32_t zoneId = g_fuzzUtils.GetData<int32_t>();
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> devices;
    std::shared_ptr<AudioDeviceDescriptor> desc =
        std::make_shared<AudioDeviceDescriptor>(DEVICE_TYPE_REMOTE_CAST, OUTPUT_DEVICE);
    CHECK_AND_RETURN(desc != nullptr);
    desc->networkId_ = "LocalDevice";
    devices.push_back(desc);
    AudioZoneService::GetInstance().BindDeviceToAudioZone(zoneId, devices);
    AudioConnectedDevice::GetInstance().AddConnectedDevice(desc);
    AudioZoneService::GetInstance().UpdateDeviceFromGlobalForAllZone(desc);
    audioAdapterManager_->volumeDataExtMaintainer_.clear();
    audioAdapterManager_->volumeDataExtMaintainer_.insert({desc->GetKey(), std::make_shared<VolumeDataMaintainer>()});
    audioAdapterManager_->SetAdjustVolumeForZone(zoneId);
}

void AudioAdapterManagerSetZoneMuteFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    int32_t zoneId = g_fuzzUtils.GetData<int32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    StreamUsage streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    bool mute = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetZoneMute(zoneId, streamType, mute, streamUsage, deviceType);
}

void AudioAdapterManagerGetZoneMuteFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    int32_t zoneId = g_fuzzUtils.GetData<int32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    audioAdapterManager_->GetZoneMute(zoneId, streamType);
}

void AudioAdapterManagerGetZoneVolumeLevelFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    int32_t zoneId = g_fuzzUtils.GetData<int32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> devices;
    std::shared_ptr<AudioDeviceDescriptor> desc =
        std::make_shared<AudioDeviceDescriptor>(DEVICE_TYPE_REMOTE_CAST, OUTPUT_DEVICE);
    CHECK_AND_RETURN(desc != nullptr);
    desc->networkId_ = "LocalDevice";
    devices.push_back(desc);
    AudioZoneService::GetInstance().BindDeviceToAudioZone(zoneId, devices);
    AudioConnectedDevice::GetInstance().AddConnectedDevice(desc);
    AudioZoneService::GetInstance().UpdateDeviceFromGlobalForAllZone(desc);
    audioAdapterManager_->volumeDataExtMaintainer_.clear();
    audioAdapterManager_->volumeDataExtMaintainer_.insert({desc->GetKey(), std::make_shared<VolumeDataMaintainer>()});
    audioAdapterManager_->GetZoneVolumeLevel(zoneId, streamType);
}

void AudioAdapterManagerSetZoneVolumeLevelFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    int32_t zoneId = g_fuzzUtils.GetData<int32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    int32_t volumeLevel = g_fuzzUtils.GetData<int32_t>();
    audioAdapterManager_->SetZoneVolumeLevel(zoneId, streamType, volumeLevel);
}

void AudioAdapterManagerGetDeviceVolumeFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager_->GetDeviceVolume(deviceType, streamType);
}

void AudioAdapterManagerHandleRingerModeFuzzTest()
{
    AudioRingerMode ringMode = g_fuzzUtils.GetData<AudioRingerMode>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->HandleRingerMode(ringMode);
}

void AudioAdapterManagerSetAppVolumeDbFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    int32_t appUid = g_fuzzUtils.GetData<int32_t>();
    audioAdapterManager_->SetAppVolumeDb(appUid);
}

void AudioAdapterManagerSetAppVolumeMutedDBFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    int32_t appUid = g_fuzzUtils.GetData<int32_t>();
    bool muted = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetAppVolumeMutedDB(appUid, muted);
}

void AudioAdapterManagerGetAppVolumeLevelFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    int32_t appUid = g_fuzzUtils.GetData<int32_t>();
    int32_t volumeLevel = g_fuzzUtils.GetData<int32_t>();
    audioAdapterManager_->GetAppVolumeLevel(appUid, volumeLevel);
}

void AudioAdapterManagerSetPersistMicMuteStateFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    bool mute = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetPersistMicMuteState(mute);
}

void AudioAdapterManagerSetSourceOutputStreamMuteFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    int32_t uid = g_fuzzUtils.GetData<int32_t>();
    bool setMute = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetSourceOutputStreamMute(uid, setMute);
}

void AudioAdapterManagerGetStreamMuteFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    std::shared_ptr<AudioDeviceDescriptor> device = std::make_shared<AudioDeviceDescriptor>();
    CHECK_AND_RETURN(device != nullptr);
    audioAdapterManager_->volumeDataExtMaintainer_.clear();
    audioAdapterManager_->volumeDataExtMaintainer_.insert({device->GetKey(), std::make_shared<VolumeDataMaintainer>()});
    audioAdapterManager_->GetStreamMute(streamType);
    audioAdapterManager_->GetStreamMute(device, streamType);
}

void AudioAdapterManagerGetAppMuteFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    if (!g_isInit) {
        g_isInit = audioAdapterManager_->Init();
    }
    int32_t appUid = g_fuzzUtils.GetData<int32_t>();
    audioAdapterManager_->GetAppMute(appUid);
}

void AudioAdapterManagerSetRingerModeFuzzTest()
{
    AudioRingerMode ringMode = g_fuzzUtils.GetData<AudioRingerMode>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->SetRingerMode(ringMode);
}

void AudioAdapterManagerSetDataShareReadyFuzzTest()
{
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->SetDataShareReady(std::atomic<bool>(g_fuzzUtils.GetData<bool>()));
}

void AudioAdapterManagerSetVolumeDbFuzzTest()
{
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    std::shared_ptr<AudioDeviceDescriptor> device = std::make_shared<AudioDeviceDescriptor>();
    CHECK_AND_RETURN(device != nullptr);
    audioAdapterManager->volumeDataExtMaintainer_.insert({device->GetKey(), std::make_shared<VolumeDataMaintainer>()});
    audioAdapterManager->useNonlinearAlgo_ = g_fuzzUtils.GetData<bool>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    audioAdapterManager->SetVolumeDb(device, streamType);
}

void AudioAdapterManagerResetOffloadSessionIdFuzzTest()
{
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->offloadSessionID_ = g_fuzzUtils.GetData<uint32_t>();
    audioAdapterManager->ResetOffloadSessionId();
}

void AudioAdapterManagerSetStreamMuteInternalFuzzTest()
{
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    std::shared_ptr<AudioDeviceDescriptor> device = std::make_shared<AudioDeviceDescriptor>();
    CHECK_AND_RETURN(device != nullptr);
    device->deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    bool mute = g_fuzzUtils.GetData<bool>();
    StreamUsage streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager->handler_ = make_shared<AudioAdapterManagerHandler>();
    audioAdapterManager->volumeDataExtMaintainer_.insert({device->GetKey(), std::make_shared<VolumeDataMaintainer>()});
    audioAdapterManager->SetStreamMuteInternal(device, streamType, mute, streamUsage, deviceType);
}

void AudioAdapterManagerGetStreamVolumeFuzzTest()
{
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    std::shared_ptr<AudioDeviceDescriptor> device = std::make_shared<AudioDeviceDescriptor>();
    CHECK_AND_RETURN(device != nullptr);
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    AudioStreamType streamForVolumeMap = VolumeUtils::GetVolumeTypeFromStreamType(streamType);
    shared_ptr<VolumeDataMaintainer> volumeDataMaintainer = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainer != nullptr);
    volumeDataMaintainer->volumeLevelMap_.insert({streamForVolumeMap, g_fuzzUtils.GetData<int32_t>()});
    audioAdapterManager->volumeDataExtMaintainer_.insert({device->GetKey(), volumeDataMaintainer});
    audioAdapterManager->GetStreamVolume(device, streamType);
}

void AudioAdapterManagerGetStreamMuteInternalFuzzTest()
{
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    std::shared_ptr<AudioDeviceDescriptor> device = std::make_shared<AudioDeviceDescriptor>();
    CHECK_AND_RETURN(device != nullptr);
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    AudioStreamType streamForVolumeMap = VolumeUtils::GetVolumeTypeFromStreamType(streamType);
    shared_ptr<VolumeDataMaintainer> volumeDataMaintainer = std::make_shared<VolumeDataMaintainer>();
    CHECK_AND_RETURN(volumeDataMaintainer != nullptr);
    volumeDataMaintainer->volumeLevelMap_.insert({streamForVolumeMap, g_fuzzUtils.GetData<int32_t>()});
    audioAdapterManager->volumeDataExtMaintainer_.insert({device->GetKey(), volumeDataMaintainer});
    audioAdapterManager->GetStreamMuteInternal(device, streamType);
}

void AudioAdapterManagerGetAllDeviceVolumeInfoFuzzTest()
{
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    bool isPCVolumeEnable = g_fuzzUtils.GetData<bool>();
    VolumeUtils::SetPCVolumeEnable(isPCVolumeEnable);
    audioAdapterManager->GetAllDeviceVolumeInfo(deviceType, streamType);
}

vector<TestFuncs> g_testFuncs = {
    AudioAdapterManagerIsAppVolumeMuteFuzzTest,
    AudioAdapterManagerSaveSpecifiedDeviceVolumeFuzzTest,
    AudioAdapterManagerHandleStreamMuteStatusFuzzTest,
    AudioAdapterManagerSetOffloadVolumeFuzzTest,
    AudioAdapterManagerSetOffloadSessionIdFuzzTest,
    AudioAdapterManagerHandleKvDataFuzzTest,
    AudioAdapterManagerReInitKVStoreFuzzTest,
    AudioAdapterManagerSaveRingtoneVolumeToLocalFuzzTest,
    AudioAdapterManagerUpdateSafeVolumeByS4FuzzTest,
    AudioAdapterManagerSetAppVolumeMutedFuzzTest,
    AudioAdapterManagerSetSystemVolumeLevelFuzzTest,
    AudioAdapterManagerSetDoubleRingVolumeDbFuzzTest,
    AudioAdapterManagerSetInnerStreamMuteFuzzTest,
    AudioAdapterManagerSetStreamMuteFuzzTest,
    AudioAdapterManagerSetSinkMuteFuzzTest,
    AudioAdapterManagerSelectDeviceFuzzTest,
    AudioAdapterManagerSetDeviceActiveFuzzTest,
    AudioAdapterManagerAdjustBluetoothVoiceAssistantVolumeFuzzTest,
    AudioAdapterManagerSaveRingerModeInfoFuzzTest,
    AudioAdapterManagerGetSinkIdInfoAndIdTypeFuzzTest,
    AudioAdapterManagerOpenNotPaAudioPortFuzzTest,
    AudioAdapterManagerUpdateSinkArgsFuzzTest,
    AudioAdapterManagerGetModuleArgsFuzzTest,
    AudioAdapterManagerGetHdiSinkIdInfoFuzzTest,
    AudioAdapterManagerGetHdiSourceIdInfoFuzzTest,
    AudioAdapterManagerGetVolumeKeyForKvStoreFuzzTest,
    AudioAdapterManagerGetStreamIDByTypeFuzzTest,
    AudioAdapterManagerGetDeviceCategoryFuzzTest,
    AudioAdapterManagerDeleteAudioPolicyKvStoreFuzzTest,
    AudioAdapterManagerUpdateSafeVolumeFuzzTest,
    AudioAdapterManagerInitVolumeMapFuzzTest,
    AudioAdapterManagerGetDefaultVolumeLevelFuzzTest,
    AudioAdapterManagerResetRemoteCastDeviceVolumeFuzzTest,
    AudioAdapterManagerInitRingerModeFuzzTest,
    AudioAdapterManagerHandleDistributedVolumeFuzzTest,
    AudioAdapterManagerInitMuteStatusMapFuzzTest,
    AudioAdapterManagerCheckAndDealMuteStatusFuzzTest,
    AudioAdapterManagerSetVolumeCallbackAfterCloneFuzzTest,
    AudioAdapterManagerCloneMuteStatusMapFuzzTest,
    AudioAdapterManagerLoadMuteStatusMapFuzzTest,
    AudioAdapterManagerInitSafeStatusFuzzTest,
    AudioAdapterManagerInitSafeTimeFuzzTest,
    AudioAdapterManagerConvertSafeTimeFuzzTest,
    AudioAdapterManagerGetCurrentDeviceSafeStatusFuzzTest,
    AudioAdapterManagerGetCurentDeviceSafeTimeFuzzTest,
    AudioAdapterManagerGetRestoreVolumeLevelFuzzTest,
    AudioAdapterManagerSetDeviceSafeStatusFuzzTest,
    AudioAdapterManagerSetDeviceSafeTimeFuzzTest,
    AudioAdapterManagerSetRestoreVolumeLevelFuzzTest,
    AudioAdapterManagerGetMuteKeyForKvStoreFuzzTest,
    AudioAdapterManagerSetSystemSoundUriFuzzTest,
    AudioAdapterManagerGetSystemSoundUriFuzzTest,
    AudioAdapterManagerGetSystemVolumeInDbFuzzTest,
    AudioAdapterManagerGetPositionInVolumePointsFuzzTest,
    AudioAdapterManagerInitVolumeMapIndexFuzzTest,
    AudioAdapterManagerUpdateVolumeMapIndexFuzzTest,
    AudioAdapterManagerSetAbsVolumeSceneFuzzTest,
    AudioAdapterManagerSetAbsVolumeMuteFuzzTest,
    AudioAdapterManagerSetAudioVolumeFuzzTest,
    AudioAdapterManagerNotifyAccountsChangedFuzzTest,
    AudioAdapterManagerDoRestoreDataFuzzTest,
    AudioAdapterManagerSetFirstBootFuzzTest,
    AudioAdapterManagerSafeVolumeDumpFuzzTest,
    AudioAdapterManagerSetVgsVolumeSupportedFuzzTest,
    AudioAdapterManagerSetAppVolumeLevelFuzzTest,
    AudioAdapterManagerSetAdjustVolumeForZoneFuzzTest,
    AudioAdapterManagerSetZoneMuteFuzzTest,
    AudioAdapterManagerSetZoneMuteFuzzTest,
    AudioAdapterManagerGetZoneMuteFuzzTest,
    AudioAdapterManagerGetZoneVolumeLevelFuzzTest,
    AudioAdapterManagerSetZoneVolumeLevelFuzzTest,
    AudioAdapterManagerGetDeviceVolumeFuzzTest,
    AudioAdapterManagerHandleRingerModeFuzzTest,
    AudioAdapterManagerSetAppVolumeDbFuzzTest,
    AudioAdapterManagerSetAppVolumeMutedDBFuzzTest,
    AudioAdapterManagerGetAppVolumeLevelFuzzTest,
    AudioAdapterManagerSetPersistMicMuteStateFuzzTest,
    AudioAdapterManagerSetSourceOutputStreamMuteFuzzTest,
    AudioAdapterManagerGetStreamMuteFuzzTest,
    AudioAdapterManagerGetAppMuteFuzzTest,
    AudioAdapterManagerSetRingerModeFuzzTest,
    AudioAdapterManagerSetDataShareReadyFuzzTest,
    AudioAdapterManagerSetVolumeDbFuzzTest,
    AudioAdapterManagerResetOffloadSessionIdFuzzTest,
    AudioAdapterManagerSetStreamMuteInternalFuzzTest,
    AudioAdapterManagerGetStreamVolumeFuzzTest,
    AudioAdapterManagerGetStreamMuteInternalFuzzTest,
    AudioAdapterManagerGetAllDeviceVolumeInfoFuzzTest,
};

} // namespace AudioStandard
} // namesapce OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    OHOS::AudioStandard::g_fuzzUtils.fuzzTest(data, size, OHOS::AudioStandard::g_testFuncs);
    return 0;
}