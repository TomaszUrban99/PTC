#ifndef SUBSCRIBE_HH
#define SUBSCRIBE_HH

#define MAX_TOPIC_NUMBER 3

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