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
#define LOG_TAG "RemoteDeviceAdapter"
#endif

#include "adapter/remote_device_adapter.h"
#include "audio_hdi_log.h"
#include "audio_errors.h"
#include "audio_utils.h"

using namespace OHOS::HDI::DistributedAudio::Audio::V1_0;

namespace OHOS {
namespace AudioStandard {
RemoteAdapterHdiCallback::RemoteAdapterHdiCallback(const RemoteAdapterHdiCbFunc cbFunc)
    : cbFunc_(cbFunc)
{
}

int32_t RemoteAdapterHdiCallback::RenderCallback(AudioCallbackType type, int8_t &reserved, int8_t &cookie)
{
    (void)type;
    (void)reserved;
    (void)cookie;
    return SUCCESS;
}

int32_t RemoteAdapterHdiCallback::ParamCallback(AudioExtParamKey key, const std::string &condition,
    const std::string &value, int8_t &reserved, int8_t cookie)
{
    (void)cookie;
    AUDIO_INFO_LOG("key: %{public}d, condition: %{public}s, value: %{public}s", key, condition.c_str(), value.c_str());
    AudioParamKey audioKey = AudioParamKey(key);
    cbFunc_(audioKey, condition.c_str(), value.c_str(), static_cast<void *>(&reserved));
    return SUCCESS;
}

RemoteDeviceAdapter::RemoteDeviceAdapter(const std::string &adapterName, sptr<IAudioAdapter> adapter,
    AudioAdapterDescriptor adapterDesc)
    : adapterName_(adapterName), adapter_(adapter), adapterDesc_(adapterDesc),
    hdiCallback_([this](const AudioParamKey key, const char *condition, const char *value,
    void *reserved) { return this->HandleEvent(key, condition, value, reserved); })
{
#ifdef FEATURE_DISTRIBUTE_AUDIO
    if (adapter_ != nullptr) {
        int32_t ret = adapter_->RegExtraParamObserver(&deviceAdapter->hdiCallback_, 0);
        CHECK_AND_RETURN_LOG(ret == SUCCESS, "regist extra param observer fail, ret: %{public}d", ret);
    }
#endif
}

HdiDeviceManagerType RemoteDeviceAdapter::GetDeviceManagerType(void)
{
    return HDI_DEVICE_MANAGER_TYPE_REMOTE;
}

bool RemoteDeviceAdapter::CheckStatus(void)
{
    if (hdiRenderIds_.size() != 0 || hdiCaptureIds_.size() != 0) {
        AUDIO_INFO_LOG("adapter %{public}s has some ports busy, renderNum: %{public}zu, captureNum: %{public}zu",
            adapterName_.c_str(), hdiRenderIds_.size(), hdiCaptureIds_.size());
        return false;
    }
    return true;
}

void RemoteDeviceAdapter::SetAudioParameter(const AudioParamKey key, const std::string &condition,
    const std::string &value)
{
#ifdef FEATURE_DISTRIBUTE_AUDIO
    AUDIO_INFO_LOG("key: %{public}d, condition: %{public}s, value: %{public}s", key, condition.c_str(), value.c_str());

    CHECK_AND_RETURN_LOG(adapter_ != nullptr, "adapter %{public}s is nullptr", adapterName_.c_str());
    AudioExtParamKey hdiKey = AudioExtParamKey(key);
    int32_t ret = adapter_->SetExtraParams(hdiKey, condition, value);
    CHECK_AND_RETURN_LOG(ret == SUCCESS, "set param fail, error code: %{public}d", ret);
#else
    AUDIO_INFO_LOG("not support");
#endif
}

std::string RemoteDeviceAdapter::GetAudioParameter(const AudioParamKey key, const std::string &condition)
{
#ifdef FEATURE_DISTRIBUTE_AUDIO
    AUDIO_INFO_LOG("key: %{public}d, condition: %{public}s", key, condition.c_str());

    CHECK_AND_RETURN_RET_LOG(adapter_ != nullptr, "", "adapter %{public}s is nullptr", adapterName_.c_str());
    AudioExtParamKey hdiKey = AudioExtParamKey(key);
    std::string value;
    int32_t ret = adapter_->GetExtraParams(hdiKey, condition.c_str(), value);
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, "", "get param fail, error code: %{public}d", ret);
    return value;
#else
    AUDIO_INFO_LOG("not support");
    return "";
#endif
}

int32_t RemoteDeviceAdapter::SetOutputRoute(const std::vector<DeviceType> &devices, int32_t streamId)
{
    CHECK_AND_RETURN_RET_LOG(!devices.empty(), ERR_INVALID_PARAM, "invalid audio devices");
    DeviceType device = devices[0];

    AudioRouteNode source = {
        .portId = 0,
        .role = AudioPortRole::AUDIO_PORT_SOURCE_ROLE,
        .type = AudioPortType::AUDIO_PORT_MIX_TYPE,
        .ext.mix.moduleId = 0,
        .ext.mix.streamId = streamId,
    };
    AudioRouteNode sink = {};
    int32_t ret = SetOutputPortPin(device, sink);
    CHECK_AND_RETURN_RET(ret == SUCCESS, ret);
    AUDIO_INFO_LOG("output, pin: 0x%{public}X", sink.ext.device.type);
    sink.portId = static_cast<int32_t>(GetPortId(PORT_OUT));
    sink.role = AudioPortRole::AUDIO_PORT_SINK_ROLE;
    sink.type = AudioPortType::AUDIO_PORT_DEVICE_TYPE;
    sink.ext.device.moduleId = 0;
    AudioRoute route;
    route.sources.push_back(source);
    route.sinks.push_back(sink);

    CHECK_AND_RETURN_RET_LOG(adapter_ != nullptr, ERR_INVALID_HANDLE, "adapter %{public}s is nullptr",
        adapterName_.c_str());
    ret = adapter_->UpdateAudioRoute(route, routeHandle_);

    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ERR_OPERATION_FAILED, "update route fail");
    return SUCCESS;
}

