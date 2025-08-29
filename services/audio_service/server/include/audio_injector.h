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
#ifndef AUDIO_INJECTOR_H
#define AUDIO_INJECTOR_H

#include <set>
#include "audio_module_info.h"

namespace OHOS {
namespace AudioStandard {
class AudioInjector {
public:
    static AudioInjector& GetInstance()
    {
        static AudioInjector instance;
        return instance;
    }
    int32_t Init();
    int32_t DeInit();
    int32_t UpdateAudioInfo(AudioModuleInfo &info);
    int32_t MoveStream(uint32_t renderId, bool flag);
    int32_t PeekAudioData(const uint32_t rendererPortIdx, uint8_t *destBuff, const size_t buffSize, AudioStreamInfo &streamInfo);
    int32_t GetRendererStreamCount();
    void SetCapturePortIdx(uint32_t idx);
    uint32_t GetCapturePortIdx();
    void SetRendererPortIdx(uint32_t idx);
    uint32_t GetRendererPortIdx();

private:
    AudioInjector();
    AudioInjector(const AudioInjector&) = delete;
    AudioInjector& operator=(const AudioInjector&) = delete;
private:
    AudioModuleInfo moduleInfo_;
    uint32_t capturePortIdx_;
    uint32_t renderPortIdx_;
    bool isConnected_;
    std::unordered_map<uint32_t, std::string> rendererStreamMap_ = {};
};
} //  namespace AudioStandard
} //  namespace OHOS
#endif  // AUDIO_INJECTOR_H