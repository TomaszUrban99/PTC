#include "subscription.h"

void add_subscriber ( struct subscriber **subs, 
        int index, int qos, int packet_identifier ){

        /* Create new instance of subscriber */
        struct subscriber *new_sub = (struct subscriber *)
                malloc ( sizeof( struct subscriber ) );

        new_sub->index = index;
        new_sub->qos = qos;
        new_sub->packet_identifier = packet_identifier;

        if ( *subs == NULL ){
            *subs= new_sub;
            return;
        }

        struct subscriber *tmp = *subs;
        while (tmp->next != NULL){
            tmp = tmp->next;
        }

        tmp->next = new_sub;
}

void add_subscription ( struct subscription **sub, uint8_t *topic, int index, int qos, int packet_identifier ){

    struct subscription *temp = (struct subscription*) calloc (1, sizeof(struct subscription));

    strcpy(temp->topic, topic);

    if ( *sub == NULL ){
        *sub = temp;
        add_subscriber(&(*sub)->client, index, qos, packet_identifier);
        return;
    }

    struct subscription *tmp = *sub;

    while ( tmp->next != NULL ){
        
        if ( strcmp( tmp->topic, topic ) == 0 ){
            add_subscriber(&tmp->client,index,qos,packet_identifier);
            free(temp);
            return;
        }
        else{
            tmp = tmp->next;
        }
    }

    tmp->next = temp;
    add_subscriber(&(tmp->next)->client,index,qos,packet_identifier);

    return;

}

void delete_subscriber ( struct subscription **sub, int index, int packet_identifier ){

    struct subscriber *cli = (*sub)->client;

    if ( cli == NULL ){
        return;
    }

    if ( cli->next == NULL ){

        /* If subscriber found- delete */
        if ( cli->index == index && cli->packet_identifier == packet_identifier ){
            free(cli);
            return;
        }

    }

    while ( cli->next != NULL ){

        if ( cli->next->index == index && 
            cli->next->packet_identifier == packet_identifier ){

                struct subscriber *tmp = cli->next;
                cli = cli->next->next;

                free(tmp);
                return;
        }

        cli = cli->next;
    }

    return;
}

int delete_subscription ( struct subscription **sub, uint8_t *topic, int index, int packet_identifier ){

    if ( *sub == NULL ){
        return -1;
    }

    struct subscription *sub_client = (*sub);

    if ( strcmp(sub_client->topic, topic) == 0){

        *sub = sub_client->next;
        delete_subscriber(&sub_client,index,packet_identifier);
        free(sub_client);

        return 0;
    }

    while (sub_client->next != NULL ){

        if ( strcmp(sub_client->next->topic, topic) == 0){

            struct subscription *tmp = sub_client->next;
            delete_subscriber(&sub_client->next,index,packet_identifier);
            sub_client->next = sub_client->next->next;

            free(tmp);
            return 0;

        }

        sub_client = sub_client->next;
    }

    return -1;
}

void delete_subscribers ( struct subscription **subs ){

    struct subscriber *sub_client = (*subs)->client;

    while (sub_client)
    {
        struct subscriber *n;
        n = sub_client;
            
        sub_client = n->next;
        free(n);
    }

    return;
    
}

void delete_subscriptions ( struct subscription **subs ){

    struct subscription *sub = *subs;

    while ( sub ){

        struct subscription *s;
        s = sub;

        delete_subscribers(&sub);
        free(sub);
        sub = s->next;
    }

    return;
}

void print_subscriptions ( struct subscription *subs ){

    struct subscription *n = subs;

    while ( n ){

        printf("%s%s\n", "Topic: ", n->topic );

        struct subscriber *sub_client = n->client;

        while (sub_client)
        {
            printf("%s%d%s%d\n", "Index: ", sub_client->index, " packet: ", sub_client->packet_identifier);
            sub_client = sub_client->next;
        }

        n = n->next;
        
    }
}