CC = gcc
CFLAGS = -Wall


all:  randomTest 

randomTest: randomMath.o randomTest.o
	$(CC) -o randomTest randomTest.o randomMath.o

randomTest.o:  randomMath.c randomMath.h
	$(CC) $(CFLAGS) -c randomTest.c

randomMath.o:  randomMath.c randomMath.h
	$(CC) $(CFLAGS) -c randomMath.c
	
clean:
	rm -f *.o *~ core*
