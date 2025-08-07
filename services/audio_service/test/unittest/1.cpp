pro_audio_stream_manager_unit_test.cpp

HWTEST(ProAudioStreamManagerUnitTest, CreateRendererStream_001, TestSize.Level1)
{
    shared_ptr<ProAudioStreamManager> audioStreamManager = make_shared<ProAudioStreamManager>(DIRECT_PLAYBACK);
    ASSERT_TRUE(audioStreamManager != nullptr);
    AudioProcessConfig config;
    // Mock InitParams to return failure
    auto stream = audioStreamManager->CreateRendererStream(config);
    EXPECT_NE(stream, nullptr);
}

HWTEST(ProAudioStreamManagerUnitTest, ReleaseRender_001, TestSize.Level1)
{
    shared_ptr<ProAudioStreamManager> audioStreamManager = make_shared<ProAudioStreamManager>(DIRECT_PLAYBACK);
    ASSERT_TRUE(audioStreamManager != nullptr);

    int32_t result = audioStreamManager->ReleaseRender(1);

    EXPECT_EQ(result, SUCCESS);
    EXPECT_EQ(audioStreamManager->rendererStreamMap_.size(), 0);
}

audio_endpoint_separate_unit_test.cpp
HWTEST(AudioEndpointSeparateUnitTest, GetAdapterBufferInfo_001, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    std::shared_ptr<AudioDeviceDescriptor> ptr2 =
        std::make_shared<AudioDeviceDescriptor>(AudioDeviceDescriptor::DEVICE_INFO);
    ptr2->deviceRole_ = OUTPUT_DEVICE;

    EXPECT_NE(SUCCESS, ptr->GetAdapterBufferInfo(ptr2));
}

HWTEST(AudioEndpointSeparateUnitTest, PrepareDeviceBuffer_001, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    int ret = 1;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    std::shared_ptr<AudioDeviceDescriptor> ptr2 =
        std::make_shared<AudioDeviceDescriptor>(AudioDeviceDescriptor::DEVICE_INFO);
    ptr2->deviceRole_ = OUTPUT_DEVICE;
    ret = ptr->PrepareDeviceBuffer(ptr2);
    EXPECT_NE(ret, SUCCESS);

    ret = ptr->GetAdapterBufferInfo(*ptr2);
    ret = ptr->PrepareDeviceBuffer(ptr2);
    EXPECT_EQ(ret, ERR_OPERATION_FAILED);
}

HWTEST(AudioEndpointSeparateUnitTest, PrepareDeviceBuffer_002, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    int ret = 1;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    std::shared_ptr<AudioDeviceDescriptor> ptr2 =
        std::make_shared<AudioDeviceDescriptor>(AudioDeviceDescriptor::DEVICE_INFO);
    ptr2->deviceRole_ = OUTPUT_DEVICE;
    ret = ptr->GetAdapterBufferInfo(*ptr2);
    ptr->spanDuration_ = MAX_SPAN_DURATION_NS + 1;
    ret = ptr->PrepareDeviceBuffer(ptr2);
    EXPECT_NE(ret, ERR_INVALID_PARAM);
}

HWTEST(AudioEndpointSeparateUnitTest, PrepareDeviceBuffer_003, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    int result = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    result = ptr->DisableFastInnerCap();
    EXPECT_EQ(result, ERR_INVALID_OPERATION);

    result = ptr->DisableFastInnerCap(1);
    EXPECT_EQ(result, ERR_INVALID_OPERATION);
}

HWTEST(AudioEndpointSeparateUnitTest, GetDeviceHandleInfo_001, TestSize.Level1)
{
    AudioEndpoint::EndpointType type = AudioEndpoint::EndpointType::TYPE_MMAP;
    uint64_t id = 0;
    AudioStreamType streamType = AudioStreamType::STREAM_DEFAULT;
    std::shared_ptr<AudioEndpointSeparate> ptr = std::make_shared<AudioEndpointSeparate>(type, id, streamType);
    uint64_t frames = 0;
    int64_t nanoTime = 0;
    EXPECT_EQ(ptr->GetDeviceHandleInfo(frames, nanoTime), false);
}
audio_service_unit_test.cpp

 //  baimiaojie

 static constexpr int32_t AUDIO_MAX_PROCESS = 2;
