#ifndef MQTT_BROKER_UTILS_H
#define MQTT_BROKER_UTILS_H

#include <stdint.h>
#include <stdio.h>

/*!
    \brief Decode fields delivered in connect's payload message
*/
void decode_field_connect ( uint8_t *destination, uint8_t *message, int *message_length );

#endif