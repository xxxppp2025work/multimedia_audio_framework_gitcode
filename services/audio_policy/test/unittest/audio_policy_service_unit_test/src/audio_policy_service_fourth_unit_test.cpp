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
#include "get_server_util.h"
#include "audio_policy_service_fourth_unit_test.h"
#include <thread>
#include <memory>
#include <vector>
using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {

void AudioPolicyServiceFourthUnitTest::SetUpTestCase(void) {}

void AudioPolicyServiceFourthUnitTest::TearDownTestCase(void) {}

void AudioPolicyServiceFourthUnitTest::SetUp(void) {}

void AudioPolicyServiceFourthUnitTest::TearDown(void) {}

class RemoteObjectTestStub : public IRemoteObject {
public:
    RemoteObjectTestStub() : IRemoteObject(u"IRemoteObject") {}
    int32_t GetObjectRefCount() { return 0; };
    int SendRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option) { return 0; };
    bool AddDeathRecipient(const sptr<DeathRecipient> &recipient) { return true; };
    bool RemoveDeathRecipient(const sptr<DeathRecipient> &recipient) { return true; };
    int Dump(int fd, const std::vector<std::u16string> &args) { return 0; };

    DECLARE_INTERFACE_DESCRIPTOR(u"RemoteObjectTestStub");
};

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: CaptureConcurrentCheck
* @tc.desc  : Test CaptureConcurrentCheck.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, CaptureConcurrentCheck, TestSize.Level1)
{
    uint32_t sessionId = 0;
    int32_t ret = AudioPolicyService::GetAudioPolicyService().CaptureConcurrentCheck(sessionId);
    EXPECT_EQ(ret, ERR_NULL_POINTER);
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: ClearAudioFocusBySessionID
* @tc.desc  : Test ClearAudioFocusBySessionID.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, ClearAudioFocusBySessionID, TestSize.Level1)
{
    uint32_t sessionId = 0;
    int32_t ret = AudioPolicyService::GetAudioPolicyService().ClearAudioFocusBySessionID(sessionId);
    EXPECT_EQ(ret, ERROR);
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: SetSleAudioOperationCallback
* @tc.desc  : Test SetSleAudioOperationCallback.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, SetSleAudioOperationCallback, TestSize.Level1)
{
    sptr<RemoteObjectTestStub> object = new RemoteObjectTestStub();
    int32_t ret = AudioPolicyService::GetAudioPolicyService().SetSleAudioOperationCallback(object);
    EXPECT_EQ(ret, SUCCESS);
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: SetDefaultAdapterEnable
* @tc.desc  : Test SetDefaultAdapterEnable.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, SetDefaultAdapterEnable, TestSize.Level1)
{
    bool isEnable = false;
    EXPECT_NO_THROW(
        AudioPolicyService::GetAudioPolicyService().SetDefaultAdapterEnable(isEnable);
    );
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: UnregisterBluetoothListener
* @tc.desc  : Test UnregisterBluetoothListener.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, UnregisterBluetoothListener, TestSize.Level1)
{
#ifdef BLUETOOTH_ENABLE
    EXPECT_NO_THROW(
        AudioPolicyService::GetAudioPolicyService().UnregisterBluetoothListener();
    );
#endif
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: OnServiceConnected
* @tc.desc  : Test OnServiceConnected.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, OnServiceConnected, TestSize.Level1)
{
    AudioServiceIndex serviceIndex = AudioServiceIndex::HDI_SERVICE_INDEX;
    EXPECT_NO_THROW(
        AudioPolicyService::GetAudioPolicyService().OnServiceConnected(serviceIndex);
    );
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: SetDefaultDeviceLoadFlag
* @tc.desc  : Test SetDefaultDeviceLoadFlag.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, SetDefaultDeviceLoadFlag, TestSize.Level1)
{
    bool isLoad = false;
    EXPECT_NO_THROW(
        AudioPolicyService::GetAudioPolicyService().SetDefaultDeviceLoadFlag(isLoad);
    );
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: RestoreSession
* @tc.desc  : Test RestoreSession.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, RestoreSession, TestSize.Level1)
{
    uint32_t sessionId = 0;
    RestoreInfo restoreInfo = {};
    EXPECT_NO_THROW(
        AudioPolicyService::GetAudioPolicyService().RestoreSession(sessionId, restoreInfo);
    );
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: UpdateSpatializationSupported
* @tc.desc  : Test UpdateSpatializationSupported.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, UpdateSpatializationSupported, TestSize.Level1)
{
    std::string macAddress = "02:7f:3a:9d:1c:5b";
    bool support = false;
    EXPECT_NO_THROW(
        AudioPolicyService::GetAudioPolicyService().UpdateSpatializationSupported(macAddress, support);
    );
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: SubscribeSafeVolumeEvent
* @tc.desc  : Test SubscribeSafeVolumeEvent.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, SubscribeSafeVolumeEvent, TestSize.Level1)
{
    EXPECT_NO_THROW(
        AudioPolicyService::GetAudioPolicyService().SubscribeSafeVolumeEvent();
    );
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: getFastControlParam
* @tc.desc  : Test getFastControlParam.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, getFastControlParam, TestSize.Level1)
{
    bool ret = AudioPolicyService::GetAudioPolicyService().getFastControlParam();
    EXPECT_EQ(ret, true);
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: NotifyAccountsChanged
* @tc.desc  : Test NotifyAccountsChanged.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, NotifyAccountsChanged, TestSize.Level1)
{
    int id = 1;
    EXPECT_NO_THROW(
        AudioPolicyService::GetAudioPolicyService().NotifyAccountsChanged(id);
    );
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: OnDeviceInfoUpdated
* @tc.desc  : Test OnDeviceInfoUpdated.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, OnDeviceInfoUpdated, TestSize.Level1)
{
    AudioDeviceDescriptor desc = AudioDeviceDescriptor();
    DeviceInfoUpdateCommand command = DeviceInfoUpdateCommand::CATEGORY_UPDATE;
    EXPECT_NO_THROW(
        AudioPolicyService::GetAudioPolicyService().OnDeviceInfoUpdated(desc, command);
    );
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: GetDistributedRoutingRoleInfo
* @tc.desc  : Test GetDistributedRoutingRoleInfo.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, GetDistributedRoutingRoleInfo, TestSize.Level1)
{
    DistributedRoutingInfo ret = AudioPolicyService::GetAudioPolicyService().GetDistributedRoutingRoleInfo();
    EXPECT_EQ(ret.descriptor, nullptr);
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: GetAndSaveClientType
* @tc.desc  : Test GetAndSaveClientType.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, GetAndSaveClientType, TestSize.Level1)
{
    uint32_t uid = 1;
    std::string bundleName = "test";
    int32_t ret = AudioPolicyService::GetAudioPolicyService().GetAndSaveClientType(uid, bundleName);
    EXPECT_EQ(ret, SUCCESS);
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: UpdateDescWhenNoBTPermission
* @tc.desc  : Test UpdateDescWhenNoBTPermission.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, UpdateDescWhenNoBTPermission, TestSize.Level1)
{
    vector<std::shared_ptr<AudioDeviceDescriptor>> descs;
    std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor = std::make_shared<AudioDeviceDescriptor>();
    audioDeviceDescriptor->deviceType_ = DEVICE_TYPE_BLUETOOTH_A2DP;
    descs.push_back(std::move(audioDeviceDescriptor));
    EXPECT_NO_THROW(
        AudioPolicyService::GetAudioPolicyService().UpdateDescWhenNoBTPermission(descs);
    );
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: RegisterDataObserver
* @tc.desc  : Test RegisterDataObserver.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, RegisterDataObserver, TestSize.Level1)
{
    EXPECT_NO_THROW(
        AudioPolicyService::GetAudioPolicyService().RegisterDataObserver();
    );
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: RegisterRemoteDevStatusCallback
* @tc.desc  : Test RegisterRemoteDevStatusCallback.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, RegisterRemoteDevStatusCallback, TestSize.Level1)
{
#ifdef FEATURE_DEVICE_MANAGER
    auto server = GetServerUtil::GetServerPtr();
    EXPECT_NO_THROW(
        AudioPolicyService::GetAudioPolicyService().RegisterRemoteDevStatusCallback();
    );
#endif
}

/**
* @tc.name  : Test AudioPolicyService.
* @tc.number: SubscribeAccessibilityConfigObserver
* @tc.desc  : Test SubscribeAccessibilityConfigObserver.
*/
HWTEST_F(AudioPolicyServiceFourthUnitTest, SubscribeAccessibilityConfigObserver, TestSize.Level1)
{
#ifdef ACCESSIBILITY_ENABLE
    EXPECT_NO_THROW(
        AudioPolicyService::GetAudioPolicyService().SubscribeAccessibilityConfigObserver();
    );
#endif
}

} // namespace AudioStandard
} // namespace OHOS