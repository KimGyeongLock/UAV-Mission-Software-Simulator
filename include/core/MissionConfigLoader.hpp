#pragma once

#include <string>

#include "core/MissionConfig.hpp"

namespace uav::core {

MissionConfig loadMissionConfigOrDefault(const std::string& path);

}   // namespace uav::core