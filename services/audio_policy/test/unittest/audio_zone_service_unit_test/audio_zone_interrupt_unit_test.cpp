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

#include "audio_zone_unit_test_base.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

class AudioZoneInterruptUnitTest : public AudioZoneUnitTestPreset {
};

struct InterruptInfo {
    StreamUsage usage;
    AudioStreamType type;
    uint32_t streamId;
    int32_t pid;
    int32_t uid;
    int32_t deviceId;
};

static void ActivateInterrupt(int32_t zoneId, InterruptInfo info)
{
    AudioInterrupt interrupt;
    interrupt.streamUsage = info.usage;
    interrupt.audioFocusType.streamType = info.type;
    interrupt.streamId = info.streamId;
    interrupt.pid = info.pid;
    interrupt.uid = info.uid;
    interrupt.deviceId = info.deviceId;
    AudioZoneService::GetInstance().ActivateAudioInterrupt(zoneId, interrupt);
}

static void DeActivateInterrupt(int32_t zoneId, InterruptInfo info)
{
    AudioInterrupt interrupt;
    interrupt.streamUsage = info.usage;
    interrupt.audioFocusType.streamType = info.type;
    interrupt.streamId = info.streamId;
    interrupt.pid = info.pid;
    interrupt.uid = info.uid;
    interrupt.deviceId = info.deviceId;
    AudioZoneService::GetInstance().DeactivateAudioInterrupt(zoneId, interrupt);
}

static void ActivateMusicInterrupt(int32_t zoneId, InterruptInfo info)
{
    info.type = STREAM_MUSIC;
    info.usage = STREAM_USAGE_MUSIC;
    ActivateInterrupt(zoneId, info);
}

static void DeActivateMusicInterrupt(int32_t zoneId, InterruptInfo info)
{
    info.type = STREAM_MUSIC;
    info.usage = STREAM_USAGE_MUSIC;
    DeActivateInterrupt(zoneId, info);
}

static void ActivateVoipInterrupt(int32_t zoneId, InterruptInfo info)
{
    info.type = STREAM_VOICE_CALL;
    info.usage = STREAM_USAGE_VOICE_COMMUNICATION;
    ActivateInterrupt(zoneId, info);
}

static void DeActivateVoipInterrupt(int32_t zoneId, InterruptInfo info)
{
    info.type = STREAM_VOICE_CALL;
    info.usage = STREAM_USAGE_VOICE_COMMUNICATION;
    DeActivateInterrupt(zoneId, info);
}

static void ActivateMovieInterrupt(int32_t zoneId, InterruptInfo info)
{
    info.type = STREAM_MOVIE;
    info.usage = STREAM_USAGE_MOVIE;
    ActivateInterrupt(zoneId, info);
}

static void DeActivateMovieInterrupt(int32_t zoneId, InterruptInfo info)
{
    info.type = STREAM_MOVIE;
    info.usage = STREAM_USAGE_MOVIE;
    DeActivateInterrupt(zoneId, info);
}

static void ActivateGamecInterrupt(int32_t zoneId, InterruptInfo info)
{
    info.type = STREAM_GAME;
    info.usage = STREAM_USAGE_GAME;
    ActivateInterrupt(zoneId, info);
}

static void DeActivateGameInterrupt(int32_t zoneId, InterruptInfo info)
{
    info.type = STREAM_GAME;
    info.usage = STREAM_USAGE_GAME;
    DeActivateInterrupt(zoneId, info);
}

static void AddInterruptToList(std::list<std::pair<AudioInterrupt, AudioFocuState>> &interrupts,
    InterruptInfo info, AudioFocuState state)
{
    AudioInterrupt interrupt;
    interrupt.streamUsage = info.usage;
    interrupt.audioFocusType.streamType = info.type;
    interrupt.streamId = info.streamId;
    interrupt.pid = info.pid;
    interrupt.uid = info.uid;
    interrupt.deviceId = info.deviceId;

    interrupts.emplace_back(std::make_pair(interrupt, state));
}

static void AddMusicInterruptToList(std::list<std::pair<AudioInterrupt, AudioFocuState>> &interrupts,
    InterruptInfo info, AudioFocuState state)
{
    info.type = STREAM_MUSIC;
    info.usage = STREAM_USAGE_MUSIC;
    AddInterruptToList(interrupts, info, state);
}

