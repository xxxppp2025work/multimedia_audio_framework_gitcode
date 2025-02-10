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
#ifndef AUDIO_DEFINITION_POLICY_UTILS_H
#define AUDIO_DEFINITION_POLICY_UTILS_H

#include <bitset>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include "singleton.h"
#include "audio_info.h"
#include "audio_utils.h"
#include "audio_errors.h"

namespace OHOS {
namespace AudioStandard {
class AudioDefinitionPolicyUtils {
public:
    static AudioDefinitionPolicyUtils& GetInstance()
    {
        static AudioDefinitionPolicyUtils instance;
        return instance;
    }
    uint32_t PcmFormatToBytes(AudioSampleFormat format);
    AudioChannel ConvertLayoutToAudioChannel(AudioChannelLayout layout);
private:
    AudioDefinitionPolicyUtils() {}
    ~AudioDefinitionPolicyUtils() {}
public:
    static std::unordered_map<std::string, DeviceRole> deviceRoleStrToEnum;
    static std::unordered_map<std::string, AudioPipeRole> pipeRoleStrToEnum;
    static std::unordered_map<std::string, DeviceType> deviceTypeStrToEnum;
    static std::unordered_map<std::string, AudioPortPin> pinStrToEnum;
    static std::unordered_map<std::string, AudioSampleFormat> formatStrToEnum;
    static std::unordered_map<std::string, AudioChannelLayout> layoutStrToEnum;
    static std::unordered_map<std::string, AudioFlagType> flagStrToEnum;
    static std::unordered_map<std::string, AudioPreloadType> preloadStrToEnum;
};

} // namespace AudioStandard
} // namespace OHOS
#endif // AUDIO_DEFINITION_POLICY_UTILS_H
