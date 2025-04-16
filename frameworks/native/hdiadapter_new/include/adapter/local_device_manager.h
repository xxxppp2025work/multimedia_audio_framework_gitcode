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

#ifndef LOCAL_DEVICE_MANAGER_H
#define LOCAL_DEVICE_MANAGER_H

#include <iostream>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <mutex>
#include "v4_0/iaudio_manager.h"
#include "hdf_remote_service.h"
#include "adapter/i_device_manager.h"
#include "adapter/i_device_adapter.h"

namespace OHOS {
namespace AudioStandard {
typedef struct LocalParameter {
    std::string adapterName_ = "";
    AudioParamKey key_ = AudioParamKey::NONE;
    std::string condition_ = "";
    std::string value_ = "";
} LocalParameter;

class LocalDeviceManager : public IDeviceManager {
public:
    LocalDeviceManager() = default;
    ~LocalDeviceManager() = default;

    std::shared_ptr<IDeviceAdapter> LoadAdapter(const std::string &adapterName, bool needReInitManager) override;
    void UnloadAdapter(std::shared_ptr<IDeviceAdapter> deviceAdapter) override;

private:
    void SaveSetParameter(const std::string &adapterName, const AudioParamKey key, const std::string &condition,
        const std::string &value);

    void InitAudioManager(void);
    int32_t SwitchAdapterDesc(struct AudioAdapterDescriptor *descs, const std::string &adapterName, uint32_t size);

private:
    static constexpr uint32_t MAX_AUDIO_ADAPTER_NUM = 5;
    static constexpr uid_t UID_BLUETOOTH_SA = 1002;

    struct IAudioManager *audioManager_ = nullptr;
    struct HdfRemoteService *hdfRemoteService_ = nullptr;
    struct HdfDeathRecipient *hdfDeathRecipient_ = nullptr;
    std::vector<LocalParameter> reSetParams_;
    uint16_t dmDeviceType_ = 0;
};

} // namespace AudioStandard
} // namespace OHOS

#endif // LOCAL_DEVICE_MANAGER_H
