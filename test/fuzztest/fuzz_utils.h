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
#ifndef FUZZ_UTILS_H
#define FUZZ_UTILS_H

#include <cstdint>
#include <securec.h>
#include "audio_log.h"

namespace OHOS {
namespace AudioStandard {

template<class T>
T GetData(const size_t dataSize, size_t &pos, const uint8_t *rawData)
{
    T object {};
    size_t objectSize = sizeof(object);
    if (dataSize < pos) {
        return object;
    }
    if (rawData == nullptr || objectSize > dataSize - pos) {
        return object;
    }
    errno_t ret = memcpy_s(&object, objectSize, rawData + pos, objectSize);
    if (ret != EOK) {
        return {};
    }
    pos += objectSize;
    return object;
}

template<class T>
uint32_t GetArrLength(T &arr)
{
    if (arr == nullptr) {
        AUDIO_INFO_LOG("%{public}s: The array length is equal to 0", __func__);
        return 0;
    }
    return sizeof(arr) / sizeof(arr[0]);
}

} // namespace AudioStandard
} // namespace OHOS

#endif // FUZZ_UTILS_H