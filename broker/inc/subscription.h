#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "subscriber.h"

#define TOPIC_MAX_LENGTH 32

struct subscription {

    uint8_t topic[TOPIC_MAX_LENGTH];

    /* Subscriber to the topic */
    struct subscriber *client;

    struct subscription *next;
};


void add_subscriber ( struct subscriber **subs, int index, int qos, int packet_identifier );

/*!
    \brief Add new subscription
*/
void add_subscription ( struct subscription **sub, uint8_t *topic, int index, int qos, int packet_identifier );

int delete_subscription ( struct subscription **sub, uint8_t *topic, int index, int packet_identifier );

/*!
    \brief Delete subscriber with index and packet_identifier from subscription
*/
void delete_subscriber ( struct subscription **sub, int index, int packet_identifier );

void delete_subscribers ( struct subscription **subs );

void delete_subscriptions ( struct subscription **subs );

void print_subscriptions ( struct subscription *subs );


#endif