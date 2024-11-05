#include "include/shellfunctions.h"
#include "include/token.h"
#include "include/command.h"

extern char **environ;

int main()
{
    // Save the current terminal settings
    struct termios terminal_settings;

    EnableRawMode(&terminal_settings); // This can go into an init function

    char* prompt_name = "";
    Command commands[MAX_COMMAND_HISTORY];
    char* tokens[MAX_NUM_TOKENS];
    Stack *command_history = create_stack();
    Node* temp_node = NULL;
    Command current_command = { "", 0, {""}, "", "", NULL };
    bool exit_shell = false;

    int current_pid = 0;
    int * current_child_status = 0;

    for(int i = 0; i < MAX_COMMAND_HISTORY; ++i) {
        initialiseCommand(&commands[i]);
    }

    while(!exit_shell) {
        if(prompt_name[0]!= '\0'){ printf("%s ", prompt_name); }
        printf("%% ");

        char c;
        char input_buffer[MAX_STR_SIZE] = {0};
        int input_length = 0;
        // Create temporary node to store the command
        if(temp_node == NULL) {
            temp_node = command_history->top;
        }
        
        while(read(STDIN_FILENO, &c, 1) == 1) { // is there a way to refactor this into a function?
            // If the user presses escape
            if(c == 27) { // can this be a switch case? or is it an if statement cause its only one?
                char seq[3];
                if (read(STDIN_FILENO, &seq[0], 1) != 1) continue;
                if (read(STDIN_FILENO, &seq[1], 1) != 1) continue;

                if(seq[0] == '[') {
                    if(seq[1] == 'A') {
                        // Up arrow
                        if(temp_node->next != NULL) {
                            temp_node = temp_node->next;
                        } else {
                            temp_node = command_history->top;
                        }
                        
                    } else if(seq[1] == 'B') {
                        // Down arrow
                        if(temp_node->prev != NULL) {
                            temp_node = temp_node->prev;
                        } else {
                            temp_node = command_history->bottom;
                        }
                    }
                    if(temp_node != NULL) {
                        write(STDOUT_FILENO, temp_node->data, strlen(temp_node->data));
                        fflush(stdout);
                    }
                }
            } else if (c == 127) {
                // backspace
                if(input_length > 0) {
                    input_length--;
                    input_buffer[input_length] = '\0';
                    write(STDOUT_FILENO, "\b \b", 3);
                }
            } else if (c == '\n') {
                if(input_buffer[0] != '\0') {
                    // strcpy(commands[0].com_pathname_, input_buffer);
                    commands[0].com_pathname_ = (char*)input_buffer;
                    tokenise(commands[0].com_pathname_, tokens);

                    int numCommands = separateCommands(tokens, commands);
        
                    if(strcmp(commands[0].com_pathname_, "exit") == 0) {
                        exit_shell = true;
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
                memset(input_buffer, 0, sizeof(input_buffer));
                break;
            } else {
                input_buffer[input_length] = c;
                input_length++;
                write(STDOUT_FILENO, &c, 1);
            }
        } 
    }
    
    // print out command history stack for testing purposes
    while (!empty_stack(command_history))
    {
        printf("%s\n", pop_stack(command_history));
    }

    // Here down can be refactored into a cleanup function
    FreeShellVars(prompt_name, command_history);
    //free(temp_node);
    free(commands[0].com_pathname_);
    printf("goodbye.\n");
    DisableRawMode(&terminal_settings);
    return 0;
}
