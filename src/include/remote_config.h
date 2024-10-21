#pragma once

#include <string>
#include <map>

struct SWConfig {
    std::string name;
    std::string value;
    long long update_time;
    bool enable;
};

class RemoteConfig {
private:
    std::string url;
    bool connected;
    long long last_unconnected_time;
    std::map<std::string, SWConfig> cache;
public:
    /**
     * 初始化url
     * @param url 为空则使用默认url:<br>
     * http://18.168.0.6:18680<br>
     * http://10.168.10.136:18680<br>
     * http://192.168.8.8:18680
     */
    explicit RemoteConfig(const std::string &url);

    /**
     * 相当于RemoteConfig("")
     */
    explicit RemoteConfig();

    /**
     * 获取远程配置
     * @param name 远程配置名
     * @param cache_time 缓存时间，单位秒, 如果缓存时间内再次调用则直接返回缓存
     * @return 远程配置，如果获取失败则返回{name:"",value:"",enable:false}
     */
    SWConfig GetConfig(const std::string &name, long long cache_time = 60 * 5);

    ~RemoteConfig() = default;
};