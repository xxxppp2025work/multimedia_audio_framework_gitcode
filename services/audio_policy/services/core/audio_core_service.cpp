#include "audio_core_service.h"
namespace OHOS {
namespace AudioStandard {
namespace {
    static const int32_t MEDIA_SERVICE_UID = 1013;
}

bool AudioCoreService::isBtListenerRegistered = false;

AudioCoreService::AudioCoreService()
{
    AUDIO_INFO_LOG("Ctor")
    audioActiveDevice_ = AudioActiveDevice::GetInstance();
    audioSceneManager_ = AudioSceneManager::GetInstance();
    audioVolumeManager_ = AudioVolumeManager::GetInstance();
    audioCapturerSession_ = AudioCapturerSession::GetInstance();
    audioDeviceManager_ = AudioDeviceManager::GetAudioDeviceManager();
    audioConnectedDevice_ = AudioConnectedDevice::GetInstance();
    audioDeviceStatus_ = AudioDeviceStatus::GetInstance();
    audioConfigManager_ = AudioConfigManager::GetInstance();
    audioEffectService_ = AudioEffectService::GetAudioEffectService();
    audioMicrophoneDescriptor_ = AudioMicrophoneDescriptor::GetInstance();

}

AudioCoreService::~AudioCoreService()
{
    AUDIO_INFO_LOG("Dtor");
}

AudioCoreService::Init()
{
    eventEntry_ = std::make_shared<EventEntry>(shared_from_this());

    audioConfigManager.ConfigCoreService(shared_from_this());

    deviceStatusListener_ = std::make_shared<DeviceStatusListener>(eventEntry_.get()); // shared_ptr.get() -> *

    // Register device status listener
    int32_t status = deviceStatusListener_->RegisterDeviceStatusListener();
    if (status != SUCCESS) {
        AudioPolicyUtils::GetInstance().WriteServiceStartupError("[Core Service] Register for device status "
            "events failed");
        AUDIO_ERR_LOG("Register for device status events failed")
    }
}

AudioCoreService::DeInit()
{
    // Remove device status listener
    deviceStatusListener_->UnRegisterDeviceStatusListener();

    if (isBtListenerRegistered) {
        UnregisterBluetoothListener();
    }
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

    SelectDeviceAndRoute(SET_AUDIO_SCENE);

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

    SelectDeviceAndRoute(AudioStreamDeviceChangeReasonExt::ExtEnum::SET_AUDIO_SCENE);

    audioCapturerSession_.ReloadSourceForDeviceChange(audioActiveDevice_.GetCurrentInputDevice(),
        audioActiveDevice_.GetCurrentOutputDevice(), "SetDevcieActive");
    return SUCCESS;
}

std::vector<std::shared_ptr<AudioDeviceDescriptor>> AudioCoreService::GetPreferredOutputDeviceDescInner(
    AudioRendererInfo &rendererInfo, std::string networkId)
{
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> deviceList = {};
    if (rendererInfo.streamUsage <= STREAM_USAGE_UNKNOWN ||
        rendererInfo.streamUsage > STREAM_USAGE_MAX) {
        AUDIO_WARNING_LOG("Invalid usage[%{public}d], return current device.", rendererInfo.streamUsage);
        std::shared_ptr<AudioDeviceDescriptor> devDesc =
            std::make_shared<AudioDeviceDescriptor>(audioActiveDevice_.GetCurrentOutputDevice());
        deviceList.push_back(devDesc);
        return deviceList;
    }
    if (networkId == LOCAL_NETWORK_ID) {
        vector<std::shared_ptr<AudioDeviceDescriptor>> descs =
            audioRouterCenter_.FetchOutputDevices(rendererInfo.streamUsage, -1);
        for (size_t i = 0; i < descs.size(); i++) {
            std::shared_ptr<AudioDeviceDescriptor> devDesc = std::make_shared<AudioDeviceDescriptor>(*descs[i]);
            deviceList.push_back(devDesc);
        }
    } else {
        vector<shared_ptr<AudioDeviceDescriptor>> descs = audioDeviceManager_.GetRemoteRenderDevices();
        for (const auto &desc : descs) {
            std::shared_ptr<AudioDeviceDescriptor> devDesc = std::make_shared<AudioDeviceDescriptor>(*desc);
            deviceList.push_back(devDesc);
        }
    }

    return deviceList;
}

std::vector<std::shared_ptr<AudioDeviceDescriptor>> AudioCoreService::GetPreferredInputDeviceDescInner(
    AudioCapturerInfo &captureInfo, std::string networkId)
{
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> deviceList = {};
    if (captureInfo.sourceType <= SOURCE_TYPE_INVALID ||
        captureInfo.sourceType > SOURCE_TYPE_MAX) {
        std::shared_ptr<AudioDeviceDescriptor> devDesc =
            std::make_shared<AudioDeviceDescriptor>(audioActiveDevice_.GetCurrentInputDevice());
        deviceList.push_back(devDesc);
        return deviceList;
    }

    if (captureInfo.sourceType == SOURCE_TYPE_WAKEUP) {
        std::shared_ptr<AudioDeviceDescriptor> devDesc =
            std::make_shared<AudioDeviceDescriptor>(DEVICE_TYPE_MIC, INPUT_DEVICE);
        devDesc->networkId_ = LOCAL_NETWORK_ID;
        deviceList.push_back(devDesc);
        return deviceList;
    }

    if (networkId == LOCAL_NETWORK_ID) {
        std::shared_ptr<AudioDeviceDescriptor> desc = audioRouterCenter_.FetchInputDevice(captureInfo.sourceType, -1);
        if (desc->deviceType_ == DEVICE_TYPE_NONE && (captureInfo.sourceType == SOURCE_TYPE_PLAYBACK_CAPTURE ||
            captureInfo.sourceType == SOURCE_TYPE_REMOTE_CAST)) {
            desc->deviceType_ = DEVICE_TYPE_INVALID;
            desc->deviceRole_ = INPUT_DEVICE;
        }
        std::shared_ptr<AudioDeviceDescriptor> devDesc = std::make_shared<AudioDeviceDescriptor>(*desc);
        deviceList.push_back(devDesc);
    } else {
        vector<shared_ptr<AudioDeviceDescriptor>> descs = audioDeviceManager_.GetRemoteCaptureDevices();
        for (const auto &desc : descs) {
            std::shared_ptr<AudioDeviceDescriptor> devDesc = std::make_shared<AudioDeviceDescriptor>(*desc);
            deviceList.push_back(devDesc);
        }
    }

    return deviceList;
}

std::shared_ptr<AudioDeviceDescriptor> AudioCoreService::GetActiveBluetoothDevice()
{
    std::shared_ptr<AudioDeviceDescriptor> preferredDesc = audioStateManager_.GetPreferredCallRenderDevice();
    if (preferredDesc->deviceType_ == DEVICE_TYPE_BLUETOOTH_SCO) {
        return preferredDesc;
    }

    std::vector<shared_ptr<AudioDeviceDescriptor>> audioPrivacyDeviceDescriptors =
        audioDeviceManager_.GetCommRenderPrivacyDevices();
    std::vector<shared_ptr<AudioDeviceDescriptor>> activeDeviceDescriptors;

    for (const auto &desc : audioPrivacyDeviceDescriptors) {
        if (desc->deviceType_ == DEVICE_TYPE_BLUETOOTH_SCO && desc->isEnable_) {
            activeDeviceDescriptors.push_back(make_shared<AudioDeviceDescriptor>(*desc));
        }
    }

    uint32_t btDeviceSize = activeDeviceDescriptors.size();
    if (btDeviceSize == 0) {
        activeDeviceDescriptors = audioDeviceManager_.GetCommRenderBTCarDevices();
    }
    btDeviceSize = activeDeviceDescriptors.size();
    if (btDeviceSize == 0) {
        return make_shared<AudioDeviceDescriptor>();
    } else if (btDeviceSize == 1) {
        shared_ptr<AudioDeviceDescriptor> res = std::move(activeDeviceDescriptors[0]);
        return res;
    }

    uint32_t index = 0;
    for (uint32_t i = 1; i < btDeviceSize; ++i) {
        if (activeDeviceDescriptors[i]->connectTimeStamp_ >
            activeDeviceDescriptors[index]->connectTimeStamp_) {
            index = i;
        }
    }
    shared_ptr<AudioDeviceDescriptor> res = std::move(activeDeviceDescriptors[index]);
    return res;
}

void AudioCoreService::OnDeviceInfoUpdated(AudioDeviceDescriptor &desc, const DeviceInfoUpdateCommand command)
{
    audioDeviceStatus_.OnDeviceInfoUpdated(desc, command);
}

int32_t AudioCoreService::SetCallDeviceActive(InternalDeviceType deviceType, bool active, std::string address)
{
    AUDIO_WARNING_LOG("Device type[%{public}d] flag[%{public}d] address[%{public}s]",
        deviceType, active, GetEncryptAddr(address).c_str());
    CHECK_AND_RETURN_RET_LOG(deviceType != DEVICE_TYPE_NONE, ERR_DEVICE_NOT_SUPPORTED, "Invalid device");

    int32_t ret = audioActiveDevice_.SetCallDeviceActive(deviceType, active, address);
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ret, "SetCallDeviceActive failed");
    ret = SelectDeviceAndRoute(AudioStreamDeviceChangeReason::OVERRODE);
    CHECK_AND_RETURN_RET_LOG(ret == SUCCESS, ret, "SelectDeviceAndRoute failed");

