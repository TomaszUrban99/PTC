#ifndef SUBSCRIBE_HH
#define SUBSCRIBE_HH

#define MAX_TOPIC_NUMBER 3

/* Suback return codes */
#define SUCCESS_QOS_0 0x00
#define SUCCESS_QOS_1 0x01
#define SUCCESS_QOS_2 0x02
#define FAILURE 0x80

#include <stdint.h>
#include "topic.hh"

struct subscribe {

    /* Packet identifier */
    uint8_t packet_identifier_lsb;
    uint8_t packet_identifier_msb;

    uint8_t topic_numbers;

    struct topic *subscribed_topics[MAX_TOPIC_NUMBER];
};

#endif