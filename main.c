#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "menu.h"
#include "parser.h"
#include "commands.h"
#include "tools.h"

// Buffer da linha de comando
char *comando;
ssize_t tam_comando;

// Arquivo de comandos
FILE *cmd_file = NULL;

int PRE_ERROR_CODE = NONE; // Erro global

// Executa comandos a partir da linha de comando
void commandLine() {
    CMD_ERROR_CODE = 0;

    while (!CMD_ERROR_CODE && prepline() && getline(&comando, &tam_comando, stdin)) {
        parser(stripStart(comando));
        errorHandlerExec(EXEC_ERROR_CODE);
    }
}

// Executa comandos a partir de um arquivo
int fromFile() {
    CMD_ERROR_CODE = 0;

    while (!CMD_ERROR_CODE && getline(&comando, &tam_comando, cmd_file)) {
        parser(stripStart(comando));
        ExecErrorHandler(EXEC_ERROR_CODE);
    }
}


int main(int argc, char *argv[]) {
    if (!init()) {
        printf("Erro.\n");
        return 0;
    }

    int needFile = 0;

    // Arquivo de comandos
    char *file = NULL;

    // Interpretação dos parâmetros
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], HELP) || strcmp(argv[i], HELPX)) {
            if (!needFile) {
                menu();
            } else {
                PRE_ERROR_CODE = PRE_MISS_FL;
            }
        } else if (strcmp(argv[i], FILE) || strcmp(argv[i], FILEX)) {
            if (!needFile) {
                if (!file) {
                    needFile = 1;
                } else {
                    PRE_ERROR_CODE = PRE_MANY_FL;
                }
            } else {
                PRE_ERROR_CODE = PRE_MISS_FL;
            }
        } else {
            if (!needFile) {
                PRE_ERROR_CODE = PRE_WRG_PRM;
            } else {
                file = argv[i];
                needFile = 0;
            }
        }

        if (!preErrorHandler()) {
            return 0;
        }
    }

    if (file) {
        cmd_file = fopen(file, "r");

        printf("Executando %s.\n", file);

        if (cmd_file) {
            fromFile(cmd_file);
        } else {
            printf("Erro ao abrir o arquivo.\n");
        }
    } else {
        commandLine();
    }

    errorHandler(CMD_ERROR_CODE);

    return 0;
}
