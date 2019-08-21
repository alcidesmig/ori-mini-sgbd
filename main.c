#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "menu.h"
#include "parser.h"
#include "commands.h"
#include "tools.h"

// Erro global
int CMD_ERROR_CODE = 0;

// Buffer da linha de comando
char comando[CMD_MAX];

// Arquivo de índice
FILE *index_file = NULL;

// Executa comandos a partir da linha de comando
void commandLine() {
    CMD_ERROR_CODE = 0;

    while (fgets(comando, CMD_MAX, stdin) && !CMD_ERROR_CODE) {
        CMD_ERROR_CODE = parser(stripStart(comando));
    }
}

// Executa comandos a partir de um arquivo
int fromFile() {
    CMD_ERROR_CODE = 0;

    while (fgets(comando, CMD_MAX, index_file) && !CMD_ERROR_CODE) {
        CMD_ERROR_CODE = parser(stripStart(comando));
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        menu();
        commandLine();
    } else {
        char *file = argv[1];
        index_file = fopen(file, "r");

        printf("Executando %s.\n", file);

        if (index_file != NULL) {
            fromFile(index_file);
        } else {
            printf("Erro ao abrir o arquivo.\n");
        }
    }

    errorHandler(CMD_ERROR_CODE);

    return 0;
}