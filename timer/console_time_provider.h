#ifndef CPP_BRICKGAME_TIMER_CONSOLE_TIME_PROVIDER_H
#define CPP_BRICKGAME_TIMER_CONSOLE_TIME_PROVIDER_H

#include <chrono>

#include "timer_interface.h"

namespace s21 {
/// @brief a class that implements the interface and is responsible for the
/// timer for the console version of games
class ConsoleTimeProvider : public ITimerProvider {
 public:
  ConsoleTimeProvider();
  int GetTimeInMilliseconds() override;

 private:
  std::chrono::steady_clock::time_point start_time_;
};
}  // namespace s21

#endif