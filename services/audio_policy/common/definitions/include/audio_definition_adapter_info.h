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
#ifndef AUDIO_DEFINITION_POLICY_CONFIG_H
#define AUDIO_DEFINITION_POLICY_CONFIG_H

#include <list>
#include <set>
#include <unordered_map>
#include <string>

#include "audio_module_info.h"
#include "audio_info.h"
#include "audio_policy_log.h"

namespace OHOS {
namespace AudioStandard {
static const char* STR_INITED = "";

static const char* ADAPTER_TYPE_PRIMARY = "primary";
static const char* ADAPTER_TYPE_A2DP = "a2dp";
static const char* ADAPTER_TYPE_REMOTE = "remote";
static const char* ADAPTER_TYPE_FILE = "file";
static const char* ADAPTER_TYPE_USB = "usb";
static const char* ADAPTER_TYPE_DP = "dp";
static const char* ADAPTER_TYPE_SLE = "sle";

enum class PolicyXmlNodeType {
    ADAPTERS,
    XML_UNKNOWN
};

enum class AudioAdapterType {
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
    std::string name_ = STR_INITED;
    std::string value_ = STR_INITED;
};

struct PaPropInfo {
    std::string lib_ = STR_INITED;
    std::string paPropRole_ = STR_INITED;
    std::string moduleName_ = STR_INITED;
};

struct AdapterPipeInfo;
struct AdapterDeviceInfo;
class PolicyAdapterInfo;

struct PipeStreamPropInfo {
    AudioSampleFormat format_ = INVALID_WIDTH;
    uint32_t sampleRate_ = 0;
    AudioChannelLayout channelLayout_ = CH_LAYOUT_UNKNOWN;
    uint32_t bufferSize_ = 0;

    std::shared_ptr<AdapterPipeInfo> pipeInfo_;
    std::list<DeviceType> supportDevices_ {};
    std::unordered_map<DeviceType, AdapterDeviceInfo&> supportDeviceMap_ {};
};

class AudioPolicyConfigData {
public:
    static AudioPolicyConfigData&  GetInstance()
    {
        static AudioPolicyConfigData instance;
        return instance;
    }
    void Reorganize();
    void SetDeviceMaps(std::list<AdapterDeviceInfo> &deviceInfos);
    void SetPipeMaps(std::list<AdapterPipeInfo> &pipeInfos);
    void SetSupportDeviceAndPipeMaps(AdapterPipeInfo &pipeInfo);

    void SetVersion(const std::string version);
    void SetAdapterInfoMap(std::unordered_map<AudioAdapterType, PolicyAdapterInfo> &adapterInfoMap);
    void AddAdapterInfoToMap(AudioAdapterType type, PolicyAdapterInfo &info);

    std::string GetVersion();
    void GetAdapterInfoMap(std::unordered_map<AudioAdapterType, PolicyAdapterInfo> &adapterInfoMap);
    void GetDeviceInfoMap(std::unordered_map<DeviceType, AdapterDeviceInfo&> &deviceInfoMap);
    void GetPipeInfoMap(std::unordered_map<std::string, AdapterPipeInfo&> &pipeInfoMap);

private:
    std::string version_ = STR_INITED;
    std::unordered_map<AudioAdapterType, PolicyAdapterInfo> adapterInfoMap_ {};
    std::unordered_map<DeviceType, AdapterDeviceInfo&> deviceInfoMap_ {};
    std::unordered_map<AudioFlagType, AdapterPipeInfo&> pipeInfoMap_ {};
    // check: use output/input deviceMap or interface in adapterInfo
    // std::unordered_map<DeviceType, AdapterDeviceInfo&> outputDeviceMap_ {};
    // std::unordered_map<DeviceType, AdapterDeviceInfo&> inputDeviceMap_ {};
    // std::unordered_map<AudioFlagType, AdapterPipeInfo&> outputPipeMap_ {};
    // std::unordered_map<AudioFlagType, AdapterPipeInfo&> inputPipeMap_ {};
};

class PolicyAdapterInfo {
public:
    static AudioAdapterType GetAdapterType(const std::string &adapterName);
    AudioAdapterType GetTypeEnum();
    std::shared_ptr<AdapterDeviceInfo> GetDeviceInfoByType(DeviceType deviceType);
    std::shared_ptr<AdapterPipeInfo> GetPipeInfoByName(const std::string &pipeName);

    void SetAdapterName(const std::string adapterName);
    void SetAdapterSupportScene(const std::string adapterSupportScene);
    void SetDeviceInfos(std::list<AdapterDeviceInfo> &deviceInfos);
    void SetPipeInfos(std::list<AdapterPipeInfo> &pipeInfos);
    std::string GetAdapterName();
    std::string GetAdapterSupportScene();
    void GetDeviceInfos(std::list<AdapterDeviceInfo> &deviceInfos);
    void GetPipeInfos(std::list<AdapterPipeInfo> &pipeInfos);

private:
    std::string adapterName_ = STR_INITED;
    std::string adapterSupportScene_ = STR_INITED;
    std::list<AdapterDeviceInfo> deviceInfos_ {};
    std::list<AdapterPipeInfo> pipeInfos_ {};
};

struct AdapterDeviceInfo {
    std::string name_ = STR_INITED;
    DeviceType type_ = DEVICE_TYPE_NONE;
    AudioPortPin pin_ = PIN_NONE;
    DeviceRole role_ = DEVICE_ROLE_NONE;

    std::shared_ptr<PolicyAdapterInfo> adapterInfo_;
    std::list<std::string> supportPipes_ {};
    std::unordered_map<AudioFlagType, AdapterPipeInfo&> supportPipeMap_ {}; // flag <-> pipeInfo
};

struct AdapterPipeInfo {
    std::string name_ = STR_INITED;
    AudioPipeRole pipeRole_ = PIPE_ROLE_NONE;
    PaPropInfo paProp_ {};

    AudioPreloadType preloadAttr = PRELOAD_TYPE_UNKNOWN;
    std::list<AudioFlagType> supportFlags_ {};

    std::shared_ptr<PolicyAdapterInfo> adapterInfo_;
    std::list<PipeStreamPropInfo> streamPropInfos_ {};
    std::list<AttributeInfo> attributeInfos_ {};
    std::unordered_map<DeviceType, AdapterDeviceInfo&> supportDeviceMap_ {};
};
} // namespace AudioStandard
} // namespace OHOS

#endif // AUDIO_DEFINITION_POLICY_CONFIG_H
