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
#ifndef LOG_TAG
#define LOG_TAG "audioAffinityParser"
#endif

#include "audio_affinity_parser.h"
#include "audio_errors.h"
#include "media_monitor_manager.h"
#include "audio_utils.h"

namespace OHOS {
namespace AudioStandard {

static std::map<std::string, DeviceType> deviceTypeMap_ = {
    {"DEVICE_TYPE_EARPIECE", DEVICE_TYPE_EARPIECE},
    {"DEVICE_TYPE_SPEAKER", DEVICE_TYPE_SPEAKER},
    {"DEVICE_TYPE_WIRED_HEADSET", DEVICE_TYPE_WIRED_HEADSET},
    {"DEVICE_TYPE_WIRED_HEADPHONES", DEVICE_TYPE_WIRED_HEADPHONES},
    {"DEVICE_TYPE_BLUETOOTH_SCO", DEVICE_TYPE_BLUETOOTH_SCO},
    {"DEVICE_TYPE_BLUETOOTH_A2DP", DEVICE_TYPE_BLUETOOTH_A2DP},
    {"DEVICE_TYPE_BLUETOOTH_A2DP_IN", DEVICE_TYPE_BLUETOOTH_A2DP_IN},
    {"DEVICE_TYPE_USB_HEADSET", DEVICE_TYPE_USB_HEADSET},
    {"DEVICE_TYPE_USB_ARM_HEADSET", DEVICE_TYPE_USB_ARM_HEADSET},
    {"DEVICE_TYPE_DP", DEVICE_TYPE_DP},
    {"DEVICE_TYPE_REMOTE_CAST", DEVICE_TYPE_REMOTE_CAST},
    {"DEVICE_TYPE_MIC", DEVICE_TYPE_MIC},
};

bool audioAffinityParser::Parse()
{
    bool ret = audioXmlNode_->Config(AFFINITY_CONFIG_FILE, nullptr, 0);
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, false, "audioAffinityParser xmlReadFile failed");

    audioXmlNode_->MoveToChildren();
    CHECK_AND_RETURN_LOG(audioXmlNode_->IsNodeValid(), "audioAffinityParser Missing node");
    if (!ParseInternal()) {
        return false;
    }
    CHECK_AND_RETURN_RET_LOG(audioAffinityManager_ != nullptr, false, "audioAffinityManager_ is null");
    audioAffinityManager_->OnXmlParsingCompleted(affinityDeviceInfoArray_);
    return true;
}

void audioAffinityParser::Destroy()
{
    audioXmlNode_->FreeDoc();
}

bool audioAffinityParser::ParseInternal()
{
    while (audioXmlNode_->IsNodeValid()) {
        if (audioXmlNode_->CompareName("OutputDevices")) {
            ParserAffinityGroups(OUTPUT_DEVICES_FLAG);
        } else if (audioXmlNode_->CompareName("InputDevices")) {
            ParserAffinityGroups(INPUT_DEVICES_FLAG);
        }
        audioXmlNode_->MoveToNext();
    }
    return true;
}

void audioAffinityParser::ParserAffinityGroups(const DeviceFlag& deviceFlag)
{
    audioXmlNode_->MoveToChildren();
    CHECK_AND_RETURN_LOG(audioXmlNode_->IsNodeValid(), "audioAffinityParser Missing node groups");

    while (audioXmlNode_->IsNodeValid()) {
        if (audioXmlNode_->CompareName("AffinityGroups")) {
            ParserAffinityGroupAttribute(deviceFlag);
        }
        audioXmlNode_->MoveToNext();
    }
}

void audioAffinityParser::ParserAffinityGroupAttribute(const DeviceFlag& deviceFlag)
{
    audioXmlNode_->MoveToChildren();
    CHECK_AND_RETURN_LOG(audioXmlNode_->IsNodeValid(), "audioAffinityParser Missing node attr");

    AffinityDeviceInfo deviceInfo = {};
    deviceInfo.deviceFlag = deviceFlag;
    while (audioXmlNode_->IsNodeValid()) {
        if (audioXmlNode_->CompareName("AffinityGroup")) {

            const char *attrPrimary = audioXmlNode_->GetProp("isPrimary");
            if (attrPrimary != nullptr) {
                CHECK_AND_RETURN_LOG(StringConverter<uint32_t>(attrPrimary, deviceInfo.isPrimary), "convert attrPrimary fail!");
                audioXmlNode_->FreeProp(attrPrimary);
            }
            const char *attrGroupName = audioXmlNode_->GetProp("name");
            if (attrGroupName != nullptr) {
                deviceInfo.groupName = attrGroupName;
                audioXmlNode_->FreeProp(attrGroupName);
            }
            ParserAffinityGroupDeviceInfos(deviceInfo);
        }
        audioXmlNode_->MoveToNext();
    }
}

void audioAffinityParser::ParserAffinityGroupDeviceInfos(AffinityDeviceInfo& deviceInfo)
{
    audioXmlNode_->MoveToChildren();
    CHECK_AND_RETURN_LOG(audioXmlNode_->IsNodeValid(), "audioAffinityParser Missing node device");

    while (audioXmlNode_->IsNodeValid()) {
        if (audioXmlNode_->CompareName("Affinity")) {
            deviceInfo.networkID = audioXmlNode_->GetProp(networkId);

            std::map<std::string, DeviceType>::iterator item = deviceTypeMap_.find(audioXmlNode_->GetProp("deviceType"));
            deviceInfo.deviceType = (item != deviceTypeMap_.end() ? item->second : DEVICE_TYPE_INVALID);

            deviceInfo.SupportedConcurrency = (audioXmlNode_->GetProp("supportedConcurrency") == "True") ? true : false;

            affinityDeviceInfoArray_.push_back(deviceInfo);
        }
        audioXmlNode_->MoveToNext();
    }
}

} // namespace AudioStandard
} // namespace OHOS
