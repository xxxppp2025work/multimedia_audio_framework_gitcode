#include "audio_core_service.h"
namespace OHOS {
namespace AudioStandard {
namespace {
    static const int32_t MEDIA_SERVICE_UID = 1013;
}

AudioCoreService::AudioCoreService()
{
    AUDIO_INFO_LOG("Ctor")
    audioActiveDevice_ = AudioActiveDevice::GetInstance();
    audioSceneManager_ = AudioSceneManager::GetInstance();
    audioVolumeManager_ = AudioVolumeManager::GetInstance();
    audioCapturerSession_ = AudioCapturerSession::GetInstance();
    audioDeviceManager_ = AudioDeviceManager::GetAudioDeviceManager();
    audioConnectedDevice_ = AudioConnectedDevice::GetInstance();
}

AudioCoreService::~AudioCoreService()
{
    AUDIO_INFO_LOG("Dtor");
}

void AudioCoreService::SetCallbackHandler(std::shared_ptr<AudioPolicyServerHandler> handler)
{
    audioPolicyServerHandler_ = handler;
}

std::shared_ptr<EventEntry> AudioCoreService::GetEventEntry()
{
    return eventEntry_;
}

void AudioCoreService::RegiestCoreService()
{
    AUDIO_INFO_LOG("In");
    sptr<CoreServiceProviderWrapper> wrapper = new(std::nothrow) CoreServiceProviderWrapper(eventEntry_.get()); // shared_ptr.get() -> *
    CHECK_AND_RETURN_LOG(wrapper != nullptr, "Get null CoreServiceProviderWrapper");
    sptr<IRemoteObject> object = wrapper->AsObject();
    CHECK_AND_RETURN_LOG(object != nullptr, "AsObject is nullptr");

    int32_t ret = AudioServerProxy::GetInstance().RegiestCoreServiceProviderProxy(object);
    AUDIO_INFO_LOG("Result:%{public}d", ret);
}

int32_t AudioCoreService::CreateClient(AudioStreamDescriptor &streamDesc, AudioFlag &audioFlag)
{
    // Select device
    std::shared_ptr<AudioDeviceDescriptor> desc;
    AudioFlag audioFlag;
    if (streamDesc.audioMode == AUDIO_MODE_PLAYBACK) {
        desc = FetchOutputDevices(streamDesc);
    } else {
        desc = audioRouterCenter_.FetchInputDevice(streamDesc.capturerInfo.sourceType, GetRealUid(streamDesc));
        audioFlag = streamDesc.capturerInfo.capturerFlags;
    }



    // Fetch pipe
    int32_t ret = FetchPipeAndExecute(streamDesc, audioFlag);
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ret, "FetchPipeAndExecute failed");

}

std::shared_ptr<AudioDeviceDescriptor> AudioCoreService::FetchOutputDevices(AudioStreamDescriptor &streamDesc)
{
    vector<std::shared_ptr<AudioDeviceDescriptor>> descs = audioRouterCenter_.FetchOutputDevices(streamDesc.rendererInfo.streamUsage, GetRealUid(streamDesc));
    std::shared_ptr<AudioDeviceDescriptor> desc = descs[0];

    // 选flag


    // move的时候做, start 的时候move
    if (desc->deviceType_ == DEVICE_TYPE_BLUETOOTH_A2DP) {
        int32_t ret = ActivateA2dpDeviceWhenDescEnabled(desc, rendererChangeInfos, reason);
        CHECK_AND_RETURN_LOG(ret == SUCCESS, "activate a2dp [%{public}s] failed", encryptMacAddr.c_str());
    } else if (desc->deviceType_ == DEVICE_TYPE_BLUETOOTH_SCO) {
        int32_t ret = HandleScoOutputDeviceFetched(desc, rendererChangeInfos, reason);
        CHECK_AND_RETURN_LOG(ret == SUCCESS, "sco [%{public}s] is not connected yet", encryptMacAddr.c_str());
    } else if (desc->deviceType_ == DEVICE_TYPE_USB_ARM_HEADSET) {
        audioEcManager_.ActivateArmDevice(desc->macAddress_, desc->deviceRole_);
    }

}

int32_t AudioCoreService::StartClient(uint32_t sessionId)
{

    // In plan: Move sink by index


    audioActiveDevice_.UpdateActiveDeviceRoute(pipeInfo.deviceType, pipeInfo.deviceFlag);

    ret = pipeManager_->StartClient(sessionId);
    return ret;
}

int32_t AudioCoreService::RemoveClient(uint32_t sessionId)
{
    int32_t DestoryRender();

    ret = pipeManager_->RemoveClient(sessionId);
    return ret;
}

int32_t AudioCoreService::SetAudioScene(AudioScene audioScene)
{
    audioSceneManager_.SetAudioScenePre(audioScene);

    // Fetch input & output device
    std::vector<std:shared_ptr<AudioStreamDesc>> outputStreamDescs = pipeManager_->GetAllOutputStreamDescs();
    FetchOutputDevices(outputStreamDescs, AudioStreamDeviceChangeReasonExt::ExtEnum::SET_AUDIO_SCENE);

    std::vector<std:shared_ptr<AudioStreamDesc>> inputStreamDescs = pipeManager_->GetAllInputStreamDescs();
    FetchInputDevices(inputStreamDescs, AudioStreamDeviceChangeReasonExt::ExtEnum::SET_AUDIO_SCENE);

    int32_t result = audioSceneManager_.SetAudioSceneAfter(audioScene, audioA2dpOffloadFlag_.GetA2dpOffloadFlag());
    CHECK_AND_RETURN_RET_LOG(result == SUCCESS, ERR_OPERATION_FAILED, "failed [%{public}d]", result);

    if (audioScene == AUDIO_SCENE_PHONE_CALL) {
        // Make sure the STREAM_VOICE_CALL volume is set before the calling starts.
        audioVolumeManager_.SetVoiceCallVolume(audioVolumeManager_.GetSystemVolumeLevel(STREAM_VOICE_CALL));
    } else {
        audioVolumeManager_.SetVoiceRingtoneMute(false);
    }
    audioCapturerSession_.ReloadSourceForDeviceChange(audioActiveDevice_.GetCurrentInputDevice(),
        audioActiveDevice_.GetCurrentOutputDevice(), "SetAudioScene");
    return SUCCESS;
}

bool AudioCoreService::IsArmUsbDevice(const AudioDeviceDescriptor &deviceDesc)
{
    return audioDeviceManager_.IsArmUsbDevice(desc);
}

std::vector<std::shared_ptr<AudioDeviceDescriptor>> AudioCoreService::GetDevices(DeviceFlag deviceFlag)
{
    return audioConnectedDevice_.GetDevicesInner(deviceFlag);
}

int32_t AudioCoreService::SetDeviceActive(InternalDeviceType deviceType, bool active)
{
    int32_t ret = audioActiveDevice_.SetDeviceActive(deviceType, active);
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ret, "SetDeviceActive failed");

