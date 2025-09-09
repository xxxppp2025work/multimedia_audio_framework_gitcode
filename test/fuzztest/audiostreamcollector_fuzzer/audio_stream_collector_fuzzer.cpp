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

#include "audio_stream_collector.h"
#include "istandard_client_tracker.h"
#include "audio_client_tracker_callback_listener.h"
#include "../fuzz_utils.h"
using namespace std;

namespace OHOS {
namespace AudioStandard {
FuzzUtils &g_fuzzUtils = FuzzUtils::GetInstance();
AudioStreamCollector audioStreamCollector_;
const int32_t NUM_2 = 2;
typedef void (*TestFuncs)();

const vector<DeviceType> g_testDeviceTypes = {
    DEVICE_TYPE_NONE,
    DEVICE_TYPE_INVALID,
    DEVICE_TYPE_EARPIECE,
    DEVICE_TYPE_SPEAKER,
    DEVICE_TYPE_WIRED_HEADSET,
    DEVICE_TYPE_WIRED_HEADPHONES,
    DEVICE_TYPE_BLUETOOTH_SCO,
    DEVICE_TYPE_BLUETOOTH_A2DP,
    DEVICE_TYPE_BLUETOOTH_A2DP_IN,
    DEVICE_TYPE_MIC,
    DEVICE_TYPE_WAKEUP,
    DEVICE_TYPE_USB_HEADSET,
    DEVICE_TYPE_DP,
    DEVICE_TYPE_REMOTE_CAST,
    DEVICE_TYPE_USB_DEVICE,
    DEVICE_TYPE_ACCESSORY,
    DEVICE_TYPE_REMOTE_DAUDIO,
    DEVICE_TYPE_HDMI,
    DEVICE_TYPE_LINE_DIGITAL,
    DEVICE_TYPE_NEARLINK,
    DEVICE_TYPE_NEARLINK_IN,
    DEVICE_TYPE_FILE_SINK,
    DEVICE_TYPE_FILE_SOURCE,
    DEVICE_TYPE_EXTERN_CABLE,
    DEVICE_TYPE_DEFAULT,
    DEVICE_TYPE_USB_ARM_HEADSET,
    DEVICE_TYPE_MAX,
};

void AudioStreamCollectorAddRendererStreamFuzzTest()
{
    AudioStreamChangeInfo streamChangeInfo;
    uint32_t randIntValue = g_fuzzUtils.GetData<bool>();
    streamChangeInfo.audioRendererChangeInfo.clientUID = randIntValue;
    streamChangeInfo.audioRendererChangeInfo.sessionId = randIntValue++;
    streamChangeInfo.audioRendererChangeInfo.channelCount = randIntValue++;
    streamChangeInfo.audioRendererChangeInfo.createrUID = randIntValue--;
    streamChangeInfo.audioRendererChangeInfo.rendererState = g_fuzzUtils.GetData<RendererState>();
    streamChangeInfo.audioRendererChangeInfo.rendererInfo.pipeType = g_fuzzUtils.GetData<AudioPipeType>();
    audioStreamCollector_.AddRendererStream(streamChangeInfo);
}

void AudioStreamCollectorGetRendererStreamInfoFuzzTest()
{
    AudioStreamChangeInfo streamChangeInfo;
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    streamChangeInfo.audioCapturerChangeInfo.clientUID = randIntValue;
    streamChangeInfo.audioCapturerChangeInfo.sessionId = randIntValue + 1;
    AudioRendererChangeInfo rendererInfo;
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    rendererChangeInfo->clientUID = randIntValue;
    rendererChangeInfo->createrUID = randIntValue;
    rendererChangeInfo->sessionId = randIntValue + 1;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));
    audioStreamCollector_.GetRendererStreamInfo(streamChangeInfo, rendererInfo);
}

void AudioStreamCollectorGetCapturerStreamInfoFuzzTest()
{
    AudioStreamChangeInfo streamChangeInfo;
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    streamChangeInfo.audioCapturerChangeInfo.clientUID = randIntValue;
    streamChangeInfo.audioCapturerChangeInfo.sessionId = randIntValue + 1;
    AudioCapturerChangeInfo capturerChangeInfo;
    shared_ptr<AudioCapturerChangeInfo> rendererChangeInfo = make_shared<AudioCapturerChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    rendererChangeInfo->clientUID = randIntValue;
    rendererChangeInfo->createrUID = randIntValue;
    rendererChangeInfo->sessionId = randIntValue + 1;
    audioStreamCollector_.audioCapturerChangeInfos_.clear();
    audioStreamCollector_.audioCapturerChangeInfos_.push_back(move(rendererChangeInfo));
    audioStreamCollector_.GetCapturerStreamInfo(streamChangeInfo, capturerChangeInfo);
}

void AudioStreamCollectorGetPipeTypeFuzzTest()
{
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t sessionId = randIntValue;
    uint32_t index = g_fuzzUtils.GetData<AudioPipeType>();
    AudioPipeType pipeType = g_fuzzUtils.GetData<AudioPipeType>();
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));
    audioStreamCollector_.GetPipeType(sessionId, pipeType);
}

