#include <QApplication>
#include <QTimer>

#include "../../../brick_game/snake/include/snake_model.h"
#include "../../../brick_game/tetris/include/tetris_model.h"
#include "../../../controller/include/controller.h"
#include "../../../controller/snake/include/snake_controller.h"
#include "../../../controller/tetris/include/tetris_controller.h"
#include "../../../timer/qt_time_provider.h"
#include "qtview.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  s21::SnakeModel snake_game_model;
  s21::TetrisModel tetris_game_model;
  s21::QtTimeProvider timer;
  s21::QView qt_view;

  s21::SnakeController snake_controller(snake_game_model, qt_view, timer);
  s21::TetrisController tetris_controller(tetris_game_model, qt_view, timer);

  s21::Controller central(snake_controller, tetris_controller, qt_view);

  qt_view.show();

  QTimer gameTimer;
  QObject::connect(&gameTimer, &QTimer::timeout, [&]() {
    central.Update();  // Обновляем игру через контроллер
    qt_view.update();  // Перерисовываем экран
  });

  gameTimer.start(128);

  return app.exec();
}
