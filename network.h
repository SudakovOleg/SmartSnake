#ifndef NETWORK_H
#define NETWORK_H

#include "matrix.h"
#include "vector"
#include <QObject>

using namespace std;

class Network
{
public:
  Matrix** weights;
  vector<double> *last_answer;
  //Конструктор
  Network(int* _sizes, int _layersN);
  //Прямое распространение
  vector<double> Forward(vector<double> _input, int _lay = 0);
  //Испытание
  int LifeStep(const vector<double>& _inp);
  int getLayersN();
  int getParamsCount();
private:
  int size;
  int* sizes;
  int layersN;
  int params;
};

#endif // NETWORK_H
