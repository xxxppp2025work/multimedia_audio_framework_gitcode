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
#include "audio_resource_service.h"
#include "../fuzz_utils.h"

namespace OHOS {
namespace AudioStandard {
using namespace std;

FuzzUtils &g_fuzzUtils = FuzzUtils::GetInstance();
const std::u16string FORMMGR_INTERFACE_TOKEN = u"IAudioPolicy";
const int32_t TEST_RTG_ID = 2;
const int32_t NUM_1 = 1;
const int32_t NUM_2 = 2;
const int32_t NUM_3 = 3;

typedef void (*TestFuncs)();

class RemoteObjectTestStub : public IRemoteObject {
public:
    RemoteObjectTestStub() : IRemoteObject(u"IRemoteObject") {}
    int32_t GetObjectRefCount() { return 0; };
    int SendRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option) { return 0; };
    bool AddDeathRecipient(const sptr<DeathRecipient> &recipient) { return true; };
    bool RemoveDeathRecipient(const sptr<DeathRecipient> &recipient) { return true; };
    int Dump(int fd, const std::vector<std::u16string> &args) { return 0; };
};

class AudioWorkgroupCallbackForMonitorTest : public AudioWorkgroupCallbackForMonitor {
public:
    AudioWorkgroupCallbackForMonitorTest() = default;
    ~AudioWorkgroupCallbackForMonitorTest() override
    {
        AUDIO_INFO_LOG("AudioWorkgroupCallbackForMonitorTest destroyed");
    }
    void OnWorkgroupChange(const AudioWorkgroupChangeInfo &info) override {}
};

void ResourceServiceCreateAudioWorkgroupFuzzTest()
{
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    MessageParcel data;
    data.WriteInterfaceToken(FORMMGR_INTERFACE_TOKEN);
    sptr<IRemoteObject> remoteObject = data.ReadRemoteObject();
    if (remoteObject == nullptr) {
        return;
    }
    auto audioResourceService = AudioResourceService::GetInstance();
    if (audioResourceService == nullptr) {
        return;
    }
    audioResourceService->CreateAudioWorkgroup(pid, remoteObject);
}

void ResourceServiceAudioWorkgroupDeathRecipientFuzzTest()
{
    AudioResourceService::AudioWorkgroupDeathRecipient deathRecipient;
    std::function<void()> diedCb = []() {
    };
    deathRecipient.diedCb_ = diedCb;
    deathRecipient.OnRemoteDied(nullptr);
}

void ResourceServiceSetNotifyCbFuzzTest()
{
    AudioResourceService::AudioWorkgroupDeathRecipient deathRecipient;
    std::function<void()> func = []() {
    };

    deathRecipient.diedCb_ = func;
    deathRecipient.SetNotifyCb(func);
}

void ResourceServiceOnWorkgroupRemoteDiedFuzzTest()
{
    std::shared_ptr<AudioWorkgroup> workgroup = std::make_shared<AudioWorkgroup>(TEST_RTG_ID);
    sptr<IRemoteObject> remoteObj = nullptr;
    auto audioResourceService = AudioResourceService::GetInstance();
    if (audioResourceService == nullptr) {
        return;
    }
    audioResourceService->audioWorkgroupMap_[1].groups[TEST_RTG_ID] = {workgroup};
    audioResourceService->OnWorkgroupRemoteDied(workgroup, remoteObj);
}

void ResourceServiceReleaseWorkgroupDeathRecipientFuzzTest()
{
    std::shared_ptr<AudioWorkgroup> workgroup = std::make_shared<AudioWorkgroup>(TEST_RTG_ID);
    CHECK_AND_RETURN(workgroup != nullptr);
    sptr<IRemoteObject> remoteObj = new RemoteObjectTestStub();
    CHECK_AND_RETURN(remoteObj != nullptr);
    AudioResourceService::GetInstance()->deathRecipientMap_[workgroup] =
        std::make_pair(remoteObj, new AudioResourceService::AudioWorkgroupDeathRecipient());
    AudioResourceService::GetInstance()->ReleaseWorkgroupDeathRecipient(workgroup, remoteObj);
}

void ResourceServiceWorkgroupRendererMonitorFuzzTest()
{
    std::shared_ptr<AudioWorkgroup> workgroup = std::make_shared<AudioWorkgroup>(TEST_RTG_ID);
    if (!workgroup) {
        return;
    }
    workgroup->callback = std::make_shared<AudioWorkgroupCallbackForMonitorTest>();
    if (!workgroup->callback) {
        return;
    }
    auto audioResourceService = AudioResourceService::GetInstance();
    int32_t testPid = g_fuzzUtils.GetData<int32_t>();
    if (audioResourceService == nullptr) {
        return;
    }
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    audioResourceService->audioWorkgroupMap_[testPid].permission = g_fuzzUtils.GetData<bool>();
    audioResourceService->audioWorkgroupMap_[testPid].groups[pid] = {workgroup};
    audioResourceService->WorkgroupRendererMonitor(testPid, true);
}

void ResourceServiceDumpAudioWorkgroupMapFuzzTest()
{
    std::shared_ptr<AudioWorkgroup> workgroup = std::make_shared<AudioWorkgroup>(TEST_RTG_ID);
    if (!workgroup) {
        return;
    }
    auto audioResourceService = AudioResourceService::GetInstance();
    if (audioResourceService == nullptr) {
        return;
    }
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    audioResourceService->audioWorkgroupMap_[0].groups[pid] = {workgroup};
    audioResourceService->DumpAudioWorkgroupMap();
}

void AudioWorkgroupCheckFuzzTest()
{
    std::shared_ptr<AudioWorkgroup> workgroup = std::make_shared<AudioWorkgroup>(TEST_RTG_ID);
    if (!workgroup) {
        return;
    }
    auto audioResourceService = AudioResourceService::GetInstance();
    if (audioResourceService == nullptr) {
        return;
    }
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    audioResourceService->audioWorkgroupMap_[pid].groups[TEST_RTG_ID] = {workgroup};
    audioResourceService->AudioWorkgroupCheck(pid);
    pid = g_fuzzUtils.GetData<int32_t>() + 1;
    audioResourceService->AudioWorkgroupCheck(pid);
}

void CreateAudioWorkgroupFuzzTest()
{
    auto audioResourceService = AudioResourceService::GetInstance();
    if (audioResourceService == nullptr) {
        return;
    }
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    sptr<IRemoteObject> object = new RemoteObjectTestStub();
    if (object == nullptr) {
        return;
    }
    audioResourceService->CreateAudioWorkgroup(pid, object);
}

void ReleaseAudioWorkgroupFuzzTest()
{
    std::shared_ptr<AudioWorkgroup> workgroup = std::make_shared<AudioWorkgroup>(TEST_RTG_ID);
    if (!workgroup) {
        return;
    }
    auto audioResourceService = AudioResourceService::GetInstance();
    if (audioResourceService == nullptr) {
        return;
    }
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    int32_t workgroupId = g_fuzzUtils.GetData<int32_t>();
    audioResourceService->audioWorkgroupMap_[pid].groups[workgroupId] = {workgroup};
    audioResourceService->ReleaseAudioWorkgroup(pid, workgroupId);
}

void AddThreadToGroupFuzzTest()
{
    std::shared_ptr<AudioWorkgroup> workgroup1 = std::make_shared<AudioWorkgroup>(TEST_RTG_ID);
    std::shared_ptr<AudioWorkgroup> workgroup2 = std::make_shared<AudioWorkgroup>(TEST_RTG_ID);
    std::shared_ptr<AudioWorkgroup> workgroup3 = std::make_shared<AudioWorkgroup>(TEST_RTG_ID);
    std::shared_ptr<AudioWorkgroup> workgroup4 = std::make_shared<AudioWorkgroup>(TEST_RTG_ID);
    if (!workgroup1 || !workgroup2 || !workgroup3 || !workgroup4) {
        return;
    }
    auto audioResourceService = AudioResourceService::GetInstance();
    if (audioResourceService == nullptr) {
        return;
    }
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    int32_t workgroupId = g_fuzzUtils.GetData<int32_t>();
    int32_t tokenId = g_fuzzUtils.GetData<int32_t>();
    audioResourceService->audioWorkgroupMap_[pid].groups[workgroupId] = {workgroup1};
    audioResourceService->audioWorkgroupMap_[pid].groups[workgroupId + NUM_1] = {workgroup2};
    audioResourceService->audioWorkgroupMap_[pid].groups[workgroupId + NUM_2] = {workgroup3};
    audioResourceService->audioWorkgroupMap_[pid].groups[workgroupId + NUM_3] = {workgroup4};
    audioResourceService->AddThreadToGroup(pid, workgroupId, tokenId);
    audioResourceService->RemoveThreadFromGroup(pid, workgroupId, tokenId);
}

void StartGroupFuzzTest()
{
    std::shared_ptr<AudioWorkgroup> workgroup = std::make_shared<AudioWorkgroup>(TEST_RTG_ID);
    if (!workgroup) {
        return;
    }
    auto audioResourceService = AudioResourceService::GetInstance();
    if (audioResourceService == nullptr) {
        return;
    }
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    int32_t workgroupId = g_fuzzUtils.GetData<int32_t>();
    uint64_t startTime = g_fuzzUtils.GetData<uint64_t>();
    uint64_t deadlineTime = g_fuzzUtils.GetData<uint64_t>();
    audioResourceService->audioWorkgroupMap_[pid].groups[workgroupId] = {workgroup};
    audioResourceService->StartGroup(pid, workgroupId, startTime, deadlineTime);
    audioResourceService->StopGroup(pid, workgroupId);
}

void GetThreadsNumPerProcessFuzzTest()
{
    std::shared_ptr<AudioWorkgroup> workgroup = std::make_shared<AudioWorkgroup>(TEST_RTG_ID);
    if (!workgroup) {
        return;
    }
    auto audioResourceService = AudioResourceService::GetInstance();
    if (audioResourceService == nullptr) {
        return;
    }
    int32_t pid = g_fuzzUtils.GetData<int32_t>();
    int32_t workgroupId = g_fuzzUtils.GetData<int32_t>();
    audioResourceService->audioWorkgroupMap_[pid].groups[workgroupId] = {workgroup};
    audioResourceService->GetThreadsNumPerProcess(pid);
}

vector<TestFuncs> g_testFuncs = {
    ResourceServiceCreateAudioWorkgroupFuzzTest,
    ResourceServiceAudioWorkgroupDeathRecipientFuzzTest,
    ResourceServiceSetNotifyCbFuzzTest,
    ResourceServiceOnWorkgroupRemoteDiedFuzzTest,
    ResourceServiceReleaseWorkgroupDeathRecipientFuzzTest,
    ResourceServiceWorkgroupRendererMonitorFuzzTest,
    ResourceServiceDumpAudioWorkgroupMapFuzzTest,
    AudioWorkgroupCheckFuzzTest,
    CreateAudioWorkgroupFuzzTest,
    ReleaseAudioWorkgroupFuzzTest,
    AddThreadToGroupFuzzTest,
    StartGroupFuzzTest,
    GetThreadsNumPerProcessFuzzTest,
};

} // namespace AudioStandard
} // namesapce OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    OHOS::AudioStandard::g_fuzzUtils.fuzzTest(data, size, OHOS::AudioStandard::g_testFuncs);
    return 0;
}
