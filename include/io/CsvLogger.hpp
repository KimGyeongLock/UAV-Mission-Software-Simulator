#pragma once

#include <fstream>
#include <string>

#include "core/Event.hpp"
#include "core/MissionState.hpp"
#include "core/Telemetry.hpp"

namespace uav::io {

class CsvLogger {
public:
    explicit CsvLogger(std::string log_path);

    bool open();

    void log(const core::MissionState state, const core::Event& event, const core::Telemetry& telemetry);

    void close();

private:
    std::string log_path_;
    std::ofstream file_; // 파일에 글을 쓰는 객체
};

}  // namespace uav::io
