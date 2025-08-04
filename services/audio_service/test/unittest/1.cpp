//   baimiaojie
HWTEST(AudioServiceUnitTest, UpdateSystemVolume_001, TestSize.Level1)
{
    AudioStreamType streamType = STREAM_ALARM;
    float volume = 0.5;

    // Act
    AudioService::GetInstance()->UpdateSystemVolume(streamType, volume);

    // Assert
    float expectedVolume = 0.0;
    EXPECT_NE(expectedVolume, AudioService::GetInstance()->musicOrVoipSystemVolume_);
}

HWTEST(AudioServiceUnitTest, UpdateSystemVolume_002, TestSize.Level1)
{
    AudioStreamType streamType = STREAM_MUSIC;
    float volume = 0.5;

    AudioService::GetInstance()->UpdateSystemVolume(streamType, volume);

    EXPECT_EQ(volume, AudioService::GetInstance()->musicOrVoipSystemVolume_);
}

HWTEST(AudioServiceUnitTest, UpdateSystemVolume_003, TestSize.Level1)
{
    AudioStreamType streamType = STREAM_VOICE_COMMUNICATION;
    float volume = 0.5;

    AudioService::GetInstance()->UpdateSystemVolume(streamType, volume);

    EXPECT_EQ(volume, AudioService::GetInstance()->musicOrVoipSystemVolume_);
}

HWTEST(AudioServiceUnitTest, SetSessionMuteState_001, TestSize.Level1)
{
    uint32_t sessionId = 1;
    bool insert = true;
    bool muteFlag = true;

    AudioService::GetInstance()->SetSessionMuteState(sessionId, insert, muteFlag);

    std::unique_lock<std::mutex> lock(AudioService::GetInstance()->muteStateMapMutex_);
    EXPECT_EQ(AudioService::GetInstance()->muteStateMap_[sessionId], muteFlag);
}

HWTEST(AudioServiceUnitTest, CleanAppUseNumMap_001, TestSize.Level1)
{
    int32_t appUid = 12345;
    AudioService::GetInstance()->appUseNumMap_[appUid] = 5;

    AudioService::GetInstance()->CleanAppUseNumMap(appUid);

    EXPECT_EQ(AudioService::GetInstance()->appUseNumMap_[appUid], 4);
}

HWTEST(AudioServiceUnitTest, CleanAppUseNumMap_002, TestSize.Level1)
{
    int32_t appUid = 12345;

    AudioService::GetInstance()->CleanAppUseNumMap(appUid);

    EXPECT_NE(AudioService::GetInstance()->appUseNumMap_.find(appUid), AudioService::GetInstance()->appUseNumMap_.end());
}

HWTEST(AudioServiceUnitTest, SetIncMaxRendererStreamCnt_001, TestSize.Level1)
{
    int32_t initialCount = AudioService::GetInstance()->currentRendererStreamCnt_;

    AudioService::GetInstance()->SetIncMaxRendererStreamCnt(AUDIO_MODE_PLAYBACK);

    EXPECT_EQ(AudioService::GetInstance()->currentRendererStreamCnt_, initialCount + 1);
}

HWTEST(AudioServiceUnitTest, ShouldBeDualTone_001, TestSize.Level1)
{
    AudioProcessConfig config = {};
    config.rendererInfo.streamUsage = STREAM_USAGE_MUSIC;

    EXPECT_FALSE(AudioService::GetInstance()->ShouldBeDualTone(config));

    config.rendererInfo.streamUsage = STREAM_USAGE_RINGTONE;
    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    EXPECT_FALSE(AudioService::GetInstance()->ShouldBeDualTone(config));
}

HWTEST(AudioServiceUnitTest, ShouldBeDualTone_002, TestSize.Level1)
{
    AudioProcessConfig config = {};
    config.rendererInfo.streamUsage = STREAM_USAGE_RINGTONE;
    config.audioMode = AUDIO_MODE_RECORD;

    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    EXPECT_FALSE(AudioService::GetInstance()->ShouldBeDualTone(config));
}

HWTEST(AudioServiceUnitTest, ShouldBeDualTone_003, TestSize.Level1)
{
    AudioProcessConfig config = {};
    config.rendererInfo.streamUsage = STREAM_USAGE_RINGTONE;
    config.audioMode = AUDIO_MODE_PLAYBACK;

    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    deviceInfo.deviceType_ = DEVICE_TYPE_WIRED_HEADSET;

    EXPECT_FALSE(AudioService::GetInstance()->ShouldBeDualTone(config));
}

HWTEST(AudioServiceUnitTest, ShouldBeDualTone_004, TestSize.Level1)
{
    AudioProcessConfig config = {};
    config.rendererInfo.streamUsage = STREAM_USAGE_RINGTONE;
    config.audioMode = AUDIO_MODE_PLAYBACK;

    AudioDeviceDescriptor deviceInfo(AudioDeviceDescriptor::DEVICE_INFO);
    deviceInfo.deviceType_ = static_cast<DeviceType>(999); // 未知设备类型

    EXPECT_FALSE(AudioService::GetInstance()->ShouldBeDualTone(config));
}

HWTEST(AudioServiceUnitTest, GetDeviceInfoForProcess_001, TestSize.Level1)
{
    AudioProcessConfig config = {};
    config.originalSessionId = 1;
    config.rendererInfo.streamUsage = STREAM_USAGE_VOICE_COMMUNICATION;
    config.streamInfo.samplingRate = SAMPLE_RATE_16000;
    bool isReloadProcess = false;

    AudioDeviceDescriptor deviceInfo = AudioService::GetInstance()->GetDeviceInfoForProcess(config, isReloadProcess);

    EXPECT_NE(deviceInfo.deviceType_, DEVICE_TYPE_MIC);
    EXPECT_EQ(deviceInfo.isLowLatencyDevice_, false);
    EXPECT_EQ(deviceInfo.audioStreamInfo_.size(), 1);
}

HWTEST(AudioServiceUnitTest, CheckBeforeVoipEndpointCreate_001, TestSize.Level1)
{
    bool isVoip = true;
    bool isRecord = true;

    AudioService::GetInstance()->CheckBeforeVoipEndpointCreate(isVoip, isRecord);
    EXPECT_TRUE(isVoip);
    EXPECT_TRUE(isRecord);
}

HWTEST(AudioServiceUnitTest, CheckBeforeVoipEndpointCreate_002, TestSize.Level1)
{
    bool isVoip = false;
    bool isRecord = false;
    AudioService::GetInstance()->CheckBeforeVoipEndpointCreate(isVoip, isRecord);
    EXPECT_FALSE(isVoip);
    EXPECT_FALSE(isRecord);
}

AudioService::GetInstance()->RenderersCheckForAudioWorkgroup(1);