#ifndef TCP_CLIENT_INFO_H
#define TCP_CLIENT_INFO_H

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

#define MAX_REQUEST_SIZE 2048

struct tcp_client_info {

    socklen_t address_length;
    struct sockaddr_storage address;
    int socket;
    char request[MAX_REQUEST_SIZE + 1];
    int received;
    struct tcp_client_info *next;
};

#endif