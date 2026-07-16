#pragma once

#include <chrono>

#include "core/Event.hpp"
#include "core/Telemetry.hpp"

namespace uav::core {

class HealthMonitor {
public:
    HealthMonitor();

    Event evaluate(const Telemetry& telemetry);

    Event checkTelemetryTimeout();

private: 
    std::chrono::steady_clock::time_point last_telemetry_time_;
};

}  // namespace uav::core
