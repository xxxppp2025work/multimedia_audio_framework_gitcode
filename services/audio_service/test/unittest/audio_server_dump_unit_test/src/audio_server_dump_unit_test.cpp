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

#include "audio_server_dump_unit_test.h"
#include "audio_server_dump.h"

#include "accesstoken_kit.h"
#include "audio_device_info.h"
#include "audio_errors.h"
#include "audio_info.h"
#include "audio_process_config.h"
#include "audio_server.h"
#include "audio_service.h"
#include "audio_stream_info.h"
#include "audio_utils.h"
#include "policy_handler.h"
#include "pa_adapter_tools.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

void AudioServerDumpUnitTest::SetUpTestCase(void) {}

void AudioServerDumpUnitTest::TearDownTestCase(void) {}

void AudioServerDumpUnitTest::SetUp(void) {}

void AudioServerDumpUnitTest::TearDown(void) {}

/**
 * @tc.name  : Test IsEndWith
 * @tc.type  : FUNC
 * @tc.number: AudioServerIsEndWith_001
 * @tc.desc  : Test IsEndWith set true end string
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerIsEndWith_001, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    bool ret = audioServerDump.IsEndWith("Hello World!", "World!");
    EXPECT_TRUE(ret);
}

/**
 * @tc.name  : Test IsEndWith
 * @tc.type  : FUNC
 * @tc.number: AudioServerIsEndWith_002
 * @tc.desc  : Test IsEndWith set false end string
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerIsEndWith_002, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    bool ret = audioServerDump.IsEndWith("Hello World!", "Hello");
    EXPECT_FALSE(ret);
}

/**
 * @tc.name  : Test Initialize
 * @tc.type  : FUNC
 * @tc.number: AudioServerInitialize_001
 * @tc.desc  : Test Initialize mainLoop is not nullptr
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerInitialize_001, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    audioServerDump.Initialize();
    EXPECT_NE(nullptr, audioServerDump.mainLoop);
}

/**
 * @tc.name  : Test Initialize
 * @tc.type  : FUNC
 * @tc.number: AudioServerInitialize_002
 * @tc.desc  : Test Initialize api is not nullptr
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerInitialize_002, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    audioServerDump.Initialize();
    EXPECT_NE(nullptr, audioServerDump.api);
}

/**
 * @tc.name  : Test Initialize
 * @tc.type  : FUNC
 * @tc.number: AudioServerInitialize_003
 * @tc.desc  : Test Initialize context is not nullptr
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerInitialize_003, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    audioServerDump.Initialize();
    EXPECT_NE(nullptr, audioServerDump.context);
}

/**
 * @tc.name  : Test Initialize
 * @tc.type  : FUNC
 * @tc.number: AudioServerInitialize_004
 * @tc.desc  : Test Initialize pa_context_connect()<0
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerInitialize_004, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    audioServerDump.Initialize();
    EXPECT_TRUE(audioServerDump.isContextConnected_);
}

/**
 * @tc.name  : Test Initialize
 * @tc.type  : FUNC
 * @tc.number: AudioServerInitialize_005
 * @tc.desc  : Test Initialize pa_threaded_mainloop_start()<0
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerInitialize_005, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    audioServerDump.Initialize();
    int32_t ret = pa_threaded_mainloop_start(audioServerDump.mainLoop);
    EXPECT_TRUE(ret >= 0);
}

/**
 * @tc.name  : Test Initialize
 * @tc.type  : FUNC
 * @tc.number: AudioServerInitialize_006
 * @tc.desc  : Test Initialize expect return AUDIO_DUMP_SUCCESS
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerInitialize_006, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    int32_t ret = audioServerDump.Initialize();
    EXPECT_EQ(AUDIO_DUMP_SUCCESS, ret);
}

/**
 * @tc.name  : Test PlaybackSinkDump
 * @tc.type  : FUNC
 * @tc.number: AudioServerPlaybackSinkDump_001
 * @tc.desc  : Test PlaybackSinkDump not enter the for loop
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerPlaybackSinkDump_001, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    std::string dumpString;
    audioServerDump.Initialize();
    audioServerDump.PlaybackSinkDump(dumpString);
    std::string expectedOutput = "Playback Streams\n- 0 Playback stream (s) available:\n";
    EXPECT_EQ(dumpString, expectedOutput);
}

/**
 * @tc.name  : Test PlaybackSinkDump
 * @tc.type  : FUNC
 * @tc.number: AudioServerPlaybackSinkDump_002
 * @tc.desc  : Test PlaybackSinkDump enter the for loop
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerPlaybackSinkDump_002, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    std::string PSDumpString;
    audioServerDump.Initialize();
    InputOutputInfo testPlaybackSinkDump = {
        .sessionId = "test_sessionId",
        .applicationName = "test_app",
        .processId = "test_processId_1",
        .userId = 1,
        .privacyType = "0",
        .sampleSpec = {},
        .corked = false,
        .sessionStartTime = "PlaybackSinkDump"
    };
    audioServerDump.streamData_.sinkInputs.push_back(testPlaybackSinkDump);
    audioServerDump.PlaybackSinkDump(PSDumpString);
    std::string endWith = "- Stream Start Time: PlaybackSinkDump\n\n";
    bool ret = audioServerDump.IsEndWith(PSDumpString, endWith);
    EXPECT_TRUE(ret);
}

/**
 * @tc.name  : Test RecordSourceDump
 * @tc.type  : FUNC
 * @tc.number: AudioServerRecordSourceDump_001
 * @tc.desc  : Test RecordSourceDump not enter the for loop
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerRecordSourceDump_001, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    std::string dumpString;
    audioServerDump.Initialize();
    audioServerDump.RecordSourceDump(dumpString);
    std::string expectedOutput = "Record Streams \n- 0 Record stream (s) available:\n";
    EXPECT_EQ(dumpString, expectedOutput);
}

/**
 * @tc.name  : Test RecordSourceDump
 * @tc.type  : FUNC
 * @tc.number: AudioServerRecordSourceDump_002
 * @tc.desc  : Test RecordSourceDump enter the for loop
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerRecordSourceDump_002, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    std::string RSDumpString;
    audioServerDump.Initialize();
    InputOutputInfo testRecordSourceDump = {
        .sessionId = "AudioServerRecordSourceDump_002",
        .applicationName = "RecordSourceDump",
        .processId = "test_processId_2",
        .userId = 1,
        .privacyType = "0",
        .sampleSpec = {},
        .corked = false,
        .sessionStartTime = "RecordSourceDump"
    };
    audioServerDump.streamData_.sourceOutputs.push_back(testRecordSourceDump);
    audioServerDump.RecordSourceDump(RSDumpString);
    std::string endWith = "- Stream Start Time: RecordSourceDump\n\n";
    bool ret = audioServerDump.IsEndWith(RSDumpString, endWith);
    EXPECT_TRUE(ret);
}

/**
 * @tc.name  : Test HDFModulesDump
 * @tc.type  : FUNC
 * @tc.number: AudioServerHDFModulesDump_001
 * @tc.desc  : Test HDFModulesDump not enter the for loop
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerHDFModulesDump_001, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    std::string HDFModulesDumpString;
    audioServerDump.Initialize();
    audioServerDump.HDFModulesDump(HDFModulesDumpString);
    std::string ret = "\nHDF Input Modules\n- 0 HDF Input Modules (s) available:\n"
                        "HDF Output Modules\n- 0 HDF Output Modules (s) available:\n";
    EXPECT_EQ(HDFModulesDumpString, ret);
}

/**
 * @tc.name  : Test IsValidModule
 * @tc.type  : FUNC
 * @tc.number: AudioServerIsValidModule_001
 * @tc.desc  : Test IsValidModule set info begin with fifo
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerIsValidModule_001, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    bool ret;
    ret = audioServerDump.IsValidModule("fifo");
    EXPECT_EQ(false, ret);

    ret = audioServerDump.IsValidModule("fifotest");
    EXPECT_EQ(false, ret);

    ret = audioServerDump.IsValidModule("fifo123");
    EXPECT_EQ(false, ret);
}

/**
 * @tc.name  : Test IsValidModule
 * @tc.type  : FUNC
 * @tc.number: AudioServerIsValidModule_002
 * @tc.desc  : Test IsValidModule set info end with monitor
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerIsValidModule_002, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    bool ret;
    ret = audioServerDump.IsValidModule("monitor");
    EXPECT_EQ(false, ret);

    ret = audioServerDump.IsValidModule("testmonitor");
    EXPECT_EQ(false, ret);

    ret = audioServerDump.IsValidModule("test.monitor");
    EXPECT_EQ(false, ret);
}

/**
 * @tc.name  : Test IsValidModule
 * @tc.type  : FUNC
 * @tc.number: AudioServerIsValidModule_003
 * @tc.desc  : Test IsValidModule set right info
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerIsValidModule_003, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    bool ret;
    ret = audioServerDump.IsValidModule("test");
    EXPECT_EQ(true, ret);

    ret = audioServerDump.IsValidModule("module");
    EXPECT_EQ(true, ret);

    ret = audioServerDump.IsValidModule("valid_module");
    EXPECT_EQ(true, ret);

    ret = audioServerDump.IsValidModule("test_module");
    EXPECT_EQ(true, ret);
}

/**
 * @tc.name  : Test ArgDataDump
 * @tc.type  : FUNC
 * @tc.number: AudioServerArgDataDump_001
 * @tc.desc  : Test ArgDataDump set argQue.empty()
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerArgDataDump_001, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    std::string dumpString;
    std::queue<std::u16string> argQue;
    audioServerDump.ArgDataDump(dumpString, argQue);
    EXPECT_TRUE(argQue.empty());
}

/**
 * @tc.name  : Test ArgDataDump
 * @tc.type  : FUNC
 * @tc.number: AudioServerArgDataDump_002
 * @tc.desc  : Test ArgDataDump set para == u"-h"
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerArgDataDump_002, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    std::string dumpString;
    std::queue<std::u16string> argQue;
    argQue.push(u"-h");
    std::string expectedHelpInfo = "usage:\n"
                                   "  -h\t\t\t|help text for hidumper audio\n"
                                   "  -p\t\t\t|dump pa playback streams\n"
                                   "  -r\t\t\t|dump pa record streams\n"
                                   "  -m\t\t\t|dump hdf input modules\n"
                                   "  -ep\t\t\t|dump policyhandler info\n";
    audioServerDump.ArgDataDump(dumpString, argQue);
    EXPECT_EQ(expectedHelpInfo, dumpString);
}

/**
 * @tc.name  : Test ArgDataDump
 * @tc.type  : FUNC
 * @tc.number: AudioServerArgDataDump_003
 * @tc.desc  : Test ArgDataDump set para == u"invalid_param"
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerArgDataDump_003, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    std::string dumpString;
    std::queue<std::u16string> argQue;
    argQue.push(u"invalid_param");
    std::string expectedHelpInfo = "Please input correct param:\n"
                                   "usage:\n"
                                   "  -h\t\t\t|help text for hidumper audio\n"
                                   "  -p\t\t\t|dump pa playback streams\n"
                                   "  -r\t\t\t|dump pa record streams\n"
                                   "  -m\t\t\t|dump hdf input modules\n"
                                   "  -ep\t\t\t|dump policyhandler info\n";
    audioServerDump.ArgDataDump(dumpString, argQue);
    EXPECT_EQ(expectedHelpInfo, dumpString);
}

/**
 * @tc.name  : Test ArgDataDump
 * @tc.type  : FUNC
 * @tc.number: AudioServerArgDataDump_004
 * @tc.desc  : Test ArgDataDump set para == u"-p"
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerArgDataDump_004, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    std::string dumpString;
    std::queue<std::u16string> argQue;
    argQue.push(u"-p");
    std::string expectedInfo = "AudioServer Data Dump:\n\n"
                               "Playback Streams\n- 0 Playback stream (s) available:\n";
    audioServerDump.ArgDataDump(dumpString, argQue);
    EXPECT_EQ(expectedInfo, dumpString);
}

/**
 * @tc.name  : Test ArgDataDump
 * @tc.type  : FUNC
 * @tc.number: AudioServerArgDataDump_005
 * @tc.desc  : Test ArgDataDump set para == u"-r"
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerArgDataDump_005, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    std::string dumpString;
    std::queue<std::u16string> argQue;
    argQue.push(u"-r");
    std::string expectedInfo = "AudioServer Data Dump:\n\n"
                               "Record Streams \n- 0 Record stream (s) available:\n";
    audioServerDump.ArgDataDump(dumpString, argQue);
    EXPECT_EQ(expectedInfo, dumpString);
}

/**
 * @tc.name  : Test ArgDataDump
 * @tc.type  : FUNC
 * @tc.number: AudioServerArgDataDump_006
 * @tc.desc  : Test ArgDataDump set para == u"-m"
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerArgDataDump_006, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    std::string dumpString;
    std::queue<std::u16string> argQue;
    argQue.push(u"-m");
    std::string expectedInfo = "AudioServer Data Dump:\n\n"
                               "\nHDF Input Modules\n- 0 HDF Input Modules (s) available:\n"
                               "HDF Output Modules\n- 0 HDF Output Modules (s) available:\n";
    audioServerDump.ArgDataDump(dumpString, argQue);
    EXPECT_EQ(expectedInfo, dumpString);
}

/**
 * @tc.name  : Test AudioDataDump
 * @tc.type  : FUNC
 * @tc.number: AudioServerAudioDataDump_001
 * @tc.desc  : Test AudioDataDump set mainLoop and context are nullptr
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerAudioDataDump_001, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    std::string dumpString;
    std::queue<std::u16string> argQue;
    audioServerDump.AudioDataDump(dumpString, argQue);
    EXPECT_EQ(nullptr, audioServerDump.mainLoop);
    EXPECT_EQ(nullptr, audioServerDump.context);
}

/**
 * @tc.name  : Test AudioDataDump
 * @tc.type  : FUNC
 * @tc.number: AudioServerAudioDataDump_002
 * @tc.desc  : Test AudioDataDump set mainLoop and context are not nullptr
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerAudioDataDump_002, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    std::string dumpString;
    std::queue<std::u16string> argQue;
    audioServerDump.Initialize();
    audioServerDump.AudioDataDump(dumpString, argQue);
    EXPECT_NE(nullptr, audioServerDump.mainLoop);
    EXPECT_NE(nullptr, audioServerDump.context);
}

/**
 * @tc.name  : Test ResetPAAudioDump
 * @tc.type  : FUNC
 * @tc.number: AudioServerResetPAAudioDump_001
 * @tc.desc  : Test ResetPAAudioDump set mainLoop is not nullptr and isMainLoopStarted_ is true
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerResetPAAudioDump_001, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    audioServerDump.Initialize();
    EXPECT_NE(nullptr, audioServerDump.mainLoop);
    EXPECT_TRUE(audioServerDump.isMainLoopStarted_);
    audioServerDump.ResetPAAudioDump();
    EXPECT_EQ(nullptr, audioServerDump.mainLoop);
    EXPECT_FALSE(audioServerDump.isMainLoopStarted_);
}

/**
 * @tc.name  : Test ResetPAAudioDump
 * @tc.type  : FUNC
 * @tc.number: AudioServerResetPAAudioDump_002
 * @tc.desc  : Test ResetPAAudioDump set context is not nullptr and isContextConnected_ is true
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerResetPAAudioDump_002, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    audioServerDump.Initialize();
    EXPECT_NE(nullptr, audioServerDump.context);
    EXPECT_TRUE(audioServerDump.isContextConnected_);
    audioServerDump.ResetPAAudioDump();
    EXPECT_EQ(nullptr, audioServerDump.context);
    EXPECT_FALSE(audioServerDump.isContextConnected_);
}

/**
 * @tc.name  : Test ResetPAAudioDump
 * @tc.type  : FUNC
 * @tc.number: AudioServerResetPAAudioDump_003
 * @tc.desc  : Test ResetPAAudioDump set mainLoop is not nullptr
 */
HWTEST_F(AudioServerDumpUnitTest, AudioServerResetPAAudioDump_003, TestSize.Level1)
{
    AudioServerDump audioServerDump;

    audioServerDump.Initialize();
    EXPECT_NE(nullptr, audioServerDump.mainLoop);
    audioServerDump.ResetPAAudioDump();
    EXPECT_EQ(nullptr, audioServerDump.mainLoop);
}

} // namespace AudioStandard
} // namespace OHOS
