#ifndef NETWORK_H
#define NETWORK_H

#include "matrix.h"
#include "vector.h"
#include <QObject>

class Network
{
public:
  Matrix** weights;
  Vector *last_answer;
  //Конструктор
  Network(int* _sizes, int _layersN);
  //Прямое распространение
  Vector &Forward(Vector *_input, int _lay = 0);
  //Испытание
  Vector LifeStep(Vector _inp);
  int getLayersN();
private:
  int size;
  int* sizes;
  int layersN;
};

#endif // NETWORK_H
