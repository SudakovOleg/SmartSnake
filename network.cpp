#include "network.h"
#include <QApplication>
#include <cmath>
#include "vector.h"
#include "snake.h"

Network::Network(int* _sizes, int _layersN)
{
  layersN = _layersN - 1;
  weights = new Matrix*[layersN];
  for(int i(1); i < _layersN; i++)
  {
    weights[i - 1] = new Matrix(_sizes[i], _sizes[i - 1]);
  }
  srand(QApplication::applicationPid());
}

Vector &Network::Forward(Vector *_input, int lay)
{
    if(lay < layersN)
    {
      Vector output(weights[lay]->n);

      for(int i(0); i < weights[lay]->n; i++)
      {
        long double y = 0;

        for(int j(0); j < weights[lay]->m; j++)
          y += weights[lay]->at(i,j) * _input->at(j);

        double temp = 1/(1+exp(-y));
        output[i] = temp;
      }
      return Forward(&output, ++lay);
    }
    return *_input;
}

Vector Network::LifeStep(Vector _inp)
{
  return Forward(&_inp);
}

int Network::getLayersN()
{
  return layersN;
}
