/*
 * Copyright (c) 2021-2025 Huawei Device Co., Ltd.
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
#define LOG_TAG "AudioBackgroundManager"
#endif

#include "audio_background_manager.h"
#include <ability_manager_client.h>
#include "iservice_registry.h"
#include "parameter.h"
#include "parameters.h"
#include "audio_policy_log.h"
#include "audio_manager_listener_stub.h"
#include "audio_inner_call.h"
#include "i_policy_provider.h"

#include "audio_server_proxy.h"


namespace OHOS {
namespace AudioStandard {

constexpr int32_t BOOTUP_MUSIC_UID = 1003;
static const int64_t WATI_PLAYBACK_TIME = 200000; // 200ms
mutex g_policyMgrListenerMutex;

int32_t AudioBackgroundManager::SetQueryAllowedPlaybackCallback(const sptr<IRemoteObject> &object)
{
    lock_guard<mutex> lock(g_policyMgrListenerMutex);
    policyManagerListener_ = iface_cast<IStandardAudioPolicyManagerListener>(object);
    return SUCCESS;
}

bool AudioBackgroundManager::IsAllowedPlayback(const int32_t &uid, const int32_t &pid)
{
#ifdef AVSESSION_ENABLE
    // Temporary solution to avoid performance issues
    if (uid == BOOTUP_MUSIC_UID) {
        return true;
    }
    lock_guard<mutex> lock(g_policyMgrListenerMutex);
    bool allowed = false;
    if (policyManagerListener_ != nullptr) {
        allowed = policyManagerListener_->OnQueryAllowedPlayback(uid, pid);
    }
    if (!allowed) {
        usleep(WATI_PLAYBACK_TIME); //wait for 200ms
        AUDIO_INFO_LOG("IsAudioPlaybackAllowed Try again after 200ms");
        if (policyManagerListener_ != nullptr) {
            allowed = policyManagerListener_->OnQueryAllowedPlayback(uid, pid);
        }
    }
    return allowed;
#endif
    return true;
}

int32_t AudioBackgroundManager::NofitySessionStateChange(const int32_t uid, const int32_t pid, bool hasSession)
{
    AUDIO_INFO_LOG("UID:%{public}d, PID:%{public}d, Session State: %{public}d", uid, pid, hasSession);
    return SUCCESS;
}

int32_t AudioBackgroundManager::NotifyFreezeStateChange(const std::set<int32_t> &pidList, bool isFreeze)
{
    for(auto pid : pidList) {
        AUDIO_INFO_LOG("PID:%{public}d, Freeze State: %{public}d",pid, isFreeze);
    }
    return SUCCESS;
}

}
}
