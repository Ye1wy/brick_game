#ifndef CPP_BRICKGAME_CONTROLLER_CONTROLLER_H
#define CPP_BRICKGAME_CONTROLLER_CONTROLLER_H

#include "../../gui/include/view_interface.h"
#include "../snake/include/snake_controller.h"
#include "../tetris/include/tetris_controller.h"

namespace s21 {

class TetrisController;
/// @brief A common controller that is responsible for game selection
class Controller {
 public:
  Controller(SnakeController &snake_controller_object,
             TetrisController &tetris_controller_object,
             ViewInterface &view_object);
  ~Controller() = default;

  int IsGameChoosed() const noexcept;
  bool IsGameEnded() const noexcept;
  void set_signal(const Signal) noexcept;

  void HandleSelectMenu() noexcept;
  void Update() noexcept;

 private:
  IControlGame *current_game;
  ViewInterface &viewer_;
  SnakeController &snake_controller_;
  TetrisController &tetris_controller_;
  SelectedGame selected_game_;
  bool is_game_selected_;
  bool is_game_ended_;
  Signal signal_;

  void SwitchGame(IControlGame *) noexcept;
  void HandleSelectedGame();
};

}  // namespace s21

#endif
