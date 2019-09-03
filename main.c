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

    init();

    while ((opt = getopt(argc, argv, "hf:")) != -1) {
        switch(opt) {
            case 'h':
                menu();
                break;
            case 'f':
                term = 0;
                FILE *cmd_file = safe_fopen(optarg, "rb+");

                while (getline(&line, &length, cmd_file) != -1) {
                    parser(line);
                }

                fclose(cmd_file);
            case '?':
                raiseError(UNSUPORTED_PARAM);
        }
    }

    if (term) {
        term_init();

        preline();
        while (getline_custom(&line, &length) != EOF) {
            parser(line);
            preline();
        }
        
        term_close();
    }

    if (search_dict) {
        freeDnodeTree(search_dict);
    }

    return 0;
}