void AudioStreamCollectorExistStreamForPipeFuzzTest()
{
    AudioPipeType pipeType = g_fuzzUtils.GetData<AudioPipeType>();
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    AudioStreamChangeInfo streamChangeInfo;
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    streamChangeInfo.audioRendererChangeInfo.clientUID = randIntValue;
    streamChangeInfo.audioRendererChangeInfo.sessionId = randIntValue + 1;
    streamChangeInfo.audioRendererChangeInfo.rendererState = g_fuzzUtils.GetData<RendererState>();

    bool result = g_fuzzUtils.GetData<bool>();
    if (result) {
        rendererChangeInfo->createrUID = streamChangeInfo.audioRendererChangeInfo.createrUID;
        rendererChangeInfo->clientUID = streamChangeInfo.audioRendererChangeInfo.clientUID;
        audioStreamCollector_.audioRendererChangeInfos_.clear();
        audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));
        audioStreamCollector_.audioRendererChangeInfos_[0]->rendererInfo.pipeType = pipeType;
    }
    audioStreamCollector_.ExistStreamForPipe(pipeType);
}

void AudioStreamCollectorGetRendererDeviceInfoFuzzTest()
{
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t sessionId = randIntValue;
    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    bool result = g_fuzzUtils.GetData<bool>();
    if (result) {
        rendererChangeInfo->clientUID = randIntValue;
        rendererChangeInfo->createrUID = randIntValue;
        rendererChangeInfo->sessionId = randIntValue + 1;
        uint32_t index = g_fuzzUtils.GetData<AudioPipeType>();
        rendererChangeInfo->rendererInfo.pipeType = g_fuzzUtils.GetData<AudioPipeType>();
        audioStreamCollector_.audioRendererChangeInfos_.clear();
        audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));
    }
    audioStreamCollector_.GetRendererDeviceInfo(sessionId, deviceInfo);
}

void AudioStreamCollectorAddCapturerStreamFuzzTest()
{
    AudioStreamChangeInfo streamChangeInfo;
    int32_t randIntValue = g_fuzzUtils.GetData<bool>();
    streamChangeInfo.audioRendererChangeInfo.clientUID = randIntValue;
    streamChangeInfo.audioRendererChangeInfo.sessionId = randIntValue++;
    streamChangeInfo.audioRendererChangeInfo.channelCount = randIntValue++;
    streamChangeInfo.audioRendererChangeInfo.createrUID = randIntValue--;
    streamChangeInfo.audioRendererChangeInfo.rendererState = g_fuzzUtils.GetData<RendererState>();
    streamChangeInfo.audioRendererChangeInfo.rendererInfo.pipeType = g_fuzzUtils.GetData<AudioPipeType>();
    audioStreamCollector_.AddCapturerStream(streamChangeInfo);
}

void AudioStreamCollectorSendCapturerInfoEventFuzzTest()
{
    AudioDeviceDescriptor inputDeviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    shared_ptr<AudioCapturerChangeInfo> captureChangeInfo = make_shared<AudioCapturerChangeInfo>();
    CHECK_AND_RETURN(captureChangeInfo != nullptr);
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    captureChangeInfo->clientUID = randIntValue;
    captureChangeInfo->createrUID = randIntValue / NUM_2;
    captureChangeInfo->sessionId = randIntValue / NUM_2 + 1;
    captureChangeInfo->inputDeviceInfo = inputDeviceInfo;
    audioStreamCollector_.audioCapturerChangeInfos_.clear();
    audioStreamCollector_.audioCapturerChangeInfos_.push_back(captureChangeInfo);

    std::vector<std::shared_ptr<AudioCapturerChangeInfo>> audioCapturerChangeInfos;
    audioCapturerChangeInfos.push_back(captureChangeInfo);
    audioStreamCollector_.SendCapturerInfoEvent(audioCapturerChangeInfos);
}

void AudioStreamCollectorRegisterTrackerFuzzTest()
{
    AudioMode audioMode = AudioMode::AUDIO_MODE_PLAYBACK;
    AudioStreamChangeInfo streamChangeInfo;
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    streamChangeInfo.audioRendererChangeInfo.clientUID = randIntValue / NUM_2;
    streamChangeInfo.audioRendererChangeInfo.sessionId = randIntValue;
    uint32_t index = g_fuzzUtils.GetData<RendererState>();
    streamChangeInfo.audioRendererChangeInfo.rendererState = g_fuzzUtils.GetData<RendererState>();
    sptr<IRemoteObject> clientTrackerObj = nullptr;

    audioStreamCollector_.RegisterTracker(audioMode, streamChangeInfo, clientTrackerObj);
    audioStreamCollector_.UpdateTracker(audioMode, streamChangeInfo);
}

void AudioStreamCollectorSetRendererStreamParamFuzzTest()
{
    AudioStreamChangeInfo streamChangeInfo;
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    streamChangeInfo.audioRendererChangeInfo.clientUID = randIntValue / NUM_2;
    streamChangeInfo.audioRendererChangeInfo.sessionId = randIntValue;
    uint32_t index = g_fuzzUtils.GetData<RendererState>();
    streamChangeInfo.audioRendererChangeInfo.rendererState = g_fuzzUtils.GetData<RendererState>();
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    audioStreamCollector_.SetRendererStreamParam(streamChangeInfo, rendererChangeInfo);
}

void AudioStreamCollectorSetCapturerStreamParamFuzzTest()
{
    AudioStreamChangeInfo streamChangeInfo;
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    streamChangeInfo.audioRendererChangeInfo.clientUID = randIntValue / NUM_2;
    streamChangeInfo.audioRendererChangeInfo.sessionId = randIntValue;
    uint32_t index = g_fuzzUtils.GetData<RendererState>();
    streamChangeInfo.audioRendererChangeInfo.rendererState = g_fuzzUtils.GetData<RendererState>();
    shared_ptr<AudioCapturerChangeInfo> rendererChangeInfo = make_shared<AudioCapturerChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    audioStreamCollector_.SetCapturerStreamParam(streamChangeInfo, rendererChangeInfo);
}

