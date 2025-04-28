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
#ifndef AUDIO_DEVICE_STREAM_INFO_H
#define AUDIO_DEVICE_STREAM_INFO_H

#include <parcel.h>
#include <set>
#include <limits>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <list>
#include "securec.h"
#include <audio_stream_info.h>

namespace OHOS {
namespace AudioStandard {
constexpr size_t AUDIO_DEVICE_INFO_SIZE_LIMIT = 30;

template<typename T> bool MarshallingSetInt32(const std::set<T> &value, Parcel &parcel)
{
    size_t size = value.size();
    if (!parcel.WriteUint64(size)) {
        return false;
    }
    for (const auto &i : value) {
        if (!parcel.WriteInt32(i)) {
            return false;
        }
    }
    return true;
}

template<typename T> std::set<T> UnmarshallingSetInt32(Parcel &parcel,
    const size_t maxSize = std::numeric_limits<size_t>::max())
{
    size_t size = parcel.ReadUint64();
    // due to security concerns, sizelimit has been imposed
    if (size > maxSize) {
        size = maxSize;
    }

    std::set<T> res;
    for (size_t i = 0; i < size; i++) {
        res.insert(static_cast<T>(parcel.ReadInt32()));
    }
    return res;
}

template<typename T> bool MarshallingSetInt64(const std::set<T> &value, Parcel &parcel)
{
    size_t size = value.size();
    if (!parcel.WriteUint64(size)) {
        return false;
    }
    for (const auto &i : value) {
        if (!parcel.WriteInt64(i)) {
            return false;
        }
    }
    return true;
}

template<typename T> std::set<T> UnmarshallingSetInt64(Parcel &parcel,
    const size_t maxSize = std::numeric_limits<size_t>::max())
{
    size_t size = parcel.ReadUint64();
    // due to security concerns, sizelimit has been imposed
    if (size > maxSize) {
        size = maxSize;
    }

    std::set<T> res;
    for (size_t i = 0; i < size; i++) {
        res.insert(static_cast<T>(parcel.ReadInt64()));
    }
    return res;
}

static AudioChannel ConvertLayoutToAudioChannel(AudioChannelLayout layout)
{
    AudioChannel channel = AudioChannel::CHANNEL_UNKNOW;
    switch (layout) {
        case AudioChannelLayout::CH_LAYOUT_MONO:
            channel = AudioChannel::MONO;
            break;
        case AudioChannelLayout::CH_LAYOUT_STEREO:
            channel = AudioChannel::STEREO;
            break;
        case AudioChannelLayout::CH_LAYOUT_2POINT1:
        case AudioChannelLayout::CH_LAYOUT_3POINT0:
            channel = AudioChannel::CHANNEL_3;
            break;
        case AudioChannelLayout::CH_LAYOUT_3POINT1:
        case AudioChannelLayout::CH_LAYOUT_4POINT0:
        case AudioChannelLayout::CH_LAYOUT_QUAD:
            channel = AudioChannel::CHANNEL_4;
            break;
        case AudioChannelLayout::CH_LAYOUT_5POINT0:
        case AudioChannelLayout::CH_LAYOUT_2POINT1POINT2:
            channel = AudioChannel::CHANNEL_5;
            break;
        case AudioChannelLayout::CH_LAYOUT_5POINT1:
        case AudioChannelLayout::CH_LAYOUT_HEXAGONAL:
        case AudioChannelLayout::CH_LAYOUT_3POINT1POINT2:
            channel = AudioChannel::CHANNEL_6;
            break;
        case AudioChannelLayout::CH_LAYOUT_7POINT0:
            channel = AudioChannel::CHANNEL_7;
            break;
        case AudioChannelLayout::CH_LAYOUT_7POINT1:
            channel = AudioChannel::CHANNEL_8;
            break;
        case AudioChannelLayout::CH_LAYOUT_7POINT1POINT2:
            channel = AudioChannel::CHANNEL_10;
            break;
        case AudioChannelLayout::CH_LAYOUT_7POINT1POINT4:
            channel = AudioChannel::CHANNEL_12;
            break;
        default:
            channel = AudioChannel::CHANNEL_UNKNOW;
            break;
    }
    return channel;
}

static AudioChannelLayout ConvertAudioChannelToLayout(AudioChannel channel)
{
    AudioChannelLayout channelLayout = AudioChannelLayout::CH_LAYOUT_UNKNOWN;

    switch (channel) {
        case AudioChannel::MONO:
            channelLayout = AudioChannelLayout::CH_LAYOUT_MONO;
            break;
        case AudioChannel::STEREO:
            channelLayout = AudioChannelLayout::CH_LAYOUT_STEREO;
            break;
        case AudioChannel::CHANNEL_3:
            channelLayout = AudioChannelLayout::CH_LAYOUT_2POINT1;
            break;
        case AudioChannel::CHANNEL_4:
            channelLayout = AudioChannelLayout::CH_LAYOUT_3POINT1;
            break;
        case AudioChannel::CHANNEL_5:
            channelLayout = AudioChannelLayout::CH_LAYOUT_2POINT1POINT2;
            break;
        case AudioChannel::CHANNEL_6:
            channelLayout = AudioChannelLayout::CH_LAYOUT_5POINT1;
            break;
        case AudioChannel::CHANNEL_7:
            channelLayout = AudioChannelLayout::CH_LAYOUT_7POINT0;
            break;
        case AudioChannel::CHANNEL_8:
            channelLayout = AudioChannelLayout::CH_LAYOUT_7POINT1;
            break;
        case AudioChannel::CHANNEL_10:
            channelLayout = AudioChannelLayout::CH_LAYOUT_7POINT1POINT2;
            break;
        case AudioChannel::CHANNEL_12:
            channelLayout = AudioChannelLayout::CH_LAYOUT_7POINT1POINT4;
            break;
        default:
            channelLayout = AudioChannelLayout::CH_LAYOUT_UNKNOWN;
            break;
    }

    return channelLayout;
}

template<typename T>
static void SerializeEnum(std::ostream &os, const T &value)
{
    int32_t intValue = static_cast<int32_t>(value);
    os.write(reinterpret_cast<const char *>(&intValue), sizeof(intValue));
}

template<typename T>
static void DeserializeEnum(std::istream &is, T &value)
{
    int32_t intValue = 0;
    if (!is.read(reinterpret_cast<char *>(&intValue), sizeof(intValue))) {
        return;
    }
    value = static_cast<T>(intValue);
}

template<typename T>
static void SerializeSet(std::ostream &os, const std::set<T> &s)
{
    uint32_t len = static_cast<uint32_t>(s.size());
    os.write(reinterpret_cast<const char *>(&len), sizeof(len));
    for (const auto &item : s) {
        SerializeEnum(os, item);
    }
}

template<typename T>
static void DeserializeSet(std::istream &is, std::set<T> &s)
{
    uint32_t len = 0;
    s.clear();
    if (!is.read(reinterpret_cast<char *>(&len), sizeof(len))) {
        return;
    }
    for (uint32_t i = 0; i < len; i++) {
        T item;
        DeserializeEnum(is, item);
        s.insert(item);
    }
}

struct DeviceStreamInfo {
    AudioEncodingType encoding = AudioEncodingType::ENCODING_PCM;
    AudioSampleFormat format = AudioSampleFormat::INVALID_WIDTH;
    std::set<AudioChannelLayout> channelLayout;
    std::set<AudioSamplingRate> samplingRate;

