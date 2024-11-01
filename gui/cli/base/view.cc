#include "../include/view.h"

#include <iostream>

s21::View::View()
    : snake_body_coordinate_(), food_coordinate_(), tetris_data_() {
  initscr();
  noecho();
  keypad(stdscr, true);
  start_color();
  use_default_colors();
  init_pair(1, COLOR_RED, -1);
  init_pair(2, COLOR_GREEN, -1);
  init_pair(3, COLOR_BLUE, -1);
  init_pair(4, COLOR_MAGENTA, -1);
  init_pair(5, COLOR_YELLOW, -1);
  init_pair(6, COLOR_RED, COLOR_RED);
  init_pair(7, COLOR_YELLOW, COLOR_YELLOW);
  curs_set(0);

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
}

s21::View::~View() { endwin(); }

void s21::View::UserInput() noexcept {
  nodelay(stdscr, true);
  int key = getch();

  switch (key) {
    case KEY_LEFT:
      signal_ = Signal::kLeft;
      return;
    case KEY_RIGHT:
      signal_ = Signal::kRight;
      return;
    case KEY_UP:
      signal_ = Signal::kUp;
      return;
    case KEY_DOWN:
      signal_ = Signal::kDown;
      return;
    case 'q':
      signal_ = Signal::kExit;
      return;
    case 'x':
      signal_ = Signal::kAction;
      return;
    case '\n':
      signal_ = Signal::kStart;
      return;
    case 'p':
      signal_ = Signal::kPause;
      return;
    default:
      signal_ = Signal::kNoSignal;
      return;
  }
}

s21::Signal s21::View::Notify() noexcept { return signal_; }

void s21::View::Update(
    const std::vector<std::pair<int, int>>& snake_body_coordinate,
    const std::pair<int, int>& food_coordinate, const int& snake_lenght,
    const int& score, const int& level, const int& high_score,
    GameStatus game_status) {
  snake_body_coordinate_ = snake_body_coordinate;
  food_coordinate_ = food_coordinate;
  game_status_ = game_status;
  snake_lenght_ = snake_lenght;
  score_ = score;
  level_ = level;
  high_score_ = high_score;
}

void s21::View::Update(const TetrisMatrix& tetris_data, const int& score,
                       const int& level, const int& high_score,
                       bool start_param, bool game_on, bool game_pause) {
  tetris_data_ = tetris_data;
  score_ = score;
  level_ = level;
  high_score_ = high_score;
  is_start_ = start_param;
  is_game_on_ = game_on;
  is_game_pause_ = game_pause;
}

void s21::View::Update(SelectedGame game_type, bool game_selected) {
  game_type_ = game_type;
  is_game_selected_ = game_selected;
}

void s21::View::Refresh() {
  if (!is_game_selected_) {
    RenderGameChoose(game_type_);
    return;
  }

  switch (game_type_) {
    case SelectedGame::kSnake:
      switch (game_status_) {
        case GameStatus::kIdle:
          RenderMenu(true);
          return;
        case GameStatus::kWin:
          RanderGameWin(high_score_, level_, high_score_);
          return;
        case GameStatus::kGameOver:
          RenderGameOver(score_, level_, high_score_);
          return;
        default:
          RenderField(snake_body_coordinate_, food_coordinate_, snake_lenght_,
                      score_, level_, high_score_);
          return;
      }
    case SelectedGame::kTetris:
      if (!is_start_) {
        RenderMenu(false);

      } else if (!is_game_on_) {
        RenderGameOver(score_, level_, high_score_);

      } else {
        RenderField(tetris_data_, score_, level_, high_score_);
      }

      break;
    default:
      break;
  }
}

/*********************************************************************************
 * @brief a method for randaring all date on game board such as: snake body
 * coordinate, food coordinate, edge of board
 *
 * @param snake_body_coordinate Representation of the coordinates of the snake's
 * body from the model
 * @param food_coordinate Representation of the coordinates of the food
 * from the model
 * @param snake_lenght Representation of the snake body lenght from the model
 **********************************************************************************/
void s21::View::RenderField(
    const std::vector<std::pair<int, int>>& snake_body_coordinate,
    const std::pair<int, int>& food_coordinate, const int& snake_lenght,
    const int& score, const int& level, const int& high_score) {
  erase();

  int food_coord_x = food_coordinate.first + 1;
  int food_coord_y = food_coordinate.second + 1;

  mvaddch(food_coord_y, food_coord_x * kCoordinateCorrection - 1, '$');

  for (int i = 0; i < snake_lenght; ++i) {
    int snake_coordinate_x = snake_body_coordinate[i].first + 1;
    int snake_coordinate_y = snake_body_coordinate[i].second + 1;

    chtype drawing_symbol = i == 0 ? 'O' : ACS_BLOCK;

    mvaddch(snake_coordinate_y, snake_coordinate_x * kCoordinateCorrection - 1,
            drawing_symbol);
  }

  RenderBox();
  DisplayInfo(score, level, high_score);
  refresh();
}

