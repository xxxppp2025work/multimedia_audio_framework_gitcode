/*
 * Copyright (c) 2021-2025 Huawei Device Co., Ltd.
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

#include "audio_renderer_unit_test.h"

#include <chrono>
#include <thread>

#include "audio_errors.h"
#include "audio_info.h"
#include "audio_renderer.h"
#include "audio_renderer_proxy_obj.h"
#include "audio_policy_manager.h"
#include "audio_renderer_private.h"
#include "audio_renderer.cpp"
#include "fast_audio_stream.h"

using namespace std;
using namespace std::chrono;
using namespace testing::ext;
using namespace testing;

namespace OHOS {
namespace AudioStandard {
namespace {
    const string AUDIORENDER_TEST_FILE_PATH = "/data/test_44100_2.wav";
    const string AUDIORENDER_TEST_PCMFILE_PATH = "/data/avs3_16.wav";
    const string AUDIORENDER_TEST_METAFILE_PATH = "/data/avs3_bitstream.bin";
    const int32_t VALUE_ZERO = 0;
    const int32_t VALUE_HUNDRED = 100;
    const int32_t VALUE_THOUSAND = 1000;
    const int32_t RENDERER_FLAG = 0;
    // Writing only 500 buffers of data for test
    const int32_t WRITE_BUFFERS_COUNT = 500;
    const int32_t MAX_BUFFER_SIZE = 20000;
    constexpr int32_t PAUSE_BUFFER_POSITION = 400000;
    constexpr int32_t PAUSE_RENDER_TIME_SECONDS = 1;

    constexpr uint32_t PLAYBACK_DURATION = 2;

    constexpr size_t AVS3METADATA_SIZE = 19824;
    constexpr size_t AUDIOVIVID_FRAME_COUNT = 1024;
    const int32_t MAX_CACHE_SIZE = 16384;
    const int32_t MIN_CACHE_SIZE = 3528;

    static size_t g_reqBufLen = 0;
} // namespace


InterruptEvent AudioRendererUnitTest::interruptEventTest_ = {};
void AudioRendererUnitTest::SetUpTestCase(void) {}
void AudioRendererUnitTest::TearDownTestCase(void) {}
void AudioRendererUnitTest::SetUp(void) {}
void AudioRendererUnitTest::TearDown(void) {}

void AudioRenderModeCallbackTest::OnWriteData(size_t length)
{
    g_reqBufLen = length;
}

static int g_writeOverflowNum = 1000;

class TestAudioStremStub : public FastAudioStream {
public:
    TestAudioStremStub() : FastAudioStream(AudioStreamType::STREAM_MUSIC,
        AudioMode::AUDIO_MODE_RECORD, 0) {}
    uint32_t GetOverflowCount() override { return g_writeOverflowNum; }
    State GetState() override { return state_; }
    bool StopAudioStream() override { return true; }
    bool StartAudioStream(StateChangeCmdType cmdType,
        AudioStreamDeviceChangeReasonExt reason) override { return true; }
    bool ReleaseAudioStream(bool releaseRunner, bool destoryAtOnce) override { return true; }

    State state_ = State::RUNNING;
};

void AudioRendererCallbackTest::OnInterrupt(const InterruptEvent &interruptEvent)
{
    AudioRendererUnitTest::interruptEventTest_.hintType = interruptEvent.hintType;
}

int32_t AudioRendererUnitTest::InitializeRenderer(unique_ptr<AudioRenderer> &audioRenderer)
{
    AudioRendererParams rendererParams;
    rendererParams.sampleFormat = SAMPLE_S16LE;
    rendererParams.sampleRate = SAMPLE_RATE_44100;
    rendererParams.channelCount = STEREO;
    rendererParams.encodingType = ENCODING_PCM;

    return audioRenderer->SetParams(rendererParams);
}

void AudioRendererUnitTest::InitializeRendererOptions(AudioRendererOptions &rendererOptions)
{
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_44100;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MOVIE;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_MOVIE;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    return;
}

void AudioRendererUnitTest::InitializeRendererSpatialOptions(AudioRendererOptions &rendererOptions)
{
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_48000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_AUDIOVIVID;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptions.streamInfo.channels = AudioChannel::CHANNEL_8;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MUSIC;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_MEDIA;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;
    return;
}

void AudioRendererUnitTest::GetBuffersAndLen(unique_ptr<AudioRenderer> &audioRenderer,
    uint8_t *&buffer, uint8_t *&metaBuffer, size_t &bufferLen)
{
    uint32_t ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);
    buffer = new uint8_t[bufferLen];
    ASSERT_NE(nullptr, buffer);
    EXPECT_GE(MAX_BUFFER_SIZE, bufferLen);
    metaBuffer = new uint8_t[AVS3METADATA_SIZE];
    ASSERT_NE(nullptr, metaBuffer);
}

void AudioRendererUnitTest::ReleaseBufferAndFiles(uint8_t* &buffer, uint8_t* &metaBuffer,
    FILE* &wavFile, FILE* &metaFile)
{
    delete []buffer;
    delete []metaBuffer;
    (void)fclose(wavFile);
    fclose(metaFile);
}

void StartRenderThread(AudioRenderer *audioRenderer, uint32_t limit)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    auto buffer = std::make_unique<uint8_t[]>(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = 0;
    int32_t bytesWritten = 0;
    size_t minBytes = 4;
    int32_t numBuffersToRender = WRITE_BUFFERS_COUNT;
    auto start = chrono::system_clock::now();

    while (numBuffersToRender) {
        bytesToWrite = fread(buffer.get(), 1, bufferLen, wavFile);
        bytesWritten = 0;
        while ((static_cast<size_t>(bytesWritten) < bytesToWrite) &&
            ((static_cast<size_t>(bytesToWrite) - bytesWritten) > minBytes)) {
            bytesWritten += audioRenderer->Write(buffer.get() + static_cast<size_t>(bytesWritten),
                                                 bytesToWrite - static_cast<size_t>(bytesWritten));
            if (bytesWritten < 0) {
                break;
            }
        }
        numBuffersToRender--;

        if ((limit > 0) && (duration_cast<seconds>(system_clock::now() - start).count() > limit)) {
            break;
        }
    }

    audioRenderer->Drain();

    fclose(wavFile);
}

/**
 * @tc.name  : Test GetSupportedFormats API
 * @tc.number: Audio_Renderer_GetSupportedFormats_001
 * @tc.desc  : Test GetSupportedFormats interface. Returns supported Formats on success.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetSupportedFormats_001, TestSize.Level0)
{
    vector<AudioSampleFormat> supportedFormatList = AudioRenderer::GetSupportedFormats();
    EXPECT_EQ(AUDIO_SUPPORTED_FORMATS.size(), supportedFormatList.size());
}

/**
 * @tc.name  : Test GetSupportedChannels API
 * @tc.number: Audio_Renderer_GetSupportedChannels_001
 * @tc.desc  : Test GetSupportedChannels interface. Returns supported Channels on success.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetSupportedChannels_001, TestSize.Level0)
{
    vector<AudioChannel> supportedChannelList = AudioRenderer::GetSupportedChannels();
    EXPECT_EQ(RENDERER_SUPPORTED_CHANNELS.size(), supportedChannelList.size());
}

/**
 * @tc.name  : Test GetSupportedEncodingTypes API
 * @tc.number: Audio_Renderer_GetSupportedEncodingTypes_001
 * @tc.desc  : Test GetSupportedEncodingTypes interface. Returns supported Encoding types on success.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetSupportedEncodingTypes_001, TestSize.Level0)
{
    vector<AudioEncodingType> supportedEncodingTypes
                                        = AudioRenderer::GetSupportedEncodingTypes();
    EXPECT_EQ(AUDIO_SUPPORTED_ENCODING_TYPES.size(), supportedEncodingTypes.size());
}

/**
 * @tc.name  : Test GetSupportedSamplingRates API
 * @tc.number: Audio_Renderer_GetSupportedSamplingRates_001
 * @tc.desc  : Test GetSupportedSamplingRates interface. Returns supported Sampling rates on success.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetSupportedSamplingRates_001, TestSize.Level0)
{
    vector<AudioSamplingRate> supportedSamplingRates = AudioRenderer::GetSupportedSamplingRates();
    EXPECT_EQ(AUDIO_SUPPORTED_SAMPLING_RATES.size(), supportedSamplingRates.size());
}

/**
 * @tc.name  : Test Create API via legal input.
 * @tc.number: Audio_Renderer_Create_001
 * @tc.desc  : Test Create interface with STREAM_MUSIC. Returns audioRenderer instance, if create is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Create_001, TestSize.Level0)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    EXPECT_NE(nullptr, audioRenderer);
}

/**
 * @tc.name  : Test Create API via legal input.
 * @tc.number: Audio_Renderer_Create_002
 * @tc.desc  : Test Create interface with STREAM_RING. Returns audioRenderer instance, if create is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Create_002, TestSize.Level0)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_RING);
    EXPECT_NE(nullptr, audioRenderer);
}

/**
 * @tc.name  : Test Create API via legal input.
 * @tc.number: Audio_Renderer_Create_003
 * @tc.desc  : Test Create interface with STREAM_VOICE_CALL. Returns audioRenderer instance if create is successful.
 *             Note: instance will be created but functional support for STREAM_VOICE_CALL not available yet.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Create_003, TestSize.Level0)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_VOICE_CALL);
    EXPECT_NE(nullptr, audioRenderer);
}

/**
 * @tc.name  : Test Create API via legal input.
 * @tc.number: Audio_Renderer_Create_004
 * @tc.desc  : Test Create interface with STREAM_SYSTEM. Returns audioRenderer instance, if create is successful.
 *             Note: instance will be created but functional support for STREAM_SYSTEM not available yet.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Create_004, TestSize.Level0)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_SYSTEM);
    EXPECT_NE(nullptr, audioRenderer);
}

/**
 * @tc.name  : Test Create API via legal input.
 * @tc.number: Audio_Renderer_Create_005
 * @tc.desc  : Test Create interface with STREAM_BLUETOOTH_SCO. Returns audioRenderer instance, if create is successful.
 *             Note: instance will be created but functional support for STREAM_BLUETOOTH_SCO not available yet
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Create_005, TestSize.Level0)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_BLUETOOTH_SCO);
    EXPECT_NE(nullptr, audioRenderer);
}

/**
 * @tc.name  : Test Create API via legal input.
 * @tc.number: Audio_Renderer_Create_006
 * @tc.desc  : Test Create interface with STREAM_ALARM. Returns audioRenderer instance, if create is successful.
 *             Note: instance will be created but functional support for STREAM_ALARM not available yet.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Create_006, TestSize.Level0)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_ALARM);
    EXPECT_NE(nullptr, audioRenderer);
}

/**
 * @tc.name  : Test Create API via legal input.
 * @tc.number: Audio_Renderer_Create_007
 * @tc.desc  : Test Create interface with STREAM_NOTIFICATION. Returns audioRenderer instance, if create is successful.
 *             Note: instance will be created but functional support for STREAM_NOTIFICATION not available yet.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Create_007, TestSize.Level0)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_NOTIFICATION);
    EXPECT_NE(nullptr, audioRenderer);
}

/**
 * @tc.name  : Test Create API via legal input.
 * @tc.number: Audio_Renderer_Create_008
 * @tc.desc  : Test Create interface with AudioRendererOptions below.
 *             Returns audioRenderer instance, if create is successful.
 *             rendererOptions.streamInfo.samplingRate = SAMPLE_RATE_96000;
 *             rendererOptions.streamInfo.encoding = ENCODING_PCM;
 *             rendererOptions.streamInfo.format = SAMPLE_U8;
 *             rendererOptions.streamInfo.channels = MONO;
 *             rendererOptions.rendererInfo.contentType = CONTENT_TYPE_MUSIC;
 *             rendererOptions.rendererInfo.streamUsage = STREAM_USAGE_MEDIA;
 *             rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Create_008, TestSize.Level0)
{
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_96000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_U8;
    rendererOptions.streamInfo.channels = AudioChannel::MONO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MUSIC;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_MEDIA;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Create API via legal input.
 * @tc.number: Audio_Renderer_Create_009
 * @tc.desc  : Test Create interface with AudioRendererOptions below.
 *             Returns audioRenderer instance, if create is successful.
 *             rendererOptions.streamInfo.samplingRate = SAMPLE_RATE_96000;
 *             rendererOptions.streamInfo.encoding = ENCODING_PCM;
 *             rendererOptions.streamInfo.format = SAMPLE_U8;
 *             rendererOptions.streamInfo.channels = STEREO;
 *             rendererOptions.rendererInfo.contentType = CONTENT_TYPE_MOVIE;
 *             rendererOptions.rendererInfo.streamUsage = STREAM_USAGE_MEDIA;
 *             rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Create_009, TestSize.Level0)
{
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_8000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_U8;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MOVIE;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_MEDIA;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Create API via legal input.
 * @tc.number: Audio_Renderer_Create_010
 * @tc.desc  : Test Create interface with AudioRendererOptions below.
 *             Returns audioRenderer instance, if create is successful.
 *             rendererOptions.streamInfo.samplingRate = SAMPLE_RATE_64000;
 *             rendererOptions.streamInfo.encoding = ENCODING_PCM;
 *             rendererOptions.streamInfo.format = SAMPLE_S32LE;
 *             rendererOptions.streamInfo.channels = MONO;
 *             rendererOptions.rendererInfo.contentType = CONTENT_TYPE_RINGTONE;
 *             rendererOptions.rendererInfo.streamUsage = STREAM_USAGE_NOTIFICATION_RINGTONE;
 *             rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Create_010, TestSize.Level0)
{
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_64000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S32LE;
    rendererOptions.streamInfo.channels = AudioChannel::MONO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_RINGTONE;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_NOTIFICATION_RINGTONE;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Create API via legal input.
 * @tc.number: Audio_Renderer_Create_011
 * @tc.desc  : Test Create interface with AudioRendererOptions below.
 *             Returns audioRenderer instance, if create is successful.
 *             rendererOptions.streamInfo.samplingRate = SAMPLE_RATE_48000;
 *             rendererOptions.streamInfo.encoding = ENCODING_PCM;
 *             rendererOptions.streamInfo.format = SAMPLE_S24LE;
 *             rendererOptions.streamInfo.channels = STEREO;
 *             rendererOptions.rendererInfo.contentType = CONTENT_TYPE_MOVIE;
 *             rendererOptions.rendererInfo.streamUsage = STREAM_USAGE_MEDIA;
 *             rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Create_011, TestSize.Level0)
{
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_48000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S24LE;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MOVIE;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_MEDIA;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Create API via legal input.
 * @tc.number: Audio_Renderer_Create_012
 * @tc.desc  : Test Create interface with AudioRendererOptions below.
 *             Returns audioRenderer instance, if create is successful.
 *             rendererOptions.streamInfo.samplingRate = SAMPLE_RATE_44100;
 *             rendererOptions.streamInfo.encoding = ENCODING_PCM;
 *             rendererOptions.streamInfo.format = SAMPLE_S16LE;
 *             rendererOptions.streamInfo.channels = MONO;
 *             rendererOptions.rendererInfo.contentType = CONTENT_TYPE_SONIFICATION;
 *             rendererOptions.rendererInfo.streamUsage = STREAM_USAGE_VOICE_ASSISTANT;
 *             rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Create_012, TestSize.Level0)
{
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_44100;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptions.streamInfo.channels = AudioChannel::MONO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_SONIFICATION;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VOICE_ASSISTANT;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Create API via legal input.
 * @tc.number: Audio_Renderer_Create_013
 * @tc.desc  : Test Create interface with AudioRendererOptions below.
 *             Returns audioRenderer instance, if create is successful.
 *             rendererOptions.streamInfo.samplingRate = SAMPLE_RATE_22050;
 *             rendererOptions.streamInfo.encoding = ENCODING_PCM;
 *             rendererOptions.streamInfo.format = SAMPLE_S24LE;
 *             rendererOptions.streamInfo.channels = STEREO;
 *             rendererOptions.rendererInfo.contentType = CONTENT_TYPE_SPEECH;
 *             rendererOptions.rendererInfo.streamUsage = STREAM_USAGE_VOICE_COMMUNICATION;
 *             rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Create_013, TestSize.Level0)
{
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_22050;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S24LE;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_SPEECH;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VOICE_COMMUNICATION;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Create API via legal input.
 * @tc.number: Audio_Renderer_Create_014
 * @tc.desc  : Test Create interface with AudioRendererOptions below.
 *             Returns audioRenderer instance, if create is successful.
 *             rendererOptions.streamInfo.samplingRate = SAMPLE_RATE_12000;
 *             rendererOptions.streamInfo.encoding = ENCODING_PCM;
 *             rendererOptions.streamInfo.format = SAMPLE_S24LE;
 *             rendererOptions.streamInfo.channels = MONO;
 *             rendererOptions.rendererInfo.contentType = CONTENT_TYPE_MUSIC;
 *             rendererOptions.rendererInfo.streamUsage = STREAM_USAGE_VOICE_ASSISTANT;
 *             rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Create_014, TestSize.Level0)
{
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_12000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S24LE;
    rendererOptions.streamInfo.channels = AudioChannel::MONO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MUSIC;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VOICE_ASSISTANT;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Create API via legal input.
 * @tc.number: Audio_Renderer_Create_015
 * @tc.desc  : Test Create interface with STREAM_MUSIC. Returns audioRenderer instance, if create is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Create_015, TestSize.Level0)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MEDIA);
    EXPECT_NE(nullptr, audioRenderer);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test CheckMaxRendererInstances API
 * @tc.number: Audio_Renderer_CheckMaxRendererInstances_001
 * @tc.desc  : Test CheckMaxRendererInstances interface. Returns SUCCESS, if check max renderer instances is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_CheckMaxRendererInstances_001, TestSize.Level0)
{
    int32_t result = AudioRenderer::CheckMaxRendererInstances();
    EXPECT_EQ(SUCCESS, result);
}

/**
 * @tc.name  : Test Mute API
 * @tc.number: Audio_Renderer_Mute_001
 * @tc.desc  : Test Mute interface. Returns true, if check Mute is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Mute_001, TestSize.Level0)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MEDIA);
    EXPECT_NE(nullptr, audioRenderer);
    bool result = audioRenderer->Mute();
    EXPECT_TRUE(result);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Unmute API
 * @tc.number: Audio_Renderer_Unmute_001
 * @tc.desc  : Test Unmute interface. Returns true, if check Unmute is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Unmute_001, TestSize.Level0)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MEDIA);
    EXPECT_NE(nullptr, audioRenderer);
    bool result = audioRenderer->Unmute();
    EXPECT_TRUE(result);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetDefaultOutputDevice API
 * @tc.number: Audio_Renderer_SetDefaultOutputDevice_001
 * @tc.desc  : Test SetDefaultOutputDevice interface. Returns true, if check Unmute is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetDefaultOutputDevice_001, TestSize.Level0)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MEDIA);
    EXPECT_NE(nullptr, audioRenderer);
    bool result = audioRenderer->SetDefaultOutputDevice(DEVICE_TYPE_INVALID);
    EXPECT_TRUE(result);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Renderer playback
 * @tc.number: Audio_Renderer_Playback_001
 * @tc.desc  : Test normal playback for 2 sec
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Playback_001, TestSize.Level0)
{
    AudioRendererOptions rendererOptions;
    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    thread renderThread(StartRenderThread, audioRenderer.get(), PLAYBACK_DURATION);

    renderThread.join();

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test SetParams API via legal input
 * @tc.number: Audio_Renderer_SetParams_001
 * @tc.desc  : Test SetParams interface. Returns 0 {SUCCESS}, if the setting is successful.
 *             rendererParams.sampleFormat = SAMPLE_S16LE;
 *             rendererParams.sampleRate = SAMPLE_RATE_44100;
 *             rendererParams.channelCount = STEREO;
 *             rendererParams.encodingType = ENCODING_PCM;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetParams_001, TestSize.Level1)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererParams rendererParams;
    rendererParams.sampleFormat = SAMPLE_S16LE;
    rendererParams.sampleRate = SAMPLE_RATE_44100;
    rendererParams.channelCount = STEREO;
    rendererParams.encodingType = ENCODING_PCM;

    int32_t ret = audioRenderer->SetParams(rendererParams);
    EXPECT_EQ(SUCCESS, ret);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetParams API via legal input.
 * @tc.number: Audio_Renderer_SetParams_002
 * @tc.desc  : Test SetParams interface. Returns 0 {SUCCESS}, if the setting is successful.
 *             rendererParams.sampleFormat = SAMPLE_S16LE;
 *             rendererParams.sampleRate = SAMPLE_RATE_8000;
 *             rendererParams.channelCount = MONO;
 *             rendererParams.encodingType = ENCODING_PCM;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetParams_002, TestSize.Level1)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererParams rendererParams;
    rendererParams.sampleFormat = SAMPLE_S16LE;
    rendererParams.sampleRate = SAMPLE_RATE_8000;
    rendererParams.channelCount = MONO;
    rendererParams.encodingType = ENCODING_PCM;

    int32_t ret = audioRenderer->SetParams(rendererParams);
    EXPECT_EQ(SUCCESS, ret);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetParams API via legal input.
 * @tc.number: Audio_Renderer_SetParams_003
 * @tc.desc  : Test SetParams interface. Returns 0 {SUCCESS}, if the setting is successful.
 *             rendererParams.sampleFormat = SAMPLE_S16LE;
 *             rendererParams.sampleRate = SAMPLE_RATE_11025;
 *             rendererParams.channelCount = STEREO;
 *             rendererParams.encodingType = ENCODING_PCM;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetParams_003, TestSize.Level1)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererParams rendererParams;
    rendererParams.sampleFormat = SAMPLE_S16LE;
    rendererParams.sampleRate = SAMPLE_RATE_11025;
    rendererParams.channelCount = STEREO;
    rendererParams.encodingType = ENCODING_PCM;

    int32_t ret = audioRenderer->SetParams(rendererParams);
    EXPECT_EQ(SUCCESS, ret);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetParams API via legal input.
 * @tc.number: Audio_Renderer_SetParams_004
 * @tc.desc  : Test SetParams interface. Returns 0 {SUCCESS}, if the setting is successful.
 *             rendererParams.sampleFormat = SAMPLE_S16LE;
 *             rendererParams.sampleRate = SAMPLE_RATE_22050;
 *             rendererParams.channelCount = MONO;
 *             rendererParams.encodingType = ENCODING_PCM;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetParams_004, TestSize.Level1)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererParams rendererParams;
    rendererParams.sampleFormat = SAMPLE_S16LE;
    rendererParams.sampleRate = SAMPLE_RATE_22050;
    rendererParams.channelCount = MONO;
    rendererParams.encodingType = ENCODING_PCM;

    int32_t ret = audioRenderer->SetParams(rendererParams);
    EXPECT_EQ(SUCCESS, ret);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetParams API via legal input.
 * @tc.number: Audio_Renderer_SetParams_005
 * @tc.desc  : Test SetParams interface. Returns 0 {SUCCESS}, if the setting is successful.
 *             rendererParams.sampleFormat = SAMPLE_S16LE;
 *             rendererParams.sampleRate = SAMPLE_RATE_96000;
 *             rendererParams.channelCount = MONO;
 *             rendererParams.encodingType = ENCODING_PCM;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetParams_005, TestSize.Level1)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererParams rendererParams;
    rendererParams.sampleFormat = SAMPLE_S16LE;
    rendererParams.sampleRate = SAMPLE_RATE_96000;
    rendererParams.channelCount = MONO;
    rendererParams.encodingType = ENCODING_PCM;

    int32_t ret = audioRenderer->SetParams(rendererParams);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test SetParams API via legal input.
 * @tc.number: Audio_Renderer_SetParams_006
 * @tc.desc  : Test SetParams interface. Returns 0 {SUCCESS}, if the setting is successful.
 *             rendererParams.sampleFormat = SAMPLE_S24LE;
 *             rendererParams.sampleRate = SAMPLE_RATE_64000;
 *             rendererParams.channelCount = MONO;
 *             rendererParams.encodingType = ENCODING_PCM;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetParams_006, TestSize.Level1)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererParams rendererParams;
    rendererParams.sampleFormat = SAMPLE_S24LE;
    rendererParams.sampleRate = SAMPLE_RATE_64000;
    rendererParams.channelCount = MONO;
    rendererParams.encodingType = ENCODING_PCM;

    int32_t ret = audioRenderer->SetParams(rendererParams);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test SetParams API via illegal input.
 * @tc.number: Audio_Renderer_SetParams_007
 * @tc.desc  : Test SetParams interface. Returns 0 {SUCCESS}, if the setting is successful.
 *             rendererParams.sampleFormat = SAMPLE_S16LE;
 *             rendererParams.sampleRate = SAMPLE_RATE_16000;
 *             rendererParams.channelCount = STEREO;
 *             rendererParams.encodingType = ENCODING_PCM;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetParams_007, TestSize.Level1)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererParams rendererParams;
    rendererParams.sampleFormat = SAMPLE_S16LE;
    rendererParams.sampleRate = SAMPLE_RATE_16000;
    rendererParams.channelCount = STEREO;
    rendererParams.encodingType = ENCODING_PCM;

    int32_t ret = audioRenderer->SetParams(rendererParams);
    EXPECT_EQ(SUCCESS, ret);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetParams API via legal input.
 * @tc.number: Audio_Renderer_SetParams_008
 * @tc.desc  : Test SetParams interface. Returns 0 {SUCCESS}, if the setting is successful.
 *             rendererParams.sampleFormat = SAMPLE_F32LE;
 *             rendererParams.sampleRate = SAMPLE_RATE_44100;
 *             rendererParams.channelCount = STEREO;
 *             rendererParams.encodingType = ENCODING_PCM;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetParams_008, TestSize.Level1)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererParams rendererParams;
    rendererParams.sampleFormat = SAMPLE_F32LE;
    rendererParams.sampleRate = SAMPLE_RATE_44100;
    rendererParams.channelCount = STEREO;
    rendererParams.encodingType = ENCODING_PCM;

    int32_t ret = audioRenderer->SetParams(rendererParams);
    EXPECT_EQ(SUCCESS, ret);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetParams API stability.
 * @tc.number: Audio_Renderer_SetParams_Stability_001
 * @tc.desc  : Test SetParams interface stability.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetParams_Stability_001, TestSize.Level1)
{
    int32_t ret = -1;
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererParams rendererParams;
    rendererParams.sampleFormat = SAMPLE_S16LE;
    rendererParams.sampleRate = SAMPLE_RATE_44100;
    rendererParams.channelCount = STEREO;
    rendererParams.encodingType = ENCODING_PCM;

    for (int i = 0; i < VALUE_HUNDRED; i++) {
        ret = audioRenderer->SetParams(rendererParams);
        EXPECT_EQ(SUCCESS, ret);

        AudioRendererParams getRendererParams;
        ret = audioRenderer->GetParams(getRendererParams);
        EXPECT_EQ(SUCCESS, ret);
    }

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetParams API via legal input.
 * @tc.number: Audio_Renderer_GetParams_001
 * @tc.desc  : Test GetParams interface. Returns 0 {SUCCESS}, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetParams_001, TestSize.Level1)
{
    int32_t ret = -1;
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererParams rendererParams;
    rendererParams.sampleFormat = SAMPLE_S16LE;
    rendererParams.sampleRate = SAMPLE_RATE_44100;
    rendererParams.channelCount = STEREO;
    rendererParams.encodingType = ENCODING_PCM;
    ret = audioRenderer->SetParams(rendererParams);
    EXPECT_EQ(SUCCESS, ret);

    AudioRendererParams getRendererParams;
    ret = audioRenderer->GetParams(getRendererParams);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(rendererParams.sampleFormat, getRendererParams.sampleFormat);
    EXPECT_EQ(rendererParams.sampleRate, getRendererParams.sampleRate);
    EXPECT_EQ(rendererParams.channelCount, getRendererParams.channelCount);
    EXPECT_EQ(rendererParams.encodingType, getRendererParams.encodingType);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetParams API via legal state, RENDERER_RUNNING: GetParams after Start.
 * @tc.number: Audio_Renderer_GetParams_002
 * @tc.desc  : Test GetParams interface. Returns 0 {SUCCESS} if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetParams_002, TestSize.Level1)
{
    int32_t ret = -1;
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererParams rendererParams;
    rendererParams.sampleFormat = SAMPLE_S16LE;
    rendererParams.sampleRate = SAMPLE_RATE_44100;
    rendererParams.channelCount = MONO;
    rendererParams.encodingType = ENCODING_PCM;
    ret = audioRenderer->SetParams(rendererParams);
    EXPECT_EQ(SUCCESS, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    AudioRendererParams getRendererParams;
    ret = audioRenderer->GetParams(getRendererParams);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetParams API via illegal state, RENDERER_NEW: Call GetParams without SetParams.
 * @tc.number: Audio_Renderer_GetParams_003
 * @tc.desc  : Test GetParams interface. Returns error code, if the renderer state is RENDERER_NEW.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetParams_003, TestSize.Level1)
{
    int32_t ret = -1;
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererParams rendererParams;
    rendererParams.sampleFormat = SAMPLE_S16LE;
    rendererParams.sampleRate = SAMPLE_RATE_44100;
    rendererParams.channelCount = MONO;
    rendererParams.encodingType = ENCODING_PCM;

    AudioRendererParams getRendererParams;
    ret = audioRenderer->GetParams(getRendererParams);
    EXPECT_EQ(ERR_OPERATION_FAILED, ret);
}

/**
 * @tc.name  : Test GetParams API via illegal state, RENDERER_RELEASED: Call GetParams after Release.
 * @tc.number: Audio_Renderer_GetParams_004
 * @tc.desc  : Test GetParams interface. Returns error code, if the renderer state is RENDERER_RELEASED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetParams_004, TestSize.Level1)
{
    int32_t ret = -1;
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    ret = AudioRendererUnitTest::InitializeRenderer(audioRenderer);
    EXPECT_EQ(SUCCESS, ret);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    AudioRendererParams getRendererParams;
    ret = audioRenderer->GetParams(getRendererParams);
    EXPECT_EQ(ERR_OPERATION_FAILED, ret);
}

/**
 * @tc.name  : Test GetParams API via legal state, RENDERER_STOPPED: GetParams after Stop.
 * @tc.number: Audio_Renderer_GetParams_005
 * @tc.desc  : Test GetParams interface. Returns 0 {SUCCESS}, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetParams_005, TestSize.Level1)
{
    int32_t ret = -1;
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    ret = AudioRendererUnitTest::InitializeRenderer(audioRenderer);
    EXPECT_EQ(SUCCESS, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    AudioRendererParams getRendererParams;
    ret = audioRenderer->GetParams(getRendererParams);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetParams API via legal input.
 * @tc.number: Audio_Renderer_GetParams_006
 * @tc.desc  : Test GetParams interface. Returns 0 {SUCCESS}, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetParams_006, TestSize.Level1)
{
    int32_t ret = -1;
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererParams rendererParams;
    rendererParams.sampleFormat = SAMPLE_S24LE;
    rendererParams.sampleRate = SAMPLE_RATE_44100;
    rendererParams.channelCount = STEREO;
    rendererParams.encodingType = ENCODING_PCM;
    ret = audioRenderer->SetParams(rendererParams);
    EXPECT_EQ(SUCCESS, ret);

    AudioRendererParams getRendererParams;
    ret = audioRenderer->GetParams(getRendererParams);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(rendererParams.sampleFormat, getRendererParams.sampleFormat);
    EXPECT_EQ(rendererParams.sampleRate, getRendererParams.sampleRate);
    EXPECT_EQ(rendererParams.channelCount, getRendererParams.channelCount);
    EXPECT_EQ(rendererParams.encodingType, getRendererParams.encodingType);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetParams API via legal input.
 * @tc.number: Audio_Renderer_GetParams_007
 * @tc.desc  : Test GetParams interface. Returns 0 {SUCCESS}, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetParams_007, TestSize.Level1)
{
    int32_t ret = -1;
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererParams rendererParams;
    rendererParams.sampleFormat = SAMPLE_S32LE;
    rendererParams.sampleRate = SAMPLE_RATE_44100;
    rendererParams.channelCount = STEREO;
    rendererParams.encodingType = ENCODING_PCM;
    ret = audioRenderer->SetParams(rendererParams);
    EXPECT_EQ(SUCCESS, ret);

    AudioRendererParams getRendererParams;
    ret = audioRenderer->GetParams(getRendererParams);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(rendererParams.sampleFormat, getRendererParams.sampleFormat);
    EXPECT_EQ(rendererParams.sampleRate, getRendererParams.sampleRate);
    EXPECT_EQ(rendererParams.channelCount, getRendererParams.channelCount);
    EXPECT_EQ(rendererParams.encodingType, getRendererParams.encodingType);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetParams API via legal input.
 * @tc.number: Audio_Renderer_GetParams_008
 * @tc.desc  : Test GetParams interface. Returns 0 {SUCCESS}, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetParams_008, TestSize.Level1)
{
    int32_t ret = -1;
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererParams getRendererParams;
    getRendererParams.sampleFormat = AudioSampleFormat::INVALID_WIDTH;
    ret = audioRenderer->GetParams(getRendererParams);
    EXPECT_EQ(true, ret < 0);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetInterruptMode API via legal input
 * @tc.number: Audio_Renderer_SetInterruptMode_001
 * @tc.desc  : Test SetInterruptMode interface. Returns 0 {SUCCESS}, if the setting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetInterruptMode_001, TestSize.Level1)
{
    int32_t ret = -1;
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    ret = AudioRendererUnitTest::InitializeRenderer(audioRenderer);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->SetInterruptMode(SHARE_MODE);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetInterruptMode API via legal input
 * @tc.number: Audio_Renderer_SetInterruptMode_002
 * @tc.desc  : Test SetInterruptMode interface. Returns 0 {SUCCESS}, if the setting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetInterruptMode_002, TestSize.Level1)
{
    int32_t ret = -1;
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    ret = AudioRendererUnitTest::InitializeRenderer(audioRenderer);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->SetInterruptMode(INDEPENDENT_MODE);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetBufQueueState
 * @tc.number: Audio_Renderer_GetBufQueueState_001
 * @tc.desc  : Test GetBufQueueState interface. Returns BufferQueueState, if obtained successfully.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetBufQueueState_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
    EXPECT_EQ(SUCCESS, ret);
    AudioRenderMode renderMode = audioRenderer->GetRenderMode();
    EXPECT_EQ(RENDER_MODE_CALLBACK, renderMode);

    shared_ptr<AudioRendererWriteCallback> cb = make_shared<AudioRenderModeCallbackTest>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(SUCCESS, ret);

    BufferQueueState bQueueSate {};
    bQueueSate.currentIndex = 1;
    bQueueSate.numBuffers = 1;

    ret = audioRenderer->GetBufQueueState(bQueueSate);
    EXPECT_EQ(SUCCESS, ret);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetParams API stability.
 * @tc.number: Audio_Renderer_GetParams_Stability_001
 * @tc.desc  : Test GetParams interface stability.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetParams_Stability_001, TestSize.Level1)
{
    int32_t ret = -1;
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererParams rendererParams;
    rendererParams.sampleFormat = SAMPLE_S16LE;
    rendererParams.sampleRate = SAMPLE_RATE_44100;
    rendererParams.channelCount = STEREO;
    rendererParams.encodingType = ENCODING_PCM;

    ret = audioRenderer->SetParams(rendererParams);
    EXPECT_EQ(SUCCESS, ret);

    for (int i = 0; i < VALUE_THOUSAND; i++) {
        AudioRendererParams getRendererParams;
        ret = audioRenderer->GetParams(getRendererParams);
        EXPECT_EQ(SUCCESS, ret);
    }

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetBufferSize API via legal input.
 * @tc.number: Audio_Renderer_GetBufferSize_001
 * @tc.desc  : Test GetBufferSize interface. Returns 0 {SUCCESS}, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetBufferSize_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetBufferSize API via illegal state, RENDERER_NEW: without initializing the renderer.
 * @tc.number: Audio_Renderer_GetBufferSize_002
 * @tc.desc  : Test GetBufferSize interface. Returns error code, if the renderer state is RENDERER_NEW.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetBufferSize_002, TestSize.Level1)
{
    int32_t ret = -1;
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(VALUE_ZERO, ret);
}

/**
 * @tc.name  : Test GetBufferSize API via illegal state, RENDERER_RELEASED: call Release before GetBufferSize
 * @tc.number: Audio_Renderer_GetBufferSize_003
 * @tc.desc  : Test GetBufferSize interface. Returns error code, if the renderer state is RENDERER_RELEASED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetBufferSize_003, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(ERR_ILLEGAL_STATE, ret);
}

/**
 * @tc.name  : Test GetBufferSize API via legal state, RENDERER_STOPPED: call Stop before GetBufferSize
 * @tc.number: Audio_Renderer_GetBufferSize_004
 * @tc.desc  : Test GetBufferSize interface. Returns 0 {SUCCESS}, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetBufferSize_004, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetBufferSize API via legal state, RENDERER_RUNNING: call Start before GetBufferSize
 * @tc.number: Audio_Renderer_GetBufferSize_005
 * @tc.desc  : test GetBufferSize interface. Returns 0 {SUCCESS}, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetBufferSize_005, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetAudioStreamId API stability.
 * @tc.number: Audio_Renderer_GetAudioStreamId_001
 * @tc.desc  : Test GetAudioStreamId interface stability.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioStreamId_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    uint32_t sessionID;
    ret = audioRenderer->GetAudioStreamId(sessionID);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetAudioRendererDesc API stability.
 * @tc.number: Audio_Renderer_SetAudioRendererDesc_001
 * @tc.desc  : Test SetAudioRendererDesc interface stability.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetAudioRendererDesc_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererDesc audioRD = {CONTENT_TYPE_MUSIC, STREAM_USAGE_VOICE_COMMUNICATION};
    ret = audioRenderer->SetAudioRendererDesc(audioRD);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetStreamType API stability.
 * @tc.number: Audio_Renderer_SetStreamType_001
 * @tc.desc  : Test SetStreamType interface stability.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetStreamType_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    AudioStreamType audioStreamType = STREAM_MUSIC;
    ret = audioRenderer->SetStreamType(audioStreamType);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetFrameCount API via legal input.
 * @tc.number: Audio_Renderer_GetFrameCount_001
 * @tc.desc  : test GetFrameCount interface, Returns 0 {SUCCESS}, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetFrameCount_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    uint32_t frameCount;
    ret = audioRenderer->GetFrameCount(frameCount);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetFrameCount API via illegal state, RENDERER_NEW: without initialiing the renderer.
 * @tc.number: Audio_Renderer_GetFrameCount_002
 * @tc.desc  : Test GetFrameCount interface. Returns error code, if the renderer state is RENDERER_NEW.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetFrameCount_002, TestSize.Level1)
{
    int32_t ret = -1;
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    uint32_t frameCount;
    ret = audioRenderer->GetFrameCount(frameCount);
    EXPECT_EQ(VALUE_ZERO, ret);
}

/**
 * @tc.name  : Test GetFrameCount API via legal state, RENDERER_RUNNING: call Start before GetFrameCount.
 * @tc.number: Audio_Renderer_GetFrameCount_003
 * @tc.desc  : Test GetFrameCount interface. Returns 0 {SUCCESS}, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetFrameCount_003, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    uint32_t frameCount;
    ret = audioRenderer->GetFrameCount(frameCount);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetFrameCount API via legal state, RENDERER_STOPPED: call Stop before GetFrameCount
 * @tc.number: Audio_Renderer_GetFrameCount_004
 * @tc.desc  : Test GetFrameCount interface. Returns 0 {SUCCESS}, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetFrameCount_004, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    uint32_t frameCount;
    ret = audioRenderer->GetFrameCount(frameCount);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetFrameCount API via illegal state, RENDERER_RELEASED: call Release before GetFrameCount
 * @tc.number: Audio_Renderer_GetFrameCount_005
 * @tc.desc  : Test GetFrameCount interface.  Returns error code, if the state is RENDERER_RELEASED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetFrameCount_005, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    uint32_t frameCount;
    ret = audioRenderer->GetFrameCount(frameCount);
    EXPECT_EQ(ERR_ILLEGAL_STATE, ret);
}

/**
 * @tc.name  : Test GetFrameCount API via legal state, RENDERER_PAUSED: call Pause before GetFrameCount
 * @tc.number: Audio_Renderer_GetFrameCount_006
 * @tc.desc  : Test GetFrameCount interface. Returns 0 {SUCCESS}, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetFrameCount_006, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isPaused = audioRenderer->Pause();
    EXPECT_EQ(true, isPaused);

    uint32_t frameCount;
    ret = audioRenderer->GetFrameCount(frameCount);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetFrameCount API via legal input when playing audiovivid in callback mode.
 * @tc.number: Audio_Renderer_GetFrameCount_007
 * @tc.desc  : Test GetFrameCount interface, Returns 0 {SUCCESS}, if the getting is successful.
 *             The frame count should be const 1024 in this situation.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetFrameCount_007, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererSpatialOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);
    ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
    EXPECT_EQ(SUCCESS, ret);

    uint32_t frameCount;
    ret = audioRenderer->GetFrameCount(frameCount);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(AUDIOVIVID_FRAME_COUNT, frameCount);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetVolume
 * @tc.number: Audio_Renderer_SetVolume_001
 * @tc.desc  : Test SetVolume interface, Returns 0 {SUCCESS}, if the track volume is set.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetVolume_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetVolume(0.5);
    EXPECT_EQ(SUCCESS, ret);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test SetVolume
 * @tc.number: Audio_Renderer_SetVolume_002
 * @tc.desc  : Test SetVolume interface for minimum and maximum volumes.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetVolume_002, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetVolume(0);
    EXPECT_EQ(SUCCESS, ret);

    ret = audioRenderer->SetVolume(1.0);
    EXPECT_EQ(SUCCESS, ret);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test SetVolume
 * @tc.number: Audio_Renderer_SetVolume_003
 * @tc.desc  : Test SetVolume interface for out of range values.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetVolume_003, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetVolume(-0.5);
    EXPECT_NE(SUCCESS, ret);

    ret = audioRenderer->SetVolume(1.5);
    EXPECT_NE(SUCCESS, ret);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test SetVolume
 * @tc.number: Audio_Renderer_SetVolume_Stability_001
 * @tc.desc  : Test SetVolume interface stability.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetVolume_Stability_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    thread renderThread(StartRenderThread, audioRenderer.get(), PLAYBACK_DURATION);

    for (int i = 0; i < VALUE_HUNDRED; i++) {
        audioRenderer->SetVolume(0.1);
        audioRenderer->SetVolume(1.0);
    }

    renderThread.join();

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test GetVolume
 * @tc.number: Audio_Renderer_GetVolume_001
 * @tc.desc  : Test GetVolume interface to get the default value.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetVolume_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    float volume = audioRenderer->GetVolume();
    EXPECT_EQ(1.0, volume);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test GetVolume
 * @tc.number: Audio_Renderer_GetVolume_002
 * @tc.desc  : Test GetVolume interface after set volume call.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetVolume_002, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetVolume(0.5);
    EXPECT_EQ(SUCCESS, ret);

    float volume = audioRenderer->GetVolume();
    EXPECT_EQ(0.5, volume);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test GetVolume
 * @tc.number: Audio_Renderer_GetVolume_003
 * @tc.desc  : Test GetVolume interface after set volume fails.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetVolume_003, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetVolume(0.5);
    EXPECT_EQ(SUCCESS, ret);

    ret = audioRenderer->SetVolume(1.5);
    EXPECT_NE(SUCCESS, ret);

    float volume = audioRenderer->GetVolume();
    EXPECT_EQ(0.5, volume);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test SetRenderRate
 * @tc.number: Audio_Renderer_SetRenderRate_001
 * @tc.desc  : Test SetRenderRate interface after set volume fails.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRenderRate_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRendererRate renderRate = RENDER_RATE_NORMAL;
    ret = audioRenderer->SetRenderRate(renderRate);
    EXPECT_EQ(SUCCESS, ret);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetRenderRate
 * @tc.number: Audio_Renderer_GetRenderRate_001
 * @tc.desc  : Test GetRenderRate interface after set volume fails.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetRenderRate_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderRate(RENDER_RATE_DOUBLE);
    EXPECT_EQ(SUCCESS, ret);

    AudioRendererRate renderRate = audioRenderer->GetRenderRate();
    EXPECT_EQ(RENDER_RATE_DOUBLE, renderRate);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Start API via legal state, RENDERER_PREPARED.
 * @tc.number: Audio_Renderer_Start_001
 * @tc.desc  : Test Start interface. Returns true if start is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Start_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test Start API via illegal state, RENDERER_NEW: without initializing the renderer.
 * @tc.number: Audio_Renderer_Start_002
 * @tc.desc  : Test Start interface. Returns false, if the renderer state is RENDERER_NEW.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Start_002, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);
}

/**
 * @tc.name  : Test Start API via illegal state, RENDERER_RELEASED: call Start after Release
 * @tc.number: Audio_Renderer_Start_003
 * @tc.desc  : Test Start interface. Returns false, if the renderer state is RENDERER_RELEASED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Start_003, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(false, isStarted);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test Start API via legal state, RENDERER_STOPPED: Start Stop and then Start again
 * @tc.number: Audio_Renderer_Start_004
 * @tc.desc  : Test Start interface. Returns true, if the start is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Start_004, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test Start API via illegal state, RENDERER_RUNNING : call Start repeatedly
 * @tc.number: Audio_Renderer_Start_005
 * @tc.desc  : Test Start interface. Returns false, if the renderer state is RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Start_005, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    isStarted = audioRenderer->Start();
    EXPECT_EQ(false, isStarted);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test Start API via legal state, RENDERER_PAUSED : call Start after pause
 * @tc.number: Audio_Renderer_Start_005
 * @tc.desc  : Test Start interface. Returns false, if the renderer state is RENDERER_PAUSED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Start_006, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isPaused = audioRenderer->Pause();
    EXPECT_EQ(true, isPaused);

    isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test Write API.
 * @tc.number: Audio_Renderer_Write_001
 * @tc.desc  : Test Write interface. Returns number of bytes written, if the write is successful.
 */

HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_001, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;
    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = 0;
    int32_t bytesWritten = 0;
    size_t minBytes = 4;
    int32_t numBuffersToRender = WRITE_BUFFERS_COUNT;

    while (numBuffersToRender) {
        bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
        bytesWritten = 0;
        while ((static_cast<size_t>(bytesWritten) < bytesToWrite) &&
            ((static_cast<size_t>(bytesToWrite) - bytesWritten) > minBytes)) {
            bytesWritten += audioRenderer->Write(buffer + static_cast<size_t>(bytesWritten),
                                                 bytesToWrite - static_cast<size_t>(bytesWritten));
            EXPECT_GE(bytesWritten, VALUE_ZERO);
            if (bytesWritten < 0) {
                break;
            }
        }
        numBuffersToRender--;
    }

    audioRenderer->Drain();
    audioRenderer->Stop();
    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test Write API via illegl state, RENDERER_NEW : without Initializing the renderer.
 * @tc.number: Audio_Renderer_Write_002
 * @tc.desc  : Test Write interface. Returns error code, if the renderer state is RENDERER_NEW.
 *           : bufferLen is invalid here, firstly bufferLen is validated in Write. So it returns ERR_INVALID_PARAM.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_002, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;
    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
    int32_t bytesWritten = audioRenderer->Write(buffer, bytesToWrite);
    EXPECT_EQ(MIN_CACHE_SIZE, bytesWritten);

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test Write API via illegl state, RENDERER_PREPARED : Write without Start.
 * @tc.number: Audio_Renderer_Write_003
 * @tc.desc  : Test Write interface. Returns error code, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_003, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
    int32_t bytesWritten = audioRenderer->Write(buffer, bytesToWrite);
    EXPECT_EQ(ERR_ILLEGAL_STATE, bytesWritten);

    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test Write API via illegal input, bufferLength = 0.
 * @tc.number: Audio_Renderer_Write_004
 * @tc.desc  : Test Write interface. Returns error code, if the bufferLength <= 0.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_004, TestSize.Level1)
{
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen = 0;

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
    int32_t bytesWritten = audioRenderer->Write(buffer, bytesToWrite);
    EXPECT_EQ(ERR_INVALID_PARAM, bytesWritten);

    audioRenderer->Stop();
    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test Write API via illegal input, buffer = nullptr.
 * @tc.number: Audio_Renderer_Write_005
 * @tc.desc  : Test Write interface. Returns error code, if the buffer = nullptr.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_005, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
    uint8_t *buffer_null = nullptr;
    int32_t bytesWritten = audioRenderer->Write(buffer_null, bytesToWrite);
    EXPECT_EQ(ERR_INVALID_PARAM, bytesWritten);

    audioRenderer->Stop();
    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test Write API via illegal state, RENDERER_STOPPED: Write after Stop
 * @tc.number: Audio_Renderer_Write_006
 * @tc.desc  : Test Write interface. Returns error code, if the renderer state is not RENDERER_RUNNING
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_006, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    size_t bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
    int32_t bytesWritten = audioRenderer->Write(buffer, bytesToWrite);
    EXPECT_EQ(ERR_ILLEGAL_STATE, bytesWritten);

    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test Write API via illegal state, RENDERER_RELEASED: Write after Release
 * @tc.number: Audio_Renderer_Write_007
 * @tc.desc  : Test Write interface. Returns error code, if the renderer state is not RENDERER_RUNNING
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_007, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    size_t bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
    int32_t bytesWritten = audioRenderer->Write(buffer, bytesToWrite);
    EXPECT_EQ(ERR_ILLEGAL_STATE, bytesWritten);

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test Write API.
 * @tc.number: Audio_Renderer_Write_008
 * @tc.desc  : Test Write interface after pause and resume. Returns number of bytes written, if the write is successful.
 */

HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_008, TestSize.Level1)
{
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    int32_t ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = 0;
    int32_t bytesWritten = 0;
    size_t minBytes = 4;
    int32_t numBuffersToRender = WRITE_BUFFERS_COUNT;
    bool pauseTested = false;

    while (numBuffersToRender) {
        bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
        bytesWritten = 0;
        uint64_t currFilePos = ftell(wavFile);
        if (!pauseTested && (currFilePos > PAUSE_BUFFER_POSITION) && audioRenderer->Pause()) {
            pauseTested = true;
            sleep(PAUSE_RENDER_TIME_SECONDS);
            isStarted = audioRenderer->Start();
            EXPECT_EQ(true, isStarted);

            ret = audioRenderer->SetVolume(0.5);
            EXPECT_EQ(SUCCESS, ret);
            float volume = audioRenderer->GetVolume();
            EXPECT_EQ(0.5, volume);
        }

        while ((static_cast<size_t>(bytesWritten) < bytesToWrite) &&
            ((static_cast<size_t>(bytesToWrite) - bytesWritten) > minBytes)) {
            bytesWritten += audioRenderer->Write(buffer + static_cast<size_t>(bytesWritten),
                                                 bytesToWrite - static_cast<size_t>(bytesWritten));
            EXPECT_GE(bytesWritten, VALUE_ZERO);
            if (bytesWritten < 0) {
                break;
            }
        }
        numBuffersToRender--;
    }

    audioRenderer->Drain();
    audioRenderer->Stop();
    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test Write API via illegl render mode, RENDER_MODE_CALLBACK.
 * @tc.number: Audio_Renderer_Write_009
 * @tc.desc  : Test Write interface. Returns error code, if the render mode is RENDER_MODE_CALLBACK.
 *           : In RENDER_MODE_CALLBACK Write API call not supported. By default render mode is RENDER_MODE_NORMAL.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_009, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
    EXPECT_EQ(SUCCESS, ret);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
    int32_t bytesWritten = audioRenderer->Write(buffer, bytesToWrite);
    EXPECT_EQ(ERR_INCORRECT_MODE, bytesWritten);

    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
   * @tc.name  : Test Write API.
   * @tc.number: Audio_Renderer_Write_With_Meta_001
   * @tc.desc  : Test Write interface. Returns number of bytes written, if the write is successful.
   */

HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_With_Meta_001, TestSize.Level1)
{
    FILE *wavFile = fopen(AUDIORENDER_TEST_PCMFILE_PATH.c_str(), "rb");
    FILE *metaFile = fopen(AUDIORENDER_TEST_METAFILE_PATH.c_str(), "rb");
    if (wavFile != nullptr) {
        ASSERT_NE(nullptr, wavFile);
        ASSERT_NE(nullptr, metaFile);

        AudioRendererOptions rendererOptions;
        AudioRendererUnitTest::InitializeRendererSpatialOptions(rendererOptions);
        unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
        ASSERT_NE(nullptr, audioRenderer);

        bool isStarted = audioRenderer->Start();
        EXPECT_EQ(true, isStarted);

        size_t bufferLen;
        uint8_t *buffer;
        uint8_t *metaBuffer;

        AudioRendererUnitTest::GetBuffersAndLen(audioRenderer, buffer, metaBuffer, bufferLen);

        size_t bytesToWrite = 0;
        size_t bytesWritten = 0;
        int32_t numBuffersToRender = WRITE_BUFFERS_COUNT;

        while (numBuffersToRender) {
            bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
            fread(metaBuffer, 1, AVS3METADATA_SIZE, metaFile);
            std::fill(buffer + bytesToWrite, buffer + bufferLen, 0);
            bytesWritten = audioRenderer->Write(buffer, bufferLen, metaBuffer, AVS3METADATA_SIZE);
            EXPECT_GE(bytesWritten, VALUE_ZERO);
            numBuffersToRender--;
        }

        audioRenderer->Drain();
        audioRenderer->Stop();
        audioRenderer->Release();

        AudioRendererUnitTest::ReleaseBufferAndFiles(buffer, metaBuffer, wavFile, metaFile);
    }
}

