#include "core/MissionConfigLoader.hpp"

#include <fstream>
#include <sstream>
#include <string>

namespace {

double readDoubleOrDefault(const std::string& content, const std::string& key, double default_value) {
    const auto key_pos = content.find("\"" + key + "\"");
    if (key_pos == std::string::npos) {
        return default_value;
    }

    const auto colon_pos = content.find(':', key_pos);
    if (colon_pos == std::string::npos) {
        return default_value;
    }

    const auto value_start = content.find_first_of("-0123456789", colon_pos);
    if (value_start == std::string::npos) {
        return default_value;
    }

    const auto value_end = content.find_first_not_of("-0123456789.", value_start);
    const auto value = content.substr(value_start, value_end - value_start);

    try {
        return std::stod(value);
    } catch (...) {
        return default_value;
    }
}

int readIntOrDefault(const std::string& content, const std::string& key, int default_value) {
    return static_cast<int>(readDoubleOrDefault(content, key, static_cast<double>(default_value)));
}

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return {};
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

}   // namespace

namespace uav::core {

MissionConfig loadMissionConfigOrDefault(const std::string& path) {
    MissionConfig config;

    const auto content = readFile(path);
    if (content.empty()) {
        return config;
    }

    config.target_altitude_m = readDoubleOrDefault(content, "target_altitude_m", config.target_altitude_m);
    config.low_battery_percent = readDoubleOrDefault(content, "low_battery_percent", config.low_battery_percent);
    config.engine_overheat_celsius = readDoubleOrDefault(content, "engine_overheat_celsius", config.engine_overheat_celsius);
    config.signal_weak_percent = readDoubleOrDefault(content, "signal_weak_percent", config.signal_weak_percent);
    config.altitude_tolerance_m = readDoubleOrDefault(content, "altitude_tolerance_m", config.altitude_tolerance_m);
    config.udp_port = static_cast<std::uint16_t>(readIntOrDefault(content, "listen_port", config.udp_port));

    const auto timeout_ms = readIntOrDefault(
        content,
        "telemetry_timeout_ms",
        static_cast<int>(config.telemetry_timeout.count())
    );
    config.telemetry_timeout = std::chrono::milliseconds(timeout_ms);

    return config;
}

}