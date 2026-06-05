#include <stdio.h>
#include <stdlib.h>

#include "history.h"
#include "parser.h"
#include "message.h"
#include "debug.h"

void show_menu(void){

    printf("===============================\n");
    printf(" Chatbot Debug Console\n");
    printf("===============================\n");

    printf("1. Send message\n");
    printf("2. Print all messages\n");
    printf("3. Search messages\n");
    printf("4. Delete message\n");
    printf("5. Reset History\n");
    printf("6. Validate History\n");
    printf("7. Compact History\n");
    printf("0. Exit\n");

}

int read_choice(void){

    int integer;
    printf("Enter an integer(0-7): ");
    int validate;
    int check = 0;
    while(!check)
    {
        validate = scanf("%d", &integer);
        if (validate != 1 || integer < 0 || integer > 7)
        {
            printf("Invalid input, Enter again: ");
            while(getchar() != '\n');
        }
        else 
        {
            check = 1;
        }
    }
    return integer;
}


int main(void){
    History h;
    history_init(&h);

    int choice;
    while(1){

        show_menu();
        choice = read_choice();

        if(choice == 0)
            break;

        dispatch_command(choice, &h);
    }

    history_reset(&h);

    return 0;
}
