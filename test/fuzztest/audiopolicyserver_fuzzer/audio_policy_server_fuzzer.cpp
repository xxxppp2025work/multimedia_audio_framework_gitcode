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

#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include "audio_info.h"
#include "audio_policy_server.h"
#include "audio_policy_service.h"
#include "audio_device_info.h"
#include "audio_utils.h"
#include "accesstoken_kit.h"
#include "nativetoken_kit.h"
#include "token_setproc.h"
#include "access_token.h"
#include "audio_channel_blend.h"
#include "volume_ramp.h"
#include "audio_speed.h"
#include "audio_policy_utils.h"
#include "audio_stream_descriptor.h"

using namespace std;

namespace OHOS {
namespace AudioStandard {
const int32_t SYSTEM_ABILITY_ID = 3009;
const bool RUN_ON_CREATE = false;
bool g_hasServerInit = false;
static const uint8_t* RAW_DATA = nullptr;
static size_t g_dataSize = 0;
static size_t g_pos;
const size_t THRESHOLD = 10;
static int32_t NUM_2 = 2;
typedef void (*TestFuncs)();

class RemoteObjectFuzzTestStub : public IRemoteObject {
public:
    RemoteObjectFuzzTestStub() : IRemoteObject(u"IRemoteObject") {}
    int32_t GetObjectRefCount() { return 0; };
    int SendRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option) { return 0; };
    bool AddDeathRecipient(const sptr<DeathRecipient> &recipient) { return true; };
    bool RemoveDeathRecipient(const sptr<DeathRecipient> &recipient) { return true; };
    int Dump(int fd, const std::vector<std::u16string> &args) { return 0; };

    DECLARE_INTERFACE_DESCRIPTOR(u"RemoteObjectFuzzTestStub");
};

class AudioClientTrackerFuzzTest : public AudioClientTracker {
    public:
        virtual ~AudioClientTrackerFuzzTest() = default;
        virtual void MuteStreamImpl(const StreamSetStateEventInternal &streamSetStateEventInternal) {};
        virtual void UnmuteStreamImpl(const StreamSetStateEventInternal &streamSetStateEventInternal) {};
        virtual void PausedStreamImpl(const StreamSetStateEventInternal &streamSetStateEventInternal) {};
        virtual void ResumeStreamImpl(const StreamSetStateEventInternal &streamSetStateEventInternal) {};
        virtual void SetLowPowerVolumeImpl(float volume) {};
        virtual void GetLowPowerVolumeImpl(float &volume) {};
        virtual void GetSingleStreamVolumeImpl(float &volume) {};
        virtual void SetOffloadModeImpl(int32_t state, bool isAppBack) {};
        virtual void UnsetOffloadModeImpl() {};
    };

const vector<DeviceFlag> g_testDeviceFlags = {
    NONE_DEVICES_FLAG,
    OUTPUT_DEVICES_FLAG,
    INPUT_DEVICES_FLAG,
    ALL_DEVICES_FLAG,
    DISTRIBUTED_OUTPUT_DEVICES_FLAG,
    DISTRIBUTED_INPUT_DEVICES_FLAG,
    ALL_DISTRIBUTED_DEVICES_FLAG,
    ALL_L_D_DEVICES_FLAG,
    DEVICE_FLAG_MAX
};

const vector<AudioStreamDeviceChangeReason> g_testReasons = {
    AudioStreamDeviceChangeReason::UNKNOWN,
    AudioStreamDeviceChangeReason::NEW_DEVICE_AVAILABLE,
    AudioStreamDeviceChangeReason::OLD_DEVICE_UNAVALIABLE,
    AudioStreamDeviceChangeReason::OVERRODE,
};

const vector<StreamSetState> g_testStreamSetStates = {
    STREAM_PAUSE,
    STREAM_RESUME,
    STREAM_MUTE,
    STREAM_UNMUTE,
};

template<class T>
uint32_t GetArrLength(T& arr)
{
    if (arr == nullptr) {
        AUDIO_INFO_LOG("%{public}s: The array length is equal to 0", __func__);
        return 0;
    }
    return sizeof(arr) / sizeof(arr[0]);
}

template<class T>
T GetData()
{
    T object {};
    size_t objectSize = sizeof(object);
    if (RAW_DATA == nullptr || objectSize > g_dataSize - g_pos) {
        return object;
    }
    errno_t ret = memcpy_s(&object, objectSize, RAW_DATA + g_pos, objectSize);
    if (ret != EOK) {
        return {};
    }
    g_pos += objectSize;
    return object;
}

sptr<AudioPolicyServer> GetServerPtr()
{
    static sptr<AudioPolicyServer> server = sptr<AudioPolicyServer>::MakeSptr(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (!g_hasServerInit && server != nullptr) {
        server->OnStart();
        server->OnAddSystemAbility(AUDIO_DISTRIBUTED_SERVICE_ID, "");
#ifdef FEATURE_MULTIMODALINPUT_INPUT
        server->OnAddSystemAbility(MULTIMODAL_INPUT_SERVICE_ID, "");
#endif
        server->OnAddSystemAbility(BLUETOOTH_HOST_SYS_ABILITY_ID, "");
        server->OnAddSystemAbility(POWER_MANAGER_SERVICE_ID, "");
        server->OnAddSystemAbility(SUBSYS_ACCOUNT_SYS_ABILITY_ID_BEGIN, "");
        server->audioPolicyService_.SetDefaultDeviceLoadFlag(true);
        g_hasServerInit = true;
    }
    return server;
}

void AudioPolicyServerRegisterDefaultVolumeTypeListenerFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    audioPolicyServer->RegisterDefaultVolumeTypeListener();
}

void AudioPolicyServerOnAddSystemAbilityExtractFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t systemAbilityId = APP_MGR_SERVICE_ID;
    std::string deviceId = "";
    audioPolicyServer->OnAddSystemAbilityExtract(systemAbilityId, deviceId);
    int32_t systemAbilityId = 0;
    audioPolicyServer->OnAddSystemAbilityExtract(systemAbilityId, deviceId);
}

