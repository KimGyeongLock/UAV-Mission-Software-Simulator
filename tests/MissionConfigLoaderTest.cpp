#include <gtest/gtest.h>

#include "core/MissionConfigLoader.hpp"

TEST(MissionConfigLoaderTest, LoadsExistingConfigFile) {
    const auto config = uav::core::loadMissionConfigOrDefault("config/mission_config.json");

    EXPECT_DOUBLE_EQ(config.low_battery_percent, 25.0);
    EXPECT_DOUBLE_EQ(config.engine_overheat_celsius, 90.0);
    EXPECT_DOUBLE_EQ(config.signal_weak_percent, 30.0);
    EXPECT_EQ(config.telemetry_timeout.count(), 3000);
    EXPECT_DOUBLE_EQ(config.altitude_tolerance_m, 50.0);
    EXPECT_EQ(config.udp_port, 5005);
}