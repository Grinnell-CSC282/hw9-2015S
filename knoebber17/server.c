#include <stdio.h>
#include <string.h>
#include "crypto_box.h"
#include "devurandom.h"
#include "display_bytes.h"
#include <assert.h>
#include <time.h>
#include "time_union.h"

/*
 Security assigment two
 by Nicolas Knoebber and Erin Callaway
 */

unsigned char nonce0[24];
unsigned char nonce2[24];
unsigned char server_pk[32];
unsigned char server_sk[32];
unsigned char init_pk[32];
unsigned char init_sk[32];
unsigned char client_pk[32];

/*
initialize the server 
 */
void server_init()
{

  randombytes(nonce0,24);
  crypto_box_keypair(server_pk,server_sk);
  crypto_box_keypair(init_pk,init_sk);
}

/*
get the keys the client needs to do the verification
 */
void  get_server_keys(char * pk, char * fsk)
{
  memcpy(pk,server_pk,32);
  memcpy(fsk,init_sk,32);
}

/*
Get the initial nonce
 */
void getN0(char * n)
{
  memcpy(n,nonce0,24);
}

/*
 produce nonce N2
 timestamp T
 concatenate N1, N2, and T
 encrypt the concatenation with the client's public key and the nonce N1
 send the encrypted message and the signature to the client. 
*/

void s_recieve_verif()
{
  printf("------------------------------\n");
  printf("Server: \n");
  printf("------------------------------\n");
  unsigned char nonce1[24];
  int i;
  int success;
  FILE * veri = fopen("verification.txt","r"); // the file that client writes to
  FILE * veri2 = fopen("verification2.txt","wb"); //the file that server will write to
  int len = crypto_box_PUBLICKEYBYTES+crypto_box_NONCEBYTES+crypto_box_ZEROBYTES;
  int response_length=crypto_box_ZEROBYTES+(crypto_box_NONCEBYTES*2)+8; 
  //FILE * clientPK = fopen("client_public.txt","r");
  unsigned char encrypted[len]; //clients encrypted message
  unsigned char decrypted[len]; //clients decrypted message
  unsigned char response[response_length]; //plain text of the response message
  unsigned char e_response[response_length]; //encrypted response message 

  //get time
  T.time_stamp = time(NULL);
  //make new nonce
  randombytes(nonce2,24);
  //read encrypted data from client
  fread(encrypted,len,1,veri);
  //decrypt data 
  success=crypto_box_open(decrypted,encrypted,len,nonce0,init_pk,server_sk);
  assert(success==0);
  printf("Decrypted client verification: \n");
  display_bytes(decrypted,len);

  for(i=0;i<24;i++)   //retrieve encrypted nonce 
    nonce1[i]=decrypted[i+crypto_box_ZEROBYTES];

  for(i=0;i<32;i++)   //get the clients public key 
    client_pk[i]=decrypted[i+24+crypto_box_ZEROBYTES];

  printf("Retrieved clients public key\n");
  fclose(veri);

  /*
    build the response message
  */
  
  for (i = 0; i < crypto_box_ZEROBYTES; i++) //put in zero bytes
    response[i] = 0;

  for(i=0;i<24;i++)   //put n1 into message
    response[i+crypto_box_ZEROBYTES]=nonce1[i];
  for(i=0;i<24;i++)   //put n2 into message
    response[i+24+crypto_box_ZEROBYTES]=nonce2[i];
  for(i=0;i<8;i++) //put time stamp into message
    response[i+48+crypto_box_ZEROBYTES]=T.char_time[i];

  //encrypt this message 
 
  success=crypto_box(e_response,response,response_length,nonce1,client_pk,server_sk);
  printf("Encrypted response for client to verify at %d seconds\n\n",T.time_stamp);
  assert(success==0);
  fwrite(e_response,sizeof(e_response),1,veri2);
  fclose(veri2);
}
/*
Answer the question the client poses and send it back encrypted
 */
void answer_question()
{
  printf("------------------------------\n");
  printf("Server: \n");
  printf("------------------------------\n");
  int i;
  int success;
  int text_len = 64;
  int len = text_len+crypto_box_ZEROBYTES+(crypto_box_NONCEBYTES*2);
  int response_len = text_len+crypto_box_ZEROBYTES+crypto_box_NONCEBYTES;
  char question_text[text_len];
  char answer_text[text_len];
  unsigned char e_question[len];
  unsigned char question[len];

  unsigned char response[len];
  unsigned char e_response[len];

  unsigned char nonce3[crypto_box_NONCEBYTES];
  unsigned char c_nonce2[crypto_box_NONCEBYTES];
  FILE * q = fopen("question.txt","r");
  FILE * a = fopen("answer.txt","wb");
  fread(e_question,len,1,q);
  fclose(q);

  success=crypto_box_open(question,e_question,len,nonce2,client_pk,server_sk);
  assert(success==0);
  printf("Decrypted client question\n");
  
  for(i=0;i<24;i++)
    c_nonce2[i]=question[i+crypto_box_ZEROBYTES];
  
  assert(memcmp(c_nonce2,nonce2,24)==0);
  printf("N2 verified\n");
  for(i=0;i<24;i++)
    nonce3[i]=question[i+crypto_box_ZEROBYTES+24];
  for(i=0;i<text_len;i++)
    question_text[i]=question[i+crypto_box_ZEROBYTES+48];
  printf("Retrieved question: \n\n");
  printf("'%s'\n\n",question_text);
  printf("Response: "); 
  fgets (answer_text, text_len, stdin);
  //remove newline
  if (strlen(answer_text)>0)
    question_text[strlen (answer_text) - 1] = '\0';
  
  //encrypt the answer
  for (i = 0; i < crypto_box_ZEROBYTES; i++)
    response[i] = 0;
  for(i=0;i<24;i++)
    response[i+crypto_box_ZEROBYTES]=nonce3[i];
  for(i=0;i<text_len;i++)
    response[i+crypto_box_ZEROBYTES+24]=answer_text[i];
  
  success=crypto_box(e_response,response,response_len,nonce3,client_pk,server_sk);
  assert(success==0);
  printf("Encrypted answer for client\n\n");
  fwrite(e_response,sizeof(e_response),1,a);
  fclose(a);
}
