mat_internals:
	gcc -c mat_internals.c

matarith: mat_internals.o
	gcc -c mat_arith.c mat_internals.o

matmult: mat_internals.o mat_arith.o
	gcc -c mat_mult.c mat_arith.o mat_internals.o

test: mat.o mat_internals.o mat_mult.o
	gcc -o mat_test mat_test.c mat_arith.o mat_internals.o mat_mult.o && ./mat_test
