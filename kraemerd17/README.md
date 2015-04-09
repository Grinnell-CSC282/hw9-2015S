#### Overview

One long term project I've put some time into is building a matrix computation library in `C`. There are plenty of good implementations that already exist, but I thought it would be good practice. It's largely incomplete, but the foundational components are for the most part functional.

#### Data structure model

Matrices are their own `struct` in this library, given by the following fields:

```C
typedef struct
{
  double * arr;
  int size;
  int rows;
  int cols;
} matrix;
```

The underlying data that make up the elements of the matrix are stored in a single, one-dimensional array. This I've found to be pretty versatile with the various procedures I'm trying to build around matrices. In my experience passing multi-dimensional arrays as function arguments has proven unwieldy, so instead I just keep track of the dimensions of the matrix separately while leaving the internals alone.

It gets tricky when implementing commonplace procedures such as `transpose`, because the resultant matrix needs to have a `double` array with the same elements as the original, but in a very specific permutation. After a considerable amount of scribbling, I'm fairly convinced that `transpose` is properly implemented. I'm much more confident about simpler procedures such as `add` and `scale`, which aren't as conceptually difficult.

Matrix multiplication is "implemented", but I know there are bugs. The biggest challenge in computing matrix products when the data are stored in a 1D array is accessing column vectors. One approach that I have not yet considered is using `transpose` to reduce the data slicing problem to one that I've already solved.

Check `matrix.h` for descriptions of all of the implemented methods.

#### Makefile stuff

Right now, I have set my `makefile` to build each of the `.c` files as well as execute my test file. Nothing fancy, but it has sped up my workflow considerably.



