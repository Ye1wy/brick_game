#include "qtview.h"

s21::QView::QView() : snake_body_coordinate_() {
  tetris_data_ = {};

  signal_ = Signal::kNoSignal;
  game_type_ = SelectedGame::kNoGame;
  is_game_selected_ = false;

  game_status_ = GameStatus::kIdle;
  is_start_ = false;
  is_game_on_ = false;
  is_game_pause_ = false;

  snake_lenght_ = 0;
  score_ = 0;
  level_ = 0;
  high_score_ = 0;

  setFixedSize(kDotWidth * kFieldWidth, kDotHight * kFieldHight + kDotHight);
  setWindowTitle("BrickGame");
  setFocusPolicy(Qt::StrongFocus);
}

s21::Signal s21::QView::Notify() {
  Signal current_signal = signal_;
  signal_ = Signal::kNoSignal;
  return current_signal;
}

void s21::QView::Update(
    const std::vector<std::pair<int, int> > &snake_body_coordinate,
    const std::pair<int, int> &food_coordinate, const int &snake_lenght,
    const int &score, const int &level, const int &high_score,
    GameStatus game_status) {
  if ((int)snake_body_coordinate.size() < snake_lenght) {
    return;
  }

  snake_body_coordinate_.resize(snake_lenght);

  for (auto i = 0; i < snake_lenght; ++i) {
    snake_body_coordinate_[i].setX(snake_body_coordinate[i].first);
    snake_body_coordinate_[i].setY(snake_body_coordinate[i].second);
  }

  food_coordinate_.setX(food_coordinate.first);
  food_coordinate_.setY(food_coordinate.second);
  game_status_ = game_status;
  snake_lenght_ = snake_lenght;
  score_ = score;
  level_ = level;
  high_score_ = high_score;
}

void s21::QView::Update(const TetrisMatrix &tetris_data, const int &score,
                        const int &level, const int &high_score,
                        bool start_param, bool game_on, bool game_pause) {
  tetris_data_ = tetris_data;
  score_ = score;
  level_ = level;
  high_score_ = high_score;
  is_start_ = start_param;
  is_game_on_ = game_on;
  is_game_pause_ = game_pause;
}

void s21::QView::Update(SelectedGame game_type, bool game_selected) {
  game_type_ = game_type;
  is_game_selected_ = game_selected;
}

void s21::QView::RenderFieldSnake(QPainter &qp) {
  qp.setBrush(Qt::green);
  qp.drawEllipse(food_coordinate_.x() * kDotWidth,
                 food_coordinate_.y() * kDotWidth, kDotWidth, kDotHight);

  for (int i = 0; i < snake_lenght_; ++i) {
    if (i == 0) {
      qp.setBrush(Qt::red);
      qp.drawRect(snake_body_coordinate_[i].x() * kDotWidth,
                  snake_body_coordinate_[i].y() * kDotWidth, kDotWidth,
                  kDotHight);

    } else {
      qp.setBrush(Qt::gray);
      qp.drawRect(snake_body_coordinate_[i].x() * kDotWidth,
                  snake_body_coordinate_[i].y() * kDotWidth, kDotWidth,
                  kDotHight);
    }
  }
}

void s21::QView::RenderFieldTetris(QPainter &qp) {
  RenderTetrisField(qp);
  RenderTetrisTable(qp);
  RenderTetrisNextBlock(qp);
}

void s21::QView::RenderMenu(bool game_switch, QPainter &qp) {
  QString game_name = game_switch ? "SNAKE GAME" : "TETRIS GAME";

  int x = (width() / 5) - 20;
  int y = height() / 3;

  qp.setFont(QFont("Arial", 12));
  qp.setPen(Qt::white);
  qp.drawText(x, y, game_name);
  int x_offset = 15;
  int y_offset = 20;
  qp.drawText(x + x_offset, y + y_offset, "press start");
}

void s21::QView::RenderGameOver(QPainter &qp) {
  int x = (width() / 5) - 10;
  int y = height() / 3;

  qp.setFont(QFont("Arial", 12));
  qp.setPen(Qt::red);
  qp.drawText(x, y, "Game Over!");
  int x_offset = 5;
  int y_offset = 20;
  qp.drawText(x + x_offset, y + y_offset, "press start");
}

void s21::QView::RanderGameWin(QPainter &qp) {
  int x = width() / 5;
  int y = height() / 3;

  qp.setPen(Qt::yellow);
  qp.drawText(x, y, "Congratulations!");
  int x_offset = 5;
  int y_offset = 15;
  qp.drawText(x + x_offset, y + y_offset, "You Win!");
  qp.drawText(x + x_offset, y + y_offset + y_offset, "press start");
}

void s21::QView::RenderGameChoose(SelectedGame signal, QPainter &qp) {
  qp.setFont(QFont("Arial", 12));

  int x = width() / 5;
  int y = height() / 3;
  int y_offset = 30;

  switch (signal) {
    case SelectedGame::kSnake:
      qp.setPen(Qt::red);  // Змейка выделена зеленым
      qp.drawText(x, y, "SNAKE");
      qp.setPen(Qt::white);  // Тетрис черным
      qp.drawText(x, y + y_offset, "TETRIS");
      break;
    case SelectedGame::kTetris:
      qp.setPen(Qt::white);  // Змейка черным
      qp.drawText(x, y, "SNAKE");
      qp.setPen(Qt::red);  // Тетрис выделен зеленым
      qp.drawText(x, y + y_offset, "TETRIS");
      break;
    default:
      qp.setPen(Qt::red);  // Змейка выделена зеленым
      qp.drawText(x, y, "SNAKE");
      qp.setPen(Qt::white);  // Тетрис черным
      qp.drawText(x, y + y_offset, "TETRIS");
      break;
  }
}

