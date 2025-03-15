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

#ifndef AUDIO_STREAM_DESCRIPTOR_H
#define AUDIO_STREAM_DESCRIPTOR_H

#include <memory>

#include "parcel.h"
#include "audio_device_descriptor.h"
#include "audio_stream_enum.h"
#include "audio_info.h"

namespace OHOS {
namespace AudioStandard {
enum AudioStreamAction : uint32_t {
    STREAM_ACTION_DEFAULT = 0,
    STREAM_ACTION_NEW,
    STREAM_ACTION_MOVE,
    STREAM_ACTION_RECREATE,
};

class AudioStreamDescriptor{
public:
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
    AudioStreamAction streamAction_ = STREAM_ACTION_DEFAULT;
    AudioStreamStatus streamStatus_ = STREAM_STATUS_NEW;
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> oldDeviceDescs_;
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> newDeviceDescs_;

    AudioStreamDescriptor();
    virtual ~AudioStreamDescriptor();

    bool Marshalling(Parcel &parcel) const;
    void Unmarshalling(Parcel &parcel);
};
} // namespace AudioStandard
} // namespace OHOS
#endif // AUDIO_STREAM_DESCRIPTOR_H
