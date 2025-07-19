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
#include "audio_zone.h"
#include "audio_zone_client_manager.h"
#include "audio_zone_service.h"

using namespace std;

namespace OHOS {
namespace AudioStandard {

static const uint8_t* RAW_DATA = nullptr;
static size_t g_dataSize = 0;
static size_t g_pos;
static size_t NUM_2;
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

void AudioZoneBindKeyAudioZoneBindKeyFuzzTest()
{   
    std::string deviceTag = "testDeviceTag";
    std::string streamTag = "testStreamTag";
    int32_t uid = GetData<int32_t>();
    AudioZoneBindKey AudioZoneBindKey_1(uid);
    AudioZoneBindKey AudioZoneBindKey_2(uid, deviceTag);
    AudioZoneBindKey AudioZoneBindKey_3(uid, deviceTag, streamTag);
    AudioZoneBindKey AudioZoneBindKey_4(AudioZoneBindKey_1);
    AudioZoneBindKey AudioZoneBindKey_5(std::move(AudioZoneBindKey_1));
}

void AudioZoneBindKeyOperatorFuzzTest()
{
    std::string deviceTag = "testDeviceTag";
    std::string streamTag = "testStreamTag";
    int32_t uid = GetData<int32_t>();
    AudioZoneBindKey AudioZoneBindKey_1(uid, deviceTag, streamTag);
    AudioZoneBindKey AudioZoneBindKey_2 = AudioZoneBindKey_1;
}

void AudioZoneBindKeyGetUidFuzzTest()
{
    int32_t uid = GetData<int32_t>();
    AudioZoneBindKey audioZoneBindKey(uid);
    AudioZoneBindKey.GetUid();
}

void AudioZoneBindKeyGetStringFuzzTest()
{   
    int32_t uid = GetData<int32_t>();
    AudioZoneBindKey audioZoneBindKey(uid);
    AudioZoneBindKey.GetString();
}

void AudioZoneBindKeyIsContainFuzzTest()
{   
    int32_t uid = GetData<int32_t>();
    AudioZoneBindKey audioZoneBindKey(uid);
    AudioZoneBindKey audioZoneBindKey1(uid);
    AudioZoneBindKey.IsContain(audioZoneBindKey1);
}

void AudioZoneBindKeyGetSupportKeysFuzzTest()
{
    int32_t uid = GetData<int32_t>();
    AudioZoneBindKey audioZoneBindKey(uid);
    AudioZoneBindKey audioZoneBindKey1(uid);
    AudioZoneBindKey.IsContain(audioZoneBindKey1);
    StreamUsage usage =
        static_cast<StreamUsage usage>(GetData<int32_t>() % StreamUsage::STREAM_USAGE_MAX);
    AudioZoneBindKey AudioZoneBindKey(uid,deviceTag, streamTag);
    AudioZoneBindKey AudioZoneBindKey_1(uid, deviceTag, streamTag);
    AudioZoneBindKey GetSupportKeys(audioZoneBindKey1);
    AudioZoneBindKey GetSupportKeys(uid,deviceTag, streamTag, usage);
}

void AudioZoneGetDescriptorFuzzTest()
{
    std::string name = "testAudioZone";
    AudioZoneContest context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    zone->GetDescriptor();
}

void AudioZoneGetStringDescriptorFuzzTest()
{
    std::string name = "testAudioZone";
    AudioZoneContest context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    zone->GetDescriptor();
}

void AudioZoneGetDescriptorNoLockFuzzTest()
{
    std::string name = "testAudioZone";
    AudioZoneContest context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    zone->GetDescriptorNoLock();
}

void AudioZoneBindKeyFuzzTest()
{
    std::string name = "testAudioZone";
    AudioZoneContest context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    int32_t uid = GetData<int32_t>();
    AudioZoneBindKey audioZoneBindKey(uid);
    zone->BindKey(audioZoneBindKey);
}

void AudioZoneRemoveKeyFuzzTest()
{
    std::string name = "testAudioZone";
    AudioZoneContest context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    int32_t uid = GetData<int32_t>();
    AudioZoneBindKey audioZoneBindKey(uid);
    zone->RemoveKey(audioZoneBindKey);
}

void AudioZoneIsContainKeyFuzzTest()
{
    std::string name = "testAudioZone";
    AudioZoneContest context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    int32_t uid = GetData<int32_t>();
    AudioZoneBindKey audioZoneBindKey(uid);
    zone->IsContainKey(audioZoneBindKey);
}

void AudioZoneAddDeviceDescriptorFuzzTest()
{
    std::string name = "testAudioZone";
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> device;
    auto device = std::make_shared<AudioDeviceDescriptor>();
    device.push_back(device);
    AudioZoneContext context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    zone->AddDeviceDescriptor(device);
}

void AudioZoneRemoveDeviceDescriptorFuzzTest()
{
    std::string name = "testAudioZone";
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> device;
    auto device = std::make_shared<AudioDeviceDescriptor>();
    device.push_back(device);
    AudioZoneContext context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    zone->RemoveDeviceDescriptor(device);
}

void AudioZoneUpdateDeviceDescriptorFuzzTest()
{
    std::string name = "testAudioZone";
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> device;
    auto device = std::make_shared<AudioDeviceDescriptor>();
    device.push_back(device);
    AudioZoneContext context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    zone->AddDeviceDescriptor(device);
    zone->UpdateDeviceDescriptor(device);
}

void AudioZoneEnableDeviceDescriptorFuzzTest()
{
    std::string name = "testAudioZone";
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> device;
    auto device = std::make_shared<AudioDeviceDescriptor>();
    device.push_back(device);
    AudioZoneContext context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    zone->AddDeviceDescriptor(device);
    zone->EnableDeviceDescriptor(device);
}

void AudioZoneDisableDeviceDescriptorStateFuzzTest()
{
    std::string name = "testAudioZone";
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> device;
    auto device = std::make_shared<AudioDeviceDescriptor>();
    device.push_back(device);
    AudioZoneContext context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    zone->AddDeviceDescriptor(device);
    zone->DisableDeviceDescriptorState(device);
}

void AudioZoneIsDeviceConnectFuzzTest()
{
    std::string name = "testAudioZone";
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> device;
    auto device = std::make_shared<AudioDeviceDescriptor>();
    device.push_back(device);
    AudioZoneContext context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    zone->AddDeviceDescriptor(device);
    zone->IsDeviceConnectFuzz(device);
}

void AudioZoneFetchOutputDevicesFuzzTest()
{   
    std::string name = "testAudioZone";
    StreamUsage usage =
        static_cast<StreamUsage>(GetData<int32_t>() % StreamUsage::STREAM_USAGE_MAX);
    int32_t clientUid = GetData<int32_t>();
    RouterType bypassType = GetData<RouterType>();
    AudioZoneContext context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    zone->FetchOutputDevices(StreamUsage, clientUid, bypassType);
}

void AudioZoneFetchInputDevicesFuzzTest()
{
    std::string name = "testAudioZone";
    SourceType sourceType =
        static_cast<SourceType>(GetData<int32_t>() % SourceType::SOURCE_TYPE_MAX);
    int32_t clientUid = GetData<int32_t>();
    AudioZoneContext context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    zone->FetchOutputDevices(sourceType, clientUid);
}

void AudioZoneEnableChangeReportFuzzTest()
{
    std::string name = "testAudioZone";
    bool enable = static_cast<bool>(GetData<int32_t>() %NUM_2);
    pid_t clientPid = GetData<pid_t>();
    AudioZoneContext context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    zone->EnableChangeReport(clientPid ,enable);
}

void AudioZoneEnableSystemVolumeProxyFuzzTest()
{
    std::string name = "testAudioZone";
    bool enable =  static_cast<bool>(GetData<int32_t>() %NUM_2);
    pid_t clientPid = GetData<pid_t>();
    AudioZoneContext context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    zone->EnableChangeReport(clientPid, enable);
}

void AudioZoneSetSystemVolumeLevelFuzzTest() 
{
    std::string name = "testAudioZone";
    AudioVolumeType volumeProxyClientPid = GetData<AudioVolumeType>();
    int32_t VolumeLevel = GetData<int32_t>();
    int32_t VolumeFlag = GetData<int32_t>();
    AudioZoneContext context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    zone->SetSystemVolumeLevel(volumeProxyClientPid, VolumeLevel, VolumeLevel);
}

void AudioZoneGetSystemVolumeLevelFuzzTest()
{
    std::string name = "testAudioZone";
    AudioVolumeType volumeProxyClientPid = GetData<AudioVolumeType>();
    AudioZoneContext context;
    std::shared_ptr<AudioZoneClientManager> zoneClientManager;
    std::shared_ptr<AudioZone> zone =
        std::make_shard<AudioZone>(zoneClientManager, name, context);
    zone->GetSystemVolumeLevel(volumeProxyClientPid);
}

TestPtr g_testPtrs[] = {
    AudioZoneBindKeyAudioZoneBindKeyFuzzTest,
    AudioZoneBindKeyOperatorFuzzTest,
    AudioZoneBindKeyGetUidFuzzTest,
    AudioZoneBindKeyGetStringFuzzTest,
    AudioZoneBindKeyIsContainFuzzTest,
    AudioZoneBindKeyGetSupportKeysFuzzTest,
    AudioZoneGetDescriptorFuzzTest,
    AudioZoneGetStringDescriptorFuzzTest,
    AudioZoneGetDescriptorNoLockFuzzTest,
    AudioZoneBindKeyFuzzTest,
    AudioZoneRemoveKeyFuzzTest,
    AudioZoneIsContainKeyFuzzTest,
    AudioZoneAddDeviceDescriptorFuzzTest,
    AudioZoneRemoveDeviceDescriptorFuzzTest,
    AudioZoneUpdateDeviceDescriptorFuzzTest,
    AudioZoneEnableDeviceDescriptorFuzzTest,
    AudioZoneDisableDeviceDescriptorStateFuzzTest,
    AudioZoneIsDeviceConnectFuzzTest,
    AudioZoneFetchOutputDevicesFuzzTest,
    AudioZoneFetchInputDevicesFuzzTest,
    AudioZoneEnableChangeReportFuzzTest,
    AudioZoneEnableSystemVolumeProxyFuzzTest,
    AudioZoneSetSystemVolumeLevelFuzzTest,
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