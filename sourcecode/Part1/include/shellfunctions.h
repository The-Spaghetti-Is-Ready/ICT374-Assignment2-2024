#ifndef COMMANDFUNCTIONS_H
#define COMMANDFUNCTIONS_H

#include "command.h"
#include "stack.h"
#include "token.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <linux/limits.h>
#include <glob.h>
#include <termios.h>

#define MAX_COMMAND_HISTORY 100

#if defined(LINE_MAX)
    #define MAX_STR_SIZE LINE_MAX
#elif defined(_POSIX_ARG_MAX)
    #define MAX_STR_SIZE _POSIX_ARG_MAX / sizeof(char)
#else
    #define MAX_STR_SIZE 4096  // Default to 4096 if neither is available
#endif

/**
 * @author Marco
 * @brief Displays the environment variables. Used for seeing if the env variables are properly inherited.
 * @param envp The environment variables
 */
void DisplayEnv(char *envp[]);

/**
* @author Marco
* @brief Puts the terminal back into cooked mode
* @pre The terminal needs to be in 'raw' mode
*@post The terminal is put in 'cooked' mode
*/
void DisableRawMode();

/**
 * @author Marco
 * @brief Puts the terminal in raw mode if not already
 * @pre Terminal needs to be in 'cooked' mode
 * @post Puts the terminal in 'raw' mode
 */
void EnableRawMode();

/**
 * @author Marco
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
 * @author Marco
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
 * @author Marco
 * @brief Get the Input/output from keyboard and process using raw mode to allow for character-based input
 * 
 * @return The keyboard input string 
 */
char * ProcessKStreams(const char * prompt, Stack *history); 

/**
 * @author Marco
 * @brief Print the current working directory
 */
void pwd();

/**
 * @author Marco
 * @brief Change the current working directory
 * @param path The path to change to
 */
void cd(char* path);

void AddCommandToHistory(Stack* stack, Command* command);

/**
 * @author Marco
 * @brief Retrieve a command from the history list
 * @param stack The structure to retrieve from
 * @param query A string query for searching the structure
 * @return The command that was found in the history stack
 */
const char * StrGetCommandHistory(const Stack *stack, char * query);

/**
 * @author Marco
 * @brief Retrieve a command from the history list
 * @param stack The structure to retrieve from
 * @param query A string query for searching the structure
 * @return The command that was found in the history stack
 */
const char * IntGetCommandHistory(const Stack *stack, int query);

/**
 * @author Marco
 * @brief Fetches a queried command from the history stack
 * @param command_history The command history stack
 * @param command The command to be queried in the stack
 * @return The command that was found in history from query
 */
const char * HistoryFetch(const Stack* command_history, Command command);

/**
 * @author Marco
 * @brief Executes a command that was fetched from  the command history stack
 * @param command_history The command history stack
 * @param command The command specifying the query
 */
void ExecuteFromHistory(const Stack * command_history, const Command command);

/**
 * @author Marco
 * @brief Executes a command using the 'execvp' function.
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

/**
 * @author Niamh
 * @brief Redirects the output of a command to the specified file, and makes a new one if it doesnt exist
 * @param current_pid the current process id
 * @param current_child_status the current child status
 * @param command the command to redirect
 */
void RedirectOutput(int current_pid, int* current_child_status, Command command);

/**
 * @author Niamh
 * @brief Redirects the input of a command to the specified file
 * @param current_pid the current process id
 * @param current_child_status the current child status
 * @param command the command to redirect
 */
void RedirectInput(int current_pid, int* current_child_status, Command command);

/**
 * @author Niamh
 * @brief Redirects the error of a command to the specified file
 * @param current_pid the current process id
 * @param current_child_status the current child status
 * @param command the command to redirect
 */
void RedirectError(int current_pid, int* current_child_status, Command command);

#endif