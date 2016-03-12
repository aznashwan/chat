#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "auth.h"
#include "debug.h"
#include "socklib.h"


void* serv_func(void* sockfd) {
    char* username;
    char* response;
    int* fd = (int *) sockfd;
    char* dbmsg = (char *) malloc(80 + STDTRANS_SIZE);

    username = auth_connection(*fd);
    if (username < 0) {
        debug("serv_func: error authenticating connection.");
        free(fd);
        return NULL;
    }

    if (username != NULL) {
        sprintf(dbmsg, "Succesfully authenticated '%s'.", username);
        response = "Congratulations, you've been authenticated.";
    } else {
        strcpy(dbmsg, "Failed to authenticate user.");
        response = "Invalid username and/or password.";
    }

    debug(dbmsg);
    free(dbmsg);

    if (write_str_to_sock(*fd, response) < 0) {
        debug("serv_func: error writing response to socket.");
    }

    close(*fd);
    free(username);
    free(fd);
    return NULL;
}

int main(int argc, char* argv[]) {
    int res;

    if (argc != 2) {
        printf("USAGE: %s PORT_NUMBER\n", argv[0]);
        return -1;
    }

    res = server(atoi(argv[1]), &serv_func);
    if (res != 0) {
        debug("main: error running server.");
    }

    return 0;
}
