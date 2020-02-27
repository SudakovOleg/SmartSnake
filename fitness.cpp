#include "fitness.h"
#include "ui_fitness.h"
#include <QThread>
#include <QList>

//Конструктор
Fitness::Fitness(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Fitness)
{
  ui->setupUi(this);
  ui->LaysSB->setRange(0,5);
  ui->NeyroSB->setRange(1,100);
  ui->AllMutRB->setChecked(true);
  ui->PopulationSizeSB->setRange(2,5000);
  ui->SizeGenSB->setRange(5, 100000);
  ui->MutChanceSB->setRange(1, 100);
  ui->DeskSizeSB->setRange(25,100);
  ui->progressBar->setRange(0,100);
  ui->progressBar->setValue(0);
  board = nullptr;
}

//Диструктор
Fitness::~Fitness()
{
  delete ui;
}

//Слот нажатя кнопки старта
void Fitness::on_StartBatton_clicked()
{
  if(!workStatus)
  {
    //Переименовываем кнопку
    ui->StartBatton->setText("Стоп");
    //Создаем поле
    if(board == nullptr)
    {
      board = new Board(this, ui->DeskSizeSB->value());
      ui->DeskLay->addWidget(board);
    }
    //Создаем массив для нейросети
    int* arr = new int;
    for(int i(0); i < ui->LaysSB->value(); i++)
    {
      arr[i] = ui->NeyroSB->value();
    }
    //Подготавливаем данные о мутациях
    mutation mut;
    if(ui->XCrossMutRB->isChecked())
    {
      mut = X_CROSS;
    }
    else if(ui->GenMutRB->isChecked())
    {
      mut = GEN_MUT;
    }
    else
    {
      mut = ALL_MUT;
    }
    //Инициализируем змеек
    snake = new Snake*[ui->PopulationSizeSB->value()];
    for(int i(0); i < ui->PopulationSizeSB->value(); i++)
    {
      snake[i] = new Snake(arr, ui->LaysSB->value(), ui->DeskSizeSB->value(), board, mut, ui->MutChanceSB->value());
    }
    delete arr;
    //Меняем статус работы
    workStatus = true;
    //Устанавливаем количество поколений
    SizeGen = ui->SizeGenSB->value();
    //Запускаем змеек
    life();
  }
  else
  {
    //Переименовываем кнопку
    ui->StartBatton->setText("Старт");
    workStatus = false;
  }
}

//Процесс жизни популяции
void Fitness::life()
{
  ui->progressBar->setRange(0,SizeGen);
  QList<Snake*> snakes;
  double midlleLen = 0;
  while (workStatus && --SizeGen > 0)
  {
    ui->progressBar->setValue(ui->progressBar->maximum() - SizeGen + 1);
    ui->PopulationLCD->display(ui->progressBar->maximum() - SizeGen);
    midlleLen = 0;
    snakes.clear();
    snakes.push_back(snake[0]);
    //Процесс игры
    for(int i(0); i < ui->PopulationSizeSB->value(); i++)
    {
      snake[i]->start();
      int steps = 1500;
      while (!snake[i]->isGameOver() && --steps > 0)
      {
        snake[i]->turn();
      }
      midlleLen += snake[i]->len();
      if(snake[i]->len() >= snakes.first()->len())
      {
        if(snake[i]->len() > snakes.first()->len())
          snakes.clear();
        ui->RecordLCD->display(snake[i]->len());
        if(snake[i]->len() > ui->MaxRecordLCD->value())
          ui->MaxRecordLCD->display(snake[i]->len());
        snakes.push_back(snake[i]);
      }
    }

    //Подсчет среднего результата
    midlleLen /= ui->PopulationSizeSB->value();
    ui->MiddleRecordLCD->display(midlleLen);

    //Вывод лучше особи
    snakes.first()->start();
    snakes.first()->show();
    while(!snakes.first()->isGameOver())
    {
      snakes.first()->turn();
      QApplication::processEvents();
    }
    snakes.first()->hide();

    //Скрещивание
    for(int i(0); i < ui->PopulationSizeSB->value(); i++)
    {
      if(!snakes.contains(snake[i]))
        snake[i]->cross(snakes);
    }
  }
}
