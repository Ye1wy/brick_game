#ifndef CPP_BRICK_GAME_DESKTOP_QTVIEW_H
#define CPP_BRICK_GAME_DESKTOP_QTVIEW_H

#include <qt/QtCore/qobjectdefs.h>

#include <QKeyEvent>
#include <QPainter>
#include <QString>
#include <QWidget>

#include "../../../constants/constants.h"
#include "../../include/view_interface.h"

namespace s21 {

constexpr int kFieldWidth = 20;
constexpr int kFieldHight = 20;
constexpr int kDotWidth = 20;
constexpr int kDotHight = 20;

/// @brief A view class that displays data received from the snake and tetris
/// controller in desktop
class QView : public QWidget, public ViewInterface {
  Q_OBJECT

 public:
  QView();
  /// @brief A method for sending a signal from the keyboard to the controller
  /// @return The signal of the pressed button
  Signal Notify() override;
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

 protected:
  void keyPressEvent(QKeyEvent*) override;
  void paintEvent(QPaintEvent*) override;

 private:
  void RenderFieldSnake(QPainter&);

  void RenderFieldTetris(QPainter&);

  void RenderMenu(bool, QPainter&);
  void RenderGameOver(QPainter&);
  void RanderGameWin(QPainter&);
  void RenderGameChoose(SelectedGame, QPainter&);

  void RenderTetrisField(QPainter&);
  void RenderTetrisTable(QPainter&);
  void RenderTetrisNextBlock(QPainter&);
  void RenderEdges(QPainter&);
  void DisplayInfoData(QPainter&);

  Signal signal_;
  SelectedGame game_type_;
  bool is_game_selected_;

  std::vector<QPoint> snake_body_coordinate_;
  QPoint food_coordinate_;
  GameStatus game_status_;
  QPoint GetPoint(int i, int);

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

#endif  // QTVIEW_H
