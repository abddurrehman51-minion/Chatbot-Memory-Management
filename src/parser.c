#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parser.h"

char *parse_input(const char *input)
{
    if (input == NULL)
    {
        return NULL;
    }
    int len = 0;
    int spaces = 0;
    int seench = 0;
    for (int i = 0; input[i] != '\0'; i++)
    {
        if (input[i] == '\n')
        {
            continue;
        }
        if(input[i] == ' ') 
        {
            if(seench == 0)
            {
                continue;
            }
            if(spaces == 1)
            {
                continue;
            }
            spaces = 1;
            len++;
        }
        else 
        {
        spaces = 0;
        seench = 1;
        len++;
        }
    }
    if (spaces && len > 0)
    {
        len--;
    }
    char* ptr = (char*)malloc((len+1)*sizeof(char));
    if (ptr == NULL)
    {
        return NULL;
    }
    int j = 0;
    int in_space = 0;
    int seenchar = 0;
    for (int i = 0; input[i] != '\0'; i++)
    {
        if (input[i] == '\n')
        {
            continue;
        }
        if(input[i] == ' ')
        {
            if(seenchar == 0)
            {
                continue;
            }
            if(in_space == 1)
            {
                continue;
            }
            ptr[j] = input[i];
            j++;
            in_space = 1;
        }
        else 
        {
            ptr[j] = input[i];
            j++;
            in_space = 0;
            seenchar = 1;
        }
    }
    while (j > 0 && ptr[j-1] == ' ')
    {
        j--;
    }
    ptr[j] = '\0';
    return ptr;
}

void parser_trim(char *text)
{
    if (text == NULL)
    {
        return;
    }
    int start = 0;
    int end = 0;
    int i = 0;

    while(text[start] == ' ')
    {
        start++;
    }

    if (text[start] == '\0')
    {
        text[0] = '\0';
        return;
    }
    i = start;
    while(text[i] != '\0')
    {
        if(text[i] != ' ')
        {
            end = i;
        }
        i++;
    }
    int j = 0;
    for (i = start; i <= end; i++)
    {
        text[j] = text[i];
        j++;
    }
    text[j] = '\0';
}

void parser_normalize(char *text)
{
    if (text == NULL)
    {
        return;
    }
    int p1 = 0;
    int p2 = 0;
    int prevSpace = 0;

    while (text[p1] != '\0')
    {
        char ch = text[p1];

        if(isprint((unsigned char)ch))
        {
            if (ch == ' ')
            {
                if (!prevSpace)
                {
                    text[p2] = ' ';
                    p2++;
                    prevSpace = 1;
                }
            }
            else
            {
                text[p2] = ch;
                p2++;
                prevSpace = 0;
            }
        }
        p1++;
    }
    text[p2] = '\0';

}

int parser_validate(const char *text)
{
    if (text == NULL)
    {
        
        return -1;
    }
    if (*text == '\0')
    {
        return 0;
    }
    for(int i = 0; text[i] != '\0'; i++)
    {
        if(!isprint((unsigned char)text[i]))
        {
            return 0;
        }
    }
    return 1;
}

void censor_text(char *text)
{
    if (text == NULL)
    {
        return;
    }
    int i = 0;
    int len = strlen(text);

    for(; i <= len - 8; i++)
    {
        if (tolower(text[i])     == 'c' &&
            tolower(text[i+1]) == 'h' &&
            tolower(text[i+2]) == 'e'  &&
            tolower(text[i+3]) == 'a' &&
            tolower(text[i+4]) == 't' &&
            tolower(text[i+5]) == 'i' &&
            tolower(text[i+6]) == 'n' &&
            tolower(text[i+7]) == 'g')
        {
            for (int j = 0; j < 8; j++)
            {
                text[i + j] = '*';
            }
        }
    }
}

int contains_blocked_word(const char *text)
{
    if (text == NULL)
    {
        return -1;
    }
    int len = strlen(text);
    for(int i = 0; i < len; i++)
    {
        if (i <= len - 10 &&
            tolower(text[i])     == 'p' &&
            tolower(text[i + 1]) == 'l' &&
            tolower(text[i + 2]) == 'a' &&
            tolower(text[i + 3]) == 'g' &&
            tolower(text[i + 4]) == 'i' &&
            tolower(text[i + 5]) == 'a' &&
            tolower(text[i + 6]) == 'r' &&
            tolower(text[i + 7]) == 'i' &&
            tolower(text[i + 8]) == 's' &&
            tolower((text[i + 9]) == 'm') )
        {
            return 1;
        }
          if (i <= len - 10 &&
            tolower(text[i])     == 'p' &&
            tolower(text[i + 1]) == 'l' &&
            tolower(text[i + 2]) == 'a' &&
            tolower(text[i + 3]) == 'g' &&
            tolower(text[i + 4]) == 'i' &&
            tolower(text[i + 5]) == 'a' &&
            tolower(text[i + 6]) == 'r' &&
            tolower(text[i + 7]) == 'i' &&
            tolower(text[i + 8]) == 's' &&
            tolower(text[i + 9]) == 'e')
        {
            return 1;
        }
        if (i <= len - 12 &&
            tolower(text[i])     == 'p' &&
            tolower(text[i + 1]) == 'l' &&
            tolower(text[i + 2]) == 'a' &&
            tolower(text[i + 3]) == 'g' &&
            tolower(text[i + 4]) == 'i' &&
            tolower(text[i + 5]) == 'a' &&
            tolower(text[i + 6]) == 'r' &&
            tolower(text[i + 7]) == 'i' &&
            tolower(text[i + 8]) == 's' &&
            tolower(text[i + 9]) == 'i' &&
            tolower(text[i +10]) == 'n' &&
            tolower(text[i +11]) == 'g')
        {
            return 1;
        }
    }
 return 0;
}

int parser_check_policy(const char *text)
{
    if (text == NULL)
    {
        return -1;
    }
    int decision = contains_blocked_word(text);
    if (decision < 0)
    {
        return -1;
    }
    if (decision > 0)
    {
        return 0;
    }
    
        return 1;
    
}

void parser_apply_policy(char *text)
{
    if (text == NULL)
    {
        return;
    }
    censor_text(text);
}

char *parser_process(const char *raw)
{
    if (raw == NULL)
    {
        return NULL;
    }
    char* ptr = parse_input(raw);
    if (ptr == NULL)
    {
        return NULL;
    }
    parser_trim(ptr);
    parser_normalize(ptr);
    int validate = parser_validate(ptr);
    if (validate < 0)
    {
        parser_free(ptr);
        return NULL;
    }
    if (validate == 0)
    {
        parser_free(ptr);
        return NULL;
    }
    int decision = parser_check_policy(ptr);
    if (decision < 0)
    {
        parser_free(ptr);
        return NULL;
    }
    if (decision == 0)
    {
        parser_free(ptr);
        return NULL;
    }
    parser_apply_policy(ptr);

    return ptr;
}

void parser_free(char *text)
{
    if (text == NULL)
    {
        return;
    }
    free(text);
    return;
}

void parser_debug_dump(const char *stage, const char *text)
{
    if (text == NULL || stage == NULL)
    {
        return;
    }
    printf("[%s] ", stage);
    printf("%s\n", text);

}
