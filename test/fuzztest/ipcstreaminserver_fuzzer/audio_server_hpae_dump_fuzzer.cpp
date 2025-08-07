/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#include "audio_server.h"
#include "audio_manager_base.h"
#include "ipc_stream_in_server.h"
using namespace std;

namespace OHOS {
namespace AudioStandard {
const std::u16string FORMMGR_INTERFACE_TOKEN = u"IStandardAudioService";
const int32_t LIMITSIZE = 4;
const int32_t SHIFT_LEFT_8 = 8;
const int32_t SHIFT_LEFT_16 = 16;
const int32_t SHIFT_LEFT_24 = 24;
const uint32_t APPID_LENGTH = 10;
typedef void (*TestPtr)(const uint8_t *, size_t);

template<class T>
uint32_t GetArrLength(T& arr)
{
    if (arr == nullptr) {
        AUDIO_INFO_LOG("%{public}s: The array length is equal to 0", __func__);
        return 0;
    }
    return sizeof(arr) / sizeof(arr[0]);
}

uint32_t Convert2Uint32(const uint8_t *ptr)
{
    if (ptr == nullptr) {
        return 0;
    }
    /* Move the 0th digit to the left by 24 bits, the 1st digit to the left by 16 bits,
       the 2nd digit to the left by 8 bits, and the 3rd digit not to the left */
    return (ptr[0] << SHIFT_LEFT_24) | (ptr[1] << SHIFT_LEFT_16) | (ptr[2] << SHIFT_LEFT_8) | (ptr[3]);
}

void AudioServerSetAsrAecModeFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    int32_t asrAecMode =  *reinterpret_cast<const int32_t*>(rawData);
    std::shared_ptr<AudioServer> audioServerPtr = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    AudioServerPtr->SetAsrAecMode(asrAecMode);
}

void AudioServerGetAsrAecModeFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    int32_t asrAecMode =  *reinterpret_cast<const int32_t*>(rawData);
    std::shared_ptr<AudioServer> audioServerPtr = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    AudioServerPtr->GetAsrAecMode(asrAecMode);
}

void AudioServerGetAsrNoiseSuppressionModeFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    int32_t asrNoiseSuppressionMode =  *reinterpret_cast<const int32_t*>(rawData);
    std::shared_ptr<AudioServer> audioServerPtr = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    AudioServerPtr->GetAsrNoiseSuppressionMode(asrNoiseSuppressionMode);
}

void AudioServerSetAsrWhisperDetectionModeFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    int32_t asrNoiseSuppressionMode =  *reinterpret_cast<const int32_t*>(rawData);
    std::shared_ptr<AudioServer> audioServerPtr = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    AudioServerPtr->SetAsrWhisperDetectionMode(asrNoiseSuppressionMode);
}

void AudioServerGetAsrWhisperDetectionModeFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    int32_t asrNoiseSuppressionMode =  *reinterpret_cast<const int32_t*>(rawData);
    std::shared_ptr<AudioServer> audioServerPtr = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    AudioServerPtr->GetAsrWhisperDetectionMode(asrNoiseSuppressionMode);
}

void AudioServerSetAsrVoiceSuppressionControlModeFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    vector<AudioParamKey> audioParamKey {
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
    uint32_t keyId = *reinterpret_cast<const uint32_t*>(rawData) % audioParamKey.size();
    AudioParamKey paramKey = static_cast<AudioParamKey>(audioParamKey[KeyId]);
    AsrVoiceControlMode asrVoiceControlMode = AsrVoiceControlMode::AUDIO_SUPPRESSION_OPPOSITE;
    bool on = *reinterpret_cast<const bool*>(rawData);
    int32_t modifyVolume = *reinterpret_cast<const int32_t*>(rawData);
    std::shared_ptr<AudioServer> audioServerPtr = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    AudioServerPtr->SetAsrVoiceSuppressionControlMode(paramKey, asrVoiceControlMode, on, modifyVolume);
}

void AudioServerSetAsrVoiceControlModeFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    int32_t asrVoiceControlMode =  *reinterpret_cast<const int32_t*>(rawData);
    bool on = *reinterpret_cast<const bool*>(rawData);
    std::shared_ptr<AudioServer> audioServerPtr = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    AudioServerPtr->SetAsrVoiceControlMode(asrVoiceControlMode, on);
}

void AudioServerSetAsrVoiceMuteModeFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    int32_t asrVoiceControlMode =  *reinterpret_cast<const int32_t*>(rawData);
    bool on = *reinterpret_cast<const bool*>(rawData);
    std::shared_ptr<AudioServer> audioServerPtr = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    AudioServerPtr->SetAsrVoiceMuteMode(asrVoiceControlMode, on);
}

void AudioServerIsWhisperingFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    int32_t whisperRes =  *reinterpret_cast<const int32_t*>(rawData);
    std::shared_ptr<AudioServer> audioServerPtr = std::make_shared<AudioServer>(SYSTEM_ABILITY_ID, RUN_ON_CREATE);
    AudioServerPtr->IsWhispering(whisperRes);
}

} // namespace AudioStandard
} // namesapce OHOS

OHOS::AudioStandard::TestPtr g_testPtrs[] = {
    OHOS::AudioStandard::AudioServerSetAsrAecModeFuzzTest,
    OHOS::AudioStandard::AudioServerGetAsrAecModeFuzzTest,
    OHOS::AudioStandard::AudioServerGetAsrNoiseSuppressionModeFuzzTest,
    OHOS::AudioStandard::AudioServerSetAsrWhisperDetectionModeFuzzTest,
    OHOS::AudioStandard::AudioServerGetAsrWhisperDetectionModeFuzzTest,
    OHOS::AudioStandard::AudioServerSetAsrVoiceSuppressionControlModeFuzzTest,
    OHOS::AudioStandard::AudioServerSetAsrVoiceControlModeFuzzTest,
    OHOS::AudioStandard::AudioServerSetAsrVoiceMuteModeFuzzTest,
    OHOS::AudioStandard::AudioServerIsWhisperingFuzzTest
};

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    /* Run your code on data */
    if (data == nullptr || size <= 1) {
        return 0;
    }
    uint32_t len = OHOS::AudioStandard::GetArrLength(g_testPtrs);
    if (len > 0) {
        uint8_t firstByte = *data % len;
        if (firstByte >= len) {
            return 0;
        }
        data = data + 1;
        size = size - 1;
        g_testPtrs[firstByte](data, size);
    }
    return 0;
}
