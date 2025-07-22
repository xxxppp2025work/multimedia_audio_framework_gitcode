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
#define LOG_TAG "CollaborativePlaybackAdapterUnitTest"
#endif

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "audio_errors.h"
#include "collaborative_playback_adapter_unit_test.h"

using namespace std;
using namespace testing::ext;
using namespace testing;

namespace OHOS {
namespace AudioStandard {
    bool IsCollaborationEnabled() override;
    bool IsStreamSupportCollaborative(StreamUsage usage) const override;
    int32_t UpdateCollaborativeState(bool collaborationEnabled) override;
    int32_t RegisterCollaborativeListener(ICollaborativeListener* listener) override;
    bool IsCollaborativeFirstChanged(int32_t sessionId, int32_t collaborationEnabled) override;
    void Enqueue(BufferAttr* buffer) override;
    void Dequeue(BufferAttr* buffer) override;
    void ResetBuffer() override;
/**
* @tc.name   : Test IsCollaborationEnabled API
* @tc.number : IsCollaborationEnabled_001
* @tc.desc   : Test IsCollaborationEnabled interface.
*/
HWTEST(CollaborativePlaybackManagerUnitTest, IsCollaborationEnabled_001, TestSize.Level1)
{
    bool ret = CollaborativePlaybackManager::GetInstance().IsCollaborationEnabled();
    EXPECT_EQ(false, ret);
}

/**
* @tc.name   : Test IsStreamSupportCollaborative API
* @tc.number : IsStreamSupportCollaborative_001
* @tc.desc   : Test IsStreamSupportCollaborative interface.
*/
HWTEST(CollaborativePlaybackManagerUnitTest, IsStreamSupportCollaborative_001, TestSize.Level1)
{
    StreamUsage usage = STREAM_USAGE_MUSIC;
    bool ret = CollaborativePlaybackManager::GetInstance().IsStreamSupportCollaborative(usage);
    EXPECT_EQ(true, ret);
    StreamUsage usage = STREAM_USAGE_GAME;
    ret = CollaborativePlaybackManager::GetInstance().IsStreamSupportCollaborative(usage);
    EXPECT_EQ(false, ret);
}

/**
* @tc.name   : Test RegisterCollaborativeListener API
* @tc.number : RegisterCollaborativeListener_001
* @tc.desc   : Test RegisterCollaborativeListener interface.
*/
HWTEST(CollaborativePlaybackManagerUnitTest, RegisterCollaborativeListener_001, TestSize.Level1)
{
    bool ret = CollaborativePlaybackManager::GetInstance().RegisterCollaborativeListener(nullptr);
    EXPECT_EQ(ERROR, ret);
    ICollaborativeListener* listener = new CollaborativeListenerMock();
    ret = CollaborativePlaybackManager::GetInstance().RegisterCollaborativeListener(listener);
    EXPECT_EQ(SUCCESS, ret);
}

/**
* @tc.name   : Test UpdateCollaborativeState API
* @tc.number : UpdateCollaborativeState_001
* @tc.desc   : Test UpdateCollaborativeState interface.
*/
HWTEST(CollaborativePlaybackManagerUnitTest, UpdateCollaborativeState_001, TestSize.Level1)
{
    bool isCollaborative = true;
    int32_t ret = CollaborativePlaybackManager::GetInstance().UpdateCollaborativeState(isCollaborative);
    EXPECT_EQ(SUCCESS, ret);
    isCollaborative = false;
    ret = CollaborativePlaybackManager::GetInstance().UpdateCollaborativeState(isCollaborative);
    EXPECT_EQ(SUCCESS, ret); 
}

/**
* @tc.name   : Test UpdateCollaborativeState API
* @tc.number : UpdateCollaborativeState_001
* @tc.desc   : Test UpdateCollaborativeState interface.
*/
HWTEST(CollaborativePlaybackManagerUnitTest, UpdateCollaborativeState_001, TestSize.Level1)
{
    bool isCollaborative = true;
    int32_t ret = CollaborativePlaybackManager::GetInstance().UpdateCollaborativeState(isCollaborative);
    EXPECT_EQ(SUCCESS, ret);
    isCollaborative = false;
    ret = CollaborativePlaybackManager::GetInstance().UpdateCollaborativeState(isCollaborative);
    EXPECT_EQ(SUCCESS, ret); 
}

/**
* @tc.name   : Test IsCollaborativeFirstChanged API
* @tc.number : IsCollaborativeFirstChanged_001
* @tc.desc   : Test IsCollaborativeFirstChanged interface.
*/
HWTEST(CollaborativePlaybackManagerUnitTest, IsCollaborativeFirstChanged_001, TestSize.Level1)
{
    int32_t sessionId = 123;
    int32_t collaborationEnabled = 1;
    bool ret = CollaborativePlaybackManager::GetInstance().IsCollaborativeFirstChanged(sessionId, collaborationEnabled);
    EXPECT_EQ(true, ret);
    ret = CollaborativePlaybackManager::GetInstance().IsCollaborativeFirstChanged(sessionId, collaborationEnabled);
    EXPECT_EQ(false, ret);
    collaborationEnabled = 0;
    ret = CollaborativePlaybackManager::GetInstance().IsCollaborativeFirstChanged(sessionId, collaborationEnabled);
    EXPECT_EQ(true, ret);
}

/**
* @tc.name   : Test Enqueue API
* @tc.number : Enqueue_001
* @tc.desc   : Test Enqueue interface.
*/
HWTEST(CollaborativePlaybackManagerUnitTest, Enqueue_001, TestSize.Level1)
{
    BufferAttr bufferAttr;
    float* bufIn;
    float* bufOut;
    vector<float> bufInVector;
    vector<float> bufOutVector;
    bufInVector.resize(10000, 0);
    bufOutVector.resize(10000, 0);
    bufferAttr.bufIn = bufInVector.data();
    bufferAttr.bufOut = bufOutVector.data();
    bufferAttr.numChanOut = COLLABORATIVE_EFFECT_CHANNEL;
    
    CollaborativePlaybackManager::GetInstance().Enqueue(&bufferAttr);
    EXPECT_EQ(bufferAttr->numChanOut, COLLABORATIVE_CHANNELS);
    EXPECT_EQ(bufferAttr->outChanLayout, COLLABORATIVE_CHANNEL_LAYOUT);
}

/**
* @tc.name   : Test Dequeue API
* @tc.number : Dequeue_001
* @tc.desc   : Test Dequeue interface.
*/
HWTEST(CollaborativePlaybackManagerUnitTest, Dequeue_001, TestSize.Level1)
{
    BufferAttr bufferAttr;
    float* bufIn;
    float* bufOut;
    vector<float> bufInVector;
    vector<float> bufOutVector;
    bufInVector.resize(10000, 0);
    bufOutVector.resize(10000, 0);
    bufferAttr.bufIn = bufInVector.data();
    bufferAttr.bufOut = bufOutVector.data();
    bufferAttr.numChanOut = COLLABORATIVE_EFFECT_CHANNEL;
    
    CollaborativePlaybackManager::GetInstance().Dequeue(&bufferAttr);
}

/**
* @tc.name   : Test ResetBuffer API
* @tc.number : ResetBuffer_001
* @tc.desc   : Test ResetBuffer interface.
*/
HWTEST(CollaborativePlaybackManagerUnitTest, ResetBuffer_001, TestSize.Level1)
{
    CollaborativePlaybackManager::GetInstance().ResetBuffer();
    // Check if the buffer is reset successfully
    EXPECT_EQ(CollaborativePlaybackManager::GetInstance().GetEnqueueCount(), 1);
    EXPECT_EQ(CollaborativePlaybackManager::GetInstance().IsEnqueueRunning(), false);
}
}
}