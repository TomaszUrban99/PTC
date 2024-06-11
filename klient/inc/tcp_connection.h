#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#define SOCKET_FAILED 1
#define CONNECT_FAILED 2

int establish_tcp_connection ( char *ip_address, char *port, int *tcp_socket );

#endif