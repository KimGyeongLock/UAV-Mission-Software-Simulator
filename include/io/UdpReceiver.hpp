#pragma once

#include <cstdint>
#include <optional>

#include "core/Telemetry.hpp"

namespace uav::io {

class UdpReceiver {
public:
    explicit UdpReceiver(std::uint16_t port);

    // TODO: Open and bind UDP socket.
    bool start();

    // TODO: Receive UDP payload and parse it into Telemetry.
    std::optional<core::Telemetry> receive();

    // TODO: Close UDP socket and release resources.
    void stop();

private:
    std::uint16_t port_{0};
};

}  // namespace uav::io
