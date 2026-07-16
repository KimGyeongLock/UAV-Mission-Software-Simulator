#pragma once

#include <chrono>

#include "core/Event.hpp"
#include "core/Telemetry.hpp"
#include "core/MissionConfig.hpp"

namespace uav::core {

class HealthMonitor {
public:
    explicit HealthMonitor(MissionConfig config = MissionConfig{});

    Event evaluate(const Telemetry& telemetry);

    Event checkTelemetryTimeout();

private: 
    MissionConfig config_;
    std::chrono::steady_clock::time_point last_telemetry_time_;
};

}  // namespace uav::core
