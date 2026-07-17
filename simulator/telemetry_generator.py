import socket
import time
import argparse
import random

UDP_IP = "127.0.0.1"
UDP_PORT = 5005

def send_payload(sock, payload):
    sock.sendto(payload.encode("utf-8"), (UDP_IP, UDP_PORT)) # 문자열을 byte 데이터로 변환
    print(f"sent: {payload}")

def normal_mission():
    """정상 임무 완료를 위한 정상 Telemetry 시퀀스를 전송"""
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        for _ in range(3): # 3번 전송
            payload = "80.0,60.0,90.0,1000.0,1000.0,1"
            send_payload(sock, payload)
            time.sleep(1)


def low_battery_return():
    """배터리가 복귀 임계값 아래로 떨어지는 Telemetry를 전송"""
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        payloads = [
            "80.0,60.0,90.0,1000.0,1000.0,1",
            "80.0,60.0,90.0,1000.0,1000.0,1",
            "20.0,60.0,90.0,1000.0,1000.0,1", # 배터리 20%
        ]

        for payload in payloads:
            send_payload(sock, payload)
            time.sleep(1)


def engine_overheat_abort():
    """엔진 과열 임계값을 초과하는 Telemetry를 전송"""
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        payloads = [
            "80.0,60.0,90.0,1000.0,1000.0,1",
            "80.0,60.0,90.0,1000.0,1000.0,1",
            "80.0,95.0,90.0,1000.0,1000.0,1", # 엔진 온도 95도
        ]

        for payload in payloads:
            send_payload(sock, payload)
            time.sleep(1)


def telemetry_timeout_abort():
    """timeout 처리가 발생할 만큼 충분히 오래 Telemetry 전송을 멈춤"""
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        payloads = [
            "80.0,60.0,90.0,1000.0,1000.0,1",
            "80.0,60.0,90.0,1000.0,1000.0,1",
        ]

        for payload in payloads:
            send_payload(sock, payload)
            time.sleep(1)

        print("stop sending telemetry to trigger timeout")
        time.sleep(5)


def altitude_deviation_warning():
    """허용 오차를 벗어난 고도 Telemetry를 전송"""
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        payloads = [
            "80.0,60.0,90.0,1000.0,1000.0,1",
            "80.0,60.0,90.0,1080.0,1000.0,1", # 80m 차이
        ]

        for payload in payloads:
            send_payload(sock, payload)
            time.sleep(1)

def make_payload(
    battery=80.0,
    engine_temp=60.0,
    signal=90.0,
    altitude=1000.0,
    target_altitude=1000.0,
    valid=1,
):
    return f"{battery:.1f},{engine_temp:.1f},{signal:.1f},{altitude:.1f},{target_altitude:.1f},{valid}"

def random_stream(duration, interval):
    """정상 범위 내에서 랜덤 Telemetr를 지속 전송한다."""
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        steps = int(duration / interval)
        battery = 95.0

        for _ in range(steps):
            battery = max(30.0, battery - random.uniform(0.1, 0.5))
            engine_temp = random.uniform(55.0, 70.0)
            signal = random.uniform(70.0, 100.0)
            altitude = random.uniform(980.0, 1020.0)

            payload = make_payload(
                battery=battery,
                engine_temp=engine_temp,
                signal=signal,
                altitude=altitude,
            )

            send_payload(sock, payload)
            time.sleep(interval)

def apply_fault(payload_values, fault):
    battery, engine_temp, signal, altitude, target_altitude, valid = payload_values

    if fault == "low_battery":
        battery = 20.0
    elif fault == "engine_overheat":
        engine_temp = 95.0
    elif fault == "signal_weak":
        signal = 20.0
    elif fault == "altitude_deviation":
        altitude = target_altitude + 100.0
    elif fault == "invalid_telemetry":
        battery = -1.0
        valid = 0

    return battery, engine_temp, signal, altitude, target_altitude, valid

def fault_injection(fault, after, duration, interval):
    """정상 Telemetry 전송 중 지정 시점 이후 fault를 주입"""
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        steps = int(duration / interval)

        for step in range(steps):
            elapsed = step * interval

            if fault == "timeout" and elapsed >= after:
                print("stop sending telemetry to trigger timeout")
                time.sleep(max(0, duration - elapsed))
                break

            values = (80.0, 60.0, 90.0, 1000.0, 1000.0, 1)

            if elapsed >= after:
                values = apply_fault(values, fault)

            payload = make_payload(*values)
            send_payload(sock, payload)
            time.sleep(interval)

def parse_args():
    parser = argparse.ArgumentParser(
        description="UAV Mission Software Simulator Telemetry Generator"
    )

    parser.add_argument(
        "--scenario",
        choices=[
            "normal",
            "low_battery",
            "engine_overheat",
            "timeout",
            "altitude_deviation"
        ],
        default="low_battery",
        help="Telemetry scenario to run",
    )

    parser.add_argument(
        "--mode",
        choices=["scenario", "random", "fault"],
        default="scenario",
        help="Generator mode",
    )

    parser.add_argument(
        "--duration",
        type=float,
        default=10.0,
        help="Streaming duration in seconds",
    )

    parser.add_argument(
        "--interval",
        type=float,
        default=1.0,
        help="Telemetry send interval in seconds",
    )

    parser.add_argument(
        "--fault",
        choices=[
            "low_battery",
            "engine_overheat",
            "signal_weak",
            "altitude_deviation",
            "invalid_telemetry",
            "timeout",
        ],
        default="low_battery",
        help="Fault type for fault injection mode",
    )

    parser.add_argument(
        "--after",
        type=float,
        default=3.0,
        help="Seconds before injecting fault",
    )

    return parser.parse_args()

if __name__ == "__main__":
    args = parse_args()

    scenarios = {
        "normal": normal_mission,
        "low_battery": low_battery_return,
        "engine_overheat": engine_overheat_abort,
        "timeout": telemetry_timeout_abort,
        "altitude_deviation":altitude_deviation_warning,
    }

    if args.mode == "scenario":
        scenarios[args.scenario]()  
    elif args.mode == "random":
        random_stream(args.duration, args.interval)
    elif args.mode == "fault":
        fault_injection(args.fault, args.after, args.duration, args.interval)


