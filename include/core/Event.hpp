#pragma once

namespace uav::core {

enum class EventType {
    NONE,
    LOW_BATTERY,
    ENGINE_OVERHEAT,
    SIGNAL_WEAK,
    TELEMETRY_TIMEOUT,
    ALTITUDE_DEVIATION,
    INVALID_TELEMETRY
};

struct Event {
    EventType type{EventType::NONE};
};

}  // namespace uav::core
