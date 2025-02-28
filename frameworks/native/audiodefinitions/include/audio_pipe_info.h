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

#ifndef PIPE_INFO_H
#define PIPE_INFO_H
#define HDI_INVALID_ID 0xFFFFFFFF

#include <memory>
#include "parcel.h"

#include "audio_module_info.h"
#include "audio_stream_descriptor.h"


namespace OHOS {
namespace AudioStandard {

enum AudioPipeAction {
    PIPE_ACTION_DEFAULT = 0,
    PIPE_ACTION_UPDATE,
    PIPE_ACTION_RELOAD,
    PIPE_ACTION_NEW,
};

enum AudioPipeRole {
    PIPE_ROLE_OUTPUT = 0,
    PIPE_ROLE_INPUT,
};

class AudioPipeInfo {
public:
    uint32_t id_ = HDI_INVALID_ID;

    AudioPipeRole pipeRole_ = PIPE_ROLE_OUTPUT;

    AudioFlag routeFlag_ = AUDIO_OUTPUT_FLAG_NONE;

    std::string adapterName_;

    AudioModuleInfo moduleInfo_;

    AudioPipeAction pipeAction_ = PIPE_ACTION_DEFAULT;

    std::vector<std::shared_ptr<AudioStreamDescriptor>> streamDescriptors_;

    AudioPipeInfo();
    virtual ~AudioPipeInfo();
};
} // namespace AudioStandard
} // namespace OHOS
#endif // PIPE_INFO_H
