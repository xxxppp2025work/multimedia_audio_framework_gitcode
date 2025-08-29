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

#include "../fuzz_utils.h"

#include "audio_adapter_manager.h"
#include "audio_server_proxy.h"

using namespace std;

namespace OHOS {
namespace AudioStandard {

typedef void (*TestPtr)();

FuzzUtils &g_fuzzUtils = FuzzUtils::GetInstance();
static const size_t FUZZ_INPUT_SIZE_THRESHOLD = 10;

static AudioAdapterManager *audioAdapterManager_;

void AudioVolumeManagerIsAppVolumeMuteFuzzTest()
{
    int32_t appUid = g_fuzzUtils.GetData<int32_t>();
    bool owned = g_fuzzUtils.GetData<bool>();
    bool isMute = g_fuzzUtils.GetData<bool>();
    AudioAdapterManager::GetInstance().IsAppVolumeMute(appUid, owned, isMute);
}

void AudioVolumeManagerSaveSpecifiedDeviceVolumeFuzzTest()
{
    AudioAdapterManager::GetInstance().Init();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    int32_t volumeLevel = g_fuzzUtils.GetData<int32_t>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    AudioAdapterManager::GetInstance().GetMinVolumeLevel(streamType);
    AudioAdapterManager::GetInstance().GetMaxVolumeLevel(streamType);
    AudioAdapterManager::GetInstance().SaveSpecifiedDeviceVolume(streamType, volumeLevel, deviceType);
}

void AudioVolumeManagerHandleStreamMuteStatusFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    bool mute = g_fuzzUtils.GetData<bool>();
    StreamUsage streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    AudioAdapterManager::GetInstance().HandleStreamMuteStatus(streamType, mute, streamUsage, deviceType);
}

void AudioVolumeManagerSetOffloadVolumeFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    float volumeDb = g_fuzzUtils.GetData<float>();
    audioAdapterManager_->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager_->SetOffloadVolume(streamType, volumeDb, "offload");
}

void AudioVolumeManagerSetOffloadSessionIdFuzzTest()
{
    uint32_t sessionId = g_fuzzUtils.GetData<uint32_t>();
    AudioAdapterManager::GetInstance().SetOffloadSessionId(sessionId);
}

void AudioVolumeManagerConnectServiceAdapterFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    audioAdapterManager_->ConnectServiceAdapter();
    audioAdapterManager_->InitKVStore();
}

void AudioVolumeManagerHandleKvDataFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    audioAdapterManager_->isNeedCopySystemUrlData_ = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->isNeedCopyVolumeData_ = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->isNeedCopyMuteData_ = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->isNeedCopyRingerModeData_ = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->isNeedCopySystemUrlData_ = g_fuzzUtils.GetData<bool>();
    bool isFirstBoot = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->HandleKvData(isFirstBoot);
}

void AudioVolumeManagerReInitKVStoreFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    audioAdapterManager_->ReInitKVStore();
}

void AudioVolumeManagerSaveRingtoneVolumeToLocalFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    int32_t volumeLevel = g_fuzzUtils.GetData<int32_t>();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioVolumeType volumeType = g_fuzzUtils.GetData<AudioStreamType>();
    audioAdapterManager_->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager_->SaveRingtoneVolumeToLocal(volumeType, volumeLevel);
}

void AudioVolumeManagerUpdateSafeVolumeByS4FuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    audioAdapterManager_->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager_->UpdateSafeVolumeByS4();
}

void AudioVolumeManagerSetAppVolumeMutedFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    bool muted = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetAppVolumeLevel(randIntValue, g_fuzzUtils.GetData<bool>());
    audioAdapterManager_->SetAppVolumeMuted(randIntValue, muted);
}

void AudioVolumeManagerSetSystemVolumeLevelFuzzTest()
{
    vector<string> testNetworkIds = {
        "LocalDevice",
        "RemoteDevice",
        "NetworkDevice1",
    };
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioDeviceDescriptor deviceDescriptor;
    deviceDescriptor.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    deviceDescriptor.networkId_ = testNetworkIds[index % testNetworkIds.size()];
    int32_t testVolumeLevel = g_fuzzUtils.GetData<int32_t>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->SetActiveDeviceDescriptor(deviceDescriptor);
    audioAdapterManager->SetSystemVolumeLevel(STREAM_MUSIC, testVolumeLevel);
}

