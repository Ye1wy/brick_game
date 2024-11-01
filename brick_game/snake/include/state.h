#ifndef CPP_BRICKGAME_INCLUDE_STATE_H
#define CPP_BRICKGAME_INCLUDE_STATE_H

#include "snake_model.h"

namespace s21 {
class SnakeModel;
/// @brief interface for all snake states
class SnakeStateMachine {
 public:
  virtual void Handle(SnakeModel *model) = 0;
};
/// @brief A class that handles the start state
class StartState : public SnakeStateMachine {
 public:
  void Handle(SnakeModel *model) override;
  static StartState &GetInstance();
};

/// @brief A class that handles the spawn state
class SpawnState : public SnakeStateMachine {
 public:
  void Handle(SnakeModel *model) override;
  static SpawnState &GetInstance();
};

/// @brief A class that handles the move state
class MoveState : public SnakeStateMachine {
 public:
  void Handle(SnakeModel *model) override;
  static MoveState &GetInstance();
};

/// @brief A class that handles the eat food state
class EatFoodState : public SnakeStateMachine {
 public:
  void Handle(SnakeModel *model) override;
  static EatFoodState &GetInstance();
};

/// @brief A class that handles the gameover state
class GameOverState : public SnakeStateMachine {
 public:
  void Handle(SnakeModel *model) override;
  static GameOverState &GetInstance();
};

/// @brief A class that handles the pause state
class PauseState : public SnakeStateMachine {
 public:
  void Handle(SnakeModel *model) override;
  static PauseState &GetInstance();
};

}  // namespace s21

#endif