    return SUCCESS;
}

std::vector<shared_ptr<AudioDeviceDescriptor>> AudioCoreService::GetAvailableDevices(AudioDeviceUsage usage)
{
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptors;
    audioDeviceDescriptors = audioDeviceManager_.GetAvailableDevicesByUsage(usage);
    return audioDeviceDescriptors;
}

void AudioCoreService::FetchOutputDeviceForTrack(AudioStreamChangeInfo &streamChangeInfo,
    const AudioStreamDeviceChangeReasonExt reason)
{
    AUDIO_INFO_LOG("fetch device for track, sessionid:%{public}d start",
        streamChangeInfo.audioRendererChangeInfo.sessionId);

    AudioMode mode = AudioMode::AUDIO_MODE_PLAYBACK;
    // Set prerunningState true to refetch devices when device info change before update tracker to running
    streamChangeInfo.audioRendererChangeInfo.prerunningState = true;
    if (streamCollector_.UpdateTrackerInternal(mode, streamChangeInfo) != SUCCESS) {
        return;
    }

    rendererChangeInfo.push_back(
        make_shared<AudioRendererChangeInfo>(streamChangeInfo.audioRendererChangeInfo));

    audioDeviceManager_.UpdateDefaultOutputDeviceWhenStarting(streamChangeInfo.audioRendererChangeInfo.sessionId);

    // Get streamDesc from pipeManager, then select deviceType and pipeType
    std::shared_ptr<AudioStreamDesc> streamDesc = GetStreamDescById(streamChangeInfo.audioRendererChangeInfo.sessionId);

    int32_t ret = FetchOutputDevice(streamDesc, reason);
    
}

void AudioCoreService::FetchInputDeviceForTrack(AudioStreamChangeInfo &streamChangeInfo)
{
    AUDIO_INFO_LOG("fetch device for track, sessionid:%{public}d start",
        streamChangeInfo.audioCapturerChangeInfo.sessionId);

    // Get streamDesc from pipeManager, then select deviceType and pipeType
    std::shared_ptr<AudioStreamDesc> streamDesc = GetStreamDescById(streamChangeInfo.audioCapturerChangeInfo.sessionId);

    audioDeviceCommon_.FetchInputDevice(capturerChangeInfo);
    int32_t ret = FetchInputDevice(streamDesc);
}

int32_t AudioCoreService::RegisterTracker(AudioMode &mode, AudioStreamChangeInfo &streamChangeInfo,
    const sptr<IRemoteObject> &object, const int32_t apiVersion)
{
    if (mode == AUDIO_MODE_RECORD) {
        audioMicrophoneDescriptor_.AddAudioCapturerMicrophoneDescriptor(
            streamChangeInfo.audioCapturerChangeInfo.sessionId, DEVICE_TYPE_NONE);
        if (apiVersion > 0 && apiVersion < API_11) {
            audioDeviceCommon_.UpdateDeviceInfo(streamChangeInfo.audioCapturerChangeInfo.inputDeviceInfo,
                std::make_shared<AudioDeviceDescriptor>(audioActiveDevice_.GetCurrentInputDevice()), false, false);
        }
    } else if (apiVersion > 0 && apiVersion < API_11) {
        audioDeviceCommon_.UpdateDeviceInfo(streamChangeInfo.audioRendererChangeInfo.outputDeviceInfo,
            std::make_shared<AudioDeviceDescriptor>(audioActiveDevice_.GetCurrentOutputDevice()), false, false);
    }
    return streamCollector_.RegisterTracker(mode, streamChangeInfo, object);
}

int32_t AudioCoreService::UpdateTracker(AudioMode &mode, AudioStreamChangeInfo &streamChangeInfo)
{
    HandleAudioCaptureState(mode, streamChangeInfo);

    int32_t ret = streamCollector_.UpdateTracker(mode, streamChangeInfo);

    const auto &rendererState = streamChangeInfo.audioRendererChangeInfo.rendererState;
    if (rendererState == RENDERER_PREPARED || rendererState == RENDERER_NEW || rendererState == RENDERER_INVALID) {
        return ret; // only update tracker in new and prepared
    }

    audioDeviceCommon_.UpdateTracker(mode, streamChangeInfo, rendererState);

    if (audioA2dpOffloadManager_) {
        audioA2dpOffloadManager_->UpdateA2dpOffloadFlagForAllStream(audioActiveDevice_.GetCurrentOutputDeviceType());
    }
    SendA2dpConnectedWhileRunning(rendererState, streamChangeInfo.audioRendererChangeInfo.sessionId);
    return ret;
}

void AudioCoreService::HandleAudioCaptureState(AudioMode &mode, AudioStreamChangeInfo &streamChangeInfo)
{
    if (mode == AUDIO_MODE_RECORD &&
        (streamChangeInfo.audioCapturerChangeInfo.capturerState == CAPTURER_RELEASED ||
         streamChangeInfo.audioCapturerChangeInfo.capturerState == CAPTURER_STOPPED)) {
        if (Util::IsScoSupportSource(streamChangeInfo.audioCapturerChangeInfo.capturerInfo.sourceType)) {
            audioDeviceCommon_.BluetoothScoDisconectForRecongnition();
            Bluetooth::AudioHfpManager::ClearRecongnitionStatus();
        }
        audioMicrophoneDescriptor_.RemoveAudioCapturerMicrophoneDescriptorBySessionID(
            streamChangeInfo.audioCapturerChangeInfo.sessionId);
    }
}

void AudioCoreService::UpdateTracker(AudioMode &mode, AudioStreamChangeInfo &streamChangeInfo,
    RendererState rendererState)
{
    if (rendererState == RENDERER_RELEASED && !streamCollector_.ExistStreamForPipe(PIPE_TYPE_MULTICHANNEL)) {
        audioOffloadStream_.UnloadMchModule();
    }

    if (mode == AUDIO_MODE_PLAYBACK && (rendererState == RENDERER_STOPPED || rendererState == RENDERER_PAUSED ||
        rendererState == RENDERER_RELEASED)) {
        audioDeviceManager_.UpdateDefaultOutputDeviceWhenStopping(streamChangeInfo.audioRendererChangeInfo.sessionId);
        if (rendererState == RENDERER_RELEASED) {
            audioDeviceManager_.RemoveSelectedDefaultOutputDevice(streamChangeInfo.audioRendererChangeInfo.sessionId);
        }
        SelectOutputDeviceAndRoute();
    }

    if (enableDualHalToneState_ && (mode == AUDIO_MODE_PLAYBACK)
        && (rendererState == RENDERER_STOPPED || rendererState == RENDERER_RELEASED)) {
        const int32_t sessionId = streamChangeInfo.audioRendererChangeInfo.sessionId;
        const StreamUsage streamUsage = streamChangeInfo.audioRendererChangeInfo.rendererInfo.streamUsage;
        if ((sessionId == enableDualHalToneSessionId_) && Util::IsRingerOrAlarmerStreamUsage(streamUsage)) {
            AUDIO_INFO_LOG("disable dual hal tone when ringer/alarm renderer stop/release.");
            UpdateDualToneState(false, enableDualHalToneSessionId_);
        }
    }
}

void AudioCoreService::UpdateDualToneState(const bool &enable, const int32_t &sessionId)
{
    AUDIO_INFO_LOG("update dual tone state, enable:%{public}d, sessionId:%{public}d", enable, sessionId);
    enableDualHalToneState_ = enable;
    if (enableDualHalToneState_) {
        enableDualHalToneSessionId_ = sessionId;
    }
    Trace trace("AudioDeviceCommon::UpdateDualToneState sessionId:" + std::to_string(sessionId));
    auto ret = AudioServerProxy::GetInstance().UpdateDualToneStateProxy(enable, sessionId);
    CHECK_AND_RETURN_LOG(ret == SUCCESS, "Failed to update the dual tone state for sessionId:%{public}d", sessionId);
}

void AudioCoreService::RegisteredTrackerClientDied(pid_t uid)
{
    UpdateDefaultOutputDeviceWhenStopping(static_cast<int32_t>(uid));

    audioMicrophoneDescriptor_.RemoveAudioCapturerMicrophoneDescriptor(static_cast<int32_t>(uid));
    streamCollector_.RegisteredTrackerClientDied(static_cast<int32_t>(uid));

    audioDeviceCommon_.ClientDiedDisconnectScoNormal();
    audioDeviceCommon_.ClientDiedDisconnectScoRecognition();

    if (!streamCollector_.ExistStreamForPipe(PIPE_TYPE_OFFLOAD)) {
        audioOffloadStream_.DynamicUnloadOffloadModule();
    }

    if (!streamCollector_.ExistStreamForPipe(PIPE_TYPE_MULTICHANNEL)) {
        audioOffloadStream_.UnloadMchModule();
    }
}

void AudioCoreService::UpdateDefaultOutputDeviceWhenStopping(int32_t uid)
{
    std::vector<uint32_t> sessionIDSet = streamCollector_.GetAllRendererSessionIDForUID(uid);
    for (const auto &sessionID : sessionIDSet) {
        audioDeviceManager_.UpdateDefaultOutputDeviceWhenStopping(sessionID);
        audioDeviceManager_.RemoveSelectedDefaultOutputDevice(sessionID);
    }
    SelectOutputDeviceAndRoute();
}



// No lock -- Should lock or not?
int32_t AudioCoreService::SetAudioDeviceAnahsCallback(const sptr<IRemoteObject> &object)
{
    return deviceStatusListener_->SetAudioDeviceAnahsCallback(object);
}

int32_t AudioCoreService::UnsetAudioDeviceAnahsCallback()
{
    return deviceStatusListener_->UnsetAudioDeviceAnahsCallback();
}

void AudioCoreService::OnUpdateAnahsSupport(std::string anahsShowType)
{
    AUDIO_INFO_LOG("OnUpdateAnahsSupport show type: %{public}s", anahsShowType.c_str());
    deviceStatusListener_->UpdateAnahsPlatformType(anahsShowType);
}

#ifdef BLUETOOTH_ENABLE
const sptr<IStandardAudioService> RegisterBluetoothDeathCallback()
{
    lock_guard<mutex> lock(g_btProxyMutex);
    if (g_btProxy == nullptr) {
        auto samgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
        CHECK_AND_RETURN_RET_LOG(samgr != nullptr, nullptr,
            "get sa manager failed");
        sptr<IRemoteObject> object = samgr->GetSystemAbility(BLUETOOTH_HOST_SYS_ABILITY_ID);
        CHECK_AND_RETURN_RET_LOG(object != nullptr, nullptr,
            "get audio service remote object failed");
        g_btProxy = iface_cast<IStandardAudioService>(object);
        CHECK_AND_RETURN_RET_LOG(g_btProxy != nullptr, nullptr,
            "get audio service proxy failed");

        // register death recipent
        sptr<AudioServerDeathRecipient> asDeathRecipient =
            new(std::nothrow) AudioServerDeathRecipient(getpid(), getuid());
        if (asDeathRecipient != nullptr) {
            asDeathRecipient->SetNotifyCb([] (pid_t pid, pid_t uid) {
                AudioCoreService::BluetoothServiceCrashedCallback(pid, uid);
            });
            bool result = object->AddDeathRecipient(asDeathRecipient);
            if (!result) {
                AUDIO_ERR_LOG("failed to add deathRecipient");
            }
        }
    }
    sptr<IStandardAudioService> gasp = g_btProxy;
    return gasp;
}

void AudioCoreService::BluetoothServiceCrashedCallback(pid_t pid, pid_t uid)
{
    AUDIO_INFO_LOG("Bluetooth sa crashed, will restore proxy in next call");
    lock_guard<mutex> lock(g_btProxyMutex);
    g_btProxy = nullptr;
    isBtListenerRegistered = false;
    Bluetooth::AudioA2dpManager::DisconnectBluetoothA2dpSink();
    Bluetooth::AudioA2dpManager::DisconnectBluetoothA2dpSource();
    Bluetooth::AudioHfpManager::DisconnectBluetoothHfpSink();
}
#endif

void AudioCoreService::RegisterBluetoothListener()
{
#ifdef BLUETOOTH_ENABLE
    AUDIO_INFO_LOG("Enter");
    Bluetooth::RegisterDeviceObserver(deviceStatusListener_->deviceObserver_);
    if (isBtListenerRegistered) {
        AUDIO_INFO_LOG("audio policy service already register bt listerer, return");
        return;
    }
    Bluetooth::AudioA2dpManager::RegisterBluetoothA2dpListener();
    Bluetooth::AudioHfpManager::RegisterBluetoothScoListener();
    isBtListenerRegistered = true;
    const sptr<IStandardAudioService> gsp = RegisterBluetoothDeathCallback();
    AudioPolicyUtils::GetInstance().SetBtConnecting(true);
    Bluetooth::AudioA2dpManager::CheckA2dpDeviceReconnect();
    Bluetooth::AudioHfpManager::CheckHfpDeviceReconnect();
    AudioPolicyUtils::GetInstance().SetBtConnecting(false);
#endif
}

void AudioCoreService::UnregisterBluetoothListener()
{
#ifdef BLUETOOTH_ENABLE
    AUDIO_INFO_LOG("Enter");
    Bluetooth::UnregisterDeviceObserver();
    Bluetooth::AudioA2dpManager::UnregisterBluetoothA2dpListener();
    Bluetooth::AudioHfpManager::UnregisterBluetoothScoListener();
    isBtListenerRegistered = false;
#endif
}

int32_t AudioCoreService::SelectDeviceAndRoute(const AudioStreamDeviceChangeReason reason)
{
    return SelectOutputDeviceAndRoute(reason) && SelectInputDeviceAndRoute();
}

int32_t AudioCoreService::SelectOutputDeviceAndRoute(const AudioStreamDeviceChangeReason reason)
{
    std::vector<std:shared_ptr<AudioStreamDesc>> outputStreamDescs = pipeManager_->GetAllOutputStreamDescs();
    int32_t ret = FetchOutputDevices(outputStreamDescs, reason);
}

int32_t AudioCoreService::SelectInputDeviceAndRoute()
{
    std::vector<std:shared_ptr<AudioStreamDesc>> inputStreamDescs = pipeManager_->GetAllInputStreamDescs();
    ret = FetchInputDevices(inputStreamDescs);
}

int32_t AudioCoreService::FetchOutputDevice(std::shared_ptr<AudioStreamDesc> streamDesc, const AudioStreamDeviceChangeReasonExt reason)
{
    vector<std::shared_ptr<AudioDeviceDescriptor>> descs = audioRouterCenter_.FetchOutputDevices(streamDesc.rendererInfo.streamUsage, GetRealUid(streamDesc));
    std::shared_ptr<AudioDeviceDescriptor> desc = descs[0];
    streamDesc->deviceDesc_ = desc;
    streamDesc->SetFlag(audioFlag);

    FetchPipeAndExecute(streamDesc, reason);
}

int32_t AudioCoreService::FetchInputDevice(std::shared_ptr<AudioStreamDesc> streamDesc)
{
    std::shared_ptr<AudioDeviceDescriptor> desc = audioRouterCenter_.FetchInputDevice(streamDesc.capturerInfo.sourceType, GetRealUid(streamDesc));
    streamDesc->deviceDesc_ = desc;
    streamDesc->SetFlag(audioFlag);

    FetchPipeAndExecute(streamDesc);
}

int32_t AudioCoreService::FetchOutputDevices(std::vector<std:shared_ptr<AudioStreamDesc>> &outputStreamDescs, const AudioStreamDeviceChangeReasonExt reason)
{
    for (std:shared_ptr<AudioStreamDesc> &streamDesc : outputStreamDescs) {
        vector<std::shared_ptr<AudioDeviceDescriptor>> descs = audioRouterCenter_.FetchOutputDevices(streamDesc.rendererInfo.streamUsage, GetRealUid(streamDesc));
        std::shared_ptr<AudioDeviceDescriptor> desc = descs[0];
        streamDesc->deviceDesc_ = desc;
        streamDesc->SetFlag(audioFlag);
    }

    FetchPipesAndExecute(outputStreamDescs, reason);
}

int32_t AudioCoreService::FetchInputDevices(std::vector<std:shared_ptr<AudioStreamDesc>> &inputStreamDescs, const AudioStreamDeviceChangeReasonExt reason)
{
    for (std:shared_ptr<AudioStreamDesc> &streamDesc : inputStreamDescs) {
        std::shared_ptr<AudioDeviceDescriptor> desc = audioRouterCenter_.FetchInputDevice(streamDesc.capturerInfo.sourceType, GetRealUid(streamDesc));
        streamDesc->deviceDesc_ = desc;
        streamDesc->SetFlag(audioFlag);
    }

    FetchPipesAndExecute(inputStreamDescs, reason);
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

// device status listsener inner
void AudioCoreService::OnDeviceStatusUpdated(DeviceType devType, bool isConnected, const std::string& macAddress,
    const std::string& deviceName, const AudioStreamInfo& streamInfo, DeviceRole role)
{
    // Pnp device status update
    audioDeviceStatus_.OnDeviceStatusUpdated(devType, isConnected, macAddress, deviceName, streamInfo, role);
}

void AudioCoreService::OnDeviceStatusUpdated(AudioDeviceDescriptor &updatedDesc, bool isConnected)
{
    // Bluetooth device status updated
    DeviceType devType = updatedDesc.deviceType_;
    string macAddress = updatedDesc.macAddress_;
    string deviceName = updatedDesc.deviceName_;
    bool isActualConnection = (updatedDesc.connectState_ != VIRTUAL_CONNECTED);
    AUDIO_INFO_LOG("Device connection is actual connection: %{public}d", isActualConnection);

    AudioStreamInfo streamInfo = {};
#ifdef BLUETOOTH_ENABLE
    if (devType == DEVICE_TYPE_BLUETOOTH_A2DP && isActualConnection && isConnected) {
        int32_t ret = Bluetooth::AudioA2dpManager::GetA2dpDeviceStreamInfo(macAddress, streamInfo);
        CHECK_AND_RETURN_LOG(ret == SUCCESS, "Get a2dp device stream info failed!");
    }
    if (devType == DEVICE_TYPE_BLUETOOTH_A2DP_IN && isActualConnection && isConnected) {
        int32_t ret = Bluetooth::AudioA2dpManager::GetA2dpInDeviceStreamInfo(macAddress, streamInfo);
        CHECK_AND_RETURN_LOG(ret == SUCCESS, "Get a2dp input device stream info failed!");
    }
    if (isConnected && isActualConnection
        && devType == DEVICE_TYPE_BLUETOOTH_SCO
        && updatedDesc.deviceCategory_ != BT_UNWEAR_HEADPHONE
        && !audioDeviceManager_.GetScoState()) {
        Bluetooth::AudioHfpManager::SetActiveHfpDevice(macAddress);
    }
#endif
    audioDeviceStatus_.OnDeviceStatusUpdated(updatedDesc, devType,
        macAddress, deviceName, isActualConnection, streamInfo, isConnected);
}

void AudioCoreService::OnDeviceStatusUpdated(DStatusInfo statusInfo, bool isStop)
{
    // Distributed devices status update
    audioDeviceStatus_.OnDeviceStatusUpdated(statusInfo, isStop);
}

void AudioCoreService::OnMicrophoneBlockedUpdate(DeviceType devType, DeviceBlockStatus status)
{
    CHECK_AND_RETURN_LOG(devType != DEVICE_TYPE_NONE, "devType is none type");
    audioDeviceStatus_.OnMicrophoneBlockedUpdate(devType, status);
}

void AudioCoreService::OnPnpDeviceStatusUpdated(AudioDeviceDescriptor &desc, bool isConnected)
{
    audioDeviceStatus_.OnPnpDeviceStatusUpdated(desc, isConnected);
}

void AudioCoreService::OnDeviceConfigurationChanged(DeviceType deviceType, const std::string &macAddress,
    const std::string &deviceName, const AudioStreamInfo &streamInfo)
{
    audioDeviceStatus_.OnDeviceConfigurationChanged(deviceType, macAddress, deviceName, streamInfo);
}

int32_t AudioCoreService::OnServiceConnected(AudioServiceIndex serviceIndex)
{
    return audioDeviceStatus_.OnServiceConnected(serviceIndex);
}

void AudioCoreService::OnForcedDeviceSelected(DeviceType devType, const std::string &macAddress)
{
    audioDeviceStatus_.OnForcedDeviceSelected(devType, macAddress);
}
// device status listener end 

std::vector<sptr<MicrophoneDescriptor>> AudioCoreService::GetAvailableMicrophones()
{
    return audioMicrophoneDescriptor_.GetAvailableMicrophones();
}

std::vector<sptr<MicrophoneDescriptor>> AudioCoreService::GetAudioCapturerMicrophoneDescriptors(int32_t sessionId)
{
    return audioMicrophoneDescriptor_.GetAudioCapturerMicrophoneDescriptors(sessionId);
}

void AudioCoreService::OnReceiveBluetoothEvent(const std::string macAddress, const std::string deviceName)
{
    audioDeviceManager_.OnReceiveBluetoothEvent(macAddress, deviceName);
    audioConnectedDevice_.SetDisplayName(macAddress, deviceName);
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
    std::lock_guard<std::mutex> lock(eventMutex_);
    return coreService_->IsArmUsbDevice(deviceDesc);
}

std::vector<std::shared_ptr<AudioDeviceDescriptor>> AudioCoreService::EventEntry::GetDevices(DeviceFlag deviceFlag)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    return coreService_->GetDevices(deviceFlag);
}

int32_t AudioCoreService::EventEntry::SetDeviceActive(InternalDeviceType deviceType, bool active)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    return coreService_->SetDeviceActive(deviceType, active);
}

