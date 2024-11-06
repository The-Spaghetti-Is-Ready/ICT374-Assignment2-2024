#include "include/shellfunctions.h"
#include "include/token.h"
#include "include/command.h"

extern char **environ;

int main()
{
    char* prompt_name = "";
    Command commands[MAX_COMMAND_HISTORY];
    char* tokens[MAX_NUM_TOKENS];
    Stack *command_history = create_stack();
  
    int current_pid = 0;
    int * current_child_status = 0;

    for(int i = 0; i < MAX_COMMAND_HISTORY; ++i) {
        initialiseCommand(&commands[i]);
    }

    while(1) {
        if(prompt_name[0]!= '\0'){ printf("%s ", prompt_name); }
        printf("%% ");

        commands[0].com_pathname_ = GetKBInput();
       
        tokenise(commands[0].com_pathname_, tokens);

        int numCommands = separateCommands(tokens, commands);
        
        if(strcmp(commands[0].com_pathname_, "exit") == 0) {
            break;
        }

        for (int i = 0; i < numCommands; ++i)
        {
            AddCommandToHistory(command_history, &commands[i]);   

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
                while (!empty_stack(command_history))
                {
                    printf("%s\n", pop_stack(command_history));
                }
            }
            else if(strstr(commands[i].com_pathname_, "!") != NULL) {
                HistoryFetch(command_history, commands[i]);
            }
            else {
                FilterExecution(current_pid, current_child_status, commands);
            }
            initialiseCommand(&commands[i]);
        }
    }

    FreeShellVars(prompt_name, command_history);
    free(commands[0].com_pathname_);
    printf("goodbye.\n");
    
    return 0;
}
