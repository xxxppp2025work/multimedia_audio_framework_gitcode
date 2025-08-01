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

#include "audio_log.h"
#include "audio_bundle_manager.h"

using namespace std;

namespace OHOS {
namespace AudioStandard {

static const uint8_t* RAW_DATA = nullptr;
static size_t g_dataSize = 0;
static size_t g_pos;
const size_t THRESHOLD = 10;
typedef void (*TestPtr)();

template<class T>
uint32_t GetArrLength(T& arr)
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

void AudioBundleManagerGetUidByBundleNameFuzzTest()
{
    std::string bundleName = "tesxBundleName"
    int userId = GetData<int>();
    AudioBundleManager::GetUidByBundleName(bundleName, userId);
}

void AudioBundleManagerGetBundleNameFuzzTest()
{
    AudioBundleManager::GetBundleName();
}

void AudioBundleManagerGetBundleNameFromUidFuzzTest()
{
    int32_t callingUid = GetData<int32_t>();
    AudioBundleManager::GetBundleNameFromUid(callingUid);
}

void AudioBundleManagerGetBundleInfoFuzzTest()
{
    AudioBundleManager::GetBundleInfo();
}

void AudioBundleManagerGetBundleInfoFromUidFuzzTest()
{
    int32_t callingUid = GetData<int32_t>();
    AudioBundleManager::GetBundleInfoFromUid(callingUid);
}

TestPtr g_testPtrs[] = {
    AudioBundleManagerGetUidByBundleNameFuzzTest,
    AudioBundleManagerGetBundleNameFuzzTest,
    AudioBundleManagerGetBundleNameFromUidFuzzTest,
    AudioBundleManagerGetBundleInfoFuzzTest,
    AudioBundleManagerGetBundleInfoFromUidFuzzTest,
};

void FuzzTest(const uint8_t* rawData, size_t size)
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
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    if (size < OHOS::AudioStandard::THRESHOLD) {
        return 0;
    }
    OHOS::AudioStandard::FuzzTest(data, size);
    return 0;
}