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

#include "audio_manager_base.h"
#include "audio_server_hpae_dump.h"
#include "audio_info.h"
#include "hpae_info.h"

using namespace std;

namespace OHOS {
namespace AudioStandard {
const std::u16string FORMMGR_INTERFACE_TOKEN = u"IStandardAudioService";
const int32_t LIMITSIZE = 4;
const int32_t SHIFT_LEFT_8 = 8;
const int32_t SHIFT_LEFT_16 = 16;
const int32_t SHIFT_LEFT_24 = 24;
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

float Convert2Float(const uint8_t *ptr)
{
    float floatValue = static_cast<float>(*ptr);
    return floatValue / 128.0f - 1.0f;
}

void AudioServerHpaeDumpServerDataDumpFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::string dumpString = "test_dumpString";
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->ServerDataDump(dumpString);
}

void AudioServerHpaeDumpGetDeviceSinkInfoFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::string dumpString = "test_dumpString";
    std::string deviceName = "test_deviceName";
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->GetDeviceSinkInfo(dumpString, deviceName);
}

void AudioServerHpaeDumpPlaybackSinkDumpFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::string dumpString = "test_dumpString";
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->PlaybackSinkDump(dumpString);
}

void AudioServerHpaeDumpOnDumpSinkInfoCbFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::string dumpStr = "test_dumpStr";
    int32_t result = *reinterpret_cast<const int32_t*>(rawData);
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->OnDumpSinkInfoCb(dumpStr, result);
}

void AudioServerHpaeDumpGetDeviceSourceInfoFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::string dumpString = "test_dumpString";
    std::string deviceName = "test_deviceName";
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->GetDeviceSourceInfo(dumpString, deviceName);
}

void AudioServerHpaeDumpRecordSourceDumpFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::string dumpString = "test_dumpString";
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->RecordSourceDump(dumpString);
}

void AudioServerHpaeDumpOnDumpSourceInfoCbFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::string dumpStr = "test_dumpStr";
    int32_t result = *reinterpret_cast<const int32_t*>(rawData);
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->OnDumpSourceInfoCb(dumpStr, result);
}

void AudioServerHpaeDumpHelpInfoDumpFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::string dumpString = "test_dumpString";
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->HelpInfoDump(dumpString);
}

void AudioServerHpaeDumpHDFModulesDumpFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::string dumpString = "test_dumpString";
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->HDFModulesDump(dumpString);
}

void AudioServerHpaeDumpOnDumpAllAvailableDeviceCbFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    int32_t result = *reinterpret_cast<const int32_t*>(rawData);
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->OnDumpAllAvailableDeviceCb(result);
}

void AudioServerHpaeDumpPolicyHandlerDumpFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::string dumpString = "test_dumpString";
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->PolicyHandlerDump(dumpString);
}

void AudioServerHpaeDumpAudioCacheMemoryDumpFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::string dumpString = "test_dumpString";
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->AudioCacheMemoryDump(dumpString);
}

void AudioServerHpaeDumpAudioPerformMonitorDumpFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::string dumpString = "test_dumpString";
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->AudioPerformMonitorDump(dumpString);
}

void AudioServerHpaeDumpHdiAdapterDumpFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::string dumpString = "test_dumpString";
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->HdiAdapterDump(dumpString);
}

void AudioServerHpaeDumpPlaybackSinkInputDumpFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::string dumpString = "test_dumpString";
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->PlaybackSinkInputDump(dumpString);
}

void AudioServerHpaeDumpRecordSourceOutputDumpFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::string dumpString = "test_dumpString";
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->RecordSourceOutputDump(dumpString);
}

void AudioServerHpaeDumpOnDumpSinkInputsInfoCbFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::vector<HpaeInputOutputInfo> sinkInputs;
    sinkInputs.push_back({0, "", 0, 0, 0, false, PRIVACY_TYPE_PUBLIC, "", HPAE::HPAE_SESSION_NEW, 0});
    int32_t result = *reinterpret_cast<const int32_t*>(rawData);
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->OnDumpSinkInputsInfoCb(sinkInputs, result);
}

void AudioServerHpaeDumpSourceOutputsInfoCbFuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr || size < LIMITSIZE) {
        return;
    }
    std::vector<HpaeInputOutputInfo> sourceOutputs;
    sourceOutputs.push_back({0, "", 0, 0, 0, false, PRIVACY_TYPE_PUBLIC, "", HPAE::HPAE_SESSION_NEW, 0});
    int32_t result = *reinterpret_cast<const int32_t*>(rawData);
    std::shared_ptr<AudioServerHpaeDump> audioServerHpaeDumpPtr = std::make_shared<AudioServerHpaeDump>();
    audioServerHpaeDumpPtr->OnDumpSourceOutputsInfoCb(sourceOutputs, result);
}

} // namespace AudioStandard
} // namesapce OHOS

OHOS::AudioStandard::TestPtr g_testPtrs[] = {
    OHOS::AudioStandard::AudioServerHpaeDumpServerDataDumpFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpGetDeviceSinkInfoFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpPlaybackSinkDumpFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpOnDumpSinkInfoCbFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpGetDeviceSourceInfoFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpRecordSourceDumpFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpOnDumpSourceInfoCbFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpHelpInfoDumpFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpHDFModulesDumpFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpOnDumpAllAvailableDeviceCbFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpPolicyHandlerDumpFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpAudioCacheMemoryDumpFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpAudioPerformMonitorDumpFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpHdiAdapterDumpFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpPlaybackSinkInputDumpFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpRecordSourceOutputDumpFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpOnDumpSinkInputsInfoCbFuzzTest,
    OHOS::AudioStandard::AudioServerHpaeDumpSourceOutputsInfoCbFuzzTest
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
