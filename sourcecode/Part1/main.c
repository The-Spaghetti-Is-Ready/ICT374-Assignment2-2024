#include "include/shellfunctions.h"
#include "include/token.h"
#include "include/command.h"
#include <fcntl.h>
extern char **environ;

int main()
{
    char* prompt_name = "";
    Command commands[MAX_COMMAND_HISTORY];
    char* tokens[MAX_NUM_TOKENS];
    Stack *command_history = create_stack();
    Command current_command = { "", 0, {""}, "", "", NULL };
    
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

        if(strcmp(current_command.com_pathname_, "exit") == 0) {
            break;
        }

        for (int i = 0; i < numCommands; ++i)
        {
            AddCommandToHistory(command_history, &commands[i]);   
            // if(commands[i].argc_ > 0) {
            //     printf("Argc: %d\n", commands[i].argc_);
            //     for(int j = 0; j < commands[i].argc_; ++j) {
            //         printf("Argv[%d]: %s\n", j, commands[i].argv_[j]);
            //     }
            // }
                 pid_t pid;
                for(int j = 0; j < commands[i].argc_ -1; ++j) {
                   
                   
                    if(strchr(commands[i].argv_[j],  '*') != '\0' || strchr(commands[i].argv_[j],  '?') != '\0') {
                           ExpandWildcards(commands[i].argv_[j]);
                    }
                    
                    if (strchr(commands[i].argv_[j], '<') != '\0') {
                        // FORK out to a different process
                         pid = fork();
                        
                        if(pid == 0) {
                            int fd = open(commands[i].argv_[j+1], O_RDONLY);
                            if(fd == -1) {
                                perror("open");
                            }
                            dup2(fd, STDIN_FILENO);
                            close(fd);
                            exit(0);
                        }

                    }

                    if (strchr(commands[i].argv_[j], '>') != '\0') {
                        pid = fork();
                        if(pid == 0) {
                            int fd = open(commands[i].argv_[j+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                            if(fd == -1) {
                                perror("open");
                            }

                            dup2(fd, STDIN_FILENO);
                            close(fd);
                            exit(0);
                        }
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
            else {
                FilterExecution(current_pid, current_child_status, commands);
            }
            initialiseCommand(&commands[i]);
        }
    }
    
    // print out command history stack for testing purposes
    while (!empty_stack(command_history))
    {
        printf("%s\n", pop_stack(command_history));
    }

    FreeShellVars(prompt_name, command_history);
    free(commands[0].com_pathname_);
    printf("goodbye.\n");
    
    return 0;
}
