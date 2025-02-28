#ifndef AUDIO_CORE_SERVICE_H
#define AUDIO_CORE_SERVICE_H
#include <mutex>

#include "audio_policy_server_handler.h"
#include "i_core_service_provider.h"
#include "idevice_status_observer.h"
#include "audio_stream_descriptor.h"
#include "audio_device_descriptor.h"
#include "audio_info.h"
#include "microphone_descriptor.h"
#include "audio_stream_change_info.h"
#include "audio_active_device.h"
#include "audio_scene_manager.h"
#include "audio_volume_manager.h"
#include "audio_capturer_session.h"
#include "audio_device_manager.h"
#include "audio_connected_device.h"
#include "audio_device_status.h"
#include "audio_effect_service.h"
#include "audio_microphone_descriptor.h"
#include "audio_recovery_device.h"
#include "device_status_listener.h"
#include "core_service_provider_stub.h"
#include "audio_pipe_info.h"
#include "audio_service_enum.h"


namespace OHOS {
namespace AudioStandard {
class AudioA2dpOffloadManager;
class AudioCoreService : public enable_shared_from_this<AudioCoreService> {
public:
    class EventEntry : public ICoreServiceProvider, public IDeviceStatusObserver {
    public:
        EventEntry(std::shared_ptr<AudioCoreService> coreService);
        void RegiestCoreService();
        int32_t CreateRendererClient(std::shared_ptr<AudioStreamDescriptor> streamDesc, AudioFlag &flag, uint32_t &sessionId);
        int32_t CreateCapturerClient(std::shared_ptr<AudioStreamDescriptor> streamDesc, AudioFlag &flag, uint32_t &sessionId);

        // ICoreServiceProvider
        int32_t UpdateSessionOperation(uint32_t sessionId, SessionOperation operation) override;

        // IDeviceStatusObserver
        void OnDeviceInfoUpdated(AudioDeviceDescriptor &desc, const DeviceInfoUpdateCommand command) override;
        void OnDeviceStatusUpdated(DeviceType devType, bool isConnected,
            const std::string &macAddress, const std::string &deviceName,
            const AudioStreamInfo &streamInfo, DeviceRole role = DEVICE_ROLE_NONE, bool hasPair = false) override;
        void OnDeviceStatusUpdated(AudioDeviceDescriptor &desc, bool isConnected) override;
        void OnDeviceStatusUpdated(DStatusInfo statusInfo, bool isStop = false) override;
        void OnMicrophoneBlockedUpdate(DeviceType devType, DeviceBlockStatus status) override;
        void OnPnpDeviceStatusUpdated(AudioDeviceDescriptor &desc, bool isConnected) override;
        void OnDeviceConfigurationChanged(DeviceType deviceType, const std::string &macAddress,
            const std::string &deviceName, const AudioStreamInfo &streamInfo) override;
        void OnServiceConnected(AudioServiceIndex serviceIndex) override;
        void OnServiceDisconnected(AudioServiceIndex serviceIndex) override;
        void OnForcedDeviceSelected(DeviceType devType, const std::string &macAddress) override;

