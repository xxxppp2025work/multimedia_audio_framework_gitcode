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
#define LOG_TAG "PipeManager"
#endif

#include "pipe_manager.h"

namespace OHOS {
namespace AudioStandard {

PipeManager::PipeManager()
{
}

PipeManager::~PipeManager()
{
    curPipeList.clear();
}

void PipeManager::AddAudioPipeInfo(const AudioPipeInfo& info)   //上层通过pipeselector获取AudioPipeInfo
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock);
    curPipeList.push_back(info);
}

void PipeManager::RemoveAudioPipeInfo(const AudioPipeInfo& info)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock);
    for (auto iter = curPipeList.begin(); iter != curPipeList.end(); iter++) {
        if (IsSamePipe(info, *iter)) {
            curPipeList.erase(iter);
            break;
        }
    }
}

void PipeManager::UpdateAudioPipeInfo(const AudioPipeInfo& oldPipe, const AudioPipeInfo& newPipe)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock);
    for (auto iter = curPipeList.begin(); iter != curPipeList.end(); iter++) {
        if (IsSamePipe(oldPipe, *iter)) {
            Assign(*iter, newPipe);
            break;
        }
    }
}

bool PipeManager::IsSamePipe(const AudioPipeInfo& info, const AudioPipeInfo& cmpInfo)     //相同pipe判断是否直接通过stream id？？？
{
    if (info.moduleName == cmpInfo.moduleName &&
        info.adapterName == cmpInfo.adapterName &&
        info.samplingRate == cmpInfo.samplingRate &&
        info.format == cmpInfo.format &&
        info.channelLayout == cmpInfo.channelLayout &&
        info.streamDesc == cmpInfo.streamDesc) {
        return true;
    }

    return false;
}

void PipeManager::Assign(AudioPipeInfo& dst, const AudioPipeInfo& src)
{
    dst.moduleName = src.moduleName;
    dst.adapterName = src.adapterName;
    dst.samplingRate = src.samplingRate;
    dst.format = src.format;
    dst.channelLayout = src.channelLayout;
    dst.pin = src.pin;
    dst.streamDesc = src.streamDesc;
}

void PipeManager::StartClient(uint32_t sessionId)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock);
    std::shared_ptr<AudioStreamDescriptor> streamDesc = streamDescMap[sessionId];
    streamDesc->streamStatus_ = STREAM_STATUS_STARTTING;
}

void PipeManager::PauseClient(uint32_t sessionId)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock);
    std::shared_ptr<AudioStreamDescriptor> streamDesc = streamDescMap[sessionId];
    streamDesc->streamStatus_ = STREAM_STATUS_PAUSED;
}

void PipeManager::RemoveClient(uint32_t sessionId)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock);
    std::shared_ptr<AudioStreamDescriptor> streamDesc = streamDescMap[sessionId];
    streamDesc->streamStatus_ = STREAM_STATUS_RELEASED;
}

const std::vector<AudioPipeInfo> PipeManager::GetPipeList()
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock);
    return curPipeList;
}

std::shared_ptr<AudioPipeInfo> PipeManager::GetPipeinfoByNameAndFlag(const std::string name, const AudioFlag routeFlag)
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock);
    for (auto it : curPipeList) {
        if (it.name_ == name && it.routeFlag_ == routeFlag) {
            return std::make_shared<AudioPipeInfo>(it);
        }
    }
}

} // namespace AudioStandard
} // namespace OHOS
