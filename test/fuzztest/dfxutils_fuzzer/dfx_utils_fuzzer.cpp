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
#include "dfx_utils.h"
#include "../fuzz_utils.h"

namespace OHOS {
namespace AudioStandard {
using namespace std;

static const uint8_t *RAW_DATA = nullptr;
static size_t g_dataSize = 0;
static size_t g_pos;
const size_t FUZZ_INPUT_SIZE_THRESHOLD = 10;
const uint8_t TESTSIZE = 3;

typedef void (*TestFuncs)();

void SerializeToJSONString1FuzzTest()
{
    DfxUtils dfxUtils;
    RendererStats data;
    dfxUtils.SerializeToJSONString(data);
}

void SerializeToJSONString2FuzzTest()
{
    DfxUtils dfxUtils;
    CapturerStats data;
    dfxUtils.SerializeToJSONString(data);
}

void SerializeToJSONString3FuzzTest()
{
    DfxUtils dfxUtils;
    std::vector<InterruptEffect> data;
    InterruptEffect gameEffect;
    gameEffect.bundleName = "com.example.game";
    gameEffect.streamUsage = GetData<uint8_t>(g_dataSize, g_pos, RAW_DATA);
    gameEffect.appState = GetData<uint8_t>(g_dataSize, g_pos, RAW_DATA);
    gameEffect.interruptEvent = GetData<uint8_t>(g_dataSize, g_pos, RAW_DATA);
    data.push_back(gameEffect);
    dfxUtils.SerializeToJSONString(data);
}

TestFuncs g_testFuncs[TESTSIZE] = {
    SerializeToJSONString1FuzzTest,
    SerializeToJSONString2FuzzTest,
    SerializeToJSONString3FuzzTest,
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

    uint32_t code = GetData<uint32_t>(g_dataSize, g_pos, RAW_DATA);
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