        int32_t SetAudioScene(AudioScene audioScene);
        bool IsArmUsbDevice(const AudioDeviceDescriptor &deviceDesc);
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> GetDevices(DeviceFlag deviceFlag);
        int32_t SetDeviceActive(InternalDeviceType deviceType, bool active);
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> GetPreferredOutputDeviceDescriptors(
            AudioRendererInfo &rendererInfo, std::string networkId = LOCAL_NETWORK_ID);
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> GetPreferredInputDeviceDescriptors(
            AudioCapturerInfo &captureInfo, std::string networkId = LOCAL_NETWORK_ID);
        std::shared_ptr<AudioDeviceDescriptor> GetActiveBluetoothDevice();
        int32_t SetCallDeviceActive(InternalDeviceType deviceType, bool active, std::string address);
        std::vector<shared_ptr<AudioDeviceDescriptor>> GetAvailableDevices(AudioDeviceUsage usage);
        int32_t RegisterTracker(AudioMode &mode, AudioStreamChangeInfo &streamChangeInfo,
            const sptr<IRemoteObject> &object, const int32_t apiVersion);
        int32_t UpdateTracker(AudioMode &mode, AudioStreamChangeInfo &streamChangeInfo);
        void RegisteredTrackerClientDied(pid_t uid);
        bool ConnectServiceAdapter();
        void OnReceiveBluetoothEvent(const std::string macAddress, const std::string deviceName);
        std::vector<sptr<MicrophoneDescriptor>> GetAvailableMicrophones();
        std::vector<sptr<MicrophoneDescriptor>> GetAudioCapturerMicrophoneDescriptors(int32_t sessionId);
        int32_t SelectOutputDevice(sptr<AudioRendererFilter> audioRendererFilter,
            std::vector<std::shared_ptr<AudioDeviceDescriptor>> selectedDesc);
        int32_t SelectInputDevice(sptr<AudioCapturerFilter> audioCapturerFilter,
            std::vector<std::shared_ptr<AudioDeviceDescriptor>> selectedDesc);
        int32_t GetCurrentRendererChangeInfos(vector<shared_ptr<AudioRendererChangeInfo>>
            &audioRendererChangeInfos, bool hasBTPermission, bool hasSystemPermission);
        void NotifyRemoteRenderState(std::string networkId, std::string condition, std::string value);
        int32_t OnCapturerSessionAdded(uint64_t sessionID, SessionInfo sessionInfo, AudioStreamInfo streamInfo);
        void OnCapturerSessionRemoved(uint64_t sessionID);
        void SetDisplayName(const std::string &deviceName, bool isLocalDevice);
        int32_t TriggerFetchDevice(AudioStreamDeviceChangeReasonExt reason);
        std::vector<sptr<VolumeGroupInfo>> GetVolumeGroupInfos();
        void FetchOutputDeviceForTrack(AudioStreamChangeInfo &streamChangeInfo,
            const AudioStreamDeviceChangeReasonExt reason);
        void FetchInputDeviceForTrack(AudioStreamChangeInfo &streamChangeInfo);
        int32_t ExcludeOutputDevices(AudioDeviceUsage audioDevUsage,
            std::vector<std::shared_ptr<AudioDeviceDescriptor>> &audioDeviceDescriptors);
        int32_t UnexcludeOutputDevices(AudioDeviceUsage audioDevUsage,
            std::vector<std::shared_ptr<AudioDeviceDescriptor>> &audioDeviceDescriptors);
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> GetExcludedOutputDevices(AudioDeviceUsage audioDevUsage);
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> GetPreferredOutputDeviceDescriptors(
            AudioCapturerInfo &captureInfo, std::string networkId = LOCAL_NETWORK_ID);
        int32_t GetPreferredOutputStreamType(AudioRendererInfo &rendererInfo, const std::string &bundleName);
        int32_t GetPreferredInputStreamType(AudioCapturerInfo &capturerInfo);

private:
#ifdef HAS_FEATURE_INNERCAPTURER
        void LoadModernInnerCapSink();
#endif
        void LoadHdiEffectModel();

private:
        std::shared_ptr<AudioCoreService> coreService_;
        std::shared_mutex eventMutex_;
    };

    // Ctor & dtor
    AudioCoreService();
    ~AudioCoreService();

    // Called by AudioPolicyServer
    static std::shared_ptr<AudioCoreService> GetCoreService();
    void Init();
    void DeInit();
    void SetCallbackHandler(std::shared_ptr<AudioPolicyServerHandler> handler);
    std::shared_ptr<EventEntry> GetEventEntry();

    // Called by EventEntry - with lock
    int32_t CreateRendererClient(std::shared_ptr<AudioStreamDescriptor> streamDesc, AudioFlag &audioFlag, uint32_t &sessionId);
    int32_t CreateCapturerClient(std::shared_ptr<AudioStreamDescriptor> streamDesc, AudioFlag &audioFlag, uint32_t &sessionId);
    int32_t StartClient(uint32_t sessionId);
    int32_t PauseClient(uint32_t sessionId);
    int32_t StopClient(uint32_t sessionId);
    int32_t ReleaseClient(uint32_t sessionId);

