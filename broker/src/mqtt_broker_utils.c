#include "mqtt_broker_utils.h"

void decode_field_connect ( uint8_t *destination, uint8_t *message, int *message_length ){

    /* Decode length of the field */
    int length = ( message[*message_length] << 8 | message[*message_length + 1] );

    for ( int i = 0; i < length; ++i ){
        destination[i] = message[*message_length + 2 + i];
    }

    message_length = message_length + length;
    destination[length] = '\0';

}