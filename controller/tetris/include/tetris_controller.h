#ifndef CPP_BRICK_GAME_INCLUDE_CONTROLLER_H
#define CPP_BRICK_GAME_INCLUDE_CONTROLLER_H

#include "../../../brick_game/tetris/include/tetris_model.h"
#include "../../../constants/constants.h"
#include "../../../gui/include/view_interface.h"
#include "../../../timer/timer_interface.h"
#include "../../include/i_control_game.h"

namespace s21 {
/// @brief The controller class for the tetris, where keystrokes are
/// processed from the user, from whom the model will change
class TetrisController : public IControlGame {
 public:
  TetrisController(TetrisModel &tetris_model_object,
                   ViewInterface &viewer_object, ITimerProvider &timer);
  ~TetrisController() = default;

  bool HandleUserInput(Signal) noexcept override;
  void Update(Signal) noexcept override;

 private:
  TetrisModel &model_;
  ViewInterface &viewer_;
  ITimerProvider &timer_;

  int msecs_t_;
  Signal signal_;

  bool HandleMoveInput() noexcept;
  bool HandleGameControlInput() noexcept;
  void Refresh() noexcept;
  void Display() const noexcept;
};

}  // namespace s21

#endif
