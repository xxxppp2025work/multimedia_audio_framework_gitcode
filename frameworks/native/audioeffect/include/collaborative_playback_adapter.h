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
#ifndef COLLABORATIVE_PLAYBACK_ADAPTER_H
#define COLLABORATIVE_PLAYBACK_ADAPTER_H

#include <stdint.h>
#include "audio_effect_chain_adapter.h"
#ifdef __cplusplus
extern "C" {
#endif

bool IsStreamSupportCollaborative(int32_t usage);
bool IsCollaborationEnabled();
void CollaborativePlaybackEnqueue(BufferAttr *bufferAttr);
void CollaborativePlaybackDequeue(BufferAttr *bufferAttr);
bool IsCollaborativeChanged(int32_t sessionID, int32_t collaborationEnabled);
void CollaborativePlaybackReset();
#ifdef __cplusplus
}
#endif
#endif // COLLABORATIVE_PLAYBACK_ADAPTER_H