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

#include "audio_effect.h"
#include "audio_session_device_info.h"
#include "audio_policy_interface.h"
#include "audio_system_manager.h"
#include "audio_ipc_serialization_unit_test.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

void AudioIpcSerializationUnitTest::SetUpTestCase(void) {}
void AudioIpcSerializationUnitTest::TearDownTestCase(void) {}
void AudioIpcSerializationUnitTest::SetUp(void) {}
void AudioIpcSerializationUnitTest::TearDown(void) {}

/**
 * @tc.name  : Test Effect.
 * @tc.number: Effect_001
 * @tc.desc  : Test Effect Deserialization.
 */
HWTEST(AudioIpcSerializationUnitTest, Effect_001, TestSize.Level1)
{
    Parcel parcel;
    Effect effect = {};
    effect.name = "abc";
    EXPECT_TRUE(effect.Marshalling(parcel));
    auto newEffect = std::shared_ptr<Effect>(Effect::Unmarshalling(parcel));
    ASSERT_NE(newEffect, nullptr);
    EXPECT_TRUE(newEffect->name == effect.name);
}

/**
 * @tc.name  : Test Effect.
 * @tc.number: Effect_002
 * @tc.desc  : Test Effect Deserialization.
 */
HWTEST(AudioIpcSerializationUnitTest, Effect_002, TestSize.Level1)
{
    Parcel parcel;
    Effect effect = {};
    effect.effectProperty.assign(Effect::MAX_EFFECT_PROPERTY_SIZE + 1, "abc");
    EXPECT_TRUE(effect.Marshalling(parcel));
    auto newEffect = std::shared_ptr<Effect>(Effect::Unmarshalling(parcel));
    EXPECT_EQ(newEffect, nullptr);
}

/**
 * @tc.name  : Test EffectChainManagerParam.
 * @tc.number: EffectChainManagerParam_001
 * @tc.desc  : Test EffectChainManagerParam Deserialization.
 */
HWTEST(AudioIpcSerializationUnitTest, EffectChainManagerParam_001, TestSize.Level1)
{
    Parcel parcel;
    EffectChainManagerParam param = {};
    param.maxExtraNum = 1;
    EXPECT_TRUE(param.Marshalling(parcel));
    auto newParam = std::shared_ptr<EffectChainManagerParam>(EffectChainManagerParam::Unmarshalling(parcel));
    ASSERT_NE(newParam, nullptr);
    EXPECT_TRUE(newParam->maxExtraNum == param.maxExtraNum);
}

/**
 * @tc.name  : Test EffectChainManagerParam.
 * @tc.number: EffectChainManagerParam_002
 * @tc.desc  : Test EffectChainManagerParam Deserialization.
 */
HWTEST(AudioIpcSerializationUnitTest, EffectChainManagerParam_002, TestSize.Level1)
{
    Parcel parcel;
    EffectChainManagerParam param = {};
    param.priorSceneList.assign(AUDIO_EFFECT_PRIOR_SCENE_UPPER_LIMIT + 1, "abc");
    EXPECT_TRUE(param.Marshalling(parcel));
    auto newParam = std::shared_ptr<EffectChainManagerParam>(EffectChainManagerParam::Unmarshalling(parcel));
    EXPECT_EQ(newParam, nullptr);
}

/**
 * @tc.name  : Test EffectChainManagerParam.
 * @tc.number: EffectChainManagerParam_003
 * @tc.desc  : Test EffectChainManagerParam Deserialization.
 */
HWTEST(AudioIpcSerializationUnitTest, EffectChainManagerParam_003, TestSize.Level1)
{
    Parcel parcel;
    EffectChainManagerParam param = {};
    for (int i = 0; i <= AUDIO_EFFECT_CHAIN_CONFIG_UPPER_LIMIT; i++) {
        param.sceneTypeToChainNameMap[std::to_string(i)] = "abc";
    }
    EXPECT_TRUE(param.Marshalling(parcel));
    auto newParam = std::shared_ptr<EffectChainManagerParam>(EffectChainManagerParam::Unmarshalling(parcel));
    EXPECT_EQ(newParam, nullptr);
}

/**
 * @tc.name  : Test EffectChainManagerParam.
 * @tc.number: EffectChainManagerParam_004
 * @tc.desc  : Test EffectChainManagerParam Deserialization.
 */
