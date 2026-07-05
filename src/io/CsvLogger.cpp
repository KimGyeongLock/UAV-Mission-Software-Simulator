#include "io/CsvLogger.hpp"
#include "core/StringUtils.hpp"

#include <utility>

namespace uav::io {

CsvLogger::CsvLogger(std::string log_path) : log_path_(std::move(log_path)) {}

bool CsvLogger::open() {
    file_.open(log_path_);

    if (!file_.is_open()) {
        return false;
    }

    file_ << "state,event,battery_percent,engine_temperature_c,altitude_m,"
          << "target_altitude_m,signal_strength_percent,valid\n";
    
    return true;
}

void CsvLogger::log(const core::MissionState state, const core::Event& event, const core::Telemetry& telemetry) {
    if (!file_.is_open()) {
        return;
    }

    file_ << uav::core::toString(state) << ','
          << uav::core::toString(event.type) << ','
          << telemetry.battery_percent << ','
          << telemetry.engine_temperature_c << ','
          << telemetry.altitude_m << ','
          << telemetry.target_altitude_m << ','
          << telemetry.signal_strength_percent << ','
          << telemetry.valid << '\n';
}

void CsvLogger::close() {
    if (file_.is_open()) {
        file_.flush();
        file_.close();
    }
}

}  // namespace uav::io
