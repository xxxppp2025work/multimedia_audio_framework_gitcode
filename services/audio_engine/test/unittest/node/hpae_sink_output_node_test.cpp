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
#include <gtest/gtest.h>
#include <cmath>
#include <memory>
#include "hpae_sink_input_node.h"
#include "hpae_sink_output_node.h"
#include "test_case_common.h"
#include "audio_errors.h"
using namespace OHOS;
using namespace AudioStandard;
using namespace HPAE;
namespace {
class HpaeSinkOutputNodeTest : public testing::Test {
public:
    void SetUp();
    void TearDown();
};

void HpaeSinkOutputNodeTest::SetUp()
{}

void HpaeSinkOutputNodeTest::TearDown()
{}

HpaeNodeInfo CreateSinkOutputNodeInfo(size_t frameLen, int channels, AudioSampleFormat format, uint32_t sessionId)
{
    HpaeNodeInfo nodeInfo;
    nodeInfo.frameLen = frameLen;
    nodeInfo.channels = channels;
    nodeInfo.format = format;
    nodeInfo.sessionId = sessionId;
    nodeInfo.samplingRate = SAMPLE_RATE_48000;
    nodeInfo.nodeId = 2000 + sessionId;
    return nodeInfo;
}

HpaeNodeInfo CreateSinkInputNodeInfo(size_t frameLen, int channels, AudioSampleFormat format, uint32_t sessionId)
{
    HpaeNodeInfo nodeInfo;
    nodeInfo.frameLen = frameLen;
    nodeInfo.channels = channels;
    nodeInfo.format = format;
    nodeInfo.sessionId = sessionId;
    nodeInfo.samplingRate = SAMPLE_RATE_48000;
    nodeInfo.nodeId = 3000 + sessionId;
    return nodeInfo;
}

IAudioSinkAttr CreateSinkAttr(const std::string& adapterName)
{
    IAudioSinkAttr attr;
    attr.adapterName = adapterName;
    attr.format = SAMPLE_S32LE;
    attr.sampleRate = SAMPLE_RATE_48000;
    attr.channel = STEREO;
    attr.filePath = nullptr;
    attr.deviceNetworkId = "LocalDevice";
    return attr;
}

TEST_F(HpaeSinkOutputNodeTest, constructHpaeSinkOutputNode)
{
    size_t frameLen = 960;
    uint32_t nodeId = 1243;
    uint32_t sessionId = 10001;
    HpaeNodeInfo nodeInfo;
    nodeInfo.nodeId = nodeId;
    nodeInfo.frameLen = frameLen;
    nodeInfo.samplingRate = SAMPLE_RATE_48000;
    nodeInfo.channels = STEREO;
    nodeInfo.format = SAMPLE_F32LE;
    nodeInfo.sessionId = sessionId;
    std::shared_ptr<HpaeSinkOutputNode> hpaeSinkOutputNode = std::make_shared<HpaeSinkOutputNode>(nodeInfo);
    EXPECT_EQ(hpaeSinkOutputNode->GetSampleRate(), nodeInfo.samplingRate);
    EXPECT_EQ(hpaeSinkOutputNode->GetNodeId(), nodeInfo.nodeId);
    EXPECT_EQ(hpaeSinkOutputNode->GetFrameLen(), nodeInfo.frameLen);
    EXPECT_EQ(hpaeSinkOutputNode->GetChannelCount(), nodeInfo.channels);
    EXPECT_EQ(hpaeSinkOutputNode->GetBitWidth(), nodeInfo.format);
    EXPECT_EQ(hpaeSinkOutputNode->GetSessionId(), nodeInfo.sessionId);

    HpaeNodeInfo &retNi = hpaeSinkOutputNode->GetNodeInfo();
    EXPECT_EQ(retNi.samplingRate, nodeInfo.samplingRate);
    EXPECT_EQ(retNi.nodeId, nodeInfo.nodeId);
    EXPECT_EQ(retNi.frameLen, nodeInfo.frameLen);
    EXPECT_EQ(retNi.channels, nodeInfo.channels);
    EXPECT_EQ(retNi.format, nodeInfo.format);
    EXPECT_EQ(retNi.sessionId, nodeInfo.sessionId);
}
static int32_t TestRendererRenderFrame(const char *data, uint64_t len)
{
    for (int32_t i = 0; i < len / SAMPLE_F32LE; i++) {
        float diff = *((float *)data + i) - i;
        EXPECT_EQ(diff, 0);
    }
    return 0;
}

TEST_F(HpaeSinkOutputNodeTest, testHpaeSinkOutConnectNode)
{
    size_t frameLen = 960;
    uint32_t nodeId = 1243;
    size_t usedCount = 2;
    HpaeNodeInfo nodeInfo;
    nodeInfo.nodeId = nodeId;
    nodeInfo.frameLen = frameLen;
    nodeInfo.samplingRate = SAMPLE_RATE_48000;
    nodeInfo.channels = STEREO;
    nodeInfo.format = SAMPLE_F32LE;
    std::shared_ptr<HpaeSinkOutputNode> hpaeSinkOutputNode = std::make_shared<HpaeSinkOutputNode>(nodeInfo);
    std::shared_ptr<HpaeSinkInputNode> hpaeSinkInputNode = std::make_shared<HpaeSinkInputNode>(nodeInfo);
    hpaeSinkOutputNode->Connect(hpaeSinkInputNode);
    std::shared_ptr<WriteIncDataCb> writeIncDataCb = std::make_shared<WriteIncDataCb>(SAMPLE_F32LE);
    hpaeSinkInputNode->RegisterWriteCallback(writeIncDataCb);
    std::string deviceClass = "file_io";
    std::string deviceNetId = "LocalDevice";
    EXPECT_EQ(hpaeSinkOutputNode->GetRenderSinkInstance(deviceClass, deviceNetId), 0);
    EXPECT_EQ(hpaeSinkOutputNode->GetSinkState() == STREAM_MANAGER_NEW, true);
    IAudioSinkAttr attr;
    attr.adapterName = "file_io";
    attr.openMicSpeaker = 0;
    attr.format = nodeInfo.format;
    attr.sampleRate = nodeInfo.samplingRate;
    attr.channel = nodeInfo.channels;
    attr.volume = 0.0f;
    attr.filePath = nullptr;
    attr.deviceNetworkId = deviceNetId.c_str();
    attr.deviceType = 0;
    attr.channelLayout = 0;
    attr.audioStreamFlag = 0;

    EXPECT_EQ(hpaeSinkOutputNode->RenderSinkInit(attr), ERROR);
    EXPECT_EQ(hpaeSinkOutputNode->GetSinkState() == STREAM_MANAGER_IDLE, true);
    EXPECT_EQ(hpaeSinkOutputNode->RenderSinkStart(), SUCCESS);
    EXPECT_EQ(hpaeSinkOutputNode->GetSinkState() == STREAM_MANAGER_RUNNING, true);
    EXPECT_EQ(hpaeSinkOutputNode->RenderSinkPause(), SUCCESS);
    EXPECT_EQ(hpaeSinkOutputNode->GetSinkState() == STREAM_MANAGER_SUSPENDED, true);
    EXPECT_EQ(hpaeSinkOutputNode->RenderSinkStop(), SUCCESS);
    EXPECT_EQ(hpaeSinkOutputNode->GetSinkState() == STREAM_MANAGER_SUSPENDED, true);
    hpaeSinkOutputNode->DoProcess();
    TestRendererRenderFrame(hpaeSinkOutputNode->GetRenderFrameData(),
        nodeInfo.frameLen * nodeInfo.channels * GetSizeFromFormat(nodeInfo.format));
    EXPECT_EQ(hpaeSinkInputNode.use_count(), usedCount);
    hpaeSinkOutputNode->DisConnect(hpaeSinkInputNode);
    EXPECT_EQ(hpaeSinkInputNode.use_count(), 1);
}

TEST_F(HpaeSinkOutputNodeTest, testDoProcessNullRendererSink)
{
    size_t frameLen = 960;
    uint32_t nodeId = 3000;
    uint32_t sessionId = 10007;
    HpaeNodeInfo nodeInfo = CreateSinkOutputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);
    std::shared_ptr<HpaeSinkOutputNode> node = std::make_shared<HpaeSinkOutputNode>(nodeInfo);
    node->DoProcess();
}

