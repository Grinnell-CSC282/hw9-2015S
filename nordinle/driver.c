// driver file
// looked at the man pages for sprintf and sscanf
// for generating a random number
// http://stackoverflow.com/questions/822323/how-to-generate-a-random-number-in-c
//
#include "server.h"
#include "client.h"
#include "crypto_box.h"

#include <stdio.h>

int main(){
	initialize_server();
	initialize_client();
	grab_server_keys();
	client_first();
	server_second();
	client_third();
	server_fourth();
	client_fifth();
	return 0;
}
