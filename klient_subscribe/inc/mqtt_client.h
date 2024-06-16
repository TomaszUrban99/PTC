#ifndef MQTT_H
#define MQTT_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "publish.h"
#include "subscribe.h"
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

/* Control packet type */
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

/* Length of client name */
#define MQTT_CLIENT_ID_LENGTH 10

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

/* Level of QoS */
#define LEVEL_QOS_0 0
#define LEVEL_QOS_1 1
#define LEVEL_QOS_2 2

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

int receive_publish ( struct mqtt_client *client );

/*!
    \brief Check correctness of puback message and information contained inside
*/
int interpret_puback ( struct publish *pub_info, uint8_t *puback_message);

/*!
    \brief Receive publish acknowledgment
*/
int receive_puback ( struct mqtt_client *client, struct publish *pub_info );

/*!
    \brief Check and interpret pubrec message
*/
int interpret_pubrec ( struct publish *pub_info, uint8_t *pubrec_message );

/*!
    \brief Receive PUBREC publish receivde message
*/
int receive_pubrec ( struct mqtt_client *client, struct publish *pub_info );

/*!
    \brief Generate pubrel message
*/
void generate_pubrel_message ( struct publish *pub_info, uint8_t *pubrel_message );

/*!
    \brief Receive pubcomp message
*/
int receive_pubcomp ( struct mqtt_client *client, struct publish *pub_info );



/*!
    \brief Generate subscribe message
    
    \retval Length of subscribe message
*/
int generate_subscribe_message ( struct mqtt_client *client, 
                                    struct subscribe *sub_info, uint8_t *subscribe_message );

/*!
    \brief Interpret SUBACK response
*/
int interpret_suback_message ( struct subscribe *sub_info, uint8_t *suback_message, int received_bytes );

/*!
    \brief Receive suback message

    According to MQTT v3.1.1 on subscribe message, server must response with 
    SUBACK message
*/
int receive_suback ( struct mqtt_client *client, struct subscribe *sub_info );

/*!
    \brief Subscribe
*/
int subscribe ( struct mqtt_client *client, struct subscribe *sub_info );

/*!
    \brief Generate unsubscribe message
*/
int generate_unsubscribe ( struct subscribe *sub_info, 
    uint8_t *unsubscribe_message, int topic_index );

/*!
    \brief Unsubscribe
*/
int unsubscribe ( struct mqtt_client *client, struct subscribe *sub_info, int topic_index );

int interpret_unsuback_message ( struct subscribe *sub_info, uint8_t *unsuback_message, int topic_index);

/*!
    \brief Receive unsuback
*/
int receive_unsuback ( struct mqtt_client *client, struct subscribe *sub_info, int topic_index );

/*!
    \brief Send ping request 
*/
int pingreq ( struct mqtt_client *client );

/*!
    \brief Receive ping response
*/
int receive_pingresponse ( struct mqtt_client *client );

/*!
    \brief Send_disconnect message
*/
int send_disconnect ( struct mqtt_client *client );

/*!
    \brief Disconnect from server
*/
int disconnect ( struct mqtt_client *client );

#endif