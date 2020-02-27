#include "snake.h"
#include "vector.h"
#include "matrix.h"
#include "network.h"

#define INPUT 12

Snake::Snake(const int *_sizesNet, int _layersNet, int _board_size, Board* _board, mutation _mut, int ChanceMut)
{
int _sizes[_layersNet + 2];
_sizes[0] = INPUT;
_sizes[_layersNet + 1] = 4;
for(int i(1); i < _layersNet + 1; i++)
{
    _sizes[i] = _sizesNet[i-1];
}
brain = new Network(_sizes,_layersNet + 2);
board_size = _board_size;
board = _board;
mut = _mut;
mutChance = ChanceMut;
cells = new QList<cell>();
apple = new cell();
}

Snake::~Snake()
{
  delete board;
  delete apple;
  delete cells;
}

//Старт игры
void Snake::start()
{
  cells->clear();
  //Создание начальной змейки
  cell head;
  head.x = 3;
  head.y = 3;
  cells->push_back(head);
  cell body;
  body.x = 3;
  body.y = 4;
  cells->push_back(body);
  //Генерация яблока
  generateApple();
  directionNow = UP;
  directionNext = UP;

  if(isShow)
  {
    board->show();
    board->clear();
    burnOnBord();
  }
}

//Проверка на окончание игры
bool Snake::isGameOver()
{
  //Если попал в бортик по горизонтали
  if(cells->first().x < 0 || cells->first().x >= board_size)
  {
    return true;
  }
  //Если попал в бортик по вертикали
  if(cells->first().y < 0 || cells->first().y >= board_size)
  {
    return true;
  }
  //Если поймал хвост
  for(int i(1); i < cells->length(); i++)
  {
    if(cells->at(0).x == cells->at(i).x && cells->at(0).y == cells->at(i).y)
    {
      return true;
    }
  }
    return false;
}

//Метод проверки наличия яблока
bool Snake::isEat()
{
  return (cells->at(0).x == apple->x && cells->at(0).y == apple->y);
}

//Метод генерации яблока
void Snake::generateApple()
{
  //Пока не создадим яблоко вне тела змеи
  bool place = true;
  while(place)
  {
    this->apple->x = rand()%(board_size-1) + 1;
    this->apple->y = rand()%(board_size-1) + 1;
    place = false;
    for(auto body_cell : *cells)
    {
      if(body_cell.x == apple->x && body_cell.y == apple->y)
        place = true;
    }
  }
}

//Метод изменения направления
void Snake::directionChange()
{
    if(isBack(directionNext))
      return;
    directionNow = directionNext;
}

//Проверка обратное ли направление
bool Snake::isBack(directon _dir)
{
  switch(directionNow)
  {
    case UP:
    {
        if(_dir == DOWN)
          return true;
        break;
    }
    case RIGHT:
    {
        if(_dir == LEFT)
          return true;
        break;
    }
    case DOWN:
    {
        if(_dir == UP)
          return true;
        break;
    }
    case LEFT:
    {
        if(_dir == RIGHT)
          return true;
        break;
    }
    default:
      break;
  }
  return false;
}

//Получение длины змейки
int Snake::len()
{
  return cells->size();
}

//Метод этапа хода
void Snake::turn()
{
  directionChange();
  if(!isEat())
  {
    cells->removeLast();
  }
  else
  {
    generateApple();
  }
  switch(directionNow)
  {
    case UP:
    {
        cell head;
        head.x = cells->at(0).x;
        head.y = cells->at(0).y - 1;
        cells->push_front(head);
        break;
    }
    case LEFT:
    {
        cell head;
        head.x = cells->at(0).x + 1;
        head.y = cells->at(0).y;
        cells->push_front(head);
        break;
    }
    case DOWN:
    {
        cell head;
        head.x = cells->at(0).x;
        head.y = cells->at(0).y + 1;
        cells->push_front(head);
        break;
    }
    case RIGHT:
    {
        cell head;
        head.x = cells->at(0).x - 1;
        head.y = cells->at(0).y;
        cells->push_front(head);
        break;
    }
    default:
      break;
  }
  if(!isGameOver() && isShow)
  {
    board->clear();
    burnOnBord();
  }

  int answer = brain->LifeStep(eye());

  switch (answer)
  {
    case 0:
      directionNext = UP;
      break;
    case 1:
      directionNext = LEFT;
      break;
    case 2:
      directionNext = DOWN;
      break;
    case 3:
      directionNext = RIGHT;
      break;
  }

}

//Включить вывод
void Snake::show()
{
  isShow = true;
}
//Выключить вывод
void Snake::hide()
{
  isShow = false;
}

