#pragma once

#include <string>

namespace uav::app {

class MissionApplication {
public:
    explicit MissionApplication(std::string config_path);

    int run();

private:
    std::string config_path_;
};

}   // namespace uav::app