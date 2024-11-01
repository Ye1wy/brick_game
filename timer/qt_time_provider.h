#ifndef CPP_BRICKGAME_TIMER_QT_TIME_PROVIDER_H
#define CPP_BRICKGAME_TIMER_QT_TIME_PROVIDER_H

#include <QElapsedTimer>

#include "timer_interface.h"

namespace s21 {
/// @brief a class that implements the interface and is responsible for the
/// timer for the desktop version of games
class QtTimeProvider : public ITimerProvider {
 public:
  QtTimeProvider();

  int GetTimeInMilliseconds() override;

 private:
  QElapsedTimer timer_;
};
}  // namespace s21

#endif
