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

#ifndef AUDIO_CAPTURER_INFO_H
#define AUDIO_CAPTURER_INFO_H

#include "audio_common_info.h"

namespace OHOS {
namespace AudioStandard {

/**
 * @brief Enumerates the capturing states of the current device.
 */
enum CapturerState {
    /** Capturer INVALID state */
    CAPTURER_INVALID = -1,
    /** Create new capturer instance */
    CAPTURER_NEW,
    /** Capturer Prepared state */
    CAPTURER_PREPARED,
    /** Capturer Running state */
    CAPTURER_RUNNING,
    /** Capturer Stopped state */
    CAPTURER_STOPPED,
    /** Capturer Released state */
    CAPTURER_RELEASED,
    /** Capturer Paused state */
    CAPTURER_PAUSED
};

enum AudioCaptureMode {
    CAPTURE_MODE_NORMAL,
    CAPTURE_MODE_CALLBACK
};

class AudioCapturerInfo {
public:
    SourceType sourceType = SOURCE_TYPE_INVALID;
    int32_t capturerFlags = 0;
    int32_t originalFlag = AUDIO_FLAG_NORMAL;
    AudioPipeType pipeType = PIPE_TYPE_UNKNOWN;
    AudioSamplingRate samplingRate = SAMPLE_RATE_8000;
    uint8_t encodingType = 0;
    uint64_t channelLayout = 0ULL;
    std::string sceneType = "";

    AudioCapturerInfo(SourceType sourceType_, int32_t capturerFlags_) : sourceType(sourceType_),
        capturerFlags(capturerFlags_) {}
    AudioCapturerInfo(const AudioCapturerInfo &audioCapturerInfo)
    {
        *this = audioCapturerInfo;
    }
    AudioCapturerInfo() = default;
    ~AudioCapturerInfo()= default;
    bool Marshalling(Parcel &parcel) const
    {
        return parcel.WriteInt32(static_cast<int32_t>(sourceType)) &&
            parcel.WriteInt32(capturerFlags) &&
            parcel.WriteInt32(originalFlag) &&
            parcel.WriteInt32(static_cast<int32_t>(pipeType)) &&
            parcel.WriteInt32(static_cast<int32_t>(samplingRate)) &&
            parcel.WriteUint8(encodingType) &&
            parcel.WriteUint64(channelLayout) &&
            parcel.WriteString(sceneType);
    }
    void Unmarshalling(Parcel &parcel)
    {
        sourceType = static_cast<SourceType>(parcel.ReadInt32());
        capturerFlags = parcel.ReadInt32();
        originalFlag = parcel.ReadInt32();
        pipeType = static_cast<AudioPipeType>(parcel.ReadInt32());
        samplingRate = static_cast<AudioSamplingRate>(parcel.ReadInt32());
        encodingType = parcel.ReadUint8();
        channelLayout = parcel.ReadUint64();
        sceneType = parcel.ReadString();
    }
};

enum FilterMode : uint32_t {
    INCLUDE = 0,
    EXCLUDE,
    MAX_FILTER_MODE
};

// 1.If the size of usages or pids is 0, FilterMode will not work.
// 2.Filters will only works with FileterMode INCLUDE or EXCLUDE while the vector size is not zero.
// 3.If usages and pids are both not empty, the result is the intersection of the two Filter.
// 4.If usages.size() == 0, defalut usages will be filtered with FilterMode::INCLUDE.
// 5.Default usages are MEDIA MUSIC MOVIE GAME and BOOK.
struct CaptureFilterOptions {
    std::vector<StreamUsage> usages;
    FilterMode usageFilterMode {FilterMode::INCLUDE};
    std::vector<int32_t> pids;
    FilterMode pidFilterMode {FilterMode::INCLUDE};
};

struct AudioPlaybackCaptureConfig {
    CaptureFilterOptions filterOptions;
    bool silentCapture {false}; // To be deprecated since 12
};

struct AudioCapturerOptions {
    AudioStreamInfo streamInfo;
    AudioCapturerInfo capturerInfo;
    AudioPlaybackCaptureConfig playbackCaptureConfig;
};

class AudioCapturerChangeInfo {
public:
    int32_t createrUID;
    int32_t clientUID;
    int32_t sessionId;
    int32_t callerPid;
    int32_t clientPid;
    AudioCapturerInfo capturerInfo;
    CapturerState capturerState;
    DeviceInfo inputDeviceInfo;
    bool muted;
    uint32_t appTokenId;

    AudioCapturerChangeInfo(const AudioCapturerChangeInfo &audioCapturerChangeInfo)
    {
        *this = audioCapturerChangeInfo;
    }
    AudioCapturerChangeInfo() = default;
    ~AudioCapturerChangeInfo() = default;
    bool Marshalling(Parcel &parcel) const
    {
        return parcel.WriteInt32(createrUID)
            && parcel.WriteInt32(clientUID)
            && parcel.WriteInt32(sessionId)
            && parcel.WriteInt32(callerPid)
            && parcel.WriteInt32(clientPid)
            && capturerInfo.Marshalling(parcel)
            && parcel.WriteInt32(static_cast<int32_t>(capturerState))
            && inputDeviceInfo.Marshalling(parcel)
            && parcel.WriteBool(muted)
            && parcel.WriteUint32(appTokenId);
    }

    bool Marshalling(Parcel &parcel, bool hasBTPermission, bool hasSystemPermission, int32_t apiVersion) const
    {
        return parcel.WriteInt32(createrUID)
            && parcel.WriteInt32(hasSystemPermission ? clientUID : EMPTY_UID)
            && parcel.WriteInt32(sessionId)
            && parcel.WriteInt32(callerPid)
            && parcel.WriteInt32(clientPid)
            && capturerInfo.Marshalling(parcel)
            && parcel.WriteInt32(hasSystemPermission ? static_cast<int32_t>(capturerState) : CAPTURER_INVALID)
            && inputDeviceInfo.Marshalling(parcel, hasBTPermission, hasSystemPermission, apiVersion)
            && parcel.WriteBool(muted)
            && parcel.WriteUint32(appTokenId);
    }

    void Unmarshalling(Parcel &parcel)
    {
        createrUID = parcel.ReadInt32();
        clientUID = parcel.ReadInt32();
        sessionId = parcel.ReadInt32();
        callerPid = parcel.ReadInt32();
        clientPid = parcel.ReadInt32();
        capturerInfo.Unmarshalling(parcel);
        capturerState = static_cast<CapturerState>(parcel.ReadInt32());
        inputDeviceInfo.Unmarshalling(parcel);
        muted = parcel.ReadBool();
        appTokenId = parcel.ReadUint32();
    }
};





} // namespace AudioStandard
} // namespace OHOS
#endif //AUDIO_CAPTURER_INFO_H
