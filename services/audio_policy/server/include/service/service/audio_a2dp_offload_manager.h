
#ifndef ST_AUDIO_A2DP_OFFLOAD_MANAGER_H
#define ST_AUDIO_A2DP_OFFLOAD_MANAGER_H

#include <bitset>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include "singleton.h"
#include "audio_group_handle.h"
#include "audio_info.h"
#include "audio_manager_base.h"
#include "audio_module_info.h"
#include "audio_utils.h"
#include "audio_errors.h"

#include "audio_a2dp_offload_flag.h"

#ifdef BLUETOOTH_ENABLE
#include "audio_server_death_recipient.h"
#include "audio_bluetooth_manager.h"
#include "bluetooth_device_manager.h"
#endif

namespace OHOS {
namespace AudioStandard {

class AudioA2dpOffloadManager final : public Bluetooth::AudioA2dpPlayingStateChangedListener,
    public std::enable_shared_from_this<AudioA2dpOffloadManager> {
public:
    AudioA2dpOffloadManager()
        : audioA2dpOffloadFlag_(AudioA2dpOffloadFlag::GetInstance())
    {}
    void Init() {Bluetooth::AudioA2dpManager::RegisterA2dpPlayingStateChangedListener(shared_from_this());};

    void ConnectA2dpOffload(const std::string &deviceAddress, const std::vector<int32_t> &sessionIds);
    void OnA2dpPlayingStateChanged(const std::string &deviceAddress, int32_t playingState) override;

    void WaitForConnectionCompleted();
    bool IsA2dpOffloadConnecting(int32_t sessionId);
    bool IsA2dpOffloadConnected();

    void SetA2dpOffloadFlag(BluetoothOffloadState state);
    BluetoothOffloadState GetA2dpOffloadFlag();
    int32_t OffloadStartPlaying(const std::vector<int32_t> &sessionIds);
    int32_t OffloadStopPlaying(const std::vector<int32_t> &sessionIds);
private:
    std::vector<int32_t> connectionTriggerSessionIds_;
    std::string a2dpOffloadDeviceAddress_ = "";
    std::mutex connectionMutex_;
    std::condition_variable connectionCV_;
    static const int32_t CONNECTION_TIMEOUT_IN_MS = 300; // 300ms
    AudioA2dpOffloadFlag& audioA2dpOffloadFlag_;
};


}
}
#endif