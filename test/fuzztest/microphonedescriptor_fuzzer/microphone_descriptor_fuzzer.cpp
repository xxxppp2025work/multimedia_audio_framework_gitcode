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
#include "microphone_descriptor.h"
#include "../fuzz_utils.h"

namespace OHOS {
namespace AudioStandard {
using namespace std;

static const uint8_t *RAW_DATA = nullptr;
static size_t g_dataSize = 0;
static size_t g_pos;
const size_t FUZZ_INPUT_SIZE_THRESHOLD = 10;
const uint8_t TESTSIZE = 5;

typedef void (*TestFuncs)();

vector<DeviceType> DeviceTypeVec = {
    DEVICE_TYPE_NONE,
    DEVICE_TYPE_INVALID,
    DEVICE_TYPE_EARPIECE,
    DEVICE_TYPE_SPEAKER,
    DEVICE_TYPE_WIRED_HEADSET,
    DEVICE_TYPE_WIRED_HEADPHONES,
    DEVICE_TYPE_BLUETOOTH_SCO,
    DEVICE_TYPE_BLUETOOTH_A2DP,
    DEVICE_TYPE_BLUETOOTH_A2DP_IN,
    DEVICE_TYPE_MIC,
    DEVICE_TYPE_WAKEUP,
    DEVICE_TYPE_USB_HEADSET,
    DEVICE_TYPE_DP,
    DEVICE_TYPE_REMOTE_CAST,
    DEVICE_TYPE_USB_DEVICE,
    DEVICE_TYPE_ACCESSORY,
    DEVICE_TYPE_REMOTE_DAUDIO,
    DEVICE_TYPE_HDMI,
    DEVICE_TYPE_LINE_DIGITAL,
    DEVICE_TYPE_NEARLINK,
    DEVICE_TYPE_NEARLINK_IN,
    DEVICE_TYPE_FILE_SINK,
    DEVICE_TYPE_FILE_SOURCE,
    DEVICE_TYPE_EXTERN_CABLE,
    DEVICE_TYPE_DEFAULT,
    DEVICE_TYPE_USB_ARM_HEADSET,
    DEVICE_TYPE_MAX,
};

void MicrophoneDescriptor1FuzzTest()
{
    int32_t id = GetData<int32_t>(g_dataSize, g_pos, RAW_DATA);
    uint32_t deviceTypeCount = GetData<uint32_t>(g_dataSize, g_pos, RAW_DATA) % DeviceTypeVec.size();
    DeviceType deviceType = DeviceTypeVec[deviceTypeCount];
    int32_t groupId = GetData<int32_t>(g_dataSize, g_pos, RAW_DATA);
    int32_t sensitivity = GetData<int32_t>(g_dataSize, g_pos, RAW_DATA);
    MicrophoneDescriptor microphoneDescriptor(id, deviceType, groupId, sensitivity);
}

void MicrophoneDescriptor2FuzzTest()
{
    sptr<MicrophoneDescriptor> micDesc = new (std::nothrow) MicrophoneDescriptor();
    if (micDesc == nullptr) {
        return;
    }
    micDesc->micId_ = GetData<int32_t>(g_dataSize, g_pos, RAW_DATA);
    MicrophoneDescriptor microphoneDescriptor(micDesc);
}

void OperatorFuzzTest()
{
    int32_t id = GetData<int32_t>(g_dataSize, g_pos, RAW_DATA);
    uint32_t deviceTypeCount = GetData<uint32_t>(g_dataSize, g_pos, RAW_DATA) % DeviceTypeVec.size();
    DeviceType deviceType = DeviceTypeVec[deviceTypeCount];
    int32_t groupId = GetData<int32_t>(g_dataSize, g_pos, RAW_DATA);
    int32_t sensitivity = GetData<int32_t>(g_dataSize, g_pos, RAW_DATA);
    MicrophoneDescriptor microphoneDescriptor(id, deviceType, groupId, sensitivity);
    MicrophoneDescriptor microphoneDescriptor2(microphoneDescriptor);
}

void MarshallingFuzzTest()
{
    int32_t id = GetData<int32_t>(g_dataSize, g_pos, RAW_DATA);
    uint32_t deviceTypeCount = GetData<uint32_t>(g_dataSize, g_pos, RAW_DATA) % DeviceTypeVec.size();
    DeviceType deviceType = DeviceTypeVec[deviceTypeCount];
    int32_t groupId = GetData<int32_t>(g_dataSize, g_pos, RAW_DATA);
    int32_t sensitivity = GetData<int32_t>(g_dataSize, g_pos, RAW_DATA);
    MicrophoneDescriptor microphoneDescriptor(id, deviceType, groupId, sensitivity);
    Parcel parcel;
    microphoneDescriptor.Marshalling(parcel);
}

void UnmarshallingFuzzTest()
{
    int32_t id = GetData<int32_t>(g_dataSize, g_pos, RAW_DATA);
    uint32_t deviceTypeCount = GetData<uint32_t>(g_dataSize, g_pos, RAW_DATA) % DeviceTypeVec.size();
    DeviceType deviceType = DeviceTypeVec[deviceTypeCount];
    int32_t groupId = GetData<int32_t>(g_dataSize, g_pos, RAW_DATA);
    int32_t sensitivity = GetData<int32_t>(g_dataSize, g_pos, RAW_DATA);
    MicrophoneDescriptor microphoneDescriptor(id, deviceType, groupId, sensitivity);
    Parcel parcel;
    microphoneDescriptor.Marshalling(parcel);
    auto micDescShared = std::shared_ptr<MicrophoneDescriptor>(MicrophoneDescriptor::Unmarshalling(parcel));
}

TestFuncs g_testFuncs[TESTSIZE] = {
    MicrophoneDescriptor1FuzzTest,
    MicrophoneDescriptor2FuzzTest,
    OperatorFuzzTest,
    MarshallingFuzzTest,
    UnmarshallingFuzzTest,
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
