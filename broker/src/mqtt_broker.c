#include "mqtt_broker.h"

int find_corresponding_mqtt_client (  struct mqtt_broker *broker, struct tcp_client_info *client ){

    for ( int i = 0; i < 10; ++i ){
        
        if ( client->socket == broker->clients[i].socket )
            return i;
    }

    return -1;
}

int send_connack ( struct mqtt_broker *broker, struct tcp_client_info *client, uint8_t return_code ){

    uint8_t connack_message[CONNACK_MESSAGE_LENGTH];

    connack_message[0] = CONNACK_CONTROL_TYPE;
    connack_message[1] = CONNACK_MESSAGE_LENGTH - 2;
    connack_message[2] = 0x00;
    connack_message[3] = return_code;

    int bytes_send = send(client->socket, (char*) &connack_message, CONNACK_MESSAGE_LENGTH, 0);

    printf("%s%d\n", "Bytes send: ", bytes_send);

    if ( bytes_send < CONNACK_MESSAGE_LENGTH){
        fprintf(stderr,"send_connack(): failed to send connack message\n");
        return -1;
    }

    return 0;
}


uint8_t *get_client_id ( uint8_t *message){


}

struct tcp_client_info *find_tcp_client ( struct mqtt_broker *broker, int tcp_socket ){
    
    for ( int i = 0; i < 10; ++i ){

        if ( tcp_socket == broker->clients[i].socket )
            return &broker->clients[i];
    }

    return NULL;
}

struct tcp_client_info *find_free_tcp_client ( struct mqtt_broker *broker ){

    for ( int i = 0; i < 10; ++i ){
        if ( !broker->clients[i].is_connected )
            return &broker->clients[i];
    }
}

int generate_connack ( struct mqtt_broker *broker, uint8_t *message ){


}

int interpret_connect ( struct mqtt_broker *broker, struct tcp_client_info *client, uint8_t *message ){

    int remaining_length = message[1] + '0';
    printf("%s%d\n", "Rem: ", remaining_length);

    /* Read protocol level */
    if ( message[6] != MQTT_PROTOCOL_LEVEL ){
        return UNACCEPTABLE_PROTOCOL_LEVEL;
    }

    int index = find_corresponding_mqtt_client(broker,client);


}

int receive_connect ( struct mqtt_broker *broker, struct tcp_client_info *client){

    send_connack(broker,client, CONNECTION_ACCEPTED);

    return 0;
}

void mqtt ( struct mqtt_broker *broker, struct tcp_client_info *client, uint8_t *message ){

        switch ( message[0] ){
        
        case CONNECT_CONTROL_TYPE:
            receive_connect(broker, client, message);
        break;

        case SUBSCRIBE_CONTROL_TYPE:

        break;

        case PUBLISH_CONTROL_TYPE:

        break;

        case PINGEQ_CONTROL_TYPE:

        break;

        case DISCONNECT_CONTROL_TYPE:

        break;

        default:
        return;
        }
}