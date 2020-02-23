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
  ui->DeskSizeSB->setRange(5,100);
  ui->progressBar->setRange(0,100);
  ui->progressBar->setValue(0);
}

Fitness::~Fitness()
{
  delete ui;
}

void Fitness::on_StartBatton_clicked()
{

}
