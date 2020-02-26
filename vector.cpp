#include "vector.h"
#include <iostream>

Vector::Vector(int  _lenght)
{
  for (int i(0); i < _lenght; i++)
  {
   vectr.push_back(0);
  }
}

Vector::Vector(const double *_values, int _size)
{
  for (int i(0); i < _size; i++)
  {
    vectr[i] = _values[i];
  }
}

double &Vector::at(int _index)
{
  if(_index > -1 && _index < vectr.size())
    return vectr[_index];
  return vectr[0];
}

double &Vector::operator[](int _index)
{
  if(_index > -1 && _index < vectr.size())
    return vectr[_index];
  return vectr[0];
}

int Vector::size()
{
  return vectr.size();
}

Vector::~Vector()
{
}


