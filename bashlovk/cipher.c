    /*****************************************************************
     * Vasilisa Bashlovkina                                          *
     * Box:  3077                                                    *
     * Supplemental Problem 3 for CSC 161                            *
     * Assignment for Friday, November 1st                           *
     *****************************************************************/

    /* ***************************************************************
     * Academic honesty certification:                               *
     *   Written/online sources used:                                *
     *    Program Correctness and Program Testing                    *
     *    created 18 May 2008 by Henry M. Walker                     *
     *    last revised 7 February 2010                               *
     *    getchar-line-example.c                                     *
     *    from Laboratory Exercise on Basic I/O                      *
     *   Help obtained:                                              *
     *    none                                                       *
     *   My signature below confirms that the above list of sources  *
     *   is complete AND that I have not talked to anyone else       *
     *   [e.g., CSC 161 students] about the solution to this problem *
     *                                                               *
     *   Signature:                                                  *
     *****************************************************************/

/* This program has two modes: ecryption or decryption. Depending on the mode
specified by the user it encrypts or decrypts a message based on provided 
cipher alphabet and a single keyword. The encrypted or decrypted message is then
printed out.
 
 * Pre-conditions:  message is a single line of less than 100 characters
 *                  cipher alphabet is a string of 26 letter characters
 *                  keyword is a string of less than 30 letter characters
 *                  mode is 1 for encryption
 *                  mode is 0 for decryption
 *
 * Post-conditions: the program prints the message, encrypted or decrypted
 *                  according to cipher alphabet and keyword.
 */


#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* This function converts all lower case letters in the string to upper case.
Pre-conditions: none
Post-conditions: all letters in the string are now uppercase
 */
void uppercase (char *string)
{
  int i;
  for (i = 0; i < strlen(string); i++)
    {
      if (islower (string[i])) //only lowercase letters are changed
        string[i]-='a'-'A'; 
    }
}

int main()
{
  //Declarations
  char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char message[100];
  char cipher_alphabet[26];
  char keyword[30];
  int shift; // shift of letters in message due to letters in keyword
  int i;     // used in outer for loops
  int j=0;     // used in while loop and inner for loop
  int mode; // specifies whether the program is encrypting or decrypting

  //Prompting, scanning, converting to upper case
  printf ("This program encrypts or decrypts single line messages based on "
          "provided cipher alphabet and a keyword (case-insensitive).\n");
  printf ("Enter the message (100 characters on one line max)\n");
  while ((message[j] = getchar()) != '\n')//scanning all input until new line 
    j++; 
  message[j] = '\0';//terminating the string
  uppercase (message);

  printf ("Enter the cipher alphabet (26 different letters, no spaces)\n");
  scanf ("%s", cipher_alphabet);
  uppercase (cipher_alphabet); 

  printf ("Enter a keyword (30 letters max)\n");
  scanf ("%s", keyword);
  uppercase (keyword);
  int keysize = strlen (keyword);

  printf ("If you would like to encrypt this message, press 1\n"
          "If this message needs to be decrypted, press 0\n");
  scanf ("%d", &mode);
 
  //Encryption or decryption
  for (i = 0; i< strlen(message); i++)
    {
      if (isalpha (message[i]))//non-letters are left alone
        {
          shift = keyword[i % keysize] - 'A';//shift ranges form 0 to 25
          if (mode) //mode = 1 = encryption
            {
              message[i] = 'A' + (message[i] - 'A' + shift) % 26;//shifting
              message[i] = cipher_alphabet[message[i] - 'A'];//encoding
            }//end of encryption
          else //decryption
            {
              for (j = 0; j < 26; j++)// converts back to normal alphabet
                {    
                  if (message[i] == cipher_alphabet[j])
                    { //finds the matching letter in cipher_alphabet            
                      message[i] = alphabet[j];//looks up same index in alphabet
                      break;
                    }
                }//the message is now converted to normal alphabet
              message[i] -= shift;// shifting back using the keyword
              if (message[i] < 'A') //if it's not a capital letter
                message[i] += 26; //"wrap" it back around the alphabet
            }//end of decryption
        }
    }// outer for

  //Printing out the result:
  if (mode)
    printf ("The encrypted ");
  else printf ("The decrypted ");
  printf ("message is %s\n", message);

  return 0;
}//main

