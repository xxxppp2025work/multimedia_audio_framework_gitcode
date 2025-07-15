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
#define LOG_TAG "AudioCollaborativeAdapter"
#endif

#include "audio_collaborative_adapter.h"
#include "audio_effect_chain_adapter.h"
namespace OHOS {
namespace AudioStandard {

bool IsStreamSupportCollaborative(StreamUsage usage)
{
    return AudioCollaborativeManager::GetInstance().IsStreamSupportCollaborative(usage);
}

bool IsCollaborationEnabled()
{
    return AudioCollaborativeManager::GetInstance().IsCollaborationEnabled();
}

void CollaborativeManagerEnqueue(BufferAttr *bufferAttr)
{
    CHECK_AND_RETURN_LOG(bufferAttr != nullptr, "bufferAttr is null");
    CHECK_AND_RETURN_LOG(bufferAttr->bufIn != nullptr && bufferAttr->bufOut != nullptr,
        "Input or output buffer is null");
    AudioCollaborativeManager::GetInstance().Enqueue(bufferAttr);
    return;
}

void CollaborativeManagerDequeue(BufferAttr *bufferAttr)
{
    CHECK_AND_RETURN_LOG(bufferAttr != nullptr, "bufferAttr is null");
    CHECK_AND_RETURN_LOG(bufferAttr->bufIn != nullptr && bufferAttr->bufOut != nullptr,
        "Input or output buffer is null");
    AudioCollaborativeManager::GetInstance().Dequeue(bufferAttr);
    return;
}

bool IsCollaborativeFirstChanged(int32_t sessionID, int32_t collaborationEnabled);
{
    AudioCollaborativeManager *audioCollaborativeManager = AudioCollaborativeManager::GetInstance();
    CHECK_AND_RETURN_RET_LOG(audioCollaborativeManager != nullptr, false, "null audioCollaborativeManager");
    // check if collaborative is first changed for the session
    return audioCollaborativeManager->IsCollaborativeFirstChanged(sessionID, collaborationEnabled);
}
}
}