int32_t RemoteDeviceAdapter::SetInputRoute(DeviceType device, int32_t streamId, int32_t inputType)
{
    (void)inputType;

    AudioRouteNode source = {};
    int32_t ret = SetInputPortPin(device, source);
    CHECK_AND_RETURN_RET(ret == SUCCESS, ret);
    AUDIO_INFO_LOG("input, pin: 0x%{public}X", source.ext.device.type);
    source.portId = static_cast<int32_t>(GetPortId(PORT_IN));
    source.role = AudioPortRole::AUDIO_PORT_SOURCE_ROLE;
    source.type = AudioPortType::AUDIO_PORT_DEVICE_TYPE;
    source.ext.device.moduleId = 0;
    AudioRouteNode sink = {
        .portId = 0,
        .role = AudioPortRole::AUDIO_PORT_SINK_ROLE,
        .type = AudioPortType::AUDIO_PORT_MIX_TYPE,
        .ext.mix.moduleId = 0,
        .ext.mix.streamId = streamId,
    };
    AudioRoute route;
    route.sources.push_back(source);
    route.sinks.push_back(sink);

    CHECK_AND_RETURN_RET_LOG(adapter_ != nullptr, ERR_INVALID_HANDLE, "adapter %{public}s is nullptr",
        adapterName_.c_str());
    ret = adapter_->UpdateAudioRoute(route, routeHandle_);

    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ERR_OPERATION_FAILED, "update route fail");
    return SUCCESS;
}

int32_t RemoteDeviceAdapter::SetVoiceVolume(float volume)
{
    AUDIO_INFO_LOG("not support");
    return ERR_NOT_SUPPORTED;
}

void RemoteDeviceAdapter::SetMicMute(bool isMute)
{
    AUDIO_INFO_LOG("not support");
}

void RemoteDeviceAdapter::SetDmDeviceType(uint16_t dmDeviceType)
{
    AUDIO_INFO_LOG("not support");
}

void RemoteDeviceAdapter::DumpInfo(std::string &dumpString)
{
    uint32_t renderNum = hdiRenderIds_.size();
    uint32_t captureNum = hdiCaptureIds_.size();
    dumpString += "  - remote/" + adapterName_ + "\trenderNum: " + std::to_string(renderNum) + "\tcaptureNum: " +
        std::to_string(captureNum) + "\n";
}

