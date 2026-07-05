#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <thread>

namespace uav::scheduler {

class PeriodicTask {
public:
    using Task = std::function<void()>;

    PeriodicTask(Task task, std::chrono::milliseconds period);
    ~PeriodicTask();

    void start();

    void stop();

private:
    Task task_;
    std::chrono::milliseconds period_;
    std::atomic<bool> running_{false};
    std::thread worker_;
};

}  // namespace uav::scheduler
