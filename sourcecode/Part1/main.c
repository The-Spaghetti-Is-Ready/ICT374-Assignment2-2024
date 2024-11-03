#include "include/shellfunctions.h"
#include "include/token.h"
#include "include/command.h"

int main()
{
    char* prompt_name = "";
    Command commands[MAX_COMMAND_HISTORY];
    Command current_command = { "", 0, {""}, "", "", NULL };
    char* tokens[MAX_NUM_TOKENS];
    
    int current_pid = 0;
    int * current_child_status = 0;

    for(int i = 0; i < MAX_COMMAND_HISTORY; ++i) {
        initialiseCommand(&commands[i]);
    }

    while(1) {
        if(prompt_name[0]!= '\0'){ printf("%s ", prompt_name); }
        printf("%% ");

        current_command.com_pathname_ = GetKBInput();
        tokenise(current_command.com_pathname_, tokens);
        
        int numCommands = separateCommands(tokens, commands);

        if(strcmp(current_command.com_pathname_, "exit") == 0) {
            break;
        }

        for (int i = 0; i < numCommands; ++i)
        {
            printf("Command %d: %s\n", i, commands[i].com_pathname_);
            if(strcmp(commands[i].com_pathname_, "cd") == 0) {
                cd(commands[i].argv_[1]);
            }
            else if(strcmp(commands[i].com_pathname_, "pwd") == 0) {
                pwd();
            }
            else if(strcmp(commands[i].com_pathname_, "prompt") == 0) {
                ReplaceString(commands[i].argv_[1], &prompt_name);
            }
            else {
                switch(commands[i].com_suffix_) {
                    case ';':
                    break;
                    case '&':
                    break;
                    case '|':
                    break;
                    default:
                    break;
                }
                if ((current_pid = fork()) <  0) 
                {
                    perror("fork");
                    exit(1);
                }
                if(current_pid == 0)
                {
                    ExecuteCommand(commands[i]);
                    exit(0);
                }
                waitpid(current_pid, current_child_status, 0); //wait until process changes state/finishes.
            }
        }
    }
    
    FreeShellVars(prompt_name);
    free(current_command.com_pathname_);

    printf("goodbye.\n");
    
    return 0;
}
