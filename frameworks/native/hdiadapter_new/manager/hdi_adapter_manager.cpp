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
#define LOG_TAG "HdiAdapterManager"
#endif

#include "manager/hdi_adapter_manager.h"
#include "audio_hdi_log.h"
#include "audio_errors.h"
#include "util/id_handler.h"
#include "manager/hdi_adapter_factory.h"

namespace OHOS {
namespace AudioStandard {
HdiAdapterManager::~HdiAdapterManager()
{
    renderSinkMtx_.lock();
    renderSinks_.clear();
    renderSinkMtx_.unlock();

    captureSourceMtx_.lock();
    captureSources_.clear();
    captureSourceMtx_.unlock();

    std::lock_guard<std::mutex> lock(deviceManagerMtx_);
    for (uint32_t i = 0; i < HDI_DEVICE_MANAGER_TYPE_NUM; ++i) {
        if (deviceManagers_[i] == nullptr) {
            continue;
        }
        deviceManagers_[i].reset();
    }
}

HdiAdapterManager &HdiAdapterManager::GetInstance(void)
{
    static HdiAdapterManager instance;
    return instance;
}

uint32_t HdiAdapterManager::CreateRenderSink(const IAudioSinkAttr &attr, HdiIdType type, const std::string &info)
{
    uint32_t id = IdHandler::GetInstance().GetId(HDI_ID_BASE_RENDER, type, info);
    CHECK_AND_RETURN_RET(id != HDI_INVALID_ID, HDI_INVALID_ID);

    std::lock_guard<std::mutex> lock(renderSinkMtx_);
    if (renderSinks_.count(id) == 0) {
        IdHandler::GetInstance().IncInfoIdUseCount(id);
        AUDIO_INFO_LOG("create sink, renderId: %{public}d, type: %{public}u, info: %{public}s", id, type, info.c_str());
        std::shared_ptr<IAudioRenderSink> sink = HdiAdapterFactory::GetInstance().CreateRenderSink(id);
        if (sink == nullptr) {
            AUDIO_ERR_LOG("create sink fail, renderId: %{public}u", id);
            IdHandler::GetInstance().DecInfoIdUseCount(id);
            return HDI_INVALID_ID;
        }
        DoRegistSinkCallback(id, sink);
        sink->Init(attr);
        renderSinks_[id].sink_ = sink;
    }
    renderSinks_[id].refCount_++;
    return id;
}

uint32_t HdiAdapterManager::CreateRenderSink(const IAudioSinkAttr &attr, const std::string &deviceClass,
    const std::string &info)
{
    uint32_t id = IdHandler::GetInstance().GetRenderIdByDeviceClass(deviceClass, info);
    CHECK_AND_RETURN_RET(id != HDI_INVALID_ID, HDI_INVALID_ID);

    std::lock_guard<std::mutex> lock(renderSinkMtx_);
    if (renderSinks_.count(id) == 0) {
        IdHandler::GetInstance().IncInfoIdUseCount(id);
        AUDIO_INFO_LOG("create sink, renderId: %{public}d, deviceClass: %{public}s, info: %{public}s", id,
            deviceClass.c_str(), info.c_str());
        std::shared_ptr<IAudioRenderSink> sink = HdiAdapterFactory::GetInstance().CreateRenderSink(id);
        if (sink == nullptr) {
            AUDIO_ERR_LOG("create sink fail, renderId: %{public}u", id);
            IdHandler::GetInstance().DecInfoIdUseCount(id);
            return HDI_INVALID_ID;
        }
        DoRegistSinkCallback(id, sink);
        if (attr.adapterName != nullptr) {
            sink->Init(attr);
        }
        renderSinks_[id].sink_ = sink;
    }
    renderSinks_[id].refCount_++;
    return id;
}

uint32_t HdiAdapterManager::CreateCaptureSource(const IAudioSourceAttr &attr, HdiIdType type, const std::string &info)
{
    uint32_t id = IdHandler::GetInstance().GetId(HDI_ID_BASE_CAPTURE, type, info);
    CHECK_AND_RETURN_RET(id != HDI_INVALID_ID, HDI_INVALID_ID);

    std::lock_guard<std::mutex> lock(captureSourceMtx_);
    if (captureSources_.count(id) == 0) {
        IdHandler::GetInstance().IncInfoIdUseCount(id);
        AUDIO_INFO_LOG("create source, captureId: %{public}d, type: %{public}u, info: %{public}s", id, type,
            info.c_str());
        std::shared_ptr<IAudioCaptureSource> source = HdiAdapterFactory::GetInstance().CreateCaptureSource(id);
        if (source == nullptr) {
            AUDIO_ERR_LOG("create source fail, captureId: %{public}u", id);
            IdHandler::GetInstance().DecInfoIdUseCount(id);
            return HDI_INVALID_ID;
        }
        DoRegistSourceCallback(id, source);
        if (attr.adapterName != nullptr) {
            source->Init(attr);
        }
        captureSources_[id].source_ = source;
    }
    captureSources_[id].refCount_++;
    return id;
}

uint32_t HdiAdapterManager::CreateCaptureSource(const IAudioSourceAttr &attr, const std::string &deviceClass,
    const SourceType sourceType, const std::string &info)
{
    uint32_t id = IdHandler::GetInstance().GetCaptureIdByDeviceClass(deviceClass, sourceType, info);
    CHECK_AND_RETURN_RET(id != HDI_INVALID_ID, HDI_INVALID_ID);

    std::lock_guard<std::mutex> lock(captureSourceMtx_);
    if (captureSources_.count(id) == 0) {
        IdHandler::GetInstance().IncInfoIdUseCount(id);
        AUDIO_INFO_LOG("create source, captureId: %{public}d, deviceClass: %{public}s, sourceType: %{public}d, "
            "info: %{public}s", id, deviceClass.c_str(), sourceType, info.c_str());
        std::shared_ptr<IAudioCaptureSource> source = HdiAdapterFactory::GetInstance().CreateCaptureSource(id);
        if (source == nullptr) {
            AUDIO_ERR_LOG("create source fail, captureId: %{public}u", id);
            IdHandler::GetInstance().DecInfoIdUseCount(id);
            return HDI_INVALID_ID;
        }
        DoRegistSourceCallback(id, source);
        source->Init(attr);
        captureSources_[id].source_ = source;
    }
    captureSources_[id].refCount_++;
    return id;
}

void HdiAdapterManager::ReleaseRenderSink(uint32_t &renderId)
{
    uint32_t tempId = renderId;
    renderId = HDI_INVALID_ID;
    CHECK_AND_RETURN(tempId != HDI_INVALID_ID && renderSinks_.count(tempId));

    std::lock_guard<std::mutex> lock(renderSinkMtx_);
    if (renderSinks_[tempId].refCount_.load() > 0) {
        renderSinks_[tempId].refCount_--;
    }
    CHECK_AND_RETURN(renderSinks_[tempId].refCount_.load() == 0);
    AUDIO_INFO_LOG("no reference of renderId %{public}u, try remove the sink", tempId);
    std::shared_ptr<IAudioRenderSink> sink = renderSinks_[tempId].sink_;
    if (sink->IsInited()) {
        sink->DeInit();
    }
    renderSinks_.erase(tempId);
    IdHandler::GetInstance().DecInfoIdUseCount(tempId);
}

void HdiAdapterManager::ReleaseCaptureSource(uint32_t &captureId)
{
    uint32_t tempId = captureId;
    captureId = HDI_INVALID_ID;
    CHECK_AND_RETURN(tempId != HDI_INVALID_ID && captureSources_.count(tempId));

    std::lock_guard<std::mutex> lock(captureSourceMtx_);
    if (captureSources_[tempId].refCount_.load() > 0) {
        captureSources_[tempId].refCount_--;
    }
    CHECK_AND_RETURN(captureSources_[tempId].refCount_.load() == 0);
    AUDIO_INFO_LOG("no reference of captureId %{public}u, try remove the source", tempId);
    std::shared_ptr<IAudioCaptureSource> source = captureSources_[tempId].source_;
    if (source->IsInited()) {
        source->DeInit();
    }
    captureSources_.erase(tempId);
    IdHandler::GetInstance().DecInfoIdUseCount(tempId);
}

uint32_t HdiAdapterManager::GetRenderSink(HdiIdType type, const std::string &info)
{
    uint32_t id = IdHandler::GetInstance().GetId(HDI_ID_BASE_RENDER, type, info);
    CHECK_AND_RETURN_RET(id != HDI_INVALID_ID, HDI_INVALID_ID);

    std::lock_guard<std::mutex> lock(renderSinkMtx_);
    return renderSinks_.count(id) ? id : HDI_INVALID_ID;
}

uint32_t HdiAdapterManager::GetRenderSink(const std::string &deviceClass, const std::string &info)
{
    uint32_t id = IdHandler::GetInstance().GetRenderIdByDeviceClass(deviceClass, info);
    CHECK_AND_RETURN_RET(id != HDI_INVALID_ID, HDI_INVALID_ID);

    std::lock_guard<std::mutex> lock(renderSinkMtx_);
    return renderSinks_.count(id) ? id : HDI_INVALID_ID;
}

uint32_t HdiAdapterManager::GetCaptureSource(HdiIdType type, const std::string &info)
{
    uint32_t id = IdHandler::GetInstance().GetId(HDI_ID_BASE_CAPTURE, type, info);
    CHECK_AND_RETURN_RET(id != HDI_INVALID_ID, HDI_INVALID_ID);

    std::lock_guard<std::mutex> lock(captureSourceMtx_);
    return captureSources_.count(id) ? id : HDI_INVALID_ID;
}

uint32_t HdiAdapterManager::GetCaptureSource(const std::string &deviceClass, const SourceType sourceType,
    const std::string &info)
{
    uint32_t id = IdHandler::GetInstance().GetCaptureIdByDeviceClass(deviceClass, sourceType, info);
    CHECK_AND_RETURN_RET(id != HDI_INVALID_ID, HDI_INVALID_ID);

    std::lock_guard<std::mutex> lock(captureSourceMtx_);
    return captureSources_.count(id) ? id : HDI_INVALID_ID;
}

std::vector<uint32_t> HdiAdapterManager::GetRenderSinkList(const HdiIdDesc &renderIdDesc)
{
    std::vector<uint32_t> renderIds;
    auto limitFunc = GetLimicFunc(renderIdDesc);
    std::lock_guard<std::mutex> lock(renderSinkMtx_);
    for (auto &item : renderSinks_) {
        if (limitFunc(item.first)) {
            renderIds.push_back(item.first);
        }
    }
    return renderIds;
}

std::vector<uint32_t> HdiAdapterManager::GetCaptureSourceList(const HdiIdDesc &captureIdDesc)
{
    std::vector<uint32_t> captureIds;
    auto limitFunc = GetLimicFunc(captureIdDesc);
    std::lock_guard<std::mutex> lock(captureSourceMtx_);
    for (auto &item : captureSources_) {
        if (limitFunc(item.first)) {
            captureIds.push_back(item.first);
        }
    }
    return captureIds;
}

template <typename CbPtr>
void HdiAdapterManager::RegistSinkCallback(HdiAdapterCallbackType type, CbPtr cb, const HdiIdDesc &renderIdDesc)
{
    CHECK_AND_RETURN_LOG(cb != nullptr, "callback of type %{public}u is nullptr", type);

    sinkCbs_.RegistCallback(type, cb);
    cbLimitFunc_[HDI_ID_BASE_RENDER][type] = GetLimicFunc(renderIdDesc);
    AUDIO_INFO_LOG("regist sink callback succ, type: %{public}u", type);
}

template <typename CbPtr>
void HdiAdapterManager::RegistSourceCallback(HdiAdapterCallbackType type, CbPtr cb, const HdiIdDesc &captureIdDesc)
{
    CHECK_AND_RETURN_LOG(cb != nullptr, "callback of type %{public}u is nullptr", type);

    sourceCbs_.RegistCallback(type, cb);
    cbLimitFunc_[HDI_ID_BASE_CAPTURE][type] = GetLimicFunc(captureIdDesc);
    AUDIO_INFO_LOG("regist source callback succ, type: %{public}u", type);
}

int32_t HdiAdapterManager::LoadAdapter(HdiDeviceManagerType type, const std::string &adapterName)
{
    std::shared_ptr<IDeviceManager> deviceManager = GetDeviceManagerHandle(type);
    CHECK_AND_RETURN_RET(deviceManager != nullptr, HDI_INVALID_ID);
    return deviceManager->LoadAdapter(adapterName);
}

void HdiAdapterManager::UnloadAdapter(HdiDeviceManagerType type, const std::string &adapterName, bool force)
{
    std::shared_ptr<IDeviceManager> deviceManager = GetDeviceManagerHandle(type);
    CHECK_AND_RETURN(deviceManager != nullptr);
    deviceManager->UnloadAdapter(adapterName, force);
}

void HdiAdapterManager::DumpInfo(std::string &dumpString)
{
    dumpString += "- adapter\n";
    deviceManagerMtx_.lock();
    for (auto &item : deviceManagers_) {
        if (item != nullptr) {
            item->DumpInfo(dumpString);
        }
    }
    deviceManagerMtx_.unlock();

    if (!renderSinks_.empty()) {
        dumpString += "\n- render\n";
        renderSinkMtx_.lock();
        for (auto &item : renderSinks_) {
            if (item.second.sink_ == nullptr ||  !item.second.sink_->IsInited()) {
                continue;
            }
            dumpString += "  - id: " + std::to_string(item.first) + "\trefCount: " +
                std::to_string(item.second.refCount_.load()) + "\t";
            item.second.sink_->DumpInfo(dumpString);
        }
        renderSinkMtx_.unlock();
    }

    if (!captureSources_.empty()) {
        dumpString += "\n- capture\n";
        captureSourceMtx_.lock();
        for (auto &item : captureSources_) {
            if (item.second.source_ == nullptr || !item.second.source_->IsInited()) {
                continue;
            }
            dumpString += "  - id: " + std::to_string(item.first) + "\trefCount: " +
                std::to_string(item.second.refCount_.load()) + "\t";
            item.second.source_->DumpInfo(dumpString);
        }
        captureSourceMtx_.unlock();
    }
}

std::function<bool(uint32_t)> HdiAdapterManager::GetLimicFunc(const HdiIdDesc &idDesc)
{
    return [&idDesc](uint32_t id) -> bool {
        uint32_t idType = IdHandler::GetInstance().ParseType(id);
        std::string idInfo = IdHandler::GetInstance().ParseInfo(id);
        if (idDesc.size() == 0) {
            return true;
        }
        for (auto &item : idDesc) {
            if (idType != item.first) {
                continue;
            }
            if (item.second.size() == 0) {
                return true;
            }
            for (auto &info : item.second) {
                if (idInfo == info) {
                    return true;
                }
            }
        }
        return false;
    };
}

void HdiAdapterManager::DoRegistSinkCallback(uint32_t id, std::shared_ptr<IAudioRenderSink> sink)
{
    CHECK_AND_RETURN_LOG(sink != nullptr, "sink is nullptr");

    for (uint32_t type = 0; type < HDI_CB_TYPE_NUM; ++type) {
        auto cb = sinkCbs_.GetCallback(type);
        auto rawCb = sinkCbs_.GetRawCallback(type);
        if (cbLimitFunc_[HDI_ID_BASE_RENDER][type] == nullptr || !cbLimitFunc_[HDI_ID_BASE_RENDER][type](id)) {
            continue;
        }
        if (cb != nullptr) {
            sink->RegistCallback(type, cb);
        } else if (rawCb != nullptr) {
            sink->RegistCallback(type, rawCb);
        } else {
            AUDIO_ERR_LOG("callback is nullptr, callback type: %{public}u", type);
        }
    }
}

void HdiAdapterManager::DoRegistSourceCallback(uint32_t id, std::shared_ptr<IAudioCaptureSource> source)
{
    CHECK_AND_RETURN_LOG(source != nullptr, "source is nullptr");

    for (uint32_t type = 0; type < HDI_CB_TYPE_NUM; ++type) {
        auto cb = sourceCbs_.GetCallback(type);
        auto rawCb = sourceCbs_.GetRawCallback(type);
        if (cbLimitFunc_[HDI_ID_BASE_CAPTURE][type] == nullptr || !cbLimitFunc_[HDI_ID_BASE_CAPTURE][type](id)) {
            continue;
        }
        if (cb != nullptr) {
            source->RegistCallback(type, cb);
        } else if (rawCb != nullptr) {
            source->RegistCallback(type, rawCb);
        } else {
            AUDIO_ERR_LOG("callback is nullptr, callback type: %{public}u", type);
        }
    }
}

std::shared_ptr<IAudioRenderSink> HdiAdapterManager::GetRenderSinkHandle(uint32_t renderId)
{
    std::lock_guard<std::mutex> lock(renderSinkMtx_);
    CHECK_AND_RETURN_RET_LOG(renderSinks_.count(renderId) != 0, nullptr, "no valid sink, renderId: %{public}u",
        renderId);
    return renderSinks_[renderId].sink_;
}

std::shared_ptr<IAudioCaptureSource> HdiAdapterManager::GetCaptureSourceHandle(uint32_t captureId)
{
    std::lock_guard<std::mutex> lock(captureSourceMtx_);
    CHECK_AND_RETURN_RET_LOG(captureSources_.count(captureId) != 0, nullptr, "no valid source, captureId: %{public}u",
        captureId);
    return captureSources_[captureId].source_;
}

std::shared_ptr<IDeviceManager> HdiAdapterManager::GetDeviceManagerHandle(HdiDeviceManagerType type)
{
    CHECK_AND_RETURN_RET_LOG(type < HDI_DEVICE_MANAGER_TYPE_NUM, nullptr, "invalid type %{public}u", type);

    std::lock_guard<std::mutex> lock(deviceManagerMtx_);
    if (deviceManagers_[type] == nullptr) {
        deviceManagers_[type] = HdiAdapterFactory::GetInstance().CreateDeviceManager(type);
    }
    return deviceManagers_[type];
}

} // namespace AudioStandard
} // namespace OHOS