void AudioPolicyServerOnRemoveSystemAbilityFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t systemAbilityId = APP_MGR_SERVICE_ID;
    std::string deviceId = "";
    audioPolicyServer->OnRemoveSystemAbility(systemAbilityId, eviceId);
}

void AudioPolicyServerMaxOrMinVolumeOptionFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t volLevel = GetData<int32_t>();
    int32_t keyType = OHOS::MMI::KeyEvent::KEYCODE_VOLUME_UP;
    AudioStreamType streamInFocus = AudioStreamType::STREAM_ALL;
    audioPolicyServer->MaxOrMinVolumeOption(volLevel, keyType, streamInFocus);
}

void AudioPolicyServerChangeVolumeOnVoiceAssistantFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    AudioStreamType streamInFocus == AudioStreamType::STREAM_VOICE_ASSISTANT;
    audioPolicyServer->ChangeVolumeOnVoiceAssistant(streamInFocus);
}

void AudioPolicyServerIsContinueAddVolFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    audioPolicyServer->IsContinueAddVol();
}

void AudioPolicyServerTriggerMuteCheckFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    audioPolicyServer->TriggerMuteCheck();
}

void AudioPolicyServerProcessVolumeKeyEventsFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t keyType = OHOS::MMI::KeyEvent::KEYCODE_VOLUME_UP;
    audioPolicyServer->ProcessVolumeKeyEvents(keyType);
}

void AudioPolicyServerSetVolumeInternalByKeyEventFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    AudioStreamType streamInFocus = AudioStreamType::STREAM_ALL;
    int32_t zoneId = GetData<int32_t>();
    int32_t keyType = OHOS::MMI::KeyEvent::KEYCODE_VOLUME_UP;
    audioPolicyServer->SetVolumeInternalByKeyEvent(streamInFocus, zoneId, keyType);
}

void AudioPolicyServerSubscribeSafeVolumeEventFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    AudioStreamType streamInFocus = AudioStreamType::STREAM_ALL;
    int32_t zoneId = GetData<int32_t>();
    int32_t keyType = OHOS::MMI::KeyEvent::KEYCODE_VOLUME_UP;
    audioPolicyServer->SubscribeSafeVolumeEvent();
}

void AudioPolicyServerIsVolumeTypeValidFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    AudioStreamType streamInFocus = AudioStreamType::STREAM_ALL;
    audioPolicyServer->IsVolumeTypeValid(streamInFocus);
}

