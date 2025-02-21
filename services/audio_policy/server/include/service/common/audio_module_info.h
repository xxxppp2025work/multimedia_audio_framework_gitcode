/*
 * Copyright (c) 2021-2024 Huawei Device Co., Ltd.
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

#ifndef ST_AUDIO_CONFIG_H
#define ST_AUDIO_CONFIG_H

#include <list>
#include <set>
#include <string>
#include <vector>

#include "audio_info.h"
#include "audio_policy_log.h"

namespace OHOS {
namespace AudioStandard {
namespace {
inline static const char* PRIMARY_CLASS = "primary";
inline static const char* A2DP_CLASS = "a2dp";
inline static const char* USB_CLASS = "usb";
inline static const char* DP_CLASS = "dp";
inline static const char* FILE_CLASS = "file_io";
inline static const char* REMOTE_CLASS = "remote";
inline static const char* OFFLOAD_CLASS = "offload";
inline static const char* MCH_CLASS = "multichannel";
inline static const char* INVALID_CLASS = "invalid";
inline static const char* DIRECT_VOIP_CLASS = "primary_direct_voip";
inline static const char* MMAP_VOIP_CLASS = "primary_mmap_voip";
inline static const char* BLUETOOTH_SPEAKER = "Bt_Speaker";
inline static const char* BLUETOOTH_MIC = "Bt_Mic";
inline static const char* PRIMARY_SPEAKER = "Speaker";
inline static const char* OFFLOAD_PRIMARY_SPEAKER = "Offload_Speaker";
inline static const char* MCH_PRIMARY_SPEAKER = "MCH_Speaker";
inline static const char* USB_SPEAKER = "Usb_arm_speaker";
inline static const char* DP_SINK = "DP_speaker";
inline static const char* USB_MIC = "Usb_arm_mic";
inline static const char* PRIMARY_MIC = "Built_in_mic";
inline static const char* PRIMARY_WAKEUP_MIC = "Built_in_wakeup";
inline static const char* FILE_SINK = "file_sink";
inline static const char* FILE_SOURCE = "file_source";
inline static const char* PIPE_SINK = "fifo_output";
inline static const char* PIPE_SOURCE = "fifo_input";
inline static const char* INTERNAL_PORT = "internal";
inline static const char* ROLE_SOURCE = "source";
inline static const char* ROLE_SINK = "sink";
inline static const char* PORT_NONE = "none";
inline static const char* PRIMARY_DIRECT_VOIP = "direct_voip";
inline static const char* PRIMARY_MMAP_VOIP = "mmap_voip";
}

enum NodeName {
    DEVICE_CLASS,
    MODULES,
    MODULE,
    PORTS,
    PORT,
    AUDIO_INTERRUPT_ENABLE,
    UPDATE_ROUTE_SUPPORT,
    AUDIO_LATENCY,
    SINK_LATENCY,
    VOLUME_GROUP_CONFIG,
    INTERRUPT_GROUP_CONFIG,
    UNKNOWN
};

enum ClassType {
    TYPE_PRIMARY,
    TYPE_A2DP,
    TYPE_USB,
    TYPE_FILE_IO,
    TYPE_REMOTE_AUDIO,
    TYPE_DP,
    TYPE_INVALID
};

struct AudioModuleInfo {
    std::string className;
    std::string name;
    std::string adapterName;
    std::string id;
    std::string lib;
    std::string role;

    std::string rate;

    std::set<uint32_t> supportedRate_;
    std::set<uint32_t> supportedChannels_;

    std::string format;
    std::string channels;
    std::string bufferSize;
    std::string fixedLatency;
    std::string sinkLatency;
    std::string renderInIdleState;
    std::string OpenMicSpeaker;
    std::string fileName;
    std::string networkId;
    std::string deviceType;
    std::string sceneName;
    std::string sourceType;
    std::string offloadEnable;

    std::string ecType;
    std::string ecAdapter;
    std::string ecSamplingRate;
    std::string ecFormat;
    std::string ecChannels;
    std::string openMicRef;
    std::string micRefRate;
    std::string micRefFormat;
    std::string micRefChannels;

    std::list<AudioModuleInfo> ports;
    std::string extra;
};

} // namespace AudioStandard
} // namespace OHOS

#endif // ST_AUDIO_CONFIG_H
