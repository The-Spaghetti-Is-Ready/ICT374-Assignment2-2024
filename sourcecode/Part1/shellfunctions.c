#include "include/shellfunctions.h"

void FreeShellVars(char* prompt,  Stack* command_history) {
    //free prompt name
    if(prompt[0] != '\0') {
        free(prompt);
    }

    //free command history stack
    clear_stack(command_history);
   
}

void ReplaceString(char* new_string, char** current_string) {

    if(*current_string[0] != '\0')
    {
        free(*current_string); //make string null if it is initialized. Pre-condition is that prompt is in heap.
    }
    if(new_string[0] != '\0') {
        *current_string = (char *) malloc(sizeof(new_string) * sizeof(char)); //can allocate here as it would be null by this point
        strcpy(*current_string, new_string);
    }
}

char * GetKBInput() { //get input from keyboard
    char * input = (char *) malloc(MAX_STR_SIZE * sizeof(char)); //128 character-limit
    
    fgets(input, MAX_STR_SIZE, stdin);
    
    input[strcspn(input, "\n")] = '\0'; //remove newline

    fseek(stdin,0,SEEK_END); //clear input buffer

    return input;
}

void pwd() {
    char cwd[PATH_MAX]; // PATH_MAX is the maximum number of characters in a path name defined in limits.h 
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}

void cd(char* path) {
    if (path == NULL)
    {
        path = getenv("HOME");
    }
    
    if(chdir(path) != 0) {
        perror("chdir() error");
    } else {
        printf("Directory changed to %s\n", getcwd(NULL, 0));
    }
}

void AddCommandToHistory(Stack* stack, Command* command) {
    char* commandString = malloc(MAX_STR_SIZE * sizeof(char));
    const char null_term = '\0';
    strcpy(commandString, command->com_pathname_);
    for(int i = 1; i < command->argc_ - 1; ++i) {
        strcat(commandString, " ");
        if(command->argv_[i] != NULL) {
            strncat(commandString, command->argv_[i], strlen(command->argv_[i]));
        }
    }
    strncat(commandString, &null_term, 1);
    
    printf("Command String: %s\n", commandString);
    push_stack(stack, commandString);
    free(commandString);
}

char * StrGetCommandHistory(Stack *stack, char* query) {
    Node *temp = stack->top;

    while(temp != NULL) {
        if(temp == NULL) {
            return "";
        }

        char * current_str = (char*) malloc (MAX_STR_SIZE * sizeof(char));
        strcpy(current_str, temp->data);

        if(strstr(current_str, query) != NULL) {
            return temp->data;
            free(current_str);
            break;
        }

        free(current_str);
        if(temp->next == NULL) {
            break;
        }
        temp = temp->next;
    }
    printf("Command not found.\n");
    return  "";
}

char * IntGetCommandHistory(Stack *stack, int query) {
    Node *temp = stack->top;
    int i = 0;

    while(i != query) {
        temp = temp->next;
        ++i;
    }
    if(strcmp(temp->data, "") != 0) {
        return temp->data;
    }

    printf("Command not found.\n");
    return "";
}

char * HistoryFetch(Stack* command_history, Command command) {
    char *full_command = (char*) malloc(MAX_STR_SIZE * sizeof(char));
    char * temp = command.com_pathname_ + 1;

    strcpy(full_command, temp); //deconstruct command back to line.
    for(int i = 1; i < (command.argc_ - 1); ++i) {
        if(command.argv_[i][0] == '\0') { break; }
        strcat(full_command, " ");
        strncat(full_command, 
            command.argv_[i], 
            strlen(command.argv_[i])
        );
    }

    if(atoi(full_command) == 0) {
        char * found = 
            StrGetCommandHistory(command_history, full_command);
        free(full_command);
        return found;
    }
    else {
        int query = atoi(full_command);
        char * found =
            IntGetCommandHistory(command_history, query);
        free(full_command);
        return found;
    }
    if(strcmp(full_command, "") != 0) {
        free(full_command);
    }
    return "";
}

void ExecuteCommand(Command command) {
    char * str_command = (char *) malloc(strlen(command.com_pathname_ + 6) * sizeof(char));
    strcpy(str_command, "/bin/");
    strncat(str_command, command.com_pathname_, strlen(command.com_pathname_));
    
    execvp(str_command, command.argv_);

    free(str_command); //free parsed command
}

void SequentialExecution(int current_pid, int* current_child_status, Command command) {
    if ((current_pid = fork()) <  0) 
    {
        perror("fork");
        exit(1);
    }
    if(current_pid == 0)
    {
        ExecuteCommand(command);
        exit(0);
    }
    waitpid(current_pid, current_child_status, 0); //wait until process changes state/finishes.
}

void ConcurrentExecution(Command command) {
    int current_pid = 0;
    if ((current_pid = fork()) <  0) 
    {
        perror("fork");
        exit(1);
    }
    if(current_pid == 0)
    {
        ExecuteCommand(command);
        exit(0);
    }
}

void PipeCommand(Command current_command, Command next_command) {

}

void FilterExecution(int current_pid, int *current_child_status, Command commands[]) {
    
    char current_suffix = ' ';

    for(int i = 0; i < MAX_COMMAND_HISTORY; ++i) { 
        if(commands[i].com_suffix_ != NULL) {
            current_suffix = commands[i].com_suffix_[0];
        }

        if(i == 0) {
            SequentialExecution(current_pid, current_child_status, commands[i]);
        }
        if(commands[i].com_pathname_[0] != '\0' && i < MAX_COMMAND_HISTORY) {
            switch(current_suffix) {
                case ';':
                    SequentialExecution(current_pid, current_child_status, commands[i+1]);
                    printf("\n");
                break;
                case '&':
                    ConcurrentExecution(commands[i+1]);
                break;
                case '|':
                    PipeCommand(commands[i], commands[i+1]);
                break;
                default:
                    printf("here4\n");
                    SequentialExecution(current_pid, current_child_status, commands[i]); 
                    //sequential execution waits for current process to finish, so it's appropriate for default cases 
                    //where only one command is present
                break;
            }
        }
    }
}