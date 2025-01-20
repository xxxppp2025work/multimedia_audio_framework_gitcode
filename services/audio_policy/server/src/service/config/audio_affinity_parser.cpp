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

bool audioAffinityParser::LoadConfiguration()
{
    bool ret = audioXmlNode_->Config(AFFINITY_CONFIG_FILE, nullptr, 0);
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, false, "audioAffinityParser xmlReadFile failed");

    audioXmlNode_->MoveToChildren();
    CHECK_AND_RETURN_LOG(audioXmlNode_->IsNodeValid(), "audioAffinityParser Missing node");
    if (!ParseInternal(audioXmlNode_)) {
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

bool audioAffinityParser::ParseInternal(std::shared_ptr<AudioXmlNode> &curNode)
{
    while (curNode->IsNodeValid()) {
        if (curNode->CompareName("OutputDevices")) {
            ParserAffinityGroups(curNode, OUTPUT_DEVICES_FLAG);
        } else if (curNode->CompareName("InputDevices")) {
            ParserAffinityGroups(curNode, INPUT_DEVICES_FLAG);
        }
        curNode->MoveToNext();
    }
    return true;
}

void audioAffinityParser::ParserAffinityGroups(std::shared_ptr<AudioXmlNode> &curNode, const DeviceFlag& deviceFlag)
{
    curNode->MoveToChildren();
    CHECK_AND_RETURN_LOG(curNode->IsNodeValid(), "audioAffinityParser Missing node groups");

    while (curNode->IsNodeValid()) {
        if (curNode->CompareName("AffinityGroups")) {
            ParserAffinityGroupAttribute(curNode, deviceFlag);
        }
        curNode->MoveToNext();
    }
}

void audioAffinityParser::ParserAffinityGroupAttribute(std::shared_ptr<AudioXmlNode> &curNode,
    const DeviceFlag& deviceFlag)
{
    curNode->MoveToChildren();
    CHECK_AND_RETURN_LOG(curNode->IsNodeValid(), "audioAffinityParser Missing node attr");

    AffinityDeviceInfo deviceInfo = {};
    deviceInfo.deviceFlag = deviceFlag;
    while (curNode->IsNodeValid()) {
        if (curNode->CompareName("AffinityGroup")) {
            std::string attrPrimary;
            if (curNode->GetProp("isPrimary", attrPrimary) == SUCCESS) {
                CHECK_AND_RETURN_LOG(StringConverter<uint32_t>(attrPrimary, deviceInfo.isPrimary),
                    "convert attrPrimary fail!");
                curNode->FreeProp(attrPrimary);
            }
            std::string attrGroupName;
            if (curNode->GetProp("name", attrGroupName) == SUCCESS) {
                deviceInfo.groupName = attrGroupName;
                curNode->FreeProp(attrGroupName);
            }
            ParserAffinityGroupDeviceInfos(curNode, deviceInfo);
        }
        curNode->MoveToNext();
    }
}

void audioAffinityParser::ParserAffinityGroupDeviceInfos(std::shared_ptr<AudioXmlNode> &curNode,
    AffinityDeviceInfo& deviceInfo)
{
    curNode->MoveToChildren();
    CHECK_AND_RETURN_LOG(curNode->IsNodeValid(), "audioAffinityParser Missing node device");

    while (curNode->IsNodeValid()) {
        if (curNode->CompareName("Affinity")) {
            CHECK_AND_RETURN_LOG(curNode->GetProp("networkId", deviceInfo.networkID) == SUCCESS,
                "get prop deviceinfo.networkID failed!");

            std::string deviceType;
            CHECK_AND_RETURN_LOG(curNode->GetProp("deviceType", deviceType) == SUCCESS,
                "get prop deviceType failed!");
            std::map<std::string, DeviceType>::iterator item = deviceTypeMap_.find(deviceType);
            deviceInfo.deviceType = (item != deviceTypeMap_.end() ? item->second : DEVICE_TYPE_INVALID);

            std::string supportedConcurrency;
            CHECK_AND_RETURN_LOG(curNode->GetProp("supportedConcurrency", supportedConcurrency) == SUCCESS,
                "get prop supportedConcurrency failed!");
            deviceInfo.SupportedConcurrency = (supportedConcurrency == "True") ? true : false;
            
            affinityDeviceInfoArray_.push_back(deviceInfo);
        }
        curNode->MoveToNext();
    }
}

} // namespace AudioStandard
} // namespace OHOS
