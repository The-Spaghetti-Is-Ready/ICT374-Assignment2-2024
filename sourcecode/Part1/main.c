#include "include/shellfunctions.h"

int main()
{
    char* prompt_name = "";
    //char* command_history[MAX_COMMAND_HISTORY]; //turn this into a stack
    Command current_command = { "", 0, {""}, "", "", ' ' };
    Stack *command_history = create_stack();
    
    Init(prompt_name, command_history);
    
    while(1) {
        if(prompt_name[0] != '\0') { printf("%s", prompt_name); }
        printf("%%");

        current_command.com_pathname_ = GetKBInput();
        if(strcmp(current_command.com_pathname_, "exit") == 0) {
            break;
        } else if (strcmp(current_command.com_pathname_, "pwd") == 0) {
            pwd();
        }
    }
    
    FreeShellVars(prompt_name, command_history);
    free(current_command.com_pathname_);
    printf("goodbye.\n");
    
    return 0;
}
