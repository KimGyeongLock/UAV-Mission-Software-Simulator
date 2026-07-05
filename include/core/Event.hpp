#pragma once

namespace uav::core {

enum class EventType {
    NONE,
    LOW_BATTERY,            // 배터리 부족 이벤트
    ENGINE_OVERHEAT,        // 엔진 또는 모터 온도 과열 이벤트
    SIGNAL_WEAK,            // 통신 신호 약화 이벤트
    TELEMETRY_TIMEOUT,      // Telemetry(무인기 상태 데이터)가 일정 시간 동안 들어오지 않는 이벤트
    ALTITUDE_DEVIATION,     // 고도 이탈 이벤트
    INVALID_TELEMETRY       // Telemetry 데이터 자체가 이상한 이벤트
};

struct Event {
    EventType type{EventType::NONE};
};

}
