#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "mqtt_broker.h"
#include "tcp_connection.h"

int main ( int argc, char **argv ){

    struct tcp_client_info *clients = 0;
    /*clients.next = NULL;*/

    struct mqtt_broker broker;
    broker.port = argv[1];
    broker.ip_address = "0";

    /* Create listening socket */
    create_socket(broker.ip_address, &broker.listen_socket, broker.port);

    /* Set of sockets */
    fd_set master;
    FD_ZERO(&master);
    FD_SET(broker.listen_socket, &master);

    int max_socket = broker.listen_socket;

    printf("Waiting for connections...\n");

    while(1) {
        
        fd_set reads;
        printf("%s\n", "ssss");
        reads = wait_on_clients(&broker.listen_socket,clients);
        printf("%s\n", "sss222s");

        if ( FD_ISSET(broker.listen_socket,&reads) ){
            struct tcp_client_info *client = get_client(-1,clients);
            printf("%s\n", "sss233s");
            client->socket = accept( broker.listen_socket, 
                (struct sockaddr*) &(client->address), &(client->address_length));

            if ( client->socket < 0 ){
                fprintf(stderr, "accept(): failed \n");
                return 1;
            }

            printf("New connection from %s. \n",
                get_client_address(client));
        }

        struct tcp_client_info *client;
        client = clients;

        while(client){

            struct tcp_client_info *next = client->next;

            if ( FD_ISSET(client->socket, &reads)){

                int bytes_received = recv(client->socket,
                    client->request + client->received, MAX_REQUEST_SIZE-client->received, 0 );

                if ( bytes_received < 1 ){
                    printf("Unexpected disconnect from %s. \n", get_client_address(client));
                    drop_client(client, clients);
                }
                else{
                    client->received += bytes_received;
                }
            }

            client = next;
        }
    }

    printf("Closing listening socket...\n");
    close(broker.listen_socket);

        printf("Finished.\n");
    return 0;
}