void AudioStreamCollectorResetRendererStreamDeviceInfoFuzzTest()
{
    AudioDeviceDescriptor outputDeviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    uint32_t index = g_fuzzUtils.GetData<AudioPipeType>();
    rendererChangeInfo->rendererInfo.pipeType = g_fuzzUtils.GetData<AudioPipeType>();
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));

    audioStreamCollector_.ResetRendererStreamDeviceInfo(outputDeviceInfo);
}

void AudioStreamCollectorResetCapturerStreamDeviceInfoFuzzTest()
{
    AudioDeviceDescriptor outputDeviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    shared_ptr<AudioCapturerChangeInfo> rendererChangeInfo = make_shared<AudioCapturerChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    audioStreamCollector_.audioCapturerChangeInfos_.clear();
    audioStreamCollector_.audioCapturerChangeInfos_.push_back(move(rendererChangeInfo));
    audioStreamCollector_.ResetCapturerStreamDeviceInfo(outputDeviceInfo);
}

void AudioStreamCollectorCheckRendererStateInfoChangedFuzzTest()
{
    AudioStreamChangeInfo streamChangeInfo;
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    streamChangeInfo.audioRendererChangeInfo.clientUID = randIntValue / NUM_2;
    streamChangeInfo.audioRendererChangeInfo.sessionId = randIntValue;
    uint32_t index = g_fuzzUtils.GetData<RendererState>();
    streamChangeInfo.audioRendererChangeInfo.rendererState = g_fuzzUtils.GetData<RendererState>();
    audioStreamCollector_.CheckRendererStateInfoChanged(streamChangeInfo);
}

void AudioStreamCollectorCheckRendererInfoChangedFuzzTest()
{
    AudioStreamChangeInfo streamChangeInfo;
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    streamChangeInfo.audioRendererChangeInfo.clientUID = randIntValue / NUM_2;
    streamChangeInfo.audioRendererChangeInfo.sessionId = randIntValue;
    streamChangeInfo.audioRendererChangeInfo.rendererState = g_fuzzUtils.GetData<RendererState>();
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    bool result = g_fuzzUtils.GetData<bool>();
    if (result) {
        rendererChangeInfo->createrUID = randIntValue / NUM_2;
        rendererChangeInfo->rendererInfo.pipeType = g_fuzzUtils.GetData<AudioPipeType>();
        audioStreamCollector_.audioRendererChangeInfos_.clear();
        audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));
    }

    audioStreamCollector_.CheckRendererInfoChanged(streamChangeInfo);
}

void AudioStreamCollectorResetRingerModeMuteFuzzTest()
{
    RendererState rendererState = g_fuzzUtils.GetData<RendererState>();
    StreamUsage streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    audioStreamCollector_.ResetRingerModeMute(rendererState, streamUsage);
}

void AudioStreamCollectorUpdateRendererStreamInternalFuzzTest()
{
    AudioStreamChangeInfo streamChangeInfo;
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    streamChangeInfo.audioRendererChangeInfo.clientUID = randIntValue / NUM_2;
    streamChangeInfo.audioRendererChangeInfo.sessionId = randIntValue;
    streamChangeInfo.audioRendererChangeInfo.rendererState = g_fuzzUtils.GetData<RendererState>();
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    bool result = g_fuzzUtils.GetData<bool>();
    if (result) {
        rendererChangeInfo->createrUID = randIntValue / NUM_2;
        rendererChangeInfo->rendererInfo.pipeType = g_fuzzUtils.GetData<AudioPipeType>();
        audioStreamCollector_.audioRendererChangeInfos_.clear();
        audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));
    }

    audioStreamCollector_.UpdateRendererStreamInternal(streamChangeInfo);
}

void AudioStreamCollectorUpdateCapturerStreamInternalFuzzTest()
{
    AudioStreamChangeInfo streamChangeInfo;
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    streamChangeInfo.audioCapturerChangeInfo.clientUID = randIntValue % NUM_2;
    streamChangeInfo.audioCapturerChangeInfo.sessionId = randIntValue;
    streamChangeInfo.audioCapturerChangeInfo.prerunningState = g_fuzzUtils.GetData<bool>();
    shared_ptr<AudioCapturerChangeInfo> capturerChangeInfo = make_shared<AudioCapturerChangeInfo>();
    CHECK_AND_RETURN(capturerChangeInfo != nullptr);
    bool result = g_fuzzUtils.GetData<bool>();
    if (result) {
        capturerChangeInfo->clientUID = randIntValue % NUM_2;
        capturerChangeInfo->sessionId = randIntValue;
        capturerChangeInfo->prerunningState = g_fuzzUtils.GetData<bool>();
        audioStreamCollector_.audioCapturerChangeInfos_.clear();
        audioStreamCollector_.audioCapturerChangeInfos_.push_back(move(capturerChangeInfo));
    }

    audioStreamCollector_.UpdateCapturerStreamInternal(streamChangeInfo);
}

void AudioStreamCollectorUpdateTrackerFuzzTest()
{
    vector<AudioMode> audioModes = {
        AUDIO_MODE_PLAYBACK,
        AUDIO_MODE_RECORD,
    };
    uint32_t index = g_fuzzUtils.GetData<uint32_t>() % audioModes.size();
    AudioMode audioMode = audioModes[index];
    AudioDeviceDescriptor audioDev(AudioDeviceDescriptor::DEVICE_INFO);
    audioStreamCollector_.UpdateTracker(audioMode, audioDev);

    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    AudioStreamChangeInfo streamChangeInfo;
    streamChangeInfo.audioRendererChangeInfo.clientUID = randIntValue / NUM_2;
    streamChangeInfo.audioRendererChangeInfo.sessionId = randIntValue;
    index = g_fuzzUtils.GetData<RendererState>();
    streamChangeInfo.audioRendererChangeInfo.rendererState = g_fuzzUtils.GetData<RendererState>();
    sptr<IRemoteObject> clientTrackerObj = nullptr;

    audioStreamCollector_.RegisterTracker(audioMode, streamChangeInfo, clientTrackerObj);
    audioStreamCollector_.UpdateTracker(audioMode, streamChangeInfo);
}

