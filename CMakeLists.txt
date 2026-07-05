cmake_minimum_required(VERSION 3.16)

project(uav_mission_sw_simulator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(uav_mission_sw_simulator
    src/main.cpp
    src/core/MissionController.cpp
    src/core/HealthMonitor.cpp
    src/core/StringUtils.cpp
    src/io/UdpReceiver.cpp
    src/io/CsvLogger.cpp
    src/scheduler/PeriodicTask.cpp
)

target_include_directories(uav_mission_sw_simulator
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# TODO: Enable GoogleTest after core behavior is implemented.
# Example future structure:
# enable_testing()
# add_subdirectory(tests)
# find_package(GTest REQUIRED)
# add_executable(uav_mission_sw_tests
#     tests/MissionControllerTest.cpp
#     tests/HealthMonitorTest.cpp
#     tests/TelemetryParsingTest.cpp
# )
# target_link_libraries(uav_mission_sw_tests PRIVATE GTest::gtest_main)
# include(GoogleTest)
# gtest_discover_tests(uav_mission_sw_tests)
