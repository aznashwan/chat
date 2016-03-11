#ifndef _AUTHLIB_H
#define _AUTHLIB_H

#include <stdlib.h>


// creds_t is the type representing a set user credentials.
struct creds_t {
    char* uname;
    char* upass;
};

struct creds_t CREDENTIALS[] = {
    { .uname = "aznashwan"    , .upass = "sockmaster" },
    { .uname = "luciantoo"    , .upass = "toolucian"  },
    { .uname = "thevladeffect", .upass = "vladspass"  },
    { .uname = NULL, .upass = NULL }
};

// auth_connection is a helper function which, given the file descriptor of
// a freshly-accepted connection, blocks until both a username and a password
// are provided. If all was well, a pointer to the username of the newly
// authenticated user will be returned, and must be properly freed, otherwise
// NULL will be returned. In case of an error, a negative value is returned.
char* auth_connection(int sockfd);

// verify_creds is a helper function which returns 1 if the given username
// and password are found in the given NULL-terminated list of
// creds_t or 0 if no match was found.
int verify_creds(struct creds_t* creds, char* uname, char* upass);

#endif
