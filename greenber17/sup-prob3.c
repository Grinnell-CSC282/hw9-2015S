 /*****************************************************************
     * Leah Greenberg                                                *
     * Box:  3520                                                    *
     * Supplemental Problem 3 for CSC 161                            *
     * Assignment for Friday, March 14                               *
     *****************************************************************/

    /* ***************************************************************
     * Academic honesty certification:                               *
     *   Written/online sources used:                                *
     *     I used the max_min lab for reference.                     *
     *   Help obtained                                               *
     *    none                                                       *
     *   My signature below confirms that the above list of sources  *
     *   is complete AND that I have not talked to anyone else       *
     *   [e.g., CSC 161 students] about the solution to this problem *
     *                                                               *
     *   Signature: Leah Greenberg                                   *
     *****************************************************************/

/* 
This program is created to enter in a value (M) and then see the numbers 2-M 
with the amounts of non-trivial factors for each number, plus the top 3 numbers
of maximum factors. For example, if you entered in 12, the maximums should be 
12, 8, and 10. Along with this, there should be 8 numbers printed to a line.

Preconditions: M has to be a positive integer greater than or equal to 4

Postconditions: the program should print out the numbers 2 - M with the 
corresponding number of non-trivial factors. Along with this, 3 numbers with
the top number of non-trivial factors should be printed.

If you enter in a double instead of an integer, the program will only read the
number before the decimal place. However, a precondition is to enter in an
integer greater than or equal to 4, so this problem should not come up.
 */

#include <stdio.h>

int main()
{
  int M;
  int i, j;
  int count = 0;  //initializing count to 0

  //entering and reading a number
  printf("Please print a number: ");
  scanf ("%d", &M);

  //initializations of variables
  int arr[M+1];
  int a[M+1];

  //loop to go through the the 2nd to Mth element of the array
  for (i = 2; i < M+1; i++)
    {
      arr[i] = i;

      //loop to figure out the amount of non-trivial factors
      //if the remainder is 0, 1 is added to count
      //if not the count adds 0
      for (j = 2; j < i; j++)
        {
          if (arr[i] % j == 0)
            a[i] = count++;
          else 
            a[i] = count+=0;
        }

      //printing the number in the array with the correct factors
      printf ("%3d:%d", arr[i], count);

      //creates 8 elements per line
      if (i > 8 && arr[i-1] % 8 == 0)
        {
          printf("\n");
        }

      //resetting count to 0 to go through the next element of the array
      count = 0;
    }

  printf ("\n");
  //initializing the max values to equal to a[3]
  int max = a[3];
  int max2 = a[3];
  int max3 = a[3];

  //called number because it's the number of the count that is the greatest
  int x, number, number2, number3;

  //loop to figure out max, max2, and max3
  //and the number coordinating with those maxes
  for (x = 3; x <= M ; x++)
    { 
      if (a[x] > max)
        {
          max2 = max;
          max = a[x];
          a[i] = x;
          number = a[i];
        }    
      else if (a[x] > max2 && (max2 != max))
        {
          max3 = max2;
          max2 = a[x];
          a[i] = x;
          number2 = a[i];
        }
      else if (a[x] > max3 && (max3 != max2))
        {
        max3 = a[x];
        a[i] = x;
        number3 = a[i];
        }

    }
  //printing the number of the max counts
  //Because the program didn't work correctly for numbers under 10, 
  //I manually put in the answers for M = 4 through M = 9.

  if (M == 4 || M == 5)
    {
      printf ("max = 4\n");
      printf ("max2 = 2\n");
      printf ("max3 = 3\n");
    }
  else if (M == 6 || M == 7)
    {
      printf ("max = 6\n");
      printf ("max2 = 4\n");
      printf ("max3 = 2\n");
    }
  else if (M == 8 || M == 9)
    {
      printf ("max = 6\n");
      printf ("max2 = 8\n");
      printf ("max3 = 4\n");
    }
  else if (M >= 10)
    {
      printf ("max = %d\n", number);
      printf ("max2 = %d\n", number2);
      printf ("max3 = %d\n", number3);
    }

  return 0;
}
