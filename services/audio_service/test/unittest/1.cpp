audio_endpoint_plus_unit_test.cpp

//   白苗洁
HWTEST_F(AudioEndpointPlusUnitTest, IsNearlinkAbsVolSupportStream_001, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::TYPE_MMAP;
    uint64_t id = 123;
    AudioProcessConfig clientConfig = {};
    auto audioEndpointnIner = std::make_shared<AudioEndpointInner>(type, id, clientConfig);
    EXPECT_TRUE(audioEndpointnIner->IsNearlinkAbsVolSupportStream(DEVICE_TYPE_NEARLINK, STREAM_MUSIC));

    EXPECT_TRUE(audioEndpointnIner->IsNearlinkAbsVolSupportStream(DEVICE_TYPE_NEARLINK, STREAM_VOICE_CALL));
}

HWTEST_F(AudioEndpointPlusUnitTest, CheckSyncInfo_001, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::TYPE_MMAP;
    uint64_t id = 123;
    AudioProcessConfig clientConfig = {};
    auto audioEndpointnIner = std::make_shared<AudioEndpointInner>(type, id, clientConfig);
    audioEndpointnIner->dstSpanSizeInframe_ = 0;
    audioEndpointnIner->CheckSyncInfo(100);
    EXPECT_EQ(audioEndpointnIner->dstSpanSizeInframe_, 0);
}

HWTEST_F(AudioEndpointPlusUnitTest, ProcessToDupStream_001, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::TYPE_MMAP;
    uint64_t id = 123;
    AudioProcessConfig clientConfig = {};
    auto audioEndpointnIner = std::make_shared<AudioEndpointInner>(type, id, clientConfig);
    std::vector<AudioStreamData> audioDataList;
    AudioStreamData dstStreamData;
    int32_t innerCapId = 1;

    audioEndpointnIner->ProcessToDupStream(audioDataList, dstStreamData, innerCapId);
    EXPECT_EQ(innerCapId, 1);
}

audio_server_unit_test.cpp

#include "audio_server_hpae_dump.h"  //  白苗洁
HWTEST_F(AudioServerUnitTest, ArgDataDump_001, TestSize.Level1)
{
    AudioServerHpaeDump audioServerHpaeDump;
    std::string dumpString;
    std::queue<std::u16string> argQue;

    audioServerHpaeDump.ArgDataDump(dumpString, argQue);

    EXPECT_NE(dumpString, "Hpae AudioServer Data Dump:\n\n");
}

audio_capturer_extra_plus_unit_test.cpp
//  白苗洁
HWTEST(AudioCapturerUnitTest, ParamsToStateCmdType_001, TestSize.Level1)
{
    FastAudioStream fastAudioStream(AudioStreamType::STREAM_MUSIC,
        AudioMode::AUDIO_MODE_RECORD, 0);
    State state;
    StateChangeCmdType cmdType;
    int32_t ret = fastAudioStream.ParamsToStateCmdType(FastAudioStreamFork2::HANDLER_PARAM_NEW, state, cmdType);
    EXPECT_EQ(ret, SUCCESS);
    EXPECT_EQ(state, NEW);
    EXPECT_EQ(cmdType, CMD_FROM_CLIENT);
}

HWTEST(AudioCapturerUnitTest, ParamsToStateCmdType_002, TestSize.Level1)
{
    FastAudioStream fastAudioStream(AudioStreamType::STREAM_MUSIC,
        AudioMode::AUDIO_MODE_RECORD, 0);
    State state;
    StateChangeCmdType cmdType;
    int32_t ret = fastAudioStream.ParamsToStateCmdType(FastAudioStreamFork2::HANDLER_PARAM_RELEASED, state, cmdType);
    EXPECT_EQ(ret, SUCCESS);
    EXPECT_EQ(state, RELEASED);
    EXPECT_EQ(cmdType, CMD_FROM_CLIENT);
}