void AudioStreamCollectorUpdateRendererDeviceInfoFuzzTest()
{
    shared_ptr<AudioDeviceDescriptor> outputDeviceInfoPtr = make_shared<AudioDeviceDescriptor>(
        AudioDeviceDescriptor::DEVICE_INFO);
    CHECK_AND_RETURN(outputDeviceInfoPtr != nullptr);
    uint32_t index = g_fuzzUtils.GetData<uint32_t>() % g_testDeviceTypes.size();
    outputDeviceInfoPtr->deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    auto info1 = std::make_unique<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(info1 != nullptr);
    info1->outputDeviceInfo.deviceType_ = g_testDeviceTypes[index / NUM_2];
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(std::move(info1));
    auto info2 = std::make_unique<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(info2 != nullptr);
    info2->outputDeviceInfo.deviceType_ = g_testDeviceTypes[(index + 1) / NUM_2];
    audioStreamCollector_.audioRendererChangeInfos_.push_back(std::move(info2));
    audioStreamCollector_.UpdateRendererDeviceInfo(outputDeviceInfoPtr);

    AudioDeviceDescriptor outputDeviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t clientUID = randIntValue / NUM_2;
    int32_t sessionId = randIntValue;
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    rendererChangeInfo->outputDeviceInfo = outputDeviceInfo;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));
    audioStreamCollector_.UpdateRendererDeviceInfo(clientUID, sessionId, outputDeviceInfo);
}

void AudioStreamCollectorUpdateCapturerDeviceInfoFuzzTest()
{
    shared_ptr<AudioDeviceDescriptor> inputDeviceInfoPtr = make_shared<AudioDeviceDescriptor>(
        AudioDeviceDescriptor::DEVICE_INFO);
    CHECK_AND_RETURN(inputDeviceInfoPtr != nullptr);
    uint32_t index = g_fuzzUtils.GetData<uint32_t>() % g_testDeviceTypes.size();
    inputDeviceInfoPtr->deviceType_ = g_fuzzUtils.GetData<DeviceType>();
    auto info1 = std::make_unique<AudioCapturerChangeInfo>();
    CHECK_AND_RETURN(info1 != nullptr);
    info1->inputDeviceInfo.deviceType_ = g_testDeviceTypes[index / NUM_2];
    audioStreamCollector_.audioCapturerChangeInfos_.clear();
    audioStreamCollector_.audioCapturerChangeInfos_.push_back(std::move(info1));
    auto info2 = std::make_unique<AudioCapturerChangeInfo>();
    CHECK_AND_RETURN(info2 != nullptr);
    info2->inputDeviceInfo.deviceType_ = g_testDeviceTypes[(index + 1) / NUM_2];
    audioStreamCollector_.audioCapturerChangeInfos_.push_back(std::move(info2));
    audioStreamCollector_.UpdateCapturerDeviceInfo(inputDeviceInfoPtr);

    AudioDeviceDescriptor inputDeviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    shared_ptr<AudioCapturerChangeInfo> captureChangeInfo = make_shared<AudioCapturerChangeInfo>();
    CHECK_AND_RETURN(captureChangeInfo != nullptr);
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    captureChangeInfo->clientUID = randIntValue / NUM_2;
    captureChangeInfo->createrUID = randIntValue / NUM_2;
    captureChangeInfo->sessionId = randIntValue;
    captureChangeInfo->inputDeviceInfo = inputDeviceInfo;
    audioStreamCollector_.audioCapturerChangeInfos_.clear();
    audioStreamCollector_.audioCapturerChangeInfos_.push_back(move(captureChangeInfo));
    int32_t clientUID = randIntValue / NUM_2;
    int32_t sessionId = randIntValue;
    AudioDeviceDescriptor outputDeviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    outputDeviceInfo.deviceRole_ = g_fuzzUtils.GetData<DeviceRole>();

    audioStreamCollector_.UpdateCapturerDeviceInfo(clientUID, sessionId, outputDeviceInfo);
}

void AudioStreamCollectorUpdateRendererPipeInfoFuzzTest()
{
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t sessionId = randIntValue;
    AudioPipeType normalPipe = g_fuzzUtils.GetData<AudioPipeType>();
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    rendererChangeInfo->rendererInfo.pipeType = g_fuzzUtils.GetData<AudioPipeType>();
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));

    audioStreamCollector_.UpdateRendererPipeInfo(sessionId, normalPipe);
}

void AudioStreamCollectorUpdateAppVolumeFuzzTest()
{
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    rendererChangeInfo->outputDeviceInfo = AudioDeviceDescriptor(AudioDeviceDescriptor::DEVICE_INFO);
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));

    int32_t appUid = randIntValue / NUM_2;
    int32_t volume = randIntValue % NUM_2;
    audioStreamCollector_.UpdateAppVolume(appUid, volume);
}

void AudioStreamCollectorGetStreamTypeFuzzTest()
{
    ContentType contentType = g_fuzzUtils.GetData<ContentType>();
    StreamUsage streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    audioStreamCollector_.GetStreamType(contentType, streamUsage);

    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    rendererChangeInfo->clientUID = randIntValue;
    rendererChangeInfo->createrUID = randIntValue;
    rendererChangeInfo->sessionId = randIntValue + 1;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));
    int32_t sessionId = randIntValue;
    audioStreamCollector_.GetStreamType(sessionId);
}

