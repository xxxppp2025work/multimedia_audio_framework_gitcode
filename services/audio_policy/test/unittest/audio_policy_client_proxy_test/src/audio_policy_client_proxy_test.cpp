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

#ifndef LOG_TAG
#define LOG_TAG "AudioPolicyClientProxyTest"
#endif

#include "audio_policy_log.h"
#include "audio_policy_client_proxy_test.h"
#include "audio_policy_client_proxy.h"
#include "audio_policy_client_proxy_mock.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

void AudioPolicyClientProxyTest::SetUpTestCase(void) {}
void AudioPolicyClientProxyTest::TearDownTestCase(void) {}
void AudioPolicyClientProxyTest::SetUp(void) {}
void AudioPolicyClientProxyTest::TearDown(void) {}

/**
* @tc.name  : Test AudioPolicyClientProxyTest.
* @tc.number: AudioPolicyClientProxyTest_001
* @tc.desc  : Test OnVolumeKeyEvent/OnRingerModeUpdated.
*/
HWTEST(AudioPolicyClientProxyTest, AudioPolicyClientProxyTest_001, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_001 Enter.");
    sptr<AudioPolicyClientStubMockOne> audioPolicyClientStubMock = new AudioPolicyClientStubMockOne();
    ASSERT_NE(audioPolicyClientStubMock, nullptr);
    sptr<IRemoteObject> mockObject = audioPolicyClientStubMock->AsObject();
    ASSERT_NE(mockObject, nullptr);
    sptr<AudioPolicyClientStubMockTwo> audioPolicyClientStubMockTwo = new AudioPolicyClientStubMockTwo();
    ASSERT_NE(audioPolicyClientStubMockTwo, nullptr);
    sptr<IRemoteObject> mockObjectTwo = audioPolicyClientStubMockTwo->AsObject();
    ASSERT_NE(mockObjectTwo, nullptr);

    VolumeEvent volumeEvent;

    sptr<AudioPolicyClientProxy> callback = new AudioPolicyClientProxy(mockObject);
    ASSERT_NE(callback, nullptr);
    callback->OnVolumeKeyEvent(volumeEvent);
    callback->OnRingerModeUpdated(AudioRingerMode::RINGER_MODE_NORMAL);

    sptr<AudioPolicyClientProxy> callbackTwo = new AudioPolicyClientProxy(mockObjectTwo);
    ASSERT_NE(callbackTwo, nullptr);
    callbackTwo->OnVolumeKeyEvent(volumeEvent);
    callbackTwo->OnRingerModeUpdated(AudioRingerMode::RINGER_MODE_NORMAL);

    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_001 Leave");
}

/**
* @tc.name  : Test AudioPolicyClientProxyTest.
* @tc.number: AudioPolicyClientProxyTest_002
* @tc.desc  : Test OnAudioFocusInfoChange/OnAudioFocusRequested/OnAudioFocusAbandoned.
*/
HWTEST(AudioPolicyClientProxyTest, AudioPolicyClientProxyTest_002, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_002 Enter.");
    sptr<AudioPolicyClientStubMockOne> audioPolicyClientStubMock = new AudioPolicyClientStubMockOne();
    ASSERT_NE(audioPolicyClientStubMock, nullptr);
    sptr<IRemoteObject> mockObject = audioPolicyClientStubMock->AsObject();
    ASSERT_NE(mockObject, nullptr);
    sptr<AudioPolicyClientStubMockTwo> audioPolicyClientStubMockTwo = new AudioPolicyClientStubMockTwo();
    ASSERT_NE(audioPolicyClientStubMockTwo, nullptr);
    sptr<IRemoteObject> mockObjectTwo = audioPolicyClientStubMockTwo->AsObject();
    ASSERT_NE(mockObjectTwo, nullptr);

    AudioInterrupt audioInterrupt;
    std::list<std::pair<AudioInterrupt, AudioFocuState>> focusInfoList;
    focusInfoList.emplace_back(audioInterrupt, AudioFocuState::PAUSE);

    sptr<AudioPolicyClientProxy> callback = new AudioPolicyClientProxy(mockObject);
    ASSERT_NE(callback, nullptr);
    callback->OnAudioFocusInfoChange(focusInfoList);
    callback->OnAudioFocusRequested(audioInterrupt);
    callback->OnAudioFocusAbandoned(audioInterrupt);

    sptr<AudioPolicyClientProxy> callbackTwo = new AudioPolicyClientProxy(mockObjectTwo);
    ASSERT_NE(callbackTwo, nullptr);
    callbackTwo->OnAudioFocusInfoChange(focusInfoList);
    callbackTwo->OnAudioFocusRequested(audioInterrupt);
    callbackTwo->OnAudioFocusAbandoned(audioInterrupt);

    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_002 Leave.");
}