void AudioVolumeManagerSetDoubleRingVolumeDbFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    audioAdapterManager_->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    int32_t volumeLevel = g_fuzzUtils.GetData<int32_t>();
    audioAdapterManager_->useNonlinearAlgo_ = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetDoubleRingVolumeDb(streamType, volumeLevel);
}

void AudioVolumeManagerSetInnerStreamMuteFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    StreamUsage streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    bool mute = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetInnerStreamMute(streamType, mute, streamUsage);
}

void AudioVolumeManagerSetStreamMuteFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    StreamUsage streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    bool mute = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetStreamMute(streamType, mute, streamUsage, deviceType);
}

void AudioVolumeManagerSetSinkMuteFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    vector<string> sinkNames = {
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

void AudioVolumeManagerSelectDeviceFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    DeviceRole deviceRole = g_fuzzUtils.GetData<DeviceRole>();
    InternalDeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager_->SelectDevice(deviceRole, deviceType, "test");
}

void AudioVolumeManagerSetDeviceActiveFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    InternalDeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    bool active = g_fuzzUtils.GetData<bool>();
    DeviceFlag flag = g_fuzzUtils.GetData<DeviceFlag>();
    audioAdapterManager_->SetDeviceActive(deviceType, "test", active, flag);
}

void AudioVolumeManagerAdjustBluetoothVoiceAssistantVolumeFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    InternalDeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    bool isA2dpSwitchToSco = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->AdjustBluetoothVoiceAssistantVolume(deviceType, isA2dpSwitchToSco);
}

void AudioVolumeManagerSetVolumeForSwitchDeviceFuzzTest()
{
    vector<string> testNetworkIds = {
        "LocalDevice",
        "RemoteDevice",
        "NetworkDevice1",
    };
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioDeviceDescriptor deviceDescriptor;
    deviceDescriptor.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    deviceDescriptor.networkId_ = testNetworkIds[index % testNetworkIds.size()];

    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->SetVolumeForSwitchDevice(deviceDescriptor);
}

void AudioVolumeManagerSaveRingerModeInfoFuzzTest()
{
    vector<AudioRingerMode> testAudioRingerModers = {
        RINGER_MODE_SILENT,
        RINGER_MODE_VIBRATE,
        RINGER_MODE_NORMAL,
    };
    AudioRingerMode ringMode = g_fuzzUtils.GetData<AudioRingerMode>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->SaveRingerModeInfo(ringMode, "test", "invocationTimeTest");
}

void AudioVolumeManagerGetSinkIdInfoAndIdTypeFuzzTest()
{
    vector<string> testAdapterNames = {
        "primary",
        "a2dp",
        "testAdapter",
    };
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    std::shared_ptr<AudioPipeInfo> pipeInfo = std::make_shared<AudioPipeInfo>();
    pipeInfo->adapterName_ = testAdapterNames[index % testAdapterNames.size()];
    pipeInfo->routeFlag_ = g_fuzzUtils.GetData<uint32_t>();
    std::string idInfo;
    HdiIdType idType;

    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetSinkIdInfoAndIdType(pipeInfo, idInfo, idType);
}

void AudioVolumeManagerOpenNotPaAudioPortFuzzTest()
{
    std::shared_ptr<AudioPipeInfo> pipeInfo = std::make_shared<AudioPipeInfo>();
    CHECK_AND_RETURN(pipeInfo != nullptr);
    pipeInfo->pipeRole_ = g_fuzzUtils.GetData<AudioPipeRole>();
    pipeInfo->routeFlag_ = g_fuzzUtils.GetData<uint32_t>();
    uint32_t paIndex = 0;

    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->audioServerProxy_ = AudioServerProxy::GetInstance().GetAudioServerProxy();
    audioAdapterManager->OpenNotPaAudioPort(pipeInfo, paIndex);
}

void AudioVolumeManagerSetAudioVolumeFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    float volumeDb = g_fuzzUtils.GetData<float>();
    audioAdapterManager_->SetAudioVolume(streamType, volumeDb);
}

