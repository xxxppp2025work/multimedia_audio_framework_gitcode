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
#ifndef ST_AUDIO_BACKGROUND_MANAGER_H
#define ST_AUDIO_BACKGROUND_MANAGER_H

#include <bitset>
#include <list>
#include <string>
#include <mutex>
#include "audio_policy_manager_factory.h"
#include "audio_stream_collector.h"
#include "common_event_manager.h"


namespace OHOS {
namespace AudioStandard {

class AudioBackgroundManager {
public:
    static AudioBackgroundManager& GetInstance()
    {
        static AudioBackgroundManager instance;
        return instance;
    }

    int32_t SetQueryAllowedPlaybackCallback(const sptr<IRemoteObject> &object);
    bool IsAllowedPlayback(const int32_t &uid, const int32_t &pid);
    int32_t NofitySessionStateChange(const int32_t uid, const int32_t pid, const bool hasSession);
    int32_t NotifyFreezeStateChange(const std::set<int32_t> &pidList, const bool isFreeze);

private:
    AudioBackgroundManager() : streamCollector_(AudioStreamCollector::GetAudioStreamCollector()) {}
    ~AudioBackgroundManager() {}
   
private:
    AudioStreamCollector& streamCollector_;

    sptr<IStandardAudioPolicyManagerListener> policyManagerListener_;
};
}
}
#endif