/**
* @tc.name  : Test AudioPolicyClientProxyTest.
* @tc.number: AudioPolicyClientProxyTest_003
* @tc.desc  : Test OnDeviceChange/OnMicrophoneBlocked/OnMicStateUpdated.
*/
HWTEST(AudioPolicyClientProxyTest, AudioPolicyClientProxyTest_003, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_003 Enter.");
    sptr<AudioPolicyClientStubMockOne> audioPolicyClientStubMock = new AudioPolicyClientStubMockOne();
    ASSERT_NE(audioPolicyClientStubMock, nullptr);
    sptr<IRemoteObject> mockObject = audioPolicyClientStubMock->AsObject();
    ASSERT_NE(mockObject, nullptr);
    sptr<AudioPolicyClientStubMockTwo> audioPolicyClientStubMockTwo = new AudioPolicyClientStubMockTwo();
    ASSERT_NE(audioPolicyClientStubMockTwo, nullptr);
    sptr<IRemoteObject> mockObjectTwo = audioPolicyClientStubMockTwo->AsObject();
    ASSERT_NE(mockObjectTwo, nullptr);

    auto descriptor = std::make_shared<AudioDeviceDescriptor>();
    DeviceChangeAction deviceChangeAction;
    deviceChangeAction.deviceDescriptors.push_back(descriptor);
    MicrophoneBlockedInfo microphoneBlockedInfo;
    microphoneBlockedInfo.devices.push_back(descriptor);
    MicStateChangeEvent micStateChangeEvent;

    sptr<AudioPolicyClientProxy> callback = new AudioPolicyClientProxy(mockObject);
    ASSERT_NE(callback, nullptr);
    callback->OnDeviceChange(deviceChangeAction);
    callback->OnMicrophoneBlocked(microphoneBlockedInfo);
    callback->OnMicStateUpdated(micStateChangeEvent);

    sptr<AudioPolicyClientProxy> callbackTwo = new AudioPolicyClientProxy(mockObjectTwo);
    ASSERT_NE(callbackTwo, nullptr);
    callbackTwo->OnDeviceChange(deviceChangeAction);
    callbackTwo->OnMicrophoneBlocked(microphoneBlockedInfo);
    callbackTwo->OnMicStateUpdated(micStateChangeEvent);

    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_003 Leave.");
}

/**
* @tc.name  : Test AudioPolicyClientProxyTest.
* @tc.number: AudioPolicyClientProxyTest_004
* @tc.desc  : Test OnPreferredOutputDeviceUpdated/OnPreferredInputDeviceUpdated.
*/
HWTEST(AudioPolicyClientProxyTest, AudioPolicyClientProxyTest_004, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_004 Enter.");
    sptr<AudioPolicyClientStubMockOne> audioPolicyClientStubMock = new AudioPolicyClientStubMockOne();
    ASSERT_NE(audioPolicyClientStubMock, nullptr);
    sptr<IRemoteObject> mockObject = audioPolicyClientStubMock->AsObject();
    ASSERT_NE(mockObject, nullptr);
    sptr<AudioPolicyClientStubMockTwo> audioPolicyClientStubMockTwo = new AudioPolicyClientStubMockTwo();
    ASSERT_NE(audioPolicyClientStubMockTwo, nullptr);
    sptr<IRemoteObject> mockObjectTwo = audioPolicyClientStubMockTwo->AsObject();
    ASSERT_NE(mockObjectTwo, nullptr);

    auto descriptor = std::make_shared<AudioDeviceDescriptor>();
    ASSERT_NE(descriptor, nullptr);
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> desc;
    desc.push_back(descriptor);
    AudioRendererInfo rendererInfo;
    AudioCapturerInfo capturerInfo;

    sptr<AudioPolicyClientProxy> callback = new AudioPolicyClientProxy(mockObject);
    ASSERT_NE(callback, nullptr);
    callback->OnPreferredOutputDeviceUpdated(rendererInfo, desc);
    callback->OnPreferredInputDeviceUpdated(capturerInfo, desc);

    sptr<AudioPolicyClientProxy> callbackTwo = new AudioPolicyClientProxy(mockObjectTwo);
    ASSERT_NE(callbackTwo, nullptr);
    callbackTwo->OnPreferredOutputDeviceUpdated(rendererInfo, desc);
    callbackTwo->OnPreferredInputDeviceUpdated(capturerInfo, desc);

    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_004 Leave.");
}

