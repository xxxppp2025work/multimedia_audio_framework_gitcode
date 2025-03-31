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
#define LOG_TAG "SleAudioDeviceManager"
#endif

#include "sle_audio_device_manager.h"

#include "audio_errors.h"
#include "audio_policy_log.h"

namespace OHOS {
namespace AudioStandard {
int32_t SleAudioDeviceManager::SetSleAudioOperationCallback(const sptr<IStandardSleAudioOperationCallback> &callback)
{
    CHECK_AND_RETURN_RET_LOG(callback != nullptr, ERR_INVALID_PARAM, "callback is nullptr");
    callback_ = callback;
    return SUCCESS;
}

void SleAudioDeviceManager::GetSleAudioDeviceList(std::vector<AudioDeviceDescriptor> &devices)
{
    CHECK_AND_RETURN_LOG(callback_ != nullptr, "callback is nullptr");
    callback_->GetSleAudioDeviceList(devices);
}

void SleAudioDeviceManager::GetSleVirtualAudioDeviceList(std::vector<AudioDeviceDescriptor> &devices)
{
    CHECK_AND_RETURN_LOG(callback_ != nullptr, "callback is nullptr");
    callback_->GetSleVirtualAudioDeviceList(virtualDevices);
}

bool SleAudioDeviceManager::IsInBandRingOpen(const std::string &device) const
{
    CHECK_AND_RETURN_RET_LOG(callback_ != nullptr, false, "callback is nullptr");
    return callback_->IsInBandRingOpen(device);
}

uint32_t SleAudioDeviceManager::GetSupportStreamType(const std::string &device) const
{
    CHECK_AND_RETURN_RET_LOG(callback_ != nullptr, ERR_INVALID_PARAM, "callback is nullptr");
    return callback_->GetSupportStreamType(device);
}

int32_t SleAudioDeviceManager::SetActiveSinkDevice(const std::string &device, uint32_t streamType)
{
    CHECK_AND_RETURN_RET_LOG(callback_ != nullptr, ERR_INVALID_PARAM, "callback is nullptr");
    return callback_->SetActiveSinkDevice(device, streamType);
}

int32_t SleAudioDeviceManager::StartPlaying(const std::string &device, uint32_t streamType)
{
    CHECK_AND_RETURN_RET_LOG(callback_ != nullptr, ERR_INVALID_PARAM, "callback is nullptr");
    return callback_->StartPlaying(device, streamType);
}

int32_t SleAudioDeviceManager::StopPlaying(const std::string &device, uint32_t streamType)
{
    CHECK_AND_RETURN_RET_LOG(callback_ != nullptr, ERR_INVALID_PARAM, "callback is nullptr");
    return callback_->StopPlaying(device, streamType);
}

int32_t SleAudioDeviceManager::ConnectAllowedProfiles(const std::string &remoteAddr) const
{
    CHECK_AND_RETURN_RET_LOG(callback_ != nullptr, ERR_INVALID_PARAM, "callback is nullptr");
    return callback_->ConnectAllowedProfiles(remoteAddr);
}

int32_t SleAudioDeviceManager::SetDeviceAbsVolume(const std::string &remoteAddr, uint32_t volume, uint32_t streamType)
{
    CHECK_AND_RETURN_RET_LOG(callback_ != nullptr, ERR_INVALID_PARAM, "callback is nullptr");
    return callback_->SetDeviceAbsVolume(remoteAddr, volume, streamType);
}

int32_t SleAudioDeviceManager::SendUserSelection(const std::string &device, uint32_t streamType)
{
    CHECK_AND_RETURN_RET_LOG(callback_ != nullptr, ERR_INVALID_PARAM, "callback is nullptr");
    return callback_->SendUserSelection(device, streamType);
}
} // namespace AudioStandard
} // namespace OHOS
