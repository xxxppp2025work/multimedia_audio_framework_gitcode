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
class FormatConverterUnitTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void FormatConverterUnitTest::SetUpTestCase(void)
{
}

void FormatConverterUnitTest::TearDownTestCase(void)
{
}

void FormatConverterUnitTest::SetUp(void)
{
}

void FormatConverterUnitTest::TearDown(void)
{
}

/**
 * @tc.name  : Test FormatConverter API
 * @tc.type  : FUNC
 * @tc.number: S16StereoToF32Stereo_001
 * @tc.desc  : Test FormatConverter interface.
 */
HWTEST_F(FormatConverterUnitTest, S16StereoToF32Stereo_001, TestSize.Level1)
{
    BufferDesc srcDesc;
    BufferDesc dstDesc;
    int32_t ret = -1;
    uint8_t srcBuffer[4] = {0};
    uint8_t dstBuffer[8] = {0};

    srcDesc.bufLength = 4;
    srcDesc.buffer = srcBuffer;
    dstDesc.bufLength = 2;
    dstDesc.buffer = dstBuffer;

    ret = FormatConverter::S16StereoToF32Stereo(srcDesc, dstDesc);
    EXPECT_EQ(ret, -1);

    dstDesc.bufLength = 8;

    ret = FormatConverter::S16StereoToF32Stereo(srcDesc, dstDesc);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name  : Test FormatConverter API
 * @tc.type  : FUNC
 * @tc.number: S16StereoToF32Mono_001
 * @tc.desc  : Test FormatConverter interface.
 */
HWTEST_F(FormatConverterUnitTest, S16StereoToF32Mono_001, TestSize.Level1)
{
    BufferDesc srcDesc;
    BufferDesc dstDesc;
    int32_t ret = -1;
    uint8_t srcBuffer[4] = {0};
    uint8_t dstBuffer[4] = {0};

    srcDesc.bufLength = 4;
    srcDesc.buffer = srcBuffer;
    dstDesc.bufLength = 2;
    dstDesc.buffer = dstBuffer;

    ret = FormatConverter::S16StereoToF32Mono(srcDesc, dstDesc);
    EXPECT_EQ(ret, -1);

    dstDesc.bufLength = 4;

    ret = FormatConverter::S16StereoToF32Mono(srcDesc, dstDesc);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name  : Test FormatConverter API
 * @tc.type  : FUNC
 * @tc.number: F32MonoToS16Stereo_001
 * @tc.desc  : Test FormatConverter interface.
 */
HWTEST_F(FormatConverterUnitTest, F32MonoToS16Stereo_001, TestSize.Level1)
{
    BufferDesc srcDesc;
    BufferDesc dstDesc;
    int32_t ret = -1;
    uint8_t srcBuffer[4] = {0};
    uint8_t dstBuffer[4] = {0};

    srcDesc.bufLength = 4;
    srcDesc.buffer = srcBuffer;
    dstDesc.bufLength = 2;
    dstDesc.buffer = dstBuffer;

    ret = FormatConverter::F32MonoToS16Stereo(srcDesc, dstDesc);
    EXPECT_EQ(ret, -1);

    dstDesc.bufLength = 4;

    ret = FormatConverter::F32MonoToS16Stereo(srcDesc, dstDesc);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name  : Test FormatConverter API
 * @tc.type  : FUNC
 * @tc.number: F32StereoToS16Stereo_001
 * @tc.desc  : Test FormatConverter interface.
 */
HWTEST_F(FormatConverterUnitTest, F32StereoToS16Stereo_001, TestSize.Level1)
{
    BufferDesc srcDesc;
    BufferDesc dstDesc;
    int32_t ret = -1;
    uint8_t srcBuffer[8] = {0};
    uint8_t dstBuffer[4] = {0};

    srcDesc.bufLength = 8;
    srcDesc.buffer = srcBuffer;
    dstDesc.bufLength = 2;
    dstDesc.buffer = dstBuffer;

    ret = FormatConverter::F32StereoToS16Stereo(srcDesc, dstDesc);
    EXPECT_EQ(ret, -1);

    dstDesc.bufLength = 4;

    ret = FormatConverter::F32StereoToS16Stereo(srcDesc, dstDesc);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name  : Test FormatConverter API
 * @tc.type  : FUNC
 * @tc.number: DataAccumulationFromVolume_Normal_001
 * @tc.desc  : Test FormatConverter interface.
 */
HWTEST_F(FormatConverterUnitTest, DataAccumulationFromVolume_Normal_001, TestSize.Level1)
{
    // 构造两个音源数据，音量均为0.5（32768/65536）
    std::vector<AudioStreamData> srcDataList;
    const int16_t SAMPLE_VALUE = 1000;
    const int32_t VOLUME_HALF = 32768; // 0.5 in Q16格式

    // 第一个音源
    AudioStreamData srcData1;
    int16_t srcBuffer1[2] = {SAMPLE_VALUE, SAMPLE_VALUE * 2};
    BufferDesc srcDesc1;
    srcDesc1.dataLength = sizeof(srcBuffer1);
    srcDesc1.buffer = reinterpret_cast<uint8_t*>(srcBuffer1);
    AudioStreamInfo srcInfo1;
    srcData1.bufferDesc = srcDesc1;
    srcData1.streamInfo = srcInfo1;
    srcData1.volumeStart = SAMPLE_VALUE;
    srcData1.volumeEnd = VOLUME_HALF;
    srcData1.isInnerCapeds[0] = true;
    srcDataList.push_back(srcData1);

    // 第二个音源
    int16_t srcBuffer2[2] = {SAMPLE_VALUE, SAMPLE_VALUE * 2};
    BufferDesc srcDesc2;
    srcDesc2.dataLength = sizeof(srcBuffer2);
    srcDesc2.buffer = reinterpret_cast<uint8_t*>(srcBuffer2);
    AudioStreamData srcData2;
    srcData2.bufferDesc = srcDesc2;
    srcData2.streamInfo = srcInfo1;
    srcData2.volumeStart = SAMPLE_VALUE;
    srcData2.volumeEnd = VOLUME_HALF;
    srcData2.isInnerCapeds[0] = true;
    srcDataList.push_back(srcData2);

    // 目标缓冲区（预期混合结果：1000*0.5 + 1000*0.5 = 1000）
    int16_t dstBuffer[2] = {0};
    BufferDesc dstDesc;
    dstDesc.dataLength = sizeof(dstBuffer);
    dstDesc.buffer = reinterpret_cast<uint8_t*>(dstBuffer);

    AudioStreamData dstData;
    dstData.bufferDesc = dstDesc;
    

    // 执行混合操作
    FormatConverter::DataAccumulationFromVolume(srcDataList, dstData);

    // 验证结果
    EXPECT_EQ(dstBuffer[0], SAMPLE_VALUE);        // 1000
    EXPECT_EQ(dstBuffer[1], SAMPLE_VALUE * 2);    // 2000
}

#if 0
/**
 * @tc.name  : Test FormatConverter API
 * @tc.type  : FUNC
 * @tc.number: DataAccumulationFromVolume_ClampMax_002
 * @tc.desc  : Test FormatConverter interface.
 */
HWTEST_F(FormatConverterUnitTest, DataAccumulationFromVolume_ClampMax_002, TestSize.Level1)
{
    // 构造两个满音量音源，触发INT16_MAX截断
    std::vector<AudioStreamData> srcDataList;
    const int32_t VOLUME_FULL = 65536; // 1.0 in Q16格式

    // 两个音源值为INT16_MAX
    int16_t srcBuffer[2] = {INT16_MAX, INT16_MAX};
    BufferDesc srcDesc;
    srcDesc.dataLength = sizeof(srcBuffer);
    srcDesc.buffer = reinterpret_cast<uint8_t*>(srcBuffer);
    srcDataList.push_back({srcDesc, VOLUME_FULL});
    srcDataList.push_back({srcDesc, VOLUME_FULL}); // 叠加后超出范围

    // 目标缓冲区
    int16_t dstBuffer[2] = {0};
    BufferDesc dstDesc;
    dstDesc.dataLength = sizeof(dstBuffer);
    dstDesc.buffer = reinterpret_cast<uint8_t*>(dstBuffer);
    AudioStreamData dstData = {dstDesc};

    // 执行混合
    FormatConverter::DataAccumulationFromVolume(srcDataList, dstData);

    // 验证结果被限制在INT16_MAX
    EXPECT_EQ(dstBuffer[0], INT16_MAX);
    EXPECT_EQ(dstBuffer[1], INT16_MAX);
}

/**
 * @tc.name  : Test FormatConverter API
 * @tc.type  : FUNC
 * @tc.number: DataAccumulationFromVolume_EmptySource_003
 * @tc.desc  : Test FormatConverter interface.
 */
HWTEST_F(FormatConverterUnitTest, DataAccumulationFromVolume_EmptySource_003, TestSize.Level1)
{
    // 空音源列表测试
    std::vector<AudioStreamData> srcDataList;
    int16_t dstBuffer[2] = {1234, 5678}; // 初始非零值

    BufferDesc dstDesc;
    dstDesc.dataLength = sizeof(dstBuffer);
    dstDesc.buffer = reinterpret_cast<uint8_t*>(dstBuffer);
    AudioStreamData dstData = {dstDesc};

    // 执行混合（应填充0）
    FormatConverter::DataAccumulationFromVolume(srcDataList, dstData);

    // 验证目标缓冲区被清零
    EXPECT_EQ(dstBuffer[0], 0);
    EXPECT_EQ(dstBuffer[1], 0);
}
#endif

}  // namespace OHOS::AudioStandard
}  // namespace OHOS
