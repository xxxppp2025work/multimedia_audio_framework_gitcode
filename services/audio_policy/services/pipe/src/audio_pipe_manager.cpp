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
#define LOG_TAG "AudioPipeManager"
#endif

#include "audio_pipe_manager.h"

namespace OHOS {
namespace AudioStandard {

AudioPipeManager::AudioPipeManager()
{
}

AudioPipeManager::~AudioPipeManager()
{
    curPipeList_.clear();
}

void AudioPipeManager::AddAudioPipeInfo(std::shared_ptr<AudioPipeInfo> info)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock_);
    curPipeList_.push_back(info);
}

void AudioPipeManager::RemoveAudioPipeInfo(std::shared_ptr<AudioPipeInfo> info)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock_);
    for (auto iter = curPipeList_.begin(); iter != curPipeList_.end(); iter++) {
        if (IsSamePipe(info, *iter)) {
            curPipeList_.erase(iter);
            break;
        }
    }
}

void AudioPipeManager::UpdateAudioPipeInfo(std::shared_ptr<AudioPipeInfo> newPipe)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock_);
    for (auto iter = curPipeList_.begin(); iter != curPipeList_.end(); iter++) {
        if (IsSamePipe(newPipe, *iter)) {
            Assign(*iter, newPipe);
            break;
        }
    }
}

bool AudioPipeManager::IsSamePipe(std::shared_ptr<AudioPipeInfo> info, std::shared_ptr<AudioPipeInfo> cmpInfo)     //相同pipe判断是否直接通过stream id？？？
{
    if ((info->adapterName_ == cmpInfo->adapterName_ && info->routeFlag_ == cmpInfo->routeFlag_) ||
        info->id_ == cmpInfo->id_) {
        return true;
    }

    return false;
}

void AudioPipeManager::Assign(std::shared_ptr<AudioPipeInfo> dst, std::shared_ptr<AudioPipeInfo> src)
{
    dst = src;
    // dst->id_ = src->id_;
    // dst->pipeRole_ = src.pipeRole_;
    // dst->adapterName_ = src.adapterName_;
    // dst->routeFlag_ = src->routeFlag_;
    // dst->moduleInfo_ = src.moduleInfo_;
    // dst->pipeAction_ = src.pipeAction_;
    // dst->streamDescriptors_ = src.streamDescriptors_;
    // dst->streamDescMap_ = src.streamDescMap_;
}

void AudioPipeManager::StartClient(uint32_t sessionId)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock_);
    std::shared_ptr<AudioStreamDescriptor> streamDesc = GetStreamDescByIdInner(sessionId);
    streamDesc->streamStatus_ = STREAM_STATUS_STARTTING;
}

void AudioPipeManager::PauseClient(uint32_t sessionId)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock_);
    std::shared_ptr<AudioStreamDescriptor> streamDesc = GetStreamDescByIdInner(sessionId);
    streamDesc->streamStatus_ = STREAM_STATUS_PAUSED;
}

void AudioPipeManager::StopClient(uint32_t sessionId)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock_);
    std::shared_ptr<AudioStreamDescriptor> streamDesc = GetStreamDescByIdInner(sessionId);
    streamDesc->streamStatus_ = STREAM_STATUS_STOPPED;
}

void AudioPipeManager::RemoveClient(uint32_t sessionId)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock_);
    std::shared_ptr<AudioStreamDescriptor> streamDesc = GetStreamDescByIdInner(sessionId);
    streamDesc->streamStatus_ = STREAM_STATUS_RELEASED;
}

const std::vector<std::shared_ptr<AudioPipeInfo>> AudioPipeManager::GetPipeList()
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock_);
    return curPipeList_;
}

std::vector<std::shared_ptr<AudioPipeInfo>> AudioPipeManager::GetUnusedPipe()
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock_);
    std::vector<std::shared_ptr<AudioPipeInfo>> newList;
    for (auto pipe : curPipeList_) {
        if (pipe->streamDescriptors_.empty()) {
            newList.push_back(pipe);
        }
    }
    return newList;
}