HWTEST(AudioCapturerUnitTest, ParamsToStateCmdType_003, TestSize.Level1)
{
    FastAudioStream fastAudioStream(AudioStreamType::STREAM_MUSIC,
        AudioMode::AUDIO_MODE_RECORD, 0);
    State state;
    StateChangeCmdType cmdType;
    int32_t ret = fastAudioStream.ParamsToStateCmdType(FastAudioStreamFork2::HANDLER_PARAM_PAUSED, state, cmdType);
    EXPECT_EQ(ret, SUCCESS);
    EXPECT_EQ(state, PAUSED);
    EXPECT_EQ(cmdType, CMD_FROM_CLIENT);
}

HWTEST(AudioCapturerUnitTest, ParamsToStateCmdType_004, TestSize.Level1)
{
    FastAudioStream fastAudioStream(AudioStreamType::STREAM_MUSIC,
        AudioMode::AUDIO_MODE_RECORD, 0);
    State state;
    StateChangeCmdType cmdType;
    int32_t ret = fastAudioStream.ParamsToStateCmdType(FastAudioStreamFork2::HANDLER_PARAM_RUNNING_FROM_SYSTEM, state, cmdType);
    EXPECT_EQ(ret, SUCCESS);
    EXPECT_EQ(state, RUNNING);
    EXPECT_EQ(cmdType, CMD_FROM_SYSTEM);
}

HWTEST(AudioCapturerUnitTest, ParamsToStateCmdType_005, TestSize.Level1)
{
    FastAudioStream fastAudioStream(AudioStreamType::STREAM_MUSIC,
        AudioMode::AUDIO_MODE_RECORD, 0);
    State state;
    StateChangeCmdType cmdType;
    int32_t ret = fastAudioStream.ParamsToStateCmdType(FastAudioStreamFork2::HANDLER_PARAM_PAUSED_FROM_SYSTEM, state, cmdType);
    EXPECT_EQ(ret, SUCCESS);
    EXPECT_EQ(state, PAUSED);
    EXPECT_EQ(cmdType, CMD_FROM_SYSTEM);
}

HWTEST(AudioCapturerUnitTest, SetStreamCallback_001, TestSize.Level1)
{
    FastAudioStream fastAudioStream(AudioStreamType::STREAM_MUSIC,
        AudioMode::AUDIO_MODE_RECORD, 0);
    std::shared_ptr<AudioStreamCallback> callback = nullptr;
    int32_t ret = fastAudioStream.SetStreamCallback(callback);
    EXPECT_EQ(ret, ERR_INVALID_PARAM);
}

renderer_in_client_unit_test.cpp

//  白苗洁
HWTEST(RendererInClientInnerUnitTest, GetAudioTime_001, TestSize.Level4)
{
    auto ptrRendererInClientInner = std::make_shared<RendererInClientInner>(AudioStreamType::STREAM_DEFAULT, getpid());
    Timestamp timestamp;
    EXPECT_EQ(ptrRendererInClientInner->GetAudioTime(timestamp, Timestamp::Timestampbase::BASESIZE), false);
}

HWTEST(RendererInClientInnerUnitTest, GetAudioTime_002, TestSize.Level4)
{
    auto ptrRendererInClientInner = std::make_shared<RendererInClientInner>(AudioStreamType::STREAM_DEFAULT, getpid());
    ptrRendererInClientInner->state_ = RELEASED;
    Timestamp timestamp;
    EXPECT_EQ(ptrRendererInClientInner->GetAudioTime(timestamp, Timestamp::Timestampbase::BASESIZE), false);
}

HWTEST(RendererInClientInnerUnitTest, GetAudioTime_003, TestSize.Level4)
{
    auto ptrRendererInClientInner = std::make_shared<RendererInClientInner>(AudioStreamType::STREAM_DEFAULT, getpid());
    ptrRendererInClientInner->paramsIsSet_ = true;
    ptrRendererInClientInner->state_ = RUNNING;
    Timestamp timestamp;
    EXPECT_EQ(ptrRendererInClientInner->GetAudioTime(timestamp, Timestamp::Timestampbase::BASESIZE), false);
}

