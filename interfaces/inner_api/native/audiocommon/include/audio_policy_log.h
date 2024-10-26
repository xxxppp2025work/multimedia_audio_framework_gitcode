/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifndef OHOS_AUDIO_POLICY_LOG_H
#define OHOS_AUDIO_POLICY_LOG_H

#include <chrono>
#include "audio_log.h"

#undef LOG_DOMAIN
#define LOG_DOMAIN 0xD002B87
#define MAX_TIMEOUT_MILLISECONDS 800
class OutputTimeout {
public:
    OutputTimeout() {
            maxTimeoutMills_ = MAX_TIMEOUT_MILLISECONDS;
            startTime_ = std::chrono::high_resolution_clock::now();
    }
    OutputTimeout(unsigned long long maxTimeoutMills)
    {
        maxTimeoutMills_ = maxTimeoutMills;
        startTime_ = std::chrono::high_resolution_clock::now();
    }
    ~OutputTimeout()
    {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_).count();
        if (duration > maxTimeoutMills_) {
            AUDIO_INFO_LOG("The current function execution time is:%llu milliseconds",
                static_cast<unsigned long long>(duration));
        }
    }
private:
    std::chrono::high_resolution_clock::time_point  startTime_ = 0;
    unsigned long long maxTimeoutMills_ = 0;
};
#endif // OHOS_AUDIO_POLICY_LOG_H
