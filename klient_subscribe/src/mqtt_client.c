#include "mqtt_client.h"

int generate_connect_message ( struct mqtt_client *client, uint8_t message[] ){

    message[0] = 0x10;

    /* ------- PROTOCOL NAME ---------------------- */

    message[2] = 0x00;
    message[3] = 0x04;
    message[4] = 0x4d; /* M */
    message[5] = 0x51; /* Q */
    message[6] = 0x54; /* T */
    message[7] = 0x54; /* T */

    /* ------- PROTOCOL LEVEL --------------------- */

    message[8] = 0x04;

    /* ------- CONNECT FLAGS ---------------------- */

    message[9] = client->connect_flags;

    /* ------- KEEP ALIVE ------------------------- */

    message[10] = client->keep_alive_msb;
    message[11] = 0x0a; /*client->keep_alive_lsb;*/
    
    int message_length = 12;

    /* ------- PAYLOAD ---------------------------- */

    /* ----- CLIENT IDENTIFIER -------------------- */
    code_field_connect_message( message, &message_length, client->client_id );

    /* --------------- WILL TOPIC ------------------ */
    if ( (client->connect_flags & BIT2) == BIT2 ){
        code_field_connect_message( message, &message_length, client->will_topic);
    }

    /* ----------------------- USER NAME ----------------------------- */
    if ( (client->connect_flags & BIT7) == BIT7 ){
        code_field_connect_message( message, &message_length, client->user_name );

        /* ------------------------ PASSWORD ---------------------------------- */
        if( (client->connect_flags & BIT6) == BIT6 ){
            code_field_connect_message( message, &message_length, client->password);
        }
    }
    
    /* Set message length */
    message[1] = message_length - 2;

    printf("%s%d\n", "Message length: ", message_length);
    message[message_length] = '\0';

    return message_length;
}

int interpret_connack_message ( uint8_t *response ){
    return response[CONNACK_RETURN_CODE_POSITION];
}

int receive_connack ( struct mqtt_client *client ){

    uint8_t connack_message [CONNACK_MESSAGE_LENGTH];

    int bytes_received = 0;

    if ( bytes_received = recv(client->tcp_socket,connack_message,CONNACK_MESSAGE_LENGTH,0) < 0){
        fprintf(stderr,"receive_connack(): failed to receive\n");
        return -1;
    }

    return interpret_connack_message(connack_message);
}

int connect_mqtt ( struct mqtt_client *client ){

    uint8_t connect_message[CONNECT_MESSAGE_LENGTH];

    int message_length = generate_connect_message(client,connect_message);
    int bytes_received;
    
    if ( bytes_received = send(client->tcp_socket,(char*)&connect_message, message_length, 0) < 0 ){
        fprintf(stderr,"connect: failed to send connect message");
        return -1;
    }
    
    return receive_connack(client);
}

int generate_publish_message (  struct mqtt_client *client,
                                struct publish *pub_info, uint8_t *message ){

    /* First byte */
    message[0] = pub_info->first_byte;

    /* --------------------- Topic ---------------------------- */
    
    /* ---------- Code length of topic ------------------------ */
    
    int message_length = 2;
    int length = strlen(pub_info->topic_name);
    
    message[2] = 0;
    message[3] = length;

    /* --------------------------------------------------------- */

    /* -------- Code topic ------------------------------------- */
    for ( int j=0; j < length; ++j ){
        message[message_length + 2 + j] = pub_info->topic_name[j];
    }
    /* --------------------------------------------------------- */

    message_length = message_length + length;

    /* Packet identifier */

    /*message[message_length + 2] = 0;
    message[message_length + 3] = client->publish_parameters.packet_identifier;

    message_length = message_length + 2;*/

    /* -------- Code payload ------------------------------------- */

    length = strlen(pub_info->message_payload);

    for ( int j=0; j < length; ++j ){
        message[message_length + 2 + j] = pub_info->message_payload[j];
    }

    message_length = message_length + length;

    /* ------- Code the remaining length of publish message ----- */
    message[1] = message_length;

    return (message_length + 2);
}

