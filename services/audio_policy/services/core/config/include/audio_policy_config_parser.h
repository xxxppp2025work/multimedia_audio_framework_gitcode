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
#ifndef AUDIO_POLICY_PARSER_H
#define AUDIO_POLICY_PARSER_H

#include <list>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <unordered_map>
#include <string>
#include <regex>

#include "audio_adapter_info.h"
#include "audio_device_info.h"
#include "audio_stream_info.h"
#include "iport_observer.h"
#include "parser.h"

namespace OHOS {
namespace AudioStandard {
class AudioPolicyParser : public Parser {
public:
    static constexpr char CHIP_PROD_CONFIG_FILE[] = "/chip_prod/etc/audio/audio_policy_config.xml";
    static constexpr char CONFIG_FILE[] = "/vendor/etc/audio/audio_policy_config.xml";

    bool LoadConfiguration() final;
    bool Parse() final;
    void Destroy() final;

    explicit AudioPolicyParser(IPortObserver &observer)
        : portObserver_(observer),
          doc_(nullptr)
    {
    }

    virtual ~AudioPolicyParser()
    {
        Destroy();
    }

private:
    PolicyXmlNodeType GetXmlNodeTypeAsInt(xmlNode &node);
    AdapterInfoType GetAdapterInfoTypeAsInt(xmlNode &node);
    PipeInfoType GetPipeInfoTypeAsInt(xmlNode &node);
    std::string ExtractPropertyValue(const std::string &propName, xmlNode &node);

    bool ParseInternal(xmlNode &node);
    void ParseAdapters(xmlNode &node);
    void ParseAdapter(xmlNode &node);
    void ParsePipes(xmlNode &node, AudioAdapterInfo &adapterInfo);
    void ParsePipeInfos(xmlNode &node, AdapterPipeInfo &pipeInfo);
    void ParsePaProp(xmlNode &node, AdapterPipeInfo &pipeInfo);
    void ParseStreamProps(xmlNode &node, AdapterPipeInfo &pipeInfo);
    void ParseAttributes(xmlNode &node, AdapterPipeInfo &pipeInfo);
    void ParseAttributeByName(AttributeInfo &attributeInfo, AdapterPipeInfo &pipeInfo);
    void ParseDevices(xmlNode &node, AudioAdapterInfo &adapterInfo);
    void SplitStringToList(std::string &str, std::list<std::string> &result);

    IPortObserver &portObserver_;
    xmlDoc *doc_;
};
} // namespace AudioStandard
} // namespace OHOS

#endif // AUDIO_POLICY_PARSER_H