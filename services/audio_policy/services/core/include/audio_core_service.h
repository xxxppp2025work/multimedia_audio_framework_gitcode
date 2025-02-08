#ifndef AUDIO_CORE_SERVICE_H
#define AUDIO_CORE_SERVICE_H
#include <mutex>

#include "core_service_provider_stub.h"

class AudioCoreService {
public:
    AudioCoreService();
    ~AudioCoreService();

    class EventEntry : public ICoreServiceProvider {
    public:
        EventEntry(std::shared_ptr<AudioCoreService> coreService);

        int32_t CreateClient(AudioStreamDescriptor &streamDesc, AudioFlag &audioFlag);

        int32_t StartClient(uint32_t sessionId) override;
        int32_t RemoveClient(uint32_t sessionId) override;

        int32_t SetAudioScene(AudioScene audioScene);
        bool IsArmUsbDevice(const AudioDeviceDescriptor &deviceDesc);
        std::vector<std::shared_ptr<AudioDeviceDescriptor>> GetDevices(DeviceFlag deviceFlag);
        int32_t SetDeviceActive(InternalDeviceType deviceType, bool active);

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


    std::shared_ptr<EventEntry> eventEntry_;
    std::shared_ptr<AudioPolicyServerHandler> audioPolicyServerHandler_ = nullptr;


    AudioActiveDevice& audioActiveDevice_;
    AudioSceneManager& audioSceneManager_;
    AudioVolumeManager& audioVolumeManager_;
    AudioCapturerSession& audioCapturerSession_;
    AudioDeviceManager &audioDeviceManager_;
    AudioConnectedDevice& audioConnectedDevice_;
};

#endif // AUDIO_CORE_SERVICE_H