std::vector<std::shared_ptr<AudioDeviceDescriptor>> AudioCoreService::EventEntry::GetPreferredOutputDeviceDescInner(
    AudioRendererInfo &rendererInfo, std::string networkId)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    return coreService_->GetPreferredOutputDeviceDescInner(rendererInfo, networkId);
}

std::vector<std::shared_ptr<AudioDeviceDescriptor>> AudioCoreService::EventEntry::GetPreferredInputDeviceDescriptors(
    AudioCapturerInfo &captureInfo, std::string networkId)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    return coreService_->GetPreferredInputDeviceDescriptors(captureInfo, networkId);
}

std::shared_ptr<AudioDeviceDescriptor> AudioCoreService::EventEntry::GetActiveBluetoothDevice()
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    return coreService_->GetPreferredInputDeviceDescriptors(captureInfo, networkId);
}

void AudioCoreService::EventEntry::OnDeviceInfoUpdated(AudioDeviceDescriptor &desc, const DeviceInfoUpdateCommand command)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_.OnDeviceInfoUpdated(desc, command);
}

int32_t AudioCoreService::EventEntry::SetCallDeviceActive(InternalDeviceType deviceType, bool active, std::string address)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_.SetCallDeviceActive(deviceType, active, address);
}

std::vector<shared_ptr<AudioDeviceDescriptor>> AudioCoreService::EventEntry::GetAvailableDevices(AudioDeviceUsage usage)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_.GetAvailableDevices(usage);
}

