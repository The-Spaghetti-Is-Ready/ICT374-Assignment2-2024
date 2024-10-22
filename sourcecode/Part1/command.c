#include "command.h"

void initialiseCommand(Command *command) {
    command->com_pathname_ = "";
    command->argc_ = 0;
    command->redirect_in_ = NULL;
    command->redirect_out_ = NULL;
    command->com_suffix_ = NULL;
    unsigned long int argv_size = sizeof(command->argv_) / sizeof(command->argv_[0]);  //cache the size calculation.

    for (unsigned long int i = 0; i < argv_size; ++i) {
        command->argv_[i] = NULL;
    }
}

int isSeparator(char* token) {
    char* separators[] = { pipeSeparator, concurrentSeparator, sequentialSeparator, NULL};
    int i = 0;

    while (separators[i] != NULL) {
        if (strcmp(token, separators[i]) == 0) {
            return 1;
        }
        ++i;
    }
    return 0;
}

int separateCommands(char* tokens[], Command commands[]) {
    int numToks = 0;

    // Find out the number of tokens
    while (tokens[numToks] != NULL) {
        numToks++;
    }

    // Return if there are no tokens
    if (numToks == 0) {
        return 0;
    }

    // Check for the first token
    if (isSeparator(tokens[0])) {
        return -3; // -3 is an error code for a missing command
    }
    
    // TODO: Check for the last token, and figure out what we're doing with it
    if(!isSeparator(tokens[numToks-1])) {
        tokens[numToks] = sequentialSeparator; // TODO check here for memory bounds
        ++numToks;
    }

    // Loop through the tokens
    int first = 0; // The first token of the command
    int last = 0; // The last token of the command
    char* separator = NULL; // The separator for the command
    int commandCount = 0; // The number of commands

    for (int i = 0; i < numToks; ++i) {
        last = i;
        if (isSeparator(tokens[i])) {
            separator = tokens[i];

            // Check for two consecutive separators
            if (first == last) {
                return -2; // -2 is an error code for two consecutive separators
            }
            
            // Construct command object
            commands[commandCount].com_pathname_ = tokens[first];
            commands[commandCount].argc_ = last - first + 1;
            commands[commandCount].com_suffix_ = separator;

            // TODO: Account for redirection
            for (int j = 0; j < commands[commandCount].argc_; ++j)
            {
                if(!isSeparator(tokens[first + j])) {
                    commands[commandCount].argv_[j] = tokens[first + j];
                }  
            }

            ++commandCount;
            first = i + 1;
        }

        if(strcmp(tokens[last], pipeSeparator) == 0) {
            return -4;
        }
    }
        return commandCount;
}