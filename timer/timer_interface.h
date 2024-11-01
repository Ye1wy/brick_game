#ifndef CPP_BRICKGAME_TIMER_TIMER_INTERFACE_H
#define CPP_BRICKGAME_TIMER_TIMER_INTERFACE_H

namespace s21 {
/// @brief An interface for all classes that provide a timer.
class ITimerProvider {
 public:
  virtual ~ITimerProvider() = default;
  virtual int GetTimeInMilliseconds() = 0;
};
}  // namespace s21

#endif