// Start 不选择设备和通路了，所以不需要调用这两个
void AudioCoreService::EventEntry::FetchOutputDeviceForTrack(AudioStreamChangeInfo &streamChangeInfo,
    const AudioStreamDeviceChangeReasonExt reason)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_.FetchOutputDeviceForTrack(streamChangeInfo, reason);
}

// Start 不选择设备和通路了，所以不需要调用这两个
void AudioCoreService::EventEntry::FetchInputDeviceForTrack(AudioStreamChangeInfo &streamChangeInfo)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_.FetchInputDeviceForTrack(streamChangeInfo);
}

int32_t AudioCoreService::EventEntry::RegisterTracker(AudioMode &mode, AudioStreamChangeInfo &streamChangeInfo,
    const sptr<IRemoteObject> &object, const int32_t apiVersion)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    return coreService_->RegisterTracker(mode, streamChangeInfo, object, apiVersion);
}

int32_t AudioCoreService::EventEntry::RegisterTracker(AudioMode &mode, AudioStreamChangeInfo &streamChangeInfo,
    const sptr<IRemoteObject> &object, const int32_t apiVersion)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    return coreService_->RegisterTracker(mode, streamChangeInfo, object, apiVersion);
}

void AudioCoreService::EventEntry::RegisteredTrackerClientDied(pid_t uid)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_->RegisteredTrackerClientDied(uid);
}

