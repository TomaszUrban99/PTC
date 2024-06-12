#ifndef TOPIC_HH
#define TOPIC_HH

#include <stdint.h>

struct topic {

    uint8_t qos;
    
    /* 
        Server return codes to given topic 
        (send by SERVER in SUBACK message)
    */
    uint8_t topic_return_code;

    uint8_t topic_length_msb;
    uint8_t topic_length_lsb;

    char *topic_name;

    uint8_t is_subscribed;
};

#endif