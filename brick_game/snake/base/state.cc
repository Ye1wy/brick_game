#include "../include/state.h"

void s21::StartState::Handle(SnakeModel *model) {
  if (model->get_game_run()) {
    model->set_new_state(s21::SpawnState::GetInstance());
  }
}

s21::StartState &s21::StartState::GetInstance() {
  static StartState singletone;
  return singletone;
}

void s21::SpawnState::Handle(SnakeModel *model) {
  model->InitGame();
  model->set_new_state(s21::MoveState::GetInstance());
}

s21::SpawnState &s21::SpawnState::GetInstance() {
  static SpawnState singltone;
  return singltone;
}

void s21::MoveState::Handle(SnakeModel *model) {
  if (model->get_game_status() == GameStatus::kPause) {
    model->set_new_state(s21::PauseState::GetInstance());
  }

  model->Moving();

  if (model->IsFoodEaten()) {
    model->set_new_state(s21::EatFoodState::GetInstance());

  } else if (model->get_game_status() == GameStatus::kGameOver or
             model->get_game_status() == GameStatus::kWin) {
    model->set_gameover_locker(true);
    model->set_new_state(s21::GameOverState::GetInstance());
  }
}

s21::MoveState &s21::MoveState::GetInstance() {
  static MoveState singletone;
  return singletone;
}

void s21::EatFoodState::Handle(SnakeModel *model) {
  model->EatingFood();
  model->set_new_state(s21::MoveState::GetInstance());
}

s21::EatFoodState &s21::EatFoodState::GetInstance() {
  static EatFoodState singletone;
  return singletone;
}

void s21::PauseState::Handle(SnakeModel *model) {
  if (model->get_game_status() == GameStatus::kGameOver) {
    model->set_game_status(s21::GameStatus::kIdle);
    model->ResetGame();
    model->set_new_state(s21::StartState::GetInstance());
    return;
  }

  if (model->get_game_status() != GameStatus::kPause) {
    model->set_new_state(s21::MoveState::GetInstance());
  }
}

s21::PauseState &s21::PauseState::GetInstance() {
  static PauseState singletone;
  return singletone;
}

void s21::GameOverState::Handle(SnakeModel *model) {
  if (!model->IsGameOverLock()) {
    model->ResetGame();
    model->RefreshHighScore();

    model->set_new_state(s21::StartState::GetInstance());
  }
}

s21::GameOverState &s21::GameOverState::GetInstance() {
  static GameOverState sigletone;
  return sigletone;
}