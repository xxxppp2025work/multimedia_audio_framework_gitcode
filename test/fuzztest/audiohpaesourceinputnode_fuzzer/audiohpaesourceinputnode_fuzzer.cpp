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
#include <vector>
#include <memory>
#include <queue>
#include <string>
#undef private
#include "hpae_node.h"
#include "hpae_pcm_buffer.h"
#include "source/i_audio_capture_source.h"
#include "common/hdi_adapter_type.h"
#include "common/hdi_adapter_info.h"
#include "manager/hdi_adapter_manager.h"
using namespace std;
using namespace OHOS::AudioStandard::HPAE;


namespace OHOS {
namespace AudioStandard {
using namespace std;
static const uint8_t *RAW_DATA = nullptr;
static size_t g_dataSize = 0;
static size_t g_pos;
const size_t THRESHOLD = 10;
typedef void (*TestPtr)(const uint8_t *,size_t)

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
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    hpaeSinkInputNode->DoProcess();
}

void ResetFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    hpaeSinkInputNode->Reset();
}

void ResetAllFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    hpaeSinkInputNode->ResetAll();
}

void GetSharedInstanceFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    hpaeSinkInputNode->GetSharedInstance(callback);
}

void GetOutputPortBufferTypeFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    HpaeNodeInfo nodeInfo;
    hpaeSinkInputNode->GetOutputPortBufferType(nodeInfo);
}

void GetCapturerSourceInstanceFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    const std::string &deviceClass = nodeInfo.deviceClass;
    const std::string &deviceNetId = nodeInfo.deviceNetId;
    const std::string &sourceType = nodeInfo.sourceType;
    const std::string &sourceName = nodeInfo.sourceName;
    hpaeSinkInputNode->GetCapturerSourceInstance(deviceClass,deviceNetId,sourceType,sourceName);
}

void CapturerSourceInitFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    IAudioSourceAttr attr;
    hpaeSinkInputNode->CapturerSourceInit(attr);
}

void CapturerSourceDeInitFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    hpaeSinkInputNode->CapturerSourceDeInit();
}

void CapturerSourceFlushFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    hpaeSinkInputNode->CapturerSourceFlush();
}

void CapturerSourcePauseFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    hpaeSinkInputNode->CapturerSourcePause();
}

void CapturerSourceResetFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    hpaeSinkInputNode->CapturerSourceReset();
}

void CapturerSourceResumeFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    hpaeSinkInputNode->CapturerSourceResume();
}

void CapturerSourceStartFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    hpaeSinkInputNode->CapturerSourceStart();
}

void CapturerSourceStopFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    hpaeSinkInputNode->CapturerSourceStop();
}

void GetSourceStateFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    hpaeSinkInputNode->GetSourceState();
}

void SetSourceStateFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    StreamManagerState sourceState;
    hpaeSinkInputNode->SetSourceState(sourceState);
}

void WriteCapturerDataFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    char *data = NULL;
    int32_t dataSize = GetData<int32_t>();
    hpaeSinkInputNode->WriteCapturerData(data,dataSize);
}

void GetOutputPortNumFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    hpaeSinkInputNode->GetOutputPortNum();
}

void GetSourceInputNodeTypeFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    hpaeSinkInputNode->GetSourceInputNodeType();
}

void SetSourceInputNodeTypeFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    HpaeSourceInputNodeType type;
    hpaeSinkInputNode->SetSourceInputNodeType(type);
}

void GetNodeInfoWithInfoFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    HpaeSourceBufferType type;
    hpaeSinkInputNode->GetNodeInfoWithInfo(type);
}

void UpdateAppsUidAndSessionIdFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    std::vector<int32_t> &appsUid = GetData<int32_t>();
    std::vector<int32_t> &sessionsId = GetData<int32_t>();
    hpaeSinkInputNode->UpdateAppsUidAndSessionId(appsUid,sessionsId);
}

void GetCaptureIdFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    hpaeSinkInputNode->GetCaptureIdSessionId();
}

typedef void (*TestFuncs[23])();

TestFuncs g_testFuncs = {
    DoProcessFuzzTest,
    ResetFuzzTest,
    ResetAllFuzzTest,
    GetSharedInstanceFuzzTest,
    GetOutputPortBufferTypeFuzzTest,
    GetCapturerSourceInstanceFuzzTest,
    CapturerSourceInitFuzzTest,
    CapturerSourceDeInitFuzzTest,
    CapturerSourceFlushFuzzTest,
    CapturerSourcePauseFuzzTest,
    CapturerSourceResetFuzzTest,
    CapturerSourceResumeFuzzTest,
    CapturerSourceStartFuzzTest,
    CapturerSourceStopFuzzTest,
    GetSourceStateFuzzTest,
    SetSourceStateFuzzTest,
    WriteCapturerDataFuzzTest,
    GetOutputPortNumFuzzTest,
    GetSourceInputNodeTypeFuzzTest,
    SetSourceInputNodeTypeFuzzTest,
    GetNodeInfoWithInfoFuzzTest,
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
