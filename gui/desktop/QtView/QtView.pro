QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../../timer/qt_time_provider.cc \
    main.cc \
    qtview.cc \
    ../../../controller/base/controller.cc \
    ../../../controller/snake/base/snake_controller.cc \
    ../../../brick_game/snake/base/snake_model.cc \
    ../../../brick_game/snake/base/food.cc \
    ../../../brick_game/snake/base/state.cc \
    ../../../brick_game/snake/base/generator.cc \
    ../../../controller/tetris/base/tetris_controller.cc \
    ../../../brick_game/tetris/base/tetris_model.cc \
    ../../../brick_game/tetris/base/legacy/gameStatus.c \
    ../../../brick_game/tetris/base/legacy/shape.c

HEADERS += \
    ../../../timer/qt_time_provider.h \
    qtview.h \
    ../../../constants/constants.h \
    ../../../controller/include/controller.h \
    ../../../controller/snake/include/snake_controller.h \
    ../../../brick_game/snake/include/snake_model.h \
    ../../../brick_game/snake/include/food.h \
    ../../../brick_game/snake/include/state.h \
    ../../../brick_game/snake/include/generator.h \
    ../../../controller/tetris/include/tetris_controller.h \
    ../../../brick_game/tetris/include/tetris_model.h \
    ../../../brick_game/tetris/include/legacy/backend.h


FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
