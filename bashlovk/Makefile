CFLAGS = -std=c99
CC = /usr/bin/gcc

default: cipher

cipher: cipher.o
	$(CC) $(CFLAGS) -o cipher cipher.o
cipher.o: cipher.c
	$(CC) $(CFLAGS) -c -o cipher.o cipher.c
clean: 
	rm -f *.o cipher
