#ifndef CPP_BRICKGAME_INCLUDE_VIEW_INTERFACE_H
#define CPP_BRICKGAME_INCLUDE_VIEW_INTERFACE_H

#include <vector>

#include "../../constants/constants.h"

namespace s21 {
/// @brief interface for the view classes that are needed for controllers
class ViewInterface {
 public:
  virtual Signal Notify() = 0;
  virtual void Update(
      const std::vector<std::pair<int, int>>& snake_body_coordinate,
      const std::pair<int, int>& food_coordinate, const int& snake_lenght,
      const int& score, const int& level, const int& high_score,
      GameStatus game_status) = 0;
  virtual void Update(const TetrisMatrix& tetris_data, const int& score,
                      const int& level, const int& high_score, bool start_param,
                      bool game_on, bool game_pause) = 0;
  virtual void Update(SelectedGame, bool) = 0;
};
}  // namespace s21

#endif
