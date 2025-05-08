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
    const int32_t VALUE_NEGATIVE = -1;
    const int32_t VALUE_ZERO = 0;
    const int32_t VALUE_HUNDRED = 100;
    const int32_t VALUE_THOUSAND = 1000;
    const int32_t VALUE_ERROR = -62980098;
    const int32_t RENDERER_FLAG = 0;
    // Writing only 500 buffers of data for test
    const int32_t WRITE_BUFFERS_COUNT = 500;
    const int32_t MAX_BUFFER_SIZE = 20000;

    constexpr uint64_t BUFFER_DURATION_FIVE = 5;
    constexpr uint64_t BUFFER_DURATION_TEN = 10;
    constexpr uint64_t BUFFER_DURATION_FIFTEEN = 15;
    constexpr uint64_t BUFFER_DURATION_TWENTY = 20;
    constexpr size_t MAX_RENDERER_INSTANCES = 16;

    constexpr size_t AVS3METADATA_SIZE = 19824;

    static size_t g_reqBufLen = 0;
} // namespace

class CapturerPositionCallbackTest : public CapturerPositionCallback {
public:
    void OnMarkReached(const int64_t &framePosition) override {}
};

class CapturerPeriodPositionCallbackTest : public CapturerPeriodPositionCallback {
public:
    void OnPeriodReached(const int64_t &frameNumber) override {}
};

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
 * @tc.name  : Test SetRendererWriteCallback via legal render mode, RENDER_MODE_CALLBACK
 * @tc.number: Audio_Renderer_SetRendererWriteCallback_001
 * @tc.desc  : Test SetRendererWriteCallback interface. Returns SUCCESS, if the callback is successfully set.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererWriteCallback_001, TestSize.Level1)
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

    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetRendererWriteCallback via illegal render mode, RENDER_MODE_NORMAL
 * @tc.number: Audio_Renderer_SetRendererWriteCallback_002
 * @tc.desc  : Test SetRendererWriteCallback interface. Returns error code, if the render mode is not callback.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererWriteCallback_002, TestSize.Level1)
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

    shared_ptr<AudioRendererWriteCallback> cb = make_shared<AudioRenderModeCallbackTest>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(ERR_INCORRECT_MODE, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetRendererWriteCallback via illegal render mode, default render mode RENDER_MODE_NORMAL
 * @tc.number: Audio_Renderer_SetRendererWriteCallback_003
 * @tc.desc  : Test SetRendererWriteCallback interface. Returns error code, if the render mode is not callback.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererWriteCallback_003, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    shared_ptr<AudioRendererWriteCallback> cb = make_shared<AudioRenderModeCallbackTest>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(ERR_INCORRECT_MODE, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetRendererWriteCallback via illegal input, nullptr
 * @tc.number: Audio_Renderer_SetRendererWriteCallback_004
 * @tc.desc  : Test SetRendererWriteCallback interface. Returns error code, if the callback reference is nullptr.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererWriteCallback_004, TestSize.Level1)
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

    ret = audioRenderer->SetRendererWriteCallback(nullptr);
    EXPECT_EQ(ERR_INVALID_PARAM, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetBufferDesc via legal render mode, RENDER_MODE_CALLBACK
 * @tc.number: Audio_Renderer_GetBufferDesc_001
 * @tc.desc  : Test GetBufferDesc interface. Returns SUCCESS, if BufferDesc obtained successfully.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetBufferDesc_001, TestSize.Level1)
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

    BufferDesc bufDesc {};
    bufDesc.buffer = nullptr;
    bufDesc.dataLength = g_reqBufLen;
    ret = audioRenderer->GetBufferDesc(bufDesc);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_NE(nullptr, bufDesc.buffer);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetBufferDesc via illegal render mode, RENDER_MODE_NORMAL
 * @tc.number: Audio_Renderer_GetBufferDesc_002
 * @tc.desc  : Test GetBufferDesc interface. Returns errorcode, if render mode is not callback.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetBufferDesc_002, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    shared_ptr<AudioRendererWriteCallback> cb = make_shared<AudioRenderModeCallbackTest>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(ERR_INCORRECT_MODE, ret);

    BufferDesc bufDesc {};
    bufDesc.buffer = nullptr;
    bufDesc.dataLength = g_reqBufLen;
    ret = audioRenderer->GetBufferDesc(bufDesc);
    EXPECT_EQ(ERR_INCORRECT_MODE, ret);
    EXPECT_EQ(nullptr, bufDesc.buffer);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test Enqueue via legal render mode, RENDER_MODE_CALLBACK
 * @tc.number: Audio_Renderer_Enqueue_001
 * @tc.desc  : Test Enqueue interface. Returns SUCCESS , if the buff desc enqueued successfully.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Enqueue_001, TestSize.Level1)
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

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    BufferDesc bufDesc {};
    bufDesc.buffer = nullptr;
    bufDesc.dataLength = g_reqBufLen;
    ret = audioRenderer->GetBufferDesc(bufDesc);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_NE(nullptr, bufDesc.buffer);

    ret = audioRenderer->Enqueue(bufDesc);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Stop();
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Enqueue via illegal render mode, RENDER_MODE_NORMAL
 * @tc.number: Audio_Renderer_Enqueue_002
 * @tc.desc  : Test Enqueue interface. Returns error code, if the render mode is not callback.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Enqueue_002, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    shared_ptr<AudioRendererWriteCallback> cb = make_shared<AudioRenderModeCallbackTest>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(ERR_INCORRECT_MODE, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    BufferDesc bufDesc {};
    bufDesc.buffer = nullptr;
    bufDesc.dataLength = g_reqBufLen;
    ret = audioRenderer->GetBufferDesc(bufDesc);
    EXPECT_EQ(ERR_INCORRECT_MODE, ret);

    ret = audioRenderer->Enqueue(bufDesc);
    EXPECT_EQ(ERR_INCORRECT_MODE, ret);

    audioRenderer->Stop();
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Enqueue via illegal input, buffer nullptr
 * @tc.number: Audio_Renderer_Enqueue_003
 * @tc.desc  : Test Enqueue interface. Returns error code, if the buffer nullptr
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Enqueue_003, TestSize.Level1)
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

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    BufferDesc bufDesc {};
    bufDesc.buffer = nullptr;
    bufDesc.dataLength = g_reqBufLen;

    ret = audioRenderer->Enqueue(bufDesc);
    EXPECT_EQ(ERR_INVALID_PARAM, ret);

    audioRenderer->Stop();
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Clear via legal render mode, RENDER_MODE_CALLBACK
 * @tc.number: Audio_Renderer_Clear_001
 * @tc.desc  : Test Clear interface. Returns SUCCESS , if the buff queue cleared successfully.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Clear_001, TestSize.Level1)
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

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    BufferDesc bufDesc {};
    bufDesc.buffer = nullptr;
    bufDesc.dataLength = g_reqBufLen;
    ret = audioRenderer->GetBufferDesc(bufDesc);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_NE(nullptr, bufDesc.buffer);

    ret = audioRenderer->Enqueue(bufDesc);
    EXPECT_EQ(SUCCESS, ret);

    ret = audioRenderer->Clear();
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Stop();
    audioRenderer->Release();
}

