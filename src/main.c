#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "tcp_connection.h"
#include "mqtt_client.h"
#include "mqtt_utils.h"

int main ( int argc, char **argv ){

    if ( argc < 3 ){
        fprintf(stderr, "usage: tcp_client hostname port \n");
        return 1;
    }

    /* Define new client */
    struct mqtt_client client;

    struct addrinfo hints;

    establish_tcp_connection(argv[1], argv[2], &client.tcp_socket);

    client.connect_flags = 0x02;
    client.keep_alive_msb = 0x00;
    client.user_name = "tomek";
    client.password = "ddada";
    strcpy(client.client_id, "AAAA");

    struct publish pub_info;
    
    pub_info.message_payload = "25";
    pub_info.topic_name = "temperature";
    pub_info.first_byte = 0b00110000;

    char read[4096];
    char message[100];
    
    int message_length = generate_connect_message(&client,message);

    for ( int i = 0; i < message_length; ++i ){
        printf("%02x", message[i]);
    }
    
    printf("\n");

    int bytes_received = 2;
    bytes_received = send(client.tcp_socket,(char*)&message, message_length, 0);

    bytes_received = 1;
    int total_bytes = 0;

    recv(client.tcp_socket, read, 4096, 0);

    int length = strlen(read);

    for ( int i = 0; i < length; ++i ){
        printf("%02x", read[i]);
    }

    printf("%s%d\n", "Message return code: ", interpret_connack_message(read));

    /*uint8_t message_publish[50];
    length = generate_publish_message(&client, message_publish);*/
    /*send(client.tcp_socket,message_publish,length,0);*/

    publish(&client, &pub_info);


    close(client.tcp_socket);

}