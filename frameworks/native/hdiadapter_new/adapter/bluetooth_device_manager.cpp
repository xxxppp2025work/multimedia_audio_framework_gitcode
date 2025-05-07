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
#define LOG_TAG "BluetoothDeviceManager"
#endif

#include "adapter/bluetooth_device_manager.h"
#include <dlfcn.h>
#include "audio_hdi_log.h"
#include "audio_errors.h"
#include "audio_utils.h"
#include "adapter/bluetooth_device_adapter.h"

using namespace OHOS::HDI::Audio_Bluetooth;

namespace OHOS {
namespace AudioStandard {
BluetoothDeviceManager::~BluetoothDeviceManager()
{
    if (handle_ != nullptr) {
#ifndef TEST_COVERAGE
        dlclose(handle_);
#endif
        handle_ = nullptr;
    }
}

std::shared_ptr<IDeviceAdapter> BluetoothDeviceManager::LoadAdapter(const std::string &adapterName,
    bool needReInitManager)
{
    if (audioManager_ == nullptr || needReInitManager) {
        audioManager_ = nullptr;
        InitAudioManager();
    }
    CHECK_AND_RETURN_RET(audioManager_ != nullptr, nullptr);

    struct AudioAdapterDescriptor *descs = nullptr;
    int32_t size = 0;
    int32_t ret = audioManager_->GetAllAdapters(audioManager_, &descs, &size);
    CHECK_AND_RETURN_RET_LOG(size <= (int32_t)MAX_AUDIO_ADAPTER_NUM && size != 0 && ret == SUCCESS && descs != nullptr,
        nullptr, "get adapters fail");
    int32_t index = SwitchAdapterDesc(descs, adapterName, size);
    CHECK_AND_RETURN_RET(index >= 0, nullptr);

    struct AudioAdapter *adapter = nullptr;
    ret = audioManager_->LoadAdapter(audioManager_, &(descs[index]), &adapter);
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS && adapter != nullptr, nullptr, "load adapter fail");
    ret = adapter->InitAllPorts(adapter);
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, nullptr, "init all ports fail");
    std::shared_ptr<IDeviceAdapter> deviceAdapter = std::make_shared<BluetoothDeviceAdapter>(adapterName, adapter,
        descs[index]);
    AUDIO_INFO_LOG("load adapter %{public}s success", adapterName.c_str());
    return deviceAdapter;
}

void BluetoothDeviceManager::UnloadAdapter(std::shared_ptr<IDeviceAdapter> deviceAdapter)
{
    CHECK_AND_RETURN_LOG(deviceAdapter != nullptr &&
        deviceAdapter->GetDeviceManagerType() == HDI_DEVICE_MANAGER_TYPE_BLUETOOTH, "invalid adapter");
    CHECK_AND_RETURN_LOG(audioManager_ != nullptr, "audio manager is nullptr");

    auto btDeviceAdapter = st::dynamic_pointer_cast<BluetoothDeviceAdapter>(deviceAdapter);
    CHECK_AND_RETURN_LOG(btDeviceAdapter != nullptr &&
        btDeviceAdapter->adapter_ != nullptr, "adapter is nullptr");
    audioManager_->UnloadAdapter(audioManager_, btDeviceAdapter->adapter_);
    AUDIO_INFO_LOG("unload adapter %{public}s success", adapterName.c_str());
}

void BluetoothDeviceManager::InitAudioManager(void)
{
    CHECK_AND_RETURN_LOG(audioManager_ == nullptr, "audio manager already inited");
    AUDIO_INFO_LOG("init audio manager");

#if (defined(__aarch64__) || defined(__x86_64__))
    char resolvedPath[] = "/vendor/lib64/chipsetsdk/libaudio_bluetooth_hdi_proxy_server.z.so";
#else
    char resolvedPath[] = "/vendor/lib/chipsetsdk/libaudio_bluetooth_hdi_proxy_server.z.so";
#endif
    handle_ = dlopen(resolvedPath, RTLD_LAZY);
    CHECK_AND_RETURN_LOG(handle_ != nullptr, "dlopen %{public}s fail", resolvedPath);
    struct AudioProxyManager *(*getAudioManager)() = nullptr;
    getAudioManager = (struct AudioProxyManager *(*)())(dlsym(handle_, "GetAudioProxyManagerFuncs"));
    CHECK_AND_RETURN_LOG(getAudioManager != nullptr, "dlsym fail");
    audioManager_ = getAudioManager();
    CHECK_AND_RETURN_LOG(audioManager_ != nullptr, "get audio manager fail");
    AUDIO_INFO_LOG("init audio manager succ");
}

int32_t BluetoothDeviceManager::SwitchAdapterDesc(struct AudioAdapterDescriptor *descs, const std::string &adapterName,
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
