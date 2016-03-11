#include <stdlib.h>
#include <strings.h>

#include "auth.h"
#include "debug.h"
#include "socklib.h"

// auth_connection is a helper function which, given the file descriptor of
// a freshly-accepted connection, blocks until both a username and a password
// are provided. If all was well, a pointer to the username of the newly
// authenticated user will be returned, and must be properly freed, otherwise
// NULL will be returned. In case of an error, a negative value is returned.
char* auth_connection(int sockfd) {
    char* uname = (char *) malloc(STDTRANS_SIZE);
    char* upass = (char *) malloc(STDTRANS_SIZE);

    if (read_str_from_sock(sockfd, uname, STDTRANS_SIZE - 1) < 0) {
        debug("auth_connection: error on reading username from socket.");
        free(uname);
        free(upass);
        return (char *) -1;
    }

    if (read_str_from_sock(sockfd, upass, STDTRANS_SIZE - 1) < 0) {
        debug("auth_connection: error on reading password from socket.");
        free(uname);
        free(upass);
        return (char *) -1;
    }

    if(!verify_creds(CREDENTIALS, uname, upass)) {
        debug("auth_connection: invalid credentials provided.");
        free(uname);
        free(upass);
        return (char *) -2;
    }

    free(upass);
    return uname;
}

// verify_creds is a helper function which returns 1 if the given username
// and password are found in the given NULL-terminated list of
// creds_t or 0 if no match was found.
int verify_creds(struct creds_t* creds, char* uname, char* upass) {
    for (;;) {
        if (creds -> uname == NULL) {
            return 0;
        }

        if (strcmp(creds -> uname, uname) == 0 &&
            strcmp(creds -> upass, upass) == 0) {
            return 1;
        }
    }

    return 0;
}
