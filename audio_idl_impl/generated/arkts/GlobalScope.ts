/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
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


// WARNING! THIS FILE IS AUTO-GENERATED, DO NOT MAKE CHANGES, THEY WILL BE LOST ON NEXT GENERATION!

import { GlobalScope_ohos_multimedia_audio } from "./OHGlobalScopeOhosMultimediaAudioMaterialized"
export function getAudioManager(): AudioManager {
    return GlobalScope_ohos_multimedia_audio.getAudioManager()
}
export function createAudioCapturer(options: AudioCapturerOptions, callback_?: AsyncCallback_AudioCapturer_Void | undefined): void {
    GlobalScope_ohos_multimedia_audio.createAudioCapturer(options, callback_)
}
export function createAudioRenderer(options: AudioRendererOptions, callback_?: AsyncCallback_AudioRenderer_Void | undefined): void {
    GlobalScope_ohos_multimedia_audio.createAudioRenderer(options, callback_)
}
export function createTonePlayer(options: AudioRendererInfo, callback_?: AsyncCallback_TonePlayer_Void | undefined): void {
    GlobalScope_ohos_multimedia_audio.createTonePlayer(options, callback_)
}
export function createAsrProcessingController(audioCapturer: AudioCapturer): AsrProcessingController {
    return GlobalScope_ohos_multimedia_audio.createAsrProcessingController(audioCapturer)
}
