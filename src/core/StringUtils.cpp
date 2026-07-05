#include "core/StringUtils.hpp"

namespace uav::core {

const char* toString(MissionState state) {
    switch (state) {
        case MissionState::READY:
            return "READY";
        case MissionState::TAKEOFF:
            return "TAKEOFF";
        case MissionState::EN_ROUTE:
            return "EN_ROUTE";
        case MissionState::MISSION_EXECUTE:
            return "MISSION_EXECUTE";
        case MissionState::RETURN_HOME:
            return "RETURN_HOME";
        case MissionState::LANDING:
            return "LANDING";
        case MissionState::COMPLETE:
            return "COMPLETE";
        case MissionState::ABORT:
            return "ABORT";
    }

    return "UNKNOWN";
}

const char* toString(EventType event_type) {
    switch (event_type) {
        case EventType::NONE:
            return "NONE";
        case EventType::LOW_BATTERY:
            return "LOW_BATTERY";
        case EventType::ENGINE_OVERHEAT:
            return "ENGINE_OVERHEAT";
        case EventType::SIGNAL_WEAK:
            return "SIGNAL_WEAK";
        case EventType::TELEMETRY_TIMEOUT:
            return "TELEMETRY_TIMEOUT";
        case EventType::ALTITUDE_DEVIATION:
            return "ALTITUDE_DEVIATION";
        case EventType::INVALID_TELEMETRY:
            return "INVALID_TELEMETRY";
    }

    return "UNKNOWN";
}

}  // namespace uav::core