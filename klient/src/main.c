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

    connect_mqtt(&client);

    struct topic tp1;
    tp1.qos = LEVEL_QOS_0;
    tp1.topic_name = "temperature";
    tp1.topic_length_lsb = strlen(tp1.topic_name);
    tp1.topic_length_msb = 0;

     printf("%s%d\n", "temp11: ", tp1.topic_length_lsb);
    
    struct topic tp2;
    tp2.qos = LEVEL_QOS_0;
    tp2.topic_name = "temperature22";
    tp2.topic_length_lsb = strlen(tp2.topic_name);
    tp2.topic_length_msb = 0;

    printf("%s%d\n", "temp22: ", tp2.topic_length_lsb);

    struct subscribe ss;
    ss.packet_identifier_lsb = 10;
    ss.packet_identifier_msb = 0;
    ss.topic_numbers = 2;

    ss.subscribed_topics[0] = &tp1;
    ss.subscribed_topics[1] = &tp2;

    uint8_t subscribe_message[256];

    subscribe(&client,&ss);
    printf("%s%d\n", "suback code(): ", receive_suback(&client,&ss));

    close(client.tcp_socket);

}