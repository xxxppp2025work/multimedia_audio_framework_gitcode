/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 #ifndef INJECTOR_H
 #define INJECTOR_H

 #include <set>
 #include "audio_module_info.h"
 #include "audio_iohandle_map.h"
 #include "iaudio_policy_interface.h"

 namespace OHOS {
 namespace AudioStandard {
    class AudioInjector {
        public:
            static std::shared_ptr<AudioInjector> Create()
            {
                static std::shared_ptr<AudioInjector> instance(new AudioInjector());
                return instance;
            }
            int32_t Init();
            int32_t DeInit();
            int32_t UpdateAudioInfo(AudioModuleInfo &Info);
            int32_t MoveIn(uint32_t renderId, bool flag);
            int32_t PeekAudioData(uint32_t streamid, uint8_t *destPtr, size_t dataSize);
            int32_t GetRenderCount();
            void SetCapIdx(uint32_t idx);
            uint32_t GetCapIdx();
        private:
            AudioInjector();
            AudioInjector(const AudioInjector&) = delete;
            AudioInjector& operator=(const AudioInjector&) = delete;
        private:
            AudioModuleInfo moduleInfo_;
            uint32_t capturePortIdx_;
            uint32_t renderPortIdx_;
            bool isConnected_;
            std::unordered_map<uint32_t, std::string> renderIdMap_ = {};
            AudioIOHandleMap& audioIOHandleMap_;
            IAudioPolicyInterface& audioPolicyManager_;
    };
 } //  namespace AudioStandard
 } //  namespace OHOS
 #endif  // INJECTOR_H