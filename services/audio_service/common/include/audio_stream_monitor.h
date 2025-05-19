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

#ifndef AUDIO_STREAM_MONITOR
#define AUDIO_STREAM_MONITOR
#include <vector>
#include <utility>
#include <mutex>
#include <map>
#include "audio_info.h"
#include "audio_stream_checker.h"

namespace OHOS {
namespace AudioStandard {
class AudioStreamMonitor {
public:
    static AudioStreamMonitor& GetInstance();
    int32_t RegisterAudioRendererDataTransferStateListener(const DataTransferMonitorParam &param,
        const int32_t pid, const int32_t callbackId);
    int32_t UnRegisterAudioRendererDataTransferStateListener(const int32_t pid, const int32_t callbackId);
    void OnCallback(int32_t uid, std::shared_ptr<AudioRendererDataTransferStateChangeInfo> info);
    void ReportStreamFreezen(int64_t intervalTime);
    void AddCheckForMonitor(uint32_t sessionId, std::shared_ptr<AudioStreamChecker> &checker);
    void DeleteCheckForMonitor(uint32_t sessionId);
private:
    AudioStreamMonitor() {}
    ~AudioStreamMonitor() {}
    bool HasRegistered(const int32_t pid, const int32_t callbackId);
    std::map<std::pair<int32_t, int32_t>, DataTransferMonitorParam> registerInfo_;
    std::map<uint32_t, std::shared_ptr<AudioStreamChecker>> audioStreamCheckers_;
    std::recursive_mutex regStatusMutex_;
};
}
}