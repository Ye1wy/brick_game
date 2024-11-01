#include "../include/controller.h"

s21::Controller::Controller(SnakeController &snake_controller_object,
                            TetrisController &tetris_controller_object,
                            ViewInterface &view_object)
    : current_game(nullptr),
      viewer_(view_object),
      snake_controller_(snake_controller_object),
      tetris_controller_(tetris_controller_object),
      selected_game_(SelectedGame::kSnake),
      is_game_selected_(false),
      is_game_ended_(false),
      signal_(Signal::kNoSignal) {}

int s21::Controller::IsGameChoosed() const noexcept {
  return is_game_selected_;
}

bool s21::Controller::IsGameEnded() const noexcept { return is_game_ended_; }

void s21::Controller::set_signal(const Signal signal) noexcept {
  signal_ = signal;
}

void s21::Controller::Update() noexcept {
  signal_ = viewer_.Notify();

  if (!is_game_selected_) {
    HandleSelectMenu();
    return;
  }

  current_game->Update(signal_);
}

void s21::Controller::HandleSelectMenu() noexcept {
  if (!is_game_selected_) {
    switch (signal_) {
      case Signal::kUp:
        selected_game_ = SelectedGame::kSnake;
        break;
      case Signal::kDown:
        selected_game_ = SelectedGame::kTetris;
        break;
      case Signal::kStart:
        is_game_selected_ = true;
        HandleSelectedGame();
        return;
      case Signal::kExit:
        is_game_ended_ = true;
        return;
      default:
        return;
    }

    viewer_.Update(selected_game_, false);
  }
}

void s21::Controller::SwitchGame(IControlGame *game) noexcept {
  current_game = game;
}

void s21::Controller::HandleSelectedGame() {
  switch (selected_game_) {
    case SelectedGame::kSnake:
      SwitchGame(&snake_controller_);
      break;
    case SelectedGame::kTetris:
      SwitchGame(&tetris_controller_);
      break;
    default:
      break;
  }

  viewer_.Update(selected_game_, true);
}
