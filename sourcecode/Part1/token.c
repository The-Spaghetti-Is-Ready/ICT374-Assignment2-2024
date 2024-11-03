/*
 * Author: Niamh
 * Date: 24/09/24
 */

#include "token.h"

int tokenise (char line[], char* token[])
{
        char *tk;
        int i=0;

        // tokenise the string using the defined separators
        tk = strtok(line, tokenSeparators);

        token[i] = tk;

        while (tk != NULL) {

                ++i;
                if (i>=MAX_NUM_TOKENS) {
                        i = -1;
                        break;
                }

                tk = strtok(NULL, tokenSeparators);
                token[i] = tk;
    }
      return i;
}