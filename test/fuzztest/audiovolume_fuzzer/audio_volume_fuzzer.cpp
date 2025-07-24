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

#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include "audio_log.h"
#include "audio_info.h"
#include "audio_volume.h"

namespace OHOS {
namespace AudioStandard {
using namespace std;

static const uint8_t *RAW_DATA = nullptr;
static size_t g_dataSize = 0;
static size_t g_pos;
const size_t FUZZ_INPUT_SIZE_THRESHOLD = 10;
const uint8_t TESTSIZE = 12;
const int32_t BINARY_STATE_LIMIT = 2;

typedef void (*TestFuncs)();

template<class T>
T GetData()
{
    T object {};
    size_t objectSize = sizeof(object);
    if (g_dataSize < g_pos) {
        return object;
    }
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

template<class T>
uint32_t GetArrLength(T &arr)
{
    if (arr == nullptr) {
        AUDIO_INFO_LOG("%{public}s: The array length is equal to 0", __func__);
        return 0;
    }
    return sizeof(arr) / sizeof(arr[0]);
}

const vector<AudioStreamType> AudioStreamTypeVec = {
    STREAM_DEFAULT,
    STREAM_VOICE_CALL,
    STREAM_MUSIC,
    STREAM_RING,
    STREAM_MEDIA,
    STREAM_VOICE_ASSISTANT,
    STREAM_SYSTEM,
    STREAM_ALARM,
    STREAM_NOTIFICATION,
    STREAM_BLUETOOTH_SCO,
    STREAM_ENFORCED_AUDIBLE,
    STREAM_DTMF,
    STREAM_TTS,
    STREAM_ACCESSIBILITY,
    STREAM_RECORDING,
    STREAM_MOVIE,
    STREAM_GAME,
    STREAM_SPEECH,
    STREAM_SYSTEM_ENFORCED,
    STREAM_ULTRASONIC,
    STREAM_WAKEUP,
    STREAM_VOICE_MESSAGE,
    STREAM_NAVIGATION,
    STREAM_INTERNAL_FORCE_STOP,
    STREAM_SOURCE_VOICE_CALL,
    STREAM_VOICE_COMMUNICATION,
    STREAM_VOICE_RING,
    STREAM_VOICE_CALL_ASSISTANT,
    STREAM_CAMCORDER,
    STREAM_APP,
    STREAM_TYPE_MAX,
    STREAM_ALL,
};

void GetVolumeFuzzTest()
{
    uint32_t sessionId = 1;
    uint32_t index = GetData<uint32_t>() % AudioStreamTypeVec.size();
    int32_t volumeType = AudioStreamTypeVec[index];
    std::string deviceClass = "speaker";
    AudioVolume::GetInstance()->SetVgsVolumeSupported(GetData<uint8_t>() % BINARY_STATE_LIMIT);
    struct VolumeValues volumes = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float volume = AudioVolume::GetInstance()->GetVolume(sessionId, volumeType, deviceClass, &volumes);
}

void GetDoNotDisturbStatusVolumeFuzzTest()
{
    uint32_t index = GetData<uint32_t>() % AudioStreamTypeVec.size();
    int32_t volumeType = AudioStreamTypeVec[index];
    int32_t appUid = GetData<int32_t>();
    uint32_t sessionId = GetData<uint32_t>();
    AudioVolume* audioVolumeTest = AudioVolume::GetInstance();
    audioVolumeTest->isDoNotDisturbStatus_ = GetData<uint8_t>() % BINARY_STATE_LIMIT;
    bool isSystemApp = GetData<uint8_t>() % BINARY_STATE_LIMIT;
    bool isVKB = GetData<uint8_t>() % BINARY_STATE_LIMIT;
    StreamVolume streamVolume(0, 0, 0, 0, 0, isSystemApp, 0, isVKB);
    audioVolumeTest->streamVolume_.insert({sessionId, streamVolume});
    audioVolumeTest->GetDoNotDisturbStatusVolume(volumeType, appUid, sessionId);
}

void SetDoNotDisturbStatusWhiteListVolumeFuzzTest()
{
    std::vector<std::map<std::string, std::string>> doNotDisturbStatusWhiteList;
    std::map<std::string, std::string> obj;
    obj["123"] = "1";
    doNotDisturbStatusWhiteList.push_back(obj);
    int32_t doNotDisturbStatusVolume = GetData<int32_t>();
    int32_t volumeType = GetData<int32_t>();
    int32_t appUid = GetData<int32_t>();
    int32_t sessionId = GetData<int32_t>();
    AudioVolume::GetInstance()->SetDoNotDisturbStatusWhiteListVolume(doNotDisturbStatusWhiteList);
    AudioVolume::GetInstance()->GetDoNotDisturbStatusVolume(volumeType, appUid, sessionId);
}

void SetDoNotDisturbStatusFuzzTest()
{
    bool isDoNotDisturbStatus = GetData<uint8_t>() % BINARY_STATE_LIMIT;
    AudioVolume::GetInstance()->SetDoNotDisturbStatus(isDoNotDisturbStatus);
}

void GetStreamVolumeFuzzTest()
{
    uint32_t sessionId = GetData<uint32_t>();
    float lowPowerFactor = GetData<float>();
    AudioVolume::GetInstance()->SetStreamVolumeLowPowerFactor(sessionId, lowPowerFactor);
    AudioVolume::GetInstance()->GetStreamVolume(sessionId);
}

void GetHistoryVolumeFuzzTest()
{
    uint32_t sessionId = GetData<uint32_t>();
    AudioVolume::GetInstance()->GetHistoryVolume(sessionId);
}

void SetHistoryVolumeFuzzTest()
{
    uint32_t sessionId = GetData<uint32_t>();
    float volume = GetData<float>();
    AudioVolume::GetInstance()->SetHistoryVolume(sessionId, volume);
    AudioVolume::GetInstance()->GetHistoryVolume(sessionId);
}

void SaveAdjustStreamVolumeInfoFuzzTest()
{
    auto audioVolume = std::make_shared<AudioVolume>();
    if (audioVolume == nullptr) {
        return;
    }
    float volume = GetData<float>();
    uint32_t sessionId = GetData<uint32_t>();
    std::string invocationTime  = GetTime();
    int32_t adjustStreamVolumeCount = static_cast<int32_t>(AdjustStreamVolume::DUCK_VOLUME_INFO) + 1;
    AdjustStreamVolume adjustStreamVolume =
        static_cast<AdjustStreamVolume>(GetData<uint8_t>() % adjustStreamVolumeCount);
    uint32_t code = static_cast<uint32_t>(adjustStreamVolume);
    audioVolume->SaveAdjustStreamVolumeInfo(volume, sessionId, invocationTime, code);
}

void GetStreamVolumeInfoFuzzTest()
{
    auto audioVolume = std::make_shared<AudioVolume>();
    if (audioVolume == nullptr) {
        return;
    }
    float volume = GetData<float>();
    uint32_t sessionId = GetData<uint32_t>();
    std::string invocationTime  = GetTime();
    uint32_t code = GetData<uint32_t>();
    audioVolume->SaveAdjustStreamVolumeInfo(volume, sessionId, invocationTime, code);
    int32_t adjustStreamVolumeCount = static_cast<int32_t>(AdjustStreamVolume::DUCK_VOLUME_INFO) + 1;
    AdjustStreamVolume adjustStreamVolume =
        static_cast<AdjustStreamVolume>(GetData<uint8_t>() % adjustStreamVolumeCount);
    AdjustStreamVolume volumeType = static_cast<AdjustStreamVolume>(adjustStreamVolume);
    audioVolume->GetStreamVolumeInfo(volumeType);
}

void GetAppVolumeFuzzTest()
{
    AudioVolume* audioVolumeTest = AudioVolume::GetInstance();
    int32_t appUid = GetData<int32_t>();
    int32_t adjustStreamVolumeCount = static_cast<int32_t>(AudioVolumeMode::AUDIOSTREAM_VOLUMEMODE_APP_INDIVIDUAL) + 1;
    AudioVolumeMode mode = static_cast<AudioVolumeMode>(GetData<uint8_t>() % adjustStreamVolumeCount);
    audioVolumeTest->appVolume_.clear();
    audioVolumeTest->GetAppVolume(appUid, mode);
}

void SetAppVolumeMuteFuzzTest()
{
    AudioVolume* audioVolumeTest = AudioVolume::GetInstance();
    int32_t appUid = GetData<int32_t>();
    bool isMuted = GetData<int32_t>() % BINARY_STATE_LIMIT;
    audioVolumeTest->appVolume_.clear();
    audioVolumeTest->streamVolume_.clear();
    audioVolumeTest->SetAppVolumeMute(appUid, isMuted);
}

void SetAppVolumeFuzzTest()
{
    AudioVolume* audioVolumeTest = AudioVolume::GetInstance();
    int32_t appUid = GetData<int32_t>();
    float volume = GetData<float>();
    int32_t volumeLevel = GetData<int32_t>();
    bool isMuted = GetData<int32_t>() % BINARY_STATE_LIMIT;
    AppVolume appVolume(appUid, volume, volumeLevel, isMuted);
    appVolume.totalVolume_ = GetData<int32_t>();
    audioVolumeTest->appVolume_.clear();
    audioVolumeTest->streamVolume_.clear();
    audioVolumeTest->SetAppVolume(appVolume);
}

TestFuncs g_testFuncs[TESTSIZE] = {
    GetVolumeFuzzTest,
    GetDoNotDisturbStatusVolumeFuzzTest,
    SetDoNotDisturbStatusWhiteListVolumeFuzzTest,
    SetDoNotDisturbStatusFuzzTest,
    GetStreamVolumeFuzzTest,
    GetHistoryVolumeFuzzTest,
    SetHistoryVolumeFuzzTest,
    SaveAdjustStreamVolumeInfoFuzzTest,
    GetStreamVolumeInfoFuzzTest,
    GetAppVolumeFuzzTest,
    SetAppVolumeMuteFuzzTest,
    SetAppVolumeFuzzTest,
};

void FuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr) {
        return;
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

    return;
}
} // namespace AudioStandard
} // namesapce OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    if (size < OHOS::AudioStandard::FUZZ_INPUT_SIZE_THRESHOLD) {
        return 0;
    }

    OHOS::AudioStandard::FuzzTest(data, size);
    return 0;
}
