
#ifndef LOG_TAG
#define LOG_TAG "AudioPolicyToneManager"
#endif

#include "audio_policy_tone_manager.h"
#include <ability_manager_client.h>
#include "iservice_registry.h"
#include "parameter.h"
#include "parameters.h"

#include "audio_utils.h"
#include "audio_log.h"
#include "audio_utils.h"
#include "audio_inner_call.h"
#include "audio_tone_parser.h"
#include "media_monitor_manager.h"

#include "audio_policy_common.h"

namespace OHOS {
namespace AudioStandard {

bool AudioPolicyToneManager::LoadToneDtmfConfig()
{
#ifdef FEATURE_DTMF_TONE
    AUDIO_INFO_LOG("Enter");
    std::unique_ptr<AudioToneParser> audioToneParser = std::make_unique<AudioToneParser>();
    if (audioToneParser == nullptr) {
        AudioPolicyCommon::GetInstance().WriteServiceStartupError("Audio Tone Load Configuration failed");
    }
    CHECK_AND_RETURN_RET_LOG(audioToneParser != nullptr, false, "Failed to create AudioToneParser");
    std::string AUDIO_TONE_CONFIG_FILE = "system/etc/audio/audio_tone_dtmf_config.xml";

    if (audioToneParser->LoadConfig(toneDescriptorMap_)) {
        std::shared_ptr<Media::MediaMonitor::EventBean> bean = std::make_shared<Media::MediaMonitor::EventBean>(
            Media::MediaMonitor::ModuleId::AUDIO, Media::MediaMonitor::EventId::LOAD_CONFIG_ERROR,
            Media::MediaMonitor::EventType::FAULT_EVENT);
        bean->Add("CATEGORY", Media::MediaMonitor::AUDIO_TONE_DTMF_CONFIG);
        Media::MediaMonitor::MediaMonitorManager::GetInstance().WriteLogMsg(bean);
        AudioPolicyCommon::GetInstance().WriteServiceStartupError("Audio Tone Load Configuration failed");
        AUDIO_ERR_LOG("Audio Tone Load Configuration failed");
        return false;
    }
    AUDIO_INFO_LOG("Done");
    return true;
#endif
}

std::vector<int32_t> AudioPolicyToneManager::GetSupportedTones()
{
    std::vector<int> supportedToneList = {};
#ifdef FEATURE_DTMF_TONE
    for (auto i = toneDescriptorMap_.begin(); i != toneDescriptorMap_.end(); i++) {
        supportedToneList.push_back(i->first);
    }
#endif
    return supportedToneList;
}

std::shared_ptr<ToneInfo> AudioPolicyToneManager::GetToneConfig(int32_t ltonetype)
{
#ifdef FEATURE_DTMF_TONE
    if (toneDescriptorMap_.find(ltonetype) != toneDescriptorMap_.end()) {
        AUDIO_DEBUG_LOG("AudioPolicyService GetToneConfig %{public}d", ltonetype);
        return toneDescriptorMap_[ltonetype];
    }
    AUDIO_DEBUG_LOG("AudioPolicyService GetToneConfig %{public}d", ltonetype);
#endif
    return nullptr;
}


}
}