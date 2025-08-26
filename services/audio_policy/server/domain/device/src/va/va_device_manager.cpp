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

#ifndef LOG_TAG
#define LOG_TAG "VADeviceManager"
#endif

#include "va_device_manager.h"
#include "audio_policy_log.h"

namespace OHOS {
namespace AudioStandard {
VADeviceManager &VADeviceManager::GetInstance()
{
	static VADeviceManager instance;
	return instance;
}

std::shared_ptr<AudioDeviceDescriptor> VADeviceManager::ConvertVADeviceToDescriptor(
	const std::shared_ptr<VADevice> &vaDevice)
{
	if (vaDevice == nullptr) {
		return nullptr;
	}
	auto desc = std::make_shared<AudioDeviceDescriptor>();
	auto config = vaDevice->configuration_;
	desc->deviceName_ = config.name_;
	desc->displayName_ = config.name_;
	switch (config.type_) {
	case VA_DEVICE_TYPE_NONE:
		desc->deviceType_ = VA_DEVICE_TYPE_NONE;
		break;
	case VA_DEVICE_TYPE_BT_SPP:
		desc->deviceType_ = VA_DEVICE_TYPE_BT_SPP;
		break;
	}
	switch (config.role_)
	{
	case VA_DEVICE_ROLE_IN:
		desc->deviceRole_ = INPUT_DEVICE;
		break;
	case VA_DEVICE_ROLE_OUT:
		desc->deviceRole_ = OUTPUT_DEVICE;
		break;
	}
	desc->macAddress_ = config.address_;
	desc->networkId_ = config.address_;

	for (const auto &streamProp : config.properties_) {
		std::shared_ptr<DeviceStreamInfo> streamInfo = ConvertVAStreamPropertyToInfo(streamProp);
		desc->audioStreamInfo_.push_back(*streamInfo);
	}
	return desc;
}

std::shared_ptr<DeviceStreamInfo> VADeviceManager::ConvertVAStreamPropertyToInfo(
	const VAAudioStreamProperty &vaStreamProperty)
{
	std::shared_ptr<DeviceStreamInfo> streamInfo = std::make_Shared<DeviceStreamInfo>();
	streamInfo->encoding = vaStreamProperty.encoding_;
	streamInfo->format = vaStreamProperty.sampleFormat_;
	streamInfo->channelLayout.insert(vaStreamProperty.channelLayout_);
	streamInfo->samplingRate.insert(static_cast<AudioSamplingRate>(vaStreamProperty.sampleRate_));
	return streamInfo;
}

void VADeviceManager::OnDevicesConnected(
	const std::shared_ptr<VADevice> &vaDevice, const sptr<IVADeviceController> &controller)
{
	std::shared_ptr<AudioDeviceDescriptor> descriptor = ConvertVADeviceToDescriptor(vaDevice);
	connectedVADeviceMap_[vaDevice->configuration_.address_] = controller;

	AudioPolicyConfigData &config = AudioPolicyConfigData::GetInstance();
	if (config.adapterInfoMap.find(AudioAdapterType::TYPE_VA) == config.adapterInfoMap.end()) {
		RegisterVAAdapterToMap(config);
	}
	AudioCoreService::GetCoreService()->GetEventEntry()->OnDeviceStatusUpdated(*descriptor, true);
}

void VADeviceManager::OnDevicesDisconnected(const std::shared_ptr<VADevice> &vaDevice)
{
	std::shared_ptr<AudioDeviceDescriptor> descriptor = ConvertVADeviceToDescriptor(vaDevice);
	AudioCoreService::GetCoreService()->GetEventEntry()->OnDeviceStatusUpdated(*descriptor, false);
	connectedVADeviceMap_.erase(vaDevice->configuration_.address_);
	if (connectedVADeviceMap_.size() <= 0) {
		AudioPolicyConfigData &config = AudioPolicyConfigData::GetInstance();
		UnregisterVAAdapterFromMap(config);
	}
}

void VADeviceManager::GetDeviceController(const std::string macAddr, sptr<IRemoteObject> &controller)
{
	sptr<IVADeviceController> vaController = connectedVADeviceMap_[macAddr];
	if (vaController == nullptr) {
		AUDIO_WARNING_LOG("cannot find controller");
		controller = nullptr;
	} else {
		controller = vaController->AsObject();
	}
	return;
}

void VADeviceManager::RegisterVAAdapterToMap(AudioPolicyConfigData &config)
{
	/*添加 va adapter 信息*/
	PolicyAdapterInfo adapterInfo{};
	std::shared_ptr<PolicyAdapterInfo> adapterInfoPtr = std::make_shared<PolicyAdapterInfo>(adapterInfo);
	adapterInfoPtr->adapterName = ADAPTER_TYPE_VA;
	config.adapterINfoMap.insert({adapterInfoPtr->GetTypeEnum(), adapterInfoPtr});

	// 添加 pipeInfo
	std::shared_ptr<AdapterPipeInfo> pipeInfo = std::make_shared<AdapterPipeInfo>();
	pipeInfo->adapterInfo_ = adapterInfoPtr;
	pipeInfo->name_ = "va_input";
	pipeInfo->role_ = AudioDefinitionPolicyUtils::pipeRoleStrToEnum["input"];
	pipeInfo->supportDevices_.push_back("Virtual Audio");

	// 添加 deviceInfo
	AdapterDeviceInfo deviceInfo{};
	deviceInfo.adapterInfo_ = adapterInfoPtr;
	deviceInfo.name = "Virtual Audio";
	deviceInfo.type_ = AudioDefinitionPolicyUtils::deviceTypeStrToEnum["DEVICE_TYPE_BT_SPP"];
	deviceInfo.role_ = AudioDefinitionPolicyUtils::deviceRoleStrToEnum["input"];
	deviceInfo.supportPipes_.push_back("va_input"); // 这里需要和pipeinfo的名字一致
	deviceInfo.supportPipeMap_[AudioFlag::AUDIO_INPUT_FLAG_NORMAL] = pipeInfo;

	adapterInfoPtr->deviceInfos.push_back(make_shared<AdapterDeviceInfo>(deviceInfo));

	// 添加 streamPropInfo
	PipeStreamPropInfo streamPropInfo = {};
	streamPropInfo.pipeInfo_ = pipeInfo;
	streamPropInfo.format_ = AudioDefinitionPolicyUtils::formatStrToEnum["s16le"];
	streamPropInfo.sampleRate_ = AudioSamplingRate::SAMPLE_RATE_16000;
	streamPropInfo.channelLayout_ = AudioDefinitionPolicyUtils::layoutStrToEnum["CH_LAYOUT_MONO"];
	streamPropInfo.channels_ = AudioDefinitionPolicyUtils::ConvertLayoutToAudioChannel(streamPropInfo.channelLayout_);
	streamPropInfo.bufferSize_ = 640; // 640
	streamPropInfo.supportDevices_.push_back("Virtual Audio");

	// pipeInfo 中添加 streamPropInfo
	pipeInfo->streamPropInfos_.push_back(make_shared<PipeStreamPropInfo>(streamPropInfo));

	// 添加 paPropInfo
	PaPropInfo paProp = {};
	paProp.lib_ = "va_lib"; // must align with va lib name in HpaeManager::OpenAudioPortInner
	paProp.role_ = "source";
	paProp.moduleName_ = "va_module_name";
	paProp.fixedLatency_ = "";
	paProp.renderInIdleState_ = "";

	// pipeInfo 中添加 pipeInfo
	pipeInfo->paProp_ = std::move(paProp);

	// adapter 中添加 pipeInfo
	adapterInfoPtr->pipeInfos.push_back(pipeInfo);

	config.Reorganize();
}

void VADeviceManager::UnregisterVAAdapterFromMap(AudioPolicyConfigData &config)
{
	config.adapterInfoMap.erase(AudioAdapterType::TYPE_VA);
	std::pair<DeviceType, DeviceRole> deviceMapkey = std::make_pair(DEVICE_TYPE_BT_SPP, INPUT_DEVICE);
	config.deviceInfoMap.erase(deviceMapKey);
}
}  //namespace VirtualAudioDevice
}  //namespace OHOS