#include "console_time_provider.h"

s21::ConsoleTimeProvider::ConsoleTimeProvider()
    : start_time_(std::chrono::steady_clock::now()) {}

int s21::ConsoleTimeProvider::GetTimeInMilliseconds() {
  auto now = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(now -
                                                               start_time_)
      .count();
}