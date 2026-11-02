#pragma once
#include <chrono>

// Counts the work an algorithm does, so we can compare algorithms fairly.
struct Metrics {
    long long comparisons = 0;  // how many times two items were compared
    long long moves = 0;        // how many times an item was moved or swapped
    void reset() { comparisons = 0; moves = 0; }
};

// A stopwatch: create it, do some work, then ask how many milliseconds passed.
class Timer {
public:
    Timer() : start_(std::chrono::steady_clock::now()) {}
    double elapsedMs() const {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration<double, std::milli>(now - start_).count();
    }
private:
    std::chrono::steady_clock::time_point start_;
};
