#pragma once

#include <chrono>
#include <cstdint>

namespace uav::core {

struct MissionConfig {
    double target_altitude_m{1000.0};
    double low_battery_percent{25.0};
    double engine_overheat_celsius{90.0};
    double signal_weak_percent{30.0};
    std::chrono::milliseconds telemetry_timeout{3000};
    double altitude_tolerance_m{50.0};

    std::uint16_t udp_port{5005};
};

}