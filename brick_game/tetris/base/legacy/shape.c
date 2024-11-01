#include "../../include/legacy/backend.h"

void swapBlock(GameInfo_t src, GameInfo_t *dst) {
  dst->blockType = src.nextBlockType;

  for (int i = 0; i < BLOCK_WIGHT; i++) {
    for (int j = 0; j < BLOCK_WIGHT; j++) {
      dst->currentBlock[i][j] = src.next[i][j];
    }
  }
}

GameInfo_t getNewBlock(GameInfo_t shape) {
  const int tetramino[7][4][4] = {
      {
          {0, 0, 0, 0},
          {0, 1, 1, 0},
          {0, 1, 1, 0},
          {0, 0, 0, 0},
      },
      {
          {0, 0, 0, 0},
          {0, 0, 1, 0},
          {1, 1, 1, 0},
          {0, 0, 0, 0},
      },
      {
          {0, 0, 0, 0},
          {1, 0, 0, 0},
          {1, 1, 1, 0},
          {0, 0, 0, 0},
      },
      {
          {0, 0, 0, 0},
          {0, 1, 0, 0},
          {1, 1, 1, 0},
          {0, 0, 0, 0},
      },
      {
          {0, 0, 0, 0},
          {1, 1, 1, 1},
          {0, 0, 0, 0},
          {0, 0, 0, 0},
      },
      {
          {0, 0, 0, 0},
          {1, 1, 0, 0},
          {0, 1, 1, 0},
          {0, 0, 0, 0},
      },
      {
          {0, 0, 0, 0},
          {0, 1, 1, 0},
          {1, 1, 0, 0},
          {0, 0, 0, 0},
      },
  };

  GameInfo_t newBlock = shape;
  newBlock.row = 0;
  newBlock.colums = 4;
  int numberOfBlock = 7;
  newBlock.nextBlockType = rand() % numberOfBlock;

  for (int i = 0; i < BLOCK_WIGHT; i++) {
    for (int j = 0; j < BLOCK_WIGHT; j++) {
      newBlock.next[i][j] = tetramino[newBlock.nextBlockType][i][j];
    }
  }

  newBlock.ancor = 0;

  if (!checkCollision(newBlock)) {
    newBlock.gameOn = 0;
  }

  return newBlock;
}

int checkCollision(GameInfo_t shape) {
  int flag = 1;

  for (int i = 0; i < BLOCK_WIGHT && flag; i++) {
    for (int j = 0; j < BLOCK_WIGHT && flag; j++) {
      if (shape.row + i - 1 >= FIELD_ROWS ||
          shape.colums - 1 + j >= FIELD_COLUMS || shape.colums + j - 1 < 0) {
        if (shape.currentBlock[i][j]) {
          flag = 0;
        }

      } else if (shape.table[shape.row + i - 1][shape.colums + j - 1] &&
                 shape.currentBlock[i][j]) {
        flag = 0;
      }
    }
  }

  return flag;
}

void writeToTable(GameInfo_t *shape) {
  for (int i = 0; i < BLOCK_WIGHT; i++) {
    for (int j = 0; j < BLOCK_WIGHT; j++) {
      if (shape->currentBlock[i][j]) {
        shape->field[shape->row + i - 1][shape->colums + j - 1] =
            shape->currentBlock[i][j];
      }
    }
  }
}

void clearField(GameInfo_t *shape) {
  for (int i = 0; i < FIELD_ROWS; i++) {
    for (int j = 0; j < FIELD_COLUMS; j++) {
      shape->field[i][j] = 0;
    }
  }
}

void blockFixing(GameInfo_t *gameTable) {
  int correctRowCoordinate = 2;

  for (int i = 0; i < BLOCK_WIGHT; i++) {
    for (int j = 0; j < BLOCK_WIGHT; j++) {
      if (gameTable->currentBlock[i][j]) {
        gameTable->table[gameTable->row - correctRowCoordinate + i]
                        [gameTable->colums - 1 + j] =
            gameTable->currentBlock[i][j];
      }
    }
  }
}

void blockRotate(GameInfo_t *game) {
  GameInfo_t tempShape = *game;

  for (int i = 0; i < BLOCK_WIGHT; i++) {
    for (int j = 0, k = BLOCK_WIGHT - 1; j < BLOCK_WIGHT; j++, k--) {
      game->currentBlock[i][j] = tempShape.currentBlock[k][i];
    }
  }
}