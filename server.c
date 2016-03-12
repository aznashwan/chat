#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "auth.h"
#include "debug.h"
#include "socklib.h"
#include "sync.h"


void* serv_func(void* sockfd) {
    char* username;
    int* fd = (int *) sockfd;
    char* dbmsg = (char *) malloc(MAX_DBMSG_LEN);

    char* msgbuff = (char *) malloc(STDTRANS_SIZE);
    char* outbuff = (char *) malloc(STDTRANS_SIZE);

    username = auth_connection(*fd);
    if (username < 0) {
        debug("serv_func: error authenticating connection.");
        goto cleanup;
    }

    if (username != NULL) {
        sprintf(dbmsg, "serv_func: succesfully authenticated '%s'.", username);
        debug(dbmsg);
        if (write_str_to_sock(*fd, "Succesfully authenticated.") < 0) {
            debug("serv_func: error sending authentication confirmation.");
        }
    } else {
        debug("serv_func: failed to authenticate user.");
        if (write_str_to_sock(*fd, "Invalid username and/or password.") < 0) {
            debug("serv_func: error writing acces denial to client.");
        }
        goto cleanup;
    }

    // add this connection to the list of open ones:
    open_conns_add(*fd);

    // permanently read incoming messages:
    while (!(read_str_from_sock(*fd, msgbuff, STDTRANS_SIZE - 1) < 0)) {
        if (strcmp(msgbuff, "") == 0) {
            continue;
        }

        sprintf(dbmsg, "serv_func: broadcasting '%s' from '%s'.", msgbuff, username);
        debug(dbmsg);

        // annotate the message with the user's name:
        strcpy(outbuff, username);
        strcat(outbuff, ": ");
        strcat(outbuff, msgbuff);

        // and broadcast it to all the other connections:
        broadcast(outbuff);
    }

    // remove the connection from the list:
    open_conns_del(*fd);

cleanup:
    close(*fd);
    free(username);
    free(msgbuff);
    free(outbuff);
    free(dbmsg);
    free(fd);
    return NULL;
}

int main(int argc, char* argv[]) {
    int res;

    if (argc != 2) {
        printf("USAGE: %s PORT_NUMBER\n", argv[0]);
        return -1;
    }

    // intialise the connections list:
    open_conns_init();

    // start serving:
    res = server(atoi(argv[1]), &serv_func);
    if (res != 0) {
        debug("main: error running server.");
    }

    return 0;
}
