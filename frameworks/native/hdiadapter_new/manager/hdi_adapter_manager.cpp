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

    deviceAdapterMtx_.lock();
    deviceAdapters_.clear();
    deviceAdapterMtx_.unlock();

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
    uint32_t renderId = IdHandler::GetInstance().GetId(HDI_ID_BASE_RENDER, type, info);
    CHECK_AND_RETURN_RET(renderId != HDI_INVALID_ID, HDI_INVALID_ID);

    std::lock_guard<std::mutex> lock(renderSinkMtx_);
    int32_t ret = DoCreateRenderSink(renderId, attr, false);
    CHECK_AND_RETURN_RET(ret == SUCCESS, HDI_INVALID_ID);
    renderSinks_[renderId].refCount_++;

    AUDIO_INFO_LOG("type: %{public}u, info: %{public}s, renderId: %{public}u, refCount: %{public}u", type, info.c_str(),
        renderId, renderSinks_[renderId].refCount_.load());
    return renderId;
}

uint32_t HdiAdapterManager::CreateRenderSink(const IAudioSinkAttr &attr, const std::string &deviceClass,
    const std::string &info)
{
    uint32_t renderId = IdHandler::GetInstance().GetRenderIdByDeviceClass(deviceClass, info);
    CHECK_AND_RETURN_RET(renderId != HDI_INVALID_ID, HDI_INVALID_ID);

    std::lock_guard<std::mutex> lock(renderSinkMtx_);
    int32_t ret = DoCreateRenderSink(renderId, attr, false);
    CHECK_AND_RETURN_RET(ret == SUCCESS, HDI_INVALID_ID);
    renderSinks_[renderId].refCount_++;

    AUDIO_INFO_LOG("deviceClass: %{public}s, renderId: %{public}u, refCount: %{public}u", deviceClass.c_str(), renderId,
        renderSinks_[renderId].refCount_.load());
    return renderId;
}

uint32_t HdiAdapterManager::CreateCaptureSource(const IAudioSourceAttr &attr, HdiIdType type, const std::string &info)
{
    uint32_t captureId = IdHandler::GetInstance().GetId(HDI_ID_BASE_CAPTURE, type, info);
    CHECK_AND_RETURN_RET(captureId != HDI_INVALID_ID, HDI_INVALID_ID);

    std::lock_guard<std::mutex> lock(captureSourceMtx_);
    int32_t ret = DoCreateCaptureSource(captureId, attr, false);
    CHECK_AND_RETURN_RET(ret == SUCCESS, HDI_INVALID_ID);
    captureSources_[captureId].refCount_++;

    AUDIO_INFO_LOG("type: %{public}u, info: %{public}s, captureId: %{public}u, refCount: %{public}u", type,
        info.c_str(), captureId, captureSources_[captureId].refCount_.load());
    return captureId;
}

uint32_t HdiAdapterManager::CreateCaptureSource(const IAudioSourceAttr &attr, const std::string &deviceClass,
    const SourceType sourceType, const std::string &info)
{
    uint32_t captureId = IdHandler::GetInstance().GetCaptureIdByDeviceClass(deviceClass, sourceType, info);
    CHECK_AND_RETURN_RET(captureId != HDI_INVALID_ID, HDI_INVALID_ID);

    std::lock_guard<std::mutex> lock(captureSourceMtx_);
    int32_t ret = DoCreateCaptureSource(captureId, attr, false);
    CHECK_AND_RETURN_RET(ret == SUCCESS, HDI_INVALID_ID);
    captureSources_[captureId].refCount_++;

    AUDIO_INFO_LOG("deviceClass: %{public}s, sourceType: %{public}d, captureId: %{public}u, refCount: %{public}u",
        deviceClass.c_str(), sourceType, captureId, captureSources_[captureId].refCount_.load());
    return captureId;
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
    captureSources_.erase(tempId);
    IdHandler::GetInstance().DecInfoIdUseCount(tempId);
}

