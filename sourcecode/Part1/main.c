#include "include/shellfunctions.h"

int main() {
    char* prompt_name = "";
    Command current_command;
    char* command_history[MAX_COMMAND_HISTORY];

    Init(prompt_name, command_history);
            

    FreeShellVars(prompt_name, command_history);

    return 0;
}
