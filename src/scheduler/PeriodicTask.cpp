#include "scheduler/PeriodicTask.hpp"

#include <utility>

namespace uav::scheduler {

PeriodicTask::PeriodicTask(Task task, std::chrono::milliseconds period)
    : task_(std::move(task)), period_(period) {}

PeriodicTask::~PeriodicTask() {
    stop();
}

void PeriodicTask::start() {
    // TODO: Start periodic worker thread.
}

void PeriodicTask::stop() {
    // TODO: Signal worker thread to stop and join it.
    running_ = false;
    if (worker_.joinable()) {
        worker_.join();
    }
}

}  // namespace uav::scheduler
