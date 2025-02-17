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

#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "audio_utils.h"
#include "common/hdi_adapter_info.h"
#include "manager/hdi_adapter_manager.h"

using namespace testing::ext;

namespace OHOS {
namespace AudioStandard {
class ManagerUnitTest : public testing::Test {
public:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

/**
 * @tc.name   : Test Manager API
 * @tc.number : ManagerUnitTest_001
 * @tc.desc   : Test manager action
 */
HWTEST_F(ManagerUnitTest, ManagerUnitTest_001, TestSize.Level1)
{
    HdiAdapterManager &manager = HdiAdapterManager::GetInstance();
    uint32_t id = manager.GetId(10, 0);
    EXPECT_EQ(id, HDI_INVALID_ID);

    id = manager.GetRenderIdByDeviceClass("");
    EXPECT_EQ(id, HDI_INVALID_ID);

    id = manager.GetCaptureIdByDeviceClass("", SOURCE_TYPE_MIC);
    EXPECT_EQ(id, HDI_INVALID_ID);

    manager.ReleaseId(id);

    std::shared_ptr<IAudioRenderSink> sink = manager.GetRenderSink(id);
    EXPECT_EQ(sink, nullptr);

    std::shared_ptr<IAudioCaptureSource> source = manager.GetCaptureSource(id);
    EXPECT_EQ(source, nullptr);

    std::function<bool(uint32_t)> limitFunc = [](uint32_t id) -> bool { return false; };
    std::function<int32_t(std::shared_ptr<IAudioCaptureSource>)> processFunc =
        [](std::shared_ptr<IAudioCaptureSource> source) -> bool { return SUCCESS; };
    auto ret = manager.ProcessCaptureSource(limitFunc, processFunc);
    EXPECT_EQ(ret, SUCCESS);

    std::shared_ptr<IDeviceManager> deviceManager = manager.GetDeviceManager(HDI_DEVICE_MANAGER_TYPE_LOCAL);
    EXPECT_NE(deviceManager, nullptr);

    manager.ReleaseDeviceManager(10); // 10: test
}

} // namespace AudioStandard
} // namespace OHOS
