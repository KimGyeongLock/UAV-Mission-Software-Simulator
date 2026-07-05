#include "core/MissionController.hpp"

namespace uav::core {

MissionController::MissionController() = default;

void MissionController::startMission() {
    if (state_ == MissionState::READY) {
        state_ = MissionState::TAKEOFF;
    }
}

void MissionController::update() {
    switch (state_)
    {
    case MissionState::TAKEOFF:
        state_ = MissionState::EN_ROUTE;
        break;
    
    case MissionState::EN_ROUTE:
        state_ = MissionState::MISSION_EXECUTE;
        break;

    case MissionState::MISSION_EXECUTE:
        state_ = MissionState::RETURN_HOME;
        break;

    case MissionState::RETURN_HOME:
        state_ = MissionState::LANDING;
        break;

    case MissionState::LANDING:
        state_ = MissionState::COMPLETE;
        break;

    default:
        break;
    }
}

void MissionController::handleEvent(const Event& event) {
    switch (event.type) {
    case EventType::LOW_BATTERY:
        if (state_ != MissionState::COMPLETE && state_ != MissionState::ABORT) {
            state_ = MissionState::RETURN_HOME;
        }
        break;
    
    case EventType::ENGINE_OVERHEAT:
    case EventType::TELEMETRY_TIMEOUT:
        if (state_ != MissionState::COMPLETE) {
            state_ = MissionState::ABORT;
        }
        break;

    case EventType::SIGNAL_WEAK:
    case EventType::ALTITUDE_DEVIATION:
    case EventType::INVALID_TELEMETRY: // 연속 invalid 3회 이상시 ABORT
    case EventType::NONE:
    default:
        break;
    }
}

MissionState MissionController::state() const {
    return state_;
}

}  // namespace uav::core