HWTEST(AudioServiceUnitTest, GetDeviceInfoForProcess_001, TestSize.Level1)
{
    AudioProcessConfig config = {};
    config.originalSessionId = 1;
    config.rendererInfo.streamUsage = STREAM_USAGE_VOICE_COMMUNICATION;
    config.capturerInfo.sourceType = SOURCE_TYPE_VOICE_COMMUNICATION;
    config.streamInfo.samplingRate = SAMPLE_RATE_16000;
    bool isReloadProcess = false;

    AudioDeviceDescriptor deviceInfo = AudioService::GetInstance()->GetDeviceInfoForProcess(config, isReloadProcess);

    EXPECT_NE(deviceInfo.deviceType_, DEVICE_TYPE_MIC);
    EXPECT_EQ(deviceInfo.isLowLatencyDevice_, false);
    EXPECT_EQ(deviceInfo.a2dpOffloadFlag_, 0);
    EXPECT_EQ(deviceInfo.audioStreamInfo_.size(), 1);
    EXPECT_EQ(deviceInfo.deviceName_, "mmap_device");
}

HWTEST(AudioServiceUnitTest, GetDeviceInfoForProcess_002, TestSize.Level1)
{
    AudioProcessConfig config = {};
    config.originalSessionId = 1;
    config.audioMode = AUDIO_MODE_RECORD;
    bool isReloadProcess = false;

    AudioDeviceDescriptor deviceInfo = AudioService::GetInstance()->GetDeviceInfoForProcess(config, isReloadProcess);

    EXPECT_EQ(deviceInfo.deviceId_, 1);
    EXPECT_EQ(deviceInfo.networkId_, LOCAL_NETWORK_ID);
    EXPECT_EQ(deviceInfo.deviceRole_, INPUT_DEVICE);
    EXPECT_EQ(deviceInfo.deviceType_, DEVICE_TYPE_MIC);
    EXPECT_EQ(deviceInfo.audioStreamInfo_.size(), 1);

    EXPECT_EQ(deviceInfo.deviceName_, "mmap_device");
}

HWTEST(AudioServiceUnitTest, InitAllDupBuffer_001, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();
    int32_t innerCapId = 1;

    std::weak_ptr<RendererInServer> server;
    std::unique_lock<std::mutex> lock(audioService->rendererMapMutex_);
    AudioService::GetInstance()->filteredRendererMap_[innerCapId].push_back(server);
    lock.unlock();

    AudioService::GetInstance()->InitAllDupBuffer(innerCapId);

    //EXPECT_EQ(server.InitDupBufferCalled, true);
}

HWTEST(AudioServiceUnitTest, RenderersCheckForAudioWorkgroup_001, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();
    audioService->RenderersCheckForAudioWorkgroup(1);
    EXPECT_FALSE(AudioResourceService::GetInstance()->IsProcessInWorkgroup(1));
    EXPECT_FALSE(AudioResourceService::GetInstance()->IsProcessHasSystemPermission(1));

    audioService->RenderersCheckForAudioWorkgroup(-1);
    EXPECT_FALSE(AudioResourceService::GetInstance()->IsProcessInWorkgroup(-1));
    EXPECT_FALSE(AudioResourceService::GetInstance()->IsProcessHasSystemPermission(-1));
}

HWTEST(AudioServiceUnitTest, GetSystemVolume_001, TestSize.Level1)
{
    AudioService *audioService = AudioService::GetInstance();
    audioService->musicOrVoipSystemVolume_ = 0.5;
    float volume = 0.0;
    volume = audioService->GetSystemVolume();
    EXPECT_EQ(volume, 0.5);

    audioService->musicOrVoipSystemVolume_ = 1.0;
    volume = audioService->GetSystemVolume();
    EXPECT_EQ(volume, 1.0);

    audioService->musicOrVoipSystemVolume_ = 0.0;
    volume = audioService->GetSystemVolume();
    EXPECT_EQ(volume, 0.0);
}

HWTEST(AudioServiceUnitTest, LinkProcessToEndpoint_001, TestSize.Level1)
{
    AudioProcessConfig config = {};
    config.audioMode = AUDIO_MODE_PLAYBACK;
    sptr<AudioProcessInServer> audioprocess =  AudioProcessInServer::Create(config, AudioService::GetInstance());
    EXPECT_NE(audioprocess, nullptr);
    std::shared_ptr<AudioEndpointInner> endpoint = std::make_shared<AudioEndpointInner>(AudioEndpoint::TYPE_VOIP_MMAP,
        123, config);
    EXPECT_NE(AudioService::GetInstance()->LinkProcessToEndpoint(audioprocess, endpoint), SUCCESS);
}

