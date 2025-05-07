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

#ifndef LOCAL_DEVICE_ADAPTER_H
#define LOCAL_DEVICE_ADAPTER_H

#include <iostream>
#include <cstring>
#include <unordered_set>
#include <vector>
#include <mutex>
#include "v4_0/iaudio_manager.h"
#include "hdf_remote_service.h"
#include "common/hdi_adapter_info.h"
#include "adapter/i_device_adapter.h"

namespace OHOS {
namespace AudioStandard {


class LocalDeviceAdapter : public IDeviceAdapter {
    friend class LocalDeviceManager;
public:
    LocalDeviceAdapter(const std::string adapterName, struct IAudioAdapter *adapter,
        struct AudioAdapterDescriptor adapterDesc);
    ~LocalDeviceAdapter() = default;

    HdiDeviceManagerType GetDeviceManagerType(void) override;
    bool CheckStatus(void) override;

    void SetAudioParameter(const AudioParamKey key, const std::string &condition, const std::string &value) override;
    std::string GetAudioParameter(const AudioParamKey key, const std::string &condition) override;

    int32_t SetOutputRoute(const std::vector<DeviceType> &devices, int32_t streamId) override;
    int32_t SetInputRoute(DeviceType device, int32_t streamId, int32_t inputType) override;

    int32_t SetVoiceVolume(float volume) override;
    void SetMicMute(bool isMute) override;
    void SetDmDeviceType(uint16_t dmDeviceType) override;

    void DumpInfo(std::string &dumpString) override;

private:
    void *CreateRender(void *param, void *deviceDesc, uint32_t &hdiRenderId) override;
    void DestroyRender(uint32_t hdiRenderId) override;
    void *CreateCapture(void *param, void *deviceDesc, uint32_t &hdiCaptureId) override;
    void DestroyCapture(uint32_t hdiCaptureId) override;

    uint32_t GetPortId(enum AudioPortDirection portFlag);
    int32_t SetOutputPortPin(DeviceType outputDevice, AudioRouteNode &sink);
    int32_t SetInputPortPin(DeviceType inputDevice, AudioRouteNode &source);

private:
    std::string adapterName_ = "";
    struct IAudioAdapter *adapter_ = nullptr;
    struct AudioAdapterDescriptor adapterDesc_ = {};
    std::unordered_set<uint32_t> hdiRenderIds_;
    std::unordered_set<uint32_t> hdiCaptureIds_;
    std::mutex renderMtx_;
    std::mutex captureMtx_;
    int32_t routeHandle_ = -1;
    std::vector<LocalParameter> reSetParams_;
    uint16_t dmDeviceType_ = 0;
};

} // namespace AudioStandard
} // namespace OHOS

#endif // LOCAL_DEVICE_ADAPTER_H