//Изменение весов путем скрещивания двух особей
void Snake::cross(QList<Snake *> snakes)
{
//Скрещивание
  for (int i(0); i < brain->getLayersN(); i++)
  {
    for (int x(0); x < brain->weights[i]->n; x++)
    {
      for(int y(0); y < brain->weights[i]->m; y++)
      {
          brain->weights[i]->at(x,y) = snakes[rand()%snakes.size()]->brain->weights[i]->at(x,y);
      }
    }
  }
//Мутация
  switch (mut)
  {
    case GEN_MUT:
      {
        genMut();
        break;
      }
    case X_CROSS:
      {
        xcrossMut();
        break;
      }
    case ALL_MUT:
      {
        allMut();
        break;
      }
  }
}

//Зрение
Vector Snake::eye()
{
  Vector see(INPUT);

  int count = 0;
  see.at(count++) = cells->at(0).x;
  see.at(count++) = cells->at(0).y;
  see.at(count++) = board_size - cells->at(0).x;
  see.at(count++) = board_size - cells->at(0).y;
  see.at(count++) = cells->at(0).x - apple->x;
  see.at(count++) = cells->at(0).y - apple->y;
  see.at(count++) = (board_size - cells->at(0).x)-(board_size - apple->x);
  see.at(count++) = (board_size - cells->at(0).y)-(board_size - apple->y);;
  see.at(count) = 0;
  //Up
  for (int i(cells->at(0).y); i >= 0; i--)
  {
    bool flagBreak = false;
    for (int body(1); body < cells->size();body++)
      if(cells->at(body).x == cells->at(0).y && cells->at(body).y == i)
      {
        flagBreak = true;
        break;
      }
    see.at(count)++;
    if(flagBreak)
      break;
  }
  see.at(++count) = 0;
  //Right
  for (int i(cells->at(0).x); i < board_size; i++)
  {
    bool flagBreak = false;
    for (int body(1); body < cells->size();body++)
      if(cells->at(body).x == i && cells->at(body).y == cells->at(0).y)
      {
        flagBreak = true;
        break;
      }
    see.at(count)++;
    if(flagBreak)
      break;
  }
  see.at(++count) = 0;
  //Down
  for (int i(cells->at(0).y); i < board_size; i++)
  {
    bool flagBreak = false;
    for (int body(1); body < cells->size();body++)
      if(cells->at(body).x == cells->at(0).y && cells->at(body).y == i)
      {
        flagBreak = true;
        break;
      }
    see.at(count)++;
    if(flagBreak)
      break;
  }
  see.at(++count) = 0;
  //Left
  for (int i(cells->at(0).x); i >= 0; i--)
  {
    bool flagBreak = false;
    for (int body(1); body < cells->size();body++)
      if(cells->at(body).x == i && cells->at(body).y == cells->at(0).y)
      {
        flagBreak = true;
        break;
      }
    see.at(count)++;
    if(flagBreak)
      break;
  }
  return see;
}

//Метод для вывода данных на поле
void Snake::burnOnBord()
{
  board->burn(apple->x, apple->y, "green");
  for(auto body_cell : *cells)
  {
    board->burn(body_cell.x, body_cell.y, "red");
  }
}

void Snake::xcrossMut()
{
  if(rand()%100 < mutChance )
  {
    int lay = rand()%brain->getLayersN();
    auto* arr = new double[brain->weights[lay]->n*brain->weights[lay]->m];
    int count = 0;
    for (int x(0); x < brain->weights[lay]->n; x++)
    {
      for(int y(0); y < brain->weights[lay]->m; y++)
      {
        arr[count++] = brain->weights[lay]->at(x,y);
      }
    }
    for (int x(0); x < brain->weights[lay]->n; x++)
    {
      for(int y(0); y < brain->weights[lay]->m; y++)
      {
        brain->weights[lay]->at(x,y) = arr[count--];
      }
    }
    delete[] arr;
  }
}

void Snake::genMut()
{
  for (int i(0); i < brain->getLayersN(); i++)
  {
    for (int x(0); x < brain->weights[i]->n; x++)
    {
      for(int y(0); y < brain->weights[i]->m; y++)
      {
        if(rand()%100 < mutChance )
        {
          if(rand()%2 == 0)
          {
            brain->weights[i]->at(x,y) += 0.001*(double)(rand()%1000);
            if(brain->weights[i]->at(x,y) > 1)
              brain->weights[i]->at(x,y) = 1;
          }
          else
          {
            brain->weights[i]->at(x,y) -= 0.001*(double)(rand()%1000);
            if(brain->weights[i]->at(x,y) < -1)
              brain->weights[i]->at(x,y) = -1;
          }
        }
      }
    }
  }
//  int placeX, placeY, placeZ;
//  placeZ = rand()%brain->getLayersN();
//  placeX = rand()%brain->weights[placeZ]->n;
//  placeY = rand()%brain->weights[placeZ]->m;
//  if(rand()%2 == 0)
//  {
//    brain->weights[placeZ]->at(placeX,placeY) = 0.001*(double)(rand()%1000);
//  }
//  else
//  {
//    brain->weights[placeZ]->at(placeX,placeY) = -0.001*(double)(rand()%1000);
//  }
}

void Snake::allMut()
{
  if(rand()%2 == 0)
  {
    genMut();
  }
  else
  {
    xcrossMut();
  }
}