TEST_F(HpaeSinkOutputNodeTest, testDoProcessWithEmptyInput)
{
    size_t frameLen = 960;
    uint32_t sessionId = 10008;
    HpaeNodeInfo nodeInfo = CreateSinkOutputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);
    std::shared_ptr<HpaeSinkOutputNode> node = std::make_shared<HpaeSinkOutputNode>(nodeInfo);

    HpaeNodeInfo sourceInfo = CreateSinkInputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);
    std::shared_ptr<HpaeSinkInputNode> sourceNode = std::make_shared<HpaeSinkInputNode>(sourceInfo);
    node->Connect(sourceNode);
    node->DoProcess();
}

TEST_F(HpaeSinkOutputNodeTest, testResetAndResetAll)
{
    size_t frameLen = 960;
    uint32_t sessionId = 10009;
    HpaeNodeInfo nodeInfo = CreateSinkOutputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);
    std::shared_ptr<HpaeSinkOutputNode> sinkNode = std::make_shared<HpaeSinkOutputNode>(nodeInfo);

    HpaeNodeInfo sourceInfo = CreateSinkInputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);
    std::shared_ptr<HpaeSinkInputNode> sourceNode = std::make_shared<HpaeSinkInputNode>(sourceInfo);
    sinkNode->Connect(sourceNode);

    EXPECT_TRUE(sinkNode->Reset());
    EXPECT_TRUE(sinkNode->ResetAll());
    EXPECT_EQ(sinkNode->GetPreOutNum(), 0);
}

