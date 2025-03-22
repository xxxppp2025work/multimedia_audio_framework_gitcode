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

 #include "remote_audio_renderer_sink_unit_test.h"
 
 #include "remote_audio_renderer_sink.cpp"
 
 using namespace std;
 using namespace testing::ext;
 using namespace testing;
 
 namespace OHOS {
 namespace AudioStandard {
 
 void RemoteAudioRendererSinkUnitTest::SetUpTestCase(void) {}
 void RemoteAudioRendererSinkUnitTest::TearDownTestCase(void) {}
 void RemoteAudioRendererSinkUnitTest::SetUp(void) {}
 void RemoteAudioRendererSinkUnitTest::TearDown(void) {}
 
 /**
  * @tc.name  : Test Template RemoteAudioRendererSink
  * @tc.number: RemoteAudioRendererSinkUnitTest_001
  * @tc.desc  : Test Template RemoteAudioRendererSink SetAudioPortMap Interface.
  */
 HWTEST(RemoteAudioRendererSinkUnitTest, RemoteAudioRendererSinkUnitTest_001, TestSize.Level1)
 {
    RemoteAudioRendererSink *remoteAudioRendererSink = RemoteAudioRendererSink::GetInstance();
    AudioAdapterDescriptor desc = {};
    vector<string> splitStreamVector = {MEDIA_STREAM_TYPE, COMMUNICATION_STREAM_TYPE, NAVIGATION_STREAM_TYPE};
    uint32_t port = 0;
    int ret = remoteAudioRendererSink->SetAudioPortMap(desc, splitStreamVector, port);
    EXPECT_EQ(ret, SUCCESS);
 }
 
 /**
  * @tc.name  : Test Template RemoteAudioRendererSink
  * @tc.number: RemoteAudioRendererSinkUnitTest_002
  * @tc.desc  : Test Template RemoteAudioRendererSink Init Interface.
  */
 HWTEST(RemoteAudioRendererSinkUnitTest, RemoteAudioRendererSinkUnitTest_001, TestSize.Level1)
 {
    RemoteAudioRendererSink *remoteAudioRendererSink = RemoteAudioRendererSink::GetInstance();
    const IAudioSinkAttr attr = {};
    int32_t ret = remoteAudioRendererSink->Init(attr);
    EXPECT_EQ(ret, SUCCESS);
 }
 } // namespace AudioStandard
 } // namespace OHOS
 