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
#include "audio_effect_service.h"
using namespace std;

namespace OHOS {
namespace AudioStandard {
using namespace std;
static const uint8_t *RAW_DATA = nullptr;
static size_t g_dataSize = 0;
static size_t g_pos;
const size_t FUZZ_INPUT_SIZE_THRESHOLD = 10;
typedef void (*TestPtr)();

template<class T>
uint32_t GetArrLength(T &arr)
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

void AudioEffectServiceFuzzTest()
{
    std::shared_ptr<AudioEffectService> audioEffectService = std::make_shared<AudioEffectService>();
    if (audioEffectService == nullptr) {
        return;
    }
    audioEffectService->EffectServiceInit();
    audioEffectService->BuildAvailableAEConfig();
}

void AudioEffectServiceGetAvailableEffectsFuzzTest()
{
    std::shared_ptr<AudioEffectService> audioEffectService = std::make_shared<AudioEffectService>();
    if (audioEffectService == nullptr) {
        return;
    }

    std::vector<Effect> availableEffects;
    audioEffectService->GetAvailableEffects(availableEffects);
}

void AudioEffectServiceGetOriginalEffectConfigFuzzTest()
{
    std::shared_ptr<AudioEffectService> audioEffectService = std::make_shared<AudioEffectService>();
    if (audioEffectService == nullptr) {
        return;
    }

    OriginalEffectConfig oriEffectConfig;
    audioEffectService->GetOriginalEffectConfig(oriEffectConfig);
}

void AudioEffectServiceUpdateAvailableEffectsFuzzTest()
{
    std::shared_ptr<AudioEffectService> audioEffectService = std::make_shared<AudioEffectService>();
    if (audioEffectService == nullptr) {
        return;
    }

    std::vector<Effect> newAvailableEffects;
    audioEffectService->UpdateAvailableEffects(newAvailableEffects);
}

void AudioEffectServiceQueryEffectManagerSceneModeFuzzTest()
{
    std::shared_ptr<AudioEffectService> audioEffectService = std::make_shared<AudioEffectService>();
    if (audioEffectService == nullptr) {
        return;
    }

    SupportedEffectConfig supportedEffectConfig;
    audioEffectService->QueryEffectManagerSceneMode(supportedEffectConfig);
}

void AudioEffectServiceGetSupportedEffectConfigFuzzTest()
{
    std::shared_ptr<AudioEffectService> audioEffectService = std::make_shared<AudioEffectService>();
    if (audioEffectService == nullptr) {
        return;
    }

    SupportedEffectConfig supportedEffectConfig;
    audioEffectService->GetSupportedEffectConfig(supportedEffectConfig);
}

void AudioEffectServiceSetMasterSinkAvailableFuzzTest()
{
    std::shared_ptr<AudioEffectService> audioEffectService = std::make_shared<AudioEffectService>();
    if (audioEffectService == nullptr) {
        return;
    }

    audioEffectService->SetMasterSinkAvailable();
    audioEffectService->SetEffectChainManagerAvailable();
    audioEffectService->CanLoadEffectSinks();
}

void AudioEffectServiceConstructEffectChainModeFuzzTest()
{
    std::shared_ptr<AudioEffectService> audioEffectService = std::make_shared<AudioEffectService>();
    if (audioEffectService == nullptr) {
        return;
    }

    Device device1;
    Device device2;
    device2.chain = "chain";
    device2.type = "DEVICE_TYPE_DEFAULT";
    StreamEffectMode mode;
    mode.mode = "mode";
    mode.devicePort.push_back(device1);
    mode.devicePort.push_back(device2);
    std::string sceneType = "sceneType";
    EffectChainManagerParam effectChainMgrParam;
    audioEffectService->ConstructEffectChainMode(mode, sceneType, effectChainMgrParam);
}

void AudioEffectServiceConstructEffectChainManagerParamFuzzTest()
{
    static const vector<ScenePriority> testScenePriorities = {
        DEFAULT_SCENE,
        PRIOR_SCENE,
        NORMAL_SCENE,
    };
    std::shared_ptr<AudioEffectService> audioEffectService = std::make_shared<AudioEffectService>();
    if (audioEffectService == nullptr || testScenePriorities.empty()) {
        return;
    }

    Stream stream1;
    Stream stream2;
    stream1.scene = "test";
    stream1.priority = testScenePriorities[GetData<uint32_t>() % testScenePriorities.size()];
    stream2.scene = "test";
    stream2.priority = testScenePriorities[GetData<uint32_t>() % testScenePriorities.size()];
    EffectChainManagerParam effectChainManagerParam;
    audioEffectService->supportedEffectConfig_.postProcessNew.stream.push_back(stream1);
    audioEffectService->supportedEffectConfig_.postProcessNew.stream.push_back(stream2);
    audioEffectService->ConstructEffectChainManagerParam(effectChainManagerParam);
}

void AudioEffectServiceConstructEnhanceChainManagerParamFuzzTest()
{
    static const vector<ScenePriority> testScenePriorities = {
        DEFAULT_SCENE,
        PRIOR_SCENE,
        NORMAL_SCENE,
    };
    std::shared_ptr<AudioEffectService> audioEffectService = std::make_shared<AudioEffectService>();
    if (audioEffectService == nullptr || testScenePriorities.empty()) {
        return;
    }

    Stream stream1;
    Stream stream2;
    stream1.scene = "test";
    stream1.priority = testScenePriorities[GetData<uint32_t>() % testScenePriorities.size()];
    stream2.scene = "test";
    stream2.priority = testScenePriorities[GetData<uint32_t>() % testScenePriorities.size()];
    EffectChainManagerParam effectChainManagerParam;
    audioEffectService->supportedEffectConfig_.preProcessNew.stream.push_back(stream1);
    audioEffectService->supportedEffectConfig_.preProcessNew.stream.push_back(stream2);
    audioEffectService->ConstructEnhanceChainManagerParam(effectChainManagerParam);
}

TestPtr g_testPtrs[] = {
    AudioEffectServiceFuzzTest,
    AudioEffectServiceGetAvailableEffectsFuzzTest,
    AudioEffectServiceGetOriginalEffectConfigFuzzTest,
    AudioEffectServiceUpdateAvailableEffectsFuzzTest,
    AudioEffectServiceQueryEffectManagerSceneModeFuzzTest,
    AudioEffectServiceGetSupportedEffectConfigFuzzTest,
    AudioEffectServiceSetMasterSinkAvailableFuzzTest,
    AudioEffectServiceConstructEffectChainModeFuzzTest,
    AudioEffectServiceConstructEffectChainManagerParamFuzzTest,
    AudioEffectServiceConstructEnhanceChainManagerParamFuzzTest,
};

void FuzzTest(const uint8_t *rawData, size_t size)
{
    if (rawData == nullptr) {
        return;
    }

    RAW_DATA = rawData;
    g_dataSize = size;
    g_pos = 0;

    uint32_t code = GetData<uint32_t>();
    uint32_t len = GetArrLength(g_testPtrs);
    if (len > 0) {
        g_testPtrs[code % len]();
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