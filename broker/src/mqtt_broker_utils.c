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

struct subscription_clients find_subscription_client ( struct mqtt_client_info *clients, int length_of_clients, uint8_t *topic){

    struct subscription_clients cli;
    cli.number_of_clients = 0;

    for ( int i = 0; i < length_of_clients; ++i )
    {
        for ( int j = 0; j < clients[i].number_of_topics; ++j ){
            
            if(!strcmp(clients[i].topic[j].topic_name,topic)){
                cli.index[cli.number_of_clients] = i;
                cli.number_of_clients++;
                break;
            }
                
        }
    }

    return cli;
}