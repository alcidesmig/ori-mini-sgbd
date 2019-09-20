#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linenoise/linenoise.h"
#include "menu.h"
#include "tools.h"
#include "parser.h"

#define HISTORY_FILE "./.history"
#define PROMPT "\x1b[1;32mSGBD\x1b[0m> "

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
                    printf("%s\n", pData->command);
                    printf("%d\n", pData->data.table.cols);
                    printf("%d\n", pData->data.table.rows);
                    printf("%s\n", pData->data.table.name);

                    for (int i = 0; i < pData->data.table.cols; i++) {
                        printf("%c %s\n", pData->data.table.types[i], pData->data.table.fields[i]);
                    }
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