int publish ( struct mqtt_client *client, struct publish *pub_info ) {

    uint8_t publish_message[50];

    int length = generate_publish_message(client, pub_info, publish_message);
    int bytes = send(client->tcp_socket,(char*)&publish_message,length,0); 

    return bytes;   

}

int interpret_puback ( struct publish *pub_info, uint8_t *puback_message ){

    if ( puback_message[0] != PUBACK_CONTROL_TYPE ){
        return -1;
    }

    if ( puback_message[2] == pub_info->packet_identifier_msb &&
        puback_message[3] == pub_info->packet_identifier_lsb ){
            return 0;
        }
    
    return -1;
}

int receive_puback ( struct mqtt_client *client, struct publish *pub_info ){

    uint8_t puback_message[PUBACK_MESSAGE_LENGTH];

    int bytes_received = 0;

    if ( bytes_received = recv(client->tcp_socket,puback_message,PUBACK_MESSAGE_LENGTH,0) < 0){
        fprintf(stderr,"receive_puback(): failed to receive puback message");
        return -1;
    }

}

int interpret_pubrec ( struct publish *pub_info, uint8_t *pubrec_message ){

    if ( pubrec_message[0] != PUBREC_CONTROL_TYPE ){
        return -1;
    }

    if ( pubrec_message[2] == pub_info->packet_identifier_msb &&
        pubrec_message[3] == pub_info->packet_identifier_lsb )
        return 0;

    return -1;
}

int receive_pubrec ( struct mqtt_client *client, struct publish *pub_info ){

    uint8_t pubrec_message[PUBREC_MESSAGE_LENGTH];

    if ( recv(client->tcp_socket, pubrec_message, PUBREC_MESSAGE_LENGTH, 0) < 0 ){
        fprintf(stderr, "receive_pubrec(): failed to receive pubrec" );
        return -1;
    }
}

void generate_pubrel_message ( struct publish *pub_info, uint8_t *pubrel_message ){

    pubrel_message[0] = PUBREL_CONTROL_TYPE;
    pubrel_message[1] = 0x02;

    pubrel_message[2] = pub_info->packet_identifier_msb;
    pubrel_message[3] = pub_info->packet_identifier_lsb;
}

int send_pubrel ( struct mqtt_client *client, struct publish *pub_info ){

    uint8_t pubrel_message[PUBREL_MESSAGE_LENGTH];
    generate_pubrel_message(pub_info,pubrel_message);

    int bytes_send = 0;

    if ( bytes_send = 
        send(client->tcp_socket,(char*)&pubrel_message,PUBREL_MESSAGE_LENGTH,0) < PUBREL_MESSAGE_LENGTH ){
            fprintf(stderr,"send_pubrel(): failed to send message");
            return -1;
        }

    return bytes_send;
}

int receive_pubcomp ( struct mqtt_client *client, struct publish *pub_info ){

    uint8_t pubcomp_message[PUBCOMP_MESSAGE_LENGTH];

    if ( recv(client->tcp_socket, pubcomp_message, PUBCOMP_MESSAGE_LENGTH,0) < PUBCOMP_MESSAGE_LENGTH){
        fprintf(stderr, "receive_pubcomp(): failed to receive pubcomp");
        return -1;
    }

    return 0;
}

