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

#include "audio_errors.h"
#include "audio_service_log.h"
#include "audio_info.h"
#include "audio_ring_cache.h"
#include "audio_process_config.h"
#include "linear_pos_time_model.h"
#include "oh_audio_buffer.h"
#include <gtest/gtest.h>
#include "audio_endpoint.h"
#include "remote_fast_audio_renderer_sink.h"

using namespace testing::ext;
namespace OHOS {
namespace AudioStandard {

class AudioEndpointSeparateUnitTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void AudioEndpointSeparateUnitTest::SetUpTestCase(void)
{
    // input testsuit setup step，setup invoked before all testcases
}

void AudioEndpointSeparateUnitTest::TearDownTestCase(void)
{
    // input testsuit teardown step，teardown invoked after all testcases
}

void AudioEndpointSeparateUnitTest::SetUp(void)
{
    // input testcase setup step，setup invoked before each testcases
}

void AudioEndpointSeparateUnitTest::TearDown(void)
{
    // input testcase teardown step，teardown invoked after each testcases
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_001
 * @tc.desc  : Test AudioEndpointSeparate::SetVolume
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_001, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    float volume = 0.0f;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    auto ret = ptr->SetVolume(streamType, volume);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_002
 * @tc.desc  : Test AudioEndpointSeparate::SetVolume
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_002, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_VOICE_CALL;
    float volume = 0.0f;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    auto ret = ptr->SetVolume(streamType, volume);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_003
 * @tc.desc  : Test AudioEndpointSeparate::ResolveBuffer
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_003, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    ptr->isInited_ = false;
    std::shared_ptr<OHAudioBuffer> buffer;
    auto ret = ptr->ResolveBuffer(buffer);
    EXPECT_EQ(ret, ERR_ILLEGAL_STATE);
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_004
 * @tc.desc  : Test AudioEndpointSeparate::ResolveBuffer
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_004, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    ptr->isInited_ = true;
    std::shared_ptr<OHAudioBuffer> buffer;
    auto ret = ptr->ResolveBuffer(buffer);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_005
 * @tc.desc  : Test AudioEndpointSeparate::Release
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_005, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    ptr->isInited_ = false;
    ptr->Release();
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_006
 * @tc.desc  : Test AudioEndpointSeparate::Release
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_006, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    ptr->isInited_ = true;
    ptr->fastSink_ = nullptr;
    ptr->Release();
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_007
 * @tc.desc  : Test AudioEndpointSeparate::Release
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_007, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    ptr->isInited_ = true;
    AudioBufferHolder bufferHolder = AudioBufferHolder::AUDIO_CLIENT;
    uint32_t totalSizeInFrame = 0;
    uint32_t spanSizeInFrame = 0;
    uint32_t byteSizePerFrame = 0;
    ptr->dstAudioBuffer_ = std::make_shared<OHAudioBuffer>(bufferHolder, totalSizeInFrame,
        spanSizeInFrame, byteSizePerFrame);
    ptr->Release();
    ptr->dstAudioBuffer_ = nullptr;
    ptr->Release();
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_008
 * @tc.desc  : Test AudioEndpointSeparate::~AudioEndpointSeparate
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_008, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    ptr->isInited_ = true;
    ptr->~AudioEndpointSeparate();
    ptr->isInited_ = false;
    ptr->~AudioEndpointSeparate();
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_009
 * @tc.desc  : Test AudioEndpointSeparate::Dump
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_009, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    std::string dumpString = {};
    AudioBufferHolder bufferHolder = AudioBufferHolder::AUDIO_CLIENT;
    uint32_t totalSizeInFrame = 0;
    uint32_t spanSizeInFrame = 0;
    uint32_t byteSizePerFrame = 0;
    ptr->dstAudioBuffer_ = std::make_shared<OHAudioBuffer>(bufferHolder, totalSizeInFrame,
        spanSizeInFrame, byteSizePerFrame);
    ptr->Dump(dumpString);
    ptr->dstAudioBuffer_ = nullptr;
    ptr->Dump(dumpString);
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_010
 * @tc.desc  : Test AudioEndpointSeparate::Config
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_010, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    std::shared_ptr<DeviceInfo> ptr2 = std::make_shared<DeviceInfo>();
    ptr2->deviceRole = INPUT_DEVICE;
    ptr2->networkId = REMOTE_NETWORK_ID;
    auto ret = ptr->Config(*ptr2);
    EXPECT_EQ(ret, false);
    ptr2->deviceRole = INPUT_DEVICE;
    ptr2->networkId = LOCAL_NETWORK_ID;
    ret = ptr->Config(*ptr2);
    EXPECT_EQ(ret, false);
    ptr2->deviceRole = OUTPUT_DEVICE;
    ptr2->networkId = REMOTE_NETWORK_ID;
    ret = ptr->Config(*ptr2);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_011
 * @tc.desc  : Test AudioEndpointSeparate::GetAdapterBufferInfo
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_011, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    std::shared_ptr<DeviceInfo> ptr2 = std::make_shared<DeviceInfo>();
    ptr->dstBufferFd_ = 0;
    ptr->dstTotalSizeInframe_ = 1;
    ptr->dstSpanSizeInframe_ = 1;
    ptr->dstBufferFd_ = 1;
    auto ret = ptr->GetAdapterBufferInfo(*ptr2);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_012
 * @tc.desc  : Test AudioEndpointSeparate::GetAdapterBufferInfo
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_012, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    std::shared_ptr<DeviceInfo> ptr2 = std::make_shared<DeviceInfo>();
    ptr->dstBufferFd_ = -1;
    ptr->dstTotalSizeInframe_ = 1;
    ptr->dstSpanSizeInframe_ = 1;
    ptr->dstBufferFd_ = 1;
    auto ret = ptr->GetAdapterBufferInfo(*ptr2);
    EXPECT_EQ(ret, ERR_ILLEGAL_STATE);
    ptr->dstBufferFd_ = 0;
    ptr->dstTotalSizeInframe_ = 0;
    ptr->dstSpanSizeInframe_ = 1;
    ptr->dstBufferFd_ = 1;
    ret = ptr->GetAdapterBufferInfo(*ptr2);
    EXPECT_EQ(ret, ERR_ILLEGAL_STATE);
    ptr->dstBufferFd_ = 0;
    ptr->dstTotalSizeInframe_ = 1;
    ptr->dstSpanSizeInframe_ = 0;
    ptr->dstBufferFd_ = 1;
    ret = ptr->GetAdapterBufferInfo(*ptr2);
    EXPECT_EQ(ret, ERR_ILLEGAL_STATE);
    ptr->dstBufferFd_ = 0;
    ptr->dstTotalSizeInframe_ = 1;
    ptr->dstSpanSizeInframe_ = 1;
    ptr->dstBufferFd_ = 0;
    ret = ptr->GetAdapterBufferInfo(*ptr2);
    EXPECT_EQ(ret, ERR_ILLEGAL_STATE);
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_013
 * @tc.desc  : Test AudioEndpointSeparate::PrepareDeviceBuffer
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_013, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    std::shared_ptr<DeviceInfo> ptr2 = std::make_shared<DeviceInfo>();
    AudioBufferHolder bufferHolder = AudioBufferHolder::AUDIO_CLIENT;
    uint32_t totalSizeInFrame = 0;
    uint32_t spanSizeInFrame = 0;
    uint32_t byteSizePerFrame = 0;
    ptr->dstAudioBuffer_ = std::make_shared<OHAudioBuffer>(bufferHolder, totalSizeInFrame,
        spanSizeInFrame, byteSizePerFrame);
    auto ret = ptr->PrepareDeviceBuffer(*ptr2);
    EXPECT_EQ(ret, SUCCESS);
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_014
 * @tc.desc  : Test AudioEndpointSeparate::PrepareDeviceBuffer
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_014, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    std::shared_ptr<DeviceInfo> ptr2 = std::make_shared<DeviceInfo>();
    ptr->dstAudioBuffer_ = nullptr;
    ptr->PrepareDeviceBuffer(*ptr2);
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_015
 * @tc.desc  : Test AudioEndpointSeparate::InitAudiobuffer
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_015, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    bool resetReadWritePos = true;
    ptr->InitAudiobuffer(resetReadWritePos);
    resetReadWritePos = false;
    ptr->InitAudiobuffer(resetReadWritePos);
    ptr->deviceInfo_.deviceRole = INPUT_DEVICE;
    ptr->InitAudiobuffer(resetReadWritePos);
    ptr->deviceInfo_.deviceRole = OUTPUT_DEVICE;
    ptr->InitAudiobuffer(resetReadWritePos);
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_016
 * @tc.desc  : Test AudioEndpointSeparate::IsAnyProcessRunning
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_016, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    ptr->processBufferList_.clear();
    auto ret = ptr->IsAnyProcessRunning();
    EXPECT_EQ(ret, false);
    AudioBufferHolder bufferHolder = AudioBufferHolder::AUDIO_CLIENT;
    uint32_t totalSizeInFrame = 0;
    uint32_t spanSizeInFrame = 0;
    uint32_t byteSizePerFrame = 0;
    std::shared_ptr<OHAudioBuffer> processBuffer = std::make_shared<OHAudioBuffer>(bufferHolder, totalSizeInFrame,
        spanSizeInFrame, byteSizePerFrame);
    ptr->processBufferList_.push_back(processBuffer);
    ret = ptr->IsAnyProcessRunning();
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_017
 * @tc.desc  : Test AudioEndpointSeparate::ResyncPosition
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_017, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    ptr->ResyncPosition();
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_018
 * @tc.desc  : Test AudioEndpointSeparate::StartDevice
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_018, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    ptr->StartDevice();
}

/**
 * @tc.name  : Test AudioEndpointSeparate API
 * @tc.type  : FUNC
 * @tc.number: AudioEndpointSeparate_019
 * @tc.desc  : Test AudioEndpointSeparate::StartDevice
 */
HWTEST(AudioEndpointSeparateUnitTest, AudioEndpointSeparate_019, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    auto ret ptr->StopDevice();
}
} // namespace AudioStandard
} // namespace OHOS