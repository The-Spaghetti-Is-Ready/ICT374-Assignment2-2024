#ifndef COMMAND_H
#define COMMAND_H

#define MAX_ARGS 150
#define pipeSeparator "|"
#define concurrentSeparator "&"
#define sequentialSeparator ";"


#include <stdio.h>

typedef struct Command { 
    char *com_pathname_;     // path name of the command
    int argc_;               // the number of arguments to the command including command itself
    
    char *argv_[MAX_ARGS];   // pointers to strings, each string
    // is an argument for the command, including
    // argument "0". The last pointer should be set to NULL. 

    char *redirect_in_;      // if this is not NULL, then the standard input
    // is redirected to the given file name

    char *redirect_out_;     // if this is not NULL, then the standard output
    // is redirected to the given file name

    char* com_suffix_;        // ' ' - no command suffix (last command);
    // '&' - the command is followed by '&'
    // ';' - the command is followed by ';';
    // '|' - the command is followed by '|'.
} Command;

/**
 * @brief Initialise the command
 * 
 * @param command The command to be initialised
 */
void initialiseCommand(Command *command);

/**
 * @brief Check if the token is a separator
 * 
 * @param token The token to be checked
 * 
 * @return 1 if the token is a separator, 0 otherwise
 */
int isSeparator(char* token);

/**
 * @brief Separate the commands from the tokens
 * 
 * @param tokens The tokens to be separated
 * @param commands The commands to be filled
 * 
 * @return The number of commands separated
 * @retval 0 No commands
 * @retval -1 No tokens
 * @retval -2 Two consecutive separators
 * @retval -3 Missing command
 * @retval -4 Missing command suffix
 */
int separateCommands(char* tokens[], Command commands[]);

#endif