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
            NI_NUMERICHOST);
    
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