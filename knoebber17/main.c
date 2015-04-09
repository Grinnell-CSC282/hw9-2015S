#include <stdio.h>
#include "client.h"
#include "server.h"
/*
 Run the client server roleplay. 
 Note the user inputs for both parties.
 
 By Nicolas Knoebber and Erin Callaway
 */
int main()
{
  server_init();
  
  unsigned char server_pk[32];
  unsigned char init_sk[32];
  unsigned char nonce0[24];

  get_server_keys(server_pk,init_sk);
  getN0(nonce0);
  c_send_verif(server_pk,init_sk,nonce0);
  s_recieve_verif();
  c_recieve_verif(server_pk);
  ask_question(server_pk);
  answer_question();
  recieve_answer(server_pk);
  return 0;
}
