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
#define LOG_TAG "AudioPolicyConfigParser"
#endif

#include "audio_policy_config_parser.h"
#include "audio_definition_policy_utils.h"
#include "audio_errors.h"
#include <sstream>

namespace OHOS {
namespace AudioStandard {
// LCOV_EXCL_START
bool AudioPolicyConfigParser::LoadConfiguration()
{
    AUDIO_INFO_LOG("Enter");
    if (curNode_->Config(CHIP_PROD_CONFIG_FILE, nullptr, 0) != SUCCESS) {
        if (curNode_->Config(CONFIG_FILE, nullptr, 0) != SUCCESS) {
            AUDIO_ERR_LOG("LoadConfiguration readFile failed");
            return false;
        }
    }
    
    if (!ParseInternal(curNode_->GetCopyNode())) {
        AUDIO_ERR_LOG("Audio policy config xml parse failed");
        return false;
    }

    configManager_.OnAudioPolicyXmlParsingCompleted(); // how to notify manager?
    AUDIO_INFO_LOG("Done");
    return true;
}

void AudioPolicyConfigParser::Destroy()
{
    curNode_->FreeDoc();
}

bool AudioPolicyConfigParser::ParseInternal(std::shared_ptr<AudioXmlNode> curNode)
{
    for (; curNode->IsNodeValid(); curNode->MoveToNext()) {
        if (curNode->IsElementNode()) {
            switch (GetXmlNodeTypeAsInt(curNode)) {
                case PolicyXmlNodeType::ADAPTERS:
                    ParseAdapters(curNode->GetCopyNode());
                    break;
                default:
                    ParseInternal(curNode->GetChildrenNode());
                    break;
            }
        }
    }
    return true;
}

void AudioPolicyConfigParser::ParseAdapters(std::shared_ptr<AudioXmlNode> curNode)
{
    curNode->MoveToChildren();

    while (curNode->IsNodeValid()) {
        if (curNode->IsElementNode()) {
            ParseAdapter(curNode->GetCopyNode());
        }
        curNode->MoveToNext();
    }
}

void AudioPolicyConfigParser::ParseAdapter(std::shared_ptr<AudioXmlNode> curNode)
{
    std::string adapterName;
    CHECK_AND_RETURN_LOG(curNode->GetProp("name", adapterName) == SUCCESS, "Get prop name failed");
    std::string supportScene;
    curNode->GetProp("supportSelectScene", supportScene);

    PolicyAdapterInfo adapterInfo = {};
    adapterInfo.SetAdapterName(adapterName);
    adapterInfo.SetAdapterSupportScene(supportScene);

    curNode->MoveToChildren();
    while (curNode->IsNodeValid()) {
        if (curNode->IsElementNode()) {
            switch (GetAdapterInfoTypeAsInt(curNode)) {
                case AdapterInfoType::PIPES:
                    ParsePipes(curNode->GetCopyNode(), adapterInfo);
                    break;
                case AdapterInfoType::DEVICES:
                    ParseDevices(curNode->GetCopyNode(), adapterInfo);
                    break;
                default:
                    ParseAdapter(curNode->GetChildrenNode());
                    break;
            }
        }
        curNode->MoveToNext();
    }
    AudioPolicyConfigData config = AudioPolicyConfigData::GetInstance();
    config.AddAdapterInfoToMap(adapterInfo.GetTypeEnum(), adapterInfo);
}

void AudioPolicyConfigParser::ParsePipes(std::shared_ptr<AudioXmlNode> curNode, PolicyAdapterInfo &adapterInfo)
{
    curNode->MoveToChildren();
    std::list<AdapterPipeInfo> pipeInfos;

    while (curNode->IsNodeValid()) {
        if (curNode->IsElementNode()) {
            AdapterPipeInfo pipeInfo {};
            pipeInfo.adapterInfo_ = &adapterInfo;
            curNode->GetProp("name", pipeInfo.name_);
            std::string pipeRole;
            curNode->GetProp("role", pipeRole);
            pipeInfo.pipeRole_ = AudioDefinitionPolicyUtils::pipeRoleStrToEnum[pipeRole];
            ParsePipeInfos(curNode->GetCopyNode(), pipeInfo);
            pipeInfos.push_back(std::move(pipeInfo));
        }
        currNode = currNode->next;
    }
    adapterInfo.SetPipeInfos(pipeInfos);
}

void AudioPolicyConfigParser::ParsePipeInfos(std::shared_ptr<AudioXmlNode> curNode, AdapterPipeInfo &pipeInfo)
{
    curNode->MoveToChildren();
    while (curNode->IsNodeValid()) {
        if (curNode->IsElementNode()) {
            switch (GetPipeInfoTypeAsInt(curNode)) {
                case PipeInfoType::PA_PROP:
                    ParsePaProp(curNode->GetCopyNode(), pipeInfo);
                    break;
                case PipeInfoType::STREAM_PROP:
                    ParseStreamProps(curNode->GetCopyNode(), pipeInfo);
                    break;
                case PipeInfoType::ATTRIBUTE:
                    ParseAttributes(curNode->GetCopyNode(), pipeInfo);
                    break;
                default:
                    ParsePipeInfos(curNode->GetChildrenNode(), pipeInfo);
                    break;
            }
        }
        curNode->MoveToNext();
    }
}

void AudioPolicyConfigParser::ParsePaProp(std::shared_ptr<AudioXmlNode> curNode, AdapterPipeInfo &pipeInfo)
{
    curNode->MoveToChildren();
    PaPropInfo paProp {};

    if (!curNode->IsNodeValid() || !curNode->IsElementNode()) {
        pipeInfo.paProp_ = paProp;
        return;
    }

    curNode->GetProp("lib", paProp.lib_);
    curNode->GetProp("role", paProp.paPropRole_);
    curNode->GetProp("moduleName", paProp.moduleName_);
    pipeInfo.paProp_ = std::move(paProp);
}

void AudioPolicyConfigParser::ParseStreamProps(std::shared_ptr<AudioXmlNode> curNode, AdapterPipeInfo &pipeInfo)
{
    curNode->MoveToChildren();
    std::list<PipeStreamPropInfo> streamPropInfos;

    while (curNode->IsNodeValid()) {
        if (curNode->IsElementNode()) {
            PipeStreamPropInfo streamPropInfo {};
            streamPropInfo.pipeInfo_ = &pipeInfo;
            std::string formatStr;
            curNode->GetProp("format", formatStr);
            streamPropInfo.format_ = AudioDefinitionPolicyUtils::formatStrToEnum[formatStr];
            std::string sampleRateStr;
            curNode->GetProp("sampleRates", sampleRateStr);
            StringConverter(sampleRateStr, streamPropInfo.sampleRate_);
            std::string channelLayoutStr;
            curNode->GetProp("channelLayout", channelLayoutStr);
            streamPropInfo.channelLayout_ = AudioDefinitionPolicyUtils::layoutStrToEnum[channelLayoutStr];
            std::string bufferSizeStr;
            curNode->GetProp("bufferSize", bufferSizeStr);
            StringConverter(bufferSizeStr, streamPropInfo.bufferSize_);
            std::string supportDevicesStr;
            curNode->GetProp("supportDevices", supportDevicesStr);
            if (supportDevicesStr != "") {
                std::list<std::string> supportDevices {};
                SplitStringToList(supportDevicesStr, supportDevices, ",");
                for (auto device : supportDevices) {
                    streamPropInfo.supportDevices_.push_back(AudioDefinitionPolicyUtils::deviceTypeStrToEnum[device]);
                }
            }

            streamPropInfos.push_back(std::move(streamPropInfo));
        }
        curNode->MoveToNext();
    }
    pipeInfo.streamPropInfos_ = std::move(streamPropInfos);
}

void AudioPolicyConfigParser::ParseAttributes(std::shared_ptr<AudioXmlNode> curNode, AdapterPipeInfo &pipeInfo)
{
    curNode->MoveToChildren();
    std::list<AttributeInfo> attributeInfos;

    while (curNode->IsNodeValid()) {
        if (curNode->IsElementNode()) {
            AttributeInfo attributeInfo = {};
            curNode->GetProp("name", attributeInfo.name_);
            curNode->GetProp("value", attributeInfo.value_);
            ParseAttributeByName(attributeInfo, pipeInfo);
            attributeInfos.push_back(std::move(attributeInfo));
        }
        curNode->MoveToNext();
    }
    pipeInfo.attributeInfos_ = std::move(attributeInfos);
}

void AudioPolicyConfigParser::ParseAttributeByName(AttributeInfo &attributeInfo, AdapterPipeInfo &pipeInfo)
{
    if (attributeInfo.name_ == "flag") {
        std::list<std::string> supportFlags {};
        SplitStringToList(attributeInfo.value_, supportFlags, "|");
        for (auto flag : supportFlags) {
            pipeInfo.supportFlags_.push_back(AudioDefinitionPolicyUtils::flagStrToEnum[flag]);
        }
    } else if (attributeInfo.name_ == "preload") {
        pipeInfo.preloadAttr = AudioDefinitionPolicyUtils::preloadStrToEnum[attributeInfo.value_];
    }
}

void AudioPolicyConfigParser::ParseDevices(std::shared_ptr<AudioXmlNode> curNode, PolicyAdapterInfo &adapterInfo)
{
    curNode->MoveToChildren();
    std::list<AdapterDeviceInfo> deviceInfos = {};

    while (curNode->IsNodeValid()) {
        if (curNode->IsElementNode()) {
            AdapterDeviceInfo deviceInfo {};
            deviceInfo.adapterInfo_ = &adapterInfo;
            curNode->GetProp("name", deviceInfo.name_);
            std::string type;
            curNode->GetProp("type", type);
            deviceInfo.type_ = AudioDefinitionPolicyUtils::deviceTypeStrToEnum[type];
            std::string pin;
            curNode->GetProp("pin", pin);
            deviceInfo.pin_ = AudioDefinitionPolicyUtils::pinStrToEnum[pin];
            std::string role;
            curNode->GetProp("role", role);
            deviceInfo.role_ = AudioDefinitionPolicyUtils::deviceRoleStrToEnum[role];
            std::string supportPipeInStr;
            curNode->GetProp("supportPipes", supportPipeInStr);
            SplitStringToList(supportPipeInStr, deviceInfo.supportPipes_, ",");
            deviceInfos.push_back(std::move(deviceInfo));
        }
        curNode->MoveToNext();
    }
    adapterInfo.SetDeviceInfos(deviceInfos);
}

void AudioPolicyConfigParser::SplitStringToList(std::string &str, std::list<std::string> &result, const char *delim)
{
    char *token = std::strtok(&str[0], delim);
    while (token != nullptr) {
        result.push_back(token);
        token = std::strtok(nullptr, delim);
    }
}

PolicyXmlNodeType AudioPolicyConfigParser::GetXmlNodeTypeAsInt(std::shared_ptr<AudioXmlNode> curNode)
{
    if (curNode->CompareName("adapters")) {
        return PolicyXmlNodeType::ADAPTERS;
    } else {
        return PolicyXmlNodeType::XML_UNKNOWN;
    }
}

AdapterInfoType AudioPolicyConfigParser::GetAdapterInfoTypeAsInt(std::shared_ptr<AudioXmlNode> curNode)
{
    if (curNode->CompareName("pipes")) {
        return AdapterInfoType::PIPES;
    } else if (curNode->CompareName("devices")) {
        return AdapterInfoType::DEVICES;
    } else {
        return AdapterInfoType::UNKNOWN;
    }
}

PipeInfoType AudioPolicyConfigParser::GetPipeInfoTypeAsInt(std::shared_ptr<AudioXmlNode> curNode)
{
    if (curNode->CompareName("paProp")) {
        return PipeInfoType::PA_PROP;
    } else if (curNode->CompareName("streamProps")) {
        return PipeInfoType::STREAM_PROP;
    } else if (curNode->CompareName("attributes")) {
        return PipeInfoType::ATTRIBUTE;
    } else {
        return PipeInfoType::UNKNOWN;
    }
}

}
}