void AudioVolumeManagerNotifyAccountsChangedFuzzTest()
{
    int id = g_fuzzUtils.GetData<int>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->NotifyAccountsChanged(id);
}

void AudioVolumeManagerDoRestoreDataFuzzTest()
{
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->DoRestoreData();
    audioAdapterManager->GetSafeVolumeLevel();
    audioAdapterManager->GetSafeVolumeTimeout();
}

void AudioVolumeManagerSetFirstBootFuzzTest()
{
    bool isFirst = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->SetFirstBoot(isFirst);
}

void AudioVolumeManagerSafeVolumeDumpFuzzTest()
{
    std::string dumpString = "test";
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->isSafeBoot_ = g_fuzzUtils.GetData<bool>();
    audioAdapterManager->SafeVolumeDump(dumpString);
}

void AudioVolumeManagerSetVgsVolumeSupportedFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    bool isVgsSupported = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager->SetVgsVolumeSupported(isVgsSupported);
    audioAdapterManager->IsVgsVolumeSupported();
}

void AudioVolumeManagerUpdateVolumeForLowLatencyFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager->UpdateVolumeForLowLatency();
}

void AudioVolumeManagerUpdateSinkArgsFuzzTest()
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

void AudioVolumeManagerGetModuleArgsFuzzTest()
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

void AudioVolumeManagerGetHdiSinkIdInfoFuzzTest()
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

void AudioVolumeManagerGetHdiSourceIdInfoFuzzTest()
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

void AudioVolumeManagerGetVolumeKeyForKvStoreFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetVolumeKeyForKvStore(deviceType, streamType);
}

void AudioVolumeManagerGetStreamIDByTypeFuzzTest()
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

void AudioVolumeManagerGetDeviceCategoryFuzzTest()
{
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetDeviceCategory(deviceType);
}

void AudioVolumeManagerDeleteAudioPolicyKvStoreFuzzTest()
{
    bool isFirstBoot = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->InitAudioPolicyKvStore(isFirstBoot);
    audioAdapterManager->DeleteAudioPolicyKvStore();
}

void AudioVolumeManagerUpdateSafeVolumeFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager->UpdateSafeVolume();
}

void AudioVolumeManagerInitVolumeMapFuzzTest()
{
    bool isFirstBoot = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->InitVolumeMap(isFirstBoot);
}

void AudioVolumeManagerGetDefaultVolumeLevelFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    std::unordered_map<AudioStreamType, int32_t> volumeLevelMapTemp;
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    int32_t volumeLevel = g_fuzzUtils.GetData<int32_t>();
    volumeLevelMapTemp.insert({streamType, volumeLevel});
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    AudioVolumeType volumeType = g_fuzzUtils.GetData<AudioStreamType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetDefaultVolumeLevel(volumeLevelMapTemp, volumeType, deviceType);
}

void AudioVolumeManagerResetRemoteCastDeviceVolumeFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->defaultVolumeTypeList_.push_back(streamType);
    audioAdapterManager->ResetRemoteCastDeviceVolume();
}

void AudioVolumeManagerInitRingerModeFuzzTest()
{
    bool isFirstBoot = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->isNeedCopyRingerModeData_ = !isFirstBoot;
    audioAdapterManager->ReInitKVStore();
    audioAdapterManager->InitRingerMode(isFirstBoot);
}

void AudioVolumeManagerHandleDistributedVolumeFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioDeviceDescriptor deviceDescriptor;
    deviceDescriptor.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    deviceDescriptor.networkId_ = "testNetworkId";
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->HandleDistributedVolume(streamType);
}

void AudioVolumeManagerInitMuteStatusMapFuzzTest()
{
    bool isFirstBoot = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->InitMuteStatusMap(isFirstBoot);
}

void AudioVolumeManagerCheckAndDealMuteStatusFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->currentActiveDevice_.deviceType_ = deviceType;
    audioAdapterManager->CheckAndDealMuteStatus(deviceType, streamType);
}

void AudioVolumeManagerSetVolumeCallbackAfterCloneFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->defaultVolumeTypeList_.push_back(streamType);
    audioAdapterManager->SetVolumeCallbackAfterClone();
}

