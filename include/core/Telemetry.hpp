#pragma once

#include <chrono>

namespace uav::core {

struct Telemetry {
    std::chrono::steady_clock::time_point received_at{};
    double latitude_deg{0.0};
    double longitude_deg{0.0};
    double altitude_m{0.0};
    double target_altitude_m{0.0};
    double battery_percent{100.0};
    double engine_temperature_c{0.0};
    double signal_strength_percent{100.0};
    bool valid{true};
};

}  // namespace uav::core
