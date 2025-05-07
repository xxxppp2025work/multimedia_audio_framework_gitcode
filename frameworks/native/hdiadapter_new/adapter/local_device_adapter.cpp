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
#define LOG_TAG "LocalDeviceAdapter"
#endif

#include "adapter/local_device_adapter.h"
#include "audio_hdi_log.h"
#include "audio_errors.h"
#include "audio_utils.h"

namespace OHOS {
namespace AudioStandard {
LocalDeviceAdapter::LocalDeviceAdapter(const std::string &adapterName, struct IAudioAdapter *adapter,
    struct AudioAdapterDescriptor adapterDesc)
    : adapterName_(adapterName), adapter_(adapter), adapterDesc_(adapterDesc)
{
}

HdiDeviceManagerType LocalDeviceAdapter::GetDeviceManagerType(void)
{
    return HDI_DEVICE_MANAGER_TYPE_LOCAL;
}

bool LocalDeviceAdapter::CheckStatus(void)
{
    if (hdiRenderIds_.size() != 0 || hdiCaptureIds_.size() == 0) {
        AUDIO_INFO_LOG("adapter %{public}s has some ports busy, renderNum: %{public}zu, captureNum: %{public}zu",
            adapterName_.c_str(), hdiRenderIds_.size(), hdiCaptureIds_.size());
        return false;
    }
    return true;
}

void LocalDeviceAdapter::SetAudioParameter(const AudioParamKey key, const std::string &condition,
    const std::string &value)
{
    AUDIO_INFO_LOG("key: %{public}d, condition: %{public}s, value: %{public}s", key, condition.c_str(), value.c_str());
    CHECK_AND_RETURN_LOG(adapter_ != nullptr, "adapter %{public}s is nullptr", adapterName_.c_str());

    AudioExtParamKey hdiKey = AudioExtParamKey(key);
    int32_t ret = adapter_->SetExtraParams(adapter_, hdiKey, condition.c_str(), value.c_str());
    CHECK_AND_RETURN_LOG(ret == SUCCESS, "set param fail, error code: %{public}d", ret);
}

std::string LocalDeviceAdapter::GetAudioParameter(const AudioParamKey key, const std::string &condition)
{
    AUDIO_INFO_LOG("key: %{public}d, condition: %{public}s", key, condition.c_str());

    CHECK_AND_RETURN_RET_LOG(adapter_ != nullptr, "", "adapter %{public}s is nullptr", adapterName_.c_str());
    AudioExtParamKey hdiKey = AudioExtParamKey(key);
    char value[DumpFileUtil::PARAM_VALUE_LENTH];
    int32_t ret = adapter_->GetExtraParams(adapter_, hdiKey, condition.c_str(), value, DumpFileUtil::PARAM_VALUE_LENTH);
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, "", "get param fail, error code: %{public}d", ret);
    return value;
}

int32_t LocalDeviceAdapter::SetOutputRoute(const std::vector<DeviceType> &devices, int32_t streamId)
{
    CHECK_AND_RETURN_RET_LOG(!devices.empty() && devices.size() <= AUDIO_CONCURRENT_ACTIVE_DEVICES_LIMIT,
        ERR_INVALID_PARAM, "invalid audio devices");

    Trace trace("LocalDeviceAdapter::SetOutputRoute device " + std::to_string(devices[0]));
    AudioRouteNode source = {
        .portId = 0,
        .role = AUDIO_PORT_SOURCE_ROLE,
        .type = AUDIO_PORT_MIX_TYPE,
        .ext.mix.moduleId = 0,
        .ext.mix.streamId = streamId,
        .ext.device.desc = (char *)"",
    };
    AudioRouteNode sinks[devices.size()];
    for (size_t i = 0; i < devices.size(); ++i) {
        sinks[i] = {};
        int32_t ret = SetOutputPortPin(devices[i], sinks[i]);
        CHECK_AND_RETURN_RET(ret == SUCCESS, ret);
        AUDIO_INFO_LOG("output[%{public}zu], device: %{public}d, pin: 0x%{public}X", i, devices[i],
            sinks[i].ext.device.type);
        sinks[i].portId = static_cast<int32_t>(GetPortId(adapterName_, PORT_OUT));
        sinks[i].role = AUDIO_PORT_SINK_ROLE;
        sinks[i].type = AUDIO_PORT_DEVICE_TYPE;
        sinks[i].ext.device.moduleId = 0;
        sinks[i].ext.device.desc = (char *)"";
    }
    AudioRoute route = {
        .sources = &source,
        .sourcesLen = 1,
        .sinks = sinks,
        .sinksLen = devices.size(),
    };

    int64_t stamp = ClockTime::GetCurNano();
    CHECK_AND_RETURN_RET_LOG(adapter_ != nullptr, ERR_INVALID_HANDLE, "adapter %{public}s is nullptr",
        adapterName_.c_str());
    int32_t ret = adapter_->UpdateAudioRoute(adapter_, &route, &routeHandle_);
    stamp = (ClockTime::GetCurNano() - stamp) / AUDIO_US_PER_SECOND;
    AUDIO_INFO_LOG("update route, adapterName: %{public}s, device: %{public}d, cost: [%{public}" PRId64 "]ms",
        adapterName_.c_str(), devices[0], stamp);

    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ERR_OPERATION_FAILED, "update route fail");
    return SUCCESS;
}

