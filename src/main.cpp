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
#include "io/UdpReceiver.hpp"

using namespace std;

int main() {
    uav::core::MissionConfig config;
    uav::core::MissionController controller;
    uav::core::HealthMonitor monitor(config);
    uav::io::UdpReceiver receiver(config.udp_port);

    if (!receiver.start()) {
        cerr << "Failed to start UDP receiver\n";
        return 1;
    }

    uav::io::CsvLogger logger("mission_log.csv");
    if (!logger.open()) {
        cerr << "Failed to open mission_log.csv\n";
        return 1;
    }

    int tick_count = 0;
    bool mission_finished = false;

    controller.startMission();

    uav::scheduler::PeriodicTask task(
        [&]() {
            const auto telemetry = receiver.receive();

            if (!telemetry.has_value()) {
                const auto event = monitor.checkTelemetryTimeout();

                if (event.type == uav::core::EventType::TELEMETRY_TIMEOUT) {
                    controller.handleEvent(event);

                    uav::core::Telemetry empty_telemetry;
                    empty_telemetry.valid = false;

                    logger.log(controller.state(), event, empty_telemetry);

                    cout << "tick=" << tick_count
                         << " state=" << uav::core::toString(controller.state())
                         << " event=" << uav::core::toString(event.type)
                         << " no telemetry received"
                         << '\n';
                } else {
                    cout << "tick=" << tick_count << " no telemetry received\n";
                }

                tick_count++;

                if (controller.state() == uav::core::MissionState::COMPLETE ||
                    controller.state() == uav::core::MissionState::ABORT ||
                    tick_count >= 10) {
                    mission_finished = true;
                }

                return;
            }

            const auto event = monitor.evaluate(telemetry.value());

            if (event.type == uav::core::EventType::NONE) {
                controller.update();
            } else {
                controller.handleEvent(event);
            }
            logger.log(controller.state(), event, telemetry.value());

            cout << "tick=" << tick_count
                 << " state=" << uav::core::toString(controller.state())
                 << " event=" << uav::core::toString(event.type)
                 << " battery=" << telemetry->battery_percent
                 << '\n';

            tick_count++;

            if (controller.state() == uav::core::MissionState::COMPLETE ||
                controller.state() == uav::core::MissionState::ABORT ||
                tick_count >= 10) {
                mission_finished = true;
            }
        },
        std::chrono::milliseconds(1000)
    );

    task.start();

    /*
    주기 task는 별도 thread에서 돈다.
    main thread는 mission_finished가 true 될 때까지 100ms마다 확인한다.
    완료/중단/최대 tick 도달 시 task.stop().    
    */
    while (!mission_finished) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    task.stop();
    receiver.stop();
    logger.close();

    return 0;
}
