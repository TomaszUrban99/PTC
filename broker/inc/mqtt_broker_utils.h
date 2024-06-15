#ifndef MQTT_BROKER_UTILS_H
#define MQTT_BROKER_UTILS_H

#include <stdint.h>
#include <stdio.h>

#include "mqtt_client_info.h"

struct subscription_clients {

    int number_of_clients;
    int index[10];
};

/*!
    \brief Decode fields delivered in connect's payload message
*/
void decode_field_connect ( uint8_t *destination, uint8_t *message, int *message_length );

/*!
    \brief Find client with subscription to given topic
*/
struct subscription_clients find_subscription_client ( struct mqtt_client_info *clients, int length_of_clients, uint8_t *topic);

#endif