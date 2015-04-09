#include <stdio.h>
#include "crypto_box.h"
#include "devurandom.h"
#include <string.h>
#include "display_bytes.h"
#include "assert.h"
#include <time.h>
#include "time_union.h"

/*
 Security assigment two
 by Nicolas Knoebber and Erin Callaway
 */

//global variables 

unsigned char pk[crypto_box_PUBLICKEYBYTES];
unsigned char sk[crypto_box_SECRETKEYBYTES];
unsigned char nonce1[crypto_box_NONCEBYTES];
unsigned char nonce2[crypto_box_NONCEBYTES];
unsigned char nonce3[crypto_box_NONCEBYTES];

/*
   generate a random  nonce N1 and use a the one time keypair from the server, init_sk. 
   Concatenate the nonce and the client pk, 
   encrypts the concatenation server_pk
   send encrypted message to the server. 
 */
void c_send_verif(unsigned char * server_pk,unsigned char * init_sk, unsigned char * nonce0)
{
  printf("------------------------------\n");
  printf("Client: \n");
  printf("------------------------------\n");
  FILE * veri = fopen("verification.txt","wb");
  int len=crypto_box_PUBLICKEYBYTES+crypto_box_NONCEBYTES+crypto_box_ZEROBYTES;
  unsigned char message[len];
  unsigned char result[len];
  int i;
  int success;

  //generate nonce1
  randombytes(nonce1,24);
  //generate keypair
  crypto_box_keypair(pk,sk);

  //put 0 bytes in 
  for (i = 0; i < crypto_box_ZEROBYTES; i++)
    message[i] = 0;

  //put nonce into message
  for(i=0;i<24;i++)
    message[i+crypto_box_ZEROBYTES] = nonce1[i];
  
  //put public key into message
  for(i=0;i<32;i++)
    message[i+24+crypto_box_ZEROBYTES]=pk[i];

  //encrypt data
  printf("Initial verification Message: \n");
  display_bytes(message,len);
  //encrypt with the keys provided by the server 
  success=crypto_box(result,message,len,nonce0,server_pk,init_sk);
  assert(success==0);

  fwrite(result,sizeof(result),1,veri);
  fclose(veri);
}

/*
 confirms the server's signature. 
 decrypt message using clients secret key.
 check that the first twenty-four bytes of the result match nonce1.
 separates the server's nonce, N2 and the timestamp, T
 check to make sure that the value of the timestamp is within the last ninety seconds
*/
void c_recieve_verif(unsigned char * server_pk)
{
  printf("------------------------------\n");
  printf("Client: \n");
  printf("------------------------------\n");
  FILE * veri2 = fopen("verification2.txt","r");
  int len=crypto_box_ZEROBYTES+(crypto_box_NONCEBYTES*2)+8;
  int success;
  int i;
  unsigned char s_nonce1[24]; //nonce1 from server, which should be equal to nonce1. 
  unsigned char e_response[len]; //servers encrypted response
  unsigned char response[len]; //servers decrypted response
  time_t current = time(NULL);
  fread(e_response,len,1,veri2);
  success=crypto_box_open(response,e_response,len,nonce1,server_pk,sk);
  fclose(veri2);
  assert(success==0);

  printf("Sucessfully decrypted server verification response\n");
  for(i=0;i<24;i++)
    s_nonce1[i] = response[i+crypto_box_ZEROBYTES];
  
  //check that nonce1 matches 
  assert(memcmp(s_nonce1,nonce1,24)==0);
  printf("Original N1: \n");
  display_bytes(nonce1,24);
  printf("N1 retrieved from server: \n");
  display_bytes(s_nonce1,24);
  printf("N1 verified\n");

  //retrieve nonce2 that server sent
  
  for(i=0;i<24;i++)
    nonce2[i] = response[i+crypto_box_ZEROBYTES+24];
  
  //retrieve the time
  for(i=0;i<8;i++)
    T.char_time[i] = response[i+48+crypto_box_ZEROBYTES];
  
  printf("N2 retrieved from server at %d seconds\n",T.time_stamp);
  assert(((current - T.time_stamp)<90) == 1);
  printf("Verification complete\n\n");
}
/*
 Concatenate N2, N3, and the text of the question Q,
 encrypt the concatenation with the server's public key and nonce N2
*/
void ask_question(unsigned char * server_pk)
{

  //the longest the question can be 
  int question_len = 64;
  int message_len = question_len+crypto_box_ZEROBYTES+(crypto_box_NONCEBYTES*2);
  char question_text[question_len];
  unsigned char message[message_len];
  unsigned char e_message[message_len];
  int i;
  int success;
  FILE * q = fopen("question.txt","wb");
  printf("Question for the server: ");
  fgets (question_text, question_len, stdin);
  //remove newline
  if (strlen(question_text)>0)
    question_text[strlen (question_text) - 1] = '\0';
  //generate nonce3
  randombytes(nonce3,24);
  //build plain text message with question

  for (i = 0; i < crypto_box_ZEROBYTES; i++)
    message[i] = 0;
  for(i=0;i<24;i++)
    message[i+crypto_box_ZEROBYTES] = nonce2[i];
  for(i=0;i<24;i++)
    message[i+crypto_box_ZEROBYTES+24] = nonce3[i];
  for(i=0;i<question_len;i++)
    message[i+48+crypto_box_ZEROBYTES]=question_text[i];
  
  success=crypto_box(e_message,message,message_len,nonce2,server_pk,sk);
  assert(success==0);
  printf("Successfully encrypted question for server\n\n");
  fwrite(e_message,sizeof(e_message),1,q);
  fclose(q);
}
/*
  Recieve and decrypt answer with nonce3, server_pk, and client sk. 
  Verify that nonce is unchanged
 */
void recieve_answer(unsigned char * server_pk)
{
  int text_len = 64;
  int message_len = text_len+crypto_box_ZEROBYTES+crypto_box_NONCEBYTES;
  char answer_text[text_len];
  unsigned char message[message_len];
  unsigned char e_message[message_len];
  unsigned char s_nonce3[24];
  FILE * a = fopen("answer.txt","r");
  fread(e_message,message_len,1,a);
  fclose(a);
  int i;
  int success;
  success=crypto_box_open(message,e_message,message_len,nonce3,server_pk,sk);
  assert(success==0);
  for(i=0;i<24;i++)
    s_nonce3[i] = message[i+crypto_box_ZEROBYTES];
  assert(memcmp(nonce3,s_nonce3,24)==0);
  printf("N3 verified\n");
  for(i=0;i<text_len;i++)
    answer_text[i] = message[24+crypto_box_ZEROBYTES+i];

  printf("The decrypted answer is:  %s\n",answer_text);

}