HWTEST(AudioIpcSerializationUnitTest, EffectChainManagerParam_004, TestSize.Level1)
{
    Parcel parcel;
    EffectChainManagerParam param = {};
    for (int i = 0; i <= AUDIO_EFFECT_COUNT_PROPERTY_UPPER_LIMIT; i++) {
        param.effectDefaultProperty[std::to_string(i)] = "abc";
    }
    EXPECT_TRUE(param.Marshalling(parcel));
    auto newParam = std::shared_ptr<EffectChainManagerParam>(EffectChainManagerParam::Unmarshalling(parcel));
    EXPECT_EQ(newParam, nullptr);
}

/**
 * @tc.name  : Test SupportedEffectConfig.
 * @tc.number: SupportedEffectConfig_001
 * @tc.desc  : Test SupportedEffectConfig Deserialization.
 */
HWTEST(AudioIpcSerializationUnitTest, SupportedEffectConfig_001, TestSize.Level1)
{
    Parcel parcel;
    SupportedEffectConfig config = {};
    SceneMappingItem item = {};
    config.postProcessSceneMap.push_back(item);
    EXPECT_TRUE(config.Marshalling(parcel));
    auto newConfig = std::shared_ptr<SupportedEffectConfig>(SupportedEffectConfig::Unmarshalling(parcel));
    ASSERT_NE(newConfig, nullptr);
    EXPECT_TRUE(newConfig->postProcessSceneMap.size() == config.postProcessSceneMap.size());
}

/**
 * @tc.name  : Test SupportedEffectConfig.
 * @tc.number: SupportedEffectConfig_002
 * @tc.desc  : Test SupportedEffectConfig Deserialization.
 */
HWTEST(AudioIpcSerializationUnitTest, SupportedEffectConfig_002, TestSize.Level1)
{
    Parcel parcel;
    SupportedEffectConfig config = {};
    Stream stream = {};
    config.preProcessNew.stream.assign(AUDIO_EFFECT_COUNT_UPPER_LIMIT + 1, stream);
    EXPECT_TRUE(config.Marshalling(parcel));
    auto newConfig = std::shared_ptr<SupportedEffectConfig>(SupportedEffectConfig::Unmarshalling(parcel));
    EXPECT_EQ(newConfig, nullptr);
}

/**
 * @tc.name  : Test SupportedEffectConfig.
 * @tc.number: SupportedEffectConfig_003
 * @tc.desc  : Test SupportedEffectConfig Deserialization.
 */
HWTEST(AudioIpcSerializationUnitTest, SupportedEffectConfig_003, TestSize.Level1)
{
    Parcel parcel;
    SupportedEffectConfig config = {};
    Stream stream = {};
    config.postProcessNew.stream.assign(AUDIO_EFFECT_COUNT_UPPER_LIMIT + 1, stream);
    EXPECT_TRUE(config.Marshalling(parcel));
    auto newConfig = std::shared_ptr<SupportedEffectConfig>(SupportedEffectConfig::Unmarshalling(parcel));
    EXPECT_EQ(newConfig, nullptr);
}

/**
 * @tc.name  : Test SupportedEffectConfig.
 * @tc.number: SupportedEffectConfig_004
 * @tc.desc  : Test SupportedEffectConfig Deserialization.
 */
HWTEST(AudioIpcSerializationUnitTest, SupportedEffectConfig_004, TestSize.Level1)
{
    Parcel parcel;
    SupportedEffectConfig config = {};
    SceneMappingItem item = {};
    config.postProcessSceneMap.assign(SupportedEffectConfig::POST_PROCESS_SCENE_MAP_MAX_SIZE + 1, item);
    EXPECT_TRUE(config.Marshalling(parcel));
    auto newConfig = std::shared_ptr<SupportedEffectConfig>(SupportedEffectConfig::Unmarshalling(parcel));
    EXPECT_EQ(newConfig, nullptr);
}

/**
 * @tc.name  : Test AudioEffectPropertyArrayV3.
 * @tc.number: AudioEffectPropertyArrayV3_001
 * @tc.desc  : Test AudioEffectPropertyArrayV3 Deserialization.
 */
HWTEST(AudioIpcSerializationUnitTest, AudioEffectPropertyArrayV3_001, TestSize.Level1)
{
    Parcel parcel;
    AudioEffectPropertyArrayV3 propertyArray = {};
    AudioEffectPropertyV3 property = {};
    propertyArray.property.push_back(property);
    EXPECT_TRUE(propertyArray.Marshalling(parcel));
    auto newPropertyArray = std::shared_ptr<AudioEffectPropertyArrayV3>(
        AudioEffectPropertyArrayV3::Unmarshalling(parcel));
    ASSERT_NE(newPropertyArray, nullptr);
    EXPECT_TRUE(newPropertyArray->property.size() == propertyArray.property.size());

    propertyArray.property.assign(AUDIO_EFFECT_COUNT_UPPER_LIMIT + 1, property);
    EXPECT_TRUE(propertyArray.Marshalling(parcel));
    newPropertyArray = std::shared_ptr<AudioEffectPropertyArrayV3>(
        AudioEffectPropertyArrayV3::Unmarshalling(parcel));
    EXPECT_EQ(newPropertyArray, nullptr);
}

