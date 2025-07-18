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
const size_t THRESHOLD = 10;
typedef void (*TestPtr)();

using AudioZoneFocusList = std::list<std::pair<AudioInterrupt, AudioFocuState>>;

const vector<StreamUsage> g_testStreamUsages = {
    STREAM_USAGE_INVALID,
    STREAM_USAGE_UNKNOWN,
    STREAM_USAGE_MEDIA,
    STREAM_USAGE_MUSIC,
    STREAM_USAGE_VOICE_COMMUNICATION,
    STREAM_USAGE_VOICE_ASSISTANT,
    STREAM_USAGE_ALARM,
    STREAM_USAGE_VOICE_MESSAGE,
    STREAM_USAGE_NOTIFICATION_RINGTONE,
    STREAM_USAGE_RINGTONE,
    STREAM_USAGE_NOTIFICATION,
    STREAM_USAGE_ACCESSIBILITY,
    STREAM_USAGE_SYSTEM,
    STREAM_USAGE_MOVIE,
    STREAM_USAGE_GAME,
    STREAM_USAGE_AUDIOBOOK,
    STREAM_USAGE_NAVIGATION,
    STREAM_USAGE_DTMF,
    STREAM_USAGE_ENFORCED_TONE,
    STREAM_USAGE_ULTRASONIC,
    STREAM_USAGE_VIDEO_COMMUNICATION,
    STREAM_USAGE_RANGING,
    STREAM_USAGE_VOICE_MODEM_COMMUNICATION,
    STREAM_USAGE_VOICE_RINGTONE,
    STREAM_USAGE_VOICE_CALL_ASSISTANT,
    STREAM_USAGE_MAX,
};

const vector<RouterType> g_testRouterTypes = {
    ROUTER_TYPE_NONE,
    ROUTER_TYPE_DEFAULT,
    ROUTER_TYPE_STREAM_FILTER,
    ROUTER_TYPE_PACKAGE_FILTER,
    ROUTER_TYPE_COCKPIT_PHONE,
    ROUTER_TYPE_PRIVACY_PRIORITY,
    ROUTER_TYPE_PUBLIC_PRIORITY,
    ROUTER_TYPE_PAIR_DEVICE,
    ROUTER_TYPE_USER_SELECT,
    ROUTER_TYPE_APP_SELECT,
};

const vector<SourceType> g_testSourceTypes = {
    SOURCE_TYPE_INVALID,
    SOURCE_TYPE_MIC,
    SOURCE_TYPE_VOICE_RECOGNITION,
    SOURCE_TYPE_PLAYBACK_CAPTURE,
    SOURCE_TYPE_WAKEUP,
    SOURCE_TYPE_VOICE_CALL,
    SOURCE_TYPE_VOICE_COMMUNICATION,
    SOURCE_TYPE_ULTRASONIC,
    SOURCE_TYPE_VIRTUAL_CAPTURE,
    SOURCE_TYPE_VOICE_MESSAGE,
    SOURCE_TYPE_REMOTE_CAST,
    SOURCE_TYPE_VOICE_TRANSCRIPTION,
    SOURCE_TYPE_CAMCORDER,
    SOURCE_TYPE_UNPROCESSED,
    SOURCE_TYPE_EC,
    SOURCE_TYPE_MIC_REF,
    SOURCE_TYPE_LIVE,
    SOURCE_TYPE_MAX,
};

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

void AudioZoneServiceCreateAudioZoneFuzzTest()
{   
    AudioZoneService& audioZoneService = AudioZoneService::GetInstance();

    std::string name = "testZone";
    AudioZoneContext context;
    std::shared_ptr<AudioClientManager>manager = std::make_shared<AudioZoneClientManager>(nullptr);
    int32_t zoneId = GetDaTa<int32_t>();
    std::shared_ptr<AudioZone>zone = std::make_shared<AudioZone>(manager,name,context);
    audioZoneService.zoneMaps_.insert(make_pair(zoneId,zone));
    audioZoneService.CreateAudioZone(name,context);
}

