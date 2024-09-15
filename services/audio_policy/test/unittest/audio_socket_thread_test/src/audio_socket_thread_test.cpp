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

#include "audio_socket_thread_test.h"
#include "audio_log.h"
#include "audio_errors.h"
#include <thread>
#include <memory>
#include <vector>
#include <sys/socket.h>
#include <cerrno>
using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

void AudioSocketThreadUnitTest::SetUpTestCase(void) {}
void AudioSocketThreadUnitTest::TearDownTestCase(void) {}
void AudioSocketThreadUnitTest::SetUp(void) {}
void AudioSocketThreadUnitTest::TearDown(void) {}


#define PRINT_LINE printf("debug __LINE__:%d\n", __LINE__)

/**
* @tc.name  : Test AudioSocketThread.
* @tc.number: AudioSocketThread_001
* @tc.desc  : Test IsUpdatePnpDeviceState.
*/
HWTEST_F(AudioSocketThreadUnitTest, AudioSocketThread_001, TestSize.Level1)
{
    AudioEvent event1;
    event1.eventType = 1;
    event1.deviceType = 2;
    event1.name = "Device1";
    event1.address = "Address1";
    event1.anahsName = "AnahsName1";

    // 给audioSocketThread结构体设置初始状态
    audioSocketThread_.UpdatePnpDeviceState(&event1);

    // 测试完全相同的事件
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event1), false);

    // 测试eventType不同的事件
    AudioEvent event2;
    event2.eventType = 2;  // 改变了eventType
    event2.deviceType = 2;
    event2.name = "Device1";
    event2.address = "Address1";
    event2.anahsName = "AnahsName1";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event2), true);

    // 测试deviceType不同的事件
    AudioEvent event3;
    event3.eventType = 1;
    event3.deviceType = 1;
    event3.name = "Device1";
    event3.address = "Address1";
    event3.anahsName = "AnahsName1";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event3), true);

    // 测试name不同的事件
    AudioEvent event4;
    event4.eventType = 1;
    event4.deviceType = 1;
    event4.name = "Device2";
    event4.address = "Address1";
    event4.anahsName = "AnahsName1";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event4), true);

    // 测试address不同的事件
    AudioEvent event5;
    event5.eventType = 1;
    event5.deviceType = 1;
    event5.name = "Device1";
    event5.address = "Address2";
    event5.anahsName = "AnahsName1";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event5), true);

    // 测试anahsName不同的事件
    AudioEvent event6;
    event6.eventType = 1;
    event6.deviceType = 1;
    event6.name = "Device1";
    event6.address = "Address1";
    event6.anahsName = "AnahsName2";

    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event6), true);
}

/**
* @tc.name  : Test AudioSocketThread.
* @tc.number: AudioSocketThread_002
* @tc.desc  : Test IsUpdatePnpDeviceState.
*/
HWTEST_F(AudioSocketThreadUnitTest, AudioSocketThread_002, TestSize.Level1)
{
    AudioEvent event1;
    event1.eventType = 1;
    event1.deviceType = 1;
    event1.name = "Device1";
    event1.address = "Address1";
    event1.anahsName = "AnahsName1";

    //设置初始状态
    audioSocketThread_.UpdatePnpDeviceState(&event1);

    // 测试完全不同的事件
    AudioEvent event2;
    event2.eventType = 2;
    event2.deviceType = 2;
    event2.name = "Device2";
    event2.address = "Address2";
    event2.anahsName = "AnahsName2";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event2), true);

    //测试eventType和eventType都不同的事件
    AudioEvent event3;
    event3.eventType = 2;
    event3.deviceType = 2;
    event3.name = "Device1";
    event3.address = "Address1";
    event3.anahsName = "AnahsName1";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event3), true);

    //测试deviceType和name都不同的事件
    AudioEvent event4;
    event4.eventType = 1;
    event4.deviceType = 2;
    event4.name = "Device2";
    event4.address = "Address1";
    event4.anahsName = "AnahsName1";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event4), true);

    //测试name和adress都不同的事件
    AudioEvent event5;
    event5.eventType = 1;
    event5.deviceType = 1;
    event5.name = "Device2";
    event5.address = "Address2";
    event5.anahsName = "AnahsName1";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event5), true);

    //测试adress和anahsName都不同的事件
    AudioEvent event6;
    event6.eventType = 1;
    event6.deviceType = 1;
    event6.name = "Device1";
    event6.address = "Address2";
    event6.anahsName = "AnahsName2";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event6), true);
}

