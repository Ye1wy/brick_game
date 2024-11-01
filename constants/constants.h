#ifndef CPP_BRICKGAME_INCLUDE_DEPENDENCIES_H
#define CPP_BRICKGAME_INCLUDE_DEPENDENCIES_H

#include <utility>

namespace s21 {

constexpr int kBlockWidth = 4;
constexpr int kWidght = 10;
constexpr int kHight = 20;
constexpr int kInitLenghtSnake = 4;
constexpr int kMaxScore = 200;
constexpr int kToNextLevel = 5;
constexpr int kFps = 500;
constexpr int kGameDifficultModifier = 20;
constexpr int kCoordinateCorrection = 2;
constexpr int kCoordinateForString = 9;

// first - x second - y
using coordinate = std::pair<int, int>;

/// @brief A class needed to handle keystrokes in order to abstract from user
/// input methods on different implementations
enum class Signal {
  kNoSignal = 0,
  kLeft,
  kRight,
  kUp,
  kDown,
  kExit,
  kAction,
  kStart,
  kPause
};

/// @brief The class required to process the selected game
enum class SelectedGame { kNoGame = 0, kSnake, kTetris };

/// @brief The class needed to determine the current status of the snake game
enum class GameStatus { kIdle = 0, kRun, kPause, kGameOver, kWin };

/// @brief The class needed to determine the current direction of the snake's
/// head
enum class HeadDirection { kUp, kDown, kLeft, kRight };

/// @brief A structure for transferring data from the tetris back to the view
struct TetrisMatrix {
  int field[kHight][kWidght];
  int table[kHight][kWidght];
  int next_block[kBlockWidth][kBlockWidth];
};

}  // namespace s21

#endif