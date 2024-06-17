#include "mqtt_broker.h"

int decode_topic_filters ( struct mqtt_broker *broker, uint8_t *message,
                                                int index, int packet_identifier, int *begin ){

    uint8_t topic[MAX_TOPIC_LENGTH];

    /* Calculate length */
    int length = ( message[ *begin ] << 8 | message[ *begin + 1 ] );

    *begin = *begin + 2;

    for ( int i = 0; i < length; ++i ){
        topic[i] = message[*begin + i];
    }

    *begin = *begin + length;
    topic[length] = '\0';

    /* Check if packet is not malformed */
    if ( (message[*begin] & 0x03) > 0x03 ){
        fprintf(stderr, "decode_topic_filters(): malformed filter\n");
    } else {
        add_subscription(&broker->subs, topic, index, (message[*begin] & 0x03), packet_identifier);
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

int receive_subscribe ( struct mqtt_broker *broker, int index,  int *packet_identifier, uint8_t *message){

    /* Decode length */
    uint8_t remaining_length = message[1];

    /* Packet identifier */
    *packet_identifier = ( message[2] << 8  | message[3] );

    /* Message length */
    int message_length = 4;

    broker->mqtt_clients[index].number_of_topics = 0;

    while ( message_length < remaining_length + 2){

        /* Topic */
        uint8_t topic[MAX_TOPIC_LENGTH];

        decode_topic_filters(broker, message,index, 
                    *packet_identifier ,&message_length);

        broker->mqtt_clients[index].number_of_topics++;
    }

    return 0;
}

int send_suback ( struct mqtt_broker *broker, int index, int packet_identifier ){

    uint8_t suback_message[SUBACK_MESSAGE_LENGTH];

    suback_message[0] = SUBACK_CONTROL_TYPE;
    suback_message[1] = 4 + broker->mqtt_clients[index].number_of_topics - 2;

    /* Packet indentifier MSB */
    suback_message[2] = ( packet_identifier >> 8 ) & 0xFF;
    /* Packet identifier LSB */
    suback_message[3] = packet_identifier & 0xFF;

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

int publish_to_clients ( struct mqtt_broker *broker, uint8_t *topic_to_publish, uint8_t *message_to_publish  ){

    /* Search through list of subscribed topics */

    /* Temporary pointer for iterating thorugh list */
    struct subscription *temp = broker->subs;
    int length = message_to_publish[1] +2;

    for ( int i = 0; i < length; ++i ){
        printf("%02x", message_to_publish[i]);
    }
    printf("\n");

    while ( temp ){

        if ( strcmp(temp->topic, topic_to_publish) == 0 ){
            struct subscriber *sub_client = temp->client;

            while ( sub_client ){

                send ( broker->clients[sub_client->index].socket, message_to_publish, length, 0);
                sub_client = sub_client->next;
            }
        }

        temp = temp->next;
    }

    return 0;
}

int receive_publish ( struct mqtt_broker *broker, int index, uint8_t *message ){

    /* Save remaining message length */
    int remaining_length = message[1];

    /* ----------------- DUP flag --------------------------------------- */

    /* ------------------ QoS level ------------------------------------- */

    /* ------------------ Will Retain ----------------------------------- */

    /* Read length of topic */
    int topic_length = ( message[2] << 8 | message[3] );

    uint8_t topic_to_publish[128];

    /* Read topic */
    for ( int i = 0; i < topic_length; ++i){
        topic_to_publish[i] = message[4 + i];
    }

    publish_to_clients(broker, topic_to_publish, message);

}

int send_pingresponse ( struct mqtt_broker *broker, struct tcp_client_info *client ){

    uint8_t pingresponse[PINGRESP_MESSAGE_LENGTH];

    pingresponse[0] = PINGRESP_CONTROL_TYPE;
    pingresponse[1] = 0;

    int total_bytes = send(client->socket,pingresponse,PINGRESP_MESSAGE_LENGTH,0);

    if ( total_bytes < PINGRESP_MESSAGE_LENGTH ){
        fprintf(stderr, "send_pingresp(): failed to send");
        return -1;
    }

    return total_bytes;
}

int receive_pingrequest ( struct mqtt_broker *broker,  struct tcp_client_info *client, uint8_t *message ){

    if ( send_pingresponse ( broker, client ) < 0){
        return -1;
    }

    return 0;

}

int send_unsuback ( struct mqtt_broker *broker, int index, int packet_identifier ){

    uint8_t unsuback_message[UNSUBACK_MESSAGE_LENGTH];

    unsuback_message[0] = UNSUBACK_CONTROL_TYPE;
    unsuback_message[1] = UNSUBACK_MESSAGE_LENGTH - 2;
    unsuback_message[2] = (packet_identifier >> 8);
    unsuback_message[3] = packet_identifier;

    int bytes_send = send(broker->clients[index].socket, unsuback_message, UNSUBACK_MESSAGE_LENGTH, 0);

    return bytes_send;
}

int receive_unsubscribe ( struct mqtt_broker *broker, int index, uint8_t *message ){

    int remaining_length = message[1];
    int packet_identifier = ( message[2] << 8 | message[3]);
    int point = 2;


    while ( point < remaining_length ){

        uint8_t topic[128];
        /* Topic length */
        int topic_length = ( message[point+2] << 8 | message[point+3] );

        for ( int i = 0; i < topic_length; ++i ){
            topic[i] = message[i + point + 4];
        }

        topic[topic_length] = '\0';

        point = point + 2 + topic_length;

        printf("before....\n");

        delete_subscription(&broker->subs, topic, index, packet_identifier);

        printf("after....\n");
    }

    send_unsuback(broker,index,packet_identifier);

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

            /* Packet identifier */
            int packet_identifier;

            /* If client is on the list receive subscribe */
            receive_subscribe(broker,index,&packet_identifier,message);

            print_subscriptions(broker->subs);

            /* Send subscribe acknowledgment SUBACK */
            if ( send_suback(broker,index, packet_identifier) < 0 ){
                fprintf(stderr,"Failed to send suback()\n");
                return;
            }

        break;

        case PUBLISH_CONTROL_TYPE:

            index = find_corresponding_mqtt_client(broker,client);
            receive_publish(broker,index,message);

        break;

        case UNSUBSCRIBE_CONTROL_TYPE:

            index = find_corresponding_mqtt_client(broker,client);
            print_subscriptions(broker->subs);

            receive_unsubscribe(broker,index,message);
        
        break;

        case PINGEQ_CONTROL_TYPE:

            receive_pingrequest(broker,client,message);

        break;

        case DISCONNECT_CONTROL_TYPE:

        break;

        default:
        return;
        }
}