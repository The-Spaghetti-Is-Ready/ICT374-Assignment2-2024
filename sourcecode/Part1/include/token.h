/*
 * Author: Niamh Wilson
 * Date: 24/09/24
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define MAX_NUM_TOKENS  1000
#define tokenSeparators " \t\n"

int tokenise (char line[], char* token[]);