void AudioStreamCollectorGetSessionIdsOnRemoteDeviceByStreamUsageFuzzTest()
{
    vector<InterruptHint> testInterruptHints = {
        INTERRUPT_HINT_NONE,
        INTERRUPT_HINT_RESUME,
        INTERRUPT_HINT_PAUSE,
        INTERRUPT_HINT_STOP,
        INTERRUPT_HINT_DUCK,
        INTERRUPT_HINT_UNDUCK,
        INTERRUPT_HINT_MUTE,
        INTERRUPT_HINT_UNMUTE
    };
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    StreamUsage streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    DeviceType deviceType = g_testDeviceTypes[index % g_testDeviceTypes.size()];
    DeviceRole role = g_fuzzUtils.GetData<DeviceRole>();
    AudioDeviceDescriptor outputDeviceInfo(deviceType, role, 0, 0, "RemoteDevice");
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    rendererChangeInfo->outputDeviceInfo = outputDeviceInfo;
    rendererChangeInfo->rendererInfo.streamUsage = streamUsage;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));
    
    audioStreamCollector_.GetSessionIdsOnRemoteDeviceByStreamUsage(streamUsage);
    audioStreamCollector_.GetSessionIdsOnRemoteDeviceByDeviceType(deviceType);
}

void AudioStreamCollectorIsOffloadAllowedFuzzTest()
{
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t sessionId = randIntValue / NUM_2;
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));

    audioStreamCollector_.IsOffloadAllowed(sessionId);
}

void AudioStreamCollectorGetChannelCountFuzzTest()
{
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t sessionId = randIntValue / NUM_2;
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));

    audioStreamCollector_.GetChannelCount(sessionId);
}

void AudioStreamCollectorGetCurrentRendererChangeInfosFuzzTest()
{
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    std::vector<shared_ptr<AudioRendererChangeInfo>> rendererChangeInfos;
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));

    audioStreamCollector_.GetCurrentRendererChangeInfos(rendererChangeInfos);
}

void AudioStreamCollectorGetCurrentCapturerChangeInfosFuzzTest()
{
    shared_ptr<AudioCapturerChangeInfo> rendererChangeInfo = make_shared<AudioCapturerChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    std::vector<shared_ptr<AudioCapturerChangeInfo>> rendererChangeInfos;
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    audioStreamCollector_.audioCapturerChangeInfos_.clear();
    audioStreamCollector_.audioCapturerChangeInfos_.push_back(move(rendererChangeInfo));
    audioStreamCollector_.GetCurrentCapturerChangeInfos(rendererChangeInfos);
}

void AudioStreamCollectorRegisteredTrackerClientDiedFuzzTest()
{
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t uid = randIntValue / NUM_2;
    int32_t pid = randIntValue / NUM_2;
    audioStreamCollector_.GetLastestRunningCallStreamUsage();
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->clientPid = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));
    audioStreamCollector_.RegisteredTrackerClientDied(uid, pid);
}

void AudioStreamCollectorGetAndCompareStreamTypeFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    StreamUsage targetUsage = g_fuzzUtils.GetData<StreamUsage>();
    AudioRendererInfo rendererInfo;
    rendererInfo.contentType = g_fuzzUtils.GetData<ContentType>();
    rendererInfo.streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    audioStreamCollector_.GetAndCompareStreamType(targetUsage, rendererInfo);
}

void AudioStreamCollectorGetUidFuzzTest()
{
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t sessionId = randIntValue;

    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));
    audioStreamCollector_.GetUid(sessionId);
}

void AudioStreamCollectorResumeStreamStateFuzzTest()
{
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));
    audioStreamCollector_.ResumeStreamState();
}

void AudioStreamCollectorUpdateStreamStateFuzzTest()
{
    vector<StreamSetState> testStreamSetState = {
        STREAM_PAUSE,
        STREAM_RESUME,
        STREAM_MUTE,
        STREAM_UNMUTE,
    };
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t clientUid = randIntValue / NUM_2;
    StreamSetStateEventInternal event;
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    event.streamSetState = testStreamSetState[index % testStreamSetState.size()];
    event.streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    auto changeInfo = std::make_unique<AudioRendererChangeInfo>();
    changeInfo->clientUID = clientUid;
    changeInfo->rendererInfo.streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    changeInfo->sessionId = randIntValue % NUM_2;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(std::move(changeInfo));
    audioStreamCollector_.UpdateStreamState(clientUid, event);
}

void AudioStreamCollectorHandleAppStateChangeFuzzTest()
{
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t clientUid = randIntValue / NUM_2;
    int32_t clientPid = g_fuzzUtils.GetData<int32_t>();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    bool notifyMute = static_cast<bool>(index % NUM_2);
    auto changeInfo = std::make_unique<AudioRendererChangeInfo>();
    changeInfo->clientUID = clientUid;
    changeInfo->clientPid = clientPid;
    changeInfo->rendererInfo.streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    changeInfo->sessionId = randIntValue;
    changeInfo->backMute = static_cast<bool>(index % NUM_2);
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(std::move(changeInfo));
    bool hasBackTask = g_fuzzUtils.GetData<bool>();
    bool mute = static_cast<bool>(index % NUM_2);
    audioStreamCollector_.HandleAppStateChange(clientUid, clientPid, mute, notifyMute, hasBackTask);
}

