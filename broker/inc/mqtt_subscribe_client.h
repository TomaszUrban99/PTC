#ifndef MQTT_SUBSCRIBE_CLIENT_H
#define MQTT_SUBSCRIBE_CLIENT_H

#define MAX_TOPIC_NUMBER 3

struct mqtt_subscribe_client {

/* Packet identifier */
    uint8_t packet_identifier_lsb;
    uint8_t packet_identifier_msb;
  
    uint8_t topic_numbers;
    struct topic *subscribed_topics[MAX_TOPIC_NUMBER];
};

#endif