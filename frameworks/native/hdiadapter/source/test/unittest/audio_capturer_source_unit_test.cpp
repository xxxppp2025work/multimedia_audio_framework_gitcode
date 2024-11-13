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

#include <gtest/gtest.h>
#include "audio_errors.h"
#include "audio_capturer_source.h"

#include "i_audio_capturer_source.h"
#include "audio_hdiadapter_info.h"
#include "securec.h"
#include <cinttypes>
#include <dlfcn.h>
#include <sstream>
#include "audio_utils.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {
class AudioCapturerSourceUnitTest : public ::testing::Test {
public:
    const std::string LOCAL_NETWORK_ID = "LocalDevice";
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp() override;
    void TearDown() override;
};

namespace {
    AudioCapturerSource *source;
    AudioCapturerSource *usbSource;
    AudioCapturerSource *wakeupSource;
} // namespace

void AudioCapturerSourceUnitTest::SetUpTestCase()
{}

void AudioCapturerSourceUnitTest::TearDownTestCase()
{}

void AudioCapturerSourceUnitTest::SetUp()
{
    source = AudioCapturerSource::GetInstance("primary");
    usbSource = AudioCapturerSource::GetInstance("usb");
    wakeupSource = AudioCapturerSource::GetInstance("primary", SOURCE_TYPE_WAKEUP);
}

void AudioCapturerSourceUnitTest::TearDown()
{
    source->DeInit();
    usbSource->DeInit();
    wakeupSource->DeInit();
}

/**
 * @tc.name  : Test RemoteFastAudioCapturerSourceUnitTest.
 * @tc.number: remote_audio_capturer_source_unittest001.
 * @tc.desc  : Test Init Function.
 */
HWTEST_F(AudioCapturerSourceUnitTest, Audio_Capture_Source_Init_001, TestSize.Level1)
{
    EXPECT_NE(source, nullptr);

    IAudioSourceAttr attr = {};
    attr.sampleRate = 48000;
    attr.channel = STEREO;
    attr.format = HdiAdapterFormat::SAMPLE_S16;
    attr.channelLayout = 2;
    attr.deviceNetworkId = LOCAL_NETWORK_ID.c_str();
    attr.volume = 1.0f;
    attr.audioStreamFlag = AUDIO_FLAG_MMAP;
    attr.sourceType = SOURCE_TYPE_WAKEUP;
    attr.adapterName = "primary";
    attr.deviceType = DEVICE_TYPE_WAKEUP;
    attr.openMicSpeaker = 1;
    int32_t ret = source->Init(attr);
    EXPECT_EQ(SUCCESS, ret);

    attr.deviceType = DEVICE_TYPE_USB_HEADSET;
    ret = source->Init(attr);
    EXPECT_EQ(SUCCESS, ret);

    bool isMute;
    ret = source->GetMute(isMute);
    EXPECT_EQ(SUCCESS, ret);

    ret = source->GetMute(isMute);
    EXPECT_EQ(SUCCESS, ret);
    usbSource->DeInit();
}

/**
 * @tc.name  : Test RemoteFastAudioCapturerSourceUnitTest.
 * @tc.number: remote_audio_capturer_source_unittest001.
 * @tc.desc  : Test Init Function.
 */
