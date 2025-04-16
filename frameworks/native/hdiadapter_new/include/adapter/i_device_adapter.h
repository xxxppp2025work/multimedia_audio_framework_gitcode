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

#ifndef I_DEVICE_ADAPTER_H
#define I_DEVICE_ADAPTER_H

#include <iostream>
#include <cstring>
#include "common/hdi_adapter_info.h"

namespace OHOS {
namespace AudioStandard {
class IDeviceAdapterCallback {
    public:
        virtual void OnAudioParamChange(const std::string &adapterName, const AudioParamKey key,
            const std::string &condition, const std::string &value) {}
    };

class IDeviceAdapter {
    friend class HdiAdapterManager;
public:
    virtual ~IDeviceAdapter() = default;

    virtual HdiDeviceManagerType GetDeviceManagerType(void) = 0;

    virtual void SetAudioParameter(const AudioParamKey key, const std::string &condition, const std::string &value) = 0;
    virtual std::string GetAudioParameter(const AudioParamKey key, const std::string &condition) = 0;

    virtual int32_t SetVoiceVolume(float volume) = 0;
    virtual int32_t SetOutputRoute(const std::vector<DeviceType> &devices, int32_t streamId) = 0;
    virtual int32_t SetInputRoute(DeviceType device, int32_t streamId, int32_t inputType) = 0;
    virtual void SetMicMute(bool isMute) = 0;
    virtual void SetDmDeviceType(uint16_t dmDeviceType) {}

    virtual void DumpInfo(std::string &dumpString) = 0;

protected:
    virtual void RegistRenderSinkCallback(uint32_t hdiRenderId, IDeviceManagerCallback *callback) {}
    virtual void RegistCaptureSourceCallback(uint32_t hdiCaptureId, IDeviceManagerCallback *callback) {}
    virtual void UnRegistRenderSinkCallback(uint32_t hdiRenderId) {}
    virtual void UnRegistCaptureSourceCallback(uint32_t hdiCaptureId) {}

    virtual void *CreateRender(void *param, void *deviceDesc, uint32_t &hdiRenderId) = 0;
    virtual void DestroyRender(uint32_t hdiRenderId) = 0;
    virtual void *CreateCapture(void *param, void *deviceDesc, uint32_t &hdiCaptureId) = 0;
    virtual void DestroyCapture(uint32_t hdiCaptureId) = 0;
};

} // namespace AudioStandard
} // namespace OHOS

#endif // I_DEVICE_ADAPTER_H