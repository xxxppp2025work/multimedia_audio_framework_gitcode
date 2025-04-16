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
#define LOG_TAG "BluetoothDeviceAdapter"
#endif

#include "adapter/bluetooth_device_adapter.h"
#include "audio_hdi_log.h"
#include "audio_errors.h"
#include "audio_utils.h"

using namespace OHOS::HDI::Audio_Bluetooth;

namespace OHOS {
namespace AudioStandard {
BluetoothDeviceAdapter::BluetoothDeviceAdapter(const std::string &adapterName, AudioAdapter *adapter,
    AudioAdapterDescriptor adapterDesc)
    : adapterName_(adapterName), adapter_(adapter), adapterDesc_(adapterDesc)
{
}

HdiDeviceManagerType BluetoothDeviceAdapter::GetDeviceManagerType(void)
{
    return HDI_DEVICE_MANAGER_TYPE_BLUETOOTH;
}

bool BluetoothDeviceAdapter::CheckStatus(void)
{
    if (renders_.size() != 0 || captures_.size() != 0) {
        AUDIO_INFO_LOG("adapter %{public}s has some ports busy, renderNum: %{public}zu, captureNum: %{public}zu",
            adapterName_.c_str(), renders_.size(), captures_.size());
        return false;
    }
    return true;
}

void BluetoothDeviceAdapter::SetAudioParameter(const AudioParamKey key, const std::string &condition,
    const std::string &value)
{
    AUDIO_INFO_LOG("not support");
}

std::string BluetoothDeviceAdapter::GetAudioParameter(const AudioParamKey key, const std::string &condition)
{
    AUDIO_INFO_LOG("key: %{public}d, condition: %{public}s", key, condition.c_str());

    CHECK_AND_RETURN_RET_LOG(adapter_ != nullptr, "", "adapter %{public}s is nullptr", adapterName_.c_str());
    AudioExtParamKey hdiKey = AudioExtParamKey(key);
    char value[DumpFileUtil::PARAM_VALUE_LENTH];
    int32_t ret = adapter_->GetExtraParams(adapter_, hdiKey, condition.c_str(), value, DumpFileUtil::PARAM_VALUE_LENTH);
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, "", "get param fail, error code: %{public}d", ret);
    return value;
}

int32_t BluetoothDeviceAdapter::SetOutputRoute(const std::vector<DeviceType> &devices, int32_t streamId)
{
    AUDIO_INFO_LOG("not support");
    return ERR_NOT_SUPPORTED;
}

int32_t BluetoothDeviceAdapter::SetInputRoute(DeviceType device, int32_t streamId, int32_t inputType)
{
    AUDIO_INFO_LOG("not support");
    return ERR_NOT_SUPPORTED;
}

int32_t BluetoothDeviceAdapter::SetVoiceVolume(float volume)
{
    AUDIO_INFO_LOG("not support");
    return ERR_NOT_SUPPORTED;
}

void BluetoothDeviceAdapter::SetMicMute(bool isMute)
{
    AUDIO_INFO_LOG("not support");
}

void BluetoothDeviceAdapter::SetDmDeviceType(uint16_t dmDeviceType)
{
    AUDIO_INFO_LOG("not support");
}

void BluetoothDeviceAdapter::DumpInfo(std::string &dumpString)
{
    uint32_t renderNum = renders_.size();
    uint32_t captureNum = captures_.size();
    dumpString += "  - bt/" + adapterName_ + "\trenderNum: " + std::to_string(renderNum) + "\tcaptureNum: " +
        std::to_string(captureNum) + "\n";
}

void *BluetoothDeviceAdapter::CreateRender(void *param, void *deviceDesc, uint32_t &hdiRenderId)
{
    CHECK_AND_RETURN_RET_LOG(param != nullptr && deviceDesc != nullptr, nullptr, "param or deviceDesc is nullptr");
    struct AudioSampleAttributes *bluetoothParam = static_cast<struct AudioSampleAttributes *>(param);
    struct AudioDeviceDescriptor *bluetoothDeviceDesc = static_cast<struct AudioDeviceDescriptor *>(deviceDesc);

    CHECK_AND_RETURN_RET_LOG(adapter_ != nullptr, nullptr, "adapter %{public}s is nullptr", adapterName_.c_str());
    bluetoothDeviceDesc->portId = GetPortId(adapterName_, PORT_OUT);

    struct AudioRender *render = nullptr;
    int32_t ret = adapter_->CreateRender(adapter_, bluetoothDeviceDesc, bluetoothParam, &render);
    if (ret != SUCCESS || render == nullptr) {
        AUDIO_ERR_LOG("create render fail");
        return nullptr;
    }
    AUDIO_INFO_LOG("create render success, desc: %{public}s", bluetoothDeviceDesc->desc);

    std::lock_guard<std::mutex> lock(renderMtx_);
    hdiRenderId = GetHdiRenderId(adapterName_);
    renders_[hdiRenderId] = render;
    return render;
}

void BluetoothDeviceAdapter::DestroyRender(uint32_t hdiRenderId)
{
    AUDIO_INFO_LOG("destroy render, hdiRenderId: %{public}u", hdiRenderId);

    CHECK_AND_RETURN_LOG(adapter_ != nullptr, "adapter %{public}s is nullptr", adapterName_.c_str());
    CHECK_AND_RETURN_LOG(renders_.count(hdiRenderId) != 0, "render not exist");
    std::lock_guard<std::mutex> lock(renderMtx_);
    adapter_->DestroyRender(adapter_, renders_[hdiRenderId]);
    renders_.erase(hdiRenderId);
    freeHdiRenderIdSet_.insert(hdiRenderId);
}

void *BluetoothDeviceAdapter::CreateCapture(void *param, void *deviceDesc, uint32_t &hdiCaptureId)
{
    CHECK_AND_RETURN_RET_LOG(param != nullptr && deviceDesc != nullptr, nullptr, "param or deviceDesc is nullptr");
    struct AudioSampleAttributes *bluetoothParam = static_cast<struct AudioSampleAttributes *>(param);
    struct AudioDeviceDescriptor *bluetoothDeviceDesc = static_cast<struct AudioDeviceDescriptor *>(deviceDesc);

    CHECK_AND_RETURN_RET_LOG(adapter_ != nullptr, nullptr, "adapter %{public}s is nullptr", adapterName_.c_str());
    bluetoothDeviceDesc->portId = GetPortId(adapterName_, PORT_IN);

    struct AudioCapture *capture = nullptr;
    int32_t ret = adapter_->CreateCapture(adapter_, bluetoothDeviceDesc, bluetoothParam, &capture);
    if (ret != SUCCESS || capture == nullptr) {
        AUDIO_ERR_LOG("create capture fail");
        return nullptr;
    }
    AUDIO_INFO_LOG("create capture success, desc: %{public}s", bluetoothDeviceDesc->desc);

    std::lock_guard<std::mutex> lock(captureMtx_);
    hdiCaptureId = GetHdiCaptureId(adapterName_);
    captures_[hdiCaptureId] = capture;
    return capture;
}

void BluetoothDeviceAdapter::DestroyCapture(uint32_t hdiCaptureId)
{
    AUDIO_INFO_LOG("destroy capture, hdiCaptureId: %{public}u", hdiCaptureId);

    CHECK_AND_RETURN_LOG(adapter_ != nullptr, "adapter %{public}s is nullptr", adapterName_.c_str());
    CHECK_AND_RETURN_LOG(captures_.count(hdiCaptureId) != 0, "capture not exist");
    std::lock_guard<std::mutex> lock(captureMtx_);
    adapter_->DestroyCapture(adapter_, captures_[hdiCaptureId]);
    captures_.erase(hdiCaptureId);
    freeHdiCaptureIdSet_.insert(hdiCaptureId);
}

uint32_t BluetoothDeviceAdapter::GetPortId(enum AudioPortDirection portFlag)
{
    CHECK_AND_RETURN_RET_LOG(adapter_ != nullptr, 0, "adapter %{public}s is nullptr", adapterName_.c_str());
    uint32_t portId = 0;
    for (uint32_t port = 0; port < adapterDesc_.portNum; ++port) {
        if (adapterDesc_.ports[port].dir == portFlag) {
            portId = adapterDesc_.ports[port].portId;
            break;
        }
    }
    AUDIO_DEBUG_LOG("portId: %{public}u", portId);
    return portId;
}

uint32_t BluetoothDeviceAdapter::GetHdiRenderId(void)
{
    if (freeHdiRenderIdSet_.empty()) {
        return renders_.size();
    }
    uint32_t hdiRenderId = *(freeHdiRenderIdSet_.begin());
    freeHdiRenderIdSet_.erase(hdiRenderId);
    return hdiRenderId;
}

uint32_t BluetoothDeviceAdapter::GetHdiCaptureId(const std::string &adapterName)
{
    if (freeHdiCaptureIdSet_.empty()) {
        return captures_.size();
    }
    uint32_t hdiCaptureId = *(freeHdiCaptureIdSet_.begin());
    freeHdiCaptureIdSet_.erase(hdiCaptureId);
    return hdiCaptureId;
}

} // namespace AudioStandard
} // namespace OHOS
