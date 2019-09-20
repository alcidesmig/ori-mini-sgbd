#ifndef TOOLS_H
#define TOOLS_H

#include <string.h>
#include "linenoise/linenoise.h"
#include "commands.h"

// Função auxilidar para auto-compleate dos comandos (uso interno da linenoise)
// prefix: string a ser completada
// lc: vetor de possibilidades
void completionHook (char const* prefix, linenoiseCompletions* lc);

// Verifica um tipo e o converte para representação interna
// type: tipo escolhido pelo usuário
// return: char respectivo ao tipo
char validateType(char *type);

#endif /* TOOLS_H */