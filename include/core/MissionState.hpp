#pragma once

namespace uav::core {

enum class MissionState {
    READY,              // 임무 시작 전 대기 상태
    TAKEOFF,            // 이륙 중인 상태
    EN_ROUTE,           // 임무 지역으로 이동 중인 상태
    MISSION_EXECUTE,    // 실제 임무를 수행하는 상태
    RETURN_HOME,        // 기지 또는 출발 지점으로 복귀하는 상태
    LANDING,            // 착륙 중인 상태
    COMPLETE,           // 임무가 정상 종료된 상태
    ABORT               // 임무 중단 상태
};

}
