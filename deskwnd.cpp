#include "deskwnd.h"
#include "ui_deskwnd.h"
#include <QFrame>
#include <QApplication>

#define SIZE 50
#define CELL_SIZE 15

Board::Board(QWidget *parent, int _deskSize) :
  QMainWindow(parent),
  ui(new Ui::DeskWnd)
{
  ui->setupUi(this);
   deskSize = _deskSize;
  //Инициализация объектов
  cell = new QFrame*[deskSize];
  for(int i(0); i < deskSize; i++)
  {
    cell[i] = new QFrame[deskSize];
  }
  mainLay = new QGridLayout;
  //Компоновка
  QSizePolicy policy;
  policy.setVerticalPolicy(QSizePolicy::Fixed);
  policy.setHorizontalPolicy(QSizePolicy::Fixed);
  for(int x(0); x < deskSize; x++)
    for(int y(0); y < deskSize; y++)
    {
      cell[x][y].setFixedSize(CELL_SIZE,CELL_SIZE);
      cell[x][y].setSizePolicy(policy);
      cell[x][y].setStyleSheet("background-color: gray;");
      mainLay->addWidget(&cell[x][y],y,x);
    }
  mainLay->setSpacing(1);
  ui->centralWidget->setLayout(mainLay);
}

//Зажечь клетку по координате
void Board::burn(int x, int y, const QString& color)
{
  cell[x][y].setStyleSheet("background-color: " + color + ";");
}
//Потушить огни
void Board::clear()
{
  for(int x(0); x < deskSize; x++)
    for(int y(0); y < deskSize; y++)
    {
      cell[x][y].setStyleSheet("background-color: gray;");
    }
}

void Board::keyPressEvent(QKeyEvent *event)
{
  int key = event->key();
  switch (key)
  {
    case 16777237:
      {
        emit pressKey(3);
        break;
      }
    case 16777236:
      {
        emit pressKey(2);
        break;
      }
    case 16777235:
      {
        emit pressKey(1);
        break;
      }
    case 16777234:
      {
        emit pressKey(4);
        break;
      }
    default:
      break;
  }
}

Board::~Board()
{
  for(int x(0); x < deskSize; x++)
      delete [] cell[x];
  delete [] cell;
  delete mainLay;
  delete ui;
}

void Board::score(QString str)
{
  ui->statusBar->showMessage(str);
}
