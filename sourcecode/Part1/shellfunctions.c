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

void ExecuteCommand(Command command) {
    char * str_command = (char *) malloc(strlen(command.com_pathname_ + 6) * sizeof(char));
    strcat(str_command, "/bin/");
    strcat(str_command, command.com_pathname_);
    
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

void EnableRawMode(struct termios* orig_termios) {
    struct termios raw;

    tcgetattr(STDIN_FILENO, orig_termios);
    raw = *orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void DisableRawMode(struct termios* orig_termios) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios);
}