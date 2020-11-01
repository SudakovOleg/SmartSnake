#include <random>
#include <cmath>
#include <algorithm>
#include "snake.h"
#include "vector"
#include "matrix.h"
#include "network.h"
#include <QDebug>

#define INPUT 16

Snake::Snake(const int *_sizesNet, int _layersNet, int _board_size, Board* _board, mutation _mut, int ChanceMut)
{
QVector<int> _sizes(_layersNet + 2);
_sizes[0] = INPUT;
_sizes[_layersNet + 1] = 4;
for(int i(1); i < _layersNet + 1; i++)
{
    _sizes[i] = _sizesNet[i-1];
}
brain = new Network(&_sizes);
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
  head.x = board_size/2;
  head.y = board_size/2;
  cells->push_back(head);
  cell body;
  body.x = (board_size/2)+1;
  body.y = board_size/2;
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

//Вывод весов нс в консоль
void Snake::debugPrint()
{
    QString buff;
    for (int i(0); i < brain->getLayersN(); i++)
    {
      for (int x(0); x < brain->weights[i]->n; x++)
      {
        for(int y(0); y < brain->weights[i]->m; y++)
        {
            buff += QString::number(brain->weights[i]->at(x,y));
            buff += ' ';
        }
      }
      qDebug() << buff;
      buff.clear();
    }
    qDebug() << "__________";
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
    if(cells->first().x == cells->at(i).x && cells->first().y == cells->at(i).y)
    {
      return true;
    }
  }
    return false;
}

//Метод проверки наличия яблока
bool Snake::isEat()
{
  return (cells->first().x == apple->x && cells->first().y == apple->y);
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
        head.x = cells->first().x;
        head.y = cells->first().y - 1;
        cells->push_front(head);
        break;
    }
    case LEFT:
    {
        cell head;
        head.x = cells->first().x + 1;
        head.y = cells->first().y;
        cells->push_front(head);
        break;
    }
    case DOWN:
    {
        cell head;
        head.x = cells->first().x;
        head.y = cells->first().y + 1;
        cells->push_front(head);
        break;
    }
    case RIGHT:
    {
        cell head;
        head.x = cells->first().x - 1;
        head.y = cells->first().y;
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
void Snake::cross(QList<Snake *> snakes, QVector<int> id)
{
  int count = 0;
  std::shuffle(id.begin(),id.end(), std::mt19937(std::random_device()()));
//Скрещивание
  for (int i(0); i < brain->getLayersN(); i++)
  {
    for (int x(0); x < brain->weights[i]->n; x++)
    {
      for(int y(0); y < brain->weights[i]->m; y++)
      {
          brain->weights[i]->at(x,y) = snakes[id[count++]]->brain->weights[i]->at(x,y);
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
vector<double> Snake::eye()
{
  vector<double> see(INPUT);

  int count = 0;
  //Ростояния до границы поля
  see.at(count++) = -cells->first().x;
  see.at(count++) = -cells->first().y;
  see.at(count++) = -(board_size - cells->first().x);
  see.at(count++) = -(board_size - cells->first().y);
  //Растояния до яблока (по прямой)
  see.at(count++) = cells->first().x - apple->x;
  see.at(count++) = cells->first().y - apple->y;
  see.at(count++) = (board_size - cells->first().x)-(board_size - apple->x);
  see.at(count++) = (board_size - cells->first().y)-(board_size - apple->y);
  //Растояния до яблока (по диагонали)
  for(int i(0); i < 4; i++)
    see.at(count+i) = 0;
  if(apple->x < cells->first().x && apple->y < cells->first().y)
  {
    see.at(count) = std::min(cells->first().x - apple->x, cells->first().y - apple->y);
  }
  else if (apple->x < cells->first().x && apple->y > cells->first().y)
  {
    see.at(count + 1) = std::min(cells->first().x - apple->x, apple->y - cells->first().y);
  }
  else if (apple->x > cells->first().x && apple->y < cells->first().y)
  {
    see.at(count + 2) = std::min(apple->x - cells->first().x, cells->first().y - apple->y);
  }
  else if (apple->x > cells->first().x && apple->y > cells->first().y)
  {
    see.at(count + 3) = std::min(apple->x - cells->first().x, apple->y - cells->first().y);
  }
  count += 4;
  //Растояния до хвоста
  see.at(count) = 0;
  //Up
  for (int i(cells->first().y); i >= 0; i--)
  {
    bool flagBreak = false;
    for (int body(1); body < cells->size();body++)
      if(cells->at(body).x == cells->first().y && cells->at(body).y == i)
      {
        flagBreak = true;
        break;
      }
    see.at(count)--;
    if(flagBreak)
      break;
  }
  see.at(++count) = 0;
  //Right
  for (int i(cells->first().x); i < board_size; i++)
  {
    bool flagBreak = false;
    for (int body(1); body < cells->size();body++)
      if(cells->at(body).x == i && cells->at(body).y == cells->first().y)
      {
        flagBreak = true;
        break;
      }
    see.at(count)--;
    if(flagBreak)
      break;
  }
  see.at(++count) = 0;
  //Down
  for (int i(cells->first().y); i < board_size; i++)
  {
    bool flagBreak = false;
    for (int body(1); body < cells->size();body++)
      if(cells->at(body).x == cells->first().y && cells->at(body).y == i)
      {
        flagBreak = true;
        break;
      }
    see.at(count)--;
    if(flagBreak)
      break;
  }
  see.at(++count) = 0;
  //Left
  for (int i(cells->first().x); i >= 0; i--)
  {
    bool flagBreak = false;
    for (int body(1); body < cells->size();body++)
      if(cells->at(body).x == i && cells->at(body).y == cells->first().y)
      {
        flagBreak = true;
        break;
      }
    see.at(count)--;
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
            brain->weights[i]->at(x,y) = .01*(double)(rand()%100);
        }
      }
    }
  }
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
