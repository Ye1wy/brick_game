#ifndef BACKEND_H
#define BACKEND_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define FIELD_ROWS 20
#define FIELD_COLUMS 10
#define BLOCK_WIGHT 4

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  Nothing
} UserAction_t;

typedef struct {
  int table[FIELD_ROWS][FIELD_COLUMS];
  int field[FIELD_ROWS][FIELD_COLUMS];
  int currentBlock[BLOCK_WIGHT][BLOCK_WIGHT];
  int next[BLOCK_WIGHT][BLOCK_WIGHT];
  int blockType, nextBlockType, ancor;
  int row, colums;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  int gameOn;
  int start;
} GameInfo_t;

// Game Status
GameInfo_t defaultFieldsState(GameInfo_t gameSettings);
GameInfo_t defaultSetting(GameInfo_t game);
int speedControl(GameInfo_t gameStatus);
int scoreDtr(int count);
void containHighScore(int score);
int getHighScore();
GameInfo_t spawn(GameInfo_t currentGameStatus);
GameInfo_t updateGameStat(GameInfo_t gameState);
// Work with shape
GameInfo_t getNewBlock(GameInfo_t shape);
void writeToTable(GameInfo_t *shape);
void clearField(GameInfo_t *shape);
int checkCollision(GameInfo_t shape);
void swapBlock(GameInfo_t src, GameInfo_t *dst);
void blockFixing(GameInfo_t *shape);
void blockRotate(GameInfo_t *game);
void bigRalf(GameInfo_t *shape);
void blockDown(GameInfo_t *gameState);
void downAction(GameInfo_t *gameState);

#endif