void AudioPolicyServerIsVolumeLevelValidFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    AudioStreamType streamInFocus = AudioStreamType::STREAM_ALL;
    int32_t volumeLevel = GetData<int32_t>();
    audioPolicyServer->IsVolumeLevelValid(streamInFocus, volumeLevel);
}

void AudioPolicyServerIsRingerModeValidFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    AudioRingerMode ringMode = AudioRingerMode::RINGER_MODE_SILENT;
    audioPolicyServer->IsRingerModeValid(ringMode);
}

void AudioPolicyServerSubscribeOsAccountChangeEventsFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    audioPolicyServer->SubscribeOsAccountChangeEvents();
}

void AudioPolicyServerAddRemoteDevstatusCallbackFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    audioPolicyServer->AddRemoteDevstatusCallback();
}

void AudioPolicyServerOnReceiveEventFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    EventFwk::CommonEventData eventData;
    audioPolicyServer->OnReceiveEvent(eventData);
}

void AudioPolicyServerSubscribeBackgroundTaskFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    audioPolicyServer->SubscribeBackgroundTask();
}

void AudioPolicyServerSubscribeCommonEventExecuteFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    audioPolicyServer->SubscribeCommonEventExecute();
}

void AudioPolicyServerOnReceiveEventFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    EventFwk::CommonEventData eventData;
    audioPolicyServer->OnReceiveEvent(eventData);
}

void AudioPolicyServerCheckSubscribePowerStateChangeFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    audioPolicyServer->CheckSubscribePowerStateChange();
}

void AudioPolicyServerOnAsyncPowerStateChangedFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    PowerMgr::PowerState state;
    audioPolicyServer->OnAsyncPowerStateChanged(state);
}

void AudioPolicyServerNotifySettingsDataReadyFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    audioPolicyServer->NotifySettingsDataReady();
}

void AudioPolicyServerGetMaxVolumeLevelFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t volumeType = GetData<int32_t>();
    int32_t deviceType = GetData<int32_t>();
    int32_t volumeLevel = GetData<int32_t>();
    audioPolicyServer->GetMaxVolumeLevel(volumeType, volumeLevel, deviceType);
}

void AudioPolicyServerGetMinVolumeLevelFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t volumeType = GetData<int32_t>();
    int32_t deviceType = GetData<int32_t>();
    int32_t volumeLevel = GetData<int32_t>();
    audioPolicyServer->GetMinVolumeLevel(volumeType, volumeLevel, deviceType);
}

void AudioPolicyServerSetSystemVolumeLevelLegacyFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t streamTypeIn = GetData<int32_t>();
    int32_t volumeLevel = GetData<int32_t>();
    audioPolicyServer->SetSystemVolumeLevelLegacy(streamTypeIn, volumeLevel);
}

void AudioPolicyServerSetAdjustVolumeForZoneFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t zoneId = GetData<int32_t>();
    audioPolicyServer->SetAdjustVolumeForZone(zoneId);
}

void AudioPolicyServerGetSelfAppVolumeLevelFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t volumeLevel = GetData<int32_t>();
    audioPolicyServer->GetSelfAppVolumeLevel(volumeLevel);
}

void AudioPolicyServerGetSystemVolumeLevelFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t streamType = GetData<int32_t>();
    int32_t uid = GetData<int32_t>();
    int32_t volumeLevel = GetData<int32_t>();
    audioPolicyServer->GetSystemVolumeLevel(streamType, uid, volumeLevel);
}

void AudioPolicyServerGetSystemVolumeLevelNoMuteStateFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    AudioStreamType streamInFocus = AudioStreamType::STREAM_ALL;
    audioPolicyServer->GetSystemVolumeLevelNoMuteState(streamInFocus);
}

void AudioPolicyServerGetSystemVolumeLevelInternalFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    AudioStreamType streamInFocus = AudioStreamType::STREAM_ALL;
    int32_t zoneId = GetData<int32_t>();
    audioPolicyServer->GetSystemVolumeLevelInternal(streamType, zoneId);
}

void AudioPolicyServerGetAppVolumeLevelInternalFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t appUid = GetData<int32_t>();
    int32_t volumeLevel = GetData<int32_t>();
    audioPolicyServer->GetAppVolumeLevelInternal(appUid, volumeLevel);
}

