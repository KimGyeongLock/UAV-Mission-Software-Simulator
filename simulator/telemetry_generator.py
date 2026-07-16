import socket
import time
import argparse

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

    scenarios[args.scenario]()
