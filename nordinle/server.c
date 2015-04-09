// server file
#include "crypto_box.h"
#include "devurandom.h"
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define QUESTION_LENGTH 45

static unsigned char pk[crypto_box_PUBLICKEYBYTES];
static unsigned char sk[crypto_box_SECRETKEYBYTES];
static unsigned char client_pk[crypto_box_PUBLICKEYBYTES];
static unsigned char first_pk[crypto_box_PUBLICKEYBYTES];
static unsigned char first_sk[crypto_box_SECRETKEYBYTES];
static unsigned char nonce_zero[crypto_box_NONCEBYTES];
static unsigned char nonce_one[crypto_box_NONCEBYTES];
static unsigned char nonce_two[crypto_box_NONCEBYTES];
static unsigned char nonce_three[crypto_box_NONCEBYTES];

void initialize_server(){
  crypto_box_keypair(pk,sk);
  crypto_box_keypair(first_pk,first_sk);

  int i;
  for(i = 0; i < crypto_box_NONCEBYTES; i++) {
    nonce_zero[i] = 0;
  }

  FILE * server_pk_file = fopen("server_public_key", "w");
  FILE * first_sk_file = fopen("first_secret_key", "w");
  FILE * nonce_z_file = fopen("nonce_zero", "w");

  fwrite(pk, crypto_box_PUBLICKEYBYTES, 1, server_pk_file);
  fclose(server_pk_file);

  fwrite(first_sk, crypto_box_PUBLICKEYBYTES, 1, first_sk_file);
  fclose(first_sk_file);

  fwrite(nonce_zero, crypto_box_NONCEBYTES, 1, nonce_z_file);
  fclose(nonce_z_file);
}

void server_second() {
	unsigned long long cipher_length = crypto_box_ZEROBYTES + crypto_box_PUBLICKEYBYTES + crypto_box_NONCEBYTES;
  unsigned char cipher[cipher_length]; 
  unsigned char received_message[cipher_length];

  FILE * com_channel = fopen("communication_channel", "r");

  fread (cipher, cipher_length, 1, com_channel);
  fclose(com_channel);

  crypto_box_open(received_message, cipher, cipher_length, nonce_zero, first_pk, sk);

  int i, j;
  for(i = crypto_box_ZEROBYTES, j = 0; i < crypto_box_ZEROBYTES + crypto_box_NONCEBYTES; i++, j++){
    nonce_one[j] = received_message[i];
  }
  for(j = 0; i < cipher_length; i++, j++){
    client_pk[j] = received_message[i];
  }

  randombytes(nonce_two, crypto_box_NONCEBYTES);

  // For knowing how to copy the time over
  // http://stackoverflow.com/questions/10319805/c-unsigned-int-to-unsigned-char-array-conversion
  // For knowing how to use time()
  // http://www.gnu.org/software/libc/manual/html_node/Time-Functions-Example.html
  time_t timestamp = time(NULL);
  int time_size = sizeof(timestamp);
  unsigned char time_array[time_size];
  memcpy(time_array, (char*)&timestamp, time_size);
  
  unsigned long long sent_message_length = crypto_box_ZEROBYTES + 2*crypto_box_NONCEBYTES + time_size;
  unsigned char sent_message[sent_message_length];
  unsigned char sent_cipher[sent_message_length];

  for(i = 0; i < crypto_box_ZEROBYTES; i++) {
    sent_message[i] = 0;
  }
  for(j = 0; i < crypto_box_ZEROBYTES + crypto_box_NONCEBYTES; i++, j++) {
    sent_message[i] = nonce_one[j];
  }
  for(j = 0; i < crypto_box_ZEROBYTES + 2*crypto_box_NONCEBYTES; i++, j++) {
    sent_message[i] = nonce_two[j];
  }

  for(j = 0; i < sent_message_length; i++, j++) {
    sent_message[i] = time_array[j];
  }

  crypto_box(sent_cipher, sent_message, sent_message_length, nonce_one, client_pk, sk);

  com_channel = fopen("communication_channel", "w");
  fwrite(sent_cipher, sent_message_length, 1, com_channel);
  fclose(com_channel);
}

void server_fourth() {
  unsigned long long cipher_length = crypto_box_ZEROBYTES + 2*crypto_box_NONCEBYTES + QUESTION_LENGTH;
  unsigned char cipher[cipher_length];
  unsigned char received_message[cipher_length];
  unsigned char question_unsigned[QUESTION_LENGTH];
  unsigned char answer[QUESTION_LENGTH];
  char question_string[QUESTION_LENGTH];

  FILE * com_channel = fopen("communication_channel", "r");
  fread (cipher, cipher_length, 1, com_channel);
  fclose(com_channel);

  crypto_box_open(received_message, cipher, cipher_length, nonce_two, client_pk, sk);

  int i, j;
  for(i = crypto_box_ZEROBYTES, j = 0; i < crypto_box_ZEROBYTES + crypto_box_NONCEBYTES; i++, j++){
    assert(nonce_two[j] == received_message[i]);
  }
  for(j = 0; i < crypto_box_ZEROBYTES + 2*crypto_box_NONCEBYTES; i++, j++){
    nonce_three[j] = received_message[i];
  }
  for(j = 0; i < cipher_length; i++, j++){
    question_unsigned[j] = received_message[i];
  }

  unsigned long number_to_square;
  sprintf(question_string, "%s", question_unsigned);
  sscanf(question_string, "what is %ld squared?", &number_to_square);
  number_to_square = number_to_square * number_to_square;
  memcpy(answer, (char*)&number_to_square, sizeof(number_to_square));

  unsigned long long sent_message_length = crypto_box_ZEROBYTES + crypto_box_NONCEBYTES + QUESTION_LENGTH;
  unsigned char sent_message[sent_message_length];
  unsigned char sent_cipher[sent_message_length];

  for(i = 0; i < crypto_box_ZEROBYTES; i++) {
    sent_message[i] = 0;
  }
  for(j = 0; i < crypto_box_ZEROBYTES + crypto_box_NONCEBYTES; i++, j++) {
    sent_message[i] = nonce_three[j];
  }
  for(j = 0; i < sent_message_length; i++, j++) {
    sent_message[i] = answer[j];
  }

  crypto_box(sent_cipher, sent_message, sent_message_length, nonce_three, client_pk, sk);

  com_channel = fopen("communication_channel", "w");
  fwrite(sent_cipher, sent_message_length, 1, com_channel);
  fclose(com_channel);
}
