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
    if ((info.adapterName_ == cmpInfo.adapterName_ && info.routeFlag_ == cmpInfo.routeFlag_) ||
        info.sinkId_ == cmpInfo.sinkId_) {
        return true;
    }

    return false;
}

void PipeManager::Assign(AudioPipeInfo& dst, const AudioPipeInfo& src)
{
    dst.sinkId_ = src.sinkId_;
    dst.role_ = src.role_;
    dst.adapterName_ = src.adapterName_;
    dst.routeFlag_ = src.routeFlag_;
    dst.moduleInfo_ = src.moduleInfo_;
    dst.action_ = src.action_;
    dst.streamDescs_ = src.streamDescs_;
    dst.streamDescMap_ = src.streamDescMap_;
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

std::vector<AudioPipeInfo> PipeManager::GetUnusedPipe()
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock);
    std::vector<AudioPipeInfo> newList = curPipeList;
    for (auto iter = newList.begin(); iter != newList.end();) {
        if (iter->streamDescs_.empty()) {
            curPipeList.erase(iter);
        } else {
            iter++;
        }
    }
    return newList;
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

std::vector<std::shared_ptr<AudioStreamDescriptor>> PipeManager::GetAllOutputStreamDescs()
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock);
    std::vector<std::shared_ptr<AudioStreamDescriptor>> streamDescs;
    for (auto it : curPipeList) {
        if (it.role_ == PIPE_ROLE_OUTPUT) {
            streamDescs.insert(streamDescs.end(), it.streamDescs_.begin(), it.streamDescs_.end());
        }
    }
    return streamDescs;
}

std::vector<std::shared_ptr<AudioStreamDescriptor>> PipeManager::GetAllInputStreamDescs()
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock);
    std::vector<std::shared_ptr<AudioStreamDescriptor>> streamDescs;
    for (auto it : curPipeList) {
        if (it.role_ == PIPE_ROLE_INPUT) {
            streamDescs.insert(streamDescs.end(), it.streamDescs_.begin(), it.streamDescs_.end());
        }
    }
    return streamDescs;
}

std::shared_ptr<AudioStreamDescriptor> PipeManager::GetStreamDescById(uint32_t sessionId)
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock);
    for (auto it : curPipeList) {
        for (auto desc : it.streamDescs_) {
            if (desc->sessionId_ == sessionId) {
                return desc;
            }
        }
    }
    return nullptr;
}

int32_t PipeManager::GetStreamCount(const std::string adapterName, const AudioFlag routeFlag)
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock);
    int32_t count = 0;
    for (auto it : curPipeList) {
        if (it.adapterName_ == adapterName && it.routeFlag_ == routeFlag) {
            count = it.streamDescs_.size();
        }
    }
    return count;
}

} // namespace AudioStandard
} // namespace OHOS
