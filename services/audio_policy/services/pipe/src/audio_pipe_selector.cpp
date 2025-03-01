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
#define LOG_TAG "AudioPipeSelector"
#endif

#include "audio_pipe_selector.h"
#include "audio_pipe_manager.h"
#include "audio_stream_collector.h"
#include <algorithm>

namespace OHOS {
namespace AudioStandard {

std::vector<std::shared_ptr<AudioPipeInfo>> AudioPipeSelector::FetchPipeAndExecute(
    std::shared_ptr<AudioStreamDescriptor> streamDesc)
{
    std::vector<std::shared_ptr<AudioPipeInfo>> pipeInfoList = AudioPipeManager::GetPipeManager().GetPipeList();

    std::vector<std::shared_ptr<AudioPipeInfo>> newPipeList;

    for (auto pipeInfo : pipeInfoList) {
        for (auto streamDesc : pipeInfo->streamDescriptors_) {
            if (streamDesc->newDeviceDescs_[0]->deviceType_ == streamDesc->newDeviceDescs_[0]->deviceType_) {
                pipeInfo->streamDescriptors_.push_back(streamDesc);
                pipeInfo->pipeAction_ = PIPE_ACTION_UPDATE;
                newPipeList.push_back(pipeInfo);
                return newPipeList;
            }
        }
    }

    AudioPipeInfo info = {};
    if (streamDesc->newDeviceDescs_[0]->deviceType_  == 8) {
        info.moduleInfo_.format = "s32le";
        info.moduleInfo_.rate = "48000";
        info.moduleInfo_.channels = "CH_LAYOUT_STEREO";
        info.moduleInfo_.bufferSize = "3840";

        info.moduleInfo_.lib = "libmodule-hdi-sink.z.so";
        info.moduleInfo_.role = "sink";
        info.moduleInfo_.name = "Bt A2dp";

        // className和adapterName是一个值，是否可以去掉一个？
        // file_io时className和adapterName不一致？
        info.moduleInfo_.adapterName = "a2dp";
        info.moduleInfo_.deviceType = "8";
        // AudioModuleInfo中的networkId是和newDeviceDesc中的一致吗？
        info.moduleInfo_.networkId = "";

        streamDesc->streamAction_ = AUDIO_STREAM_ACTION_NEW; // 新增流可以这样用，切换时呢？
        info.streamDescriptors_.push_back(streamDesc);
        info.streamDescMap_[streamDesc->sessionId_] = streamDesc;
    } else {
        info.moduleInfo_.format = "s32le";
        info.moduleInfo_.rate = "48000";
        info.moduleInfo_.channels = "CH_LAYOUT_STEREO";
        info.moduleInfo_.bufferSize = "3840";

        info.moduleInfo_.lib = "libmodule-hdi-sink.z.so";
        info.moduleInfo_.role = "sink";
        info.moduleInfo_.name = "Speaker";

        // className和adapterName是一个值，是否可以去掉一个？
        // file_io时className和adapterName不一致？
        info.moduleInfo_.adapterName = "primary";
        info.moduleInfo_.deviceType = "2";
        // AudioModuleInfo中的networkId是和newDeviceDesc中的一致吗？
        info.moduleInfo_.networkId = "";

        streamDesc->streamAction_ = AUDIO_STREAM_ACTION_NEW; // 新增流可以这样用，切换时呢？
        info.streamDescriptors_.push_back(streamDesc);
        info.streamDescMap_[streamDesc->sessionId_] = streamDesc;
    }
    // ConvertStreamDescToPipeInfo(streamDesc, streamPropInfo, info);
    info.pipeAction_ = PIPE_ACTION_NEW;
    newPipeList.push_back(std::make_shared<AudioPipeInfo>(info));
    return newPipeList;
}

} // namespace AudioStandard
} // namespace OHOS
