
#ifndef LOG_TAG
#define LOG_TAG "AudioA2dpOffloadFlag"
#endif

#include "audio_a2dp_offload_flag.h"
#include <ability_manager_client.h>
#include "iservice_registry.h"
#include "parameter.h"
#include "parameters.h"
#include "audio_utils.h"
#include "audio_log.h"
#include "audio_utils.h"
#include "audio_manager_listener_stub.h"
#include "audio_inner_call.h"
#include "media_monitor_manager.h"

namespace OHOS {
namespace AudioStandard {

int32_t AudioA2dpOffloadFlag::OffloadStopPlaying(const std::vector<int32_t> &sessionIds)
{
#ifdef BLUETOOTH_ENABLE
    AUDIO_PRERELEASE_LOGI("OffloadStopPlaying, a2dpOffloadFlag_: %{public}d, sessionIds: %{public}zu",
        a2dpOffloadFlag_, sessionIds.size());
    if (a2dpOffloadFlag_ != A2DP_OFFLOAD || sessionIds.size() == 0) {
        return SUCCESS;
    }
    return Bluetooth::AudioA2dpManager::OffloadStopPlaying(sessionIds);
#else
    return SUCCESS;
#endif
}

void AudioA2dpOffloadFlag::SetA2dpOffloadFlag(BluetoothOffloadState state)
{
    a2dpOffloadFlag_ = state;
}

BluetoothOffloadState AudioA2dpOffloadFlag::GetA2dpOffloadFlag()
{
    return a2dpOffloadFlag_;
}

int32_t AudioA2dpOffloadFlag::OffloadStartPlaying(const std::vector<int32_t> &sessionIds)
{
#ifdef BLUETOOTH_ENABLE
    AUDIO_INFO_LOG("OffloadStartPlaying, a2dpOffloadFlag_: %{public}d, sessionIds: %{public}zu",
        a2dpOffloadFlag_, sessionIds.size());
    if (a2dpOffloadFlag_ != A2DP_OFFLOAD || sessionIds.size() == 0) {
        return SUCCESS;
    }
    return Bluetooth::AudioA2dpManager::OffloadStartPlaying(sessionIds);
#else
    return SUCCESS;
#endif
}

A2dpOffloadConnectionState AudioA2dpOffloadFlag::GetCurrentOffloadConnectedState()
{
    return currentOffloadConnectionState_;
}

void AudioA2dpOffloadFlag::SetCurrentOffloadConnectedState(A2dpOffloadConnectionState currentOffloadConnectionState)
{
    currentOffloadConnectionState_ = currentOffloadConnectionState;
}

bool AudioA2dpOffloadFlag::IsA2dpOffloadConnected()
{
    return currentOffloadConnectionState_ == CONNECTION_STATUS_CONNECTED;
}

}
}