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

int receice_puback ( struct mqtt_client *client, struct publish *pub_info ){

}