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
#define LOG_TAG "CoreServiceProviderStub"
#endif

#include "core_service_provider_stub.h"
#include "audio_service_log.h"
#include "audio_errors.h"

namespace OHOS {
namespace AudioStandard {
bool CoreServiceProviderStub::CheckInterfaceToken(MessageParcel &data)
{
    static auto localDescriptor = IPolicyProviderIpc::GetDescriptor();
    auto remoteDescriptor = data.ReadInterfaceToken();
    CHECK_AND_RETURN_RET_LOG(remoteDescriptor == localDescriptor, false, "CheckInterFfaceToken failed.");
    return true;
}

int CoreServiceProviderStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    bool ret = CheckInterfaceToken(data);
    CHECK_AND_RETURN_RET(ret, AUDIO_ERR);
    if (code >= IPolicyProviderMsg::CORE_SERVICE_PROVIDER_MAX_MSG) {
        AUDIO_WARNING_LOG("Unsupported request code:%{public}d.", code);
        return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
    }
    switch (code) {
        case START_CLIENT:
            return HandleStartClient(data, reply);
        case REMOVE_CLIENT:
            return HandleRemoveClient(data, reply);
        default:
            AUDIO_WARNING_LOG("Unsupported request code:%{public}d.", code);
            return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
    }
}

int32_t CoreServiceProviderStub::HandleStartClient(MessageParcel &data, MessageParcel &reply)
{
    uint32_t sessionId = data.ReadUint32();
    int32_t ret = StartClient(sessionId);
    reply.WriteInt32(ret);
    return AUDIO_OK;
}

int32_t CoreServiceProviderStub::HandleRemoveClient(MessageParcel &data, MessageParcel &reply)
{
    uint32_t sessionId = data.ReadUint32();
    int32_t ret = RemoveClient(sessionId);
    reply.WriteInt32(ret);
    return AUDIO_OK;
}

CoreServiceProviderWrapper::~CoreServiceProviderWrapper()
{
    coreServiceWorker_ = nullptr;
}

CoreServiceProviderWrapper::CoreServiceProviderWrapper(ICoreServiceProvider *coreServiceWorker) : coreServiceWorker_(coreServiceWorker)
{
}

int32_t CoreServiceProviderWrapper::StartClient(uint32_t sessionId)
{
    CHECK_AND_RETURN_RET_LOG(coreServiceWorker_ != nullptr, AUDIO_INIT_FAIL, "coreServiceWorker_ is null");
    return coreServiceWorker_->StartClient(sessionId);
}

int32_t CoreServiceProviderWrapper::RemoveClient(uint32_t sessionId)
{
    CHECK_AND_RETURN_RET_LOG(coreServiceWorker_ != nullptr, AUDIO_INIT_FAIL, "coreServiceWorker_ is null");
    return coreServiceWorker_->RemoveClient(sessionId);
}
} // namespace AudioStandard
} // namespace OHOS
