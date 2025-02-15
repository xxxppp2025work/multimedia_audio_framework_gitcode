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
#define LOG_TAG "CoreServiceProviderProxy"
#endif

#include "policy_provider_proxy.h"
#include "audio_service_log.h"
#include "audio_errors.h"

namespace OHOS {
namespace AudioStandard {
CoreServiceProviderProxy::CoreServiceProviderProxy(const sptr<IRemoteObject> &impl) : IRemoteProxy<ICoreServiceProviderIpc>(impl)
{
}

CoreServiceProviderProxy::~CoreServiceProviderProxy()
{
}

int32_t CoreServiceProviderProxy::StartClient(uint32_t sessionId)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    CHECK_AND_RETURN_RET_LOG(data.WriteInterfaceToken(GetDescriptor()), ERROR, "Write descriptor failed!");
    data.WriteUint32(sessionId);

    int ret = Remote()->SendRequest(ICoreServiceProviderMsg::START_CLIENT, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(ret == AUDIO_OK, ERR_OPERATION_FAILED, "failed, error: %{public}d", ret);

    return reply.ReadInt32();
}


int32_t CoreServiceProviderProxy::RemoveClient(uint32_t sessionId)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    CHECK_AND_RETURN_RET_LOG(data.WriteInterfaceToken(GetDescriptor()), ERROR, "Write descriptor failed!");
    data.WriteUint32(sessionId);

    int ret = Remote()->SendRequest(ICoreServiceProviderMsg::REMOVE_CLIENT, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(ret == AUDIO_OK, ERR_OPERATION_FAILED, "failed, error: %{public}d", ret);

    return reply.ReadInt32();
}
} // namespace AudioStandard
} // namespace OHOS
