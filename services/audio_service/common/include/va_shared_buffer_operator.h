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
#ifndef VA_SHARED_BUFFER_OPERATOR_H
#define VA_SHARED_BUFFER_OPERATOR_H

#include <stdio.h>
#include <unistd.h>

#include <chrono>
#include <memory>
#include <map>
#include <string>

#include "va_shared_buffer.h"
#include "audio_log.h"

#include <thread>

namespace OHOS {
namespace AudioStandard {

class VASharedBufferOperator {
public:
    VASharedBufferOperator(const VASharedBuffer &buffer);
    ~VASharedBufferOperator();

    static constexpr uint32_t LOCK_RELEASED=0;
    static constexpr uint32_t LOCK_OWNED = 1;      //  writing or reading

    void SetMinReadSize(const size_t MinReadSize);

    void Reset();

    void SetReadPosToWritePos();

    size_t GetReadableSize();

    size_t GetReadableSizeNoLock();

    size_t GetWritableSizeNoLock();

    size_t Read(uint8_t *data, size_t dataSize);

    size_t Write(uint8_t *data, size_t dataSize);

    void GetVASharedMemInfo(VASharedMemInfo &memInfo);
private:
    VASharedBuffer buffer_;

    size_t capacity;
    
    sptr<Ashmem> dataAshmem_;

    VASharedStatusInfo *statusInfo_ = nullptr;

    std::atomic<uint32_t> *GetFutex();

    size_t minReadSize_ = 1;

    void InitVASharedStatusInfo();

    int32_t SizeCheck(size_t dataSize);

    bool HasEnoughReadableData();
};

}   //namespace AudioStandard
}   //namespace OHOS
#endif //VA_SHARED_BUFFER_OPERATOR_H