int generate_subscribe_message ( struct mqtt_client *client, 
                                    struct subscribe *sub_info, uint8_t *subscribe_message ){
    
    /* Set packet control type */
    subscribe_message[0] = SUBSCRIBE_CONTROL_TYPE;

    /* Entire message length */
    int message_length = 4;

    /* Set packet identifier */
    subscribe_message[2] = sub_info->packet_identifier_msb;
    subscribe_message[3] = sub_info->packet_identifier_lsb;

    /* Encode topics */
    for ( int i = 0; i < sub_info->topic_numbers; ++i ){

        /* ---------------- TOPIC FILTER ------------------------------------------------------ */
            subscribe_message[message_length] = sub_info->subscribed_topics[i]->topic_length_msb;
            subscribe_message[message_length+1] = sub_info->subscribed_topics[i]->topic_length_lsb;

        /* Topic filter header length */
        message_length = message_length + 2;

        for ( int j = 0; j < sub_info->subscribed_topics[i]->topic_length_lsb; ++j ){
            subscribe_message[message_length+j] = sub_info->subscribed_topics[i]->topic_name[j];
        }

        /* Calculate new message length */
        message_length = message_length + sub_info->subscribed_topics[i]->topic_length_lsb;
        
        subscribe_message[message_length] = sub_info->subscribed_topics[i]->qos;
        message_length++;
    }

    subscribe_message[1] = message_length - 2;

    return message_length;
}

int interpret_suback_message ( struct subscribe *sub_info, uint8_t *suback_message, int received_bytes ){

    if (suback_message[0] != SUBACK_CONTROL_TYPE ){
        fprintf(stderr,"suback(): incorrect control type");
        return -1;
    }

    if ( suback_message[2] != sub_info->packet_identifier_msb 
        || suback_message[3] != sub_info->packet_identifier_lsb ){
            fprintf(stderr,"suback(): incorrect packet identifier");
            return -1;
    }

    printf("%s%d\n", "Received bytes: ", received_bytes);

    if ( received_bytes - 4 != sub_info->topic_numbers ){
        fprintf(stderr,"suback(): incorrect number of topic's responses");
        return -1;
    }

    for ( int i = 0; i < sub_info->topic_numbers; ++ i){

        sub_info->subscribed_topics[i]->topic_return_code = suback_message[4 + i];

        if ( suback_message[4 + i] != FAILURE ){
            sub_info->subscribed_topics[i]->is_subscribed = 1;
        }
        else{
            sub_info->subscribed_topics[i]->is_subscribed = 0;
            fprintf(stderr,"%s%d%s", "suback(): ", i, "-topic failure");
        }
    }

    return 0;
}

int receive_suback ( struct mqtt_client *client, struct subscribe *sub_info ){

    uint8_t suback_message[SUBACK_MESSAGE_LENGTH];

    int received_bytes = recv(client->tcp_socket,suback_message,SUBACK_MESSAGE_LENGTH,0);

    if ( received_bytes < 0){
        fprintf(stderr,"receive_suback(): failed to receive suback message");
        return -1;
    }

    return interpret_suback_message(sub_info,suback_message,received_bytes);
}

int subscribe ( struct mqtt_client *client, struct subscribe *sub_info ){

    uint8_t subscribe_message[SUBSCRIBE_MESSAGE_LENGTH];

    /* Generate subscribe message. Length of message is returned */
    int message_length = generate_subscribe_message(client,sub_info,subscribe_message);

    if ( send(client->tcp_socket, (char*)&subscribe_message, message_length, 0) < message_length ){
        fprintf(stderr, "subscribe(): failed to send subscribe message" );
        return -1;
    }

    return 0;
}

int generate_unsubscribe ( struct subscribe *sub_info, 
    uint8_t *unsubscribe_message, int topic_index ){

        unsubscribe_message[0] = UNSUBSCRIBE_CONTROL_TYPE;

        int message_length = 4;

        unsubscribe_message[2] = sub_info->packet_identifier_msb;
        unsubscribe_message[3] = sub_info->packet_identifier_lsb;

        if ( sub_info->subscribed_topics[topic_index]->is_subscribed ){
            
            /* Encode topic to unsubscribe */
            unsubscribe_message[4] = sub_info->subscribed_topics[topic_index]->topic_length_msb;
            unsubscribe_message[5] = sub_info->subscribed_topics[topic_index]->topic_length_lsb;

            /* Calculate message length */
            message_length = message_length + 2;

            for ( int i = 0; i < unsubscribe_message[5]; ++i ){
                unsubscribe_message[message_length + i] = sub_info->subscribed_topics[topic_index]->topic_name[i];
            }

            message_length = message_length + sub_info->subscribed_topics[topic_index]->topic_length_lsb;

            unsubscribe_message[1] = message_length - 2;
            return message_length;
        }
        else {
            fprintf(stderr,"unsubscribe(): not subscribed");
            return -1;
        }
}

