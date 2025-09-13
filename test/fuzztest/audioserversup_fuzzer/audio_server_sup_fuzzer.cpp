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

#include <securec.h>

#include "audio_log.h"
#include "audio_server.h"
#include "audio_policy_manager_listener_stub_impl.h"
#include "../fuzz_utils.h"

namespace OHOS {
namespace AudioStandard {
using namespace std;

FuzzUtils &g_fuzzUtils = FuzzUtils::GetInstance();

const int32_t SYSTEM_ABILITY_ID = 3001;
const int32_t MEDIA_SERVICE_UID = 1013;
const bool RUN_ON_CREATE = false;
const std::string PCM_DUMP_KEY = "PCM_DUMP";

typedef void (*TestFuncs)();

void AudioServerSetReleaseFlagFuzzTest()
{
    bool flag = g_fuzzUtils.GetData<bool>();
    uint32_t sessionId = g_fuzzUtils.GetData<uint32_t>();
    std::shared_ptr<PipeInfoGuard> pipeinfoGuard = std::make_shared<PipeInfoGuard>(sessionId);
    if (pipeinfoGuard == nullptr) {
        return;
    }
    pipeinfoGuard->SetReleaseFlag(flag);
}

void RemoveRendererDataTransferCallbackFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    auto callback = make_shared<DataTransferStateChangeCallbackInnerImpl>();
    if (callback == nullptr) {
        return;
    }
    audioServer->audioDataTransferCbMap_[pid] = callback;
    audioServer->RemoveRendererDataTransferCallback(pid);
}

void RegisterDataTransferCallbackFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    sptr<AudioPolicyManagerListenerStubImpl> listener = new (std::nothrow) AudioPolicyManagerListenerStubImpl();
    if (listener == nullptr) {
        return;
    }
    sptr<IRemoteObject> object = listener->AsObject();
    if (object == nullptr) {
        AUDIO_INFO_LOG("AudioServer:set listener object is nullptr");
        return;
    }
    audioServer->RegisterDataTransferCallback(object);
}

void UnregisterDataTransferMonitorParamFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    int32_t callbackId = g_fuzzUtils.GetData<int32_t>();
    audioServer->UnregisterDataTransferMonitorParam(callbackId);
}

void OnDataTransferStateChangeFuzzTest()
{
    std::shared_ptr<DataTransferStateChangeCallbackInnerImpl> callback =
        std::make_shared<DataTransferStateChangeCallbackInnerImpl>();
    if (callback == nullptr) {
        return;
    }
    int32_t callbackId = g_fuzzUtils.GetData<int32_t>();
    AudioRendererDataTransferStateChangeInfo info;
    callback->OnDataTransferStateChange(callbackId, info);
}

void GetPcmDumpParameterFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    std::vector<std::string> subKeys;
    std::string subKey = "test";
    subKeys.push_back(subKey);
    std::vector<std::pair<std::string, std::string>> result;
    std::pair<std::string, std::string> pair;
    pair.first = "test";
    pair.second = "test";
    result.push_back(pair);
    audioServer->GetPcmDumpParameter(subKeys, result);
}

void InnerCheckCaptureLimitFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    #ifdef HAS_FEATURE_INNERCAPTURER
    AudioPlaybackCaptureConfig config;
    int32_t innerCapId = g_fuzzUtils.GetData<int32_t>();
    audioServer->InnerCheckCaptureLimit(config, innerCapId);
    #endif
}

void SetParameterCallbackFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    sptr<AudioPolicyManagerListenerStubImpl> listener = new (std::nothrow) AudioPolicyManagerListenerStubImpl();
    if (listener == nullptr) {
        return;
    }
    sptr<IRemoteObject> object = listener->AsObject();
    if (object == nullptr) {
        AUDIO_INFO_LOG("AudioServer:set listener object is nullptr");
        return;
    }
    audioServer->SetParameterCallback(object);
}

void SetWakeupSourceCallbackFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    sptr<AudioPolicyManagerListenerStubImpl> listener = new (std::nothrow) AudioPolicyManagerListenerStubImpl();
    if (listener == nullptr) {
        return;
    }
    sptr<IRemoteObject> object = listener->AsObject();
    if (object == nullptr) {
        AUDIO_INFO_LOG("AudioServer:set listener object is nullptr");
        return;
    }
    audioServer->SetWakeupSourceCallback(object);
}

void CheckInnerRecorderPermissionFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    AudioProcessConfig config;
    config.capturerInfo.sourceType = SOURCE_TYPE_REMOTE_CAST;
    audioServer->CheckInnerRecorderPermission(config);
    config.capturerInfo.sourceType = SOURCE_TYPE_PLAYBACK_CAPTURE;
    config.innerCapMode = MODERN_INNER_CAP;
    audioServer->CheckInnerRecorderPermission(config);
}

void SetForegroundListFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    std::string foreground = "test";
    std::vector<std::string> list;
    list.push_back(foreground);
    audioServer->SetForegroundList(list);
}

void SendInterruptEventToAudioServerFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    uint32_t sessionId = g_fuzzUtils.GetData<uint32_t>();
    InterruptEventInternal interruptEvent;
    audioServer->SendInterruptEventToAudioServer(sessionId, interruptEvent);
}

void CheckVoiceCallRecorderPermissionFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    Security::AccessToken::AccessTokenID tokenId = Security::AccessToken::INVALID_TOKENID;
    audioServer->CheckVoiceCallRecorderPermission(tokenId);
}

void NotifyStreamVolumeChangedFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    int32_t streamType = g_fuzzUtils.GetData<int32_t>();
    float volume = g_fuzzUtils.GetData<float>();
    audioServer->NotifyStreamVolumeChanged(streamType, volume);
}

void ResetRouteForDisconnectFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    int32_t type = g_fuzzUtils.GetData<int32_t>();
    audioServer->ResetRouteForDisconnect(type);
}

void GetMaxAmplitudeFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    bool isOutputDevice = g_fuzzUtils.GetData<bool>();
    std::string deviceClass = "test";
    int32_t sourceType = g_fuzzUtils.GetData<int32_t>();
    float maxAmplitude = g_fuzzUtils.GetData<float>();
    audioServer->GetMaxAmplitude(isOutputDevice, deviceClass, sourceType, maxAmplitude);
}

void ResetAudioEndpointFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    audioServer->ResetAudioEndpoint();
}

void ReportEventFuzzTest()
{
    std::shared_ptr<DataTransferStateChangeCallbackInnerImpl> callback =
        std::make_shared<DataTransferStateChangeCallbackInnerImpl>();
    if (callback == nullptr) {
        return;
    }
    int32_t callbackId = g_fuzzUtils.GetData<int32_t>();
    AudioRendererDataTransferStateChangeInfo info;
    info.stateChangeType = DATA_TRANS_STOP;
    callback->OnDataTransferStateChange(callbackId, info);
}

void GetOfflineAudioEffectChainsFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    std::string effectChain = "test";
    std::vector<std::string> effectChains;
    effectChains.push_back(effectChain);
    audioServer->GetOfflineAudioEffectChains(effectChains);
}

void GetAllSinkInputsFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    std::vector<SinkInput> sinkInputs = {};
    audioServer->GetAllSinkInputs(sinkInputs);
}

void DestroyHdiPortFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    uint32_t id = g_fuzzUtils.GetData<uint32_t>();
    audioServer->DestroyHdiPort(id);
}

void SetBtHdiInvalidStateFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    audioServer->SetBtHdiInvalidState();
}

void ForceStopAudioStreamFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    vector<StopAudioType> stopTypes = {
        STOP_ALL,
        STOP_RENDER,
        STOP_RECORD
    };
    int32_t audioType = stopTypes[g_fuzzUtils.GetData<int32_t>() % stopTypes.size()];
    audioServer->ForceStopAudioStream(audioType);
}

void GetExtraParametersInnerFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    std::string mainKey = PCM_DUMP_KEY;
    audioServer->isAudioParameterParsed_ = g_fuzzUtils.GetData<bool>();
    std::vector<std::string> subKeys = {"key1", "key2"};
    std::vector<std::pair<std::string, std::string>> result;
    audioServer->audioParameterKeys = {
        {
            PCM_DUMP_KEY, {
                {"Key1", {"Value1", "Value2"}}
            }
        }
    };
    audioServer->GetExtraParametersInner(mainKey, subKeys, result);
}

void GetAudioParameterInnerFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    std::string networkId = LOCAL_NETWORK_ID;
    vector<AudioParamKey> keys = {
        NONE,
        VOLUME,
        INTERRUPT,
        PARAM_KEY_STATE,
        A2DP_SUSPEND_STATE,
        BT_HEADSET_NREC,
        BT_WBS,
        A2DP_OFFLOAD_STATE,
        GET_DP_DEVICE_INFO,
        GET_PENCIL_INFO,
        GET_UWB_INFO,
        USB_DEVICE,
        PERF_INFO,
        MMI,
        PARAM_KEY_LOWPOWER,
    };
    AudioParamKey key = keys[g_fuzzUtils.GetData<int32_t>() % keys.size()];
    std::string condition = "";
    audioServer->GetAudioParameterInner(networkId, key, condition);
}

void SetAudioMonoStateFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    bool audioMono = g_fuzzUtils.GetData<bool>();
    audioServer->SetAudioMonoState(audioMono);
}

void SetAudioBalanceValueFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    float rawBalance = g_fuzzUtils.GetData<float>();
    float audioBalance = std::clamp(rawBalance, -1.0f, 1.0f);
    audioServer->SetAudioBalanceValue(audioBalance);
}

void ResetRecordConfigFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    AudioProcessConfig config;
    config.capturerInfo.sourceType = SOURCE_TYPE_PLAYBACK_CAPTURE;
    config.callerUid = g_fuzzUtils.GetData<int32_t>();
    audioServer->ResetRecordConfig(config);
    config.callerUid = MEDIA_SERVICE_UID;
    audioServer->ResetRecordConfig(config);
}

void CheckRendererFormatFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    AudioProcessConfig config;
    config.rendererInfo.streamUsage = static_cast<StreamUsage>(STREAM_USAGE_MAX + 1);
    audioServer->CheckRendererFormat(config);
}

void CheckStreamInfoFormatFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    AudioProcessConfig config;
    config.streamInfo.samplingRate = g_fuzzUtils.GetData<AudioSamplingRate>();
    config.streamInfo.format = static_cast<AudioSampleFormat>(SAMPLE_F32LE + 1);
    audioServer->CheckStreamInfoFormat(config);
    config.streamInfo.format = g_fuzzUtils.GetData<AudioSampleFormat>();
    config.streamInfo.encoding = static_cast<AudioEncodingType>(ENCODING_EAC3 + 1);
    audioServer->CheckStreamInfoFormat(config);
    config.streamInfo.encoding = g_fuzzUtils.GetData<AudioEncodingType>();
    config.streamInfo.channelLayout = static_cast<AudioChannelLayout>(CH_LAYOUT_HOA_ORDER3_FUMA + 1);
    audioServer->CheckStreamInfoFormat(config);
    config.streamInfo.channelLayout = g_fuzzUtils.GetData<AudioChannelLayout>();
    config.audioMode = AUDIO_MODE_PLAYBACK;
    config.streamInfo.channels = static_cast<AudioChannel>(CHANNEL_16 + 1);
    audioServer->CheckStreamInfoFormat(config);
    config.audioMode = AUDIO_MODE_RECORD;
    config.streamInfo.channels = static_cast<AudioChannel>(CHANNEL_6 + 1);
    audioServer->CheckStreamInfoFormat(config);
    config.streamInfo.channels = g_fuzzUtils.GetData<AudioChannel>();
    audioServer->CheckStreamInfoFormat(config);
}

void CheckRecorderFormatFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    AudioProcessConfig config;
    config.capturerInfo.sourceType = static_cast<SourceType>(SOURCE_TYPE_LIVE + 1);
    audioServer->CheckRecorderFormat(config);
    config.capturerInfo.capturerFlags = AUDIO_FLAG_MMAP;
    audioServer->CheckRecorderFormat(config);
}

void CheckConfigFormatFuzzTest()
{
    std::shared_ptr<AudioServer> audioServer = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    if (audioServer == nullptr) {
        return;
    }
    AudioProcessConfig config;
    config.audioMode = g_fuzzUtils.GetData<AudioMode>();
    audioServer->CheckConfigFormat(config);
    config.audioMode = static_cast<AudioMode>(AUDIO_MODE_RECORD + 1);
    audioServer->CheckConfigFormat(config);
}

vector<TestFuncs> g_testFuncs = {
    AudioServerSetReleaseFlagFuzzTest,
    RemoveRendererDataTransferCallbackFuzzTest,
    RegisterDataTransferCallbackFuzzTest,
    UnregisterDataTransferMonitorParamFuzzTest,
    OnDataTransferStateChangeFuzzTest,
    GetPcmDumpParameterFuzzTest,
    InnerCheckCaptureLimitFuzzTest,
    SetParameterCallbackFuzzTest,
    SetWakeupSourceCallbackFuzzTest,
    CheckInnerRecorderPermissionFuzzTest,
    SetForegroundListFuzzTest,
    SendInterruptEventToAudioServerFuzzTest,
    CheckVoiceCallRecorderPermissionFuzzTest,
    NotifyStreamVolumeChangedFuzzTest,
    ResetRouteForDisconnectFuzzTest,
    GetMaxAmplitudeFuzzTest,
    ResetAudioEndpointFuzzTest,
    ReportEventFuzzTest,
    GetOfflineAudioEffectChainsFuzzTest,
    GetAllSinkInputsFuzzTest,
    DestroyHdiPortFuzzTest,
    SetBtHdiInvalidStateFuzzTest,
    ForceStopAudioStreamFuzzTest,
    GetExtraParametersInnerFuzzTest,
    GetAudioParameterInnerFuzzTest,
    SetAudioMonoStateFuzzTest,
    SetAudioBalanceValueFuzzTest,
    ResetRecordConfigFuzzTest,
    CheckRendererFormatFuzzTest,
    CheckStreamInfoFormatFuzzTest,
    CheckRecorderFormatFuzzTest,
    CheckConfigFormatFuzzTest,
};

} // namespace AudioStandard
} // namesapce OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    OHOS::AudioStandard::g_fuzzUtils.fuzzTest(data, size, OHOS::AudioStandard::g_testFuncs);
    return 0;
}
