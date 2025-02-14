/*
 * Copyright (c) 2022-2025 Huawei Device Co., Ltd.
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
#define LOG_TAG "AudioGroupManager"
#endif

#include "audio_group_manager.h"

#include <mutex>
#include "system_ability_definition.h"
#include "iservice_registry.h"

#include "audio_service_log.h"
#include "audio_errors.h"
#include "audio_utils.h"
#include "audio_manager_proxy.h"
#include "audio_policy_manager.h"
#include "audio_system_manager.h"

namespace OHOS {
namespace AudioStandard {

AudioGroupManager::AudioGroupManager(int32_t groupId)
    : groupId_(groupId)
{
}

AudioGroupManager::~AudioGroupManager()
{
    AUDIO_DEBUG_LOG("AudioGroupManager start");
    if (cbClientId_ != -1) {
        UnsetRingerModeCallback(cbClientId_);
    }
}

int32_t AudioGroupManager::GetGroupId()
{
    return groupId_;
}

int32_t AudioGroupManager::SetVolume(AudioVolumeType volumeType, int32_t volume, int32_t volumeFlag)
{
    if (connectType_ == CONNECT_TYPE_DISTRIBUTED) {
        CHECK_AND_RETURN_RET(InitNetworkIdByGroupId() == SUCCESS, ERROR);
        std::string condition = "EVENT_TYPE=1;VOLUME_GROUP_ID=" + std::to_string(groupId_) + ";AUDIO_VOLUME_TYPE="
            + std::to_string(volumeType) + ";";
        std::string value = std::to_string(volume);
        AudioSystemManager::GetInstance()->SetAudioParameter(networkId_, AudioParamKey::VOLUME, condition, value);
        return SUCCESS;
    }

    AUDIO_INFO_LOG("SetSystemVolume: volumeType[%{public}d], volume[%{public}d], flag[%{public}d]",
        volumeType, volume, volumeFlag);

    /* Validate volume type and return INVALID_PARAMS error */
    switch (volumeType) {
        case STREAM_VOICE_CALL:
        case STREAM_VOICE_COMMUNICATION:
        case STREAM_RING:
        case STREAM_MUSIC:
        case STREAM_ALARM:
        case STREAM_SYSTEM:
        case STREAM_ACCESSIBILITY:
        case STREAM_VOICE_ASSISTANT:
                break;
        case STREAM_ULTRASONIC:
        case STREAM_ALL:{
            bool ret = PermissionUtil::VerifySelfPermission();
            CHECK_AND_RETURN_RET_LOG(ret, ERR_PERMISSION_DENIED, "No system permission");
            break;
        }
        default:
            AUDIO_ERR_LOG("volumeType[%{public}d] is not supported", volumeType);
            return ERR_NOT_SUPPORTED;
    }

    /* Call Audio Policy SetSystemVolumeLevel */
    return AudioPolicyManager::GetInstance().SetSystemVolumeLevel(volumeType, volume, false, volumeFlag);
}

AudioStreamType AudioGroupManager::GetActiveVolumeType(const int32_t clientUid)
{
    return AudioPolicyManager::GetInstance().GetSystemActiveVolumeType(clientUid);
}

int32_t AudioGroupManager::GetVolume(AudioVolumeType volumeType)
{
    if (connectType_ == CONNECT_TYPE_DISTRIBUTED) {
        CHECK_AND_RETURN_RET(InitNetworkIdByGroupId() == SUCCESS, ERROR);
        std::string condition = "EVENT_TYPE=1;VOLUME_GROUP_ID=" + std::to_string(groupId_) + ";AUDIO_VOLUME_TYPE="
            + std::to_string(volumeType) + ";";
        std::string value =
            AudioSystemManager::GetInstance()->GetAudioParameter(networkId_, AudioParamKey::VOLUME, condition);
        int32_t convertValue = 0;
        CHECK_AND_RETURN_RET_LOG(StringConverter(value, convertValue), 0,
            "convert invalid value: %{public}s", value.c_str());
        return convertValue;
    }

    switch (volumeType) {
        case STREAM_MUSIC:
        case STREAM_RING:
        case STREAM_NOTIFICATION:
        case STREAM_VOICE_CALL:
        case STREAM_VOICE_COMMUNICATION:
        case STREAM_VOICE_ASSISTANT:
        case STREAM_ALARM:
        case STREAM_SYSTEM:
        case STREAM_ACCESSIBILITY:
            break;
        case STREAM_ULTRASONIC:
        case STREAM_ALL:{
            bool ret = PermissionUtil::VerifySelfPermission();
            CHECK_AND_RETURN_RET_LOG(ret, ERR_PERMISSION_DENIED, "No system permission");
            break;
        }
        default:
            AUDIO_ERR_LOG("volumeType=%{public}d not supported", volumeType);
            return ERR_NOT_SUPPORTED;
    }

    return AudioPolicyManager::GetInstance().GetSystemVolumeLevel(volumeType);
}

