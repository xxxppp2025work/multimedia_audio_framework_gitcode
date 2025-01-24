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

#ifndef ID_HANDLER_H
#define ID_HANDLER_H

#include <iostream>
#include <mutex>
#include <atomic>
#include <unordered_map>
#include <unordered_set>
#include "audio_info.h"
#include "common/hdi_adapter_info.h"

namespace OHOS {
namespace AudioStandard {
sturct IdInfo {
    std::string info_;
    std::atomic<uint32_t> refCount_;
};

class IdHandler {
public:
    static IdHandler &GetInstance(void);
    // if isResident is true, will record ref count, need release after use
    uint32_t GetId(uint32_t base, uint32_t type, const std::string &info = HDI_ADAPTER_ID_INFO_DEFAULT,
        bool isResident = false);
    uint32_t GetRenderIdByDeviceClass(const std::string &deviceClass,
        const std::string &info = HDI_ADAPTER_ID_INFO_DEFAULT, bool isResident = false);
    uint32_t GetCaptureIdByDeviceClass(const std::string &deviceClass,
        const std::string &info = HDI_ADAPTER_ID_INFO_DEFAULT, bool isResident = false);
    void ReleaseId(uint32_t &id);
    bool CheckId(uint32_t id, uint32_t requireBase);
    uint32_t ParseBase(uint32_t id);
    uint32_t ParseType(uint32_t id);
    std::string ParseInfo(uint32_t id);
    uint32_t GetRefCount(uint32_t id);

private:
    IdHandler() = default;
    ~IdHandler() = default;
    IdHandler(const IdHandler &) = delete;
    IdHandler &operator=(const IdHandler &) = delete;
    IdHandler(IdHandler &&) = delete;
    IdHandler &operator=(IdHandler &&) = delete;

    uint32_t FindId(uint32_t base, uint32_t type, const std::string &info);
    uint32_t GenerateInfoId(void);
};

} // namespace AudioStandard
} // namespace OHOS

#endif // ID_HANDLER_H
