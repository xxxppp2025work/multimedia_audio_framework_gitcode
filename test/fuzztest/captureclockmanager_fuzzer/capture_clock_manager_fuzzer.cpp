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

#include "capturer_clock_manager.h"
#include "../fuzz_utils.h"

namespace OHOS {
namespace AudioStandard {

FuzzUtils &g_fuzzUtils = FuzzUtils::GetInstance();
typedef void (*TestFuncs)();

void CreateCapturerClockFuzzTest()
{
    uint32_t sessionId = g_fuzzUtils.GetData<uint32_t>();
    uint32_t sampleRate = g_fuzzUtils.GetData<uint32_t>();
    CapturerClockManager::GetInstance().CreateCapturerClock(sessionId, sampleRate);
}

void DeleteCapturerClockFuzzTest()
{
    uint32_t sessionId = g_fuzzUtils.GetData<uint32_t>();
    CapturerClockManager::GetInstance().DeleteCapturerClock(sessionId);
}

void GetCapturerClockFuzzTest()
{
    uint32_t sessionId = g_fuzzUtils.GetData<uint32_t>();
    CapturerClockManager::GetInstance().GetCapturerClock(sessionId);
}

void RegisterAudioSourceClockFuzzTest()
{
    uint32_t captureId = g_fuzzUtils.GetData<uint32_t>();
    std::shared_ptr<AudioSourceClock> srcClock = std::make_shared<AudioSourceClock>();
    CapturerClockManager::GetInstance().RegisterAudioSourceClock(captureId, srcClock);
}

void DeleteAudioSourceClockFuzzTest()
{
    uint32_t captureId = g_fuzzUtils.GetData<uint32_t>();
    CapturerClockManager::GetInstance().DeleteAudioSourceClock(captureId);
}

void GetAudioSourceClockFuzzTest()
{
    uint32_t captureId = g_fuzzUtils.GetData<uint32_t>();
    CapturerClockManager::GetInstance().GetAudioSourceClock(captureId);
}

std::vector<TestFuncs> g_testFuncs = {
    CreateCapturerClockFuzzTest,
    DeleteCapturerClockFuzzTest,
    GetCapturerClockFuzzTest,
    RegisterAudioSourceClockFuzzTest,
    DeleteAudioSourceClockFuzzTest,
    GetAudioSourceClockFuzzTest,
};
} // namespace AudioStandard
} // namesapce OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    OHOS::AudioStandard::g_fuzzUtils.fuzzTest(data, size, OHOS::AudioStandard::g_testFuncs);
    return 0;
}
