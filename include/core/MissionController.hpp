#pragma once

#include "core/Event.hpp"
#include "core/MissionState.hpp"

namespace uav::core {

class MissionController {
public:
    MissionController();

    // TODO: Implement mission start transition from READY to TAKEOFF.
    void startMission();

    // TODO: Implement normal mission progress transitions.
    void update();

    // TODO: Implement event-driven transition policy.
    void handleEvent(const Event& event);

    // TODO: Add helper methods for state validation if needed.
    MissionState state() const;

private:
    MissionState state_{MissionState::READY};
};

}  // namespace uav::core
