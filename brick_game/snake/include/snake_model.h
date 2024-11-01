#ifndef CPP_BRICKGAME_SRC_SNAKEMODEL_H
#define CPP_BRICKGAME_SRC_SNAKEMODEL_H

#include <fstream>
#include <memory>
#include <random>
#include <utility>
#include <vector>

#include "../../../constants/constants.h"
#include "food.h"
#include "generator.h"
#include "state.h"

namespace s21 {

class SnakeStateMachine;
/// @brief The model class represents the handler of the entire snake, this is
/// the core of the snake
class SnakeModel {
  using reference = SnakeModel &;

 public:
  /// @brief Defualt contructor. All out of the box.
  SnakeModel();
  /// @brief the default constructor, where you can change the generator for the
  /// snake
  SnakeModel(std::unique_ptr<IGenerator> generator);
  ~SnakeModel();

  /// @brief a method for processing the current state of the game
  void Handle();

  const SnakeStateMachine *get_current_state() const noexcept;
  const std::vector<coordinate> &get_coordinate() const noexcept;
  const coordinate &get_food_coordinate() const noexcept;
  int get_lenght() const noexcept;
  GameStatus get_game_status() const noexcept;
  bool get_game_run() const noexcept;
  int get_score() const noexcept;
  int get_high_score() const noexcept;
  int get_level() const noexcept;
  bool IsGameOverLock() const noexcept;

  void set_game_status(GameStatus) noexcept;
  void set_new_state(SnakeStateMachine &) noexcept;
  void SwitchPauseStatus() noexcept;
  void SwitchingGameRun(bool) noexcept;
  void set_gameover_locker(bool) noexcept;

  bool TurnToRight() noexcept;
  bool TurnToLeft() noexcept;
  bool TurnToUp() noexcept;
  bool TurnToDown() noexcept;

  void Moving() noexcept;

  bool IsFoodEaten() const noexcept;
  void EatingFood();

  void InitGame() noexcept;
  void ResetGame() noexcept;
  void RefreshHighScore() noexcept;

  int Difficult(int, int) noexcept;

 private:
  coordinate head_coordinate_;
  std::vector<coordinate> body_coordinate_;
  int snake_lenght_;
  HeadDirection head_direction_;

  Food food_;

  GameStatus game_status_;
  bool is_game_on_;
  bool gameover_locker_;
  SnakeStateMachine *state_;

  int score_;
  int high_score_;
  int level_;

  std::fstream file_;

  std::string path_to_data_file_ = "./data/snake_data.txt";

  void RespawnFood();
  void MoveBody();
  void CheckCollision() noexcept;
  int get_x() const noexcept;
  int get_y() const noexcept;
  int get_food_x() const noexcept;
  int get_food_y() const noexcept;
  void ScoreHandle() noexcept;
  void FileRead() noexcept;
};

}  // namespace s21

#endif