// device status listener

void AudioCoreService::EventEntry::OnDeviceStatusUpdated(DeviceType devType, bool isConnected, const std::string& macAddress,
    const std::string& deviceName, const AudioStreamInfo& streamInfo, DeviceRole role)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_->OnDeviceStatusUpdated(devType, isConnected, macAddress, deviceName, streamInfo, role);
}

void AudioCoreService::EventEntry::OnDeviceStatusUpdated(AudioDeviceDescriptor &updatedDesc, bool isConnected)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_->OnDeviceStatusUpdated(updatedDesc, isConnected);
}

void AudioCoreService::EventEntry::OnDeviceStatusUpdated(DStatusInfo statusInfo, bool isStop)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_->OnDeviceStatusUpdated(statusInfo, isStop);
}

void AudioCoreService::EventEntry::OnMicrophoneBlockedUpdate(DeviceType devType, DeviceBlockStatus status)
{
    CHECK_AND_RETURN_LOG(devType != DEVICE_TYPE_NONE, "devType is none type");
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_->OnMicrophoneBlockedUpdate(devType, status);
}

void AudioCoreService::EventEntry::OnPnpDeviceStatusUpdated(AudioDeviceDescriptor &desc, bool isConnected)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_.OnPnpDeviceStatusUpdated(desc, isConnected);
}

