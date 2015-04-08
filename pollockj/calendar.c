  /***********************************************************************
     * Name(s)  Wesley Pollock                                             *
     * Box(s):  4507                                                       *
     * Supplemental Problem 4: A Daily Calendar Program                    *
     *                                                                     *
     * Assignment for November 21, 2014                                    *
     ***********************************************************************/

    /* *********************************************************************
     * Academic honesty certification:                                     *
     *   Written/online sources used:                                      *
     *     "array-list.c" from lab on Tuesday, November 4th for an example *
     *        of using malloc to allocate more space for arrays            *
     *      "Insertion Sort" reading from lab on Wednesday, October 29th   *
     *        for an example of using insertion sort in C code             *
     *                                                                     *
     *   Help obtained                                                     *
     *     none                                                            *
     *                                                                     *
     *   My/our signature(s) below confirms that the above list of sources *
     *   is complete AND that I/we have not talked to anyone else          *
     *   (e.g., CSC 161 students) about the solution to this problem       *
     *                                                                     *
     *   Signature:                                                        *
     ***********************************************************************/

//Compile and run with
//clear && gcc -o c4 calendar4.c && c4

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*The max length of the string used in an appointment's description*/
const int maxLen = 80;

/* A struct for an appointment with a date (day,month,year) and text */
typedef struct {
  int year;
  int month; /* 1=January, 2=February, ..., 12=December */
  int day;
  char text[80];
} appt_t;

void addAppt(appt_t * calArr[], appt_t newAppt, int * apptCount,  int * size);
/*
Pre-conditions :
   calArr is a pointer to the array of appointments (array of type appt_t)
   newAppt is the appointment to be added (type appt_t)
   appCount a pointer to the total number of appointments (apptCount >= 0)
   size a pointer to the max length of the array of appointments (size > 0)
Post-conditions:
   addAppt adds the a new appointment (newAppt) to the array of appointments 
   (calArr) and increaments the total number of appointments so far (appCount).
   If the number of appointments equals the size of the array of appointments,
   addAppt doubles the size of the array of appointments to make room for the 
   new appointment. 
*/

int comesFirstByDate (appt_t app1, appt_t app2);
/* 
Pre-conditions :
   app1 and app2 are the appointments to be compared with valid month, day,
   and year values (type appt_t)
Post-conditions:
   returns 1 if the date for appointment app1 comes before the date for app2, 
   and returns 0 otherwise 
*/

int comesFirstByText (appt_t app1, appt_t app2);
/* 
Pre-conditions :
   app1 and app2 are the appointments to be compared with valid text values
   (type appt_t)
Post-conditions:
   returns 1 if the text for appointment app1 comes before the 
   date for app2, and returns 0 otherwise 
*/

void insertionSort(appt_t * calArr[], int apptCount, 
                   int comesFirst(appt_t app1 , appt_t app2));

/*
Pre-conditions :
   calArr is a pointer to the array of appointments (array of type appt_t)
   appCount is the total number of appointments (apptCount >= 0)
   comesFirst is a procedure which compares to appointments and returns if app1
      is before app2 and returns 0 otherwise.
Post-conditions:
   Uses insertion sort to sort the array of appointments (calArr) with some 
   function (comesFirst) to determine which appointment goes before the other. 
*/

void printAppt(appt_t calArr[], int apptCount);
/*
Pre-conditions :
   calArr is the array of appointments (array of type appt_t)
   appCount is the total number of appointments (apptCount >= 0)
Post-conditions:
   printAppt prints all the appointments in the current array of appointments. 
   It follows the format: "day/month/year: Text"
*/

