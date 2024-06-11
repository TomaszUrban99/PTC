#ifndef PUBLISH_H
#define PUBLISH_H

#include <stdint.h>

/* QoS states 

    QoS - level of assurance
      0   |   0   |   at most once delivery
      0   |   1   |   at least once delivery
      1   |   0   |   exactly once delivery
      1   |   1   |   reserved- must not be used

*/
#define QOS_AT_MOST_ONCE 0b00000000;
#define QOS_AT_LEAST_ONCE   0b00000010;
#define QOS_EXACTLY_ONCE   0b00000100;

#define RETAIN 0b00000001;
#define NOT_RETAIN 0b00000000;

struct publish {

    /* 
        MQTT Control Packet type 7-4 bits
        DUP flag    3 bit
        QoS level   2-1 bit
        RETAIN 0 bit
    */
    uint8_t first_byte;

    /* Topic name to publish message */
    char *topic_name;

    /* Only present if QoS set to 1 or 2*/
    uint8_t packet_identifier;

    /* Stores message payload */
    char *message_payload;

};

#endif