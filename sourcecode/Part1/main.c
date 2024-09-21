#include "include/shellfunctions.h"

int main()
{
    char* promptName = "";
    Command current_command = { "", 0, {""}, "", "", ' ' };

    while(1) {
        if(promptName[0] != '\0') { printf("%s", promptName); }
        printf("%%");

        current_command.com_pathname = GetKBInput();
        if(strcmp(current_command.com_pathname, "exit") == 0) {
            break;
        }
    }
    
    free(current_command.com_pathname);
    printf("goodbye.\n");

    return 0;
}
