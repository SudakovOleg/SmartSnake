#include "fitness.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Fitness w;
  w.show();

  return a.exec();
}