int32_t LocalDeviceAdapter::SetInputRoute(DeviceType device, int32_t streamId, int32_t inputType)
{
    AudioRouteNode source = {};
    int32_t ret = SetInputPortPin(device, source);
    CHECK_AND_RETURN_RET(ret == SUCCESS, ret);
    AUDIO_INFO_LOG("input, device: %{public}d, pin: 0x%{public}X", device, source.ext.device.type);
    source.portId = static_cast<int32_t>(GetPortId(adapterName_, PORT_IN));
    source.role = AUDIO_PORT_SOURCE_ROLE;
    source.type = AUDIO_PORT_DEVICE_TYPE;
    source.ext.mix.moduleId = 0;
    source.ext.device.desc = (char *)"";
    AudioRouteNode sink = {
        .portId = 0,
        .role = AUDIO_PORT_SINK_ROLE,
        .type = AUDIO_PORT_MIX_TYPE,
        .ext.mix.moduleId = 0,
        .ext.mix.streamId = streamId,
        .ext.mix.source = inputType,
        .ext.device.desc = (char *)"",
    };
    AudioRoute route = {
        .sources = &source,
        .sourcesLen = 1,
        .sinks = &sink,
        .sinksLen = 1,
    };

    int64_t stamp = ClockTime::GetCurNano();
    CHECK_AND_RETURN_RET_LOG(adapter_ != nullptr, ERR_INVALID_HANDLE, "adapter %{public}s is nullptr",
        adapterName_.c_str());
    ret = adapter_->UpdateAudioRoute(adapter_, &route, &routeHandle_);
    stamp = (ClockTime::GetCurNano() - stamp) / AUDIO_US_PER_SECOND;
    AUDIO_INFO_LOG("update route, adapterName: %{public}s, device: %{public}d, cost: [%{public}" PRId64 "]ms",
        adapterName_.c_str(), device, stamp);

    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ERR_OPERATION_FAILED, "update route fail");
    return SUCCESS;
}

int32_t LocalDeviceAdapter::SetVoiceVolume(float volume)
{
    AUDIO_INFO_LOG("set modem call, volume: %{public}f", volume);

    Trace trace("LocalDeviceAdapter::SetVoiceVolume");
    CHECK_AND_RETURN_RET_LOG(adapter_ != nullptr, ERR_INVALID_HANDLE, "adapter %{public}s is nullptr",
        adapterName_.c_str());
    return adapter_->SetVoiceVolume(adapter_, volume);
}

void LocalDeviceAdapter::SetMicMute(bool isMute)
{
    AUDIO_INFO_LOG("isMute: %{public}s", isMute ? "true" : "false");

    CHECK_AND_RETURN_LOG(adapter_ != nullptr, "adapter %{public}s is nullptr", adapterName_.c_str());
    int32_t ret = adapter_->SetMicMute(adapter_, isMute);
    if (ret != SUCCESS) {
        AUDIO_WARNING_LOG("set mute fail");
    } else {
        AUDIO_INFO_LOG("set mute success");
    }
}

void LocalDeviceAdapter::SetDmDeviceType(uint16_t dmDeviceType)
{
    dmDeviceType_ = dmDeviceType;
}

void LocalDeviceAdapter::DumpInfo(std::string &dumpString)
{
    uint32_t renderNum = hdiRenderIds_.size();
    uint32_t captureNum = hdiCaptureIds_.size();
    dumpString += "  - local/" + adapterName_ + "\trenderNum: " + std::to_string(renderNum) + "\tcaptureNum: " +
        std::to_string(captureNum) + "\n";
}

