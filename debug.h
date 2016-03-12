#ifndef _DEBUG_H
#define _DEBUG_H

#define DEBUG 1
#define HEADER "DEBUG: "
#define MAX_DBMSG_LEN 1024

// prints the given message to standard error. The message will be truncated
// to MAX_DBMSG_LEN if it is longer. Any memory allocated for the message
// is not freed and is left to the caller's discretion.
void debug(char* message);

// get_ip_addr_str is a helper function which, given the unsigned int
// representing an IP address, allocates and returns a pointer to its string
// representation. The returned pointer should be freed when done with.
char* get_ip_addr_str(unsigned int n);

#endif
