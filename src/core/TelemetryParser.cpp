#include "core/TelemetryParser.hpp"

#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace uav::core {

optional<Telemetry> parseTelemetryLine(const std::string& line) {
    stringstream ss(line);
    string token;
    vector<string> tokens;

    while (getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    if (tokens.size() != 6) {
        return nullopt;
    }

    try {
        Telemetry telemetry;
        telemetry.battery_percent = stod(tokens[0]);
        telemetry.engine_temperature_c = stod(tokens[1]);
        telemetry.signal_strength_percent = stod(tokens[2]);
        telemetry.altitude_m = stod(tokens[3]);
        telemetry.target_altitude_m = stod(tokens[4]);
        telemetry.valid = (stoi(tokens[5]) != 0);

        return telemetry;
    } catch (...) {
        return nullopt;
    }
}

}  // namespace uav::core