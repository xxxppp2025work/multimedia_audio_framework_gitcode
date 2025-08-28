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
#include "audio_injector.h"
#include "audio_policy_manager_factory.h"

namespace OHOS {
namespace AudioStandard {
AudioInjector::AudioInjector()
    : audioIOHandleMap_(AudioIOHandleMap::GetInstance()),
      audioPolicyManager_(AudioPolicyManagerFactory::GetAudioPolicyManager())
{
}

int32_t AudioInjector::Init()
{
    return 0;
}

int32_t AudioInjector::DeInit()
{
    return 0;
}

int32_t AudioInjector::UpdateAudioInfo(AudioModuleInfo &Info)
{
    return 0;
}

int32_t AudioInjector::MoveStream(uint32_t renderId, bool flag)
{
    return 0;
}
int32_t AudioInjector::PeekAudioData(uint32_t streamid, uint8_t *destPtr, size_t dataSize)
{
    return 0;
}

int32_t AudioInjector::GetRenderCount()
{
    return renderIdMap_.size();
}

void AudioInjector::SetCaptureIdx(uint32_t idx)
{
    capturePortIdx_ = idx;
}

uint32_t AudioInjector::GetCaptureIdx()
{
    return capturePortIdx_;
}

void AudioInjector::SetRenderIdx(uint32_t idx)
{
    renderPortIdx_ = idx;
}

uint32_t AudioInjector::GetRenderIdx()
{
    return renderPortIdx_;
}
}  //  namespace AudioStandard
}  //  namespace OHOS