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

#ifndef AUDIO_POLICY_CLIENT_PROXY_MOCK_H
#define AUDIO_POLICY_CLIENT_PROXY_MOCK_H

#include "audio_policy_log.h"
#include "ipc_types.h"
#include "iremote_broker.h"
#include "iremote_proxy.h"
#include "iremote_stub.h"

namespace OHOS {
namespace AudioStandard {
class IAudioPolicyMockOne : public IRemoteBroker {
public:

    virtual void TesFunc(int32_t code) = 0;

public:
    DECLARE_INTERFACE_DESCRIPTOR(u"IAudioPolicyMockOne");
};

class AudioPolicyClientStubMockOne : public IRemoteStub<IAudioPolicyMockOne> {
public:
    AudioPolicyClientStubMockOne() = default;
    ~AudioPolicyClientStubMockOne() = default;

    virtual int OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
        MessageOption &option)override
    {
        (void)code;
        (void)data;
        (void)reply;
        (void)option;
        return 0;
    }

    virtual void TesFunc(int32_t code) override
    {
        (void)code;
    }
};

class IAudioPolicyMockTwo : public IRemoteBroker {
public:

    virtual void TesFunc(int32_t code) = 0;

public:
    DECLARE_INTERFACE_DESCRIPTOR(u"IAudioPolicyMockTwo");
};

class AudioPolicyClientStubMockTwo : public IRemoteStub<IAudioPolicyMockTwo> {
public:
    AudioPolicyClientStubMockTwo() = default;
    ~AudioPolicyClientStubMockTwo() = default;

    virtual int OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
        MessageOption &option)override
    {
        (void)code;
        (void)data;
        (void)reply;
        (void)option;
        return -1;
    }

    virtual void TesFunc(int32_t code) override
    {
        (void)code;
    }
};
} // namespace AudioStandard
} // namespace OHOS
#endif // AUDIO_POLICY_CLIENT_PROXY_TEST_H