void AudioVolumeManagerOpenPaAudioPortFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    audioAdapterManager_->ConnectServiceAdapter();
    audioAdapterManager_->audioServerProxy_ = AudioServerProxy::GetInstance().GetAudioServerProxy();
    vector<AudioPipeRole> testAudioPipeRoles = {
        PIPE_ROLE_OUTPUT,
        PIPE_ROLE_INPUT,
        PIPE_ROLE_NONE,
    };
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    std::shared_ptr<AudioPipeInfo> pipeInfo = std::make_shared<AudioPipeInfo>();
    CHECK_AND_RETURN(pipeInfo != nullptr);
    pipeInfo->pipeRole_ = testAudioPipeRoles[index % testAudioPipeRoles.size()];
    uint32_t paIndex = 0;
    audioAdapterManager_->OpenPaAudioPort(pipeInfo, paIndex, "test");
}

void AudioVolumeManagerCloneMuteStatusMapFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager->CloneMuteStatusMap();
}

void AudioVolumeManagerLoadMuteStatusMapFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->isNeedCopyMuteData_ = g_fuzzUtils.GetData<bool>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    audioAdapterManager->defaultVolumeTypeList_.push_back(streamType);
    audioAdapterManager->LoadMuteStatusMap();
}

void AudioVolumeManagerInitSafeStatusFuzzTest()
{
    bool isFirstBoot = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->InitSafeStatus(isFirstBoot);
}

void AudioVolumeManagerInitSafeTimeFuzzTest()
{
    bool isFirstBoot = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->InitSafeTime(isFirstBoot);
}

void AudioVolumeManagerConvertSafeTimeFuzzTest()
{
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->safeActiveTime_ = g_fuzzUtils.GetData<int64_t>();
    audioAdapterManager->safeActiveBtTime_ = g_fuzzUtils.GetData<bool>();
    audioAdapterManager->ConvertSafeTime();
}

void AudioVolumeManagerGetCurrentDeviceSafeStatusFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetCurrentDeviceSafeStatus(deviceType);
}

void AudioVolumeManagerGetCurentDeviceSafeTimeFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetCurentDeviceSafeTime(deviceType);
}

void AudioVolumeManagerGetRestoreVolumeLevelFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetRestoreVolumeLevel(deviceType);
}

void AudioVolumeManagerSetDeviceSafeStatusFuzzTest()
{
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    SafeStatus status = g_fuzzUtils.GetData<SafeStatus>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->SetDeviceSafeStatus(deviceType, status);
}

void AudioVolumeManagerSetDeviceSafeTimeFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    int64_t time = g_fuzzUtils.GetData<int64_t>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->SetDeviceSafeTime(deviceType, time);
}

void AudioVolumeManagerSetRestoreVolumeLevelFuzzTest()
{
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    int32_t volume = g_fuzzUtils.GetData<int32_t>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->SetRestoreVolumeLevel(deviceType, volume);
}

void AudioVolumeManagerGetMuteKeyForKvStoreFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetMuteKeyForKvStore(deviceType, streamType);
}

void AudioVolumeManagerSetSystemSoundUriFuzzTest()
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
    audioAdapterManager->SetSystemSoundUri(key, uri);
}

void AudioVolumeManagerGetSystemSoundUriFuzzTest()
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
    audioAdapterManager->GetSystemSoundUri(key);
}

void AudioVolumeManagerGetSystemVolumeInDbFuzzTest()
{
    int32_t volumeLevel = g_fuzzUtils.GetData<int32_t>();
    AudioVolumeType volumeType =  g_fuzzUtils.GetData<AudioStreamType>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->useNonlinearAlgo_ = g_fuzzUtils.GetData<bool>();
    audioAdapterManager->GetSystemVolumeInDb(volumeType, volumeLevel, deviceType);
}

void AudioVolumeManagerGetPositionInVolumePointsFuzzTest()
{
    VolumePoint volumePoint1;
    volumePoint1.index = g_fuzzUtils.GetData<uint32_t>();
    volumePoint1.dbValue = g_fuzzUtils.GetData<bool>();
    VolumePoint volumePoint2;
    volumePoint2.index = g_fuzzUtils.GetData<uint32_t>();
    volumePoint2.dbValue = g_fuzzUtils.GetData<bool>();
    std::vector<VolumePoint> volumePoints = {};
    volumePoints.push_back(volumePoint1);
    volumePoints.push_back(volumePoint2);
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->GetPositionInVolumePoints(volumePoints, 0);
}

