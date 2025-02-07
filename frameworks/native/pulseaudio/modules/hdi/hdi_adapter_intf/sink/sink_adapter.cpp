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
#ifndef LOG_TAG
#define LOG_TAG "SinkAdapter"
#endif

#include "sink/sink_adapter.h"
#include <functional>
#include "audio_errors.h"
#include "audio_hdi_log.h"
#include "sink/i_audio_render_sink.h"
#include "util/id_handler.h"
#include "manager/hdi_adapter_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

using namespace OHOS::AudioStandard;
typedef void OnRenderCallback(const RenderCallbackType type, int8_t *userdata);

static inline std::shared_ptr<IAudioRenderSink> GetRenderSink(uint32_t renderId)
{
    HdiAdapterManager &manager = HdiAdapterManager::GetInstance();
    return manager.GetRenderSink(renderId, true);
}

int32_t InitSinkAdapter(struct SinkAdapter *adapter, const char *deviceClass, const char *info)
{
    CHECK_AND_RETURN_RET_LOG(adapter != nullptr, ERR_INVALID_HANDLE, "adapter is nullptr");

    adapter->renderId = HDI_INVALID_ID;
    adapter->deviceClass = nullptr;
    IdHandler &idHandler = IdHandler::GetInstance();
    if (info == nullptr) {
        adapter->renderId = idHandler.GetRenderIdByDeviceClass(deviceClass, HDI_ADAPTER_ID_INFO_DEFAULT, true);
    } else {
        adapter->renderId = idHandler.GetRenderIdByDeviceClass(deviceClass, std::string(info), true);
    }
    std::shared_ptr<IAudioRenderSink> sink = GetRenderSink(adapter->renderId);
    if (sink == nullptr) {
        AUDIO_ERR_LOG("get sink fail, deviceClass: %{public}s, info: %{public}s, renderId: %{public}u", deviceClass,
            info, adapter->renderId);
        idHandler.ReleaseId(adapter->renderId);
        return ERR_OPERATION_FAILED;
    }
    adapter->deviceClass = strdup(deviceClass);
    return SUCCESS;
}

void DeInitSinkAdapter(struct SinkAdapter *adapter)
{
    CHECK_AND_RETURN_LOG(adapter != nullptr, "adapter is nullptr");
    IdHandler::GetInstance().ReleaseId(adapter->renderId);
    if (adapter->deviceClass != nullptr) {
        free(const_cast<char *>(adapter->deviceClass));
        adapter->deviceClass = nullptr;
    }
}

int32_t SinkAdapterInit(struct SinkAdapter *adapter, const struct SinkAdapterAttr *attr)
{
    CHECK_AND_RETURN_RET_LOG(adapter != nullptr && adapter->renderId != HDI_INVALID_ID, ERR_INVALID_HANDLE, "invalid adapter");
    CHECK_AND_RETURN_RET_LOG(attr != nullptr, ERR_INVALID_PARAM, "attr is nullptr");
    std::shared_ptr<IAudioRenderSink> sink = GetRenderSink(adapter->renderId);
    CHECK_AND_RETURN_RET_LOG(sink != nullptr, ERR_INVALID_HANDLE, "get sink fail");
    if (sink->IsInited()) {
        return SUCCESS;
    }

    IAudioSinkAttr sinkAttr = {
        .adapterName = attr->adapterName,
        .openMicSpeaker = attr->openMicSpeaker,
        .format = static_cast<AudioSampleFormat>(attr->format),
        .sampleRate = attr->sampleRate,
        .channel = attr->channel,
        .volume = attr->volume,
        .deviceNetworkId = attr->deviceNetworkId,
        .deviceType = attr->deviceType,
        .channelLayout = attr->channelLayout,
        .aux = attr->aux,
    };

    return sink->Init(sinkAttr);
}

void SinkAdapterDeInit(struct SinkAdapter *adapter)
{
    CHECK_AND_RETURN_LOG(adapter != nullptr && adapter->renderId != HDI_INVALID_ID, "invalid adapter");
    std::shared_ptr<IAudioRenderSink> sink = GetRenderSink(adapter->renderId);
    CHECK_AND_RETURN_LOG(sink != nullptr, "get sink fail");
    if (!sink->IsInited()) {
        return;
    }

    sink->DeInit();
}

int32_t SinkAdapterStart(struct SinkAdapter *adapter)
{   
    CHECK_AND_RETURN_RET_LOG(adapter != nullptr && adapter->renderId != HDI_INVALID_ID, ERR_INVALID_HANDLE,
        "invalid adapter");
    std::shared_ptr<IAudioRenderSink> sink = GetRenderSink(adapter->renderId);
    CHECK_AND_RETURN_RET_LOG(sink != nullptr, ERR_INVALID_HANDLE, "get sink fail");
    CHECK_AND_RETURN_RET_LOG(sink->IsInited(), ERR_ILLEGAL_STATE, "sink not init");

    return sink->Start();
}