void s21::View::RenderField(const TetrisMatrix& tetris_data, const int& score,
                            const int& level, const int& high_score) {
  erase();

  RenderTetrisTable(tetris_data);
  RenderTetrisField(tetris_data);
  RenderTetrisNextBlock(tetris_data);

  RenderBox();
  DisplayInfo(score, level, high_score);
  refresh();
}

void s21::View::RenderMenu(bool game_switch) noexcept {
  erase();
  RenderBox();
  DisplayInfo(000, 000, 000);

  const char* game_name = game_switch ? "SNAKE GAME" : "TETRIS GAME";

  mvprintw(kCoordinateForString,
           kCoordinateForString - (kCoordinateCorrection + 1), game_name);
  mvprintw(kCoordinateForString + kCoordinateCorrection,
           kCoordinateForString - (kCoordinateCorrection + 1), "press start");

  refresh();
}

void s21::View::RenderGameOver(const int& score, const int& level,
                               const int& high_score) noexcept {
  erase();
  RenderBox();
  DisplayInfo((score), (level), (high_score));

  int x = kWidght / kCoordinateCorrection;
  int y = kCoordinateForString;

  mvprintw(y, x + kCoordinateCorrection, "Game Over");
  mvprintw(y + kCoordinateCorrection, x + 1, "press start");
  refresh();
}

void s21::View::RanderGameWin(const int& score, const int& level,
                              const int& high_score) noexcept {
  erase();
  RenderBox();
  DisplayInfo(score, level, high_score);

  int x = kWidght / kCoordinateCorrection;
  int y = kCoordinateForString;

  mvprintw(y, x - 1, "Congratulation!");
  mvprintw(y + kCoordinateCorrection, x + kCoordinateCorrection + 1,
           "YOU WIN!");
  mvprintw(y + kCoordinateCorrection * kCoordinateCorrection, x + 1,
           "press start");
  // stars
  mvaddch(y - kCoordinateCorrection, x, ACS_PLUS);
  mvaddch(y + kCoordinateCorrection + 1, x - 1, ACS_PLUS);
  mvaddch(y * kCoordinateCorrection - kCoordinateCorrection,
          x * x - x * kCoordinateCorrection, ACS_PLUS);
  mvaddch(y - kCoordinateCorrection - 1,
          x * kCoordinateCorrection * kCoordinateCorrection, ACS_PLUS);
  mvaddch(y - y + kCoordinateCorrection, x * kCoordinateCorrection, ACS_PLUS);
  mvaddch(y * kCoordinateCorrection, x * x, ACS_PLUS);

  mvprintw(y + kCoordinateCorrection * kCoordinateCorrection + 1,
           x * x + kCoordinateCorrection + x, "~(-V-~)");
  refresh();
}

void s21::View::RenderGameChoose(SelectedGame signal) noexcept {
  erase();
  RenderBox();
  DisplayInfo(0, 0, 0);

  int x = kWidght / kCoordinateCorrection;
  int y = (kHight / kCoordinateCorrection) - (kCoordinateCorrection + 1);

  const char* snake = "SNAKE";
  const char* tetris = "TETRIS";

  switch (signal) {
    case SelectedGame::kSnake:
      attron(COLOR_PAIR(1));
      mvprintw(y + kCoordinateCorrection, x + 1, snake);
      attroff(COLOR_PAIR(1));

      mvprintw(y + kCoordinateCorrection + kCoordinateCorrection, x + 1,
               tetris);
      break;
    case SelectedGame::kTetris:
      mvprintw(y + kCoordinateCorrection, x + 1, snake);

      attron(COLOR_PAIR(1));
      mvprintw(y + kCoordinateCorrection + kCoordinateCorrection, x + 1,
               tetris);
      attroff(COLOR_PAIR(1));
      break;
    default:
      attron(COLOR_PAIR(1));
      mvprintw(y + kCoordinateCorrection, x + 1, snake);
      attroff(COLOR_PAIR(1));

      mvprintw(y + kCoordinateCorrection + kCoordinateCorrection, x + 1,
               tetris);
      break;
  }

  refresh();
}

/**
 * @brief A methods for drawing the boundaries of a game board
 *
 */
