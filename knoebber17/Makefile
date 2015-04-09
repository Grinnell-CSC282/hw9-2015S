default: main


main: server.o client.o devurandom.o main.c display_bytes.o
	gcc -o com main.c server.o client.o devurandom.o display_bytes.o  -lnacl


server.o: server.h  server.c
	gcc -c server.c -I/usr/include/nacl

client.o: client.h client.c
	gcc -c client.c -I/usr/include/nacl 

devurandom.o: devurandom.h devurandom.c
	gcc -c devurandom.c

display_bytes.o: display_bytes.h display_bytes.c
	gcc -c display_bytes.c


clean:
	rm -f *.o *~ *.txt
