#include "compaction.h"
#include "message.h"
#include <stdlib.h>


Message **compaction_build_array(History *h){

    if (h == NULL)
    {
        return NULL;
    }
    Message** new_array = malloc(h->count*sizeof(Message*));
    if(new_array == NULL && h->count > 0)
    {
        return NULL;
    }
    Node* temp = h->head;
    for(int i = 0; i < h->count; i++)
    {
        new_array[i] = temp->msg;
        temp = temp->next;
    }


    return new_array;
}


Node *compaction_build_list(Message **arr, int count){

    if (arr == NULL && count > 0)
    {
        return NULL;
    }
    if (count == 0)
    {
        return NULL;
    }
    Node *head = NULL;
    Node *tail = NULL;

    for (int i = 0; i < count; i++)
    {
        Node *node = malloc(sizeof(Node));
        if (node == NULL)
        {
            Node* temp = head;
            while (temp != NULL)
            {
                Node* next = temp->next;
                free(temp);
                temp = next;
            }
            return NULL;
        }

        node->msg = arr[i];
        node->next = NULL;

        if (head == NULL)
        {
            head = node;
            tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }
    }

    return head;
}


void compaction_free_old_storage(History *h){
    if (h == NULL)
    {
        return;
    }
    
    Node* temp = h->head;
    while(temp != NULL)
    {
        Node* next = temp->next;
        free(temp);
        temp = next;
    }
    free(h->array);
    h->array = NULL;
    h->head = NULL;
    h->tail = NULL;
    h->count = 0;
    h->capacity = 0;

}


int compaction_replace_storage(History *h,
                               Message **arr,
                               Node *list){
    if (h == NULL || arr == NULL || list == NULL)
    {
        return -1;
    }
    Message** old_aarr = h->array;
    Node* old_list = h->head;

    Node* temp = list;
    Node* new_tail = NULL;
    int new_count = 0;

    while(temp != NULL)
    {
        new_tail = temp;
        temp = temp->next;
        new_count++;
    }
    h->array = arr;
    h->head = list;
    h->tail = new_tail;
    h->count = new_count;
    h->capacity = new_count;

    Node* temp1 = old_list;
    while(temp1 != NULL)
    {
        Node* next = temp1->next;
        free(temp1);
        temp1 = next;
    }

    free(old_aarr);
    return 0;
}


int compaction_run(History *h){
    if (h == NULL)
    {
        return -1;
    }
    Message **arr = compaction_build_array(h);
    if (!arr) return -1;

    Node *list = compaction_build_list(arr, h->count);
    if (!list)
    {
        free(arr);
         return -1;
    }     

    compaction_free_old_storage(h);
    if (compaction_replace_storage(h, arr, list) < 0)
    {
        Node *temp = list;
        while (temp != NULL)
        {
            Node *next = temp->next;
            free(temp);
            temp = next;
        }
        free(arr);


        return -1;

    
    }
    return 0;
}
