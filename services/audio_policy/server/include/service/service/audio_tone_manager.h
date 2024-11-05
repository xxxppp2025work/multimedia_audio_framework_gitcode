#ifndef ST_AUDIO_POLICY_TONE_MANAGER_H
#define ST_AUDIO_POLICY_TONE_MANAGER_H

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
#include "audio_volume_config.h"
#include "audio_ec_info.h"
#include "datashare_helper.h"
#include "audio_utils.h"
#include "audio_errors.h"

namespace OHOS {
namespace AudioStandard {

class AudioPolicyToneManager {
public:
    static AudioPolicyToneManager& GetInstance()
    {
        static AudioPolicyToneManager instance;
        return instance;
    }
    bool LoadToneDtmfConfig();
    std::vector<int32_t> GetSupportedTones();
    std::shared_ptr<ToneInfo> GetToneConfig(int32_t ltonetype);
private:
    AudioPolicyToneManager() {}
    ~AudioPolicyToneManager() {}
private:
    std::unordered_map<int32_t, std::shared_ptr<ToneInfo>> toneDescriptorMap_;
};

}
}

#endif