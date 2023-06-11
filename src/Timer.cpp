#include "Timer.h"
#include <bits/chrono.h>
#include <iostream>
#include <thread>

Timer::Timer() {
    Start();
}

void Timer::WaitUntilPassed(unsigned long long microseconds) {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start).count();
    auto remaining = microseconds - elapsed;
    if (remaining > 0) {
        std::this_thread::sleep_for(std::chrono::microseconds(remaining));
    }
}

void Timer::Start() {
    start = std::chrono::steady_clock::now();
}

std::chrono::microseconds Timer::PassedTime() {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(now - start);
}

