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
    curPipeList.remove();
}

void PipeManager::AddPipeInfo(const PipeInfo& info)   //上层通过pipeselector获取pipeinfo
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock);
    curPipeList.push_back(info);
}

void PipeManager::RemovePipeInfo(const PipeInfo& info)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock);
    for (auto iter = curPipeList.begin(); iter != curPipeList.end(); iter++) {
        if (IsSamePipe(info, *iter)) {
            curPipeList.erase(iter);
            break;
        }
    }
}

void PipeManager::UpdatePipeInfo(const PipeInfo& old, const PipeInfo& new)
{
    std::unique_lock<std::shared_mutex> pLock(pipeListLock);
    for (auto iter = curPipeList.begin(); iter != curPipeList.end(); iter++) {
        if (IsSamePipe(old, *iter)) {
            Assign(*iter, new);
            break;
        }
    }
}

bool PipeManager::IsSamePipe(const PipeInfo& info, const PipeInfo& cmpInfo)     //相同pipe判断是否直接通过stream id？？？
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

void PipeManager::Assign(PipeInfo& dst, const PipeInfo& src)
{
    dst.moduleName = src.moduleName;
    dst.adapterName = src.adapterName;
    dst.samplingRate = src.samplingRate;
    dst.format = src.format;
    dst.channelLayout = src.channelLayout;
    dst.pin = src.pin;
    dst.streamDesc = src.streamDesc;
}

const std::vector<PipeInfo> PipeManager::GetPipeList()
{
    std::shared_lock<std::shared_mutex> pLock(pipeListLock);
    return curPipeList;
}

} // namespace AudioStandard
} // namespace OHOS
