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
#include "capturer_in_server.h"
#include "ipc_stream_in_server.h"
#include "../fuzz_utils.h"

namespace OHOS {
namespace AudioStandard {
using namespace std;

FuzzUtils &g_fuzzUtils = FuzzUtils::GetInstance();
const size_t FUZZ_INPUT_SIZE_THRESHOLD = 10;
const uint32_t APPID_LENGTH = 10;
std::shared_ptr<CapturerInServer> capturerInServer_ = nullptr;

typedef void (*TestFuncs)();

class ICapturerStreamTest : public ICapturerStream {
public:
    int32_t GetStreamFramesRead(uint64_t &framesRead) override { return 0; }
    int32_t GetCurrentTimeStamp(uint64_t &timestamp) override { return 0; }
    int32_t GetLatency(uint64_t &latency) override { return 0; }
    void RegisterReadCallback(const std::weak_ptr<IReadCallback> &callback) override { return; }
    int32_t GetMinimumBufferSize(size_t &minBufferSize) const override { return 0; }
    void GetByteSizePerFrame(size_t &byteSizePerFrame) const override { return; }
    void GetSpanSizePerFrame(size_t &spanSizeInFrame) const override { spanSizeInFrame = 0; }
    int32_t DropBuffer() override { return 0; }
    void SetStreamIndex(uint32_t index) override { return; }
    uint32_t GetStreamIndex() override { return 0; }
    int32_t Start() override { return 0; }
    int32_t Pause(bool isStandby = false) override { return 0; }
    int32_t Flush() override { return 0; }
    int32_t Drain(bool stopFlag = false) override { return 0; }
    int32_t Stop() override { return 0; }
    int32_t Release() override { return 0; }
    void RegisterStatusCallback(const std::weak_ptr<IStatusCallback> &callback) override { return; }
    BufferDesc DequeueBuffer(size_t length) override
    {
        BufferDesc bufferDesc;
        return bufferDesc;
    }
    int32_t EnqueueBuffer(const BufferDesc &bufferDesc) override { return 0; }
};

class ConcreteIStreamListener : public IStreamListener {
    int32_t OnOperationHandled(Operation operation, int64_t result) { return SUCCESS; }
};

static AudioProcessConfig GetInnerCapConfig()
{
    AudioProcessConfig config;
    config.appInfo.appUid = APPID_LENGTH;
    config.appInfo.appPid = APPID_LENGTH;
    config.streamInfo.format = SAMPLE_S32LE;
    config.streamInfo.samplingRate = SAMPLE_RATE_48000;
    config.streamInfo.channels = STEREO;
    config.streamInfo.channelLayout = AudioChannelLayout::CH_LAYOUT_STEREO;
    config.audioMode = AudioMode::AUDIO_MODE_PLAYBACK;
    config.streamType = AudioStreamType::STREAM_MUSIC;
    config.deviceType = DEVICE_TYPE_USB_HEADSET;
    return config;
}

void Init()
{
    AudioProcessConfig config = GetInnerCapConfig();
    std::weak_ptr<IStreamListener> innerListener = std::weak_ptr<IStreamListener>();
    capturerInServer_ = std::make_shared<CapturerInServer>(config, innerListener);
}

void OnStatusUpdateFuzzTest()
{
    Init();
    CHECK_AND_RETURN(capturerInServer_ != nullptr);
    std::shared_ptr<StreamListenerHolder> streamListenerHolder =
        std::make_shared<StreamListenerHolder>();
    CHECK_AND_RETURN(streamListenerHolder != nullptr);
    capturerInServer_->streamListener_ = std::weak_ptr<IStreamListener>();
    if (capturerInServer_->streamListener_.lock() == nullptr) {
        capturerInServer_->streamListener_ = streamListenerHolder;
    }
    AppInfo appInfo;
    appInfo.appUid = g_fuzzUtils.GetData<int32_t>();
    appInfo.appPid = g_fuzzUtils.GetData<int32_t>();
    appInfo.appTokenId = g_fuzzUtils.GetData<uint32_t>();
    appInfo.appFullTokenId = g_fuzzUtils.GetData<uint64_t>();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    capturerInServer_->recorderDfx_ = std::make_unique<RecorderDfxWriter>(appInfo, index);
    capturerInServer_->OnStatusUpdate(g_fuzzUtils.GetData<IOperation>());
}

void DequeueBufferFuzzTest()
{
    Init();
    CHECK_AND_RETURN(capturerInServer_ != nullptr);
    capturerInServer_->status_ = g_fuzzUtils.GetData<IStatus>();
    capturerInServer_->HandleOperationFlushed();

    size_t length = g_fuzzUtils.GetData<size_t>();
    capturerInServer_->stream_ = std::make_shared<ICapturerStreamTest>();
    CHECK_AND_RETURN(capturerInServer_->stream_ != nullptr);
    capturerInServer_->DequeueBuffer(length);
}

void IsReadDataOverFlowFuzzTest()
{
    Init();
    CHECK_AND_RETURN(capturerInServer_ != nullptr);
    size_t length = g_fuzzUtils.GetData<size_t>();
    uint64_t currentWriteFrame = g_fuzzUtils.GetData<uint64_t>();
    uint32_t totalSizeInFrame = g_fuzzUtils.GetData<uint32_t>();
    uint32_t spanSizeInFrame = g_fuzzUtils.GetData<uint32_t>();
    uint32_t byteSizePerFrame = g_fuzzUtils.GetData<uint32_t>();
    std::shared_ptr<IStreamListener> stateListener = std::make_shared<ConcreteIStreamListener>();
    CHECK_AND_RETURN(stateListener != nullptr);
    capturerInServer_->stream_ = std::make_shared<ICapturerStreamTest>();
    CHECK_AND_RETURN(capturerInServer_->stream_ != nullptr);
    capturerInServer_->audioServerBuffer_ = std::make_shared<OHAudioBuffer>(g_fuzzUtils.GetData<AudioBufferHolder>(),
        totalSizeInFrame, spanSizeInFrame, byteSizePerFrame);
    CHECK_AND_RETURN(capturerInServer_->audioServerBuffer_ != nullptr);
    auto bufferInfo = std::make_shared<BasicBufferInfo>();
    CHECK_AND_RETURN(bufferInfo != nullptr);
    capturerInServer_->audioServerBuffer_->ohAudioBufferBase_.basicBufferInfo_ = bufferInfo.get();
    CHECK_AND_RETURN(capturerInServer_->audioServerBuffer_->ohAudioBufferBase_.basicBufferInfo_ != nullptr);
    capturerInServer_->IsReadDataOverFlow(length, currentWriteFrame, stateListener);
}

void UpdateBufferTimeStampFuzzTest()
{
    Init();
    CHECK_AND_RETURN(capturerInServer_ != nullptr);
    uint32_t capturerSampleRate = g_fuzzUtils.GetData<uint32_t>();
    size_t readLen = g_fuzzUtils.GetData<size_t>();
    capturerInServer_->capturerClock_ = std::make_shared<CapturerClock>(capturerSampleRate);
    CHECK_AND_RETURN(capturerInServer_->capturerClock_ != nullptr);
    uint32_t totalSizeInFrame = g_fuzzUtils.GetData<uint32_t>();
    uint32_t spanSizeInFrame = g_fuzzUtils.GetData<uint32_t>();
    uint32_t byteSizePerFrame = g_fuzzUtils.GetData<uint32_t>();
    capturerInServer_->audioServerBuffer_ = std::make_shared<OHAudioBuffer>(AudioBufferHolder::AUDIO_CLIENT,
        totalSizeInFrame, spanSizeInFrame, byteSizePerFrame);
    CHECK_AND_RETURN(capturerInServer_->audioServerBuffer_ != nullptr);
    capturerInServer_->processConfig_ = GetInnerCapConfig();
    capturerInServer_->processConfig_.streamInfo.format = g_fuzzUtils.GetData<AudioSampleFormat>();
    capturerInServer_->UpdateBufferTimeStamp(readLen);
}

void ReadDataFuzzTest()
{
    Init();
    CHECK_AND_RETURN(capturerInServer_ != nullptr);
    size_t cacheSize = g_fuzzUtils.GetData<size_t>();
    capturerInServer_->ringCache_ = AudioRingCache::Create(cacheSize);
    CHECK_AND_RETURN(capturerInServer_->ringCache_ != nullptr);
    size_t length = g_fuzzUtils.GetData<size_t>();
    capturerInServer_->ReadData(length);
}

void OnReadDataFuzzTest()
{
    Init();
    CHECK_AND_RETURN(capturerInServer_ != nullptr);
    size_t length = g_fuzzUtils.GetData<size_t>();
    capturerInServer_->OnReadData(length);
    int8_t outputData = g_fuzzUtils.GetData<int8_t>();
    size_t requestDataLen = g_fuzzUtils.GetData<size_t>();
    capturerInServer_->OnReadData(&outputData, requestDataLen);
}

void RestoreSessionFuzzTest()
{
    Init();
    CHECK_AND_RETURN(capturerInServer_ != nullptr);
    RestoreInfo restoreInfo;
    restoreInfo.restoreReason = g_fuzzUtils.GetData<RestoreReason>();
    restoreInfo.deviceChangeReason = g_fuzzUtils.GetData<int32_t>();
    restoreInfo.targetStreamFlag = g_fuzzUtils.GetData<int32_t>();
    restoreInfo.routeFlag = g_fuzzUtils.GetData<uint32_t>();
    uint32_t totalSizeInFrame = g_fuzzUtils.GetData<uint32_t>();
    uint32_t spanSizeInFrame = g_fuzzUtils.GetData<uint32_t>();
    uint32_t byteSizePerFrame = g_fuzzUtils.GetData<uint32_t>();
    capturerInServer_->audioServerBuffer_ = std::make_shared<OHAudioBuffer>(AudioBufferHolder::AUDIO_CLIENT,
        totalSizeInFrame, spanSizeInFrame, byteSizePerFrame);
    CHECK_AND_RETURN(capturerInServer_->audioServerBuffer_ != nullptr);
    capturerInServer_->RestoreSession(restoreInfo);
}

void GetLatencyFuzzTest()
{
    Init();
    CHECK_AND_RETURN(capturerInServer_ != nullptr);
    capturerInServer_->StopSession();

    capturerInServer_->lastStopTime_ = g_fuzzUtils.GetData<int64_t>();
    capturerInServer_->lastStartTime_ = 0;
    capturerInServer_->GetLastAudioDuration();

    capturerInServer_->stream_ = std::make_shared<ICapturerStreamTest>();
    CHECK_AND_RETURN(capturerInServer_->stream_ != nullptr);
    uint64_t latency = g_fuzzUtils.GetData<uint64_t>();
    capturerInServer_->GetLatency(latency);
}

void GetAudioTimeFuzzTest()
{
    Init();
    CHECK_AND_RETURN(capturerInServer_ != nullptr);
    capturerInServer_->stream_ = std::make_shared<ICapturerStreamTest>();
    CHECK_AND_RETURN(capturerInServer_->stream_ != nullptr);
    uint64_t framePos = g_fuzzUtils.GetData<uint64_t>();
    uint64_t timestamp = g_fuzzUtils.GetData<uint64_t>();
    capturerInServer_->status_ = g_fuzzUtils.GetData<IStatus>();
    capturerInServer_->resetTime_ = g_fuzzUtils.GetData<bool>();
    capturerInServer_->GetAudioTime(framePos, timestamp);
}

void UpdatePlaybackCaptureConfigFuzzTest()
{
    Init();
    CHECK_AND_RETURN(capturerInServer_ != nullptr);
    #ifdef HAS_FEATURE_INNERCAPTURER
    AudioPlaybackCaptureConfig config;
    config.filterOptions.usages.push_back(g_fuzzUtils.GetData<StreamUsage>());
    capturerInServer_->processConfig_.capturerInfo.sourceType = g_fuzzUtils.GetData<SourceType>();
    capturerInServer_->UpdatePlaybackCaptureConfig(config);
    #endif
}

void UpdatePlaybackCaptureConfigInLegacyFuzzTest()
{
    Init();
    CHECK_AND_RETURN(capturerInServer_ != nullptr);
    #ifdef HAS_FEATURE_INNERCAPTURER
    AudioPlaybackCaptureConfig config;
    config.filterOptions.usages.push_back(g_fuzzUtils.GetData<StreamUsage>());
    capturerInServer_->UpdatePlaybackCaptureConfigInLegacy(config);
    #endif
}

void InitCacheBufferFuzzTest()
{
    Init();
    CHECK_AND_RETURN(capturerInServer_ != nullptr);
    capturerInServer_->status_ = g_fuzzUtils.GetData<IStatus>();
    capturerInServer_->needCheckBackground_ = g_fuzzUtils.GetData<bool>();
    capturerInServer_->streamIndex_ = g_fuzzUtils.GetData<uint32_t>();
    capturerInServer_->stream_ = std::make_shared<ICapturerStreamTest>();
    CHECK_AND_RETURN(capturerInServer_->stream_ != nullptr);
    capturerInServer_->Pause();

    uint32_t totalSizeInFrame = g_fuzzUtils.GetData<uint32_t>();
    uint32_t spanSizeInFrame = g_fuzzUtils.GetData<uint32_t>();
    uint32_t byteSizePerFrame = g_fuzzUtils.GetData<uint32_t>();
    capturerInServer_->audioServerBuffer_ = std::make_shared<OHAudioBuffer>(g_fuzzUtils.GetData<AudioBufferHolder>(),
        totalSizeInFrame, spanSizeInFrame, byteSizePerFrame);
    CHECK_AND_RETURN(capturerInServer_->audioServerBuffer_ != nullptr);
    capturerInServer_->Flush();

    uint32_t capturerSampleRate = g_fuzzUtils.GetData<uint32_t>();
    capturerInServer_->capturerClock_ = std::make_shared<CapturerClock>(capturerSampleRate);
    CHECK_AND_RETURN(capturerInServer_->capturerClock_ != nullptr);
    capturerInServer_->needCheckBackground_ = g_fuzzUtils.GetData<bool>();
    capturerInServer_->Stop();

    capturerInServer_->processConfig_.capturerInfo.sourceType = g_fuzzUtils.GetData<SourceType>();
    capturerInServer_->Release();

    size_t targetSize = g_fuzzUtils.GetData<size_t>();
    size_t cacheSize = g_fuzzUtils.GetData<size_t>();
    capturerInServer_->ringCache_ = AudioRingCache::Create(cacheSize);
    CHECK_AND_RETURN(capturerInServer_->ringCache_ != nullptr);
    capturerInServer_->spanSizeInBytes_ = g_fuzzUtils.GetData<size_t>();
    capturerInServer_->InitCacheBuffer(targetSize);
}

vector<TestFuncs> g_testFuncs = {
    OnStatusUpdateFuzzTest,
    DequeueBufferFuzzTest,
    IsReadDataOverFlowFuzzTest,
    UpdateBufferTimeStampFuzzTest,
    ReadDataFuzzTest,
    OnReadDataFuzzTest,
    RestoreSessionFuzzTest,
    GetLatencyFuzzTest,
    GetAudioTimeFuzzTest,
    UpdatePlaybackCaptureConfigFuzzTest,
    UpdatePlaybackCaptureConfigInLegacyFuzzTest,
    InitCacheBufferFuzzTest,
};

} // namespace AudioStandard
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    if (size < OHOS::AudioStandard::FUZZ_INPUT_SIZE_THRESHOLD) {
        return 0;
    }

    OHOS::AudioStandard::g_fuzzUtils.fuzzTest(data, size, OHOS::AudioStandard::g_testFuncs);
    return 0;
}
