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
#ifndef ST_PIPE_SELECTOR_H
#define ST_PIPE_SELECTOR_H

#include <vector>
#include "audio_stream_info.h"

namespace OHOS {
namespace AudioStandard {

class PipeSelector {
public:
    PipeSelector() = default;
    ~PipeSelector() = default;

    static std::vector<std::shared_ptr<AudioPipeInfo>> FetchPipeAndExecute(std::shared_ptr<AudioStreamDescriptor> streamDesc);
    static std::vector<std::shared_ptr<AudioPipeInfo>> FetchPipesAndExecute(
        std::vector<std::shared_ptr<AudioStreamDescriptor>> &streamDescs);

private:
    int32_t GetRouteFlagByStreamDesc(std::shared_ptr<AudioStreamDescriptor> streamDesc);
    int32_t GetPipeInfoByStreamDesc(std::shared_ptr<AudioStreamDescriptor> streamDesc, AudioPipeInfo &info);
    void ConvertStreamDescToPipeInfo(std::shared_ptr<AudioStreamDescriptor> streamDesc,
        const PipeStreamPropInfo streamPropInfo, AudioPipeInfo &info);
    AudioStreamAction JudgeStreamAction(AudioFlag oldFlag, AudioFlag newFlag);
    void SortStreamDescsByStartTime(std::vector<std::shared_ptr<AudioStreamDescriptor>> &streamDescs);

    std::shared_ptr<AudioPolicyConfigManager> configManager_ = nullptr;
};
} // namespace AudioStandard
} // namespace OHOS
#endif // ST_PIPE_SELECTOR_H