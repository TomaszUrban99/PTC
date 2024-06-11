#ifndef MQTT_UTILS_H
#define MQTT_UTILS_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>

void code_field_connect_message ( uint8_t *message_to_send, int *message_length, 
                                        uint8_t *field_data );

#endif