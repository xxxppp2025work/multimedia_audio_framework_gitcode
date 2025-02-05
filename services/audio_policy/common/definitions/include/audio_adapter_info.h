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
#ifndef ST_AUDIO_POLICY_CONFIG_H
#define ST_AUDIO_POLICY_CONFIG_H

#include <list>
#include <set>
#include <unordered_map>
#include <string>

#include "audio_module_info.h"
#include "audio_info.h"
#include "audio_policy_log.h"

namespace OHOS {
namespace AudioStandard {
static const char* STR_INIT = "";

static const char* ADAPTER_TYPE_PRIMARY = "primary";
static const char* ADAPTER_TYPE_A2DP = "a2dp";
static const char* ADAPTER_TYPE_REMOTE = "remote";
static const char* ADAPTER_TYPE_FILE = "file";
static const char* ADAPTER_TYPE_USB = "usb";
static const char* ADAPTER_TYPE_DP = "dp";
static const char* ADAPTER_TYPE_SLE = "sle";

enum class XmlNodeType {
    ADAPTERS,
    XML_UNKNOWN
};

enum class AdapterType {
    TYPE_PRIMARY,
    TYPE_A2DP,
    TYPE_USB,
    TYPE_FILE_IO,
    TYPE_REMOTE_AUDIO,
    TYPE_DP,
    TYPE_SLE,
    TYPE_INVALID
};

enum class AdapterInfoType {
    PIPES,
    DEVICES,
    UNKNOWN
};

enum class PipeInfoType {
    PA_PROP,
    STREAM_PROP,
    ATTRIBUTE,
    UNKNOWN
};

struct AttributeInfo {
    std::string name_ = STR_INIT;
    std::string value_ = STR_INIT;
}

struct PaPropInfo {
    std::string lib_ = STR_INIT;
    std::string paPropRole_ = STR_INIT;
    std::string moduleName_ = STR_INIT;
};

struct PipeInfo;
struct AdapterDeviceInfo;
class AudioAdapterInfo;

struct StreamPropInfo {
    AudioSampleFormat format_ = INVALID_WIDTH;
    uint32_t sampleRate_ = 0;
    AudioChannelLayout channelLayout_ = CH_LAYOUT_UNKNOWN;
    uint32_t bufferSize_ = 0;

    PipeInfo *pipeInfo_;
    std::list<DeviceType> supportDevices_ {}; // delete?
    std::unordered_map<DeviceType, AdapterDeviceInfo&> supportDeviceMap_ {};
};

class AudioPolicyConfigData {
public:
    static AudioPolicyConfigData&  GetInstace()
    {
        static AudioPolicyConfigData instance;
        return instance;
    }
    void Reorganize();
    void SetDeviceMaps(std::list<AdapterDeviceInfo> &deviceInfos);
    void SetPipeMaps(std::list<PipeInfo> &pipeInfos);
    void SetSupportDeviceAndPipeMaps(PipeInfo &pipeInfo);

    std::string version_ = STR_INIT;
    std::unordered_map<AdapterType, AudioAdapterInfo> adapterInfoMap_ {};
    std::unordered_map<DeviceType, AdapterDeviceInfo&> deviceInfoMap_ {};
    std::unordered_map<std::string, PipeInfo&> pipeInfoMap_ {};
    // check: use output/input deviceMap or interface in adapterInfo
    std::unordered_map<DeviceType, AdapterDeviceInfo&> outputDeviceMap_ {};
    std::unordered_map<DeviceType, AdapterDeviceInfo&> inputDeviceMap_ {};
    std::unordered_map<std::string, PipeInfo&> outputPipeMap_ {};
    std::unordered_map<std::string, PipeInfo&> inputPipeMap_ {};
};

class AudioAdapterInfo {
public:
    static AdapterType GetAdapterType(const std::string &adapterName);
    AdapterType GetTypeEnum();
    AdapterDeviceInfo* GetDeviceInfoByType(DeviceType deviceType);
    PipeInfo* GetPipeInfoByName(const std::string &pipeName);

    std::string adapterName_ = STR_INIT;
    std::string adapterSupportScene_ = STR_INIT;
    std::list<AdapterDeviceInfo> deviceInfos_ {};
    std::list<PipeInfo> pipeInfos_ {};
};

struct AdapterDeviceInfo {
    std::string name_ = STR_INIT;
    DeviceType type_ = DEVICE_TYPE_NONE;
    AudioPortPin pin_ = PIN_NONE;
    DeviceRole role_ = DEVICE_ROLE_NONE;

    AudioAdapterInfo *adapterInfo_;
    std::list<std::string> supportPipes_ {}; // delete?
    std::unordered_map<std::string, PipeInfo&> supportPipeMap_ {};
};

struct PipeInfo {
    std::string name_ = STR_INIT;
    AudioPipeRole pipeRole_ = PIPE_ROLE_NONE;
    PaPropInfo paProp_ {};

    AudioPreloadType preloadAttr = PRELOAD_TYPE_UNKNOWN;
    std::list<AudioFlagType> supportFlags_ {};

    AudioAdapterInfo *adapterInfo_;
    std::list<StreamPropInfo> streamPropInfos_ {};
    std::list<AttributeInfo> attributeInfos_ {};
    std::unordered_map<DeviceType, AdapterDeviceInfo&> supportDeviceMap_ {};
};
} // namespace AudioStandard
} // namespace OHOS

#endif // ST_AUDIO_POLICY_CONFIG_H