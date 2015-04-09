#Makefile for the clinic problem
#John Brady, April 9 2015
#CSC282 HW 9, Rebelsky

CC = /usr/bin/gcc
FLAGS = -g -Wall

#Compile clinic.c into an object file
clinic.o:
	$(CC) $(FLAGS) $(DEFINES) -c clinic.c
#Compile clinic.c into an executable
clinic:
	$(CC) $(FLAGS) $(DEFINES) -o clinic -lpthread -lrt clinic.c

#Clean directory, removing compiled code, object files, and temp files
clean:
	rm -f *.o *~ clinic