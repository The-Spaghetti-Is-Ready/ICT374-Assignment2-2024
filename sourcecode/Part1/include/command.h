#ifndef COMMAND_H
#define COMMAND_H

#define MAX_ARGS 150

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

    char com_suffix_;        // ' ' - no command suffix (last command);
    // '&' - the command is followed by '&'
    // ';' - the command is followed by ';';
    // '|' - the command is followed by '|'.
} Command;


#endif