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
#define LOG_TAG "AudioDefinitionPolicyUtils"
#endif

#include "audio_definition_policy_utils.h"
#include <ability_manager_client.h>
#include "iservice_registry.h"
#include "parameter.h"
#include "parameters.h"
#include "audio_utils.h"
#include "audio_log.h"

namespace OHOS {
namespace AudioStandard {
std::unordered_map<std::string, DeviceRole> AudioDefinitionPolicyUtils::deviceRoleStrToEnum = {
    {"input", INPUT_DEVICE},
    {"output", OUTPUT_DEVICE},
};

std::unordered_map<std::string, AudioPipeRole> AudioDefinitionPolicyUtils::pipeRoleStrToEnum = {
    {"input", PIPE_ROLE_IN},
    {"output", PIPE_ROLE_OUT},
};

std::unordered_map<std::string, DeviceType> AudioDefinitionPolicyUtils::deviceTypeStrToEnum = {
    {"DEVICE_TYPE_EARPIECE", DEVICE_TYPE_EARPIECE},
    {"DEVICE_TYPE_SPEAKER", DEVICE_TYPE_SPEAKER},
    {"DEVICE_TYPE_WIRED_HEADSET", DEVICE_TYPE_WIRED_HEADSET},
    {"DEVICE_TYPE_WIRED_HEADPHONES", DEVICE_TYPE_WIRED_HEADPHONES},
    {"DEVICE_TYPE_BLUETOOTH_SCO", DEVICE_TYPE_BLUETOOTH_SCO},
    {"DEVICE_TYPE_BLUETOOTH_A2DP", DEVICE_TYPE_BLUETOOTH_A2DP},
    {"DEVICE_TYPE_BLUETOOTH_A2DP_IN", DEVICE_TYPE_BLUETOOTH_A2DP_IN},
    {"DEVICE_TYPE_MIC", DEVICE_TYPE_MIC},
    {"DEVICE_TYPE_WAKEUP", DEVICE_TYPE_WAKEUP},
    {"DEVICE_TYPE_USB_HEADSET", DEVICE_TYPE_USB_HEADSET},
    {"DEVICE_TYPE_DP", DEVICE_TYPE_DP},
    {"DEVICE_TYPE_REMOTE_CAST", DEVICE_TYPE_REMOTE_CAST},
    {"DEVICE_TYPE_HDMI", DEVICE_TYPE_HDMI},
    {"DEVICE_TYPE_LINE_DIGITAL", DEVICE_TYPE_LINE_DIGITAL},
    {"DEVICE_TYPE_FILE_SINK", DEVICE_TYPE_FILE_SINK},
    {"DEVICE_TYPE_FILE_SOURCE", DEVICE_TYPE_FILE_SOURCE},
    {"DEVICE_TYPE_EXTERN_CABLE", DEVICE_TYPE_EXTERN_CABLE},
    {"DEVICE_TYPE_DEFAULT", DEVICE_TYPE_DEFAULT},
    {"DEVICE_TYPE_USB_ARM_HEADSET", DEVICE_TYPE_USB_ARM_HEADSET},
};

std::unordered_map<std::string, AudioPortPin> AudioDefinitionPolicyUtils::pinStrToEnum = {
    {"PIN_OUT_SPEAKER", PIN_OUT_SPEAKER},
    {"PIN_OUT_HEADSET", PIN_OUT_HEADSET},
    {"PIN_OUT_LINEOUT", PIN_OUT_LINEOUT},
    {"PIN_OUT_HDMI", PIN_OUT_HDMI},
    {"PIN_OUT_USB", PIN_OUT_USB},
    {"PIN_OUT_USB_EXT", PIN_OUT_USB_EXT},
    {"PIN_OUT_EARPIECE", PIN_OUT_EARPIECE},
    {"PIN_OUT_BLUETOOTH_SCO", PIN_OUT_BLUETOOTH_SCO},
    {"PIN_OUT_DAUDIO_DEFAULT", PIN_OUT_DAUDIO_DEFAULT},
    {"PIN_OUT_HEADPHONE", PIN_OUT_HEADPHONE},
    {"PIN_OUT_USB_HEADSET", PIN_OUT_USB_HEADSET},
    {"PIN_OUT_BLUETOOTH_A2DP", PIN_OUT_BLUETOOTH_A2DP},
    {"PIN_OUT_DP", PIN_OUT_DP},
    {"PIN_IN_MIC", PIN_IN_MIC},
    {"PIN_IN_HS_MIC", PIN_IN_HS_MIC},
    {"PIN_IN_LINEIN", PIN_IN_LINEIN},
    {"PIN_IN_USB_EXT", PIN_IN_USB_EXT},
    {"PIN_IN_BLUETOOTH_SCO_HEADSET", PIN_IN_BLUETOOTH_SCO_HEADSET},
    {"PIN_IN_DAUDIO_DEFAULT", PIN_IN_DAUDIO_DEFAULT},
    {"PIN_IN_USB_HEADSET", PIN_IN_USB_HEADSET},
};

std::unordered_map<std::string, AudioSampleFormat> AudioDefinitionPolicyUtils::formatStrToEnum = {
    {"s16le", SAMPLE_S16LE},
    {"s24le", SAMPLE_S24LE},
    {"s32le", SAMPLE_S32LE},
};

std::unordered_map<std::string, AudioChannelLayout> AudioDefinitionPolicyUtils::layoutStrToEnum = {
    {"CH_LAYOUT_MONO", CH_LAYOUT_MONO},
    {"CH_LAYOUT_STEREO", CH_LAYOUT_STEREO},
    {"CH_LAYOUT_2POINT1", CH_LAYOUT_2POINT1},
    {"CH_LAYOUT_3POINT0", CH_LAYOUT_3POINT0},
    {"CH_LAYOUT_3POINT1", CH_LAYOUT_3POINT1},
    {"CH_LAYOUT_5POINT0", CH_LAYOUT_5POINT0},
    {"CH_LAYOUT_2POINT1POINT2", CH_LAYOUT_2POINT1POINT2},
    {"CH_LAYOUT_5POINT1", CH_LAYOUT_5POINT1},
    {"CH_LAYOUT_HEXAGONAL", CH_LAYOUT_HEXAGONAL},
    {"CH_LAYOUT_3POINT1POINT2", CH_LAYOUT_3POINT1POINT2},
    {"CH_LAYOUT_7POINT0", CH_LAYOUT_7POINT0},
    {"CH_LAYOUT_7POINT1", CH_LAYOUT_7POINT1},
    {"CH_LAYOUT_7POINT1POINT2", CH_LAYOUT_7POINT1POINT2},
    {"CH_LAYOUT_7POINT1POINT4", CH_LAYOUT_7POINT1POINT4},
};

std::unordered_map<std::string, AudioFlagType> AudioDefinitionPolicyUtils::flagStrToEnum = {
    {"AUDIO_OUTPUT_FLAG_NORMAL", FLAG_TYPE_OUTPUT_NORMAL},
    {"AUDIO_OUTPUT_FLAG_FAST", FLAG_TYPE_OUTPUT_FAST},
    {"AUDIO_OUTPUT_FLAG_VOIP", FLAG_TYPE_OUTPUT_VOIP},
    {"AUDIO_OUTPUT_FLAG_DIRECT", FLAG_TYPE_OUTPUT_DIRECT},
    {"AUDIO_OUTPUT_FLAG_HD", FLAG_TYPE_OUTPUT_HD},
    {"AUDIO_INPUT_FLAG_NORMAL", FLAG_TYPE_INPUT_NORMAL},
    {"AUDIO_INPUT_FLAG_FAST", FLAG_TYPE_INPUT_FAST},
    {"AUDIO_INPUT_FLAG_VOIP", FLAG_TYPE_INPUT_VOIP},
    {"AUDIO_INPUT_FLAG_WAKEUP", FLAG_TYPE_INPUT_WAKEUP},
};

std::unordered_map<std::string, AudioPreloadType> AudioDefinitionPolicyUtils::preloadStrToEnum = {
    {"false", PRELOAD_TYPE_NOTSUPPORT},
    {"true", PRELOAD_TYPE_SUPPORT},
};

uint32_t AudioDefinitionPolicyUtils::PcmFormatToBytes(AudioSampleFormat format)
{
    // AudioSampleFormat / PCM_8_BIT
    switch (format) {
        case SAMPLE_U8:
            return 1; // 1 byte
        case SAMPLE_S16LE:
            return 2; // 2 byte
        case SAMPLE_S24LE:
            return 3; // 3 byte
        case SAMPLE_S32LE:
            return 4; // 4 byte
        case SAMPLE_F32LE:
            return 4; // 4 byte
        default:
            return 2; // 2 byte
    }
}

AudioChannel AudioDefinitionPolicyUtils::ConvertLayoutToAudioChannel(AudioChannelLayout layout)
{
    AudioChannel channel = AudioChannel::CHANNEL_UNKNOWN;
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
            channel = AudioChannel::CHANNEL_UNKNOWN;
            break;
    }
    return channel;
}
}
}