void *RemoteDeviceAdapter::CreateRender(void *param, void *deviceDesc, uint32_t &hdiRenderId)
{
    CHECK_AND_RETURN_RET_LOG(param != nullptr && deviceDesc != nullptr, nullptr, "param or deviceDesc is nullptr");
    AudioSampleAttributes &remoteParam = *(static_cast<struct AudioSampleAttributes *>(param));
    AudioDeviceDescriptor &remoteDeviceDesc = *(static_cast<struct AudioDeviceDescriptor *>(deviceDesc));

    CHECK_AND_RETURN_RET_LOG(adapter_ != nullptr, nullptr, "adapter %{public}s is nullptr", adapterName.c_str());
    remoteDeviceDesc.portId = GetPortId(PORT_OUT);

    sptr<IAudioRender> render = nullptr;
    int32_t ret = adapter_->CreateRender(remoteDeviceDesc, remoteParam, render, hdiRenderId);
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS && render != nullptr, nullptr, "create render fail");
    IAudioRender *rawRender = render.GetRefPtr();
    render.ForceSetRefPtr(nullptr);
    AUDIO_INFO_LOG("create render success, hdiRenderId: %{public}u, desc: %{public}s", hdiRenderId,
        remoteDeviceDesc.desc.c_str());

    std::lock_guard<std::mutex> lock(renderMtx_);
    hdiRenderIds_.insert(hdiRenderId);
    return rawRender;
}

void RemoteDeviceAdapter::DestroyRender(uint32_t hdiRenderId)
{
    AUDIO_INFO_LOG("destroy render, hdiRenderId: %{public}u", hdiRenderId);

    CHECK_AND_RETURN_LOG(adapter_ != nullptr, "adapter %{public}s is nullptr", adapterName_.c_str());
    CHECK_AND_RETURN_LOG(hdiRenderIds_.count(hdiRenderId) != 0, "render not exist");
    adapter_->DestroyRender(hdiRenderId);

    std::lock_guard<std::mutex> lock(renderMtx_);
    hdiRenderIds_.erase(hdiRenderId);
}

void *RemoteDeviceAdapter::CreateCapture(void *param, void *deviceDesc, uint32_t &hdiCaptureId)
{
    CHECK_AND_RETURN_RET_LOG(param != nullptr && deviceDesc != nullptr, nullptr, "param or deviceDesc is nullptr");
    AudioSampleAttributes &remoteParam = *(static_cast<struct AudioSampleAttributes *>(param));
    AudioDeviceDescriptor &remoteDeviceDesc = *(static_cast<struct AudioDeviceDescriptor *>(deviceDesc));

    CHECK_AND_RETURN_RET_LOG(adapter_ != nullptr, nullptr, "adapter %{public}s is nullptr", adapterName_.c_str());
    remoteDeviceDesc.portId = GetPortId(PORT_IN);

    sptr<IAudioCapture> capture = nullptr;
    int32_t ret = adapter_->CreateCapture(remoteDeviceDesc, remoteParam, capture, hdiCaptureId);
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS && capture != nullptr, nullptr, "create capture fail");
    IAudioCapture *rawCapture = capture.GetRefPtr();
    capture.ForceSetRefPtr(nullptr);
    AUDIO_INFO_LOG("create capture success, hdiCaptureId: %{public}u, desc: %{public}s", hdiCaptureId,
        remoteDeviceDesc.desc.c_str());

    std::lock_guard<std::mutex> lock(captureMtx_);
    hdiCaptureIds_.insert(hdiCaptureId);
    return rawCapture;
}

void RemoteDeviceAdapter::DestroyCapture(uint32_t hdiCaptureId)
{
    AUDIO_INFO_LOG("destroy capture, hdiCaptureId: %{public}u", hdiCaptureId);

    CHECK_AND_RETURN_LOG(adapter_ != nullptr, "adapter %{public}s is nullptr", adapterName_.c_str());
    CHECK_AND_RETURN_LOG(hdiCaptureIds_.count(hdiCaptureId) != 0, "capture not exist");
    adapter_->DestroyCapture(hdiCaptureId);

    std::lock_guard<std::mutex> lock(captureMtx_);
    hdiCaptureIds_.erase(hdiCaptureId);
}

void RemoteDeviceAdapter::RegistRenderSinkCallback(uint32_t hdiRenderId, IDeviceAdapterCallback *callback)
{
    std::lock_guard<std::mutex> lock(renderCallbackMtx_);
    CHECK_AND_RETURN_LOG(renderCallbacks_.count(hdiRenderId) == 0,
        "callback already existed, hdiRenderId: %{public}u", hdiRenderId);
    renderCallbacks_[hdiRenderId] = callback;
}

