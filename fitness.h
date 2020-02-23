#ifndef FITNESS_H
#define FITNESS_H

#include <QMainWindow>

namespace Ui {
  class Fitness;
}

class Fitness : public QMainWindow
{
  Q_OBJECT

public:
  explicit Fitness(QWidget *parent = nullptr);
  ~Fitness();

private slots:
  void on_StartBatton_clicked();

private:
  Ui::Fitness *ui;
  int PopulationSize = 2;
  int SizeGen = 5;
};

#endif // FITNESS_H
