/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License")
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "multimedia_audio_ffi.h"
#include "multimedia_audio_error.h"
#include "multimedia_audio_impl.h"

#include "audio_log.h"

using namespace OHOS::FFI;
// using namespace OHOS::AudioStandard::MMAAudioCapturerImpl;

namespace OHOS {
namespace AudioStandard {
extern "C" {
// Audio Volumne Group Manager
int32_t FfiMMAGetMaxVolume(int64_t id, int32_t volumeType, int32_t *errorCode)
{
    auto inst = FFIData::GetData<MMAAudioCapturerImpl>(id);
    if (!inst) {
        *errorCode = CJ_ERR_SYSTEM;
        AUDIO_ERR_LOG("Get MMAAudioCapturerImpl error");
        return CJ_ERR_INVALID_RETURN_VALUE;
    }
    *errorCode = SUCCESS_CODE;
    return inst->GetMaxVolume(volumeType);

}
