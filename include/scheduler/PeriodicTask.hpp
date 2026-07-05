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

    // TODO: Start a worker thread that runs task periodically.
    void start();

    // TODO: Stop the worker thread safely.
    void stop();

private:
    Task task_;
    std::chrono::milliseconds period_;
    std::atomic<bool> running_{false};
    std::thread worker_;
};

}  // namespace uav::scheduler
