#include "core/HealthMonitor.hpp"
#include <cmath>

namespace {
    
    constexpr double kLowBatteryThreshold = 25.0;
    constexpr double kEngineOverheatThreshold = 90.0;
    constexpr double kSignalWeakThreshold = 30.0;
    constexpr double kAltitudeTolerance = 50.0;

} // namespace

namespace uav::core {

HealthMonitor::HealthMonitor() = default;

Event HealthMonitor::evaluate(const Telemetry& telemetry) {
    if (!telemetry.valid ||
        telemetry.battery_percent < 0.0 ||
        telemetry.battery_percent > 100.0 ||
        telemetry.signal_strength_percent < 0.0 ||
        telemetry.signal_strength_percent > 100.0) {
        return Event{EventType::INVALID_TELEMETRY};
    }

    if (telemetry.engine_temperature_c > kEngineOverheatThreshold) {
        return Event{EventType::ENGINE_OVERHEAT};
    }

    if (telemetry.battery_percent < kLowBatteryThreshold) {
        return Event{EventType::LOW_BATTERY};
    }

    if (telemetry.signal_strength_percent < kSignalWeakThreshold) {
        return Event{EventType::SIGNAL_WEAK};
    }

    if (std::abs(telemetry.altitude_m - telemetry.target_altitude_m) > kAltitudeTolerance) {
        return Event{EventType::ALTITUDE_DEVIATION};
    }

    return Event{EventType::NONE};
}

Event HealthMonitor::checkTelemetryTimeout() {
    // TODO: Detect TELEMETRY_TIMEOUT using configured timeout threshold.
    return Event{};
}

}  // namespace uav::core
