#include "fitness.h"
#include "ui_fitness.h"

Fitness::Fitness(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Fitness)
{
  ui->setupUi(this);
  ui->LaysSB->setRange(0,5);
  ui->NeyroSB->setRange(1,100);
  ui->AllMutRB->setChecked(true);
  ui->PopulationSizeSB->setRange(2,100);
  ui->SizeGenSB->setRange(5, 100000);
  ui->MutChanceSB->setRange(1, 100);
  ui->DeskSizeSB->setRange(25,100);
  ui->progressBar->setRange(0,100);
  ui->progressBar->setValue(0);
}

Fitness::~Fitness()
{
  delete ui;
}

void Fitness::on_StartBatton_clicked()
{
  if(!workStatus)
  {
    board = new Board(this, ui->DeskSizeSB->value());
    ui->DeskLay->addWidget(board);
    snake = new Snake(nullptr, 0, ui->DeskSizeSB->value(), board);
    snake->show();
    workStatus = true;
    snake->start();
    life();
  }
  else
  {

  }
}

void Fitness::life()
{
  while (workStatus && !snake->isGameOver())
  {
    snake->turn();
    QApplication::processEvents();
  }
}
