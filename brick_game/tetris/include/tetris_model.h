#ifndef CPP_BRICKGAME_TETRIS_MODEL_H
#define CPP_BRICKGAME_TETRIS_MODEL_H

#include "../../../constants/constants.h"
#ifndef __cli
extern "C" {
#endif
// Объявления ваших функций
#include "legacy/backend.h"

#ifndef __cli
}
#endif

namespace s21 {
/// @brief The model class represents the handler of the entire tetris, this is
/// the core of the tetris
class TetrisModel {
 public:
  TetrisModel();
  ~TetrisModel() = default;

  bool get_start_parametr() const noexcept;
  bool get_pause_parametr() const noexcept;
  bool get_game_on_flag() const noexcept;
  int get_score() const noexcept;
  int get_level() const noexcept;
  int get_high_score() const noexcept;
  int get_speed() const noexcept;
  const TetrisMatrix &get_matrix() const noexcept;

  void set_start_state(bool) noexcept;
  void set_pause_state(bool) noexcept;
  void set_game_on_state(bool) noexcept;

  void InitGame() noexcept;
  void Spawn() noexcept;
  void UpdateGameStat() noexcept;

  bool TurnLeft() noexcept;
  bool TurnRight() noexcept;
  bool Rotation() noexcept;
  bool BlockDown() noexcept;
  void ActionDown() noexcept;

  void ClearField() noexcept;
  void FillTransmitionData() noexcept;

 private:
  GameInfo_t model_;

  TetrisMatrix data_for_view_;
};

}  // namespace s21

#endif