void s21::QView::RenderTetrisField(QPainter &qp) {
  QPoint point;

  int x_offset = 10;
  int y_offset = 15;

  for (int i = 0; i < kHight; ++i) {
    for (int j = 0; j < kWidght; ++j) {
      if (tetris_data_.field[i][j]) {
        point = GetPoint(i, j);
        qp.setBrush(Qt::yellow);
        qp.drawRect(point.x() * kDotWidth + x_offset,
                    point.y() * kDotWidth + y_offset, kDotWidth, kDotHight);
      }
    }
  }
}

void s21::QView::RenderTetrisTable(QPainter &qp) {
  QPoint point;

  int x_offset = 10;
  int y_offset = 15;

  for (int i = 0; i < kHight; ++i) {
    for (int j = 0; j < kWidght; ++j) {
      if (tetris_data_.table[i][j]) {
        point = GetPoint(i, j);
        qp.setBrush(Qt::red);
        qp.drawRect(point.x() * kDotWidth + x_offset,
                    point.y() * kDotWidth + y_offset, kDotWidth, kDotHight);
      }
    }
  }
}

void s21::QView::RenderTetrisNextBlock(QPainter &qp) {
  QPoint point;

  int x_offset = width() / 2 + 70;
  int y_offset = 50;

  for (int i = 0; i < kBlockWidth; ++i) {
    for (int j = 0; j < kBlockWidth; ++j) {
      if (tetris_data_.next_block[i][j]) {
        point = GetPoint(i, j);
        qp.setBrush(Qt::blue);
        qp.drawRect(point.x() * kDotWidth + x_offset,
                    point.y() * kDotWidth + y_offset, kDotWidth, kDotHight);
      }
    }
  }
}

void s21::QView::RenderEdges(QPainter &qp) {
  QPen pen(Qt::white, 2, Qt::SolidLine);
  qp.setPen(pen);

  // Прямоугольник для игрового поля
  int game_field_x = 0;
  int game_field_y = 0;
  int game_field_width = 220;
  int game_field_height = this->height();
  qp.drawRect(game_field_x, game_field_y, game_field_width, game_field_height);

  // Отдельные прямоугольники для инфорамационной панели справа (Next, Score,
  // High score, Level)
  int right_panel_x = game_field_x + game_field_width;
  int right_panel_y = 0;
  int right_panel_width = 180;
  int section_height = 30;

  // "Next" поле
  qp.drawRect(right_panel_x, right_panel_y, right_panel_width, section_height);

  // "Score" поле

  int gap = 150;
  right_panel_y += gap;
  section_height = 35;
  qp.drawRect(right_panel_x, right_panel_y, right_panel_width, section_height);

  // "High score" поле
  right_panel_y += section_height;
  section_height = 40;
  qp.drawRect(right_panel_x, right_panel_y, right_panel_width, section_height);

  // // "Level" поле
  gap = 200;
  right_panel_y += gap;
  section_height = 35;
  qp.drawRect(right_panel_x, right_panel_y, right_panel_width, section_height);
}

void s21::QView::DisplayInfoData(QPainter &qp) {
  qp.setFont(QFont("Arial", 12));
  qp.setPen(Qt::white);
  int x_offset = 170;
  int y_offset = 0;
  int x = width() - x_offset;
  int y = 20 + y_offset;

  QString score = "Score: " + QString::number(score_);
  QString high_score = "High score: " + QString::number(high_score_);
  QString level = "Level: " + QString::number(level_);

  qp.drawText(x, y, "Next:");
  y_offset = 155;
  y += y_offset;
  qp.drawText(x, y, score);
  y_offset = 37;
  y += y_offset;
  qp.drawText(x, y, high_score);
  y_offset = 195;
  y += y_offset;
  qp.drawText(x, y, level);
}

QPoint s21::QView::GetPoint(int i, int j) { return QPoint(j, i); }

void s21::QView::keyPressEvent(QKeyEvent *e) {
  int key = e->key();

  switch (key) {
    case Qt::Key_Left:
      signal_ = Signal::kLeft;
      break;
    case Qt::Key_Right:
      signal_ = Signal::kRight;
      break;
    case Qt::Key_Up:
      signal_ = Signal::kUp;
      break;
    case Qt::Key_Down:
      signal_ = Signal::kDown;
      break;
    case Qt::Key_Q:
      signal_ = Signal::kExit;
      break;
    case Qt::Key_X:
      signal_ = Signal::kAction;
      break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
      signal_ = Signal::kStart;
      break;
    case Qt::Key_P:
      signal_ = Signal::kPause;
      break;
    default:
      signal_ = Signal::kNoSignal;
      break;
  }
}

void s21::QView::paintEvent(QPaintEvent *e) {
  Q_UNUSED(e);

  QPainter qp(this);

  RenderEdges(qp);
  DisplayInfoData(qp);

  if (!is_game_selected_) {
    RenderGameChoose(game_type_, qp);
    return;
  }

  switch (game_type_) {
    case SelectedGame::kSnake:
      switch (game_status_) {
        case GameStatus::kIdle:
          RenderMenu(true, qp);
          return;
        case GameStatus::kWin:
          RanderGameWin(qp);
          return;
        case GameStatus::kGameOver:
          RenderGameOver(qp);
          return;
        default:
          RenderFieldSnake(qp);
          return;
      }
    case SelectedGame::kTetris:
      if (!is_start_) {
        RenderMenu(false, qp);

      } else if (!is_game_on_) {
        RenderGameOver(qp);

      } else {
        RenderFieldTetris(qp);
      }
    default:
      break;
  }

  update();
}
