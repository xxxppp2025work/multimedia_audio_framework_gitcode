/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#ifndef AUDIO_SESSION_DEVICE_INFO_H
#define AUDIO_SESSION_DEVICE_INFO_H

#include "audio_device_descriptor.h"
#include "audio_device_info.h"

namespace OHOS {
namespace AudioStandard {

/**
 * Enumerates the recommend action when device changed.
 * @since 20
 */
enum class DeviceChangeRecommendedAction {
    /**
     * No special recommendations, the playback can be continue or not.
     */
    RECOMMEND_TO_CONTINUE = 0,
    /**
     * Recommend to stop the playback.
     */
    RECOMMEND_TO_STOP = 1,
};

/**
 * Audio session device change info.
 * @since 20
 */
struct CurrentOutputDeviceChangedEvent {
    /**
     * Audio device descriptors after changed.
     * @since 20
     */
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> devices;
    /**
     * Audio device changed reason.
     * @since 20
     */
    AudioStreamDeviceChangeReason changeReason;
    /**
     * Recommend action when device changed.
     * @since 20
     */
    DeviceChangeRecommendedAction recommendedAction;
};
} // namespace AudioStandard
} // namespace OHOS
#endif // AUDIO_SESSION_DEVICE_INFO_H
