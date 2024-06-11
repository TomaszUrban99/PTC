#ifndef MQTT_H
#define MQTT_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "publish.h"
#include "mqtt_utils.h"
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

/* Length of client name */
#define MQTT_CLIENT_ID_LENGTH 10

/* Position of return code in CONNACK message */
#define CONNACK_RETURN_CODE_POSITION 3

/* Length of messages */
#define CONNACK_MESSAGE_LENGTH 5

/* */
struct mqtt_client {

    /* Broker parameters */
        char *ip_address; /* IP address */
        char *port_number; /* Port number */
    
    int tcp_socket;

    char client_id[MQTT_CLIENT_ID_LENGTH];
    
    char *will_topic;
    char *topic;
    
    /* Username and password of MQTT client */
        char *user_name;
        char *password;

    /* ----------------- Connect flags ------------------

        bit |   Flag
        ------------------------------------
        7   |   User Name flag 
        6   |   Password flag
        5   |   Will Retain
        4   |
        3   |   Will QoS
        2   |   Will Flag
        1   |   Clean Session
        0   |   Reserved

    */
    int connect_flags;

    /* Time interval measured in seconds */
    uint8_t keep_alive_msb;
    uint8_t keep_alive_lsb;

};

int generate_connect_message ( struct mqtt_client *client, uint8_t message[] );

/*!
    \brief Connect MQTT client to broker
*/
int connect_mqtt ( struct mqtt_client *client );


/* ----------------- Connect Return Code --------------------- 

    Value   |   Return Code |   Description
    -------------------------------------------------------------------------------------
    0       |       0x00    |   Connection Accepted
    1       |       0x01    |   Connection Refused- unacceptable protocol version
    2       |       0x02    |   Connection Refused- Client ID is correct but not allowed
    3       |       0x03    |   Server unavailable, MQTT service unavailable
    4       |       0x04    |   Bad user name or password
    5       |       0x05    |   Client not authorized to connect
    6-255   |               |   Reserved for future use
*/
#define CONNECTION_ACCEPTED 0
#define REFUSED_PROTOCOL_VERSION 1
#define REFUSED_ID_NOT_ALLOWED 2
#define REFUSED_SERVER_UNAVAILABLE 3
#define REFUSED_BAD_USER_OR_PASSWORD 4
#define REFUSED_CLIENT_NOT_AUTHORIZED 5

/*!
    \brief Interpret connack message (based on return code)
*/
int interpret_connack_message ( uint8_t *response );

/*!
    \brief Receive connack message
*/
int receive_connack ( struct mqtt_client *client );

/*!
    \brief Generate publish message

    \par struct mqtt_client *client
    \par uint8_t *message

    \retval Return length of publish message
*/
int generate_publish_message (  struct mqtt_client *client,
                                struct publish *pub_info, uint8_t *message );

/*!
    \brief Publish message on a given topic

    \par struct mqtt_client *client - MQTT client that wants to publish on a given topic
    \par char *topic - topic to publish
    \par char *message - message to be published
*/
int publish ( struct mqtt_client *client, struct publish *pub_info );

/*!
    \brief Receive publish acknowledgment
*/
int receive_puback ( struct mqtt_client *client, struct publish *pub_info );



#endif