HWTEST(AudioServiceUnitTest, UpdateForegroundState_001, TestSize.Level1)
{
    uint32_t appTokenId = 12345;
    bool isActive = true;
    bool result = AudioService::GetInstance()->UpdateForegroundState(appTokenId, isActive);
    EXPECT_TRUE(result);
}

HWTEST(AudioServiceUnitTest, UpdateForegroundState_002, TestSize.Level1)
{
    uint32_t appTokenId = -1;
    bool isActive = true;
    bool result = AudioService::GetInstance()->UpdateForegroundState(appTokenId, isActive);
    EXPECT_TRUE(result);
}

HWTEST(AudioServiceUnitTest, DumpForegroundList_001, TestSize.Level1)
{
    std::string dumpString;
    AudioService::GetInstance()->DumpForegroundList(dumpString);
    EXPECT_NE(dumpString, "DumpForegroundList:\n");
}

audio_resource_service_unit_test.cpp


HWTEST(AudioResourceServiceUnitTest, IsProcessHasSystemPermission_001, TestSize.Level0)
{
    int32_t pid = 123;
    audioResourceService.audioWorkgroupMap_[pid].hasSystemPermission = true;
    EXPECT_TRUE(audioResourceService.IsProcessHasSystemPermission(pid));
}

HWTEST(AudioResourceServiceUnitTest, IsProcessHasSystemPermission_002, TestSize.Level0)
{
    int32_t pid = 123;
    audioResourceService.audioWorkgroupMap_[pid].hasSystemPermission = false;
    EXPECT_FALSE(audioResourceService.IsProcessHasSystemPermission(pid));
}

HWTEST(AudioResourceServiceUnitTest, RegisterAudioWorkgroupMonitor_002, TestSize.Level0)
{
    int32_t pid = 1;
    int32_t groupId = 1;
    sptr<IRemoteObject> remoteObj = new RemoteObjectTestStub();
    EXPECT_NE(audioResourceService.RegisterAudioWorkgroupMonitor(pid, groupId, remoteObj), SUCCESS);
}

HWTEST(AudioResourceServiceUnitTest, RegisterAudioWorkgroupMonitor_003, TestSize.Level0)
{
    int32_t pid = 1;
    int32_t groupId = 1;
    sptr<IRemoteObject> object = nullptr;
    EXPECT_EQ(audioResourceService.RegisterAudioWorkgroupMonitor(pid, groupId, object), ERR_INVALID_PARAM);
}

HWTEST(AudioResourceServiceUnitTest, RegisterAudioWorkgroupMonitor_004, TestSize.Level0)
{
    int32_t pid = 1;
    int32_t groupId = 1;
    sptr<IRemoteObject> remoteObj = new RemoteObjectTestStub();
    audioResourceService.audioWorkgroupMap_[pid].hasSystemPermission = false;
    EXPECT_EQ(audioResourceService.RegisterAudioWorkgroupMonitor(pid, groupId, remoteObj), ERR_INVALID_PARAM);
}

HWTEST(AudioResourceServiceUnitTest, RegisterAudioWorkgroupMonitor_005, TestSize.Level0)
{
    int32_t pid = 1;
    int32_t groupId = 1;
    sptr<IRemoteObject> remoteObj = new RemoteObjectTestStub();
    std::shared_ptr<AudioWorkgroup> workgroup = std::make_shared<AudioWorkgroup>(testRtgId);
    audioResourceService.audioWorkgroupMap_[pid].hasSystemPermission = false;
    audioResourceService.audioWorkgroupMap_[1].groups[testRtgId] = {workgroup};
    EXPECT_NE(audioResourceService.RegisterAudioWorkgroupMonitor(pid, groupId, remoteObj), SUCCESS);
}

HWTEST(AudioResourceServiceUnitTest, StartGroup_001, TestSize.Level0)
{
    int32_t pid = 1;
    int32_t workgroupId = 1;
    uint64_t startTime = 1000;
    uint64_t deadlineTime = 2000;

    int32_t ret = audioResourceService.StartGroup(pid, workgroupId, startTime, deadlineTime);

    EXPECT_EQ(ret, ERR_INVALID_PARAM);
}

