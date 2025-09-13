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
#include "audio_service.h"
#include "../fuzz_utils.h"

namespace OHOS {
namespace AudioStandard {
using namespace std;

FuzzUtils &g_fuzzUtils = FuzzUtils::GetInstance();

typedef void (*TestFuncs)();

void DisableLoopbackFuzzTest()
{
    shared_ptr<AudioService> audioService = make_shared<AudioService>();
    if (audioService == nullptr) {
        return;
    }
    audioService->DisableLoopback();
}

void SendInterruptEventToAudioServiceFuzzTest()
{
    shared_ptr<AudioService> audioService = make_shared<AudioService>();
    if (audioService == nullptr) {
        return;
    }
    uint32_t sessionId = g_fuzzUtils.GetData<uint32_t>();
    InterruptEventInternal interruptEvent;
    audioService->SendInterruptEventToAudioService(sessionId, interruptEvent);
}

void UpdateResumeInterruptEventMapFuzzTest()
{
    shared_ptr<AudioService> audioService = make_shared<AudioService>();
    if (audioService == nullptr) {
        return;
    }
    uint32_t sessionId = g_fuzzUtils.GetData<uint32_t>();
    InterruptEventInternal interruptEvent;
    audioService->resumeInterruptEventMap_[sessionId] = interruptEvent;
    interruptEvent.hintType = INTERRUPT_HINT_RESUME;
    audioService->SendInterruptEventToAudioService(sessionId, interruptEvent);
}

void UpdatePauseInterruptEventMapFuzzTest()
{
    shared_ptr<AudioService> audioService = make_shared<AudioService>();
    if (audioService == nullptr) {
        return;
    }
    uint32_t sessionId = g_fuzzUtils.GetData<uint32_t>();
    InterruptEventInternal interruptEvent;
    audioService->pauseInterruptEventMap_[sessionId] = interruptEvent;
    interruptEvent.hintType = INTERRUPT_HINT_PAUSE;
    audioService->SendInterruptEventToAudioService(sessionId, interruptEvent);
}

void RemoveResumeInterruptEventMapFuzzTest()
{
    shared_ptr<AudioService> audioService = make_shared<AudioService>();
    if (audioService == nullptr) {
        return;
    }
    uint32_t sessionId = g_fuzzUtils.GetData<uint32_t>();
    InterruptEventInternal interruptEvent;
    audioService->resumeInterruptEventMap_[sessionId] = interruptEvent;
    audioService->RemoveResumeInterruptEventMap(sessionId);
}

void IsStreamInterruptResumeFuzzTest()
{
    shared_ptr<AudioService> audioService = make_shared<AudioService>();
    if (audioService == nullptr) {
        return;
    }
    uint32_t sessionId = g_fuzzUtils.GetData<uint32_t>();
    InterruptEventInternal interruptEvent;
    audioService->resumeInterruptEventMap_[sessionId] = interruptEvent;
    audioService->IsStreamInterruptResume(sessionId);
}

void RemovePauseInterruptEventMapFuzzTest()
{
    shared_ptr<AudioService> audioService = make_shared<AudioService>();
    if (audioService == nullptr) {
        return;
    }
    uint32_t sessionId = g_fuzzUtils.GetData<uint32_t>();
    InterruptEventInternal interruptEvent;
    audioService->pauseInterruptEventMap_[sessionId] = interruptEvent;
    audioService->RemovePauseInterruptEventMap(sessionId);
}

void IsStreamInterruptPauseFuzzTest()
{
    shared_ptr<AudioService> audioService = make_shared<AudioService>();
    if (audioService == nullptr) {
        return;
    }
    uint32_t sessionId = g_fuzzUtils.GetData<uint32_t>();
    InterruptEventInternal interruptEvent;
    audioService->pauseInterruptEventMap_[sessionId] = interruptEvent;
    audioService->IsStreamInterruptPause(sessionId);
}

vector<TestFuncs> g_testFuncs = {
    DisableLoopbackFuzzTest,
    SendInterruptEventToAudioServiceFuzzTest,
    UpdateResumeInterruptEventMapFuzzTest,
    UpdatePauseInterruptEventMapFuzzTest,
    RemoveResumeInterruptEventMapFuzzTest,
    IsStreamInterruptResumeFuzzTest,
    RemovePauseInterruptEventMapFuzzTest,
    IsStreamInterruptPauseFuzzTest,
};

} // namespace AudioStandard
} // namesapce OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    OHOS::AudioStandard::g_fuzzUtils.fuzzTest(data, size, OHOS::AudioStandard::g_testFuncs);
    return 0;
}
