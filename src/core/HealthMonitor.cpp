#include "core/HealthMonitor.hpp"

namespace uav::core {

HealthMonitor::HealthMonitor() = default;

Event HealthMonitor::evaluate(const Telemetry& telemetry) {
    (void)telemetry;
    // TODO: Detect LOW_BATTERY, ENGINE_OVERHEAT, SIGNAL_WEAK, ALTITUDE_DEVIATION, and INVALID_TELEMETRY.
    return Event{};
}

Event HealthMonitor::checkTelemetryTimeout() {
    // TODO: Detect TELEMETRY_TIMEOUT using configured timeout threshold.
    return Event{};
}

}  // namespace uav::core