static void AddVoipInterruptToList(std::list<std::pair<AudioInterrupt, AudioFocuState>> &interrupts,
    InterruptInfo info, AudioFocuState state)
{
    info.type = STREAM_VOICE_CALL;
    info.usage = STREAM_USAGE_VOICE_COMMUNICATION;
    AddInterruptToList(interrupts, info, state);
}

static void AddMovieInterruptToList(std::list<std::pair<AudioInterrupt, AudioFocuState>> &interrupts,
    InterruptInfo info, AudioFocuState state)
{
    info.type = STREAM_MOVIE;
    info.usage = STREAM_USAGE_MOVIE;
    AddInterruptToList(interrupts, info, state);
}

static void AddGameInterruptToList(std::list<std::pair<AudioInterrupt, AudioFocuState>> &interrupts,
    InterruptInfo info, AudioFocuState state)
{
    info.type = STREAM_GAME;
    info.usage = STREAM_USAGE_GAME;
    AddInterruptToList(interrupts, info, state);
}

/**
* @tc.name  : Test AudioZoneInterrupt.
* @tc.number: AudioZoneInterrupt_001
* @tc.desc  : Test audio zone interrupt.
*/
HWTEST_F(AudioZoneInterruptUnitTest, AudioZoneInterrupt_001, TestSize.Level1)
{
    InterruptInfo info;
    info.streamId = 10;
    info.pid = 10;
    info.uid = 10;
    info.deviceId = -1;
    ActivateMusicInterrupt(0, info);
    info.streamId = 20;
    info.pid = 20;
    info.uid = 20;
    ActivateVoipInterrupt(0, info);
    auto interruptList = AudioZoneService::GetInstance().GetAudioInterruptForZone(0);
    EXPECT_EQ(interruptList.size(), 2);
    AudioZoneService::GetInstance().AddUidToAudioZone(zoneId1_, 10);
    interruptList = AudioZoneService::GetInstance().GetAudioInterruptForZone(zoneId1_);
    EXPECT_EQ(interruptList.size(), 1);
}

/**
* @tc.name  : Test AudioZoneInterrupt.
* @tc.number: AudioZoneInterrupt_002
* @tc.desc  : Test audio zone interrupt.
*/
HWTEST_F(AudioZoneInterruptUnitTest, AudioZoneInterrupt_002, TestSize.Level1)
{
    InterruptInfo info;
    info.streamId = 10;
    info.pid = 10;
    info.uid = 10;
    info.deviceId = -1;
    ActivateMusicInterrupt(0, info);
    AudioZoneService::GetInstance().AddUidToAudioZone(zoneId1_, 20);
    info.streamId = 20;
    info.pid = 20;
    info.uid = 20;
    ActivateVoipInterrupt(zoneId1_, info);
    AudioZoneService::GetInstance().RemoveUidFromAudioZone(zoneId1_, 20);
    auto interruptList1 = AudioZoneService::GetInstance().GetAudioInterruptForZone(zoneId1_);
    EXPECT_EQ(interruptList1.size(), 0);
    auto interruptList2 = AudioZoneService::GetInstance().GetAudioInterruptForZone(0);
    EXPECT_EQ(interruptList2.size(), 2);
}

/**
* @tc.name  : Test AudioZoneInterrupt.
* @tc.number: AudioZoneInterrupt_003
* @tc.desc  : Test audio zone interrupt.
*/
HWTEST_F(AudioZoneInterruptUnitTest, AudioZoneInterrupt_003, TestSize.Level1)
{
    InterruptInfo info;
    info.streamId = 10;
    info.pid = 10;
    info.uid = 10;
    info.deviceId = -1;
    AudioZoneService::GetInstance().EnableAudioZoneInterruptReport(1000, zoneId1_, -1, true);
    ActivateMusicInterrupt(0, info);
    AudioZoneService::GetInstance().AddUidToAudioZone(zoneId1_, 20);
    info.streamId = 20;
    info.pid = 20;
    info.uid = 20;
    ActivateVoipInterrupt(zoneId1_, info);
    client1000_->Wait();
    EXPECT_EQ(client1000_->recvEvent_.type, AUDIO_ZONE_INTERRUPT_EVENT);
    AudioZoneService::GetInstance().EnableAudioZoneInterruptReport(1000, zoneId1_, -1, false);
}
} // namespace AudioStandard
} // namespace OHOS