#include <thread>

#include "brick_game/snake/include/snake_model.h"
#include "brick_game/tetris/include/tetris_model.h"
#include "controller/include/controller.h"
#include "gui/cli/include/view.h"
#include "timer/console_time_provider.h"

int main() {
  s21::View viewer;
  s21::SnakeModel snake_game_model;
  s21::TetrisModel tetris_game_model;
  s21::ConsoleTimeProvider timer;

  s21::SnakeController snake_controller(snake_game_model, viewer, timer);
  s21::TetrisController tetris_controller(tetris_game_model, viewer, timer);

  s21::Controller central(snake_controller, tetris_controller, viewer);

  while (!central.IsGameEnded()) {
    viewer.UserInput();
    central.Update();
    viewer.Refresh();
  }
}
