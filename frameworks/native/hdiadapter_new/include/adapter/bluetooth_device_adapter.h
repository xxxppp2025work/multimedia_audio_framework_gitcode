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

#ifndef BLUETOOTH_DEVICE_ADAPTER_H
#define BLUETOOTH_DEVICE_ADAPTER_H

#include <iostream>
#include <cstring>
#include <unordered_map>
#include <set>
#include <mutex>
#include "audio_proxy_manager.h"
#include "adapter/i_device_adapter.h"

namespace OHOS {
namespace AudioStandard {
typedef struct OHOS::HDI::Audio_Bluetooth::AudioAdapter BtAudioAdapter;
typedef struct OHOS::HDI::Audio_Bluetooth::AudioAdapterDescriptor BtAudioAdapterDescriptor;
typedef OHOS::HDI::Audio_Bluetooth::AudioRender BtAudioRender;
typedef OHOS::HDI::Audio_Bluetooth::AudioCapture BtAudioCapture;
typedef enum OHOS::HDI::Audio_Bluetooth::AudioPortDirection BtAudioPortDirection;

class BluetoothDeviceAdapter : public IDeviceAdapter {
    friend class BluetoothDeviceManager;
public:
    BluetoothDeviceAdapter(const std::string &adapterName, BtAudioAdapter *adapter,
        BtAudioAdapterDescriptor adapterDesc);
    ~BluetoothDeviceAdapter() = default;

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

    uint32_t GetPortId(BtAudioPortDirection portFlag);
    uint32_t GetHdiRenderId(void);
    uint32_t GetHdiCaptureId(void);

private:
    std::string adapterName_ = "";
    BtAudioAdapter *adapter_ = nullptr;
    BtAudioAdapterDescriptor adapterDesc_ = {};
    std::unordered_map<uint32_t, BtAudioRender *> renders_;
    std::unordered_map<uint32_t, BtAudioCapture *> captures_;
    std::set<uint32_t> freeHdiRenderIdSet_;
    std::set<uint32_t> freeHdiCaptureIdSet_;
    std::mutex renderMtx_;
    std::mutex captureMtx_;
};

} // namespace AudioStandard
} // namespace OHOS

#endif // BLUETOOTH_DEVICE_ADAPTER_H
