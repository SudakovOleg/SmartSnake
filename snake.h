#ifndef SNAKE_H
#define SNAKE_H

#include <QList>
#include "deskwnd.h"

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
  XCross,
  GenMut,
  AllMut
};

class Snake
{
public:
  Snake(const int* _sizesNet, int _layersNet,
        int _board_size, Board* board = nullptr,
        mutation = AllMut, int ChanceMut = 15);
  ~Snake();
  void start();
  bool isGameOver();
  int len();
  void turn();
  void show();
  void hide();
private:
  bool isBack(directon);
  void directionChange();
  void generateApple();
  bool isEat();
  void burnOnBord();

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
