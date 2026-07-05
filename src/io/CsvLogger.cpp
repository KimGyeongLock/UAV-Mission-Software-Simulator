#include "io/CsvLogger.hpp"

#include <utility>

namespace uav::io {

CsvLogger::CsvLogger(std::string log_path) : log_path_(std::move(log_path)) {}

bool CsvLogger::open() {
    // TODO: Open CSV file and write header row.
    return true;
}

void CsvLogger::log(const core::MissionState state, const core::Event& event, const core::Telemetry& telemetry) {
    (void)state;
    (void)event;
    (void)telemetry;
    // TODO: Write one CSV row.
}

void CsvLogger::close() {
    // TODO: Flush and close CSV output stream.
}

}  // namespace uav::io
