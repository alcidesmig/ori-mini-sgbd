#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "menu.h"
#include "parser.h"
#include "commands.h"
#include "aux.h"

void commandLine() {
    char comando[CMD_MAX];

    fgets(comando, CMD_MAX, stdin); // Lê o comando
    toUpperCase(comando); // Não diferenciação de letras maiúsculas e minúsculas

    // Identifica o comando
    if (parser(comando)) {
        return;
    }

    commandLine();
}

int fromFile(char *file) {
    printf("Executando %s\n", file);

    FILE *fp = fopen(file, "r");

    if (fp == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return 1;
    }

    char comando[CMD_MAX];

    // Lê os comandos
    while (fgets(comando, CMD_MAX, fp)) {
        toUpperCase(comando); //Não diferenciação de letras maiúsculas e minúsculas
        
        parser(comando); // Identifica o comando
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        menu();
        commandLine();

        printf("Saindo.\n");
    } else {
        fromFile(argv[1]);

        printf("Saindo.\n");
    }

    return 0;
}