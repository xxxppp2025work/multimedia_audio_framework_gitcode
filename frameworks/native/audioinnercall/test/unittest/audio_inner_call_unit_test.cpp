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

#include "audio_inner_call.h"
#include <gtest/gtest.h>

using namespace testing::ext;
using namespace std;

namespace OHOS {
namespace AudioStandard {

static const int32_t MAX_STREAM_DESCRIPTORS_SIZE = 1000;

class AudioInnerCallUnitTest : public ::testing::Test {
public:
    static void SetUpTestCase(){};
    static void TearDownTestCase(){};
    virtual void SetUp(){};
    virtual void TearDown() {}
};

/**
 * @tc.name   : Test GetIAudioServerInnerCall
 * @tc.number : GetIAudioServerInnerCall_001
 * @tc.desc   : Test GetIAudioServerInnerCall
 */
HWTEST_F(AudioInnerCallUnitTest, GetIAudioServerInnerCall_001, TestSize.Level1)
{
    AudioInnerCall *audioInnerCall = audioInnerCall::GetInstance();
    audioInnerCall->isAudioServerRegistered_ = false;
    audioInnerCall->RegisterAudioServer(nullptr);
    IAudioServerInnerCall *audioServer = audioInnerCall->GetIAudioServerInnerCall();
    EXPECT_EQ(audioServer, nullptr);
}

/**
 * @tc.name   : Test GetIAudioServerInnerCall
 * @tc.number : GetIAudioServerInnerCall_002
 * @tc.desc   : Test GetIAudioServerInnerCall
 */
HWTEST_F(AudioInnerCallUnitTest, GetIAudioServerInnerCall_002, TestSize.Level1)
{
    AudioInnerCall *audioInnerCall = audioInnerCall::GetInstance();
    audioInnerCall->isAudioServerRegistered_ = true;
    audioInnerCall->RegisterAudioServer(nullptr);
    IAudioServerInnerCall *audioServer = audioInnerCall->GetIAudioServerInnerCall();
    EXPECT_EQ(audioServer, nullptr);
}
} // namespace AudioStandard
} // namespace OHOS