#include "include/shellfunctions.h"

int main() {
    char* promptName = "";
    Command current_command;
    char* command_history[MAX_COMMAND_HISTORY];

    Init(promptName, command_history);
            

    FreeShellVars(promptName, command_history);

    return 0;
}
