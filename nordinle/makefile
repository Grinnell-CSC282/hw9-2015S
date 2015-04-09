# looked at sam's makefile http://www.cs.grinnell.edu/~rebelsky/Courses/CSC282/2015S/examples/make/gcd/Makefile
make: driver.c server.c client.c devurandom.c
	gcc -o driver driver.c server.c client.c devurandom.c -I/usr/include/nacl -lnacl && ./driver

CFLAGS = -g -Wall
CC = /usr/bin/gcc

default: driver

driver: driver server.o client.o devurandom.o
	$(CC) $(CFLAGS) -o driver driver.o -I/usr/include/nacl -lnacl
driver.o: driver.c
	$(CC) $(CFLAGS) -c -o driver.o driver.c
client.o: client.c
	$(CC) $(CFLAGS) -c -o client.o client.c
server.o: server.c
	$(CC) $(CFLAGS) -c -o server.o server.c

clean:
	rm -f *.o driver 


test: driver
	./driver
