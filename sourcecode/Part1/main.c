#define _POSIX_C_SOURCE 200809L //Accessing certain POSIX features like SIGBLOCK constants

#include "include/shellfunctions.h"
#include "include/token.h"
#include "include/command.h"
#include "include/signals.h"

extern char **environ;

int main(int argc __attribute_maybe_unused__, char **argv __attribute_maybe_unused__, char *envp[])
{
    Command commands[MAX_COMMAND_HISTORY];
    char* tokens[MAX_NUM_TOKENS];
    Stack *command_history = create_stack();
  
    char* prompt_name = (char*) malloc(MAX_STR_SIZE * sizeof(char));
    prompt_name[0] = '\0';

    int current_pid = 0;
    int * current_child_status = 0;

    SetupSigs();

    system("clear"); //clean the screen completely. This is the only instance where system call is used.

    for(int i = 0; i < MAX_COMMAND_HISTORY; ++i) {
        initialiseCommand(&commands[i]);
    }

    while(1) {
        SigBlock(SIG_BLOCK, SIGCHLD);

        commands[0].com_pathname_ = ProcessKStreams(prompt_name, command_history);
       
        tokenise(commands[0].com_pathname_, tokens);

        int numCommands = separateCommands(tokens, commands);
        
        if(strcmp(commands[0].com_pathname_, "exit") == 0) {
            break;
        }

        for (int i = 0; i < numCommands; ++i)
        {
            if(strstr(commands[i].com_pathname_, "!") != NULL) {
                ExecuteFromHistory(command_history, commands[i]);
            }

            AddCommandToHistory(command_history, &commands[i]);   

            for(int j = 0; j < commands[i].argc_ -1; ++j) {
                if(strchr(commands[i].argv_[j],  '*') != (void*)0 || 
                strchr(commands[i].argv_[j],  '?') != (void*)0) {
                    ExpandWildcards(commands[i].argv_[j]);
                }
            }
                
            if(strcmp(commands[i].com_pathname_, "cd") == 0) {
                cd(commands[i].argv_[1]);
            }
            else if(strcmp(commands[i].com_pathname_, "pwd") == 0) {
                pwd();
            }
            else if(strcmp(commands[i].com_pathname_, "prompt") == 0) {
                ReplaceString(commands[i].argv_[1], &prompt_name);
            }
            else if(strcmp(commands[i].com_pathname_, "history") == 0) {
                if(!empty_stack(command_history))
                {
                    for(int i = 0; i < command_history->size; ++i) {
                        printf("\n%s", IntGetCommandHistory(command_history, i));
                    }
                }
            }
            else if(strcmp(commands[i].com_pathname_, "env") == 0 || 
            strcmp(commands[i].com_pathname_, "printenv") == 0) {
                DisplayEnv(envp);
            }
            else {
                FilterExecution(current_pid, current_child_status, commands);
            }
            initialiseCommand(&commands[i]);
        }
        SigBlock(SIG_UNBLOCK, SIGCHLD);
    }
 
    FreeShellVars(prompt_name, command_history);
    free(commands[0].com_pathname_);
    printf("\ngoodbye.\n");
    
    return 0;
}