/**
* @tc.name  : Test AudioPolicyClientProxyTest.
* @tc.number: AudioPolicyClientProxyTest_005
* @tc.desc  : Test OnRendererStateChange/OnCapturerStateChange.
*/
HWTEST(AudioPolicyClientProxyTest, AudioPolicyClientProxyTest_005, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_005 Enter.");
    sptr<AudioPolicyClientStubMockOne> audioPolicyClientStubMock = new AudioPolicyClientStubMockOne();
    ASSERT_NE(audioPolicyClientStubMock, nullptr);
    sptr<IRemoteObject> mockObject = audioPolicyClientStubMock->AsObject();
    ASSERT_NE(mockObject, nullptr);
    sptr<AudioPolicyClientStubMockTwo> audioPolicyClientStubMockTwo = new AudioPolicyClientStubMockTwo();
    ASSERT_NE(audioPolicyClientStubMockTwo, nullptr);
    sptr<IRemoteObject> mockObjectTwo = audioPolicyClientStubMockTwo->AsObject();
    ASSERT_NE(mockObjectTwo, nullptr);

    auto rendererChangeInfo = std::make_shared<AudioRendererChangeInfo>();
    ASSERT_NE(rendererChangeInfo, nullptr);
    std::vector<std::shared_ptr<AudioRendererChangeInfo>> audioRendererChangeInfos;
    audioRendererChangeInfos.emplace_back(rendererChangeInfo);
    audioRendererChangeInfos.emplace_back(nullptr);
    auto capturerChangeInfo = std::make_shared<AudioCapturerChangeInfo>();
    ASSERT_NE(capturerChangeInfo, nullptr);
    std::vector<std::shared_ptr<AudioCapturerChangeInfo>> audioCapturerChangeInfos;
    audioCapturerChangeInfos.emplace_back(capturerChangeInfo);
    audioCapturerChangeInfos.emplace_back(nullptr);

    sptr<AudioPolicyClientProxy> callback = new AudioPolicyClientProxy(mockObject);
    ASSERT_NE(callback, nullptr);
    callback->OnRendererStateChange(audioRendererChangeInfos);
    callback->OnCapturerStateChange(audioCapturerChangeInfos);

    sptr<AudioPolicyClientProxy> callbackTwo = new AudioPolicyClientProxy(mockObjectTwo);
    ASSERT_NE(callbackTwo, nullptr);
    callbackTwo->OnRendererStateChange(audioRendererChangeInfos);
    callbackTwo->OnCapturerStateChange(audioCapturerChangeInfos);

    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_005 Leave.");
}

/**
* @tc.name  : Test AudioPolicyClientProxyTest.
* @tc.number: AudioPolicyClientProxyTest_006
* @tc.desc  : Test OnRendererDeviceChange/OnRecreateRendererStreamEvent/OnRecreateCapturerStreamEvent.
*/
HWTEST(AudioPolicyClientProxyTest, AudioPolicyClientProxyTest_006, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_006 Enter.");
    sptr<AudioPolicyClientStubMockOne> audioPolicyClientStubMock = new AudioPolicyClientStubMockOne();
    ASSERT_NE(audioPolicyClientStubMock, nullptr);
    sptr<IRemoteObject> mockObject = audioPolicyClientStubMock->AsObject();
    ASSERT_NE(mockObject, nullptr);
    sptr<AudioPolicyClientStubMockTwo> audioPolicyClientStubMockTwo = new AudioPolicyClientStubMockTwo();
    ASSERT_NE(audioPolicyClientStubMockTwo, nullptr);
    sptr<IRemoteObject> mockObjectTwo = audioPolicyClientStubMockTwo->AsObject();
    ASSERT_NE(mockObjectTwo, nullptr);

    uint32_t sessionId = 1;
    int32_t streamFlag = 0;
    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    AudioStreamDeviceChangeReasonExt reason(AudioStreamDeviceChangeReason::NEW_DEVICE_AVAILABLE);

    sptr<AudioPolicyClientProxy> callback = new AudioPolicyClientProxy(mockObject);
    ASSERT_NE(callback, nullptr);
    callback->OnRendererDeviceChange(sessionId, deviceInfo, reason);
    callback->OnRecreateRendererStreamEvent(sessionId, streamFlag, reason);
    callback->OnRecreateCapturerStreamEvent(sessionId, streamFlag, reason);

    sptr<AudioPolicyClientProxy> callbackTwo = new AudioPolicyClientProxy(mockObjectTwo);
    ASSERT_NE(callbackTwo, nullptr);
    callbackTwo->OnRendererDeviceChange(sessionId, deviceInfo, reason);
    callbackTwo->OnRecreateRendererStreamEvent(sessionId, streamFlag, reason);
    callbackTwo->OnRecreateCapturerStreamEvent(sessionId, streamFlag, reason);

    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_006 Leave.");
}

