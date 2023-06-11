#pragma once

#include <chrono>
#include <iostream>

class Timer {
private:
    std::chrono::steady_clock::time_point start;

public:
    Timer();
    void WaitUntilPassed(unsigned long long microseconds);
    void Start();
    std::chrono::microseconds PassedTime();
};

