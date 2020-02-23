#include "vector.h"
#include <iostream>

Vector::Vector(int _lenght)
{
  this->lenght = _lenght;
  vectr = new double[_lenght];
}

Vector::Vector(const double *_values, int _size)
{
  this->lenght = _size;
  vectr = new double[lenght];
  for (int i(0); i < lenght; i++)
  {
    vectr[i] = _values[i];
  }
}

double &Vector::at(int _index)
{
  if(_index > -1 && _index < lenght)
    return vectr[_index];
  return vectr[0];
}

double &Vector::operator[](int _index)
{
  if(_index > -1 && _index < lenght)
    return vectr[_index];
  return vectr[0];
}

int Vector::size()
{
  return lenght;
}

