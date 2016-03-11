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

    // attempt auth:
    char* uname = "aznashwan";
    char* pass = "sockmaster";

    if (write_str_to_sock(sockfd, uname) < 0) {
        printf("Error writing username on socket.");
        return -3;
    }

    if (write_str_to_sock(sockfd, pass) < 0) {
        printf("Error writing password on socket.");
        return -4;
    }

    // recieve the response:
    char msg[STDTRANS_SIZE];
    bzero(msg, STDTRANS_SIZE);
    if(read_str_from_sock(sockfd, msg, STDTRANS_SIZE) < 0) {
        printf("Error reading response from socket.");
        return -4;
    }

    printf("%s\n", msg);
}

