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
#ifndef ST_PIPE_MANAGER_H
#define ST_PIPE_MANAGER_H

#include <string>
#include <mutex>
#include <shared_mutex>

namespace OHOS {
namespace AudioStandard {

enum PipeAction {
    PIPE_ACTION_DEFAULT,
    PIPE_ACTION_RECREATE,
    PIPE_ACTION_NEW
};

class PipeInfo {
public:
    std::string moduleName;
    std::string adapterName;
    int samplingRate;
    int format;
    int channelLayout;
    int pin;
    enum PipeAction action;
    std::shared_ptr<AudioStreamDescriptor> streamDesc;  //AudioStreamDescriptor中包含流ID信息？？？
};

class PipeManager {
public:
    static PipeManager& GetPipeManager()
    {
        static PipeManager pipeManager;
        return pipeManager;
    }

    void AddPipeInfo(const PipeInfo& info);
    void RemovePipeInfo(const PipeInfo& info);
    void UpdatePipeInfo(const PipeInfo& old, const PipeInfo& new);

    const std::vector<PipeInfo> GetPipeList();
    void Assign(PipeInfo& dst, const PipeInfo& src);
    bool IsSamePipe(const PipeInfo& info, const PipeInfo& cmpInfo);

private:
    PipeManager();
    ~PipeManager();

    std::vector<PipeInfo> curPipeList;    //只保存当前存在的pipe信息？？？如果并发是否需要保存原始pipe信息，还是通过AudioStreamDescriptor获取？？？
    std::shared_mutex pipeListLock;
};
} // namespace AudioStandard
} // namespace OHOS
#endif // ST_PIPE_MANAGER_H
