/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#ifndef AUDIO_TONE_INFO_H
#define AUDIO_TONE_INFO_H

#include <parcel.h>

namespace OHOS {
namespace AudioStandard {

#ifdef FEATURE_DTMF_TONE
// Maximun number of sine waves in a tone segment
constexpr uint32_t TONEINFO_MAX_WAVES = 3;

// Maximun number of segments in a tone descriptor
constexpr uint32_t TONEINFO_MAX_SEGMENTS = 12;
constexpr uint32_t TONEINFO_INF = 0xFFFFFFFF;
class ToneSegment : public Parcelable {
public:
    uint32_t duration;
    uint16_t waveFreq[TONEINFO_MAX_WAVES+1];
    uint16_t loopCnt;
    uint16_t loopIndx;
    bool Marshalling(Parcel &parcel) const override
    {
        parcel.WriteUint32(duration);
        parcel.WriteUint16(loopCnt);
        parcel.WriteUint16(loopIndx);
        for (uint32_t i = 0; i < TONEINFO_MAX_WAVES + 1; i++) {
            parcel.WriteUint16(waveFreq[i]);
        }
        return true;
    }
    void Unmarshalling(Parcel &parcel)
    {
        duration = parcel.ReadUint32();
        loopCnt = parcel.ReadUint16();
        loopIndx = parcel.ReadUint16();
        for (uint32_t i = 0; i < TONEINFO_MAX_WAVES + 1; i++) {
            waveFreq[i] = parcel.ReadUint16();
        }
    }
};

class ToneInfo : public Parcelable {
public:
    ToneSegment segments[TONEINFO_MAX_SEGMENTS+1];
    uint32_t segmentCnt;
    uint32_t repeatCnt;
    uint32_t repeatSegment;
    bool Marshalling(Parcel &parcel) const override
    {
        parcel.WriteUint32(segmentCnt);
        parcel.WriteUint32(repeatCnt);
        parcel.WriteUint32(repeatSegment);
        for (uint32_t i = 0; i < segmentCnt; i++) {
            segments[i].Marshalling(parcel);
        }
        return true;
    }
    void Unmarshalling(Parcel &parcel)
    {
        segmentCnt = parcel.ReadUint32();
        repeatCnt = parcel.ReadUint32();
        repeatSegment = parcel.ReadUint32();
        for (uint32_t i = 0; i < segmentCnt; i++) {
            segments[i].Unmarshalling(parcel);
        }
    }
};
#endif

} // namespace AudioStandard
} // namespace OHOS
#endif // AUDIO_TONE_INFO_H
