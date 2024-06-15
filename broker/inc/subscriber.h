#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

struct subscriber {

    /* index of tcp_client in array */
    int index;

    /* packet identifier */
    int packet_identifier;

    /* qos */
    int qos;

    /* Next subscriber */
    struct subscriber *next;
};

#endif