    int32_t SetAudioScene(AudioScene audioScene);
    bool IsArmUsbDevice(const AudioDeviceDescriptor &deviceDesc);
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> GetDevices(DeviceFlag deviceFlag);
    int32_t SetDeviceActive(InternalDeviceType deviceType, bool active);
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> GetPreferredOutputDeviceDescInner(
        AudioRendererInfo &rendererInfo, std::string networkId);
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> GetPreferredInputDeviceDescInner(
        AudioCapturerInfo &captureInfo, std::string networkId);
    std::shared_ptr<AudioDeviceDescriptor> GetActiveBluetoothDevice();
    void OnDeviceInfoUpdated(AudioDeviceDescriptor &desc, const DeviceInfoUpdateCommand command);
    int32_t SetCallDeviceActive(InternalDeviceType deviceType, bool active, std::string address);
    std::vector<shared_ptr<AudioDeviceDescriptor>> GetAvailableDevices(AudioDeviceUsage usage);
    void FetchOutputDeviceForTrack(AudioStreamChangeInfo &streamChangeInfo,
        const AudioStreamDeviceChangeReasonExt reason);
    void FetchInputDeviceForTrack(AudioStreamChangeInfo &streamChangeInfo);
    int32_t ExcludeOutputDevices(AudioDeviceUsage audioDevUsage,
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> &audioDeviceDescriptors);
    int32_t UnexcludeOutputDevices(AudioDeviceUsage audioDevUsage,
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> &audioDeviceDescriptors);
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> GetExcludedOutputDevices(AudioDeviceUsage audioDevUsage);
    bool ConnectServiceAdapter();
    bool GetVolumeGroupInfos(std::vector<sptr<VolumeGroupInfo>> &infos);
    void LoadModernInnerCapSink();
    int32_t RegisterTracker(AudioMode &mode, AudioStreamChangeInfo &streamChangeInfo,
                            const sptr<IRemoteObject> &object, const int32_t apiVersion);
    int32_t UpdateTracker(AudioMode &mode, AudioStreamChangeInfo &streamChangeInfo);
    void HandleAudioCaptureState(AudioMode &mode, AudioStreamChangeInfo &streamChangeInfo);
    void UpdateTracker(AudioMode &mode, AudioStreamChangeInfo &streamChangeInfo,
        RendererState rendererState);
    void RegisteredTrackerClientDied(pid_t uid);
    void UpdateDefaultOutputDeviceWhenStopping(int32_t uid);


    // device status listener inner
    void OnDeviceStatusUpdated(DeviceType devType, bool isConnected,
        const std::string &macAddress, const std::string &deviceName,
        const AudioStreamInfo &streamInfo, DeviceRole role = DEVICE_ROLE_NONE, bool hasPair = false);
    void OnDeviceStatusUpdated(AudioDeviceDescriptor &desc, bool isConnected);
    void OnDeviceStatusUpdated(DStatusInfo statusInfo, bool isStop = false);
    void OnMicrophoneBlockedUpdate(DeviceType devType, DeviceBlockStatus status);
    void OnPnpDeviceStatusUpdated(AudioDeviceDescriptor &desc, bool isConnected);
    void OnDeviceConfigurationChanged(DeviceType deviceType, const std::string &macAddress,
        const std::string &deviceName, const AudioStreamInfo &streamInfo);
    int32_t OnServiceConnected(AudioServiceIndex serviceIndex);
    void OnForcedDeviceSelected(DeviceType devType, const std::string &macAddress);
    // device status listener inner end

    std::vector<sptr<MicrophoneDescriptor>> GetAvailableMicrophones();
    std::vector<sptr<MicrophoneDescriptor>> GetAudioCapturerMicrophoneDescriptors(int32_t sessionId);
    void OnReceiveBluetoothEvent(const std::string macAddress, const std::string deviceName);

    int32_t SelectOutputDevice(sptr<AudioRendererFilter> audioRendererFilter,
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> selectedDesc);
    int32_t SelectInputDevice(sptr<AudioCapturerFilter> audioCapturerFilter,
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> selectedDesc);
    int32_t GetPreferredOutputStreamType(AudioRendererInfo &rendererInfo, const std::string &bundleName);
    int32_t GetPreferredInputStreamType(AudioCapturerInfo &capturerInfo);
    int32_t GetCurrentRendererChangeInfos(vector<shared_ptr<AudioRendererChangeInfo>>
        &audioRendererChangeInfos, bool hasBTPermission, bool hasSystemPermission);
    int32_t OnCapturerSessionAdded(uint64_t sessionID, SessionInfo sessionInfo, AudioStreamInfo streamInfo);
    void NotifyRemoteRenderState(std::string networkId, std::string condition, std::string value);
    void OnCapturerSessionRemoved(uint64_t sessionID);
    void SetDisplayName(const std::string &deviceName, bool isLocalDevice);
    int32_t TriggerFetchDevice(AudioStreamDeviceChangeReasonExt reason);

