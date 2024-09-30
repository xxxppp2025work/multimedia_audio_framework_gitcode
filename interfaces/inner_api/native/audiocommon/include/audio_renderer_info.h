/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#ifndef AUDIO_RENDERER_INFO_H
#define AUDIO_RENDERER_INFO_H

#include "audio_common_info.h"

namespace OHOS {
namespace AudioStandard {

    /**
 * @brief Enumerates the rendering states of the current device.
 */
enum RendererState {
    /** INVALID state */
    RENDERER_INVALID = -1,
    /** Create New Renderer instance */
    RENDERER_NEW,
    /** Reneder Prepared state */
    RENDERER_PREPARED,
    /** Rendere Running state */
    RENDERER_RUNNING,
    /** Renderer Stopped state */
    RENDERER_STOPPED,
    /** Renderer Released state */
    RENDERER_RELEASED,
    /** Renderer Paused state */
    RENDERER_PAUSED
};

struct AudioRendererDesc {
    ContentType contentType = CONTENT_TYPE_UNKNOWN;
    StreamUsage streamUsage = STREAM_USAGE_UNKNOWN;
};

/**
* Enumerates the renderer playback speed.
*/
enum AudioRendererRate {
    RENDER_RATE_NORMAL = 0,
    RENDER_RATE_DOUBLE = 1,
    RENDER_RATE_HALF = 2,
};

enum AudioRenderMode {
    RENDER_MODE_NORMAL,
    RENDER_MODE_CALLBACK
};

struct AudioRendererInfo {
    ContentType contentType = CONTENT_TYPE_UNKNOWN;
    StreamUsage streamUsage = STREAM_USAGE_UNKNOWN;
    int32_t rendererFlags = AUDIO_FLAG_NORMAL;
    std::string sceneType = "";
    bool spatializationEnabled = false;
    bool headTrackingEnabled = false;
    int32_t originalFlag = AUDIO_FLAG_NORMAL;
    AudioPipeType pipeType = PIPE_TYPE_UNKNOWN;
    AudioSamplingRate samplingRate = SAMPLE_RATE_8000;
    uint8_t encodingType = 0;
    uint64_t channelLayout = 0ULL;
    AudioSampleFormat format = SAMPLE_S16LE;
    bool isOffloadAllowed = true;
    bool isSatellite = false;

    bool Marshalling(Parcel &parcel) const
    {
        return parcel.WriteInt32(static_cast<int32_t>(contentType))
            && parcel.WriteInt32(static_cast<int32_t>(streamUsage))
            && parcel.WriteInt32(rendererFlags)
            && parcel.WriteInt32(originalFlag)
            && parcel.WriteString(sceneType)
            && parcel.WriteBool(spatializationEnabled)
            && parcel.WriteBool(headTrackingEnabled)
            && parcel.WriteInt32(static_cast<int32_t>(pipeType))
            && parcel.WriteInt32(static_cast<int32_t>(samplingRate))
            && parcel.WriteUint8(encodingType)
            && parcel.WriteUint64(channelLayout)
            && parcel.WriteInt32(format)
            && parcel.WriteBool(isOffloadAllowed);
    }
    void Unmarshalling(Parcel &parcel)
    {
        contentType = static_cast<ContentType>(parcel.ReadInt32());
        streamUsage = static_cast<StreamUsage>(parcel.ReadInt32());
        rendererFlags = parcel.ReadInt32();
        originalFlag = parcel.ReadInt32();
        sceneType = parcel.ReadString();
        spatializationEnabled = parcel.ReadBool();
        headTrackingEnabled = parcel.ReadBool();
        pipeType = static_cast<AudioPipeType>(parcel.ReadInt32());
        samplingRate = static_cast<AudioSamplingRate>(parcel.ReadInt32());
        encodingType = parcel.ReadUint8();
        channelLayout = parcel.ReadUint64();
        format = static_cast<AudioSampleFormat>(parcel.ReadInt32());
        isOffloadAllowed = parcel.ReadBool();
    }
};

struct AudioRendererOptions {
    AudioStreamInfo streamInfo;
    AudioRendererInfo rendererInfo;
    AudioPrivacyType privacyType = PRIVACY_TYPE_PUBLIC;
};

enum WriteDataCallbackType {
    /**
     * Use OH_AudioRenderer_Callbacks.OH_AudioRenderer_OnWriteData
     * @since 12
     */
    WRITE_DATA_CALLBACK_WITHOUT_RESULT = 0,
    /**
     * Use OH_AudioRenderer_OnWriteDataCallback.
     * @since 12
     */
    WRITE_DATA_CALLBACK_WITH_RESULT = 1
};

} // namespace AudioStandard
} // namespace OHOS
#endif //AUDIO_RENDERER_INFO_H
