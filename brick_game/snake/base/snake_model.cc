#include "../include/snake_model.h"

s21::SnakeModel::SnakeModel()
    : head_coordinate_(),
      body_coordinate_(),
      food_(kWidght, kHight),
      state_(&StartState::GetInstance()),
      file_() {
  snake_lenght_ = 4;
  head_direction_ = s21::HeadDirection::kRight;
  game_status_ = s21::GameStatus::kIdle;
  is_game_on_ = false;
  gameover_locker_ = false;
  score_ = 0;
  high_score_ = 0;
  level_ = 0;
  FileRead();
}

s21::SnakeModel::SnakeModel(std::unique_ptr<IGenerator> generator)
    : head_coordinate_(),
      body_coordinate_(),
      food_(kWidght, kHight, std::move(generator)),
      state_(&StartState::GetInstance()),
      file_() {
  snake_lenght_ = 4;
  head_direction_ = s21::HeadDirection::kRight;
  game_status_ = s21::GameStatus::kIdle;
  is_game_on_ = false;
  gameover_locker_ = false;
  score_ = 0;
  high_score_ = 0;
  level_ = 0;
  FileRead();
}

s21::SnakeModel::~SnakeModel() {
  RefreshHighScore();

  if (file_.is_open()) {
    file_.close();
  }
}

/// @brief A methods for exectute current state and change state
void s21::SnakeModel::Handle() { state_->Handle(this); }

const s21::SnakeStateMachine *s21::SnakeModel::get_current_state()
    const noexcept {
  return state_;
}

/**
 * @brief Accessor for all (x and y coordinate) body coordingate
 *
 * @return std::vector<s21::SnakeModel::coordinate>
 */
const std::vector<s21::coordinate> &s21::SnakeModel::get_coordinate()
    const noexcept {
  return body_coordinate_;
}

/// @brief Accessor for get food coordinate:  first - x, second - y
const s21::coordinate &s21::SnakeModel::get_food_coordinate() const noexcept {
  return food_.GetCoordinate();
}

/// @brief Accessor for private field snake_lenght_
/// @return snake_lenght_
int s21::SnakeModel::get_lenght() const noexcept { return snake_lenght_; }

s21::GameStatus s21::SnakeModel::get_game_status() const noexcept {
  return game_status_;
}

bool s21::SnakeModel::get_game_run() const noexcept { return is_game_on_; }

int s21::SnakeModel::get_score() const noexcept { return score_; }

int s21::SnakeModel::get_high_score() const noexcept { return high_score_; }

int s21::SnakeModel::get_level() const noexcept { return level_; }

bool s21::SnakeModel::IsGameOverLock() const noexcept {
  return gameover_locker_;
}

void s21::SnakeModel::set_game_status(s21::GameStatus new_status) noexcept {
  if (game_status_ == GameStatus::kWin or game_status_ == GameStatus::kIdle) {
    return;
  }

  game_status_ = new_status;
}

/// @brief Mutator for change current state
/// @param new_state can come: StartState, SpawnState, MoveState,
///                            FoodEatenState, PauseState, GameOverState
void s21::SnakeModel::set_new_state(SnakeStateMachine &new_state) noexcept {
  state_ = &new_state;
}

void s21::SnakeModel::SwitchPauseStatus() noexcept {
  if (game_status_ != GameStatus::kPause) {
    game_status_ = GameStatus::kPause;

  } else {
    game_status_ = GameStatus::kRun;
  }
}

void s21::SnakeModel::SwitchingGameRun(bool signal) noexcept {
  is_game_on_ = signal;
}

void s21::SnakeModel::set_gameover_locker(bool signal) noexcept {
  gameover_locker_ = signal;
}

/**
 * @brief A method respawn food
 *
 */
void s21::SnakeModel::RespawnFood() { food_.Respawn(body_coordinate_); }

/// @brief  Methods from group of control snake directions, that methods change
/// snake head directions to RIGHT if he don't see to LEFT
bool s21::SnakeModel::TurnToRight() noexcept {
  if (head_direction_ != s21::HeadDirection::kLeft and
      game_status_ != GameStatus::kPause) {
    head_direction_ = s21::HeadDirection::kRight;
    return true;
  }

  return false;
}

/// @brief  Methods from group of control snake directions, that methods change
/// snake head directions to LEFT if he don't see to RIGHT
bool s21::SnakeModel::TurnToLeft() noexcept {
  if (head_direction_ != s21::HeadDirection::kRight and
      game_status_ != GameStatus::kPause) {
    head_direction_ = s21::HeadDirection::kLeft;
    return true;
  }

  return false;
}

/// @brief  Methods from group of control snake directions, that methods change
/// snake head directions to UP if he don't see to DOWN
bool s21::SnakeModel::TurnToUp() noexcept {
  if (head_direction_ != s21::HeadDirection::kDown and
      game_status_ != GameStatus::kPause) {
    head_direction_ = s21::HeadDirection::kUp;
    return true;
  }

  return false;
}

/// @brief  Methods from group of control snake directions, that methods change
/// snake head directions to DOWN if he don't see to left
bool s21::SnakeModel::TurnToDown() noexcept {
  if (head_direction_ != s21::HeadDirection::kUp and
      game_status_ != GameStatus::kPause) {
    head_direction_ = s21::HeadDirection::kDown;
    return true;
  }

  return false;
}