/**
* @tc.name  : Test AudioSocketThread.
* @tc.number: AudioSocketThread_003
* @tc.desc  : Test IsUpdatePnpDeviceState.
*/
HWTEST_F(AudioSocketThreadUnitTest, AudioSocketThread_003, TestSize.Level1)
{
    AudioEvent event1;
    event1.eventType = 1;
    event1.deviceType = 1;
    event1.name = "Device1";
    event1.address = "Address1";
    event1.anahsName = "AnahsName1";

    //设置初始状态
    audioSocketThread_.UpdatePnpDeviceState(&event1);

    // 测试eventType,eventType和name不同的事件
    AudioEvent event2;
    event2.eventType = 2;
    event2.eventType = 2;
    event2.name = "Device2";
    event2.address = "Address1";
    event2.anahsName = "AnahsName1";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event2), true);

    // 测试eventType,name和address不同的事件
    AudioEvent event3;
    event3.eventType = 2;
    event3.eventType = 2;
    event3.name = "Device2";
    event3.address = "Address1";
    event3.anahsName = "AnahsName1";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event3), true);

    // 测试name,address和anahsName不同的事件
    AudioEvent event4;
    event4.eventType = 2;
    event4.eventType = 2;
    event4.name = "Device2";
    event4.address = "Address1";
    event4.anahsName = "AnahsName1";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event4), true);

    // 测试address,anahsName和eventType不同的事件
    AudioEvent event5;
    event5.eventType = 2;
    event5.eventType = 1;
    event5.name = "Device1";
    event5.address = "Address2";
    event5.anahsName = "AnahsName2";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event5), true);

    // 测试anahsName,eventType和eventType不同的事件
    AudioEvent event6;
    event6.eventType = 2;
    event6.eventType = 2;
    event6.name = "Device1";
    event6.address = "Address1";
    event6.anahsName = "AnahsName2";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event6), true);
}

/**
* @tc.name  : Test AudioSocketThread.
* @tc.number: AudioSocketThread_004
* @tc.desc  : Test IsUpdatePnpDeviceState.
*/
HWTEST_F(AudioSocketThreadUnitTest, AudioSocketThread_004, TestSize.Level1)
{
    AudioEvent event1;
    event1.eventType = 1;
    event1.deviceType = 1;
    event1.name = "Device1";
    event1.address = "Address1";
    event1.anahsName = "AnahsName1";

    //设置初始状态
    audioSocketThread_.UpdatePnpDeviceState(&event1);

    // 测试eventType,eventType,name和adress都不同的事件
    AudioEvent event2;
    event2.eventType = 2;
    event2.deviceType = 2;
    event2.name = "Device2";
    event2.address = "Address2";
    event2.anahsName = "AnahsName1";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event2), true);

    // 测试eventType,name,adress和anahsName都不同的事件
    AudioEvent event3;
    event3.eventType = 1;
    event3.deviceType = 2;
    event3.name = "Device2";
    event3.address = "Address2";
    event3.anahsName = "AnahsName2";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event3), true);

    // 测试name,adress,anahsName和eventType都不同的事件
    AudioEvent event4;
    event4.eventType = 2;
    event4.deviceType = 1;
    event4.name = "Device2";
    event4.address = "Address2";
    event4.anahsName = "AnahsName2";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event4), true);

    // 测试adress,anahsName,eventType和deviceType都不同的事件
    AudioEvent event5;
    event5.eventType = 2;
    event5.deviceType = 2;
    event5.name = "Device1";
    event5.address = "Address2";
    event5.anahsName = "AnahsName2";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event5), true);

    // 测试anahsName,eventType,deviceType和name都不同的事件
    AudioEvent event6;
    event6.eventType = 2;
    event6.deviceType = 2;
    event6.name = "Device2";
    event6.address = "Address1";
    event6.anahsName = "AnahsName2";
    EXPECT_EQ(audioSocketThread_.IsUpdatePnpDeviceState(&event6), true);
}

