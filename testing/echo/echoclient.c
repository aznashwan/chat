#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "socklib.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("USAGE: %s SERVER_ADDRESS SERVER_PORT\n", argv[0]);
        return -1;
    }

    // open the socket to the server:
    int sockfd = connect_to_server(argv[1], atoi(argv[2]));
    if (sockfd < 0) {
        printf("Error connecting to server.\n");
        return -2;
    }

    // write a message:
    char* msg = "This is the message that should be echoed.";
    int len = strlen(msg) + 1;
    if (write_to_sock(sockfd, msg, len) < 0) {
        printf("Error writing on socket.");
        return -3;
    }

    // recieve the response:
    bzero(msg, strlen(msg) + 1);
    if(read_from_sock(sockfd, msg, len) < 0) {
        printf("Error reading from socket.");
        return -4;
    }

    printf("%s\n", msg);
}

