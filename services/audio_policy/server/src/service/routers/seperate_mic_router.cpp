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

 #ifndef LOG_TAG
 #define LOG_TAG "SeperateMicRouter"
 #endif
 
 #include "seperate_mic_router.h"
 #include "audio_log.h"
 #include "audio_device_manager.h"
 
 using namespace std;
 
 namespace {
 const string ROUTER_CLASS_NAME = "seperate_mic_router";
 } // unamed namespace
 
 namespace OHOS {
 namespace AudioStandard {
 
 SeperateMicRouter::SeperateMicRouter()
 {
 }
 
 SeperateMicRouter::~SeperateMicRouter()
 {
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
     AUDIO_INFO_LOG("enter");
     vector<shared_ptr<AudioDeviceDescriptor>> callCaptureDevices = 
         AudioDeviceManager::GetAudioDeviceManager().GetAvailableDevicesByUsage(CALL_INPUT_DEVICES);
     // sort call capture devices by connect time, begin from the latest connection
     SortDevicesByConnectTime(callCaptureDevices);
     AUDIO_INFO_LOG("device num: %{public}d.", callCaptureDevices.size());
     for (auto micDevice : callCaptureDevices) {
         if (micDevice != nullptr && micDevice->pairDeviceDescriptor_ == nullptr) {
             AUDIO_INFO_LOG("find seperate Mic, type: %{public}d.", micDevice->deviceType_);
             return micDevice;
         }
     }
     AUDIO_INFO_LOG("not find seperate Mic");
     return make_shared<AudioDeviceDescriptor>();
 }
 
 vector<shared_ptr<AudioDeviceDescriptor>> SeperateMicRouter::GetRingRenderDevices(StreamUsage streamUsage,
     int32_t clientUID)
 {
     vector<shared_ptr<AudioDeviceDescriptor>> descs;
     return descs;
 }
 
 shared_ptr<AudioDeviceDescriptor> SeperateMicRouter::GetRecordCaptureDevice(SourceType sourceType, int32_t clientUID)
 {
     AUDIO_INFO_LOG("enter");
     vector<shared_ptr<AudioDeviceDescriptor>> recordCaptureDevices =
         AudioDeviceManager::GetAudioDeviceManager().GetAvailableDevicesByUsage(MEDIA_INPUT_DEVICES);
     SortDevicesByConnectTime(recordCaptureDevices);
     for (auto micDevice : recordCaptureDevices) {
         if (micDevice != nullptr && micDevice->pairDeviceDescriptor_ == nullptr) {
             AUDIO_INFO_LOG("find seperate Mic, type: %{public}d.", micDevice->deviceType_);
             return micDevice;
         }
     }
     AUDIO_INFO_LOG("no seperate mic");
     return make_shared<AudioDeviceDescriptor>();
 }
 
 std::shared_ptr<AudioDeviceDescriptor> SeperateMicRouter::GetToneRenderDevice(StreamUsage streamUsage, int32_t clientUID)
 {
     return make_shared<AudioDeviceDescriptor>();
 }
 
 void SeperateMicRouter::SortDevicesByConnectTime(vector<shared_ptr<AudioDeviceDescriptor>>& descs)
 {
     AUDIO_INFO_LOG("enter");
     sort(descs.begin(), descs.end(), [](shared_ptr<AudioDeviceDescriptor>& desc1,
         shared_ptr<AudioDeviceDescriptor>& desc2) {
         if (desc1 == nullptr || desc2 == nullptr) {
             return false;
         }
         return desc1->connectTimeStamp_ > desc2->connectTimeStamp_;
     });
 }
 
 } // namespace AudioStandard
 } // namespace OHOS
 