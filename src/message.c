#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "message.h"


Message *msg_create(int id, const char *raw){
    if (raw == NULL)
    {
        return NULL;
    }
    Message* ptr = (Message*)malloc(sizeof(Message));
    if (ptr == NULL)
    {
        return NULL;
    }
    size_t len = strlen(raw); 
    ptr->text = (char*)malloc((len+1)*sizeof(char));
    strcpy(ptr->text,raw);
    ptr->id = id;

    return ptr;
}


void msg_destroy(Message *msg){

    if (msg == NULL)
    {
        return;
    }

    if(msg->text == NULL)
    {
        free(msg);
    }
    else
    {
        free(msg->text);
        free(msg);
    }
    msg = NULL;
}


void msg_print(const Message *msg){
    if (msg == NULL)
    {
        return;
    }
    printf("Message ID: %d", msg->id);
    if(msg->text == NULL)
    {
        printf("Empty text buffer");
        return;
    }
    printf("Message text: %s", msg->text);
}


int msg_contains(const Message *msg, const char *word){
    if (msg == NULL)
    {
        return -1;
    }
    if (msg->text == NULL)
    {
        return -1;
    }
    if (word == NULL)
    {
        return -1;
    }
    int msg_len = strlen(msg->text);
    int word_len = strlen(word);

    if (word_len == 0)
    {
        return 0;
    }
    if (word_len > msg_len)
    {
        return 0;
    }
    for (int i = 0; i <= msg_len - word_len; i++)
    {
        int match = 1;
        for (int j = 0; j < word_len; j++)
        {
            if(tolower(msg->text[i+j]) != tolower(word[j]))
            {
                match = 0;
                break;
            }
        }
        if (match == 1)
        {
            return 1;
        }
    }
        return 0;
}


int msg_length(const Message *msg){

    if (msg == NULL)
    {
        return -1;
    }
    if (msg->text == NULL)
    {
        return -1;
    }
    int i = 0;
    int len = 0;
    while((msg->text)[i] != '\0')
    {
        if(isprint((msg->text)[i]))
        {
            len++;
        }
        i++;
    }
    return len;
}


Message *msg_clone(const Message *src, int new_id){
    if (src == NULL)
    {
        return NULL;
    }
    Message* ptr = (Message*)malloc(sizeof(Message));
    if (ptr == NULL)
    {
        return NULL;
    }
    ptr->id = new_id;
    size_t len = strlen(src->text);
    ptr->text = (char*)malloc((len+1)*sizeof(char));
    if (ptr->text == NULL)
    {
        return NULL;
    }
    strcpy(ptr->text,src->text);



    return ptr;
}