/**
 * @tc.name  : Test Clear via illegal render mode, RENDER_MODE_NORMAL
 * @tc.number: Audio_Renderer_Clear_002
 * @tc.desc  : Test Clear interface. Returns error code, if the render mode is not callback.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Clear_002, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    shared_ptr<AudioRendererWriteCallback> cb = make_shared<AudioRenderModeCallbackTest>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(ERR_INCORRECT_MODE, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    BufferDesc bufDesc {};
    bufDesc.buffer = nullptr;
    bufDesc.dataLength = g_reqBufLen;
    ret = audioRenderer->GetBufferDesc(bufDesc);
    EXPECT_EQ(ERR_INCORRECT_MODE, ret);

    ret = audioRenderer->Enqueue(bufDesc);
    EXPECT_EQ(ERR_INCORRECT_MODE, ret);

    ret = audioRenderer->Clear();
    EXPECT_EQ(ERR_INCORRECT_MODE, ret);

    audioRenderer->Stop();
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetRendererInfo API after calling create
 * @tc.number: Audio_Renderer_GetRendererInfo_001
 * @tc.desc  : Test GetRendererInfo interface. Check whether renderer info returns proper data
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetRendererInfo_001, TestSize.Level1)
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
    EXPECT_NE(nullptr, audioRenderer);

    AudioRendererInfo rendererInfo;
    audioRenderer->GetRendererInfo(rendererInfo);

    EXPECT_EQ(ContentType::CONTENT_TYPE_MUSIC, rendererInfo.contentType);
    EXPECT_EQ(StreamUsage::STREAM_USAGE_MEDIA, rendererInfo.streamUsage);
    EXPECT_EQ(RENDERER_FLAG, rendererInfo.rendererFlags);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetRendererInfo API via legal state, RENDERER_RUNNING: GetRendererInfo after Start.
 * @tc.number: Audio_Renderer_GetRendererInfo_002
 * @tc.desc  : Test GetRendererInfo interface. Check whether renderer info returns proper data
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetRendererInfo_002, TestSize.Level1)
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
    EXPECT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    AudioRendererInfo rendererInfo;
    audioRenderer->GetRendererInfo(rendererInfo);

    EXPECT_EQ(ContentType::CONTENT_TYPE_MUSIC, rendererInfo.contentType);
    EXPECT_EQ(StreamUsage::STREAM_USAGE_MEDIA, rendererInfo.streamUsage);
    EXPECT_EQ(RENDERER_FLAG, rendererInfo.rendererFlags);

    audioRenderer->Stop();
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetRendererInfo API via legal state, RENDERER_RELEASED: Call GetRendererInfo after Release.
 * @tc.number: Audio_Renderer_GetRendererInfo_003
 * @tc.desc  : Test GetRendererInfo interface. Check whether renderer info returns proper data
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetRendererInfo_003, TestSize.Level1)
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
    EXPECT_NE(nullptr, audioRenderer);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    AudioRendererInfo rendererInfo;
    audioRenderer->GetRendererInfo(rendererInfo);

    EXPECT_EQ(ContentType::CONTENT_TYPE_MUSIC, rendererInfo.contentType);
    EXPECT_EQ(StreamUsage::STREAM_USAGE_MEDIA, rendererInfo.streamUsage);
    EXPECT_EQ(RENDERER_FLAG, rendererInfo.rendererFlags);
}

/**
 * @tc.name  : Test GetRendererInfo API via legal state, RENDERER_STOPPED: Call GetRendererInfo after Stop.
 * @tc.number: Audio_Renderer_GetRendererInfo_004
 * @tc.desc  : Test GetRendererInfo interface. Check whether renderer info returns proper data
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetRendererInfo_004, TestSize.Level1)
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
    EXPECT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    AudioRendererInfo rendererInfo;
    audioRenderer->GetRendererInfo(rendererInfo);

    EXPECT_EQ(ContentType::CONTENT_TYPE_MUSIC, rendererInfo.contentType);
    EXPECT_EQ(StreamUsage::STREAM_USAGE_MEDIA, rendererInfo.streamUsage);
    EXPECT_EQ(RENDERER_FLAG, rendererInfo.rendererFlags);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetRendererInfo API Stability
 * @tc.number: Audio_Renderer_GetRendererInfo_Stability_001
 * @tc.desc  : Test GetRendererInfo interface Stability
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetRendererInfo_Stability_001, TestSize.Level1)
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
    EXPECT_NE(nullptr, audioRenderer);

    for (int i = 0; i < VALUE_THOUSAND; i++) {

        AudioRendererInfo rendererInfo;
        audioRenderer->GetRendererInfo(rendererInfo);

        EXPECT_EQ(ContentType::CONTENT_TYPE_MUSIC, rendererInfo.contentType);
        EXPECT_EQ(StreamUsage::STREAM_USAGE_MEDIA, rendererInfo.streamUsage);
        EXPECT_EQ(RENDERER_FLAG, rendererInfo.rendererFlags);
    }
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetStreamInfo API after calling create
 * @tc.number: Audio_Renderer_GetStreamInfo_001
 * @tc.desc  : Test GetStreamInfo interface. Check whether stream related data is returned correctly
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetStreamInfo_001, TestSize.Level1)
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
    EXPECT_NE(nullptr, audioRenderer);

    AudioStreamInfo streamInfo;
    audioRenderer->GetStreamInfo(streamInfo);

    EXPECT_EQ(AudioSamplingRate::SAMPLE_RATE_96000, streamInfo.samplingRate);
    EXPECT_EQ(AudioEncodingType::ENCODING_PCM, streamInfo.encoding);
    EXPECT_EQ(AudioSampleFormat::SAMPLE_U8, streamInfo.format);
    EXPECT_EQ(AudioChannel::MONO, streamInfo.channels);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetStreamInfo via legal state,  RENDERER_RUNNING: GetStreamInfo after Start.
 * @tc.number: Audio_Renderer_GetStreamInfo_002
 * @tc.desc  : Test GetStreamInfo interface. Check whether stream related data is returned correctly
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetStreamInfo_002, TestSize.Level1)
{
    int32_t ret = -1;

    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_96000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_U8;
    rendererOptions.streamInfo.channels = AudioChannel::MONO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MUSIC;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_MEDIA;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    EXPECT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    AudioStreamInfo streamInfo;
    ret = audioRenderer->GetStreamInfo(streamInfo);

    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(AudioSamplingRate::SAMPLE_RATE_96000, streamInfo.samplingRate);
    EXPECT_EQ(AudioEncodingType::ENCODING_PCM, streamInfo.encoding);
    EXPECT_EQ(AudioSampleFormat::SAMPLE_U8, streamInfo.format);
    EXPECT_EQ(AudioChannel::MONO, streamInfo.channels);

    audioRenderer->Stop();
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetStreamInfo via illegal state, RENDERER_RELEASED: GetStreamInfo after Release.
 * @tc.number: Audio_Renderer_GetStreamInfo_003
 * @tc.desc  : Test GetStreamInfo interface. Returns error code, if the renderer state is RENDERER_RELEASED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetStreamInfo_003, TestSize.Level1)
{
    int32_t ret = -1;

    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_96000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_U8;
    rendererOptions.streamInfo.channels = AudioChannel::MONO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MUSIC;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_MEDIA;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    EXPECT_NE(nullptr, audioRenderer);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);

    AudioStreamInfo streamInfo;
    ret = audioRenderer->GetStreamInfo(streamInfo);

    EXPECT_EQ(ERR_OPERATION_FAILED, ret);
}

/**
 * @tc.name  : Test GetStreamInfo via legal state, RENDERER_STOPPED: GetStreamInfo after Stop.
 * @tc.number: Audio_Renderer_GetStreamInfo_004
 * @tc.desc  : Test GetStreamInfo interface. Check whether stream related data is returned correctly
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetStreamInfo_004, TestSize.Level1)
{
    int32_t ret = -1;

    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_96000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_U8;
    rendererOptions.streamInfo.channels = AudioChannel::MONO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MUSIC;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_MEDIA;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    EXPECT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);

    AudioStreamInfo streamInfo;
    ret = audioRenderer->GetStreamInfo(streamInfo);

    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(AudioSamplingRate::SAMPLE_RATE_96000, streamInfo.samplingRate);
    EXPECT_EQ(AudioEncodingType::ENCODING_PCM, streamInfo.encoding);
    EXPECT_EQ(AudioSampleFormat::SAMPLE_U8, streamInfo.format);
    EXPECT_EQ(AudioChannel::MONO, streamInfo.channels);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetStreamInfo via legal state, RENDERER_PAUSED: GetStreamInfo after Pause.
 * @tc.number: Audio_Renderer_GetStreamInfo_005
 * @tc.desc  : Test GetStreamInfo interface. Check whether stream related data is returned correctly
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetStreamInfo_005, TestSize.Level1)
{
    int32_t ret = -1;

    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_96000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_U8;
    rendererOptions.streamInfo.channels = AudioChannel::MONO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MUSIC;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_MEDIA;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    EXPECT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    bool isPaused = audioRenderer->Pause();
    EXPECT_EQ(true, isPaused);

    AudioStreamInfo streamInfo;
    ret = audioRenderer->GetStreamInfo(streamInfo);

    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(AudioSamplingRate::SAMPLE_RATE_96000, streamInfo.samplingRate);
    EXPECT_EQ(AudioEncodingType::ENCODING_PCM, streamInfo.encoding);
    EXPECT_EQ(AudioSampleFormat::SAMPLE_U8, streamInfo.format);
    EXPECT_EQ(AudioChannel::MONO, streamInfo.channels);

    audioRenderer->Stop();
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetStreamInfo API stability.
 * @tc.number: Audio_Renderer_GetStreamInfo_Stability_001
 * @tc.desc  : Test GetStreamInfo interface stability
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetStreamInfo_Stability_001, TestSize.Level1)
{
    int32_t ret = -1;

    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_96000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_U8;
    rendererOptions.streamInfo.channels = AudioChannel::MONO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MUSIC;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_MEDIA;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    EXPECT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);


    for (int i = 0; i < VALUE_THOUSAND; i++) {
        AudioStreamInfo streamInfo;
        ret = audioRenderer->GetStreamInfo(streamInfo);
        EXPECT_EQ(SUCCESS, ret);
    }

    audioRenderer->Stop();
    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetBufferDuration API
 * @tc.number: Audio_Renderer_SetBufferDuration_001
 * @tc.desc  : Test SetBufferDuration interface. Check whether valid parameters are accepted.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetBufferDuration_001, TestSize.Level1)
{
    int32_t ret = -1;

    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_96000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_U8;
    rendererOptions.streamInfo.channels = AudioChannel::MONO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MUSIC;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_MEDIA;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    EXPECT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetBufferDuration(BUFFER_DURATION_FIVE);
    EXPECT_EQ(SUCCESS, ret);

    ret = audioRenderer->SetBufferDuration(BUFFER_DURATION_TEN);
    EXPECT_EQ(SUCCESS, ret);

    ret = audioRenderer->SetBufferDuration(BUFFER_DURATION_FIFTEEN);
    EXPECT_EQ(SUCCESS, ret);

    ret = audioRenderer->SetBufferDuration(BUFFER_DURATION_TWENTY);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test SetBufferDuration API
 * @tc.number: Audio_Renderer_SetBufferDuration_002
 * @tc.desc  : Test SetBufferDuration interface. Check whether invalid parameters are rejected.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetBufferDuration_002, TestSize.Level1)
{
    int32_t ret = -1;

    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_96000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_U8;
    rendererOptions.streamInfo.channels = AudioChannel::MONO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_MUSIC;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_MEDIA;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    EXPECT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetBufferDuration(VALUE_NEGATIVE);
    EXPECT_NE(SUCCESS, ret);

    ret = audioRenderer->SetBufferDuration(VALUE_ZERO);
    EXPECT_NE(SUCCESS, ret);

    ret = audioRenderer->SetBufferDuration(VALUE_HUNDRED);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.name  : Test SetRendererPositionCallback API
 * @tc.number: Audio_Renderer_SetRendererPositionCallback_001
 * @tc.desc  : Test SetRendererPositionCallback interface to check set position callback is success for valid callback.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererPositionCallback_001, TestSize.Level1)
{
    int32_t ret = -1;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    shared_ptr<RendererPositionCallbackTest> positionCB = std::make_shared<RendererPositionCallbackTest>();
    ret = audioRenderer->SetRendererPositionCallback(VALUE_THOUSAND, positionCB);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test SetRendererPositionCallback API
 * @tc.number: Audio_Renderer_SetRendererPositionCallback_002
 * @tc.desc  : Test SetRendererPositionCallback interface again after unregister.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererPositionCallback_002, TestSize.Level1)
{
    int32_t ret = -1;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    shared_ptr<RendererPositionCallbackTest> positionCB1 = std::make_shared<RendererPositionCallbackTest>();
    ret = audioRenderer->SetRendererPositionCallback(VALUE_THOUSAND, positionCB1);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->UnsetRendererPositionCallback();

    shared_ptr<RendererPositionCallbackTest> positionCB2 = std::make_shared<RendererPositionCallbackTest>();
    ret = audioRenderer->SetRendererPositionCallback(VALUE_THOUSAND, positionCB2);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test SetRendererPositionCallback API
 * @tc.number: Audio_Renderer_SetRendererPositionCallback_003
 * @tc.desc  : Test SetRendererPositionCallback interface with null callback.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererPositionCallback_003, TestSize.Level1)
{
    int32_t ret = -1;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRendererPositionCallback(VALUE_THOUSAND, nullptr);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.name  : Test SetRendererPositionCallback API
 * @tc.number: Audio_Renderer_SetRendererPositionCallback_004
 * @tc.desc  : Test SetRendererPositionCallback interface with invalid parameter.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererPositionCallback_004, TestSize.Level1)
{
    int32_t ret = -1;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    shared_ptr<RendererPositionCallbackTest> positionCB = std::make_shared<RendererPositionCallbackTest>();
    ret = audioRenderer->SetRendererPositionCallback(VALUE_ZERO, positionCB);
    EXPECT_NE(SUCCESS, ret);

    ret = audioRenderer->SetRendererPositionCallback(VALUE_NEGATIVE, positionCB);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.name  : Test SetRendererPeriodPositionCallback API
 * @tc.number: Audio_Renderer_SetRendererPeriodPositionCallback_001
 * @tc.desc  : Test SetRendererPeriodPositionCallback interface to check set period position
 *             callback is success for valid callback.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererPeriodPositionCallback_001, TestSize.Level1)
{
    int32_t ret = -1;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    shared_ptr<RendererPeriodPositionCallbackTest> positionCB = std::make_shared<RendererPeriodPositionCallbackTest>();
    ret = audioRenderer->SetRendererPeriodPositionCallback(VALUE_THOUSAND, positionCB);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test SetRendererPeriodPositionCallback API
 * @tc.number: Audio_Renderer_SetRendererPeriodPositionCallback_002
 * @tc.desc  : Test SetRendererPeriodPositionCallback interface again after unregister.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererPeriodPositionCallback_002, TestSize.Level1)
{
    int32_t ret = -1;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    shared_ptr<RendererPeriodPositionCallbackTest> positionCB1 = std::make_shared<RendererPeriodPositionCallbackTest>();
    ret = audioRenderer->SetRendererPeriodPositionCallback(VALUE_THOUSAND, positionCB1);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->UnsetRendererPeriodPositionCallback();

    shared_ptr<RendererPeriodPositionCallbackTest> positionCB2 = std::make_shared<RendererPeriodPositionCallbackTest>();
    ret = audioRenderer->SetRendererPeriodPositionCallback(VALUE_THOUSAND, positionCB2);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.name  : Test SetRendererPeriodPositionCallback API
 * @tc.number: Audio_Renderer_SetRendererPeriodPositionCallback_003
 * @tc.desc  : Test SetRendererPeriodPositionCallback interface with null callback.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererPeriodPositionCallback_003, TestSize.Level1)
{
    int32_t ret = -1;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRendererPeriodPositionCallback(VALUE_THOUSAND, nullptr);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.name  : Test SetRendererPeriodPositionCallback API
 * @tc.number: Audio_Renderer_SetRendererPeriodPositionCallback_004
 * @tc.desc  : Test SetRendererPeriodPositionCallback interface with invalid parameter.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererPeriodPositionCallback_004, TestSize.Level1)
{
    int32_t ret = -1;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    shared_ptr<RendererPeriodPositionCallbackTest> positionCB =
        std::make_shared<RendererPeriodPositionCallbackTest>();
    ret = audioRenderer->SetRendererPeriodPositionCallback(VALUE_ZERO, positionCB);
    EXPECT_NE(SUCCESS, ret);

    ret = audioRenderer->SetRendererPeriodPositionCallback(VALUE_NEGATIVE, positionCB);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.name  : Test max renderer instances.
 * @tc.number: Audio_Renderer_Max_Renderer_Instances_001
 * @tc.desc  : Test creating maximum configured audio renderer instances.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Max_Renderer_Instances_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;
    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    vector<unique_ptr<AudioRenderer>> rendererList;
    vector<shared_ptr<AudioRendererChangeInfo>> audioRendererChangeInfos = {};
    AudioPolicyManager::GetInstance().GetCurrentRendererChangeInfos(audioRendererChangeInfos);

    // Create renderer instance with the maximum number of configured instances
    while (audioRendererChangeInfos.size() < MAX_RENDERER_INSTANCES) {
        auto audioRenderer = AudioRenderer::Create(rendererOptions);
        EXPECT_NE(nullptr, audioRenderer);
        rendererList.push_back(std::move(audioRenderer));
        audioRendererChangeInfos.clear();
        AudioPolicyManager::GetInstance().GetCurrentRendererChangeInfos(audioRendererChangeInfos);
    }

    for (auto it = rendererList.begin(); it != rendererList.end();) {
        bool isReleased = (*it)->Release();
        EXPECT_EQ(true, isReleased);
        it = rendererList.erase(it);
    }
    EXPECT_EQ(rendererList.size(), 0);
}

/**
 * @tc.name  : Test set renderer samplingrate.
 * @tc.number: Audio_Renderer_Set_Renderer_SamplingRate_001
 * @tc.desc  : Test SetRendererSamplingRate and GetRendererSamplingRate.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Set_Renderer_SamplingRate_001, TestSize.Level1)
{
    int32_t ret = -1;
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

    uint32_t sampleRate = AudioSamplingRate::SAMPLE_RATE_48000;
    ret = audioRenderer->SetRendererSamplingRate(sampleRate);
    EXPECT_EQ(ERROR, ret);

    uint32_t sampleRateRet = audioRenderer->GetRendererSamplingRate();
    EXPECT_EQ(AudioSamplingRate::SAMPLE_RATE_44100, sampleRateRet);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test set renderer instance.
 * @tc.number: Audio_Renderer_Set_Renderer_Instance_001
 * @tc.desc  : Test renderer instance GetMinStreamVolume,GetMaxStreamVolume,GetCurrentOutputDevices,GetUnderflowCount
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Set_Renderer_Instance_001, TestSize.Level1)
{
    int32_t ret = -1;
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

    float minVolume = audioRenderer->GetMinStreamVolume();
    float maxVolume = audioRenderer->GetMaxStreamVolume();
    EXPECT_LT(minVolume, maxVolume);

    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    ret = audioRenderer->GetCurrentOutputDevices(deviceInfo);
    EXPECT_EQ(SUCCESS, ret);

    float count = audioRenderer->GetUnderflowCount();
    EXPECT_GE(count, 0);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test set renderer instance.
 * @tc.number: Audio_Renderer_Set_Renderer_Instance_003
 * @tc.desc  : Test renderer instance RegisterAudioRendererEventListener,DestroyAudioRendererStateCallback
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Set_Renderer_Instance_003, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_44100;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_SONIFICATION;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VOICE_ASSISTANT;
    rendererOptions.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    int32_t clientPid = getpid();
    const std::shared_ptr<AudioRendererPolicyServiceDiedCallback> serviceCallback =
        std::make_shared<AudioRendererPolicyServiceDiedCallbackTest>();
    ret = audioRenderer->RegisterAudioPolicyServerDiedCb(clientPid, serviceCallback);
    EXPECT_EQ(SUCCESS, ret);

    ret = audioRenderer->RegisterAudioPolicyServerDiedCb(clientPid, nullptr);
    EXPECT_EQ(ERR_INVALID_PARAM, ret);

    ret = audioRenderer->UnregisterAudioPolicyServerDiedCb(clientPid);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test set renderer instance.
 * @tc.number: Audio_Renderer_Set_Renderer_Instance_005
 * @tc.desc  : Test ResumeStreamImpl and PausedStreamImpl on AudioRendererProxyObj
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Set_Renderer_Instance_005, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    unique_ptr<AudioRendererProxyObj> audioRendererProxyObj = std::make_unique<AudioRendererProxyObj>();

    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);
    std::shared_ptr<AudioRenderer> sharedRenderer = std::move(audioRenderer);
    std::weak_ptr<AudioRenderer> weakRenderer = sharedRenderer;

    audioRendererProxyObj->SaveRendererObj(weakRenderer);
    const StreamSetStateEventInternal streamSetStateEventInternal = {};
    audioRendererProxyObj->ResumeStreamImpl(streamSetStateEventInternal);
    audioRendererProxyObj->PausedStreamImpl(streamSetStateEventInternal);
    ASSERT_NE(nullptr, audioRendererPrivate);
}

/**
 * @tc.name  : Test set renderer instance.
 * @tc.number: Audio_Renderer_Set_Renderer_Instance_006
 * @tc.desc  : Test ResumeStreamImpl and PausedStreamImpl on AudioRendererProxyObj when rederer is nullptr
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Set_Renderer_Instance_006, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);

    unique_ptr<AudioRendererProxyObj> audioRendererProxyObj = std::make_unique<AudioRendererProxyObj>();

    audioRendererProxyObj->SaveRendererObj(std::weak_ptr<AudioRendererPrivate>());
    const StreamSetStateEventInternal streamSetStateEventInternal = {};
    audioRendererProxyObj->ResumeStreamImpl(streamSetStateEventInternal);
    audioRendererProxyObj->PausedStreamImpl(streamSetStateEventInternal);
    ASSERT_NE(nullptr, audioRendererPrivate);
}

/**
 * @tc.name  : Test SetAudioEffectMode via legal input, EFFECT_NONE
 * @tc.number: Audio_Renderer_SetAudioEffectMode_001
 * @tc.desc  : Test SetAudioEffectMode interface. Returns SUCCESS, if the effect mode is successfully set.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetAudioEffectMode_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetAudioEffectMode(EFFECT_NONE);
    EXPECT_EQ(SUCCESS, ret);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetAudioEffectMode via legal input, EFFECT_DEFAULT
 * @tc.number: Audio_Renderer_SetAudioEffectMode_002
 * @tc.desc  : Test SetAudioEffectMode interface. Returns SUCCESS, if the effect mode is successfully set.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetAudioEffectMode_002, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetAudioEffectMode(EFFECT_DEFAULT);
    EXPECT_EQ(SUCCESS, ret);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetAudioEffectMode with, EFFECT_NONE
 * @tc.number: Audio_Renderer_GetAudioEffectMode_001
 * @tc.desc  : Test GetAudioEffectMode interface. Returns the current effect mode.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioEffectMode_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetAudioEffectMode(EFFECT_NONE);
    EXPECT_EQ(SUCCESS, ret);

    AudioEffectMode effectMode = audioRenderer->GetAudioEffectMode();
    EXPECT_EQ(EFFECT_NONE, effectMode);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetAudioEffectMode with, EFFECT_DEFAULT
 * @tc.number: Audio_Renderer_GetAudioEffectMode_002
 * @tc.desc  : Test GetAudioEffectMode interface. Returns the current effect mode.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioEffectMode_002, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetAudioEffectMode(EFFECT_DEFAULT);
    EXPECT_EQ(SUCCESS, ret);

    AudioEffectMode effectMode = audioRenderer->GetAudioEffectMode();
    EXPECT_EQ(EFFECT_DEFAULT, effectMode);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetAudioEffectMode with, default effectMode
 * @tc.number: Audio_Renderer_GetAudioEffectMode_003
 * @tc.desc  : Test GetAudioEffectMode interface. Returns the default effect mode EFFECT_DEFAULT.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioEffectMode_003, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    AudioEffectMode effectMode = audioRenderer->GetAudioEffectMode();
    EXPECT_EQ(EFFECT_DEFAULT, effectMode);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetMinStreamVolume
 * @tc.number: Audio_Renderer_GetMinStreamVolume_001
 * @tc.desc  : Test GetMinStreamVolume interface to get the min volume value.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetMinStreamVolume_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    float volume = audioRenderer->GetMinStreamVolume();
    EXPECT_EQ(0.0, volume);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test GetMinStreamVolume
 * @tc.number: Audio_Renderer_GetMinStreamVolume_Stability_001
 * @tc.desc  : Test GetMinStreamVolume interface to get the min volume value for 1000 times.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetMinStreamVolume_Stability_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    for (int i = 0; i < VALUE_THOUSAND; i++) {
        float volume = audioRenderer->GetMinStreamVolume();
        EXPECT_EQ(0.0, volume);
    }

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test GetMaxStreamVolume
 * @tc.number: Audio_Renderer_GetMaxStreamVolume_001
 * @tc.desc  : Test GetMaxStreamVolume interface to get the max volume value.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetMaxStreamVolume_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    float volume = audioRenderer->GetMaxStreamVolume();
    EXPECT_EQ(1.0, volume);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test GetMaxStreamVolume
 * @tc.number: Audio_Renderer_GetMaxStreamVolume_Stability_001
 * @tc.desc  : Test GetMaxStreamVolume interface to get the max volume value for 1000 times.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetMaxStreamVolume_Stability_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    for (int i = 0; i < VALUE_THOUSAND; i++) {
        float volume = audioRenderer->GetMaxStreamVolume();
        EXPECT_EQ(1.0, volume);
    }

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test GetUnderflowCount
 * @tc.number: Audio_Renderer_GetUnderflowCount_001
 * @tc.desc  : Test GetUnderflowCount interface get underflow value.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetUnderflowCount_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->GetUnderflowCount();
    EXPECT_GE(ret, SUCCESS);

    audioRenderer->Release();
}


/**
 * @tc.name  : Test GetUnderflowCount
 * @tc.number: Audio_Renderer_GetUnderflowCount_002
 * @tc.desc  : Test GetUnderflowCount interface get underflow value.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetUnderflowCount_002, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    // Use the STREAM_USAGE_VOICE_COMMUNICATION to prevent entering offload mode, as offload does not support underflow.
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_UNKNOWN;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VOICE_COMMUNICATION;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    size_t bufferSize;
    int32_t ret = audioRenderer->GetBufferSize(bufferSize);
    EXPECT_EQ(ret, SUCCESS);

    auto buffer = std::make_unique<uint8_t[]>(bufferSize);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    ret = audioRenderer->Write(buffer.get(), bufferSize);

    std::this_thread::sleep_for(1s);
    auto underFlowCount = audioRenderer->GetUnderflowCount();

    // Ensure the underflowCount is at least 1
    EXPECT_GE(underFlowCount, 1);

    audioRenderer->Stop();
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetUnderflowCount
 * @tc.number: Audio_Renderer_GetUnderflowCount_004
 * @tc.desc  : Test GetUnderflowCount interface get underflow value.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetUnderflowCount_004, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    // Use the STREAM_USAGE_VOICE_COMMUNICATION to prevent entering offload mode, as offload does not support underflow.
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_UNKNOWN;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VOICE_COMMUNICATION;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
    EXPECT_EQ(SUCCESS, ret);

    shared_ptr<AudioRendererWriteCallbackMock> cb = make_shared<AudioRendererWriteCallbackMock>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(SUCCESS, ret);

    int32_t count = 0;
    cb->Install([&count, &audioRenderer](size_t length) {
                // only execute once
                if (count++ > 0) {
                    return;
                }
                BufferDesc bufDesc {};
                bufDesc.buffer = nullptr;
                bufDesc.dataLength = g_reqBufLen;
                auto ret = audioRenderer->GetBufferDesc(bufDesc);
                EXPECT_EQ(SUCCESS, ret);
                EXPECT_NE(nullptr, bufDesc.buffer);
                audioRenderer->Enqueue(bufDesc);
                });

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    std::this_thread::sleep_for(1s);

    // Verify that the callback is invoked at least once
    EXPECT_GE(cb->GetExeCount(), 1);

    auto underFlowCount = audioRenderer->GetUnderflowCount();

    // Ensure the underflowCount is at least 1
    EXPECT_GE(underFlowCount, 1);

    audioRenderer->Stop();
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetUnderflowCount
 * @tc.number: Audio_Renderer_GetUnderflowCount_Stability_001
 * @tc.desc  : Test GetUnderflowCount interface get underflow value for 1000 times.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetUnderflowCount_Stability_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    for (int i = 0; i < VALUE_THOUSAND; i++) {
        ret = audioRenderer->GetUnderflowCount();
        EXPECT_GE(ret, SUCCESS);
    }

    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetRendererSamplingRate
 * @tc.number: Audio_Renderer_SetRendererSamplingRate_001
 * @tc.desc  : Test SetRendererSamplingRate interface for valid samplingRate.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererSamplingRate_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    uint32_t samplingRate = 44100;
    ret = audioRenderer->SetRendererSamplingRate(samplingRate);
    EXPECT_EQ(ERROR, ret);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetRendererSamplingRate
 * @tc.number: Audio_Renderer_SetRendererSamplingRate_002
 * @tc.desc  : Test SetRendererSamplingRate interface for invalid samplingRate.
 */


