#pragma once

#include <chrono>
#include <cstdint>
#include <iostream>

class Timer {
private:
    std::chrono::steady_clock::time_point start;

public:
    Timer();
    void WaitUntilPassed(uint32_t microseconds);
    void Start();
    std::chrono::microseconds PassedTime() const;

    static void Sleep(uint32_t microseconds);
};

