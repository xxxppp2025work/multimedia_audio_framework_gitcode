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

#include "audio_effect_config_parser_test.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

void AudioEffectConfigParserTest::SetUpTestCase(void) {}
void AudioEffectConfigParserTest::TearDownTestCase(void) {}
void AudioEffectConfigParserTest::SetUp(void) {}

void AudioEffectConfigParserTest::TearDown(void) {}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_001
* @tc.desc  : Test AudioEffectConfigParser interfaces.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_001, TestSize.Level1)
{
    OriginalEffectConfig result;
    AudioEffectConfigParser effectConfigParserTest;

    int32_t ret = effectConfigParserTest.LoadEffectConfig(result);
    EXPECT_NE(ret, 0);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_002
* @tc.desc  : Test static LoadEffectConfigLibraries.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_002, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {0, 0, 0, 0, 0, 0};

    LoadEffectConfigLibraries(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_003
* @tc.desc  : Test static LoadEffectConfigLibraries.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_003, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {5, 5, 5, 5, 5, 5};

    LoadEffectConfigLibraries(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_004
* @tc.desc  : Test static LoadEffectConfigLibraries.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_004, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {1, 1, 1, 1, 1, 1};

    LoadEffectConfigLibraries(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_005
* @tc.desc  : Test static LoadEffectConfigEffects.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_005, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {0, 0, 0, 0, 0, 0};

    LoadEffectConfigEffects(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_006
* @tc.desc  : Test static LoadEffectConfigEffects.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_006, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {5, 5, 5, 5, 5, 5};

    LoadEffectConfigEffects(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_007
* @tc.desc  : Test static LoadEffectConfigEffects.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_007, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {1, 1, 1, 1, 1, 1};

    LoadEffectConfigEffects(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_008
* @tc.desc  : Test static LoadEffectConfigEffectChains.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_008, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {0, 0, 0, 0, 0, 0};

    LoadEffectConfigEffectChains(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_009
* @tc.desc  : Test static LoadEffectConfigEffectChains.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_009, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {5, 5, 5, 5, 5, 5};

    LoadEffectConfigEffectChains(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_010
* @tc.desc  : Test static LoadEffectConfigEffectChains.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_010, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {1, 1, 1, 1, 1, 1};

    LoadEffectConfigEffectChains(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_011
* @tc.desc  : Test static LoadEffectConfigEffectChains.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_011, TestSize.Level1)
{
    std::vector<PreStreamScene> scenes;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t nodeCounter = 0;

    LoadPreStreamScenesCheck(scenes, currNode, nodeCounter);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_012
* @tc.desc  : Test static LoadEffectConfigEffectChains.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_012, TestSize.Level1)
{
    std::vector<PreStreamScene> scenes;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t nodeCounter = 5;

    LoadPreStreamScenesCheck(scenes, currNode, nodeCounter);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_013
* @tc.desc  : Test static LoadEffectConfigEffectChains.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_013, TestSize.Level1)
{
    std::vector<PreStreamScene> scenes;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t nodeCounter = 1;

    LoadPreStreamScenesCheck(scenes, currNode, nodeCounter);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_014
* @tc.desc  : Test static LoadPreprocessExceptionCheck.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_014, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE_PRE] = {0, 0, 0, 0};

    LoadPreprocessExceptionCheck(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_015
* @tc.desc  : Test static LoadPreprocessExceptionCheck.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_015, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE_PRE] = {5, 5, 5, 5};

    LoadPreprocessExceptionCheck(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_016
* @tc.desc  : Test static LoadPreprocessExceptionCheck.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_016, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE_PRE] = {1, 1, 1, 1};

    LoadPreprocessExceptionCheck(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_017
* @tc.desc  : Test static LoadPreProcessCfg.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_017, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    currNode->type = XML_ELEMENT_NODE;

    LoadPreProcessCfg(result, currNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_018
* @tc.desc  : Test static LoadPreProcessCfg.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_018, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    currNode->type = XML_ATTRIBUTE_NODE;

    LoadPreProcessCfg(result, currNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_019
* @tc.desc  : Test static LoadEffectConfigPreProcessCfg.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_019, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {0, 0, 0, 0, 0, 0};

    LoadEffectConfigPreProcessCfg(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_020
* @tc.desc  : Test static LoadEffectConfigPreProcessCfg.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_020, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {5, 5, 5, 5, 5, 5};

    LoadEffectConfigPreProcessCfg(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_021
* @tc.desc  : Test static LoadEffectConfigPreProcessCfg.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_021, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {1, 1, 1, 1, 1, 1};

    LoadEffectConfigPreProcessCfg(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_022
* @tc.desc  : Test static LoadPostStreamScenesCheck.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_022, TestSize.Level1)
{
    std::vector<PostStreamScene> scenes;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t nodeCounter = 0;

    LoadPostStreamScenesCheck(scenes, currNode, nodeCounter);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_023
* @tc.desc  : Test static LoadPostStreamScenesCheck.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_023, TestSize.Level1)
{
    std::vector<PostStreamScene> scenes;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t nodeCounter = 5;

    LoadPostStreamScenesCheck(scenes, currNode, nodeCounter);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_024
* @tc.desc  : Test static LoadPostStreamScenesCheck.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_024, TestSize.Level1)
{
    std::vector<PostStreamScene> scenes;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t nodeCounter = 1;

    LoadPostStreamScenesCheck(scenes, currNode, nodeCounter);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_025
* @tc.desc  : Test static LoadStreamUsageMappingCheck.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_025, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE_POST] = {1, 1, 1, 1, 1};

    LoadStreamUsageMappingCheck(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_026
* @tc.desc  : Test static LoadStreamUsageMappingCheck.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_026, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE_POST] = {0, 0, 0, 0, 0};

    LoadStreamUsageMappingCheck(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_027
* @tc.desc  : Test static LoadStreamUsageMappingCheck.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_027, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE_POST] = {5, 5, 5, 5, 5};

    LoadStreamUsageMappingCheck(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_028
* @tc.desc  : Test static LoadPostprocessExceptionCheck.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_028, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE_POST] = {5, 5, 5, 5, 5};

    LoadPostprocessExceptionCheck(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_029
* @tc.desc  : Test static LoadPostprocessExceptionCheck.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_029, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE_POST] = {0, 0, 0, 0, 0};

    LoadPostprocessExceptionCheck(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_030
* @tc.desc  : Test static LoadPostprocessExceptionCheck.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_030, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE_POST] = {1, 1, 1, 1, 1};

    LoadPostprocessExceptionCheck(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_031
* @tc.desc  : Test static LoadEffectConfigPostProcessCfg.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_031, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {1, 1, 1, 1, 1, 1};

    LoadEffectConfigPostProcessCfg(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_032
* @tc.desc  : Test static LoadEffectConfigPostProcessCfg.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_032, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {0, 0, 0, 0, 0, 0};

    LoadEffectConfigPostProcessCfg(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_033
* @tc.desc  : Test static LoadEffectConfigPostProcessCfg.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_033, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {5, 5, 5, 5, 5, 5};

    LoadEffectConfigPostProcessCfg(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_034
* @tc.desc  : Test static LoadEffectConfigException.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_034, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {5, 5, 5, 5, 5, 5};

    LoadEffectConfigException(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_035
* @tc.desc  : Test static LoadEffectConfigException.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_035, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {0, 0, 0, 0, 0, 0};

    LoadEffectConfigException(result, currNode, countFirstNode);
}

/**
* @tc.name  : Test AudioEffectConfigParser.
* @tc.number: AudioEffectConfigParser_036
* @tc.desc  : Test static LoadEffectConfigException.
*/
HWTEST(AudioEffectConfigParserTest, AudioEffectConfigParser_036, TestSize.Level1)
{
    OriginalEffectConfig result;
    auto currNode = std::make_shared<xmlNode>().get();
    EXPECT_NE(currNode, nullptr);

    int32_t countFirstNode[NODE_SIZE] = {1, 1, 1, 1, 1, 1};

    LoadEffectConfigException(result, currNode, countFirstNode);
}
} // namespace AudioStandard
} // namespace OHOS