void AudioStreamCollectorHandleFreezeStateChangeFuzzTest()
{
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t clientPid = randIntValue / NUM_2;
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    bool hasSession = static_cast<bool>(index % NUM_2);
    auto changeInfo = std::make_unique<AudioRendererChangeInfo>();
    changeInfo->clientPid = clientPid;
    changeInfo->rendererInfo.streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    changeInfo->sessionId = randIntValue / NUM_2;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(std::move(changeInfo));
    audioStreamCollector_.HandleFreezeStateChange(clientPid, static_cast<bool>(index % NUM_2), hasSession);
}

void AudioStreamCollectorHandleBackTaskStateChangeFuzzTest()
{
    static uint32_t stepSize = 0;
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t clientUid = randIntValue / NUM_2;
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    bool hasSession = static_cast<bool>(index % NUM_2);
    auto changeInfo = std::make_unique<AudioRendererChangeInfo>();
    changeInfo->clientUID = clientUid;
    changeInfo->rendererInfo.streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    changeInfo->sessionId = randIntValue / NUM_2;

    changeInfo->backMute = static_cast<bool>((index + stepSize++) % NUM_2);
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(std::move(changeInfo));
    audioStreamCollector_.HandleBackTaskStateChange(clientUid, hasSession);
}

void AudioStreamCollectorHandleStartStreamMuteStateFuzzTest()
{
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t clientUid = randIntValue;
    int32_t createrUID = randIntValue;
    int32_t clientPid = randIntValue;
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    bool mute = static_cast<bool>(index % NUM_2);
    auto changeInfo = std::make_unique<AudioRendererChangeInfo>();
    changeInfo->clientUID = clientUid;
    changeInfo->createrUID = createrUID;
    changeInfo->clientPid = clientPid;
    changeInfo->rendererInfo.streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    changeInfo->sessionId = randIntValue / NUM_2;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(std::move(changeInfo));
    audioStreamCollector_.HandleStartStreamMuteState(clientUid, clientPid, mute, mute);
}

void AudioStreamCollectorIsStreamActiveFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioStreamType volumeType = g_fuzzUtils.GetData<AudioStreamType>();
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    rendererChangeInfo->rendererState = g_fuzzUtils.GetData<RendererState>();
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));
    audioStreamCollector_.IsStreamActive(volumeType);
}

void AudioStreamCollectorGetRunningStreamFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioRendererInfo rendererInfo;
    rendererInfo.contentType = g_fuzzUtils.GetData<ContentType>();
    rendererInfo.streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    std::unique_ptr<AudioRendererChangeInfo> info = std::make_unique<AudioRendererChangeInfo>();
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    info->sessionId = randIntValue;
    info->rendererState = g_fuzzUtils.GetData<RendererState>();
    info->rendererInfo = rendererInfo;
    info->channelCount = randIntValue % NUM_2;

    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(std::move(info));
    audioStreamCollector_.GetRunningStream(g_fuzzUtils.GetData<AudioStreamType>(), 0);
}

void AudioStreamCollectorGetStreamTypeFromSourceTypeFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    audioStreamCollector_.GetStreamTypeFromSourceType(g_fuzzUtils.GetData<SourceType>());
}

void AudioStreamCollectorSetGetLowPowerVolumeFuzzTest()
{
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t streamId = randIntValue / NUM_2;

    audioStreamCollector_.SetLowPowerVolume(streamId, g_fuzzUtils.GetData<float>());
    audioStreamCollector_.GetLowPowerVolume(streamId);
}

void AudioStreamCollectorSetOffloadModeFuzzTest()
{
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t streamId = randIntValue / NUM_2;
    int32_t state = randIntValue / NUM_2 - 1;
    bool isAppBack = g_fuzzUtils.GetData<bool>();

    audioStreamCollector_.SetOffloadMode(streamId, state, isAppBack);
}

void AudioStreamCollectorUnsetOffloadModeFuzzTest()
{
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t streamId = randIntValue % NUM_2;
    AudioStreamChangeInfo streamChangeInfo;
    streamChangeInfo.audioRendererChangeInfo.clientUID = randIntValue / NUM_2;
    streamChangeInfo.audioRendererChangeInfo.sessionId = randIntValue;
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    streamChangeInfo.audioRendererChangeInfo.rendererState = g_fuzzUtils.GetData<RendererState>();
    sptr<IRemoteObject> object;
    sptr<IStandardClientTracker> listener = iface_cast<IStandardClientTracker>(object);
    std::shared_ptr<AudioClientTracker> callback = std::make_shared<ClientTrackerCallbackListener>(listener);
    CHECK_AND_RETURN(callback != nullptr);
    int32_t clientId = streamChangeInfo.audioRendererChangeInfo.sessionId;
    audioStreamCollector_.clientTracker_[clientId] = callback;
    audioStreamCollector_.UnsetOffloadMode(streamId);
}

void AudioStreamCollectorGetSingleStreamVolumeFuzzTest()
{
    int32_t streamId = g_fuzzUtils.GetData<int32_t>();
    audioStreamCollector_.GetSingleStreamVolume(streamId);
}

