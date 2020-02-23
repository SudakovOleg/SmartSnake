#ifndef MATRIX_H
#define MATRIX_H


class Matrix
{
public:
  Matrix(int _n, int _m);
  double& at(int index_first, int index_second);
  int n,m;
private:
  double** mtx;
};

#endif // MATRIX_H
