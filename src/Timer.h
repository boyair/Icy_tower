#pragma once

#include <chrono>
#include <cstdint>
#include <iostream>

class Timer {
public:
  Timer();
  // waits unfil specified time passed since creation / last start call
  void WaitUntilPassed(uint32_t microseconds);
  void Start();
  std::chrono::microseconds PassedTime() const;

  static void Sleep(uint32_t microseconds);

private:
  std::chrono::steady_clock::time_point
      start; // the time point of creation or last Start() call.
};