HWTEST(RendererInClientInnerUnitTest, GetAudioTime_004, TestSize.Level4)
{
    auto ptrRendererInClientInner = std::make_shared<RendererInClientInner>(AudioStreamType::STREAM_DEFAULT, getpid());
    ptrRendererInClientInner->paramsIsSet_ = true;
    ptrRendererInClientInner->state_ = RUNNING;
    ptrRendererInClientInner->offloadEnable_ = true;
    Timestamp timestamp;
    EXPECT_NE(ptrRendererInClientInner->GetAudioTime(timestamp, Timestamp::Timestampbase::BASESIZE), true);
}

HWTEST(RendererInClientInnerUnitTest, SetAudioStreamType_001, TestSize.Level4)
{
    auto ptrRendererInClientInner = std::make_shared<RendererInClientInner>(AudioStreamType::STREAM_DEFAULT, getpid());
    AudioStreamType audioStreamType = AudioStreamType::STREAM_DEFAULT;
    int32_t ret = ptrRendererInClientInner->SetAudioStreamType(audioStreamType);
    EXPECT_EQ(ret, SUCCESS);
}

HWTEST(RendererInClientInnerUnitTest, Write_001, TestSize.Level4)
{
    auto ptrRendererInClientInner = std::make_shared<RendererInClientInner>(AudioStreamType::STREAM_DEFAULT, getpid());
    uint8_t pcmBuffer[10] = {0};
    size_t pcmBufferSize = 10;
    uint8_t metaBuffer[10] = {0};
    size_t metaBufferSize = 10;

    ptrRendererInClientInner->renderMode_ = RENDER_MODE_NORMAL;
    int32_t ret = ptrRendererInClientInner->Write(pcmBuffer, pcmBufferSize, metaBuffer, metaBufferSize);
    EXPECT_NE(ret, pcmBufferSize);
}

HWTEST(RendererInClientInnerUnitTest, Write_002, TestSize.Level4)
{
    auto ptrRendererInClientInner = std::make_shared<RendererInClientInner>(AudioStreamType::STREAM_DEFAULT, getpid());
    uint8_t pcmBuffer[10] = {0};
    size_t pcmBufferSize = 10;
    uint8_t metaBuffer[10] = {0};
    size_t metaBufferSize = 10;

    ptrRendererInClientInner->renderMode_ = RENDER_MODE_CALLBACK;
    int32_t ret = ptrRendererInClientInner->Write(pcmBuffer, pcmBufferSize, metaBuffer, metaBufferSize);
    EXPECT_EQ(ret, ERR_INCORRECT_MODE);
}

HWTEST(RendererInClientInnerUnitTest, GetStreamSwitchInfo_001, TestSize.Level4)
{
    auto ptrRendererInClientInner = std::make_shared<RendererInClientInner>(AudioStreamType::STREAM_DEFAULT, getpid());
    IAudioStream::SwitchInfo info;

    // Set up the renderer with some known values
    ptrRendererInClientInner->effectMode_ = EFFECT_NONE;
    ptrRendererInClientInner->rendererRate_ = RENDER_RATE_NORMAL;
    ptrRendererInClientInner->clientPid_ = 1234;
    ptrRendererInClientInner->clientUid_ = 5678;
    ptrRendererInClientInner->clientVolume_ = 50;
    ptrRendererInClientInner->duckVolume_ = 30;
    ptrRendererInClientInner->silentModeAndMixWithOthers_ = false;
    ptrRendererInClientInner->rendererMarkPosition_ = 1000;
    ptrRendererInClientInner->rendererPositionCallback_ = nullptr;
    ptrRendererInClientInner->rendererPeriodSize_ = 1024;
    ptrRendererInClientInner->rendererPeriodPositionCallback_ = nullptr;
    ptrRendererInClientInner->writeCb_ = nullptr;

    // Call the function under test
    ptrRendererInClientInner->GetStreamSwitchInfo(info);

    // Check if the SwitchInfo structure is correctly set
    EXPECT_EQ(info.underFlowCount, ptrRendererInClientInner->GetUnderflowCount());
    EXPECT_EQ(info.effectMode, EFFECT_NONE);
    EXPECT_NE(info.renderRate, 44100);
    EXPECT_EQ(info.clientPid, 1234);
    EXPECT_EQ(info.clientUid, 5678);
    EXPECT_EQ(info.volume, 50);
    EXPECT_EQ(info.duckVolume, 30);
    EXPECT_EQ(info.silentModeAndMixWithOthers, false);
    EXPECT_EQ(info.frameMarkPosition, 1000);
    EXPECT_EQ(info.renderPositionCb, nullptr);
    EXPECT_EQ(info.framePeriodNumber, 1024);
    EXPECT_EQ(info.renderPeriodPositionCb, nullptr);
    EXPECT_EQ(info.rendererWriteCallback, nullptr);
}

