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
#define LOG_TAG "AudioPolicyParser"
#endif

#include "audio_policy_parser.h"
#include "audio_policy_utils.h"
#include <sstream>

namespace OHOS {
namespace AudioStandard {
bool AudioPolicyParser::LoadConfiguration()
{
    AUDIO_INFO_LOG("Enter");
    doc_ = xmlReadFile(CHIP_PROD_CONFIG_FILE, nullptr, 0);
    if (doc_ == nullptr) {
        doc_ = xmlReadFile(CONFIG_FILE, nullptr, 0);
        if (doc_ == nullptr) {
            AUDIO_ERR_LOG("XmlReadFile failed");
            return false;
        }
    }
    AUDIO_INFO_LOG("Done");
    return true;
}

// LCOV_EXCL_START
bool AudioPolicyParser::Parse()
{
    AUDIO_INFO_LOG("Enter");
    xmlNode *root = xmlDocGetRootElement(doc_);
    if (root == nullptr) {
        AUDIO_ERR_LOG("XmlDocGetRootElement failed");
        return false;
    }
    if (!xmlStrcmp(root->name, reinterpret_cast<const xmlChar*>("audioPolicyConfiguration"))) {
        AudioPolicyConfigData config = AudioPolicyConfigData::GetInstance();
        config.SetVersion(ExtractPropertyValue("version", *root));

    }
    if (!ParseInternal(*root)) {
        AUDIO_ERR_LOG("Audio policy config xml parse failed");
        return false;
    }

    portObserver_.OnAudioPolicyXmlParsingCompleted();
    AUDIO_INFO_LOG("Done");
    return true;
}

void AudioPolicyParser::Destroy()
{
    if (doc_ != nullptr) {
        xmlFreeDoc(doc_);
    }
}

bool AudioPolicyParser::ParseInternal(xmlNode &node)
{
    xmlNode *currNode = &node;
    for (; currNode; currNode = currNode->next) {
        if (XML_ELEMENT_NODE == currNode->type) {
            switch (GetXmlNodeTypeAsInt(*currNode)) {
                case PolicyXmlNodeType::ADAPTERS:
                    ParseAdapters(*currNode);
                    break;
                default:
                    ParseInternal(*(currNode->children));
                    break;
            }
        }
    }
    return true;
}

void AudioPolicyParser::ParseAdapters(xmlNode &node)
{
    xmlNode *currNode = node.xmlChildrenNode;

    while (currNode != nullptr) {
        if (currNode->type == XML_ELEMENT_NODE) {
            ParseAdapter(*currNode);
        }
        currNode = currNode->next;
    }
}

void AudioPolicyParser::ParseAdapter(xmlNode &node)
{
    std::string adapterName = ExtractPropertyValue("name", node);
    if (adapterName.empty()) {
        AUDIO_ERR_LOG("No name provided for the adapter class %{public}s", node.name);
        return;
    }

    AudioAdapterInfo adapterInfo = {};
    adapterInfo.SetAdapterName(adapterName);
    adapterInfo.SetAdapterSupportScene(ExtractPropertyValue("supportScene", node));

    xmlNode *currNode = node.xmlChildrenNode;
    while (currNode != nullptr) {
        if (currNode->type == XML_ELEMENT_NODE) {
            switch (GetAdapterInfoTypeAsInt(*currNode)) {
                case AdapterInfoType::PIPES:
                    ParsePipes(*currNode, adapterInfo);
                    break;
                case AdapterInfoType::DEVICES:
                    ParseDevices(*currNode, adapterInfo);
                    break;
                default:
                    ParseAdapter(*(currNode->children));
                    break;
            }
        }
        currNode = currNode->next;
    }
    AudioPolicyConfigData config = AudioPolicyConfigData::GetInstance();
    config.AddAdapterInfoToMap(adapterInfo.GetTypeEnum(), adapterInfo);
}

void AudioPolicyParser::ParsePipes(xmlNode &node, AudioAdapterInfo &adapterInfo)
{
    xmlNode *currNode = node.xmlChildrenNode;
    std::list<AdapterPipeInfo> pipeInfos;

    while (currNode != nullptr) {
        if (currNode->type == XML_ELEMENT_NODE) {
            AdapterPipeInfo pipeInfo {};
            pipeInfo.adapterInfo_ = &adapterInfo;
            pipeInfo.name_ = ExtractPropertyValue("name", *currNode);
            std::string pipeRole = ExtractPropertyValue("role", *currNode);
            pipeInfo.pipeRole_ = AudioPolicyUtils::pipeRoleStrToEnum[pipeRole];
            ParsePipeInfos(*currNode, pipeInfo);
            pipeInfos.push_back(std::move(pipeInfo));
        }
        currNode = currNode->next;
    }
    adapterInfo.SetPipeInfos(pipeInfos);
}

void AudioPolicyParser::ParsePipeInfos(xmlNode &node, AdapterPipeInfo &pipeInfo)
{
    xmlNode *currNode = node.xmlChildrenNode;
    while (currNode != nullptr) {
        if (currNode->type == XML_ELEMENT_NODE) {
            switch (GetPipeInfoTypeAsInt(*currNode)) {
                case PipeInfoType::PA_PROP:
                    ParsePaProp(*currNode, pipeInfo);
                    break;
                case PipeInfoType::STREAM_PROP:
                    ParseStreamProps(*currNode, pipeInfo);
                    break;
                case PipeInfoType::ATTRIBUTE:
                    ParseAttributes(*currNode, pipeInfo);
                    break;
                default:
                    ParsePipeInfos(*(currNode->children), pipeInfo);
                    break;
            }
        }
        currNode = currNode->next;
    }
}

void AudioPolicyParser::ParsePaProp(xmlNode &node, AdapterPipeInfo &pipeInfo)
{
    xmlNode *currNode = node.xmlChildrenNode;
    PaPropInfo paProp {};

    if (currNode == nullptr || currNode->type != XML_ELEMENT_NODE) {
        pipeInfo.paProp_ = paProp;
        return;
    }

    paProp.lib_ = ExtractPropertyValue("lib", *currNode);
    paProp.paPropRole_ = ExtractPropertyValue("role", *currNode);
    paProp.moduleName_ = ExtractPropertyValue("moduleName", *currNode);
    pipeInfo.paProp_ = std::move(paProp);
}

void AudioPolicyParser::ParseStreamProps(xmlNode &node, AdapterPipeInfo &pipeInfo)
{
    xmlNode *currNode = node.xmlChildrenNode;
    std::list<PipeStreamPropInfo> streamPropInfos;

    while (currNode != nullptr) {
        if (currNode->type == XML_ELEMENT_NODE) {
            PipeStreamPropInfo streamPropInfo {};
            streamPropInfo.pipeInfo_ = &pipeInfo;
            std::string formatStr = ExtractPropertyValue("format", *currNode);
            streamPropInfo.format_ = AudioPolicyUtils::formatStrToEnum[formatStr];
            std::string sampleRateStr = ExtractPropertyValue("sampleRates", *currNode);
            if (sampleRateStr != "") {
                CHECK_AND_RETURN_LOG(StringConverter(sampleRateStr, streamPropInfo.sampleRate_),
                    "Convert invalid sampleRate: %{public}s", sampleRateStr.c_str());
                pipeInfo.sampleRates_.push_back(streamPropInfo.sampleRate_);
            }
            std::string channelLayoutStr = ExtractPropertyValue("channelLayout", *currNode);
            if (channelLayoutStr != "") {
                streamPropInfo.channelLayout_ = AudioPolicyUtils::layoutStrToEnum[channelLayoutStr];
                pipeInfo.channelLayouts_.push_back(streamPropInfo.channelLayout_);
            }
            std::string bufferSizeStr = ExtractPropertyValue("bufferSize", *currNode);
            if (bufferSizeStr != "") {
                CHECK_AND_RETURN_LOG(StringConverter(bufferSizeStr, streamPropInfo.bufferSize_),
                    "Convert invalid bufferSize: %{public}s", bufferSizeStr.c_str());
            }
            std::string supportDevicesStr = ExtractPropertyValue("supportDevices", *currNode);
            if (supportDevicesStr != "") {
                std::list<std::string> supportDevices {};
                SplitStringToList(supportDevicesStr, supportDevices, ",");
                for (auto device : supportDevices) {
                    streamPropInfo.supportDevices_.push_back(AudioPolicyUtils::deviceTypeStrToEnum[device]);
                }
            }

            streamPropInfos.push_back(std::move(streamPropInfo));
        }
        currNode = currNode->next;
    }
    pipeInfo.streamPropInfos_ = std::move(streamPropInfos);
}

void AudioPolicyParser::ParseAttributes(xmlNode &node, AdapterPipeInfo &pipeInfo)
{
    xmlNode *currNode = node.xmlChildrenNode;
    std::list<AttributeInfo> attributeInfos;

    while (currNode != nullptr) {
        if (currNode->type == XML_ELEMENT_NODE) {
            AttributeInfo attributeInfo = {};
            attributeInfo.name_ = ExtractPropertyValue("name", *currNode);
            attributeInfo.value_ = ExtractPropertyValue("value", *currNode);
            ParseAttributeByName(attributeInfo, pipeInfo);
            attributeInfos.push_back(std::move(attributeInfo));
        }
        currNode = currNode->next;
    }
    pipeInfo.attributeInfos_ = std::move(attributeInfos);
}

void AudioPolicyParser::ParseAttributeByName(AttributeInfo &attributeInfo, AdapterPipeInfo &pipeInfo)
{
    if (attributeInfo.name_ == "flag") {
        std::list<std::string> supportFlags {};
        SplitStringToList(attributeInfo.value_, supportFlags, "|");
        for (auto flag : supportFlags) {
            pipeInfo.supportFlags_.push_back(AudioPolicyUtils::flagStrToEnum[flag]);
        }
    } else if (attributeInfo.name_ == "preload") {
        pipeInfo.preloadAttr = AudioPolicyUtils::preloadStrToEnum[attributeInfo.value_];
    }
}

void AudioPolicyParser::ParseDevices(xmlNode &node, AudioAdapterInfo &adapterInfo)
{
    xmlNode *currNode = node.xmlChildrenNode;
    std::list<AdapterDeviceInfo> deviceInfos = {};

    while (currNode != nullptr) {
        if (currNode->type == XML_ELEMENT_NODE) {
            AdapterDeviceInfo deviceInfo {};
            deviceInfo.adapterInfo_ = &adapterInfo;
            deviceInfo.name_ = ExtractPropertyValue("name", *currNode);
            std::string type = ExtractPropertyValue("type", *currNode);
            deviceInfo.type_ = AudioPolicyUtils::deviceTypeStrToEnum[type];
            std::string pin = ExtractPropertyValue("pin", *currNode);
            deviceInfo.pin_ = AudioPolicyUtils::pinStrToEnum[pin];
            std::string role = ExtractPropertyValue("role", *currNode);
            deviceInfo.role_ = AudioPolicyUtils::deviceRoleStrToEnum[role];
            std::string supportPipeInStr = ExtractPropertyValue("supportPipes", *currNode);
            SplitStringToList(supportPipeInStr, deviceInfo.supportPipes_, ",");
            deviceInfos.push_back(std::move(deviceInfo));
        }
        currNode = currNode->next;
    }
    adapterInfo.SetDeviceInfos(deviceInfos);
}

void AudioPolicyParser::SplitStringToList(std::string &str, std::list<std::string> &result, const char *delim)
{
    char *token = std::strtok(&str[0], delim);
    while (token != nullptr) {
        result.push_back(token);
        token = std::strtok(nullptr, delim);
    }
}

PolicyXmlNodeType AudioPolicyParser::GetXmlNodeTypeAsInt(xmlNode &node)
{
    if (!xmlStrcmp(node.name, reinterpret_cast<const xmlChar*>("adapters"))) {
        return PolicyXmlNodeType::ADAPTERS;
    } else {
        return PolicyXmlNodeType::XML_UNKNOWN;
    }
}

AdapterInfoType AudioPolicyParser::GetAdapterInfoTypeAsInt(xmlNode &node)
{
    if (!xmlStrcmp(node.name, reinterpret_cast<const xmlChar*>("pipes"))) {
        return AdapterInfoType::PIPES;
    } else if (!xmlStrcmp(node.name, reinterpret_cast<const xmlChar*>("devices"))) {
        return AdapterInfoType::DEVICES;
    } else {
        return AdapterInfoType::UNKNOWN;
    }
}

PipeInfoType AudioPolicyParser::GetPipeInfoTypeAsInt(xmlNode &node)
{
    if (!xmlStrcmp(node.name, reinterpret_cast<const xmlChar*>("paProp"))) {
        return PipeInfoType::PA_PROP;
    } else if (!xmlStrcmp(node.name, reinterpret_cast<const xmlChar*>("streamProps"))) {
        return PipeInfoType::STREAM_PROP;
    } else if (!xmlStrcmp(node.name, reinterpret_cast<const xmlChar*>("attributes"))) {
        return PipeInfoType::ATTRIBUTE;
    } else {
        return PipeInfoType::UNKNOWN;
    }
}

std::string AudioPolicyParser::ExtractPropertyValue(const std::string &propName, xmlNode &node)
{
    std::string propValue = "";
    xmlChar *tempValue = nullptr;

    if (xmlHasProp(&node, reinterpret_cast<const xmlChar*>(propName.c_str()))) {
        tempValue = xmlGetProp(&node, reinterpret_cast<const xmlChar*>(propName.c_str()));
    }

    if (tempValue != nullptr) {
        propValue = reinterpret_cast<const char*>(tempValue);
        xmlFree(tempValue);
    }

    return propValue;
}

}
}