/*
 * Copyright (c) 2023-2025 Huawei Device Co., Ltd.
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
#define LOG_TAG "BluetoothScoManager"
#endif

#include "bluetooth_sco_manager.h"
#include "audio_errors.h"
#include "audio_common_log.h"
#include "audio_utils.h"
#include "bluetooth_device_utils.h"

namespace OHOS {
namespace Bluetooth {
using namespace AudioStandard;

BluetoothScoManager &BluetoothScoManager::GetInstance()
{
    static BluetoothScoManager scoManager;
    return scoManager;
}

BluetoothScoManager::BluetoothScoManager()
{
    hfpInstance_ = HandsFreeAudioGateway::GetProfile();
    currentScoState_ = AudioScoState::DISCONNECTED;
}

void BluetoothScoManager::UpdateScoState(HfpScoConnectState scoState, const BluetoothRemoteDevice &device)
{
    {
        std::unique_lock<std::mutex> stateLock(scoLock_);
        AUDIO_INFO_LOG("recv sco %{public}s state with %{public}s device and current sco state %{public}d",
            scoState == HfpScoConnectState::SCO_DISCONNECTED ? "diconnect" : "connect",
            IsSameHfpDevice(activeHfpDevice_, device) ? "same" : "not same", currentScoState_);
        switch (currentScoState_) {
            case AudioScoState::DISCONNECTED:
                UpdateScoStateWhenDisconnected(scoState, device);
                break;
            case AudioScoState::CONNECTED:
                UpdateScoStateWhenConnected(scoState, device);
                break;
            case AudioScoState::CONNECTING:
                UpdateScoStateWhenConnecting(scoState, device);
                break;
            case AudioScoState::DISCONNECTING:
                UpdateScoStateWhenDiconnecting(scoState, device);
                break;
            default:
                break;
        }
    }
}

void BluetoothScoManager::UpdateScoStateWhenDisconnected(HfpScoConnectState scoState, const BluetoothRemoteDevice &device)
{
    if (scoState == HfpScoConnectState::SCO_CONNECTED) {
        ForceUpdateScoState();
    }
}

void BluetoothScoManager::UpdateScoStateWhenConnected(HfpScoConnectState scoState, const BluetoothRemoteDevice &device)
{
    if (scoState == HfpScoConnectState::SCO_DISCONNECTED) {
        if (IsSameHfpDevice(activeHfpDevice_, device)) {
            currentScoState_ = AudioScoState::DISCONNECTED;
        }
    } else if (scoState == HfpScoConnectState::SCO_CONNECTED) {
        ForceUpdateScoState();
    }
}

void BluetoothScoManager::UpdateScoStateWhenConnecting(HfpScoConnectState scoState, const BluetoothRemoteDevice &device)
{
    if (scoState == HfpScoConnectState::SCO_DISCONNECTED) {
        if (IsSameHfpDevice(activeHfpDevice_, device)) {
            currentScoState_ = AudioScoState::DISCONNECTED;
        }
    } else if (scoState == HfpScoConnectState::SCO_CONNECTED) {
        if (IsSameHfpDevice(activeHfpDevice_, device)) {
            currentScoState_ = AudioScoState::CONNECTED;
        } else {
            ForceUpdateScoState();
        }
    }
    ProcCacheRequest();
}

void BluetoothScoManager::UpdateScoStateWhenDiconnecting(HfpScoConnectState scoState, const BluetoothRemoteDevice &device)
{
    if (scoState == HfpScoConnectState::SCO_DISCONNECTED) {
        if (IsSameHfpDevice(activeHfpDevice_, device)) {
            currentScoState_ = AudioScoState::DISCONNECTED;
        }
    } else if (scoState == HfpScoConnectState::SCO_CONNECTED) {
        ForceUpdateScoState();
    }
    ProcCacheRequest();
}

void BluetoothScoManager::ForceUpdateScoState()
{
    // need query sco type from bluetooth to refresh local
}

void BluetoothScoManager::ProcCacheRequest()
{
    std::shared_ptr<ScoCacheRequest> req = nullptr;
    {
        std::lock_guard<std::mutex> stateLock(scoLock_);
        req = cacheReq_;
        cacheReq_ = nullptr;
    }
    if (req == nullptr) {
        return;
    }

    AUDIO_INFO_LOG("proc cache %{public}d request category %{public}d and current sco state %{public}d",
        req->connectReq ? "connect" : "disconnect", req->category, currentScoState_);
    if (req->connectReq) {
        HandleScoConnect(req->category, req->device);
    } else {
        HandleScoDisconnect(req->device);
    }
}

int32_t BluetoothScoManager::HandleScoConnect(ScoCategory scoCategory, const BluetoothRemoteDevice &device)
{
    CHECK_AND_RETURN_RET_LOG(hfpInstance_ != nullptr, ERROR, "HFP AG profile instance unavailable");
    std::lock_guard<std::mutex> stateLock(scoLock_);
    int32_t ret = SUCCESS;
    switch (currentScoState_) {
        case AudioScoState::DISCONNECTED:
            ret = ProcConnectReqWhenDisconnected(scoCategory, device);
            break;
        case AudioScoState::CONNECTED:
            ret = ProcConnectReqWhenConnected(scoCategory, device);
            break;
        case AudioScoState::CONNECTING:
            ret = ProcConnectReqWhenConnecting(scoCategory, device);
            break;
        case AudioScoState::DISCONNECTING:
            ret = SaveRequestToCache(true, scoCategory, device);
            break;
        default:
            ret = ERROR;
            break;
    }
    return ret;
}

int32_t BluetoothScoManager::ProcConnectReqWhenDisconnected(ScoCategory scoCategory, const BluetoothRemoteDevice &device)
{
    int32_t ret = ConnectSco(scoCategory, device);
    CHECK_AND_RETURN_RET(ret == 0, ERROR);
    currentScoState_ = AudioScoState::DISCONNECTING;
    currentScoCategory_ = scoCategory;
    activeHfpDevice_ = device;
    return SUCCESS;
}

int32_t BluetoothScoManager::ProcConnectReqWhenConnected(ScoCategory scoCategory, const BluetoothRemoteDevice &device)
{
    bool isSameDevice = IsSameHfpDevice(activeHfpDevice_, device);
    if (IsNeedSwitchScoCategory(scoCategory) && isSameDevice) {
        AUDIO_INFO_LOG("bypass connect category %{public}d current category %{public}d for %{public}s device",
            scoCategory, currentScoCategory_, isSameDevice);
        return SUCCESS;
    }

    AUDIO_INFO_LOG("connect category %{public}d current category %{public}d for %{public}s device",
        scoCategory, currentScoCategory_, isSameDevice);
    int32_t ret = DisconnectSco(currentScoCategory_, activeHfpDevice_);
    CHECK_AND_RETURN_RET(ret == 0, ERROR);
    currentScoState_ = AudioScoState::DISCONNECTING;
    return SaveRequestToCache(true, scoCategory, device);
}

int32_t BluetoothScoManager::ProcConnectReqWhenConnecting(ScoCategory scoCategory, const BluetoothRemoteDevice &device)
{
    bool isSameDevice = IsSameHfpDevice(activeHfpDevice_, device);
    if (IsNeedSwitchScoCategory(scoCategory) && isSameDevice) {
        AUDIO_INFO_LOG("connect category %{public}d current category %{public}d for %{public}s device",
            scoCategory, currentScoCategory_, isSameDevice);
        return SUCCESS;
    }
    return SaveRequestToCache(true, scoCategory, device);
}

bool BluetoothScoManager::IsNeedSwitchScoCategory(ScoCategory scoCategory)
{
    if (scoCategory == currentScoCategory_ ) {
        return false;
    }

    if ((currentScoCategory_ == ScoCategory::SCO_DEFAULT &&
        scoCategory == ScoCategory::SCO_VIRTUAL) ||
        (currentScoCategory_ == ScoCategory::SCO_VIRTUAL &&
        scoCategory == ScoCategory::SCO_DEFAULT)) {
        return false;
    }

    if ((currentScoCategory_ == ScoCategory::SCO_RECOGNITION) &&
        (scoCategory == ScoCategory::SCO_CALLULAR ||
        scoCategory == ScoCategory::SCO_VIRTUAL)) {
        return false;
    }

    if ((scoCategory == ScoCategory::SCO_RECOGNITION) &&
        (currentScoCategory_ == ScoCategory::SCO_CALLULAR ||
        currentScoCategory_ == ScoCategory::SCO_VIRTUAL)) {
        return false;
    }
    return true;
}

int32_t BluetoothScoManager::HandleScoDisconnect(const BluetoothRemoteDevice &device)
{
    CHECK_AND_RETURN_RET_LOG(hfpInstance_ != nullptr, ERROR, "HFP AG profile instance unavailable");
    std::lock_guard<std::mutex> stateLock(scoLock_);
    int32_t ret = SUCCESS;
    switch (currentScoState_) {
        case AudioScoState::DISCONNECTED:
        case AudioScoState::DISCONNECTING:
            cacheReq_ = nullptr;
            break;
        case AudioScoState::CONNECTED:
            ret = ProcDisconnectReqWhenConnected(device);
            break;
        case AudioScoState::CONNECTING:
            ret = ProcDisconnectReqWhenConnecting(device);
            break;
        default:
            ret = ERROR;
            break;
    }
    return ret;
}

int32_t BluetoothScoManager::ProcDisconnectReqWhenConnected(const BluetoothRemoteDevice &device)
{
    if (!IsSameHfpDevice(activeHfpDevice_, device)) {
        AUDIO_WARNING_LOG("disconnect device not meeting expectations");
    }
    cacheReq_ = nullptr;
    int32_t ret = DisconnectSco(currentScoCategory_, activeHfpDevice_);
    CHECK_AND_RETURN_RET(ret == 0, ERROR);
    currentScoState_ = AudioScoState::DISCONNECTING;
    return SUCCESS;
}

int32_t BluetoothScoManager::ProcDisconnectReqWhenConnecting(const BluetoothRemoteDevice &device)
{
    if (!IsSameHfpDevice(activeHfpDevice_, device)) {
        AUDIO_WARNING_LOG("disconnect device not meeting expectations");
    }
    cacheReq_ = nullptr;
    return SaveRequestToCache(false, currentScoCategory_, device);
}

int32_t BluetoothScoManager::SaveRequestToCache(bool isConnect, ScoCategory scoCategory,
    const BluetoothRemoteDevice &device)
{
    if (cacheReq_ != nullptr) {
        cacheReq_ = std::make_shared<ScoCacheRequest>();
    }
    CHECK_AND_RETURN_RET_LOG(cacheReq_ != nullptr, ERROR, "request cache is nullptr");
    cacheReq_->connectReq = isConnect;
    cacheReq_->category = scoCategory;
    cacheReq_->device = device;
    AUDIO_INFO_LOG("cache request, scoCategory: %{public}d isConnect: %{public}d ", scoCategory, isConnect);
    return SUCCESS;
}

bool BluetoothScoManager::IsSameHfpDevice(const BluetoothRemoteDevice &device1, const BluetoothRemoteDevice &device2)
{
    return device1.GetDeviceAddr() == device2.GetDeviceAddr();
}

int32_t BluetoothScoManager::ConnectSco(ScoCategory scoCategory, const BluetoothRemoteDevice &device)
{
    int32_t ret = ERROR;
    if (scoCategory == ScoCategory::SCO_RECOGNITION) {
        ret = hfpInstance_->OpenVoiceRecognition(device) ? SUCCESS : ERROR;
    } else {
        if (scoCategory == ScoCategory::SCO_DEFAULT) {
            scoCategory = SCO_VIRTUAL;
        }
        ret = hfpInstance_->ConnectSco(static_cast<uint8_t> (scoCategory));
    }

    AUDIO_INFO_LOG("ConnectSco, scoCategory: %{public}d ret: %{public}d ", scoCategory, ret);
    return ret;
}

int32_t BluetoothScoManager::DisconnectSco(ScoCategory scoCategory, const BluetoothRemoteDevice &device)
{
    int32_t ret = ERROR;
    if (scoCategory == ScoCategory::SCO_RECOGNITION) {
        ret = hfpInstance_->CloseVoiceRecognition(device) ? SUCCESS : ERROR;
    } else {
        if (scoCategory == ScoCategory::SCO_DEFAULT) {
            scoCategory = SCO_VIRTUAL;
        }
        ret = hfpInstance_->DisconnectSco(static_cast<uint8_t> (scoCategory));
    }

    AUDIO_INFO_LOG("DisconnectSco, scoCategory: %{public}d ret: %{public}d ", scoCategory, ret);
    return ret;
}

AudioScoState BluetoothScoManager::GetAudioScoState()
{
    std::lock_guard<std::mutex> stateLock(scoLock_);
    return currentScoState_;
}

bool BluetoothScoManager::IsInScoCategory(ScoCategory scoCategory)
{
    std::lock_guard<std::mutex> stateLock(scoLock_);
    return (currentScoCategory_ == scoCategory) &&
        (currentScoState_ == AudioScoState::CONNECTING ||
        currentScoState_ == AudioScoState::CONNECTED);
}
}
}