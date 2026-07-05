#pragma once

#include <string>

#include "core/Event.hpp"
#include "core/MissionState.hpp"
#include "core/Telemetry.hpp"

namespace uav::io {

class CsvLogger {
public:
    explicit CsvLogger(std::string log_path);

    // TODO: Open CSV file and write header.
    bool open();

    // TODO: Write mission state, event, and telemetry fields to CSV.
    void log(const core::MissionState state, const core::Event& event, const core::Telemetry& telemetry);

    // TODO: Flush and close CSV file.
    void close();

private:
    std::string log_path_;
};

}  // namespace uav::io
