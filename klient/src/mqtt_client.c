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

    uint pubcomp_message[PUBCOMP_MESSAGE_LENGTH];

    if ( recv(client->tcp_socket, pubcomp_message, PUBCOMP_MESSAGE_LENGTH,0) < PUBCOMP_MESSAGE_LENGTH){
        fprintf(stderr, "receive_pubcomp(): failed to receive pubcomp");
        return -1;
    }

    return 0;
}