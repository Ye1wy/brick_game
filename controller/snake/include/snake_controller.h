#ifndef CPP_BRICKGAME_SRC_SNAKE_CONTROLLER_H
#define CPP_BRICKGAME_SRC_SNAKE_CONTROLLER_H

#include "../../../brick_game/snake/include/snake_model.h"
#include "../../../constants/constants.h"
#include "../../../gui/include/view_interface.h"
#include "../../../timer/timer_interface.h"
#include "../../include/i_control_game.h"

namespace s21 {
/// @brief The controller class for the snake, where keystrokes are processed
/// from the user, from whom the model will change
class SnakeController : public IControlGame {
 public:
  SnakeController(SnakeModel &model, ViewInterface &viewer,
                  ITimerProvider &timer) noexcept;
  ~SnakeController() = default;

  void Update(Signal signal) noexcept override;
  bool HandleUserInput(Signal) noexcept override;

 private:
  SnakeModel &model_;
  ViewInterface &viewer_;
  ITimerProvider &timer_;
  int msecs_t_;
  Signal signal_;

  bool HandlerMovementInput() noexcept;
  bool HandlerGameControlInput() noexcept;
  void Refresh() noexcept;
  void HandleGameStatus() const noexcept;
};

}  // namespace s21

#endif
