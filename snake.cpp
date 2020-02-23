#include "snake.h"

Snake::Snake(const int *_sizesNet, int _layersNet, int _board_size, Board* _board, mutation _mut, int ChanceMut)
{
Q_UNUSED(_sizesNet);
Q_UNUSED(_layersNet);
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
}

//Включить вывод
void Snake::show()
{
  isShow = true;
}
//Выключить вывод
void Snake::hide()
{
  isShow - false;
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
