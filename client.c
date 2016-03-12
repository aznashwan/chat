#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "debug.h"
#include "socklib.h"
#include "termlib.h"


// the mutex used to guard console writes:
pthread_mutex_t console_mutex;

void* conn_thread(void* sockfd) {
    int* fd  = (int *) sockfd;
    char* msg = (char *) malloc (STDTRANS_SIZE);

    for (;;) {
        if (read_str_from_sock(*fd, msg, STDTRANS_SIZE - 1) <= 0) {
            printf("%sError reading message from socket.\n", CLEARLINE);
            goto cleanup;
        }
        pthread_mutex_lock(&console_mutex);
        printm(msg);
        pthread_mutex_unlock(&console_mutex);
    }

cleanup:
    free(msg);
    free(sockfd);
    exit(-1);
    return NULL;
}

int main(int argc, char* argv[]) {
    int nerror = 0;

    if (argc != 5) {
        printf("USAGE: %s SERVER_ADDRESS SERVER_PORT USERNAME PASSWORD\n", argv[0]);
        return -1;
    }

    // open the socket to the server:
    int* sockfd = (int *) malloc(sizeof(int));
    *(sockfd) = connect_to_server(argv[1], atoi(argv[2]));
    if (sockfd < 0) {
        printf("Error connecting to server.\n");
        return -2;
    }

    // attempt auth:
    debug("writing username to server");
    if (write_str_to_sock(*sockfd, argv[3]) < 0) {
        printf("Error writing username on socket.\n");
        close(*sockfd);
        return -3;
    }

    debug("writing password to server");
    if (write_str_to_sock(*sockfd, argv[4]) < 0) {
        printf("Error writing password on socket.\n");
        close(*sockfd);
        return -4;
    }

    // the message read from the console:
    char* cmsg = (char *) malloc(STDTRANS_SIZE); *(cmsg) = '\0';

    // placeholder message used for constant comparison of update:
    char* plcmsg = (char *) malloc (STDTRANS_SIZE); strcpy(plcmsg, cmsg);

    // create the message reader thread:
    pthread_t read_thread;
    if (pthread_create(&read_thread, NULL, &conn_thread, sockfd) != 0) {
        debug("Error starting reading thread.");
        goto cleanup;
    }
    
    // loop and keep sending out messages:
    for (;;) {
        getl(cmsg);

        // skip if return key not hit yet:
        if (strcmp(cmsg, plcmsg) == 0) {
            continue;
        }

        // skip if no input provided:
        if (strcmp(cmsg, "") == 0) {
            printf(RETURN);
            printf(UP);
            printf(CLEARLINE);
            printf(PROMPT);
            continue;
        }

        // clear the input line:
        pthread_mutex_lock(&console_mutex);
        printf(RETURN);
        printf(UP);
        printf(CLEARLINE);
        printf(PROMPT);
        pthread_mutex_unlock(&console_mutex);

        // remember last input:
        strcpy(plcmsg, cmsg);

        // send the message to the server:
        if (write_str_to_sock(*sockfd, cmsg) < 0) {
            printf("%sError writing message to socket.\n", CLEARLINE);
            nerror = -6;
            goto cleanup;
        }
    }

cleanup:
    free(cmsg);
    free(plcmsg);
    close(*sockfd);
    free(sockfd);

    return nerror;
}
