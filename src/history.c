#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "history.h"
#include "parser.h"
#include "compaction.h"

void history_init(History *h){
    if (h == NULL)
    {
        return;}
    h->capacity = 4;
    h->count = 0;
    h->head = NULL;
    h->tail = NULL;
    h->array = (Message**)calloc(h->capacity, sizeof(Message*));
    // if (h->array == NULL)
    // {
    //     return;
    // }
    // for (int i = 0; i < h->capacity; i++)
    // {
    //     h->array[i] = NULL;
    // }
}

int history_add(History *h, Message *msg){

    if (h == NULL || msg == NULL || msg->text == NULL)
    {
        return 0;
    }
    int decision = parser_check_policy(msg->text);
    if (decision == 0)
    {
        msg_destroy(msg);
        return -1;
    }
     if (decision < 0)
    {
        msg_destroy(msg);
        return -1;
    }
    parser_apply_policy(msg->text);
    if (h->count >= h->capacity)
    {   
        int old_capacity = h->capacity;
        int new_capacity = h->capacity*2;
        Message** new_array = realloc(h->array, new_capacity*sizeof(Message*));
        if (new_array == NULL)
        {
            return -1;
        }
        h->array = new_array;
        h->capacity = new_capacity;
        for (int i = old_capacity; i < h->capacity; i++)  // <-- init new slots to NULL
        {
            h->array[i] = NULL;
        }
    }

    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL)
    {
        return -1;
    }
    node->msg = msg;
    node->next = NULL;

    h->array[h->count] = msg;

    if (h->tail == NULL)
    {
        h->head = node;
        h->tail = node;
    }
    else
    {
        h->tail->next = node;
        h->tail = node;
    }
    h->count++;

    return 1;
}

Message *history_get(History *h, int index){

    if (h == NULL)
    {
        return NULL;
    }
    if (index >= h->count || index < 0)
    {
        return NULL;
    }
    return h->array[index];

}

int history_delete_kth_last(History *h, int k){

    if (h == NULL || k <= 0 || k > h->count)
    {
        return 0;
    }
    
    int target = h->count - k;
    Node* prev = NULL;
    Node* temp = h->head;
    for (int i = 0; i < target; i++)
    {   
        prev = temp;
        temp = temp->next;
    }
    if (prev == NULL)
    {
        h->head = temp->next;
    }
    else {
        prev->next = temp->next;
    }
    if (temp-> next == NULL)
    {
        h->tail = prev;
    }

    Message* msg_to_delete = temp->msg;
    free(temp);

    for (int i = target; i < h->count - 1; i++)
    {
        h->array[i] = h->array[i+1];
    }
    h->array[h->count - 1] = NULL;
    h->count--;
    msg_destroy(msg_to_delete);
    return 1;
}

int history_purge_word(History *h, const char *word)
{

    if (h == NULL || word == NULL)
    {
        return -1;
    }
    if (word[0] == '\0')
    {
        return -1;
    }
    int count = 0;
    Node* prev = NULL;
    Node* temp = h->head;

    while(temp != NULL)
    {
        Node* next_node = temp->next;

        if (msg_contains(temp->msg, word) > 0)
        {
            Node* to_delete = temp;
            Message* msg_to_find = temp->msg;
            if (prev == NULL)
            {
                h->head = next_node;
            }
            else{
                prev->next = next_node;
            }
            if (temp == h->tail)
            {
                h->tail = prev;
            }
            temp = next_node;

            for (int i = 0; i < h->count; i++)
            {
                if (h->array[i] == msg_to_find)
                {
                    for (int j = i; j < h->count - 1; j++)
                    {
                        h->array[j] = h->array[j+1];
                    }
                    h->array[h->count-1] = NULL;
                    break; 
                } 
            }
            msg_destroy(to_delete->msg);
            free(to_delete);
            h->count--;
            count++;
        }
        else{
            prev = temp;
            temp = next_node;

        }
    }



    return count;
}

void history_shift_left(History *h){

    if (h == NULL || h->count == 0)
    {
        return;
    }
    Node* old_head = h->head;
    h->head = old_head->next;

    if (h->head == NULL)
    {
        h->tail = NULL;
    }
    msg_destroy(old_head->msg);
    free(old_head);

    for (int i = 0; i < h->count - 1; i++)
    {
        h->array[i] = h->array[i+1];
    }
    h->array[h->count-1] = NULL;
    h->count--;

}

void history_rewind(History *h, int k){

    if (h == NULL || k <= 0)
    {
        return;
    }
    if (k > h->count)
    {
        k = h->count;
    }
    int i = 0;
    while(i < k)
    {
        history_delete_kth_last(h,1);
        i++;
    }
}

void history_reset(History *h){

    if (h == NULL)
    {
        return;
    }


    for (int i = 0; i < h->count; i++)
    {
        if (h->array[i] != NULL)
        {
            msg_destroy(h->array[i]);
            h->array[i] = NULL;
        }
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
    h->count = 0;
    h->capacity = 0;
    h->head = NULL;
    h->tail = NULL;
}

int history_validate(History *h){

    if (h == NULL)
    {
        return -1;
    }
    if (h->array == NULL)
    {
        return -1;
    }
    int count = 0;

    Node* temp = h->head;
    Node* prev = NULL;
    while(temp != NULL)
    {
        if (temp->msg == NULL)
        {
            return -1;
        }
        prev = temp;
        temp = temp->next;
        count++;
    }
    if(count != h->count)
    {
        return -1;
    }
    if(h->count == 0)
    {
        if (h->head != NULL || h->tail != NULL)
        {
            return -1;
        }
        return 0;
    }

    if(prev != h->tail)
    {
        return -1;
    }
    if(h->tail->next != NULL)
    {
        return -1;
    }
    temp = h->head;
    for (int i = 0; i < h->count; i++)
    {
        if(h->array[i] == NULL)
        {
            return -1;
        }
        if(h->array[i] != temp->msg)
        {
            return -1;
        }
        temp = temp->next;
    }
    for (int i = 0; i < h->count; i++)
    {
        for (int j = i+1; j < h->count; j++)
        {
            if (h->array[i] == h->array[j])
            {
                return -1;
            }
        }
    }
    return 0;

}

int history_compact(History *h){

    if (h == NULL)
    {
        return 0;
    }
    return compaction_run(h);
}