void *LocalDeviceAdapter::CreateRender(void *param, void *deviceDesc, uint32_t &hdiRenderId)
{
    CHECK_AND_RETURN_RET_LOG(param != nullptr && deviceDesc != nullptr, nullptr, "param or deviceDesc is nullptr");
    struct AudioSampleAttributes *localParam = static_cast<struct AudioSampleAttributes *>(param);
    struct AudioDeviceDescriptor *localDeviceDesc = static_cast<struct AudioDeviceDescriptor *>(deviceDesc);

    CHECK_AND_RETURN_RET_LOG(adapter_ != nullptr, nullptr, "adapter %{public}s is nullptr", adapterName_.c_str());
    localDeviceDesc->portId = GetPortId(adapterName_, PORT_OUT);

    struct IAudioRender *render = nullptr;
    int32_t ret = adapter_->CreateRender(adapter_, localDeviceDesc, localParam, &render, &hdiRenderId);
    if (ret != SUCCESS || render == nullptr) {
        AUDIO_ERR_LOG("create render fail");
        return nullptr;
    }
    AUDIO_INFO_LOG("create render success, hdiRenderId: %{public}u, desc: %{public}s", hdiRenderId,
        localDeviceDesc->desc);

    std::lock_guard<std::mutex> lock(renderMtx_);
    hdiRenderIds_.insert(hdiRenderId);
    return render;
}

void LocalDeviceAdapter::DestroyRender(uint32_t hdiRenderId)
{
    AUDIO_INFO_LOG("destroy render, hdiRenderId: %{public}u", hdiRenderId);

    CHECK_AND_RETURN_LOG(adapter_ != nullptr, "adapter %{public}s is nullptr", adapterName_.c_str());
    CHECK_AND_RETURN_LOG(hdiRenderIds_.count(hdiRenderId) != 0, "render not exist");
    adapter_->DestroyRender(adapter_, hdiRenderId);

    std::lock_guard<std::mutex> lock(renderMtx_);
    hdiRenderIds_.erase(hdiRenderId);
}

void *LocalDeviceAdapter::CreateCapture(void *param, void *deviceDesc, uint32_t &hdiCaptureId)
{
    CHECK_AND_RETURN_RET_LOG(param != nullptr && deviceDesc != nullptr, nullptr, "param or deviceDesc is nullptr");
    struct AudioSampleAttributes *localParam = static_cast<struct AudioSampleAttributes *>(param);
    struct AudioDeviceDescriptor *localDeviceDesc = static_cast<struct AudioDeviceDescriptor *>(deviceDesc);

    CHECK_AND_RETURN_RET_LOG(adapter_ != nullptr, nullptr, "adapter %{public}s is nullptr", adapterName_.c_str());
    localDeviceDesc->portId = GetPortId(adapterName_, PORT_IN);

    struct IAudioCapture *capture = nullptr;
    int32_t ret = adapter_->CreateCapture(adapter_, localDeviceDesc, localParam, &capture, &hdiCaptureId);
    if (ret != SUCCESS || capture == nullptr) {
        AUDIO_ERR_LOG("create capture fail");
        return nullptr;
    }
    AUDIO_INFO_LOG("create capture success, hdiCaptureId: %{public}u, desc: %{public}s", hdiCaptureId,
        localDeviceDesc->desc);

    std::lock_guard<std::mutex> lock(captureMtx_);
    hdiCaptureIds_.insert(hdiCaptureId);
    return capture;
}

void LocalDeviceAdapter::DestroyCapture(uint32_t hdiCaptureId)
{
    AUDIO_INFO_LOG("destroy capture, hdiCaptureId: %{public}u", hdiCaptureId);

    CHECK_AND_RETURN_LOG(adapter_ != nullptr, "adapter %{public}s is nullptr", adapterName_.c_str());
    CHECK_AND_RETURN_LOG(hdiCaptureIds_.count(hdiCaptureId) != 0, "capture not exist");
    adapter_->DestroyCapture(adapter_, hdiCaptureId);

    std::lock_guard<std::mutex> lock(captureMtx_);
    hdiCaptureIds_.erase(hdiCaptureId);
}