void s21::View::RenderBox() noexcept {
  int bottom_edge = kHight + kCoordinateCorrection;
  int box_edge = kWidght * kCoordinateCorrection;

  mvaddch(0, 0, ACS_ULCORNER);
  mvaddch(bottom_edge, 0, ACS_LLCORNER);

  for (int i = 0; i <= box_edge; ++i) {
    mvaddch(0, i + kCoordinateCorrection - 1, ' ' | A_REVERSE);
    mvaddch(bottom_edge, i + kCoordinateCorrection - 1, ' ' | A_REVERSE);
  }

  mvaddch(0, box_edge + kCoordinateCorrection, ACS_TTEE);
  mvaddch(bottom_edge, box_edge + kCoordinateCorrection, ACS_BTEE);

  for (int i = 1; i <= kHight + 1; ++i) {
    mvaddch(i, 0, ACS_VLINE);
    mvaddch(i, box_edge + kCoordinateCorrection, ACS_VLINE);
  }

  RenderAdditionalBox();
}

void s21::View::RenderAdditionalBox() noexcept {
  int bottom_edge = kHight + kCoordinateCorrection;
  int init_additiolal_block_x_coordinate = kWidght * kCoordinateCorrection + 1;
  int right_edge =
      (kWidght * kCoordinateCorrection + 1) * kCoordinateCorrection;
  int init_coodrinate_x =
      init_additiolal_block_x_coordinate + kCoordinateCorrection;

  for (int i = init_coodrinate_x; i <= right_edge; ++i) {
    mvaddch(0, i, ACS_HLINE);
    mvaddch(bottom_edge, i, ACS_HLINE);
  }

  for (int i = 0; i <= kHight + 1; ++i) {
    mvaddch(i, right_edge, ACS_VLINE);
  }

  mvaddch(0, right_edge, ACS_URCORNER);
  mvaddch(bottom_edge, right_edge, ACS_LRCORNER);

  int separator_x = kHight + kCoordinateCorrection + 1;
  int separator_y = kWidght / 4;

  RenderSeparator(separator_x, separator_y);

  separator_y = (kWidght / 4) + 6;

  RenderSeparator(separator_x, separator_y);

  separator_y = (kWidght / 4) + 8;

  RenderSeparator(separator_x, separator_y);

  separator_y = (kWidght / 4) + 11;

  RenderSeparator(separator_x, separator_y);

  separator_y = (kWidght / 4) + 18;

  RenderSeparator(separator_x, separator_y);
}

void s21::View::DisplayInfo(const int& score, const int& level,
                            const int& high_score) noexcept {
  int x_coordinate = kWidght + (kCoordinateCorrection * kCoordinateCorrection *
                                kCoordinateCorrection * kCoordinateCorrection);
  int y_coordinate = 1;

  mvprintw(y_coordinate, x_coordinate, "Next:");

  y_coordinate = kHight - 11;

  mvprintw(y_coordinate, x_coordinate, "Score: %d", score);

  y_coordinate = kHight - 9;

  mvprintw(y_coordinate, x_coordinate, "High score:");

  y_coordinate = kHight - 8;

  mvprintw(y_coordinate, x_coordinate, "\t      %d", high_score);

  y_coordinate = kHight + 1;
  mvprintw(y_coordinate, x_coordinate, "Level: %d", level);
}

void s21::View::RenderSeparator(const int x, const int y) noexcept {
  int right_edge =
      (kWidght * kCoordinateCorrection + 1) * kCoordinateCorrection;

  for (int i = x; i < right_edge; ++i) {
    mvaddch(y, i, ACS_HLINE);
  }
}

void s21::View::RenderTetrisField(const TetrisMatrix& tetris_data) {
  for (int i = 0; i < kHight; ++i) {
    for (int j = 0; j < kWidght; ++j) {
      if (tetris_data.field[i][j]) {
        mvaddch(i + 2, (j * kCoordinateCorrection) + kCoordinateCorrection,
                ' ' | A_REVERSE);
        mvaddch(i + 2, (j * kCoordinateCorrection) + kCoordinateCorrection + 1,
                ' ' | A_REVERSE);
      }
    }
  }
}

void s21::View::RenderTetrisTable(const TetrisMatrix& tetris_data) {
  for (int i = 0; i < kHight; ++i) {
    for (int j = 0; j < kWidght; ++j) {
      if (tetris_data.table[i][j]) {
        mvaddch(i + 2, (j * kCoordinateCorrection) + kCoordinateCorrection,
                ' ' | A_REVERSE);
        mvaddch(i + 2, (j * kCoordinateCorrection) + kCoordinateCorrection + 1,
                ' ' | A_REVERSE);
      }
    }
  }
}

void s21::View::RenderTetrisNextBlock(const TetrisMatrix& tetris_data) {
  int add = (kWidght * kCoordinateCorrection + kCoordinateCorrection * 4);

  for (int i = 0; i < kBlockWidth; ++i) {
    for (int j = 0; j < kBlockWidth; ++j) {
      if (tetris_data.next_block[i][j]) {
        int x = j * kCoordinateCorrection + add;
        int y = i + kCoordinateCorrection;

        mvaddch(y, x, ' ' | A_REVERSE);
        mvaddch(y, x + 1, ' ' | A_REVERSE);
      }
    }
  }
}
