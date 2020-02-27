#ifndef SNAKE_H
#define SNAKE_H

#include <QList>
#include "deskwnd.h"
#include "vector.h"
#include "network.h"

struct cell
{
  int x;
  int y;
};

enum directon
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

enum mutation
{
  X_CROSS,
  GEN_MUT,
  ALL_MUT
};

class Snake
{
public:
  Snake(const int *_sizesNet, int _layersNet,
        int _board_size, Board* board = nullptr,
        mutation = ALL_MUT, int ChanceMut = 15);
  ~Snake();
  void start();
  bool isGameOver();
  int len();
  void turn();
  void show();
  void hide();
  void cross(QList<Snake*> snakes);

  Network* brain;
private:
  Vector eye();
  bool isBack(directon);
  void directionChange();
  void generateApple();
  bool isEat();
  void burnOnBord();
  void xcrossMut();
  void genMut();
  void allMut();
  mutation mut;
  int mutChance;
  int board_size;
  directon directionNext;
  directon directionNow;
  bool isShow = false;
  Board* board;
  cell* apple;
  QList<cell>* cells;
};

#endif // SNAKE_H
