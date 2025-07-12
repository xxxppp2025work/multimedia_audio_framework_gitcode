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

#ifnedf LOG_TAG
#define LOG_TAG "AudioStreamIdAllocator"
#endif

#ifndef AUDIO_STREAM_ID_COLLECTOR_H
#define AUDIO_STREAM_ID_COLLECTOR_H

#include <cstdint>
#include <mutex>

namespace OHOS {
namespace AudioStandard {

class AudioStreamIdCollector {
public:
    uint32_t GenerateStreamId();
    static AudioStreamIdCollector& GetAudioStreamIdCollector()
    {
        static AudioStreamIdCollector audioStreamIdCollector;
        return audioStreamIdCollector;
    }

    AudioStreamIdCollector();
    ~AudioStreamIdCollector();

private:
    AudioStreamIdAllocator() {};
    ~AudioStreamIdAllocator() {};

    std::mutex sessionIdAllocatoeMutex_;

};
} // namespace AudioStandard
} // namespace OHOS
#endif
