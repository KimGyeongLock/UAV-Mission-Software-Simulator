#pragma once

#include <string>
#include <atomic>
#include <mutex>
#include <optional>
#include <thread>
#include <queue>

#include "core/Telemetry.hpp"
#include "io/UdpReceiver.hpp"

namespace uav::app {

class MissionApplication {
public:
    explicit MissionApplication(std::string config_path);

    int run();

private:
    std::string config_path_;
    std::atomic<bool> receiver_running_{false};
    std::thread receiver_thread_;

    std::mutex telemetry_mutex_;
    std::queue<uav::core::Telemetry> telemetry_queue_;

    void startReceiverThread(uav::io::UdpReceiver& receiver);
    void stopReceiverThread();
    std::optional<uav::core::Telemetry> popTelemetry();
};

}   // namespace uav::app