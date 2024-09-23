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

#define private public
#define protected public

#include <gtest/gtest.h>
#include "audio_errors.h"
#include "remote_fast_audio_capturer_source.h"
#include "i_audio_capturer_source.h"
#include "audio_hdiadapter_info.h"
#include "remote_fast_audio_capturer_source.cpp"
#include "securec.h"
#include <cinttypes>
#include <dlfcn.h>
#include <sstream>
#include "audio_errors.h"
#include "audio_hdi_log.h"
#include "audio_utils.h"
#include "i_audio_device_adapter.h"
#include "i_audio_device_manager.h"
#include <v1_0/iaudio_manager.h>
#include <v1_0/iaudio_callback.h>
#include <v1_0/audio_types.h>
#include <audio_module_info.h>

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {
// class RemoteFastAudioCapturerSourceInner;
class IAudioDeviceManagerInner : public IAudioDeviceManager {
public:
    IAudioDeviceManagerInner() {};
    ~IAudioDeviceManagerInner() {};

    int32_t GetAllAdapters() override
    {
        return SUCCESS;
    };
    struct AudioAdapterDescriptor *GetTargetAdapterDesc(const std::string &adapterName, bool isMmap) override
    {
        return nullptr;
    };
    std::shared_ptr<IAudioDeviceAdapter> LoadAdapters(const std::string &adapterName, bool isMmap) override
    {
        return nullptr;
    };
    int32_t UnloadAdapter(const std::string &adapterName) override
    {
        return SUCCESS;
    };
    int32_t Release() override
    {
        return SUCCESS;
    };
};
class IAudioDeviceAdapterInner : public IAudioDeviceAdapter {
public:
    IAudioDeviceAdapterInner() {};
    ~IAudioDeviceAdapterInner() {};

    int32_t Init() override
    {
        return SUCCESS;
    };
    int32_t RegExtraParamObserver() override
    {
        return SUCCESS;
    };
    int32_t CreateRender(const AudioDeviceDescriptor &devDesc, const AudioSampleAttributes &attr,
        sptr<IAudioRender> &audioRender, IAudioDeviceAdapterCallback *renderCb, uint32_t &renderId) override
    {
        return SUCCESS;
    };
    void DestroyRender(sptr<IAudioRender> audioRender, uint32_t &renderId) override
    {
    };
    int32_t CreateCapture(const AudioDeviceDescriptor &devDesc, const AudioSampleAttributes &attr,
        sptr<IAudioCapture> &audioCapture, IAudioDeviceAdapterCallback *captureCb, uint32_t &captureId) override
    {
        return SUCCESS;
    };
    void DestroyCapture(sptr<IAudioCapture> audioCapture, uint32_t &captureId) override
    {
    };
    void SetAudioParameter(const AudioParamKey key, const std::string &condition,
        const std::string &value) override
    {
    };
    std::string GetAudioParameter(const AudioParamKey key, const std::string &condition) override
    {
        return nullptr;
    };
    int32_t UpdateAudioRoute(const AudioRoute &route) override
    {
        return SUCCESS;
    };
    int32_t Release() override
    {
        return SUCCESS;
    };
};
class RemoteFastAudioCapturerSourceUnitTest : public ::testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp() override;
    void TearDown() override;
};

void RemoteFastAudioCapturerSourceUnitTest::SetUpTestCase()
{}

void RemoteFastAudioCapturerSourceUnitTest::TearDownTestCase()
{}

void RemoteFastAudioCapturerSourceUnitTest::SetUp()
{}

void RemoteFastAudioCapturerSourceUnitTest::TearDown()
{}


/*
 * Feature: Framework
 * Function: Test GetInstance by deviceNetworkId
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test GetInstance by deviceNetworkId
 */

HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest001, TestSize.Level0)
{
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>("exist_device_network_id");
    const std::string deviceNetworkId = "exist_device_network_id";
    auto rfCapturer = remoteFastAudioCapturerSourceInner->GetInstance(deviceNetworkId);
    EXPECT_NE(nullptr, rfCapturer);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest002, TestSize.Level0)
{
    const std::string deviceNetworkId = "device_network_id";
    auto rfCapturer = RemoteFastAudioCapturerSource::GetInstance(deviceNetworkId);
    EXPECT_NE(nullptr, rfCapturer);
}

/*
 * Feature: Framework
 * Function: Test ConvertToHdiFormat
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test ConvertToHdiFormat
 */

HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest003, TestSize.Level0)
{
    auto format = HdiAdapterFormat::SAMPLE_S16;
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    AudioFormat hdiFormat = remoteFastAudioCapturerSourceInner->ConvertToHdiFormat(format);
    EXPECT_EQ(AudioFormat::AUDIO_FORMAT_TYPE_PCM_16_BIT, hdiFormat);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest004, TestSize.Level0)
{
    auto format = HdiAdapterFormat::SAMPLE_S24;
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    AudioFormat hdiFormat = remoteFastAudioCapturerSourceInner->ConvertToHdiFormat(format);
    EXPECT_EQ(AudioFormat::AUDIO_FORMAT_TYPE_PCM_24_BIT, hdiFormat);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest005, TestSize.Level0)
{
    auto format = HdiAdapterFormat::SAMPLE_S32;
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    AudioFormat hdiFormat = remoteFastAudioCapturerSourceInner->ConvertToHdiFormat(format);
    EXPECT_EQ(AudioFormat::AUDIO_FORMAT_TYPE_PCM_32_BIT, hdiFormat);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest006, TestSize.Level0)
{
    auto format = HdiAdapterFormat::INVALID_WIDTH;
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    AudioFormat hdiFormat = remoteFastAudioCapturerSourceInner->ConvertToHdiFormat(format);
    EXPECT_EQ(AudioFormat::AUDIO_FORMAT_TYPE_PCM_16_BIT, hdiFormat);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest007, TestSize.Level0)
{
    auto format = HdiAdapterFormat::SAMPLE_U8;
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    AudioFormat hdiFormat = remoteFastAudioCapturerSourceInner->ConvertToHdiFormat(format);
    EXPECT_EQ(AudioFormat::AUDIO_FORMAT_TYPE_PCM_8_BIT, hdiFormat);
}
/*
 * Feature: Framework
 * Function: Test ~RemoteFastAudioCapturerSourceInner
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test ~RemoteFastAudioCapturerSourceInner
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest008, TestSize.Level0)
{
    const std::string deviceNetworkId = "device_network_id";
    RemoteFastAudioCapturerSourceInner *rfCapturer =
        new RemoteFastAudioCapturerSourceInner(deviceNetworkId);
    std::atomic<bool>* unittestPtr = new std::atomic<bool>();
    unittestPtr->store(false);
    rfCapturer->capturerInited_ = unittestPtr;
    delete rfCapturer;
    EXPECT_EQ(nullptr, rfCapturer);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest009, TestSize.Level0)
{
    const std::string deviceNetworkId = "device_network_id";
    RemoteFastAudioCapturerSourceInner *rfCapturer =
        new RemoteFastAudioCapturerSourceInner(deviceNetworkId);
    delete rfCapturer;
    EXPECT_EQ(nullptr, rfCapturer);
}
/*
 * Feature: Framework
 * Function: Test DeInit
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test DeInit
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest010, TestSize.Level0)
{
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    remoteFastAudioCapturerSourceInner->DeInit();
    EXPECT_EQ(allRFSources.count(deviceNetworkId), 0);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest011, TestSize.Level0)
{
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    allRFSources[deviceNetworkId] =nullptr;
    remoteFastAudioCapturerSourceInner->DeInit();
    EXPECT_EQ(allRFSources.count(deviceNetworkId), 0);
}
/*
 * Feature: Framework
 * Function: Test CreateCapture
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test CreateCapture
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest012, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    AudioPort capturePort = {
    .dir = HDI::DistributedAudio::Audio::V1_0::PORT_OUT,
    .portId = 8080,
    .portName = capturePortName,
    };
    remoteFastAudioCapturerSourceInner->attr_.audioStreamFlag = AUDIO_FLAG_VOIP_FAST;
    auto ret = remoteFastAudioCapturerSourceInner->CreateCapture(capturePort);
    EXPECT_EQ(ret, SUCCESS);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest013, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    AudioPort capturePort = {
    .dir = HDI::DistributedAudio::Audio::V1_0::PORT_OUT,
    .portId = 8080,
    .portName = capturePortName,
    };
    remoteFastAudioCapturerSourceInner->attr_.audioStreamFlag = AUDIO_FLAG_NORMAL;
    auto ret = remoteFastAudioCapturerSourceInner->CreateCapture(capturePort);
    EXPECT_EQ(ret, SUCCESS);
}
/*
 * Feature: Framework
 * Function: Test Start
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test Start
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest014, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    std::atomic<bool>* boolAtomPtr = new std::atomic<bool>();
    boolAtomPtr->store(false);
    remoteFastAudioCapturerSourceInner->isCapturerCreated_ = boolAtomPtr;
    auto ret = remoteFastAudioCapturerSourceInner->Start();
    EXPECT_EQ(ret, SUCCESS);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest015, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    std::atomic<bool>* boolAtomPtr = new std::atomic<bool>();
    boolAtomPtr->store(true);
    remoteFastAudioCapturerSourceInner->isCapturerCreated_ = boolAtomPtr;
    auto ret = remoteFastAudioCapturerSourceInner->Start();
    EXPECT_EQ(ret, SUCCESS);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest016, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    std::atomic<bool>* boolAtomPtr = new std::atomic<bool>();
    boolAtomPtr->store(true);
    remoteFastAudioCapturerSourceInner->started_ = boolAtomPtr;
    auto ret = remoteFastAudioCapturerSourceInner->Start();
    EXPECT_EQ(ret, SUCCESS);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest017, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    std::atomic<bool>* boolAtomPtr = new std::atomic<bool>();
    boolAtomPtr->store(false);
    remoteFastAudioCapturerSourceInner->started_ = boolAtomPtr;
    auto ret = remoteFastAudioCapturerSourceInner->Start();
    EXPECT_EQ(ret, SUCCESS);
}
/*
 * Feature: Framework
 * Function: Test Stop
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test Stop
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest018, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    std::atomic<bool>* boolAtomPtr = new std::atomic<bool>();
    boolAtomPtr->store(false);
    remoteFastAudioCapturerSourceInner->started_ = boolAtomPtr;
    auto ret = remoteFastAudioCapturerSourceInner->Stop();
    EXPECT_EQ(ret, SUCCESS);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest019, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    std::atomic<bool>* boolAtomPtr = new std::atomic<bool>();
    boolAtomPtr->store(false);
    remoteFastAudioCapturerSourceInner->started_ = boolAtomPtr;
    auto ret = remoteFastAudioCapturerSourceInner->Stop();
    EXPECT_EQ(ret, SUCCESS);
}
/*
 * Feature: Framework
 * Function: Test Pause
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test Pause
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest020, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    std::atomic<bool>* boolAtomPtr = new std::atomic<bool>();
    boolAtomPtr->store(false);
    remoteFastAudioCapturerSourceInner->paused_ = boolAtomPtr;
    auto ret = remoteFastAudioCapturerSourceInner->Pause();
    EXPECT_EQ(ret, SUCCESS);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest021, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    std::atomic<bool>* boolAtomPtr = new std::atomic<bool>();
    boolAtomPtr->store(false);
    remoteFastAudioCapturerSourceInner->paused_ = boolAtomPtr;
    auto ret = remoteFastAudioCapturerSourceInner->Pause();
    EXPECT_EQ(ret, SUCCESS);
}
/*
 * Feature: Framework
 * Function: Test Resume
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test Resume
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest022, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    std::atomic<bool>* boolAtomPtr = new std::atomic<bool>();
    boolAtomPtr->store(false);
    remoteFastAudioCapturerSourceInner->paused_ = boolAtomPtr;
    auto ret = remoteFastAudioCapturerSourceInner->Resume();
    EXPECT_EQ(ret, SUCCESS);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest023, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    std::atomic<bool>* boolAtomPtr = new std::atomic<bool>();
    boolAtomPtr->store(false);
    remoteFastAudioCapturerSourceInner->paused_ = boolAtomPtr;
    auto ret = remoteFastAudioCapturerSourceInner->Resume();
    EXPECT_EQ(ret, SUCCESS);
}
/*
 * Feature: Framework
 * Function: Test SetVolume
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test SetVolume
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest024, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    auto ret = remoteFastAudioCapturerSourceInner->SetVolume(0, 1);
    EXPECT_EQ(ret, 1);
    ret = remoteFastAudioCapturerSourceInner->SetVolume(1, 0);
    EXPECT_EQ(ret, 1);
    ret = remoteFastAudioCapturerSourceInner->SetVolume(1, 1);
    EXPECT_EQ(ret, 1);
    ret = remoteFastAudioCapturerSourceInner->SetVolume(0, 0);
    EXPECT_EQ(ret, 0);
}
/*
 * Feature: Framework
 * Function: Test SetMute
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test SetMute
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest025, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    auto ret = remoteFastAudioCapturerSourceInner->SetMute(true);
    EXPECT_EQ(ret, SUCCESS);
    ret = remoteFastAudioCapturerSourceInner->SetMute(false);
    EXPECT_EQ(ret, SUCCESS);
}
/*
 * Feature: Framework
 * Function: Test GetMute
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test GetMute
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest026, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    bool isHdiMute = false;
    auto ret = remoteFastAudioCapturerSourceInner->GetMute(isHdiMute);
    EXPECT_EQ(ret, SUCCESS);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest027, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    bool isHdiMute = true;
    auto ret = remoteFastAudioCapturerSourceInner->GetMute(isHdiMute);
    EXPECT_EQ(ret, SUCCESS);
}
/*
 * Feature: Framework
 * Function: Test SetInputPortPin
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test SetInputPortPin
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest028, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    AudioRouteNode source = {};
    auto ret = remoteFastAudioCapturerSourceInner->SetInputPortPin(DEVICE_TYPE_MIC, source);
    EXPECT_EQ(ret, SUCCESS);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest029, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    AudioRouteNode source = {};
    auto ret = remoteFastAudioCapturerSourceInner->SetInputPortPin(DEVICE_TYPE_WIRED_HEADSET, source);
    EXPECT_EQ(ret, SUCCESS);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest030, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    AudioRouteNode source = {};
    auto ret = remoteFastAudioCapturerSourceInner->SetInputPortPin(DEVICE_TYPE_USB_HEADSET, source);
    EXPECT_EQ(ret, SUCCESS);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest031, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    AudioRouteNode source = {};
    auto ret = remoteFastAudioCapturerSourceInner->SetInputPortPin(DEVICE_TYPE_FILE_SOURCE, source);
    EXPECT_EQ(ret, ERR_NOT_SUPPORTED);
}
/*
 * Feature: Framework
 * Function: Test GetAudioCategory
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test GetAudioCategory
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest032, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    auto ret = remoteFastAudioCapturerSourceInner->GetAudioCategory(AUDIO_SCENE_DEFAULT);
    EXPECT_EQ(ret, AudioCategory::AUDIO_IN_MEDIA);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest033, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    auto ret = remoteFastAudioCapturerSourceInner->GetAudioCategory(AUDIO_SCENE_RINGING);
    EXPECT_EQ(ret, AudioCategory::AUDIO_IN_RINGTONE);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest034, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    auto ret = remoteFastAudioCapturerSourceInner->GetAudioCategory(AUDIO_SCENE_VOICE_RINGING);
    EXPECT_EQ(ret, AudioCategory::AUDIO_IN_RINGTONE);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest035, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    auto ret = remoteFastAudioCapturerSourceInner->GetAudioCategory(AUDIO_SCENE_PHONE_CALL);
    EXPECT_EQ(ret, AudioCategory::AUDIO_IN_CALL);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest036, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    auto ret = remoteFastAudioCapturerSourceInner->GetAudioCategory(AUDIO_SCENE_PHONE_CHAT);
    EXPECT_EQ(ret, AudioCategory::AUDIO_IN_COMMUNICATION);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest037, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    auto ret = remoteFastAudioCapturerSourceInner->GetAudioCategory(AUDIO_SCENE_MAX);
    EXPECT_EQ(ret, AudioCategory::AUDIO_IN_MEDIA);
}
/*
 * Feature: Framework
 * Function: Test PcmFormatToBits
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test PcmFormatToBits
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest038, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    auto ret = remoteFastAudioCapturerSourceInner->PcmFormatToBits(HdiAdapterFormat::SAMPLE_U8);
    EXPECT_EQ(ret, RemoteFastAudioCapturerSourceInner::PCM_8_BIT);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest039, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    auto ret = remoteFastAudioCapturerSourceInner->PcmFormatToBits(HdiAdapterFormat::SAMPLE_S16);
    EXPECT_EQ(ret, RemoteFastAudioCapturerSourceInner::PCM_16_BIT);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest040, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    auto ret = remoteFastAudioCapturerSourceInner->PcmFormatToBits(HdiAdapterFormat::SAMPLE_S24);
    EXPECT_EQ(ret, RemoteFastAudioCapturerSourceInner::PCM_24_BIT);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest041, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    auto ret = remoteFastAudioCapturerSourceInner->PcmFormatToBits(HdiAdapterFormat::SAMPLE_S32);
    EXPECT_EQ(ret, RemoteFastAudioCapturerSourceInner::PCM_32_BIT);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest042, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    auto ret = remoteFastAudioCapturerSourceInner->PcmFormatToBits(HdiAdapterFormat::SAMPLE_F32);
    EXPECT_EQ(ret, RemoteFastAudioCapturerSourceInner::PCM_32_BIT);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest043, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    auto ret = remoteFastAudioCapturerSourceInner->PcmFormatToBits(HdiAdapterFormat::INVALID_WIDTH);
    EXPECT_EQ(ret, RemoteFastAudioCapturerSourceInner::PCM_16_BIT);
}
/*
 * Feature: Framework
 * Function: Test OnAudioParamChange
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test OnAudioParamChange
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest044, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    const std::string adapterName = "adapterName";
    const std::string condition = "condition";
    const std::string value = "value";
    remoteFastAudioCapturerSourceInner->OnAudioParamChange(adapterName, AudioParamKey::PARAM_KEY_STATE,
        condition, value);
    remoteFastAudioCapturerSourceInner->OnAudioParamChange(adapterName, AudioParamKey::VOLUME, condition, value);
}

/*
 * Feature: Framework
 * Function: Test ClearCapture
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test ClearCapture
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest045, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    remoteFastAudioCapturerSourceInner->bufferFd_ = RemoteFastAudioCapturerSourceInner::INVALID_FD;
    remoteFastAudioCapturerSourceInner->audioAdapter_ = nullptr;
    remoteFastAudioCapturerSourceInner->audioManager_ = nullptr;
    remoteFastAudioCapturerSourceInner->ClearCapture();
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest046, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    remoteFastAudioCapturerSourceInner->bufferFd_ = RemoteFastAudioCapturerSourceInner::HALF_FACTOR;
    std::shared_ptr<IAudioDeviceAdapter> audioAdapter = std::make_shared<IAudioDeviceAdapterInner>();
    remoteFastAudioCapturerSourceInner->audioAdapter_ = audioAdapter;
    std::shared_ptr<IAudioDeviceManager> audioManager = std::make_shared<IAudioDeviceManagerInner>();
    remoteFastAudioCapturerSourceInner->audioManager_ = audioManager;
    remoteFastAudioCapturerSourceInner->ClearCapture();
}
/*
 * Feature: Framework
 * Function: Test Init
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test Init
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest047, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    IAudioSourceAttr attr = {
        .deviceType = 0,
    };
    std::atomic<bool>* isCapturerCreated = new std::atomic<bool>();
    isCapturerCreated->store(false);
    remoteFastAudioCapturerSourceInner->isCapturerCreated_ = isCapturerCreated;
    remoteFastAudioCapturerSourceInner->audioManager_->GetTargetAdapterDesc(deviceNetworkId, true)->ports[0].portId =
        AudioPortPin::PIN_IN_MIC;
    auto ret =remoteFastAudioCapturerSourceInner->Init(attr);
    EXPECT_EQ(ret, SUCCESS);
}
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest048, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    IAudioSourceAttr attr = {
        .deviceType = 0,
    };
    std::atomic<bool>* isCapturerCreated = new std::atomic<bool>();
    isCapturerCreated->store(true);
    remoteFastAudioCapturerSourceInner->isCapturerCreated_ = isCapturerCreated;
    remoteFastAudioCapturerSourceInner->audioManager_->GetTargetAdapterDesc(deviceNetworkId, true)->ports[0].portId =
        AudioPortPin::PIN_IN_HS_MIC;
    auto ret =remoteFastAudioCapturerSourceInner->Init(attr);
    EXPECT_EQ(ret, SUCCESS);
}


/*
 * Feature: Framework
 * Function: Test CheckPositionTime
 * SubFunction: NA
 * FunctionPoints: NA
 * EnvConditions: NA
 * CaseDescription: Test CheckPositionTime
 */
HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest049, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    auto ret = remoteFastAudioCapturerSourceInner->CheckPositionTime();
    EXPECT_EQ(ret, SUCCESS);
}

#ifdef AUDIO_NS_PER_SECOND
#undef AUDIO_NS_PER_SECOND
#endif
#define AUDIO_NS_PER_SECOND ((uint64_t)10000000000)

HWTEST_F(RemoteFastAudioCapturerSourceUnitTest, remote_fast_audio_capturer_source_unittest050, TestSize.Level0)
{
    const std::string capturePortName = "creatCapture";
    const std::string deviceNetworkId = "device_network_id";
    std::shared_ptr<RemoteFastAudioCapturerSourceInner> remoteFastAudioCapturerSourceInner =
        std::make_shared<RemoteFastAudioCapturerSourceInner>(deviceNetworkId);
    auto ret = remoteFastAudioCapturerSourceInner->CheckPositionTime();
    EXPECT_EQ(ret, ERROR);
}
}
}
