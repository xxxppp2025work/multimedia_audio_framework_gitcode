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
#define LOG_TAG "CollaborativePlaybackManager"
#endif

#include "collaborative_playback_manager.h"

namespace OHOS {
namespace AudioStandard {
static constexpr int32_t DEFAULT_CHANNELS = 2;
static constexpr int32_t COLLABORATIVE_EFFECT_CHANNEL = 4;
static constexpr int32_t DIRECT_CHANNELS = 2;
static constexpr int32_t COLLABORATIVE_CHANNELS = 2;
static constexpr int32_t COLLABORATIVE_CHANNEL_LAYOUT = 3;
static constexpr int32_t COLLABORATIVE_OUTPUT_CHANNEL_1_INDEX = 2;
static constexpr int32_t COLLABORATIVE_OUTPUT_CHANNEL_2_INDEX = 3;
static constexpr int32_t DEFAULT_FRAME_LEN = 960;
static constexpr int32_t MAX_CACHE_SIZE = 500;
static constexpr int32_t DEFAULT_FRAME_LEN_MS = 20;
static constexpr int32_t MS_PER_SECOND = 1000;
static constexpr int32_t TEST_LATENCY = 280;
static constexpr int32_t ENQUEUE_DONE_FRAME = 10;

ICollaborativePlaybackManager& CollaborativePlaybackManager::GetInstance()
{
    static CollaborativePlaybackManager instance;
    return instance;
}

CollaborativePlaybackManager::CollaborativePlaybackManager()
{
    const size_t size = SAMPLE_RATE_48000 * static_cast<int32_t>(STEREO) *
        sizeof(float) * MAX_CACHE_SIZE / MS_PER_SECOND;
    AUDIO_INFO_LOG("Created ring cache, size: %{public}zu", size);
    ringCache_ = AudioRingCache::Create(size);
    CHECK_AND_RETURN_LOG(ringCache_ != nullptr, "Create ring cache failed");
    collaborativeOutput_ = std::make_unique<std::vector<float>>(COLLABORATIVE_CHANNELS * DEFAULT_FRAME_LEN);
    silenceData_ = std::make_unique<std::vector<float>>(COLLABORATIVE_CHANNELS * DEFAULT_FRAME_LEN, 0.0f);
}

bool CollaborativePlaybackManager::IsCollaborationEnabled()
{
    std::lock_guard<std::mutex> lock(mutex_);
    return isCollaborativeEnabled_;
}

bool CollaborativePlaybackManager::IsStreamSupportCollaborative(StreamUsage usage) const
{
    return std::find(defaultUsages_.begin(), defaultUsages_.end(), usage) != defaultUsages_.end();
}

int32_t CollaborativePlaybackManager::UpdateCollaborativeState(bool isCollaborative)
{
    if (isCollaborativeEnabled_ != isCollaborative) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            isCollaborativeEnabled_ = isCollaborative;
            AUDIO_INFO_LOG("UpdateCollaborativeState, isCollaborative: %{public}d", isCollaborative);
        }
        // Notify listener about the change in collaborative state
        CHECK_AND_RETURN_RET_LOG(listener_ != nullptr, ERROR,
            "UpdateCollaborativeState failed, listener is null");
        listener_->OnCollaborativeStateChanged(isCollaborative);
    }
    return SUCCESS;
}

int32_t CollaborativePlaybackManager::RegisterCollaborativeListener(ICollaborativeListener* listener)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (listener == nullptr) {
        AUDIO_ERR_LOG("registerCollaborativeListener failed, listener is null");
        return ERROR;
    }
    listener_ = listener;
    AUDIO_INFO_LOG("registerCollaborativeListener success, listener registered");
    return SUCCESS;
}

