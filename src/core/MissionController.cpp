#include "core/MissionController.hpp"

namespace uav::core {

MissionController::MissionController() = default;

void MissionController::startMission() {
    // TODO: Transition READY -> TAKEOFF when mission start command is accepted.
}

void MissionController::update() {
    // TODO: Implement normal mission state progression.
}

void MissionController::handleEvent(const Event& event) {
    (void)event;
    // TODO: Implement abnormal event response policy.
}

MissionState MissionController::state() const {
    return state_;
}

}  // namespace uav::core