HWTEST(RendererInClientInnerUnitTest, SetSourceDuration_001, TestSize.Level4)
{
    auto ptrRendererInClientInner = std::make_shared<RendererInClientInner>(AudioStreamType::STREAM_DEFAULT, getpid());
    int64_t duration = 100;
    ptrRendererInClientInner->ipcStream_ = nullptr;
    int32_t ret = ptrRendererInClientInner->SetSourceDuration(duration);
    EXPECT_EQ(ret, ERR_OPERATION_FAILED);
}

HWTEST(RendererInClientInnerUnitTest, SetSourceDuration_002, TestSize.Level4)
{
    auto ptrRendererInClientInner = std::make_shared<RendererInClientInner>(AudioStreamType::STREAM_DEFAULT, getpid());
    int64_t duration = 100;
    ptrRendererInClientInner->ipcStream_ = new(std::nothrow) IpcStreamTest();
    int32_t ret = ptrRendererInClientInner->SetSourceDuration(duration);
    EXPECT_NE(ret, ERROR);
}

HWTEST(RendererInClientInnerUnitTest, SetOffloadDataCallbackState_001, TestSize.Level4)
{
    auto ptrRendererInClientInner = std::make_shared<RendererInClientInner>(AudioStreamType::STREAM_DEFAULT, getpid());
    int cbState = 1;
    EXPECT_EQ(ptrRendererInClientInner->SetOffloadDataCallbackState(cbState), ERR_OPERATION_FAILED);
}

HWTEST(RendererInClientInnerUnitTest, SetOffloadDataCallbackState_002, TestSize.Level4)
{
    auto ptrRendererInClientInner = std::make_shared<RendererInClientInner>(AudioStreamType::STREAM_DEFAULT, getpid());
    ptrRendererInClientInner->ipcStream_ = new(std::nothrow) IpcStreamTest();
    int cbState = 1;
    EXPECT_EQ(ptrRendererInClientInner->SetOffloadDataCallbackState(cbState), SUCCESS);
}

audio_spatial_channel_converter_unit_test.cpp
//  白苗洁
HWTEST(AudioApatialChannelCoverterUnitTest, GetInputBufferSize_001, TestSize.Level1)
{
    auto audioSpatialChannelConverter = std::make_shared<AudioSpatialChannelConverter>();
    ASSERT_TRUE(audioSpatialChannelConverter != nullptr);
    size_t bufferSize = 0;
    bool result = audioSpatialChannelConverter->GetInputBufferSize(bufferSize);
    EXPECT_FALSE(result);
}

HWTEST(AudioApatialChannelCoverterUnitTest, GetOutputBufferStream_001, TestSize.Level1)
{
    auto audioSpatialChannelConverter = std::make_shared<AudioSpatialChannelConverter>();
    ASSERT_TRUE(audioSpatialChannelConverter != nullptr);
    uint8_t *buffer = nullptr;
    uint32_t bufferLen = 0;

    audioSpatialChannelConverter->GetOutputBufferStream(buffer, bufferLen);

    EXPECT_EQ(buffer, nullptr);
}

