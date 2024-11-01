#include "../include/tetris_controller.h"

s21::TetrisController::TetrisController(TetrisModel& tetris_model_object,
                                        ViewInterface& viewer_object,
                                        ITimerProvider& timer)
    : model_(tetris_model_object),
      viewer_(viewer_object),
      timer_(timer),
      msecs_t_(0),
      signal_(Signal::kNoSignal) {}

void s21::TetrisController::Update(Signal signal) noexcept {
  int current_time = timer_.GetTimeInMilliseconds();

  if (msecs_t_ == 0) {
    msecs_t_ = current_time;
  }

  int elapsed_time = current_time - msecs_t_;

  if (HandleUserInput(signal)) {
    Refresh();
    return;
  }

  if (elapsed_time >= model_.get_speed()) {
    Refresh();
  }
}

bool s21::TetrisController::HandleUserInput(Signal signal) noexcept {
  signal_ = signal;

  if (HandleMoveInput() || HandleGameControlInput()) {
    return true;
  }

  return false;
}

bool s21::TetrisController::HandleMoveInput() noexcept {
  switch (signal_) {
    case Signal::kLeft:
      return model_.TurnLeft();
    case Signal::kRight:
      return model_.TurnRight();
    case Signal::kDown:
      return model_.BlockDown();
    case Signal::kUp:
      return model_.Rotation();
    case Signal::kAction:
      return model_.Rotation();
    default:
      return false;
  }
}

bool s21::TetrisController::HandleGameControlInput() noexcept {
  switch (signal_) {
    case Signal::kStart:
      if (!model_.get_game_on_flag() and model_.get_start_parametr()) {
        model_.InitGame();
        model_.set_start_state(false);
        return false;
      }

      model_.set_start_state(true);
      model_.set_game_on_state(true);
      return true;
    case Signal::kPause:
      model_.set_pause_state(!model_.get_pause_parametr());
      return true;
    case Signal::kExit:
      model_.set_game_on_state(false);
      model_.InitGame();
      model_.FillTransmitionData();
      return true;
    default:
      return false;
  }
}

void s21::TetrisController::Refresh() noexcept {
  msecs_t_ = 0;
  model_.UpdateGameStat();
  model_.ClearField();
  viewer_.Update(model_.get_matrix(), model_.get_score(), model_.get_level(),
                 model_.get_high_score(), model_.get_start_parametr(),
                 model_.get_game_on_flag(), model_.get_pause_parametr());
  signal_ = Signal::kNoSignal;
}