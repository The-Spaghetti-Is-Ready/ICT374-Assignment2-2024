#include "include/shellfunctions.h"

char * GetKBInput() { //get input from keyboard
    char * input = (char *) malloc(MAX_STR_SIZE * sizeof(char)); //128 character-limit

    fgets(input, 128, stdin);
    
    input[strcspn(input, "\n")] = '\0'; //remove newline

    return input;
}