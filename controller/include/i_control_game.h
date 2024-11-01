#ifndef CPP_BRICKGAME_INCLUDE_I_CONTROL_GAME_H
#define CPP_BRICKGAME_INCLUDE_I_CONTROL_GAME_H

namespace s21 {
/// @brief interface for controllers that are currently controlling the game in
/// a shared controller
class IControlGame {
 public:
  virtual bool HandleUserInput(Signal) = 0;
  virtual void Update(Signal signal) = 0;
};

}  // namespace s21

#endif