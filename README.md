The audio framework is used to implement audio-related features, including audio playback, audio recording, volume management, and device management.

### Basic Concepts

- **Sampling**: A process to obtain discrete-time signals by extracting samples from analog signals in a continuous time domain at a specific interval.
- **Sampling Rate**: The number of samples extracted per second from a continuous signal to form a discrete signal. It is measured in Hz. Common sampling rates include 8 kHz, 11.025 kHz, 22.05 kHz, 16 kHz, 37.8 kHz, 44.1 kHz, 48 kHz, 96 kHz, and 192 kHz.
- **Channel**: Refers to different spatial positions where independent audio signals are recorded or played. The number of channels indicates the number of audio sources used during recording or the number of speakers used during playback.
- **Audio Frame**: Represents a data unit in audio processing, typically containing a small duration of audio data (2.5 to 60 milliseconds). This unit is referred to as a sampling time.
- **PCM (Pulse Code Modulation)**: A method used to digitally represent sampled analog signals. It converts continuous-time analog signals into discrete-time digital signal samples.

### Directory Structure

The repository structure is as follows:

```
/foundation/multimedia/audio_standard # Service code of the audio framework
├── frameworks                         # Framework code
│   ├── native                         # Internal native API implementation
│   └── js                             # External JS API implementation
│       └── napi                       # External native API implementation
├── interfaces                         # API code
│   ├── inner_api                      # Internal APIs
│   └── kits                           # External APIs
├── sa_profile                         # Service configuration profile
├── services                           # Service code
├── LICENSE                            # License file
└── bundle.json                        # Build file
```

### Usage Guidelines

#### Audio Playback

To implement audio playback:

1. Create an `AudioRenderer` instance with the required stream type.
   ```cpp
   AudioStreamType streamType = STREAM_MUSIC;
   std::unique_ptr<AudioRenderer> audioRenderer = AudioRenderer::Create(streamType);
   ```

2. (Optional) Use static APIs like `GetSupportedFormats()`, `GetSupportedChannels()`, `GetSupportedEncodingTypes()`, and `GetSupportedSamplingRates()` to determine supported parameters.

3. Set parameters using `SetParams()`:
   ```cpp
   AudioRendererParams rendererParams;
   rendererParams.sampleFormat = SAMPLE_S16LE;
   rendererParams.sampleRate = SAMPLE_RATE_44100;
   rendererParams.channelCount = STEREO;
   rendererParams.encodingType = ENCODING_PCM;

   audioRenderer->SetParams(rendererParams);
   ```

4. (Optional) Use `GetParams(rendererParams)` to retrieve the set parameters.

5. Start the playback task with `Start()`.

6. Use `GetBufferSize()` to determine the buffer length for writing data.
   ```cpp
   audioRenderer->GetBufferSize(bufferLen);
   ```

7. Use `Write(buffer, bytesToWrite)` to pass the audio data to a byte stream. This API can be called repeatedly to write data.

8. Call `Drain()` to clear the stream before stopping.

9. Stop the playback with `Stop()`.

10. Release resources using `Release()` after playback completes.

11. Use `SetVolume(float)` and `GetVolume()` to adjust and retrieve the volume of the audio stream, which ranges from 0.0 to 1.0.

