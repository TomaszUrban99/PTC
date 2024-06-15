#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#include <stdint.h>
#include "subscriber.h"

#define TOPIC_MAX_LENGTH 32

struct subscription {

    uint8_t topic[TOPIC_MAX_LENGTH];

    /* Subscriber to the topic */
    struct subscriber *client;

    struct subscription *next;
};

#endif