/**
 * @tc.name  : Test Write API via illegl state, RENDERER_NEW : without Initializing the renderer.
 * @tc.number: Audio_Renderer_Write_With_Meta_002
 * @tc.desc  : Test Write interface. Returns error code, if the renderer state is RENDERER_NEW.
 *           : encodingType is not initialized here, so it returns ERR_NOT_SUPPORTED for encodingType that is wrong.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_With_Meta_002, TestSize.Level1)
{
    FILE *wavFile = fopen(AUDIORENDER_TEST_PCMFILE_PATH.c_str(), "rb");
    FILE *metaFile = fopen(AUDIORENDER_TEST_METAFILE_PATH.c_str(), "rb");
    if (wavFile != nullptr) {
        ASSERT_NE(nullptr, wavFile);
        ASSERT_NE(nullptr, metaFile);

        AudioRendererOptions rendererOptions;
        AudioRendererUnitTest::InitializeRendererSpatialOptions(rendererOptions);
        unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
        ASSERT_NE(nullptr, audioRenderer);

        bool isStarted = audioRenderer->Start();
        EXPECT_EQ(true, isStarted);

        size_t bufferLen;
        uint8_t *buffer;
        uint8_t *metaBuffer;

        int32_t ret = audioRenderer->GetBufferSize(bufferLen);
        EXPECT_EQ(SUCCESS, ret);

        buffer = new uint8_t[bufferLen];
        ASSERT_NE(nullptr, buffer);
        metaBuffer = new uint8_t[AVS3METADATA_SIZE];
        ASSERT_NE(nullptr, metaBuffer);

        fread(buffer, 1, bufferLen, wavFile);
        fread(metaBuffer, 1, AVS3METADATA_SIZE, metaFile);
        int32_t bytesWritten = audioRenderer->Write(buffer, bufferLen, metaBuffer, AVS3METADATA_SIZE);
        EXPECT_EQ(MAX_CACHE_SIZE, bytesWritten);

        AudioRendererUnitTest::ReleaseBufferAndFiles(buffer, metaBuffer, wavFile, metaFile);
    }
}

/**
 * @tc.name  : Test Write API via illegl state, RENDERER_PREPARED : Write without Start.
 * @tc.number: Audio_Renderer_Write_With_Meta_003
 * @tc.desc  : Test Write interface. Returns error code, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_With_Meta_003, TestSize.Level1)
{
    FILE *wavFile = fopen(AUDIORENDER_TEST_PCMFILE_PATH.c_str(), "rb");
    FILE *metaFile = fopen(AUDIORENDER_TEST_METAFILE_PATH.c_str(), "rb");
    if (wavFile != nullptr) {
        ASSERT_NE(nullptr, wavFile);
        ASSERT_NE(nullptr, metaFile);

        AudioRendererOptions rendererOptions;

        AudioRendererUnitTest::InitializeRendererSpatialOptions(rendererOptions);
        unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
        ASSERT_NE(nullptr, audioRenderer);

        size_t bufferLen;
        uint8_t *buffer;
        uint8_t *metaBuffer;

        AudioRendererUnitTest::GetBuffersAndLen(audioRenderer, buffer, metaBuffer, bufferLen);

        fread(buffer, 1, bufferLen, wavFile);
        fread(metaBuffer, 1, AVS3METADATA_SIZE, metaFile);
        int32_t bytesWritten = audioRenderer->Write(buffer, bufferLen, metaBuffer, AVS3METADATA_SIZE);
        EXPECT_EQ(ERR_ILLEGAL_STATE, bytesWritten);

        audioRenderer->Release();

        AudioRendererUnitTest::ReleaseBufferAndFiles(buffer, metaBuffer, wavFile, metaFile);
    }
}

/**
 * @tc.name  : Test Write API via illegal input, bufferLength = 0.
 * @tc.number: Audio_Renderer_Write_With_Meta_004
 * @tc.desc  : Test Write interface. Returns error code, if the bufferLength != samples * inchs * bps.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_With_Meta_004, TestSize.Level1)
{
    FILE *wavFile = fopen(AUDIORENDER_TEST_PCMFILE_PATH.c_str(), "rb");
    FILE *metaFile = fopen(AUDIORENDER_TEST_METAFILE_PATH.c_str(), "rb");
    if (wavFile != nullptr) {
        ASSERT_NE(nullptr, wavFile);
        ASSERT_NE(nullptr, metaFile);

        AudioRendererOptions rendererOptions;

        AudioRendererUnitTest::InitializeRendererSpatialOptions(rendererOptions);
        unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
        ASSERT_NE(nullptr, audioRenderer);

        bool isStarted = audioRenderer->Start();
        EXPECT_EQ(true, isStarted);

        size_t bufferLen = 0;

        uint8_t *buffer = new uint8_t[bufferLen];
        ASSERT_NE(nullptr, buffer);
        uint8_t *metaBuffer = new uint8_t[AVS3METADATA_SIZE];
        ASSERT_NE(nullptr, metaBuffer);

        fread(buffer, 1, bufferLen, wavFile);
        fread(metaBuffer, 1, AVS3METADATA_SIZE, metaFile);

        int32_t bytesWritten = audioRenderer->Write(buffer, bufferLen, metaBuffer, AVS3METADATA_SIZE);
        EXPECT_EQ(ERR_INVALID_PARAM, bytesWritten);

        audioRenderer->Stop();
        audioRenderer->Release();

        AudioRendererUnitTest::ReleaseBufferAndFiles(buffer, metaBuffer, wavFile, metaFile);
    }
}

/**
 * @tc.name  : Test Write API via illegal input, metaLength = 0.
 * @tc.number: Audio_Renderer_Write_With_Meta_005
 * @tc.desc  : Test Write interface. Returns error code, if the metaLength != sizeoof(avs3metadata).
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_With_Meta_005, TestSize.Level1)
{
    FILE *wavFile = fopen(AUDIORENDER_TEST_PCMFILE_PATH.c_str(), "rb");
    FILE *metaFile = fopen(AUDIORENDER_TEST_METAFILE_PATH.c_str(), "rb");
    if (wavFile != nullptr) {
        ASSERT_NE(nullptr, wavFile);
        ASSERT_NE(nullptr, metaFile);

        AudioRendererOptions rendererOptions;

        AudioRendererUnitTest::InitializeRendererSpatialOptions(rendererOptions);
        unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
        ASSERT_NE(nullptr, audioRenderer);

        bool isStarted = audioRenderer->Start();
        EXPECT_EQ(true, isStarted);

        size_t bufferLen;
        int32_t ret = audioRenderer->GetBufferSize(bufferLen);
        EXPECT_EQ(SUCCESS, ret);

        uint8_t *buffer = new uint8_t[bufferLen];
        ASSERT_NE(nullptr, buffer);
        uint8_t *metaBuffer = new uint8_t[0];
        ASSERT_NE(nullptr, metaBuffer);

        fread(buffer, 1, bufferLen, wavFile);
        fread(metaBuffer, 1, 0, metaFile);

        int32_t bytesWritten = audioRenderer->Write(buffer, bufferLen, metaBuffer, 0);
        EXPECT_EQ(ERR_INVALID_PARAM, bytesWritten);

        audioRenderer->Stop();
        audioRenderer->Release();

        AudioRendererUnitTest::ReleaseBufferAndFiles(buffer, metaBuffer, wavFile, metaFile);
    }
}

/**
 * @tc.name  : Test Write API via illegal input, buffer = nullptr.
 * @tc.number: Audio_Renderer_Write_With_Meta_006
 * @tc.desc  : Test Write interface. Returns error code, if the buffer = nullptr.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_With_Meta_006, TestSize.Level1)
{
    FILE *wavFile = fopen(AUDIORENDER_TEST_PCMFILE_PATH.c_str(), "rb");
    FILE *metaFile = fopen(AUDIORENDER_TEST_METAFILE_PATH.c_str(), "rb");
    if (wavFile != nullptr) {
        ASSERT_NE(nullptr, wavFile);
        ASSERT_NE(nullptr, metaFile);

        AudioRendererOptions rendererOptions;

        AudioRendererUnitTest::InitializeRendererSpatialOptions(rendererOptions);
        unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
        ASSERT_NE(nullptr, audioRenderer);

        bool isStarted = audioRenderer->Start();
        EXPECT_EQ(true, isStarted);

        size_t bufferLen;
        uint8_t *buffer;
        uint8_t *metaBuffer;

        AudioRendererUnitTest::GetBuffersAndLen(audioRenderer, buffer, metaBuffer, bufferLen);

        fread(buffer, 1, bufferLen, wavFile);
        fread(metaBuffer, 1, AVS3METADATA_SIZE, metaFile);
        uint8_t *buffer_null = nullptr;
        int32_t bytesWritten = audioRenderer->Write(buffer_null, bufferLen, metaBuffer, AVS3METADATA_SIZE);
        EXPECT_EQ(ERR_INVALID_PARAM, bytesWritten);

        audioRenderer->Stop();
        audioRenderer->Release();

        AudioRendererUnitTest::ReleaseBufferAndFiles(buffer, metaBuffer, wavFile, metaFile);
    }
}

/**
 * @tc.name  : Test Write API via illegal input, metaBuffer = nullptr.
 * @tc.number: Audio_Renderer_Write_With_Meta_007
 * @tc.desc  : Test Write interface. Returns error code, if the metaBuffer = nullptr.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_With_Meta_007, TestSize.Level1)
{
    FILE *wavFile = fopen(AUDIORENDER_TEST_PCMFILE_PATH.c_str(), "rb");
    FILE *metaFile = fopen(AUDIORENDER_TEST_METAFILE_PATH.c_str(), "rb");
    if (wavFile != nullptr) {
        ASSERT_NE(nullptr, wavFile);
        ASSERT_NE(nullptr, metaFile);

        AudioRendererOptions rendererOptions;

        AudioRendererUnitTest::InitializeRendererSpatialOptions(rendererOptions);
        unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
        ASSERT_NE(nullptr, audioRenderer);

        bool isStarted = audioRenderer->Start();
        EXPECT_EQ(true, isStarted);

        size_t bufferLen;
        uint8_t *buffer;
        uint8_t *metaBuffer;

        AudioRendererUnitTest::GetBuffersAndLen(audioRenderer, buffer, metaBuffer, bufferLen);

        fread(buffer, 1, bufferLen, wavFile);
        fread(metaBuffer, 1, AVS3METADATA_SIZE, metaFile);
        uint8_t *buffer_null = nullptr;
        int32_t bytesWritten = audioRenderer->Write(buffer, bufferLen, buffer_null, AVS3METADATA_SIZE);
        EXPECT_EQ(ERR_INVALID_PARAM, bytesWritten);

        audioRenderer->Stop();
        audioRenderer->Release();

        AudioRendererUnitTest::ReleaseBufferAndFiles(buffer, metaBuffer, wavFile, metaFile);
    }
}

/**
 * @tc.name  : Test Write API via illegal state, RENDERER_STOPPED: Write after Stop
 * @tc.number: Audio_Renderer_Write_With_Meta_008
 * @tc.desc  : Test Write interface. Returns error code, if the renderer state is not RENDERER_RUNNING
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_With_Meta_008, TestSize.Level1)
{
    FILE *wavFile = fopen(AUDIORENDER_TEST_PCMFILE_PATH.c_str(), "rb");
    FILE *metaFile = fopen(AUDIORENDER_TEST_METAFILE_PATH.c_str(), "rb");
    if (wavFile != nullptr) {
        ASSERT_NE(nullptr, wavFile);
        ASSERT_NE(nullptr, metaFile);

        AudioRendererOptions rendererOptions;

        AudioRendererUnitTest::InitializeRendererSpatialOptions(rendererOptions);
        unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
        ASSERT_NE(nullptr, audioRenderer);

        bool isStarted = audioRenderer->Start();
        EXPECT_EQ(true, isStarted);

        size_t bufferLen;
        uint8_t *buffer;
        uint8_t *metaBuffer;

        AudioRendererUnitTest::GetBuffersAndLen(audioRenderer, buffer, metaBuffer, bufferLen);

        bool isStopped = audioRenderer->Stop();
        EXPECT_EQ(true, isStopped);

        fread(buffer, 1, bufferLen, wavFile);
        fread(metaBuffer, 1, AVS3METADATA_SIZE, metaFile);
        int32_t bytesWritten = audioRenderer->Write(buffer, bufferLen, metaBuffer, AVS3METADATA_SIZE);
        EXPECT_EQ(ERR_ILLEGAL_STATE, bytesWritten);

        audioRenderer->Release();

        AudioRendererUnitTest::ReleaseBufferAndFiles(buffer, metaBuffer, wavFile, metaFile);
    }
}

/**
 * @tc.name  : Test Write API via illegal state, RENDERER_RELEASED: Write after Release
 * @tc.number: Audio_Renderer_Write_With_Meta_009
 * @tc.desc  : Test Write interface. Returns error code, if the renderer state is not RENDERER_RUNNING
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_With_Meta_009, TestSize.Level1)
{
    FILE *wavFile = fopen(AUDIORENDER_TEST_PCMFILE_PATH.c_str(), "rb");
    FILE *metaFile = fopen(AUDIORENDER_TEST_METAFILE_PATH.c_str(), "rb");
    if (wavFile != nullptr) {
        ASSERT_NE(nullptr, wavFile);
        ASSERT_NE(nullptr, metaFile);

        AudioRendererOptions rendererOptions;

        AudioRendererUnitTest::InitializeRendererSpatialOptions(rendererOptions);
        unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
        ASSERT_NE(nullptr, audioRenderer);

        bool isStarted = audioRenderer->Start();
        EXPECT_EQ(true, isStarted);

        size_t bufferLen;
        uint8_t *buffer;
        uint8_t *metaBuffer;

        AudioRendererUnitTest::GetBuffersAndLen(audioRenderer, buffer, metaBuffer, bufferLen);

        bool isReleased = audioRenderer->Release();
        EXPECT_EQ(true, isReleased);


        fread(buffer, 1, bufferLen, wavFile);
        fread(metaBuffer, 1, AVS3METADATA_SIZE, metaFile);
        int32_t bytesWritten = audioRenderer->Write(buffer, bufferLen, metaBuffer, AVS3METADATA_SIZE);
        EXPECT_EQ(ERR_ILLEGAL_STATE, bytesWritten);

        AudioRendererUnitTest::ReleaseBufferAndFiles(buffer, metaBuffer, wavFile, metaFile);
    }
}

/**
 * @tc.name  : Test Write API.
 * @tc.number: Audio_Renderer_Write_With_Meta_010
 * @tc.desc  : Test Write interface after pause and resume. Returns number of bytes written, if the write is successful.
 */

HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_With_Meta_010, TestSize.Level1)
{
    FILE *wavFile = fopen(AUDIORENDER_TEST_PCMFILE_PATH.c_str(), "rb");
    FILE *metaFile = fopen(AUDIORENDER_TEST_METAFILE_PATH.c_str(), "rb");
    if (wavFile != nullptr) {
        ASSERT_NE(nullptr, wavFile);
        ASSERT_NE(nullptr, metaFile);

        AudioRendererOptions rendererOptions;

        AudioRendererUnitTest::InitializeRendererSpatialOptions(rendererOptions);
        unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
        ASSERT_NE(nullptr, audioRenderer);

        bool isStarted = audioRenderer->Start();
        EXPECT_EQ(true, isStarted);

        size_t bufferLen;
        uint8_t *buffer;
        uint8_t *metaBuffer;

        AudioRendererUnitTest::GetBuffersAndLen(audioRenderer, buffer, metaBuffer, bufferLen);

        size_t bytesToWrite = 0;
        int32_t bytesWritten = 0;
        int32_t numBuffersToRender = WRITE_BUFFERS_COUNT;
        bool pauseTested = false;

        while (numBuffersToRender) {
            bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
            fread(metaBuffer, 1, AVS3METADATA_SIZE, metaFile);

            std::fill(buffer + bytesToWrite, buffer + bufferLen, 0);

            bytesWritten = 0;
            uint64_t currFilePos = ftell(wavFile);
            if (!pauseTested && (currFilePos > PAUSE_BUFFER_POSITION) && audioRenderer->Pause()) {
                pauseTested = true;
                sleep(PAUSE_RENDER_TIME_SECONDS);
                isStarted = audioRenderer->Start();
                EXPECT_EQ(true, isStarted);

                int32_t ret = audioRenderer->SetVolume(0.5);
                EXPECT_EQ(SUCCESS, ret);
                float volume = audioRenderer->GetVolume();
                EXPECT_EQ(0.5, volume);
            }

            bytesWritten = audioRenderer->Write(buffer, bufferLen, metaBuffer, AVS3METADATA_SIZE);
            EXPECT_GE(bytesWritten, VALUE_ZERO);
            numBuffersToRender--;
        }

        audioRenderer->Drain();
        audioRenderer->Stop();
        audioRenderer->Release();

        AudioRendererUnitTest::ReleaseBufferAndFiles(buffer, metaBuffer, wavFile, metaFile);
    }
}

