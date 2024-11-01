#include "../../include/legacy/backend.h"

GameInfo_t defaultFieldsState(GameInfo_t gameSettings) {
  for (int i = 0; i < FIELD_ROWS; i++) {
    for (int j = 0; j < FIELD_COLUMS; j++) {
      if (i < BLOCK_WIGHT && j < BLOCK_WIGHT) {
        gameSettings.next[i][j] = 0;
      }

      gameSettings.field[i][j] = 0;
      gameSettings.table[i][j] = 0;
    }
  }

  gameSettings = getNewBlock(gameSettings);
  swapBlock(gameSettings, &gameSettings);
  gameSettings = getNewBlock(gameSettings);
  return gameSettings;
}

int speedControl(GameInfo_t gameStatus) {
  int fps = 400;
  int speedSeter = 20;
  return fps - gameStatus.level * speedSeter;
}

GameInfo_t defaultSetting(GameInfo_t gameSettings) {
  gameSettings = defaultFieldsState(gameSettings);
  gameSettings.ancor = 0;
  gameSettings.row = 0;
  gameSettings.colums = 4;
  gameSettings.level = 1;
  gameSettings.speed = speedControl(gameSettings);
  gameSettings.score = 0;
  gameSettings.high_score = getHighScore();
  gameSettings.gameOn = 0;
  gameSettings.pause = 0;
  gameSettings.start = 0;
  return gameSettings;
}

GameInfo_t updateGameStat(GameInfo_t gameState) {
  bigRalf(&gameState);
  int nextLevelDtr = 0;

  if (gameState.score > gameState.high_score) {
    containHighScore(gameState.score);
    gameState.high_score = gameState.score;
  }

  if (gameState.level <= 10) {
    nextLevelDtr = 600;
    gameState.level = gameState.score / nextLevelDtr + 1;
  }

  gameState.speed = speedControl(gameState);
  return gameState;
}

void bigRalf(GameInfo_t *shape) {
  int lineDestroed = 0;
  int sum = 0;

  for (int i = 0; i < FIELD_ROWS; i++) {
    sum = 0;

    for (int j = 0; j < FIELD_COLUMS; j++) {
      sum += shape->table[i][j];
    }

    if (sum == FIELD_COLUMS) {
      lineDestroed++;
      int k = 0;

      for (k = i; k >= 1; k--) {
        for (int j = 0; j < FIELD_COLUMS; j++) {
          shape->table[k][j] = shape->table[k - 1][j];
        }
      }

      for (int l = 0; l < FIELD_COLUMS; l++) {
        shape->table[k][l] = 0;
      }
    }
  }

  shape->score += scoreDtr(lineDestroed);
}

int scoreDtr(int count) {
  int oneRow = 100;
  int twoRow = 300;
  int threeRow = 700;
  int fourRow = 1500;
  return count == 1   ? oneRow
         : count == 2 ? twoRow
         : count == 3 ? threeRow
         : count == 4 ? fourRow
                      : 0;
}

void containHighScore(int score) {
  FILE *fp = fopen("data/score_table.txt", "w");

  if (fp != NULL) {
    fprintf(fp, "%d", score);
    fclose(fp);
  }
}

int getHighScore() {
  int score = 0;
  FILE *fp = fopen("data/score_table.txt", "r");

  if (fp != NULL) {
    fscanf(fp, "%d", &score);
    fclose(fp);
  }

  return score;
}

void blockDown(GameInfo_t *gameState) {
  while (checkCollision(*gameState)) {
    writeToTable(gameState);
    gameState->row++;
    clearField(gameState);
  }

  gameState->ancor = 1;
  blockFixing(gameState);
}

void downAction(GameInfo_t *gameState) {
  if (checkCollision(*gameState)) {
    writeToTable(gameState);
    gameState->row++;

  } else {
    gameState->ancor = 1;
    blockFixing(gameState);
  }
}

GameInfo_t spawn(GameInfo_t currentGameStatus) {
  if (currentGameStatus.ancor) {
    swapBlock(currentGameStatus, &currentGameStatus);
    currentGameStatus = getNewBlock(currentGameStatus);
  }

  return currentGameStatus;
}