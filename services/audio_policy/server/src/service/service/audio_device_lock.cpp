/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
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
#define LOG_TAG "AudioDeviceLock"
#endif

#include "audio_device_lock.h"
#include <ability_manager_client.h>
#include "iservice_registry.h"
#include "parameter.h"
#include "parameters.h"
#include "audio_policy_log.h"
#include "media_monitor_manager.h"
#include "audio_state_manager.h"

#include "audio_policy_utils.h"
#include "audio_server_proxy.h"

namespace OHOS {
namespace AudioStandard {
const int32_t DATA_LINK_CONNECTED = 11;
static constexpr int64_t WAIT_LOAD_DEFAULT_DEVICE_TIME_MS = 200; // 200ms
static constexpr int32_t RETRY_TIMES = 25;

static std::string GetEncryptAddr(const std::string &addr)
{
    const int32_t START_POS = 6;
    const int32_t END_POS = 13;
    const int32_t ADDRESS_STR_LEN = 17;
    if (addr.empty() || addr.length() != ADDRESS_STR_LEN) {
        return std::string("");
    }
    std::string tmp = "**:**:**:**:**:**";
    std::string out = addr;
    for (int i = START_POS; i <= END_POS; i++) {
        out[i] = tmp[i];
    }
    return out;
}

void AudioDeviceLock::Init(std::shared_ptr<AudioA2dpOffloadManager> audioA2dpOffloadManager)
{
    audioA2dpOffloadManager_ = audioA2dpOffloadManager;
}

void AudioDeviceLock::DeInit()
{
    audioA2dpOffloadManager_ = nullptr;
}

void AudioDeviceLock::UpdateSessionConnectionState(const int32_t &sessionID, const int32_t &state)
{
    AudioServerProxy::GetInstance().UpdateSessionConnectionStateProxy(sessionID, state);
}

void AudioDeviceLock::SendA2dpConnectedWhileRunning(const RendererState &rendererState, const uint32_t &sessionId)
{
    if ((rendererState == RENDERER_RUNNING) && (audioA2dpOffloadManager_ != nullptr) &&
        !audioA2dpOffloadManager_->IsA2dpOffloadConnecting(sessionId)) {
        AUDIO_INFO_LOG("Notify client not to block.");
        std::thread sendConnectedToClient(&AudioDeviceLock::UpdateSessionConnectionState, this, sessionId,
            DATA_LINK_CONNECTED);
        sendConnectedToClient.detach();
    }
}

void AudioDeviceLock::HandleAudioCaptureState(AudioMode &mode, AudioStreamChangeInfo &streamChangeInfo)
{
    if (mode == AUDIO_MODE_RECORD &&
        (streamChangeInfo.audioCapturerChangeInfo.capturerState == CAPTURER_RELEASED ||
         streamChangeInfo.audioCapturerChangeInfo.capturerState == CAPTURER_STOPPED)) {
        if (Util::IsScoSupportSource(streamChangeInfo.audioCapturerChangeInfo.capturerInfo.sourceType)) {
            audioDeviceCommon_.BluetoothScoDisconectForRecongnition();
            Bluetooth::AudioHfpManager::ClearRecongnitionStatus();
        }
        audioMicrophoneDescriptor_.RemoveAudioCapturerMicrophoneDescriptorBySessionID(
            streamChangeInfo.audioCapturerChangeInfo.sessionId);
    }
}

static void UpdateRendererInfoWhenNoPermission(const shared_ptr<AudioRendererChangeInfo> &audioRendererChangeInfos,
    bool hasSystemPermission)
{
    if (!hasSystemPermission) {
        audioRendererChangeInfos->clientUID = 0;
        audioRendererChangeInfos->rendererState = RENDERER_INVALID;
    }
}

int32_t AudioDeviceLock::GetCurrentRendererChangeInfos(vector<shared_ptr<AudioRendererChangeInfo>>
    &audioRendererChangeInfos, bool hasBTPermission, bool hasSystemPermission)
{
    std::shared_lock deviceLock(deviceStatusUpdateSharedMutex_);

    int32_t status = streamCollector_.GetCurrentRendererChangeInfos(audioRendererChangeInfos);
    CHECK_AND_RETURN_RET_LOG(status == SUCCESS, status,
        "AudioPolicyServer:: Get renderer change info failed");

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> outputDevices =
        audioConnectedDevice_.GetDevicesInner(OUTPUT_DEVICES_FLAG);
    DeviceType activeDeviceType = audioActiveDevice_.GetCurrentOutputDeviceType();
    DeviceRole activeDeviceRole = OUTPUT_DEVICE;
    std::string activeDeviceMac = audioActiveDevice_.GetCurrentOutputDeviceMacAddr();

    const auto& itr = std::find_if(outputDevices.begin(), outputDevices.end(),
        [&activeDeviceType, &activeDeviceRole, &activeDeviceMac](const std::shared_ptr<AudioDeviceDescriptor> &desc) {
        if ((desc->deviceType_ == activeDeviceType) && (desc->deviceRole_ == activeDeviceRole)) {
            // This A2DP device is not the active A2DP device. Skip it.
            return activeDeviceType != DEVICE_TYPE_BLUETOOTH_A2DP || desc->macAddress_ == activeDeviceMac;
        }
        return false;
    });

    if (itr != outputDevices.end()) {
        size_t rendererInfosSize = audioRendererChangeInfos.size();
        for (size_t i = 0; i < rendererInfosSize; i++) {
            UpdateRendererInfoWhenNoPermission(audioRendererChangeInfos[i], hasSystemPermission);
            audioDeviceCommon_.UpdateDeviceInfo(audioRendererChangeInfos[i]->outputDeviceInfo, *itr,
                hasBTPermission, hasSystemPermission);
        }
    }

    return status;
}

int32_t AudioDeviceLock::SelectOutputDevice(sptr<AudioRendererFilter> audioRendererFilter,
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> selectedDesc)
{
    std::lock_guard<std::shared_mutex> deviceLock(deviceStatusUpdateSharedMutex_);
    return audioRecoveryDevice_.SelectOutputDevice(audioRendererFilter, selectedDesc);
}

int32_t AudioDeviceLock::SelectInputDevice(sptr<AudioCapturerFilter> audioCapturerFilter,
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> selectedDesc)
{
    std::lock_guard<std::shared_mutex> deviceLock(deviceStatusUpdateSharedMutex_);
    return audioRecoveryDevice_.SelectInputDevice(audioCapturerFilter, selectedDesc);
}

void AudioDeviceLock::UpdateTrackerDeviceChange(const vector<std::shared_ptr<AudioDeviceDescriptor>> &desc)
{
    AUDIO_INFO_LOG("Start");

    DeviceType curOutputDeviceType = audioActiveDevice_.GetCurrentOutputDeviceType();
    for (std::shared_ptr<AudioDeviceDescriptor> deviceDesc : desc) {
        if (deviceDesc->deviceRole_ == OUTPUT_DEVICE) {
            DeviceType type = curOutputDeviceType;
            std::string macAddress = audioActiveDevice_.GetCurrentOutputDeviceMacAddr();
            auto itr = audioConnectedDevice_.CheckExistOutputDevice(type, macAddress);
            if (itr != nullptr) {
                AudioDeviceDescriptor outputDevice(AudioDeviceDescriptor::DEVICE_INFO);
                audioDeviceCommon_.UpdateDeviceInfo(outputDevice, itr, true, true);
                streamCollector_.UpdateTracker(AUDIO_MODE_PLAYBACK, outputDevice);
            }
        }

        if (deviceDesc->deviceRole_ == INPUT_DEVICE) {
            DeviceType type = audioActiveDevice_.GetCurrentInputDeviceType();
            auto itr = audioConnectedDevice_.CheckExistInputDevice(type);
            if (itr != nullptr) {
                AudioDeviceDescriptor inputDevice(AudioDeviceDescriptor::DEVICE_INFO);
                audioDeviceCommon_.UpdateDeviceInfo(inputDevice, itr, true, true);
                audioMicrophoneDescriptor_.UpdateAudioCapturerMicrophoneDescriptor(itr->deviceType_);
                streamCollector_.UpdateTracker(AUDIO_MODE_RECORD, inputDevice);
            }
        }
    }
}

void AudioDeviceLock::NotifyRemoteRenderState(std::string networkId, std::string condition, std::string value)
{
    std::lock_guard<std::shared_mutex> deviceLock(deviceStatusUpdateSharedMutex_);

    AUDIO_INFO_LOG("device<%{public}s> condition:%{public}s value:%{public}s",
        GetEncryptStr(networkId).c_str(), condition.c_str(), value.c_str());

    vector<SinkInput> sinkInputs = audioPolicyManager_.GetAllSinkInputs();
    vector<SinkInput> targetSinkInputs = {};
    for (auto sinkInput : sinkInputs) {
        if (sinkInput.sinkName == networkId) {
            targetSinkInputs.push_back(sinkInput);
        }
    }
    AUDIO_DEBUG_LOG("move [%{public}zu] of all [%{public}zu]sink-inputs to local.",
        targetSinkInputs.size(), sinkInputs.size());
    std::shared_ptr<AudioDeviceDescriptor> localDevice = std::make_shared<AudioDeviceDescriptor>();
    CHECK_AND_RETURN_LOG(localDevice != nullptr, "Device error: null device.");
    localDevice->networkId_ = LOCAL_NETWORK_ID;
    localDevice->deviceRole_ = DeviceRole::OUTPUT_DEVICE;
    localDevice->deviceType_ = DeviceType::DEVICE_TYPE_SPEAKER;

    int32_t ret;
    AudioDeviceDescriptor curOutputDeviceDesc = audioActiveDevice_.GetCurrentOutputDevice();
    if (localDevice->deviceType_ != curOutputDeviceDesc.deviceType_) {
        AUDIO_WARNING_LOG("device[%{public}d] not active, use device[%{public}d] instead.",
            static_cast<int32_t>(localDevice->deviceType_), static_cast<int32_t>(curOutputDeviceDesc.deviceType_));
        ret = audioDeviceCommon_.MoveToLocalOutputDevice(targetSinkInputs,
            std::make_shared<AudioDeviceDescriptor>(curOutputDeviceDesc));
    } else {
        ret = audioDeviceCommon_.MoveToLocalOutputDevice(targetSinkInputs, localDevice);
    }
    CHECK_AND_RETURN_LOG((ret == SUCCESS), "MoveToLocalOutputDevice failed!");

    // Suspend device, notify audio stream manager that device has been changed.
    ret = audioPolicyManager_.SuspendAudioDevice(networkId, true);
    CHECK_AND_RETURN_LOG((ret == SUCCESS), "SuspendAudioDevice failed!");

    std::vector<std::shared_ptr<AudioDeviceDescriptor>> desc = {};
    desc.push_back(localDevice);
    UpdateTrackerDeviceChange(desc);
    audioDeviceCommon_.OnPreferredOutputDeviceUpdated(curOutputDeviceDesc);
    AUDIO_DEBUG_LOG("Success");
}

int32_t AudioDeviceLock::OnCapturerSessionAdded(uint64_t sessionID, SessionInfo sessionInfo,
    AudioStreamInfo streamInfo)
{
    std::lock_guard<std::shared_mutex> deviceLock(deviceStatusUpdateSharedMutex_);
    return audioCapturerSession_.OnCapturerSessionAdded(sessionID, sessionInfo, streamInfo);
}

void AudioDeviceLock::OnCapturerSessionRemoved(uint64_t sessionID)
{
    std::lock_guard<std::shared_mutex> deviceLock(deviceStatusUpdateSharedMutex_);
    audioCapturerSession_.OnCapturerSessionRemoved(sessionID);
}

int32_t AudioDeviceLock::OnServiceConnected(AudioServiceIndex serviceIndex)
{
    std::lock_guard<std::shared_mutex> deviceLock(deviceStatusUpdateSharedMutex_);
    return audioDeviceStatus_.OnServiceConnected(serviceIndex);
}

// new lock
void AudioDeviceLock::OnPnpDeviceStatusUpdated(AudioDeviceDescriptor &desc, bool isConnected)
{
    std::lock_guard<std::shared_mutex> deviceLock(deviceStatusUpdateSharedMutex_);
    audioDeviceStatus_.OnPnpDeviceStatusUpdated(desc, isConnected);
}

void AudioDeviceLock::OnBlockedStatusUpdated(DeviceType devType, DeviceBlockStatus status)
{
    std::lock_guard<std::shared_mutex> deviceLock(deviceStatusUpdateSharedMutex_);
    audioDeviceStatus_.OnBlockedStatusUpdated(devType, status);
}

void AudioDeviceLock::OnMicrophoneBlockedUpdate(DeviceType devType, DeviceBlockStatus status)
{
    std::lock_guard<std::shared_mutex> deviceLock(deviceStatusUpdateSharedMutex_);
    audioDeviceStatus_.OnMicrophoneBlockedUpdate(devType, status);
}

void AudioDeviceLock::OnServiceDisconnected(AudioServiceIndex serviceIndex)
{
    AUDIO_WARNING_LOG("Start for [%{public}d]", serviceIndex);
}

void AudioDeviceLock::SetDisplayName(const std::string &deviceName, bool isLocalDevice)
{
    std::lock_guard<std::shared_mutex> deviceLock(deviceStatusUpdateSharedMutex_);
    audioConnectedDevice_.SetDisplayName(deviceName, isLocalDevice);
}

int32_t AudioDeviceLock::TriggerFetchDevice(AudioStreamDeviceChangeReasonExt reason)
{
    std::lock_guard<std::shared_mutex> deviceLock(deviceStatusUpdateSharedMutex_);
    audioDeviceCommon_.FetchDevice(true, reason);
    audioDeviceCommon_.FetchDevice(false, reason);

    // update a2dp offload
    audioA2dpOffloadManager_->UpdateA2dpOffloadFlagForAllStream();
    return SUCCESS;
}

std::vector<sptr<VolumeGroupInfo>> AudioDeviceLock::GetVolumeGroupInfos()
{
    std::vector<sptr<VolumeGroupInfo>> infos = {};
    for (int32_t i = 0; i < RETRY_TIMES; i++) {
        std::shared_lock deviceLock(deviceStatusUpdateSharedMutex_);
        if (audioVolumeManager_.GetVolumeGroupInfosNotWait(infos)) {
            return infos;
        } else {
            deviceLock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_LOAD_DEFAULT_DEVICE_TIME_MS));
        }
    }
    AUDIO_ERR_LOG("timeout");
    return infos;
}

}
}