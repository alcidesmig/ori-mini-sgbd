#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defines.h"
#include "menu.h"
#include "parser.h"
#include "commands.h"
#include "tools.h"

char *line = NULL; // Buffer da linha de comando
ssize_t length = 0; // Tamanho da linha lida por getline

FILE *run(FILE *cmd_file, int term) {
    if (term)
        preline();
    while (getline(&line, &length, cmd_file) != -1) {
        parser(line);
        if (term)
            preline();
    }

    return cmd_file;
}

int main(int argc, char *argv[]) {
    int opt; // Auxiliar dos parâmetroschar
    int term = 1; // Flag de uso do terminal

    init();

    while ((opt = getopt(argc, argv, "hf:")) != -1) {
        switch(opt) {
            case 'h':
                menu();
            case 'f':
                term = 0;
                fclose(run(safe_fopen(optarg, "rb+"), term));
            case '?':
                raiseError(UNSUPORTED_PARAM);
        }
    }

    if (term) {
        run(stdin, term);
    }

    if (search_dict) {
        freeDnodeTree(search_dict);
    }
    
    return 0;
}