/**
 * @tc.name  : Test Write API via illegl render mode, RENDER_MODE_CALLBACK.
 * @tc.number: Audio_Renderer_Write_With_Meta_011
 * @tc.desc  : Test Write interface. Returns error code, if the render mode is RENDER_MODE_CALLBACK.
 *           : In RENDER_MODE_CALLBACK Write API call not supported. By default render mode is RENDER_MODE_NORMAL.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Write_With_Meta_011, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_PCMFILE_PATH.c_str(), "rb");
    FILE *metaFile = fopen(AUDIORENDER_TEST_METAFILE_PATH.c_str(), "rb");
    if (wavFile != nullptr) {
        ASSERT_NE(nullptr, wavFile);
        ASSERT_NE(nullptr, metaFile);

        AudioRendererOptions rendererOptions;

        AudioRendererUnitTest::InitializeRendererSpatialOptions(rendererOptions);
        unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
        ASSERT_NE(nullptr, audioRenderer);

        ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
        EXPECT_EQ(SUCCESS, ret);

        size_t bufferLen;
        uint8_t *buffer;
        uint8_t *metaBuffer;

        AudioRendererUnitTest::GetBuffersAndLen(audioRenderer, buffer, metaBuffer, bufferLen);

        bool isStarted = audioRenderer->Start();
        EXPECT_EQ(true, isStarted);

        fread(buffer, 1, bufferLen, wavFile);
        fread(metaBuffer, 1, AVS3METADATA_SIZE, metaFile);
        int32_t bytesWritten = audioRenderer->Write(buffer, bufferLen, metaBuffer, AVS3METADATA_SIZE);
        EXPECT_EQ(ERR_INCORRECT_MODE, bytesWritten);

        audioRenderer->Release();

        AudioRendererUnitTest::ReleaseBufferAndFiles(buffer, metaBuffer, wavFile, metaFile);
    }
}

/**
 * @tc.name  : Test GetAudioTime API via legal input.
 * @tc.number: Audio_Renderer_GetAudioTime_001
 * @tc.desc  : Test GetAudioTime interface. Returns true, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioTime_001, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
    int32_t bytesWritten = audioRenderer->Write(buffer, bytesToWrite);
    EXPECT_GE(bytesWritten, VALUE_ZERO);

    Timestamp timestamp;
    bool getAudioTime = audioRenderer->GetAudioTime(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(true, getAudioTime);
    EXPECT_GE(timestamp.time.tv_sec, (const long)VALUE_ZERO);
    EXPECT_GE(timestamp.time.tv_nsec, (const long)VALUE_ZERO);

    audioRenderer->Drain();
    audioRenderer->Stop();
    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test GetAudioTime API via illegal state, RENDERER_NEW: GetAudioTime without initializing the renderer.
 * @tc.number: Audio_Renderer_GetAudioTime_002
 * @tc.desc  : Test GetAudioTime interface. Returns false, if the renderer state is RENDERER_NEW
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioTime_002, TestSize.Level1)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    Timestamp timestamp;
    bool getAudioTime = audioRenderer->GetAudioTime(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(false, getAudioTime);
}

/**
 * @tc.name  : Test GetAudioTime API via legal state, RENDERER_RUNNING.
 * @tc.number: Audio_Renderer_GetAudioTime_003
 * @tc.desc  : test GetAudioTime interface. Returns true, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioTime_003, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    Timestamp timestamp;
    bool getAudioTime = audioRenderer->GetAudioTime(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(true, getAudioTime);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetAudioTime API via legal state, RENDERER_STOPPED.
 * @tc.number: Audio_Renderer_GetAudioTime_004
 * @tc.desc  : Test GetAudioTime interface. Returns true, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioTime_004, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    Timestamp timestamp;
    bool getAudioTime = audioRenderer->GetAudioTime(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(false, getAudioTime);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetAudioTime API via illegal state, RENDERER_RELEASED: GetAudioTime after Release.
 * @tc.number: Audio_Renderer_GetAudioTime_005
 * @tc.desc  : Test GetAudioTime interface. Returns false, if the renderer state is RENDERER_RELEASED
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioTime_005, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    Timestamp timestamp;
    bool getAudioTime = audioRenderer->GetAudioTime(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(false, getAudioTime);
}

/**
 * @tc.name  : Test GetAudioTime API via legal state, RENDERER_PAUSED.
 * @tc.number: Audio_Renderer_GetAudioTime_006
 * @tc.desc  : Test GetAudioTime interface. Returns true, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioTime_006, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isPaused = audioRenderer->Pause();
    EXPECT_EQ(true, isPaused);

    Timestamp timestamp;
    bool getAudioTime = audioRenderer->GetAudioTime(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(true, getAudioTime);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetAudioTime API via legal state, RENDERER_PAUSED.
 * @tc.number: Audio_Renderer_GetAudioTime_007
 * @tc.desc  : Test GetAudioTime interface. Timestamp should be larger after pause 1s.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioTime_007, TestSize.Level2)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferSize = 3528; // 44.1 khz, 20ms
    std::unique_ptr<uint8_t[]> tempBuffer = std::make_unique<uint8_t[]>(bufferSize);
    int loopCount = 20; // 400ms
    while (loopCount-- > 0) {
        audioRenderer->Write(tempBuffer.get(), bufferSize);
    }
    Timestamp timestamp1;
    audioRenderer->GetAudioTime(timestamp1, Timestamp::Timestampbase::MONOTONIC);

    bool isPaused = audioRenderer->Pause();
    EXPECT_EQ(true, isPaused);

    size_t sleepTime = 1000000; // sleep 1s
    usleep(sleepTime);

    isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    loopCount = 10; // 200ms
    while (loopCount-- > 0) {
        audioRenderer->Write(tempBuffer.get(), bufferSize);
    }
    Timestamp timestamp2;
    audioRenderer->GetAudioTime(timestamp2, Timestamp::Timestampbase::MONOTONIC);

    int64_t duration = (timestamp2.time.tv_sec - timestamp1.time.tv_sec) * 1000000 + (timestamp2.time.tv_nsec -
        timestamp1.time.tv_nsec) / VALUE_THOUSAND; // ns -> us
    EXPECT_GE(duration, sleepTime);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test Drain API.
 * @tc.number: Audio_Renderer_Drain_001
 * @tc.desc  : Test Drain interface. Returns true, if the flush is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Drain_001, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
    int32_t bytesWritten = audioRenderer->Write(buffer, bytesToWrite);
    EXPECT_GE(bytesWritten, VALUE_ZERO);

    bool isDrained = audioRenderer->Drain();
    EXPECT_EQ(true, isDrained);

    audioRenderer->Stop();
    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test Drain API via illegal state, RENDERER_NEW: Without initializing the renderer.
 * @tc.number: Audio_Renderer_Drain_002
 * @tc.desc  : Test Drain interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Drain_002, TestSize.Level1)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    bool isDrained = audioRenderer->Drain();
    EXPECT_EQ(false, isDrained);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test Drain API via illegal state, RENDERER_PREPARED: Without Start.
 * @tc.number: Audio_Renderer_Drain_003
 * @tc.desc  : Test Drain interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Drain_003, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isDrained = audioRenderer->Drain();
    EXPECT_EQ(false, isDrained);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test Drain API via illegal state, RENDERER_STOPPED: call Stop before Drain.
 * @tc.number: Audio_Renderer_Drain_004
 * @tc.desc  : Test Drain interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Drain_004, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    bool isDrained = audioRenderer->Drain();
    EXPECT_EQ(false, isDrained);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test Drain API via illegal state, RENDERER_RELEASED: call Release before Drain.
 * @tc.number: Audio_Renderer_Drain_005
 * @tc.desc  : Test Drain interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Drain_005, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    bool isDrained = audioRenderer->Drain();
    EXPECT_EQ(false, isDrained);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test Drain API via illegal state, RENDERER_PAUSED: call Pause before Drain.
 * @tc.number: Audio_Renderer_Drain_006
 * @tc.desc  : Test Drain interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Drain_006, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isPaused = audioRenderer->Pause();
    EXPECT_EQ(true, isPaused);

    bool isDrained = audioRenderer->Drain();
    EXPECT_EQ(false, isDrained);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test Drain API stability.
 * @tc.number: Audio_Renderer_Drain_Stability_001
 * @tc.desc  : Test Drain interface stability.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Drain_Stability_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    thread renderThread(StartRenderThread, audioRenderer.get(), PLAYBACK_DURATION);

    for (int i = 0; i < VALUE_THOUSAND; i++) {
        bool isDrained = audioRenderer->Drain();
        if (isDrained != true) {
            return ;
        }
    }

    renderThread.join();

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test Flush API.
 * @tc.number: Audio_Renderer_Flush_001
 * @tc.desc  : Test Flush interface. Returns true, if the flush is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Flush_001, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
    int32_t bytesWritten = audioRenderer->Write(buffer, bytesToWrite);
    EXPECT_GE(bytesWritten, VALUE_ZERO);

    bool isFlushed = audioRenderer->Flush();
    EXPECT_EQ(true, isFlushed);

    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test Flush API.
 * @tc.number: Audio_Renderer_Flush_002
 * @tc.desc  : Test Flush interface after Pause call. Returns true, if the flush is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Flush_002, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
    int32_t bytesWritten = audioRenderer->Write(buffer, bytesToWrite);
    EXPECT_GE(bytesWritten, VALUE_ZERO);

    audioRenderer->Pause();

    bool isFlushed = audioRenderer->Flush();
    EXPECT_EQ(true, isFlushed);

    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test Flush API via illegal state, RENDERER_NEW: Without initializing the renderer.
 * @tc.number: Audio_Renderer_Flush_003
 * @tc.desc  : Test Flush interface. Returns false, if the renderer state is not RENDERER_RUNNING or RENDERER_PAUSED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Flush_003, TestSize.Level1)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    bool isFlushed = audioRenderer->Flush();
    EXPECT_EQ(false, isFlushed);
}

/**
 * @tc.name  : Test Flush API via illegal state, RENDERER_PREPARED: Without Start.
 * @tc.number: Audio_Renderer_Flush_004
 * @tc.desc  : Test Flush interface. Returns false, if the renderer state is not RENDERER_RUNNING or RENDERER_PAUSED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Flush_004, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isFlushed = audioRenderer->Flush();
    EXPECT_EQ(false, isFlushed);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test Flush API: call Stop before Flush.
 * @tc.number: Audio_Renderer_Flush_005
 * @tc.desc  : Test Flush interface. Returns true, if the renderer state is RENDERER_STOPPED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Flush_005, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    bool isFlushed = audioRenderer->Flush();
    EXPECT_EQ(true, isFlushed);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test Flush API via illegal state, RENDERER_RELEASED: call Release before Flush.
 * @tc.number: Audio_Renderer_Flush_006
 * @tc.desc  : Test Flush interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Flush_006, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    bool isFlushed = audioRenderer->Flush();
    EXPECT_EQ(false, isFlushed);
}

/**
 * @tc.name  : Test Flush API stability.
 * @tc.number: Audio_Renderer_Flush_Stability_001
 * @tc.desc  : Test Flush interface stability.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Flush_Stability_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    thread renderThread(StartRenderThread, audioRenderer.get(), PLAYBACK_DURATION);

    for (int i = 0; i < VALUE_THOUSAND; i++) {
        bool isFlushed = audioRenderer->Flush();
        EXPECT_EQ(true, isFlushed);
    }

    renderThread.join();

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test Pause API.
 * @tc.number: Audio_Renderer_Pause_001
 * @tc.desc  : Test Pause interface. Returns true, if the pause is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Pause_001, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
    int32_t bytesWritten = audioRenderer->Write(buffer, bytesToWrite);
    EXPECT_GE(bytesWritten, VALUE_ZERO);

    audioRenderer->Drain();

    bool isPaused = audioRenderer->Pause();
    EXPECT_EQ(true, isPaused);

    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test Pause API via illegal state, RENDERER_NEW: call Pause without Initializing the renderer.
 * @tc.number: Audio_Renderer_Pause_002
 * @tc.desc  : Test Pause interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Pause_002, TestSize.Level1)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    bool isPaused = audioRenderer->Pause();
    EXPECT_EQ(false, isPaused);
}

/**
 * @tc.name  : Test Pause API via illegal state, RENDERER_PREPARED: call Pause without Start.
 * @tc.number: Audio_Renderer_Pause_003
 * @tc.desc  : Test Pause interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Pause_003, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isPaused = audioRenderer->Pause();
    EXPECT_EQ(false, isPaused);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test Pause API via illegal state, RENDERER_RELEASED: call Pause after Release.
 * @tc.number: Audio_Renderer_Pause_004
 * @tc.desc  : Test Pause interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Pause_004, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    bool isPaused = audioRenderer->Pause();
    EXPECT_EQ(false, isPaused);
}

/**
 * @tc.name  : Test Pause and resume
 * @tc.number: Audio_Renderer_Pause_005
 * @tc.desc  : Test Pause interface. Returns true , if the pause is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Pause_005, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isPaused = audioRenderer->Pause();
    EXPECT_EQ(true, isPaused);

    isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    audioRenderer->Stop();
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Pause API via illegal state, RENDERER_STOPPED: call Pause after Stop.
 * @tc.number: Audio_Renderer_Pause_006
 * @tc.desc  : Test Pause interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Pause_006, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    bool isPaused = audioRenderer->Pause();
    EXPECT_EQ(false, isPaused);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test PauseTransitent API.
 * @tc.number: Audio_Renderer_PauseTransitent_001
 * @tc.desc  : Test Pause interface. Returns true, if the pause is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_PauseTransitent_001, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
    int32_t bytesWritten = audioRenderer->Write(buffer, bytesToWrite);
    EXPECT_GE(bytesWritten, VALUE_ZERO);

    audioRenderer->Drain();

    bool isPaused = audioRenderer->PauseTransitent();
    EXPECT_EQ(true, isPaused);

    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test PauseTransitent API via illegal state, RENDERER_NEW: call Pause without Initializing the renderer.
 * @tc.number: Audio_Renderer_PauseTransitent_002
 * @tc.desc  : Test Pause interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_PauseTransitent_002, TestSize.Level1)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    bool isPaused = audioRenderer->Pause();
    EXPECT_EQ(false, isPaused);
}

/**
 * @tc.name  : Test PauseTransitent API via illegal state, RENDERER_PREPARED: call Pause without Start.
 * @tc.number: Audio_Renderer_PauseTransitent_003
 * @tc.desc  : Test Pause interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_PauseTransitent_003, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isPaused = audioRenderer->PauseTransitent();
    EXPECT_EQ(false, isPaused);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test PauseTransitent API via illegal state, RENDERER_RELEASED: call Pause after Release.
 * @tc.number: Audio_Renderer_PauseTransitent_004
 * @tc.desc  : Test Pause interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_PauseTransitent_004, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    bool isPaused = audioRenderer->PauseTransitent();
    EXPECT_EQ(false, isPaused);
}

/**
 * @tc.name  : Test Pause and resume
 * @tc.number: Audio_Renderer_PauseTransitent_005
 * @tc.desc  : Test Pause interface. Returns true , if the pause is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_PauseTransitent_005, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isPaused = audioRenderer->PauseTransitent();
    EXPECT_EQ(true, isPaused);

    isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    audioRenderer->Stop();
    audioRenderer->Release();
}

/**
 * @tc.name  : Test PauseTransitent API via illegal state, RENDERER_STOPPED: call Pause after Stop.
 * @tc.number: Audio_Renderer_Pause_006
 * @tc.desc  : Test Pause interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_PauseTransitent_006, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    bool isPaused = audioRenderer->PauseTransitent();
    EXPECT_EQ(false, isPaused);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Pause and resume
 * @tc.number: Audio_Renderer_PauseTransitent_007
 * @tc.desc  : Test Pause interface. Returns false, if the isSwitching_ is true.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_PauseTransitent_007, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);

    audioRendererPrivate->isSwitching_ = true;
    bool ret = audioRendererPrivate->PauseTransitent(CMD_FROM_CLIENT);
    EXPECT_FALSE(ret);
}

/**
 * @tc.name  : Test Pause and resume
 * @tc.number: Audio_Renderer_PauseTransitent_008
 * @tc.desc  : Test Pause interface. Returns true, if the streamUsage is STREAM_USAGE_VOICE_MODEM_COMMUNICATION.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_PauseTransitent_008, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);

    audioRendererPrivate->isSwitching_ = false;
    audioRendererPrivate->audioInterrupt_.streamUsage = STREAM_USAGE_VOICE_MODEM_COMMUNICATION;
    bool ret = audioRendererPrivate->PauseTransitent(CMD_FROM_SYSTEM);
    EXPECT_FALSE(ret);
}

/**
 * @tc.name  : Test Pause and resume
 * @tc.number: Audio_Renderer_Pause_Stability_001
 * @tc.desc  : Test Pause interface for stability.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Pause_Stability_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = 0;
    int32_t bytesWritten = 0;
    size_t minBytes = 4;
    int32_t numBuffersToRender = WRITE_BUFFERS_COUNT;

    while (numBuffersToRender) {
        bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
        bytesWritten = 0;
        while ((static_cast<size_t>(bytesWritten) < bytesToWrite) &&
            ((static_cast<size_t>(bytesToWrite) - bytesWritten) > minBytes)) {
            bytesWritten += audioRenderer->Write(buffer + static_cast<size_t>(bytesWritten),
                                                 bytesToWrite - static_cast<size_t>(bytesWritten));
            EXPECT_GE(bytesWritten, VALUE_ZERO);
            if (bytesWritten < 0) {
                break;
            }
        }
        EXPECT_EQ(true, audioRenderer->Pause());
        EXPECT_EQ(true, audioRenderer->Start());
        numBuffersToRender--;
    }

    audioRenderer->Drain();

    free(buffer);
    fclose(wavFile);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test Stop API.
 * @tc.number: Audio_Renderer_Stop_001
 * @tc.desc  : Test Stop interface. Returns true, if the stop is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Stop_001, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
    int32_t bytesWritten = audioRenderer->Write(buffer, bytesToWrite);
    EXPECT_GE(bytesWritten, VALUE_ZERO);

    audioRenderer->Drain();

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test Stop API via illegal state, RENDERER_NEW: call Stop without Initializing the renderer.
 * @tc.number: Audio_Renderer_Stop_002
 * @tc.desc  : Test Stop interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Stop_002, TestSize.Level1)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(false, isStopped);
}

/**
 * @tc.name  : Test Stop API via illegal state, RENDERER_PREPARED: call Stop without Start.
 * @tc.number: Audio_Renderer_Stop_003
 * @tc.desc  : Test Stop interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Stop_003, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(false, isStopped);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test Stop API via illegal state, RENDERER_RELEASED: call Stop after Release.
 * @tc.number: Audio_Renderer_Stop_004
 * @tc.desc  : Test Stop interface. Returns false, if the renderer state is not RENDERER_RUNNING.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Stop_004, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(false, isStopped);
}

/**
 * @tc.name  : Test Stop API via legal state. call Start, Stop, Start and Stop again
 * @tc.number: Audio_Renderer_Stop_005
 * @tc.desc  : Test Stop interface. Returns true , if the stop is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Stop_005, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Stop API via legal state, RENDERER_PAUSED: call Stop after Pause.
 * @tc.number: Audio_Renderer_Stop_006
 * @tc.desc  : Test Stop interface. Returns false, if the renderer state is not RENDERER_RUNNING or RENDERER_PAUSED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Stop_006, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isPaused = audioRenderer->Pause();
    EXPECT_EQ(true, isPaused);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Release API.
 * @tc.number: Audio_Renderer_Release_001
 * @tc.desc  : Test Release interface. Returns true, if the release is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Release_001, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
    int32_t bytesWritten = audioRenderer->Write(buffer, bytesToWrite);
    EXPECT_GE(bytesWritten, VALUE_ZERO);

    audioRenderer->Drain();
    audioRenderer->Stop();

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test Release API via illegal state, RENDERER_NEW: Call Release without initializing the renderer.
 * @tc.number: Audio_Renderer_Release_002
 * @tc.desc  : Test Release interface, Returns true, if the state is RENDERER_NEW.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Release_002, TestSize.Level1)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test Release API via illegal state, RENDERER_RELEASED: call Release repeatedly.
 * @tc.number: Audio_Renderer_Release_003
 * @tc.desc  : Test Release interface. Returns true, if the state is already RENDERER_RELEASED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Release_003, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test Release API via legal state, RENDERER_RUNNING: call Release after Start
 * @tc.number: Audio_Renderer_Release_004
 * @tc.desc  : Test Release interface. Returns true, if the release is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Release_004, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test Release API via legal state, RENDERER_STOPPED: call release after Start and Stop
 * @tc.number: Audio_Renderer_Release_005
 * @tc.desc  : Test Release interface. Returns true, if the release is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Release_005, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test Release API via legal state, RENDERER_PAUSED: call release after Start and Pause
 * @tc.number: Audio_Renderer_Release_006
 * @tc.desc  : Test Release interface. Returns true, if the release is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Release_006, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isPaused = audioRenderer->Pause();
    EXPECT_EQ(true, isPaused);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test GetStatus API.
 * @tc.number: Audio_Renderer_GetStatus_001
 * @tc.desc  : Test GetStatus interface. Returns correct state on success.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetStatus_001, TestSize.Level1)
{
    RendererState state = RENDERER_INVALID;

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    state = audioRenderer->GetStatus();
    EXPECT_EQ(RENDERER_PREPARED, state);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);
    state = audioRenderer->GetStatus();
    EXPECT_EQ(RENDERER_RUNNING, state);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);
    state = audioRenderer->GetStatus();
    EXPECT_EQ(RENDERER_STOPPED, state);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
    state = audioRenderer->GetStatus();
    EXPECT_EQ(RENDERER_RELEASED, state);
}

/**
 * @tc.name  : Test GetStatus API, call Start without Initializing the renderer
 * @tc.number: Audio_Renderer_GetStatus_002
 * @tc.desc  : Test GetStatus interface. Not changes to RENDERER_RUNNING, if the current state is RENDERER_NEW.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetStatus_002, TestSize.Level1)
{
    RendererState state = RENDERER_INVALID;

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);
    state = audioRenderer->GetStatus();
    EXPECT_EQ(RENDERER_RUNNING, state);
}

/**
 * @tc.name  : Test GetStatus API, call Stop without Start
 * @tc.number: Audio_Renderer_GetStatus_003
 * @tc.desc  : Test GetStatus interface. Not changes to RENDERER_STOPPED, if the current state is RENDERER_PREPARED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetStatus_003, TestSize.Level1)
{
    RendererState state = RENDERER_INVALID;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(false, isStopped);
    state = audioRenderer->GetStatus();
    EXPECT_NE(RENDERER_STOPPED, state);
    EXPECT_EQ(RENDERER_PREPARED, state);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetStatus API, call Start, Stop and then Start again
 * @tc.number: Audio_Renderer_GetStatus_004
 * @tc.desc  : Test GetStatus interface.  Returns correct state on success.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetStatus_004, TestSize.Level1)
{
    RendererState state = RENDERER_INVALID;

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);
    state = audioRenderer->GetStatus();
    EXPECT_EQ(RENDERER_RUNNING, state);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);
    state = audioRenderer->GetStatus();
    EXPECT_EQ(RENDERER_STOPPED, state);

    isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);
    state = audioRenderer->GetStatus();
    EXPECT_EQ(RENDERER_RUNNING, state);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetStatus API, call Release without initializing
 * @tc.number: Audio_Renderer_GetStatus_005
 * @tc.desc  : Test GetStatus interface. Not changes to RENDERER_RELEASED, if the current state is RENDERER_NEW.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetStatus_005, TestSize.Level1)
{
    RendererState state = RENDERER_INVALID;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
    state = audioRenderer->GetStatus();
    EXPECT_EQ(RENDERER_RELEASED, state);
}

/**
 * @tc.name  : Test GetLatency API.
 * @tc.number: Audio_Renderer_GetLatency_001
 * @tc.desc  : Test GetLatency interface. Returns 0 {SUCCESS}, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetLatency_001, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = 0;
    int32_t bytesWritten = 0;
    size_t minBytes = 4;
    int32_t numBuffersToRender = WRITE_BUFFERS_COUNT;

    while (numBuffersToRender) {
        bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
        bytesWritten = 0;
        uint64_t latency;
        ret = audioRenderer->GetLatency(latency);
        EXPECT_EQ(SUCCESS, ret);
        while ((static_cast<size_t>(bytesWritten) < bytesToWrite) &&
            ((static_cast<size_t>(bytesToWrite) - bytesWritten) > minBytes)) {
            bytesWritten += audioRenderer->Write(buffer + static_cast<size_t>(bytesWritten),
                                                 bytesToWrite - static_cast<size_t>(bytesWritten));
            EXPECT_GE(bytesWritten, VALUE_ZERO);
            if (bytesWritten < 0) {
                break;
            }
        }
        numBuffersToRender--;
    }

    audioRenderer->Drain();
    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test GetLatency API via illegal state, RENDERER_NEW: without initializing the renderer
 * @tc.number: Audio_Renderer_GetLatency_002
 * @tc.desc  : Test GetLatency interface. Returns error code, if the renderer state is RENDERER_NEW.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetLatency_002, TestSize.Level1)
{
    int32_t ret = -1;

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    uint64_t latency;
    ret = audioRenderer->GetLatency(latency);
    EXPECT_EQ(VALUE_ZERO, ret);
}

/**
 * @tc.name  : Test GetLatency API via legal state, RENDERER_PREPARED
 * @tc.number: Audio_Renderer_GetLatency_003
 * @tc.desc  : Test GetLatency interface. Returns 0 {SUCCESS}, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetLatency_003, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    uint64_t latency;
    ret = audioRenderer->GetLatency(latency);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetLatency API via legal state, RENDERER_STOPPED: After Stop
 * @tc.number: Audio_Renderer_GetLatency_004
 * @tc.desc  : Test GetLatency interface. Returns 0 {SUCCESS}, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetLatency_004, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    uint64_t latency;
    ret = audioRenderer->GetLatency(latency);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetLatency API via illegal state, RENDERER_RELEASED: After Release
 * @tc.number: Audio_Renderer_GetLatency_005
 * @tc.desc  : Test GetLatency interface. Returns error code, if the renderer state is RENDERER_RELEASED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetLatency_005, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    uint64_t latency;
    ret = audioRenderer->GetLatency(latency);
    EXPECT_EQ(ERR_ILLEGAL_STATE, ret);
}

/**
 * @tc.name  : Test SetRendererCallback with null pointer.
 * @tc.number: Audio_Renderer_SetRendererCallback_001
 * @tc.desc  : Test SetRendererCallback interface. Returns error code, if null pointer is set.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererCallback_001, TestSize.Level1)
{
    int32_t ret = -1;

    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_44100;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MUSIC;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_MEDIA;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRendererCallback(nullptr);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(ERR_INVALID_PARAM, ret);
}

/**
 * @tc.name  : Test SetRendererCallback with valid callback pointer.
 * @tc.number: Audio_Renderer_SetRendererCallback_002
 * @tc.desc  : Test SetRendererCallback interface. Returns success, if valid callback is set.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererCallback_002, TestSize.Level1)
{
    int32_t ret = -1;

    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_44100;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MUSIC;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_MEDIA;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    shared_ptr<AudioRendererCallbackTest> audioRendererCB = make_shared<AudioRendererCallbackTest>();
    ret = audioRenderer->SetRendererCallback(audioRendererCB);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test SetRendererCallback via illegal state, RENDERER_RELEASED: After RELEASED
 * @tc.number: Audio_Renderer_SetRendererCallback_003
 * @tc.desc  : Test SetRendererCallback interface. Returns error, if callback is set in released state.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererCallback_003, TestSize.Level1)
{
    int32_t ret = -1;

    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_44100;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MUSIC;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_MEDIA;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    RendererState state = audioRenderer->GetStatus();
    EXPECT_EQ(RENDERER_RELEASED, state);

    shared_ptr<AudioRendererCallbackTest> audioRendererCB = make_shared<AudioRendererCallbackTest>();
    ret = audioRenderer->SetRendererCallback(audioRendererCB);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(ERR_ILLEGAL_STATE, ret);
}

/**
 * @tc.name  : Test SetRendererCallback via legal state, RENDERER_PREPARED: After PREPARED
 * @tc.number: Audio_Renderer_SetRendererCallback_004
 * @tc.desc  : Test SetRendererCallback interface. Returns success, if callback is set in proper state.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererCallback_004, TestSize.Level1)
{
    int32_t ret = -1;

    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_44100;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MUSIC;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_MEDIA;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    RendererState state = audioRenderer->GetStatus();
    EXPECT_EQ(RENDERER_PREPARED, state);

    shared_ptr<AudioRendererCallbackTest> audioRendererCB = make_shared<AudioRendererCallbackTest>();
    ret = audioRenderer->SetRendererCallback(audioRendererCB);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test SetRenderMode via legal input, RENDER_MODE_CALLBACK
 * @tc.number: Audio_Renderer_SetRenderMode_001
 * @tc.desc  : Test SetRenderMode interface. Returns SUCCESS, if the render mode is successfully set.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRenderMode_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetRenderMode via legal input, RENDER_MODE_NORMAL
 * @tc.number: Audio_Renderer_SetRenderMode_002
 * @tc.desc  : Test SetRenderMode interface. Returns SUCCESS, if the render mode is successfully set.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRenderMode_002, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_NORMAL);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetRenderMode with, RENDER_MODE_CALLBACK
 * @tc.number: Audio_Renderer_GetRenderMode_001
 * @tc.desc  : Test GetRenderMode interface. Returns the current render mode.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetRenderMode_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
    EXPECT_EQ(SUCCESS, ret);
    AudioRenderMode renderMode = audioRenderer->GetRenderMode();
    EXPECT_EQ(RENDER_MODE_CALLBACK, renderMode);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetRenderMode with, RENDER_MODE_NORMAL
 * @tc.number: Audio_Renderer_GetRenderMode_002
 * @tc.desc  : Test GetRenderMode interface. Returns the current render mode.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetRenderMode_002, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_NORMAL);
    EXPECT_EQ(SUCCESS, ret);
    AudioRenderMode renderMode = audioRenderer->GetRenderMode();
    EXPECT_EQ(RENDER_MODE_NORMAL, renderMode);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetRenderMode with, default renderMode
 * @tc.number: Audio_Renderer_GetRenderMode_003
 * @tc.desc  : Test GetRenderMode interface. Returns the default render mode RENDER_MODE_NORMAL.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetRenderMode_003, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    AudioRenderMode renderMode = audioRenderer->GetRenderMode();
    EXPECT_EQ(RENDER_MODE_NORMAL, renderMode);

    audioRenderer->Release();
}
} // namespace AudioStandard
} // namespace OHOS
