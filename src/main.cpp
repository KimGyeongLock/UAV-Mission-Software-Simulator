#include "app/MissionApplication.hpp"

int main() {
    uav::app::MissionApplication app("config/mission_config.json");
    return app.run();
}