void RemoteDeviceAdapter::RegistCaptureSourceCallback(uint32_t hdiCaptureId, IDeviceAdapterCallback *callback)
{
    std::lock_guard<std::mutex> lock(captureCallbackMtx_);
    CHECK_AND_RETURN_LOG(captureCallbacks_.count(hdiCaptureId) == 0,
        "callback already existed, hdiCaptureId: %{public}u", hdiCaptureId);
    captureCallbacks_[hdiCaptureId] = callback;
}

void RemoteDeviceAdapter::UnRegistRenderSinkCallback(uint32_t hdiRenderId)
{
    std::lock_guard<std::mutex> lock(renderCallbackMtx_);
    CHECK_AND_RETURN_LOG(renderCallbacks_.count(hdiRenderId) != 0,
        "callback not exist, hdiRenderId: %{public}u", hdiRenderId);
    renderCallbacks_.erase(hdiRenderId);
}

void RemoteDeviceAdapter::UnRegistCaptureSourceCallback(const std::string &adapterName, uint32_t hdiCaptureId)
{
    std::lock_guard<std::mutex> lock(captureCallbackMtx_);
    CHECK_AND_RETURN_LOG(captureCallbacks_.count(hdiCaptureId) != 0,
        "callback not exist, hdiCaptureId: %{public}u", hdiCaptureId);
    captureCallbacks_.erase(hdiCaptureId);
}

uint32_t RemoteDeviceAdapter::GetPortId(enum AudioPortDirection portFlag)
{
    uint32_t portId = 0;
    if (portFlag == PORT_OUT) {
        portId = AudioPortPin::PIN_OUT_SPEAKER;
    } else if (portFlag == PORT_IN) {
        portId = AudioPortPin::PIN_IN_MIC;
    }
    AUDIO_DEBUG_LOG("portId: %{public}u", portId);
    return portId;
}

int32_t RemoteDeviceAdapter::HandleEvent(const AudioParamKey key, const char *condition, const char *value,
    void *reserved)
{
    AUDIO_INFO_LOG("key: %{public}d, condition: %{public}s, value: %{public}s", key, condition, value);
    int32_t ret = SUCCESS;
    switch (key) {
        case AudioParamKey::PARAM_KEY_STATE:
            ret = HandleStateChangeEvent(key, condition, value);
            break;
        case AudioParamKey::VOLUME:
        case AudioParamKey::INTERRUPT:
            ret = HandleRenderParamEvent(key, condition, value);
            break;
        default:
            AUDIO_ERR_LOG("not support, key: %{public}d", key);
            return ERR_NOT_SUPPORTED;
    }
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ret, "handle fail, key: %{public}d, error code: %{public}d", key, ret);
    return SUCCESS;
}

int32_t RemoteDeviceAdapter::HandleStateChangeEvent(const AudioParamKey key, const char *condition, const char *value)
{
    char eventDes[EVENT_DES_SIZE];
    char contentDes[ADAPTER_STATE_CONTENT_DES_SIZE];
    CHECK_AND_RETURN_RET_LOG(sscanf_s(condition, "%[^;];%s", eventDes, EVENT_DES_SIZE, contentDes,
        ADAPTER_STATE_CONTENT_DES_SIZE) == PARAMS_STATE_NUM, ERR_INVALID_PARAM, "parse condition fail");
    CHECK_AND_RETURN_RET_LOG(strcmp(eventDes, "ERR_EVENT") == 0, ERR_NOT_SUPPORTED, "not support event %{public}s",
        eventDes);

    std::string devTypeKey = "DEVICE_TYPE=";
    std::string contentDesStr = std::string(contentDes);
    size_t devTypeKeyPos = contentDesStr.find(devTypeKey);
    CHECK_AND_RETURN_RET_LOG(devTypeKeyPos != std::string::npos, ERR_INVALID_PARAM,
        "not find daudio device type info, contentDes: %{public}s", contentDesStr.c_str());
    size_t devTypeValPos = devTypeKeyPos + devTypeKey.length();
    CHECK_AND_RETURN_RET_LOG(devTypeValPos < contentDesStr.length(), ERR_INVALID_PARAM,
        "not find daudio device type value, contentDes: %{public}s", contentDesStr.c_str());

    int32_t ret = SUCCESS;
    if (contentDesStr[devTypeValPos] == DAUDIO_DEV_TYPE_SPK) {
        AUDIO_INFO_LOG("ERR_EVENT is DAUDIO_DEV_TYPE_SPK");
        ret = HandleRenderParamEvent(key, condition, value);
    } else if (contentDesStr[devTypeValPos] == DAUDIO_DEV_TYPE_MIC) {
        AUDIO_INFO_LOG("ERR_EVENT is DAUDIO_DEV_TYPE_MIC");
        ret = HandleCaptureParamEvent(key, condition, value);
    } else {
        AUDIO_ERR_LOG("not support device type, contentDes: %{public}s", contentDesStr.c_str());
        return ERR_NOT_SUPPORTED;
    }
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ret, "handle event %{public}s fail", contentDesStr.c_str());
    return SUCCESS;
}

