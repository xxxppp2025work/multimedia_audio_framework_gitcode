/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "format_converter.h"
#include <string>

namespace OHOS {
namespace AudioStandard {
#define PCM_FLOAT_EPS 1e-6f
#define BIT_16 16

static float CapMax(float v)
{
    float value = v;
    if (v >= 1.0f) {
        value = 1.0f - PCM_FLOAT_EPS;
    } else if (v <= -1.0f) {
        value = -1.0f + PCM_FLOAT_EPS;
    }
    return value;
}

static int16_t ConvertFromFloatTo16Bit(const float *a)
{
    float tmp = *a;
    float v = CapMax(tmp) * (1 << (BIT_16 - 1));
    return static_cast<int16_t>(v);
}

int32_t FormatConverter::S16MonoToS16Stereo(const BufferDesc &srcDesc, const BufferDesc &dstDesc)
{
    size_t half = 2; // mono(1) -> stereo(2)
    if (srcDesc.bufLength != dstDesc.bufLength / half || srcDesc.buffer == nullptr || dstDesc.buffer == nullptr) {
        return -1;
    }
    int16_t *stcPtr = reinterpret_cast<int16_t *>(srcDesc.buffer);
    int16_t *dstPtr = reinterpret_cast<int16_t *>(dstDesc.buffer);
    size_t count = srcDesc.bufLength / sizeof(int16_t);
    for (size_t idx = 0; idx < count; idx++) {
        *(dstPtr++) = *stcPtr;
        *(dstPtr++) = *stcPtr++;
    }
    return 0;
}

int32_t FormatConverter::S16StereoToS16Mono(const BufferDesc &srcDesc, const BufferDesc &dstDesc)
{
    size_t half = 2; // stereo(2) -> mono(1)
    if (dstDesc.bufLength != srcDesc.bufLength / half || srcDesc.buffer == nullptr || dstDesc.buffer == nullptr) {
        return -1;
    }
    int16_t *stcPtr = reinterpret_cast<int16_t *>(srcDesc.buffer);
    int16_t *dstPtr = reinterpret_cast<int16_t *>(dstDesc.buffer);
    size_t count = srcDesc.bufLength / half / sizeof(int16_t);
    for (size_t idx = 0; idx < count; idx++) {
        *(dstPtr++) = (*stcPtr + *(stcPtr + 1)) / 2; // To obtain mono channel, add left to right, then divide by 2
        stcPtr += 2; // ptr++ on mono is equivalent to ptr+=2 on stereo
    }
    return 0;
}

int32_t FormatConverter::S16StereoToF32Stereo(const BufferDesc &srcDesc, const BufferDesc &dstDesc)
{
    size_t half = 2;
    if (srcDesc.bufLength != dstDesc.bufLength / half || srcDesc.buffer == nullptr || dstDesc.buffer == nullptr) {
        return -1;
    }
    int16_t *srcPtr = reinterpret_cast<int16_t *>(srcDesc.buffer);
    float *dstPtr = reinterpret_cast<float *>(dstDesc.buffer);
    size_t count = srcDesc.bufLength / sizeof(int16_t);
    const float FLOAT_SCALE = 1.0f / (1 << (BIT_16 - 1));
    for (size_t idx = 0; idx < count; idx++) {
        *dstPtr = (*srcPtr) * FLOAT_SCALE;
        dstPtr++;
        srcPtr++;
    }
    return 0;
}

int32_t FormatConverter::S16StereoToF32Mono(const BufferDesc &srcDesc, const BufferDesc &dstDesc)
{
    size_t half = 2;
    if (srcDesc.bufLength != dstDesc.bufLength || srcDesc.buffer == nullptr || dstDesc.buffer == nullptr) {
        return -1;
    }
    int16_t *srcPtr = reinterpret_cast<int16_t *>(srcDesc.buffer);
    float *dstPtr = reinterpret_cast<float *>(dstDesc.buffer);
    size_t count = srcDesc.bufLength / half / sizeof(int16_t);
    const float FLOAT_SCALE = 1.0f / (1 << (BIT_16 - 1));
    const size_t SRC_INCREMENT = 2;
    for (size_t idx = 0; idx < count; idx++) {
        *dstPtr = (static_cast<float>(*srcPtr + *(srcPtr + 1)) / half) * FLOAT_SCALE;
        dstPtr++;
        srcPtr += SRC_INCREMENT;
    }
    return 0;
}

int32_t FormatConverter::F32MonoToS16Stereo(const BufferDesc &srcDesc, const BufferDesc &dstDesc)
{
    size_t quarter = 4;
    if (srcDesc.bufLength != dstDesc.bufLength || srcDesc.buffer == nullptr || dstDesc.buffer == nullptr ||
        srcDesc.bufLength % quarter != 0) {
        return -1;
    }
    float *stcPtr = reinterpret_cast<float *>(srcDesc.buffer);
    int16_t *dstPtr = reinterpret_cast<int16_t *>(dstDesc.buffer);
    size_t count = srcDesc.bufLength / quarter;

    for (size_t idx = 0; idx < count; idx++) {
        int16_t temp = ConvertFromFloatTo16Bit(stcPtr);
        stcPtr++;
        *(dstPtr++) = temp;
        *(dstPtr++) = temp;
    }
    return 0;
}

int32_t FormatConverter::F32StereoToS16Stereo(const BufferDesc &srcDesc, const BufferDesc &dstDesc)
{
    size_t half = 2;
    if (srcDesc.bufLength / half != dstDesc.bufLength || srcDesc.buffer == nullptr || dstDesc.buffer == nullptr ||
        dstDesc.bufLength % half != 0) {
        return -1;
    }
    float *stcPtr = reinterpret_cast<float *>(srcDesc.buffer);
    int16_t *dstPtr = reinterpret_cast<int16_t *>(dstDesc.buffer);
    size_t count = srcDesc.bufLength / half / half;

    for (size_t idx = 0; idx < count; idx++) {
        int16_t temp = ConvertFromFloatTo16Bit(stcPtr);
        stcPtr++;
        *(dstPtr++) = temp;
    }
    return 0;
}

} // namespace AudioStandard
} // namespace OHOS
