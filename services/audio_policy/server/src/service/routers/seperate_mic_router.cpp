/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
#ifndef LOG_TAG
#define LOG_TAG "SeperateMicRouter"
#endif

#include "seperate_mic_router.h"
#include "audio_log.h"
#include "audio_device_manager.h"
#include "audio_policy_service.h"

using namespace std;

namespace {
const string ROUTER_CLASS_NAME = "seperate_mic_router";
} // !unnamed namespace

namespace OHOS {
namespace AudioStandard {

#define DEVICE_MGR AudioDeviceManager::GetAudioDeviceManager()

SeperateMicRouter::SeperateMicRouter()
{
    AUDIO_INFO_LOG("SeperateMicRouter::SeperateMicRouter()");
}

SeperateMicRouter::~SeperateMicRouter()
{
    AUDIO_INFO_LOG("SeperateMicRouter::~SeperateMicRouter()");
}

string SeperateMicRouter::GetClassName()
{
    return ROUTER_CLASS_NAME;
}

RouterType SeperateMicRouter::GetRouterType()
{
    return ROUTER_TYPE_SEPERATE_MIC;
}

shared_ptr<AudioDeviceDescriptor> SeperateMicRouter::GetMediaRenderDevice(StreamUsage streamUsage, int32_t clientUID)
{
    return make_shared<AudioDeviceDescriptor>();
}

shared_ptr<AudioDeviceDescriptor> SeperateMicRouter::GetCallRenderDevice(StreamUsage streamUsage, int32_t clientUID)
{
    return make_shared<AudioDeviceDescriptor>();
}

shared_ptr<AudioDeviceDescriptor> SeperateMicRouter::GetCallCaptureDevice(SourceType sourceType, int32_t clientUID)
{
    // VOIP scene
    vector<shared_ptr<AudioDeviceDescriptor>> callDevices = DEVICE_MGR.GetAvailableDevicesByUsage(CALL_INPUT_DEVICES);
    if (callDevices.empty()) {
        AUDIO_INFO_LOG("no call devices");
        return make_shared<AudioDeviceDescriptor>();
    }
    // sort call input Device by connect time
    sort(callDevices.begin(), callDevices.end(), [](shared_ptr<AudioDeviceDescriptor>& desc1,
        shared_ptr<AudioDeviceDescriptor>& desc2) {
        return desc1->connectTimeStamp_ > desc2->connectTimeStamp_;
     });

    for (auto micDevice : callDevices) {
        if (micDevice->deviceType_ == DEVICE_TYPE_BLUETOOTH_A2DP_IN) {
            AUDIO_INFO_LOG("call scene not support a2dpin BT device");
            continue;
        }
        if (micDevice->pairDeviceDescriptor_ == nullptr) {
            return micDevice;
        }      
    }
    AUDIO_INFO_LOG("no seperate mic, return native mic");
    return make_shared<AudioDeviceDescriptor>();
}

vector<shared_ptr<AudioDeviceDescriptor>> SeperateMicRouter::GetRingRenderDevices(StreamUsage streamUsage, int32_t clientUID)
{
    AUDIO_INFO_LOG("enter");
    vector<shared_ptr<AudioDeviceDescriptor>> descs;
    return descs;
}

shared_ptr<AudioDeviceDescriptor> SeperateMicRouter::GetRecordCaptureDevice(SourceType sourceType, int32_t clientUID)
{
    // Non-VOIP scene
    vector<shared_ptr<AudioDeviceDescriptor>> recordDevices = DEVICE_MGR.GetAvailableDevicesByUsage(MEDIA_INPUT_DEVICES);
    if (recordDevices.empty()) {
        AUDIO_INFO_LOG("no call devices");
        return make_shared<AudioDeviceDescriptor>();
    }
    // sort call input Device by connect time
    sort(recordDevices.begin(), recordDevices.end(), [](shared_ptr<AudioDeviceDescriptor>& desc1,
        shared_ptr<AudioDeviceDescriptor>& desc2) {
        return desc1->connectTimeStamp_ > desc2->connectTimeStamp_;
    });
    for (auto micDevice : recordDevices) {
        if (micDevice->pairDeviceDescriptor_ == nullptr) {
            return micDevice;
        }      
    }
    AUDIO_INFO_LOG("no seperate mic, return native mic");
    return make_shared<AudioDeviceDescriptor>();
}

shared_ptr<AudioDeviceDescriptor> SeperateMicRouter::GetToneRenderDevice(StreamUsage streamUsage, int32_t clientUID)
{
    return make_shared<AudioDeviceDescriptor>();
}

} // !namespace AudioStandard
} // !namespace OHOS