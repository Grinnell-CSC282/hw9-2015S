#ifndef MAT_INTERNALS_H_
#define MAT_INTERNALS_H_

/* Matrix computations library header file */

typedef struct {
  double * arr;
  int size, rows, cols;
} matrix;

void
acpy (double * orig, double * target, int n);
/* Copies first n elements of a double array to the location specified by target */

matrix
mat_alloc (int size, int rows, int cols);
/* Allocates space in main memory for a new matrix struct */

matrix
create (double * arr, int rows, int cols);
/* Creates a matrix from a specified array and shape */

matrix
matcpy (matrix m);
/* Returns a copy of a matrix */

void
reshape (matrix * m, int rows, int cols);
/* Changes the shape (rows and columns) of an existing matrix. Must be
   a legal reshaping (e.g. (4,3) --> (6,2) is fine but (4,3) --> (4,2) is not */

matrix
fill ( int rows, int cols, double val );
/* Initialize a matrix of shape (rows, cols) filled with val at each entry */

matrix
zeros ( int rows, int cols );
/* Initializes a zero matrix of shape (rows, cols) */

matrix
eye ( int n );
/* Initializes an n by n identity matrix */

matrix
ones ( int rows, int cols );
/* Initialize a matrix of ones of shape (rows, cols) */

void 
transpose (matrix * m);
/* Transposes an existing matrix. That is, if M has shape (m,n) initially,
   after transposing M has shape (n,m) and the (i,j)th element of M 
   corresponds to the (j,i)th element of the old M */

matrix
add (matrix m, matrix n);
/* Returns a new matrix formed by the sum of m and n. Checks that the
   matrices have the same shape before proceeding */

matrix
scale (matrix m, double c);
/* Returns a new matrix formed by scaling the matrix m by the value c */

void
print (matrix m);
/* Prints a matrix in human-readable form. */

double
element (matrix m, int i, int j);
/* Returns the element corresponding to m_(i,j) */

void
set_element (matrix * m, int i, int j, double val);
/* Sets the value of m_(i,j) to val (side effects) */

matrix
dot ( matrix A, matrix B);
/* Returns a matrix formed by the marix product AB.
   INCOMPLETE, NOT PROPERLY IMPLEMENTED. */

double
vdot (double * u, double * v, int n);
/* Returns the vector dot product of u and v (up to n) terms,
   does not check if size of u or v are at least n, so can cause
   unknown behavior. */

#endif // MAT_INTERNALS_H_
