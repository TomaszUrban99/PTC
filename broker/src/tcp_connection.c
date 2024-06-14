#include "tcp_connection.h"

int establish_tcp_connection ( char *ip_address, char *port, int *tcp_socket ){

    printf("Konfiguracja adresu serwera...\n");
    
    /* Structure for holding server data */
        
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        
        /* SOCK_STREAM - we would like to use TCP connection */
        hints.ai_socktype = SOCK_STREAM;
        struct addrinfo *peer_address;

    /* If everything is correct, remote address will be in peer_address */
        if ( getaddrinfo( ip_address, port, &hints, &peer_address)){
            fprintf(stderr, "getaddrinfo() failed. (%d)\n", 2);
            return 1;
        }

    /* Display message */
        printf("Adres serwera do połączenia: \n");
    
        char address_buffer[100];
        char service_buffer[100];
    
        getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
            address_buffer, sizeof(address_buffer),
            service_buffer, sizeof(service_buffer),
            0);
    
        printf("%s %s\n", address_buffer, service_buffer);
    
    /* Tworzenie gniazda do połączenia */
        printf("Tworzenie gniazda...\n");

        *tcp_socket = socket( peer_address->ai_family,
            peer_address->ai_socktype, peer_address->ai_protocol );
    
        if ( *tcp_socket < 0){
            fprintf(stderr, "socket() failed. (%d)\n", 2);
            return SOCKET_FAILED;
        }

    printf("Nawiazywanie polaczenia...\n");
        
        if ( connect(*tcp_socket,
            peer_address->ai_addr, peer_address->ai_addrlen)){
                fprintf(stderr, "connect() failed. (%d) \n", 2);
                return CONNECT_FAILED;
        }

    freeaddrinfo(peer_address);
    printf("Polaczono\n");
}

int create_socket ( char *ip_address, int *listen_socket, const char *port ){

    printf("Configuring local address...\n");
    
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    struct addrinfo *bind_address;
    getaddrinfo(ip_address, port, &hints, &bind_address);

    *listen_socket = socket(bind_address->ai_family,
         bind_address->ai_socktype, bind_address->ai_protocol);
    
    if (*listen_socket < 0) {
     fprintf(stderr, "socket() failed \n");
     exit(1);
    }
    
    printf("Binding socket to local address...\n");
    if (bind(*listen_socket,
             bind_address->ai_addr, bind_address->ai_addrlen)) {
     fprintf(stderr, "bind() failed\n");
     exit(1);
    }
    
    freeaddrinfo(bind_address);
    printf("Listening...\n");
    
    if (listen(*listen_socket, 0) < 0) {
     fprintf(stderr, "listen() failed \n");
     exit(1);
    }
 
    return *listen_socket;
}

fd_set wait_on_clients ( int *listen_socket, struct tcp_client_info *clients ){
    
    fd_set reads;
    
    FD_ZERO(&reads);
    FD_SET(*listen_socket, &reads);
    int max_socket = *listen_socket;

    struct tcp_client_info *ci = clients;

    while(ci) {

        FD_SET(ci->socket, &reads);

        if (ci->socket > max_socket){
            max_socket = ci->socket;
        }
        ci = ci->next;
    }

    if (select(max_socket+1, &reads, 0, 0, 0) < 0) {
        fprintf(stderr, "select() failed \n");
        exit(1);
    }

    return reads;
}

struct tcp_client_info *get_client ( int socket, struct tcp_client_info *clients ){
    
    struct tcp_client_info *ci = clients;

    while (ci)
    {
        if ( ci->socket == socket )
            break;
        
        ci = ci->next;
    }

    if (ci) return ci;

    struct tcp_client_info *n = 
        (struct tcp_client_info*) calloc (1, sizeof(struct tcp_client_info));

    if ( !n ) {
        fprintf( stderr, "Out of memory\n" );
        exit(1);
    }

    n->address_length = sizeof(n->address);
    n->next = clients;
    clients = n;

    return n;

}

const char *get_client_address ( struct tcp_client_info *ci ){
    
    static char address_buffer[100];
    
    getnameinfo((struct sockaddr*) &ci->address, ci->address_length,
        address_buffer, sizeof(address_buffer), 0, 0, 0);

    return address_buffer;
}

void drop_client ( struct tcp_client_info *client, struct tcp_client_info *clients ){


    close(client->socket);

    struct tcp_client_info **p = &clients;

    while (*p){

        if ( *p == client ){
            *p = client->next;
            free(client);
            return;
        }

        p = &(*p)->next;
    }

    fprintf(stderr, "drop_client(): not found. \n");
    exit(1);
}

int accept_new_connection ( struct tcp_client_info *client, fd_set *master, int *max_socket, int *listen_socket ){

    int i = 0;

    while ( i < 10 && client[i].is_connected){
        ++i;
    }

    client->address_length = sizeof(client->address);
    client->socket = accept(*listen_socket, (struct sockaddr*) &client->address, &client->address_length);
 
    if (client->socket < 0) {
        fprintf(stderr, "accept() failed\n");
        return 1;
    }
    
    /* Add new client to master set */
    FD_SET(client->socket, master);
 
    if (client->socket > *max_socket)
        *max_socket = client->socket;
    
    char address_buffer[100];
    getnameinfo((struct sockaddr*)&client->address, client->address_length,
         address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
    
    client->is_connected = 1;
    
    printf("New connection from %s\n", address_buffer);

    return 0;
}

int receive_message ( int *client_socket, uint8_t *message, int message_length ){

    int received_bytes = recv( *client_socket, message, message_length, 0);

    if ( received_bytes < 1 ){
        fprintf(stderr,"receive message(): failed to receive message\n");
        return -1;
    }

    return received_bytes;
}