int32_t AudioGroupManager::GetMaxVolume(AudioVolumeType volumeType)
{
    if (connectType_ == CONNECT_TYPE_DISTRIBUTED) {
        CHECK_AND_RETURN_RET(InitNetworkIdByGroupId() == SUCCESS, ERROR);
        std::string condition = "EVENT_TYPE=3;VOLUME_GROUP_ID=" + std::to_string(groupId_) + ";AUDIO_VOLUME_TYPE=" +
            std::to_string(volumeType) + ";";
        std::string value =
            AudioSystemManager::GetInstance()->GetAudioParameter(networkId_, AudioParamKey::VOLUME, condition);
        int32_t convertValue = 0;
        CHECK_AND_RETURN_RET_LOG(StringConverter(value, convertValue), 0,
            "convert invalid value: %{public}s", value.c_str());
        return convertValue;
    }

    if (volumeType == STREAM_ALL || volumeType == STREAM_ULTRASONIC) {
        bool ret = PermissionUtil::VerifySelfPermission();
        CHECK_AND_RETURN_RET_LOG(ret, ERR_PERMISSION_DENIED, "No system permission");
    }

    return AudioPolicyManager::GetInstance().GetMaxVolumeLevel(volumeType);
}

int32_t AudioGroupManager::GetMinVolume(AudioVolumeType volumeType)
{
    if (connectType_ == CONNECT_TYPE_DISTRIBUTED) {
        CHECK_AND_RETURN_RET(InitNetworkIdByGroupId() == SUCCESS, ERROR);
        std::string condition = "EVENT_TYPE=2;VOLUME_GROUP_ID=" + std::to_string(groupId_) + ";AUDIO_VOLUME_TYPE" +
            std::to_string(volumeType) + ";";
        std::string value =
            AudioSystemManager::GetInstance()->GetAudioParameter(networkId_, AudioParamKey::VOLUME, condition);
        int32_t convertValue = 0;
        CHECK_AND_RETURN_RET_LOG(StringConverter(value, convertValue), 0,
            "convert invalid value: %{public}s", value.c_str());
        return convertValue;
    }

    if (volumeType == STREAM_ALL || volumeType == STREAM_ULTRASONIC) {
        bool ret = PermissionUtil::VerifySelfPermission();
        CHECK_AND_RETURN_RET_LOG(ret, ERR_PERMISSION_DENIED, "No system permission");
    }

    return AudioPolicyManager::GetInstance().GetMinVolumeLevel(volumeType);
}

int32_t AudioGroupManager::SetMute(AudioVolumeType volumeType, bool mute, const DeviceType &deviceType)
{
    if (connectType_ == CONNECT_TYPE_DISTRIBUTED) {
        CHECK_AND_RETURN_RET(InitNetworkIdByGroupId() == SUCCESS, ERROR);
        std::string conditon = "EVENT_TYPE=4;VOLUME_GROUP_ID=" + std::to_string(groupId_) + ";AUDIO_VOLUME_TYPE="
            + std::to_string(volumeType) + ";";
        std::string value = mute ? "1" : "0";
        AudioSystemManager::GetInstance()->SetAudioParameter(networkId_, AudioParamKey::VOLUME, conditon, value);
        return SUCCESS;
    }

    if (deviceType != DEVICE_TYPE_NONE) {
        AUDIO_INFO_LOG("deviceType [%{public}d], mute [%{public}d]", deviceType, mute);
    }

    AUDIO_INFO_LOG("volumeType [%{public}d], mute [%{public}d]", volumeType, mute);
    switch (volumeType) {
        case STREAM_MUSIC:
        case STREAM_RING:
        case STREAM_NOTIFICATION:
        case STREAM_VOICE_CALL:
        case STREAM_VOICE_COMMUNICATION:
        case STREAM_VOICE_ASSISTANT:
        case STREAM_ALARM:
        case STREAM_SYSTEM:
        case STREAM_ACCESSIBILITY:
        case STREAM_ULTRASONIC:
        case STREAM_ALL:
            break;
        default:
            AUDIO_ERR_LOG("volumeType [%{public}d] is not supported", volumeType);
            return ERR_NOT_SUPPORTED;
    }

    /* Call Audio Policy SetStreamMute */
    return AudioPolicyManager::GetInstance().SetStreamMute(volumeType, mute, false, deviceType);
}

