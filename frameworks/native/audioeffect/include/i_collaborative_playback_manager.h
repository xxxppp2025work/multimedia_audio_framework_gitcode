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

#ifndef I_COLLABORATIVE_PLAYBACK_MANAGER_H
#define I_COLLABORATIVE_PLAYBACK_MANAGER_H

#include <cstdint>
#include "audio_effect_chain_adapter.h"
#include "audio_stream_info.h"

namespace OHOS {
namespace AudioStandard {
class ICollaborativePlaybackManager {
public:
    virtual ~ICollaborativePlaybackManager() = default;
    virtual bool IsCollaborationEnabled() = 0;
    virtual bool IsStreamSupportCollaborative(StreamUsage usage) const = 0;
    virtual int32_t UpdateCollaborativeState(bool collaborationEnabled) = 0;
    virtual int32_t RegisterCollaborativeListener(ICollaborativeListener* listener) = 0;
    virtual bool IsCollaborativeChanged(int32_t sessionId, int32_t collaborationEnabled) = 0;
    virtual void Enqueue(BufferAttr* buffer) = 0;
    virtual void Dequeue(BufferAttr* buffer) = 0;
    virtual void ResetBuffer() = 0;
};
} // namespace AudioStandard
} // namespace OHOS
#endif // I_COLLABORATIVE_PLAYBACK_MANAGER_H