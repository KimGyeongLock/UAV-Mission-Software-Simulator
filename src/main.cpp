#include <iostream>

#include "core/MissionController.hpp"

int main() {
    uav::core::MissionController controller;

    // TODO: Wire UdpReceiver, HealthMonitor, MissionController, PeriodicTask, and CsvLogger.
    std::cout << "UAV Mission Software Simulator scaffold\n";

    controller.startMission();
    controller.update();
    controller.update();

    return 0;
}
