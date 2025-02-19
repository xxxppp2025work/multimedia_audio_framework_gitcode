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
#include "audio_errors.h"
#include "format_converter.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

static int32_t NUM_0 = 0;

class FormatConverterUnitTest : public ::testing::Test {
public:
    void SetUp();
    void TearDown();
};

void FormatConverterUnitTest::SetUp(void)
{
    // input testcase setup step，setup invoked before each testcases
}

void FormatConverterUnitTest::TearDown(void)
{
    // input testcase teardown step，teardown invoked after each testcases
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_001
 * @tc.desc  : Test FormatConverter::S16MonoToS16Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_001, TestSize.Level1)
{
    BufferDesc srcDesc = {nullptr, 1, 0};
    BufferDesc dstDesc = {nullptr, 4, 0};

    auto ret = FormatConverter::S16MonoToS16Stereo(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_002
 * @tc.desc  : Test FormatConverter::S16MonoToS16Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_002, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {nullptr, 1, 0};
    BufferDesc dstDesc = {&buffer, 2, 0};

    EXPECT_NE(dstDesc.buffer, nullptr);

    auto ret = FormatConverter::S16MonoToS16Stereo(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_003
 * @tc.desc  : Test FormatConverter::S16MonoToS16Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_003, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {&buffer, 1, 0};
    BufferDesc dstDesc = {nullptr, 2, 0};

    EXPECT_NE(srcDesc.buffer, nullptr);

    auto ret = FormatConverter::S16MonoToS16Stereo(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_004
 * @tc.desc  : Test FormatConverter::S16MonoToS16Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_004, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {&buffer, 1, 0};
    BufferDesc dstDesc = {&buffer, 2, 0};

    EXPECT_NE(srcDesc.buffer, nullptr);
    EXPECT_NE(dstDesc.buffer, nullptr);

    auto ret = FormatConverter::S16MonoToS16Stereo(srcDesc, dstDesc);
    EXPECT_EQ(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_005
 * @tc.desc  : Test FormatConverter::S16StereoToS16Mono().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_005, TestSize.Level1)
{
    BufferDesc srcDesc = {nullptr, 4, 0};
    BufferDesc dstDesc = {nullptr, 1, 0};

    auto ret = FormatConverter::S16StereoToS16Mono(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_006
 * @tc.desc  : Test FormatConverter::S16StereoToS16Mono().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_006, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {nullptr, 2, 0};
    BufferDesc dstDesc = {&buffer, 1, 0};

    EXPECT_NE(dstDesc.buffer, nullptr);

    auto ret = FormatConverter::S16StereoToS16Mono(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_007
 * @tc.desc  : Test FormatConverter::S16StereoToS16Mono().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_007, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {&buffer, 2, 0};
    BufferDesc dstDesc = {nullptr, 2, 0};

    EXPECT_NE(srcDesc.buffer, nullptr);

    auto ret = FormatConverter::S16StereoToS16Mono(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_008
 * @tc.desc  : Test FormatConverter::S16StereoToS16Mono().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_008, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {&buffer, 2, 0};
    BufferDesc dstDesc = {&buffer, 1, 0};

    EXPECT_NE(srcDesc.buffer, nullptr);
    EXPECT_NE(dstDesc.buffer, nullptr);

    auto ret = FormatConverter::S16StereoToS16Mono(srcDesc, dstDesc);
    EXPECT_EQ(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_009
 * @tc.desc  : Test FormatConverter::S16StereoToF32Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_009, TestSize.Level1)
{
    BufferDesc srcDesc = {nullptr, 1, 0};
    BufferDesc dstDesc = {nullptr, 4, 0};

    auto ret = FormatConverter::S16StereoToF32Stereo(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_010
 * @tc.desc  : Test FormatConverter::S16StereoToF32Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_010, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {nullptr, 1, 0};
    BufferDesc dstDesc = {&buffer, 2, 0};

    EXPECT_NE(dstDesc.buffer, nullptr);

    auto ret = FormatConverter::S16StereoToF32Stereo(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_011
 * @tc.desc  : Test FormatConverter::S16StereoToF32Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_011, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {&buffer, 1, 0};
    BufferDesc dstDesc = {nullptr, 2, 0};

    EXPECT_NE(srcDesc.buffer, nullptr);

    auto ret = FormatConverter::S16StereoToF32Stereo(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_012
 * @tc.desc  : Test FormatConverter::S16StereoToF32Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_012, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {&buffer, 1, 0};
    BufferDesc dstDesc = {&buffer, 2, 0};

    EXPECT_NE(srcDesc.buffer, nullptr);
    EXPECT_NE(dstDesc.buffer, nullptr);

    auto ret = FormatConverter::S16StereoToF32Stereo(srcDesc, dstDesc);
    EXPECT_EQ(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_013
 * @tc.desc  : Test FormatConverter::S16StereoToF32Mono().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_013, TestSize.Level1)
{
    BufferDesc srcDesc = {nullptr, 1, 0};
    BufferDesc dstDesc = {nullptr, 1, 0};

    auto ret = FormatConverter::S16StereoToF32Mono(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_014
 * @tc.desc  : Test FormatConverter::S16StereoToF32Mono().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_014, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {nullptr, 1, 0};
    BufferDesc dstDesc = {&buffer, 1, 0};

    EXPECT_NE(dstDesc.buffer, nullptr);

    auto ret = FormatConverter::S16StereoToF32Mono(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_015
 * @tc.desc  : Test FormatConverter::S16StereoToF32Mono().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_015, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {&buffer, 1, 0};
    BufferDesc dstDesc = {nullptr, 1, 0};

    EXPECT_NE(srcDesc.buffer, nullptr);

    auto ret = FormatConverter::S16StereoToF32Mono(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_016
 * @tc.desc  : Test FormatConverter::S16StereoToF32Mono().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_016, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {&buffer, 1, 0};
    BufferDesc dstDesc = {&buffer, 1, 0};

    EXPECT_NE(srcDesc.buffer, nullptr);
    EXPECT_NE(dstDesc.buffer, nullptr);

    auto ret = FormatConverter::S16StereoToF32Mono(srcDesc, dstDesc);
    EXPECT_EQ(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_017
 * @tc.desc  : Test FormatConverter::F32MonoToS16Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_017, TestSize.Level1)
{
    BufferDesc srcDesc = {nullptr, 1, 0};
    BufferDesc dstDesc = {nullptr, 1, 0};

    auto ret = FormatConverter::F32MonoToS16Stereo(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_018
 * @tc.desc  : Test FormatConverter::F32MonoToS16Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_018, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {nullptr, 1, 0};
    BufferDesc dstDesc = {&buffer, 1, 0};

    EXPECT_NE(dstDesc.buffer, nullptr);

    auto ret = FormatConverter::F32MonoToS16Stereo(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_019
 * @tc.desc  : Test FormatConverter::F32MonoToS16Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_019, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {&buffer, 1, 0};
    BufferDesc dstDesc = {nullptr, 1, 0};

    EXPECT_NE(srcDesc.buffer, nullptr);

    auto ret = FormatConverter::F32MonoToS16Stereo(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_020
 * @tc.desc  : Test FormatConverter::F32MonoToS16Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_020, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {&buffer, 1, 0};
    BufferDesc dstDesc = {&buffer, 1, 0};

    EXPECT_NE(srcDesc.buffer, nullptr);
    EXPECT_NE(dstDesc.buffer, nullptr);

    auto ret = FormatConverter::F32MonoToS16Stereo(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_021
 * @tc.desc  : Test FormatConverter::F32MonoToS16Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_021, TestSize.Level1)
{
    float srcData[] = {1.0f, 2.0f};
    size_t srcSize = sizeof(srcData);

    int16_t dstData[4] = {0};
    size_t dstSize = sizeof(dstData);

    BufferDesc srcDesc = {
        .buffer = reinterpret_cast<uint8_t *>(srcData),
        .bufLength = srcSize,
        .dataLength = srcSize,
        .metaBuffer = nullptr,
        .metaLength = 0
    };

    BufferDesc dstDesc = {
        .buffer = reinterpret_cast<uint8_t *>(dstData),
        .bufLength = dstSize,
        .dataLength = dstSize,
        .metaBuffer = nullptr,
        .metaLength = 0
    };

    EXPECT_NE(srcDesc.buffer, nullptr);
    EXPECT_NE(dstDesc.buffer, nullptr);

    auto ret = FormatConverter::F32MonoToS16Stereo(srcDesc, dstDesc);
    EXPECT_EQ(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_022
 * @tc.desc  : Test FormatConverter::F32StereoToS16Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_022, TestSize.Level1)
{
    BufferDesc srcDesc = {nullptr, 1, 0};
    BufferDesc dstDesc = {nullptr, 1, 0};

    auto ret = FormatConverter::F32StereoToS16Stereo(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_023
 * @tc.desc  : Test FormatConverter::F32StereoToS16Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_023, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {nullptr, 1, 0};
    BufferDesc dstDesc = {&buffer, 1, 0};

    EXPECT_NE(dstDesc.buffer, nullptr);

    auto ret = FormatConverter::F32StereoToS16Stereo(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_024
 * @tc.desc  : Test FormatConverter::F32StereoToS16Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_024, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {&buffer, 1, 0};
    BufferDesc dstDesc = {nullptr, 1, 0};

    EXPECT_NE(srcDesc.buffer, nullptr);

    auto ret = FormatConverter::F32StereoToS16Stereo(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_025
 * @tc.desc  : Test FormatConverter::F32StereoToS16Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_025, TestSize.Level1)
{
    uint8_t buffer = 1;
    BufferDesc srcDesc = {&buffer, 1, 0};
    BufferDesc dstDesc = {&buffer, 1, 0};

    EXPECT_NE(srcDesc.buffer, nullptr);
    EXPECT_NE(dstDesc.buffer, nullptr);

    auto ret = FormatConverter::F32StereoToS16Stereo(srcDesc, dstDesc);
    EXPECT_NE(ret, NUM_0);
}

/**
 * @tc.name  : Test FormatConverter.
 * @tc.type  : FUNC
 * @tc.number: FormatConverter_026
 * @tc.desc  : Test FormatConverter::F32StereoToS16Stereo().
 */
HWTEST_F(FormatConverterUnitTest, FormatConverter_026, TestSize.Level1)
{
    float srcData[] = {1.0f, 2.0f, 3.0f, 4.0f};
    size_t srcSize = sizeof(srcData);

    int16_t dstData[4] = {0};
    size_t dstSize = sizeof(dstData);

    BufferDesc srcDesc = {
        .buffer = reinterpret_cast<uint8_t *>(srcData),
        .bufLength = srcSize,
        .dataLength = srcSize,
        .metaBuffer = nullptr,
        .metaLength = 0
    };

    BufferDesc dstDesc = {
        .buffer = reinterpret_cast<uint8_t *>(dstData),
        .bufLength = dstSize,
        .dataLength = dstSize,
        .metaBuffer = nullptr,
        .metaLength = 0
    };
    EXPECT_NE(srcDesc.buffer, nullptr);
    EXPECT_NE(dstDesc.buffer, nullptr);

    auto ret = FormatConverter::F32StereoToS16Stereo(srcDesc, dstDesc);
    EXPECT_EQ(ret, NUM_0);
}
}
}