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
#include "audio_stream_descriptor.h"
#include "audio_module_info.h"

namespace OHOS {
namespace AudioStandard {

enum PipeAction {
    PIPE_ACTION_DEFAULT,
    PIPE_ACTION_UPDATE,
    PIPE_ACTION_NEW
};

class AudioPipeInfo {
public:
    int32_t id_;
    AudioPipeRole role_;
    std::string adapterName_;
    AudioFlag routeFlag_;
    AudioModuleInfo moduleInfo_;
    PipeAction action_;
    std::vector<std::shared_ptr<AudioStreamDescriptor>> streamDescs_;  //AudioStreamDescriptor中包含流ID信息？？？
    std::unordered_map<uint32_t, std::shared_ptr<AudioStreamDescriptor>> streamDescMap_;
};

class PipeManager {
public:
    static PipeManager& GetPipeManager()
    {
        static PipeManager pipeManager;
        return pipeManager;
    }

    void AddAudioPipeInfo(std::shared_ptr<AudioPipeInfo> info);
    void RemoveAudioPipeInfo(std::shared_ptr<AudioPipeInfo> info);
    void UpdateAudioPipeInfo(std::shared_ptr<AudioPipeInfo> newPipe);
    void Assign(std::shared_ptr<AudioPipeInfo> dst, std::shared_ptr<AudioPipeInfo> src);
    bool IsSamePipe(std::shared_ptr<AudioPipeInfo> info, std::shared_ptr<AudioPipeInfo> cmpInfo);

    const std::vector<std::shared_ptr<AudioPipeInfo>> GetPipeList();
    std::vector<std::shared_ptr<AudioPipeInfo>> RemoveUnusedPipe();
    std::shared_ptr<AudioPipeInfo> GetPipeinfoByNameAndFlag(const std::string name, const AudioFlag routeFlag);

    void StartClient(uint32_t sessionId);
    void PauseClient(uint32_t sessionId);
    void StopClient(uint32_t sessionId);
    void RemoveClient(uint32_t sessionId);

    std::vector<std::shared_ptr<AudioStreamDescriptor>> GetAllOutputStreamDescs();
    std::vector<std::shared_ptr<AudioStreamDescriptor>> GetAllInputStreamDescs();
    std::shared_ptr<AudioStreamDescriptor> GetStreamDescById(uint32_t sessionId);
    std::shared_ptr<AudioStreamDescriptor> GetStreamDescByIdInner(uint32_t sessionId);
    int32_t GetStreamCount(const std::string adapterName, const AudioFlag routeFlag);

private:
    PipeManager();
    ~PipeManager();

    std::vector<std::shared_ptr<AudioPipeInfo>> curPipeList_;    //只保存当前存在的pipe信息？？？如果并发是否需要保存原始pipe信息，还是通过AudioStreamDescriptor获取？？？
    std::shared_mutex pipeListLock_;
};
} // namespace AudioStandard
} // namespace OHOS
#endif // ST_PIPE_MANAGER_H