HWTEST(AudioRendererUnitTest, Audio_Renderer_SetRendererSamplingRate_002, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    uint32_t invalidRate_1 = 0;
    ret = audioRenderer->SetRendererSamplingRate(invalidRate_1);
    EXPECT_EQ(ERROR, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetRendererSamplingRate
 * @tc.number: Audio_Renderer_GetRendererSamplingRate_001
 * @tc.desc  : Test GetRendererSamplingRate get default samplingRate.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetRendererSamplingRate_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    uint32_t ret = audioRenderer->GetRendererSamplingRate();
    EXPECT_EQ(SAMPLE_RATE_44100, ret);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetRendererSamplingRate
 * @tc.number: Audio_Renderer_GetRendererSamplingRate_002
 * @tc.desc  : Test GetRendererSamplingRate get valid samplingRate after set valid samplingRate.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetRendererSamplingRate_002, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    uint32_t samplingRate = 48000;
    ret = audioRenderer->SetRendererSamplingRate(samplingRate);
    EXPECT_EQ(ERROR, ret);

    uint32_t retSamplerate = audioRenderer->GetRendererSamplingRate();
    EXPECT_EQ(SAMPLE_RATE_44100, retSamplerate);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetRendererSamplingRate
 * @tc.number: Audio_Renderer_GetRendererSamplingRate_003
 * @tc.desc  : Test GetRendererSamplingRate get default samplingRate after set invalid samplingRate.
 */

HWTEST(AudioRendererUnitTest, Audio_Renderer_GetRendererSamplingRate_003, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    uint32_t samplingRate = 0;
    ret = audioRenderer->SetRendererSamplingRate(samplingRate);
    EXPECT_EQ(ERROR, ret);

    uint32_t retSamplerate = audioRenderer->GetRendererSamplingRate();
    EXPECT_EQ(SAMPLE_RATE_44100, retSamplerate);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetRendererSamplingRate
 * @tc.number: Audio_Renderer_GetRendererSamplingRate_004
 * @tc.desc  : Test GetRendererSamplingRate get valid samplingRate after set invalid samplingRate.
 */

HWTEST(AudioRendererUnitTest, Audio_Renderer_GetRendererSamplingRate_004, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    uint32_t validRate = 48000;
    ret = audioRenderer->SetRendererSamplingRate(validRate);
    EXPECT_EQ(ERROR, ret);

    uint32_t invalidRate = 0;
    ret = audioRenderer->SetRendererSamplingRate(invalidRate);
    EXPECT_EQ(ERROR, ret);

    uint32_t retSampleRate = audioRenderer->GetRendererSamplingRate();
    EXPECT_EQ(SAMPLE_RATE_44100, retSampleRate);
    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetRendererSamplingRate
 * @tc.number: Audio_Renderer_GetRendererSamplingRate_Stability_001
 * @tc.desc  : Test GetRendererSamplingRate get valid samplingRate 1000 times after set valid samplingRate.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetRendererSamplingRate_Stability_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    for (int i = 0; i < VALUE_THOUSAND; i++) {
        uint32_t samplingRate = 48000;
        ret = audioRenderer->SetRendererSamplingRate(samplingRate);
        EXPECT_EQ(ERROR, ret);

        uint32_t retSampleRate = audioRenderer->GetRendererSamplingRate();
        EXPECT_EQ(SAMPLE_RATE_44100, retSampleRate);
    }

    audioRenderer->Release();
}

/**
* @tc.name  : Test RegisterAudioPolicyServerDiedCb via legal state
* @tc.number: Audio_Renderer_RegisterAudioPolicyServerDiedCb_001
* @tc.desc  : Test registerAudioRendererEventListener interface. Returns success.
*/
HWTEST(AudioRendererUnitTest, Audio_Renderer_RegisterAudioPolicyServerDiedCb_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    int32_t clientId = getpid();
    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    shared_ptr<AudioRendererPolicyServiceDiedCallbackTest> callback =
        make_shared<AudioRendererPolicyServiceDiedCallbackTest>();
    int32_t ret = audioRenderer->RegisterAudioPolicyServerDiedCb(clientId, callback);
    EXPECT_EQ(SUCCESS, ret);

    ret = audioRenderer->UnregisterAudioPolicyServerDiedCb(clientId);
    EXPECT_EQ(SUCCESS, ret);
}

/**
* @tc.name  : Test RegisterAudioPolicyServerDiedCb via legal state
* @tc.number: Audio_Renderer_RegisterAudioPolicyServerDiedCb_002
* @tc.desc  : Test registerAudioRendererEventListener interface. Returns ERR_INVALID_PARAM.
*/
HWTEST(AudioRendererUnitTest, Audio_Renderer_RegisterAudioPolicyServerDiedCb_002, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    int32_t clientId = getpid();
    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    int32_t ret = audioRenderer->RegisterAudioPolicyServerDiedCb(clientId, nullptr);
    EXPECT_EQ(VALUE_ERROR, ret);
}

/**
* @tc.name  : Test RegisterAudioPolicyServerDiedCb via legal state
* @tc.number: Audio_Renderer_RegisterAudioPolicyServerDiedCb_Stability_001
* @tc.desc  : Test registerAudioRendererEventListener interface valid callback 1000 times.
*/
HWTEST(AudioRendererUnitTest, Audio_Renderer_RegisterAudioPolicyServerDiedCb_Stability_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    int32_t clientId = getpid();
    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);
    for (int i = 0; i < VALUE_THOUSAND; i++) {
        shared_ptr<AudioRendererPolicyServiceDiedCallbackTest> callback =
            make_shared<AudioRendererPolicyServiceDiedCallbackTest>();
        int32_t ret = audioRenderer->RegisterAudioPolicyServerDiedCb(clientId, callback);
        EXPECT_EQ(SUCCESS, ret);

        ret = audioRenderer->UnregisterAudioPolicyServerDiedCb(clientId);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
* @tc.name  : Test RegisterAudioPolicyServerDiedCb via legal state
* @tc.number: Audio_Renderer_RegisterAudioPolicyServerDiedCb_002
* @tc.desc  : Test registerAudioRendererEventListener interface invalid callback 1000 times.
*/
HWTEST(AudioRendererUnitTest, Audio_Renderer_RegisterAudioPolicyServerDiedCb_Stability_002, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    int32_t clientId = getpid();
    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);
    for (int i = 0; i < VALUE_THOUSAND; i++) {
        int32_t ret = audioRenderer->RegisterAudioPolicyServerDiedCb(clientId, nullptr);
        EXPECT_EQ(VALUE_ERROR, ret);
    }
}

/**
* @tc.name  : Test UnregisterAudioPolicyServerDiedCb via legal state
* @tc.number: Audio_Renderer_UnregisterAudioPolicyServerDiedCb_001
* @tc.desc  : Test UnregisterAudioPolicyServerDiedCb interface. Returns success.
*/
HWTEST(AudioRendererUnitTest, Audio_Renderer_UnregisterAudioPolicyServerDiedCb_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    int32_t clientId = getpid();
    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    shared_ptr<AudioRendererPolicyServiceDiedCallbackTest> callback =
        make_shared<AudioRendererPolicyServiceDiedCallbackTest>();
    int32_t ret = audioRenderer->RegisterAudioPolicyServerDiedCb(clientId, callback);
    EXPECT_EQ(SUCCESS, ret);

    ret = audioRenderer->UnregisterAudioPolicyServerDiedCb(clientId);
    EXPECT_EQ(SUCCESS, ret);
}

/**
* @tc.name  : Test UnregisterAudioPolicyServerDiedCb via legal state
* @tc.number: Audio_Renderer_UnregisterAudioPolicyServerDiedCb_Stability_001
* @tc.desc  : Test UnregisterAudioPolicyServerDiedCb interface valid callback 1000 times.
*/
HWTEST(AudioRendererUnitTest, Audio_Renderer_UnregisterAudioPolicyServerDiedCb_Stability_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    int32_t clientId = getpid();
    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);
    for (int i = 0; i < VALUE_THOUSAND; i++) {
        shared_ptr<AudioRendererPolicyServiceDiedCallbackTest> callback =
            make_shared<AudioRendererPolicyServiceDiedCallbackTest>();
        int32_t ret = audioRenderer->RegisterAudioPolicyServerDiedCb(clientId, callback);
        EXPECT_EQ(SUCCESS, ret);

        ret = audioRenderer->UnregisterAudioPolicyServerDiedCb(clientId);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.name  : Test GetCurrentOutputDevices API after calling create
 * @tc.number: Audio_Renderer_GetCurrentOutputDevices_001
 * @tc.desc  : Test GetCurrentOutputDevices interface. Check whether renderer info returns proper data
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetCurrentOutputDevices_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    EXPECT_NE(nullptr, audioRenderer);

    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    ret = audioRenderer->GetCurrentOutputDevices(deviceInfo);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetCurrentOutputDevices API after calling create
 * @tc.number: Audio_Renderer_GetCurrentOutputDevices_002
 * @tc.desc  : Test GetCurrentOutputDevices interface.Check the deviceinfo is proper data when using speaker.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetCurrentOutputDevices_002, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    EXPECT_NE(nullptr, audioRenderer);

    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    audioRenderer->GetCurrentOutputDevices(deviceInfo);

    EXPECT_EQ(OUTPUT_DEVICE, deviceInfo.deviceRole_);
    EXPECT_EQ(DEVICE_TYPE_SPEAKER, deviceInfo.deviceType_);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetCurrentOutputDevices API after calling create
 * @tc.number: Audio_Renderer_GetCurrentOutputDevices_001
 * @tc.desc  : Test GetCurrentOutputDevices interface check if it is success for 1000 times
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetCurrentOutputDevices_Stability_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    EXPECT_NE(nullptr, audioRenderer);

    for (int i = 0; i < VALUE_THOUSAND; i++) {
        AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
        audioRenderer->GetCurrentOutputDevices(deviceInfo);

        EXPECT_EQ(OUTPUT_DEVICE, deviceInfo.deviceRole_);
        EXPECT_EQ(DEVICE_TYPE_SPEAKER, deviceInfo.deviceType_);
    }

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetCurrentOutputDevices API after calling create
 * @tc.number: Audio_Renderer_GetCurrentOutputDevices_001
 * @tc.desc  : Test GetCurrentOutputDevices interface check proper data when using speaker for 1000 times
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetCurrentOutputDevices_Stability_002, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    EXPECT_NE(nullptr, audioRenderer);

    for (int i = 0; i < VALUE_THOUSAND; i++) {
        AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
        audioRenderer->GetCurrentOutputDevices(deviceInfo);

        EXPECT_EQ(OUTPUT_DEVICE, deviceInfo.deviceRole_);
        EXPECT_EQ(DEVICE_TYPE_SPEAKER, deviceInfo.deviceType_);
    }

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetFramesWritten API after calling create
 * @tc.number: Audio_Renderer_GetFramesWritten_001
 * @tc.desc  : Test GetFramesWritten interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetFramesWritten_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    EXPECT_NE(nullptr, audioRenderer);

    ret = audioRenderer->GetFramesWritten();
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test SetSpeed
 * @tc.number: Audio_Renderer_SetSpeed_001
 * @tc.desc  : Test SetSpeed interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetSpeed_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetSpeed(0.5);
    EXPECT_EQ(SUCCESS, ret);

    ret = audioRenderer->SetSpeed(0.25); // 0.25 min speed
    EXPECT_EQ(SUCCESS, ret);

    ret = audioRenderer->SetSpeed(4); // 4 max speed
    EXPECT_EQ(SUCCESS, ret);

    ret = audioRenderer->SetSpeed(0.124); // 0.124 lower
    EXPECT_EQ(ERR_INVALID_PARAM, ret);

    ret = audioRenderer->SetSpeed(4.01); // 4.01 upper
    EXPECT_EQ(ERR_INVALID_PARAM, ret);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test SetPitch
 * @tc.number: Audio_Renderer_SetPitch_001
 * @tc.desc  : Test SetPitch interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetPitch_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetPitch(0.5);
    EXPECT_EQ(SUCCESS, ret);

    ret = audioRenderer->SetPitch(0.25); // 0.25 min speed
    EXPECT_EQ(SUCCESS, ret);

    ret = audioRenderer->SetPitch(4); // 4 max speed
    EXPECT_EQ(SUCCESS, ret);

    ret = audioRenderer->SetPitch(0.124); // 0.124 lower
    EXPECT_EQ(ERR_INVALID_PARAM, ret);

    ret = audioRenderer->SetPitch(4.01); // 4.01 upper
    EXPECT_EQ(ERR_INVALID_PARAM, ret);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test GetSpeed
 * @tc.number: Audio_Renderer_GetSpeed_001
 * @tc.desc  : Test GetSpeed interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetSpeed_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    float speed = audioRenderer->GetSpeed();
    EXPECT_EQ(1.0, speed);

    ret = audioRenderer->SetSpeed(4.0);
    EXPECT_EQ(SUCCESS, ret);

    speed = audioRenderer->GetSpeed();
    EXPECT_EQ(4.0, speed);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test SetSpeed and Write API.
 * @tc.number: Audio_Renderer_SetSpeed_Write_001
 * @tc.desc  : Test SetSpeed and Write interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetSpeed_Write_001, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;
    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetSpeed(1.0); // 1.0 speed
    EXPECT_EQ(SUCCESS, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);

    size_t bytesToWrite = 0;
    int32_t bytesWritten = 0;
    size_t minBytes = 4; // 4 min bytes
    int32_t numBuffersToRender = WRITE_BUFFERS_COUNT;

    while (numBuffersToRender) {
        if (numBuffersToRender == WRITE_BUFFERS_COUNT / 2) { // 2 half count
            ret = audioRenderer->SetSpeed(2.0); // 2.0 speed
            EXPECT_EQ(SUCCESS, ret);
        }
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
 * @tc.name  : Test SetSpeed and Write with meta API.
 * @tc.number: Audio_Renderer_SetSpeed_Write_002
 * @tc.desc  : Test SetSpeed and Write with meta interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetSpeed_Write_002, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_PCMFILE_PATH.c_str(), "rb");
    FILE *metaFile = fopen(AUDIORENDER_TEST_METAFILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);
    ASSERT_NE(nullptr, metaFile);

    AudioRendererOptions rendererOptions;
    AudioRendererUnitTest::InitializeRendererSpatialOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetSpeed(1.0); // 1.0 speed
    EXPECT_EQ(SUCCESS, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    uint8_t *buffer = nullptr;
    uint8_t *metaBuffer = nullptr;
    AudioRendererUnitTest::GetBuffersAndLen(audioRenderer, buffer, metaBuffer, bufferLen);

    size_t bytesToWrite = 0;
    int32_t bytesWritten = 0;
    size_t minBytes = 4; // 4 min bytes
    int32_t numBuffersToRender = WRITE_BUFFERS_COUNT;

    while (numBuffersToRender) {
        if (numBuffersToRender == WRITE_BUFFERS_COUNT / 2) { // 2 half count
            ret = audioRenderer->SetSpeed(2.0);              // 2.0 speed
            EXPECT_EQ(SUCCESS, ret);
        }
        bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
        fread(metaBuffer, 1, AVS3METADATA_SIZE, metaFile);
        bytesWritten = 0;
        while ((static_cast<size_t>(bytesWritten) < bytesToWrite) &&
            ((static_cast<size_t>(bytesToWrite) - bytesWritten) > minBytes)) {
            bytesWritten += audioRenderer->Write(buffer + static_cast<size_t>(bytesWritten),
                bytesToWrite - static_cast<size_t>(bytesWritten), metaBuffer, AVS3METADATA_SIZE);
            EXPECT_GE(bytesWritten, VALUE_ZERO);
        }
        numBuffersToRender--;
    }

    audioRenderer->Drain();
    audioRenderer->Stop();
    audioRenderer->Release();
    AudioRendererUnitTest::ReleaseBufferAndFiles(buffer, metaBuffer, wavFile, metaFile);
}

/**
 * @tc.name  : Test SetOffloadAllowed API.
 * @tc.number: Audio_Renderer_SetOffloadAllowed_001
 * @tc.desc  : Test SetOffloadAllowed interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetOffloadAllowed_001, TestSize.Level1)
{
    int32_t ret = -1;
    FILE *wavFile = fopen(AUDIORENDER_TEST_FILE_PATH.c_str(), "rb");
    ASSERT_NE(nullptr, wavFile);

    AudioRendererOptions rendererOptions;
    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetOffloadAllowed(false);
    EXPECT_EQ(SUCCESS, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    size_t bufferLen;
    ret = audioRenderer->GetBufferSize(bufferLen);
    EXPECT_EQ(SUCCESS, ret);

    uint8_t *buffer = (uint8_t *) malloc(bufferLen);
    ASSERT_NE(nullptr, buffer);

    size_t bytesToWrite = 0;
    int32_t bytesWritten = 0;
    size_t minBytes = 4; // 4 min bytes
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
 * @tc.name  : Test voip can not interrupt voiceCall
 * @tc.number: SetVoipInterruptVoiceCall_001
 * @tc.desc  : When voip comes after voiceCall, voip will be deny by voiceCall
 */
HWTEST(AudioRendererUnitTest, SetVoipInterruptVoiceCall_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptionsForVoip;
    rendererOptionsForVoip.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_44100;
    rendererOptionsForVoip.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptionsForVoip.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptionsForVoip.streamInfo.channels = AudioChannel::STEREO;
    rendererOptionsForVoip.rendererInfo.contentType = ContentType::CONTENT_TYPE_UNKNOWN;
    rendererOptionsForVoip.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VOICE_COMMUNICATION;
    rendererOptionsForVoip.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRendererForVoip = AudioRenderer::Create(rendererOptionsForVoip);
    if (audioRendererForVoip == nullptr) {
        return ;
    }
    shared_ptr<AudioRendererCallbackTest> audioRendererCB = make_shared<AudioRendererCallbackTest>();
    int32_t ret = audioRendererForVoip->SetRendererCallback(audioRendererCB);
    EXPECT_EQ(SUCCESS, ret);

    audioRendererForVoip->SetInterruptMode(INDEPENDENT_MODE);
    bool isStartedforVoip = audioRendererForVoip->Start();
    EXPECT_EQ(true, isStartedforVoip);

    AudioRendererOptions rendererOptionsForVoice;
    rendererOptionsForVoice.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_44100;
    rendererOptionsForVoice.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptionsForVoice.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptionsForVoice.streamInfo.channels = AudioChannel::STEREO;
    rendererOptionsForVoice.rendererInfo.contentType = ContentType::CONTENT_TYPE_UNKNOWN;
    rendererOptionsForVoice.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VOICE_MODEM_COMMUNICATION;
    rendererOptionsForVoice.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRendererForVoiceCall = AudioRenderer::Create(rendererOptionsForVoice);
    if (audioRendererForVoiceCall == nullptr) {
        return ;
    }
    audioRendererForVoiceCall->SetInterruptMode(INDEPENDENT_MODE);
    bool isStartedforVoiceCall = audioRendererForVoiceCall->Start();
    EXPECT_EQ(true, isStartedforVoiceCall);

    std::this_thread::sleep_for(std::chrono::seconds(3));
    EXPECT_EQ(AudioRendererUnitTest::interruptEventTest_.hintType, INTERRUPT_HINT_PAUSE);

    audioRendererForVoiceCall->Stop();
    audioRendererForVoiceCall->Release();
    audioRendererForVoip->Stop();
    audioRendererForVoip->Release();
}

/**
 * @tc.name  : Test voiceCall can interrupt voip
 * @tc.number: SetVoiceCallInterruptVoip_001
 * @tc.desc  : When voiceCall comes after voip, voip will be stopped by voiceCall
 */
HWTEST(AudioRendererUnitTest, SetVoiceCallInterruptVoip_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptionsForVoice;
    rendererOptionsForVoice.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_44100;
    rendererOptionsForVoice.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptionsForVoice.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptionsForVoice.streamInfo.channels = AudioChannel::STEREO;
    rendererOptionsForVoice.rendererInfo.contentType = ContentType::CONTENT_TYPE_UNKNOWN;
    rendererOptionsForVoice.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VOICE_MODEM_COMMUNICATION;
    rendererOptionsForVoice.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRendererForVoiceCall = AudioRenderer::Create(rendererOptionsForVoice);
    if (audioRendererForVoiceCall == nullptr) {
        return ;
    }
    audioRendererForVoiceCall->SetInterruptMode(INDEPENDENT_MODE);
    bool isStartedforVoiceCall = audioRendererForVoiceCall->Start();
    EXPECT_EQ(true, isStartedforVoiceCall);

    AudioRendererOptions rendererOptionsForVoip;
    rendererOptionsForVoip.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_44100;
    rendererOptionsForVoip.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptionsForVoip.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptionsForVoip.streamInfo.channels = AudioChannel::STEREO;
    rendererOptionsForVoip.rendererInfo.contentType = ContentType::CONTENT_TYPE_UNKNOWN;
    rendererOptionsForVoip.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VOICE_COMMUNICATION;
    rendererOptionsForVoip.rendererInfo.rendererFlags = RENDERER_FLAG;

    unique_ptr<AudioRenderer> audioRendererForVoip = AudioRenderer::Create(rendererOptionsForVoip);
    if (audioRendererForVoip == nullptr) {
        return ;
    }
    audioRendererForVoip->SetInterruptMode(INDEPENDENT_MODE);
    bool isStartedforVoip = audioRendererForVoip->Start();
    EXPECT_EQ(false, isStartedforVoip);

    audioRendererForVoip->Stop();
    audioRendererForVoip->Release();

    audioRendererForVoip->Stop();
    audioRendererForVoip->Release();
}

/*
 * @tc.name  : Test GetAudioPosition API via legal input.
 * @tc.number: Audio_Renderer_GetAudioPosition_001
 * @tc.desc  : Test GetAudioPosition interface. Returns true, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioPosition_001, TestSize.Level1)
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
    bool getAudioPositionRet = audioRenderer->GetAudioPosition(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(true, getAudioPositionRet);
    EXPECT_GE(timestamp.time.tv_sec, (const long)VALUE_ZERO);
    EXPECT_GE(timestamp.time.tv_nsec, (const long)VALUE_ZERO);

    audioRenderer->Drain();
    audioRenderer->Stop();
    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test GetAudioPosition API via illegal state, RENDERER_NEW: GetAudioPosition without initializing
 * the renderer.
 * @tc.number: Audio_Renderer_GetAudioPosition_002
 * @tc.desc  : Test GetAudioPosition interface. Returns false, if the renderer state is RENDERER_NEW
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioPosition_002, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    Timestamp timestamp;
    bool ret = audioRenderer->GetAudioPosition(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(false, ret);
}

/**
 * @tc.name  : Test GetAudioPosition API via legal state, RENDERER_RUNNING.
 * @tc.number: Audio_Renderer_GetAudioPosition_003
 * @tc.desc  : test GetAudioPosition interface. Returns true, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioPosition_003, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    Timestamp timestamp;
    bool ret = audioRenderer->GetAudioPosition(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(true, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetAudioPosition API via illegal state, RENDERER_STOPPED: GetAudioPosition after Stop.
 * @tc.number: Audio_Renderer_GetAudioPosition_004
 * @tc.desc  : Test GetAudioPosition interface. Returns false, if the renderer state is RENDERER_STOPPED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioPosition_004, TestSize.Level1)
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
    bool ret = audioRenderer->GetAudioPosition(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(false, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetAudioPosition API via illegal state, RENDERER_RELEASED: GetAudioPosition after Release.
 * @tc.number: Audio_Renderer_GetAudioPosition_005
 * @tc.desc  : Test GetAudioPosition interface. Returns false, if the renderer state is RENDERER_RELEASED
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioPosition_005, TestSize.Level1)
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
    bool ret = audioRenderer->GetAudioPosition(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(false, ret);
}

/**
 * @tc.name  : Test GetAudioPosition API via illegal state, RENDERER_PAUSED: GetAudioPosition after Stop.
 * @tc.number: Audio_Renderer_GetAudioPosition_006
 * @tc.desc  : Test GetAudioPosition interface. Returns false, if the renderer state is RENDERER_PAUSED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioPosition_006, TestSize.Level1)
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
    bool ret = audioRenderer->GetAudioPosition(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(false, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetAudioPosition API via legal state, RENDERER_PAUSED.
 * @tc.number: Audio_Renderer_GetAudioPosition_007
 * @tc.desc  : Test GetAudioPosition interface. Timestamp should be larger after pause 1s.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioPosition_007, TestSize.Level2)
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
    audioRenderer->GetAudioPosition(timestamp1, Timestamp::Timestampbase::MONOTONIC);

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
    audioRenderer->GetAudioPosition(timestamp2, Timestamp::Timestampbase::MONOTONIC);

    int64_t duration = (timestamp2.time.tv_sec - timestamp1.time.tv_sec) * 1000000 + (timestamp2.time.tv_nsec -
        timestamp1.time.tv_nsec) / VALUE_THOUSAND; // ns -> us
    EXPECT_GE(duration, sleepTime);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetSilentModeAndMixWithOthers
 * @tc.number: Audio_Renderer_GetSilentModeAndMixWithOthers_001
 * @tc.desc  : Test GetSpeed interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetSilentModeAndMixWithOthers_001, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool on = audioRenderer->GetSilentModeAndMixWithOthers();
    EXPECT_EQ(false, on);

    audioRenderer->SetSilentModeAndMixWithOthers(true);

    on = audioRenderer->GetSilentModeAndMixWithOthers();
    EXPECT_EQ(true, on);

    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test SetLowPowerVolumeImpl
 * @tc.number: Audio_Renderer_Set_Low_Power_Volume_001
 * @tc.desc  : Test SetLowPowerVolume interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Set_Low_Power_Volume_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    unique_ptr<AudioRendererProxyObj> audioRendererProxyObj = std::make_unique<AudioRendererProxyObj>();
    audioRendererProxyObj->SaveRendererObj(audioRendererPrivate);
    audioRendererProxyObj->SetOffloadModeImpl(0, true);
    audioRendererProxyObj->UnsetOffloadModeImpl();
    float ret = -1.0f;
    audioRendererProxyObj->SetLowPowerVolumeImpl(1.0f);
    audioRendererProxyObj->GetLowPowerVolumeImpl(ret);
    EXPECT_EQ(1.0f, ret);
}

/**
 * @tc.name  : Test SetLowPowerVolumeImpl
 * @tc.number: Audio_Renderer_Set_Low_Power_Volume_001
 * @tc.desc  : Test SetLowPowerVolume interface. if the renderer is nullptr.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Set_Low_Power_Volume_002, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    unique_ptr<AudioRendererProxyObj> audioRendererProxyObj = std::make_unique<AudioRendererProxyObj>();
    audioRendererProxyObj->SaveRendererObj(std::weak_ptr<AudioRendererPrivate>());
    audioRendererProxyObj->SetOffloadModeImpl(0, true);
    audioRendererProxyObj->UnsetOffloadModeImpl();
    float ret = -1.0f;
    audioRendererProxyObj->SetLowPowerVolumeImpl(1.0f);
    audioRendererProxyObj->GetLowPowerVolumeImpl(ret);
    EXPECT_EQ(-1.0f, ret);
}

/**
 * @tc.name  : Test SetSwitchInfo
 * @tc.number: Audio_Renderer_SetSwitchInfo_001
 * @tc.desc  : Test SetSwitchInfo interface. if the renderPositionCb is nullptr, frameMarkPosition is 0.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetSwitchInfo_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    AudioStreamParams audioStreamParams;
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    IAudioStream::SwitchInfo switchInfo;
    switchInfo.renderPositionCb = nullptr;
    switchInfo.renderPeriodPositionCb = nullptr;
    switchInfo.capturePeriodPositionCb = nullptr;
    switchInfo.capturePositionCb = nullptr;
    switchInfo.frameMarkPosition = 0;
    switchInfo.framePeriodNumber = 0;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::FAST_STREAM,
        audioStreamParams, STREAM_DEFAULT, appInfo.appPid);
    audioRendererPrivate->SetSwitchInfo(switchInfo, audioStream);
    ASSERT_NE(nullptr, audioRendererPrivate);
}

/**
 * @tc.name  : Test SetSwitchInfo
 * @tc.number: Audio_Renderer_SetSwitchInfo_002
 * @tc.desc  : Test SetSwitchInfo interface. if the renderPositionCb is nullptr, frameMarkPosition is 1.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetSwitchInfo_002, TestSize.Level1)
{
    AppInfo appInfo = {};
    AudioStreamParams audioStreamParams;
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    IAudioStream::SwitchInfo switchInfo;
    switchInfo.renderPositionCb = nullptr;
    switchInfo.renderPeriodPositionCb = nullptr;
    switchInfo.capturePeriodPositionCb = nullptr;
    switchInfo.capturePositionCb = nullptr;
    switchInfo.frameMarkPosition = 1;
    switchInfo.framePeriodNumber = 1;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::FAST_STREAM,
        audioStreamParams, STREAM_DEFAULT, appInfo.appPid);
    audioRendererPrivate->SetSwitchInfo(switchInfo, audioStream);
    ASSERT_NE(nullptr, audioRendererPrivate);
}

/**
 * @tc.name  : Test SetSwitchInfo
 * @tc.number: Audio_Renderer_SetSwitchInfo_003
 * @tc.desc  : Test SetSwitchInfo interface. if the renderPositionCb is not nullptr, frameMarkPosition is 0.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetSwitchInfo_003, TestSize.Level1)
{
    AppInfo appInfo = {};
    AudioStreamParams audioStreamParams;
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    IAudioStream::SwitchInfo switchInfo;
    shared_ptr<RendererPositionCallbackTest> positionCB = std::make_shared<RendererPositionCallbackTest>();
    shared_ptr<RendererPeriodPositionCallbackTest> periodPositionCB =
        std::make_shared<RendererPeriodPositionCallbackTest>();
    shared_ptr<CapturerPeriodPositionCallbackTest> capturerPeriodPositionCB =
        std::make_shared<CapturerPeriodPositionCallbackTest>();
    shared_ptr<CapturerPositionCallbackTest> capturerPositionCB = std::make_shared<CapturerPositionCallbackTest>();
    switchInfo.renderPositionCb = positionCB;
    switchInfo.renderPeriodPositionCb = periodPositionCB;
    switchInfo.capturePeriodPositionCb = capturerPeriodPositionCB;
    switchInfo.capturePositionCb = capturerPositionCB;
    switchInfo.frameMarkPosition = 0;
    switchInfo.framePeriodNumber = 0;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::FAST_STREAM,
        audioStreamParams, STREAM_DEFAULT, appInfo.appPid);
    audioRendererPrivate->SetSwitchInfo(switchInfo, audioStream);
    ASSERT_NE(nullptr, audioRendererPrivate);
}

/**
 * @tc.name  : Test SetSwitchInfo
 * @tc.number: Audio_Renderer_SetSwitchInfo_004
 * @tc.desc  : Test SetSwitchInfo interface. if the renderPositionCb is not nullptr, frameMarkPosition is 1.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_SetSwitchInfo_004, TestSize.Level1)
{
    AppInfo appInfo = {};
    AudioStreamParams audioStreamParams;
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    IAudioStream::SwitchInfo switchInfo;
    shared_ptr<RendererPositionCallbackTest> positionCB = std::make_shared<RendererPositionCallbackTest>();
    shared_ptr<RendererPeriodPositionCallbackTest> periodPositionCB =
        std::make_shared<RendererPeriodPositionCallbackTest>();
    shared_ptr<CapturerPeriodPositionCallbackTest> capturerPeriodPositionCB =
        std::make_shared<CapturerPeriodPositionCallbackTest>();
    shared_ptr<CapturerPositionCallbackTest> capturerPositionCB = std::make_shared<CapturerPositionCallbackTest>();
    switchInfo.renderPositionCb = positionCB;
    switchInfo.renderPeriodPositionCb = periodPositionCB;
    switchInfo.capturePeriodPositionCb = capturerPeriodPositionCB;
    switchInfo.capturePositionCb = capturerPositionCB;
    switchInfo.frameMarkPosition = 1;
    switchInfo.framePeriodNumber = 1;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::FAST_STREAM,
        audioStreamParams, STREAM_DEFAULT, appInfo.appPid);
    audioRendererPrivate->SetSwitchInfo(switchInfo, audioStream);
    ASSERT_NE(nullptr, audioRendererPrivate);
}

/**
 * @tc.name  : Test HandleAndNotifyForcedEvent
 * @tc.number: Audio_Renderer_HandleAndNotifyForcedEvent_001
 * @tc.desc  : Test HandleAndNotifyForcedEvent interface. if the InterruptHint is INTERRUPT_HINT_PAUSE.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_HandleAndNotifyForcedEvent_001, TestSize.Level1)
{
    AudioStreamParams audioStreamParams;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::FAST_STREAM,
        audioStreamParams, STREAM_DEFAULT, 1);
    AudioInterrupt audioInterrupt;
    auto audioInterruptCallback = std::make_shared<AudioRendererInterruptCallbackImpl>(audioStream, audioInterrupt);
    InterruptEventInternal interruptEvent {INTERRUPT_TYPE_BEGIN, INTERRUPT_FORCE, INTERRUPT_HINT_PAUSE, 20.0f};
    audioInterruptCallback->HandleAndNotifyForcedEvent(interruptEvent);
    EXPECT_FALSE(audioInterruptCallback->isForcePaused_);
}

/**
 * @tc.name  : Test HandleAndNotifyForcedEvent
 * @tc.number: Audio_Renderer_HandleAndNotifyForcedEvent_002
 * @tc.desc  : Test HandleAndNotifyForcedEvent interface. if the InterruptHint is INTERRUPT_HINT_RESUME.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_HandleAndNotifyForcedEvent_002, TestSize.Level1)
{
    AudioStreamParams audioStreamParams;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::PA_STREAM,
        audioStreamParams, STREAM_DEFAULT, 1);
    AudioInterrupt audioInterrupt;
    auto audioInterruptCallback = std::make_shared<AudioRendererInterruptCallbackImpl>(audioStream, audioInterrupt);
    InterruptEventInternal interruptEvent {INTERRUPT_TYPE_BEGIN, INTERRUPT_FORCE, INTERRUPT_HINT_RESUME, 20.0f};
    audioInterruptCallback->HandleAndNotifyForcedEvent(interruptEvent);
    EXPECT_FALSE(audioInterruptCallback->isForcePaused_);
}

/**
 * @tc.name  : Test HandleAndNotifyForcedEvent
 * @tc.number: Audio_Renderer_HandleAndNotifyForcedEvent_003
 * @tc.desc  : Test HandleAndNotifyForcedEvent interface. if the InterruptHint is INTERRUPT_HINT_DUCK.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_HandleAndNotifyForcedEvent_003, TestSize.Level1)
{
    AudioStreamParams audioStreamParams;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::PA_STREAM,
        audioStreamParams, STREAM_DEFAULT, 1);
    AudioInterrupt audioInterrupt;
    auto audioInterruptCallback = std::make_shared<AudioRendererInterruptCallbackImpl>(audioStream, audioInterrupt);
    InterruptEventInternal interruptEvent {INTERRUPT_TYPE_BEGIN, INTERRUPT_FORCE, INTERRUPT_HINT_DUCK, 20.0f};
    audioInterruptCallback->HandleAndNotifyForcedEvent(interruptEvent);
    EXPECT_FALSE(audioInterruptCallback->isForcePaused_);
}

/**
 * @tc.name  : Test HandleAndNotifyForcedEvent
 * @tc.number: Audio_Renderer_HandleAndNotifyForcedEvent_004
 * @tc.desc  : Test HandleAndNotifyForcedEvent interface. if the InterruptHint is INTERRUPT_HINT_UNDUCK.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_HandleAndNotifyForcedEvent_004, TestSize.Level1)
{
    AudioStreamParams audioStreamParams;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::PA_STREAM,
        audioStreamParams, STREAM_DEFAULT, 1);
    AudioInterrupt audioInterrupt;
    auto audioInterruptCallback = std::make_shared<AudioRendererInterruptCallbackImpl>(audioStream, audioInterrupt);
    InterruptEventInternal interruptEvent {INTERRUPT_TYPE_BEGIN, INTERRUPT_FORCE, INTERRUPT_HINT_UNDUCK, 20.0f};
    audioInterruptCallback->HandleAndNotifyForcedEvent(interruptEvent);
    EXPECT_FALSE(audioInterruptCallback->isForcePaused_);
}

/**
 * @tc.name  : Test HandleAndNotifyForcedEvent
 * @tc.number: Audio_Renderer_HandleAndNotifyForcedEvent_005
 * @tc.desc  : Test HandleAndNotifyForcedEvent interface. if the InterruptHint is INTERRUPT_HINT_NONE.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_HandleAndNotifyForcedEvent_005, TestSize.Level1)
{
    AudioStreamParams audioStreamParams;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::PA_STREAM,
        audioStreamParams, STREAM_DEFAULT, 1);
    AudioInterrupt audioInterrupt;
    auto audioInterruptCallback = std::make_shared<AudioRendererInterruptCallbackImpl>(audioStream, audioInterrupt);
    InterruptEventInternal interruptEvent {INTERRUPT_TYPE_BEGIN, INTERRUPT_FORCE, INTERRUPT_HINT_NONE, 20.0f};
    audioInterruptCallback->HandleAndNotifyForcedEvent(interruptEvent);
    EXPECT_FALSE(audioInterruptCallback->isForcePaused_);
}

/**
 * @tc.name  : Test IsDirectVoipParams
 * @tc.number: Audio_Renderer_IsDirectVoipParams_001
 * @tc.desc  : Test Create interface with IsDirectVoipParams below.
 *              audioStreamParams.samplingRate = SAMPLE_RATE_8000;
 *              audioStreamParams.channels = MONO;
 *              audioStreamParams.format = SAMPLE_S16LE;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_IsDirectVoipParams_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    AudioStreamParams audioStreamParams;
    audioStreamParams.samplingRate = SAMPLE_RATE_16000;
    audioStreamParams.channels = MONO;
    audioStreamParams.format = SAMPLE_S16LE;
    bool ret = audioRendererPrivate->IsDirectVoipParams(audioStreamParams);
    EXPECT_TRUE(ret);
}

/**
 * @tc.name  : Test IsDirectVoipParams
 * @tc.number: Audio_Renderer_IsDirectVoipParams_002
 * @tc.desc  : Test Create interface with IsDirectVoipParams below.
 *              audioStreamParams.samplingRate = SAMPLE_RATE_16000;
 *              audioStreamParams.channels = STEREO;
 *              audioStreamParams.format = SAMPLE_S32LE;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_IsDirectVoipParams_002, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    AudioStreamParams audioStreamParams;
    audioStreamParams.samplingRate = SAMPLE_RATE_16000;
    audioStreamParams.channels = STEREO;
    audioStreamParams.format = SAMPLE_S32LE;
    bool ret = audioRendererPrivate->IsDirectVoipParams(audioStreamParams);
    EXPECT_TRUE(ret);
}

/**
 * @tc.name  : Test IsDirectVoipParams
 * @tc.number: Audio_Renderer_IsDirectVoipParams_003
 * @tc.desc  : Test Create interface with IsDirectVoipParams below.
 *              audioStreamParams.samplingRate = SAMPLE_RATE_48000;
 *              audioStreamParams.channels = STEREO;
 *              audioStreamParams.format = INVALID_WIDTH;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_IsDirectVoipParams_003, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    AudioStreamParams audioStreamParams;
    audioStreamParams.samplingRate = SAMPLE_RATE_48000;
    audioStreamParams.channels = STEREO;
    audioStreamParams.format = INVALID_WIDTH;
    bool ret = audioRendererPrivate->IsDirectVoipParams(audioStreamParams);
    EXPECT_FALSE(ret);
}

/**
 * @tc.name  : Test IsDirectVoipParams
 * @tc.number: Audio_Renderer_IsDirectVoipParams_004
 * @tc.desc  : Test Create interface with IsDirectVoipParams below.
 *              audioStreamParams.samplingRate = SAMPLE_RATE_16000;
 *              audioStreamParams.channels = CHANNEL_3;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_IsDirectVoipParams_004, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    AudioStreamParams audioStreamParams;
    audioStreamParams.samplingRate = SAMPLE_RATE_16000;
    audioStreamParams.channels = CHANNEL_3;
    bool ret = audioRendererPrivate->IsDirectVoipParams(audioStreamParams);
    EXPECT_FALSE(ret);
}

/**
 * @tc.name  : Test IsDirectVoipParams
 * @tc.number: Audio_Renderer_IsDirectVoipParams_005
 * @tc.desc  : Test Create interface with IsDirectVoipParams below.
 *              audioStreamParams.samplingRate = SAMPLE_RATE_192000;
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_IsDirectVoipParams_005, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    AudioStreamParams audioStreamParams;
    audioStreamParams.samplingRate = SAMPLE_RATE_192000;
    bool ret = audioRendererPrivate->IsDirectVoipParams(audioStreamParams);
    EXPECT_FALSE(ret);
}

/**
 * @tc.name  : Test OnAudioPolicyServiceDied
 * @tc.number: Audio_Renderer_OnAudioPolicyServiceDied_001
 * @tc.desc  : Test OnAudioPolicyServiceDied interface
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_OnAudioPolicyServiceDied_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    audioRendererPrivate->RegisterRendererPolicyServiceDiedCallback();
    audioRendererPrivate->RegisterOutputDeviceChangeWithInfoCallback(nullptr);
    audioRendererPrivate->audioPolicyServiceDiedCallback_->OnAudioPolicyServiceDied();
    ASSERT_NE(nullptr, audioRendererPrivate);
}

/**
 * @tc.name  : Test RegisterOutputDeviceChangeWithInfoCallback
 * @tc.number: Audio_Renderer_RegisterOutputDeviceChangeWithInfoCallback_001
 * @tc.desc  : Test RegisterOutputDeviceChangeWithInfoCallback interface
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_RegisterOutputDeviceChangeWithInfoCallback_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    audioRendererPrivate->RegisterOutputDeviceChangeWithInfoCallback(nullptr);
    ASSERT_NE(nullptr, audioRendererPrivate);
}

/**
 * @tc.name  : Test SetSilentModeAndMixWithOthers
 * @tc.number: Audio_Renderer_WriteUnderrunEvent_001
 * @tc.desc  : Test SetSilentModeAndMixWithOthers interface
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_WriteUnderrunEvent_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    audioRendererPrivate->rendererInfo_.originalFlag = AUDIO_FLAG_MMAP;
    std::shared_ptr<FastAudioStream> audioStream = std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK,
        appInfo.appUid);
    audioRendererPrivate->audioStream_ = audioStream;
    audioStream->state_ = RUNNING;
    audioStream->silentModeAndMixWithOthers_ = false;
    audioRendererPrivate->SetSilentModeAndMixWithOthers(true);
    EXPECT_TRUE(audioStream->silentModeAndMixWithOthers_);
}

/**
 * @tc.name  : Test SetSilentModeAndMixWithOthers
 * @tc.number: Audio_Renderer_WriteUnderrunEvent_002
 * @tc.desc  : Test SetSilentModeAndMixWithOthers interface
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_WriteUnderrunEvent_002, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    audioRendererPrivate->rendererInfo_.originalFlag = AUDIO_FLAG_MMAP;
    std::shared_ptr<FastAudioStream> audioStream = std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK,
        appInfo.appUid);
    audioRendererPrivate->audioStream_ = audioStream;
    audioStream->state_ = RUNNING;
    audioStream->silentModeAndMixWithOthers_ = true;
    audioRendererPrivate->SetSilentModeAndMixWithOthers(true);
    EXPECT_TRUE(audioStream->silentModeAndMixWithOthers_);
}

/**
 * @tc.name  : Test SetSilentModeAndMixWithOthers
 * @tc.number: Audio_Renderer_WriteUnderrunEvent_003
 * @tc.desc  : Test SetSilentModeAndMixWithOthers interface
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_WriteUnderrunEvent_003, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    audioRendererPrivate->rendererInfo_.originalFlag = AUDIO_FLAG_MMAP;
    std::shared_ptr<FastAudioStream> audioStream = std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK,
        appInfo.appUid);
    audioRendererPrivate->audioStream_ = audioStream;
    audioStream->state_ = RUNNING;
    audioStream->silentModeAndMixWithOthers_ = false;
    audioRendererPrivate->SetSilentModeAndMixWithOthers(false);
    EXPECT_FALSE(audioStream->silentModeAndMixWithOthers_);
}

/**
 * @tc.name  : Test SetSilentModeAndMixWithOthers
 * @tc.number: Audio_Renderer_WriteUnderrunEvent_004
 * @tc.desc  : Test SetSilentModeAndMixWithOthers interface
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_WriteUnderrunEvent_004, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    audioRendererPrivate->rendererInfo_.originalFlag = AUDIO_FLAG_MMAP;
    std::shared_ptr<FastAudioStream> audioStream = std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK,
        appInfo.appUid);
    audioRendererPrivate->audioStream_ = audioStream;
    audioStream->state_ = RUNNING;
    audioStream->silentModeAndMixWithOthers_ = true;
    audioRendererPrivate->SetSilentModeAndMixWithOthers(false);
    EXPECT_FALSE(audioStream->silentModeAndMixWithOthers_);
}

/**
 * @tc.name  : Test OnInterrupt
 * @tc.number: Audio_Renderer_OnInterrupt_001
 * @tc.desc  : Test OnInterrupt interface
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_OnInterrupt_001, TestSize.Level1)
{
    AudioStreamParams audioStreamParams;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::FAST_STREAM,
        audioStreamParams, STREAM_DEFAULT, 1);
    AudioInterrupt audioInterrupt;
    auto audioInterruptCallback = std::make_shared<AudioRendererInterruptCallbackImpl>(audioStream, audioInterrupt);
    InterruptEventInternal interruptEvent {INTERRUPT_TYPE_BEGIN, INTERRUPT_SHARE, INTERRUPT_HINT_PAUSE, 20.0f};
    audioInterruptCallback->audioStream_ = nullptr;
    audioInterruptCallback->OnInterrupt(interruptEvent);
}

/**
 * @tc.name  : Test ConcedeStream
 * @tc.number: Audio_Renderer_ConcedeStream_001
 * @tc.desc  : Test ConcedeStream interface
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_ConcedeStream_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    audioRendererPrivate->rendererInfo_.originalFlag = AUDIO_FLAG_MMAP;
    std::shared_ptr<FastAudioStream> audioStream = std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK,
        appInfo.appUid);
    audioStream->rendererInfo_.pipeType = PIPE_TYPE_LOWLATENCY_OUT;
    audioRendererPrivate->ConcedeStream();
    ASSERT_NE(nullptr, audioRendererPrivate);
}

/**
 * @tc.name  : Test ConcedeStream
 * @tc.number: Audio_Renderer_ConcedeStream_002
 * @tc.desc  : Test ConcedeStream interface
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_ConcedeStream_002, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    audioRendererPrivate->rendererInfo_.originalFlag = AUDIO_FLAG_MMAP;
    std::shared_ptr<FastAudioStream> audioStream = std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK,
        appInfo.appUid);
    audioStream->rendererInfo_.pipeType = PIPE_TYPE_DIRECT_MUSIC;
    audioRendererPrivate->ConcedeStream();
    ASSERT_NE(nullptr, audioRendererPrivate);
}

/**
 * @tc.name  : Test ConcedeStream
 * @tc.number: Audio_Renderer_ConcedeStream_003
 * @tc.desc  : Test ConcedeStream interface
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_ConcedeStream_003, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    audioRendererPrivate->rendererInfo_.originalFlag = AUDIO_FLAG_MMAP;
    std::shared_ptr<FastAudioStream> audioStream = std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK,
        appInfo.appUid);
    audioRendererPrivate->WriteUnderrunEvent();
    audioStream->rendererInfo_.pipeType = PIPE_TYPE_UNKNOWN;
    audioRendererPrivate->ConcedeStream();
    ASSERT_NE(nullptr, audioRendererPrivate);
}

/**
 * @tc.name  : Test direct VoIP Audio Render
 * @tc.number: Audio_Renderer_Direct_VoIP_001
 * @tc.desc  : Test the direct VoIP stream type with STREAM_USAGE_VOICE_COMMUNICATION
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Direct_VoIP_001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_8000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_UNKNOWN;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VOICE_COMMUNICATION;
    rendererOptions.rendererInfo.rendererFlags = 0;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
    EXPECT_EQ(SUCCESS, ret);

    shared_ptr<AudioRendererWriteCallbackMock> cb = make_shared<AudioRendererWriteCallbackMock>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(SUCCESS, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    std::this_thread::sleep_for(1s);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);
    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test direct VoIP Audio Render
 * @tc.number: Audio_Renderer_Direct_VoIP_002
 * @tc.desc  : Test the direct VoIP stream type with STREAM_USAGE_VOICE_COMMUNICATION
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Direct_VoIP_002, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_16000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_UNKNOWN;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VOICE_COMMUNICATION;
    rendererOptions.rendererInfo.rendererFlags = 0;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
    EXPECT_EQ(SUCCESS, ret);

    shared_ptr<AudioRendererWriteCallbackMock> cb = make_shared<AudioRendererWriteCallbackMock>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(SUCCESS, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    std::this_thread::sleep_for(1s);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);
    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test direct VoIP Audio Render
 * @tc.number: Audio_Renderer_Direct_VoIP_003
 * @tc.desc  : Test the direct VoIP stream type with STREAM_USAGE_VOICE_COMMUNICATION
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Direct_VoIP_003, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_48000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_UNKNOWN;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VOICE_COMMUNICATION;
    rendererOptions.rendererInfo.rendererFlags = 0;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
    EXPECT_EQ(SUCCESS, ret);

    shared_ptr<AudioRendererWriteCallbackMock> cb = make_shared<AudioRendererWriteCallbackMock>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(SUCCESS, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    std::this_thread::sleep_for(1s);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);
    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test direct VoIP Audio Render
 * @tc.number: Audio_Renderer_Direct_VoIP_004
 * @tc.desc  : Test the direct VoIP stream type with STREAM_USAGE_VOICE_COMMUNICATION
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Direct_VoIP_004, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_48000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S32LE;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_UNKNOWN;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VOICE_COMMUNICATION;
    rendererOptions.rendererInfo.rendererFlags = 0;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
    EXPECT_EQ(SUCCESS, ret);

    shared_ptr<AudioRendererWriteCallbackMock> cb = make_shared<AudioRendererWriteCallbackMock>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(SUCCESS, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    std::this_thread::sleep_for(1s);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);
    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test direct VoIP Audio Render
 * @tc.number: Audio_Renderer_Direct_VoIP_005
 * @tc.desc  : Test the direct VoIP stream type with STREAM_USAGE_VOICE_COMMUNICATION
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Direct_VoIP_005, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_48000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptions.streamInfo.channels = AudioChannel::MONO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_UNKNOWN;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VOICE_COMMUNICATION;
    rendererOptions.rendererInfo.rendererFlags = 0;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
    EXPECT_EQ(SUCCESS, ret);

    shared_ptr<AudioRendererWriteCallbackMock> cb = make_shared<AudioRendererWriteCallbackMock>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(SUCCESS, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    std::this_thread::sleep_for(1s);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);
    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test direct VoIP Audio Render
 * @tc.number: Audio_Renderer_Direct_VoIP_006
 * @tc.desc  : Test the direct VoIP stream type with STREAM_USAGE_VIDEO_COMMUNICATION
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Direct_VoIP_006, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_8000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_UNKNOWN;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VIDEO_COMMUNICATION;
    rendererOptions.rendererInfo.rendererFlags = 0;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
    EXPECT_EQ(SUCCESS, ret);

    shared_ptr<AudioRendererWriteCallbackMock> cb = make_shared<AudioRendererWriteCallbackMock>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(SUCCESS, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    std::this_thread::sleep_for(1s);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);
    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test direct VoIP Audio Render
 * @tc.number: Audio_Renderer_Direct_VoIP_007
 * @tc.desc  : Test the direct VoIP stream type with STREAM_USAGE_VIDEO_COMMUNICATION
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Direct_VoIP_007, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_16000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_UNKNOWN;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VIDEO_COMMUNICATION;
    rendererOptions.rendererInfo.rendererFlags = 0;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
    EXPECT_EQ(SUCCESS, ret);

    shared_ptr<AudioRendererWriteCallbackMock> cb = make_shared<AudioRendererWriteCallbackMock>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(SUCCESS, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    std::this_thread::sleep_for(1s);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);
    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test direct VoIP Audio Render
 * @tc.number: Audio_Renderer_Direct_VoIP_008
 * @tc.desc  : Test the direct VoIP stream type with STREAM_USAGE_VIDEO_COMMUNICATION
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Direct_VoIP_008, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_48000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_UNKNOWN;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VIDEO_COMMUNICATION;
    rendererOptions.rendererInfo.rendererFlags = 0;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
    EXPECT_EQ(SUCCESS, ret);

    shared_ptr<AudioRendererWriteCallbackMock> cb = make_shared<AudioRendererWriteCallbackMock>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(SUCCESS, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    std::this_thread::sleep_for(1s);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);
    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test direct VoIP Audio Render
 * @tc.number: Audio_Renderer_Direct_VoIP_009
 * @tc.desc  : Test the direct VoIP stream type with STREAM_USAGE_VIDEO_COMMUNICATION
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Direct_VoIP_009, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_48000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S32LE;
    rendererOptions.streamInfo.channels = AudioChannel::STEREO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_UNKNOWN;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VIDEO_COMMUNICATION;
    rendererOptions.rendererInfo.rendererFlags = 0;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
    EXPECT_EQ(SUCCESS, ret);

    shared_ptr<AudioRendererWriteCallbackMock> cb = make_shared<AudioRendererWriteCallbackMock>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(SUCCESS, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    std::this_thread::sleep_for(1s);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);
    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test direct VoIP Audio Render
 * @tc.number: Audio_Renderer_Direct_VoIP_010
 * @tc.desc  : Test the direct VoIP stream type with STREAM_USAGE_VIDEO_COMMUNICATION
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_Direct_VoIP_010, TestSize.Level1)
{
    int32_t ret = -1;
    AudioRendererOptions rendererOptions;
    rendererOptions.streamInfo.samplingRate = AudioSamplingRate::SAMPLE_RATE_48000;
    rendererOptions.streamInfo.encoding = AudioEncodingType::ENCODING_PCM;
    rendererOptions.streamInfo.format = AudioSampleFormat::SAMPLE_S16LE;
    rendererOptions.streamInfo.channels = AudioChannel::MONO;
    rendererOptions.rendererInfo.contentType = ContentType::CONTENT_TYPE_UNKNOWN;
    rendererOptions.rendererInfo.streamUsage = StreamUsage::STREAM_USAGE_VIDEO_COMMUNICATION;
    rendererOptions.rendererInfo.rendererFlags = 0;

    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    ret = audioRenderer->SetRenderMode(RENDER_MODE_CALLBACK);
    EXPECT_EQ(SUCCESS, ret);

    shared_ptr<AudioRendererWriteCallbackMock> cb = make_shared<AudioRendererWriteCallbackMock>();

    ret = audioRenderer->SetRendererWriteCallback(cb);
    EXPECT_EQ(SUCCESS, ret);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    std::this_thread::sleep_for(1s);

    bool isStopped = audioRenderer->Stop();
    EXPECT_EQ(true, isStopped);
    bool isReleased = audioRenderer->Release();
    EXPECT_EQ(true, isReleased);
}

/**
 * @tc.name  : Test GetFormatSize
 * @tc.number: GetFormatSize
 * @tc.desc  : Test GetFormatSize
 */
HWTEST(AudioRendererUnitTest, GetFormatSize_001, TestSize.Level1)
{
    AudioStreamParams params;
    params.format = SAMPLE_U8;
    const AudioStreamParams info_1 = params;
    size_t ret = GetFormatSize(info_1);
    EXPECT_EQ(ret, 1);

    params.format = SAMPLE_S16LE;
    const AudioStreamParams info_2 = params;
    ret = GetFormatSize(info_2);
    EXPECT_EQ(ret, 2);

    params.format = SAMPLE_S24LE;
    const AudioStreamParams info_3 = params;
    ret = GetFormatSize(info_3);
    EXPECT_EQ(ret, 3);

    params.format = SAMPLE_S32LE;
    const AudioStreamParams info_4 = params;
    ret = GetFormatSize(info_4);
    EXPECT_EQ(ret, 4);

    params.format = INVALID_WIDTH;
    const AudioStreamParams info_5 = params;
    ret = GetFormatSize(info_5);
    EXPECT_EQ(ret, 2);

    params.format = SAMPLE_F32LE;
    const AudioStreamParams info_6 = params;
    ret = GetFormatSize(info_6);
    EXPECT_EQ(ret, 4);
}

/**
 * @tc.name  : Test InitAudioInterruptCallback
 * @tc.number: InitAudioInterruptCallback
 * @tc.desc  : Test InitAudioInterruptCallback
 */
HWTEST(AudioRendererUnitTest, InitAudioInterruptCallback_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    audioRendererPrivate->audioInterrupt_.streamId = 1;
    audioRendererPrivate->InitAudioInterruptCallback();
    EXPECT_EQ(audioRendererPrivate->audioInterrupt_.streamId, 1);
}

/**
 * @tc.name  : Test GetPreferredStreamClass
 * @tc.number: GetPreferredStreamClass
 * @tc.desc  : Test GetPreferredStreamClass
 */
HWTEST(AudioRendererUnitTest, GetPreferredStreamClass_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    AudioStreamParams audioStreamParams;
    audioStreamParams.samplingRate = SAMPLE_RATE_64000;
    audioRendererPrivate->rendererInfo_.originalFlag = AUDIO_FLAG_MMAP;

    audioRendererPrivate->GetPreferredStreamClass(audioStreamParams);
    EXPECT_EQ(audioRendererPrivate->rendererInfo_.rendererFlags, AUDIO_FLAG_NORMAL);
}

/**
 * @tc.name  : Test IsDirectVoipParams
 * @tc.number: IsDirectVoipParams
 * @tc.desc  : Test IsDirectVoipParams
 */
HWTEST(AudioRendererUnitTest, IsDirectVoipParams_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    AudioStreamParams audioStreamParams_;
    audioStreamParams_.samplingRate = SAMPLE_RATE_48000;
    audioStreamParams_.channels = CHANNEL_3;
    const AudioStreamParams audioStreamParams = audioStreamParams_;

    bool ret = audioRendererPrivate->IsDirectVoipParams(audioStreamParams);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name  : Test IsDirectVoipParams
 * @tc.number: IsDirectVoipParams
 * @tc.desc  : Test IsDirectVoipParams
 */
HWTEST(AudioRendererUnitTest, IsDirectVoipParams_002, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    AudioStreamParams audioStreamParams_;
    audioStreamParams_.samplingRate = SAMPLE_RATE_48000;
    audioStreamParams_.channels = STEREO;
    audioStreamParams_.format = SAMPLE_S16LE;
    const AudioStreamParams audioStreamParams = audioStreamParams_;

    bool ret = audioRendererPrivate->IsDirectVoipParams(audioStreamParams);
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name  : Test IsDirectVoipParams
 * @tc.number: IsDirectVoipParams
 * @tc.desc  : Test IsDirectVoipParams
 */
HWTEST(AudioRendererUnitTest, IsDirectVoipParams_003, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    AudioStreamParams audioStreamParams_;
    audioStreamParams_.samplingRate = SAMPLE_RATE_48000;
    audioStreamParams_.channels = STEREO;
    audioStreamParams_.format = SAMPLE_F32LE;
    const AudioStreamParams audioStreamParams = audioStreamParams_;

    bool ret = audioRendererPrivate->IsDirectVoipParams(audioStreamParams);
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name  : Test PrepareAudioStream
 * @tc.number: PrepareAudioStream
 * @tc.desc  : Test PrepareAudioStream
 */
HWTEST(AudioRendererUnitTest, PrepareAudioStream_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    audioRendererPrivate->audioStream_ = nullptr;
    AudioStreamParams audioStreamParams;
    const AudioStreamType audioStreamType = STREAM_VOICE_CALL;
    IAudioStream::StreamClass streamClass;

    int32_t ret = audioRendererPrivate->PrepareAudioStream(audioStreamParams, audioStreamType, streamClass);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test GetStreamInfo
 * @tc.number: GetStreamInfo
 * @tc.desc  : Test GetStreamInfo
 */
HWTEST(AudioRendererUnitTest, GetStreamInfo_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    AudioStreamInfo streamInfo;

    int32_t ret = audioRendererPrivate->GetStreamInfo(streamInfo);
    EXPECT_EQ(ret, -62980101);
}

/**
 * @tc.name  : Test PauseTransitent
 * @tc.number: PauseTransitent
 * @tc.desc  : Test PauseTransitent
 */
HWTEST(AudioRendererUnitTest, PauseTransitent_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    StateChangeCmdType cmdType = CMD_FROM_SYSTEM;
    audioRendererPrivate->isSwitching_ = true;

    bool ret = audioRendererPrivate->PauseTransitent(cmdType);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name  : Test PauseTransitent
 * @tc.number: PauseTransitent
 * @tc.desc  : Test PauseTransitent
 */
HWTEST(AudioRendererUnitTest, PauseTransitent_002, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    StateChangeCmdType cmdType = CMD_FROM_SYSTEM;
    audioRendererPrivate->rendererInfo_.streamUsage = STREAM_USAGE_VOICE_MODEM_COMMUNICATION;
    audioRendererPrivate->isEnableVoiceModemCommunicationStartStream_ = false;

    bool ret = audioRendererPrivate->PauseTransitent(cmdType);
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name  : Test UpdateAudioInterruptStrategy
 * @tc.number: UpdateAudioInterruptStrategy
 * @tc.desc  : Test UpdateAudioInterruptStrategy
 */
HWTEST(AudioRendererUnitTest, UpdateAudioInterruptStrategy_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    audioRendererPrivate->isStillMuted_ = true;
    float volume = 1;

    audioRendererPrivate->UpdateAudioInterruptStrategy(volume);
    EXPECT_EQ(audioRendererPrivate->isStillMuted_, false);
}

/**
 * @tc.name  : Test AudioRendererPrivate
 * @tc.number: UpdateAudioInterruptStrategy_002
 * @tc.desc  : Test UpdateAudioInterruptStrategy API
 */
HWTEST(AudioRendererUnitTest, UpdateAudioInterruptStrategy_002, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    audioRendererPrivate->isStillMuted_ = true;
    float volume = 1;
    std::shared_ptr<TestAudioStremStub> testAudioStremStub = std::make_shared<TestAudioStremStub>();

    testAudioStremStub->state_ = RUNNING;
    audioRendererPrivate->audioStream_ = testAudioStremStub;
    audioRendererPrivate->UpdateAudioInterruptStrategy(volume);
    EXPECT_EQ(audioRendererPrivate->isStillMuted_, false);
}

/**
 * @tc.name  : Test HandleAndNotifyForcedEvent
 * @tc.number: HandleAndNotifyForcedEvent
 * @tc.desc  : Test HandleAndNotifyForcedEvent
 */
HWTEST(AudioRendererUnitTest, HandleAndNotifyForcedEvent_001, TestSize.Level1)
{
    AudioStreamParams audioStreamParams;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::FAST_STREAM,
        audioStreamParams, STREAM_DEFAULT, 1);
    AudioInterrupt audioInterrupt;
    auto audioInterruptCallback = std::make_shared<AudioRendererInterruptCallbackImpl>(audioStream, audioInterrupt);
    InterruptEventInternal interruptEvent_;
    interruptEvent_.hintType = INTERRUPT_HINT_PAUSE;

    const InterruptEventInternal interruptEvent = interruptEvent_;
    std::shared_ptr<TestAudioStremStub> testAudioStremStub = std::make_shared<TestAudioStremStub>();
    testAudioStremStub->state_ = NEW;

    audioInterruptCallback->HandleAndNotifyForcedEvent(interruptEvent);
    EXPECT_EQ(testAudioStremStub->state_, NEW);
}

/**
 * @tc.name  : Test HandleAndNotifyForcedEvent
 * @tc.number: HandleAndNotifyForcedEvent
 * @tc.desc  : Test HandleAndNotifyForcedEvent
 */
HWTEST(AudioRendererUnitTest, HandleAndNotifyForcedEvent_002, TestSize.Level1)
{
    AudioStreamParams audioStreamParams;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::FAST_STREAM,
        audioStreamParams, STREAM_DEFAULT, 1);
    AudioInterrupt audioInterrupt;
    auto audioInterruptCallback = std::make_shared<AudioRendererInterruptCallbackImpl>(audioStream, audioInterrupt);
    InterruptEventInternal interruptEvent_;
    interruptEvent_.hintType = INTERRUPT_HINT_RESUME;
    audioInterruptCallback->isForcePaused_ = true;

    const InterruptEventInternal interruptEvent = interruptEvent_;
    std::shared_ptr<TestAudioStremStub> testAudioStremStub = std::make_shared<TestAudioStremStub>();
    testAudioStremStub->state_ = PAUSED;

    audioInterruptCallback->HandleAndNotifyForcedEvent(interruptEvent);
    EXPECT_EQ(audioInterruptCallback->isForcePaused_, true);
}

/**
 * @tc.name  : Test HandleAndNotifyForcedEvent
 * @tc.number: HandleAndNotifyForcedEvent
 * @tc.desc  : Test HandleAndNotifyForcedEvent
 */
HWTEST(AudioRendererUnitTest, HandleAndNotifyForcedEvent_003, TestSize.Level1)
{
    AudioStreamParams audioStreamParams;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::FAST_STREAM,
        audioStreamParams, STREAM_DEFAULT, 1);
    AudioInterrupt audioInterrupt;
    auto audioInterruptCallback = std::make_shared<AudioRendererInterruptCallbackImpl>(audioStream, audioInterrupt);
    InterruptEventInternal interruptEvent_;
    interruptEvent_.hintType = INTERRUPT_HINT_RESUME;
    audioInterruptCallback->isForcePaused_ = true;

    const InterruptEventInternal interruptEvent = interruptEvent_;
    std::shared_ptr<TestAudioStremStub> testAudioStremStub = std::make_shared<TestAudioStremStub>();
    testAudioStremStub->state_ = NEW;

    audioInterruptCallback->HandleAndNotifyForcedEvent(interruptEvent);
    EXPECT_EQ(testAudioStremStub->state_, NEW);
}

/**
 * @tc.name  : Test HandleAndNotifyForcedEvent
 * @tc.number: HandleAndNotifyForcedEvent
 * @tc.desc  : Test HandleAndNotifyForcedEvent
 */
HWTEST(AudioRendererUnitTest, HandleAndNotifyForcedEvent_004, TestSize.Level1)
{
    AudioStreamParams audioStreamParams;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::FAST_STREAM,
        audioStreamParams, STREAM_DEFAULT, 1);
    AudioInterrupt audioInterrupt;
    auto audioInterruptCallback = std::make_shared<AudioRendererInterruptCallbackImpl>(audioStream, audioInterrupt);
    InterruptEventInternal interruptEvent_;
    interruptEvent_.hintType = INTERRUPT_HINT_DUCK;
    audioInterruptCallback->isForcePaused_ = true;
    interruptEvent_.duckVolume = 0.5f;

    const InterruptEventInternal interruptEvent = interruptEvent_;
    std::shared_ptr<TestAudioStremStub> testAudioStremStub = std::make_shared<TestAudioStremStub>();
    testAudioStremStub->state_ = NEW;

    audioInterruptCallback->HandleAndNotifyForcedEvent(interruptEvent);
    EXPECT_EQ(audioInterruptCallback->isForcePaused_, true);
}

/**
 * @tc.name  : Test HandleAndNotifyForcedEvent
 * @tc.number: HandleAndNotifyForcedEvent
 * @tc.desc  : Test HandleAndNotifyForcedEvent
 */
HWTEST(AudioRendererUnitTest, HandleAndNotifyForcedEvent_005, TestSize.Level1)
{
    AudioStreamParams audioStreamParams;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::FAST_STREAM,
        audioStreamParams, STREAM_DEFAULT, 1);
    AudioInterrupt audioInterrupt;
    auto audioInterruptCallback = std::make_shared<AudioRendererInterruptCallbackImpl>(audioStream, audioInterrupt);
    InterruptEventInternal interruptEvent_;
    interruptEvent_.hintType = INTERRUPT_HINT_DUCK;
    audioInterruptCallback->isForcePaused_ = true;
    interruptEvent_.duckVolume = 1.5f;

    const InterruptEventInternal interruptEvent = interruptEvent_;
    std::shared_ptr<TestAudioStremStub> testAudioStremStub = std::make_shared<TestAudioStremStub>();
    testAudioStremStub->state_ = NEW;

    audioInterruptCallback->HandleAndNotifyForcedEvent(interruptEvent);
    EXPECT_EQ(testAudioStremStub->state_, NEW);
}

/**
 * @tc.name  : Test HandleAndNotifyForcedEvent
 * @tc.number: HandleAndNotifyForcedEvent
 * @tc.desc  : Test HandleAndNotifyForcedEvent
 */
HWTEST(AudioRendererUnitTest, HandleAndNotifyForcedEvent_006, TestSize.Level1)
{
    AudioStreamParams audioStreamParams;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::FAST_STREAM,
        audioStreamParams, STREAM_DEFAULT, 1);
    AudioInterrupt audioInterrupt;
    auto audioInterruptCallback = std::make_shared<AudioRendererInterruptCallbackImpl>(audioStream, audioInterrupt);
    InterruptEventInternal interruptEvent_;
    interruptEvent_.hintType = INTERRUPT_HINT_DUCK;
    audioInterruptCallback->isForcePaused_ = true;
    interruptEvent_.duckVolume = 1.5f;

    const InterruptEventInternal interruptEvent = interruptEvent_;
    std::shared_ptr<TestAudioStremStub> testAudioStremStub = std::make_shared<TestAudioStremStub>();
    testAudioStremStub->state_ = NEW;

    audioInterruptCallback->HandleAndNotifyForcedEvent(interruptEvent);
    EXPECT_EQ(testAudioStremStub->state_, NEW);
}

/**
 * @tc.name  : Test RegisterOutputDeviceChangeWithInfoCallback
 * @tc.number: RegisterOutputDeviceChangeWithInfoCallback
 * @tc.desc  : Test RegisterOutputDeviceChangeWithInfoCallback
 */
HWTEST(AudioRendererUnitTest, RegisterOutputDeviceChangeWithInfoCallback_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    const std::shared_ptr<AudioRendererOutputDeviceChangeCallback> callback = nullptr;

    int32_t ret = audioRendererPrivate->RegisterOutputDeviceChangeWithInfoCallback(callback);
    EXPECT_EQ(ret, ERR_INVALID_PARAM);
}

/**
 * @tc.name  : Test InitSwitchInfo
 * @tc.number: InitSwitchInfo
 * @tc.desc  : Test InitSwitchInfo
 */
HWTEST(AudioRendererUnitTest, InitSwitchInfo_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    IAudioStream::StreamClass targetClass = IAudioStream::VOIP_STREAM;
    IAudioStream::SwitchInfo info;
    audioRendererPrivate->rendererInfo_.rendererFlags = AUDIO_FLAG_DIRECT;

    audioRendererPrivate->InitSwitchInfo(targetClass, info);
    EXPECT_EQ(info.params.originalSessionId, INVALID_SESSION_ID);
}

/**
 * @tc.name  : InitSwitchInfo_ShouldSetRendererFlags_WhenRendererFlagsIsNormal
 * @tc.number: InitSwitchInfoTest_002
 * @tc.desc  : Test when rendererFlags is AUDIO_FLAG_NORMAL then rendererFlags is set to AUDIO_FLAG_NORMAL
 */
HWTEST(AudioRendererUnitTest, InitSwitchInfo_ShouldSetRendererFlags_WhenRendererFlagsIsNormal, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    IAudioStream::StreamClass targetClass = IAudioStream::PA_STREAM;
    IAudioStream::SwitchInfo info;
    audioRendererPrivate->rendererInfo_.rendererFlags = AUDIO_FLAG_NORMAL;
    audioRendererPrivate->InitSwitchInfo(targetClass, info);
    EXPECT_EQ(info.rendererInfo.rendererFlags, AUDIO_FLAG_NORMAL);
}

/**
 * @tc.name  : InitSwitchInfo_ShouldSetRendererFlags_WhenRendererFlagsIsMMAP
 * @tc.number: InitSwitchInfoTest_003
 * @tc.desc  : Test when rendererFlags is AUDIO_FLAG_MMAP then rendererFlags is set to AUDIO_FLAG_MMAP
 */
HWTEST(AudioRendererUnitTest, InitSwitchInfo_ShouldSetRendererFlags_WhenRendererFlagsIsMMAP, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    IAudioStream::StreamClass targetClass = IAudioStream::FAST_STREAM;
    IAudioStream::SwitchInfo info;
    audioRendererPrivate->rendererInfo_.rendererFlags = AUDIO_FLAG_MMAP;
    audioRendererPrivate->InitSwitchInfo(targetClass, info);
    EXPECT_EQ(info.rendererInfo.rendererFlags, AUDIO_FLAG_MMAP);
}

/**
 * @tc.name  : Test RestoreTheadLoop
 * @tc.number: RestoreTheadLoop
 * @tc.desc  : Test RestoreTheadLoop
 */
HWTEST(AudioRendererUnitTest, RestoreTheadLoop_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);

    const std::shared_ptr<RendererPolicyServiceDiedCallback> serviceCallback =
        std::make_shared<RendererPolicyServiceDiedCallback>();
    std::shared_ptr<FastAudioStream> audioStream =
    std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK, appInfo.appUid);
    audioRendererPrivate->audioStream_ = audioStream;
    serviceCallback->renderer_ = std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    audioRendererPrivate->abortRestore_ = true;
    audioRendererPrivate->rendererInfo_.streamUsage = STREAM_USAGE_VOICE_MODEM_COMMUNICATION;
    audioRendererPrivate->isEnableVoiceModemCommunicationStartStream_ = false;
    audioRendererPrivate->state_ = RENDERER_RUNNING;

    serviceCallback->RestoreTheadLoop();

    EXPECT_EQ(audioRendererPrivate->abortRestore_, true);
}

/**
 * @tc.name  : Test AudioRendererPrivate
 * @tc.number: SetAudioInterrupt
 * @tc.desc  : Test SetAudioInterrupt API
 */
HWTEST(AudioRendererUnitTest, SetAudioInterrupt_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    AudioInterrupt audioInterrupt;

    audioRendererPrivate->SetAudioInterrupt(audioInterrupt);
    EXPECT_EQ(audioRendererPrivate->audioInterrupt_.streamId, 0);
}

/**
 * @tc.name  : Test AudioRendererPrivate
 * @tc.number: GetSourceDuration
 * @tc.desc  : Test GetSourceDuration API
 */
HWTEST(AudioRendererUnitTest, GetSourceDuration_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);

    audioRendererPrivate->sourceDuration_ = 1;
    EXPECT_EQ(audioRendererPrivate->GetSourceDuration(), 1);
}

/**
 * @tc.name  : Test AudioRendererPrivate
 * @tc.number: SetSourceDuration
 * @tc.desc  : Test SetSourceDuration API
 */
HWTEST(AudioRendererUnitTest, SetSourceDuration_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    int64_t duration = 1;
    audioRendererPrivate->audioStream_ = nullptr;
    AudioStreamParams audioStreamParams;
    const AudioStreamType audioStreamType = STREAM_VOICE_CALL;
    IAudioStream::StreamClass streamClass;

    int32_t ret = audioRendererPrivate->PrepareAudioStream(audioStreamParams, audioStreamType, streamClass);
    EXPECT_EQ(ret, SUCCESS);

    audioRendererPrivate->SetSourceDuration(duration);
    EXPECT_EQ(audioRendererPrivate->sourceDuration_, 1);
}

/**
 * @tc.name  : Test AudioRendererPrivate
 * @tc.number: SetAudioPrivacyType_001
 * @tc.desc  : Test SetAudioPrivacyType API
 */
HWTEST(AudioRendererUnitTest, SetAudioPrivacyType_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    AudioPrivacyType privacyType = PRIVACY_TYPE_PUBLIC;

    audioRendererPrivate->audioStream_ = nullptr;
    audioRendererPrivate->SetAudioPrivacyType(privacyType);
    EXPECT_EQ(audioRendererPrivate->audioStream_, nullptr);

    std::shared_ptr<TestAudioStremStub> testAudioStremStub = std::make_shared<TestAudioStremStub>();
    audioRendererPrivate->audioStream_ = testAudioStremStub;
    audioRendererPrivate->SetAudioPrivacyType(privacyType);
    EXPECT_NE(audioRendererPrivate->audioStream_, nullptr);
}

/**
 * @tc.name  : Test AudioRendererPrivate
 * @tc.number: SetClientInfo_001
 * @tc.desc  : Test SetClientInfo API
 */
HWTEST(AudioRendererUnitTest, SetClientInfo_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    uint32_t flag = AUDIO_OUTPUT_FLAG_FAST;
    IAudioStream::StreamClass streamClass;

    audioRendererPrivate->SetClientInfo(flag, streamClass);
    EXPECT_EQ(streamClass, IAudioStream::StreamClass::FAST_STREAM);

    flag = AUDIO_OUTPUT_FLAG_FAST | AUDIO_OUTPUT_FLAG_VOIP;
    audioRendererPrivate->SetClientInfo(flag, streamClass);
    EXPECT_EQ(streamClass, IAudioStream::StreamClass::VOIP_STREAM);

    flag = AUDIO_OUTPUT_FLAG_DIRECT;
    audioRendererPrivate->SetClientInfo(flag, streamClass);
    EXPECT_EQ(streamClass, IAudioStream::StreamClass::PA_STREAM);

    flag = AUDIO_OUTPUT_FLAG_MULTICHANNEL;
    audioRendererPrivate->SetClientInfo(flag, streamClass);
    EXPECT_EQ(streamClass, IAudioStream::StreamClass::PA_STREAM);

    flag = AUDIO_FLAG_NONE;
    audioRendererPrivate->SetClientInfo(flag, streamClass);
    EXPECT_EQ(streamClass, IAudioStream::StreamClass::PA_STREAM);
}

/**
 * @tc.name  : Test AudioRendererPrivate
 * @tc.number: SetVolumeMode_001
 * @tc.desc  : Test SetVolumeMode API
 */
HWTEST(AudioRendererUnitTest, SetVolumeMode_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    int32_t mode = 0;

    audioRendererPrivate->audioStream_ = nullptr;
    int32_t ret = audioRendererPrivate->SetVolumeMode(mode);
    EXPECT_EQ(ret, ERROR_ILLEGAL_STATE);

    std::shared_ptr<TestAudioStremStub> testAudioStremStub = std::make_shared<TestAudioStremStub>();
    audioRendererPrivate->audioStream_ = testAudioStremStub;
    ret = audioRendererPrivate->SetVolumeMode(mode);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioRendererPrivate
 * @tc.number: SetChannelBlendMode_001
 * @tc.desc  : Test SetChannelBlendMode API
 */
HWTEST(AudioRendererUnitTest, SetChannelBlendMode_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    ChannelBlendMode blendMode = MODE_DEFAULT;

    audioRendererPrivate->audioStream_ = nullptr;
    int32_t ret = audioRendererPrivate->SetChannelBlendMode(blendMode);
    EXPECT_EQ(ret, ERROR_ILLEGAL_STATE);

    std::shared_ptr<TestAudioStremStub> testAudioStremStub = std::make_shared<TestAudioStremStub>();
    audioRendererPrivate->audioStream_ = testAudioStremStub;
    ret = audioRendererPrivate->SetChannelBlendMode(blendMode);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioRendererInterruptCallbackImpl
 * @tc.number: UpdateAudioStream_001
 * @tc.desc  : Test UpdateAudioStream API
 */
HWTEST(AudioRendererUnitTest, UpdateAudioStream_001, TestSize.Level1)
{
    AudioStreamParams audioStreamParams;
    std::shared_ptr<IAudioStream> audioStream = IAudioStream::GetPlaybackStream(IAudioStream::FAST_STREAM,
        audioStreamParams, STREAM_DEFAULT, 1);
    AudioInterrupt audioInterrupt;
    auto audioInterruptCallback = std::make_shared<AudioRendererInterruptCallbackImpl>(audioStream, audioInterrupt);
    std::shared_ptr<TestAudioStremStub> testAudioStremStub = std::make_shared<TestAudioStremStub>();

    audioInterruptCallback->UpdateAudioStream(testAudioStremStub);
    EXPECT_NE(audioInterruptCallback->audioStream_, nullptr);
}

/**
 * @tc.name  : Test AudioRendererPrivate
 * @tc.number: UnregisterOutputDeviceChangeWithInfoCallback_001
 * @tc.desc  : Test UnregisterOutputDeviceChangeWithInfoCallback API
 */
HWTEST(AudioRendererUnitTest, UnregisterOutputDeviceChangeWithInfoCallback_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);

    audioRendererPrivate->outputDeviceChangeCallback_ = std::make_shared<OutputDeviceChangeWithInfoCallbackImpl>();
    EXPECT_EQ(audioRendererPrivate->UnregisterOutputDeviceChangeWithInfoCallback(), SUCCESS);
}

/**
 * @tc.name  : Test AudioRendererPrivate
 * @tc.number: UnregisterOutputDeviceChangeWithInfoCallback_002
 * @tc.desc  : Test UnregisterOutputDeviceChangeWithInfoCallback API
 */
HWTEST(AudioRendererUnitTest, UnregisterOutputDeviceChangeWithInfoCallback_002, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    std::shared_ptr<AudioRendererOutputDeviceChangeCallback> callback = nullptr;

    audioRendererPrivate->outputDeviceChangeCallback_ = std::make_shared<OutputDeviceChangeWithInfoCallbackImpl>();
    EXPECT_EQ(audioRendererPrivate->UnregisterOutputDeviceChangeWithInfoCallback(callback), SUCCESS);
}

/**
 * @tc.name  : Test AudioRendererPrivate
 * @tc.number: UpdateRendererAudioStream_001
 * @tc.desc  : Test UpdateRendererAudioStream API
 */
HWTEST(AudioRendererUnitTest, UpdateRendererAudioStream_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    std::shared_ptr<IAudioStream> testAudioStremStub = std::make_shared<TestAudioStremStub>();

    audioRendererPrivate->audioInterruptCallback_ = nullptr;
    audioRendererPrivate->UpdateRendererAudioStream(testAudioStremStub);
    EXPECT_EQ(audioRendererPrivate->audioInterruptCallback_, nullptr);

    AudioInterrupt audioInterrupt;
    audioRendererPrivate->audioInterruptCallback_ = std::make_shared<AudioRendererInterruptCallbackImpl>(
        testAudioStremStub, audioInterrupt);
    audioRendererPrivate->UpdateRendererAudioStream(testAudioStremStub);
    EXPECT_NE(audioRendererPrivate->audioInterruptCallback_, nullptr);
}

/**
 * @tc.name  : Test AudioRendererPrivate
 * @tc.number: UnsetOffloadModeInner_001
 * @tc.desc  : Test UnsetOffloadModeInner API
 */
HWTEST(AudioRendererUnitTest, UnsetOffloadModeInner_001, TestSize.Level1)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    std::shared_ptr<IAudioStream> testAudioStremStub = std::make_shared<TestAudioStremStub>();

    audioRendererPrivate->audioStream_ = testAudioStremStub;
    auto ret = audioRendererPrivate->UnsetOffloadModeInner();
    EXPECT_NE(ret, SUCCESS);
}

/**
 * @tc.name  : Test IsFastStreamClass
 * @tc.number: IsFastStreamClass
 * @tc.desc  : Test IsFastStreamClass
 */
HWTEST(AudioRendererUnitTest, IsFastStreamClass_001, TestSize.Level1)
{
    EXPECT_EQ(IAudioStream::IsFastStreamClass(IAudioStream::PA_STREAM), false);
    EXPECT_EQ(IAudioStream::IsFastStreamClass(IAudioStream::FAST_STREAM), true);
    EXPECT_EQ(IAudioStream::IsFastStreamClass(IAudioStream::VOIP_STREAM), true);
}

/*
 * @tc.name  : Test GetAudioTimestampInfo API via legal input.
 * @tc.number: Audio_Renderer_GetAudioTimestampInfo_001
 * @tc.desc  : Test GetAudioTimestampInfo interface. Returns true, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioTimestampInfo_001, TestSize.Level1)
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

    EXPECT_EQ(SUCCESS, audioRenderer->SetSpeed(2.0));

    size_t bytesToWrite = fread(buffer, 1, bufferLen, wavFile);
    int32_t bytesWritten = audioRenderer->Write(buffer, bytesToWrite);
    EXPECT_GE(bytesWritten, VALUE_ZERO);

    Timestamp timestamp;
    int32_t getAudioTimestampInfoRet =
        audioRenderer->GetAudioTimestampInfo(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(SUCCESS, getAudioTimestampInfoRet);
    EXPECT_GE(timestamp.time.tv_sec, (const long)VALUE_ZERO);
    EXPECT_GE(timestamp.time.tv_nsec, (const long)VALUE_ZERO);

    audioRenderer->Drain();
    audioRenderer->Stop();
    audioRenderer->Release();

    free(buffer);
    fclose(wavFile);
}

/**
 * @tc.name  : Test GetAudioTimestampInfo API via illegal state, RENDERER_NEW: GetAudioTimestampInfo without
 *             initializing the renderer.
 * @tc.number: Audio_Renderer_GetAudioTimestampInfo_002
 * @tc.desc  : Test GetAudioTimestampInfo interface. Returns false, if the renderer state is RENDERER_NEW
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioTimestampInfo_002, TestSize.Level1)
{
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(STREAM_MUSIC);
    ASSERT_NE(nullptr, audioRenderer);

    Timestamp timestamp;
    int32_t ret = audioRenderer->GetAudioTimestampInfo(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(ERR_ILLEGAL_STATE, ret);
}

/**
 * @tc.name  : Test GetAudioTimestampInfo API via legal state, RENDERER_RUNNING.
 * @tc.number: Audio_Renderer_GetAudioTimestampInfo_003
 * @tc.desc  : test GetAudioTimestampInfo interface. Returns true, if the getting is successful.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioTimestampInfo_003, TestSize.Level1)
{
    AudioRendererOptions rendererOptions;

    AudioRendererUnitTest::InitializeRendererOptions(rendererOptions);
    unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(rendererOptions);
    ASSERT_NE(nullptr, audioRenderer);

    bool isStarted = audioRenderer->Start();
    EXPECT_EQ(true, isStarted);

    Timestamp timestamp;
    int32_t ret = audioRenderer->GetAudioTimestampInfo(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(SUCCESS, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetAudioTimestampInfo API via illegal state, RENDERER_STOPPED: GetAudioTimestampInfo after Stop.
 * @tc.number: Audio_Renderer_GetAudioTimestampInfo_004
 * @tc.desc  : Test GetAudioTimestampInfo interface. Returns false, if the renderer state is RENDERER_STOPPED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioTimestampInfo_004, TestSize.Level1)
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
    int32_t ret = audioRenderer->GetAudioTimestampInfo(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(ERR_ILLEGAL_STATE, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetAudioTimestampInfo API via illegal state, RENDERER_RELEASED: GetAudioTimestampInfo after Release.
 * @tc.number: Audio_Renderer_GetAudioTimestampInfo_005
 * @tc.desc  : Test GetAudioTimestampInfo interface. Returns false, if the renderer state is RENDERER_RELEASED
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioTimestampInfo_005, TestSize.Level1)
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
    int32_t ret = audioRenderer->GetAudioTimestampInfo(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(ERR_ILLEGAL_STATE, ret);
}

/**
 * @tc.name  : Test GetAudioTimestampInfo API via illegal state, RENDERER_PAUSED: GetAudioTimestampInfo after Stop.
 * @tc.number: Audio_Renderer_GetAudioTimestampInfo_006
 * @tc.desc  : Test GetAudioTimestampInfo interface. Returns false, if the renderer state is RENDERER_PAUSED.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioTimestampInfo_006, TestSize.Level1)
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
    int32_t ret = audioRenderer->GetAudioTimestampInfo(timestamp, Timestamp::Timestampbase::MONOTONIC);
    EXPECT_EQ(ERR_ILLEGAL_STATE, ret);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test GetAudioTimestampInfo API via legal state, RENDERER_PAUSED.
 * @tc.number: Audio_Renderer_GetAudioTimestampInfo_007
 * @tc.desc  : Test GetAudioTimestampInfo interface. Timestamp should be larger after pause 1s.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_GetAudioTimestampInfo_007, TestSize.Level2)
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
    audioRenderer->GetAudioTimestampInfo(timestamp1, Timestamp::Timestampbase::MONOTONIC);

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
    audioRenderer->GetAudioTimestampInfo(timestamp2, Timestamp::Timestampbase::MONOTONIC);

    int64_t duration = (timestamp2.time.tv_sec - timestamp1.time.tv_sec) * 1000000 + (timestamp2.time.tv_nsec -
        timestamp1.time.tv_nsec) / VALUE_THOUSAND; // ns -> us
    EXPECT_GE(duration, sleepTime);

    audioRenderer->Release();
}

/**
 * @tc.name  : Test InitFormatUnsupportedErrorCallback API.
 * @tc.number: Audio_Renderer_InitFormatUnsupportedErrorCallback_001
 * @tc.desc  : Test InitFormatUnsupportedErrorCallback interface.
 */
HWTEST(AudioRendererUnitTest, Audio_Renderer_InitFormatUnsupportedErrorCallback_001, TestSize.Level2)
{
    AppInfo appInfo = {};
    std::shared_ptr<AudioRendererPrivate> audioRendererPrivate =
        std::make_shared<AudioRendererPrivate>(AudioStreamType::STREAM_MEDIA, appInfo);
    int32_t ret = audioRendererPrivate->InitFormatUnsupportedErrorCallback();
    EXPECT_EQ(SUCCESS, ret);
}
} // namespace AudioStandard
} // namespace OHOS
