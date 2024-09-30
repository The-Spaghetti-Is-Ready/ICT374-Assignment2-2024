#include "include/shellfunctions.h"

int main()
{
    char* prompt_name = "";
    Command current_command = { "", 0, {""}, "", "", ' ' };

    while(1) {
        if(prompt_name[0] != '\0') { printf("%s", prompt_name); }
        printf("%%");

        current_command.com_pathname_ = GetKBInput();
        if(strcmp(current_command.com_pathname_, "exit") == 0) {
            break;
        }
    }
    
    free(current_command.com_pathname_);
    printf("goodbye.\n");

    return 0;
}