int32_t AudioGroupManager::IsStreamMute(AudioVolumeType volumeType, bool &isMute)
{
    if (connectType_ == CONNECT_TYPE_DISTRIBUTED) {
        CHECK_AND_RETURN_RET(InitNetworkIdByGroupId() == SUCCESS, ERROR);
        std::string condition = "EVENT_TYPE=4;VOLUME_GROUP_ID=" + std::to_string(groupId_) + ";AUDIO_VOLUME_TYPE="
            + std::to_string(volumeType) + ";";
        std::string ret =
            AudioSystemManager::GetInstance()->GetAudioParameter(networkId_, AudioParamKey::VOLUME, condition);
        isMute = (ret == "1" ? true : false);
        return SUCCESS;
    }

    switch (volumeType) {
        case STREAM_MUSIC:
        case STREAM_RING:
        case STREAM_NOTIFICATION:
        case STREAM_VOICE_CALL:
        case STREAM_VOICE_COMMUNICATION:
        case STREAM_VOICE_ASSISTANT:
        case STREAM_ALARM:
        case STREAM_SYSTEM:
        case STREAM_ACCESSIBILITY:
            break;
        case STREAM_ULTRASONIC:
        case STREAM_ALL:{
            bool ret = PermissionUtil::VerifySelfPermission();
            CHECK_AND_RETURN_RET_LOG(ret, ERR_PERMISSION_DENIED, "No system permission");
            break;
        }
        default:
            AUDIO_ERR_LOG("volumeType [%{public}d] is not supported", volumeType);
            return false;
    }

    isMute = AudioPolicyManager::GetInstance().GetStreamMute(volumeType);
    return SUCCESS;
}

int32_t AudioGroupManager::SetRingerModeCallback(const int32_t clientId,
    const std::shared_ptr<AudioRingerModeCallback> &callback)
{
    CHECK_AND_RETURN_RET_LOG(callback != nullptr, ERR_INVALID_PARAM, "callback is nullptr");

    cbClientId_ = clientId;

    return AudioPolicyManager::GetInstance().SetRingerModeCallback(clientId, callback, API_9);
}

int32_t AudioGroupManager::UnsetRingerModeCallback(const int32_t clientId) const
{
    return AudioPolicyManager::GetInstance().UnsetRingerModeCallback(clientId);
}

int32_t AudioGroupManager::UnsetRingerModeCallback(const int32_t clientId,
    const std::shared_ptr<AudioRingerModeCallback> &callback) const
{
    return AudioPolicyManager::GetInstance().UnsetRingerModeCallback(clientId, callback);
}

int32_t AudioGroupManager::SetRingerMode(AudioRingerMode ringMode) const
{
    AUDIO_INFO_LOG("ringer mode: %{public}d", ringMode);
    CHECK_AND_RETURN_RET_LOG(networkId_ == LOCAL_NETWORK_ID, ERROR, "only support local device");
    return AudioPolicyManager::GetInstance().SetRingerMode(ringMode);
}

AudioRingerMode AudioGroupManager::GetRingerMode() const
{
    CHECK_AND_RETURN_RET_LOG(networkId_ == LOCAL_NETWORK_ID, AudioRingerMode::RINGER_MODE_NORMAL,
        "only support local device");
    return AudioPolicyManager::GetInstance().GetRingerMode();
}

int32_t AudioGroupManager::SetMicrophoneMute(bool isMute)
{
    CHECK_AND_RETURN_RET_LOG(networkId_ == LOCAL_NETWORK_ID, ERROR, "only support local device");
    return AudioPolicyManager::GetInstance().SetMicrophoneMuteAudioConfig(isMute);
}

