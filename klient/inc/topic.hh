#ifndef TOPIC_HH
#define TOPIC_HH

#include <stdint.h>

struct topic {

    uint8_t qos;

    uint8_t topic_length_msb;
    uint8_t topic_length_lsb;

    char *topic_name;    
};

#endif