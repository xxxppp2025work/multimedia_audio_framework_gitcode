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

#include "audio_stream_descriptor.h"
#include "audio_common_log.h"

namespace OHOS {
namespace AudioStandard {
AudioStreamDescriptor::AudioStreamDescriptor()
{
}

AudioStreamDescriptor::~AudioStreamDescriptor()
{
}
AudioStreamInfo streamInfo_;
AudioMode audioMode_ = AUDIO_MODE_PLAYBACK;
AudioFlag audioFlag_ = AUDIO_OUTPUT_FLAG_NONE;
AudioFlag routeFlag_ = AUDIO_OUTPUT_FLAG_NONE;
int64_t startTimeStamp_ = 0;
AudioRendererInfo rendererInfo_ = {};
AudioCapturerInfo capturerInfo_ = {};
AppInfo appInfo_ = {}; // Only use pid in binder call
uint32_t sessionId_ = 0;
int32_t callerUid_ = -1;
AudioStreamAction streamAction_ = AUDIO_STREAM_ACTION_DEFAULT;
std::vector<std::shared_ptr<AudioDeviceDescriptor>> oldDeviceDescs_;
std::vector<std::shared_ptr<AudioDeviceDescriptor>> newDeviceDescs_;
int32_t appUid { INVALID_UID };
uint32_t appTokenId { 0 };
int32_t appPid { 0 };
uint64_t appFullTokenId { 0 };

bool AudioStreamDescriptor::Marshalling(Parcel &parcel) const
{
    return streamInfo_.Marshalling(parcel) &&
        parcel.WriteUint32(audioMode_) &&
        parcel.WriteUint32(audioFlag_) &&
        parcel.WriteUint32(routeFlag_) &&
        parcel.WriteInt64(startTimeStamp_) &&
        rendererInfo_.Marshalling(parcel) &&
        capturerInfo_.Marshalling(parcel) &&
        parcel.WriteInt32(appInfo_.appUid) &&
        parcel.WriteUint32(appInfo_.appTokenId) &&
        parcel.WriteInt32(appInfo_.appPid) &&
        parcel.WriteUint64(appInfo_.appFullTokenId) &&
        parcel.WriteUint32(sessionId_) &&
        parcel.WriteInt32(callerUid_) &&
        parcel.WriteUint32(streamAction_) &&
        WriteDeviceDescVectorToParcel(parcel, oldDeviceDescs_) &&
        WriteDeviceDescVectorToParcel(parcel, newDeviceDescs_);

}

void AudioStreamDescriptor::Unmarshalling(Parcel &parcel)
{
    streamInfo_.Unmarshalling(parcel);
    audioMode_ = static_cast<AudioMode>(parcel.ReadUint32());
    audioFlag_ = static_cast<AudioFlag>(parcel.ReadUint32());
    routeFlag_ = static_cast<AudioFlag>(parcel.ReadUint32());
    startTimeStamp_ = parcel.ReadInt64();
    rendererInfo_.Unmarshalling(parcel);
    capturerInfo_.Unmarshalling(parcel);
    appInfo_.appUid = parcel.ReadInt32();
    appInfo_.appTokenId = parcel.ReadUint32();
    appInfo_.appPid = parcel.ReadInt32();
    appInfo_.appFullTokenId = parcel.ReadUint32();
    sessionId_ = parcel.ReadUint32();
    callerUid_ = parcel.ReadInt32();
    streamAction_ = static_cast<AudioStreamAction>(parcel.ReadUint32());
    UnmarshallingDeviceDescVector(parcel, oldDeviceDescs_);
    UnmarshallingDeviceDescVector(parcel, newDeviceDescs_);
}

bool AudioStreamDescriptor::WriteDeviceDescVectorToParcel(Parcel &parcel, std::vector<std::shared_ptr<AudioDeviceDescriptor>> &descs) const
{
    size_t size = descs.size();
    if (size > 1000) {
        return parcel.WriteInt32(-1);
    }
    bool ret = parcel.WriteInt32(static_cast<int32_t>(size));
    CHECK_AND_RETURN_RET_LOG(ret, false, "write vector size failed");

    for (auto desc : descs) {
        ret = desc->Marshalling(parcel);
        CHECK_AND_RETURN_RET_LOG(ret, false, "Marshalling device desc failed");
    }
    return true;
}

void AudioStreamDescriptor::UnmarshallingDeviceDescVector(Parcel &parcel, std::vector<std::shared_ptr<AudioDeviceDescriptor>> &descs)
{
    int32_t size = 0;
    parcel.ReadInt32(size);
    if (size == -1) {
        AUDIO_ERR_LOG("Invalid vector size");
        return;
    }
    for (int32_t i = 0; i < size; i++) {
        descs.push_back(AudioDeviceDescriptor::UnmarshallingPtr(parcel));
    }
}

} // AudioStandard
} // namespace OHOS