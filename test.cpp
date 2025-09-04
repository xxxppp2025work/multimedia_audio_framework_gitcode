#include <fstream>
#include "parameters.h"

constexpr const char* RECLAIM_MEMORY = "AudioReclaimMemory";
constexpr uint32_t TIME_OF_RECLAIM_MEMORY = 240000; //4min
constexpr const char* RECLAIM_FILE_STRING = "1";

    PostReclaimMemoryTask();

void AudioStreamCollector::PostReclaimMemoryTask()
{
    if (system::GetParameter("persist.ace.testmode.enabled", "0") != "1" ||
        audioPolicyServerHandler_ == nullptr) {
        AUDIO_INFO_LOG("wyt11--------------------------non test scenario-------------------------");
        return;
    }
    if (!taskNotStarted_.load() && !CheckAudioStateIdle()) {
        AUDIO_INFO_LOG("wyt11--------------------------clear timer-------------------------");
        audioPolicyServerHandler_->RemoveTask(RECLAIM_MEMORY);
        taskNotStarted_.store(true);
        return;
    }
    if (taskNotStarted_.load() && CheckAudioStateIdle()) {
        AUDIO_INFO_LOG("wyt11--------------------------start timer-------------------------");
        auto task = [this]() {
            ReclaimMem();
            taskNotStarted_.store(true);
        };
        audioPolicyServerHandler_->PostTask(task, RECLAIM_MEMORY, TIME_OF_RECLAIM_MEMORY);
        taskNotStarted_.store(false);
    }
}
 
void AudioStreamCollector::ReclaimMem()
{
    std::lock_guard<std::mutex> lock(clearMemoryMutex_);
    std::string reclaimPath = "/proc/" + std::to_string(getpid()) + "/reclaim";
    std::string reclaimContent = RECLAIM_FILE_STRING;
    AUDIO_INFO_LOG("wyt11-----------------Start Reclaim File = %{public}s", reclaimPath.c_str());
    std::ofstream outfile(reclaimPath);
    if (outfile.is_open()) {
        outfile << reclaimContent;
        outfile.close();
    } else {
        AUDIO_ERR_LOG("reclaim cannot open file");
    }
}
 
bool AudioStreamCollector::CheckAudioStateIdle()
{
    if (audioRendererChangeInfos_.empty() && audioCapturerChangeInfos_.empty()) {
        AUDIO_INFO_LOG("wyt11--------------------------ALL is null-------------------------");
        return true;
    }
    if (!audioRendererChangeInfos_.empty()) {
        for (auto rendererInfo : audioRendererChangeInfos_) {
            if (rendererInfo->rendererState == RENDERER_RUNNING) {
                AUDIO_INFO_LOG("wyt11--------------------------rendererInfo is RUNNING-------------------------");
                return false;
            }
        }
    }
    if (!audioCapturerChangeInfos_.empty()) {
        for (auto capturerInfo : audioCapturerChangeInfos_) {
            if (capturerInfo->capturerState == CAPTURER_RUNNING) {
                AUDIO_INFO_LOG("wyt11--------------------------capturerInfo is RUNNING-------------------------");
                return false;
            }
        }
    }
    AUDIO_INFO_LOG("wyt11--------------------------ALL is idle-------------------------");
    return true;
}
 

    void PostReclaimMemoryTask();
    void ReclaimMem();
    bool CheckAudioStateIdle();
    std::atomic_bool taskNotStarted_ = true;
    std::mutex clearMemoryMutex_;

