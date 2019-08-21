#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "menu.h"
#include "parser.h"
#include "commands.h"
#include "tools.h"

// Buffer da linha de comando
char comando[CMD_MAX];

// Arquivo de comandos
FILE *cmd_file = NULL;

// Executa comandos a partir da linha de comando
void commandLine() {
    CMD_ERROR_CODE = 0;

    while (!CMD_ERROR_CODE && prepline() && fgets(comando, CMD_MAX, stdin)) {
        parser(stripStart(comando));
    }
}

// Executa comandos a partir de um arquivo
int fromFile() {
    CMD_ERROR_CODE = 0;

    while (!CMD_ERROR_CODE && fgets(comando, CMD_MAX, cmd_file)) {
        parser(stripStart(comando));
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        menu();
        commandLine();
    } else {
        char *file = argv[1];
        cmd_file = fopen(file, "r");

        printf("Executando %s.\n", file);

        if (cmd_file != NULL) {
            fromFile(cmd_file);
        } else {
            printf("Erro ao abrir o arquivo.\n");
        }
    }

    errorHandler(CMD_ERROR_CODE);

    return 0;
}