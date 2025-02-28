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

 #include <cstdint>

enum StreamClass : uint32_t {
    PA_STREAM = 0,
    FAST_STREAM,
    VOIP_STREAM,
};

enum AudioFlag : uint32_t {
    AUDIO_OUTPUT_FLAG_NONE = 0, // select
    AUDIO_OUTPUT_FLAG_NORAML, // route
    AUDIO_OUTPUT_FLAG_DIRECT, // route
    AUDIO_OUTPUT_FLAG_HD, // select
    AUDIO_OUTPUT_FLAG_MULTICHANNEL, // select, route
    AUDIO_OUTPUT_FLAG_LOWPOWER, // select, route
    AUDIO_OUTPUT_FLAG_FAST, // select, route
    AUDIO_OUTPUT_FLAG_VOIP, // select
    AUDIO_OUTPUT_FLAG_VOIP_FAST, // select, route
    AUDIO_OUTPUT_FLAG_HWDECODING, // select, route
    AUDIO_INPUT_FLAG_NONE = 100, // select
    AUDIO_INPUT_FLAG_NORAML, // route
    AUDIO_INPUT_FLAG_FAST, // select, route
    AUDIO_INPUT_FLAG_VOIP, // select
    AUDIO_INPUT_FLAG_VOIP_FAST, // select, route
    AUDIO_INPUT_FLAG_WAKEUP, // select, route
    AUDIO_FLAG_MAX,
};

enum AudioStreamStatus : uint32_t {
    STREAM_STATUS_NEW = 0,
    STREAM_STATUS_STARTTING,
    STREAM_STATUS_PAUSED,
    STREAM_STATUS_STOPPED,
    STREAM_STATUS_RELEASED,
};