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

#include "audio_schedule.h"
#include "audio_schedule_guard.h"
#include "parameter.h"

#include <gtest/gtest.h>
#include <pthread.h>
#include <thread>
#include <chrono>

using namespace testing::ext;
using namespace std;

namespace OHOS {
namespace AudioStandard {

const int32_t HIGH_LEVEL_THREAD_PRIORITY = 4;
const int32_t AUDIO_DEFAULT_PRIORITY = 1;

int32_t GetIntParameter(const char* key, int32_t defaultValue)
{
    return defaultValue;
}

void AUDIO_INFO_LOG(const char* format, ...) {}
void AUDIO_ERR_LOG(const char* format, ...) {}

void ScheduleReportData(unit32_t pid, unit32_t tid, const char* bundleName) {}

int pthread_getschedparam(pthread_t thread, int* policy, struct sched_param* param)
{
    *policy = SCHED_RR;
    return 0;
};

int sched_setscheduler(pid_t pid, int policy, struct sched_param* param)
{
    return 0;
};

class AudioScheduleUnitTest : public ::testing::Test{
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp();
    virtual void TearDown() {}
  
};

/**
 * @tc.name   : Test AudioScheduleUnit 
 * @tc.number : AudioScheduleUnitTest_001
 * @tc.desc   : Test AudioScheduleUnitTest_001
 */
HWTEST_F(AudioScheduleUnitTest, AudioScheduleUnitTest_001,  TestSize.Level1)
{
    //
    SetProcessDataThreadPriority(1);
    GetIntParameter("const.multimedia.audio_setPriority", AUDIO_DEFAULT_PRIORITY);
    EXPECT_TRUE(SetEndpointThreadPriority());
}

/**
 * @tc.name   : Test AudioScheduleUnit 
 * @tc.number : AudioScheduleUnitTest_002
 * @tc.desc   : Test AudioScheduleUnitTest_002
 */
HWTEST_F(AudioScheduleUnitTest, AudioScheduleUnitTest_002,  TestSize.Level1)
{
    //
    SetProcessDataThreadPriority(4);
    GetIntParameter("const.multimedia.audio_setPriority", HIGH_LEVEL_THREAD_PRIORITY);
    EXPECT_TRUE(SetEndpointThreadPriority());
}

} // namespace AudioStandard
} // namespace OHOS
