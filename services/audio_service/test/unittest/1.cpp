HWTEST(FastSystemStreamUnitTest, RestoreAudioStream_007, TestSize.Level1)
{
    int32_t appUid = static_cast<int32_t>(getuid());
    std::shared_ptr<FastAudioStream> fastAudioStream =
        std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK, appUid);
    EXPECT_EQ(fastAudioStream->RestoreAudioStream(true), false);
}

HWTEST(FastSystemStreamUnitTest, RestoreAudioStream_008, TestSize.Level1)
{
    int32_t appUid = static_cast<int32_t>(getuid());
    std::shared_ptr<FastAudioStream> fastAudioStream =
        std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK, appUid);
    fastAudioStream->state_ = NEW;
    EXPECT_EQ(fastAudioStream->RestoreAudioStream(true), true);
}

HWTEST(FastSystemStreamUnitTest, RestoreAudioStream_009, TestSize.Level1)
{
    int32_t appUid = static_cast<int32_t>(getuid());
    std::shared_ptr<FastAudioStream> fastAudioStream =
        std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK, appUid);
    fastAudioStream->state_ = RUNNING;
    EXPECT_EQ(fastAudioStream->RestoreAudioStream(true), false);
}

HWTEST(FastSystemStreamUnitTest, RestoreAudioStream_010, TestSize.Level1)
{
    int32_t appUid = static_cast<int32_t>(getuid());
    std::shared_ptr<FastAudioStream> fastAudioStream =
        std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK, appUid);
    fastAudioStream->state_ = PAUSED;
    EXPECT_EQ(fastAudioStream->RestoreAudioStream(true), false);
}

HWTEST(FastSystemStreamUnitTest, RestoreAudioStream_011, TestSize.Level1)
{
    int32_t appUid = static_cast<int32_t>(getuid());
    std::shared_ptr<FastAudioStream> fastAudioStream =
        std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK, appUid);
    fastAudioStream->state_ = STOPPED;
    EXPECT_EQ(fastAudioStream->RestoreAudioStream(true), false);
}

HWTEST(FastSystemStreamUnitTest, RestoreAudioStream_012, TestSize.Level1)
{
    int32_t appUid = static_cast<int32_t>(getuid());
    std::shared_ptr<FastAudioStream> fastAudioStream =
        std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK, appUid);
    fastAudioStream->state_ = STOPPING;
    EXPECT_EQ(fastAudioStream->RestoreAudioStream(true), false);
}

HWTEST(FastSystemStreamUnitTest, GetDefaultOutputDevice_001, TestSize.Level1)
{
    int32_t appUid = static_cast<int32_t>(getuid());
    std::shared_ptr<FastAudioStream> fastAudioStream =
        std::make_shared<FastAudioStream>(STREAM_MUSIC, AUDIO_MODE_PLAYBACK, appUid);
    fastAudioStream->defaultOutputDevice_ = DeviceType::DEVICE_TYPE_SPEAKER;

    DeviceType result = fastAudioStream->GetDefaultOutputDevice();

    EXPECT_EQ(result, DeviceType::DEVICE_TYPE_SPEAKER);
}