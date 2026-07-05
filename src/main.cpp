#include <iostream>

#include "core/Event.hpp"
#include "core/HealthMonitor.hpp"
#include "core/MissionController.hpp"
#include "core/Telemetry.hpp"

namespace {

const char* toString(uav::core::MissionState state) {
    switch (state) {
        case uav::core::MissionState::READY:
            return "READY";
        case uav::core::MissionState::TAKEOFF:
            return "TAKEOFF";
        case uav::core::MissionState::EN_ROUTE:
            return "EN_ROUTE";
        case uav::core::MissionState::MISSION_EXECUTE:
            return "MISSION_EXECUTE";
        case uav::core::MissionState::RETURN_HOME:
            return "RETURN_HOME";
        case uav::core::MissionState::LANDING:
            return "LANDING";
        case uav::core::MissionState::COMPLETE:
            return "COMPLETE";
        case uav::core::MissionState::ABORT:
            return "ABORT";
    }

    return "UNKNOWN";
}

const char* toString(uav::core::EventType event_type) {
    switch (event_type) {
        case uav::core::EventType::NONE:
            return "NONE";
        case uav::core::EventType::LOW_BATTERY:
            return "LOW_BATTERY";
        case uav::core::EventType::ENGINE_OVERHEAT:
            return "ENGINE_OVERHEAT";
        case uav::core::EventType::SIGNAL_WEAK:
            return "SIGNAL_WEAK";
        case uav::core::EventType::TELEMETRY_TIMEOUT:
            return "TELEMETRY_TIMEOUT";
        case uav::core::EventType::ALTITUDE_DEVIATION:
            return "ALTITUDE_DEVIATION";
        case uav::core::EventType::INVALID_TELEMETRY:
            return "INVALID_TELEMETRY";
    }

    return "UNKNOWN";
}

} // namespace

using namespace std;

int main() {
    uav::core::MissionController controller;
    uav::core::HealthMonitor monitor;

    cout << "UAV Mission Software Simulator\n";

    cout << "Initial state: " << toString(controller.state()) << "\n";

    controller.startMission();
    cout << "After startMission: " << toString(controller.state()) << "\n";

    controller.update();
    cout << "After update: " << toString(controller.state()) << "\n";

    uav::core::Telemetry telemetry;
    telemetry.battery_percent = 20.0;
    telemetry.engine_temperature_c = 60.0;
    telemetry.signal_strength_percent = 90.0;
    telemetry.altitude_m = 1000.0;
    telemetry.target_altitude_m = 1000.0;
    telemetry.valid = true;

    const auto event = monitor.evaluate(telemetry);
    cout << "Detected event: " << toString(event.type) << "\n";

    controller.handleEvent(event);
    cout << "After handleEvent: " << toString(controller.state()) << "\n";

    return 0;
}
