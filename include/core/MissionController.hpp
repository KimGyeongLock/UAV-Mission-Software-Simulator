#pragma once // 이 헤더 파일이 한 번만 포함되게 해주는 지시문

#include "core/Event.hpp"
#include "core/MissionState.hpp"

namespace uav::core {

class MissionController {
public:
    MissionController();

    void startMission();

    void update();

    void handleEvent(const Event& event);

    // TODO: Add helper methods for state validation if needed.
    MissionState state() const;

private:
    MissionState state_{MissionState::READY};
};

}  // namespace uav::core
