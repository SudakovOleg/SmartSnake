#include "fitness.h"
#include "ui_fitness.h"
#include <QThread>
#include <QList>
#include "graphwidget.h"

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
    GraphMidRecord = new GraphWidget(Range(0,ui->SizeGenSB->value()/10),Range(0,50),this);
    ui->GraphGB->layout()->addWidget(GraphMidRecord);
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
    delete GraphMidRecord;
    workStatus = false;
  }
}

//Процесс жизни популяции
void Fitness::life()
{
  ui->statusBar->showMessage("Провожу отсеивание");
  ui->progressBar->setRange(0,SizeGen);

  QList<Snake*> bestSnakes;
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
      if(snake[i]->len() > bestSnakes.back()->len())
      {
        for(int n(0); n < bestSnakes.size(); n++)
        {
          if(bestSnakes[n]->len() < snake[i]->len())
          {
            bestSnakes.insert(n - 1, snake[i]);
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
    GraphMidRecord->addElement((ui->SizeGenSB->value() - SizeGen)/10,totalLen);
    ui->RecordLCD->display(bestSnakes.first()->len());
    if(bestSnakes.first()->len() > ui->MaxRecordLCD->value())
      ui->MaxRecordLCD->display(bestSnakes.first()->len());

    ui->statusBar->showMessage("Димонстрация промежуточного результата");
    test(bestSnakes.first());

    cross(bestSnakes);
  }
}

//Метод скрещивания
void Fitness::cross(const QList<Snake*>& best_s)
{
  double totalLen = 0;
  std::vector<int> index_to_cross(best_s.first()->brain->getParamsCount());
  for(auto s : best_s)
  {
    totalLen += s->len();
  }

  int count = 0;
  for(int i(0); i < best_s.size(); i++)
  {
    int part = (index_to_cross.size() * ((double)best_s[i]->len()/totalLen));
    for(int j(0); j < part; j++)
    {
      index_to_cross[count++] = i;
    }
  }
  //Скрещивание
  for(int i(0); i < ui->PopulationSizeSB->value(); i++)
  {
    if(!best_s.contains(snake[i]))
      snake[i]->cross(best_s,index_to_cross);
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