void AudioVolumeManagerInitVolumeMapIndexFuzzTest()
{
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->defaultVolumeTypeList_.push_back(streamType);
    audioAdapterManager->InitVolumeMapIndex();
}

void AudioVolumeManagerUpdateVolumeMapIndexFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    VolumePoint volumePoint;
    volumePoint.index = g_fuzzUtils.GetData<uint32_t>();
    volumePoint.dbValue = g_fuzzUtils.GetData<bool>();
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
    streamVolumeInfoPtr->maxLevel = g_fuzzUtils.GetData<int>() | 1;
    streamVolumeInfoPtr->minLevel = g_fuzzUtils.GetData<int>();
    streamVolumeInfoPtr->defaultLevel = g_fuzzUtils.GetData<bool>();
    streamVolumeInfoPtr->deviceVolumeInfos = deviceVolumeInfoMap;
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->streamVolumeInfos_.insert({streamVolumeInfoPtr->streamType, streamVolumeInfoPtr});
    audioAdapterManager->UpdateVolumeMapIndex();
}

void AudioVolumeManagerSetAbsVolumeSceneFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    bool isAbsVolumeScene = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager->SetAbsVolumeScene(isAbsVolumeScene);
}

void AudioVolumeManagerSetAbsVolumeMuteFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    bool mute = g_fuzzUtils.GetData<bool>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager->SetAbsVolumeMute(mute);
}

void AudioVolumeManagerSetAppVolumeLevelFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    audioAdapterManager_->currentActiveDevice_.deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    int32_t randIntValue = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetAppVolumeLevel(randIntValue, randIntValue);
}
 
void AudioVolumeManagerSetAdjustVolumeForZoneFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    int32_t zoneId = g_fuzzUtils.GetData<int32_t>();
    audioAdapterManager_->SetAdjustVolumeForZone(zoneId);
}
 
void AudioVolumeManagerSetZoneMuteFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    int32_t zoneId = g_fuzzUtils.GetData<int32_t>();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    StreamUsage streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    bool mute = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetZoneMute(zoneId, streamType, mute, streamUsage, deviceType);
}
 
void AudioVolumeManagerGetZoneMuteFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    int32_t zoneId = g_fuzzUtils.GetData<int32_t>();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    audioAdapterManager_->GetZoneMute(zoneId, streamType);
}
 
void AudioVolumeManagerGetZoneVolumeLevelFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    int32_t zoneId = g_fuzzUtils.GetData<int32_t>();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    audioAdapterManager_->GetZoneVolumeLevel(zoneId, streamType);
}
 
void AudioVolumeManagerSetZoneVolumeLevelFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    int32_t zoneId = g_fuzzUtils.GetData<int32_t>();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    int32_t volumeLevel = g_fuzzUtils.GetData<int32_t>();
    audioAdapterManager_->SetZoneVolumeLevel(zoneId, streamType, volumeLevel);
}
 
void AudioVolumeManagerGetDeviceVolumeFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    DeviceType deviceType = g_fuzzUtils.GetData<DeviceType>();
    audioAdapterManager_->GetDeviceVolume(deviceType, streamType);
}
 
void AudioVolumeManagerHandleRingerModeFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioRingerMode ringMode = g_fuzzUtils.GetData<AudioRingerMode>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->HandleRingerMode(ringMode);
}
 
void AudioVolumeManagerSetAppVolumeDbFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    int32_t appUid = g_fuzzUtils.GetData<int32_t>();
    audioAdapterManager_->SetAppVolumeDb(appUid);
}
 
void AudioVolumeManagerSetAppVolumeMutedDBFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    int32_t appUid = g_fuzzUtils.GetData<int32_t>();
    bool muted = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetAppVolumeMutedDB(appUid, muted);
}
 