void AudioZoneServiceReleaseAudioZoneFuzzTest()   
{   
    AudioZoneService& audioZoneService = AudioZoneService::GetInstance();

    AudioZoneService.DeInit();
    AudioZoneService.Init(DelaySingleton<AudioPolicyServerHandler>Handler>::GetInstance();
    std::make_shared<AudioInterruptService>());
    std::string name = "testZone";
    AudioZoneContext context;
    std::shared_ptr<AudioClientManager>manager = std::make_shared<AudioZoneClientManager>(nullptr);
    int32_t zoneId = GetDaTa<int32_t>();
    std::shared_ptr<AudioZone>zone = std::make_shared<AudioZone>(manager,name,context);
    audioZoneService.zoneMaps_.insert(make_pair(zoneId,zone));
    audioZoneService.ReleaseAudioZone(zoneId);
}

void AudioZoneServiceGetAllAudioZoneFuzzTest()   
{   
    AudioZoneService& audioZoneService = AudioZoneService::GetInstance();

    AudioZoneService.DeInit();
    AudioZoneService.Init(DelaySingleton<AudioPolicyServerHandler>Handler>::GetInstance();
    std::make_shared<AudioInterruptService>());
    std::string name = "testZone";
    AudioZoneContext context;
    std::shared_ptr<AudioClientManager>manager = std::make_shared<AudioZoneClientManager>(nullptr);
    int32_t zoneId = GetDaTa<int32_t>();
    std::shared_ptr<AudioZone>zone = std::make_shared<AudioZone>(manager,name,context);
    audioZoneService.zoneMaps_.insert(make_pair(zoneId,zone));
    audioZoneService.GetAllAudioZone();
}

void AudioZoneServiceGetAllAudioZoneFuzzTest()
{   
    AudioZoneService& audioZoneService = AudioZoneService::GetInstance();

    AudioZoneService.DeInit();
    AudioZoneService.Init(DelaySingleton<AudioPolicyServerHandler>Handler>::GetInstance();
    std::make_shared<AudioInterruptService>());
    std::string name = "testZone";
    AudioZoneContext context;
    std::shared_ptr<AudioClientManager>manager = std::make_shared<AudioZoneClientManager>(nullptr);
    int32_t zoneId = GetDaTa<int32_t>();
    std::shared_ptr<AudioZone>zone = std::make_shared<AudioZone>(manager,name,context);
    audioZoneService.zoneMaps_.insert(make_pair(zoneId,zone));
    std::shared_ptr<AudioDeviceDescriptor>audioDeviceDescriptor = make_shared<AudioDeviceDescriptor>();
    std::vector<std::shared_ptr<AudioDeviceDescriptor>>device;
    device.push_back(audioDeviceDescriptor);
    audioZoneService.BindDeviceToAudioZone(zoneId,device);
}

void AudioZoneServiceRemoveDeviceFromGlobalFuzzTest()
{   
    AudioZoneService& audioZoneService = AudioZoneService::GetInstance();

    AudioZoneService.DeInit();
    AudioZoneService.Init(DelaySingleton<AudioPolicyServerHandler>Handler>::GetInstance();
    std::make_shared<AudioInterruptService>());
    std::shared_ptr<AudioDeviceDescriptor>audioDeviceDescriptor = make_shared<AudioDeviceDescriptor>();
    audioZoneService.RemoveDeviceFromGlobal(AudioDeviceDescriptor);
}

void AudioZoneServiceUnBindDeviceToAudioZoneFuzzTest()
{   
    AudioZoneService& audioZoneService = AudioZoneService::GetInstance();

    AudioZoneService.DeInit();
    AudioZoneService.Init(DelaySingleton<AudioPolicyServerHandler>Handler>::GetInstance();
    std::make_shared<AudioInterruptService>());
    AudioZoneContext context;
    std::string name = "testZone";
    std::shared_ptr<AudioClientManager>manager = std::make_shared<AudioZoneClientManager>(nullptr);
    int32_t zoneId = GetDaTa<int32_t>();
    std::shared_ptr<AudioZone>zone = std::make_shared<AudioZone>(manager,name,context);
    audioZoneService.zoneMaps_.insert(make_pair(zoneId,zone));
    std::shared_ptr<AudioDeviceDescriptor>audioDeviceDescriptor = make_shared<AudioDeviceDescriptor>();
    std::vector<std::shared_ptr<AudioDeviceDescriptor>>device;
    device.push_back(audioDeviceDescriptor);
    audioZoneService.UnBindDeviceToAudioZone(zoneId,device);
}