/**
* @tc.name  : Test AudioSocketThread.
* @tc.number: AudioSocketThread_005
* @tc.desc  : Test IsUpdatePnpDeviceState.
*/
HWTEST_F(AudioSocketThreadUnitTest, AudioSocketThread_005, TestSize.Level1)
{
    AudioEvent event;
    event.eventType = 1;
    event.deviceType = 2;
    event.name = "Device1";
    event.address = "Address1";
    event.anahsName = "AnahsName1";

    AudioSocketThread::UpdatePnpDeviceState(&event);

    // 验证更新后的状态
    EXPECT_EQ(AudioSocketThread::audioSocketEvent_.eventType, 1);
    EXPECT_EQ(AudioSocketThread::audioSocketEvent_.deviceType, 2);
    EXPECT_EQ(AudioSocketThread::audioSocketEvent_.name, "Device1");
    EXPECT_EQ(AudioSocketThread::audioSocketEvent_.address, "Address1");
    EXPECT_EQ(AudioSocketThread::audioSocketEvent_.anahsName, "AnahsName1");

    // 更新为新的事件
    AudioEvent newEvent;
    newEvent.eventType = 3;
    newEvent.deviceType = 4;
    newEvent.name = "Device2";
    newEvent.address = "Address2";
    newEvent.anahsName = "AnahsName2";

    AudioSocketThread::UpdatePnpDeviceState(&newEvent);

    // 验证新的更新后的状态
    EXPECT_EQ(AudioSocketThread::audioSocketEvent_.eventType, 3);
    EXPECT_EQ(AudioSocketThread::audioSocketEvent_.deviceType, 4);
    EXPECT_EQ(AudioSocketThread::audioSocketEvent_.name, "Device2");
    EXPECT_EQ(AudioSocketThread::audioSocketEvent_.address, "Address2");
    EXPECT_EQ(AudioSocketThread::audioSocketEvent_.anahsName, "AnahsName2");
}

/**
* @tc.name  : Test AudioSocketThread.
* @tc.number: AudioSocketThread_006
* @tc.desc  : Test IsUpdatePnpDeviceState.
*/
HWTEST_F(AudioSocketThreadUnitTest, AudioSocketThread_006, TestSize.Level1)
{
    AudioPnpUevent uevent = {
        .subSystem = UEVENT_SUBSYSTEM_SWITCH,
        .switchName = UEVENT_SWITCH_NAME_H2W,
        .switchState = "0",
        .name = "TestDevice",
        .devName = "TestDevName"
    };

    AudioEvent event;
    int32_t result = AudioSocketThread::SetAudioPnpServerEventValue(&event, &uevent);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(event.eventType, AUDIO_DEVICE_REMOVE);
    EXPECT_EQ(event.deviceType, AUDIO_HEADSET);  // Assuming h2wTypeLast was AUDIO_HEADSET
    EXPECT_EQ(event.name, "TestDevice");
    EXPECT_EQ(event.address, "TestDevName");
}

/**
* @tc.name  : Test AudioSocketThread.
* @tc.number: AudioSocketThread_007
* @tc.desc  : Test IsUpdatePnpDeviceState.
*/
HWTEST_F(AudioSocketThreadUnitTest, AudioSocketThread_007, TestSize.Level1)
{
    AudioPnpUevent uevent = {
        .subSystem = UEVENT_SUBSYSTEM_SWITCH,
        .switchName = UEVENT_SWITCH_NAME_H2W,
        .switchState = "1",
        .name = "Headset",
        .devName = "HeadsetDevName"
    };

    AudioEvent event;
    int32_t result = AudioSocketThread::SetAudioPnpServerEventValue(&event, &uevent);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(event.eventType, AUDIO_DEVICE_ADD);
    EXPECT_EQ(event.deviceType, AUDIO_HEADSET);
    EXPECT_EQ(event.name, "Headset");
    EXPECT_EQ(event.address, "HeadsetDevName");
}
