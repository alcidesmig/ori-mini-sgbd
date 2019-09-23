#include "tools.h"

// Função auxilidar para auto-compleate dos comandos (uso interno da linenoise)
// prefix: string a ser completada
// lc: vetor de possibilidades
void completionHook (char const* prefix, linenoiseCompletions* lc) {
    int i = 0;

    while (i < NUM_COMMANDS) {
        char *prefixU = strdup(prefix);
        toUpperCase(prefixU);
        if (!strncmp(prefixU, commands[i], strlen(prefix))) {
            linenoiseAddCompletion(lc, commands[i]);
        }
        i++;
        free(prefixU);
    }
}

// Verifica um tipo e o converte para representação interna
// type: tipo escolhido pelo usuário
// return: char respectivo ao tipo
char validateType(char *type) {
    if (!strncmp(type, INT, TYPE_LIMIT)) {
        return 'i';
    } else if (!strncmp(type, STR, TYPE_LIMIT)) {
        return 's';
    } else if (!strncmp(type, FLT,TYPE_LIMIT)) {
        return 'f';
    } else if (!strncmp(type, BIN, TYPE_LIMIT)) {
        return 'b';
    } else {
        return '\0';
    }
}