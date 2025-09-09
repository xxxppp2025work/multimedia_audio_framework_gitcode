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

#include <iostream>
#include <cstddef>
#include <cstdint>

#include "audio_manager_base.h"
#include "audio_policy_manager_listener_stub_impl.h"
#include "audio_server.h"
#include "audio_service.h"
#include "sink/i_audio_render_sink.h"
#include "common/hdi_adapter_info.h"
#include "manager/hdi_adapter_manager.h"
#include "audio_endpoint.h"
#include "access_token.h"
#include "message_parcel.h"
#include "audio_process_in_client.h"
#include "audio_process_in_server.h"
#include "audio_param_parser.h"
#include "none_mix_engine.h"
#include "audio_playback_engine.h"
#include "pro_renderer_stream_impl.h"
#include "oh_audio_buffer.h"
#include "../fuzz_utils.h"
using namespace std;

namespace OHOS {
namespace AudioStandard {
constexpr int32_t DEFAULT_STREAM_ID = 10;
static std::unique_ptr<NoneMixEngine> playbackEngine_ = nullptr;
static std::unique_ptr<AudioPlaybackEngine> audioPlaybackEngine_ = nullptr;
FuzzUtils &g_fuzzUtils = FuzzUtils::GetInstance();
const static int32_t TEST_NUM_TWO = 2;

typedef void (*TestFuncs)();
/*
* describe: get data from outside untrusted data(g_data) which size is according to sizeof(T)
* tips: only support basic type
*/

void ReleaseNoneEngine()
{
    if (playbackEngine_ != nullptr) {
        playbackEngine_->Stop();
        playbackEngine_ = nullptr;
    }
}

void ReleaseAudioPlaybackEngine()
{
    if (audioPlaybackEngine_ != nullptr) {
        audioPlaybackEngine_->Stop();
        audioPlaybackEngine_ = nullptr;
    }
}

void DeviceFuzzTestSetUp()
{
    if (playbackEngine_ != nullptr) {
        return;
    }
    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    deviceInfo.deviceType_ = DEVICE_TYPE_USB_HEADSET;
    playbackEngine_ = std::make_unique<NoneMixEngine>();
    bool isVoip = g_fuzzUtils.GetData<bool>();
    playbackEngine_->Init(deviceInfo, isVoip);
    playbackEngine_->Start();
    playbackEngine_->Flush();
    playbackEngine_->Pause();
    playbackEngine_->Stop();
    ReleaseNoneEngine();
}

static AudioProcessConfig InitProcessConfig()
{
    AudioProcessConfig config;
    config.appInfo.appUid = DEFAULT_STREAM_ID;
    config.appInfo.appPid = DEFAULT_STREAM_ID;
    config.streamInfo.format = SAMPLE_S32LE;
    config.streamInfo.samplingRate = SAMPLE_RATE_48000;
    config.streamInfo.channels = STEREO;
    config.streamInfo.channelLayout = AudioChannelLayout::CH_LAYOUT_STEREO;
    config.audioMode = AudioMode::AUDIO_MODE_RECORD;
    config.streamType = AudioStreamType::STREAM_MUSIC;
    config.deviceType = DEVICE_TYPE_USB_HEADSET;
    return config;
}

void DirectAudioPlayBackEngineStateFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    rendererStream->InitParams();
    uint32_t num = g_fuzzUtils.GetData<uint32_t>();
    rendererStream->SetStreamIndex(num);
    rendererStream->Start();
    rendererStream->Pause();
    rendererStream->Flush();
    rendererStream->Stop();
    rendererStream->Release();
}

void NoneMixEngineAddRendererFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    rendererStream->InitParams();
    uint32_t num = g_fuzzUtils.GetData<uint32_t>();
    rendererStream->SetStreamIndex(num);
    rendererStream->Start();
    playbackEngine_ = std::make_unique<NoneMixEngine>();
    playbackEngine_->AddRenderer(rendererStream);
    ReleaseNoneEngine();
}

void NoneMixEngineRemoveRendererFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    rendererStream->InitParams();
    uint32_t num = g_fuzzUtils.GetData<uint32_t>();
    rendererStream->SetStreamIndex(num);
    rendererStream->Start();
    playbackEngine_ = std::make_unique<NoneMixEngine>();
    playbackEngine_->AddRenderer(rendererStream);
    playbackEngine_->RemoveRenderer(rendererStream);
    ReleaseNoneEngine();
}

void PlaybackEngineInitFuzzTest()
{
    if (playbackEngine_ != nullptr) {
        return;
    }
    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    deviceInfo.deviceType_ = DEVICE_TYPE_USB_HEADSET;
    audioPlaybackEngine_ = std::make_unique<AudioPlaybackEngine>();
    bool isVoip = g_fuzzUtils.GetData<bool>();
    audioPlaybackEngine_->Init(deviceInfo, isVoip);
    audioPlaybackEngine_->Start();
    audioPlaybackEngine_->Flush();
    audioPlaybackEngine_->Pause();
    audioPlaybackEngine_->Stop();
    audioPlaybackEngine_->IsPlaybackEngineRunning();
    audioPlaybackEngine_->GetLatency();
    ReleaseAudioPlaybackEngine();
}

void PlaybackEngineAddRendererFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    rendererStream->InitParams();
    uint32_t num = g_fuzzUtils.GetData<uint32_t>();
    rendererStream->SetStreamIndex(num);
    rendererStream->Start();
    audioPlaybackEngine_ = std::make_unique<AudioPlaybackEngine>();
    audioPlaybackEngine_->AddRenderer(rendererStream);
    ReleaseAudioPlaybackEngine();
}

void PlaybackEngineRemoveRendererFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    rendererStream->InitParams();
    uint32_t num = g_fuzzUtils.GetData<uint32_t>();
    rendererStream->SetStreamIndex(num);
    rendererStream->Start();
    audioPlaybackEngine_ = std::make_unique<AudioPlaybackEngine>();
    audioPlaybackEngine_->AddRenderer(rendererStream);
    audioPlaybackEngine_->RemoveRenderer(rendererStream);
    ReleaseAudioPlaybackEngine();
}

void ResourceServiceAudioWorkgroupCheckFuzzTest()
{
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    AudioResourceService::GetInstance()->AudioWorkgroupCheck(pid);
}

void ResourceServiceCreateAudioWorkgroupFuzzTest()
{
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    sptr<IRemoteObject> remoteObject = nullptr;
    AudioResourceService::GetInstance()->CreateAudioWorkgroup(pid, remoteObject);
}

void ResourceServiceReleaseAudioWorkgroupFuzzTest()
{
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    int32_t workgroupId = g_fuzzUtils.GetData<int32_t>();
    AudioResourceService::GetInstance()->ReleaseAudioWorkgroup(pid, workgroupId);
}

void ResourceServiceAddThreadToGroupFuzzTest()
{
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    int32_t workgroupId = g_fuzzUtils.GetData<int32_t>();
    int32_t tokenId = g_fuzzUtils.GetData<int32_t>();
    AudioResourceService::GetInstance()->AddThreadToGroup(pid, workgroupId, tokenId);
}

void ResourceServiceRemoveThreadFromGroupFuzzTest()
{
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    int32_t workgroupId = g_fuzzUtils.GetData<int32_t>();
    int32_t tokenId = g_fuzzUtils.GetData<int32_t>();
    AudioResourceService::GetInstance()->RemoveThreadFromGroup(pid, workgroupId, tokenId);
}

void ResourceServiceStartGroupFuzzTest()
{
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    int32_t workgroupId = g_fuzzUtils.GetData<int32_t>();
    uint64_t startTime = g_fuzzUtils.GetData<uint64_t>();
    uint64_t deadlineTime = g_fuzzUtils.GetData<uint64_t>();
    AudioResourceService::GetInstance()->StartGroup(pid, workgroupId, startTime, deadlineTime);
}

