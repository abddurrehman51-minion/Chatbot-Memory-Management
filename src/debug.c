#include <stdio.h>
#include "debug.h"


void debug_print_array(History *h){
    if (h == NULL || h->array == NULL)
    {
        return;
    }
    for(int i = 0; i < h->capacity; i++)
    {
        if(h->array[i] == NULL)
        {
            printf("[%d] -> NULL\n", i);
        }
        else{
            printf("[%d] -> ID=%d\n",i,h->array[i]->id);
        }
    }

}


void debug_print_list(History *h){
    if (h == NULL)
    {
        return;
    }
    if (h->head == NULL)
    {
        return;
    }
    Node* temp = h->head;
    printf("HEAD -> ");

    while(temp != NULL)
    {
        if (temp->msg != NULL)
        {
        printf("%d", temp->msg->id);
        }
        else
        {
            printf("NULL");
        }
        temp = temp->next;
        if (temp != NULL)
        {
            printf(" -> ");
        }
    }
    printf(" -> NULL\n");

}


void debug_memory_stats(History *h){
    if (h == NULL || h->array == NULL)
    {
        return;
    }
    Node* temp = h->head;
    int nodes = 0;
    while(temp != NULL)
    {
        nodes++;
        temp = temp->next;
    }
    int fragments = 0;
    for(int i = 0; i < h->capacity; i++)
    {
        if (h->array[i] == NULL)
        {
            fragments++;
        }
    }
    printf("Messages: %d", h->count);
    printf("Array Capacity: %d", h->capacity);
    printf("Nodes: %d", nodes);
    printf("Fragmented Slots: %d", fragments);
}