void AudioStreamCollectorUpdateCapturerInfoMuteStatusFuzzTest()
{
    auto changeInfo = std::make_unique<AudioCapturerChangeInfo>();
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    changeInfo->clientUID = randIntValue;
    changeInfo->muted = static_cast<bool>(index % NUM_2);
    changeInfo->sessionId = randIntValue / NUM_2;
    changeInfo->capturerInfo.sourceType = g_fuzzUtils.GetData<SourceType>();
    changeInfo->inputDeviceInfo.deviceType_ = g_testDeviceTypes[index % g_testDeviceTypes.size()];
    audioStreamCollector_.audioCapturerChangeInfos_.clear();
    audioStreamCollector_.audioCapturerChangeInfos_.push_back(std::move(changeInfo));
    audioStreamCollector_.audioPolicyServerHandler_ = std::make_shared<AudioPolicyServerHandler>();
    CHECK_AND_RETURN(audioStreamCollector_.audioPolicyServerHandler_ != nullptr);
    audioStreamCollector_.UpdateCapturerInfoMuteStatus(randIntValue, true);
}

void AudioStreamCollectorIsCallStreamUsageFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<StreamUsage>();
    StreamUsage usage = g_fuzzUtils.GetData<StreamUsage>();

    audioStreamCollector_.IsCallStreamUsage(usage);
}

void AudioStreamCollectorGetRunningStreamUsageNoUltrasonicFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();

    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    rendererChangeInfo->rendererState = g_fuzzUtils.GetData<RendererState>();
    rendererChangeInfo->rendererInfo.streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));

    audioStreamCollector_.GetRunningStreamUsageNoUltrasonic();
}

void AudioStreamCollectorGetRunningSourceTypeNoUltrasonicFuzzTest()
{
    const vector<CapturerState> testCapturerStates = {
        CAPTURER_INVALID,
        CAPTURER_NEW,
        CAPTURER_PREPARED,
        CAPTURER_RUNNING,
        CAPTURER_STOPPED,
        CAPTURER_RELEASED,
        CAPTURER_PAUSED,
    };
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();

    auto changeInfo = std::make_unique<AudioCapturerChangeInfo>();
    changeInfo->clientUID = randIntValue;
    changeInfo->sessionId = randIntValue / NUM_2;
    changeInfo->capturerState = testCapturerStates[index % testCapturerStates.size()];
    changeInfo->capturerInfo.sourceType = g_fuzzUtils.GetData<SourceType>();
    audioStreamCollector_.audioCapturerChangeInfos_.clear();
    audioStreamCollector_.audioCapturerChangeInfos_.push_back(move(changeInfo));

    audioStreamCollector_.GetRunningSourceTypeNoUltrasonic();
}

void AudioStreamCollectorGetLastestRunningCallStreamUsageFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();

    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    rendererChangeInfo->rendererState = g_fuzzUtils.GetData<RendererState>();
    rendererChangeInfo->rendererInfo.streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));

    audioStreamCollector_.GetLastestRunningCallStreamUsage();
}

void AudioStreamCollectorGetAllRendererSessionIDForUIDFuzzTest()
{
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t uid = randIntValue / NUM_2;
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    rendererChangeInfo->clientUID = randIntValue /NUM_2;
    rendererChangeInfo->createrUID = randIntValue /NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));

    audioStreamCollector_.GetAllRendererSessionIDForUID(uid);
}

void AudioStreamCollectorGetAllCapturerSessionIDForUIDFuzzTest()
{
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t uid = randIntValue / NUM_2;
    shared_ptr<AudioCapturerChangeInfo> capturerChangeInfo = make_shared<AudioCapturerChangeInfo>();
    CHECK_AND_RETURN(capturerChangeInfo != nullptr);
    capturerChangeInfo->clientUID = randIntValue /NUM_2;
    capturerChangeInfo->createrUID = randIntValue /NUM_2;
    capturerChangeInfo->sessionId = randIntValue;
    audioStreamCollector_.audioCapturerChangeInfos_.clear();
    audioStreamCollector_.audioCapturerChangeInfos_.push_back(move(capturerChangeInfo));

    audioStreamCollector_.GetAllCapturerSessionIDForUID(uid);
}

void AudioStreamCollectorChangeVoipCapturerStreamToNormalFuzzTest()
{
    shared_ptr<AudioCapturerChangeInfo> rendererChangeInfo = make_shared<AudioCapturerChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    rendererChangeInfo->capturerInfo.sourceType = g_fuzzUtils.GetData<SourceType>();
    audioStreamCollector_.audioCapturerChangeInfos_.clear();
    audioStreamCollector_.audioCapturerChangeInfos_.push_back(move(rendererChangeInfo));

    audioStreamCollector_.ChangeVoipCapturerStreamToNormal();
}

void AudioStreamCollectorHasVoipRendererStreamFuzzTest()
{
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t uid = randIntValue / NUM_2;
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    CHECK_AND_RETURN(rendererChangeInfo != nullptr);
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    rendererChangeInfo->rendererInfo.originalFlag = randIntValue % NUM_2;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));

    audioStreamCollector_.HasVoipRendererStream();
}

void AudioStreamCollectorIsMediaPlayingFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioRendererInfo rendererInfo;
    rendererInfo.contentType = g_fuzzUtils.GetData<ContentType>();
    rendererInfo.streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    std::unique_ptr<AudioRendererChangeInfo> info = std::make_unique<AudioRendererChangeInfo>();
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    info->sessionId = randIntValue % NUM_2;
    info->rendererState = g_fuzzUtils.GetData<RendererState>();
    info->rendererInfo = rendererInfo;
    info->channelCount = randIntValue % NUM_2;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(std::move(info));
    audioStreamCollector_.IsMediaPlaying();
}

void AudioStreamCollectorIsVoipStreamActiveFuzzTest()
{
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    AudioRendererInfo rendererInfo;
    rendererInfo.streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    std::unique_ptr<AudioRendererChangeInfo> info = std::make_unique<AudioRendererChangeInfo>();
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    info->sessionId = randIntValue % NUM_2;
    info->rendererState = g_fuzzUtils.GetData<RendererState>();
    info->rendererInfo = rendererInfo;
    info->channelCount = randIntValue % NUM_2;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(std::move(info));
    audioStreamCollector_.IsVoipStreamActive();
}