uint32_t HdiAdapterManager::GetRenderSink(HdiIdType type, const std::string &info, bool needPreload)
{
    uint32_t renderId = IdHandler::GetInstance().GetId(HDI_ID_BASE_RENDER, type, info);
    CHECK_AND_RETURN_RET(renderId != HDI_INVALID_ID, HDI_INVALID_ID);

    std::lock_guard<std::mutex> lock(renderSinkMtx_);
    if (!needPreload) {
        return renderSinks_.count(renderId) == 0 ? HDI_INVALID_ID : renderId;
    }
    int32_t ret = DoCreateRenderSink(renderId, {}, true);
    CHECK_AND_RETURN_RET(ret == SUCCESS, HDI_INVALID_ID);

    return renderId;
}

uint32_t HdiAdapterManager::GetRenderSink(const std::string &deviceClass, const std::string &info, bool needPreload)
{
    uint32_t renderId = IdHandler::GetInstance().GetRenderIdByDeviceClass(deviceClass, info);
    CHECK_AND_RETURN_RET(renderId != HDI_INVALID_ID, HDI_INVALID_ID);

    std::lock_guard<std::mutex> lock(renderSinkMtx_);
    if (!needPreload) {
        return renderSinks_.count(renderId) == 0 ? HDI_INVALID_ID : renderId;
    }
    int32_t ret = DoCreateRenderSink(renderId, {}, true);
    CHECK_AND_RETURN_RET(ret == SUCCESS, HDI_INVALID_ID);

    return renderId;
}

uint32_t HdiAdapterManager::GetCaptureSource(HdiIdType type, const std::string &info, bool needPreload)
{
    uint32_t captureId = IdHandler::GetInstance().GetId(HDI_ID_BASE_CAPTURE, type, info);
    CHECK_AND_RETURN_RET(captureId != HDI_INVALID_ID, HDI_INVALID_ID);

    std::lock_guard<std::mutex> lock(captureSourceMtx_);
    if (!needPreload) {
        return captureSources_.count(captureId) == 0 ? HDI_INVALID_ID : captureId;
    }
    int32_t ret = DoCreateCaptureSource(captureId, {}, true);
    CHECK_AND_RETURN_RET(ret == SUCCESS, HDI_INVALID_ID);

    return captureId;
}

