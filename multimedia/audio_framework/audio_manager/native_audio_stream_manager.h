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

/**
 * @addtogroup OHAudio
 * @{
 *
 * @brief Provide the definition of the C interface for the audio module.
 *
 * @syscap SystemCapability.Multimedia.Audio.Core
 *
 * @since 20
 */

/**
 * @file native_audio_stream_manager.h
 *
 * @brief Declare audio stream manager related interfaces.
 *
 * This file interfaces are used for the creation of AudioStreamManager.
 *
 * @library libohaudio.so
 * @syscap SystemCapability.Multimedia.Audio.Core
 * @kit AudioKit
 * @since 20
 */

#ifndef NATIVE_AUDIO_STREAM_MANAGER_H
#define NATIVE_AUDIO_STREAM_MANAGER_H

#include "native_audio_common.h"
#include "native_audiostream_base.h"
#include "native_audio_device_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Declare the audio stream manager.
 *     Audio stream manager provides many functions about audio streams, like monitoring audio streams status,
 *     getting different stream types supported information and so on.
 *
 * @since 20
 */
typedef struct OH_AudioStreamManager OH_AudioStreamManager;

/**
 * @brief Fetch the audio stream manager handle, which is a singleton.
 *
 * @param streamManager output parameter to get the {@link #OH_AudioStreamManager}.
 * @return
 *     {@link #AUDIOCOMMON_RESULT_SUCCESS} if execution succeeds
 *     {@link #AUDIOCOMMON_RESULT_ERROR_SYSTEM} if system state error
 * @since 20
 */
OH_AudioCommon_Result OH_AudioManager_GetAudioStreamManager(
    OH_AudioStreamManager **streamManager);

/**
 * @brief Return if fast playback is supported for the specific audio stream info and usage type
 *     in current device situation.
 *
 * @param streamManager {@link OH_AudioStreamManager} handle
 *     provided by {@link OH_AudioManager_GetAudioStreamManager}.
 * @param streamInfo reference of stream info structure to describe basic audio format.
 * @param usage stream usage type used to decide the audio device and pipe type selection result.
 * @return {@code true} if fast playback is supported in this situation.
 * @since 20
 */
bool OH_AudioStreamManager_IsFastPlaybackSupported(
    OH_AudioStreamManager *streamManager, OH_AudioStreamInfo *streamInfo, OH_AudioStream_Usage usage);

/**
 * @brief Return if fast recording is supported for the specific audio stream info and source type
 *     in current device situation.
 *
 * @param streamManager {@link OH_AudioStreamManager} handle
 *     provided by {@link OH_AudioManager_GetAudioStreamManager}.
 * @param streamInfo reference of stream info structure to describe basic audio format.
 * @param source stream source type used to decide the audio device and pipe type selection result.
 * @return {@code true} if fast recording is supported in this situation.
 * @since 20
 */
bool OH_AudioStreamManager_IsFastRecordingSupported(
    OH_AudioStreamManager *streamManager, OH_AudioStreamInfo *streamInfo, OH_AudioStream_SourceType source);

#ifdef __cplusplus
}
#endif
 
#endif // NATIVE_AUDIO_ROUTING_MANAGER_H
/** @} */