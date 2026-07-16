#include "core/HealthMonitor.hpp"
#include <cmath>
#include <chrono>

namespace uav::core {

HealthMonitor::HealthMonitor(MissionConfig config)
    : config_(config),
      last_telemetry_time_(std::chrono::steady_clock::now()) {}

Event HealthMonitor::evaluate(const Telemetry& telemetry) {
    last_telemetry_time_ = std::chrono::steady_clock::now(); // 마지막 수신 시간
    if (!telemetry.valid ||
        telemetry.battery_percent < 0.0 ||
        telemetry.battery_percent > 100.0 ||
        telemetry.signal_strength_percent < 0.0 ||
        telemetry.signal_strength_percent > 100.0) {
        return Event{EventType::INVALID_TELEMETRY};
    }

    if (telemetry.engine_temperature_c > config_.engine_overheat_celsius) {
        return Event{EventType::ENGINE_OVERHEAT};
    }

    if (telemetry.battery_percent < config_.low_battery_percent) {
        return Event{EventType::LOW_BATTERY};
    }

    if (telemetry.signal_strength_percent < config_.signal_weak_percent) {
        return Event{EventType::SIGNAL_WEAK};
    }

    if (std::abs(telemetry.altitude_m - telemetry.target_altitude_m) > config_.altitude_tolerance_m) {
        return Event{EventType::ALTITUDE_DEVIATION};
    }

    return Event{EventType::NONE};
}

Event HealthMonitor::checkTelemetryTimeout() {
    const auto now = std::chrono::steady_clock::now();

    if (now - last_telemetry_time_ > config_.telemetry_timeout) {
        return Event{EventType::TELEMETRY_TIMEOUT};
    }

    return Event{EventType::NONE};
}

}  // namespace uav::core
