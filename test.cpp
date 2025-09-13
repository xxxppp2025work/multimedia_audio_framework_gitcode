111
/**
 * @tc.name  : Test AudioInterruptService
 * @tc.number: AudioInterruptService_071
 * @tc.desc  : Test ShouldCallbackToClient
*/
HWTEST(AudioInterruptServiceSecondUnitTest, AudioInterruptService_071, TestSize.Level1)
{
    auto audioInterruptService = std::make_shared<AudioInterruptService>();
    ASSERT_NE(audioInterruptService, nullptr);

    uint32_t uid = 0;
    int32_t streamId = 0;
    InterruptEventInternal interruptEvent;
    interruptEvent.hintType = INTERRUPT_HINT_RESUME;

    uint32_t uid2 = 0;
    ClientType clientType = CLIENT_TYPE_GAME;
    ClientTypeManager::GetInstance()->OnClientTypeQueryCompleted(uid2, clientType);
    audioInterruptService->policyServer_ = new AudioPolicyServer(0);
    ASSERT_NE(audioInterruptService->policyServer_, nullptr);
    auto ret = audioInterruptService->ShouldCallbackToClient(uid, streamId, interruptEvent);
    EXPECT_EQ(ret, false);
    interruptEvent.hintType = INTERRUPT_HINT_PAUSE;
    ret = audioInterruptService->ShouldCallbackToClient(uid, streamId, interruptEvent);
    EXPECT_EQ(ret, false);
}
/**
 * @tc.name  : Test AudioInterruptService
 * @tc.number: AudioInterruptService_072
 * @tc.desc  : Test AudioInterruptIsActiveInFocusList
*/
HWTEST(AudioInterruptServiceSecondUnitTest, AudioInterruptService_072, TestSize.Level1)
{
    auto audioInterruptService = std::make_shared<AudioInterruptService>();
    ASSERT_NE(audioInterruptService, nullptr);

    uint32_t streamId = 1;
    int32_t zoneId = 0;
    audioInterruptService->mutedGameSessionId_.insert(streamId);
    auto ret = audioInterruptService->AudioInterruptIsActiveInFocusList(zoneId, streamId);
    EXPECT_EQ(ret, true);
}