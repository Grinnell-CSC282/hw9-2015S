#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

matrix
fill ( int rows, int cols, double val )
{
  int size = rows * cols;
  int i;
  double arr [size];
  for (i=0; i < size; arr[i++] = val); // for

  return create(arr, rows, cols);  
} // fill

matrix
zeros ( int rows, int cols )
{
  return fill ( rows, cols, 0.0 );
} // zeros

matrix
eye ( int n )
{
  matrix I = zeros ( n, n );
  int i;
  for (i=0; i < n*n; i += (n+1))
    I.arr[i] = 1.0;
  return I;
}

matrix
ones ( int rows, int cols )
{
  return fill ( rows, cols, 1.0 );
}

// i * m->cols + j

void 
transpose ( matrix * m )
{
  double tarr[m->size];
  int a, i, j, b;
  for (a=0; a<m->size; a++)
  { 
    /*
      a is the current index of the transpose array
      j represents a column of M
      i represents a row of M
      b is the index of M corresponding to (j,i)
      a |--> (i,j) |--> (j,i) |--> b
      Ultimately, set a = b.
      
    */
    j = a % m->rows;
    i = a / m->rows;
    b = j * m->cols + i;
    tarr[a] = m->arr[b];
  }
  acpy(tarr, m->arr, m->size);
  reshape(m, m->cols, m->rows);
}

matrix
add ( matrix m, matrix n )
{
  if ((m.rows != n.rows) || (m.cols != n.cols))  
  {
    fprintf(stderr, "mismatched matrix shapes\n"); 
    exit(1);
  }

  matrix p = mat_alloc(m.size, m.rows, m.cols);
  int i;
  for (i=0; i<m.size; i++)
    p.arr[i] = m.arr[i] + n.arr[i];
  return p;
}

matrix
scale ( matrix m, double c )
{
  matrix n = matcpy ( m );
  int i;
  for (i = 0; i < m.size; i++)
    m.arr[i] *= c;

  return n;
}

