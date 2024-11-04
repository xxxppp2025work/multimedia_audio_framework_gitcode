
#ifndef ST_AUDIO_ROUTER_MAP_H
#define ST_AUDIO_ROUTER_MAP_H

#include <bitset>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include "audio_info.h"
#include "audio_device_info.h"

namespace OHOS {
namespace AudioStandard {
class AudioRouteMap {
public:
    static AudioRouteMap& GetInstance()
    {
        static AudioRouteMap instance;
        return instance;
    }
    std::string GetDeviceInfoByUidAndPid(int32_t uid, int32_t pid);
    bool DelRouteMapInfoByKey(int32_t uid);
    void AddRouteMapInfo(int32_t uid, std::string device, int32_t pid);
    void AddFastRouteMapInfo(int32_t uid, std::string device, DeviceRole role);
    void RemoveDeviceInRouterMap(std::string networkId);
    void RemoveDeviceInFastRouterMap(std::string networkId);
    void GetNetworkIDInFastRouterMap(int32_t uid, DeviceRole role, std::string& newworkId);
private:
    AudioRouteMap() {}
    ~AudioRouteMap() {}
private:
    std::mutex fastRouterMapMutex_; // unordered_map is not concurrently-secure
    std::mutex routerMapMutex_; // unordered_map is not concurrently-secure
    std::unordered_map<int32_t, std::pair<std::string, int32_t>> routerMap_;
    std::unordered_map<int32_t, std::pair<std::string, DeviceRole>> fastRouterMap_; // key:uid value:<netWorkId, Role>
};
}
}
#endif