For more details, refer to [audio_renderer.h](https://gitee.com/openharmony/multimedia_audio_framework/blob/master/interfaces/inner_api/native/audiorenderer/include/audio_renderer.h) and [audio_info.h](https://gitee.com/openharmony/multimedia_audio_framework/blob/master/interfaces/inner_api/native/audiocommon/include/audio_info.h).

#### Audio Recording

To implement audio recording:

1. Create an `AudioCapturer` instance with the required stream type.
   ```cpp
   AudioStreamType streamType = STREAM_MUSIC;
   std::unique_ptr<AudioCapturer> audioCapturer = AudioCapturer::Create(streamType);
   ```

2. (Optional) Use static APIs like `GetSupportedFormats()`, `GetSupportedChannels()`, `GetSupportedEncodingTypes()`, and `GetSupportedSamplingRates()` to determine supported parameters.

3. Set parameters using `SetParams()`:
   ```cpp
   AudioCapturerParams capturerParams;
   capturerParams.sampleFormat = SAMPLE_S16LE;
   capturerParams.sampleRate = SAMPLE_RATE_44100;
   capturerParams.channelCount = STEREO;
   capturerParams.encodingType = ENCODING_PCM;

   audioCapturer->SetParams(capturerParams);
   ```

4. (Optional) Use `GetParams(capturerParams)` to retrieve the set parameters.

5. Start the recording task with `Start()`.

6. Use `GetBufferSize()` to determine the buffer length for reading data.
   ```cpp
   audioCapturer->GetBufferSize(bufferLen);
   ```

7. Use `Read(buffer, bufferLen, isBlocking)` to read captured audio data. This API can be called repeatedly until manually stopped.
   ```cpp
   bytesRead = audioCapturer->Read(*buffer, bufferLen, isBlockingRead);
   while (numBuffersToCapture) {
       bytesRead = audioCapturer->Read(*buffer, bufferLen, isBlockingRead);
       if (bytesRead < 0) {
           break;
       } else if (bytesRead > 0) {
           fwrite(buffer, size, bytesRead, recFile); // Writes the recorded data into a file
           numBuffersToCapture--;
       }
   }
   ```

8. (Optional) Use `Flush()` to clear the recording stream buffer.

9. Stop the recording with `Stop()`.

10. Release resources using `Release()` after recording completes.

For more details, refer to [audio_capturer.h](https://gitee.com/openharmony/multimedia_audio_framework/blob/master/interfaces/inner_api/native/audiocapturer/include/audio_capturer.h) and [audio_info.h](https://gitee.com/openharmony/multimedia_audio_framework/blob/master/interfaces/inner_api/native/audiocommon/include/audio_info.h).

#### Audio Management

Use the APIs in [audio_system_manager.h](https://gitee.com/openharmony/multimedia_audio_framework/blob/master/interfaces/inner_api/native/audiomanager/include/audio_system_manager.h) to manage volume and devices.

1. Obtain an `AudioSystemManager` instance:
   ```cpp
   AudioSystemManager *audioSystemMgr = AudioSystemManager::GetInstance();
   ```

##### Volume Control

2. Use `GetMaxVolume()` and `GetMinVolume()` to obtain the allowed volume range for a stream.
   ```cpp
   AudioVolumeType streamType = AudioVolumeType::STREAM_MUSIC;
   int32_t maxVol = audioSystemMgr->GetMaxVolume(streamType);
   int32_t minVol = audioSystemMgr->GetMinVolume(streamType);
   ```

3. Use `SetVolume()` and `GetVolume()` to set and retrieve the volume of the audio stream.
   ```cpp
   int32_t result = audioSystemMgr->SetVolume(streamType, 10);
   int32_t vol = audioSystemMgr->GetVolume(streamType);
   ```

4. Use `SetMute()` and `IsStreamMute()` to set and retrieve the mute status of the audio stream.
   ```cpp
   int32_t result = audioSystemMgr->SetMute(streamType, true);
   bool isMute = audioSystemMgr->IsStreamMute(streamType);
   ```

5. Use `SetRingerMode()` and `GetRingerMode()` to set and retrieve the ringer mode. The supported ringer modes are defined in `AudioRingerMode` in [audio_info.h](https://gitee.com/openharmony/multimedia_audio_framework/blob/master/interfaces/inner_api/native/audiocommon/include/audio_info.h).

6. Use `SetMicrophoneMute()` and `IsMicrophoneMute()` to set and retrieve the mute status of the microphone.
   ```cpp
   int32_t result = audioSystemMgr->SetMicrophoneMute(true);
   bool isMicMute = audioSystemMgr->IsMicrophoneMute();
   ```

##### Device Control

7. Use `GetDevices(deviceFlag)` to obtain information about audio input and output devices. Use `deviceType_` and `deviceRole_` to identify the device type and role.
   ```cpp
   DeviceFlag deviceFlag = OUTPUT_DEVICES_FLAG;
   vector<std::shared_ptr<AudioDeviceDescriptor>> audioDeviceDescriptors = audioSystemMgr->GetDevices(deviceFlag);
   std::shared_ptr<AudioDeviceDescriptor> audioDeviceDescriptor = audioDeviceDescriptors[0];
   cout << audioDeviceDescriptor->deviceType_;
   cout << audioDeviceDescriptor->deviceRole_;
   ```

8. Use `SetDeviceActive()` and `IsDeviceActive()` to activate or deactivate an audio device and check its activation status.
   ```cpp
   DeviceType deviceType = DeviceType::DEVICE_TYPE_SPEAKER;
   int32_t result = audioSystemMgr->SetDeviceActive(deviceType, true);
   bool isDevActive = audioSystemMgr->IsDeviceActive(deviceType);
   ```

9. Use other APIs like `IsStreamActive()`, `SetAudioParameter()`, and `GetAudioParameter()` if needed.

10. Use `AudioManagerNapi::On` to subscribe to system volume changes. The following parameters are used to notify the application of a volume change:
    - `volumeType`: Type of the system volume changed.
    - `volume`: Current volume level.
    - `updateUi`: Indicates whether to show the change on the UI.

    ```cpp
    const audioManager = audio.getAudioManager();

    export default {
      onCreate() {
        audioManager.on('volumeChange', (volumeChange) ==> {
          console.info('volumeType = '+volumeChange.volumeType);
          console.info('volume = '+volumeChange.volume);
          console.info('updateUi = '+volumeChange.updateUi);
        }
      }
    }
    ```

##### Audio Scene

11. Use `SetAudioScene()` and `GetAudioScene()` to set and retrieve the audio scene. For supported audio scenes, refer to the `AudioScene` enumeration in [audio_info.h](https://gitee.com/openharmony/multimedia_audio_framework/blob/master/interfaces/inner_api/native/audiocommon/include/audio_info.h).

##### Audio Stream Management

Use the APIs in [audio_stream_manager.h](https://gitee.com/openharmony/multimedia_audio_framework/blob/master/interfaces/inner_api/native/audiomanager/include/audio_stream_manager.h) to manage audio streams.

1. Obtain an `AudioStreamManager` instance:
   ```cpp
   AudioStreamManager *audioStreamMgr = AudioStreamManager::GetInstance();
   ```

2. Register a listener for renderer state changes using `RegisterAudioRendererEventListener()`. Override `OnRendererStateChange()` in the `AudioRendererStateChangeCallback` class.
   ```cpp
   const int32_t clientPid;

   class RendererStateChangeCallback : public AudioRendererStateChangeCallback {
   public:
       RendererStateChangeCallback() = default;
       ~RendererStateChangeCallback() = default;
       void OnRendererStateChange(
           const std::vector<std::shared_ptr<AudioRendererChangeInfo>> &audioRendererChangeInfos) override
       {
           cout << \"OnRendererStateChange entered\" << endl;
       }
   };

   std::shared_ptr<AudioRendererStateChangeCallback> callback = std::make_shared<RendererStateChangeCallback>();
   int32_t state = audioStreamMgr->RegisterAudioRendererEventListener(clientPid, callback);
   int32_t result = audioStreamMgr->UnregisterAudioRendererEventListener(clientPid);
   ```

3. Register a listener for capturer state changes using `RegisterAudioCapturerEventListener()`. Override `OnCapturerStateChange()` in the `AudioCapturerStateChangeCallback` class.
   ```cpp
   const int32_t clientPid;

   class CapturerStateChangeCallback : public AudioCapturerStateChangeCallback {
   public:
       CapturerStateChangeCallback() = default;
       ~CapturerStateChangeCallback() = default;
       void OnCapturerStateChange(
           const std::vector<std::shared_ptr<AudioCapturerChangeInfo>> &audioCapturerChangeInfos) override
       {
           cout << \"OnCapturerStateChange entered\" << endl;
       }
   };

   std::shared_ptr<AudioCapturerStateChangeCallback> callback = std::make_shared<CapturerStateChangeCallback>();
   int32_t state = audioStreamMgr->RegisterAudioCapturerEventListener(clientPid, callback);
   int32_t result = audioStreamMgr->UnregisterAudioCapturerEventListener(clientPid);
   ```

4. Use `GetCurrentRendererChangeInfos()` to obtain information about all running renderers, including client UID, session ID, renderer info, renderer state, and output device details.
   ```cpp
   std::vector<std::shared_ptr<AudioRendererChangeInfo>> audioRendererChangeInfos;
   int32_t currentRendererChangeInfo = audioStreamMgr->GetCurrentRendererChangeInfos(audioRendererChangeInfos);
   ```

5. Use `GetCurrentCapturerChangeInfos()` to obtain information about all running capturers, including client UID, session ID, capturer info, capturer state, and input device details.
   ```cpp
   std::vector<std::shared_ptr<AudioCapturerChangeInfo>> audioCapturerChangeInfos;
   int32_t currentCapturerChangeInfo = audioStreamMgr->GetCurrentCapturerChangeInfos(audioCapturerChangeInfos);
   ```

6. Use `IsAudioRendererLowLatencySupported()` to check whether low latency is supported.
   ```cpp
   const AudioStreamInfo &audioStreamInfo;
   bool isLatencySupport = audioStreamMgr->IsAudioRendererLowLatencySupported(audioStreamInfo);
   ```

##### Using JavaScript APIs

JavaScript applications can use the audio management APIs to control volume and devices. For more information, see [js-apis-audio.md](https://gitee.com/openharmony/docs/blob/master/en/application-dev/reference/apis-audio-kit/js-apis-audio.md#audiomanager).

##### Bluetooth SCO Call

Use the APIs in [audio_bluetooth_manager.h](https://gitee.com/openharmony/multimedia_audio_framework/blob/master/frameworks/native/bluetoothclient/audio_bluetooth_manager.h) to implement Bluetooth calls over synchronous connection-oriented (SCO) links.

1. Use `OnScoStateChanged()` to listen for SCO link state changes.
   ```cpp
   const BluetoothRemoteDevice &device;
   int state;
   void OnScoStateChanged(const BluetoothRemoteDevice &device, int state);
   ```

2. (Optional) Use `RegisterBluetoothScoAgListener()` to register a Bluetooth SCO listener and `UnregisterBluetoothScoAgListener()` to unregister it when no longer needed.

### Supported Devices

The audio framework supports the following device types:

1. **USB Type-C Headset**: A digital headset with its own DAC and amplifier.
2. **Wired Headset**: An analog headset without a DAC, which may have a 3.5 mm jack or a USB-C socket without DAC.
3. **Bluetooth Headset**: A Bluetooth A2DP headset for wireless audio transmission.
4. **Internal Speaker and MIC**: Devices with built-in speakers and microphones, used as default devices for playback and recording, respectively.

### Repositories Involved

[multimedia_audio_framework](https://gitee.com/openharmony/multimedia_audio_framework)