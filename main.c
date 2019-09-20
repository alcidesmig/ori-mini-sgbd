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
                    if (!strncmp(*cmd, CT, CMD_LIMIT)) {
                        printf("%s\n", pData->command);
                        printf("%d\n", pData->data.table.cols);
                        printf("%d\n", pData->data.table.rows);
                        printf("%s\n", pData->data.table.name);

                        for (int i = 0; i < pData->data.table.cols; i++) {
                            printf("%c %s\n", pData->data.table.types[i], pData->data.table.fields[i]);
                        }
                    } else if (!strncmp(*cmd, RT, CMD_LIMIT)) {
                        printf("%s\n", pData->command);
                        printf("%s\n", pData->data.table.name);
                    } else if (!strncmp(*cmd, AT, CMD_LIMIT)) {
                        printf("%s\n", pData->command);
                        printf("%s\n", pData->data.table.name);
                    } else if (!strncmp(*cmd, LT, CMD_LIMIT)) {
                        printf("%s\n", pData->command);
                    } else if (!strncmp(*cmd, IR, CMD_LIMIT)) {
                        printf("%s\n", pData->command);
                        printf("%d\n", pData->data.row.cols);
                        printf("%s\n", pData->data.row.tableName);

                        for (int i = 0; i < pData->data.row.cols; i++) {
                            printf("%s\n", (char *)pData->data.row.values[i]);
                        }
                    } else if (!strncmp(*cmd, BR, CMD_LIMIT)) {
                        printf("%s\n", pData->command);
                        printf("%c\n", pData->data.selection.parameter);
                        printf("%s\n", pData->data.selection.tableName);
                        printf("%s\n", pData->data.selection.field);
                        printf("%s\n", (char *)pData->data.selection.value);
                    } else if (!strncmp(*cmd, AR, CMD_LIMIT)) {
                        printf("%s\n", pData->command);
                        printf("%s\n", pData->data.table.name);
                    } else if (!strncmp(*cmd, RR, CMD_LIMIT)) {
                        printf("%s\n", pData->command);
                        printf("%s\n", pData->data.table.name);
                    } else if (!strncmp(*cmd, CI, CMD_LIMIT)) {
                        printf("%s\n", pData->command);
                        printf("%c\n", pData->data.selection.parameter);
                        printf("%s\n", pData->data.selection.tableName);
                        printf("%s\n", pData->data.selection.field);
                    } else if (!strncmp(*cmd, RI, CMD_LIMIT)) {
                        printf("%s\n", pData->command);
                        printf("%s\n", pData->data.selection.tableName);
                        printf("%s\n", pData->data.selection.field);
                    } else if (!strncmp(*cmd, GI, CMD_LIMIT)) {
                        printf("%s\n", pData->command);
                        printf("%s\n", pData->data.selection.tableName);
                        printf("%s\n", pData->data.selection.field);
                    } else {
                        printf("Outro\n");
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
