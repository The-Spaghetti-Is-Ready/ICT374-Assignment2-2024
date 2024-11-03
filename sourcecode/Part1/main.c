#include "include/shellfunctions.h"
#include "include/token.h"
#include "include/command.h"

int main()
{
    char* prompt_name = "";
    Command commands[MAX_COMMAND_HISTORY];
    char* tokens[MAX_NUM_TOKENS];
    Command current_command = { "", 0, {""}, "", "", NULL };
    int current_pid = 0;
    bool shell = true;

    for(int i = 0; i < MAX_COMMAND_HISTORY; ++i) {
        initialiseCommand(&commands[i]);
    }

    while(shell) {
        if(prompt_name[0]!= '\0'){ printf("%s ", prompt_name); }
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
            else if(strcmp(commands[i].com_pathname_, "prompt") == 0) {
                ReplaceString(commands[i].argv_[1], &prompt_name);
            }
            else {
                int * child_status = 0;
                if ((current_pid = fork()) <  0) 
                {
                    perror("fork");
                    exit(1);
                }
                if(current_pid == 0)
                {
                    executeCommand(commands[i]);
                    exit(0);
                }
                waitpid(current_pid, child_status, 0); //wait until process changes state/finishes.
            }
        }
        
        if(strcmp(current_command.com_pathname_, "exit") == 0) {
            shell = false;
        }
    }
    
    FreeShellVars(prompt_name);
    free(current_command.com_pathname_);

    printf("goodbye.\n");
    
    return 0;
}
