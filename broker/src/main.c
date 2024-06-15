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

    /* Initialize clients */
    
    struct mqtt_broker broker;
    
    broker.port = argv[1];
    broker.ip_address = "127.0.0.1";
    broker.connected_clients_number = 0;
    broker.subs = NULL;
    
    /* Create listening socket */
    create_socket(broker.ip_address, &broker.listen_socket, broker.port);

    printf("Waiting for connections...\n");

    /* -------------------- Set of sockets ----------------------------- */
    fd_set master;
    FD_ZERO(&master);
    FD_SET(broker.listen_socket, &master);
    FD_SET(0, &master);
    
    int max_socket = broker.listen_socket;

    while (1) {

        fd_set reads;
        fd_set writers;

        reads = master;
        writers = master;

        if ( select ( max_socket + 1, &reads, &writers, 0, 0) < 0 ) {
            fprintf(stderr, "select(): failed\n" );
            return 1;
        }

        int i;

        if(FD_ISSET(0, &reads)) {
            
            char input[2];
            if (!fgets(input, 2, stdin)) break;

            delete_subscriber(&broker);

            return 0;
        }

        for ( int i = 1; i <= max_socket; ++i ){
            
            if ( FD_ISSET(i, &reads)){

                if (i == broker.listen_socket) {

                    struct tcp_client_info *client = find_free_tcp_client(&broker);

                    accept_new_connection( client,
                        &master, &max_socket, &broker.listen_socket);  
                }
                else {    
                     
                    char buffer_message[MAX_MESSAGE_LENGTH];
                    struct tcp_client_info *client = find_tcp_client(&broker,i);

                    int received_bytes = recv(client->socket, buffer_message, MAX_MESSAGE_LENGTH,0);
                    
                    /*if ( received_bytes  < 1 ){ 
                        
                        client->is_connected = 0;
                        FD_CLR(i, &master);
                        close(i);

                        continue;
                    }*/
                   
                    mqtt(&broker, client, buffer_message);
                
                }
            }
        }
    }

    return 0;
}
