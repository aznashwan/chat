#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "socklib.h"


void* serv_func(void *sockfd) {
    char* buff = (char *) malloc(STDTRANS_SIZE);
    int* fd = (int *) sockfd;
    int res;

    res = read_str_from_sock(*fd, buff, STDTRANS_SIZE);
    if (res < 0) {
        debug("serv_func: error reading incoming request.");
        free(buff);
        free(sockfd);
        return NULL;
    }

    printf("%s\n", buff);

    res = write_str_to_sock(*fd, buff);
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
