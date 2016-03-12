#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"


// prints the given message to standard error. The message will be truncated
// to MAX_DBMSG_LEN if it is longer. Any memory allocated for the message
// is not freed and is left to the caller's discretion.
void debug(char* message) {
    if (!DEBUG) return;

    char* buff = (char *) malloc(MAX_DBMSG_LEN);
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
