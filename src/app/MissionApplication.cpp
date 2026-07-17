#include "app/MissionApplication.hpp"

#include <chrono>
#include <iostream>
#include <thread>
#include <utility>

#include "core/Event.hpp"
#include "core/HealthMonitor.hpp"
#include "core/MissionConfigLoader.hpp"
#include "core/MissionController.hpp"
#include "core/MissionState.hpp"
#include "core/StringUtils.hpp"
#include "core/Telemetry.hpp"
#include "io/CsvLogger.hpp"
#include "io/UdpReceiver.hpp"
#include "scheduler/PeriodicTask.hpp"

namespace {

constexpr int kMaxSimulationTicks = 10;
constexpr auto kMissionTaskPeriod = std::chrono::milliseconds(1000);
constexpr auto kMainLoopSleep = std::chrono::milliseconds(100);

}  // namespace

namespace uav::app {

bool shouldFinishMission(core::MissionState state, int tick_count) {
    return state == core::MissionState::COMPLETE ||
           state == core::MissionState::ABORT ||
           tick_count >= kMaxSimulationTicks;
}

MissionApplication::MissionApplication(std::string config_path)
    : config_path_(std::move(config_path)) {}

int MissionApplication::run() {
    const auto config = core::loadMissionConfigOrDefault(config_path_);

    core::MissionController controller;
    core::HealthMonitor monitor(config);
    io::UdpReceiver receiver(config.udp_port);

    if (!receiver.start()) {
        std::cerr << "Failed to start UDP receiver\n";
        return 1;
    }

    io::CsvLogger logger("mission_log.csv");
    if (!logger.open()) {
        std::cerr << "Failed to open mission_log.csv\n";
        receiver.stop();
        return 1;
    }

    int tick_count = 0;
    bool mission_finished = false;

    controller.startMission();

    scheduler::PeriodicTask task(
        [&]() {
            const auto telemetry = popTelemetry();

            if (!telemetry.has_value()) {
                const auto event = monitor.checkTelemetryTimeout();

                if (event.type == core::EventType::TELEMETRY_TIMEOUT) {
                    controller.handleEvent(event);

                    core::Telemetry empty_telemetry;
                    empty_telemetry.valid = false;

                    logger.log(controller.state(), event, empty_telemetry);

                    std::cout << "tick=" << tick_count
                              << " state=" << core::toString(controller.state())
                              << " event=" << core::toString(event.type)
                              << " no telemetry received"
                              << '\n';
                } else {
                    std::cout << "tick=" << tick_count << " no telemetry received\n";
                }

                tick_count++;

                mission_finished = shouldFinishMission(controller.state(), tick_count);

                return;
            }

            const auto event = monitor.evaluate(telemetry.value());

            if (event.type == core::EventType::NONE) {
                controller.update();
            } else {
                controller.handleEvent(event);
            }

            logger.log(controller.state(), event, telemetry.value());

            std::cout << "tick=" << tick_count
                      << " state=" << core::toString(controller.state())
                      << " event=" << core::toString(event.type)
                      << " battery=" << telemetry->battery_percent
                      << '\n';

            tick_count++;

            mission_finished = shouldFinishMission(controller.state(), tick_count);
        },
        kMissionTaskPeriod
    );

    startReceiverThread(receiver);
    task.start();

    while (!mission_finished) {
        std::this_thread::sleep_for(kMainLoopSleep);
    }

    task.stop();
    stopReceiverThread();
    receiver.stop();
    logger.close();

    return 0;
}

constexpr std::size_t kMaxTelemetryQueueSize = 100;

void MissionApplication::startReceiverThread(io::UdpReceiver& receiver) {
    receiver_running_ = true;

    /*
        receiver thread는 UDP 데이터를 기다린다.
        데이터가 오면 latest_telemetry_에 저장한다.
    */
    receiver_thread_ = std::thread([this, &receiver]() {
        while (receiver_running_) {
            auto telemetry = receiver.receive();

            if (telemetry.has_value()) {
                std::lock_guard<std::mutex> lock(telemetry_mutex_);

                if (telemetry_queue_.size() >= kMaxTelemetryQueueSize) {
                    telemetry_queue_.pop();
                }
            
                telemetry_queue_.push(telemetry.value());
            }
        }
    });
}

void MissionApplication::stopReceiverThread() {
    receiver_running_ = false;

    if (receiver_thread_.joinable()) {
        receiver_thread_.join();
    }
}

std::optional<core::Telemetry> MissionApplication::popTelemetry() {
    std::lock_guard<std::mutex> lock(telemetry_mutex_);

    if (telemetry_queue_.empty()) {
        return std::nullopt;
    }

    auto telemetry = telemetry_queue_.front();
    telemetry_queue_.pop();

    return telemetry;
}

}  // namespace uav::app