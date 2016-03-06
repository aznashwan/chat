#include <stdlib.h>
#include <stdio.h>

#include "debug.h"
#include "socklib.h"


void* serv_func(void *sockfd) {
    char* buff = (char *) malloc(1024);
    int* fd = (int *) sockfd;
    int res;

    res = read_from_sock(*fd, buff, 1024);
    if (res < 0) {
        debug("serv_func: error reading incoming request.");
        return NULL;
    }

    printf("%s\n", buff);

    res = write_to_sock(*fd, buff, strlen(buff) + 1);
    if (res < 0) {
        debug("serv_func: error writing response.");
    }

    free(buff);
    free(sockfd);
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
