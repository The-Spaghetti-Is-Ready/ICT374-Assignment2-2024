#ifndef COMMANDFUNCTIONS_H
#define COMMANDFUNCTIONS_H

#include "command.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <linux/limits.h>

#define MAX_COMMAND_HISTORY 100
#define MAX_STR_SIZE 256

/**
 * @brief Initialize Unix shell environment
 * 
 * @pre The program executable has been built
 * @post The shell variables are initialized to a usable state
 */
void Init(char* prompt, char** command_history);

/**
 * @brief Frees all the heap-allocated shell variables
 * 
 * @param prompt The shell name
 * @param command_history The shell command history
 * 
 * @pre Shell variables have been allocated memory
 * @post Shell variables are freed
 */
void FreeShellVars(char* prompt, char** command_history);

/**
 * @brief Replaces heap-allocated string with a new specified string.
 * 
 * @param new_string The new string to be put into old string
 * @param current_string The string to be replaced
 * 
 * @pre All strings have been heap-allocated (Through unknown-length user input)
 * @post current_string is made null if not already. New_string gets assigned to current_string.
 */
void ReplaceString(char* new_string, char* current_string);

/**
 * @brief Get the Input string from user keyboard
 * 
 * @return The keyboard input string 
 */
char * GetKBInput(); 

/**
 * @brief Print the current working directory
 */
void pwd();

/**
 * @brief Change the current working directory
 */
void cd(char* path);

#endif