void AudioCoreService::EventEntry::OnDeviceConfigurationChanged(DeviceType deviceType, const std::string &macAddress,
    const std::string &deviceName, const AudioStreamInfo &streamInfo)
{
    coreService_.OnDeviceConfigurationChanged(deviceType, macAddress, deviceName, streamInfo);
}

bool AudioCoreService::EventEntry::ConnectServiceAdapter()
{
    bool ret = audioPolicyManager_.ConnectServiceAdapter();
    CHECK_AND_RETURN_RET_LOG(ret, false, "Error in connecting to audio service adapter");

    OnServiceConnected(AudioServiceIndex::AUDIO_SERVICE_INDEX);

    return true;
}

void AudioCoreService::EventEntry::OnServiceConnected(AudioServiceIndex serviceIndex)
{
    AUDIO_INFO_LOG("[module_load]::OnServiceConnected for [%{public}d]", serviceIndex);
    CHECK_AND_RETURN_LOG(serviceIndex >= HDI_SERVICE_INDEX && serviceIndex <= AUDIO_SERVICE_INDEX, "invalid index");

    // If audio service or hdi service is not ready, donot load default modules
    lock_guard<mutex> lock(serviceFlagMutex_);
    serviceFlag_.set(serviceIndex, true);
    if (serviceFlag_.count() != MIN_SERVICE_COUNT) {
        AUDIO_INFO_LOG("[module_load]::hdi service or audio service not up. Cannot load default module now");
        return;
    }

    std::unique_lock<std::mutex> lock(eventMutex_);
    int32_t ret = coreService_->OnServiceConnected(serviceIndex);
    lock.unlock();
    if (ret == SUCCESS) {
#ifdef USB_ENABLE
        AudioUsbManager::GetInstance().Init(this);
#endif
        coreService_->audioEffectService_.SetMasterSinkAvailable();
    }
#ifdef HAS_FEATURE_INNERCAPTURER
    // load inner-cap-sink
    LoadModernInnerCapSink();
#endif
    // RegisterBluetoothListener() will be called when bluetooth_host is online
    // load hdi-effect-model
    LoadHdiEffectModel();
}

