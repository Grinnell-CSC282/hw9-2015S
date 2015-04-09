/* Ezra Edgerton and John Brady
 * Boxes 3503 and 3119
 * CSC213 Lab 7: Synchronization, Weinman
 * clinic.c - program that implements the psuedocode in our written argument 
 * in order to show correct output, which is done by printing to the terminal
 * Acknowledgements: We used the POSIX Threads Programming for assistance in 
 * creating our main procedure, which creates all the threads. We also 
 * used "http://www.csc.villanova.edu/~mdamian/threads/posixsem.html" to 
 *find the compiling flags that work with semaphores.
 *Revised by John Brady, April 9 2015
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <semaphore.h>
#include <unistd.h>

//create semaphores
sem_t Access;/* semaphore that will allow or prevent anyone to enter, 
and prevents patients from leaving. Also used as a mutex to prevent 
concurrent alterations of variables
*/

sem_t allReg;// barrier that will be incremented by the doctor once all are
//registered
sem_t patReg;// barrier that makes sure all patients are registered before the
//doctor begins treatment

sem_t lock;// mutex variable that acts as a lock


//create variables
int entered = 1;//number of patients entered into the clinic
int registered = 0;//number of entered patients that are registered
int voucher = 0;// number of vouchers available to treated patients

//int to keep main from returning and ending program
int test = 0;


void *doctor(void *threadid)
{
    long tid;
    tid = (long)threadid;
    //   printf("doc %d enters\n", tid);
    while (1)
    {
        printf("doctor %d queues to enter\n", tid);
        //keep anyone else from entering and patients from leaving
        sem_wait(&Access);
        //enters
        printf("doc %d enters\n", tid);
        /*What if we have no patients? Then there is nobody to treat 
        or give vouchers to, so the doctor can skip these steps and leave.
        */
        if (entered != 1)//wait until all patients are registered
        {
            sem_wait(&allReg);
            //now we are here, all patients in the clinic are registered. 
            //TREAT!
            //make some vouchers for the number of patients treated
            printf("doctor %d treats %d patients\n", tid, registered);
            voucher = registered;
            //open the barrier that allows patients to get vouchers
            sem_post(&patReg);
        }//if
        //patients treated, vouchers created. Doc can now leave.
        printf("doctor %d leaves\n", tid);
        //open the door and allow anybody to enter or leave
        sem_post(&Access);
        //sleep for random time
        usleep( random() % (long)1e6 ); // Sleep up to one second
    }//while
}//void *doctor


void *patient(void *threadid)
{
    long tid;
    tid = (long)threadid;
    //  printf("pat %d enters\n", tid);
    while (1)
    {
        //sleep for random time
        usleep( random() % (long)1e6 ); // Sleep up to one second
        printf("patient %d queues to enter\n", tid);
        //turnstile that increments the value of entered as patients enter
        sem_wait(&Access);
        entered--;
        printf("patient %d enters, %d entered\n", tid, entered);
        sem_post(&Access);
        /*sleeps for random amount of time to seperate entering and 
         registering steps, makes code correctness more evident.
         */
        usleep( random() % (long)1e6 ); // Sleep up to one second
        //atomic
        sem_wait(&lock);
        //registering and checking atomic so only one post to allReg happens
        registered++;
        printf("patient %d registers, %d/%d registered\n", tid, 
          registered, entered);
        if (entered == registered)
        {
            sem_post(&allReg); //if we are ready for treatment, call doctor
        }//if
        sem_post(&lock);
        // end atomic

        //patient goes back to sleep while there are no vouchers to take.
        while (voucher < 3)
        {
            usleep( random() % (long)1e6 ); // Sleep up to two seconds
        }//while

        sem_wait(&patReg);//patient is registered, waits to receive voucher
        voucher--; // now has voucher after treatment
        printf("patient %d gets voucher, vouchers: %d\n", tid, voucher);
        sem_post(&patReg);//wakes up another patient to get voucher

        // ready to leave, voucher in hand. Check for doctor. If yes, sleep
        // atomic function/turnstile that makes the patient leave the clinic.
        sem_wait(&Access);
        printf("patient %d leaves\n", tid);
        entered--;
        registered--;
        sem_post(&Access);
    }//while
}//void *patient

void *observer(void *threadid)
{
    long tid;
    tid = (long)threadid;
    while (1)
    {
        //sleep for random time
        usleep( random() % (long)2e6 ); // Sleep up to two seconds
        printf("observer %d queues to enter\n", tid);
        sem_wait(&Access);//can observers enter? if not, sleep until they can
        printf("observer %d enters\n", tid);
        sem_post(&Access);
        usleep( random() % (long)2e6 ); // Sleep up to two seconds
        printf("observer %d leaves\n", tid);
    }//while
}//void *observer


/**
Procedure: Main
Preconditions: input of form int numDoctors, int numPatients, int numObservers
with all arguments greater than 0.
Postconditions: output stream prints endless lines of output in accordance 
to the rules set forth in task details of Jerod Weinman's Lab 7.
Threads remain unjoined because they never complete execution.
Acknowledgements:
thread creation used concepts from the POSIX Thread Programming
**/

int main (int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "incorrect inputs. Correct usage: clinic 4 30 10\n");
        return 0;
    }

    //Initialization of semaphores
    sem_init(&Access, 1, 1);
    sem_init(&allReg, 1, 0);
    sem_init(&patReg, 1, 0);
    sem_init(&lock, 1, 1);

    int i;
    int rc;
    //initialization of numbers of people
    int numDocs =  atoi(argv[1]);
    int numPats = atoi(argv[2]);
    int numObs = atoi(argv[3]);

    if (numDocs < 2 || numPats < 1 || numObs < 1)
    {
        fprintf(stderr, "please use sizes of > 0\n");
        return 0;
    }

    //arrays to make different people threads
    pthread_t docThreads[numDocs];
    pthread_t patThreads[numPats];
    pthread_t obsThreads[numObs];

    //arrays to pass in person id numbers
    int *docTaskIds[numDocs];
    int *patTaskIds[numPats];
    int *obsTaskIds[numObs];

    //for loop to create Doctor threads
    for (i = 0; i < numDocs; i++)
    {
        docTaskIds[i] = (int *) malloc(sizeof(int));
        docTaskIds[i] = i;
        fprintf(stdout, "Creating doctor %d\n", docTaskIds[i]);
        //creates thread
        rc = pthread_create(&docThreads[i], NULL, doctor, 
          (void *)docTaskIds[i]);
        if (rc)//error handling
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }//for
    //for loop to create Patient threads
    for (i = 0; i < numPats; i++)
    {
        patTaskIds[i] = (int *) malloc(sizeof(int));
        patTaskIds[i] = i;
        fprintf(stdout, "Creating patient %d\n", patTaskIds[i]);
        //creates thread
        rc = pthread_create(&patThreads[i], NULL, patient, 
          (void *)patTaskIds[i]);
        if (rc)//error handling
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }//for
    //for loop to create Observer threads
    for (i = 0; i < numObs; i++)
    {
        obsTaskIds[i] = (int *) malloc(sizeof(int));
        obsTaskIds[i] = i;
        fprintf(stdout, "Creating observer %d\n", obsTaskIds[i]);
        //creates thread
        rc = pthread_create(&obsThreads[i], NULL, observer, 
          (void *)obsTaskIds[i]);
        if (rc)//error handling
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }//for
    //while loop to prevent main from returning and ending the program run.
    while (1)
    {
        test = 1;//thing to give while loop content
    }//while
    return 0;
}//main
