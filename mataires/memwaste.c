/*
Shaun Mataire
Valgrin Example
citation:
	http://valgrind.org/docs/manual
*/

#include <stdlib.h>

void memwaste(){
	int* mem = malloc(10 * sizeof(int));
	// x is not freed, so there is a bab ad memory leak
	mem[10] = 1;

}

int main(){
	memwaste();
	
	return 0;
}
