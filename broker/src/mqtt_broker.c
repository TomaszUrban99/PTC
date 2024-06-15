#include "mqtt_broker.h"

int decode_topic_filters ( struct mqtt_broker *broker, uint8_t *message,
                                                int index, int topic_number, int *begin ){

    /* Calculate length */
    int length = ( message[ *begin ] << 8 | message[ *begin + 1 ] );

    *begin = *begin + 2;

    for ( int i = 0; i < length; ++i ){
        broker->mqtt_clients[index].topic[topic_number].topic_name[i] = message[*begin + i];
    }

    *begin = *begin + length;
    broker->mqtt_clients[index].topic[topic_number].topic_name[length] = '\0';

    broker->mqtt_clients[index].number_of_topics++;


    /* Check if packet is not malformed */
    if ( (message[*begin] & 0x03) > 0x03 ){
        
        fprintf(stderr, "decode_topic_filters(): malformed filter\n");
        /* Save qos */
        broker->mqtt_clients[index].topic[topic_number].qos = FAILURE;
    } else {
        /* Save qos */
        broker->mqtt_clients[index].topic[topic_number].qos = (message[*begin] & 0x03);
    }

    *begin = *begin + 1;

    return 0;
}

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

    int remaining_length = message[1];

    /* Read protocol level */
    if ( message[8] != MQTT_PROTOCOL_LEVEL ){
        return UNACCEPTABLE_PROTOCOL_LEVEL;
    }

    /* Find index of mqtt_client */
    int index = find_corresponding_mqtt_client(broker,client);

    /* Read connect flags */
    broker->mqtt_clients[index].connect_flags = message[9];

    /* ------------------- Clean session handling ------------------------------ */


    /* ------------------------- Will Flag -------------------------------------- */

    /* ------------------------- Will QoS --------------------------------------- */

    if ( broker->mqtt_clients[index].connect_flags & BIT2 == 0x00 )
        broker->mqtt_clients[index].will_qos = 0x00;
    else {
        broker->mqtt_clients[index].will_qos = (broker->mqtt_clients[index].connect_flags & ( BIT3 | BIT4 )) >> 3;
    }

    /* ------------------------- keep alive -------------------------------------- */

    broker->mqtt_clients[index].keep_alive_msb = message[10];
    broker->mqtt_clients[index].keep_alive_lsb = message[11];

    int message_length = 12;

    /* ------------------------ Client ID --------------------------------------------- */
    decode_field_connect( broker->mqtt_clients[index].client_id, message, &message_length);


    /* ----------------------- Save will topic ---------------------------------------- */
    
    if ( broker->mqtt_clients[index].connect_flags & BIT2 == BIT2 ){

        decode_field_connect( broker->mqtt_clients[index].will_topic, message, &message_length );

    /* -------------------- WILL MESSAGE -------------------------------------------------- */

        decode_field_connect( broker->mqtt_clients[index].will_message, message, &message_length );
    }

    /* ------------------ SAVE USER NAME -------------------------------------------------- */

      if ( (broker->mqtt_clients[index].connect_flags & BIT7) == BIT7 ){

        decode_field_connect( broker->mqtt_clients[index].user_name, message, &message_length );
    }

    /* ----------------- SAVE PASSWORD ---------------------------------------------------- */

    if ( (broker->mqtt_clients[index].connect_flags & BIT6) == BIT6 ){

         decode_field_connect( broker->mqtt_clients[index].password, message, &message_length ); 

    }

    broker->mqtt_clients[index].is_connected = 1;

    return CONNECTION_ACCEPTED;

}

int receive_connect ( struct mqtt_broker *broker, struct tcp_client_info *client, uint8_t *message ){

    send_connack(broker,client, interpret_connect(broker,client,message));

    return 0;
}

int receive_subscribe ( struct mqtt_broker *broker, int index, uint8_t *message){

    /* Decode length */
    uint8_t remaining_length = message[1];

    /* Packet identifier */
    broker->mqtt_clients[index].packet_identifier = ( message[2] << 8  | message[3] );

    /* Message length */
    int message_length = 4;

    broker->mqtt_clients[index].number_of_topics = 0;

    while ( message_length < remaining_length + 2){
        
        decode_topic_filters(broker,message,index, 
                    broker->mqtt_clients[index].number_of_topics,&message_length);
    }

    return 0;
}

int send_suback ( struct mqtt_broker *broker, int index ){

    uint8_t suback_message[SUBACK_MESSAGE_LENGTH];

    suback_message[0] = SUBACK_CONTROL_TYPE;
    suback_message[1] = 4 + broker->mqtt_clients[index].number_of_topics - 2;

    /* Packet indentifier MSB */
    suback_message[2] = ( broker->mqtt_clients[index].packet_identifier >> 8 ) & 0xFF;
    /* Packet identifier LSB */
    suback_message[3] = broker->mqtt_clients[index].packet_identifier & 0xFF;

    /* Write QoS */
    for ( int i = 0; i < broker->mqtt_clients[index].number_of_topics; ++i ){
        suback_message[4 + i] = broker->mqtt_clients[index].topic[i].qos;
    }

    if ( send(broker->clients[index].socket,(char*) &suback_message, suback_message[1] + 2, 0) < 0 ){
        fprintf(stderr, "receive_suback(): failed to send message\n" );
        return -1;
    }

    return 0;
}

void mqtt ( struct mqtt_broker *broker, struct tcp_client_info *client, uint8_t *message ){

        switch ( message[0] ){
        
        case CONNECT_CONTROL_TYPE:
            receive_connect(broker, client, message);
        break;

        case SUBSCRIBE_CONTROL_TYPE:

            int index = find_corresponding_mqtt_client(broker, client);

            if ( index < 0 ){
                fprintf(stderr, "Client not on the list of connected mqtt_clients\n");
                return;
            }

            /* If client is on the list receive subscribe */
            receive_subscribe(broker,index,message);

            if ( send_suback(broker,index) < 0 ){
                fprintf(stderr,"Failed to send suback()\n");
                return;
            }

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