#include "parameters.h"

 
/**
* @tc.name  : Test PostReclaimMemoryTask.
* @tc.number: PostReclaimMemoryTask_001
* @tc.desc  : Test PostReclaimMemoryTask.
*/
HWTEST_F(AudioStreamCollectorUnitTest, PostReclaimMemoryTask_001, TestSize.Level1)
{
    AudioStreamCollector collector;
    collector.audioRendererChangeInfos_.clear();
    collector.audioCapturerChangeInfos_.clear();
    collector.audioPolicyServerHandler_ = DelayedSingleton<AudioPolicyServerHandler>::GetInstance();
    collector.PostReclaimMemoryTask();
    std::string retString = system::GetParameter("persist.ace.testmode.enabled", "0");
    system::SetParameter("persist.ace.testmode.enabled", "1");
    collector.PostReclaimMemoryTask();
    collector.ReclaimMem();
    collector.taskNotStarted_ = false;
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    EXPECT_NE(rendererChangeInfo, nullptr);
    rendererChangeInfo->rendererState = RENDERER_RUNNING;
    collector.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));
    collector.PostReclaimMemoryTask();
    system::SetParameter("persist.ace.testmode.enabled", retString);
    EXPECT_NE("1", system::GetParameter("persist.ace.testmode.enabled", "0"));
}
 
/**
* @tc.name  : Test CheckAudioStateIdle.
* @tc.number: CheckAudioStateIdle_001
* @tc.desc  : Test CheckAudioStateIdle.
*/
HWTEST_F(AudioStreamCollectorUnitTest, CheckAudioStateIdle_001, TestSize.Level1)
{
    AudioStreamCollector collector;
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    EXPECT_NE(rendererChangeInfo, nullptr);
    rendererChangeInfo->rendererState = RENDERER_RUNNING;
    collector.audioRendererChangeInfos_.push_back(rendererChangeInfo);
    EXPECT_EQ(false, collector.CheckAudioStateIdle());
 
    collector.audioRendererChangeInfos_.clear();
    shared_ptr<AudioCapturerChangeInfo> capturerChangeInfo = make_shared<AudioCapturerChangeInfo>();
    EXPECT_NE(capturerChangeInfo, nullptr);
    capturerChangeInfo->capturerState = CAPTURER_RUNNING;
    collector.audioCapturerChangeInfos_.push_back(capturerChangeInfo);
    EXPECT_EQ(false, collector.CheckAudioStateIdle());
 
    collector.audioCapturerChangeInfos_.clear();
    rendererChangeInfo->rendererState = RENDERER_STOPPED;
    capturerChangeInfo->capturerState = CAPTURER_STOPPED;
    collector.audioRendererChangeInfos_.push_back(rendererChangeInfo);
    collector.audioCapturerChangeInfos_.push_back(capturerChangeInfo);
    EXPECT_EQ(true, collector.CheckAudioStateIdle());
}


void AudioStreamCollectorPostReclaimMemoryTaskFuzzTest(const uint8_t *rawData, size_t size)
{
    uint32_t index = static_cast<uint32_t>(size);
    AudioStreamType volumeType = g_testAudioStreamTypes[index % g_testAudioStreamTypes.size()];
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    int32_t randIntValue = static_cast<int32_t>(size);
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    rendererChangeInfo->rendererState = g_testRendererState[index % g_testRendererState.size()];
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));
    audioStreamCollector_.PostReclaimMemoryTask();
    audioStreamCollector_.ReclaimMem();
}
 
void AudioStreamCollectorCheckAudioStateIdleFuzzTest(const uint8_t *rawData, size_t size)
{
    uint32_t index = static_cast<uint32_t>(size);
    AudioStreamType volumeType = g_testAudioStreamTypes[index % g_testAudioStreamTypes.size()];
    shared_ptr<AudioRendererChangeInfo> rendererChangeInfo = make_shared<AudioRendererChangeInfo>();
    int32_t randIntValue = static_cast<int32_t>(size);
    rendererChangeInfo->createrUID = randIntValue / NUM_2;
    rendererChangeInfo->clientUID = randIntValue / NUM_2;
    rendererChangeInfo->sessionId = randIntValue;
    rendererChangeInfo->rendererState = g_testRendererState[index % g_testRendererState.size()];
    audioStreamCollector_.audioRendererChangeInfos_.clear();
    audioStreamCollector_.audioRendererChangeInfos_.push_back(move(rendererChangeInfo));
    audioStreamCollector_.CheckAudioStateIdle();
}


    OHOS::AudioStandard::AudioStreamCollectorPostReclaimMemoryTaskFuzzTest,
    OHOS::AudioStandard::AudioStreamCollectorCheckAudioStateIdleFuzzTest,

