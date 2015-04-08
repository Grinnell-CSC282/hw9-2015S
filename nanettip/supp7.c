

    /*****************************************************************
     * Daniel Nanetti-Palacios                                       *
     * Box:  4426                                                    *
     * Supplemental Problem 7 for CSC 161                            *
     *****************************************************************/

    /* ***************************************************************
     * Academic honesty certification:                               *
     *   Written/online sources used:                                *
     *     None                                                      *
     *   Help obtained                                               *
     *     None                                                      *
     *   My signature below confirms that the above list of sources  *
     *   is complete AND that I have not talked to anyone else       *
     *   [e.g., CSC 161 students] about the solution to this problem *
     *                                                               *
     *   Signature:                                                  *
     *****************************************************************/

#include <string.h>
#include <stdio.h>

int main()
{// Common variables used for loops
  int i, j, k;

  /* array that contains the strings for all numbers in the first digit, not
     including "one" */
  char *singledig[9] = {"eight", "five", "four", "nine", "seven", "six", "three", "two", "zero"};

  /* array that contains the strings for all numbers between 10 and 20.*/
  char *tens[8] = {"eighteen", "fifteen", "fourteen", "nineteen", "seventeen", "sixteen", "thirteen", "twelve"};

  /* array that contains the strings for all numbers in the second digit,
     not including "ten" */
  char *doubledig[8] = {"eighty", "fifty", "fourty", "ninety", "seventy", "sixty", "thirty", "twenty"};

  char *hundreds[2] = {"one hundred", "two hundred"};

  /* One, Ten, and Eleven are separated from the rest because they act
     differently than the rest. */
  char *ones[3] = {"one", "ten", "eleven"};

  // This loop prints out the outcome of all numbers in alphabetical order.
  for (i = 0; i < 8; i++)
    {
      /* Since Two, Twenty, and Two Hundred are slightly different than the rest
         of the numbers, they change up the standard loop. */
      if (i == 7)
        {
          printf("%s\n", tens[i]);
          for (j = 0; j < 8; j++)
            {
              printf("%s ", doubledig[i]);
              printf("%s\n", singledig[j]);
            }
          printf("%s\n", singledig[i]);
          printf("%s\n", hundreds[1]);
        }
      else
      {
        /* Basic loop for all numbers, where it prints the original number,
           the number multiplied by ten, and every combination of a tens number
           with a single digit number. */
          printf("%s\n", singledig[i]);
          printf("%s\n", tens[i]);
          for (j = 0; j < 8; j++)
            {
              printf("%s ", doubledig[i]);
              printf("%s\n", singledig[j]);
            }
          if (i == 0)
            {
              // This prints Eleven at its proper location.
              printf("%s\n", ones[2]);
            }
          if (i == 3)
            {
              /* This loop is responsible for printing One, One hundred,
                 and every value between 100 and 200. The hundreds loop is very
                 similar to the overall loop so it can repeat all the values
                 over again. */
              printf("%s\n", ones[0]);
              printf("%s\n", hundreds[0]);
              for (k = 0; k < 8; k++)
                {
                  if (k == 7)
                    {
                      printf("%s ", hundreds[0]);
                      printf("%s\n", tens[k]);
                      for (j = 0; j < 8; j++)
                        {
                          printf("%s ", hundreds[0]);
                          printf("%s ", doubledig[k]);
                          printf("%s\n", singledig[j]);
                        }
                      printf("%s ", hundreds[0]);
                      printf("%s\n", singledig[k]);
                    }
                  else
                    {
                      printf("%s ", hundreds[0]);
                      printf("%s\n", singledig[k]);
                      printf("%s ", hundreds[0]);
                      printf("%s\n", tens[k]);
                      for (j = 0; j < 8; j++)
                        {
                          printf("%s ", hundreds[0]);
                          printf("%s ", doubledig[k]);
                          printf("%s\n", singledig[j]);
                        }
                      if (k == 0)
                        {
                          printf("%s ", hundreds[0]);
                          printf("%s\n", ones[2]);
                        }
                      if (k == 3)
                        {
                          printf("%s ", hundreds[0]);
                          printf("%s\n", ones[0]);
                        }
                      if (k == 5)
                        {
                          printf("%s ", hundreds[0]);
                          printf("%s\n", ones [1]);
                        }
                    }
                }
            }
          if (i == 5)
            {
              /*This value prints out Ten since it does not follow the laws of
                the main function. */
              printf("%s\n", ones [1]);
            }
      }
    }
  // Zero is the last number in this order, so it is printed outside the loop.
  printf("%s\n", singledig[8]);
}