int main ()
/*
Pre-conditions :
   appCount is the total number of appointments (must be initialize to zero)
   maxSize is the starting maximum size of the appointment array (maxSize > 0)
   option is the user chosen option (must be a character)
Post-conditions:
   main initializes the array of appointments, maxSize, and apptCount then 
   prints a list of options for the user. The user can select an option and main
   calls the procedure associated with the user chosen option.
*/
{
  char option;
  int i;
  int apptCount = 0;
  appt_t * calendar;
  int maxSize = 1;
  calendar = malloc(maxSize * sizeof(appt_t));
  char validMonthsArr[12][10]= {"January","Febuary","March","April","May","June"
                                ,"July", "August","September","October",
                                "November", "December"};

  while (1)
    {
      /*Menu of options*/
      printf("n: add a new appointment (date, text)\n");
      printf("d: sort appointments by date\n");
      printf("a: sort appointments alphabetically by appointment text\n");
      printf("p: print current appointments\n");
      printf("q: quit\n");

      /* Gets user input */
      printf("Enter option: ");
      option = getchar();
      while(getchar()!='\n');
      printf("User option is %c\n",option);
      /*Checks which option the user entered */

      /*If user inputs to add a new appointment*/
      if (option == 'n')
        {
          char month[20];
          appt_t newAppt;
          printf("Enter month: ");
          char ch  = getchar();
          int charCount = 0;
          while (ch != '\n')
            {
              month[charCount] = ch;
              charCount++;
              ch = getchar();
            }
          month[charCount] = 0;
          int validMonth = 0;
          for (i=0; i<12; i++)
            {
              if(!strncmp(month,validMonthsArr[i],10))
                {
                newAppt.month = i+1;
                validMonth = 1;
                }
            }
          if (validMonth)
            {
              printf("Enter day and year (day,year): ");
              scanf("%d,%d", &newAppt.day, &newAppt.year);
              while(getchar()!='\n');
              printf("Enter text: ");
              charCount = 0;
              ch = getchar();
              while (ch != '\n')
                {
                  newAppt.text[charCount] = ch;
                  charCount++;
                  ch = getchar();
                }
              newAppt.text[charCount] = 0;          
              /*printf("Text: %s\n",newAppt.text);
              printf("%s %d, %d", month, newAppt.day, newAppt.year);*/
              addAppt(&calendar, newAppt, &apptCount, &maxSize);
            }
          else
            printf("Invalid month name\n");
        }

      /*If user inputs to sort by date*/
      else if (option == 'd')
        insertionSort(&calendar,apptCount, comesFirstByDate);
        //dateSort(&calendar,apptCount);

      /*If user inputs to sort by text*/
      else if (option == 'a')
        insertionSort(&calendar,apptCount, comesFirstByText);
          //alphaSort(&calendar,apptCount);

      /*If user inputs to print*/
      else if (option == 'p')
          printAppt(calendar,apptCount);

      /*If user inputs to quit*/
      else if (option == 'q')
        {
          printf("Quitting program\n\n");
          return 0;
        }
      else
        printf("Command not recongized\n");

    }
}

void addAppt(appt_t * calArr[], appt_t newAppt, int * apptCount, int * size)
{
  /*Expand the array if necessary */
  if (*apptCount == *size)
    {
      appt_t * newArr;
      /* Doubles the size of the array */
      newArr  = malloc (2 * (*size) * sizeof(appt_t));
      int i;
      for (i=0; i <= *apptCount; i++)
        newArr[i] = (*calArr)[i];
      free (*calArr);
      *calArr = newArr;
      *size *= 2;
    }
  /* Add new appointment to array and increment the appointment count*/
  (*calArr)[*apptCount] = newAppt;
  (*apptCount)++;
}


int combineDate(appt_t appt)
/*
Pre-conditions :
   appt is the appointment to find the combined date of (type appt_t)
Post-conditions:
   returns the sum of year*10000, month*100, and day of the given appointment
   (appt). With the combinedDate various dates of appointments can be compared 
   with the correct weight on years, months, and days.
*/
{
  int combinedDate = (appt.year*10000+
                      appt.month*100+
                      appt.day);
  return combinedDate;
}

int comesFirstByDate (appt_t app1, appt_t app2)
{
  return (combineDate(app1) < combineDate(app2));
}

int comesFirstByText (appt_t app1, appt_t app2)
{
  if (strncmp(app1.text, app2.text, maxLen) < 0)
    return 1;
  else
    return 0;
}

void insertionSort(appt_t * calArr[], int apptCount, 
                   int comesFirst(appt_t app1 , appt_t app2))
{
  int k;
  for (k =1; k < apptCount; k++)
    {
      appt_t appt_K = (*calArr)[k];
      int i = k-1;
      while ((i>= 0) && comesFirst(appt_K,(*calArr)[i]))
        {
          (*calArr)[i+1] = (*calArr)[i];
          i--;
        }
      (*calArr)[i+1]= appt_K;
    }
}

void printAppt(appt_t calArr[], int apptCount)
{
  char months[12][10]= {"January","Febuary","March","April","May","June",
                        "July", "August","September","October", "November", 
                        "December"};
  int i;
  for (i=0; i< apptCount; i++)
    {
      printf("%10s %2.0d, %4.0d: %s\n",months[calArr[i].month-1], calArr[i].day,
                                       calArr[i].year, calArr[i].text);
    }
}

