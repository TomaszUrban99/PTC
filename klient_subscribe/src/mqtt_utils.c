#include "mqtt_utils.h"

void code_field_connect_message ( uint8_t *message_to_send, int *message_length, uint8_t *field_data ){

    int length = strlen(field_data);

    printf("%s\n", field_data);
    
    message_to_send[*message_length] = 0;
    message_to_send[*message_length+1] = length;

    *message_length = *message_length + 2;
    
    for ( int j = 0; j < length; ++j ){
        message_to_send[*message_length+j] = field_data[j];
    }
    
    *message_length = *message_length + length;
}