    // Fetch input & output device
    std::vector<std:shared_ptr<AudioStreamDesc>> outputStreamDescs = pipeManager_->GetAllOutputStreamDescs();
    FetchOutputDevices(outputStreamDescs, AudioStreamDeviceChangeReasonExt::ExtEnum::SET_AUDIO_SCENE);

    std::vector<std:shared_ptr<AudioStreamDesc>> inputStreamDescs = pipeManager_->GetAllInputStreamDescs();
    FetchInputDevices(inputStreamDescs, AudioStreamDeviceChangeReasonExt::ExtEnum::SET_AUDIO_SCENE);

    audioCapturerSession_.ReloadSourceForDeviceChange(audioActiveDevice_.GetCurrentInputDevice(),
        audioActiveDevice_.GetCurrentOutputDevice(), "SetDevcieActive");
    return SUCCESS;
}





int32_t AudioCoreService::FetchOutputDevices(std::vector<AudioStreamDesc> &outputStreamDescs, const AudioStreamDeviceChangeReasonExt reason)
{
    for (std:shared_ptr<AudioStreamDesc> &streamDesc : outputStreamDescs) {
        vector<std::shared_ptr<AudioDeviceDescriptor>> descs = audioRouterCenter_.FetchOutputDevices(streamDesc.rendererInfo.streamUsage, GetRealUid(streamDesc));
        std::shared_ptr<AudioDeviceDescriptor> desc = descs[0];
        streamDesc->deviceDesc_ = desc;
        streamDesc->SetFlag(audioFlag);
    }

    FetchPipesAndExecute(outputStreamDescs, reason);

}

