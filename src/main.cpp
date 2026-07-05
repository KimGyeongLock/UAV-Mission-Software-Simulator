#include <iostream>
#include <chrono>
#include <thread>

#include "core/Event.hpp"
#include "core/HealthMonitor.hpp"
#include "core/MissionController.hpp"
#include "core/Telemetry.hpp"
#include "core/StringUtils.hpp"
#include "io/CsvLogger.hpp"
#include "scheduler/PeriodicTask.hpp"

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

    //
    logger.log(controller.state(), event, telemetry);
    logger.close();

    //
    int tick_count = 0;
    uav::scheduler::PeriodicTask task(
        [&]() {
            std::cout << "Periodic tick: " << tick_count << "\n";
            ++tick_count;
        },
        std::chrono::milliseconds(1000)
    );

    task.start();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    task.stop();

    return 0;
}
