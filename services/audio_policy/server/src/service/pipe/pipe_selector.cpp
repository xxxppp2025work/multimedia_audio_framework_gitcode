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

namespace OHOS {
namespace AudioStandard {

static std::map<int, AudioPipeType> flagPipeTypeMap_ = {
    {AUDIO_OUTPUT_FLAG_NORMAL,       PIPE_TYPE_NORMAL_OUT},
    {AUDIO_INPUT_FLAG_NORMAL,        PIPE_TYPE_NORMAL_IN},
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
    std::shared_ptr<AudioStreamDescriptor> streamDesc)
{
    std::vector<std::shared_ptr<AudioPipeInfo>> pipes;
    streamDesc->routeFlag_ = GetRouteFlagByStreamDesc(streamDesc);

    std::map<std::pair<AudioPipeType, AudioPipeType>, ConcurrencyAction> ruleMap =
        AudioStreamCollector::GetAudioStreamCollector().GetConcurrencyMap();
    std::vector<AudioPipeInfo> pipeList = PipeManager::GetPipeManager().GetPipeList();

    // 遍历list，根据并发策略确定每条stream应该选择的routeFlag，并标记action
    for (auto &it : pipeList) {
        bool isUpdate = false;
        for (auto &streamIt : it.streamDescs_) {
            ConcurrencyAction action = ruleMap[std::make_pair(flagPipeTypeMap_[streamIt->streamDesc->audioFlag_],
                flagPipeTypeMap_[streamDesc->routeFlag_])];
            switch (action) {
                case PLAY_BOTH:
                    streamIt->streamAction_ = STREAM_ACTION_DEFAULT;
                    break;
                case CONCEDE_INCOMING: // 新增流降级为primary流
                    streamIt->streamAction_ = STREAM_ACTION_DEFAULT;
                    streamDesc->routeFlag_ = AUDIO_OUTPUT_FLAG_NORAML; // 降级后flag，未考虑不是normal的情况，input/output在哪判断？
                    break;
                case CONCEDE_EXISTING: // existing流降级为primary流
                    isUpdate = true;
                    int32_t newFlag = AUDIO_OUTPUT_FLAG_NORAML; // 降级后flag，未考虑不是normal的情况，input/output在哪判断？
                    streamIt->streamAction_ = JudgeStreamAction(streamIt->routeFlag_, newFlag);
                    streamIt->routeFlag_ = newFlag;
                    break;
                default:
                    break;
            }
        }
        it.aciton_ = isUpdate ? PIPE_ACTION_UPDATE : PIPE_ACTION_DEFAULT;
        pipes.push_back(std::make_shared<AudioPipeInfo>(it));
    }

    AudioPipeInfo info = {};
    int ret = GetPipeInfoByStreamDesc(streamDesc, &info);
    CHECK_AND_RETURN_RET_LOG(ret, pipes, "GetPipeInfoByStreamDesc failed");
    info->action_ = PIPE_ACTION_NEW;
    pipes.push_back(std::make_shared<AudioPipeInfo>(info));
    return pipes;
}

//更新流时使用，比如设备更新，下发所有流信息，AudioStreamDescriptor中包含时间信息，以此排序，生成pipelist中修改后信息，并通过流信息查询现存pipelist，获取原始pipe信息
//停流也走这个接口？？？
std::vector<std::shared_ptr<AudioPipeInfo>> PipeSelector::FetchPipesAndExecute(
    std::vector<std::shared_ptr<AudioStreamDescriptor>> &streamDescs)
{
    std::vector<std::shared_ptr<AudioPipeInfo>> pipes;
    std::map<std::pair<AudioPipeType, AudioPipeType>, ConcurrencyAction> ruleMap =
        AudioStreamCollector::GetAudioStreamCollector().GetConcurrencyMap();
    std::vector<AudioPipeInfo> pipeList = PipeManager::GetPipeManager().GetPipeList();
    
    // streamDescs中流信息按时间排序
    SortStreamDescsByStartTime(streamDescs);

    // 对流遍历，每条流和当前list中的流进行并发管控，最后通过流的pipe是否变化确定streamAction
    // 遍历list时，是否需要判断是否是当前在做判断的流
    // 和新增的逻辑一样，对每个pipe的streamDesc做并发
    for (auto &streamDesc : streamDescs) {
        streamDesc->routeFlag_ = GetRouteFlagByStreamDesc(streamDesc);
        for (auto &it : pipeList) {
            bool isUpdate = false;
            for (auto &streamIt : it.streamDescs_) {
                ConcurrencyAction action = ruleMap[std::make_pair(flagPipeTypeMap_[streamIt->streamDesc->audioFlag_],
                    flagPipeTypeMap_[streamDesc->routeFlag_])];
                switch (action) {
                    case PLAY_BOTH:
                        streamIt->streamAction_ = STREAM_ACTION_DEFAULT;
                        break;
                    case CONCEDE_INCOMING: // 新增流降级为primary流
                        streamIt->streamAction_ = STREAM_ACTION_DEFAULT;
                        streamDesc->routeFlag_ = AUDIO_OUTPUT_FLAG_NORAML; // 降级后flag，未考虑不是normal的情况，input/output在哪判断？
                        break;
                    case CONCEDE_EXISTING: // existing流降级为primary流
                        isUpdate = true;
                        int32_t newFlag = AUDIO_OUTPUT_FLAG_NORAML; // 降级后flag，未考虑不是normal的情况，input/output在哪判断？
                        streamIt->streamAction_ = JudgeStreamAction(streamIt->routeFlag_, newFlag);
                        streamIt->routeFlag_ = newFlag;
                        break;
                    default:
                        break;
                }
            }
            it.aciton_ = isUpdate ? PIPE_ACTION_UPDATE : PIPE_ACTION_DEFAULT;
            pipes.push_back(std::make_shared<AudioPipeInfo>(it));
        }
    }

    AudioPipeInfo info = {};
    for (auto &streamDesc : streamDescs) {
        int ret = GetPipeInfoByStreamDesc(streamDesc, &info);
        CHECK_AND_RETURN_RET_LOG(ret, pipes, "GetPipeInfoByStreamDesc failed");
        info->action_ = PIPE_ACTION_NEW;
        pipes.push_back(std::make_shared<AudioPipeInfo>(info));
    }
    return pipes;
}

int32_t PipeSelector::GetRouteFlagByStreamDesc(std::shared_ptr<AudioStreamDescriptor> streamDesc)
{
    int32_t flag = 0;
    CHECK_AND_RETURN_RET_LOG(streamDesc != nullptr, flag, "streamDesc is nullptr");
    flag = configManager_->GetRouteFlag(streamDesc);
    return flag;
}

int32_t PipeSelector::GetPipeInfoByStreamDesc(std::shared_ptr<AudioStreamDescriptor> streamDesc, AudioPipeInfo &info)
{
    CHECK_AND_RETURN_RET_LOG(streamDesc != nullptr, ERROR, "streamDesc is nullptr");
    PipeStreamPropInfo streamPropInfo = {};
    configManager_->GetStreamPropInfo(streamDesc, streamPropInfo);
    ConvertStreamDescToPipeInfo(streamDesc, streamPropInfo, info);
    return SUCCESS;
}

void PipeSelector::ConvertStreamDescToPipeInfo(std::shared_ptr<AudioStreamDescriptor> streamDesc,
    const PipeStreamPropInfo streamPropInfo, AudioPipeInfo &info)
{
    // xml解析后保存枚举类型，AudioModuleInfo中对应变量是否要修改？
    info.moduleInfo_.format = streamPropInfo.format_;
    info.moduleInfo_.rate = streamPropInfo.sampleRate_;
    info.moduleInfo_.channels = streamPropInfo.channelLayout_;
    info.moduleInfo_.bufferSize = streamPropInfo.bufferSize_;

    info.moduleInfo_.lib = streamPropInfo.pipeInfo_->paProp_.lib_;
    info.moduleInfo_.role = streamPropInfo.pipeInfo_->paProp_.paPropRole_;
    info.moduleInfo_.name = streamPropInfo.pipeInfo_->paProp_.moduleName_;

    // className和adapterName是一个值，是否可以去掉一个？
    // file_io时className和adapterName不一致？
    info.moduleInfo_.adapterName = streamPropInfo.pipeInfo_->adapterInfo_->adapterName_;
    info.moduleInfo_.deviceType = streamDesc->newDeviceDesc_->deviceType_;
    // AudioModuleInfo中的networkId是和newDeviceDesc中的一致吗？
    info.moduleInfo_.networkId = streamDesc->newDeviceDesc_->networkId_;

    streamDesc->streamAction_ = STREAM_ACTION_NEW; // 新增流可以这样用，切换时呢？
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
    
}

} // namespace AudioStandard
} // namespace OHOS
