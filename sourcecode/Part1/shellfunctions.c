#include "include/shellfunctions.h"

void FreeShellVars(char* prompt) {
    //free prompt name
    if(prompt[0] != '\0') {
        free(prompt);
    }
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

void executeCommand(Command command)
{
    char * str_command = (char *) malloc(strlen(command.com_pathname_ + 6) * sizeof(char));
    strcat(str_command, "/bin/");
    strcat(str_command, command.com_pathname_);
    
    execv(str_command, command.argv_);

    free(str_command); //free parsed command
}