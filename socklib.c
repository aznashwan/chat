#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "debug.h"
#include "socklib.h"

// setup_server sets up a socket to listen on the specified sockaddr with the
// given max connection backlog and returns the afferent socket's file
// descriptor or a negative value in case of an error.
int setup_server(struct sockaddr_in addr, int backlog) {
    int fd, res;

    // open the socket:
    debug("setup_server: creating server socket.");
    if ((fd = open_socket()) < 0) {
        debug("setup_server: unable to create server socket, returning.");
        return fd;
    }

    // bind the socket to the addr:
    debug("setup_server: binding server socket.");
    res = bind(fd, (struct sockaddr *) &addr, SOCKADDR_IN_SIZE);
    if (res < 0) {
        debug("setup_server: unable to bind server socket, returning.");
        return res;
    }

    // setup socket as listening:
    res = listen(fd, backlog);
    if (res < 0) {
        debug("setup_server: unable to start listening on server socket, returning.");
    }

    return fd;
}

// server sets up a socket listening on the provided port.
// whenever an incoming connection is accepted, the provided server_func_t is
// launched in its own pthread with the connection's file descriptor.
// It is up to the server_func_t to close the provided file descriptor upon
// termination of communication and deallocate its memory.
// The return value of the server_func_t is ignored, and as such, should not be
// anything with memory allocated on the heap which is ever expected to be freed.
int server(int port, server_func_t f) {
    int fd;
    int* connfd = (int *) malloc(sizeof(int));
    struct sockaddr_in caddr, saddr;
    int sockadd_in_size = SOCKADDR_IN_SIZE;
    char* client_ip;
    char *msgbuf = (char *) malloc(STDTRANS_SIZE);
    pthread_t thread;

    // setup the server socket:
    saddr = mksockaddr("", port);
    fd = setup_server(saddr, MAX_BACKLOG);
    if (fd < 0) {
        debug("server: unable to setup server, returning.");
        return fd;
    }

    // start acepting connections:
    while (1) {
        // accept and log the connection:
        *connfd = accept(fd, (struct sockaddr *) &caddr, (socklen_t*) &sockadd_in_size);
        client_ip = get_ip_addr_str(caddr.sin_addr.s_addr);
        sprintf(msgbuf, "server: accepted connection from %s", client_ip);
        debug(msgbuf);
        free(client_ip);

        // spawn a thread with the server_func_t to server the request:
        if (pthread_create(&thread, NULL, f, connfd) != 0) {
            debug("server: error spawning new server thread.");
            return -1;
        }

        // reallocate the connfd and continue waiting for connections:
        connfd = (int *) malloc(sizeof(int));
    }

    // reduntant, but still:
    free(connfd);
    free(msgbuf);
    return 0;
}

// connect_to_server is a convenience function which returns a file descriptor
// to a socket which has been opened and connected to a server with the given
// address and port. It returns a negative int on error.
int connect_to_server(char* addr, int port) {
    int fd;
    struct sockaddr_in saddr;

    // open the socket:
    debug("connect_to_server: opening socket");
    fd = open_socket();
    if (fd < 0) {
        debug("connect_to_server: error opening client socket.");
        return fd;
    }

    // connect to the server:
    debug("connect_to_server: connecting to server.");
    saddr = mksockaddr(addr, port);
    if (connect(fd, (struct sockaddr *) &saddr, SOCKADDR_IN_SIZE) < 0) {
        debug("connect_to_server: error connecting to server.");
        return -2;
    }

    return fd;
}

// open_socket sets up and opens a socket, returning its file descriptor
// or a negative exit code on failure.
int open_socket() {
    int fd;

    fd = socket(
        PF_INET,        // IPv4 protocol family
        SOCK_STREAM,    // bytestream over tpc/ip, unfixed length, blocking
        0               // default protocol (tcp/ip)
    );

    return fd;
}

// mksockaddr creates a struct sockaddr_in with the given address and port
// number. If on the server-side, no address is required.
//
// Example:
//  mksockaddr("13.23.65.18", 5656); // client-side
//  mksockaddr("", 5656);            // server-side
struct sockaddr_in mksockaddr(char* addr, int portno) {
    struct sockaddr_in sockaddr;

    // initialise and apply generic params:
    bzero((char*) &sockaddr, sizeof(struct sockaddr_in));
    sockaddr.sin_port = htons(portno);
    sockaddr.sin_family = AF_INET;  // IPv4

    // setup address:
    if (strcmp(addr, "") == 0) {
        sockaddr.sin_addr.s_addr = INADDR_ANY;
    } else {
        struct hostent* host = gethostbyname(addr);
        sockaddr.sin_addr.s_addr = *((unsigned int *) host -> h_addr_list[0]);
    }

    return sockaddr;
}

// write_to_sock is a convenience function to write from the specified buffer
// to the file descriptor of a socket the given number of bytes.
// It returns the number of bytes written or a negative number on error.
int write_to_sock(int fd, void* buff, int len) {
    int written;
    int total = 0;
    int rest = len;

    while (rest > 0) {
        written = write(fd, buff, rest);
        if (written == -1) {
            return -1;
        } else {
            total = total + written;
            buff = buff + written;
            rest = rest - written;
        }
    }

    return total;
}

// write_str_to_sock is a convenience wrapper around write_to_sock which
// specially handles writing a string to the given socket file descriptor.
// It returns -1 on error or the number of characters of the string written
// (which is bound to be the strlen() + 1 due to the added EOF character).
// See write_to_sock for more details.
int write_str_to_sock(int fd, char* str) {
    return write_to_sock(fd, str, strlen(str) + 1);
}

// read_from_sock is a convenience function which reads from a socket given by
// a file descriptor a given number of bytes into the given buffer.
// It returns the number of bytes read, or a negative number on error.
// To implement some form of application-level termination, it ends whenever a
// read chunk ends in the TERMINATION_CHAR.
int read_from_sock(int fd, void* buff, int len) {
    int rd;
    int total = 0;
    int rest = len;

    while (rest > 0) {
        rd = read(fd, buff, 1);

        if (rd == -1) {
            return -1;
        } else {
            if (rd == 0 || *((char *)buff+rd-1) == TERMINATION_CHAR) {
                break;
            }

            total = total + rd;
            buff = buff + rd;
            rest = rest - rd;
        }
    }

    return total;
}

// read_str_from sock is a wrapper around read_from_sock which is optimized for
// handling strings. It automatically adds an EOF to the result, thus the given
// length should always be the expected length plus one.
// See read_from_sock for more details.
int read_str_from_sock(int fd, char* str, int len) {
    int rd = read_from_sock(fd, str, len);
    if (rd < 0) {
        return -1;
    }

    *(str + rd) = '\0';
    return rd;
}
