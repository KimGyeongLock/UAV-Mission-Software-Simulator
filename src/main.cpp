#include <iostream>

#include "core/Event.hpp"
#include "core/HealthMonitor.hpp"
#include "core/MissionController.hpp"
#include "core/Telemetry.hpp"
#include "core/StringUtils.hpp"
#include "io/CsvLogger.hpp"

using namespace std;

int main() {
    uav::core::MissionController controller;
    uav::core::HealthMonitor monitor;

    uav::io::CsvLogger logger("mission_log.csv");
    if (!logger.open()) {
        cerr << "Failed to open mission_log.csv\n";
        return 1;
    }

    cout << "UAV Mission Software Simulator\n";

    cout << "Initial state: " << uav::core::toString(controller.state()) << "\n";

    controller.startMission();
    cout << "After startMission: " << uav::core::toString(controller.state()) << "\n";

    controller.update();
    cout << "After update: " << uav::core::toString(controller.state()) << "\n";

    uav::core::Telemetry telemetry;
    telemetry.battery_percent = 20.0;
    telemetry.engine_temperature_c = 60.0;
    telemetry.signal_strength_percent = 90.0;
    telemetry.altitude_m = 1000.0;
    telemetry.target_altitude_m = 1000.0;
    telemetry.valid = true;

    const auto event = monitor.evaluate(telemetry);
    cout << "Detected event: " << uav::core::toString(event.type) << "\n";

    controller.handleEvent(event);
    cout << "After handleEvent: " << uav::core::toString(controller.state()) << "\n";

    logger.log(controller.state(), event, telemetry);
    logger.close();

    return 0;
}
