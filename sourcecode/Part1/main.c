#include "include/shellfunctions.h"
#include "include/token.h"
#include "include/command.h"

int main()
{
    char* prompt_name = "";
    Command commands[MAX_COMMAND_HISTORY];
    char* tokens[MAX_NUM_TOKENS];
    Command current_command = { "", 0, {""}, "", "", NULL };
    
    Init(prompt_name);
    
    for(int i = 0; i < MAX_COMMAND_HISTORY; ++i) {
        initialiseCommand(&commands[i]);
    }

    while(1) {
        if(prompt_name[0] != '\0') { printf("%s", prompt_name); }
        printf("%%");

        current_command.com_pathname_ = GetKBInput();
        tokenise(current_command.com_pathname_, tokens);
        
        int numCommands = separateCommands(tokens, commands);

        for (int i = 0; i < numCommands; ++i)
        {
            printf("Command %d: %s\n", i, commands[i].com_pathname_);
            if(strcmp(commands[i].com_pathname_, "cd") == 0) {
                cd(commands[i].argv_[1]);
            }
            else if(strcmp(commands[i].com_pathname_, "pwd") == 0) {
                pwd();
            }
        }
        
        if(strcmp(current_command.com_pathname_, "exit") == 0) {
            break;
        }
    }
    
    FreeShellVars(prompt_name);
    free(current_command.com_pathname_);
    printf("goodbye.\n");
    
    return 0;
}
