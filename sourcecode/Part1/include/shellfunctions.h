#ifndef COMMANDFUNCTIONS_H
#define COMMANDFUNCTIONS_H

#include "command.h"
#include "stack.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <linux/limits.h>
#include <glob.h>

#define MAX_COMMAND_HISTORY 100

#if defined(LINE_MAX)
    #define MAX_STR_SIZE LINE_MAX
#elif defined(_POSIX_ARG_MAX)
    #define MAX_STR_SIZE _POSIX_ARG_MAX / sizeof(char)
#else
    #define MAX_STR_SIZE 4096  // Default to 4096 if neither is available
#endif

/**
 * @brief Initialize Unix shell environment
 * 
 * @pre The program executable has been built
 * @post The shell variables are initialized to a usable state
 */
void Init(char* prompt);

/**
 * @brief Frees all the heap-allocated shell variables
 * 
 * @param prompt The shell name
 * @param command_history The shell command history
 * 
 * @pre Shell variables have been allocated memory
 * @post Shell variables are freed
 */
void FreeShellVars(char* prompt, Stack* command_history);

/**
 * @brief Replaces heap-allocated string with a new specified string.
 * 
 * @param new_string The new string to be put into old string
 * @param current_string The string to be replaced
 * 
 * @pre All strings have been heap-allocated (Through unknown-length user input)
 * @post current_string is made null if not already. New_string gets assigned to current_string.
 */
void ReplaceString(char* new_string, char** current_string);

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
 * @param path The path to change to
 */
void cd(char* path);

void AddCommandToHistory(Stack* stack, Command* command);

/**
 * @brief Retrieve a command from the history list
 * @param stack The structure to retrieve from
 * @param query A string query for searching the structure
 */
char * StrGetCommandHistory(Stack *stack, char * query);

/**
 * @brief Retrieve a command from the history list
 * @param stack The structure to retrieve from
 * @param query A string query for searching the structure
 */
char * IntGetCommandHistory(Stack *stack, int query);

char * HistoryFetch(Stack* command_history, Command command);

/**
 * @author Marco
 * @brief Executes a command using the 'execl' function.
 * @param command The command to be executed.
 */
void ExecuteCommand(Command command);

/**
 * @author Marco
 * @brief Excutes a set of commands sequentially based on ';' suffix
 * @param commmands the command list to execute
 */
void SequentialExecution(int current_pid, int* current_child_status, Command command);

/**
 * @author Marco
 * @brief Executes a set of commands concurrently based on '&' suffix
 * @param commands the command to execute
 */
void ConcurrentExecution(Command command);

/**
 * @author Marco
 * @brief Executes a set of commands and pipes them
 * @param commands the commands to execute
 */
void PipeCommand(Command current_command, Command next_command);

/**
 * @author Marco
 * @brief filters through commands array and executes concurrently or sequantially depending on suffix
 * @param commands the command list to execute
 */
void FilterExecution(int current_pid, int *current_child_status, Command commands[]);

/**
 * @author Niamh
 * @brief Expands the wildcards given
 * @param pattern the filename with the wildcard character
 */
void ExpandWildcards(const char* pattern);

/**
 * @author Niamh
 * @brief Converts a command to a string
 * @param command the command to convert
 * @param dest the destination string
 */
void CommandToString(const Command* command, char* dest);

void RedirectOutput(int current_pid, int* current_child_status, Command command);

#endif