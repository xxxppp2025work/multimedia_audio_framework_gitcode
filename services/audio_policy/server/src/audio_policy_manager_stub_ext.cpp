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
#define LOG_TAG "AudioPolicyManagerStub"
#endif

#include "audio_policy_manager_stub.h"

#include "audio_errors.h"
#include "audio_policy_log.h"
#include "audio_utils.h"
#include "ipc_skeleton.h"

namespace OHOS {
namespace AudioStandard {

void AudioPolicyManagerStub::SaveAdjustVolumeInfoInternal(MessageParcel &data, MessageParcel &reply)
{
    float volume = data.ReadFloat();
    uint32_t sessionId = data.ReadUint32();
    std::string invocationTime = data.ReadString();
    uint32_t volumeType = data.ReadUint32();
    int32_t result = SaveAdjustStreamVolumeInfo(volume, sessionId, invocationTime, volumeType);
    reply.WriteInt32(result);
}

void AudioPolicyManagerStub::SetRingerModeLegacyInternal(MessageParcel &data, MessageParcel &reply)
{
    AudioRingerMode rMode = static_cast<AudioRingerMode>(data.ReadInt32());
    int32_t result = SetRingerModeLegacy(rMode);
    reply.WriteInt32(result);
}

void AudioPolicyManagerStub::SetRingerModeInternal(MessageParcel &data, MessageParcel &reply)
{
    AudioRingerMode rMode = static_cast<AudioRingerMode>(data.ReadInt32());
    int32_t result = SetRingerMode(rMode);
    reply.WriteInt32(result);
}
} // namespace audio_policy
} // namespace OHOS