uint32_t LocalDeviceAdapter::GetPortId(enum AudioPortDirection portFlag)
{
    CHECK_AND_RETURN_RET_LOG(adapter_ != nullptr, 0, "adapter %{public}s is nullptr", adapterName_.c_str());
    uint32_t portId = 0;
    for (uint32_t port = 0; port < adapterDesc_.portsLen; ++port) {
        if (adapterDesc_.ports[port].dir == portFlag) {
            portId = adapterDesc_.ports[port].portId;
            break;
        }
    }
    AUDIO_DEBUG_LOG("portId: %{public}u", portId);
    return portId;
}

int32_t LocalDeviceAdapter::SetOutputPortPin(DeviceType outputDevice, AudioRouteNode &sink)
{
    int32_t ret = SUCCESS;

    switch (outputDevice) {
        case DEVICE_TYPE_EARPIECE:
            sink.ext.device.type = PIN_OUT_EARPIECE;
            sink.ext.device.desc = (char *)"pin_out_earpiece";
            break;
        case DEVICE_TYPE_SPEAKER:
            sink.ext.device.type = PIN_OUT_SPEAKER;
            sink.ext.device.desc = (char *)"pin_out_speaker";
            break;
        case DEVICE_TYPE_WIRED_HEADSET:
            sink.ext.device.type = PIN_OUT_HEADSET;
            sink.ext.device.desc = (char *)"pin_out_headset";
            break;
        case DEVICE_TYPE_USB_ARM_HEADSET:
            sink.ext.device.type = PIN_OUT_USB_HEADSET;
            sink.ext.device.desc = (char *)"pin_out_usb_headset";
            break;
        case DEVICE_TYPE_USB_HEADSET:
            sink.ext.device.type = PIN_OUT_USB_EXT;
            sink.ext.device.desc = (char *)"pin_out_usb_ext";
            break;
        case DEVICE_TYPE_BLUETOOTH_SCO:
            sink.ext.device.type = PIN_OUT_BLUETOOTH_SCO;
            sink.ext.device.desc = (char *)"pin_out_bluetooth_sco";
            break;
        case DEVICE_TYPE_BLUETOOTH_A2DP:
            sink.ext.device.type = PIN_OUT_BLUETOOTH_A2DP;
            sink.ext.device.desc = (char *)"pin_out_bluetooth_a2dp";
            break;
        case DEVICE_TYPE_NONE:
            sink.ext.device.type = PIN_NONE;
            sink.ext.device.desc = (char *)"pin_out_none";
            break;
        default:
            ret = ERR_NOT_SUPPORTED;
            break;
    }

    return ret;
}

int32_t LocalDeviceAdapter::SetInputPortPin(DeviceType inputDevice, AudioRouteNode &source)
{
    int32_t ret = SUCCESS;
    switch (inputDevice) {
        case DEVICE_TYPE_MIC:
        case DEVICE_TYPE_EARPIECE:
        case DEVICE_TYPE_SPEAKER:
        case DEVICE_TYPE_BLUETOOTH_A2DP_IN:
            source.ext.device.type = PIN_IN_MIC;
            source.ext.device.desc = (char *)"pin_in_mic";
            break;
        case DEVICE_TYPE_WIRED_HEADSET:
            source.ext.device.type = PIN_IN_HS_MIC;
            source.ext.device.desc = (char *)"pin_in_hs_mic";
            break;
        case DEVICE_TYPE_USB_ARM_HEADSET:
            source.ext.device.type = PIN_IN_USB_HEADSET;
            source.ext.device.desc = (char *)"pin_in_usb_headset";
            break;
        case DEVICE_TYPE_USB_HEADSET:
            source.ext.device.type = PIN_IN_USB_EXT;
            source.ext.device.desc = (char *)"pin_in_usb_ext";
            break;
        case DEVICE_TYPE_BLUETOOTH_SCO:
            source.ext.device.type = PIN_IN_BLUETOOTH_SCO_HEADSET;
            source.ext.device.desc = (char *)"pin_in_bluetooth_sco_headset";
            break;
        case DEVICE_TYPE_ACCESSORY:
            if (dmDeviceType_ == DM_DEVICE_TYPE_PENCIL) {
                source.ext.device.type = PIN_IN_PENCIL;
                source.ext.device.desc = (char *)"pin_in_pencil";
            } else if (dmDeviceType_ == DM_DEVICE_TYPE_UWB) {
                source.ext.device.type = PIN_IN_UWB;
                source.ext.device.desc = (char *)"pin_in_uwb";
            }
            break;
        default:
            ret = ERR_NOT_SUPPORTED;
            break;
    }

    return ret;
}

} // namespace AudioStandard
} // namespace OHOS
