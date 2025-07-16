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

#ifndef AUDIO_COLLABORATIVE_MANAGER_H
#define AUDIO_COLLABORATIVE_MANAGER_H

#include <stdint.h>
#include <mutex>

#include "audio_info.h"
#include "audio_ring_cache.h"
#include "audio_effect_chain_adapter.h"
#include "audio_collaborative_listener.h"
#include "i_audio_collaborative_manager.h"

namespace OHOS {
namespace AudioStandard {
class AudioCollaborativeManager : public IAudioCollaborativeManager {
public:
    static IAudioCollaborativeManager& GetInstance();
    bool IsCollaborationEnabled() override;
    bool IsStreamSupportCollaborative(StreamUsage usage) const override;
    void UpdateCollaborativeState(bool collaborationEnabled) override;
    int32_t RegisterCollaborativeListener(ICollaborativeListener* listener) override;
    bool IsCollaborativeFirstChanged(int32_t sessionId, int32_t collaborationEnabled) override;
    void Enqueue(BufferAttr* buffer) override;
    void Dequeue(BufferAttr* buffer) override;
    AudioCollaborativeManager(const AudioCollaborativeManager&) = delete;
    AudioCollaborativeManager(AudioCollaborativeManager&&) = delete;
    AudioCollaborativeManager& operator=(const AudioCollaborativeManager&) = delete;
    AudioCollaborativeManager& operator=(AudioCollaborativeManager&&) = delete;
private:
    AudioCollaborativeManager() = default;
    ~AudioCollaborativeManager() = default;
    void ProcessInputFrameInner();
    void ProcessOutputFrameInner(BufferAttr* buffer);
    void SplitCollaborativeDataInner(BufferAttr* buffer);
    void FillSilenceFramesInner(uint32_t latencyMs);
private:
    std::mutex mutex_;
    std::vector<StreamUsage> defaultUsages_ = { STREAM_USAGE_MUSIC, STREAM_USAGE_MOVIE};
    std::unique_ptr<std::vector<float>> collaborativeOutput_;
    std::unique_ptr<std::vector<float>> silenceData_;
    std::map<uint32_t, bool> sessionCollaborativeState_;
    bool isCollaborativeEnabled_ = false;
    ICollaborativeListener* listener_ = nullptr;
    std::unique_ptr<AudioRingCache> ringCache_ = nullptr;
    bool enqueueRunning_ = false;
    int32_t enqueueCount_ = 1;
};
}
}
#endif // AUDIO_COLLABORATIVE_MANAGER_H