void ResourceServiceStopGroupFuzzTest()
{
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    int32_t workgroupId = g_fuzzUtils.GetData<int32_t>();
    AudioResourceService::GetInstance()->StopGroup(pid, workgroupId);
}

void ResourceServiceGetAudioWorkgroupPtrFuzzTest()
{
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    int32_t workgroupId = g_fuzzUtils.GetData<int32_t>();
    AudioResourceService::GetInstance()->GetAudioWorkgroupPtr(pid, workgroupId);
}

void ResourceServiceGetThreadsNumPerProcessFuzzTest()
{
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    AudioResourceService::GetInstance()->GetThreadsNumPerProcess(pid);
}

void ResourceServiceIsProcessHasSystemPermissionFuzzTest()
{
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    AudioResourceService::GetInstance()->IsProcessHasSystemPermission(pid);
}

void ResourceServiceRegisterAudioWorkgroupMonitorFuzzTest()
{
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    int32_t groupId = g_fuzzUtils.GetData<int32_t>();
    sptr<IRemoteObject> object = nullptr;
    AudioResourceService::GetInstance()->RegisterAudioWorkgroupMonitor(pid, groupId, object);
}

void RenderInServerHandleOperationStartedFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;
    CHECK_AND_RETURN(renderer != nullptr);

    renderer->GetLastAudioDuration();
    renderer->StandByCheck();
    renderer->ShouldEnableStandBy();
    renderer->standByEnable_ = g_fuzzUtils.GetData<bool>();
    renderer->HandleOperationStarted();
}

void RenderInServerGetStandbyStatusFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;

    bool isStandby = g_fuzzUtils.GetData<bool>();
    int64_t enterStandbyTime = g_fuzzUtils.GetData<int64_t>();
    renderer->GetStandbyStatus(isStandby, enterStandbyTime);
}

void RenderInServerWriteMuteDataSysEventFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;
    CHECK_AND_RETURN(renderer != nullptr);

    BufferDesc desc;
    desc.buffer = nullptr;
    desc.bufLength = 0;
    desc.dataLength = 0;
    renderer->isInSilentState_ = g_fuzzUtils.GetData<bool>();
    renderer->WriteMuteDataSysEvent(desc);
}

void RenderInServerInnerCaptureEnqueueBufferFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;
    CHECK_AND_RETURN(renderer != nullptr);

    BufferDesc bufferDesc;
    bufferDesc.buffer = nullptr;
    bufferDesc.bufLength = 0;
    bufferDesc.dataLength = 0;
    CaptureInfo captureInfo;
    AudioProcessConfig audioProcessConfig;
    audioProcessConfig.streamType = STREAM_MUSIC;
    captureInfo.dupStream = std::make_shared<ProRendererStreamImpl>(audioProcessConfig, true);
    int32_t innerCapId = g_fuzzUtils.GetData<int32_t>();
    renderer->renderEmptyCountForInnerCap_ = g_fuzzUtils.GetData<int32_t>();
    renderer->InnerCaptureEnqueueBuffer(bufferDesc, captureInfo, innerCapId);
}

void RenderInServerInnerCaptureOtherStreamFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;
    BufferDesc bufferDesc;
    bufferDesc.buffer = nullptr;
    bufferDesc.bufLength = 0;
    bufferDesc.dataLength =0;
    CaptureInfo captureInfo;
    int32_t innerCapId = g_fuzzUtils.GetData<int32_t>();
    renderer->InnerCaptureOtherStream(bufferDesc, captureInfo, innerCapId);
}

void RenderInServerOtherStreamEnqueueFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;
    CHECK_AND_RETURN(renderer != nullptr);

    unsigned char inputData[] = "test";
    BufferDesc bufferDesc;
    bufferDesc.buffer = inputData;
    bufferDesc.bufLength = 0;
    bufferDesc.dataLength =0;
    renderer->isDualToneEnabled_ = g_fuzzUtils.GetData<bool>();
    renderer->OtherStreamEnqueue(bufferDesc);
}

void RenderInServerIsInvalidBufferFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;

    unsigned char inputData[] = "test";
    BufferDesc bufferDesc;
    bufferDesc.buffer = inputData;
    bufferDesc.bufLength = 0;
    bufferDesc.dataLength =0;
    renderer->IsInvalidBuffer(bufferDesc.buffer, bufferDesc.bufLength);
}

void RenderInServerRecordStandbyTimeFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;
    bool isStandby = false;
    bool isStandbyStart = g_fuzzUtils.GetData<bool>();
    renderer->dualToneStreamInStart();
    renderer->RecordStandbyTime(isStandby, isStandbyStart);
}

void ProRendererGetCurrentPositionFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    uint64_t framesWritten = 0;
    rendererStream->GetStreamFramesWritten(framesWritten);
    uint64_t timestamp = 0;
    rendererStream->GetCurrentTimeStamp(timestamp);
    uint64_t framePosition = 0;
    uint64_t latency = 0;
    rendererStream->GetLatency(latency);
    uint32_t base = g_fuzzUtils.GetData<uint32_t>();
    rendererStream->GetCurrentPosition(framePosition, timestamp, latency, base);
}

void ProRendererSetAudioEffectModeFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    int32_t effectMode = g_fuzzUtils.GetData<int32_t>();
    rendererStream->SetAudioEffectMode(effectMode);
    rendererStream->GetAudioEffectMode(effectMode);
}

void ProRendererSetPrivacyTypeFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    int32_t privacyType = g_fuzzUtils.GetData<int32_t>();
    rendererStream->SetPrivacyType(privacyType);
    rendererStream->GetPrivacyType(privacyType);
}

void ProRendererSetSpeedFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    float speed = g_fuzzUtils.GetData<float>();
    rendererStream->SetSpeed(speed);
}

void ProRendererDequeueBufferFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    size_t length = g_fuzzUtils.GetData<size_t>();
    rendererStream->DequeueBuffer(length);
    BufferDesc bufferDesc;
    bufferDesc.buffer = nullptr;
    bufferDesc.bufLength = 0;
    bufferDesc.dataLength = 0;
    rendererStream->EnqueueBuffer(bufferDesc);
}

void ProRendererOffloadSetVolumeFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    size_t minBufferSize = 0;
    rendererStream->GetMinimumBufferSize(minBufferSize);
    size_t byteSizePerFrame = 0;
    rendererStream->GetByteSizePerFrame(byteSizePerFrame);
    size_t spanSizeInFrame = 0;
    rendererStream->GetSpanSizePerFrame(spanSizeInFrame);
    rendererStream->GetStreamIndex();
    float volume = g_fuzzUtils.GetData<float>();
    rendererStream->OffloadSetVolume(volume);
}

void ProRendererSetOffloadDataCallbackStateFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    int32_t state = g_fuzzUtils.GetData<int32_t>();
    rendererStream->SetOffloadDataCallbackState(state);
}

void ProRendererUpdateSpatializationStateFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    bool spatializationEnabled = g_fuzzUtils.GetData<bool>();
    bool headTrackingEnabled = g_fuzzUtils.GetData<bool>();
    rendererStream->UpdateSpatializationState(spatializationEnabled, headTrackingEnabled);
}

void ProRendererReturnIndexFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    uint64_t framePos = 0;
    int64_t sec = 0;
    int64_t nanoSec = 0;
    rendererStream->GetAudioTime(framePos, sec, nanoSec);
    int32_t index = 0;
    std::vector<char> audioBuffer = {};
    rendererStream->Peek(&audioBuffer, index);
    index = g_fuzzUtils.GetData<int32_t>();
    rendererStream->ReturnIndex(index);
}

void ProRendererSetClientVolumeFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    float clientVolume = g_fuzzUtils.GetData<float>();
    rendererStream->SetClientVolume(clientVolume);
}

void ProRendererSetLoudnessGainFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    float loudnessGain = g_fuzzUtils.GetData<float>();
    rendererStream->SetLoudnessGain(loudnessGain);
}

