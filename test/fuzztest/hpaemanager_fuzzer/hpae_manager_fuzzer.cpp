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
#include <cstring>
#include "audio_info.h"
#include "audio_policy_server.h"
#include "audio_policy_service.h"
#include "audio_device_info.h"
#include "audio_utils.h"
#include "accesstoken_kit.h"
#include "nativetoken_kit.h"
#include "token_setproc.h"
#include "access_token.h"
#include "audio_channel_blend.h"
#include "volume_ramp.h"
#include "audio_speed.h"

#include "audio_policy_utils.h"
#include "audio_stream_descriptor.h"
#include "audio_limiter_manager.h"
#include "dfx_msg_manager.h"
#include "hpae_manager.h"
#include "hpae_manager_fuzzer.h"
#include "audio_info.h"

namespace OHOS {
namespace AudioStandard {
using namespace std;

static const uint8_t* RAW_DATA = nullptr;
static size_t g_dataSize = 0;
static size_t g_pos;
const size_t THRESHOLD = 10;
const uint8_t TESTSIZE = 6;
static std::string g_rootPath = "/data/";
static int32_t NUM_2 = 2;

typedef void (*TestFuncs)();

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

void InitFuzzTest()
{
    std::shared_ptr<HPAE::HpaeManager> hpaeManager_ = std::make_shared<HPAE::HpaeManager>();
    if (hpaeManager_ == nullptr) {
        return;
    }
    hpaeManager_->Init();
    hpaeManager_->IsInit();
    sleep(1);
    hpaeManager_->IsRunning();
    hpaeManager_->DeInit();
    hpaeManager_->IsInit();
    sleep(1);
    hpaeManager_->IsRunning();
    hpaeManager_->DeInit();
    hpaeManager_ = nullptr;
}

void SuspendAudioDeviceFuzzTest()
{
    std::shared_ptr<HPAE::HpaeManager> hpaeManager_ = std::make_shared<HPAE::HpaeManager>();
    if (hpaeManager_ == nullptr) {
        return;
    }
    hpaeManager_->Init();
    AudioModuleInfo audioModuleInfo;
    audioModuleInfo.adapterName = "Speaker_File";
    bool isSuspend = GetData<uint32_t>() % NUM_2;
    hpaeManager_->OpenAudioPort(audioModuleInfo);
    hpaeManager_->SetDefaultSink(audioModuleInfo.name);
    hpaeManager_->SuspendAudioDevice(audioModuleInfo.adapterName, isSuspend);
    hpaeManager_->DeInit();
    hpaeManager_ = nullptr;
}

void SetSinkMuteFuzzTest()
{
    std::shared_ptr<HPAE::HpaeManager> hpaeManager_ = std::make_shared<HPAE::HpaeManager>();
    if (hpaeManager_ == nullptr) {
        return;
    }
    hpaeManager_->Init();
    std::string deviceName = "virtual1";
    hpaeManager_->SetSinkMute(deviceName, GetData<uint32_t>() % NUM_2, GetData<uint32_t>() % NUM_2);
    hpaeManager_->DeInit();
    hpaeManager_ = nullptr;
}

void SetSourceOutputMuteFuzzTest()
{
    std::shared_ptr<HPAE::HpaeManager> hpaeManager_ = std::make_shared<HPAE::HpaeManager>();
    if (hpaeManager_ == nullptr) {
        return;
    }
    hpaeManager_->Init();
    int32_t uid = GetData<int32_t>();
    hpaeManager_->SetSourceOutputMute(uid, GetData<uint32_t>() % NUM_2);
    hpaeManager_->DeInit();
    hpaeManager_ = nullptr;
}

void GetAllSinksFuzzTest()
{
    std::shared_ptr<HPAE::HpaeManager> hpaeManager_ = std::make_shared<HPAE::HpaeManager>();
    if (hpaeManager_ == nullptr) {
        return;
    }
    hpaeManager_->Init();
    hpaeManager_->GetAllSinks();
    hpaeManager_->DeInit();
    hpaeManager_ = nullptr;
}

void RegisterSerivceCallbackFuzzTest()
{
    std::shared_ptr<HPAE::HpaeManager> hpaeManager_ = std::make_shared<HPAE::HpaeManager>();
    if (hpaeManager_ == nullptr) {
        return;
    }
    hpaeManager_->Init();
    AudioModuleInfo audioModuleInfo;
    audioModuleInfo.lib = "libmodule-hdi-sink.z.so";
    audioModuleInfo.channels = "2";
    audioModuleInfo.rate = "48000";
    audioModuleInfo.name = "Speaker_File";
    audioModuleInfo.adapterName = "file_io";
    audioModuleInfo.className = "file_io";
    audioModuleInfo.bufferSize = "7680";
    audioModuleInfo.format = "s32le";
    audioModuleInfo.fixedLatency = "1";
    audioModuleInfo.offloadEnable = "0";
    audioModuleInfo.networkId = "LocalDevice";
    audioModuleInfo.fileName = g_rootPath + audioModuleInfo.adapterName + "_" + audioModuleInfo.rate + "_" +
                               audioModuleInfo.channels + "_" + audioModuleInfo.format + ".pcm";
    std::stringstream typeValue;
    typeValue << static_cast<int32_t>(DEVICE_TYPE_SPEAKER);
    audioModuleInfo.deviceType = typeValue.str();
    uint32_t sinkSourceIndex = GetData<uint32_t>();
    bool isReload = GetData<uint32_t>() % NUM_2;
    hpaeManager_->CreateRendererManager(audioModuleInfo, sinkSourceIndex, isReload);
    hpaeManager_->DeInit();
    hpaeManager_ = nullptr;
}

TestFuncs g_testFuncs[TESTSIZE] = {
    InitFuzzTest,
    SuspendAudioDeviceFuzzTest,
    SetSinkMuteFuzzTest,
    SetSourceOutputMuteFuzzTest,
    GetAllSinksFuzzTest,
    RegisterSerivceCallbackFuzzTest,
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
