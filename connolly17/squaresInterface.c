#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main ()
{
  char x;
  int squareIt;
  while (1){
    printf("Enter C to continue or Q to quit.\n");
    //x = getchar();
    scanf("%c", &x);
    switch(x)
      {
      case 'c':
      case 'C':
        printf("Value of squareIt is %d\n", squareIt);
        printf("Enter a value to compute\n");
        //squareIt = getchar();
        scanf("%d", &squareIt);
        printf("Square not yet implemented but you entered %d\n", squareIt);
        break;
      case 'q':
      case 'Q':
        return 0;
        break;
      default: printf("Invalid option, enter C to continue or Q to quit");
        continue;
      }
  }
  return 0;
}
