#ifndef _SOCKLIB_H
#define _SOCKLIB_H

#include <netinet/in.h>

#define SOCKADDR_IN_SIZE sizeof(struct sockaddr_in)

// Because the socket stream API over tcp/ip requires message
// termination to be done at the application level, a character is needed to
// indicate the end of a transmission.
#define TERMINATION_CHAR '\0'

// For consistency among various sub-modules of the code, a standard definition
// of a normal transmission size is useful.
#define STDTRANS_SIZE 1024

// the maximum number of connection requests to be backlogged
// while waiting to be accepted:
#define MAX_BACKLOG 5

// setup_server sets up a socket to listen on the specified sockaddr with the
// given max connection backlog and returns the afferent socket's file
// descriptor or a negative value in case of an error.
int setup_server(struct sockaddr_in addr, int backlog);

// server_func_t is the type of functions which can serve requests.
// a server_func_t takes a void pointer to the int file descriptor of the
// accepted connection's temporary socket, reads the request/writes the
// response as seen fit, after which it MUST BOTH CLOSE THE FILE DESCRIPTOR
// AND FREE ITS RESPECTIVE MEMORY.
// Any result value of a server_func_t should be presumed ignored, and as such
// any memory allocated for the result will most likely never get freed...
typedef void* (*server_func_t)(void* fd);

// server sets up a socket listening on the provided port.
// whenever an incoming connection is accepted, the provided server_func_t is
// launched in its own pthread with the connection's file descriptor.
// It is up to the server_func_t to close the provided file descriptor upon
// termination of communication and deallocate its memory.
// The return value of the server_func_t is ignored, and as such, should not be
// anything with memory allocated on the heap which is ever expected to be freed.
int server(int port, server_func_t f);

// connect_to_server is a convenience function which returns a file descriptor
// to a socket which has been opened and connected to a server with the given
// address and port. It returns a negative int on error.
int connect_to_server(char* addr, int port);

// open_socket sets up and opens a socket, returning its file descriptor
// or a negative exit code on failure.
int open_socket();

// mksockaddr creates a struct sockaddr_in with the given address and port
// number. If on the server-side, no address is required.
//
// Example:
//  mksockaddr("13.23.65.18", 5656); // client-side
//  mksockaddr("", 5656);            // server-side
struct sockaddr_in mksockaddr(char* addr, int portno);

// write_str_to_sock is a convenience wrapper around write_to_sock which
// specially handles writing a string to the given socket file descriptor.
// It returns -1 on error or the number of characters of the string written
// (which is bound to be the strlen() + 1 due to the added EOF character).
// See write_to_sock for more details.
int write_str_to_sock(int fd, char* str);

// write_to_sock is a convenience function to write from the specified buffer
// to the file descriptor of a socket the given number of bytes.
// It returns the number of bytes written or a negative number on error.
int write_to_sock(int fd, void* message, int len);

// read_str_from sock is a wrapper around read_from_sock which is optimized for
// handling strings. It automatically adds an EOF to the result, thus the given
// length should always be the expected length plus one.
// See read_from_sock for more details.
int read_str_from_sock(int fd, char* str, int len);

// read_from_sock is a convenience function which reads from a socket given by
// a file descriptor a given number of bytes into the given buffer.
// It returns the number of bytes read, or a negative number on error.
int read_from_sock(int fd, void* buff, int len);

#endif
