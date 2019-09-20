#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linenoise/linenoise.h"
#include "menu.h"
#include "parser.h"
#include "defines.h"

void execute(ParsedData pData) {

}

int main(int argc, char *argv[]) {
    linenoiseInstallWindowChangeHandler();
    linenoiseHistoryLoad(HISTORY_FILE);
    linenoiseSetCompletionCallback(completionHook);

    // Args Handler
    while(argc > 1) {
        argc--;
        argv++;
        if (!strcmp(*argv, "-h") || !strcmp(*argv, "--help")) {
            menu();
        }
    }

    while (1) {
        char* line = linenoise(PROMPT);

        if (line) {
            if (*line != '\0') {
                linenoiseHistoryAdd(line);

                ParsedData *pData = parser(line);

                if (pData) {
                    execute(pData);
                }

                free(pData);
            }
            
            free(line);
        } else {
            break;
        }
    }

    linenoiseHistorySave(HISTORY_FILE);
    linenoiseHistoryFree();

    return 0;
}
