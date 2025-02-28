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
    curPipeList_.clear();
}

void PipeManager::AddAudioPipeInfo(std::shared_ptr<AudioPipeInfo> info)   //上层通过pipeselector获取AudioPipeInfo
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock_);
    curPipeList_.push_back(info);
}

void PipeManager::RemoveAudioPipeInfo(std::shared_ptr<AudioPipeInfo> info)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock_);
    for (auto iter = curPipeList_.begin(); iter != curPipeList_.end(); iter++) {
        if (IsSamePipe(info, *iter)) {
            curPipeList_.erase(iter);
            break;
        }
    }
}

void PipeManager::UpdateAudioPipeInfo(std::shared_ptr<AudioPipeInfo> newPipe)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock_);
    for (auto iter = curPipeList_.begin(); iter != curPipeList_.end(); iter++) {
        if (IsSamePipe(newPipe, *iter)) {
            Assign(*iter, newPipe);
            break;
        }
    }
}

bool PipeManager::IsSamePipe(std::shared_ptr<AudioPipeInfo> info, std::shared_ptr<AudioPipeInfo> cmpInfo)     //相同pipe判断是否直接通过stream id？？？
{
    if ((info->adapterName_ == cmpInfo->adapterName_ && info->routeFlag_ == cmpInfo->routeFlag_) ||
        info->id_ == cmpInfo->id_) {
        return true;
    }

    return false;
}

void PipeManager::Assign(std::shared_ptr<AudioPipeInfo> dst, std::shared_ptr<AudioPipeInfo> src)
{
    dst = src;
}

void PipeManager::StartClient(uint32_t sessionId)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock_);
    std::shared_ptr<AudioStreamDescriptor> streamDesc = GetStreamDescByIdInner(sessionId);
    streamDesc->streamStatus_ = STREAM_STATUS_STARTTING;
}

void PipeManager::PauseClient(uint32_t sessionId)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock_);
    std::shared_ptr<AudioStreamDescriptor> streamDesc = GetStreamDescByIdInner(sessionId);
    streamDesc->streamStatus_ = STREAM_STATUS_PAUSED;
}

void PipeManager::StopClient(uint32_t sessionId)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock_);
    std::shared_ptr<AudioStreamDescriptor> streamDesc = GetStreamDescByIdInner(sessionId);
    streamDesc->streamStatus_ = STREAM_STATUS_STOPPED;
}

void PipeManager::RemoveClient(uint32_t sessionId)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock_);
    std::shared_ptr<AudioStreamDescriptor> streamDesc = GetStreamDescByIdInner(sessionId);
    streamDesc->streamStatus_ = STREAM_STATUS_RELEASED;
}

const std::vector<std::shared_ptr<AudioPipeInfo>> PipeManager::GetPipeList()
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock_);
    return curPipeList_;
}

std::vector<std::shared_ptr<AudioPipeInfo>> PipeManager::RemoveUnusedPipe()
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock_);
    std::vector<std::shared_ptr<AudioPipeInfo>> newList = curPipeList_;
    for (auto iter = newList.begin(); iter != newList.end();) {
        if ((*iter)->streamDescs_.empty()) {
            curPipeList_.erase(iter);
        } else {
            iter++;
        }
    }
    return newList;
}

std::shared_ptr<AudioPipeInfo> PipeManager::GetPipeinfoByNameAndFlag(const std::string adapterName, const AudioFlag routeFlag)
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock_);
    for (auto it : curPipeList_) {
        if (it->adapterName_ == adapterName && it->routeFlag_ == routeFlag) {
            return it;
        }
    }
}

std::vector<std::shared_ptr<AudioStreamDescriptor>> PipeManager::GetAllOutputStreamDescs()
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock_);
    std::vector<std::shared_ptr<AudioStreamDescriptor>> streamDescs;
    for (auto it : curPipeList_) {
        if (it->role_ == PIPE_ROLE_OUTPUT) {
            streamDescs.insert(streamDescs.end(), it->streamDescs_.begin(), it->streamDescs_.end());
        }
    }
    return streamDescs;
}

std::vector<std::shared_ptr<AudioStreamDescriptor>> PipeManager::GetAllInputStreamDescs()
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock_);
    std::vector<std::shared_ptr<AudioStreamDescriptor>> streamDescs;
    for (auto it : curPipeList_) {
        if (it->role_ == PIPE_ROLE_INPUT) {
            streamDescs.insert(streamDescs.end(), it->streamDescs_.begin(), it->streamDescs_.end());
        }
    }
    return streamDescs;
}

std::shared_ptr<AudioStreamDescriptor> PipeManager::GetStreamDescById(uint32_t sessionId)
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock);
    return GetStreamDescByIdInner(sessionId);
}

std::shared_ptr<AudioStreamDescriptor> PipeManager::GetStreamDescByIdInner(uint32_t sessionId)
{
    for (auto it : curPipeList_) {
        for (auto desc : it->streamDescs_) {
            if (desc->sessionId_ == sessionId) {
                return desc;
            }
        }
    }
    return nullptr;
}

int32_t PipeManager::GetStreamCount(const std::string adapterName, const AudioFlag routeFlag)
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock_);
    int32_t count = 0;
    for (auto it : curPipeList_) {
        if (it->adapterName_ == adapterName && it->routeFlag_ == routeFlag) {
            count = it->streamDescs_.size();
        }
    }
    return count;
}

} // namespace AudioStandard
} // namespace OHOS