/**
* @tc.name  : Test AudioPolicyClientProxyTest.
* @tc.number: AudioPolicyClientProxyTest_007
* @tc.desc  : Test OnHeadTrackingDeviceChange/OnSpatializationEnabledChange.
*/
HWTEST(AudioPolicyClientProxyTest, AudioPolicyClientProxyTest_007, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_007 Enter.");
    sptr<AudioPolicyClientStubMockOne> audioPolicyClientStubMock = new AudioPolicyClientStubMockOne();
    ASSERT_NE(audioPolicyClientStubMock, nullptr);
    sptr<IRemoteObject> mockObject = audioPolicyClientStubMock->AsObject();
    ASSERT_NE(mockObject, nullptr);
    sptr<AudioPolicyClientStubMockTwo> audioPolicyClientStubMockTwo = new AudioPolicyClientStubMockTwo();
    ASSERT_NE(audioPolicyClientStubMockTwo, nullptr);
    sptr<IRemoteObject> mockObjectTwo = audioPolicyClientStubMockTwo->AsObject();
    ASSERT_NE(mockObjectTwo, nullptr);

    std::unordered_map<std::string, bool> changeInfo;
    changeInfo["Unknown"] = false;

    sptr<AudioPolicyClientProxy> callback = new AudioPolicyClientProxy(mockObject);
    ASSERT_NE(callback, nullptr);
    callback->OnHeadTrackingDeviceChange(changeInfo);
    callback->hasSystemPermission_ = true;
    callback->OnSpatializationEnabledChange(true);
    callback->hasSystemPermission_ = false;
    callback->OnSpatializationEnabledChange(false);

    sptr<AudioPolicyClientProxy> callbackTwo = new AudioPolicyClientProxy(mockObjectTwo);
    ASSERT_NE(callbackTwo, nullptr);
    callbackTwo->OnHeadTrackingDeviceChange(changeInfo);
    callbackTwo->OnSpatializationEnabledChange(true);

    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_007 Leave.");
}

/**
* @tc.name  : Test AudioPolicyClientProxyTest.
* @tc.number: AudioPolicyClientProxyTest_008
* @tc.desc  : Test OnHeadTrackingDeviceChange/OnSpatializationEnabledChange.
*/
HWTEST(AudioPolicyClientProxyTest, AudioPolicyClientProxyTest_008, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_008 Enter.");
    sptr<AudioPolicyClientStubMockOne> audioPolicyClientStubMock = new AudioPolicyClientStubMockOne();
    ASSERT_NE(audioPolicyClientStubMock, nullptr);
    sptr<IRemoteObject> mockObject = audioPolicyClientStubMock->AsObject();
    ASSERT_NE(mockObject, nullptr);
    sptr<AudioPolicyClientStubMockTwo> audioPolicyClientStubMockTwo = new AudioPolicyClientStubMockTwo();
    ASSERT_NE(audioPolicyClientStubMockTwo, nullptr);
    sptr<IRemoteObject> mockObjectTwo = audioPolicyClientStubMockTwo->AsObject();
    ASSERT_NE(mockObjectTwo, nullptr);

    std::unordered_map<std::string, bool> changeInfo;
    changeInfo["Unknown"] = false;

    sptr<AudioPolicyClientProxy> callback = new AudioPolicyClientProxy(mockObject);
    ASSERT_NE(callback, nullptr);
    callback->OnHeadTrackingDeviceChange(changeInfo);
    callback->hasSystemPermission_ = true;
    callback->OnSpatializationEnabledChange(true);
    callback->hasSystemPermission_ = false;
    callback->OnSpatializationEnabledChange(false);

    sptr<AudioPolicyClientProxy> callbackTwo = new AudioPolicyClientProxy(mockObjectTwo);
    ASSERT_NE(callbackTwo, nullptr);
    callbackTwo->OnHeadTrackingDeviceChange(changeInfo);
    callbackTwo->OnSpatializationEnabledChange(true);

    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_008 Leave.");
}

