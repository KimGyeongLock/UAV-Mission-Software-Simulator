# Event Policy

## EventType 목록

| 이벤트 | 의미 |
| --- | --- |
| NONE | 이벤트 없음 |
| LOW_BATTERY | 배터리 잔량 부족 |
| ENGINE_OVERHEAT | 엔진 온도 과열 |
| SIGNAL_WEAK | 통신 신호 약화 |
| TELEMETRY_TIMEOUT | 일정 시간 이상 Telemetry 미수신 |
| ALTITUDE_DEVIATION | 기준 고도 범위 이탈 |
| INVALID_TELEMETRY | Telemetry 값 또는 형식 이상 |

## 탐지 조건과 대응 상태

| 이벤트 | 탐지 조건 초안 | 대응 상태 | 대응 정책 |
| --- | --- | --- | --- |
| LOW_BATTERY | 배터리 잔량이 설정 임계값 미만 | RETURN_HOME | 임무 수행을 중단하고 복귀 절차로 전환 |
| ENGINE_OVERHEAT | 엔진 온도가 설정 임계값 초과 | ABORT | 치명 이벤트로 판단하고 임무 중단 |
| SIGNAL_WEAK | 통신 품질 또는 RSSI가 설정 임계값 미만 | 현재 상태 유지 또는 RETURN_HOME | 지속 시간에 따라 경고 또는 복귀 판단 |
| TELEMETRY_TIMEOUT | 마지막 Telemetry 수신 이후 허용 시간이 초과 | ABORT | 상태 판단 불가로 임무 중단 |
| ALTITUDE_DEVIATION | 현재 고도가 목표 고도 허용 오차를 초과 | 현재 상태 유지 | Warning 이벤트 기록 후 보정 대상 표시 |
| INVALID_TELEMETRY | 배터리 음수, 온도 비정상, 좌표 누락 등 | 현재 상태 유지 또는 ABORT | 단발성 오류는 무시/로그, 반복 오류는 Abort 검토 |

## 구현 시 확정할 파라미터

| 항목 | 예시 값 | 설정 위치 |
| --- | --- | --- |
| 배터리 부족 임계값 | 25% | `config/mission_config.json` |
| 엔진 과열 임계값 | 90 degC | `config/mission_config.json` |
| Telemetry timeout | 3000 ms | `config/mission_config.json` |
| 고도 허용 오차 | 50 m | `config/mission_config.json` |
| 약한 신호 임계값 | 30% | `config/mission_config.json` |
