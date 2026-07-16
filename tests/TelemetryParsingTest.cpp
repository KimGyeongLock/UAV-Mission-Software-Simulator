#include <gtest/gtest.h>

#include "core/TelemetryParser.hpp"

TEST(TelemetryParsingTest, ParseValidPayload) {
    const auto telemetry = uav::core::parseTelemetryLine("80.0,60.0,90.0,1000.0,1000.0,1");

    ASSERT_TRUE(telemetry.has_value());
    EXPECT_DOUBLE_EQ(telemetry->battery_percent, 80.0);
    EXPECT_DOUBLE_EQ(telemetry->engine_temperature_c, 60.0);
    EXPECT_DOUBLE_EQ(telemetry->signal_strength_percent, 90.0);
    EXPECT_DOUBLE_EQ(telemetry->altitude_m, 1000.0);
    EXPECT_DOUBLE_EQ(telemetry->target_altitude_m, 1000.0);
    EXPECT_TRUE(telemetry->valid);
}


TEST(TelemetryParsingTest, RejectsMalformedPayload) {
    const auto telemetry = uav::core::parseTelemetryLine("invalid,payload");

    EXPECT_FALSE(telemetry.has_value());
}