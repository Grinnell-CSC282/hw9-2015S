# looked at sam's makefile http://www.cs.grinnell.edu/~rebelsky/Courses/CSC282/2015S/examples/make/gcd/Makefile
CFLAGS = -g -Wall
CC = /usr/bin/gcc
CRYPTO = -I/usr/include/nacl -lnacl

default: driver

driver: driver driver.o server.o client.o devurandom.o
	$(CC) $(CFLAGS) -o driver driver.o server.o client.o devurandom.o $(CRYPTO)
driver.o: driver.c
	$(CC) $(CFLAGS) -c -o driver.o driver.c $(CRYPTO)
client.o: client.c
	$(CC) $(CFLAGS) -c -o client.o client.c $(CRYPTO)
server.o: server.c
	$(CC) $(CFLAGS) -c -o server.o server.c $(CRYPTO)
devurandom.o: devurandom.c
	$(CC) $(CFLAGS) -c -o devurandom.o devurandom.c $(CRYPTO)

clean:
	rm -f *.o driver 
