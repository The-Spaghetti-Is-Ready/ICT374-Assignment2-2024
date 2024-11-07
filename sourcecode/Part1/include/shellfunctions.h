#ifndef COMMANDFUNCTIONS_H
#define COMMANDFUNCTIONS_H

#include "command.h"
#include "stack.h"
#include "token.h"

#if defined(__linux__)
    #include <linux/limits.h>
#elif defined(__APPLE__) && defined(__MACH__)
    #include <sys/syslimits.h>
#else
    #include <limits.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <glob.h>

#define MAX_COMMAND_HISTORY 100

#if defined(LINE_MAX)
    #define MAX_STR_SIZE LINE_MAX
#elif defined(_POSIX_ARG_MAX)
    #define MAX_STR_SIZE _POSIX_ARG_MAX / sizeof(char)
#else
    #define MAX_STR_SIZE 4096  // Default to 4096 if neither is available
#endif

enum ArrowKey {
    ARROW_UP,
    ARROW_DOWN,
    ARROW_NONE
};

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
 * @author Niamh
 * @brief Print the current working directory
 */
void pwd();

/**
 * @author Niamh
 * @brief Change the current working directory
 * @param path The path to change to
 */
void cd(char* path);

/**
 * @author Niamh
 * @brief Adds a command to the command history stack
 * 
 * @param stack The stack to add the command to
 * @param command The command to add to the stack
 */
void AddCommandToHistory(Stack* stack, Command* command);

/**
 * @brief Retrieve a command from the history list
 * @param stack The structure to retrieve from
 * @param query A string query for searching the structure
 * @return The command that was found in the history stack
 */
const char * StrGetCommandHistory(const Stack *stack, char * query);

/**
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
 * @brief Enables raw mode by disabling echo and canonical mode
 * 
 * @param orig_termios The original terminal settings
 */
void EnableRawMode(struct termios* orig_termios);

/**
 * @author Niamh
 * @brief Disables raw mode
 * 
 * @param orig_termios The original terminal settings
 */
void DisableRawMode(struct termios* orig_termios);

/**
 * @brief Read a key from the keyboard
 * @return The key read
 */
int ReadKey();

enum ArrowKey ReadArrowKey();

/**
 * @author Niamh
 * @brief Expands the wildcards given
 * @param pattern the filename with the wildcard character
 */
void ExpandWildcards(const char* pattern);

#endif