/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
#include "audio_concurrency_parser.h"

namespace OHOS {
namespace AudioStandard {

int32_t AudioConcurrencyParser::LoadConfig(std::map<std::pair<AudioPipeType, AudioPipeType>,
    ConcurrencyAction> &concurrencyMap)
{
    audioXmlNode_->Config(AUDIO_CONCURRENCY_CONFIG_FILE, nullptr, 0);
    if (!audioXmlNode_->CompareName("audioConcurrencyPolicy")) {
        AUDIO_ERR_LOG("Missing tag - audioConcurrencyPolicy");
        audioXmlNode_->FreeDoc();
        return ERR_OPERATION_FAILED;
    }
    ParseInternal(concurrencyMap);
    return SUCCESS;
}

void AudioConcurrencyParser::ParseInternal(std::map<std::pair<AudioPipeType, AudioPipeType>,
    ConcurrencyAction> &concurrencyMap)
{
    for (; audioXmlNode_->IsNodeValid(); audioXmlNode_->MoveToNext()) {
        if (audioXmlNode_->CompareName("existingStream")) {
            std::string existingStream;
            CHECK_AND_RETURN_LOG(audioXmlNode_->GetProp("name", existingStream), "GetProp name fail!");
            AUDIO_DEBUG_LOG("existingStream: %{public}s", existingStream.c_str());
            audioXmlNode_->MoveToChildren();
            ParseIncoming(existingStream, concurrencyMap);
        } else {
            audioXmlNode_->MoveToChildren();
            ParseInternal(concurrencyMap);
        }
    }
    return;
}

void AudioConcurrencyParser::ParseIncoming(const std::string &existing,
    std::map<std::pair<AudioPipeType, AudioPipeType>, ConcurrencyAction> &concurrencyMap)
{
    while (audioXmlNode_->IsNodeValid()) {
        if (audioXmlNode_->CompareName"incomingStream") {
            std::string incoming;
            std::string action;
            CHECK_AND_RETURN_LOG(audioXmlNode_->GetProp("name", incoming), "getprop name fail!");
            CHECK_AND_RETURN_LOG(audioXmlNode_->GetProp("action", action), "getprop action fail!");
            AUDIO_DEBUG_LOG("existing: %{public}s %{public}d, incoming: %{public}s %{public}d, action: %{public}s",
                existing.c_str(), audioPipeTypeMap_[existing], incoming.c_str(),
                audioPipeTypeMap_[incoming], action.c_str());
            std::pair<AudioPipeType, AudioPipeType> concurrencyPair =
                std::make_pair(audioPipeTypeMap_[existing], audioPipeTypeMap_[incoming]);
            ConcurrencyAction concurrencyAction = (action == "play both" || action == "mix") ? PLAY_BOTH :
                (action == "concede existing" ? CONCEDE_EXISTING : CONCEDE_INCOMING);
            concurrencyMap.emplace(concurrencyPair, concurrencyAction);
        }
        audioXmlNode_->MoveToNext();
    }
}
} // namespace AudioStandard
} // namespace OHOS