    // Called by Others - without lock
    int32_t SetAudioDeviceAnahsCallback(const sptr<IRemoteObject> &object);
    int32_t UnsetAudioDeviceAnahsCallback();
    void OnUpdateAnahsSupport(std::string anahsShowType);
    void RegisterBluetoothListener();
    void UnregisterBluetoothListener();
    void NotifyAccountsChanged(const int &id);
    void RegisterDataObserver();
    int32_t FetchRendererPipesAndExecute(std::vector<std::shared_ptr<AudioStreamDescriptor>> &streamDescs, const AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN);
    int32_t FetchCapturerPipesAndExecute(std::vector<std::shared_ptr<AudioStreamDescriptor>> &streamDescs);
    int32_t SelectInputDeviceAndRoute();
    int32_t SelectOutputDeviceAndRoute(const AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN);

private:


#ifdef BLUETOOTH_ENABLE
    const sptr<IStandardAudioService> RegisterBluetoothDeathCallback();
    static void BluetoothServiceCrashedCallback(pid_t pid, pid_t uid);
#endif

    int32_t SelectDeviceAndRoute(const AudioStreamDeviceChangeReasonExt reason);
    int32_t FetchOutputDevice(std::shared_ptr<AudioStreamDescriptor> streamDesc, const AudioStreamDeviceChangeReasonExt reason);
    int32_t FetchInputDevice(std::shared_ptr<AudioStreamDescriptor> streamDesc);
    int32_t FetchOutputDevices(std::vector<std::shared_ptr<AudioStreamDescriptor>> &outputStreamDescs, const AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReasonExt::ExtEnum::UNKNOWN);
    int32_t FetchInputDevices(std::vector<std::shared_ptr<AudioStreamDescriptor>> &inputStreamDescs);
    int32_t FetchPipesAndExecute(std::vector<AudioStreamDescriptor> &outputStreamDescs, const AudioStreamDeviceChangeReasonExt reason);
    int32_t FetchRendererPipeAndExecute(std::shared_ptr<AudioStreamDescriptor> streamDesc, uint32_t &sessionId, AudioFlag &audioFlag, const AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReasonExt::ExtEnum::UNKNOWN);
    void ProcessOutputPipeNew(std::shared_ptr<AudioPipeInfo> pipeInfo, AudioFlag &flag);
    void ProcessOutputPipeUpdate(std::shared_ptr<AudioPipeInfo> pipeInfo, AudioFlag &flag);
    int32_t FetchCapturerPipeAndExecute(std::shared_ptr<AudioStreamDescriptor> streamDesc, AudioFlag &audioFlag, uint32_t &sessionId);
    void ProcessInputPipeNew(std::shared_ptr<AudioPipeInfo> pipeInfo, AudioFlag &flag);
    void ProcessInputPipeUpdate(std::shared_ptr<AudioPipeInfo> pipeInfo, AudioFlag &flag);
    void RemoveUnusedPipe();
    uint32_t GenerateSessionId(int32_t uid);
    void MoveToNewOutputDevice(std::shared_ptr<AudioStreamDescriptor> streamDesc, const AudioStreamDeviceChangeReasonExt reason = AudioStreamDeviceChangeReason::UNKNOWN);
    int32_t MoveToRemoteOutputDevice(std::vector<SinkInput> sinkInputIds, std::shared_ptr<AudioDeviceDescriptor> remoteDeviceDescriptor);
    void MoveToNewInputDevice(std::shared_ptr<AudioStreamDescriptor> streamDesc);
    int32_t MoveToLocalInputDevice(std::vector<SourceOutput> sourceOutputs, std::shared_ptr<AudioDeviceDescriptor> localDeviceDescriptor);
    int32_t MoveToRemoteInputDevice(std::vector<SourceOutput> sourceInputs, std::shared_ptr<AudioDeviceDescriptor> remoteDeviceDescriptor);
    int32_t OpenRemoteAudioDevice(std::string networkId, DeviceRole deviceRole, DeviceType deviceType, std::shared_ptr<AudioDeviceDescriptor> remoteDeviceDescriptor);
    bool SelectRingerOrAlarmDevices(std::shared_ptr<AudioStreamDescriptor> streamDesc);
    void UpdateDualToneState(const bool &enable, const int32_t &sessionId);
    int32_t MoveToLocalOutputDevice(std::vector<SinkInput> sinkInputIds, std::shared_ptr<AudioDeviceDescriptor> localDeviceDescriptor);
    void UpdateDeviceInfo(std::shared_ptr<AudioDeviceDescriptor> oldDeviceDesc, const std::shared_ptr<AudioDeviceDescriptor> newDeviceDesc, bool hasBTPermission, bool hasSystemPermission);
    bool HasLowLatencyCapability(DeviceType deviceType, bool isRemote);
    void TriggerRecreateRendererStreamCallback(int32_t callerPid, int32_t sessionId, int32_t streamFlag,
                                               const AudioStreamDeviceChangeReasonExt::ExtEnum reason = AudioStreamDeviceChangeReasonExt::ExtEnum::UNKNOWN);
    void TriggerRecreateCapturerStreamCallback(int32_t callerPid, int32_t sessionId, int32_t streamFlag);
    uint32_t OpenNewAudioPortAndRoute(std::shared_ptr<AudioPipeInfo> pipeInfo);
    static int32_t GetRealUid(std::shared_ptr<AudioStreamDescriptor> streamDesc);
    static void UpdateRendererInfoWhenNoPermission(const shared_ptr<AudioRendererChangeInfo> &audioRendererChangeInfos,
        bool hasSystemPermission);
    void SendA2dpConnectedWhileRunning(const RendererState &rendererState, const uint32_t &sessionId);
    void UpdateSessionConnectionState(const int32_t &sessionID, const int32_t &state);
    void UpdateTrackerDeviceChange(const vector<std::shared_ptr<AudioDeviceDescriptor>> &desc);
    void SubscribeAccessibilityConfigObserver();
    void RegisterAccessibilityMonitorHelper();
    void RegisterAccessiblilityBalance();
    void RegisterAccessiblilityMono();
    void OnMonoAudioConfigChanged(bool audioMono);
    void RegisterNameMonitorHelper();
    void SetPlaybackStreamFlag(std::shared_ptr<AudioStreamDescriptor> streamDesc);
    void SetRecordStreamFlag(std::shared_ptr<AudioStreamDescriptor> streamDesc);
    void GetPlaybackStreamFlag(std::shared_ptr<AudioStreamDescriptor> streamDesc);
    std::vector<SourceOutput> FilterSourceOutputs(int32_t sessionId);
    std::vector<SourceOutput> GetSourceOutputs();
    void UpdateOutputRoute(std::shared_ptr<AudioStreamDescriptor> streamDesc);