void AudioPolicyServerSetLowPowerVolumeFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t streamId = GetData<int32_t>();
    float volume = true;
    audioPolicyServer->SetLowPowerVolume(streamId, volume);
}

void AudioPolicyServerGetFastStreamInfoFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    AudioStreamInfo streamInfo;
    audioPolicyServer->GetFastStreamInfo(streamInfo);
}

void AudioPolicyServerGetLowPowerVolumeFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t streamId = GetData<int32_t>();
    float outVolume;
    audioPolicyServer->GetLowPowerVolume(streamId, outVolume);
}

void AudioPolicyServerGetSingleStreamVolumeFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t streamId = GetData<int32_t>();
    float outVolume;
    audioPolicyServer->GetSingleStreamVolume(streamId, outVolume);
}

void AudioPolicyServerIsVolumeUnadjustableFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    bool unadjustable;
    audioPolicyServer->IsVolumeUnadjustable(unadjustable);
}

void AudioPolicyServerCheckCanMuteVolumeTypeByStepFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    AudioVolumeType volumeType = AudioVolumeType::STREAM_VOICE_CALL;
    int32_t volumeLevel = GetData<int32_t>();
    audioPolicyServer->CheckCanMuteVolumeTypeByStep(volumeType, volumeLevel);
}

void AudioPolicyServerGetSystemVolumeInDbFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t volumeTypeIn = GetData<int32_t>();
    int32_t volumeLevel = GetData<int32_t>();
    int32_t deviceTypeIn = GetData<int32_t>();
    float volume;
    audioPolicyServer->GetSystemVolumeInDb(volumeTypeIn, volumeLevel, deviceTypeIn, volume);
}

void AudioPolicyServerSetStreamMuteLegacyFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t streamTypeIn = GetData<int32_t>();
    int32_t deviceTypeIn = GetData<int32_t>();
    bool mute = true;
    audioPolicyServer->SetStreamMuteLegacy(streamTypeIn, mute, deviceTypeIn);
}

void AudioPolicyServerSetStreamMuteInternalFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t zoneId = GetData<int32_t>();
    DeviceType deviceType = DEVICE_TYPE_NONE;
    bool mute = true;
    bool isUpdateUi = true;
    AudioStreamType streamType = AudioVolumeType::STREAM_ALL;
    audioPolicyServer->SetStreamMuteInternal(streamType, mute, isUpdateUi, deviceType, zoneId);
}

void AudioPolicyServerUpdateSystemMuteStateAccordingMusicStateFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    bool mute = true;
    bool isUpdateUi = true;
    AudioStreamType streamType = AudioVolumeType::STREAM_ALL;
    audioPolicyServer->UpdateSystemMuteStateAccordingMusicState(streamType, mute, isUpdateUi);
}

void AudioPolicyServerSendMuteKeyEventCbWithUpdateUiOrNotFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t zoneId = GetData<int32_t>();
    bool isUpdateUi = true;
    AudioStreamType streamType = AudioVolumeType::STREAM_ALL;
    audioPolicyServer->SendMuteKeyEventCbWithUpdateUiOrNot(streamType, isUpdateUi, zoneId);
}

void AudioPolicyServerSetSingleStreamMuteFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    int32_t zoneId = GetData<int32_t>();
    bool isUpdateUi = true;
    AudioStreamType streamType = AudioVolumeType::STREAM_ALL;
    DeviceType deviceType = DEVICE_TYPE_NONE;
    bool mute = true;
    audioPolicyServer->SetSingleStreamMute(streamType, mute, isUpdateUi, deviceType, zoneId);
}

void AudioPolicyServerProcUpdateRingerModeForMuteFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    bool updateRingerMode = true;
    bool mute = true;
    audioPolicyServer->ProcUpdateRingerModeForMute(updateRingerMode, mute);
}

void AudioPolicyServerGetSystemVolumeDbFuzzTest()
{
    auto audioPolicyServer = GetServerPtr();
    CHECK_AND_RETURN(audioPolicyServer != nullptr);
    AudioStreamType streamType = AudioVolumeType::STREAM_ALL;
    audioPolicyServer->GetSystemVolumeDb(streamType);
}


