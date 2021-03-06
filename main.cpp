#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linenoise/linenoise.h"
#include "menu.hpp"
#include "parser.hpp"
#include "defines.h"
#include "commands.hpp"

#define HISTORY_FILE "./.history"
#define PROMPT "\x1b[1;32mSGBD\x1b[0m> "

// #define DEBUG

void execute(ParsedData *pData);

int main(int argc, char *argv[]) {
    start();
    
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
        else if (!strcmp(*argv, "-f") || !strcmp(*argv, "--file")) {
            FILE * fp = fopen(*(argv + 1), "r");
            char * line = NULL;
            size_t len = 0;
            while(getline(&line, &len, fp) != -1){
                #ifdef DEBUG
                printf("%s\n", line);
                #endif
                ParsedData *pData = parser(line);

                if (pData) {
                    if (!strncmp(pData->command, EB, CMD_LIMIT)) {
                        printf("Saindo...\n");
                        break;
                    }
                    execute(pData);
                }
            }
            return 0;
        }
    }

    while (1) {
        char* line = linenoise(PROMPT);
        #ifdef DEBUG
        if(line != NULL) printf("%s\n", line); 
        #endif
        if (line) {
            if (*line != '\0') {
                linenoiseHistoryAdd(line);

                ParsedData *pData = parser(line);

                if (pData) {
                    if (!strncmp(pData->command, EB, CMD_LIMIT)) {
                        printf("Saindo...\n");
                        break;
                    }

                    execute(pData);
                }

                free(pData);
                linenoiseHistorySave(HISTORY_FILE);
            }
            
            free(line);
        } else {
            printf("Saindo...\n");
            break;
        }
    }

    linenoiseHistoryFree();

    end();
    
    return 0;
}

void execute(ParsedData *pData) {
    char *cmd = pData->command;

    // Identifica o comando
    if (!strncmp(cmd, CT, CMD_LIMIT)) {
        criarTabela(&(pData->data.table));
    } else if (!strncmp(cmd, RT, CMD_LIMIT)) {
        removerTabela(&(pData->data.table));
    } else if (!strncmp(cmd, AT, CMD_LIMIT)) {
        apresentarTabela(&(pData->data.table));
    } else if (!strncmp(cmd, LT, CMD_LIMIT)) {
        listarTabela();
    } else if (!strncmp(cmd, IR, CMD_LIMIT)) {
        incluirRegistro(&(pData->data.row));
    } else if (!strncmp(cmd, BR, CMD_LIMIT)) {
        buscarRegistros(&(pData->data.selection));
    } else if (!strncmp(cmd, AR, CMD_LIMIT)) {
        apresentarRegistros(&(pData->data.selection));
    } else if (!strncmp(cmd, RR, CMD_LIMIT)) {
        removerRegistros(&(pData->data.selection));
    } else if (!strncmp(cmd, CI, CMD_LIMIT)) {
        criarIndex(&(pData->data.selection));
    } else if (!strncmp(cmd, RI, CMD_LIMIT)) {
        removerIndex(pData->data.selection.tableName, pData->data.selection.field, 1, 0); // paramêtro 1 = printar log pro usuário, 2 = remover todos índices
    } else if (!strncmp(cmd, GI, CMD_LIMIT)) {
        gerarIndex(&(pData->data.selection), '*');
    }
}