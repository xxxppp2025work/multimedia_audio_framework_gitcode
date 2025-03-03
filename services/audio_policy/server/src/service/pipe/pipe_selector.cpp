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
#define LOG_TAG "PipeSelector"
#endif

#include "pipe_selector.h"
#include "audio_stream_collector.h"
#include <algorithm>

namespace OHOS {
namespace AudioStandard {

static std::map<int, AudioPipeType> flagPipeTypeMap_ = {
    {AUDIO_OUTPUT_FLAG_NORMAL,        PIPE_TYPE_NORMAL_OUT},
    {AUDIO_INPUT_FLAG_NORMAL,         PIPE_TYPE_NORMAL_IN},
    {AUDIO_OUTPUT_FLAG_LOWLATENCY,    PIPE_TYPE_NORMAL_OUT},    //LOWLATEENCY对应NORMAL还是对应LOWLATENCY？其他不在表中的FLAG如何对应？
    {AUDIO_INPUT_FLAG_LOWLATENCY,     PIPE_TYPE_NORMAL_IN},
    {AUDIO_OUTPUT_FLAG_MMAP,          PIPE_TYPE_LOWLATENCY_OUT},
    {AUDIO_INPUT_FLAG_MMAP,           PIPE_TYPE_LOWLATENCY_IN},
    {AUDIO_OUTPUT_FLAG_OFFLOAD,       PIPE_TYPE_OFFLOAD},
    {AUDIO_OUTPUT_FLAG_MULTI_CHANNEL, PIPE_TYPE_MULTICHANNEL},
    {AUDIO_OUTPUT_FLAG_DIRECT,        PIPE_TYPE_DIERCT_MUSIC},
    {AUDIO_INPUT_FLAG_VOICE,          PIPE_TYPE_CALL_IN},
    {AUDIO_OUTPUT_FLAG_VOICE,         PIPE_TYPE_CALL_OUT},
};

//新增流时调用，通过新增流信息获取对应pipe信息，遍历现存pipelist，根据并发策略表确定每个pipe最终状态，返回修改后的pipelist
//返回的信息包含原始pipe信息和修改后pipe信息
//FetchPipeAndExecute   FetchPipe？？？ 此处是否应该只实现获取pipe功能，打开在主流程中实现
std::vector<std::shared_ptr<AudioPipeInfo>> PipeSelector::FetchPipeAndExecute(
    std::shared_ptr<AudioStreamDescriptor> &streamDesc)
{
    std::vector<std::shared_ptr<AudioPipeInfo>> pipeList = PipeManager::GetPipeManager().GetPipeList();
    ScanPipeListForStreamDesc(pipeList, streamDesc);

    streamDesc->streamAction_ = STREAM_ACTION_NEW;
    PipeStreamPropInfo streamPropInfo = {};
    configManager_->GetStreamPropInfo(streamDesc, streamPropInfo);
    for (auto &it : pipeList) {
        if (it->adapterName_ == streamPropInfo.pipeInfo_->adapterInfo_->GetAdapterName() &&
            it->routeFlag_ == streamDesc->routeFlag_) {
            it->streamDescs_.push_back(streamDesc);
            it->streamDescMap_[streamDesc->sessionId_] = streamDesc;
            it->action_ = PIPE_ACTION_UPDATE;
            return pipeList;
        }
    }
    AudioPipeInfo info = {};
    ConvertStreamDescToPipeInfo(streamDesc, streamPropInfo, info);
    info.action_ = PIPE_ACTION_NEW;
    pipeList.push_back(std::make_shared<AudioPipeInfo>(info));
    return pipeList;
}

//更新流时使用，比如设备更新，下发所有流信息，AudioStreamDescriptor中包含时间信息，以此排序，生成pipelist中修改后信息，并通过流信息查询现存pipelist，获取原始pipe信息
//停流也走这个接口？？？
std::vector<std::shared_ptr<AudioPipeInfo>> PipeSelector::FetchPipesAndExecute(
    std::vector<std::shared_ptr<AudioStreamDescriptor>> &streamDescs)
{
    std::vector<std::shared_ptr<AudioPipeInfo>> pipeList = PipeManager::GetPipeManager().GetPipeList();
    std::vector<std::shared_ptr<AudioPipeInfo>> newPipeList;
    for (auto it : pipeList) {
        it->streamDescs_.clear();
        it->streamDescMap_.clear();
        newPipeList.push_back(it);
    }

    SortStreamDescsByStartTime(streamDescs);
    for (auto &streamDesc : streamDescs) {
        std::string adapterName = GetAdapterNameByStreamDesc(streamDesc);
        ScanPipeListForStreamDesc(newPipeList, streamDesc);
        streamDesc->streamAction_ = STREAM_ACTION_NEW;
        for (auto &it : newPipeList) {
            if (it->adapterName_ == adapterName && it->routeFlag_ == streamDesc->routeFlag_) {
                it->streamDescs_.push_back(streamDesc);
                it->streamDescMap_[streamDesc->sessionId_] = streamDesc;
                it->action_ = PIPE_ACTION_UPDATE;
            }
        }
    }

    return newPipeList;
}

void PipeSelector::ScanPipeListForStreamDesc(std::vector<std::shared_ptr<AudioPipeInfo>> &pipeList,
    std::shared_ptr<AudioStreamDescriptor> streamDesc)
{
    streamDesc->routeFlag_ = GetRouteFlagByStreamDesc(streamDesc);
    for (auto &it : pipeList) {
        bool isUpdate = false;
        for (auto streamIt = it->streamDescs_.begin(); streamIt!= it->streamDescs_.end();) {
            isUpdate = ProcessConcurrency(*streamIt, streamDesc);
            if ((*streamIt)->streamAction_ == STREAM_ACTION_DEFAULT) {
                streamIt++;
                continue;
            }
            for (auto &newPipe : pipeList) {
                if (newPipe->adapterName_ == it->adapterName_ && newPipe->routeFlag_ == (*streamIt)->routeFlag_) {
                    newPipe->streamDescs_.push_back(*streamIt);
                    newPipe->streamDescMap_[(*streamIt)->sessionId_] = *streamIt;
                    streamIt = it->streamDescs_.erase(streamIt);
                    break;
                }
            }
        }
        it->action_ = isUpdate ? PIPE_ACTION_UPDATE : PIPE_ACTION_DEFAULT;
    }
}

bool PipeSelector::ProcessConcurrency(std::shared_ptr<AudioStreamDescriptor> stream,
    std::shared_ptr<AudioStreamDescriptor> cmpStream)
{
    bool isUpdate = false;
    std::map<std::pair<AudioPipeType, AudioPipeType>, ConcurrencyAction> ruleMap =
        AudioStreamCollector::GetAudioStreamCollector().GetConcurrencyMap();
    ConcurrencyAction action = ruleMap[std::make_pair(flagPipeTypeMap_[stream->routeFlag_],
        flagPipeTypeMap_[cmpStream->routeFlag_])];
    AudioFlag newFlag;
    switch (action) {
        case PLAY_BOTH:
            stream->streamAction_ = STREAM_ACTION_DEFAULT;
            break;
        case CONCEDE_INCOMING: // 新增流降级为primary流
            stream->streamAction_ = STREAM_ACTION_DEFAULT;
            cmpStream->routeFlag_ = cmpStream->audioMode_ == AUDIO_MODE_PLAYBACK ?
                AUDIO_OUTPUT_FLAG_NORMAL : AUDIO_INPUT_FLAG_NORMAL; // 降级后flag，未考虑不是normal的情况
            break;
        case CONCEDE_EXISTING: // existing流降级为primary流
            isUpdate = true;
            newFlag = stream->audioMode_ == AUDIO_MODE_PLAYBACK ?
                AUDIO_OUTPUT_FLAG_NORMAL : AUDIO_INPUT_FLAG_NORMAL; // 降级后flag，未考虑不是normal的情况
            stream->streamAction_ = JudgeStreamAction(stream->routeFlag_, newFlag);
            stream->routeFlag_ = newFlag;
            break;
        default:
            break;
    }
    return isUpdate;
}

AudioFlag PipeSelector::GetRouteFlagByStreamDesc(std::shared_ptr<AudioStreamDescriptor> streamDesc)
{
    AudioFlag flag = AUDIO_OUTPUT_FLAG_NONE;
    CHECK_AND_RETURN_RET_LOG(streamDesc != nullptr, flag, "streamDesc is nullptr");
    flag = configManager_->GetRouteFlag(streamDesc);
    return flag;
}

std::string PipeSelector::GetAdapterNameByStreamDesc(std::shared_ptr<AudioStreamDescriptor> streamDesc)
{
    std::string name = "";
    CHECK_AND_RETURN_RET_LOG(streamDesc != nullptr, name, "streamDesc is nullptr");
    PipeStreamPropInfo streamPropInfo = {};
    configManager_->GetStreamPropInfo(streamDesc, streamPropInfo);
    name = streamPropInfo.pipeInfo_->adapterInfo_->GetAdapterName();
    return name;
}

void PipeSelector::ConvertStreamDescToPipeInfo(std::shared_ptr<AudioStreamDescriptor> streamDesc,
    const PipeStreamPropInfo streamPropInfo, AudioPipeInfo &info)
{
    // xml解析后保存枚举类型，AudioModuleInfo中对应变量是否要修改？
    info.moduleInfo_.format = AudioDefinitionPolicyUtils::enumToFormatStr[streamPropInfo.format_];
    info.moduleInfo_.rate = std::to_string(streamPropInfo.sampleRate_);
    info.moduleInfo_.channels = std::to_string(AudioDefinitionPolicyUtils::ConvertLayoutToAudioChannel(
        streamPropInfo.channelLayout_));
    info.moduleInfo_.bufferSize = std::to_string(streamPropInfo.bufferSize_);

    info.moduleInfo_.lib = streamPropInfo.pipeInfo_->paProp_.lib_;
    info.moduleInfo_.role = streamPropInfo.pipeInfo_->paProp_.paPropRole_;
    info.moduleInfo_.name = streamPropInfo.pipeInfo_->paProp_.moduleName_;

    // className和adapterName是一个值，是否可以去掉一个？
    // file_io时className和adapterName不一致？
    info.moduleInfo_.adapterName = streamPropInfo.pipeInfo_->adapterInfo_->GetAdapterName();
    info.moduleInfo_.deviceType = std::to_string(streamDesc->newDeviceDescs_[0]->deviceType_);
    // AudioModuleInfo中的networkId是和newDeviceDesc中的一致吗？
    info.moduleInfo_.networkId = streamDesc->newDeviceDescs_[0]->networkId_;

    info.streamDescs_.push_back(streamDesc);
    info.streamDescMap_[streamDesc->sessionId_] = streamDesc;
}

AudioStreamAction PipeSelector::JudgeStreamAction(AudioFlag oldFlag, AudioFlag newFlag)
{
    if (oldFlag == AUDIO_OUTPUT_FLAG_FAST || newFlag == AUDIO_OUTPUT_FLAG_FAST ||
        oldFlag == AUDIO_OUTPUT_FLAG_DIRECT || newFlag == AUDIO_OUTPUT_FLAG_DIRECT) {
            return STREAM_ACTION_RECREATE;
    } else {
        return STREAM_ACTION_MOVE;
    }
}

void PipeSelector::SortStreamDescsByStartTime(std::vector<std::shared_ptr<AudioStreamDescriptor>> &streamDescs)
{
    sort(streamDescs.begin(), streamDescs.end(), [](const std::shared_ptr<AudioStreamDescriptor> &streamDesc1,
        const std::shared_ptr<AudioStreamDescriptor> &streamDesc2) {
            return streamDesc1->startTimeStamp_ < streamDesc2->startTimeStamp_;
        });
}

} // namespace AudioStandard
} // namespace OHOS
