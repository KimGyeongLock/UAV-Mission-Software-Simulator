#include "io/UdpReceiver.hpp"

namespace uav::io {

UdpReceiver::UdpReceiver(std::uint16_t port) : port_(port) {}

bool UdpReceiver::start() {
    // TODO: Create UDP socket and bind to port_.
    return true;
}

std::optional<core::Telemetry> UdpReceiver::receive() {
    // TODO: Receive UDP payload and parse Telemetry.
    return std::nullopt;
}

void UdpReceiver::stop() {
    // TODO: Close UDP socket.
}

}  // namespace uav::io
