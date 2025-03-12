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
#include <functional>
#include <utility>
#include "common/hdi_adapter_info.h"
#include "sink/i_audio_render_sink.h"
#include "source/i_audio_capture_source.h"
#include "adapter/i_device_manager.h"
#include "util/callback_wrapper.h"

// generate func, real func name is prefix + func name
// eg: IAudioRenderSink::Init(const IAudioSinkAttr *attr) -> SinkInit(uint32_t id, const IAudioSinkAttr *attr)
#define SINK_FORWARD_FUNC(func, default_ret) \
    FORWARD_FUNC(Sink, func, default_ret, GetRenderSinkHandle)
#define SINK_FORWARD_FUNC_VOID(func) \
    FORWARD_FUNC_VOID(Sink, func, GetRenderSinkHandle)

#define SOURCE_FORWARD_FUNC(func, default_ret) \
    FORWARD_FUNC(Source, func, default_ret, GetCaptureSourceHandle)
#define SOURCE_FORWARD_FUNC_VOID(func) \
    FORWARD_FUNC_VOID(Source, func, GetCaptureSourceHandle)

#define DEV_MGR_FORWARD_FUNC(func, default_ret) \
    FORWARD_FUNC(DevMgr, func, default_ret, GetDeviceManagerHandle)
#define DEV_MGR_FORWARD_FUNC_VOID(func) \
    FORWARD_FUNC_VOID(DevMgr, func, GetDeviceManagerHandle)

#define FORWARD_FUNC(prefix, func, default_ret, get_handle_func) \
    template <typename T, typename... Args> \
    auto prefix##func(T id, Args &&... args) -> decltype(get_handle_func(id)->func(std::forward<Args>(args)...)) \
    { \
        auto item = get_handle_func(id); \
        if (item == nullptr) { return default_ret; } \
        return item->func(std::forward<Args>(args)...); \
    }

#define FORWARD_FUNC_VOID(prefix, func, get_handle_func) \
    template <typename T, typename... Args> \
    auto prefix##func(T id, Args &&... args) -> decltype(get_handle_func(id)->func(std::forward<Args>(args)...)) \
    { \
        auto item = get_handle_func(id); \
        if (item == nullptr) { return; } \
        item->func(std::forward<Args>(args)...); \
    }

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

typedef std::vector<std::pair<HdiIdType, std::vector<std::string> > > HdiIdDesc;

class HdiAdapterManager {
public:
    static HdiAdapterManager &GetInstance(void);

    uint32_t CreateRenderSink(const IAudioSinkAttr &attr, HdiIdType type,
        const std::string &info = HDI_ID_INFO_DEFAULT);
    uint32_t CreateRenderSink(const IAudioSinkAttr &attr, const std::string &deviceClass,
        const std::string &info = HDI_ID_INFO_DEFAULT);
    uint32_t CreateCaptureSource(const IAudioSourceAttr &attr, HdiIdType type,
        const std::string &info = HDI_ID_INFO_DEFAULT);
    uint32_t CreateCaptureSource(const IAudioSourceAttr &attr, const std::string &deviceClass,
        const SourceType sourceType, const std::string &info = HDI_ID_INFO_DEFAULT);

    void ReleaseRenderSink(uint32_t &renderId);
    void ReleaseCaptureSource(uint32_t &captureId);

    uint32_t GetRenderSink(HdiIdType type, const std::string &info = HDI_ID_INFO_DEFAULT);
    uint32_t GetRenderSink(const std::string &deviceClass, const std::string &info = HDI_ID_INFO_DEFAULT);
    uint32_t GetCaptureSource(HdiIdType type, const std::string &info = HDI_ID_INFO_DEFAULT);
    uint32_t GetCaptureSource(const std::string &deviceClass, const SourceType sourceType,
        const std::string &info = HDI_ID_INFO_DEFAULT);

    std::vector<uint32_t> GetRenderSinkList(const HdiIdDesc &renderIdDesc);
    std::vector<uint32_t> GetCaptureSourceList(const HdiIdDesc &captureIdDesc);

    int32_t LoadAdapter(HdiDeviceManagerType type, const std::string &adapterName);
    void UnloadAdapter(HdiDeviceManagerType type, const std::string &adapterName, bool force = false);

    template <typename CbPtr>
    void RegistSinkCallback(HdiAdapterCallbackType type, CbPtr cb, const HdiIdDesc &renderIdDesc);
    template <typename CbPtr>
    void RegistSourceCallback(HdiAdapterCallbackType type, CbPtr cb, const HdiIdDesc &renderIdDesc);

    void DumpInfo(std::string &dumpString);

    // TODO: sink/source operation
    SINK_FORWARD_FUNC(Init, ERR_INVALID_HANDLE);


    // TODO: device adapter operation

private:
    HdiAdapterManager() = default;
    ~HdiAdapterManager();
    HdiAdapterManager(const HdiAdapterManager &) = delete;
    HdiAdapterManager &operator=(const HdiAdapterManager &) = delete;
    HdiAdapterManager(HdiAdapterManager &&) = delete;
    HdiAdapterManager &operator=(HdiAdapterManager &&) = delete;

    std::function<bool(uint32_t)> GetLimicFunc(const HdiIdDesc &idDesc);

    void DoRegistSinkCallback(uint32_t id, std::shared_ptr<IAudioRenderSink> sink);
    void DoRegistSourceCallback(uint32_t id, std::shared_ptr<IAudioCaptureSource> source);

    std::shared_ptr<IAudioRenderSink> GetRenderSinkHandle(uint32_t renderId);
    std::shared_ptr<IAudioCaptureSource> GetCaptureSourceHandle(uint32_t captureId);
    std::shared_ptr<IDeviceManager> GetDeviceManagerHandle(HdiDeviceManagerType type);

private:
    std::unordered_map<uint32_t, RenderSinkInfo> renderSinks_;
    std::unordered_map<uint32_t, CaptureSourceInfo> captureSources_;
    std::shared_ptr<IDeviceManager> deviceManagers_[HDI_DEVICE_MANAGER_TYPE_NUM];
    std::mutex renderSinkMtx_;
    std::mutex captureSourceMtx_;
    std::mutex deviceManagerMtx_;
    // callback
    SinkCallbackWrapper sinkCbs_;
    SourceCallbackWrapper sourceCbs_;
    std::function<bool(uint32_t)> cbLimitFunc_[HDI_ID_BASE_NUM][HDI_CB_TYPE_NUM];
};

} // namespace AudioStandard
} // namespace OHOS

#endif // HDI_ADAPTER_MANAGER_H
