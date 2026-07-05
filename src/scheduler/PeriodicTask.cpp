#include "scheduler/PeriodicTask.hpp"

#include <utility>

namespace uav::scheduler {

PeriodicTask::PeriodicTask(Task task, std::chrono::milliseconds period)
    : task_(std::move(task)), period_(period) {}

PeriodicTask::~PeriodicTask() {
    stop();
}

void PeriodicTask::start() {
    if (running_) {
        return;
    }

    running_ = true;

    worker_ = std::thread([this]() {
        while (running_) {
            task_();
            std::this_thread::sleep_for(period_);
        }
    });
}

void PeriodicTask::stop() {
    running_ = false;
    if (worker_.joinable()) {
        worker_.join();
    }
}

}  // namespace uav::scheduler
