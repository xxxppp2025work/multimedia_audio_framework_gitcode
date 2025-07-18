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
#include <vector>
#include <memory>
#include <queue>
#include <string>
#undef private
#include "hpae_source_input_cluster.h"
#include "audio_errors.h"
#include "hpae_source_input_node.h"
#include "hpae_source_output_node.h"
#include "hpae_format_convert.h"
using namespace std;
using namespace OHOS::AudioStandard::HPAE;


namespace OHOS {
namespace AudioStandard {
using namespace std;
static const uint8_t *RAW_DATA = nullptr;
static size_t g_dataSize = 0;
static size_t g_pos;
const size_t THRESHOLD = 10;
typedef void (*TestPtr)(const uint8_t *,size_t);

template<class T>
T GetData()
{
    T object {};
    size_t objectSize = sizeof(object);
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
uint32_t GetArrLength(T& arr)
{
    if (arr == nullptr) {
        AUDIO_INFO_LOG("%{public}s: The array length is equal to 0", __func__);
        return 0;
    }
    return sizeof(arr) / sizeof(arr[0]);
}


void DoProcessFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->DoProcess();
}

void ResetFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->Reset();
}

void ResetAllFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->ResetAll();
}

void GetSharedInstanceFuzzTest1()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->GetSharedInstance();
}

void GetSharedInstanceFuzzTest2()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->GetSharedInstance(nodeInfo);
}

void GetOutputPortFuzzTest1()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->GetOutputPort();
}

void GetOutputPortFuzzTest2()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->GetOutputPort(nodeInfo);
}

void GetCapturerSourceInstanceFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    const std::string deviceClass = nodeInfo.deviceClass;
    const std::string deviceNetId = nodeInfo.deviceNetId;
    const SourceType sourceType;
    const std::string sourceName = nodeInfo.sourceName;
    hpaeProcessCluster->GetCapturerSourceInstance(deviceClass,deviceNetId,sourceType,sourceName);
}

void GetOutputPortFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    IAudioSourceAttr attr;
    hpaeProcessCluster->GetOutputPort(attr);
}

void CapturerSourceDeInitFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->CapturerSourceDeInit();
}

void CapturerSourceFlushFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->CapturerSourceFlush();
}

void CapturerSourcePauseFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->CapturerSourcePause();
}

void CapturerSourceResetFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->CapturerSourceReset();
}

void CapturerSourceResumeFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->CapturerSourceResume();
}

void CapturerSourceStartFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->CapturerSourceStart();
}

void CapturerSourceStopFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->CapturerSourceStop();
}

void GetSourceStateFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->GetSourceState();
}

void GetOutputPortNumFuzzTest1()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->GetOutputPortNum();
}

void GetOutputPortNumFuzzTest2()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->GetOutputPortNum(nodeInfo);
}

void GetSourceInputNodeTypeFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->GetSourceInputNodeType();
}

void GSetSourceInputNodeTypeFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    HpaeSourceInputNodeType type = HpaeSourceInputNodeType::HPAE_SOURCE_DEFAULT;
    hpaeProcessCluster->SetSourceInputNodeType(type);
}

void UpdateAppsUidAndSessionIdFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    std::vector<int32_t> appsUid = {GetData<int32_t>()};
    std::vector<int32_t> sessionsId = {GetData<int32_t>()};
    hpaeProcessCluster->UpdateAppsUidAndSessionId(appsUid,sessionsId);
}

void GetCaptureIdFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeProcessCluster = std::make_shared<HpaeProcessCluster>(nodeInfo);
    hpaeProcessCluster->GetCaptureId();
}

typedef void (*TestFuncs[23])();

TestFuncs g_testFuncs = {
    DoProcessFuzzTest,
    ResetFuzzTest,
    ResetAllFuzzTest,
    GetSharedInstanceFuzzTest1,
    GetSharedInstanceFuzzTest2,
    GetOutputPortFuzzTest1,
    GetOutputPortFuzzTest2,
    GetCapturerSourceInstanceFuzzTest,
    GetOutputPortFuzzTest,
    CapturerSourceDeInitFuzzTest,
    CapturerSourceFlushFuzzTest,
    CapturerSourcePauseFuzzTest,
    CapturerSourceResetFuzzTest,
    CapturerSourceResumeFuzzTest,
    CapturerSourceStartFuzzTest,
    CapturerSourceStopFuzzTest,
    GetSourceStateFuzzTest,
    GetOutputPortNumFuzzTest1,
    GetOutputPortNumFuzzTest2,
    GetSourceInputNodeTypeFuzzTest,
    GSetSourceInputNodeTypeFuzzTest,
    UpdateAppsUidAndSessionIdFuzzTest,
    GetCaptureIdFuzzTest,
};

bool FuzzTest(const uint8_t* rawData, size_t size)
{
    if (rawData == nullptr) {
        return false;
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

    return true;
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