/**
* @tc.name  : Test AudioPolicyClientProxyTest.
* @tc.number: AudioPolicyClientProxyTest_009
* @tc.desc  : Test OnSpatializationEnabledChangeForAnyDevice/OnHeadTrackingEnabledChange.
*/
HWTEST(AudioPolicyClientProxyTest, AudioPolicyClientProxyTest_009, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_009 Enter.");
    sptr<AudioPolicyClientStubMockOne> audioPolicyClientStubMock = new AudioPolicyClientStubMockOne();
    ASSERT_NE(audioPolicyClientStubMock, nullptr);
    sptr<IRemoteObject> mockObject = audioPolicyClientStubMock->AsObject();
    ASSERT_NE(mockObject, nullptr);
    sptr<AudioPolicyClientStubMockTwo> audioPolicyClientStubMockTwo = new AudioPolicyClientStubMockTwo();
    ASSERT_NE(audioPolicyClientStubMockTwo, nullptr);
    sptr<IRemoteObject> mockObjectTwo = audioPolicyClientStubMockTwo->AsObject();
    ASSERT_NE(mockObjectTwo, nullptr);

    auto descriptor = std::make_shared<AudioDeviceDescriptor>();
    ASSERT_NE(descriptor, nullptr);

    sptr<AudioPolicyClientProxy> callback = new AudioPolicyClientProxy(mockObject);
    ASSERT_NE(callback, nullptr);
    callback->hasSystemPermission_ = true;
    callback->OnSpatializationEnabledChangeForAnyDevice(descriptor, true);
    callback->OnHeadTrackingEnabledChange(true);
    callback->OnHeadTrackingEnabledChangeForAnyDevice(descriptor, true);
    callback->hasSystemPermission_ = false;
    callback->OnSpatializationEnabledChangeForAnyDevice(descriptor, false);
    callback->OnHeadTrackingEnabledChange(false);
    callback->OnHeadTrackingEnabledChangeForAnyDevice(descriptor, false);

    sptr<AudioPolicyClientProxy> callbackTwo = new AudioPolicyClientProxy(mockObjectTwo);
    ASSERT_NE(callbackTwo, nullptr);
    callbackTwo->OnSpatializationEnabledChangeForAnyDevice(descriptor, true);
    callbackTwo->OnHeadTrackingEnabledChange(true);
    callbackTwo->OnHeadTrackingEnabledChangeForAnyDevice(descriptor, true);

    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_009 Leave.");
}

/**
* @tc.name  : Test AudioPolicyClientProxyTest.
* @tc.number: AudioPolicyClientProxyTest_010
* @tc.desc  : Test OnNnStateChange/OnAudioSessionDeactive.
*/
HWTEST(AudioPolicyClientProxyTest, AudioPolicyClientProxyTest_010, TestSize.Level1)
{
    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_010 Enter.");
    sptr<AudioPolicyClientStubMockOne> audioPolicyClientStubMock = new AudioPolicyClientStubMockOne();
    ASSERT_NE(audioPolicyClientStubMock, nullptr);
    sptr<IRemoteObject> mockObject = audioPolicyClientStubMock->AsObject();
    ASSERT_NE(mockObject, nullptr);
    sptr<AudioPolicyClientStubMockTwo> audioPolicyClientStubMockTwo = new AudioPolicyClientStubMockTwo();
    ASSERT_NE(audioPolicyClientStubMockTwo, nullptr);
    sptr<IRemoteObject> mockObjectTwo = audioPolicyClientStubMockTwo->AsObject();
    ASSERT_NE(mockObjectTwo, nullptr);

    int32_t state = 0;
    AudioSessionDeactiveEvent deactiveEvent;
    deactiveEvent.deactiveReason = AudioSessionDeactiveReason::TIMEOUT;

    sptr<AudioPolicyClientProxy> callback = new AudioPolicyClientProxy(mockObject);
    ASSERT_NE(callback, nullptr);
    callback->OnNnStateChange(state);
    callback->OnAudioSessionDeactive(deactiveEvent);

    sptr<AudioPolicyClientProxy> callbackTwo = new AudioPolicyClientProxy(mockObjectTwo);
    ASSERT_NE(callbackTwo, nullptr);
    callbackTwo->OnNnStateChange(state);
    callbackTwo->OnAudioSessionDeactive(deactiveEvent);

    AUDIO_INFO_LOG("AudioPolicyClientProxyTest_010 Leave.");
}
} // namespace AudioStandard
} // namespace OHOS