uint32_t HdiAdapterManager::GetCaptureSource(const std::string &deviceClass, const SourceType sourceType,
    const std::string &info)
{
    uint32_t captureId = IdHandler::GetInstance().GetCaptureIdByDeviceClass(deviceClass, sourceType, info);
    CHECK_AND_RETURN_RET(captureId != HDI_INVALID_ID, HDI_INVALID_ID);

    std::lock_guard<std::mutex> lock(captureSourceMtx_);
    if (!needPreload) {
        return captureSources_.count(captureId) == 0 ? HDI_INVALID_ID : captureId;
    }
    int32_t ret = DoCreateCaptureSource(captureId, {}, true);
    CHECK_AND_RETURN_RET(ret == SUCCESS, HDI_INVALID_ID);

    return captureId;
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

int32_t HdiAdapterManager::LoadAdapter(HdiDeviceManagerType type, const std::string &adapterName)
{
    std::lock_guard<std::mutex> lock(deviceAdapterMtx_);
    CHECK_AND_RETURN_RET_LOG(deviceAdapters_.count(adapterName) == 0 || deviceAdapters_[adapterName] == nullptr,
        SUCCESS, "adapter %{public}s already loaded", adapterName.c_str());

    AUDIO_INFO_LOG("load adapter, adapterName: %{public}s", adapterName.c_str());
    std::shared_ptr<IDeviceManager> deviceManager = GetDeviceManagerHandle(type);
    CHECK_AND_RETURN_RET(deviceManager != nullptr, ERR_INVALID_HANDLE);
    bool needReInitManager = true;
    for (auto &item : deviceAdapters_) {
        if (item.second != nullptr && item.second->GetDeviceManagerType() == type) {
            needReInitManager = false;
            break;
        }
    }
    std::shared_ptr<IDeviceAdapter> deviceAdapter = deviceManager->LoadAdapter(adapterName, needReInitManager);
    CHECK_AND_RETURN_RET(deviceAdapter != nullptr, ERR_OPERATION_FAILED);
    deviceAdapters_[adapterName] = deviceAdapter;

    return SUCCESS;
}

void HdiAdapterManager::UnloadAdapter(const std::string &adapterName, bool force)
{
    std::lock_guard<std::mutex> lock(deviceAdapterMtx_);
    CHECK_AND_RETURN(deviceAdapters_.count(adapterName) != 0);
    CHECK_AND_RETURN(force || deviceAdapters_[adapterName]->CheckStatus());

    AUDIO_INFO_LOG("unload adapter, adapterName: %{public}s", adapterName.c_str());
    HdiDeviceManagerType type = deviceAdapters_[adapterName]->GetDeviceManagerType();
    std::shared_ptr<IDeviceManager> deviceManager = GetDeviceManagerHandle(type);
    if (deviceManager != nullptr) {
        deviceManager->UnloadAdapter(deviceAdapters_[adapterName]);
    } else {
        AUDIO_WARNING_LOG("device manager is nullptr");
    }
    deviceAdapters_.erase(adapterName);
}

std::vector<std::string> GetAdapterList(HdiDeviceManagerType type)
{
    std::vector<std::string> adapterNames;
    std::lock_guard<std::mutex> lock(deviceAdapterMtx_);
    for (auto &item : deviceAdapters_) {
        if (item.second != nullptr && item.second->GetDeviceManagerType() == type) {
            adapterNames.push_back(item.first);
        }
    }
    return adapterNames;
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

void HdiAdapterManager::RegistSinkCallbackGenerator(HdiAdapterCallbackType type,
    const std::function<std::shared_ptr<IAudioSinkCallback>(uint32_t)> cbGenerator, const HdiIdDesc &renderIdDesc)
{
    CHECK_AND_RETURN_LOG(cbGenerator, "callback generator of type %{public}u is nullptr", type);

    sinkCbs_.RegistCallbackGenerator(type, cbGenerator);
    cbLimitFunc_[HDI_ID_BASE_RENDER][type] = GetLimicFunc(renderIdDesc);
    AUDIO_INFO_LOG("regist sink callback generator succ, type: %{public}u", type);
}

void HdiAdapterManager::RegistSourceCallbackGenerator(HdiAdapterCallbackType type,
    const std::function<std::shared_ptr<IAudioSourceCallback>(uint32_t)> cbGenerator, const HdiIdDesc &captureIdDesc)
{
    CHECK_AND_RETURN_LOG(cbGenerator, "callback generator of type %{public}u is nullptr", type);

    sourceCbs_.RegistCallbackGenerator(type, cbGenerator);
    cbLimitFunc_[HDI_ID_BASE_CAPTURE][type] = GetLimicFunc(captureIdDesc);
    AUDIO_INFO_LOG("regist source callback generator succ, type: %{public}u", type);
}

void HdiAdapterManager::DumpInfo(std::string &dumpString)
{
    dumpString += "- adapter\n";
    deviceAdapterMtx_.lock();
    for (auto &item : deviceAdapters_) {
        if (item != nullptr) {
            item->DumpInfo(dumpString);
        }
    }
    deviceAdapterMtx_.unlock();

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

std::string HdiAdapterManager::AdapterSetAudioParameter(const std::string &adapterName, HdiDeviceManagerType type,
    const AudioParamKey key, const std::string &condition, const std::string &value)
{
    auto deviceAdapter = GetDeviceAdapterHandle(adapterName);
    // LCOV_EXCL_START
    if (deviceAdapter == nullptr) {
        auto deviceManager = GetDeviceManagerHandle(type);
        CHECK_AND_RETURN(deviceManager != nullptr);
        deviceManager->SaveSetParameter(adapterName, key, condition, value);
        return "";
    }
    // LCOV_EXCL_STOP
    return deviceAdapter->SetAudioParameter(key, condition, value);
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

int32_t HdiAdapterManager::DoCreateRenderSink(uint32_t renderId, const IAudioSinkAttr &attr, bool isPreload)
{
    if (renderSinks_.count(renderId) != 0 && renderSinks_[renderId].sink_ != nullptr) {
        goto INIT_SINK;
    }

    AUDIO_INFO_LOG("create sink, renderId: %{public}u", renderId);
    std::shared_ptr<IAudioRenderSink> sink = HdiAdapterFactory::GetInstance().CreateRenderSink(renderId);
    CHECK_AND_RETURN_RET_LOG(sink != nullptr, ERR_OPERATION_FAILED, "create sink fail, renderId: %{public}u", renderId);
    DoRegistSinkCallback(renderId, sink);
    if (renderSinks_.count(renderId) == 0) {
        IdHandler::GetInstance().IncInfoIdUseCount(renderId);
    }
    renderSinks_[renderId].sink_ = sink;

INIT_SINK:
    if ((!isPreload) && (!renderSinks_[renderId]->IsInited())) {
        renderSinks_[renderId].sink_->Init(attr);
    }
    return SUCCESS;
}

int32_t HdiAdapterManager::DoCreateCaptureSource(uint32_t captureId, const IAudioSourceAttr &attr, bool isPreload)
{
    if (captureSources_.count(captureId) != 0 && captureSources_[captureId].source_ != nullptr) {
        goto INIT_SOURCE;
    }

    AUDIO_INFO_LOG("create source, captureId: %{public}u", captureId);
    std::shared_ptr<IAudioCaptureSource> source = HdiAdapterFactory::GetInstance().CreateCaptureSource(captureId);
    CHECK_AND_RETURN_RET_LOG(source != nullptr, ERR_OPERATION_FAILED, "create source fail, captureId: %{public}u", captureId);
    DoRegistSourceCallback(captureId, source);
    if (captureSources_.count(captureId) == 0) {
        IdHandler::GetInstance().IncInfoIdUseCount(captureId);
    }
    captureSources_[captureId].source_ = source;

INIT_SOURCE:
    if (!captureSources_[captureId]->IsInited()) {
        captureSources_[captureId].source_->Init(attr);
    }
    return SUCCESS;
}

void HdiAdapterManager::DoRegistSinkCallback(uint32_t renderId, std::shared_ptr<IAudioRenderSink> sink)
{
    CHECK_AND_RETURN_LOG(sink != nullptr, "sink is nullptr");

    for (uint32_t type = 0; type < HDI_CB_TYPE_NUM; ++type) {
        if (cbLimitFunc_[HDI_ID_BASE_RENDER][type] == nullptr || !cbLimitFunc_[HDI_ID_BASE_RENDER][type](renderId)) {
            continue;
        }
        auto cb = sinkCbs_.GetCallback(type, id);
        auto rawCb = sinkCbs_.GetRawCallback(type);
        if (cb != nullptr) {
            sink->RegistCallback(type, cb);
        } else if (rawCb != nullptr) {
            sink->RegistCallback(type, rawCb);
        } else {
            AUDIO_ERR_LOG("callback is nullptr, callback type: %{public}u", type);
        }
    }
}

void HdiAdapterManager::DoRegistSourceCallback(uint32_t captureId, std::shared_ptr<IAudioCaptureSource> source)
{
    CHECK_AND_RETURN_LOG(source != nullptr, "source is nullptr");

    for (uint32_t type = 0; type < HDI_CB_TYPE_NUM; ++type) {
        if (cbLimitFunc_[HDI_ID_BASE_CAPTURE][type] == nullptr || !cbLimitFunc_[HDI_ID_BASE_CAPTURE][type](captureId)) {
            continue;
        }
        auto cb = sourceCbs_.GetCallback(type, id);
        auto rawCb = sourceCbs_.GetRawCallback(type);
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

std::shared_ptr<IDeviceAdapter> GetDeviceAdapterHandle(const std::string &adapterName)
{
    std::lock_guard<std::mutex> lock(deviceAdapterMtx_);
    CHECK_AND_RETURN_RET_LOG(deviceAdapters_.count(adapterName) != 0, nullptr,
        "no valid adapter, adapterName: %{public}s", adapterName.c_str());
    return deviceAdapters_[adapterName];
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