void ProRendererUpdateMaxLengthFuzzTest()
{
    AudioProcessConfig config = InitProcessConfig();
    std::shared_ptr<ProRendererStreamImpl> rendererStream = std::make_shared<ProRendererStreamImpl>(config, true);
    uint32_t maxLength = g_fuzzUtils.GetData<uint32_t>();
    rendererStream->UpdateMaxLength(maxLength);
    int32_t index = 0;
    std::vector<char> audioBuffer = {};
    rendererStream->PopSinkBuffer(&audioBuffer, index);
    rendererStream->GetStreamVolume();
}

void ReConfigDupStreamCallbackFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;
    CHECK_AND_RETURN(renderer != nullptr);

    renderer->dupTotalSizeInFrame_ = g_fuzzUtils.GetData<size_t>();
    renderer->ReConfigDupStreamCallback();
    RingBufferWrapper bufferDesc;
    renderer->DoFadingOut(bufferDesc);
    renderer->PrepareOutputBuffer(bufferDesc);
}

void CopyDataToInputBufferFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;
    CHECK_AND_RETURN(renderer != nullptr);

    int8_t* inPutData = nullptr;
    size_t requestDataLen = g_fuzzUtils.dataSize_ / TEST_NUM_TWO;
    for (size_t i = 0; i < requestDataLen; i++) {
        inPutData[i] = g_fuzzUtils.GetData<int8_t>();
    }
    RingBufferWrapper ringBufferDesc;
    renderer->CopyDataToInputBuffer(inPutData, requestDataLen, ringBufferDesc);
}

void OnWriteDataFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;
    CHECK_AND_RETURN(renderer != nullptr);

    size_t length = g_fuzzUtils.GetData<size_t>();
    renderer->OnWriteData(length);
}

void PauseFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;
    CHECK_AND_RETURN(renderer != nullptr);

    renderer->standByEnable_ = g_fuzzUtils.GetData<bool>();
    renderer->Pause();
    renderer->DisableAllInnerCap();
}

void OnStatusUpdateFuzzTest()
{
    uint32_t streamIndex = g_fuzzUtils.GetData<uint32_t>();
    auto StreamCallbacksPtr = std::make_shared<StreamCallbacks>(streamIndex);
    CHECK_AND_RETURN(StreamCallbacksPtr != nullptr);

    IOperation operation = g_fuzzUtils.GetData<IOperation>();
    StreamCallbacksPtr->OnStatusUpdate(operation);
}

void OnWriteDataStreamsCallbackFuzzTest()
{
    uint32_t streamIndex = g_fuzzUtils.GetData<uint32_t>();
    auto StreamCallbacksPtr = std::make_shared<StreamCallbacks>(streamIndex);
    CHECK_AND_RETURN(StreamCallbacksPtr != nullptr);

    size_t length = g_fuzzUtils.GetData<size_t>();
    StreamCallbacksPtr->OnWriteData(length);
}

void GetAvailableSizeStreamsCallbackFuzzTest()
{
    uint32_t streamIndex = g_fuzzUtils.GetData<uint32_t>();
    auto StreamCallbacksPtr = std::make_shared<StreamCallbacks>(streamIndex);
    CHECK_AND_RETURN(StreamCallbacksPtr != nullptr);

    size_t length = g_fuzzUtils.GetData<size_t>();
    StreamCallbacksPtr->GetAvailableSize(length);
}

void SetMuteFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;
    CHECK_AND_RETURN(renderer != nullptr);

    renderer->IsHighResolution();
    bool isMute = g_fuzzUtils.GetData<bool>();
    renderer->SetMute(isMute);
}

void SetDuckFactorFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;
    CHECK_AND_RETURN(renderer != nullptr);

    float duckFactor = g_fuzzUtils.GetData<float>();
    renderer->SetDuckFactor(duckFactor);
}

void SetDefaultOutputDeviceFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;
    CHECK_AND_RETURN(renderer != nullptr);

    DeviceType defaultOutputDevice = g_fuzzUtils.GetData<DeviceType>();
    renderer->SetDefaultOutputDevice(defaultOutputDevice);
}

void SetSpeedFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;
    CHECK_AND_RETURN(renderer != nullptr);

    float speed = g_fuzzUtils.GetData<float>();
    renderer->SetSpeed(speed);
}

void InitDupBufferFuzzTest()
{
    AudioProcessConfig processConfig;
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    std::weak_ptr<IStreamListener> streamListener = streamListenerHolder;
    std::shared_ptr<RendererInServer> rendererInServer =
        std::make_shared<RendererInServer>(processConfig, streamListener);
    std::shared_ptr<RendererInServer> renderer = rendererInServer;
    CHECK_AND_RETURN(renderer != nullptr);

    int32_t innerCapId = g_fuzzUtils.GetData<int32_t>();
    renderer->InitDupBuffer(innerCapId);
    renderer->StopSession();
}

vector<TestFuncs> g_testFuncs = {
    DeviceFuzzTestSetUp,
    DirectAudioPlayBackEngineStateFuzzTest,
    NoneMixEngineAddRendererFuzzTest,
    NoneMixEngineRemoveRendererFuzzTest,
    PlaybackEngineInitFuzzTest,
    PlaybackEngineAddRendererFuzzTest,
    PlaybackEngineRemoveRendererFuzzTest,
    ResourceServiceAudioWorkgroupCheckFuzzTest,
    ResourceServiceCreateAudioWorkgroupFuzzTest,
    ResourceServiceReleaseAudioWorkgroupFuzzTest,
    ResourceServiceAddThreadToGroupFuzzTest,
    ResourceServiceRemoveThreadFromGroupFuzzTest,
    ResourceServiceStartGroupFuzzTest,
    ResourceServiceStopGroupFuzzTest,
    ResourceServiceGetAudioWorkgroupPtrFuzzTest,
    ResourceServiceGetThreadsNumPerProcessFuzzTest,
    ResourceServiceIsProcessHasSystemPermissionFuzzTest,
    ResourceServiceRegisterAudioWorkgroupMonitorFuzzTest,
    RenderInServerHandleOperationStartedFuzzTest,
    RenderInServerGetStandbyStatusFuzzTest,
    RenderInServerWriteMuteDataSysEventFuzzTest,
    RenderInServerInnerCaptureEnqueueBufferFuzzTest,
    RenderInServerInnerCaptureOtherStreamFuzzTest,
    RenderInServerOtherStreamEnqueueFuzzTest,
    RenderInServerIsInvalidBufferFuzzTest,
    RenderInServerRecordStandbyTimeFuzzTest,
    ProRendererGetCurrentPositionFuzzTest,
    ProRendererSetAudioEffectModeFuzzTest,
    ProRendererSetPrivacyTypeFuzzTest,
    ProRendererSetSpeedFuzzTest,
    ProRendererDequeueBufferFuzzTest,
    ProRendererOffloadSetVolumeFuzzTest,
    ProRendererSetOffloadDataCallbackStateFuzzTest,
    ProRendererUpdateSpatializationStateFuzzTest,
    ProRendererReturnIndexFuzzTest,
    ProRendererSetClientVolumeFuzzTest,
    ProRendererSetLoudnessGainFuzzTest,
    ProRendererUpdateMaxLengthFuzzTest,
    ReConfigDupStreamCallbackFuzzTest,
    CopyDataToInputBufferFuzzTest,
    OnWriteDataFuzzTest,
    PauseFuzzTest,
    OnStatusUpdateFuzzTest,
    OnWriteDataStreamsCallbackFuzzTest,
    GetAvailableSizeStreamsCallbackFuzzTest,
    SetMuteFuzzTest,
    SetDuckFactorFuzzTest,
    SetDefaultOutputDeviceFuzzTest,
    SetSpeedFuzzTest,
    InitDupBufferFuzzTest,
};
} // namespace AudioStandard
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    OHOS::AudioStandard::g_fuzzUtils.fuzzTest(data, size, OHOS::AudioStandard::g_testFuncs);
    return 0;
}