HWTEST(AudioResourceServiceUnitTest, RemoveThreadFromGroup_001, TestSize.Level0)
{
    int32_t pid = 1;
    int32_t workgroupId = 1;
    int32_t tokenId = 1;

    int32_t ret = audioResourceService.RemoveThreadFromGroup(pid, workgroupId, tokenId);

    EXPECT_EQ(ret, ERR_INVALID_PARAM);
}

HWTEST(AudioResourceServiceUnitTest, AddThreadToGroup_002, TestSize.Level0)
{
    int32_t pid = 1;
    int32_t workgroupId = 1;
    int32_t tokenId = 1;
    int32_t ret = audioResourceService.AddThreadToGroup(pid, workgroupId, tokenId);
    EXPECT_EQ(ret, ERR_OPERATION_FAILED);
}

HWTEST(AudioResourceServiceUnitTest, AddThreadToGroup_003, TestSize.Level0)
{
    int32_t pid = 2;
    int32_t workgroupId = 2;
    int32_t tokenId = 2;
    int32_t ret = audioResourceService.AddThreadToGroup(pid, workgroupId, tokenId);
    EXPECT_NE(ret, ERR_INVALID_PARAM);
}

HWTEST(AudioResourceServiceUnitTest, AddThreadToGroup_004, TestSize.Level0)
{
    int32_t pid = 3;
    int32_t workgroupId = 3;
    int32_t tokenId = 3;
    int32_t ret = audioResourceService.AddThreadToGroup(pid, workgroupId, tokenId);
    EXPECT_NE(ret, ERR_NOT_SUPPORTED);
}

HWTEST(AudioResourceServiceUnitTest, AddThreadToGroup_005, TestSize.Level0)
{
    int32_t pid = 4;
    int32_t workgroupId = 4;
    int32_t tokenId = 4;
    int32_t ret = audioResourceService.AddThreadToGroup(pid, workgroupId, tokenId);
    EXPECT_NE(ret, SUCCESS);
}

HWTEST(AudioResourceServiceUnitTest, ReleaseAudioWorkgroup_002, TestSize.Level0)
{
    int32_t pid = -1;
    int32_t workgroupId = 1;
    int32_t ret = audioResourceService.ReleaseAudioWorkgroup(pid, workgroupId);
    EXPECT_EQ(ret, ERR_OPERATION_FAILED);
}

HWTEST(AudioResourceServiceUnitTest, ReleaseAudioWorkgroup_003, TestSize.Level0)
{
    int32_t pid = 1;
    int32_t workgroupId = 1;
    int32_t ret = audioResourceService.ReleaseAudioWorkgroup(pid, workgroupId);
    EXPECT_NE(ret, SUCCESS);
}

HWTEST(AudioResourceServiceUnitTest, ReleaseAudioWorkgroup_004, TestSize.Level0)
{
    int32_t pid = 1;
    int32_t workgroupId = -1;
    int32_t ret = audioResourceService.ReleaseAudioWorkgroup(pid, workgroupId);
    EXPECT_NE(ret, ERR_OPERATION_FAILED);
}

HWTEST(AudioResourceServiceUnitTest, ReleaseAudioWorkgroup_005, TestSize.Level0)
{
    int32_t pid = 1;
    int32_t workgroupId = 1;
    int32_t ret = audioResourceService.ReleaseAudioWorkgroup(pid, workgroupId);
    EXPECT_NE(ret, SUCCESS);
}

HWTEST(AudioResourceServiceUnitTest, AudioWorkgroupCheck_003, TestSize.Level0)
{
    int32_t pid = 1234;
    for (int i = 0; i < AUDIO_MAX_PROCESS; i++) {
        audioResourceService.audioWorkgroupMap_[AUDIO_MAX_PROCESS].hasSystemPermission = false;
    }
    EXPECT_NE(audioResourceService.AudioWorkgroupCheck(pid), ERR_NOT_SUPPORTED);
}

HWTEST(AudioResourceServiceUnitTest, AudioWorkgroupCheck_004, TestSize.Level0)
{
    int32_t pid = 1234;
    EXPECT_EQ(audioResourceService.AudioWorkgroupCheck(pid), SUCCESS);
}








