#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main ( int argc, char **argv ){

    if ( argc < 3 ){
        fprintf(stderr, "usage: tcp_client hostname port \n");
        return 1;
    }

    printf("Konfiguracja adresu serwera...\n");
    
    /* Structure for holding server data */
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    /* SOCK_STREAM - we would like to use TCP connection */
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *peer_address;

    /* If everything is correct, remote address will be in peer_address */
    if ( getaddrinfo( argv[1], argv[2], &hints, &peer_address)){
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
    int socket_peer;
    socket_peer = socket( peer_address->ai_family,
            peer_address->ai_socktype, peer_address->ai_protocol );
    
    if ( socket_peer < 0){
        fprintf(stderr, "socket() failed. (%d)\n", 2);
        return 1;
    }

    printf("Nawiazywanie polaczenia...\n");

    if ( connect(socket_peer,
        peer_address->ai_addr, peer_address->ai_addrlen)){
        fprintf(stderr, "connect() failed. (%d) \n", 2);
        return 1;
    }

    freeaddrinfo(peer_address);

    printf("Polaczono\n");

    uint8_t message_to_send[] = {0x10, 0x0f, 0x00, 0x04, 0x4d, 0x51, 0x54, 0x54, 0x04, 0x00, 0x00, 0x0a, 0x00, 0x03, 0x41, 0x42, 0x43};
    char read[4096];

    int bytes_received = 2;

    bytes_received = send(socket_peer,(char*)&message_to_send, sizeof(message_to_send), 0);

    printf("%s %d\n", "Wyslano: ", bytes_received);

    bytes_received = 1;

    int length = strlen(read);

    while (bytes_received > 0)
    {
        bytes_received = recv(socket_peer, read, 4096, 0);

        length = strlen(read);

        for ( int i = 0; i < length; ++i ){
            printf("%02x", read[i]);
        }
        printf("\n");
        printf("%d\n", bytes_received);
    }

    printf("\n");

    printf("Konczenie polaczenia\n");
    
    close(socket_peer);

}