/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

import audio from '@ohos.multimedia.audio';
import { describe, beforeAll, beforeEach, afterEach, afterAll, it, expect } from 'deccjsunit/index'


describe("AudioVolumeManagerJsUnitTest", function () {
    let audioManager = audio.getAudioManager();
    let audioVolumeManager = audioManager.getVolumeManager();
    let audioVolumeGroupManager = audioManager.getVolumeGroupManagerSync();

    beforeAll(async function () {

        // input testsuit setup step，setup invoked before all testcases
        console.info('AudioVolumeManagerJsUnitTest:beforeAll called')
    })

    afterAll(function () {

        // input testsuit teardown step，teardown invoked after all testcases
        console.info('AudioVolumeManagerJsUnitTest:afterAll called')
    })

    beforeEach(function () {

        // input testcase setup step，setup invoked before each testcases
        console.info('AudioVolumeManagerJsUnitTest:beforeEach called')
    })

    afterEach(function () {

        // input testcase teardown step，teardown invoked after each testcases
        console.info('AudioVolumeManagerJsUnitTest:afterEach called')
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_GROUP_MANAGER_SYNC_001
     * @tc.desc:getVolumeGroupManagerSync success
     * @tc.type: FUNC
     * @tc.require: I7V04L
     */
    it("SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_GROUP_MANAGER_SYNC_001", 0, async function (done) {
        let groupid = audio.DEFAULT_VOLUME_GROUP_ID;
        try {
            let value = audioVolumeManager.getVolumeGroupManagerSync(groupid);
            console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_GROUP_MANAGER_SYNC_001 SUCCESS: ${value}.`);
            expect(typeof value).assertEqual('object');
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_GROUP_MANAGER_SYNC_001 ERROR: ${err}`);
            expect(false).assertTrue();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_GROUP_INFOS_SYNC_001
     * @tc.desc:getVolumeGroupInfosSync success
     * @tc.type: FUNC
     * @tc.require: I7V04L
     */
    it("SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_GROUP_INFOS_SYNC_001", 0, async function (done) {
        try {
            let value = audioVolumeManager.getVolumeGroupInfosSync(audio.LOCAL_NETWORK_ID);
            console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_GROUP_INFOS_SYNC_001 SUCCESS: ${value}.`);
            expect(value.length).assertLarger(0);
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_GROUP_INFOS_SYNC_001 ERROR: ${err}`);
            expect(false).assertTrue();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_SYSTEM_VOLUME_001
     * @tc.desc: 入参正确，能正确获取音量
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_GET_SYSTEM_VOLUME_001", 0, async function (done) {
        try {
            let value = audioVolumeManager.getSystemVolume(audio.AudioVolumeType.MUSIC);
            console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_SYSTEM_VOLUME_001 SUCCESS: ${value}.`);
            expect(value >= MIN_VOLUME_LEVEL && value <= MAX_VOLUME_LEVEL).assertTrue();
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_SYSTEM_VOLUME_001 ERROR: ${err}`);
            expect(false).assertTrue();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_SYSTEM_VOLUME_002
     * @tc.desc: 入参数量错误，抛出异常
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    // it("SUB_AUDIO_VOLUME_MANAGER_GET_SYSTEM_VOLUME_002", 0, async function (done) {
    //     try {
    //         let value = audioVolumeManager.getSystemVolume();
    //         console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_SYSTEM_VOLUME_002 SUCCESS: ${value}.`);
    //         expect(value >= MIN_VOLUME_LEVEL && value <= MAX_VOLUME_LEVEL).assertTrue();
    //         done();
    //     } catch (err) {
    //         console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_SYSTEM_VOLUME_002 ERROR: ${err}`);
    //         expect(false).assertTrue();
    //         done();
    //     }
    // })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_SYSTEM_VOLUME_003
     * @tc.desc: 入参类型错误，抛出异常
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    // it("SUB_AUDIO_VOLUME_MANAGER_GET_SYSTEM_VOLUME_003", 0, async function (done) {
    //     try {
    //         let value = audioVolumeManager.getSystemVolume("audio.AudioVolumeType.MUSIC");
    //         console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_SYSTEM_VOLUME_003 SUCCESS: ${value}.`);
    //         expect(value >= MIN_VOLUME_LEVEL && value <= MAX_VOLUME_LEVEL).assertTrue();
    //         done();
    //     } catch (err) {
    //         console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_SYSTEM_VOLUME_003 ERROR: ${err}`);
    //         expect(false).assertTrue();
    //         done();
    //     }
    // })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_SYSTEM_VOLUME_004
     * @tc.desc: 入参枚举值范围错误，抛出异常
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    // it("SUB_AUDIO_VOLUME_MANAGER_GET_SYSTEM_VOLUME_004", 0, async function (done) {
    //     try {
    //         let value = audioVolumeManager.getSystemVolume(1000);
    //         console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_SYSTEM_VOLUME_004 SUCCESS: ${value}.`);
    //         expect(value >= MIN_VOLUME_LEVEL && value <= MAX_VOLUME_LEVEL).assertTrue();
    //         done();
    //     } catch (err) {
    //         console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_SYSTEM_VOLUME_004 ERROR: ${err}`);
    //         expect(false).assertTrue();
    //         done();
    //     }
    // })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_MIN_SYSTEM_VOLUME_001
     * @tc.desc:getMinSystemVolume success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_GET_MIN_SYSTEM_VOLUME_001", 0, async function (done) {
        try {
            let value = audioVolumeManager.getMinSystemVolume(audio.AudioVolumeType.MUSIC);
            console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_MIN_SYSTEM_VOLUME_001 SUCCESS: ${value}.`);
            expect(value == MIN_VOLUME_LEVEL).assertTrue();
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_MIN_SYSTEM_VOLUME_001 ERROR: ${err}`);
            expect(false).assertTrue();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_MIN_SYSTEM_VOLUME_001
     * @tc.desc:getMinSystemVolume success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    // it("SUB_AUDIO_VOLUME_MANAGER_GET_MIN_SYSTEM_VOLUME_001", 0, async function (done) {
    //     try {
    //         let value = audioVolumeManager.getMinSystemVolume();
    //         console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_MIN_SYSTEM_VOLUME_001 SUCCESS: ${value}.`);
    //         expect(value == MIN_VOLUME_LEVEL).assertTrue();
    //         done();
    //     } catch (err) {
    //         console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_MIN_SYSTEM_VOLUME_001 ERROR: ${err}`);
    //         expect(false).assertTrue();
    //         done();
    //     }
    // })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_MIN_SYSTEM_VOLUME_001
     * @tc.desc:getMinSystemVolume success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    // it("SUB_AUDIO_VOLUME_MANAGER_GET_MIN_SYSTEM_VOLUME_001", 0, async function (done) {
    //     try {
    //         let value = audioVolumeManager.getMinSystemVolume("audio.AudioVolumeType.MUSIC");
    //         console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_MIN_SYSTEM_VOLUME_001 SUCCESS: ${value}.`);
    //         expect(value == MIN_VOLUME_LEVEL).assertTrue();
    //         done();
    //     } catch (err) {
    //         console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_MIN_SYSTEM_VOLUME_001 ERROR: ${err}`);
    //         expect(false).assertTrue();
    //         done();
    //     }
    // })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_MIN_SYSTEM_VOLUME_001
     * @tc.desc:getMinSystemVolume success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    // it("SUB_AUDIO_VOLUME_MANAGER_GET_MIN_SYSTEM_VOLUME_001", 0, async function (done) {
    //     try {
    //         let value = audioVolumeManager.getMinSystemVolume(10001);
    //         console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_MIN_SYSTEM_VOLUME_001 SUCCESS: ${value}.`);
    //         expect(value == MIN_VOLUME_LEVEL).assertTrue();
    //         done();
    //     } catch (err) {
    //         console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_MIN_SYSTEM_VOLUME_001 ERROR: ${err}`);
    //         expect(false).assertTrue();
    //         done();
    //     }
    // })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_MAX_SYSTEM_VOLUME_001
     * @tc.desc:getMaxSystemVolume success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_GET_MAX_SYSTEM_VOLUME_001", 0, async function (done) {
        try {
            let value = audioVolumeManager.getMaxSystemVolume(audio.AudioVolumeType.MUSIC);
            console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_MAX_SYSTEM_VOLUME_001 SUCCESS: ${value}.`);
            expect(value == MAX_VOLUME_LEVEL).assertTrue();
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_MAX_SYSTEM_VOLUME_001 ERROR: ${err}`);
            expect(false).assertTrue();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_MAX_SYSTEM_VOLUME_001
     * @tc.desc:getMaxSystemVolume success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    // it("SUB_AUDIO_VOLUME_MANAGER_GET_MAX_SYSTEM_VOLUME_001", 0, async function (done) {
    //     try {
    //         let value = audioVolumeManager.getMaxSystemVolume();
    //         console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_MAX_SYSTEM_VOLUME_001 SUCCESS: ${value}.`);
    //         expect(value == MAX_VOLUME_LEVEL).assertTrue();
    //         done();
    //     } catch (err) {
    //         console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_MAX_SYSTEM_VOLUME_001 ERROR: ${err}`);
    //         expect(false).assertTrue();
    //         done();
    //     }
    // })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_MAX_SYSTEM_VOLUME_001
     * @tc.desc:getMaxSystemVolume success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    // it("SUB_AUDIO_VOLUME_MANAGER_GET_MAX_SYSTEM_VOLUME_001", 0, async function (done) {
    //     try {
    //         let value = audioVolumeManager.getMaxSystemVolume("audio.AudioVolumeType.MUSIC");
    //         console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_MAX_SYSTEM_VOLUME_001 SUCCESS: ${value}.`);
    //         expect(value == MAX_VOLUME_LEVEL).assertTrue();
    //         done();
    //     } catch (err) {
    //         console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_MAX_SYSTEM_VOLUME_001 ERROR: ${err}`);
    //         expect(false).assertTrue();
    //         done();
    //     }
    // })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_MAX_SYSTEM_VOLUME_001
     * @tc.desc:getMaxSystemVolume success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    // it("SUB_AUDIO_VOLUME_MANAGER_GET_MAX_SYSTEM_VOLUME_001", 0, async function (done) {
    //     try {
    //         let value = audioVolumeManager.getMaxSystemVolume(10001);
    //         console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_MAX_SYSTEM_VOLUME_001 SUCCESS: ${value}.`);
    //         expect(value == MAX_VOLUME_LEVEL).assertTrue();
    //         done();
    //     } catch (err) {
    //         console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_MAX_SYSTEM_VOLUME_001 ERROR: ${err}`);
    //         expect(false).assertTrue();
    //         done();
    //     }
    // })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_001
     * @tc.desc:isSystemMuted success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_001", 0, async function (done) {
        try {
            audioVolumeGroupManager.mute(audio.AudioVolumeType.MUSIC, true, () => {
                let value = audioVolumeManager.isSystemMuted(audio.AudioVolumeType.MUSIC);
                console.info(`SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_001 SUCCESS: ${value}.`);
                expect(value).assertTrue();
                done();
            })
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_001 ERROR: ${err}`);
            expect(false).assertTrue();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_001
     * @tc.desc:isSystemMuted success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    // it("SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_001", 0, async function (done) {
    //     try {
    //         audioVolumeGroupManager.mute(audio.AudioVolumeType.MUSIC, true, () => {
    //             let value = audioVolumeManager.isSystemMuted();
    //             console.info(`SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_001 SUCCESS: ${value}.`);
    //             expect(value).assertTrue();
    //             done();
    //         })
    //     } catch (err) {
    //         console.error(`SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_001 ERROR: ${err}`);
    //         expect(false).assertTrue();
    //         done();
    //     }
    // })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_001
     * @tc.desc:isSystemMuted success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    // it("SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_001", 0, async function (done) {
    //     try {
    //         audioVolumeGroupManager.mute(audio.AudioVolumeType.MUSIC, true, () => {
    //             let value = audioVolumeManager.isSystemMuted("audio.AudioVolumeType.MUSIC");
    //             console.info(`SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_001 SUCCESS: ${value}.`);
    //             expect(value).assertTrue();
    //             done();
    //         })
    //     } catch (err) {
    //         console.error(`SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_001 ERROR: ${err}`);
    //         expect(false).assertTrue();
    //         done();
    //     }
    // })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_001
     * @tc.desc:isSystemMuted success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    // it("SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_001", 0, async function (done) {
    //     try {
    //         audioVolumeGroupManager.mute(audio.AudioVolumeType.MUSIC, true, () => {
    //             let value = audioVolumeManager.isSystemMuted("audio.AudioVolumeType.MUSIC");
    //             console.info(`SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_001 SUCCESS: ${value}.`);
    //             expect(value).assertTrue();
    //             done();
    //         })
    //     } catch (err) {
    //         console.error(`SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_001 ERROR: ${err}`);
    //         expect(false).assertTrue();
    //         done();
    //     }
    // })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_IN_UNIT_OF_DB_001
     * @tc.desc:getVolumeInUnitOfDb success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_IN_UNIT_OF_DB_001", 0, async function (done) {
        try {
            let volumeLevel = 5;
            let value = audioVolumeManager.getVolumeInUnitOfDb(audio.AudioVolumeType.MUSIC,
                volumeLevel,
                audio.DeviceType.SPEAKER
            );
            console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_IN_UNIT_OF_DB_001 SUCCESS: ${value}.`);
            expect(typeof value).assertEqual('number');
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_IN_UNIT_OF_DB_001 ERROR: ${err}`);
            expect(false).assertTrue();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_BY_STREAM_001
     * @tc.desc:getVolumeByStream success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_BY_STREAM_001", 0, async function (done) {
        try {
            let value = audioVolumeManager.getVolumeByStream(audio.StreamUsage.STREAM_USAGE_MEDIA);
            console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_BY_STREAM_001 SUCCESS: ${value}.`);
            expect(value >= MIN_VOLUME_LEVEL && value <= MAX_VOLUME_LEVEL).assertTrue();
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_BY_STREAM_001 ERROR: ${err}`);
            expect(false).assertTrue();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_MIN_VOLUME_BY_STREAM_001
     * @tc.desc:getMinVolumeByStream success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_GET_MIN_VOLUME_BY_STREAM_001", 0, async function (done) {
        try {
            let value = audioVolumeManager.getMinVolumeByStream(audio.StreamUsage.STREAM_USAGE_MEDIA);
            console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_MIN_VOLUME_BY_STREAM_001 SUCCESS: ${value}.`);
            expect(value == MIN_VOLUME_LEVEL).assertTrue();
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_MIN_VOLUME_BY_STREAM_001 ERROR: ${err}`);
            expect(false).assertTrue();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_MAX_VOLUME_BY_STREAM_001
     * @tc.desc:getMaxVolumeByStream success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_GET_MAX_VOLUME_BY_STREAM_001", 0, async function (done) {
        try {
            let value = audioVolumeManager.getMaxVolumeByStream(audio.StreamUsage.STREAM_USAGE_MEDIA);
            console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_MAX_VOLUME_BY_STREAM_001 SUCCESS: ${value}.`);
            expect(value == MAX_VOLUME_LEVEL).assertTrue();
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_MAX_VOLUME_BY_STREAM_001 ERROR: ${err}`);
            expect(false).assertTrue();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_FOR_STREAM_001
     * @tc.desc:isSystemMutedForStream success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_FOR_STREAM_001", 0, async function (done) {
        try {
            audioVolumeGroupManager.mute(audio.StreamUsage.STREAM_USAGE_MEDIA, true, () => {
                let value = audioVolumeManager.isSystemMutedForStream(audio.StreamUsage.STREAM_USAGE_MEDIA);
                console.info(`SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_FOR_STREAM_001 SUCCESS: ${value}.`);
                expect(value).assertTrue();
                done();
            })
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_IS_SYSTEM_MUTED_FOR_STREAM_001 ERROR: ${err}`);
            expect(false).assertTrue();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_IN_UNIT_OF_DB_BY_STREAM_001
     * @tc.desc:getVolumeInUnitOfDbByStream success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_IN_UNIT_OF_DB_BY_STREAM_001", 0, async function (done) {
        try {
            let volumeLevel = 8;
            let value = audioVolumeManager.getVolumeInUnitOfDbByStream(audio.StreamUsage.STREAM_USAGE_MEDIA,
                volumeLevel,
                audio.DeviceType.SPEAKER);
            console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_IN_UNIT_OF_DB_BY_STREAM_001 SUCCESS: ${value}.`);
            expect(typeof value).assertEqual('number');
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_VOLUME_IN_UNIT_OF_DB_BY_STREAM_001 ERROR: ${err}`);
            expect(false).assertTrue();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_SUPPORTED_AUDIO_VOLUME_TYPES_001
     * @tc.desc:getSupportedAudioVolumeTypes success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_GET_SUPPORTED_AUDIO_VOLUME_TYPES_001", 0, async function (done) {
        try {
            let value = audioVolumeManager.getSupportedAudioVolumeTypes();
            console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_SUPPORTED_AUDIO_VOLUME_TYPES_001 SUCCESS: ${value}.`);
            expect(value.length).assertLarger(0);
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_SUPPORTED_AUDIO_VOLUME_TYPES_001 ERROR: ${err}`);
            expect(false).assertTrue();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_AUDIO_VOLUME_TYPE_BY_STREAM_USAGE_001
     * @tc.desc:getAudioVolumeTypeByStreamUsage success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_GET_AUDIO_VOLUME_TYPE_BY_STREAM_USAGE_001", 0, async function (done) {
        try {
            let value = audioVolumeManager.getAudioVolumeTypeByStreamUsage(audio.StreamUsage.STREAM_USAGE_MEDIA);
            console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_AUDIO_VOLUME_TYPE_BY_STREAM_USAGE_001 SUCCESS: ${value}.`);
            expect(value.length).assertLarger(0);
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_AUDIO_VOLUME_TYPE_BY_STREAM_USAGE_001 ERROR: ${err}`);
            expect(false).assertTrue();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_GET_STREAM_USAGES_BY_VOLUME_TYPE_001
     * @tc.desc:getStreamUsagesByVolumeType success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_GET_STREAM_USAGES_BY_VOLUME_TYPE_001", 0, async function (done) {
        try {
            let value = audioVolumeManager.getStreamUsagesByVolumeType(audio.AudioVolumeType.MUSIC);
            console.info(`SUB_AUDIO_VOLUME_MANAGER_GET_STREAM_USAGES_BY_VOLUME_TYPE_001 SUCCESS: ${value}.`);
            expect(value.length).assertLarger(0);
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_GET_STREAM_USAGES_BY_VOLUME_TYPE_001 ERROR: ${err}`);
            expect(false).assertTrue();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_ON_SYSTEM_VOLUME_CHANGE_001
     * @tc.desc:on system volume change callback execute success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_ON_SYSTEM_VOLUME_CHANGE_001", 0, async function (done) {
        try {
            audioVolumeManager.on("systemVolumeChange", (volumeEvent) => {});
            console.info(`SUB_AUDIO_VOLUME_MANAGER_ON_SYSTEM_VOLUME_CHANGE_001 SUCCESS.`);
            expect(true).assertTrue();
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_ON_SYSTEM_VOLUME_CHANGE_001 ERROR: ${err}`);
            expect().assertFail();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_OFF_SYSTEM_VOLUME_CHANGE_001
     * @tc.desc:off system volume change callback execute success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_OFF_SYSTEM_VOLUME_CHANGE_001", 0, async function (done) {
        try {
            audioVolumeManager.off("systemVolumeChange", (volumeEvent) => {});
            console.info(`SUB_AUDIO_VOLUME_MANAGER_OFF_SYSTEM_VOLUME_CHANGE_001 SUCCESS.`);
            expect(true).assertTrue();
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_OFF_SYSTEM_VOLUME_CHANGE_001 ERROR: ${err}`);
            expect().assertFail();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_OFF_SYSTEM_VOLUME_CHANGE_002
     * @tc.desc:off system volume change callback execute success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_OFF_SYSTEM_VOLUME_CHANGE_002", 0, async function (done) {
        try {
            audioVolumeManager.off("systemVolumeChange");
            console.info(`SUB_AUDIO_VOLUME_MANAGER_OFF_SYSTEM_VOLUME_CHANGE_002 SUCCESS.`);
            expect(true).assertTrue();
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_OFF_SYSTEM_VOLUME_CHANGE_002 ERROR: ${err}`);
            expect().assertFail();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_ON_STREAM_VOLUME_CHANGE_001
     * @tc.desc:on stream volume change callback execute success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_ON_STREAM_VOLUME_CHANGE_001", 0, async function (done) {
        try {
            audioVolumeManager.on("streamVolumeChange", (streamVolumeEvent) => {});
            console.info(`SUB_AUDIO_VOLUME_MANAGER_ON_STREAM_VOLUME_CHANGE_001 SUCCESS.`);
            expect(true).assertTrue();
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_ON_STREAM_VOLUME_CHANGE_001 ERROR: ${err}`);
            expect().assertFail();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_OFF_STREAM_VOLUME_CHANGE_001
     * @tc.desc:off stream volume change callback execute success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_OFF_STREAM_VOLUME_CHANGE_001", 0, async function (done) {
        try {
            audioVolumeManager.off("streamVolumeChange", (streamVolumeEvent) => {});
            console.info(`SUB_AUDIO_VOLUME_MANAGER_OFF_STREAM_VOLUME_CHANGE_001 SUCCESS.`);
            expect(true).assertTrue();
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_OFF_STREAM_VOLUME_CHANGE_001 ERROR: ${err}`);
            expect().assertFail();
            done();
        }
    })

    /*
     * @tc.name:SUB_AUDIO_VOLUME_MANAGER_OFF_STREAM_VOLUME_CHANGE_002
     * @tc.desc:off stream volume change callback execute success
     * @tc.type: FUNC
     * @tc.require: ICH6FD
     */
    it("SUB_AUDIO_VOLUME_MANAGER_OFF_STREAM_VOLUME_CHANGE_002", 0, async function (done) {
        try {
            audioVolumeManager.off("streamVolumeChange");
            console.info(`SUB_AUDIO_VOLUME_MANAGER_OFF_STREAM_VOLUME_CHANGE_002 SUCCESS.`);
            expect(true).assertTrue();
            done();
        } catch (err) {
            console.error(`SUB_AUDIO_VOLUME_MANAGER_OFF_STREAM_VOLUME_CHANGE_002 ERROR: ${err}`);
            expect().assertFail();
            done();
        }
    })
})