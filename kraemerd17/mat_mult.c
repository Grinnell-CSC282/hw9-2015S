#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

double
vdot (double * u, double * v, int n)
{
  int i;
  double prod = 0.0;
  while (i++ < n)
    prod += u[i] * v[i];
  return prod;
}

matrix
dot (matrix A, matrix B)
{
  if (A.cols != B.rows)
  {
    fprintf(stderr, "mismatched matrix shapes\n");
    exit(1);
  }

  int size_C = A.rows * B.cols;
  matrix C = mat_alloc ( size_C, A.rows, B.cols );
  double col_vec[B.rows], val;
  int i, j, k;

  for (i=0; i < A.rows; i++)
  {
    for (j=0; j < B.cols; j++)
    {
      for (k=0; k < B.rows; k++)
      {
        col_vec[k] = B.arr[k*B.cols + j];
      }
      val = vdot (A.arr, col_vec, B.rows);
      C.arr[i * B.cols + j] = val;
    }
  }

  return C;

} // dot
