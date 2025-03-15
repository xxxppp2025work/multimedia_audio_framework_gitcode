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

namespace OHOS {
namespace AudioStandard {
AudioStreamDescriptor::AudioStreamDescriptor()
{
}

AudioStreamDescriptor::~AudioStreamDescriptor()
{
}

bool AudioStreamDescriptor::Marshalling(Parcel &parcel) const
{
    return streamInfo_.Marshalling(parcel) &&
        parcel.WriteUint32(audioMode_) &&
        parcel.WriteUint32(audioFlag_) &&
        deviceDesc_->Marshalling(parcel) &&
        parcel.WriteInt64(startTimeStamp_) &&
        rendererInfo_.Marshalling(parcel) &&
        capturerInfo_.Marshalling(parcel) &&
        parcel.WriteInt32(appInfo_.appUid) &&
        parcel.WriteUint32(sessionId_);
}

void AudioStreamDescriptor::Unmarshalling(Parcel &parcel)
{
    streamInfo_.Unmarshalling(parcel);
    audioMode_ = static_cast<AudioMode>(parcel.ReadUint32());
    deviceDesc_ = AudioDeviceDescriptor::UnmarshallingPtr(parcel);
    startTimeStamp_ = parcel.ReadInt64();
    rendererInfo_.Unmarshalling(parcel);
    capturerInfo_.Unmarshalling(parcel);
    appInfo_.appUid = parcel.ReadInt32();
    sessionId_ = parcel.ReadUint32();
}
} // AudioStandard
} // namespace OHOS
 