int32_t AudioGroupManager::SetMicrophoneMutePersistent(const bool isMute, const PolicyType type)
{
    AUDIO_INFO_LOG("Set persistent mic mute state %{public}d", isMute);
    CHECK_AND_RETURN_RET_LOG(networkId_ == LOCAL_NETWORK_ID, ERROR, "only support local device");
    return AudioPolicyManager::GetInstance().SetMicrophoneMutePersistent(isMute, type);
}

bool AudioGroupManager::GetPersistentMicMuteState()
{
    CHECK_AND_RETURN_RET_LOG(networkId_ == LOCAL_NETWORK_ID, ERROR, "only support local device");
    return AudioPolicyManager::GetInstance().GetPersistentMicMuteState();
}

bool AudioGroupManager::IsMicrophoneMuteLegacy()
{
    /* Call Audio Policy GetRingerMode */
    CHECK_AND_RETURN_RET_LOG(networkId_ == LOCAL_NETWORK_ID, false, "only support local device");
    return AudioPolicyManager::GetInstance().IsMicrophoneMuteLegacy();
}

bool AudioGroupManager::IsMicrophoneMute()
{
    /* Call Audio Policy GetRingerMode */
    CHECK_AND_RETURN_RET_LOG(networkId_ == LOCAL_NETWORK_ID, false, "only support local device");
    return AudioPolicyManager::GetInstance().IsMicrophoneMute();
}

int32_t AudioGroupManager::SetMicStateChangeCallback(
    const std::shared_ptr<AudioManagerMicStateChangeCallback> &callback)
{
    AUDIO_INFO_LOG("Enter");

    CHECK_AND_RETURN_RET_LOG(callback != nullptr, ERR_INVALID_PARAM, "callback is null");

    return AudioPolicyManager::GetInstance().SetMicStateChangeCallback(static_cast<int32_t>(getpid()), callback);
}

int32_t AudioGroupManager::UnsetMicStateChangeCallback(
    const std::shared_ptr<AudioManagerMicStateChangeCallback> &callback)
{
    return AudioPolicyManager::GetInstance().UnsetMicStateChangeCallback(callback);
}

bool AudioGroupManager::IsVolumeUnadjustable()
{
    CHECK_AND_RETURN_RET_LOG(networkId_ == LOCAL_NETWORK_ID, ERROR, "only support local device");
    return AudioPolicyManager::GetInstance().IsVolumeUnadjustable();
}

int32_t AudioGroupManager::AdjustVolumeByStep(VolumeAdjustType adjustType)
{
    return AudioPolicyManager::GetInstance().AdjustVolumeByStep(adjustType);
}

int32_t AudioGroupManager::AdjustSystemVolumeByStep(AudioVolumeType volumeType, VolumeAdjustType adjustType)
{
    return AudioPolicyManager::GetInstance().AdjustSystemVolumeByStep(volumeType, adjustType);
}

float AudioGroupManager::GetSystemVolumeInDb(AudioVolumeType volumeType, int32_t volumeLevel, DeviceType deviceType)
{
    CHECK_AND_RETURN_RET_LOG(networkId_ == LOCAL_NETWORK_ID, static_cast<float>(ERROR), "only support local device");
    return AudioPolicyManager::GetInstance().GetSystemVolumeInDb(volumeType, volumeLevel, deviceType);
}

float AudioGroupManager::GetMaxAmplitude(const int32_t deviceId)
{
    return AudioPolicyManager::GetInstance().GetMaxAmplitude(deviceId);
}

int32_t AudioGroupManager::InitNetworkIdByGroupId(int32_t groupId)
{
    std::lock_guard<std::mutex> lock(lock_);
    if (initNetworkIdFlag_) {
        AUDIO_INFO_LOG("already inited");
        return SUCCESS;
    }

    AUDIO_INFO_LOG("init networkId");
    std::string networkId;
    int32_t ret = AudioPolicyManager::GetInstance().GetNetworkIdByGroupId(groupId, networkId);
    if (ret != SUCCESS) {
        AUDIO_ERR_LOG("init failed, has no valid group");
        return ERROR;
    }

    networkId_ = networkId;
    connectType_ = (networkId_ == LOCAL_NETWORK_ID) ? CONNECT_TYPE_LOCAL : CONNECT_TYPE_DISTRIBUTED;
    initNetworkIdFlag_ = true;

    return SUCCESS;
}

} // namespace AudioStandard
} // namespace OHOS
