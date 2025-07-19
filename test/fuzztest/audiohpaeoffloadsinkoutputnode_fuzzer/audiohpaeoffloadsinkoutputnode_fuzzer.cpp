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
#include "audio_info.h"
#include "sink/i_audio_render_sink.h"
#include "common/hdi_adapter_info.h"
#include "manager/hdi_adapter_manager.h"
#include "high_resolution_timer.h"

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
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    hpaeOffloadSinkOutputNode->DoProcess();
}

void ResetFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    hpaeOffloadSinkOutputNode->Reset();
}

void ResetAllFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    hpaeOffloadSinkOutputNode->ResetAll();
}

void ConnectFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    std::shared_ptr<OutputNode<HpaePcmBuffer *>> inputs;
    const std::shared_ptr<OutputNode<HpaePcmBuffer *>> &preNode = inputs;
    hpaeOffloadSinkOutputNode->Connect(preNode);
}

void DisConnectFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    std::shared_ptr<OutputNode<HpaePcmBuffer *>> inputs;
    const std::shared_ptr<OutputNode<HpaePcmBuffer *>> &preNode = inputs;
    hpaeOffloadSinkOutputNode->DisConnect(preNode);
}

void GetRenderSinkInstanceFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    const std::string &deviceClass = nodeInfo.deviceClass;
    const std::string &deviceNetworkId = nodeInfo.deviceNetworkId;
    hpaeOffloadSinkOutputNode->GetRenderSinkInstance(deviceClass,deviceNetworkId);
}

void RenderSinkInitFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    IAudioSinkAttr attr;
    hpaeOffloadSinkOutputNode->RenderSinkInit(attr);
}

void RenderSinkDeInitFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    hpaeOffloadSinkOutputNode->RenderSinkDeInit();
}

void RenderSinkFlushFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    hpaeOffloadSinkOutputNode->RenderSinkFlush();
}

void RenderSinkStartFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    hpaeOffloadSinkOutputNode->RenderSinkStart();
}

void RenderSinkStopFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    hpaeOffloadSinkOutputNode->RenderSinkStop();
}

void GetPreOutNumFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    hpaeOffloadSinkOutputNode->GetPreOutNum();
}

void GetSinkStateFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    hpaeOffloadSinkOutputNode->GetSinkState();
}

void SetSinkStateFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    StreamManagerState sinkState;
    hpaeOffloadSinkOutputNode->SetSinkState(sinkState);
}

void GetRenderFrameDataFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    hpaeOffloadSinkOutputNode->GetRenderFrameData();
}

void StopStreamFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    hpaeOffloadSinkOutputNode->StopStream();
}

void FlushStreamFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    hpaeOffloadSinkOutputNode->FlushStream();
}

void SetPolicyStateFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    int32_t policyState = GetData<int32_t>();
    hpaeOffloadSinkOutputNode->SetPolicyState(policyState);
}

void GetLatencyFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    hpaeOffloadSinkOutputNode->GetLatency();
}

void SetTimeoutStopThdFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    uint32_t timeoutThdMs = GetData<uint32_t>();
    hpaeOffloadSinkOutputNode->SetTimeoutStopThd(timeoutThdMs);
}

void SetOffloadRenderCallbackTypeFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    int32_t type = GetData<int32_t>();
    hpaeOffloadSinkOutputNode->SetOffloadRenderCallbackType(type);
}

void UpdateAppsUidFuzzTest()
{
    HpaeNodeInfo nodeInfo;
    auto hpaeOffloadSinkOutputNode = std::make_shared<HpaeOffloadSinkOutputNode>(nodeInfo);
    const std::vector<int32_t> &appsUid = GetData<int32_t>();
    hpaeOffloadSinkOutputNode->UpdateAppsUid(appsUid);
}

typedef void (*TestFuncs[22])();

TestFuncs g_testFuncs = {
    DoProcessFuzzTest,
    ResetFuzzTest,
    ResetAllFuzzTest,
    ConnectFuzzTest,
    DisConnectFuzzTest,
    GetRenderSinkInstanceFuzzTest,
    RenderSinkInitFuzzTest,
    RenderSinkDeInitFuzzTest,
    RenderSinkFlushFuzzTest,
    RenderSinkStartFuzzTest,
    RenderSinkStopFuzzTest,
    GetPreOutNumFuzzTest,
    GetSinkStateFuzzTest,
    SetSinkStateFuzzTest,
    GetRenderFrameDataFuzzTest,
    StopStreamFuzzTest,
    FlushStreamFuzzTest,
    SetPolicyStateFuzzTest,
    GetLatencyFuzzTest,
    SetTimeoutStopThdFuzzTest,
    SetOffloadRenderCallbackTypeFuzzTest,
    UpdateAppsUidFuzzTest,
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