/// @brief  Moving method provides moving heads with snake bodies and check
/// collisions with board edge and self bodies
void s21::SnakeModel::Moving() noexcept {
  switch (head_direction_) {
    case s21::HeadDirection::kLeft:
      --head_coordinate_.first;
      break;
    case s21::HeadDirection::kRight:
      ++head_coordinate_.first;
      break;
    case s21::HeadDirection::kUp:
      --head_coordinate_.second;
      break;
    case s21::HeadDirection::kDown:
      ++head_coordinate_.second;
      break;
  }

  MoveBody();
}

/**
 * @brief Main methode who handle all snake move, grow and eaten fruit
 *
 */
void s21::SnakeModel::MoveBody() {
  // displacement of all snake body
  for (int i = snake_lenght_; i > 0; --i) {
    std::swap(body_coordinate_[i], body_coordinate_[i - 1]);
  }

  // update head coordinate
  body_coordinate_[0] = head_coordinate_;
  CheckCollision();
}

/**
 * @brief Method for check snake eat food or not. If head coordinate equal to
 * food coordinate = snake eat food and snake grow
 *
 * @return true
 * @return false
 */
bool s21::SnakeModel::IsFoodEaten() const noexcept {
  if (head_coordinate_.first == get_food_x() and
      head_coordinate_.second == get_food_y()) {
    return true;
  }

  return false;
}

/// @brief that method growing up snake body if food is eaten
void s21::SnakeModel::EatingFood() {
  score_ += 1;
  RespawnFood();
  body_coordinate_.push_back(body_coordinate_[snake_lenght_]);
  ++snake_lenght_;

  ScoreHandle();
  Moving();
}

/// @brief the method sets initial state of game
void s21::SnakeModel::InitGame() noexcept {
  int init_x = kWidght / 2;
  int init_y = kHight / 2;

  head_coordinate_ = {init_x, init_y};
  snake_lenght_ = 4;
  head_direction_ = s21::HeadDirection::kRight;
  game_status_ = s21::GameStatus::kRun;
  score_ = 0;
  level_ = 1;

  for (int i = 0; i < kInitLenghtSnake; ++i) {
    body_coordinate_.push_back({init_x - i, init_y});
  }

  RespawnFood();
}

void s21::SnakeModel::ResetGame() noexcept {
  head_coordinate_ = {0, 0};
  snake_lenght_ = 0;
  head_direction_ = s21::HeadDirection::kRight;
  game_status_ = s21::GameStatus::kIdle;
  is_game_on_ = false;
  score_ = 0;
  body_coordinate_.clear();
}

void s21::SnakeModel::RefreshHighScore() noexcept {
  file_.open(path_to_data_file_, std::ios::out);

  if (file_.is_open()) {
    file_ << high_score_;
    file_.close();
  }
}

int s21::SnakeModel::Difficult(int base_timer,
                               int difficult_modifaer) noexcept {
  return base_timer - level_ * difficult_modifaer;
}

/**
 * @brief a method for check board collision but not checket sanke eaten self or
 * not
 *
 * @return true if with snake all ok
 * @return false if with snake all not ok
 */
void s21::SnakeModel::CheckCollision() noexcept {
  if (get_game_status() == GameStatus::kWin) {
    return;
  }

  if ((get_x() > kWidght) or (get_x() < 0) or (get_y() > kHight) or
      (get_y() < 0)) {
    game_status_ = s21::GameStatus::kGameOver;
    return;
  }

  for (int i = 1; i < snake_lenght_; ++i) {
    if (body_coordinate_[i].first == head_coordinate_.first and
        body_coordinate_[i].second == head_coordinate_.second) {
      game_status_ = s21::GameStatus::kGameOver;
      return;
    }
  }
}

/**
 * @brief Accessor to head x coordinate
 *
 * @return uint x coordinate
 */
int s21::SnakeModel::get_x() const noexcept {
  return int((body_coordinate_.begin())->first);
}

/**
 * @brief Accessor to head y coordinate
 *
 * @return uint y coordinate
 */
int s21::SnakeModel::get_y() const noexcept {
  return int((body_coordinate_.begin())->second);
}

/**
 * @brief Accessor to food x coordinate
 *
 * @return uint x coordinate
 */
int s21::SnakeModel::get_food_x() const noexcept { return food_.x(); }

/**
 * @brief Accessor to food y coordinate
 *
 * @return uint y coordinate
 */
int s21::SnakeModel::get_food_y() const noexcept { return food_.y(); }

void s21::SnakeModel::ScoreHandle() noexcept {
  if (score_ % kToNextLevel == 0) {
    level_ = level_ < 10 ? level_ + 1 : level_;
  }

  if (score_ > high_score_) {
    high_score_ = score_;
  }

  if (score_ == kMaxScore) {
    game_status_ = GameStatus::kWin;
  }
}

void s21::SnakeModel::FileRead() noexcept {
  std::string line;

  file_.open(path_to_data_file_, std::ios::in);

  if (file_.is_open()) {
    getline(file_, line);

    if (!line.empty()) {
      high_score_ = stoi(line);
    }

    file_.close();
  }
}