bool CollaborativePlaybackManager::IsCollaborativeFirstChanged(int32_t sessionID, int32_t collaborationEnabled)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = sessionCollaborativeState_.find(sessionID);
    if (it == sessionCollaborativeState_.end()) {
        // not found, add new session
        sessionCollaborativeState_[sessionID] = collaborationEnabled;
        return true;
    }
    if (it->second != collaborationEnabled) {
        it->second = collaborationEnabled;
        // first changed
        return true;
    }
    return false;
}

void CollaborativePlaybackManager::Enqueue(BufferAttr* buffer)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_LOG(buffer != nullptr, "Enqueue failed, buffer is null");
    CHECK_AND_RETURN_LOG(buffer->numChanOut == COLLABORATIVE_EFFECT_CHANNEL,
        "Output channel count is not equal to collaborative channel count");
    // todo dump

    // spilit data into direct and collaborative output
    SplitCollaborativeDataInner(buffer);
    DumpFileUtil::WriteDumpFile(dumpFileInput_, static_cast<void *>(buffer->bufOut),
        DEFAULT_FRAME_LEN * COLLABORATIVE_CHANNELS * sizeof(float));
    // process input buffer
    ProcessInputFrameInner();
    
    // process enqueue flag
    if (enqueueCount_ < ENQUEUE_DONE_FRAME) {
        enqueueCount_++;
    } else if (enqueueCount_ == ENQUEUE_DONE_FRAME) {
        enqueueCount_++;
        enqueueRunning_ = true;
        // fill silence frames for latency adjustment
        AUDIO_INFO_LOG("Filling silence frames for latency adjustment");
        ringCache_->ResetBuffer();
        FillSilenceFramesInner(TEST_LATENCY);
    }
    // set output buffer attributes
    buffer->numChanOut = COLLABORATIVE_CHANNELS;
    buffer->outChanLayout = COLLABORATIVE_CHANNEL_LAYOUT;
}

void CollaborativePlaybackManager::Dequeue(BufferAttr* buffer)
{
    std::lock_guard<std::mutex> lock(mutex_);
    
    // write silence data if enqueue is not running
    if (!enqueueRunning_) {
        return;
    }
    // process output buffer
    ProcessOutputFrameInner(buffer);
    DumpFileUtil::WriteDumpFile(dumpFileOutput_, static_cast<void *>(buffer->bufOut),
        DEFAULT_FRAME_LEN * COLLABORATIVE_CHANNELS * sizeof(float));
}

void CollaborativePlaybackManager::ProcessInputFrameInner()
{
    CHECK_AND_RETURN_LOG(ringCache_ != nullptr, "Ring cache is null");
    
    const size_t writeLen = COLLABORATIVE_CHANNELS * DEFAULT_FRAME_LEN * sizeof(float);
    
    // check writable size
    OptResult result = ringCache_->GetWritableSize();
    CHECK_AND_RETURN_LOG(result.ret == OPERATION_SUCCESS, "Get writable size failed");
    CHECK_AND_RETURN_LOG(result.size >= writeLen,
        "Insufficient cache space: %{public}zu < %{public}zu", result.size, writeLen);
    
    // enqueue buffer
    BufferWrap bufferWrap = {reinterpret_cast<uint8_t*>(collaborativeOutput_->data()), writeLen};
    result = ringCache_->Enqueue(bufferWrap);
    CHECK_AND_RETURN_LOG(result.ret == OPERATION_SUCCESS, "Enqueue data failed");
}

