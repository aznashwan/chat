#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "debug.h"
#include "socklib.h"
#include "sync.h"

// open_conns_init initializes the list of open connetions.
void open_conns_init() {
    debug("open_conns_init: initialising connection list.");

    pthread_mutex_init(&open_conns_mutex, NULL);

    int i;
    open_conns = (int *) malloc(MAX_OPEN_CONNS * sizeof(int));
    for (i = 0; i < MAX_OPEN_CONNS; i++) {
        *(open_conns + i) = OPEN_CONNS_EMPTY;
    }
}

// open_conns_add adds a connection specified by the given file descriptor
// to the first opening in the open_conns list. It returns 0 if all went well
// or -1 in case there are no more open connection slots currently available.
int open_conns_add(int connfd) {
    int i, res = -1;
    char* dbmsg = (char *) malloc(MAX_DBMSG_LEN);

    pthread_mutex_lock(&open_conns_mutex);
    for (i = 0; i < MAX_OPEN_CONNS; i++) {
        if (*(open_conns + i) == OPEN_CONNS_EMPTY) {
            *(open_conns + i) = connfd;

            sprintf(dbmsg, "open_conns_add: succesfully added connection %d.", connfd);
            debug(dbmsg);

            res = 0;
            break;
        }
    }
    pthread_mutex_unlock(&open_conns_mutex);

    free(dbmsg);
    return res;
}

// open_conns_del deletes a connection specified by the given file descriptor
// from the open_conns list by setting it to OPEN_CONNS_EMPTY.
void open_conns_del(int connfd) {
    int i;
    char* dbmsg = (char *) malloc(MAX_DBMSG_LEN);
    sprintf(dbmsg, "open_conns_del: deleting connection %d.", connfd);
    debug(dbmsg);
    free(dbmsg);

    pthread_mutex_lock(&open_conns_mutex);
    for (i = 0; i < MAX_OPEN_CONNS; i++) {
        if (*(open_conns + i) == connfd) {
            *(open_conns + i) = OPEN_CONNS_EMPTY;
            break;
        }
    }
    pthread_mutex_unlock(&open_conns_mutex);
}

// broadcast sends out the given message to all open connections.
void broadcast(char* msg) {
    int i;
    char* dbmsg = (char *) malloc(MAX_DBMSG_LEN);

    pthread_mutex_lock(&open_conns_mutex);
    for(i = 0; i < MAX_OPEN_CONNS; i++) {
        if (*(open_conns + i) != OPEN_CONNS_EMPTY) {
            if(write_str_to_sock(*(open_conns + i), msg) < 0) {
                sprintf(dbmsg, "broadcast: error writing on socket %d.",
                        *(open_conns + i));
                debug(dbmsg);
            }
        }
    }
    pthread_mutex_unlock(&open_conns_mutex);

    free(dbmsg);
}
