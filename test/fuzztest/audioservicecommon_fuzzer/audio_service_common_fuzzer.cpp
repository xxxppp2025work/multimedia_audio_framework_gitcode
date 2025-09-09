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
#ifndef LOG_TAG
#define LOG_TAG "AudioFuzzTest"
#endif

#include <iostream>
#include <cstddef>
#include <cstdint>
#include "audio_hdi_log.h"
#include "audio_common_converter.h"
#include "message_parcel.h"
#include "audio_info.h"
#include "audio_source_type.h"
#include "audio_ring_cache.h"
#include "audio_thread_task.h"
#include "../fuzz_utils.h"

namespace OHOS {
namespace AudioStandard {
using namespace std;
static const std::string THREAD_NAME = "FuzzTestThreadName";
FuzzUtils &g_fuzzUtils = FuzzUtils::GetInstance();
typedef void (*TestFuncs)();

void AudioThreadTaskFuzzTest()
{
    std::unique_ptr<AudioThreadTask> audioThreadTask;
    audioThreadTask = std::make_unique<AudioThreadTask>(THREAD_NAME);
    CHECK_AND_RETURN(audioThreadTask != nullptr);
    auto myJob = []() {
        AUDIO_INFO_LOG("Hello Fuzz Test!");
    };
    audioThreadTask->state_ = g_fuzzUtils.GetData<AudioThreadTask::RunningState>();
    audioThreadTask->RegisterJob(std::move(myJob));
    audioThreadTask->state_ = g_fuzzUtils.GetData<AudioThreadTask::RunningState>();
    audioThreadTask->Start();
    audioThreadTask->state_ = g_fuzzUtils.GetData<AudioThreadTask::RunningState>();
    audioThreadTask->CheckThreadIsRunning();
    audioThreadTask->state_ = g_fuzzUtils.GetData<AudioThreadTask::RunningState>();
    audioThreadTask->Pause();
    audioThreadTask->state_ = g_fuzzUtils.GetData<AudioThreadTask::RunningState>();
    audioThreadTask->Start();
    audioThreadTask->state_ = g_fuzzUtils.GetData<AudioThreadTask::RunningState>();
    audioThreadTask->PauseAsync();
    audioThreadTask->state_ = g_fuzzUtils.GetData<AudioThreadTask::RunningState>();
    audioThreadTask->Start();
    audioThreadTask->state_ = g_fuzzUtils.GetData<AudioThreadTask::RunningState>();
    audioThreadTask->StopAsync();
    audioThreadTask->state_ = g_fuzzUtils.GetData<AudioThreadTask::RunningState>();
    audioThreadTask->Start();
    audioThreadTask->state_ = g_fuzzUtils.GetData<AudioThreadTask::RunningState>();
    audioThreadTask->Stop();
}

vector<TestFuncs> g_testFuncs = {
    AudioThreadTaskFuzzTest,
};
} // namespace AudioStandard
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    /* Run your code on data */
    OHOS::AudioStandard::g_fuzzUtils.fuzzTest(data, size, OHOS::AudioStandard::g_testFuncs);
    return 0;
}