int unsubscribe ( struct mqtt_client *client, struct  subscribe *sub_info, int topic_index ){

    uint8_t unsubscribe_message[UNSUBSCRIBE_MESSAGE_LENGTH];

    /* Generate unsubscribe message */
    int message_length = generate_unsubscribe(sub_info,unsubscribe_message,topic_index);

    if ( send(client->tcp_socket,(char*) &unsubscribe_message, message_length, 0) < message_length ){
        fprintf(stderr,"unsubscribe(): failed to send unsubscribe message");
        return -1;
    }

    return 0;
}

int interpret_unsuback_message ( struct subscribe *sub_info, uint8_t *unsuback_message, int topic_index){

    if ( unsuback_message[0] != UNSUBACK_CONTROL_TYPE ){
        fprintf(stderr, "unsuback(): incorrect unsuback control type");
        return -1;
    }

    if ( unsuback_message[2] != sub_info->packet_identifier_msb ||
            unsuback_message[3] != sub_info->packet_identifier_lsb ){
                fprintf(stderr, "unsuback(): incorrect packet identifier");
                return -1;
            }
    
    sub_info->subscribed_topics[topic_index]->is_subscribed = 0;
    return 0;
}

int receive_unsuback ( struct mqtt_client *client, struct subscribe *sub_info, int topic_index ){

    uint8_t unsuback_message[UNSUBACK_MESSAGE_LENGTH];

    int bytes_received = recv(client->tcp_socket, unsuback_message, UNSUBACK_MESSAGE_LENGTH, 0);

    if ( bytes_received < UNSUBACK_MESSAGE_LENGTH){
        fprintf(stderr, "unsuback(): failed to receive unsuback message");
        return -1;
    }
}

int pingreq ( struct mqtt_client *client ){

    uint8_t pingreq_message[PINGREQ_MESSAGE_LENGTH];

    pingreq_message[0] = PINGEQ_CONTROL_TYPE;
    pingreq_message[1] = 0x00;

    if ( send(client->tcp_socket,(char*)&pingreq_message,PINGREQ_MESSAGE_LENGTH,0) < PINGREQ_MESSAGE_LENGTH ){
        fprintf(stderr,"pingreq(): failed to send pingreq");
        return -1;
    }

    return 0;
}

int receive_pingresponse ( struct mqtt_client *client ){

    uint8_t pingresp_message[PINGRESP_MESSAGE_LENGTH];

    int bytes_received = recv(client->tcp_socket, (char*) &pingresp_message, PINGRESP_MESSAGE_LENGTH, 0);

    if ( bytes_received < PINGRESP_MESSAGE_LENGTH ){
        fprintf(stderr,"pingresp(): failed to receive pingresp");
        return -1;
    }
    
    if ( pingresp_message[0] == PINGRESP_CONTROL_TYPE &&
    pingresp_message[1] == 0x00 ){
        return 0;
    }

    return -1;
}

int send_disconnect ( struct mqtt_client *client ){

    uint8_t disconnect_message[DISCONNECT_MESSAGE_LENGTH];

    /* Prepare message */
    disconnect_message[0] = DISCONNECT_CONTROL_TYPE;
    disconnect_message[1] = 0x00;

    if ( send(client->tcp_socket, (char*) &disconnect_message, 
        DISCONNECT_MESSAGE_LENGTH, 0) < DISCONNECT_MESSAGE_LENGTH ){
            fprintf(stderr, "send_disconnect(): failed to send disconnect message\n");
            return -1;
        }

    return 0;
}

int disconnect ( struct mqtt_client *client ){

    if ( send_disconnect(client) < 0 ){
        fprintf(stderr, "disconnect(): failed to send disconnect\n");
        return -1;
    }

    return close(client->tcp_socket);
}
