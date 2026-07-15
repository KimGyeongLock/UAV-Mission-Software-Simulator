#pragma once

#include <optional>
#include <string>

#include "core/Telemetry.hpp"

namespace uav::core {

std::optional<Telemetry> parseTelemetryLine(const std::string& line);

} // namespace uav::core