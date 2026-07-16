#pragma once

#include <string>
#include <atomic>
#include <mutex>
#include <optional>
#include <thread>

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
    std::optional<uav::core::Telemetry> latest_telemetry_;

    void startReceiverThread(uav::io::UdpReceiver& receiver);
    void stopReceiverThread();
    std::optional<uav::core::Telemetry> consumeLatestTelemetry();
};

}   // namespace uav::app