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

#ifndef REMOTE_DEVICE_ADAPTER_H
#define REMOTE_DEVICE_ADAPTER_H

#include <iostream>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <v1_0/iaudio_manager.h>
#include <v1_0/iaudio_callback.h>
#include <v1_0/audio_types.h>
#include "audio_info.h"
#include "adapter/i_device_manager.h"

namespace OHOS {
namespace AudioStandard {
typedef OHOS::HDI::DistributedAudio::Audio::V1_0::IAudioAdapter RemoteIAudioAdapter;
typedef struct OHOS::HDI::DistributedAudio::Audio::V1_0::AudioAdapterDescriptor RemoteAudioAdapterDescriptor;
typedef OHOS::HDI::DistributedAudio::Audio::V1_0::IAudioCallback RemoteIAudioCallback;
typedef OHOS::HDI::DistributedAudio::Audio::V1_0::AudioCallbackType RemoteAudioCallbackType;
typedef OHOS::HDI::DistributedAudio::Audio::V1_0::AudioExtParamKey RemoteAudioExtParamKey;
typedef enum OHOS::HDI::DistributedAudio::Audio::V1_0::AudioPortDirection RemoteAudioPortDirection;
typedef OHOS::HDI::DistributedAudio::Audio::V1_0::IAudioManager RemoteIAudioManager;
typedef OHOS::HDI::DistributedAudio::Audio::V1_0::AudioRouteNode RemoteAudioRouteNode;

typedef std::function<int32_t(const AudioParamKey key, const char *condition, const char *value,
    void *reserved)> RemoteAdapterHdiCbFunc;

class RemoteAdapterHdiCallback final : public RemoteIAudioCallback {
public:
    RemoteAdapterHdiCallback(const RemoteAdapterHdiCbFunc cbFunc);
    ~RemoteAdapterHdiCallback() override {}

    int32_t RenderCallback(RemoteAudioCallbackType type, int8_t &reserved, int8_t &cookie) override;
    int32_t ParamCallback(RemoteAudioExtParamKey key, const std::string &condition, const std::string &value,
        int8_t &reserved, int8_t cookie) override;

private:
    RemoteAdapterHdiCbFunc cbFunc_;
};

class RemoteDeviceAdapter : public IDeviceAdapter {
    friend class RemoteDeviceManager;
public:
    RemoteDeviceAdapter(const std::string &adapterName, sptr<RemoteIAudioAdapter> adapter,
        RemoteAudioAdapterDescriptor adapterDesc);
    ~RemoteDeviceAdapter() = default;

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

    void RegistRenderSinkCallback(uint32_t hdiRenderId, IDeviceAdapterCallback *callback) override;
    void RegistCaptureSourceCallback(uint32_t hdiCaptureId, IDeviceAdapterCallback *callback) override;
    void UnRegistRenderSinkCallback(uint32_t hdiRenderId) override;
    void UnRegistCaptureSourceCallback(uint32_t hdiCaptureId) override;

    uint32_t GetPortId(RemoteAudioPortDirection portFlag);
    int32_t HandleEvent(const AudioParamKey key, const char *condition, const char *value, void *reserved);
    int32_t HandleStateChangeEvent(const AudioParamKey key, const char *condition, const char *value);
    int32_t HandleRenderParamEvent(const AudioParamKey key, const char *condition, const char *value);
    int32_t HandleCaptureParamEvent(const AudioParamKey key, const char *condition, const char *value);
    int32_t SetOutputPortPin(DeviceType outputDevice, RemoteAudioRouteNode &sink);
    int32_t SetInputPortPin(DeviceType inputDevice, RemoteAudioRouteNode &source);

private:
    static constexpr int32_t EVENT_DES_SIZE = 60;
    static constexpr int32_t ADAPTER_STATE_CONTENT_DES_SIZE = 60;
    static constexpr int32_t PARAMS_STATE_NUM = 2;
    static constexpr char DAUDIO_DEV_TYPE_SPK = '1';
    static constexpr char DAUDIO_DEV_TYPE_MIC = '2';

    std::string adapterName_ = "";
    sptr<RemoteIAudioAdapter> adapter_ = nullptr;
    RemoteAudioAdapterDescriptor adapterDesc_ = {};
    std::unordered_set<uint32_t> hdiRenderIds_;
    std::unordered_set<uint32_t> hdiCaptureIds_;
    std::mutex renderMtx_;
    std::mutex captureMtx_;
    int32_t routeHandle_ = -1;
    std::unordered_map<uint32_t, IDeviceManagerCallback *> renderCallbacks_;
    std::unordered_map<uint32_t, IDeviceManagerCallback *> captureCallbacks_;
    std::mutex renderCallbackMtx_;
    std::mutex captureCallbackMtx_;
    RemoteAdapterHdiCallback hdiCallback_;
};

} // namespace AudioStandard
} // namespace OHOS

#endif // REMOTE_DEVICE_ADAPTER_H