TEST_F(HpaeSinkOutputNodeTest, testInvalidDeviceInitialization)
{
    size_t frameLen = 960;
    uint32_t sessionId = 10010;
    HpaeNodeInfo nodeInfo = CreateSinkOutputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);
    std::shared_ptr<HpaeSinkOutputNode> node = std::make_shared<HpaeSinkOutputNode>(nodeInfo);
    EXPECT_EQ(node->GetRenderSinkInstance("invalid_device", ""), ERROR);
}

TEST_F(HpaeSinkOutputNodeTest, testRendererLifecycleErrorPaths)
{
    size_t frameLen = 960;
    uint32_t sessionId = 10011;
    HpaeNodeInfo nodeInfo = CreateSinkOutputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);
    std::shared_ptr<HpaeSinkOutputNode> node = std::make_shared<HpaeSinkOutputNode>(nodeInfo);
    EXPECT_EQ(node->RenderSinkDeInit(), ERROR);
    EXPECT_EQ(node->RenderSinkStart(), ERROR);
    EXPECT_EQ(node->RenderSinkStop(), ERROR);
    EXPECT_EQ(node->RenderSinkPause(), ERROR);
    EXPECT_EQ(node->RenderSinkResume(), ERROR);
    EXPECT_EQ(node->RenderSinkFlush(), ERROR);
    EXPECT_EQ(node->RenderSinkReset(), ERROR);
}

TEST_F(HpaeSinkOutputNodeTest, testPaPowerHandlingNonPrimaryDevice)
{
    size_t frameLen = 960;
    uint32_t sessionId = 10012;
    HpaeNodeInfo nodeInfo = CreateSinkOutputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);
    std::shared_ptr<HpaeSinkOutputNode> node = std::make_shared<HpaeSinkOutputNode>(nodeInfo);
    HpaePcmBuffer buffer(frameLen, SAMPLE_F32LE, true);
    node->HandlePaPower(&buffer);
}

TEST_F(HpaeSinkOutputNodeTest, testSilentDataProcessing)
{
    size_t frameLen = 960;
    uint32_t sessionId = 10013;
    HpaeNodeInfo nodeInfo = CreateSinkOutputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);

    std::shared_ptr<HpaeSinkOutputNode> node = std::make_shared<HpaeSinkOutputNode>(nodeInfo);

    HpaeNodeInfo sourceInfo = CreateSinkInputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);
    std::shared_ptr<HpaeSinkInputNode> sourceNode = std::make_shared<HpaeSinkInputNode>(sourceInfo);
    node->Connect(sourceNode);
    sourceNode->DoProcess();

    EXPECT_EQ(node->GetRenderSinkInstance("primary", ""), SUCCESS);

    IAudioSinkAttr attr = CreateSinkAttr("primary");
    EXPECT_EQ(node->RenderSinkInit(attr), SUCCESS);
    EXPECT_EQ(node->RenderSinkStart(), SUCCESS);

    for (int i = 0; i < 5; i++) {
        sourceNode->DoProcess();
        node->DoProcess();
    }
    
    node->RenderSinkStop();
    node->RenderSinkDeInit();
}

TEST_F(HpaeSinkOutputNodeTest, testRemoteDeviceTiming)
{
    size_t frameLen = 960;
    uint32_t sessionId = 10014;
    HpaeNodeInfo nodeInfo = CreateSinkOutputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);
    
    std::shared_ptr<HpaeSinkOutputNode> node = std::make_shared<HpaeSinkOutputNode>(nodeInfo);
    
    HpaeNodeInfo sourceInfo = CreateSinkInputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);
    std::shared_ptr<HpaeSinkInputNode> sourceNode = std::make_shared<HpaeSinkInputNode>(sourceInfo);
    node->Connect(sourceNode);
    
    EXPECT_EQ(node->GetRenderSinkInstance("remote", "remote_device_01"), SUCCESS);
    
    IAudioSinkAttr attr = CreateSinkAttr("remote");
    EXPECT_EQ(node->RenderSinkInit(attr), SUCCESS);
    EXPECT_EQ(node->RenderSinkStart(), SUCCESS);
    
    for (int i = 0; i < 3; i++) {
        sourceNode->DoProcess();
        node->DoProcess();
    }

    node->RenderSinkStop();
    node->RenderSinkDeInit();
}