    bool IsRingerOrAlarmerDualDevicesRange(const InternalDeviceType &deviceType);
    void OnAudioBalanceChanged(float audioBalance);
    bool GetFastControlParam();

private:
    std::shared_ptr<EventEntry> eventEntry_;
    std::shared_ptr<AudioPolicyServerHandler> audioPolicyServerHandler_ = nullptr;

    AudioActiveDevice& audioActiveDevice_;
    AudioSceneManager& audioSceneManager_;
    AudioVolumeManager& audioVolumeManager_;
    AudioCapturerSession& audioCapturerSession_;
    AudioDeviceManager &audioDeviceManager_;
    AudioConnectedDevice& audioConnectedDevice_;
    AudioDeviceStatus& audioDeviceStatus_;
    AudioConfigManager& audioConfigManager_;
    AudioEffectService& audioEffectService_;
    AudioMicrophoneDescriptor& audioMicrophoneDescriptor_;
    AudioRecoveryDevice& audioRecoveryDevice_;
    AudioRouterCenter& audioRouterCenter_;
    AudioStreamCollector& streamCollector_;
    AudioStateManager &audioStateManager_;
    AudioDeviceCommon& audioDeviceCommon_;
    AudioOffloadStream& audioOffloadStream_;
    AudioA2dpOffloadFlag& audioA2dpOffloadFlag_;
    IAudioPolicyInterface& audioPolicyManager_;
    AudioRouteMap& audioRouteMap_;
    AudioIOHandleMap& audioIOHandleMap_;

    std::shared_ptr<AudioA2dpOffloadManager> audioA2dpOffloadManager_ = nullptr;
    std::shared_ptr<DeviceStatusListener> deviceStatusListener_;

    static bool isBtListenerRegistered;
    static constexpr int32_t MIN_SERVICE_COUNT = 2;
    std::bitset<MIN_SERVICE_COUNT> serviceFlag_;
    bool isCurrentRemoteRenderer_ = false;
    bool isOpenRemoteDevice = false;
    int32_t enableDualHalToneSessionId_ = -1;
    bool enableDualHalToneState_ = false;
    int32_t shouldUpdateDeviceDueToDualTone_ = false;
    bool isFastControlled_ = true;
    std::mutex serviceFlagMutex_;
};
static std::string GetEncryptAddr(const std::string &addr);
}
}
#endif // AUDIO_CORE_SERVICE_H
