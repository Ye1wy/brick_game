#include "../include/snake_controller.h"

s21::SnakeController::SnakeController(SnakeModel &model, ViewInterface &view,
                                      ITimerProvider &timer) noexcept
    : model_(model),
      viewer_(view),
      timer_(timer),
      msecs_t_(0),
      signal_(Signal::kNoSignal) {}

void s21::SnakeController::Update(Signal signal) noexcept {
  int current_time = timer_.GetTimeInMilliseconds();

  if (msecs_t_ == 0) {
    msecs_t_ = current_time;
  }

  int elapsed_time = current_time - msecs_t_;

  if (HandleUserInput(signal)) {
    Refresh();
    return;
  }

  if (elapsed_time >= model_.Difficult(kFps, kGameDifficultModifier)) {
    Refresh();
  }
}

bool s21::SnakeController::HandleUserInput(Signal signal) noexcept {
  signal_ = signal;

  if (HandlerMovementInput() || HandlerGameControlInput()) {
    return true;
  }

  return false;
}

bool s21::SnakeController::HandlerMovementInput() noexcept {
  switch (signal_) {
    case Signal::kLeft:
      return model_.TurnToLeft();
    case Signal::kRight:
      return model_.TurnToRight();
    case Signal::kUp:
      return model_.TurnToUp();
    case Signal::kDown:
      return model_.TurnToDown();
    default:
      return false;
  }
}

bool s21::SnakeController::HandlerGameControlInput() noexcept {
  switch (signal_) {
    case Signal::kExit:
      model_.set_game_status(GameStatus::kGameOver);
      return true;
    case Signal::kPause:
      model_.SwitchPauseStatus();
      return true;
    case Signal::kStart:
      HandleGameStatus();
      return false;
    default:
      return false;
  }
}

void s21::SnakeController::Refresh() noexcept {
  msecs_t_ = 0;
  model_.Handle();
  viewer_.Update(model_.get_coordinate(), model_.get_food_coordinate(),
                 model_.get_lenght(), model_.get_score(), model_.get_level(),
                 model_.get_high_score(), model_.get_game_status());
  signal_ = Signal::kNoSignal;
}

void s21::SnakeController::HandleGameStatus() const noexcept {
  switch (model_.get_game_status()) {
    case GameStatus::kIdle:
      model_.SwitchingGameRun(true);
      break;
    case GameStatus::kGameOver:
    case GameStatus::kWin:
      model_.set_gameover_locker(false);
      break;
    default:
      break;
  }
}
