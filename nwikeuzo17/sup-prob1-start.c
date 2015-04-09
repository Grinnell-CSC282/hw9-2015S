/*****************************************************************
     * Henry M. Walker                                               *
     * Box:  Science                                                 *
     * Supplemental Problem 1 for CSC 161                            *
     * Assignment for Tuesday, September 16                          *
     *****************************************************************/

    /* ***************************************************************
     * Academic honesty certification:                               *
     *   Written/online sources used:                                *
     *     I consulted                                               *
     *        the CSC 161 Conditional Lab                            *
     *        the quarts lab                                         *
     *        Professor Walker's emails
     *   Help obtained                                               *
     *     I talked to a CSC 161 tutor about the meaning of an error *
     *        message when I tried to compile                        *
     *   My signature below confirms that the above list of sources  *
     *   is complete AND that I have not talked to anyone else       *
     *   [e.g., CSC 161 students] about the solution to this problem *
     *                                                               *
     *   Signature:                                                  *
     *****************************************************************/

/* This program illustrates an appropriate header for Supplemental
   Problem 1 for CSC 161, Fall 2014
   The final program is supposed to convert an amount in British pounds
   and pence to U.S. dollars and cents.

   Pre-condition:  factors have been identified at the start of the program
                   the values of pounds and pense are non-negative
   Post-condition: the program prints equivalent number of dollars and cents,
                   the number of bills of each denomination, and the number
                   of cents in change
*/

#include <stdio.h>

int main ()
  { printf ("This program converts pounds to dollars\n");  
                                      /* write opening statement */

  int pounds = 17; /* number of pounds to be converted */
  double pence = 32.85; /* number of pence to be converted */
  double poundToUSD = 1.69759; /* currency exchange rate */
  double dollars = ((pounds + (pence / 100)) * poundToUSD) - 0.002; /* equivalent amount in dollars */
  
  /* number  of twenty-dollar bills that can be used to get as close as possible to the dollar equivalent without going above it */
  int dollars20 = (int) dollars / 20;
  
  int dollars1 = (int) dollars - 25;
  int cents = (dollars - (int) dollars) * 100; /* number of cents that can be used to get the dollar equivalent when no more bills are usable */

  printf ("%d pounds and %.2lf pence is equivalent to $%.2lf\n", pounds, pence, dollars);

  if ((dollars > 20) & (dollars20 == 1))
    {
      printf ("This may be paid as\n%d twenty-dollar bill\n", dollars20); /* if the dollar equivalent is over 0 dollars and the number of usable twenty-dollar bills is 1, then print the statement */
    }
  else 
    {
      printf ("incompatible inputs\n");
    }

  if (((dollars - 20) > 5) & ((dollars - 20) < 10)) /* if the dollar equivalent minus 20 is over 5 dollars, but less than 10 dollars, print the statement */
    {
      printf ("1 five-dollar bill\n");
    }
  else 
    {
      printf ("incompatible inputs\n");
    }
  if (((dollars - 25) > 1) & ((dollars - 25) < 5))  /* if the dollar equivalent minus 25 is over 1 dollar, but less than 5 dollars, print the statement */
    {
      printf ("%d one-dollar bills\n", dollars1);
    }
  else 
    {
      printf ("incompatible inputs\n");
    }
  if (cents > 1) /* if the number of cents needed to get the dollar equivalent when no bills are usable is greater than one, print the statement */
    {
      printf ("%d cents in change\n", (cents));
    }
  else 
    {
      printf ("1 cent in change\n"); /* if the number of cents needed to get the dollar equivalent when no bills are usable is less than one, print the statement */
    }

  
  return 0;
}