int32_t AudioCoreService::FetchPipesAndExecute(std::vector<AudioStreamDesc> &outputStreamDescs, const AudioStreamDeviceChangeReasonExt reason)
{
    // GetPipeInfosBystreamDesc std::vector<PipeInfo> pipeInfos
    for (auto &pipeInfo : pipeInfos) {
        if (pipeInfo.action == 1) { // new
            OpenNewAudioPortAndRoute(pipeInfo);
            // streamClass =  GetStreamClass()
        } else if (pipeInfo.action == 2) { // recreate
            TriggerRecreateRendererStreamCallback(pipeInfo.callerPid,
                pipeInfo.sessionId, pipeInfo.streamClass);
        } else if (pipeInfo.action == 0) { // default
            // do nothing
        }
    }
}

int32_t AudioCoreService::FetchPipeAndExecute(std::shared_ptr<AudioStreamDescriptor> streamDesc, AudioFlag &audioFlag)
{
    // In plan
    // GetPipeInfosBystreamDesc std::vector<PipeInfo> pipeInfos

    for (auto &pipeInfo : pipeInfos) {
        if (pipeInfo.action == 1) { // new
            OpenNewAudioPortAndRoute(pipeInfo);
            // streamClass =  GetStreamClass()
        } else if (pipeInfo.action == 2) { // recreate
            TriggerRecreateRendererStreamCallback(pipeInfo.callerPid,
                pipeInfo.sessionId, pipeInfo.streamClass);
        } else if (pipeInfo.action == 0) { // default
            // do nothing
        }
    }

    pipeManager_->RemoveUnusedPipe();
}

void AudioCoreService::TriggerRecreateRendererStreamCallback(int32_t callerPid, int32_t sessionId, int32_t streamFlag)
{
    Trace trace("AudioCoreService::TriggerRecreateRendererStreamCallback");
    AUDIO_INFO_LOG("Trigger recreate renderer stream, pid: %{public}d, sessionId: %{public}d, flag: %{public}d",
        callerPid, sessionId, streamFlag);
    if (audioPolicyServerHandler_ != nullptr) {
        audioPolicyServerHandler_->SendRecreateRendererStreamEvent(callerPid, sessionId, streamFlag);
    } else {
        AUDIO_WARNING_LOG("No audio policy server handler");
    }
}


int32_t AudioCoreService::OpenNewAudioPortAndRoute(PipeInfo &pipeInfo)
{

    CreateRender(); // In plan: 打开通路，这里需要调用罗琪接口？

    pipeManager_->AddPipe(pipeInfo);
}

static int32_t AudioCoreService::GetRealUid(AudioStreamDescriptor &streamDesc)
{
    if (callerUid == MEDIA_SERVICE_UID) {
        return streamDesc.appInfo.appUid;
    }
    return streamDesc.callerUid;
}



// 内部类的实现
AudioCoreService::EventEntry::EventEntry(std::shared_ptr<AudioCoreService> coreService) : coreService_(coreService) {}

int32_t AudioCoreService::EventEntry::CreateClient(AudioStreamDescriptor &streamDesc, AudioFlag &audioFlag)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_->CreateClient(streamDesc, audioFlag);
}

int32_t AudioCoreService::EventEntry::StartClient(uint32_t sessionId)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_->StartClient(sessionId);
}

int32_t AudioCoreService::EventEntry::RemoveClient(uint32_t sessionId)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_->RemoveClient(sessionId);
}

int32_t AudioCoreService::EventEntry::SetAudioScene(AudioScene audioScene)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_->SetAudioScene(audioScene);
}

bool AudioCoreService::EventEntry::IsArmUsbDevice(const AudioDeviceDescriptor &deviceDesc)
{
    return coreService_->IsArmUsbDevice(deviceDesc);
}

std::vector<std::shared_ptr<AudioDeviceDescriptor>> AudioCoreService::EventEntry::GetDevices(DeviceFlag deviceFlag)
{
    return coreService_->GetDevices(deviceFlag);
}

int32_t AudioCoreService::EventEntry::SetDeviceActive(InternalDeviceType deviceType, bool active)
{
    return coreService_->SetDeviceActive(deviceType, active);
}
} // namespace AudioStandard
} // namespace OHOS
