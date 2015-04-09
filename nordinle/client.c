/* client file
 * for csc 214 server and client project
 * By Leland Nordin & Hattie Zucker
 */

#include "crypto_box.h"
#include "devurandom.h"
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define QUESTION_LENGTH 45 // length of question being asked

// The variables to hold the varius keys, nonces and questions
static unsigned char pk[crypto_box_PUBLICKEYBYTES];
static unsigned char sk[crypto_box_SECRETKEYBYTES];
static unsigned char server_pk[crypto_box_PUBLICKEYBYTES];
static unsigned char first_pk[crypto_box_PUBLICKEYBYTES];
static unsigned char first_sk[crypto_box_PUBLICKEYBYTES];
static unsigned char nonce_zero[crypto_box_NONCEBYTES];
static unsigned char nonce_one[crypto_box_NONCEBYTES];
static unsigned char nonce_two[crypto_box_NONCEBYTES];
static unsigned char nonce_three[crypto_box_NONCEBYTES];
static unsigned char question[QUESTION_LENGTH];

// Function that initializes the client by creating a one time keypair
// and picks a random number to be squared in the question.
void initialize_client(){
  crypto_box_keypair(pk,sk); // producing keypair
  srand(time(NULL)); // creating seed for the random number generator
  unsigned int random_integer = rand() % 65535; // pick a random unsigned int
  // put the random number and question into the question array
  sprintf((char*) question, "what is %d squared?", random_integer);
}

// function to grab the server's public key, first time server secret key,
// and whateer the zeroth nonce
void grab_server_keys(){
  // file variables to hold all of the opened file streams
  FILE * server_pk_file = fopen("server_public_key", "r");
  FILE * first_sk_file = fopen("first_secret_key", "r");
  FILE * nonce_zero_file = fopen("nonce_zero", "r");
  
  // reading each of the files byte by byte and storing them in their
  // respective unsigned char arrays
  fread (server_pk, crypto_box_PUBLICKEYBYTES, 1, server_pk_file);
  fclose(server_pk_file);
  fread (first_sk, crypto_box_PUBLICKEYBYTES, 1, first_sk_file);
  fclose(first_sk_file);
  fread (nonce_zero, crypto_box_NONCEBYTES, 1, nonce_zero_file);
  fclose(nonce_zero_file);
}

// function to do the first interaction between server and client
// where the client sends the server an encrypted version of the client's 
// public key and a nonce to use for further communication using the first 
// secret key published by the server
void client_first() {
  // variables to hold the message and cipher of the first message
  // length is determind by the amount of zerobytes required by cryptobox
  // and the size of our message which in this case is going to be
  // a public key + a nonce long
  int message_length = crypto_box_ZEROBYTES + crypto_box_PUBLICKEYBYTES + crypto_box_NONCEBYTES;
  unsigned char message[message_length];
  unsigned char cipher[message_length];

  // producing the random noce to use for future communication
  randombytes(nonce_one, crypto_box_NONCEBYTES);

  // a bunch of for loops to put the padded zero bytes then the nonce
  // then the public key into the message
  int i;
  int j;
  for(i = 0; i < crypto_box_ZEROBYTES; i++) {
    message[i] = 0;
  }
  for(j = 0; i < crypto_box_ZEROBYTES + crypto_box_NONCEBYTES; i++, j++) {
    message[i] = nonce_one[j];
  }
  for(j = 0; i < message_length; i++, j++) {
    message[i] = pk[j];
  }

  // crypto box function which does all the magic. encrypting with the server's
  // public key and signing with the first time secret key provided by the 
  // server
  crypto_box(cipher, message, message_length, nonce_zero, server_pk, first_sk);

  // placing the encrypted message in the "communication channel"
  FILE * com_channel = fopen("communication_channel", "w");
  fwrite(cipher, message_length, 1, com_channel);
  fclose(com_channel);
}