TestFuncs g_testFuncs[] = {
    AudioPolicyServerRegisterDefaultVolumeTypeListenerFuzzTest,
    AudioPolicyServerOnAddSystemAbilityExtractFuzzTest,
    AudioPolicyServerOnRemoveSystemAbilityFuzzTest,
    AudioPolicyServerMaxOrMinVolumeOptionFuzzTest,
    AudioPolicyServerChangeVolumeOnVoiceAssistantFuzzTest,
    AudioPolicyServerIsContinueAddVolFuzzTest,
    AudioPolicyServerTriggerMuteCheckFuzzTest,
    AudioPolicyServerProcessVolumeKeyEventsFuzzTest,
    AudioPolicyServerSetVolumeInternalByKeyEventFuzzTest,
    AudioPolicyServerSubscribeSafeVolumeEventFuzzTest,
    AudioPolicyServerIsVolumeTypeValidFuzzTest,
    AudioPolicyServerIsVolumeLevelValidFuzzTest,
    AudioPolicyServerIsRingerModeValidFuzzTest,
    AudioPolicyServerSubscribeOsAccountChangeEventsFuzzTest,
    AudioPolicyServerAddRemoteDevstatusCallbackFuzzTest,
    AudioPolicyServerOnReceiveEventFuzzTest,
    AudioPolicyServerSubscribeBackgroundTaskFuzzTest,
    AudioPolicyServerSubscribeCommonEventExecuteFuzzTest,
    AudioPolicyServerOnReceiveEventFuzzTest,
    AudioPolicyServerCheckSubscribePowerStateChangeFuzzTest,
    AudioPolicyServerOnAsyncPowerStateChangedFuzzTest,
    AudioPolicyServerNotifySettingsDataReadyFuzzTest,
    AudioPolicyServerGetMaxVolumeLevelFuzzTest,
    AudioPolicyServerGetMinVolumeLevelFuzzTest,
    AudioPolicyServerSetSystemVolumeLevelLegacyFuzzTest,
    AudioPolicyServerSetAdjustVolumeForZoneFuzzTest,
    AudioPolicyServerGetSelfAppVolumeLevelFuzzTest,
    AudioPolicyServerGetSystemVolumeLevelFuzzTest,
    AudioPolicyServerGetSystemVolumeLevelNoMuteStateFuzzTest,
    AudioPolicyServerGetSystemVolumeLevelInternalFuzzTest,
    AudioPolicyServerGetAppVolumeLevelInternalFuzzTest,
    AudioPolicyServerSetLowPowerVolumeFuzzTest,
    AudioPolicyServerGetFastStreamInfoFuzzTest,
    AudioPolicyServerGetLowPowerVolumeFuzzTest,
    AudioPolicyServerGetSingleStreamVolumeFuzzTest,
    AudioPolicyServerIsVolumeUnadjustableFuzzTest,
    AudioPolicyServerCheckCanMuteVolumeTypeByStepFuzzTest,
    AudioPolicyServerGetSystemVolumeInDbFuzzTest,
    AudioPolicyServerSetStreamMuteLegacyFuzzTest,
    AudioPolicyServerSetStreamMuteInternalFuzzTest,
    AudioPolicyServerUpdateSystemMuteStateAccordingMusicStateFuzzTest,
    AudioPolicyServerSendMuteKeyEventCbWithUpdateUiOrNotFuzzTest,
    AudioPolicyServerSetSingleStreamMuteFuzzTest,
    AudioPolicyServerProcUpdateRingerModeForMuteFuzzTest,
    AudioPolicyServerGetSystemVolumeDbFuzzTest,
};

bool FuzzTest(const uint8_t* rawData, size_t size)
{
    if (rawData == nullptr) {
        return false;
    }

    // initialize data
    RAW_DATA = rawData;
    g_dataSize = size;
    g_pos = 0;

    uint32_t code = GetData<uint32_t>();
    uint32_t len = GetArrLength(g_testFuncs);
    if (len > 0) {
        g_testFuncs[code % len]();
    } else {
        AUDIO_INFO_LOG("%{public}s: The len length is equal to 0", __func__);
    }

    return true;
}
} // namespace AudioStandard
} // namesapce OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    if (size < OHOS::AudioStandard::THRESHOLD) {
        return 0;
    }

    OHOS::AudioStandard::FuzzTest(data, size);
    return 0;
}
