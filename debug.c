#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"


// prints the given message to standard error:
void debug(char* message) {
    if (!DEBUG) return;

    char* buff = (char *) malloc(strlen(HEADER) + strlen(message) + 2);
    strcpy(buff, HEADER);
    strcat(buff, message);
    strcat(buff, "\n");

    fputs(buff, stderr);
    free(buff);
}

// get_ip_addr_str is a helper function which, given the unsigned int
// representing an IP address, allocates and returns a pointer to its string
// representation. The returned pointer should be freed when done with.
char* get_ip_addr_str(unsigned int n) {
    char* buff = (char*) malloc(16);

    sprintf(buff, "%u.%u.%u.%u",
            n  & 0x000000FF,
            (n & 0x0000FF00) >> 8,
            (n & 0x00FF0000) >> 16,
            (n & 0xFF000000) >> 24);

    return buff;
}
