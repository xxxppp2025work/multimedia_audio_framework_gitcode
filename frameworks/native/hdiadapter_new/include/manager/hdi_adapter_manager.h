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

#ifndef HDI_ADAPTER_MANAGER_H
#define HDI_ADAPTER_MANAGER_H

#include <iostream>
#include <cstring>
#include <mutex>
#include <unordered_map>
#include <memory>
#include "common/hdi_adapter_info.h"
#include "sink/i_audio_render_sink.h"
#include "source/i_audio_capture_source.h"
#include "adapter/i_device_manager.h"

namespace OHOS {
namespace AudioStandard {
typedef struct RenderSinkInfo {
    std::shared_ptr<IAudioRenderSink> sink_;
    std::atomic<uint32_t> refCount_ = 0;
} RenderSinkInfo;

typedef struct CaptureSourceInfo {
    std::shared_ptr<IAudioCaptureSource> source_;
    std::atomic<uint32_t> refCount_ = 0;
} CaptureSourceInfo;

class HdiAdapterManager {
public:
    static HdiAdapterManager &GetInstance(void);

    uint32_t GetId(uint32_t base, uint32_t type, const std::string &info = HDI_ADAPTER_ID_INFO_DEFAULT,
        bool isResident = false);
    uint32_t GetRenderIdByDeviceClass(const std::string &deviceClass,
        const std::string &info = HDI_ADAPTER_ID_INFO_DEFAULT, bool isResident = false);
    uint32_t GetCaptureIdByDeviceClass(const std::string &deviceClass, const SourceType sourceType,
        const std::string &info = HDI_ADAPTER_ID_INFO_DEFAULT, bool isResident = false);
    void ReleaseId(uint32_t &id);
    std::shared_ptr<IAudioRenderSink> GetRenderSink(uint32_t renderId, bool tryCreate = false);
    std::shared_ptr<IAudioCaptureSource> GetCaptureSource(uint32_t captureId, bool tryCreate = false);
    int32_t ProcessCaptureSource(const std::function<bool(uint32_t)> &limitFunc,
        const std::function<int32_t(std::shared_ptr<IAudioCaptureSource>)> &processFunc);
    std::shared_ptr<IDeviceManager> GetDeviceManager(uint32_t type);
    void ReleaseDeviceManager(uint32_t type);
    int32_t LoadAdapter(uint32_t deviceManagerType, const std::string &adapterName);
    void UnloadAdapter(uint32_t deviceManagerType, const std::string &adapterName, bool force = false);
    void DumpInfo(std::string &dumpString);

private:
    HdiAdapterManager() = default;
    ~HdiAdapterManager();
    HdiAdapterManager(const HdiAdapterManager &) = delete;
    HdiAdapterManager &operator=(const HdiAdapterManager &) = delete;
    HdiAdapterManager(HdiAdapterManager &&) = delete;
    HdiAdapterManager &operator=(HdiAdapterManager &&) = delete;

    void IncRefCount(uint32_t id);
    void DecRefCount(uint32_t id);

private:
    std::unordered_map<uint32_t, RenderSinkInfo> renderSinks_;
    std::unordered_map<uint32_t, CaptureSourceInfo> captureSources_;
    std::shared_ptr<IDeviceManager> deviceManagers_[HDI_DEVICE_MANAGER_TYPE_NUM];
    std::mutex renderSinkMtx_;
    std::mutex captureSourceMtx_;
    std::mutex deviceManagerMtx_;
};

} // namespace AudioStandard
} // namespace OHOS

#endif // HDI_ADAPTER_MANAGER_H