void AudioStreamCollectorCheckVoiceCallActiveFuzzTest()
{
    int32_t randIntValue = g_fuzzUtils.GetData<int32_t>();
    int32_t clientPid = randIntValue / NUM_2;
    uint32_t index = g_fuzzUtils.GetData<uint32_t>();
    auto changeInfo = std::make_unique<AudioRendererChangeInfo>();
    changeInfo->clientPid = clientPid;
    changeInfo->rendererInfo.streamUsage = g_fuzzUtils.GetData<StreamUsage>();
    changeInfo->sessionId = randIntValue / NUM_2;
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(std::move(changeInfo));
    audioStreamCollector_.CheckVoiceCallActive(clientPid);
}

vector<TestFuncs> g_testFuncs = {
    AudioStreamCollectorAddRendererStreamFuzzTest,
    AudioStreamCollectorGetRendererStreamInfoFuzzTest,
    AudioStreamCollectorGetCapturerStreamInfoFuzzTest,
    AudioStreamCollectorGetPipeTypeFuzzTest,
    AudioStreamCollectorExistStreamForPipeFuzzTest,
    AudioStreamCollectorGetRendererDeviceInfoFuzzTest,
    AudioStreamCollectorAddCapturerStreamFuzzTest,
    AudioStreamCollectorSendCapturerInfoEventFuzzTest,
    AudioStreamCollectorRegisterTrackerFuzzTest,
    AudioStreamCollectorSetRendererStreamParamFuzzTest,
    AudioStreamCollectorSetCapturerStreamParamFuzzTest,
    AudioStreamCollectorResetRendererStreamDeviceInfoFuzzTest,
    AudioStreamCollectorResetCapturerStreamDeviceInfoFuzzTest,
    AudioStreamCollectorCheckRendererStateInfoChangedFuzzTest,
    AudioStreamCollectorCheckRendererInfoChangedFuzzTest,
    AudioStreamCollectorResetRingerModeMuteFuzzTest,
    AudioStreamCollectorUpdateRendererStreamInternalFuzzTest,
    AudioStreamCollectorUpdateCapturerStreamInternalFuzzTest,
    AudioStreamCollectorUpdateTrackerFuzzTest,
    AudioStreamCollectorUpdateRendererDeviceInfoFuzzTest,
    AudioStreamCollectorUpdateCapturerDeviceInfoFuzzTest,
    AudioStreamCollectorUpdateRendererPipeInfoFuzzTest,
    AudioStreamCollectorUpdateAppVolumeFuzzTest,
    AudioStreamCollectorGetStreamTypeFuzzTest,
    AudioStreamCollectorGetSessionIdsOnRemoteDeviceByStreamUsageFuzzTest,
    AudioStreamCollectorIsOffloadAllowedFuzzTest,
    AudioStreamCollectorGetChannelCountFuzzTest,
    AudioStreamCollectorGetCurrentRendererChangeInfosFuzzTest,
    AudioStreamCollectorGetCurrentCapturerChangeInfosFuzzTest,
    AudioStreamCollectorRegisteredTrackerClientDiedFuzzTest,
    AudioStreamCollectorGetAndCompareStreamTypeFuzzTest,
    AudioStreamCollectorGetUidFuzzTest,
    AudioStreamCollectorResumeStreamStateFuzzTest,
    AudioStreamCollectorUpdateStreamStateFuzzTest,
    AudioStreamCollectorHandleAppStateChangeFuzzTest,
    AudioStreamCollectorHandleFreezeStateChangeFuzzTest,
    AudioStreamCollectorHandleBackTaskStateChangeFuzzTest,
    AudioStreamCollectorHandleStartStreamMuteStateFuzzTest,
    AudioStreamCollectorIsStreamActiveFuzzTest,
    AudioStreamCollectorGetRunningStreamFuzzTest,
    AudioStreamCollectorGetStreamTypeFromSourceTypeFuzzTest,
    AudioStreamCollectorSetGetLowPowerVolumeFuzzTest,
    AudioStreamCollectorSetOffloadModeFuzzTest,
    AudioStreamCollectorUnsetOffloadModeFuzzTest,
    AudioStreamCollectorGetSingleStreamVolumeFuzzTest,
    AudioStreamCollectorUpdateCapturerInfoMuteStatusFuzzTest,
    AudioStreamCollectorIsCallStreamUsageFuzzTest,
    AudioStreamCollectorGetRunningStreamUsageNoUltrasonicFuzzTest,
    AudioStreamCollectorGetRunningSourceTypeNoUltrasonicFuzzTest,
    AudioStreamCollectorGetLastestRunningCallStreamUsageFuzzTest,
    AudioStreamCollectorGetAllRendererSessionIDForUIDFuzzTest,
    AudioStreamCollectorGetAllCapturerSessionIDForUIDFuzzTest,
    AudioStreamCollectorChangeVoipCapturerStreamToNormalFuzzTest,
    AudioStreamCollectorHasVoipRendererStreamFuzzTest,
    AudioStreamCollectorIsMediaPlayingFuzzTest,
    AudioStreamCollectorIsVoipStreamActiveFuzzTest,
    AudioStreamCollectorCheckVoiceCallActiveFuzzTest,
};

} // namespace AudioStandard
} // namesapce OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    OHOS::AudioStandard::g_fuzzUtils.fuzzTest(data, size, OHOS::AudioStandard::g_testFuncs);
    return 0;
}