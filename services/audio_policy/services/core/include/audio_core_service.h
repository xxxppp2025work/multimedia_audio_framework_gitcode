#ifndef AUDIO_CORE_SERVICE_H
#define AUDIO_CORE_SERVICE_H
#include <mutex>

#include "core_service_provider_stub.h"

class AudioCoreService : public enable_shared_from_this<AudioCoreService>{
public:
    AudioCoreService();
    ~AudioCoreService();

    class EventEntry : public ICoreServiceProvider, IDeviceStatusObserver {
    public:
        EventEntry(std::shared_ptr<AudioCoreService> coreService);

        int32_t CreateClient(AudioStreamDescriptor &streamDesc, AudioFlag &audioFlag);

        int32_t StartClient(uint32_t sessionId) override;
        int32_t RemoveClient(uint32_t sessionId) override;

        int32_t SetAudioScene(AudioScene audioScene);
        bool IsArmUsbDevice(const AudioDeviceDescriptor &deviceDesc);
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> GetDevices(DeviceFlag deviceFlag);
        int32_t SetDeviceActive(InternalDeviceType deviceType, bool active);

        // device status listener
        void OnDeviceStatusUpdated(DeviceType devType, bool isConnected,
            const std::string &macAddress, const std::string &deviceName,
            const AudioStreamInfo &streamInfo, DeviceRole role = DEVICE_ROLE_NONE) override;
        void OnDeviceStatusUpdated(AudioDeviceDescriptor &desc, bool isConnected) override;
        void OnDeviceStatusUpdated(DStatusInfo statusInfo, bool isStop = false) override;
        void OnMicrophoneBlockedUpdate(DeviceType devType, DeviceBlockStatus status) override;
        void OnPnpDeviceStatusUpdated(AudioDeviceDescriptor &desc, bool isConnected) override;
        void OnDeviceConfigurationChanged(DeviceType deviceType, const std::string &macAddress,
            const std::string &deviceName, const AudioStreamInfo &streamInfo) override;
        bool ConnectServiceAdapter();
        void OnServiceConnected(AudioServiceIndex serviceIndex) override;
#ifdef HAS_FEATURE_INNERCAPTURER
        void LoadModernInnerCapSink();
#endif
        void LoadHdiEffectModel();
        void OnServiceDisconnected(AudioServiceIndex serviceIndex) override;
        void OnForcedDeviceSelected(DeviceType devType, const std::string &macAddress) override;
        void OnDeviceInfoUpdated(AudioDeviceDescriptor &desc, const DeviceInfoUpdateCommand command) override;
        // device status listener end

        vector<sptr<MicrophoneDescriptor>> GetAvailableMicrophones();
        std::vector<sptr<MicrophoneDescriptor>> GetAudioCapturerMicrophoneDescriptors(int32_t sessionId);
        void OnReceiveBluetoothEvent(const std::string macAddress, const std::string deviceName);




    private:
        std::shared_ptr<AudioCoreService> coreService_;
        std::mutex eventMutex_;
    };

    std::shared_ptr<AudioCoreService> GetAudioCoreService()
    std::shared_ptr<EventEntry> GetEventEntry();
    void RegiestCoreService();

    void SetCallbackHandler(std::shared_ptr<AudioPolicyServerHandler> handler);

private:
    int32_t CreateClient(AudioStreamDescriptor &streamDesc, AudioFlag &audioFlag);
    int32_t StartClient(uint32_t sessionId);
    int32_t RemoveClient(uint32_t sessionId);

    int32_t SetAudioScene(AudioScene audioScene);
    bool IsArmUsbDevice(const AudioDeviceDescriptor &deviceDesc);
    std::vector<std::shared_ptr<AudioDeviceDescriptor>> GetDevices(DeviceFlag deviceFlag);
    int32_t SetDeviceActive(InternalDeviceType deviceType, bool active);


    // device status listener inner
    void OnDeviceStatusUpdated(DeviceType devType, bool isConnected,
        const std::string &macAddress, const std::string &deviceName,
        const AudioStreamInfo &streamInfo, DeviceRole role = DEVICE_ROLE_NONE);
    void OnDeviceStatusUpdated(AudioDeviceDescriptor &desc, bool isConnected);
    void OnDeviceStatusUpdated(DStatusInfo statusInfo, bool isStop = false);
    void OnMicrophoneBlockedUpdate(DeviceType devType, DeviceBlockStatus status);
    void OnPnpDeviceStatusUpdated(AudioDeviceDescriptor &desc, bool isConnected);
    void OnDeviceConfigurationChanged(DeviceType deviceType, const std::string &macAddress,
        const std::string &deviceName, const AudioStreamInfo &streamInfo);
    void OnServiceConnected(AudioServiceIndex serviceIndex);
    // device status listener inner end

    std::vector<sptr<MicrophoneDescriptor>> GetAvailableMicrophones();
    std::vector<sptr<MicrophoneDescriptor>> GetAudioCapturerMicrophoneDescriptors(int32_t sessionId);
    void OnReceiveBluetoothEvent(const std::string macAddress, const std::string deviceName);


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

    std::shared_ptr<DeviceStatusListener> deviceStatusListener_;

    static bool isBtListenerRegistered;
    std::bitset<MIN_SERVICE_COUNT> serviceFlag_;
};

#endif // AUDIO_CORE_SERVICE_H