audio_general_manager_unit_test.cpp
//  白苗洁
HWTEST(AudioGeneralManagerUnitTest, SaveCallback_001, TestSize.Level4)
{
    auto audioFocusInfoChangeCallbackImpl = std::make_shared<AudioFocusInfoChangeCallbackImpl>();
    EXPECT_NE(audioFocusInfoChangeCallbackImpl, nullptr);
    std::shared_ptr<AudioFocusInfoChangeCallback> callback = std::make_shared<AudioFocusInfoChangeCallbackImpl>();
    EXPECT_NE(callback, nullptr);

    audioFocusInfoChangeCallbackImpl->SaveCallback(callback);

    EXPECT_EQ(audioFocusInfoChangeCallbackImpl->callbackList_.size(), 1);
}

audio_system_manager_unit_test.cpp

//  白苗洁
HWTEST(AudioSystemManagerUnitTest, OnWorkgroupChange_001, TestSize.Level4)
{
    AudioWorkgroupCallbackImpl audioWorkgroupCallbackImpl;
    AudioWorkgroupChangeInfoIpc info;
    audioWorkgroupCallbackImpl.workgroupCb_ = nullptr;
    EXPECT_EQ(audioWorkgroupCallbackImpl.OnWorkgroupChange(info), ERROR);
}

HWTEST(AudioSystemManagerUnitTest, RemoveWorkgroupChangeCallback_001, TestSize.Level4)
{
    AudioWorkgroupCallbackImpl audioWorkgroupCallbackImpl;
    audioWorkgroupCallbackImpl.RemoveWorkgroupChangeCallback();
    EXPECT_EQ(nullptr, audioWorkgroupCallbackImpl.workgroupCb_);
}

audio_stream_manager_unit_test.cpp

//   白苗洁
HWTEST(AudioStreamManagerUnitTest, RegisterAudioRendererEventListener_001, TestSize.Level1)
{
    int32_t clientPid = 1;
    std::shared_ptr<AudioRendererStateChangeCallback> callback = nullptr;

    EXPECT_EQ(AudioStreamManager::GetInstance()->RegisterAudioRendererEventListener(clientPid, callback), ERR_INVALID_PARAM);
}

HWTEST(AudioStreamManagerUnitTest, GetHardwareOutputSamplingRate_002, TestSize.Level1)
{
    std::shared_ptr<AudioDeviceDescriptor> desc = nullptr;
    int32_t result = AudioStreamManager::GetInstance()->GetHardwareOutputSamplingRate(desc);
    EXPECT_NE(result, ERR_INVALID_PARAM);
}

HWTEST(AudioStreamManagerUnitTest, GetHardwareOutputSamplingRate_003, TestSize.Level1)
{
    std::shared_ptr<AudioDeviceDescriptor> desc = std::make_shared<AudioDeviceDescriptor>();
    desc->deviceType_ = DEVICE_TYPE_SPEAKER;
    desc->deviceRole_ = OUTPUT_DEVICE;
    int32_t result = AudioStreamManager::GetInstance()->GetHardwareOutputSamplingRate(desc);
    EXPECT_NE(result, 0);
}

HWTEST(AudioStreamManagerUnitTest, GetSupportedAudioEffectProperty_002, TestSize.Level1)
{
    AudioEffectPropertyArray propertyArray;
    int32_t result = AudioStreamManager::GetInstance()->GetSupportedAudioEffectProperty(propertyArray);
    EXPECT_NE(result, 0);
}

HWTEST(AudioStreamManagerUnitTest, GetSupportedAudioEnhanceProperty_001, TestSize.Level1)
{
    AudioEnhancePropertyArray propertyArray;
    int32_t result = AudioStreamManager::GetInstance()->GetSupportedAudioEnhanceProperty(propertyArray);
    EXPECT_NE(result, 0);
}

HWTEST(AudioStreamManagerUnitTest, SetAudioEffectProperty_001, TestSize.Level1)
{
    AudioEffectPropertyArray propertyArray;
    int32_t result = AudioStreamManager::GetInstance()->SetAudioEffectProperty(propertyArray);
    EXPECT_NE(result, 0);
}

