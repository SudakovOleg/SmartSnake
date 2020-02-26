#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <cstdlib>
#include <vector>

class Vector
{
public:
  Vector(int _lenght);
  Vector(const double *_values,int size);
  ~Vector();
  double& at(int _index);
  double& operator[](int _index);
  int size();
private:
  std::vector<double> vectr;
};

#endif // VECTOR_H