// function to grab the server's response that contains the nonce that was
// sent in client_first, a new nonce for encrypting the question, and a time
// stamp that the client has to check was created in the past 90 seconds. Once
// the nonce is confirmed to be the same as client_first the client sends
// the question
void client_third() {
  // variables to hold the ciper and decrypted message. the size is
  // based on that the client knows it is recieving two nonces and a timestamp
  unsigned long long cipher_length = crypto_box_ZEROBYTES + 2*crypto_box_NONCEBYTES + sizeof(time_t);
  unsigned char cipher[cipher_length];
  unsigned char received_message[cipher_length];

  // creating a timestamp so that we can check that the server's message
  // was sent in the last 90 seconds
  time_t time_now = time(NULL);
  int time_size = sizeof(time_now);
  unsigned char time_array[time_size];

  // open the communication_chanel file and grab the encrypted message that
  // the server sent
  FILE * com_channel = fopen("communication_channel", "r");
  fread (cipher, cipher_length, 1, com_channel);
  fclose(com_channel);

  // open the encrypted message using the nonce the client sent in client_first,
  // the clients secret key and the server's public key to check the signature
  crypto_box_open(received_message, cipher, cipher_length, nonce_one, server_pk, sk);

  // three for loops to parse the unencrypted message
  // first one checks that the nonce sent in client_first is the same
  // as the nonce received. Second loop parses out the second nonce that the
  // client will now use to communication. Third loops parses out the timestamp
  int i, j;
  for(i = crypto_box_ZEROBYTES, j = 0; i < crypto_box_ZEROBYTES + crypto_box_NONCEBYTES; i++, j++){
    assert(nonce_one[j] == received_message[i]);
  }
  for(j = 0; i < crypto_box_ZEROBYTES + 2*crypto_box_NONCEBYTES; i++, j++){
    nonce_two[j] = received_message[i];
  }
  for(j = 0; i < cipher_length; i++, j++){
    time_array[j] = received_message[i];
  }

  // variable to hold the timestamp
  unsigned long rec_message_timestamp;
  // use memcpy to translate the timestamp from unsigned char[] to a long
  memcpy(&rec_message_timestamp, &time_array, time_size);
  // make sure that the timestamp on the message is within 90 seconds of now
  assert(time_now - 90 <= rec_message_timestamp);
  // create a new nonce to put with your question that the server will use
  // to encrypt the answer
  randombytes(nonce_three, crypto_box_NONCEBYTES);

  // variables to hold the size of the final message from client. size is based
  // on the fact that we are sending two nonces and a question
  unsigned long long sent_message_length = crypto_box_ZEROBYTES + 2*crypto_box_NONCEBYTES + QUESTION_LENGTH;
  unsigned char sent_message[sent_message_length];
  unsigned char sent_cipher[sent_message_length];

  // for loops to pad the message with zero bytes, then put the second nonce
  // in, then the new third nonce and finally the question.
  for(i = 0; i < crypto_box_ZEROBYTES; i++) {
    sent_message[i] = 0;
  }
  for(j = 0; i < crypto_box_ZEROBYTES + crypto_box_NONCEBYTES; i++, j++) {
    sent_message[i] = nonce_two[j];
  }
  for(j = 0; i < crypto_box_ZEROBYTES + 2*crypto_box_NONCEBYTES; i++, j++) {
    sent_message[i] = nonce_three[j];
  }
  for(j = 0; i < sent_message_length; i++, j++) {
    sent_message[i] = question[j];
  }

  // encrypt the message using the nonce that the server sent earlier in this
  // function, the server's public key and sign with clients secret key
  crypto_box(sent_cipher, sent_message, sent_message_length, nonce_two, server_pk, sk);

  // write the cipher text to the communication channel
  com_channel = fopen("communication_channel", "w");
  fwrite(sent_cipher, sent_message_length, 1, com_channel);
  fclose(com_channel);
}

// final function to grab the encrypted answer to the question, decrypt it,
// and then print the answer to check if it is correct
void client_fifth () {
  unsigned long long cipher_length = crypto_box_ZEROBYTES + crypto_box_NONCEBYTES + QUESTION_LENGTH;
  unsigned char cipher[cipher_length];
  unsigned char received_message[cipher_length];
  unsigned char answer_array[QUESTION_LENGTH];
  unsigned long answer_long;

  FILE * com_channel = fopen("communication_channel", "r");
  fread (cipher, cipher_length, 1, com_channel);
  fclose(com_channel);

  crypto_box_open(received_message, cipher, cipher_length, nonce_three, server_pk, sk);

  int i, j;
  for(i = crypto_box_ZEROBYTES, j = 0; i < crypto_box_ZEROBYTES + crypto_box_NONCEBYTES; i++, j++){
    assert(nonce_three[j] == received_message[i]);
  }
  for(j = 0; i < cipher_length; i++, j++){
    answer_array[j] = received_message[i];
  }

  memcpy(&answer_long, &answer_array, sizeof(answer_long));
  printf("The Client asked: %s\n", question);
  printf("The Server's answer was %ld", answer_long);
}
