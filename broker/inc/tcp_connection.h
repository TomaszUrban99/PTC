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
#include <stdlib.h>

#include "tcp_client_info.h"

#define SOCKET_FAILED 1
#define CONNECT_FAILED 2

#define MAX_MESSAGE_LENGTH 2048

int establish_tcp_connection ( char *ip_address, char *port, int *tcp_socket );

int create_socket ( char *ip_address, int *listen_socket, const char *port );

fd_set wait_on_clients ( int *listen_socket, struct tcp_client_info *clients );

struct tcp_client_info *get_client ( int socket, struct tcp_client_info *clients );

const char *get_client_address ( struct tcp_client_info *ci );

void drop_client ( struct tcp_client_info *client, struct tcp_client_info *clients );

/*!
    \brief Accept new connection
*/
int accept_new_connection ( struct tcp_client_info *client, fd_set *master, int *max_socket, int *listen_socket );

/*!
    \brief Receive TCP message
*/
int receive_message ( int *client_socket, uint8_t *message, int message_length );

#endif