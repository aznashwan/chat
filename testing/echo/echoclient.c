#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    char msg[STDTRANS_SIZE] = "This is the message that should be echoed.";
    if (write_str_to_sock(sockfd, msg) < 0) {
        printf("Error writing on socket.");
        return -3;
    }

    // recieve the response:
    memset(msg, '\0', STDTRANS_SIZE);
    if(read_str_from_sock(sockfd, msg, STDTRANS_SIZE) < 0) {
        printf("Error reading from socket.");
        return -4;
    }

    // print the recieved result:
    printf("%s\n", msg);

    close(sockfd);
}
