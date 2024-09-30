/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#ifndef AUDIO_COMMON_INFO_H
#define AUDIO_COMMON_INFO_H

namespace OHOS {
namespace AudioStandard {
constexpr int32_t ROOT_UID = 0;
constexpr int32_t INVALID_UID = -1;

constexpr int32_t AUDIO_FLAG_INVALID = -1;
constexpr int32_t AUDIO_FLAG_NORMAL = 0;
constexpr int32_t AUDIO_FLAG_MMAP = 1;
constexpr int32_t AUDIO_FLAG_VOIP_FAST = 2;
constexpr int32_t AUDIO_FLAG_DIRECT = 3;
constexpr int32_t AUDIO_FLAG_VOIP_DIRECT = 4;
constexpr int32_t AUDIO_FLAG_FORCED_NORMAL = 10;

enum StateChangeCmdType {
    CMD_FROM_CLIENT = 0,
    CMD_FROM_SYSTEM = 1
};

struct AppInfo {
    int32_t appUid { INVALID_UID };
    uint32_t appTokenId { 0 };
    int32_t appPid { 0 };
    uint64_t appFullTokenId { 0 };
};

} // namespace AudioStandard
} // namespace OHOS
#endif //AUDIO_COMMON_INFO_H
