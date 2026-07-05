"""Telemetry generator scaffold for UAV Mission Software Simulator.

This file intentionally contains scenario stubs only.
Implement UDP transmission and scenario data generation step by step.
"""


def normal_mission():
    """TODO: Send nominal telemetry sequence for full mission completion."""
    pass


def low_battery_return():
    """TODO: Send telemetry that drops battery below the return-home threshold."""
    pass


def engine_overheat_abort():
    """TODO: Send telemetry that exceeds the engine overheat threshold."""
    pass


def telemetry_timeout_abort():
    """TODO: Stop sending telemetry long enough to trigger timeout handling."""
    pass


def altitude_deviation_warning():
    """TODO: Send telemetry with altitude outside the allowed tolerance."""
    pass


if __name__ == "__main__":
    # TODO: Add argparse scenario selection after C++ receiver behavior is implemented.
    normal_mission()
