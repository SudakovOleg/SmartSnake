#include "matrix.h"
#include <random>

Matrix::Matrix(int _n, int _m)
{
  n = _n;
  m = _m;
  mtx = new double*[n];
  for(int i(0); i < n; i++)
  {
    mtx[i] = new double[m];
    for (int j(0); j < m; j++)
    {
      mtx[i][j] = 1. * (rand()%10);
    }
  }
}

Matrix::~Matrix()
{
  for (int i(0); i < n; n++)
  {
    delete [] mtx[i];
  }
  delete [] mtx;
}

double &Matrix::at(int index_first, int index_second)
{
  double& temp = mtx[index_first][index_second];
  return temp;
}
