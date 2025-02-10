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
#include "audio_concurrency_service.h" 
#include "pipe_manager.h"

namespace OHOS {
namespace AudioStandard {

static std::map<int, AudioPipeType> flagPipeTypeMap_ = {
    {AUDIO_OUTPUT_FLAG_PRIMARY,       PIPE_TYPE_NORMAL_OUT},
    {AUDIO_INPUT_FLAG_PRIMARY,        PIPE_TYPE_NORMAL_IN},
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
std::vector<std::pair<PipeInfo, PipeInfo>> PipeSelector::FetchPipeAndExecute(std::shared_ptr<AudioStreamDescriptor> streamDesc)
{
    std::vector<std::pair<PipeInfo, PipeInfo>> pipes;

    PipeInfo info;
    int ret = GetPipeInfoByStreamDesc(streamDesc, info);  //根据audio policy xml获取pipe信息，接口待提供，info传引用
    if (ret < 0) {
        return pipes;
    }

    std::map<std::pair<AudioPipeType, AudioPipeType>, ConcurrencyAction> map = 
        AudioStreamCollector::GetAudioStreamCollector().GetConcurrencyMap();
    std::vector<PipeInfo> pipeList = PipeManager::GetPipeManager().GetPipeList();

    for (auto it = pipeList.begin(); it != pipeList.end(); it++) {
        switch (map[std::make_pair(flagPipeTypeMap_[it.streamDesc->flag], flagPipeTypeMap_[info.streamDesc->flag])]) {    //flag如果在flagPipeTypeMap_中不存在如何匹配？默认NORMAL？
        case PLAY_BOTH:
            it.action = PIPE_ACTION_DEFAULT;
            pipes.push_back(std::make_pair(it, it));
            break;
        case CONCEDE_INCOMING:
            it.action = PIPE_ACTION_DEFAULT;
            pipes.push_back(std::make_pair(it, it));
            info = GetPrimaryPipeInfo(info);    //新增流降级为primary流，  GetPrimaryPipeInfo待实现。
            break;
        case CONCEDE_EXISTING:
            PipeInfo concedeInfo = GetPrimaryPipeInfo(it);
            concedeInfo.action = PIPE_ACTION_RECREATE;
            pipes.push_back(std::make_pair(concedeInfo, it));    //existing流降级为primary流
            break:
        default:
            break;
        }
    }

    info.action = PIPE_ACTION_NEW;
    pipes.push_back(std::make_pair(info, info));

    return pipes;
}

//更新流时使用，比如设备更新，下发所有流信息，AudioStreamDescriptor中包含时间信息，以此排序，生成pipelist中修改后信息，并通过流信息查询现存pipelist，获取原始pipe信息
//停流也走这个接口？？？
std::vector<std::pair<PipeInfo, PipeInfo>> PipeSelector::FetchPipesAndExecute(const std::vector<std::shared_ptr<AudioStreamDescriptor>> &streamDescs)
{
    int ret = 0;
    PipeInfo info;
    std::vector<std::pair<PipeInfo, PipeInfo>> pipes;

    std::map<std::pair<AudioPipeType, AudioPipeType>, ConcurrencyAction> map = 
        AudioStreamCollector::GetAudioStreamCollector().GetConcurrencyMap();
    std::vector<PipeInfo> pipeList = PipeManager::GetPipeManager().GetPipeList();
    
    //streamDescs中流信息按时间排序？？？

    for (auto it = streamDescs.begin(); it != streamDescs.end(); it++) {
        ret = GetPipeInfoByStreamDesc(*it, info);
        if (ret < 0) {
            continue;
        }

        for(auto iter = pipes.begin(); iter != pipes.end(); iter++) {
            switch(map[std::make_pair(iter.streamDesc->flag, info.streamDesc->flag)]) {
            case PLAY_BOTH:
                break;
            case CONCEDE_INCOMING:
                info = GetPrimaryPipeInfo(info);
                break;
            case CONCEDE_EXISTING:
                PipeInfo concedeInfo = GetPrimaryPipeInfo(it);
                PipeManager::GetPipeManager().Assign(iter.first, concedeInfo);    //existing流降级为primary流
                break:
            default:
                break;
            }
        }

        pipes.push_back(std::make_pair(info, info));
    }

    //更新pipes中pipe action
    for (auto it = pipes.begin(); it != pipes.end(); it++) {
        for (auto iter = pipeList.begin(); iter != pipeList.end(); iter++) {
            if (it.first.streamDesc == iter.streamDesc) {
                if (!PipeManager::GetPipeManager().IsSamePipe(it.first, iter)) {
                    it.first.action = PIPE_ACTION_RECREATE;
                } else {
                    it.first.action = PIPE_ACTION_DEFAULT;
                }
            }
            break;
        }
    }

    return pipes;
}

} // namespace AudioStandard
} // namespace OHOS
