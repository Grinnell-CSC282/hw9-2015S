supp7.c is a c file of an exercise done in CSC161.

The make file has 2 commands, all and clean. all runs when "make" is
inputed into the terminal, and compiles an object for supp7.c that runs the
program. clean removes the object file that make creates. There is an error
message that pops up when running clean:

$ make clean
rm *.o supp7
rm: cannot remove `*.o': No such file or directory
make: *** [clean] Error 1

While this error appears, supp7.o is still removed.
