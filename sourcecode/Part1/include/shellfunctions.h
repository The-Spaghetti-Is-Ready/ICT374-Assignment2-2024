#ifndef COMMANDFUNCTIONS_H
#define COMMANDFUNCTIONS_H

#include "command.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define MAX_STR_SIZE 256

/**
 * @brief Get the Input string from user keyboard
 * 
 * @return The keyboard input string 
 */
char * GetKBInput(); 

#endif