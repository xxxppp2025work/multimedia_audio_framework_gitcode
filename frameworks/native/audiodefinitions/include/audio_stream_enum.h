#include <cstdint>

enum StreamClass : uint32_t {
    PA_STREAM = 0,
    FAST_STREAM,
    VOIP_STREAM,
};

enum AudioFlag : uint32_t {
    AUDIO_OUTPUT_FLAG_NONE = 0, // select
    AUDIO_OUTPUT_FLAG_NORMAL, // route
    AUDIO_OUTPUT_FLAG_DIRECT, // route
    AUDIO_OUTPUT_FLAG_HD, // select
    AUDIO_OUTPUT_FLAG_MULTICHANNEL, // select, route
    AUDIO_OUTPUT_FLAG_LOWPOWER, // select, route
    AUDIO_OUTPUT_FLAG_FAST, // select, route
    AUDIO_OUTPUT_FLAG_VOIP, // select
    AUDIO_OUTPUT_FLAG_VOIP_FAST, // select, route
    AUDIO_OUTPUT_FLAG_HWDECODING, // select, route
    AUDIO_INPUT_FLAG_NONE = 100, // select
    AUDIO_INPUT_FLAG_NORMAL, // route
    AUDIO_INPUT_FLAG_FAST, // select, route
    AUDIO_INPUT_FLAG_VOIP, // select
    AUDIO_INPUT_FLAG_VOIP_FAST, // select, route
    AUDIO_INPUT_FLAG_WAKEUP, // select, route
    AUDIO_FLAG_MAX,
};

enum AudioStreamStatus : uint32_t {
    STREAM_STATUS_NEW = 0,
    STREAM_STATUS_STARTTING,
    STREAM_STATUS_PAUSED,
    STREAM_STATUS_STOPPED,
    STREAM_STATUS_RELEASED,
};
