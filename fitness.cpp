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
  ui->PopulationSizeSB->setRange(2,15000);
  ui->SizeGenSB->setRange(5, 100000);
  ui->MutChanceSB->setRange(1, 100);
  ui->DeskSizeSB->setRange(25,100);
  ui->progressBar->setRange(0,100);
  ui->progressBar->setValue(0);
  ui->statusBar->showMessage("Готов к работе");
  board = nullptr;
  srand(time(nullptr));
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
    ui->MaxRecordLCD->display(0);
    ui->statusBar->showMessage("Готовлю первое поколение");
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
    ui->statusBar->showMessage("Остановка");
    workStatus = false;
  }
}

//Процесс жизни популяции
void Fitness::life()
{
  ui->statusBar->showMessage("Провожу отсеивание");
  ui->progressBar->setRange(0,SizeGen);

  QList<Snake*> bestSnakes;
  QString tempStrForInfo[2];
  double totalLen = 0;

  while (workStatus && --SizeGen > 0)
  {
    ui->progressBar->setValue(ui->progressBar->maximum() - SizeGen + 1);
    ui->LifePB->setRange(0, ui->PopulationSizeSB->value());
    ui->LifePB->reset();
    ui->PopulationLCD->display(ui->progressBar->maximum() - SizeGen);
    totalLen = 0;
    bestSnakes.clear();
    bestSnakes.push_back(snake[0]);

    //Процесс игры
    for(int i(0); i < ui->PopulationSizeSB->value(); i++)
    {
      snake[i]->start();
      int steps = 200;
      int pastLen = 2;

      while (!snake[i]->isGameOver() && --steps > 0)
      {
        snake[i]->turn();
        if(snake[i]->len() > pastLen)
        {
          steps += 200;
          pastLen = snake[i]->len();
        }
      }

      totalLen += snake[i]->len();
      if(snake[i]->len() > bestSnakes.back()->len() || bestSnakes.size() < ui->croosSB->value())
      {
        for(int n(0); n < bestSnakes.size(); n++)
        {
          if(bestSnakes[n]->len() < snake[i]->len())
          {
            bestSnakes.insert(n, snake[i]);
            break;
          }
        }
        if(bestSnakes.size() > ui->croosSB->value())
          bestSnakes.pop_back();
      }
      ui->LifePB->setValue(ui->LifePB->value() + 1);
      QApplication::processEvents();
    }

    //Подсчет результатов
    totalLen /= ui->PopulationSizeSB->value();
    ui->MiddleRecordLCD->display(totalLen);
    tempStrForInfo[0].setNum(bestSnakes.size());
    tempStrForInfo[1].setNum(bestSnakes.first()->len());
    ui->RecordLCD->display(bestSnakes.first()->len());
    if(bestSnakes.first()->len() > ui->MaxRecordLCD->value())
      ui->MaxRecordLCD->display(bestSnakes.first()->len());

    ui->statusBar->showMessage("Димонстрация промежуточного результата");
    test(bestSnakes.first());

    ui->statusBar->showMessage("Провожу скрещивание " + tempStrForInfo[0] + " особей с длинной " + tempStrForInfo[1]);
    cross(bestSnakes);
  }
}

//Метод скрещивания
void Fitness::cross(const QList<Snake*>& best_s)
{
  //Скрещивание
  for(int i(0); i < ui->PopulationSizeSB->value(); i++)
  {
    if(!best_s.contains(snake[i]))
      snake[i]->cross(best_s);
  }
  QApplication::processEvents();
}

//Метод тестирования
void Fitness::test(Snake *s)
{
  //Вывод лучше особи каждые 10 поколений
  if(SizeGen%1 == 0)
  {
    s->start();
    s->show();
    int steps = 200;
    int pastLen = 2;
    while(!s->isGameOver()&& --steps > 0)
    {
      s->turn();
      if(s->len() > pastLen)
      {
        steps += 200;
        pastLen = s->len();
      }
      QApplication::processEvents();
    }
    s->hide();
  }
}
