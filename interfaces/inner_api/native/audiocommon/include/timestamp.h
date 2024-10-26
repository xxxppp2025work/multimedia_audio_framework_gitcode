/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
#ifndef TIMESTAMP_H
#define TIMESTAMP_H
#ifdef __MUSL__
#include <sys/time.h>

#include <stdint.h>
#endif // __MUSL__
#include <unistd.h>
#include <chrono>

#include "audio_log.h"

#define MAX_TIMEOUT_MILLISECONDS 800
namespace OHOS {
namespace AudioStandard {
/**
 * @brief Represents Timestamp information, including the frame position information and high-resolution time source.
 */
class Timestamp {
public:
    Timestamp() : framePosition(0)
    {
        time.tv_sec = 0;
        time.tv_nsec = 0;
    }
    virtual ~Timestamp() = default;
    uint32_t framePosition;
    struct timespec time;

    /**
     * @brief Enumerates the time base of this <b>Timestamp</b>. Different timing methods are supported.
     *
     */
    enum Timestampbase {
        /** Monotonically increasing time, excluding the system sleep time */
        MONOTONIC = 0
    };
};
class OutputTimeout {
public:
    OutputTimeout()
    {
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
        if (static_cast<unsigned long long>(duration) > maxTimeoutMills_) {
            AUDIO_INFO_LOG("The current function execution time is:%llu milliseconds",
                static_cast<unsigned long long>(duration));
        }
    }
private:
    std::chrono::high_resolution_clock::time_point startTime_ = {};
    unsigned long long maxTimeoutMills_ = 0;
};
} // namespace AudioStandard
} // namespace OHOS
#endif // TIMESTAMP_H
