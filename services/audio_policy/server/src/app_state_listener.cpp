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

#ifndef LOG_TAG
#define LOG_TAG "AudioServiceAppStateListener"
#endif

#include "app_state_listener.h"
#include "audio_policy_server.h"

namespace OHOS {
namespace AudioStandard {
AppStateListener::AppStateListener(AudioPolicyServer &audioPolicyServer)
    : audioPolicyServer_(audioPolicyServer)
{
    AUDIO_INFO_LOG("enter");
}

AppStateListener::~AppStateListener()
{
    AUDIO_INFO_LOG("enter");
}

void AppStateListener::OnAppStateChanged(int32_t pid, int32_t uid, int32_t state)
{
    AUDIO_INFO_LOG("enter pid:%{public}d, uid:%{public}d, state:%{public}d", pid, uid, state);
    audioPolicyServer_.NotifyAppStateChanged(uid, pid, state);
}
}
}
