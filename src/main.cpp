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
#include "core/TelemetryParser.hpp"

using namespace std;

namespace {

uav::core::Telemetry createTelemetryForTick(int tick_count) {
    string line = "80.0,60.0,90.0,1000.0,1000.0,1";

    if (tick_count >= 2) {
        line = "20.0,60.0,90.0,1000.0,1000.0,1";
    }

    const auto telemetry = uav::core::parseTelemetryLine(line);
    if (telemetry.has_value()) {
        return telemetry.value();
    }

    uav::core::Telemetry invalid;
    invalid.valid = false;
    return invalid;
}

} // namespace

int main() {
    uav::core::MissionController controller;
    uav::core::HealthMonitor monitor;

    uav::io::CsvLogger logger("mission_log.csv");
    if (!logger.open()) {
        cerr << "Failed to open mission_log.csv\n";
        return 1;
    }

    int tick_count = 0;

    controller.startMission();

    uav::scheduler::PeriodicTask task(
        [&]() {
            const auto telemetry = createTelemetryForTick(tick_count);

            const auto event = monitor.evaluate(telemetry);

            if (event.type == uav::core::EventType::NONE) {
                controller.update();
            } else {
                controller.handleEvent(event);
            }
            logger.log(controller.state(), event, telemetry);

            cout << "tick=" << tick_count
                 << " state=" << uav::core::toString(controller.state())
                 << " event=" << uav::core::toString(event.type)
                 << " battery=" << telemetry.battery_percent
                 << '\n';

            tick_count++;
        },
        std::chrono::milliseconds(1000)
    );

    task.start();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    task.stop();
    logger.close();

    return 0;
}
