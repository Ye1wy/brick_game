#ifndef CPP_BRICKGAME_INCLUDE_VIEW_H
#define CPP_BRICKGAME_INCLUDE_VIEW_H

#include <ncurses.h>

#include <list>
#include <vector>

#include "../../../constants/constants.h"
#include "../../include/view_interface.h"

namespace s21 {

struct TetrisMatrix;
/// @brief A view class that displays data received from the snake and tetris
/// controller in the console
class View : public ViewInterface {
 public:
  View();
  ~View();

  void UserInput() noexcept;
  /// @brief A method for sending a signal from the keyboard to the controller
  /// @return The signal of the pressed button
  Signal Notify() noexcept override;
  /// @brief Method for updating snake rendering data
  /// @param snake_body_coordinate
  /// @param food_coordinate
  /// @param snake_lenght
  /// @param score
  /// @param level
  /// @param high_score
  /// @param game_status
  void Update(const std::vector<std::pair<int, int>>& snake_body_coordinate,
              const std::pair<int, int>& food_coordinate,
              const int& snake_lenght, const int& score, const int& level,
              const int& high_score, GameStatus game_status) override;
  /// @brief Method for updating tetris rendering data
  /// @param tetris_data
  /// @param score
  /// @param level
  /// @param high_score
  /// @param start_param
  /// @param game_on
  /// @param game_pause
  void Update(const TetrisMatrix& tetris_data, const int& score,
              const int& level, const int& high_score, bool start_param,
              bool game_on, bool game_pause) override;
  /// @brief Method for updating game selection data
  /// @param game_type
  /// @param game_selected
  void Update(SelectedGame game_type, bool game_selected) override;
  /// @brief Method for updating the rendering of frames
  void Refresh();

 private:
  void RenderField(
      const std::vector<std::pair<int, int>>& snake_body_coordinate,
      const std::pair<int, int>& food_coordinate, const int& snake_lenght,
      const int& score, const int& level, const int& high_score);

  void RenderField(const TetrisMatrix& tetris_data, const int& score,
                   const int& level, const int& high_score);

  void RenderMenu(bool) noexcept;
  void RenderGameOver(const int& score, const int& level,
                      const int& high_score) noexcept;
  void RanderGameWin(const int& score, const int& level,
                     const int& high_score) noexcept;
  void RenderGameChoose(SelectedGame) noexcept;
  void RenderBox() noexcept;
  void RenderAdditionalBox() noexcept;
  void DisplayInfo(const int& score, const int& level,
                   const int& high_score) noexcept;
  void RenderSeparator(const int x, const int y) noexcept;

  void RenderTetrisField(const TetrisMatrix& tetris_data);
  void RenderTetrisTable(const TetrisMatrix& tetris_data);
  void RenderTetrisNextBlock(const TetrisMatrix& tetris_data);

  Signal signal_;
  SelectedGame game_type_;
  bool is_game_selected_;

  std::vector<std::pair<int, int>> snake_body_coordinate_;
  std::pair<int, int> food_coordinate_;
  GameStatus game_status_;

  TetrisMatrix tetris_data_;
  bool is_start_;
  bool is_game_on_;
  bool is_game_pause_;

  int snake_lenght_;
  int score_;
  int level_;
  int high_score_;
};
}  // namespace s21

#endif