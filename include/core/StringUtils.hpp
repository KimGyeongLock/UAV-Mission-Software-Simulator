#pragma once

#include "core/Event.hpp"
#include "core/MissionState.hpp"

namespace uav::core {

const char* toString(MissionState state);
const char* toString(EventType event_type);

} // namespace uav::core