void AudioVolumeManagerGetAppVolumeLevelFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    int32_t appUid = g_fuzzUtils.GetData<int32_t>();
    int32_t volumeLevel = g_fuzzUtils.GetData<int32_t>();
    audioAdapterManager_->GetAppVolumeLevel(appUid, volumeLevel);
}
 
void AudioVolumeManagerSetPersistMicMuteStateFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    bool mute = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetPersistMicMuteState(mute);
}
 
void AudioVolumeManagerSetSourceOutputStreamMuteFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    int32_t uid = g_fuzzUtils.GetData<int32_t>();
    bool setMute = g_fuzzUtils.GetData<bool>();
    audioAdapterManager_->SetSourceOutputStreamMute(uid, setMute);
}
 
void AudioVolumeManagerGetStreamMuteFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioStreamType streamType = g_fuzzUtils.GetData<AudioStreamType>();
    audioAdapterManager_->GetStreamMute(streamType);
}
 
void AudioVolumeManagerGetAppMuteFuzzTest()
{
    CHECK_AND_RETURN(audioAdapterManager_ != nullptr);
    audioAdapterManager_->Init();
    int32_t appUid = g_fuzzUtils.GetData<int32_t>();
    audioAdapterManager_->GetAppMute(appUid);
}
 
void AudioVolumeManagerSetRingerModeFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioRingerMode ringMode = g_fuzzUtils.GetData<AudioRingerMode>();
    auto audioAdapterManager = std::make_shared<AudioAdapterManager>();
    CHECK_AND_RETURN(audioAdapterManager != nullptr);
    audioAdapterManager->SetRingerMode(ringMode);
}

