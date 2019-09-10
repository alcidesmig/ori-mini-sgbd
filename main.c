#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defines.h"
#include "menu.h"
#include "parser.h"
#include "commands.h"
#include "tools.h"
#include "getline_custom.h"

char *line = NULL; // Buffer da linha de comando
ssize_t length = 0; // Tamanho da linha lida por getline

int main(int argc, char *argv[]) {
    int opt; // Auxiliar dos parâmetroschar
    int term = 1; // Flag de uso do terminal

    // Prepara variáveis e arquivos para o programa
    init();

    // Loop de parâmetros da linha de comando
    while ((opt = getopt(argc, argv, "hf:")) != -1) {
        switch(opt) {
            // Mostra o help
            case 'h':
                menu();
                break;
            // Executa um arquivo
            case 'f':
                // Modifica a flag de uso do terminal
                term = 0;

                FILE *cmd_file = safe_fopen(optarg, "rb+");

                // Loop de leitura
                while (getline(&line, &length, cmd_file) != -1) {
                    parser(line);
                }

                fclose(cmd_file);
            case '?':
                raiseError(UNSUPORTED_PARAM);
        }
    }

    // Caso nenhum arquivo foi executado
    if (term) {
        // Configura o terminal
        term_init();

        preline();

        // Loop de leitura
        while (getline_custom(&line, &length) != EOF) {
            parser(line);
            preline();
        }
        
        term_close();
    }

    // Libera a memória usada pela lista de resultados
    if (search_dict) {
        freeDnodeTree(search_dict);
    }

    return 0;
}
