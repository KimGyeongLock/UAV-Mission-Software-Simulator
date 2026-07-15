#pragma once

#include <cstdint>
#include <optional>

#include "core/Telemetry.hpp"

namespace uav::io {

class UdpReceiver {
public:
    explicit UdpReceiver(std::uint16_t port);

    bool start();

    std::optional<core::Telemetry> receive();

    void stop();

private:
    std::uint16_t port_{0}; // 수신할 UDP 포트 번호
    int socket_fd_{-1};     // 운영체제가 만든 socket을 가리키는 번호
};

}  // namespace uav::io