vector<TestPtr> g_testPtrs = {
    AudioVolumeManagerIsAppVolumeMuteFuzzTest,
    AudioVolumeManagerSaveSpecifiedDeviceVolumeFuzzTest,
    AudioVolumeManagerHandleStreamMuteStatusFuzzTest,
    AudioVolumeManagerSetOffloadVolumeFuzzTest,
    AudioVolumeManagerSetOffloadSessionIdFuzzTest,
    AudioVolumeManagerConnectServiceAdapterFuzzTest,
    AudioVolumeManagerHandleKvDataFuzzTest,
    AudioVolumeManagerReInitKVStoreFuzzTest,
    AudioVolumeManagerSaveRingtoneVolumeToLocalFuzzTest,
    AudioVolumeManagerUpdateSafeVolumeByS4FuzzTest,
    AudioVolumeManagerSetAppVolumeMutedFuzzTest,
    AudioVolumeManagerSetSystemVolumeLevelFuzzTest,
    AudioVolumeManagerSetDoubleRingVolumeDbFuzzTest,
    AudioVolumeManagerSetInnerStreamMuteFuzzTest,
    AudioVolumeManagerSetStreamMuteFuzzTest,
    AudioVolumeManagerSetSinkMuteFuzzTest,
    AudioVolumeManagerSelectDeviceFuzzTest,
    AudioVolumeManagerSetDeviceActiveFuzzTest,
    AudioVolumeManagerAdjustBluetoothVoiceAssistantVolumeFuzzTest,
    AudioVolumeManagerSetVolumeForSwitchDeviceFuzzTest,
    AudioVolumeManagerSaveRingerModeInfoFuzzTest,
    AudioVolumeManagerGetSinkIdInfoAndIdTypeFuzzTest,
    AudioVolumeManagerOpenNotPaAudioPortFuzzTest,
    AudioVolumeManagerUpdateSinkArgsFuzzTest,
    AudioVolumeManagerGetModuleArgsFuzzTest,
    AudioVolumeManagerGetHdiSinkIdInfoFuzzTest,
    AudioVolumeManagerGetHdiSourceIdInfoFuzzTest,
    AudioVolumeManagerGetVolumeKeyForKvStoreFuzzTest,
    AudioVolumeManagerGetStreamIDByTypeFuzzTest,
    AudioVolumeManagerGetDeviceCategoryFuzzTest,
    AudioVolumeManagerDeleteAudioPolicyKvStoreFuzzTest,
    AudioVolumeManagerUpdateSafeVolumeFuzzTest,
    AudioVolumeManagerInitVolumeMapFuzzTest,
    AudioVolumeManagerGetDefaultVolumeLevelFuzzTest,
    AudioVolumeManagerResetRemoteCastDeviceVolumeFuzzTest,
    AudioVolumeManagerInitRingerModeFuzzTest,
    AudioVolumeManagerHandleDistributedVolumeFuzzTest,
    AudioVolumeManagerInitMuteStatusMapFuzzTest,
    AudioVolumeManagerCheckAndDealMuteStatusFuzzTest,
    AudioVolumeManagerSetVolumeCallbackAfterCloneFuzzTest,
    AudioVolumeManagerOpenPaAudioPortFuzzTest,
    AudioVolumeManagerCloneMuteStatusMapFuzzTest,
    AudioVolumeManagerLoadMuteStatusMapFuzzTest,
    AudioVolumeManagerInitSafeStatusFuzzTest,
    AudioVolumeManagerInitSafeTimeFuzzTest,
    AudioVolumeManagerConvertSafeTimeFuzzTest,
    AudioVolumeManagerGetCurrentDeviceSafeStatusFuzzTest,
    AudioVolumeManagerGetCurentDeviceSafeTimeFuzzTest,
    AudioVolumeManagerGetRestoreVolumeLevelFuzzTest,
    AudioVolumeManagerSetDeviceSafeStatusFuzzTest,
    AudioVolumeManagerSetDeviceSafeTimeFuzzTest,
    AudioVolumeManagerSetRestoreVolumeLevelFuzzTest,
    AudioVolumeManagerGetMuteKeyForKvStoreFuzzTest,
    AudioVolumeManagerSetSystemSoundUriFuzzTest,
    AudioVolumeManagerGetSystemSoundUriFuzzTest,
    AudioVolumeManagerGetSystemVolumeInDbFuzzTest,
    AudioVolumeManagerGetPositionInVolumePointsFuzzTest,
    AudioVolumeManagerInitVolumeMapIndexFuzzTest,
    AudioVolumeManagerUpdateVolumeMapIndexFuzzTest,
    AudioVolumeManagerSetAbsVolumeSceneFuzzTest,
    AudioVolumeManagerSetAbsVolumeMuteFuzzTest,
    AudioVolumeManagerSetAudioVolumeFuzzTest,
    AudioVolumeManagerNotifyAccountsChangedFuzzTest,
    AudioVolumeManagerDoRestoreDataFuzzTest,
    AudioVolumeManagerSetFirstBootFuzzTest,
    AudioVolumeManagerSafeVolumeDumpFuzzTest,
    AudioVolumeManagerSetVgsVolumeSupportedFuzzTest,
    AudioVolumeManagerSetAppVolumeLevelFuzzTest,
    AudioVolumeManagerSetAdjustVolumeForZoneFuzzTest,
    AudioVolumeManagerSetZoneMuteFuzzTest,
    AudioVolumeManagerSetZoneMuteFuzzTest,
    AudioVolumeManagerGetZoneMuteFuzzTest,
    AudioVolumeManagerGetZoneVolumeLevelFuzzTest,
    AudioVolumeManagerSetZoneVolumeLevelFuzzTest,
    AudioVolumeManagerGetDeviceVolumeFuzzTest,
    AudioVolumeManagerHandleRingerModeFuzzTest,
    AudioVolumeManagerSetAppVolumeDbFuzzTest,
    AudioVolumeManagerSetAppVolumeMutedDBFuzzTest,
    AudioVolumeManagerGetAppVolumeLevelFuzzTest,
    AudioVolumeManagerSetPersistMicMuteStateFuzzTest,
    AudioVolumeManagerSetSourceOutputStreamMuteFuzzTest,
    AudioVolumeManagerGetStreamMuteFuzzTest,
    AudioVolumeManagerGetAppMuteFuzzTest,
    AudioVolumeManagerSetRingerModeFuzzTest,
};
} // namespace AudioStandard
} // namesapce OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    if (size < OHOS::AudioStandard::FUZZ_INPUT_SIZE_THRESHOLD) {
        return 0;
    }

    OHOS::AudioStandard::g_fuzzUtils.fuzzTest(data, size, OHOS::AudioStandard::g_testPtrs);
    return 0;
}