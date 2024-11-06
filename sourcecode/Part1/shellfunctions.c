#include "include/shellfunctions.h"
#include "include/token.h"

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
    
    for(int i = 0; i < command->argc_; ++i) {
        if(command->argv_[i] != NULL) {
            strcat(commandString, command->argv_[i]);
            if(i < command->argc_ - 1) {
                strcat(commandString, " ");
            }
        }
    }
    
    printf("Command String: %s\n", commandString);
    push_stack(stack, commandString);
    free(commandString);
}

char * StrGetCommandHistory(Stack *stack, char* query) {
    Node *temp = stack->top;
    char * string_found = "";

    while(temp != NULL) {
        
        char * current_string_line = (char*) malloc (MAX_STR_SIZE * sizeof(char));
        strcpy(current_string_line, temp->data);

        if(strstr(query, current_string_line) != NULL) {
            string_found = (char*) malloc(strlen(current_string_line) * sizeof(char));
            strcpy(string_found, current_string_line);
            free(current_string_line);
            break;
        }

        free(current_string_line);
        temp = temp->next;
    }
    if(strcmp(string_found, "") == 0) {
        printf("Command not found.\n");
        return "empty";
    }

    temp = NULL;
    return string_found;
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

    temp = NULL;
    printf("Command not found.\n");
    return "empty.";
}

void HistoryFetch(Stack* command_history, Command command) {
    char * string_found = "";
    char *temp_tokens[MAX_NUM_TOKENS];

    if(atoi(command.com_pathname_ + 1) == 0) {
        string_found = 
            StrGetCommandHistory(command_history, command.com_pathname_);
    }
    else {
        int query = atoi(command.com_pathname_);
        string_found =
            IntGetCommandHistory(command_history, query);
    }
    tokenise(string_found, temp_tokens);
    printf("%s\n", string_found);
    printf("%s\n", temp_tokens[0]);
    
    //execvp(string_found, temp_tokens);
    if(strcmp(string_found, "") != 0) {
        free(string_found);
    }
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