void CollaborativePlaybackManager::ProcessOutputFrameInner(BufferAttr* buffer)
{
    CHECK_AND_RETURN_LOG(ringCache_ != nullptr, "Ring cache is null");
    
    const size_t requestDataLen = SAMPLE_RATE_48000 * static_cast<int32_t>(STEREO) *
                                sizeof(float) * DEFAULT_FRAME_LEN_MS / MS_PER_SECOND;
    
    // check readable size
    OptResult result = ringCache_->GetReadableSize();
    CHECK_AND_RETURN_LOG(result.ret == OPERATION_SUCCESS, "Get readable size failed");
    
    if (result.size < requestDataLen) {
        AUDIO_WARNING_LOG("Insufficient data: %{public}zu < %{public}zu, outputting silence",
            result.size, requestDataLen);
        return;
    }
    // read buffer
    BufferWrap bufferWrap = {reinterpret_cast<uint8_t *>(collaborativeOutput_->data()), requestDataLen};
    result = ringCache_->Dequeue(bufferWrap);
    CHECK_AND_RETURN_LOG(result.ret == OPERATION_SUCCESS, "Dequeue data failed");
    for (uint32_t i = 0; i < DEFAULT_FRAME_LEN; ++i) {
        buffer->bufOut[DEFAULT_CHANNELS * i] = (*collaborativeOutput_)[COLLABORATIVE_CHANNELS * i];
        buffer->bufOut[DEFAULT_CHANNELS * i + 1] = (*collaborativeOutput_)[COLLABORATIVE_CHANNELS * i + 1];
    }
}

void CollaborativePlaybackManager::SplitCollaborativeDataInner(BufferAttr* buffer)
{
    for (uint32_t i = 0; i < buffer->frameLen; ++i) {
        buffer->bufOut[DIRECT_CHANNELS * i] = buffer->bufOut[COLLABORATIVE_EFFECT_CHANNEL * i];
        buffer->bufOut[DIRECT_CHANNELS * i + 1] = buffer->bufOut[COLLABORATIVE_EFFECT_CHANNEL * i + 1];
        (*collaborativeOutput_)[COLLABORATIVE_CHANNELS * i] =
            buffer->bufOut[COLLABORATIVE_EFFECT_CHANNEL * i + COLLABORATIVE_OUTPUT_CHANNEL_1_INDEX];
        (*collaborativeOutput_)[COLLABORATIVE_CHANNELS * i + 1] =
            buffer->bufOut[COLLABORATIVE_EFFECT_CHANNEL * i + COLLABORATIVE_OUTPUT_CHANNEL_2_INDEX];
    }
}

void CollaborativePlaybackManager::FillSilenceFramesInner(uint32_t latencyMs)
{
    CHECK_AND_RETURN_LOG(ringCache_ != nullptr, "Ring cache is null");
    
    uint32_t offset = 0;
    const size_t frameSize = DEFAULT_FRAME_LEN * COLLABORATIVE_CHANNELS * sizeof(float);
    
    while (offset < latencyMs) {
        // check writable size
        OptResult result = ringCache_->GetWritableSize();
        CHECK_AND_RETURN_LOG(result.ret == OPERATION_SUCCESS, "Get writable size failed");
        if (result.size < frameSize) {
            AUDIO_WARNING_LOG("Insufficient space for silence frame: %{public}zu < %{public}zu",
                result.size, frameSize);
            break;
        }
        
        // create silence frame
        BufferWrap bufferWrap = {reinterpret_cast<uint8_t *>(silenceData_->data()), frameSize};
        result = ringCache_->Enqueue(bufferWrap);
        CHECK_AND_RETURN_LOG(result.ret == OPERATION_SUCCESS, "Enqueue silence frame failed");
        offset += DEFAULT_FRAME_LEN_MS;
    }
    AUDIO_INFO_LOG("Filled %{public}u ms of silence frames", offset);
}

void CollaborativePlaybackManager::ResetBuffer()
{
    // reset status flag
    enqueueCount_ = 1;
    enqueueRunning_ = false;
    dumpNameIn_ = "dump_collaborative_in.pcm";
    dumpNameOut_ = "dump_collaborative_out.pcm";
    DumpFileUtil::OpenDumpFile(DumpFileUtil::DUMP_SERVER_PARA, dumpNameIn_, &dumpFileInput_);
    DumpFileUtil::OpenDumpFile(DumpFileUtil::DUMP_SERVER_PARA, dumpNameOut_, &dumpFileOutput_);
}
} // namespace AudioStandard
} // namespace OHOS