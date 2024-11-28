/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "hdi_utils_ringbuffer_unit_test.h"
#include <climits>

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {
void HdiUtilsRingBufferUnitTest::SetUpTestCase(void) {}

void HdiUtilsRingBufferUnitTest::TearDownTestCase(void) {}

void HdiUtilsRingBufferUnitTest::SetUp(void) {}

void HdiUtilsRingBufferUnitTest::TearDown(void) {}

/**
 * @tc.name  : Test HdiRingBuffer API
 * @tc.number: HdiRingBuffer_001
 * @tc.desc  : Test HdiRingBuffer::Init()
 */
HWTEST(HdiUtilsRingBufferUnitTest, HdiRingBuffer_001, TestSize.Level0)
{
    std::shared_ptr<HdiRingBuffer> ptrHdiRingBuffer = std::make_shared<HdiRingBuffer>();
    ASSERT_NE(ptrHdiRingBuffer, nullptr);

    const uint32_t sampleRate = 1;
    const uint32_t channelCount = 1;
    const uint32_t formatBytes = 1;
    const uint32_t onceFrameNum = 1;
    const uint32_t maxFrameNum = 1;

    ptrHdiRingBuffer->Init(sampleRate, channelCount, formatBytes, onceFrameNum, maxFrameNum);
}

/**
 * @tc.name  : Test HdiRingBuffer API
 * @tc.number: HdiRingBuffer_002
 * @tc.desc  : Test HdiRingBuffer::Init()
 */
HWTEST(HdiUtilsRingBufferUnitTest, HdiRingBuffer_002, TestSize.Level0)
{
    std::shared_ptr<HdiRingBuffer> ptrHdiRingBuffer = std::make_shared<HdiRingBuffer>();
    ASSERT_NE(ptrHdiRingBuffer, nullptr);

    const uint32_t sampleRate = 1;
    const uint32_t channelCount = 1;
    const uint32_t formatBytes = 1;
    const uint32_t onceFrameNum = 1;
    const uint32_t maxFrameNum = 0;

    ptrHdiRingBuffer->Init(sampleRate, channelCount, formatBytes, onceFrameNum, maxFrameNum);
}

/**
 * @tc.name  : Test HdiRingBuffer API
 * @tc.number: HdiRingBuffer_003
 * @tc.desc  : Test HdiRingBuffer::Init()
 */
HWTEST(HdiUtilsRingBufferUnitTest, HdiRingBuffer_003, TestSize.Level0)
{
    std::shared_ptr<HdiRingBuffer> ptrHdiRingBuffer = std::make_shared<HdiRingBuffer>();
    ASSERT_NE(ptrHdiRingBuffer, nullptr);

    const uint32_t sampleRate = 1e8;
    const uint32_t channelCount = 1e8;
    const uint32_t formatBytes = 1e8;
    const uint32_t onceFrameNum = 1e8;
    const uint32_t maxFrameNum = 1;

    ptrHdiRingBuffer->Init(sampleRate, channelCount, formatBytes, onceFrameNum, maxFrameNum);
}

/**
 * @tc.name  : Test HdiRingBuffer API
 * @tc.number: HdiRingBuffer_004
 * @tc.desc  : Test HdiRingBuffer::Init()
 */
HWTEST(HdiUtilsRingBufferUnitTest, HdiRingBuffer_004, TestSize.Level0)
{
    std::shared_ptr<HdiRingBuffer> ptrHdiRingBuffer = std::make_shared<HdiRingBuffer>();
    ASSERT_NE(ptrHdiRingBuffer, nullptr);

    const uint32_t sampleRate = UINT32_MAX;
    const uint32_t channelCount = 1;
    const uint32_t formatBytes = 1;
    const uint32_t onceFrameNum = 1;
    const uint32_t maxFrameNum = 100;

    ptrHdiRingBuffer->Init(sampleRate, channelCount, formatBytes, onceFrameNum, maxFrameNum);
}

/**
 * @tc.name  : Test HdiRingBuffer API
 * @tc.number: HdiRingBuffer_005
 * @tc.desc  : Test HdiRingBuffer::AddWriteIndex()
 */
HWTEST(HdiUtilsRingBufferUnitTest, HdiRingBuffer_005, TestSize.Level0)
{
    std::shared_ptr<HdiRingBuffer> ptrHdiRingBuffer = std::make_shared<HdiRingBuffer>();
    ASSERT_NE(ptrHdiRingBuffer, nullptr);

    ptrHdiRingBuffer->writeIdx_ = 5;

    ptrHdiRingBuffer->AddWriteIndex();
    EXPECT_EQ(ptrHdiRingBuffer->writeIdx_, 6);
}

/**
 * @tc.name  : Test HdiRingBuffer API
 * @tc.number: HdiRingBuffer_006
 * @tc.desc  : Test HdiRingBuffer::AddWriteIndex()
 */
HWTEST(HdiUtilsRingBufferUnitTest, HdiRingBuffer_006, TestSize.Level0)
{
    std::shared_ptr<HdiRingBuffer> ptrHdiRingBuffer = std::make_shared<HdiRingBuffer>();
    ASSERT_NE(ptrHdiRingBuffer, nullptr);

    ptrHdiRingBuffer->writeIdx_ = UINT64_MAX;

    ptrHdiRingBuffer->AddWriteIndex();
    EXPECT_EQ(ptrHdiRingBuffer->writeIdx_, UINT64_MAX / 100 + 1);
}

/**
 * @tc.name  : Test HdiRingBuffer API
 * @tc.number: HdiRingBuffer_007
 * @tc.desc  : Test HdiRingBuffer::AddReadIndex()
 */
HWTEST(HdiUtilsRingBufferUnitTest, HdiRingBuffer_007, TestSize.Level0)
{
    std::shared_ptr<HdiRingBuffer> ptrHdiRingBuffer = std::make_shared<HdiRingBuffer>();
    ASSERT_NE(ptrHdiRingBuffer, nullptr);

    ptrHdiRingBuffer->readIdx_ = 5;

    ptrHdiRingBuffer->AddReadIndex();
    EXPECT_EQ(ptrHdiRingBuffer->readIdx_, 6);
}

/**
 * @tc.name  : Test HdiRingBuffer API
 * @tc.number: HdiRingBuffer_008
 * @tc.desc  : Test HdiRingBuffer::AddReadIndex()
 */
HWTEST(HdiUtilsRingBufferUnitTest, HdiRingBuffer_008, TestSize.Level0)
{
    std::shared_ptr<HdiRingBuffer> ptrHdiRingBuffer = std::make_shared<HdiRingBuffer>();
    ASSERT_NE(ptrHdiRingBuffer, nullptr);

    ptrHdiRingBuffer->readIdx_ = UINT64_MAX;

    ptrHdiRingBuffer->AddReadIndex();
    EXPECT_EQ(ptrHdiRingBuffer->readIdx_, 0);
}

} // namespace AudioStandard
} // namespace OHOS