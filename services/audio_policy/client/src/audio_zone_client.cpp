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
#define LOG_TAG "AudioZoneClient"
#endif

#include "audio_zone_client.h"
#include "audio_errors.h"
#include "audio_utils.h"
#include "audio_policy_manager.h"

namespace OHOS {
namespace AudioStandard {
AudioZoneClientStub::AudioZoneClientStub()
{}

AudioZoneClientStub::~AudioZoneClientStub()
{}

int32_t AudioZoneClientStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    switch (code) {
        case static_cast<uint32_t>(AudioZoneClientCode::ON_AUDIO_ZONE_ADD):
            HandleAudioZoneAdd(data, reply);
            break;
        case static_cast<uint32_t>(AudioZoneClientCode::ON_AUDIO_ZONE_REMOVE):
            HandleAudioZoneRemove(data, reply);
            break;
        case static_cast<uint32_t>(AudioZoneClientCode::ON_AUDIO_ZONE_CHANGE):
            HandleAudioZoneChange(data, reply);
            break;
        case static_cast<uint32_t>(AudioZoneClientCode::ON_AUDIO_ZONE_INTERRUPT):
            HandleAudioZoneInterrupt(data, reply);
            break;
        case static_cast<uint32_t>(AudioZoneClientCode::ON_AUDIO_ZONE_DEVICE_INTERRUPT):
            HandleAudioZoneDeviceInterrupt(data, reply);
            break;
        case static_cast<uint32_t>(AudioZoneClientCode::ON_AUDIO_ZONE_SYSTEM_VOLUME_SET):
            HandleAudioZoneSystemVolumeSet(data, reply);
            break;
        case static_cast<uint32_t>(AudioZoneClientCode::ON_AUDIO_ZONE_SYSTEM_VOLUME_GET):
            HandleAudioZoneSystemVolumeGet(data, reply);
            break;
        default:
            break;
    }
    return SUCCESS;
}

void AudioZoneClientStub::HandleAudioZoneAdd(MessageParcel &data, MessageParcel &reply)
{
    AudioZoneDescriptor desc;
    desc.Unmarshalling(data);
    OnAudioZoneAdded(desc);
}

void AudioZoneClientStub::HandleAudioZoneRemove(MessageParcel &data, MessageParcel &reply)
{
    OnAudioZoneRemoved(data.ReadInt32());
}

void AudioZoneClientStub::HandleAudioZoneChange(MessageParcel &data, MessageParcel &reply)
{
   int32_t zoneId = data.ReadInt32();
   AudioZoneDescriptor desc;
   desc.Unmarshalling(data);
   AudioZoneChangeReason reason = static_cast<AudioZoneChangeReason>(data.ReadInt32());
   OnAudioZoneChanged(zoneId, desc, reason); 
}

void AudioZoneClientStub::HandleAudioZoneInterrupt(MessageParcel &data, MessageParcel &reply)
{
    int32_t zoneId = data.ReadInt32();
    int32_t size = data.ReadInt32();
    std::list<std::pair<AudioInterrupt, AudioFocusState>> interrupts; 
    for (int i = 0; i < size; i++) {
        AudioInterrupt temp;
        AudioInterrupt::Unmarshalling(data, temp);
        AudioFocusState state = static_cast<AudioFocusState>(data.ReadInt32());
        interrupts.emplace_back(std::make_pair(temp, state));
    }
    AudioZoneInterruptReason reason = static_cast<AudioZoneInterruptReason>(data.ReadInt32());
    OnInterruptEvnet(zoneId, interrupts, reason);
}

void AudioZoneClientStub::HandleAudioZoneDeviceInterrupt(MessageParcel &data, MessageParcel &reply)
{
    int32_t zoneId = data.ReadInt32();
    int32_t deviceId = data.ReadInt32();
    int32_t size = data.ReadInt32();
    std::list<std::pair<AudioInterrupt, AudioFocusState>> interrupts;
    for (int i = 0; i < size; i++) {
        AudioInterrupt temp;
        AudioInterrupt::Unmarshalling(data, temp);
        AudioFocusState state = static_cast<AudioFocusState>(data.ReadInt32());
        interrupts.emplace_back(std::make_pair(temp, state)); 
    }
    AudioZoneInterruptReason reason = static_cast<AudioZoneInterruptReason>(data.ReadInt32());
    OnnterruptEvnet(zoneId, deviceId, interrupts, reason);
}

void AudioZoneClientStub::HandleAudioZoneSystemVolumeSet(MessageParcel &data, MessageParcel &reply)
{
    int32_t zoneId = data.ReadInt32();
    AudioVolumeType volumeType = static_cast<AudioVolumeType>(data.ReadInt32());
    int32_t volumeLevel = data.ReadInt32();
    int32_t volumeFlag = data.ReadInt32();
    reply.WriteInt32(SetSystemVolumeLevel(zoneId, volumeType, volumeLevel, volumeFlag)); 
}

void AudioZoneClientStub::HandleAudioZoneSystemVolumeGet(MessageParcel &data, MessageParcel &reply)
{
    int32_t zoneId = data.ReadInt32();
    AudioVolumeType volumeType = static_cast<AudioVolumeType>(data.ReadInt32());
    reply.WriteInt32(GetSystemVolumeLevel(zoneId, volumeType)); 
}

AudioZoneClient::AudioZoneClient()
{}

AudioZoneClient::~AudioZoneClient()
{}

int32_t AudioZoneClient::AddAudioZoneCallback(const std::shared_ptr<AudioZoneCallback> &callback)
{
    std::lock_guard<std::mutex> lk(audioZoneCallbackMutex_);
    if (audioZoneCallback_ == nullptr) {
        int32_t result = AudioPolicyManager::GetInstance().EnableAudioZoneReport(true);
        CHECK_AND_RETURN_RET_LOG(result == SUCCESS, ERR_OPERATION_FAILED,
            "EnableAudioZoneReport result:${public}d", result);
    }
    audioZoneCallback_ = callback;
    return SUCCESS;
}

void AudioZoneClient::RemoveAudioZoneCallback()
{
    std::lock_guard<std::mutex> lk(audioZoneCallbackMutex_);
    CHECK_AND_RETURN_LOG(audioZoneCallback_ != nullptr, "audioZoneCallback is null.");
    AudioPolicyManager::GetInstance().EnableAudioZoneReport(false);
    audioZoneCallback_ = nullptr;
}