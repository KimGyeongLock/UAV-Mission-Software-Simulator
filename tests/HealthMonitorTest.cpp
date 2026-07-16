#include <gtest/gtest.h>

#include "core/Event.hpp"
#include "core/HealthMonitor.hpp"
#include "core/Telemetry.hpp"

namespace {

uav::core::Telemetry createNormalTelemetry() {
    uav::core::Telemetry telemetry;
    telemetry.battery_percent = 80.0;
    telemetry.engine_temperature_c = 60.0;
    telemetry.signal_strength_percent = 90.0;
    telemetry.altitude_m = 1000.0;
    telemetry.target_altitude_m = 1000.0;
    telemetry.valid = true;
    return telemetry;
}

} // namespace

TEST(HealthMonitorTest, ReturnsNoneForNormalTelemetry) {
    uav::core::HealthMonitor monitor;

    const auto event = monitor.evaluate(createNormalTelemetry());

    EXPECT_EQ(event.type, uav::core::EventType::NONE);
}

TEST(HealthMonitorTest, DetectsLowBattery) {
    uav::core::HealthMonitor monitor;
    auto telemetry = createNormalTelemetry();
    telemetry.battery_percent = 20.0;

    const auto event = monitor.evaluate(telemetry);

    EXPECT_EQ(event.type, uav::core::EventType::LOW_BATTERY);
}

TEST(HealthMonitorTest, DetectsEngineOverheat) {
    uav::core::HealthMonitor monitor;
    auto telemetry = createNormalTelemetry();
    telemetry.engine_temperature_c = 95.9;

    const auto event = monitor.evaluate(telemetry);

    EXPECT_EQ(event.type, uav::core::EventType::ENGINE_OVERHEAT);
}

TEST(HealthMonitorTest, DetectsAltitudeDeviation) {
    uav::core::HealthMonitor monitor;
    auto telemetry = createNormalTelemetry();
    telemetry.altitude_m = 1080.0;

    const auto event = monitor.evaluate(telemetry);

    EXPECT_EQ(event.type, uav::core::EventType::ALTITUDE_DEVIATION);
}