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

class AudioInnerCallUnitTest : public ::testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void AudioInnerCallUnitTest::SetUpTestCase(void)
{
    //input testsuit setup step, setup invoked before all testcases
}

void AudioInnerCallUnitTest::TearDownTestCase(void)
{
    //input testsuit setdown step, setdown invoked after all testcases
}

void AudioInnerCallUnitTest::SetUp(void)
{
    //input testsuit setup step, setup invoked before each testcases
}

void AudioInnerCallUnitTest::TearDown(void)
{
    //input testsuit setdown step, setdown invoked after each testcases
}

/**
 * @tc.name   : Test GetIAudioServerInnerCall
 * @tc.number : GetIAudioServerInnerCall_001
 * @tc.desc   : Test GetIAudioServerInnerCall
 */
HWTEST_F(AudioInnerCallUnitTest, GetIAudioServerInnerCall_001, TestSize.Level1)
{
    AudioInnerCall *audioInnerCall = AudioInnerCall::GetInstance();
    ASSERT_NE(audioInnerCall, nullptr);
    audioInnerCall->isAudioServerRegistered_ = false;
    audioInnerCall->RegisterAudioServer(nullptr);
    IAudioServerInnerCall *result = audioInnerCall->GetIAudioServerInnerCall();
    EXPECT_EQ(result, nullptr);
    delete audioInnerCall;
}

/**
 * @tc.name   : Test GetIAudioServerInnerCall
 * @tc.number : GetIAudioServerInnerCall_002
 * @tc.desc   : Test GetIAudioServerInnerCall
 */
HWTEST_F(AudioInnerCallUnitTest, GetIAudioServerInnerCall_002, TestSize.Level1)
{
    AudioInnerCall *audioInnerCall = AudioInnerCall::GetInstance();
    ASSERT_NE(audioInnerCall, nullptr);
    audioInnerCall->isAudioServerRegistered_ = true;
    audioInnerCall->RegisterAudioServer(nullptr);
    IAudioServerInnerCall *result = audioInnerCall->GetIAudioServerInnerCall();
    EXPECT_EQ(result, nullptr);
    delete audioInnerCall;
}
} // namespace AudioStandard
} // namespace OHOS