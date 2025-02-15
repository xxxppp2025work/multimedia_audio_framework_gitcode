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

namespace OHOS {
namespace AudioStandard {
class AudioStreamDescriptor{
public:
    enum AudioMode {
        AUDIO_MODE_PLAYBACK,
        AUDIO_MODE_RECORD
    };
    enum AudioStreamFlag {
        AUDIO_FLAG_NORMAL,
        AUDIO_FLAG_MMAP,
        AUDIO_FLAG_VOIP_FAST,
        AUDIO_FLAG_DIRECT,
        AUDIO_FLAG_VOIP_DIRECT,
        AUDIO_FLAG_FORCED_NORMAL
    };

    AudioStreamParams audioStreamparams_;
    AudioMode audioMode_ = AUDIO_MODE_PLAYBACK;
    AudioFlag audioFlag_ = AUDIO_FLAG_NONE;
    std::shared_ptr<AudioDeviceDescriptor> deviceDesc_;
    int64_t startTimeStamp_ = 0;
    AudioRendererInfo rendererInfo_ = {};
    AudioCapturerInfo capturerInfo_ = {};
    AppInfo appInfo_ = {};
    uint32_t sessionId_ = 0;

    AudioStreamDescriptor();
    virtual ~AudioStreamDescriptor();
};
} // namespace AudioStandard
} // namespace OHOS
#endif // AUDIO_STREAM_DESCRIPTOR_H