void AudioZoneServiceRegisterAudioZoneClientFuzzTest()
{   
    AudioZoneService& audioZoneService = AudioZoneService::GetInstance();

    AudioZoneService.DeInit();
    AudioZoneService.Init(DelaySingleton<AudioPolicyServerHandler>Handler>::GetInstance();
    std::make_shared<AudioInterruptService>());
    int32_t ClientPid = GetDaTa<int32_t>();
    sptr<IStandarAudioZoneClient> client = nullptr;
    audioZoneService.RegisterAudioZoneClient(ClientPid,client);
}

void AudioZoneServiceUnRegisterAudioZoneClientFuzzTest()
{   
    AudioZoneService& audioZoneService = AudioZoneService::GetInstance();

    AudioZoneService.DeInit();
    AudioZoneService.Init(DelaySingleton<AudioPolicyServerHandler>Handler>::GetInstance();
    std::make_shared<AudioInterruptService>());
    AudioZoneContext context;
    std::string name = "testZone";
    std::shared_ptr<AudioClientManager>manager = std::make_shared<AudioZoneClientManager>(nullptr);
    int32_t zoneId = GetDaTa<int32_t>();
    std::shared_ptr<AudioZone>zone = std::make_shared<AudioZone>(manager,name,context);
    audioZoneService.zoneMaps_.insert(make_pair(zoneId,zone));
    int32_t ClientPid = GetDaTa<int32_t>();
    audioZoneService.UnRegisterAudioZoneClient(ClientPid);
}

void AudioZoneServiceInjectInterruptToAudioZoneFuzzTest()
{   
    AudioZoneService& audioZoneService = AudioZoneService::GetInstance();

    AudioZoneService.DeInit();
    AudioZoneService.Init(DelaySingleton<AudioPolicyServerHandler>Handler>::GetInstance();
    std::make_shared<AudioInterruptService>());
    AudioZoneContext context;
    std::string name = "testZone";
    std::shared_ptr<AudioClientManager>manager = std::make_shared<AudioZoneClientManager>(nullptr);
    int32_t zoneId = GetDaTa<int32_t>();
    std::shared_ptr<AudioZone>zone = std::make_shared<AudioZone>(manager,name,context);
    audioZoneService.zoneMaps_.insert(make_pair(zoneId,zone));
    AudioZoneFouseList interrupts = AudioZoneService.GetAudioInterruptForZone(zoneId);
    audioZoneService.InjectInterruptToAudioZone(zoneId,interrupts);
}

void AudioZoneServiceFetchOutputDeviceFuzzTest()
{   
    AudioZoneService& audioZoneService = AudioZoneService::GetInstance();

    AudioZoneService.DeInit();
    AudioZoneService.Init(DelaySingleton<AudioPolicyServerHandler>Handler>::GetInstance();
    std::make_shared<AudioInterruptService>());
    AudioZoneContext context;
    std::string name = "testZone";
    std::shared_ptr<AudioClientManager>manager = std::make_shared<AudioZoneClientManager>(nullptr);
    int32_t zoneId = GetDaTa<int32_t>();
    std::shared_ptr<AudioZone>zone = std::make_shared<AudioZone>(manager,name,context);
    audioZoneService.zoneMaps_.insert(make_pair(zoneId,zone));
    if(g_testStreamUsage.size() == 0 || g_testRouter Type.size() == 0){
        return;
    }
    StreamUsage streamUsage = g_testStreamUsage [GetData<int32_t>() % g_testStreamUsage.size()];
    int32_t ClientPid = GetDaTa<int32_t>();
    RouterType bypassType = g_testRouter [GetData<int32_t>() % g_testRouter.size()];
    audioZoneService.FetchOutputDevice(zoneId,streamUsage,ClientPid,bypassType);
}

