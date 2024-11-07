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
    CommandToString(command, commandString);
    
    printf("Command String: %s\n", commandString);
    push_stack(stack, commandString);
    free(commandString);
}

void CommandToString(const Command* command, char* dest) {
    const char null_term = '\0';
    strcpy(dest, command->com_pathname_);
    for(int i = 1; i < command->argc_ - 1; ++i) {
        strcat(dest, " ");
        if(command->argv_[i] != NULL) {
            strncat(dest, command->argv_[i], strlen(command->argv_[i]));
        }
        strncat(dest, &null_term, 1);
    }
}

const char * StrGetCommandHistory(const Stack *stack, char* query) {
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
    return  "";
}

const char * IntGetCommandHistory(const Stack *stack, int query) {
    Node *temp = stack->top;
    int i = 0;

    while(i != query) {
        if(temp == NULL) {
            return "";
        }
        if(strcmp(temp->data, "") != 0) {
            return temp->data;
        }
        if(temp->next == NULL) {
           break;
        }
        ++i;
    }
    return "";
}

const char * HistoryFetch(const Stack* command_history, Command command) {
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
        const char * found = 
            StrGetCommandHistory(command_history, full_command);
        free(full_command);
        return found;
    }
    else {
        int query = atoi(full_command);
        const char * found =
            IntGetCommandHistory(command_history, query);
        free(full_command);
        return found;
    }
    if(strcmp(full_command, "") != 0) {
        free(full_command);
    }
    return "";
}

void ExecuteFromHistory(const Stack * command_history, const Command command) {
    const char * found_str = HistoryFetch(command_history, command);
    char * command_str = (char*) malloc(MAX_STR_SIZE * sizeof(char));

    strcpy(command_str, found_str);

    char *found_tokens[MAX_NUM_TOKENS];
    Command commands[MAX_COMMAND_HISTORY];

    int pid = 0;
    int *current_child_status = 0;

    if(strcmp(command_str, "") != 0) {
        printf("Found command: %s\n", command_str);
        
        for(int i = 0; i < MAX_COMMAND_HISTORY; ++i) {
            initialiseCommand(&commands[i]);
        }

        commands[0].com_pathname_ = command_str;
        printf("%s\n" , commands[0].com_pathname_);

        tokenise(command_str, found_tokens);
        separateCommands(found_tokens, commands);

        FilterExecution(pid, current_child_status, commands);
    }
    else {
        printf("Command not found.\n");
    }
    free(command_str);
}

void RedirectOutput(int current_pid, int* current_child_status, Command command) {
    // fork process
    if ((current_pid = fork()) <  0) 
    {
        perror("fork");
        exit(1);
    }
    if(current_pid == 0)
    {
        // Convert to string for popen
        char* commandString = malloc(MAX_STR_SIZE * sizeof(char));
        CommandToString(&command, commandString);

        // open the command for reading
        FILE* fp;
        char path[MAX_STR_SIZE];
        fp = popen(commandString, "r");

        if(fp == NULL) {
            perror("popen");
            exit(EXIT_FAILURE);
        }

        // Read the output line by line and save it to the file
        while(fgets(path, sizeof(path), fp) != NULL) {
            printf("%s", path);
        }

        pclose(fp);
        // free the memory used by the command string
        free(commandString);
        exit(0);
    }
    waitpid(current_pid, current_child_status, 0); //wait until process changes state/finishes.
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

        if(commands[i].redirect_out_ != NULL) {
            RedirectOutput(current_pid, current_child_status, commands[i]);
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

void ExpandWildcards(const char* pattern) {
    glob_t glob_results;
    int flags = 0;

    if(glob(pattern, flags, NULL, &glob_results) == 0) {
        // iterate over any matches found + display them to the screen
        for(size_t i = 0; i < glob_results.gl_pathc; ++i) {
            printf("%s\n", glob_results.gl_pathv[i]);
        }
    } else {
        printf("No matches found for pattern: %s\n", pattern);
    }

    globfree(&glob_results);
}