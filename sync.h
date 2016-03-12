#ifndef _SYNC_H
#define _SYNC_H

#include <pthread.h>

#define OPEN_CONNS_EMPTY 0
#define MAX_OPEN_CONNS 1024

// open_conns is the list of integer file descriptors of the open connections
// Upon termination of communication, the entry from open_conn may be set to
// OPEN_CONNS_EMPTY to mark as ready to be reallocated for a new connection
// by open_conns_add.
static int* open_conns = NULL;

// the mutex which guards the open_conns; 
static pthread_mutex_t open_conns_mutex;

// open_conns_init initializes the list of open connections with
// OPEN_CONNS_EMPTY.
void open_conns_init();

// open_conns_add adds a connection specified by the given file descriptor
// to the first opening in the open_conns list. It returns 0 if all went well
// or -1 in case there are no more open connection slots currently available.
int open_conns_add(int connfd);

// open_conns_del deletes a connection specified by the given file descriptor
// from the open_conns list by setting it to OPEN_CONNS_EMPTY.
void open_conns_del(int connfd);

// broadcast sends out the given message to all open connections.
void broadcast(char* msg);

#endif