void AudioZoneServiceFetchInputDeviceFuzzTest()
{   
    AudioZoneService& audioZoneService = AudioZoneService::GetInstance();

    AudioZoneService.DeInit();
    AudioZoneService.Init(DelaySingleton<AudioPolicyServerHandler>Handler>::GetInstance();
    std::make_shared<AudioInterruptService>());
    AudioZoneContext context;
    std::string name = "testZone";
    std::shared_ptr<AudioClientManager>manager = std::make_shared<AudioZoneClientManager>(nullptr);
    int32_t zoneId = GetDaTa<int32_t>();
    std::shared_ptr<AudioZone>zone = std::make_shared<AudioZone>(manager,name,context);
    audioZoneService.zoneMaps_.insert(make_pair(zoneId,zone));
    if(g_testSourceTypes.size() == 0){
        return;
    }
    SourceTypes sourceTypes =  g_testSourceTypes [GetData<int32_t>() % g_testSourceTypes.size()];
    int32_t ClientPid = GetDaTa<int32_t>();
    audioZoneService.FetchInputDevice(zoneId,sourceTypes,ClientPid);
}

void AudioZoneServiceGetZoneStringDeviceDescriptorFuzzTest()
{   
    AudioZoneService& audioZoneService = AudioZoneService::GetInstance();

    AudioZoneService.DeInit();
    AudioZoneService.Init(DelaySingleton<AudioPolicyServerHandler>Handler>::GetInstance();
    std::make_shared<AudioInterruptService>());
    AudioZoneContext context;
    std::string name = "testZone";
    std::shared_ptr<AudioClientManager>manager = std::make_shared<AudioZoneClientManager>(nullptr);
    int32_t zoneId = GetDaTa<int32_t>();
    std::shared_ptr<AudioZone>zone = std::make_shared<AudioZone>(manager,name,context);
    audioZoneService.zoneMaps_.insert(make_pair(zoneId,zone));
    audioZoneService.GetZoneStringDeviceDescriptor(zoneId);
}

void AudioZoneUpdataDeviceFromGlobalForAllZoneFuzzTest()
{   
    AudioZoneService& audioZoneService = AudioZoneService::GetInstance();

    AudioZoneService.DeInit();
    AudioZoneService.Init(DelaySingleton<AudioPolicyServerHandler>Handler>::GetInstance();
    std::make_shared<AudioInterruptService>());
    AudioZoneContext context;
    std::string name = "testZone";
    std::shared_ptr<AudioClientManager>manager = std::make_shared<AudioZoneClientManager>(nullptr);
    int32_t zoneId = GetDaTa<int32_t>();
    std::shared_ptr<AudioZone>zone = std::make_shared<AudioZone>(manager,name,context);
    audioZoneService.zoneMaps_.insert(make_pair(zoneId,zone));
    std::shared_ptr<AudioDeviceDescriptor>audioDeviceDescriptor = make_shared<AudioDeviceDescriptor>();
    audioZoneService.UpdataDeviceFromGlobalForAllZone(device);
}

TestPtr g_testPtrs[] = {
    AudioZoneServiceCreateAudioZoneFuzzTest,
    AudioZoneServiceReleaseAudioZoneFuzzTest,
    AudioZoneServiceGetAllAudioZoneFuzzTest,
    AudioZoneServiceGetAllAudioZoneFuzzTest,
    AudioZoneServiceRemoveDeviceFromGlobalFuzzTest,
    AudioZoneServiceUnBindDeviceToAudioZoneFuzzTest,
    AudioZoneServiceRegisterAudioZoneClientFuzzTest,
    AudioZoneServiceUnRegisterAudioZoneClientFuzzTest,
    AudioZoneServiceInjectInterruptToAudioZoneFuzzTest,
    AudioZoneServiceFetchOutputDeviceFuzzTest,
    AudioZoneServiceFetchInputDeviceFuzzTest,
    AudioZoneServiceGetZoneStringDeviceDescriptorFuzzTest,
    AudioZoneUpdataDeviceFromGlobalForAllZoneFuzzTest,
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