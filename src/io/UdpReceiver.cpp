#include "io/UdpReceiver.hpp"

#include <array>
#include <cstring>
#include <string>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "core/TelemetryParser.hpp"

namespace uav::io {

UdpReceiver::UdpReceiver(std::uint16_t port) : port_(port) {}

bool UdpReceiver::start() {
    socket_fd_ = ::socket(AF_INET, SOCK_DGRAM, 0); // IPv4 UDP socket 생성
    if (socket_fd_ < 0) {
        return false;
    }

    sockaddr_in address{}; // IPv4 주소 정보를 담는 구조체
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // 내 컴퓨터의 어떤 IP로 들어와도 받겠다
    address.sin_port = htons(port_); // 수신할 포트 설정

    if (::bind(socket_fd_, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0) { // socket을 특정 포트에 연결
        ::close(socket_fd_);
        socket_fd_ = -1;
        return false;
    }

    return true;
}

std::optional<core::Telemetry> UdpReceiver::receive() {
    if (socket_fd_ < 0) { // socket이 열렸는지 확인
        return std::nullopt;
    }

    std::array<char, 1024> buffer{}; // UDP로 받은 문자열을 담을 공간, 최대 1024바이트

    const auto received_size = ::recvfrom( // UDP 데이터를 받는 함수 / blocking 함수
        socket_fd_,
        buffer.data(),
        buffer.size() - 1,
        0,
        nullptr,
        nullptr
    );

    if (received_size <= 0) {
        return std::nullopt;
    }

    std::string payload(buffer.data(), static_cast<std::size_t>(received_size)); // buffer에 들어온 raw 데이터를 C++ 문자열로 변환
    return core::parseTelemetryLine(payload);
}

void UdpReceiver::stop() {
    if (socket_fd_ >= 0) {
        ::close(socket_fd_);
        socket_fd_ = -1;
    }
}

}  // namespace uav::io