#ifdef HAS_FEATURE_INNERCAPTURER
void AudioCoreService::EventEntry::LoadModernInnerCapSink()
{
    AUDIO_INFO_LOG("Start");
    AudioModuleInfo moduleInfo = {};
    moduleInfo.lib = "libmodule-inner-capturer-sink.z.so";
    moduleInfo.name = INNER_CAPTURER_SINK;

    moduleInfo.format = "s16le";
    moduleInfo.channels = "2"; // 2 channel
    moduleInfo.rate = "48000";
    moduleInfo.bufferSize = "3840"; // 20ms

    audioIOHandleMap_.OpenPortAndInsertIOHandle(moduleInfo.name, moduleInfo);
}
#endif

void AudioCoreService::EventEntry::LoadHdiEffectModel()
{
    return AudioServerProxy::GetInstance().LoadHdiEffectModelProxy();
}

void AudioCoreService::EventEntry::OnServiceDisconnected(AudioServiceIndex serviceIndex)
{
    AUDIO_WARNING_LOG("Service index [%{public}d]", serviceIndex);
}

void AudioCoreService::EventEntry::OnForcedDeviceSelected(DeviceType devType, const std::string &macAddress)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_->OnForcedDeviceSelected(devType, macAddress);
}
// device status listener end

vector<sptr<MicrophoneDescriptor>> AudioCoreService::EventEntry::GetAvailableMicrophones()
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    return coreService_->GetAvailableMicrophones();
}

vector<sptr<MicrophoneDescriptor>> AudioPolicyService::EventEntry::GetAudioCapturerMicrophoneDescriptors(int32_t sessionId)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    return coreService_->GetAudioCapturerMicrophoneDescriptors(sessionId);
}

void AudioPolicyService::EventEntry::OnReceiveBluetoothEvent(const std::string macAddress, const std::string deviceName)
{
    std::lock_guard<std::mutex> lock(eventMutex_);
    coreService_->OnReceiveBluetoothEvent(macAddress, deviceName);
}

} // namespace AudioStandard
} // namespace OHOS
