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
#include <gtest/gtest.h>
#include "audio_stream_checker.h"
#include "audio_errors.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

class AudioStreamCheckerTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void AudioStreamCheckerTest::SetUpTestCase(void)
{
    // input testsuit setup step，setup invoked before all testcases
}

void AudioStreamCheckerTest::TearDownTestCase(void)
{
    // input testsuit teardown step，teardown invoked after all testcases
}

void AudioStreamCheckerTest::SetUp(void)
{
    // input testcase setup step，setup invoked before each testcases
}

void AudioStreamCheckerTest::TearDown(void)
{
    // input testcase teardown step，teardown invoked after each testcases
}

/**
 * @tc.name  : Test InitChecker API
 * @tc.type  : FUNC
 * @tc.number: InitCheckerTest_001
 */
HWTEST(AudioStreamCheckerTest, InitCheckerTest_001, TestSize.Level1)
{
    int32_t ret = SUCCESS;
    AudioProcessConfig cfg;
    DataTransferMonitorParam para;
    std::shared_ptr<AudioStreamChecker> checker = std::make_shared<AudioStreamChecker>(cfg);
    checker->InitChecker(para, 100000, 100000);
    checker->InitChecker(para, 100000, 100000);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test RecordFrame API
 * @tc.type  : FUNC
 * @tc.number: RecordFrame_001
 */
HWTEST(AudioStreamCheckerTest, RecordFrame_001, TestSize.Level1)
{
    int32_t ret = SUCCESS;
    AudioProcessConfig cfg;
    DataTransferMonitorParam para;
    std::shared_ptr<AudioStreamChecker> checker = std::make_shared<AudioStreamChecker>(cfg);
    checker->InitChecker(para, 100000, 100000);
    checker->RecordMuteFrame();
    checker->RecordNodataFrame();
    checker->RecordNormalFrame();
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test GetAppUid API
 * @tc.type  : FUNC
 * @tc.number: GetAppUid_001
 */
HWTEST(AudioStreamCheckerTest, GetAppUid_001, TestSize.Level1)
{
    AudioProcessConfig cfg;
    cfg.appInfo.appUid = 20002000;
    std::shared_ptr<AudioStreamChecker> checker = std::make_shared<AudioStreamChecker>(cfg);
    int32_t uid = checker->GetAppUid();
    EXPECT_EQ(uid, 20002000);
}

/**
 * @tc.name  : Test DeleteCheckerPara API
 * @tc.type  : FUNC
 * @tc.number: DeleteCheckerPara_001
 */
HWTEST(AudioStreamCheckerTest, DeleteCheckerPara_001, TestSize.Level1)
{
    int32_t ret = SUCCESS;
    AudioProcessConfig cfg;
    DataTransferMonitorParam para;
    std::shared_ptr<AudioStreamChecker> checker = std::make_shared<AudioStreamChecker>(cfg);
    checker->InitChecker(para, 100000, 100000);
    checker->DeleteCheckerPara(100000, 100000);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test MonitorCheckFrame API
 * @tc.type  : FUNC
 * @tc.number: MonitorCheckFrame_001
 */
HWTEST(AudioStreamCheckerTest, MonitorCheckFrame_001, TestSize.Level1)
{
    int32_t ret = SUCCESS;
    AudioProcessConfig cfg;
    DataTransferMonitorParam para;
    para.badDataTransferTypeBitMap = 3;
    para.timeInterval = 0;
    para.badFramesRatio = 50;
    std::shared_ptr<AudioStreamChecker> checker = std::make_shared<AudioStreamChecker>(cfg);
    checker->InitChecker(para, 100000, 100000);
    checker->RecordMuteFrame();
    checker->RecordNormalFrame();
    checker->MonitorCheckFrame();
    checker->MonitorCheckFrame();
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test MonitorCheckFrame API
 * @tc.type  : FUNC
 * @tc.number: MonitorCheckFrame_002
 */
HWTEST(AudioStreamCheckerTest, MonitorCheckFrame_002, TestSize.Level1)
{
    int32_t ret = SUCCESS;
    AudioProcessConfig cfg;
    DataTransferMonitorParam para;
    para.badDataTransferTypeBitMap = 3;
    para.timeInterval = 0;
    para.badFramesRatio = 50;
    std::shared_ptr<AudioStreamChecker> checker = std::make_shared<AudioStreamChecker>(cfg);
    checker->InitChecker(para, 100000, 100000);
    checker->RecordNormalFrame();
    checker->MonitorCheckFrame();
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test MonitorCheckFrame API
 * @tc.type  : FUNC
 * @tc.number: MonitorCheckFrame_003
 */
HWTEST(AudioStreamCheckerTest, MonitorCheckFrame_003, TestSize.Level1)
{
    int32_t ret = SUCCESS;
    AudioProcessConfig cfg;
    DataTransferMonitorParam para;
    para.badDataTransferTypeBitMap = 3;
    para.timeInterval = 0;
    para.badFramesRatio = 50;
    std::shared_ptr<AudioStreamChecker> checker = std::make_shared<AudioStreamChecker>(cfg);
    checker->InitChecker(para, 100000, 100000);
    checker->RecordMuteFrame();
    checker->RecordNormalFrame();
    checker->MonitorCheckFrame();
    for (int i = 0; i < 4; i++) {
        checker->RecordNormalFrame();
    }
    checker->MonitorCheckFrame();
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test MonitorOnAllCallback API
 * @tc.type  : FUNC
 * @tc.number: MonitorOnAllCallback_001
 */
HWTEST(AudioStreamCheckerTest, MonitorOnAllCallback_001, TestSize.Level1)
{
    int32_t ret = SUCCESS;
    AudioProcessConfig cfg;
    DataTransferMonitorParam para;
    para.badDataTransferTypeBitMap = 3;
    para.timeInterval = 2000000000;
    para.badFramesRatio = 50;
    std::shared_ptr<AudioStreamChecker> checker = std::make_shared<AudioStreamChecker>(cfg);
    checker->InitChecker(para, 100000, 100000);
    checker->MonitorOnAllCallback(AUDIO_STREAM_START);
    EXPECT_EQ(ret, SUCCESS);
}

}
}