std::shared_ptr<AudioPipeInfo> AudioPipeManager::GetPipeinfoByNameAndFlag(const std::string adapterName, const AudioFlag routeFlag)
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock_);
    for (auto it : curPipeList_) {
        if (it->adapterName_ == adapterName && it->routeFlag_ == routeFlag) {
            return it;
        }
    }
    AUDIO_ERR_LOG("Can not find pipe %{public}s", adapterName.c_str());
    return nullptr;
}

std::vector<std::shared_ptr<AudioStreamDescriptor>> AudioPipeManager::GetAllOutputStreamDescs()
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock_);
    std::vector<std::shared_ptr<AudioStreamDescriptor>> streamDescs;
    for (auto it : curPipeList_) {
        if (it->pipeRole_ == PIPE_ROLE_OUTPUT) {
            streamDescs.insert(streamDescs.end(), it->streamDescriptors_.begin(), it->streamDescriptors_.end());
        }
    }
    return streamDescs;
}

std::vector<std::shared_ptr<AudioStreamDescriptor>> AudioPipeManager::GetAllInputStreamDescs()
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock_);
    std::vector<std::shared_ptr<AudioStreamDescriptor>> streamDescs;
    for (auto it : curPipeList_) {
        if (it->pipeRole_ == PIPE_ROLE_INPUT) {
            streamDescs.insert(streamDescs.end(), it->streamDescriptors_.begin(), it->streamDescriptors_.end());
        }
    }
    return streamDescs;
}

std::shared_ptr<AudioStreamDescriptor> AudioPipeManager::GetStreamDescById(uint32_t sessionId)
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock_);
    return GetStreamDescByIdInner(sessionId);
}

std::shared_ptr<AudioStreamDescriptor> AudioPipeManager::GetStreamDescByIdInner(uint32_t sessionId)
{
    for (auto it : curPipeList_) {
        for (auto desc : it->streamDescriptors_) {
            if (desc->sessionId_ == sessionId) {
                return desc;
            }
        }
    }
    return nullptr;
}

int32_t AudioPipeManager::GetStreamCount(const std::string adapterName, const AudioFlag routeFlag)
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock_);
    int32_t count = 0;
    for (auto it : curPipeList_) {
        if (it->adapterName_ == adapterName && it->routeFlag_ == routeFlag) {
            count = it->streamDescriptors_.size();
        }
    }
    return count;
}

void AudioPipeManager::Dump(std::string &dumpString)
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock_);
    AUDIO_INFO_LOG("AudioPipeManager Dump Start!");
    dumpString += "\n^^^^^^^^^^AudioPipeManager Infos^^^^^^^^^^\n";
    dumpString += "\nTotalPipeNums: " + std::to_string(curPipeList_.size()) + "\n\n";

    std::shared_ptr<AudioPipeInfo> curPipeInfo = nullptr;
    for (size_t pipeIdx = 0; pipeIdx < curPipeList_.size(); ++pipeIdx) {
        curPipeInfo = curPipeList_[pipeIdx];
        dumpString += "\n**********Pipe " + std::to_string(pipeIdx + 1) + "**********\n"; // pipeinfo start
        dumpString += "\nadapterName_: " + curPipeInfo->adapterName_ + "\tid_: " + std::to_string(curPipeInfo->id_);
        dumpString += "\nPipeRole_: ";
        dumpString += (curPipeInfo->pipeRole_ == PIPE_ROLE_OUTPUT ? "OUTPUT" : "INPUT");
        dumpString += "\npipeAction_: " + std::to_string(curPipeInfo->pipeAction_);
        dumpString += "\nrouteFlag_: " + std::to_string(curPipeInfo->routeFlag_);
        for (size_t streamIdx = 0; streamIdx < curPipeInfo->streamDescriptors_.size(); ++streamIdx) {
            dumpString += "\n----------Stream " + std::to_string(streamIdx + 1) + " in Pipe " + std::to_string(pipeIdx + 1) + "----------\n"; // streaminfo start
            curPipeInfo->streamDescriptors_[streamIdx]->Dump(dumpString);
            dumpString += "\n"; //streaminfo end
        }
        dumpString += "\n"; // pipeinfo end
    }
    dumpString += "\n^^^^^^^^^^AudioPipeManager Infos^^^^^^^^^^\n";
}

} // namespace AudioStandard
} // namespace OHOS
