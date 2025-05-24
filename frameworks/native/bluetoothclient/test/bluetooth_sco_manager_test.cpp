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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "audio_errors.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "audio_errors.h"
#include "bluetooth_sco_manager.h"
#include "bluetooth_hfp_mock_interface.h"

namespace OHOS {
namespace Bluetooth {

using namespace testing::ext;
using namespace testing;

class BluetoothScoManagerTest : public testing::Test {
public:
    void SetUp() {}
    void TearDown() {}
};

/**
 * @tc.name  : Test BluetoothScoManager.
 * @tc.number: BluetoothScoManagerTest_001
 * @tc.desc  : Test connect sco.
 */
HWTEST_F(BluetoothScoManagerTest, BluetoothScoManagerTest_001, TestSize.Level1)
{
    EXPECT_CALL(BluetoothHfpInterface::GetInstance(), ConnectSco())
        .Times(2)
        .WillOnce(Return(SUCCESS));
    
    BluetoothRemoteDevice device;
    BluetoothScoManager::GetInstance().HandleScoConnect(ScoCategory::SCO_VIRTUAL, device);
}
} // namespace Bluetooth
} // namespace OHOS