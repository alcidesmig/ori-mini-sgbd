#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linenoise/linenoise.h"
#include "menu.h"
#include "parser.h"
#include "defines.h"

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
                    char **cmd = &(pData->command);
                    if (*cmd == NULL) {
                        printf("NULL\n");
                        break;
                    } else if (!strncmp(*cmd, CT, CMD_LIMIT)) {
                    } else if (!strncmp(*cmd, RT, CMD_LIMIT)) {
                    } else if (!strncmp(*cmd, AT, CMD_LIMIT)) {
                    } else if (!strncmp(*cmd, LT, CMD_LIMIT)) {
                    } else if (!strncmp(*cmd, IR, CMD_LIMIT)) {
                    } else if (!strncmp(*cmd, BR, CMD_LIMIT)) {
                    } else if (!strncmp(*cmd, AR, CMD_LIMIT)) {
                    } else if (!strncmp(*cmd, RR, CMD_LIMIT)) {
                    } else if (!strncmp(*cmd, CI, CMD_LIMIT)) {
                    } else if (!strncmp(*cmd, RI, CMD_LIMIT)) {
                    } else if (!strncmp(*cmd, GI, CMD_LIMIT)) {
                    } else if (!strncmp(*cmd, EB, CMD_LIMIT)) {
                        printf("Saindo...\n");
                        break;
                    } else {
                        printf("%s\n", *cmd);
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
