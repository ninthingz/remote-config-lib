#include<httplib.h>
#include<remote_config.h>
#include <thread>
#include <nlohmann/json.hpp>

RemoteConfig::RemoteConfig()
        : RemoteConfig("") {

}

RemoteConfig::RemoteConfig(const std::string &url) {
    connected = false;
    last_unconnected_time = 0;
    if (url.empty()) {
        std::thread([=]() {
            std::string server_url = "http://18.168.0.6:18680";
            httplib::Client cli(server_url);
            auto res = cli.Get("/api/v1/config");
            if (res && res->status == 200) {
                connected = true;
                this->url = server_url;
            }
        }).detach();

        std::thread([=]() {
            std::string server_url = "http://10.168.10.136:18680";
            httplib::Client cli(server_url);
            auto res = cli.Get("/api/v1/config");
            if (res && res->status == 200) {
                connected = true;
                this->url = server_url;
            }
        }).detach();

        std::thread([=]() {
            std::string server_url = "http://192.168.8.8:18680";
            httplib::Client cli(server_url);
            auto res = cli.Get("/api/v1/config");
            if (res && res->status == 200) {
                connected = true;
                this->url = server_url;
            }
        }).detach();

        for (int i = 0; i < 100; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            if (connected) {
                break;
            }
        }
    } else {
        connected = true;
        this->url = url;
    }
}

std::string urlEncode(const char* str) {
    std::string encoded_string;
    while (*str) {
        // 字母 数字 -_.~ 直接编码，不翻译
        if (isalnum((unsigned char)*str) || strchr("-_.~", *str)) {
            encoded_string += *str;
        } else {
            char buf[4];
            snprintf(buf, sizeof(buf), "%%%02X", (unsigned char)*str);
            encoded_string += buf;
        }
        str++;
    }
    return encoded_string;
}


SWConfig RemoteConfig::GetConfig(const std::string &name, long long cache_time) {
    auto now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    if (cache.find(name) != cache.end()) {
        auto config = cache[name];
        if (now_time - config.update_time < cache_time) {
            return config;
        }
    }

    if (!connected) {
        if (now_time - last_unconnected_time < 60 * 5) {
            return SWConfig{"", "", false};
        }
    }

    httplib::Client cli(url);
    auto res = cli.Get("/api/v1/config/element?name=" + urlEncode(name.c_str()));
    cli.set_default_headers({
                                    {"Content-Type", "application/json"},
                                    {"SIGN",         "0D000721"}
                            });
    if (res && res->status == 200) {
        connected = true;
        auto json = nlohmann::json::parse(res->body);
        if (json["code"] == 200) {
            auto config = SWConfig{json["data"]["name"], json["data"]["value"], now_time, json["data"]["enable"]};
            if (cache.find(name) != cache.end()) {
                cache[name] = config;
            }
            else {
                cache.insert(std::make_pair(name, config));
            }
            return config;
        } else {
            nlohmann::json j;
            j["name"] = name;
            j["value"] = "";
            j["message"] = "";
            j["secret"] = "";
            j["enable"] = false;
            res = cli.Post("/api/v1/config", j.dump(), "application/json");
            auto json2 = nlohmann::json::parse(res->body);
            return SWConfig{"", "", 0, false};
        }
    } else {
        last_unconnected_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        connected = false;
        return SWConfig{"", "", 0, false};
    }
}


