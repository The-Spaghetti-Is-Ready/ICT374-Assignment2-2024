#include "include/shellfunctions.h"

void Init(char* prompt, char** command_history) {
    prompt = (char*) malloc(MAX_STR_SIZE * sizeof(char));
    
    for(int current = 0; current < MAX_COMMAND_HISTORY; ++current) {
        command_history[current] = (char*) malloc(MAX_STR_SIZE * sizeof(char));
    }
}

void FreeShellVars(char* prompt, char** command_history) {
    //free prompt name
    if(prompt[0] != '\0') {
        free(prompt);
    }

    //free command history
    for(int current = 0; current < MAX_COMMAND_HISTORY; ++current) {
        if(command_history[current][0] != '\0') {
            free(command_history[current]);
        }
    }
}

void ReplaceString(char* new_string, char* current_string) {

    if(current_string[0] != '\0')
    {
        free(current_string); //make string null if it is initialized. Pre-condition is that prompt is in heap.
    }
   
    current_string = (char *) malloc(sizeof(new_string) * sizeof(char)); //can allocate here as it would be null by this point
    strcpy(current_string, new_string);
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
    DIR *dir = opendir(path);
    struct dirent *entry;

    if(!dir) {
        printf("Directory does not exist\n");
    } 

    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);  // Print the name of the file/directory
    }
}