HWTEST_F(AudioCapturerSourceUnitTest, Audio_Capture_Source_Mix_001, TestSize.Level1)
{
    EXPECT_NE(source, nullptr);

    IAudioSourceAttr attr = {};
    attr.sampleRate = 48000;
    attr.channel = STEREO;
    attr.format = HdiAdapterFormat::SAMPLE_S16;
    attr.channelLayout = 2;
    attr.deviceNetworkId = LOCAL_NETWORK_ID.c_str();
    attr.volume = 1.0f;
    attr.audioStreamFlag = AUDIO_FLAG_MMAP;
    attr.sourceType = SOURCE_TYPE_WAKEUP;
    attr.adapterName = "primary";
    attr.deviceType = DEVICE_TYPE_SPEAKER;
    attr.openMicSpeaker = 0;
    int32_t ret = source->Init(attr);
    EXPECT_EQ(SUCCESS, ret);

    ret = source->Pause();
    EXPECT_EQ(SUCCESS, ret);

    ret = source->Resume();
    EXPECT_NE(SUCCESS, ret);

    ret = source->Reset();
    EXPECT_EQ(SUCCESS, ret);

    ret = source->Start();
    EXPECT_EQ(SUCCESS, ret);

    ret = source->Start();
    EXPECT_EQ(SUCCESS, ret);

    ret = source->Pause();
    EXPECT_NE(SUCCESS, ret);

    ret = source->Reset();
    EXPECT_EQ(SUCCESS, ret);

    ret = source->Stop();
    EXPECT_EQ(SUCCESS, ret);

    ret = source->SetVolume(0.0f, 0.0f);
    EXPECT_EQ(SUCCESS, ret);

    ret = source->SetVolume(1.0f, 0.0f);
    EXPECT_EQ(SUCCESS, ret);

    ret = source->SetVolume(0.0f, 1.0f);
    EXPECT_EQ(SUCCESS, ret);

    ret = source->SetVolume(1.0f, 1.0f);
    EXPECT_EQ(SUCCESS, ret);
}

HWTEST_F(AudioCapturerSourceUnitTest, Audio_Capture_Source_Mix_002, TestSize.Level1)
{
    EXPECT_NE(source, nullptr);

    uint64_t frames;
    int64_t timeSec = 0;
    int64_t timeNanoSec = 0;
    int32_t ret = source->GetPresentationPosition(frames, timeSec, timeNanoSec);
    EXPECT_EQ(SUCCESS, ret);

    IAudioSourceAttr attr = {};
    attr.sampleRate = 48000;
    attr.channel = STEREO;
    attr.format = HdiAdapterFormat::SAMPLE_S16;
    attr.channelLayout = 2;
    attr.deviceNetworkId = LOCAL_NETWORK_ID.c_str();
    attr.volume = 1.0f;
    attr.audioStreamFlag = AUDIO_FLAG_MMAP;
    attr.sourceType = SOURCE_TYPE_WAKEUP;
    attr.adapterName = "primary";
    attr.deviceType = DEVICE_TYPE_WAKEUP;
    attr.openMicSpeaker = 0;

    ret = source->Init(attr);
    EXPECT_EQ(SUCCESS, ret);

    char* frame = nullptr;
    uint64_t requestBytes = 0;
    uint64_t replyBytes = 20;
    ret = source->CaptureFrame(frame, requestBytes, replyBytes);
    EXPECT_EQ(SUCCESS, ret);

    ret = source->CaptureFrameWithEc(nullptr, requestBytes, nullptr, requestBytes);
    EXPECT_EQ(SUCCESS, ret);

    ret = source->CaptureFrame(frame, requestBytes, replyBytes);
    EXPECT_EQ(SUCCESS, ret);

    ret = source->CaptureFrameWithEc(nullptr, requestBytes, nullptr, requestBytes);
    EXPECT_EQ(SUCCESS, ret);

    ret = source->SetAudioScene(AUDIO_SCENE_DEFAULT, DEVICE_TYPE_SPEAKER, "usb");
    EXPECT_EQ(SUCCESS, ret);

    ret = source->SetInputRoute(DEVICE_TYPE_MIC, "usb");
    EXPECT_EQ(SUCCESS, ret);

    ret = source->GetPresentationPosition(frames, timeSec, timeNanoSec);
    EXPECT_EQ(SUCCESS, ret);

    uint32_t captureId = 0;
    ret = source->GetCaptureId(captureId);
    EXPECT_EQ(SUCCESS, ret);
}

// HWTEST_F(AudioCapturerSourceUnitTest, Audio_Capture_wakeup_Source_Create_001, TestSize.Level1)
// {

// }
} // namespace AudioStandard
} // namespace OHOS