    DeviceStreamInfo(AudioSamplingRate samplingRate_, AudioEncodingType encoding_, AudioSampleFormat format_,
        AudioChannelLayout channelLayout_)
        : encoding(encoding_), format(format_), channelLayout({channelLayout_}), samplingRate({samplingRate_})
    {}
    DeviceStreamInfo(AudioStreamInfo audioStreamInfo) : DeviceStreamInfo(audioStreamInfo.samplingRate,
        audioStreamInfo.encoding, audioStreamInfo.format, audioStreamInfo.channelLayout)
    {}
    DeviceStreamInfo() = default;

    std::set<AudioChannel> GetChannels() const
    {
        std::set<AudioChannel> channels;
        for (const auto &layout : channelLayout) {
            channels.insert(ConvertLayoutToAudioChannel(layout));
        }
        return channels;
    }

    // warning: force set default channelLayout by channel
    void SetChannels(const std::set<AudioChannel> &channels)
    {
        channelLayout.clear();
        for (const auto &channel : channels) {
            channelLayout.insert(ConvertAudioChannelToLayout(channel));
        }
    }

    bool Marshalling(Parcel &parcel) const
    {
        return parcel.WriteInt32(static_cast<int32_t>(encoding))
            && parcel.WriteInt32(static_cast<int32_t>(format))
            && MarshallingSetInt32(samplingRate, parcel)
            && MarshallingSetInt64(channelLayout, parcel);
    }
    void Unmarshalling(Parcel &parcel)
    {
        encoding = static_cast<AudioEncodingType>(parcel.ReadInt32());
        format = static_cast<AudioSampleFormat>(parcel.ReadInt32());
        samplingRate = UnmarshallingSetInt32<AudioSamplingRate>(parcel, AUDIO_DEVICE_INFO_SIZE_LIMIT);
        channelLayout = UnmarshallingSetInt64<AudioChannelLayout>(parcel, AUDIO_DEVICE_INFO_SIZE_LIMIT);
    }