/**
 * @tc.name  : Test AudioEnhancePropertyArray.
 * @tc.number: AudioEnhancePropertyArray_001
 * @tc.desc  : Test AudioEnhancePropertyArray Deserialization.
 */
HWTEST(AudioIpcSerializationUnitTest, AudioEnhancePropertyArray_001, TestSize.Level1)
{
    Parcel parcel;
    AudioEnhancePropertyArray propertyArray = {};
    AudioEnhanceProperty property = {};
    propertyArray.property.push_back(property);
    EXPECT_TRUE(propertyArray.Marshalling(parcel));
    auto newPropertyArray = std::shared_ptr<AudioEnhancePropertyArray>(
        AudioEnhancePropertyArray::Unmarshalling(parcel));
    ASSERT_NE(newPropertyArray, nullptr);
    EXPECT_TRUE(newPropertyArray->property.size() == propertyArray.property.size());

    propertyArray.property.assign(AUDIO_EFFECT_COUNT_UPPER_LIMIT + 1, property);
    EXPECT_TRUE(propertyArray.Marshalling(parcel));
    newPropertyArray = std::shared_ptr<AudioEnhancePropertyArray>(
        AudioEnhancePropertyArray::Unmarshalling(parcel));
    EXPECT_EQ(newPropertyArray, nullptr);
}

/**
 * @tc.name  : Test CurrentOutputDeviceChangedEvent.
 * @tc.number: CurrentOutputDeviceChangedEvent_001
 * @tc.desc  : Test CurrentOutputDeviceChangedEvent Deserialization.
 */
HWTEST(AudioIpcSerializationUnitTest, CurrentOutputDeviceChangedEvent_001, TestSize.Level1)
{
    Parcel parcel;
    CurrentOutputDeviceChangedEvent event = {};
    auto device = std::make_shared<AudioDeviceDescriptor>();
    EXPECT_NE(device, nullptr);
    event.devices.push_back(device);
    event.devices.push_back(nullptr);
    EXPECT_TRUE(event.Marshalling(parcel));
    auto newEvent = std::shared_ptr<CurrentOutputDeviceChangedEvent>(
        CurrentOutputDeviceChangedEvent::Unmarshalling(parcel));
    EXPECT_NE(newEvent, nullptr);

    event.devices.assign(CurrentOutputDeviceChangedEvent::DEVICE_CHANGE_VALID_SIZE + 1, device);
    EXPECT_TRUE(event.Marshalling(parcel));
    newEvent = std::shared_ptr<CurrentOutputDeviceChangedEvent>(
        CurrentOutputDeviceChangedEvent::Unmarshalling(parcel));
    EXPECT_EQ(newEvent, nullptr);
}

/**
 * @tc.name  : Test MicrophoneBlockedInfo.
 * @tc.number: MicrophoneBlockedInfo_001
 * @tc.desc  : Test MicrophoneBlockedInfo Deserialization.
 */
HWTEST(AudioIpcSerializationUnitTest, MicrophoneBlockedInfo_001, TestSize.Level1)
{
    Parcel parcel;
    MicrophoneBlockedInfo info = {};
    auto device = std::make_shared<AudioDeviceDescriptor>();
    EXPECT_NE(device, nullptr);
    info.blockStatus = DeviceBlockStatus::DEVICE_UNBLOCKED;
    EXPECT_TRUE(info.Marshalling(parcel));
    auto newInfo = std::shared_ptr<MicrophoneBlockedInfo>(MicrophoneBlockedInfo::Unmarshalling(parcel));
    ASSERT_NE(newInfo, nullptr);
    EXPECT_EQ(newInfo->blockStatus, info.blockStatus);

    info.devices.assign(MicrophoneBlockedInfo::DEVICE_CHANGE_VALID_SIZE + 1, device);
    EXPECT_TRUE(info.Marshalling(parcel));
    newInfo = std::shared_ptr<MicrophoneBlockedInfo>(MicrophoneBlockedInfo::Unmarshalling(parcel));
    EXPECT_EQ(newInfo, nullptr);
}
} // namespace AudioStandard
} // namespace OHOS
 