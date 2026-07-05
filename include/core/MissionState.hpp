#pragma once

namespace uav::core {

enum class MissionState {
    READY,
    TAKEOFF,
    EN_ROUTE,
    MISSION_EXECUTE,
    RETURN_HOME,
    LANDING,
    COMPLETE,
    ABORT
};

}  // namespace uav::core
