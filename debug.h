#ifndef _DEBUG_H
#define _DEBUG_H

#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 1
#define HEADER "DEBUG: "

// prints the given message to standard error:
void debug(char* message);

// get_ip_addr_str is a helper function which, given the unsigned int
// representing an IP address, allocates and returns a pointer to its string
// representation. The returned pointer should be freed when done with.
char* get_ip_addr_str(unsigned int n);

#endif
