#pragma once

#include "core/Event.hpp"
#include "core/Telemetry.hpp"

namespace uav::core {

class HealthMonitor {
public:
    HealthMonitor();

    // TODO: Implement battery, engine, signal, altitude, timeout, and validity checks.
    Event evaluate(const Telemetry& telemetry);

    // TODO: Implement timeout detection based on last telemetry receive time.
    Event checkTelemetryTimeout();
};

}  // namespace uav::core
