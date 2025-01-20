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
#define LOG_TAG "AudioAbilityManager"
#endif

#include "audio_ability_manager.h"

#include <mutex>
#include "audio_server_death_recipient.h"
#include "audio_manager_base.h"
#include "audio_common_log.h"
#include "xcollie/xcollie.h"
#include "xcollie/xcollie_define.h"
#include "system_ability_definition.h"
#include "audio_utils.h"
#include "bundle_mgr_interface.h"

#include "iservice_registry.h"
#include "system_ability_definition.h"
#include "bundle_mgr_interface.h"

#include "audio_common_log.h"
#include "audio_errors.h"
#include "audio_manager_base.h"

#include "audio_server_death_recipient.h"
#include "audio_policy_manager.h"
#include "audio_utils.h"
#include "audio_manager_listener_stub.h"
#include "ipc_skeleton.h"

#include "avsession_manager.h"


namespace OHOS {
namespace AudioStandard {
constexpr unsigned int XCOLLIE_TIME_OUT_SECONDS = 10;
std::mutex g_asProxyMutex;
sptr<IStandardAudioService> g_asProxy = nullptr;
static const int64_t WATI_PLAYBACK_TIME = 200000; // 200
static constexpr int32_t BOOTUP_MUSIC_UID = 1003;

AudioAbilityManager::~AudioAbilityManager()
{
    if (g_asProxy != nullptr) {
        g_asProxy = nullptr;
    }
}

static const sptr<IStandardAudioService> GetAudioAbilityManagerProxy()
{
    AudioXCollie xcollieGetAudioSystemManagerProxy("GetAudioSystemManagerProxy", XCOLLIE_TIME_OUT_SECONDS);
    std::lock_guard<std::mutex> lock(g_asProxyMutex);
    if (g_asProxy == nullptr) {
        AudioXCollie xcollieGetSystemAbilityManager("GetSystemAbilityManager", XCOLLIE_TIME_OUT_SECONDS);
        auto samgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
        CHECK_AND_RETURN_RET_LOG(samgr != nullptr, nullptr, "get sa manager failed");
        xcollieGetSystemAbilityManager.CancelXCollieTimer();

        AudioXCollie xcollieGetSystemAbility("GetSystemAbility", XCOLLIE_TIME_OUT_SECONDS);
        sptr<IRemoteObject> object = samgr->GetSystemAbility(AUDIO_DISTRIBUTED_SERVICE_ID);
        CHECK_AND_RETURN_RET_LOG(object != nullptr, nullptr, "get audio service remote object failed");
        g_asProxy = iface_cast<IStandardAudioService>(object);
        CHECK_AND_RETURN_RET_LOG(g_asProxy != nullptr, nullptr, "get audio service proxy failed");
        xcollieGetSystemAbility.CancelXCollieTimer();
    }
    sptr<IStandardAudioService> gasp = g_asProxy;
    return gasp;
}

AudioAbilityManager *AudioAbilityManager::GetInstance()
{
    static AudioAbilityManager audioManager;
    return &audioManager;
}

uint64_t AudioAbilityManager::GetTransactionId(DeviceType deviceType, DeviceRole deviceRole)
{
    const sptr<IStandardAudioService> gasp = GetAudioAbilityManagerProxy();
    CHECK_AND_RETURN_RET_LOG(gasp != nullptr, 0, "Audio service unavailable.");
    return gasp->GetTransactionId(deviceType, deviceRole);
}

int32_t AudioAbilityManager::SetMicrophoneMuteProxy(bool isMute)
{
    const sptr<IStandardAudioService> gsp = GetAudioAbilityManagerProxy();
    CHECK_AND_RETURN_RET_LOG(gsp != nullptr, ERR_OPERATION_FAILED, "Service proxy unavailable");
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    int32_t ret = gsp->SetMicrophoneMute(isMute);
    IPCSkeleton::SetCallingIdentity(identity);
    return ret;
}

bool AudioAbilityManager::IsAllowedPlayback(const int32_t &uid, const int32_t &pid)
{
#ifdef AVSESSION_ENABLE
    // Temporary solution to avoid performance issues
    if (uid == BOOTUP_MUSIC_UID) {
        return true;
    }
    bool allowed = false;
    allowed = OHOS::AVSession::AVSessionManager::GetInstance().IsAudioPlaybackAllowed(uid, pid);
    if (!allowed) {
        usleep(WATI_PLAYBACK_TIME); //wait for 200ms
        AUDIO_INFO_LOG("IsAudioPlaybackAllowed Try again after 200ms");
        allowed = OHOS::AVSession::AVSessionManager::GetInstance().IsAudioPlaybackAllowed(uid, pid);
    }
    return allowed;
#endif
    return true;
}
} // namespace AudioStandard
} // namespace OHOS