    static std::string SerializeList(const std::list<DeviceStreamInfo> &streamInfoList)
    {
        std::string res;
        for (const auto &streamInfo : streamInfoList) {
            std::string infoStr = streamInfo.Serialize();
            uint32_t len = infoStr.length();
            res.append(reinterpret_cast<const char*>(&len), sizeof(len));
            res.append(infoStr);
        }
        return res;
    }

    static std::list<DeviceStreamInfo> DeserializeList(const std::string &data)
    {
        std::list<DeviceStreamInfo> res;
        uint32_t offset = 0;
        while (offset < data.length()) {
            uint32_t len = 0;
            if (data.length() - offset < sizeof(len)) {
                return {};
            }
            if (memcpy_s(&len, sizeof(len), data.data() + offset, sizeof(len)) != 0) {
                return {};
            }
            offset += sizeof(len);
            if (data.length() - offset < len) {
                return {};
            }
            std::string infoStr(data.data() + offset, len);
            offset += len;
            DeviceStreamInfo streamInfo;
            streamInfo.Deserialize(infoStr);
            res.push_back(streamInfo);
        }
        return res;
    }

    std::string Serialize() const
    {
        std::ostringstream oss(std::ios::binary);
        SerializeEnum(oss, encoding);
        SerializeEnum(oss, format);
        SerializeSet(oss, samplingRate);
        SerializeSet(oss, channelLayout);
        return oss.str();
    }

    void Deserialize(const std::string &data)
    {
        std::istringstream iss(data, std::ios::binary);
        DeserializeEnum(iss, encoding);
        DeserializeEnum(iss, format);
        DeserializeSet(iss, samplingRate);
        DeserializeSet(iss, channelLayout);
    }

    bool operator==(const DeviceStreamInfo& info) const
    {
        return encoding == info.encoding && format == info.format &&
            channelLayout == info.channelLayout && samplingRate == info.samplingRate;
    }

    bool CheckParams()
    {
        if (samplingRate.size() == 0) {
            return false;
        }
        if (channelLayout.size() == 0) {
            return false;
        }
        return true;
    }
};
} // namespace AudioStandard
} // namespace OHOS
#endif // AUDIO_DEVICE_STREAM_INFO_H