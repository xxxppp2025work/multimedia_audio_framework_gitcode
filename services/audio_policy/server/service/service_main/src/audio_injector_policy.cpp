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
#include "audio_injector_policy.h"
#include "audio_policy_manager_factory.h"

namespace OHOS {
namespace AudioStandard {
AudioInjectorPolicy::AudioInjectorPolicy()
    :audioIOHandleMap_(AudioIOHandleMap::GetInstance()),
     audioPolicyManager_(AudioPolicyManagerFactory::GetAudioPolicyManager())
{
    pipeManager_ = AudioPipeManager::GetPipeManager();
    isConnected_ = false;
}

int32_t AudioInjectorPolicy::Init()
{
    std::lock_guard<std::shared_mutex> lock(injectLock_);
    if (rendererStreamMap_.size() == 0) {
        AUDIO_INFO_LOG("Start");
        AudioModuleInfo moduleInfo = {};
        moduleInfo.lib = "libmodule-hdi-sink.z.so";
        std::string name = "";
        moduleInfo.name = name;

        moduleInfo.format = "s16le";
        moduleInfo.channels = "2"; // 2 channel
        moduleInfo.rate = "48000";
        moduleInfo.bufferSize = "3840"; // 20ms

        int32_t ret = audioIOHandleMap_.OpenPortAndInsertIOHandle(moduleInfo.name, moduleInfo);
        CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ERROR, "open port failed");
        this->moduleInfo_ = moduleInfo;
        CHECK_AND_RETURN_RET_LOG(pipeManager_ != nullptr, ERROR, "pipeManager_ is null");
        renderPortIdx_ = pipeManager_->GetPaIndexByName(moduleInfo.name);
        CHECK_AND_RETURN_RET_LOG(renderPortIdx_ != HDI_INVALID_ID, ERROR, "renderPortIdx error!");
    }
    return SUCCESS;
}

int32_t AudioInjectorPolicy::DeInit()
{
    std::lock_guard<std::shared_mutex> lock(injectLock_);
    if (rendererStreamMap_.size() == 0) {
        int32_t ret = audioIOHandleMap_.ClosePortAndEraseIOHandle(moduleInfo_.name);
        CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ERROR, "close port failed");
        renderPortIdx_ = HDI_INVALID_ID;
    }
    return SUCCESS;
}

int32_t AudioInjectorPolicy::UpdateAudioInfo(AudioModuleInfo &info)
{
    return SUCCESS;
}

int32_t AudioInjectorPolicy::MoveStream(uint32_t renderId, bool flag)
{
    std::lock_guard<std::shared_mutex> lock(injectLock_);
    CHECK_AND_RETURN_RET_LOG(pipeManager_ != nullptr, ERROR, "pipeManager_ is null");
    int32_t ret = ERROR;
    if (flag) {
        std::shared_ptr<AudioPipeInfo> info = pipeManager_->GetPipeBySessionId(renderId);
        CHECK_AND_RETURN_RET_LOG(info != nullptr, ERROR, "get pipeinfo failed");
        ret = audioPolicyManager_.MoveSinkInputByIndexOrName(renderId, 1, moduleInfo_.name);
        CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ERROR, "move stream in failed");
        rendererStreamMap_[renderId] = info;
    } else {
        std::string portName = "";
        std::shared_ptr<AudioPipeInfo> oldInfo = rendererStreamMap_[renderId];
        if (pipeManager_->IsPipeAlive(oldInfo)) {
            CHECK_AND_RETURN_RET_LOG(oldInfo != nullptr, ERROR, "oldInfo is null");
            portName = oldInfo->name_;
        } else {
            AudioModuleInfo moduleInfo = oldInfo->moduleInfo_;
            portName = moduleInfo.name;
            ret = audioIOHandleMap_.OpenPortAndInsertIOHandle(portName, moduleInfo);
            CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ERROR, "open original port failed");
        }
        ret = audioPolicyManager_.MoveSinkInputByIndexOrName(renderId, 1, portName);
        CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ERROR, "move stream out failed");
        rendererStreamMap_[renderId] = nullptr;
        rendererStreamMap_.erase(renderId);
    }
    return SUCCESS;
}

// get the number of rendererStream moved in Injector
int32_t AudioInjectorPolicy::GetRendererStreamCount()
{
    return rendererStreamMap_.size();
}

void AudioInjectorPolicy::SetCapturePortIdx(uint32_t idx)
{
    std::lock_guard<std::shared_mutex> lock(injectLock_);
    capturePortIdx_ = idx;
}

uint32_t AudioInjectorPolicy::GetCapturePortIdx()
{
    return capturePortIdx_;
}

void AudioInjectorPolicy::SetRendererPortIdx(uint32_t idx)
{
    std::lock_guard<std::shared_mutex> lock(injectLock_);
    renderPortIdx_ = idx;
}

uint32_t AudioInjectorPolicy::GetRendererPortIdx()
{
    return renderPortIdx_;
}

AudioModuleInfo& AudioInjectorPolicy::GetAudioModuleInfo()
{
    return moduleInfo_;
}

int32_t AudioInjectorPolicy::AddCaptureInjector()
{
    std::lock_guard<std::shared_mutex> lock(injectLock_);
    int32_t ret = ERROR;
    if (!isConnected_) {
        CHECK_AND_RETURN_RET_LOG(pipeManager_ != nullptr, ERROR, "pipeManager_ is null");
        if (pipeManager_->IsVoIPCall() == NORMAL_VOIP) {
            ret = audioPolicyManager_.AddCaptureInjector(renderPortIdx_, capturePortIdx_, SOURCE_TYPE_VOICE_COMMUNICATION);
        } else if (pipeManager_->IsVoIPCall() == FAST_VOIP) {
            ret = audioPolicyManager_.AddCaptureInjector();
        }
        CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ERROR, "AddCaptureInjector failed");
    }
    isConnected_ = true;
    return SUCCESS;
}
    
int32_t AudioInjectorPolicy::RemoveCaptureInjector()
{
    std::lock_guard<std::shared_mutex> lock(injectLock_);
    int32_t ret = ERROR;
    if (isConnected_) {
        CHECK_AND_RETURN_RET_LOG(pipeManager_ != nullptr, ERROR, "pipeManager_ is null");
        if (pipeManager_->IsVoIPCall() == NORMAL_VOIP) {
            ret = audioPolicyManager_.RemoveCaptureInjector(renderPortIdx_, capturePortIdx_, SOURCE_TYPE_VOICE_COMMUNICATION);
        } else if (pipeManager_->IsVoIPCall() == FAST_VOIP) {
            ret = audioPolicyManager_.RemoveCaptureInjector();
        }
        CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ERROR, "RemoveCaptureInjector failed");
    }
    isConnected_ = false;
    return SUCCESS;
}
}  //  namespace AudioStandard
}  //  namespace OHOS