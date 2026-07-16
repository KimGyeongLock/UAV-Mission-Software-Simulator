# Test Scenarios

## 시나리오 목록

| 시나리오 | 입력 조건 | 기대 이벤트 | 기대 상태 | 구현 상태 |
| --- | --- | --- | --- | --- |
| 정상 임무 수행 | 정상 배터리, 정상 엔진 온도, 정상 통신, 목표 고도 범위 유지 | NONE | READY -> TAKEOFF -> EN_ROUTE -> MISSION_EXECUTE -> RETURN_HOME -> LANDING -> COMPLETE | 구현 |
| 배터리 부족 복귀 | EN_ROUTE 또는 MISSION_EXECUTE 중 배터리 잔량이 임계값 미만 | LOW_BATTERY | RETURN_HOME | 구현 |
| 엔진 과열 Abort | 엔진 온도가 임계값 초과 | ENGINE_OVERHEAT | ABORT | 구현 |
| 통신 두절 Abort | 허용 시간 이상 Telemetry 미수신 | TELEMETRY_TIMEOUT | ABORT | 구현 |
| 고도 이탈 Warning | 현재 고도가 목표 고도 허용 오차 초과 | ALTITUDE_DEVIATION | 현재 임무 상태 유지 | 구현 |
| 비정상 Telemetry 처리 | 음수 배터리, 비정상 온도, 누락된 필드 등 | INVALID_TELEMETRY | 상태 유지 또는 ABORT 정책 확장 대상 | 일부 구현 |

## 시나리오별 샘플 로그 경로
| 시나리오 | 샘플 로그 |
| --- | --- |
| 배터리 부족 복귀 | `docs/sample-logs/low_battery_mission_log.csv` |
| 엔진 과열 Abort | `docs/sample-logs/engine_overheat_mission_log.csv` |
| 통신 두절 Abort | `docs/sample-logs/timeout_mission_log.csv` |

## GoogleTest 구현 현황

| 테스트 파일 | 검증 내용 |
| --- | --- |
| `MissionControllerTest.cpp` | 정상 상태 전이, 이벤트 기반 RETURN_HOME, 치명 이벤트 ABORT |
| `HealthMonitorTest.cpp` | 정상 Telemetry, 배터리 부족, 엔진 과열, 고도 이탈 탐지 |
| `TelemetryParsingTest.cpp` | 정상 payload 파싱, malformed payload 거부 |

현재 테스트 결과:

```text
100% tests passed, 0 tests failed out of 10
```
