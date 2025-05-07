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

#ifndef LOG_TAG
#define LOG_TAG "LocalDeviceManager"
#endif

#include "adapter/local_device_manager.h"
#include "audio_hdi_log.h"
#include "audio_errors.h"
#include "audio_utils.h"
#include "ipc_skeleton.h"
#include "adapter/local_device_adapter.h"

namespace OHOS {
namespace AudioStandard {
std::shared_ptr<IDeviceAdapter> LocalDeviceManager::LoadAdapter(const std::string &adapterName, bool needReInitManager)
{
    if (audioManager_ == nullptr) {
        InitAudioManager();
    }
    CHECK_AND_RETURN_RET(audioManager_ != nullptr, nullptr);

    struct AudioAdapterDescriptor descs[MAX_AUDIO_ADAPTER_NUM];
    uint32_t size = MAX_AUDIO_ADAPTER_NUM;
    int32_t ret = audioManager_->GetAllAdapters(audioManager_, (struct AudioAdapterDescriptor *)&descs, &size);
    CHECK_AND_RETURN_RET_LOG(size <= MAX_AUDIO_ADAPTER_NUM && size != 0 && ret == SUCCESS, nullptr,
        "get adapters fail");
    int32_t index = SwitchAdapterDesc((struct AudioAdapterDescriptor *)&descs, adapterName, size);
    CHECK_AND_RETURN_RET(index >= 0, nullptr);

    struct IAudioAdapter *adapter = nullptr;
    ret = audioManager_->LoadAdapter(audioManager_, &(descs[index]), &adapter);
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS && adapter != nullptr, nullptr, "load adapter fail");
    ret = adapter->InitAllPorts(adapter);
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, nullptr, "init all ports fail");
    std::shared_ptr<IDeviceAdapter> deviceAdapter = std::make_shared<LocalDeviceAdapter>(adapterName, adapter,
        descs[index]);
    // LCOV_EXCL_START
    for (auto it = reSetParams_.begin(); it != reSetParams_.end();) {
        if (it->adapterName_ == adapterName) {
            deviceAdapter->SetAudioParameter(it->key_, it->condition_, it->value_);
            it = reSetParams_.erase(it);
            continue;
        }
        ++it;
    }
    // LCOV_EXCL_STOP
    AUDIO_INFO_LOG("load adapter %{public}s success", adapterName.c_str());
    return deviceAdapter;
}

void LocalDeviceManager::UnloadAdapter(std::shared_ptr<IDeviceAdapter> deviceAdapter)
{
    CHECK_AND_RETURN_LOG(deviceAdapter != nullptr &&
        deviceAdapter->GetDeviceManagerType() == HDI_DEVICE_MANAGER_TYPE_LOCAL, "invalid adapter");
    CHECK_AND_RETURN_LOG(audioManager_ != nullptr, "audio manager is nullptr");

    auto localDeviceAdapter = std::dynamic_pointer_cast<LocalDeviceAdapter>(deviceAdapter);
    CHECK_AND_RETURN_LOG(localDeviceAdapter != nullptr &&
        localDeviceAdapter->adapter_ != nullptr, "adapter is nullptr");
    if (localDeviceAdapter->routeHandle_ != -1) {
        localDeviceAdapter->adapter_->ReleaseAudioRoute(localDeviceAdapter->adapter_, localDeviceAdapter->routeHandle_);
    }
    audioManager_->UnloadAdapter(audioManager_, localDeviceAdapter->adapterDesc_.adapterName);
    AUDIO_INFO_LOG("unload adapter %{public}s success", adapterName.c_str());
}

void LocalDeviceManager::SaveSetParameter(const std::string &adapterName, const AudioParamKey key,
    const std::string &condition, const std::string &value)
{
    // save set param
    auto callerUid = IPCSkeleton::GetCallingUid();
    AUDIO_INFO_LOG("save param when adapter is nullptr, callerUid is %{public}u", callerUid);
    reSetParams_.push_back({ adapterName, key, condition, value });
}

static void AudioHostOnRemoteDied(struct HdfDeathRecipient *recipent, struct HdfRemoteService *service)
{
    CHECK_AND_RETURN_LOG(recipent != nullptr && service != nullptr, "receive die message but params are nullptr");
    AUDIO_ERR_LOG("auto exit for audio host die");
    _Exit(0);
}

void LocalDeviceManager::InitAudioManager(void)
{
    CHECK_AND_RETURN_LOG(audioManager_ == nullptr, "audio manager already inited");
    AUDIO_INFO_LOG("init audio manager");
    audioManager_ = IAudioManagerGet(false);
    CHECK_AND_RETURN_LOG(audioManager_ != nullptr, "get audio manager fail");

    CHECK_AND_RETURN_LOG(hdfRemoteService_ == nullptr, "hdf remote service already inited");
    hdfRemoteService_ = audioManager_->AsObject(audioManager_);
    // Don't need to free, existing with process
    hdfDeathRecipient_ = (struct HdfDeathRecipient *)calloc(1, sizeof(*hdfDeathRecipient_));
    CHECK_AND_RETURN_LOG(hdfDeathRecipient_ != nullptr, "create hdf death recipient fail");
    hdfDeathRecipient_->OnRemoteDied = AudioHostOnRemoteDied;
    HdfRemoteServiceAddDeathRecipient(hdfRemoteService_, hdfDeathRecipient_);

    AUDIO_INFO_LOG("init audio manager succ");
}

int32_t LocalDeviceManager::SwitchAdapterDesc(struct AudioAdapterDescriptor *descs, const std::string &adapterName,
    uint32_t size)
{
    CHECK_AND_RETURN_RET(descs != nullptr, ERROR);

    for (uint32_t index = 0; index < size; ++index) {
        struct AudioAdapterDescriptor *desc = &descs[index];
        if (desc == nullptr || desc->adapterName == nullptr) {
            continue;
        }
        AUDIO_DEBUG_LOG("index: %{public}u, adapterName: %{public}s", index, desc->adapterName);
        if (!strcmp(desc->adapterName, adapterName.c_str())) {
            AUDIO_INFO_LOG("match adapter %{public}s", desc->adapterName);
            return index;
        }
    }
    AUDIO_ERR_LOG("switch adapter fail, adapterName: %{public}s", adapterName.c_str());
    return ERR_INVALID_INDEX;
}

} // namespace AudioStandard
} // namespace OHOS