HWTEST(AudioStreamManagerUnitTest, GetAudioEffectProperty_002, TestSize.Level1)
{
    AudioEffectPropertyArray propertyArray;
    int32_t result = AudioStreamManager::GetInstance()->GetAudioEffectProperty(propertyArray);
    EXPECT_NE(result, 0);
}

HWTEST(AudioStreamManagerUnitTest, SetAudioEnhanceProperty_001, TestSize.Level1)
{
    AudioEnhancePropertyArray propertyArray;
    int32_t result = AudioStreamManager::GetInstance()->SetAudioEnhanceProperty(propertyArray);
    EXPECT_NE(result, 0);
}

HWTEST(AudioStreamManagerUnitTest, GetAudioEnhanceProperty_002, TestSize.Level1)
{
    AudioEnhancePropertyArray propertyArray;
    int32_t result = AudioStreamManager::GetInstance()->GetAudioEnhanceProperty(propertyArray);
    EXPECT_NE(result, 0);
}

HWTEST(AudioStreamManagerUnitTest, IsCapturerFocusAvailable_001, TestSize.Level1)
{
    AudioCapturerInfo capturerInfo;
    int32_t result = AudioStreamManager::GetInstance()->IsCapturerFocusAvailable(capturerInfo);
    EXPECT_NE(result, 0);
}




HWTEST(RendererInClientInnerUnitTest, GetStreamSwitchInfo_001, TestSize.Level4)
{
    auto ptrRendererInClientInner = std::make_shared<RendererInClientInner>(AudioStreamType::STREAM_DEFAULT, getpid());
    IAudioStream::SwitchInfo info;

    // Set up the renderer with some known values
    ptrRendererInClientInner->effectMode_ = EFFECT_NONE;
    ptrRendererInClientInner->rendererRate_ = RENDER_RATE_NORMAL;
    ptrRendererInClientInner->clientPid_ = 1234;
    ptrRendererInClientInner->clientUid_ = 5678;
    ptrRendererInClientInner->clientVolume_ = 50;
    ptrRendererInClientInner->duckVolume_ = 30;
    ptrRendererInClientInner->silentModeAndMixWithOthers_ = false;
    ptrRendererInClientInner->rendererMarkPosition_ = 1000;
    ptrRendererInClientInner->rendererPositionCallback_ = nullptr;
    ptrRendererInClientInner->rendererPeriodSize_ = 1024;
    ptrRendererInClientInner->rendererPeriodPositionCallback_ = nullptr;
    ptrRendererInClientInner->writeCb_ = nullptr;

    // Call the function under test
    ptrRendererInClientInner->GetStreamSwitchInfo(info);

    // Check if the SwitchInfo structure is correctly set
    EXPECT_EQ(info.underFlowCount, ptrRendererInClientInner->GetUnderflowCount());
    EXPECT_EQ(info.effectMode, ptrRendererInClientInner->effectMode_);
    EXPECT_EQ(info.renderRate, ptrRendererInClientInner->rendererRate_);
    EXPECT_EQ(info.clientPid, ptrRendererInClientInner->clientPid_);
    EXPECT_EQ(info.clientUid, ptrRendererInClientInner->clientUid_);
    EXPECT_EQ(info.volume, ptrRendererInClientInner->clientVolume_);
    EXPECT_EQ(info.duckVolume, ptrRendererInClientInner->duckVolume_);
    EXPECT_EQ(info.silentModeAndMixWithOthers, ptrRendererInClientInner->silentModeAndMixWithOthers_);
    EXPECT_EQ(info.frameMarkPosition, ptrRendererInClientInner->rendererMarkPosition_);
    EXPECT_EQ(info.renderPositionCb, ptrRendererInClientInner->rendererPositionCallback_);
    EXPECT_EQ(info.framePeriodNumber, ptrRendererInClientInner->rendererPeriodSize_);
    EXPECT_EQ(info.renderPeriodPositionCb, ptrRendererInClientInner->rendererPeriodPositionCallback_);
    EXPECT_EQ(info.rendererWriteCallback, ptrRendererInClientInner->writeCb_);
}






