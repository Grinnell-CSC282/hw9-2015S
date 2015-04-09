/*
initialize stuff for server
 */
void server_init();

/*
get the initial nonce
 */
void getN0(char * n);

/*
get the servers public key 
 */
void get_server_keys(char * pk,char * fsk);

void s_send_verif();

void s_recieve_verif();

void answer_question();