int32_t RemoteDeviceAdapter::HandleRenderParamEvent(const AudioParamKey key, const char *condition, const char *value)
{
    IDeviceAdapterCallback *renderCallback = nullptr;
    {
        std::lock_guard<std::mutex> lock(renderCallbackMtx_);
        if (renderCallbacks_.size() != 1) {
            AUDIO_WARNING_LOG("exist %{public}zu renders port in adapter", renderCallbacks_.size());
        }
        for (auto &cb : renderCallbacks_) {
            if (cb.second != nullptr) {
                renderCallback = cb.second;
                break;
            }
        }
    }
    CHECK_AND_RETURN_RET_LOG(renderCallback != nullptr, ERR_INVALID_HANDLE, "not find render port in adapter");
    renderCallback->OnAudioParamChange(adapterName_, key, std::string(condition), std::string(value));
    return SUCCESS;
}

int32_t RemoteDeviceAdapter::HandleCaptureParamEvent(const AudioParamKey key, const char *condition, const char *value)
{
    IDeviceAdapterCallback *captureCallback = nullptr;
    {
        std::lock_guard<std::mutex> lock(captureCallbackMtx_);
        if (captureCallbacks_.size() != 1) {
            AUDIO_WARNING_LOG("exist %{public}zu captures port in adapter", captureCallbacks_.size());
        }
        for (auto &cb : captureCallbacks_) {
            if (cb.second != nullptr) {
                captureCallback = cb.second;
                break;
            }
        }
    }
    CHECK_AND_RETURN_RET_LOG(captureCallback != nullptr, ERR_INVALID_HANDLE, "not find capture port in adapter");
    captureCallback->OnAudioParamChange(adapterName_, key, std::string(condition), std::string(value));
    return SUCCESS;
}

int32_t RemoteDeviceAdapter::SetOutputPortPin(DeviceType outputDevice, AudioRouteNode &sink)
{
    int32_t ret = SUCCESS;

    switch (outputDevice) {
        case DEVICE_TYPE_SPEAKER:
            sink.ext.device.type = AudioPortPin::PIN_OUT_SPEAKER;
            sink.ext.device.desc = "pin_out_speaker";
            break;
        case DEVICE_TYPE_WIRED_HEADSET:
            sink.ext.device.type = AudioPortPin::PIN_OUT_HEADSET;
            sink.ext.device.desc = "pin_out_headset";
            break;
        case DEVICE_TYPE_USB_HEADSET:
            sink.ext.device.type = AudioPortPin::PIN_OUT_USB_EXT;
            sink.ext.device.desc = "pin_out_usb_ext";
            break;
        default:
            ret = ERR_NOT_SUPPORTED;
            break;
    }

    return ret;
}

int32_t RemoteDeviceAdapter::SetInputPortPin(DeviceType inputDevice, AudioRouteNode &source)
{
    int32_t ret = SUCCESS;

    switch (inputDevice) {
        case DEVICE_TYPE_MIC:
            source.ext.device.type = AudioPortPin::PIN_IN_MIC;
            source.ext.device.desc = "pin_in_mic";
            break;
        case DEVICE_TYPE_WIRED_HEADSET:
            source.ext.device.type = AudioPortPin::PIN_IN_HS_MIC;
            source.ext.device.desc = "pin_in_hs_mic";
            break;
        case DEVICE_TYPE_USB_HEADSET:
            source.ext.device.type = AudioPortPin::PIN_IN_USB_EXT;
            source.ext.device.desc = "pin_in_usb_ext";
            break;
        default:
            ret = ERR_NOT_SUPPORTED;
            break;
    }

    return ret;
}

} // namespace AudioStandard
} // namespace OHOS
