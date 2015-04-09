#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main ()
{
  matrix I = fill(2,4,1.0);
  set_element(&I, 1,0, 10.0);
  set_element(&I, 0, 2, 5.0);
  print(I);
  matrix A = scale(I, 3.14);

  transpose(&A);

  print(A);

  print (dot(I, A));

  //print (add(I, A));
  exit(0);
}
