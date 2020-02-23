#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

class Vector
{
public:
  Vector(int _len);
  Vector(const double *_values,int size);
  double& at(int _index);
  double& operator[](int _index);
  int size();
private:
  double* vectr;
  int lenght;
};

#endif // VECTOR_H
