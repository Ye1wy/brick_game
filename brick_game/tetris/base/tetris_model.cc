#include "../include/tetris_model.h"

s21::TetrisModel::TetrisModel() : model_(), data_for_view_() {
  model_ = {};
  data_for_view_ = {};
  InitGame();
}

int s21::TetrisModel::get_speed() const noexcept { return model_.speed; }

bool s21::TetrisModel::get_start_parametr() const noexcept {
  return model_.start;
}

bool s21::TetrisModel::get_pause_parametr() const noexcept {
  return model_.pause;
}

bool s21::TetrisModel::get_game_on_flag() const noexcept {
  return model_.gameOn;
}

int s21::TetrisModel::get_score() const noexcept { return model_.score; }

int s21::TetrisModel::get_level() const noexcept { return model_.level; }

int s21::TetrisModel::get_high_score() const noexcept {
  return model_.high_score;
}

const s21::TetrisMatrix& s21::TetrisModel::get_matrix() const noexcept {
  return data_for_view_;
}

void s21::TetrisModel::set_start_state(bool signal) noexcept {
  model_.start = signal;
}

void s21::TetrisModel::set_pause_state(bool signal) noexcept {
  model_.pause = signal;
}

void s21::TetrisModel::set_game_on_state(bool signal) noexcept {
  model_.gameOn = signal;
}

void s21::TetrisModel::InitGame() noexcept { model_ = defaultSetting(model_); }

void s21::TetrisModel::Spawn() noexcept { model_ = spawn(model_); }

void s21::TetrisModel::UpdateGameStat() noexcept {
  if (model_.pause || !model_.gameOn) {
    return;
  }

  if (!model_.gameOn and model_.start) {
    model_.gameOn = true;
    return;
  }

  model_ = spawn(model_);
  model_ = updateGameStat(model_);
  ActionDown();
  FillTransmitionData();
}

bool s21::TetrisModel::TurnLeft() noexcept {
  --model_.colums;

  if (!checkCollision(model_)) {
    ++model_.colums;

    return false;
  }

  return true;
}

bool s21::TetrisModel::TurnRight() noexcept {
  ++model_.colums;

  if (!checkCollision(model_)) {
    --model_.colums;

    return false;
  }

  return true;
}

bool s21::TetrisModel::Rotation() noexcept {
  GameInfo_t temp = model_;
  blockRotate(&temp);

  if (checkCollision(temp)) {
    blockRotate(&model_);
    return true;
  }

  return false;
}

bool s21::TetrisModel::BlockDown() noexcept {
  blockDown(&model_);
  return true;
}

void s21::TetrisModel::ActionDown() noexcept { downAction(&model_); }

void s21::TetrisModel::ClearField() noexcept { clearField(&model_); }

void s21::TetrisModel::FillTransmitionData() noexcept {
  for (int i = 0; i < FIELD_ROWS; ++i) {
    for (int j = 0; j < FIELD_COLUMS; ++j) {
      data_for_view_.field[i][j] = model_.field[i][j];
      data_for_view_.table[i][j] = model_.table[i][j];
    }
  }

  for (int i = 0; i < BLOCK_WIGHT; ++i) {
    for (int j = 0; j < BLOCK_WIGHT; ++j) {
      data_for_view_.next_block[i][j] = model_.next[i][j];
    }
  }
}
