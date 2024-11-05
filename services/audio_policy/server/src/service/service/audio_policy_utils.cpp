
#ifndef LOG_TAG
#define LOG_TAG "AudioPolicyCommon"
#endif

#include "audio_policy_common.h"
#include <ability_manager_client.h>
#include "iservice_registry.h"
#include "parameter.h"
#include "parameters.h"
#include "audio_utils.h"
#include "audio_log.h"
#include "audio_utils.h"
#include "audio_manager_listener_stub.h"
#include "audio_inner_call.h"
#include "media_monitor_manager.h"
#include "data_share_observer_callback.h"
#include "audio_policy_manager_factory.h"
#include "device_init_callback.h"

#include "audio_a2dp_offload_flag.h"

namespace OHOS {
namespace AudioStandard {

static const int64_t CALL_IPC_COST_TIME_MS = 20000000; // 20ms

static const std::string SETTINGS_DATA_BASE_URI =
    "datashare:///com.ohos.settingsdata/entry/settingsdata/SETTINGSDATA?Proxy=true";
static const std::string SETTINGS_DATA_EXT_URI = "datashare:///com.ohos.settingsdata.DataAbility";
static const std::string SETTINGS_DATA_FIELD_KEYWORD = "KEYWORD";
static const std::string SETTINGS_DATA_FIELD_VALUE = "VALUE";
static const std::string PREDICATES_STRING = "settings.general.device_name";

static const std::string PIPE_PRIMARY_OUTPUT = "primary_output";
static const std::string PIPE_PRIMARY_INPUT = "primary_input";
static const std::string PIPE_USB_ARM_OUTPUT = "usb_arm_output";
static const std::string PIPE_USB_ARM_INPUT = "usb_arm_input";
static const std::string PIPE_DP_OUTPUT = "dp_output";

const uint32_t PCM_8_BIT = 8;
const uint32_t PCM_16_BIT = 16;
const uint32_t PCM_24_BIT = 24;
const uint32_t PCM_32_BIT = 32;
constexpr int32_t MS_PER_S = 1000;
constexpr int32_t NS_PER_MS = 1000000;

static const std::map<std::pair<DeviceType, DeviceType>, EcType> DEVICE_TO_EC_TYPE = {
    {{DEVICE_TYPE_MIC, DEVICE_TYPE_SPEAKER}, EC_TYPE_SAME_ADAPTER},
    {{DEVICE_TYPE_MIC, DEVICE_TYPE_USB_HEADSET}, EC_TYPE_SAME_ADAPTER},
    {{DEVICE_TYPE_MIC, DEVICE_TYPE_WIRED_HEADSET}, EC_TYPE_SAME_ADAPTER},
    {{DEVICE_TYPE_MIC, DEVICE_TYPE_USB_ARM_HEADSET}, EC_TYPE_DIFF_ADAPTER},
    {{DEVICE_TYPE_MIC, DEVICE_TYPE_BLUETOOTH_SCO}, EC_TYPE_SAME_ADAPTER},
    {{DEVICE_TYPE_MIC, DEVICE_TYPE_DP}, EC_TYPE_DIFF_ADAPTER},

    {{DEVICE_TYPE_USB_HEADSET, DEVICE_TYPE_SPEAKER}, EC_TYPE_SAME_ADAPTER},
    {{DEVICE_TYPE_USB_HEADSET, DEVICE_TYPE_USB_HEADSET}, EC_TYPE_SAME_ADAPTER},
    {{DEVICE_TYPE_USB_HEADSET, DEVICE_TYPE_WIRED_HEADSET}, EC_TYPE_SAME_ADAPTER},
    {{DEVICE_TYPE_USB_HEADSET, DEVICE_TYPE_USB_ARM_HEADSET}, EC_TYPE_DIFF_ADAPTER},
    {{DEVICE_TYPE_USB_HEADSET, DEVICE_TYPE_BLUETOOTH_SCO}, EC_TYPE_DIFF_ADAPTER},
    {{DEVICE_TYPE_USB_HEADSET, DEVICE_TYPE_DP}, EC_TYPE_DIFF_ADAPTER},

    {{DEVICE_TYPE_WIRED_HEADSET, DEVICE_TYPE_SPEAKER}, EC_TYPE_SAME_ADAPTER},
    {{DEVICE_TYPE_WIRED_HEADSET, DEVICE_TYPE_USB_HEADSET}, EC_TYPE_SAME_ADAPTER},
    {{DEVICE_TYPE_WIRED_HEADSET, DEVICE_TYPE_WIRED_HEADSET}, EC_TYPE_SAME_ADAPTER},
    {{DEVICE_TYPE_WIRED_HEADSET, DEVICE_TYPE_USB_ARM_HEADSET}, EC_TYPE_DIFF_ADAPTER},
    {{DEVICE_TYPE_WIRED_HEADSET, DEVICE_TYPE_BLUETOOTH_SCO}, EC_TYPE_DIFF_ADAPTER},
    {{DEVICE_TYPE_WIRED_HEADSET, DEVICE_TYPE_DP}, EC_TYPE_DIFF_ADAPTER},

    {{DEVICE_TYPE_USB_ARM_HEADSET, DEVICE_TYPE_SPEAKER}, EC_TYPE_DIFF_ADAPTER},
    {{DEVICE_TYPE_USB_ARM_HEADSET, DEVICE_TYPE_USB_HEADSET}, EC_TYPE_DIFF_ADAPTER},
    {{DEVICE_TYPE_USB_ARM_HEADSET, DEVICE_TYPE_WIRED_HEADSET}, EC_TYPE_DIFF_ADAPTER},
    {{DEVICE_TYPE_USB_ARM_HEADSET, DEVICE_TYPE_USB_ARM_HEADSET}, EC_TYPE_SAME_ADAPTER},
    {{DEVICE_TYPE_USB_ARM_HEADSET, DEVICE_TYPE_BLUETOOTH_SCO}, EC_TYPE_DIFF_ADAPTER},
    {{DEVICE_TYPE_USB_ARM_HEADSET, DEVICE_TYPE_DP}, EC_TYPE_DIFF_ADAPTER},

    {{DEVICE_TYPE_BLUETOOTH_SCO, DEVICE_TYPE_SPEAKER}, EC_TYPE_SAME_ADAPTER},
    {{DEVICE_TYPE_BLUETOOTH_SCO, DEVICE_TYPE_USB_HEADSET}, EC_TYPE_SAME_ADAPTER},
    {{DEVICE_TYPE_BLUETOOTH_SCO, DEVICE_TYPE_WIRED_HEADSET}, EC_TYPE_SAME_ADAPTER},
    {{DEVICE_TYPE_BLUETOOTH_SCO, DEVICE_TYPE_USB_ARM_HEADSET}, EC_TYPE_DIFF_ADAPTER},
    {{DEVICE_TYPE_BLUETOOTH_SCO, DEVICE_TYPE_BLUETOOTH_SCO}, EC_TYPE_SAME_ADAPTER},
    {{DEVICE_TYPE_BLUETOOTH_SCO, DEVICE_TYPE_DP}, EC_TYPE_DIFF_ADAPTER},
};

static std::map<std::string, ClassType> portStrToEnum = {
    {PRIMARY_SPEAKER, TYPE_PRIMARY},
    {PRIMARY_MIC, TYPE_PRIMARY},
    {PRIMARY_WAKEUP_MIC, TYPE_PRIMARY},
    {BLUETOOTH_SPEAKER, TYPE_A2DP},
    {USB_SPEAKER, TYPE_USB},
    {USB_MIC, TYPE_USB},
    {DP_SINK, TYPE_DP},
    {FILE_SINK, TYPE_FILE_IO},
    {FILE_SOURCE, TYPE_FILE_IO},
    {REMOTE_CLASS, TYPE_REMOTE_AUDIO},
};
const std::string AUDIO_SERVICE_PKG = "audio_manager_service";

int32_t AudioPolicyCommon::startDeviceId = 1;

std::shared_ptr<DataShare::DataShareHelper> AudioPolicyCommon::CreateDataShareHelperInstance()
{
    auto samgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    CHECK_AND_RETURN_RET_LOG(samgr != nullptr, nullptr, "[Policy Service] Get samgr failed.");

    sptr<IRemoteObject> remoteObject = samgr->GetSystemAbility(AUDIO_POLICY_SERVICE_ID);
    CHECK_AND_RETURN_RET_LOG(remoteObject != nullptr, nullptr, "[Policy Service] audio service remote object is NULL.");

    int64_t startTime = ClockTime::GetCurNano();
    sptr<IRemoteObject> dataSharedServer = samgr->CheckSystemAbility(DISTRIBUTED_KV_DATA_SERVICE_ABILITY_ID);
    int64_t cost = ClockTime::GetCurNano() - startTime;
    if (cost > CALL_IPC_COST_TIME_MS) {
        AUDIO_WARNING_LOG("Call get DataShare server cost too long: %{public}" PRId64"ms.", cost / AUDIO_US_PER_SECOND);
    }

    CHECK_AND_RETURN_RET_LOG(dataSharedServer != nullptr, nullptr, "DataShare server is not started!");

    startTime = ClockTime::GetCurNano();
    std::pair<int, std::shared_ptr<DataShare::DataShareHelper>> res = DataShare::DataShareHelper::Create(remoteObject,
        SETTINGS_DATA_BASE_URI, SETTINGS_DATA_EXT_URI);
    cost = ClockTime::GetCurNano() - startTime;
    if (cost > CALL_IPC_COST_TIME_MS) {
        AUDIO_WARNING_LOG("DataShareHelper::Create cost too long: %{public}" PRId64"ms.", cost / AUDIO_US_PER_SECOND);
    }
    if (res.first == DataShare::E_DATA_SHARE_NOT_READY) {
        AUDIO_WARNING_LOG("DataShareHelper::Create failed: E_DATA_SHARE_NOT_READY");
        return nullptr;
    }
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper = res.second;
    CHECK_AND_RETURN_RET_LOG(res.first == DataShare::E_OK && dataShareHelper != nullptr, nullptr, "fail:%{public}d",
        res.first);
    return dataShareHelper;
}

void AudioPolicyCommon::RegisterNameMonitorHelper()
{
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper = CreateDataShareHelperInstance();
    CHECK_AND_RETURN_LOG(dataShareHelper != nullptr, "dataShareHelper is NULL");

    auto uri = std::make_shared<Uri>(SETTINGS_DATA_BASE_URI + "&key=" + PREDICATES_STRING);
    sptr<AAFwk::DataAbilityObserverStub> settingDataObserver = std::make_unique<DataShareObserverCallBack>().release();
    dataShareHelper->RegisterObserver(*uri, settingDataObserver);

    dataShareHelper->Release();
}

int32_t AudioPolicyCommon::GetDeviceNameFromDataShareHelper(std::string &deviceName)
{
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper = CreateDataShareHelperInstance();
    CHECK_AND_RETURN_RET_LOG(dataShareHelper != nullptr, ERROR, "GetDeviceNameFromDataShareHelper NULL");

    std::shared_ptr<Uri> uri = std::make_shared<Uri>(SETTINGS_DATA_BASE_URI);
    std::vector<std::string> columns;
    columns.emplace_back(SETTINGS_DATA_FIELD_VALUE);
    DataShare::DataSharePredicates predicates;
    predicates.EqualTo(SETTINGS_DATA_FIELD_KEYWORD, PREDICATES_STRING);

    auto resultSet = dataShareHelper->Query(*uri, predicates, columns);
    if (resultSet == nullptr) {
        AUDIO_ERR_LOG("Failed to query device name from dataShareHelper!");
        dataShareHelper->Release();
        return ERROR;
    }

    int32_t numRows = 0;
    resultSet->GetRowCount(numRows);
    if (numRows <= 0) {
        AUDIO_ERR_LOG("The result of querying is zero row!");
        resultSet->Close();
        dataShareHelper->Release();
        return ERROR;
    }

    int columnIndex;
    resultSet->GoToFirstRow();
    resultSet->GetColumnIndex(SETTINGS_DATA_FIELD_VALUE, columnIndex);
    resultSet->GetString(columnIndex, deviceName);
    AUDIO_INFO_LOG("GetDeviceNameFromDataShareHelper deviceName[%{public}s]", deviceName.c_str());

    resultSet->Close();
    dataShareHelper->Release();
    return SUCCESS;
}

bool AudioPolicyCommon::IsDataShareReady()
{
    auto samgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    CHECK_AND_RETURN_RET_LOG(samgr != nullptr, false, "[Policy Service] Get samgr failed.");
    sptr<IRemoteObject> remoteObject = samgr->GetSystemAbility(AUDIO_POLICY_SERVICE_ID);
    CHECK_AND_RETURN_RET_LOG(remoteObject != nullptr, false, "[Policy Service] audio service remote object is NULL.");
    std::pair<int, std::shared_ptr<DataShare::DataShareHelper>> res = DataShare::DataShareHelper::Create(remoteObject,
        SETTINGS_DATA_BASE_URI, SETTINGS_DATA_EXT_URI);
    if (res.first == DataShare::E_OK) {
        AUDIO_INFO_LOG("DataShareHelper is ready.");
        auto helper = res.second;
        if (helper != nullptr) {
            helper->Release();
        }
        return true;
    } else {
        AUDIO_WARNING_LOG("DataShareHelper::Create failed: E_DATA_SHARE_NOT_READY");
        return false;
    }
}

DeviceRole AudioPolicyCommon::GetDeviceRole(DeviceType deviceType) const
{
    switch (deviceType) {
        case DeviceType::DEVICE_TYPE_EARPIECE:
        case DeviceType::DEVICE_TYPE_SPEAKER:
        case DeviceType::DEVICE_TYPE_BLUETOOTH_SCO:
        case DeviceType::DEVICE_TYPE_BLUETOOTH_A2DP:
        case DeviceType::DEVICE_TYPE_WIRED_HEADSET:
        case DeviceType::DEVICE_TYPE_WIRED_HEADPHONES:
        case DeviceType::DEVICE_TYPE_USB_HEADSET:
        case DeviceType::DEVICE_TYPE_DP:
        case DeviceType::DEVICE_TYPE_USB_ARM_HEADSET:
        case DeviceType::DEVICE_TYPE_REMOTE_CAST:
            return DeviceRole::OUTPUT_DEVICE;
        case DeviceType::DEVICE_TYPE_MIC:
        case DeviceType::DEVICE_TYPE_WAKEUP:
            return DeviceRole::INPUT_DEVICE;
        default:
            return DeviceRole::DEVICE_ROLE_NONE;
    }
}

DeviceRole AudioPolicyCommon::GetDeviceRole(const std::string &role)
{
    if (role == ROLE_SINK) {
        return DeviceRole::OUTPUT_DEVICE;
    } else if (role == ROLE_SOURCE) {
        return DeviceRole::INPUT_DEVICE;
    } else {
        return DeviceRole::DEVICE_ROLE_NONE;
    }
}

DeviceRole AudioPolicyCommon::GetDeviceRole(AudioPin pin) const
{
    switch (pin) {
        case OHOS::AudioStandard::AUDIO_PIN_NONE:
            return DeviceRole::DEVICE_ROLE_NONE;
        case OHOS::AudioStandard::AUDIO_PIN_OUT_SPEAKER:
        case OHOS::AudioStandard::AUDIO_PIN_OUT_HEADSET:
        case OHOS::AudioStandard::AUDIO_PIN_OUT_LINEOUT:
        case OHOS::AudioStandard::AUDIO_PIN_OUT_HDMI:
        case OHOS::AudioStandard::AUDIO_PIN_OUT_USB:
        case OHOS::AudioStandard::AUDIO_PIN_OUT_USB_EXT:
        case OHOS::AudioStandard::AUDIO_PIN_OUT_DAUDIO_DEFAULT:
            return DeviceRole::OUTPUT_DEVICE;
        case OHOS::AudioStandard::AUDIO_PIN_IN_MIC:
        case OHOS::AudioStandard::AUDIO_PIN_IN_HS_MIC:
        case OHOS::AudioStandard::AUDIO_PIN_IN_LINEIN:
        case OHOS::AudioStandard::AUDIO_PIN_IN_USB_EXT:
        case OHOS::AudioStandard::AUDIO_PIN_IN_DAUDIO_DEFAULT:
            return DeviceRole::INPUT_DEVICE;
        default:
            return DeviceRole::DEVICE_ROLE_NONE;
    }
}


std::string AudioPolicyCommon::ConvertToHDIAudioFormat(AudioSampleFormat sampleFormat)
{
    switch (sampleFormat) {
        case SAMPLE_U8:
            return "u8";
        case SAMPLE_S16LE:
            return "s16le";
        case SAMPLE_S24LE:
            return "s24le";
        case SAMPLE_S32LE:
            return "s32le";
        default:
            return "";
    }
}

uint32_t AudioPolicyCommon::GetSampleFormatValue(AudioSampleFormat sampleFormat)
{
    switch (sampleFormat) {
        case SAMPLE_U8:
            return PCM_8_BIT;
        case SAMPLE_S16LE:
            return PCM_16_BIT;
        case SAMPLE_S24LE:
            return PCM_24_BIT;
        case SAMPLE_S32LE:
            return PCM_32_BIT;
        default:
            return PCM_16_BIT;
    }
}

std::string AudioPolicyCommon::ParseAudioFormat(std::string format)
{
    if (format == "AUDIO_FORMAT_PCM_16_BIT") {
        return "s16";
    } else if (format == "AUDIO_FORMAT_PCM_24_BIT") {
        return "s24";
    } else if (format == "AUDIO_FORMAT_PCM_32_BIT") {
        return "s32";
    } else {
        return "";
    }
}

int64_t AudioPolicyCommon::GetCurrentTimeMS()
{
    timespec tm {};
    clock_gettime(CLOCK_MONOTONIC, &tm);
    return tm.tv_sec * MS_PER_S + (tm.tv_nsec / NS_PER_MS);
}

uint32_t AudioPolicyCommon::PcmFormatToBits(AudioSampleFormat format)
{
    switch (format) {
        case SAMPLE_U8:
            return 1; // 1 byte
        case SAMPLE_S16LE:
            return 2; // 2 byte
        case SAMPLE_S24LE:
            return 3; // 3 byte
        case SAMPLE_S32LE:
            return 4; // 4 byte
        case SAMPLE_F32LE:
            return 4; // 4 byte
        default:
            return 2; // 2 byte
    }
}

std::string AudioPolicyCommon::GetPipeNameByDeviceForEc(const std::string &role, const DeviceType deviceType)
{
    switch (deviceType) {
        case DEVICE_TYPE_SPEAKER:
            return PIPE_PRIMARY_OUTPUT;
        case DEVICE_TYPE_WIRED_HEADSET:
        case DEVICE_TYPE_USB_HEADSET:
        case DEVICE_TYPE_BLUETOOTH_SCO:
            if (role == ROLE_SOURCE) {
                return PIPE_PRIMARY_INPUT;
            }
            return PIPE_PRIMARY_OUTPUT;
        case DEVICE_TYPE_MIC:
            return PIPE_PRIMARY_INPUT;
        case DEVICE_TYPE_USB_ARM_HEADSET:
            if (role == ROLE_SOURCE) {
                return PIPE_USB_ARM_INPUT;
            }
            return PIPE_USB_ARM_OUTPUT;
        case DEVICE_TYPE_DP:
            return PIPE_DP_OUTPUT;
        default:
            AUDIO_ERR_LOG("invalid deevice type %{public}d for role %{public}s", deviceType, role.c_str());
            return PIPE_PRIMARY_OUTPUT;
    }
}

void AudioPolicyCommon::GetTargetSourceTypeAndMatchingFlag(SourceType source, bool isEcFeatureEnable,
    SourceType &targetSource, bool &useMatchingPropInfo)
{
    switch (source) {
        case SOURCE_TYPE_VOICE_RECOGNITION:
            targetSource = SOURCE_TYPE_VOICE_RECOGNITION;
            useMatchingPropInfo = true;
            break;
        case SOURCE_TYPE_VOICE_COMMUNICATION:
        case SOURCE_TYPE_VOICE_TRANSCRIPTION:
            targetSource = SOURCE_TYPE_VOICE_COMMUNICATION;
            useMatchingPropInfo = isEcFeatureEnable ? false : true;
            break;
        case SOURCE_TYPE_VOICE_CALL:
            targetSource = SOURCE_TYPE_VOICE_CALL;
            break;
        case SOURCE_TYPE_CAMCORDER:
            targetSource = SOURCE_TYPE_CAMCORDER;
            break;
        default:
            targetSource = SOURCE_TYPE_MIC;
            break;
    }
}

DeviceType AudioPolicyCommon::GetDeviceTypeFromPin(AudioPin hdiPin)
{
    switch (hdiPin) {
        case OHOS::AudioStandard::AUDIO_PIN_NONE:
            break;
        case OHOS::AudioStandard::AUDIO_PIN_OUT_SPEAKER:
        case OHOS::AudioStandard::AUDIO_PIN_OUT_DAUDIO_DEFAULT:
            return DeviceType::DEVICE_TYPE_SPEAKER;
        case OHOS::AudioStandard::AUDIO_PIN_OUT_HEADSET:
            break;
        case OHOS::AudioStandard::AUDIO_PIN_OUT_LINEOUT:
            break;
        case OHOS::AudioStandard::AUDIO_PIN_OUT_HDMI:
            break;
        case OHOS::AudioStandard::AUDIO_PIN_OUT_USB:
            break;
        case OHOS::AudioStandard::AUDIO_PIN_OUT_USB_EXT:
            break;
        case OHOS::AudioStandard::AUDIO_PIN_OUT_USB_HEADSET:
        case OHOS::AudioStandard::AUDIO_PIN_IN_USB_HEADSET:
            return DeviceType::DEVICE_TYPE_USB_ARM_HEADSET;
        case OHOS::AudioStandard::AUDIO_PIN_IN_MIC:
        case OHOS::AudioStandard::AUDIO_PIN_IN_DAUDIO_DEFAULT:
            return DeviceType::DEVICE_TYPE_MIC;
        case OHOS::AudioStandard::AUDIO_PIN_IN_HS_MIC:
            break;
        case OHOS::AudioStandard::AUDIO_PIN_IN_LINEIN:
            break;
        case OHOS::AudioStandard::AUDIO_PIN_IN_USB_EXT:
            break;
        default:
            break;
    }
    return DeviceType::DEVICE_TYPE_DEFAULT;
}

std::string AudioPolicyCommon::GetSourcePortName(InternalDeviceType deviceType)
{
    std::string portName = PORT_NONE;
    switch (deviceType) {
        case InternalDeviceType::DEVICE_TYPE_MIC:
        case InternalDeviceType::DEVICE_TYPE_USB_HEADSET:
        case InternalDeviceType::DEVICE_TYPE_BLUETOOTH_SCO:
            portName = PRIMARY_MIC;
            break;
        case InternalDeviceType::DEVICE_TYPE_USB_ARM_HEADSET:
            portName = USB_MIC;
            break;
        case InternalDeviceType::DEVICE_TYPE_WAKEUP:
            portName = PRIMARY_WAKEUP;
            break;
        case InternalDeviceType::DEVICE_TYPE_FILE_SOURCE:
            portName = FILE_SOURCE;
            break;
        default:
            portName = PORT_NONE;
            break;
    }

    return portName;
}

bool AudioPolicyCommon::HasLowLatencyCapability(DeviceType deviceType, bool isRemote)
{
    // Distributed devices are low latency devices
    if (isRemote) {
        return true;
    }

    switch (deviceType) {
        case DeviceType::DEVICE_TYPE_EARPIECE:
        case DeviceType::DEVICE_TYPE_SPEAKER:
        case DeviceType::DEVICE_TYPE_WIRED_HEADSET:
        case DeviceType::DEVICE_TYPE_WIRED_HEADPHONES:
        case DeviceType::DEVICE_TYPE_USB_HEADSET:
        case DeviceType::DEVICE_TYPE_DP:
            return true;

        case DeviceType::DEVICE_TYPE_BLUETOOTH_SCO:
        case DeviceType::DEVICE_TYPE_BLUETOOTH_A2DP:
            return false;
        default:
            return false;
    }
}

void AudioPolicyCommon::GetUsbModuleInfo(std::string deviceInfo, AudioModuleInfo &moduleInfo)
{
    if (moduleInfo.role == "sink") {
        auto sinkRate_begin = deviceInfo.find("sink_rate:");
        auto sinkRate_end = deviceInfo.find_first_of(";", sinkRate_begin);
        moduleInfo.rate = deviceInfo.substr(sinkRate_begin + std::strlen("sink_rate:"),
            sinkRate_end - sinkRate_begin - std::strlen("sink_rate:"));
        auto sinkFormat_begin = deviceInfo.find("sink_format:");
        auto sinkFormat_end = deviceInfo.find_first_of(";", sinkFormat_begin);
        std::string format = deviceInfo.substr(sinkFormat_begin + std::strlen("sink_format:"),
            sinkFormat_end - sinkFormat_begin - std::strlen("sink_format:"));
        moduleInfo.format = ParseAudioFormat(format);
    } else {
        auto sourceRate_begin = deviceInfo.find("source_rate:");
        auto sourceRate_end = deviceInfo.find_first_of(";", sourceRate_begin);
        moduleInfo.rate = deviceInfo.substr(sourceRate_begin + std::strlen("source_rate:"),
            sourceRate_end - sourceRate_begin - std::strlen("source_rate:"));
        auto sourceFormat_begin = deviceInfo.find("source_format:");
        auto sourceFormat_end = deviceInfo.find_first_of(";", sourceFormat_begin);
        std::string format = deviceInfo.substr(sourceFormat_begin + std::strlen("source_format:"),
            sourceFormat_end - sourceFormat_begin - std::strlen("source_format:"));
        moduleInfo.format = ParseAudioFormat(format);
    }
}

void AudioPolicyCommon::GetDPModuleInfo(AudioModuleInfo &moduleInfo, std::string deviceInfo)
{
    if (moduleInfo.role == "sink") {
        auto sinkRate_begin = deviceInfo.find("rate=");
        auto sinkRate_end = deviceInfo.find_first_of(" ", sinkRate_begin);
        moduleInfo.rate = deviceInfo.substr(sinkRate_begin + std::strlen("rate="),
            sinkRate_end - sinkRate_begin - std::strlen("rate="));

        auto sinkFormat_begin = deviceInfo.find("format=");
        auto sinkFormat_end = deviceInfo.find_first_of(" ", sinkFormat_begin);
        std::string format = deviceInfo.substr(sinkFormat_begin + std::strlen("format="),
            sinkFormat_end - sinkFormat_begin - std::strlen("format="));
        if (!format.empty()) moduleInfo.format = format;

        auto sinkChannel_begin = deviceInfo.find("channels=");
        auto sinkChannel_end = deviceInfo.find_first_of(" ", sinkChannel_begin);
        std::string channel = deviceInfo.substr(sinkChannel_begin + std::strlen("channels="),
            sinkChannel_end - sinkChannel_begin - std::strlen("channels="));
        moduleInfo.channels = channel;

        auto sinkBSize_begin = deviceInfo.find("buffer_size=");
        auto sinkBSize_end = deviceInfo.find_first_of(" ", sinkBSize_begin);
        std::string bufferSize = deviceInfo.substr(sinkBSize_begin + std::strlen("buffer_size="),
            sinkBSize_end - sinkBSize_begin - std::strlen("buffer_size="));
        moduleInfo.bufferSize = bufferSize;
    }
}

std::string AudioPolicyCommon::GetRemoteModuleName(std::string networkId, DeviceRole role)
{
    return networkId + (role == DeviceRole::OUTPUT_DEVICE ? "_out" : "_in");
}

EcType AudioPolicyCommon::GetEcType(const DeviceType inputDevice, const DeviceType outputDevice)
{
    EcType ecType = EC_TYPE_NONE;
    auto element = DEVICE_TO_EC_TYPE.find(std::make_pair(inputDevice, outputDevice));
    if (element != DEVICE_TO_EC_TYPE.end()) {
        ecType = element->second;
    }
    AUDIO_INFO_LOG("GetEcType ecType: %{public}d", ecType);
    return ecType;
}

InternalDeviceType AudioPolicyCommon::GetDeviceType(const std::string &deviceName)
{
    InternalDeviceType devType = InternalDeviceType::DEVICE_TYPE_NONE;
    if (deviceName == "Speaker") {
        devType = InternalDeviceType::DEVICE_TYPE_SPEAKER;
    } else if (deviceName == "Built_in_mic") {
        devType = InternalDeviceType::DEVICE_TYPE_MIC;
    } else if (deviceName == "Built_in_wakeup") {
        devType = InternalDeviceType::DEVICE_TYPE_WAKEUP;
    } else if (deviceName == "fifo_output" || deviceName == "fifo_input") {
        devType = DEVICE_TYPE_BLUETOOTH_SCO;
    } else if (deviceName == "file_sink") {
        devType = DEVICE_TYPE_FILE_SINK;
    } else if (deviceName == "file_source") {
        devType = DEVICE_TYPE_FILE_SOURCE;
    }

    return devType;
}

void AudioPolicyCommon::WriteServiceStartupError(std::string reason)
{
    std::shared_ptr<Media::MediaMonitor::EventBean> bean = std::make_shared<Media::MediaMonitor::EventBean>(
        Media::MediaMonitor::ModuleId::AUDIO, Media::MediaMonitor::EventId::AUDIO_SERVICE_STARTUP_ERROR,
        Media::MediaMonitor::EventType::FAULT_EVENT);
    bean->Add("SERVICE_ID", static_cast<int32_t>(Media::MediaMonitor::AUDIO_POLICY_SERVICE_ID));
    bean->Add("ERROR_CODE", static_cast<int32_t>(Media::MediaMonitor::AUDIO_POLICY_SERVER));
    Media::MediaMonitor::MediaMonitorManager::GetInstance().WriteLogMsg(bean);
}

std::string AudioPolicyCommon::GetEncryptAddr(const std::string &addr)
{
    const int32_t START_POS = 6;
    const int32_t END_POS = 13;
    const int32_t ADDRESS_STR_LEN = 17;
    if (addr.empty() || addr.length() != ADDRESS_STR_LEN) {
        return std::string("");
    }
    std::string tmp = "**:**:**:**:**:**";
    std::string out = addr;
    for (int i = START_POS; i <= END_POS; i++) {
        out[i] = tmp[i];
    }
    return out;
}

AdaptersType AudioPolicyCommon::GetAdapterType(std::string sinkPortName)
{
    return static_cast<AdaptersType>(portStrToEnum[sinkPortName]);
}

std::string AudioPolicyCommon::GetSinkPortName(DeviceType deviceType, AudioPipeType pipeType)
{
    std::string portName = PORT_NONE;
    switch (deviceType) {
        case DeviceType::DEVICE_TYPE_BLUETOOTH_A2DP:
            // BTH tells us that a2dpoffload is OK and also a2dpOffload path has setup
            if (AudioA2dpOffloadFlag::GetInstance().GetA2dpOffloadFlag() == A2DP_OFFLOAD
               && AudioA2dpOffloadFlag::GetInstance().IsA2dpOffloadConnected()) {
                portName = PRIMARY_SPEAKER;
                if (pipeType == PIPE_TYPE_OFFLOAD) {
                    portName = OFFLOAD_PRIMARY_SPEAKER;
                } else if (pipeType == PIPE_TYPE_MULTICHANNEL) {
                    portName = MCH_PRIMARY_SPEAKER;
                }
            } else {
                portName = BLUETOOTH_SPEAKER;
            }
            break;
        case DeviceType::DEVICE_TYPE_EARPIECE:
        case DeviceType::DEVICE_TYPE_SPEAKER:
        case DeviceType::DEVICE_TYPE_WIRED_HEADSET:
        case DeviceType::DEVICE_TYPE_WIRED_HEADPHONES:
        case DeviceType::DEVICE_TYPE_USB_HEADSET:
        case DeviceType::DEVICE_TYPE_BLUETOOTH_SCO:
            if (pipeType == PIPE_TYPE_OFFLOAD) {
                portName = OFFLOAD_PRIMARY_SPEAKER;
            } else if (pipeType == PIPE_TYPE_MULTICHANNEL) {
                portName = MCH_PRIMARY_SPEAKER;
            } else {
                portName = PRIMARY_SPEAKER;
            }
            break;
        case DeviceType::DEVICE_TYPE_USB_ARM_HEADSET:
            portName = USB_SPEAKER;
            break;
        case DeviceType::DEVICE_TYPE_DP:
            portName = DP_SINK;
            break;
        case DeviceType::DEVICE_TYPE_FILE_SINK:
            portName = FILE_SINK;
            break;
        case DeviceType::DEVICE_TYPE_REMOTE_CAST:
            portName = REMOTE_CAST_INNER_CAPTURER_SINK_NAME;
            break;
        default:
            portName = PORT_NONE;
            break;
    }

    return portName;
}


int32_t AudioPolicyCommon::SetPreferredDevice(const PreferredType preferredType,
    const sptr<AudioDeviceDescriptor> &desc)
{
    int32_t ret = SUCCESS;
    switch (preferredType) {
        case AUDIO_MEDIA_RENDER:
            AudioStateManager::GetAudioStateManager().SetPreferredMediaRenderDevice(desc);
            break;
        case AUDIO_CALL_RENDER:
            AudioStateManager::GetAudioStateManager().SetPreferredCallRenderDevice(desc);
            break;
        case AUDIO_CALL_CAPTURE:
            AudioStateManager::GetAudioStateManager().SetPreferredCallCaptureDevice(desc);
            break;
        case AUDIO_RECORD_CAPTURE:
            AudioStateManager::GetAudioStateManager().SetPreferredRecordCaptureDevice(desc);
            break;
        case AUDIO_RING_RENDER:
        case AUDIO_TONE_RENDER:
            AUDIO_WARNING_LOG("preferredType:%{public}d, not supported", preferredType);
            ret = ERR_INVALID_PARAM;
            break;
        default:
            AUDIO_ERR_LOG("invalid preferredType: %{public}d", preferredType);
            ret = ERR_INVALID_PARAM;
            break;
    }
    if (desc == nullptr || desc->deviceType_ == DEVICE_TYPE_NONE) {
        ErasePreferredDeviceByType(preferredType);
    }
    if (ret != SUCCESS) {
        AUDIO_ERR_LOG("Set preferredType %{public}d failed, ret: %{public}d", preferredType, ret);
    }
    return ret;
}

void AudioPolicyCommon::SetBtConnecting(bool flag)
{
    isBTReconnecting_ = flag;
}

int32_t AudioPolicyCommon::ErasePreferredDeviceByType(const PreferredType preferredType)
{
    if (isBTReconnecting_) {
        return SUCCESS;
    }
    auto type = static_cast<Media::MediaMonitor::PerferredType>(preferredType);
    int32_t ret = Media::MediaMonitor::MediaMonitorManager::GetInstance().ErasePreferredDeviceByType(type);
    if (ret != SUCCESS) {
        AUDIO_ERR_LOG("Erase preferredType %{public}d failed, ret: %{public}d", preferredType, ret);
        return ERROR;
    }
    return SUCCESS;
}

void AudioPolicyCommon::ClearScoDeviceSuspendState(std::string macAddress)
{
    AUDIO_DEBUG_LOG("Clear sco suspend state %{public}s", GetEncryptAddr(macAddress).c_str());
    vector<shared_ptr<AudioDeviceDescriptor>> descs = AudioDeviceManager::GetAudioDeviceManager().GetDevicesByFilter(
        DEVICE_TYPE_BLUETOOTH_SCO, DEVICE_ROLE_NONE, macAddress, "", SUSPEND_CONNECTED);
    for (auto &desc : descs) {
        desc->connectState_ = DEACTIVE_CONNECTED;
    }
}

void AudioPolicyCommon::UpdateDisplayName(sptr<AudioDeviceDescriptor> deviceDescriptor)
{
    if (deviceDescriptor->networkId_ == LOCAL_NETWORK_ID) {
        std::string devicesName = "";
        int32_t ret = GetDeviceNameFromDataShareHelper(devicesName);
        CHECK_AND_RETURN_LOG(ret == SUCCESS, "Local UpdateDisplayName init device failed");
        deviceDescriptor->displayName_ = devicesName;
    } else {
#ifdef FEATURE_DEVICE_MANAGER
        std::shared_ptr<DistributedHardware::DmInitCallback> callback = std::make_shared<DeviceInitCallBack>();
        int32_t ret = DistributedHardware::DeviceManager::GetInstance().InitDeviceManager(AUDIO_SERVICE_PKG, callback);
        CHECK_AND_RETURN_LOG(ret == SUCCESS, "UpdateDisplayName init device failed");
        std::vector<DistributedHardware::DmDeviceInfo> deviceList;
        if (DistributedHardware::DeviceManager::GetInstance().GetTrustedDeviceList(AUDIO_SERVICE_PKG, "", deviceList) == SUCCESS) {
            for (auto deviceInfo : deviceList) {
                std::string strNetworkId(deviceInfo.networkId);
                if (strNetworkId == deviceDescriptor->networkId_) {
                    AUDIO_INFO_LOG("UpdateDisplayName remote name [%{public}s]", deviceInfo.deviceName);
                    deviceDescriptor->displayName_ = deviceInfo.deviceName;
                    break;
                }
            }
        };
#endif
    }
}

std::vector<std::unique_ptr<AudioDeviceDescriptor>> AudioPolicyCommon::GetAvailableDevicesInner(AudioDeviceUsage usage)
{
    std::vector<std::unique_ptr<AudioDeviceDescriptor>> audioDeviceDescriptors;

    audioDeviceDescriptors = AudioDeviceManager::GetAudioDeviceManager().GetAvailableDevicesByUsage(usage);

    AUDIO_INFO_LOG("audioDeviceDescriptors size:%{public}zu", audioDeviceDescriptors.size());
    return audioDeviceDescriptors;
}

void AudioPolicyCommon::UpdateDescWhenNoBTPermission(vector<sptr<AudioDeviceDescriptor>> &deviceDescs)
{
    AUDIO_WARNING_LOG("UpdateDescWhenNoBTPermission: No bt permission");

    for (sptr<AudioDeviceDescriptor> &desc : deviceDescs) {
        if ((desc->deviceType_ == DEVICE_TYPE_BLUETOOTH_A2DP) || (desc->deviceType_ == DEVICE_TYPE_BLUETOOTH_SCO)) {
            sptr<AudioDeviceDescriptor> copyDesc = new AudioDeviceDescriptor(desc);
            copyDesc->deviceName_ = "";
            copyDesc->macAddress_ = "";
            desc = copyDesc;
        }
    }
}

std::vector<sptr<AudioDeviceDescriptor>> AudioPolicyCommon::DeviceFilterByUsageInner(AudioDeviceUsage usage,
    const std::vector<sptr<AudioDeviceDescriptor>>& descs)
{
    std::vector<std::unique_ptr<AudioDeviceDescriptor>> audioDeviceDescriptors;

    unordered_map<AudioDevicePrivacyType, list<DevicePrivacyInfo>> devicePrivacyMaps =
        AudioDeviceManager::GetAudioDeviceManager().GetDevicePrivacyMaps();
    for (const auto &dev : descs) {
        for (const auto &devicePrivacy : devicePrivacyMaps) {
            list<DevicePrivacyInfo> deviceInfos = devicePrivacy.second;
            AudioDeviceManager::GetAudioDeviceManager().GetAvailableDevicesWithUsage(usage, deviceInfos, dev, audioDeviceDescriptors);
        }
    }
    std::vector<sptr<AudioDeviceDescriptor>> deviceDescriptors;
    for (const auto &dec : audioDeviceDescriptors) {
        sptr<AudioDeviceDescriptor> tempDec = new(std::nothrow) AudioDeviceDescriptor(*dec);
        deviceDescriptors.push_back(move(tempDec));
    }
    return deviceDescriptors;
}


AudioModuleInfo AudioPolicyCommon::ConstructRemoteAudioModuleInfo(std::string networkId, DeviceRole deviceRole,
    DeviceType deviceType)
{
    AudioModuleInfo audioModuleInfo = {};
    if (deviceRole == DeviceRole::OUTPUT_DEVICE) {
        audioModuleInfo.lib = "libmodule-hdi-sink.z.so";
        audioModuleInfo.format = "s16le"; // 16bit little endian
        audioModuleInfo.fixedLatency = "1"; // here we need to set latency fixed for a fixed buffer size.
    } else if (deviceRole == DeviceRole::INPUT_DEVICE) {
        audioModuleInfo.lib = "libmodule-hdi-source.z.so";
        audioModuleInfo.format = "s16le"; // we assume it is bigger endian
    } else {
        AUDIO_WARNING_LOG("Invalid flag provided %{public}d", static_cast<int32_t>(deviceType));
    }

    // used as "sink_name" in hdi_sink.c, hope we could use name to find target sink.
    audioModuleInfo.name = GetRemoteModuleName(networkId, deviceRole);
    audioModuleInfo.networkId = networkId;

    std::stringstream typeValue;
    typeValue << static_cast<int32_t>(deviceType);
    audioModuleInfo.deviceType = typeValue.str();

    audioModuleInfo.adapterName = "remote";
    audioModuleInfo.className = "remote"; // used in renderer_sink_adapter.c
    audioModuleInfo.fileName = "remote_dump_file";

    audioModuleInfo.channels = "2";
    audioModuleInfo.rate = "48000";
    audioModuleInfo.bufferSize = "3840";
    audioModuleInfo.renderInIdleState = "1";

    return audioModuleInfo;
}

}
}