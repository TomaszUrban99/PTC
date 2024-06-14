#ifndef MQTT_CLIENT_INFO_H
#define MQTT_CLIENT_INFO_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "topic.hh"
#include "tcp_client_info.h"

#define MQTT_CLIENT_ID_LENGTH 10
#define MQTT_USER_NAME_LENGTH 32
#define MQTT_PASSWORD_NAME_LENGTH 32
#define MQTT_WILL_TOPIC 32
#define MQTT_WILL_MESSAGE 32

#define MAX_NUMBER_OF_TOPICS 3

struct mqtt_client_info {

      int is_connected;

       /*char *ip_address; /* IP address */
       /*char *port_number; /* Port number */
   
        int tcp_socket;
        char client_id[MQTT_CLIENT_ID_LENGTH];
   
        uint8_t will_topic[MQTT_WILL_TOPIC];
        uint8_t will_message[MQTT_WILL_MESSAGE];
        
        int will_qos;

      int number_of_topics;

      /* Subscribed topics */
      struct topic topic[MAX_NUMBER_OF_TOPICS];
   
    /* Username and password of MQTT client */
       char user_name[MQTT_USER_NAME_LENGTH];
       char password[MQTT_PASSWORD_NAME_LENGTH];
   
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
        uint8_t connect_flags;
        
        /* Time interval measured in seconds */
        uint8_t keep_alive_msb;
        uint8_t keep_alive_lsb;

        struct mqtt_client_info *next;
    
};

#endif