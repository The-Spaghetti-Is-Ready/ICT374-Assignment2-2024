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

char * StrGetCommandHistory(Stack *stack, const char* query) {
    Node *temp = stack->top;

    while(temp != NULL) {
        if(strstr(query, temp->data) != NULL) {
            printf("here 1\n");
            return temp->data;
            break;
        }
        else {
            temp = temp->next;
        }
    }
    if(strcmp(temp->data, "") == 0) {
        temp = NULL;
        printf("Command not found.\n");
        return "empty";
    }

    temp = NULL;
    return "empty";
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
    else {
        printf("Command not found.\n");
        return "empty.";
    }
    
    temp = NULL;
    return "empty";
}

void HistoryFetch(Stack * command_history, Command command) {
    char * string_found = "";
    char * temp_tokens[MAX_NUM_TOKENS];

    char * parsed_arg = (char*) malloc(MAX_STR_SIZE * sizeof(char));

    strcpy(parsed_arg, command.com_pathname_ + 1); //get rid of the '!'
    for(int i = 1; i < command.argc_ - 1; ++i) {
        if(strcmp(command.argv_[i], "") == 0) { break ;}
        strcat(parsed_arg, " ");
        strncat(parsed_arg, command.argv_[i], strlen(command.argv_[i]));
    }
    printf("%s\n", parsed_arg);
    if(atoi(parsed_arg) == 0) { //if arg is not an integer
        string_found = 
            StrGetCommandHistory(command_history, parsed_arg);
    }
    else {
        int query = atoi(parsed_arg);
        string_found =
            IntGetCommandHistory(command_history, query);
    }
    tokenise(string_found, temp_tokens);
    printf("here10\n");
    printf("%s %s\n", string_found, temp_tokens[1]);
    
    //execvp(string_found, temp_tokens);
    if(strcmp(string_found, "") != 0) {
        free(string_found);
        printf("here4\n");
    }
    if(strcmp(parsed_arg, "") != 0) {
        free(parsed_arg);
    }
    printf("here3\n");
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