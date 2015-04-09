#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

void
acpy (double * orig, double * target, int n)
{
  int i;
  for (i=0; i<n; i++)
    target[i] = orig[i];
}

matrix
mat_alloc (int size, int rows, int cols)
{
  matrix mat;
  mat.size = size;
  mat.rows = rows;
  mat.cols = cols;
  mat.arr = malloc( sizeof(double) * size);  
  return mat;

}

matrix
create (double * arr, int rows, int cols)
{
  int size = rows * cols;
  matrix mat = mat_alloc ( size, rows, cols );
  acpy(arr, mat.arr, size);
  return mat;
}

matrix
matcpy (matrix m)
{
  return create (m.arr, m.rows, m.cols);
}

void
reshape (matrix * m, int rows, int cols)
{
  if (m->size != rows * cols)
  {
    fprintf(stderr, "invalid reshape option\n");
    exit(1);
  }

  m->rows = rows;
  m->cols = cols;
}

double
element (matrix m, int i, int j)
{
  if (m.rows <= i || m.cols <= j)
  {
    fprintf(stderr, "element does not exist\n");
    exit(1);
  }

  return m.arr[i * m.cols + j];
}

void
set_element (matrix * m, int i, int j, double val)
{
  if (m->rows <= i || m->cols <= j)
  {
    fprintf(stderr, "index out of bounds\n");
    exit(1);
  }
  m->arr[i * m->cols + j] = val;
}

void
print (matrix m)
{
  int i;
  printf("\n");
  for (i = 0; i < m.size; i++) 
  {
    if (i % m.cols == 0) printf("[ ");
    
    printf("%5.2lf ", m.arr[i]);

    if (i % m.cols == m.cols - 1) printf ("]\n");
  }
}
