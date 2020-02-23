#ifndef FITNESS_H
#define FITNESS_H

#include <QMainWindow>
#include "deskwnd.h"
#include "snake.h"

namespace Ui {
  class Fitness;
}

class Fitness : public QMainWindow
{
  Q_OBJECT

public:
  explicit Fitness(QWidget *parent = nullptr);
  void life();
  ~Fitness();

private slots:
  void on_StartBatton_clicked();

private:
  Ui::Fitness *ui;
  int PopulationSize = 2;
  int SizeGen = 5;
  Board* board;
  bool workStatus = false;
  Snake** snake;
};

#endif // FITNESS_H