TEST_F(HpaeSinkOutputNodeTest, testStateTransitions)
{
    size_t frameLen = 960;
    uint32_t sessionId = 10015;
    HpaeNodeInfo nodeInfo = CreateSinkOutputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);
    
    std::shared_ptr<HpaeSinkOutputNode> node = std::make_shared<HpaeSinkOutputNode>(nodeInfo);
    EXPECT_EQ(node->GetRenderSinkInstance("primary", ""), SUCCESS);

    IAudioSinkAttr attr = CreateSinkAttr("primary");
    EXPECT_EQ(node->RenderSinkInit(attr), SUCCESS);
    EXPECT_EQ(node->GetSinkState(), STREAM_MANAGER_IDLE);

    EXPECT_EQ(node->RenderSinkStart(), SUCCESS);
    EXPECT_EQ(node->GetSinkState(), STREAM_MANAGER_RUNNING);

    EXPECT_EQ(node->RenderSinkPause(), SUCCESS);
    EXPECT_EQ(node->GetSinkState(), STREAM_MANAGER_SUSPENDED);

    EXPECT_EQ(node->RenderSinkResume(), SUCCESS);
    EXPECT_EQ(node->GetSinkState(), STREAM_MANAGER_RUNNING);

    EXPECT_EQ(node->RenderSinkStop(), SUCCESS);
    EXPECT_EQ(node->GetSinkState(), STREAM_MANAGER_SUSPENDED);

    EXPECT_EQ(node->RenderSinkDeInit(), SUCCESS);
}

TEST_F(HpaeSinkOutputNodeTest, testNullPcmBufferHandling)
{
    size_t frameLen = 960;
    uint32_t sessionId = 10016;
    HpaeNodeInfo nodeInfo = CreateSinkOutputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);

    std::shared_ptr<HpaeSinkOutputNode> node = std::make_shared<HpaeSinkOutputNode>(nodeInfo);
    node->HandlePaPower(nullptr);
}

TEST_F(HpaeSinkOutputNodeTest, testUpdateAppsUidWithoutInitialization)
{
    size_t frameLen = 960;
    uint32_t sessionId = 10017;
    HpaeNodeInfo nodeInfo = CreateSinkOutputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);

    std::shared_ptr<HpaeSinkOutputNode> node = std::make_shared<HpaeSinkOutputNode>(nodeInfo);
    std::vector<int32_t> appsUid = {1001, 1002, 1003};
    EXPECT_EQ(node->UpdateAppsUid(appsUid), ERROR);
}

TEST_F(HpaeSinkOutputNodeTest, testClosePaAfterSilence)
{
    size_t frameLen = 960;
    uint32_t sessionId = 10018;
    HpaeNodeInfo nodeInfo = CreateSinkOutputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);

    std::shared_ptr<HpaeSinkOutputNode> node = std::make_shared<HpaeSinkOutputNode>(nodeInfo);
    EXPECT_EQ(node->GetRenderSinkInstance("primary", ""), SUCCESS);

    IAudioSinkAttr attr = CreateSinkAttr("primary");
    EXPECT_EQ(node->RenderSinkInit(attr), SUCCESS);
    EXPECT_EQ(node->RenderSinkStart(), SUCCESS);

    HpaePcmBuffer silenceBuffer(frameLen, SAMPLE_F32LE, true);

    for (int i = 0; i < 1000; i++) {
        node->HandlePaPower(&silenceBuffer);
    }
    
    node->RenderSinkStop();
    node->RenderSinkDeInit();
}

TEST_F(HpaeSinkOutputNodeTest, testValidConnectionWorkflow)
{
    size_t frameLen = 960;
    uint32_t sessionId = 10019;
    HpaeNodeInfo sinkInfo = CreateSinkOutputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);
    std::shared_ptr<HpaeSinkOutputNode> sinkNode = std::make_shared<HpaeSinkOutputNode>(sinkInfo);

    HpaeNodeInfo sourceInfo = CreateSinkInputNodeInfo(frameLen, STEREO, SAMPLE_F32LE, sessionId);
    std::shared_ptr<HpaeSinkInputNode> sourceNode = std::make_shared<HpaeSinkInputNode>(sourceInfo);

    sinkNode->Connect(sourceNode);
    EXPECT_EQ(sinkNode->GetPreOutNum(), 1);

    sinkNode->DisConnect(sourceNode);
    EXPECT_EQ(sinkNode->GetPreOutNum(), 0);
}
}  // namespace