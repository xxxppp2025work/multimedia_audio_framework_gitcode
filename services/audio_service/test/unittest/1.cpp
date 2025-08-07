audio_resource_service_unit_test.cpp


static constexpr int32_t AUDIO_MAX_PROCESS = 2;

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

audio_service_unit_test.cpp

HWTEST(AudioServiceUnitTest, ConfigCoreServiceProvider_002, TestSize.Level1)
{
    auto coreServiceHandler = CoreServiceHandler::GetInstance();
    auto samgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_NE(nullptr, samgr);
    sptr<IRemoteObject> object = samgr->GetSystemAbility(AUDIO_DISTRIBUTED_SERVICE_ID);
    sptr<ICoreServiceProviderIpc> coreServiceProvider = iface_cast<ICoreServiceProviderIpc>(object);
    int32_t ret = coreServiceHandler.ConfigCoreServiceProvider(coreServiceProvider);
    EXPECT_EQ(ret, SUCCESS);
}

HWTEST(AudioServiceUnitTest, ConfigCoreServiceProvider_003, TestSize.Level1)
{
    auto coreServiceHandler = CoreServiceHandler::GetInstance();
    auto samgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_NE(nullptr, samgr);
    sptr<IRemoteObject> object = samgr->GetSystemAbility(AUDIO_DISTRIBUTED_SERVICE_ID);
    sptr<ICoreServiceProviderIpc> coreServiceProvider = iface_cast<ICoreServiceProviderIpc>(object);
    coreServiceHandler.ConfigCoreServiceProvider(coreServiceProvider); // Set the provider
    int32_t ret = coreServiceHandler.ConfigCoreServiceProvider(coreServiceProvider);
    EXPECT_EQ(ret, ERR_INVALID_OPERATION);
}






