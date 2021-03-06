#include "network.h"
#include <QApplication>
#include <cmath>
#include "vector"
#include "snake.h"

Network::Network(int* _sizes, int _layersN)
{
  layersN = _layersN - 1;
  weights = new Matrix*[layersN];
  for(int i(1); i < _layersN; i++)
  {
    weights[i - 1] = new Matrix(_sizes[i], _sizes[i - 1]);
    params += _sizes[i] * _sizes[i - 1];
  }
}

vector<double> Network::Forward(vector<double> _input, int lay)
{
    if(lay < layersN)
    {
      vector<double> output(weights[lay]->n);

      for(int i(0); i < weights[lay]->n; i++)
      {
        long double y = 0;

        for(int j(0); j < weights[lay]->m; j++)
          y += weights[lay]->at(i,j) * _input.at(j);

        double temp = 1/(1+std::exp(-y));
        output.at(i) = temp;
      }
      return Forward(output, ++lay);
    }
    return _input;
}

int Network::LifeStep(const vector<double>& _inp)
{
  vector<double> newDir = Forward(_inp);
  int answer;
  double max = -100;
  for(int i(0); i < newDir.size(); i++)
      if(newDir.at(i) > max)
      {
        max = newDir.at(i);
        answer = i;
      }
  return answer;
}

int Network::getLayersN()
{
  return layersN;
}

int Network::getParamsCount()
{
  return params;
}
