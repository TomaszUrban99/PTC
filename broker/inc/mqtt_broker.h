#ifndef MQTT_H
#define MQTT_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "mqtt_broker_utils.h"
#include "mqtt_client_info.h"
#include "tcp_connection.h"

/* Bits */
#define BIT0 0b00000001
#define BIT1 0b00000010
#define BIT2 0b00000100
#define BIT3 0b00001000
#define BIT4 0b00010000
#define BIT5 0b00100000
#define BIT6 0b01000000
#define BIT7 0b10000000

/* Control packet type */
#define CONNECT_CONTROL_TYPE 0x10
#define CONNACK_CONTROL_TYPE 0x20

#define PUBLISH_CONTROL_TYPE 0x30
#define PUBACK_CONTROL_TYPE 0x40
#define PUBREC_CONTROL_TYPE 0x50
#define PUBREL_CONTROL_TYPE 0x60
#define PUBCOMP_CONTROL_TYPE 0x70

#define SUBSCRIBE_CONTROL_TYPE 0x82
#define SUBACK_CONTROL_TYPE 0x90
#define UNSUBSCRIBE_CONTROL_TYPE 0xa2
#define UNSUBACK_CONTROL_TYPE 0xb0

#define PINGEQ_CONTROL_TYPE 0xc0
#define PINGRESP_CONTROL_TYPE 0xd0

#define DISCONNECT_CONTROL_TYPE 0Xe0

/* Position of return code in CONNACK message */
#define CONNACK_RETURN_CODE_POSITION 3

/* Length of messages */
#define CONNECT_MESSAGE_LENGTH 256
#define CONNACK_MESSAGE_LENGTH 5

#define PUBACK_MESSAGE_LENGTH 4
#define PUBREC_MESSAGE_LENGTH 4
#define PUBREL_MESSAGE_LENGTH 4
#define PUBCOMP_MESSAGE_LENGTH 4

#define SUBSCRIBE_MESSAGE_LENGTH 256
/* Look at maximum number of topics: #define MAX_TOPIC_NUMBER 3 in subscribe.h */
#define SUBACK_MESSAGE_LENGTH 10 
#define UNSUBSCRIBE_MESSAGE_LENGTH 128
#define UNSUBACK_MESSAGE_LENGTH 4

#define PINGREQ_MESSAGE_LENGTH 2
#define PINGRESP_MESSAGE_LENGTH 2

#define DISCONNECT_MESSAGE_LENGTH 2

#define MQTT_PROTOCOL_LEVEL 0x04

/* Level of QoS */
#define LEVEL_QOS_0 0
#define LEVEL_QOS_1 1
#define LEVEL_QOS_2 2

#define UNACCEPTABLE_PROTOCOL_LEVEL 0x01

/* */
struct mqtt_broker {

    char *ip_address;
    char *port;

    int listen_socket;

    /* Number of connected clients */
    int connected_clients_number;

    struct tcp_client_info clients[10];

    /* Pointer to the first connected client */
    struct mqtt_client_info mqtt_clients[10];

};

#define CONNECTION_ACCEPTED 0
#define REFUSED_PROTOCOL_VERSION 1
#define REFUSED_ID_NOT_ALLOWED 2
#define REFUSED_SERVER_UNAVAILABLE 3

int decode_topic_filters ( struct mqtt_broker *broker, uint8_t *message,
                                                        int index, int topic_number, int *begin );

uint8_t *get_client_id ( uint8_t *message);

int find_corresponding_mqtt_client ( struct mqtt_broker *broker, struct tcp_client_info *client );

struct tcp_client_info *find_tcp_client ( struct mqtt_broker *broker, int tcp_socket );

struct tcp_client_info *find_free_tcp_client ( struct mqtt_broker *broker );

int generate_connack ( struct mqtt_broker *broker, uint8_t *message );

int send_connack ( struct mqtt_broker *broker, struct tcp_client_info *client, uint8_t return_code );

int interpret_connect ( struct mqtt_broker *broker, struct tcp_client_info *client, uint8_t *message );

int receive_connect ( struct mqtt_broker *broker, struct tcp_client_info *client, uint8_t *message);

/*!
    \brief Receive subscribe message
*/
int receive_subscribe ( struct mqtt_broker *broker, int index, uint8_t *message);

/*!
    \brief Main function for providing mqtt service
*/
void mqtt ( struct mqtt_broker *broker, struct tcp_client_info *client, uint8_t *message );

#endif