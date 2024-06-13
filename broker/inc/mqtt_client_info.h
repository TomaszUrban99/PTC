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

#define MQTT_CLIENT_ID_LENGTH 10

struct mqtt_client_info {

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

#endif