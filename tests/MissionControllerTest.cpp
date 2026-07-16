#include <gtest/gtest.h>

#include "core/Event.hpp"
#include "core/MissionController.hpp"
#include "core/MissionState.hpp"

TEST(MissionControllerTest, StartsMissionFromReadyToTakeoff) {
    uav::core::MissionController controller;

    controller.startMission();

    EXPECT_EQ(controller.state(), uav::core::MissionState::TAKEOFF);
}

TEST(MissionControllerTest, ProgressesNormalMissionStates) {
    uav::core::MissionController controller;

    controller.startMission();
    controller.update();

    EXPECT_EQ(controller.state(), uav::core::MissionState::EN_ROUTE);

    controller.update();
    EXPECT_EQ(controller.state(), uav::core::MissionState::MISSION_EXECUTE);

        controller.update();
    EXPECT_EQ(controller.state(), uav::core::MissionState::RETURN_HOME);

    controller.update();
    EXPECT_EQ(controller.state(), uav::core::MissionState::LANDING);

    controller.update();
    EXPECT_EQ(controller.state(), uav::core::MissionState::COMPLETE);
}

TEST(MissionControllerTest, LowBatteryTransitionsToReturnHome) {
    uav::core::MissionController controller;

    controller.startMission();
    controller.update();

    controller.handleEvent(uav::core::Event{uav::core::EventType::LOW_BATTERY});

    EXPECT_EQ(controller.state(), uav::core::MissionState::RETURN_HOME);
}

TEST(MissionControllerTest, EngineOverheatTransitionsToAbort) {
    uav::core::MissionController controller;

    controller.startMission();

    controller.handleEvent(uav::core::Event{uav::core::EventType::ENGINE_OVERHEAT});

    EXPECT_EQ(controller.state(), uav::core::MissionState::ABORT);
}