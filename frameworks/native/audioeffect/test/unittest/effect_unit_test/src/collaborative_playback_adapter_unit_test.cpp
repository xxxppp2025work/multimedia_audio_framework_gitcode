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

#include "collaborative_playback_adapter_unit_test.h"

#include <chrono>
#include <thread>
#include <fstream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "audio_effect.h"
#include "audio_effect_log.h"
#include "audio_effect_chain_adapter.h"
#include "audio_effect_chain_manager.h"
#include "audio_errors.h"

using namespace std;
using namespace testing::ext;
using namespace testing;

namespace OHOS {
namespace AudioStandard {
/**
* @tc.name   : Test IsStreamSupportCollaborative API
* @tc.number : IsStreamSupportCollaborative_001
* @tc.desc   : Test IsStreamSupportCollaborative interface.
*/
HWTEST(CollaborativePlaybackAdapterUnitTest, IsStreamSupportCollaborative_001, TestSize.Level1)
{
    int32_t usage = 1;
    bool ret = IsStreamSupportCollaborative(usage);
    EXPECT_EQ(true, ret);
}

/**
* @tc.name   : Test IsCollaborationEnabled API
* @tc.number : IsCollaborationEnabled_001
* @tc.desc   : Test IsCollaborationEnabled interface.
*/
HWTEST(CollaborativePlaybackAdapterUnitTest, IsCollaborationEnabled_001, TestSize.Level1)
{
    bool ret = IsCollaborationEnabled();
    EXPECT_EQ(true, ret);
}

/**
* @tc.name   : Test CollaborativePlaybackEnqueue API
* @tc.number : CollaborativePlaybackEnqueue_001
* @tc.desc   : Test CollaborativePlaybackEnqueue interface.
*/
HWTEST(CollaborativePlaybackAdapterUnitTest, CollaborativePlaybackEnqueue_001, TestSize.Level1)
{
    struct BufferAttr bufferAttr;
    float* bufIn;
    float* bufOut;
    vector<float> bufInVector;
    vector<float> bufOutVector;
    bufInVector.resize(10000, 0);
    bufOutVector.resize(10000, 0);
    bufferAttr.bufIn = bufInVector.data();
    bufferAttr.bufOut = bufOutVector.data();
    CollaborativePlaybackEnqueue(bufferAttr);
}

/**
* @tc.name   : Test CollaborativePlaybackDequeue API
* @tc.number : CollaborativePlaybackDequeue_001
* @tc.desc   : Test CollaborativePlaybackDequeue interface.
*/
HWTEST(CollaborativePlaybackAdapterUnitTest, CollaborativePlaybackDequeue_001, TestSize.Level1)
{
    struct BufferAttr bufferAttr;
    float* bufIn;
    float* bufOut;
    vector<float> bufInVector;
    vector<float> bufOutVector;
    bufInVector.resize(10000, 0);
    bufOutVector.resize(10000, 0);
    bufferAttr.bufIn = bufInVector.data();
    bufferAttr.bufOut = bufOutVector.data();
    CollaborativePlaybackDequeue(bufferAttr);
}

/**
* @tc.name   : Test IsCollaborativeFirstChanged API
* @tc.number : IsCollaborativeFirstChanged_001
* @tc.desc   : Test IsCollaborativeFirstChanged interface.
*/
HWTEST(CollaborativePlaybackAdapterUnitTest, IsCollaborativeFirstChanged_001, TestSize.Level1)
{
    int32_t sessionID = 1;
    int32_t collaborationEnabled = 1;
    bool ret = IsCollaborativeFirstChanged(sessionID, collaborationEnabled);
    EXPECT_EQ(true, ret);
    ret = IsCollaborativeFirstChanged(sessionID, collaborationEnabled);
    EXPECT_EQ(false, ret);
}
}
}