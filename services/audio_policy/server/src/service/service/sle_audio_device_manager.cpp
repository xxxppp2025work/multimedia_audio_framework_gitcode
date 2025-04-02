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
namespace {
const std::map<uint32_t, std::set<StreamUsage>> STREAM_USAGE_TO_SLE_STREAM_TYPE = {
    {0x00000000, {}}, // NONE
    {0x00000002, {STREAM_USAGE_UNKNOWN, STREAM_USAGE_MEDIA, STREAM_USAGE_MUSIC, STREAM_USAGE_ALARM,
        STREAM_USAGE_AUDIOBOOK, STREAM_USAGE_ULTRASONIC, STREAM_USAGE_VOICE_MESSAGE,
        STREAM_USAGE_ACCESSIBILITY, STREAM_USAGE_ENFORCED_TONE}}, // MUSIC
    {0x00000004, {STREAM_USAGE_VOICE_MODEM_COMMUNICATION}}, // VOICE_CALL
    {0x00000008, {STREAM_USAGE_VOICE_ASSISTANT, STREAM_USAGE_DTMF}}, // VOICE_ASSISTANT
    {0x00000010, {STREAM_USAGE_NOTIFICATION_RINGTONE, STREAM_USAGE_RINGTONE, STREAM_USAGE_RANGING,
        STREAM_USAGE_VOICE_RINGTONE}}, // RING
    {0x00000020, {STREAM_USAGE_VOICE_COMMUNICATION, STREAM_USAGE_VIDEO_COMMUNICATION,
        STREAM_USAGE_VOICE_CALL_ASSISTANT}}, // VOIP
    {0x00000040, {STREAM_USAGE_GAME}}, // GAME
    {0x00000100, {STREAM_USAGE_NOTIFICATION, STREAM_USAGE_SYSTEM}}, // ALERT
    {0x00000200, {STREAM_USAGE_MOVIE}}, // VIDEO
    {0x00000400, {STREAM_USAGE_NAVIGATION}} // NAVIGATION
};

const std::map<uint32_t, std::set<SourceType>> SOURCE_TYPE_TO_SLE_STREAM_TYPE = {
    {0x00000000, {}}, // NONE
    {0x00000004, {SOURCE_TYPE_VIRTUAL_CAPTURE, SOURCE_TYPE_VOICE_CALL}}, // VOICE_CALL
    {0x00000020, {SOURCE_TYPE_VOICE_COMMUNICATION}}, // VOIP
    {0x00000080, {SOURCE_TYPE_MIC, SOURCE_TYPE_VOICE_RECOGNITION, SOURCE_TYPE_ULTRASONIC, SOURCE_TYPE_VOICE_MESSAGE
        SOURCE_TYPE_VOICE_TRANSCRIPTION, SOURCE_TYPE_CAMCORDER, SOURCE_TYPE_UNPROCESSED}} // RECORD
};
} // namespace
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

uint32_t SleAudioDeviceManager::GetSleStreamTypeByStreamUsage(StreamUsage streamUsage) const
{
    for (const auto &pair : STREAM_USAGE_TO_SLE_STREAM_TYPE) {
        if (pair.second.find(streamUsage) != pair.second.end()) {
            return pair.first;
        }
    }
    return STREAM_USAGE_TO_SLE_STREAM_TYPE.begin()->first; // Default to NONE
}

uint32_t SleAudioDeviceManager::GetSleStreamTypeBySourceType(SourceType sourceType) const
{
    for (const auto &pair : SOURCE_TYPE_TO_SLE_STREAM_TYPE) {
        if (pair.second.find(sourceType) != pair.second.end()) {
            return pair.first;
        }
    }
    return SOURCE_TYPE_TO_SLE_STREAM_TYPE.begin()->first; // Default to NONE
}

int32_t SleAudioDeviceManager::SetActiveDevice(const std::string &device, StreamUsage streamUsage)
{
    return SetActiveSinkDevice(device, GetSleStreamTypeByStreamUsage(streamUsage));
}

int32_t SleAudioDeviceManager::SetActiveDevice(const std::string &device, SourceType sourceType)
{
    return SetActiveSinkDevice(device, GetSleStreamTypeBySourceType(sourceType));
}

int32_t SleAudioDeviceManager::StopPlaying(const AudioDeviceDescriptor &deviceDesc, StreamUsage streamUsage)
{
    CHECK_AND_RETURN_RET_LOG(deviceDesc.deviceType_ == DEVICE_TYPE_NEARLINK, ERROR, "device type is not nearlink");
    return StopPlaying(deviceDesc.macAddress_, GetSleStreamTypeByStreamUsage(streamUsage));
}

int32_t SleAudioDeviceManager::StopPlaying(const AudioDeviceDescriptor &deviceDesc, SourceType sourceType)
{
    CHECK_AND_RETURN_RET_LOG(deviceDesc.deviceType_ == DEVICE_TYPE_NEARLINK, ERROR, "device type is not nearlink");
    return StopPlaying(deviceDesc.macAddress_, GetSleStreamTypeBySourceType(sourceType));
}

int32_t SleAudioDeviceManager::StartPlaying(const AudioDeviceDescriptor &deviceDesc, StreamUsage streamUsage)
{
    CHECK_AND_RETURN_RET_LOG(deviceDesc.deviceType_ == DEVICE_TYPE_NEARLINK, ERROR, "device type is not nearlink");
    return StartPlaying(deviceDesc.macAddress_, GetSleStreamTypeByStreamUsage(streamUsage));
}

int32_t SleAudioDeviceManager::StartPlaying(const AudioDeviceDescriptor &deviceDesc, SourceType sourceType)
{
    CHECK_AND_RETURN_RET_LOG(deviceDesc.deviceType_ == DEVICE_TYPE_NEARLINK, ERROR, "device type is not nearlink");
    return StartPlaying(deviceDesc.macAddress_, GetSleStreamTypeBySourceType(sourceType));
}
} // namespace AudioStandard
} // namespace OHOS
