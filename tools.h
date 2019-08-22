#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

// Separa uma string usando os separadores.
// str: String
// splitter: separador
// Retorna vetor de strings
char ** split(char * str, char splitter);

void toUpperCase(char * str);

// Acha uma substring em string, pode haver limite de caracteres de matching
// haystack: string
// needle: substring
// limit: limite
char *findl(char *haystack, const char *needle, int limit);

// Acha uma substring em string
// haystack: string
// needle: substring
char *find(char *haystack, const char *needle);

// Remove espaços no começo de um uma string
// Retorna ponteiro para a nova posição
char *stripStart(char *command);

// Retira espaços indesejados no meio dos tipos e substitui os espaços dos atributos por '_'
void fixingCommand(char * command);

// Printa mensagens de acordo com o erro
void errorHandler(int error);

int prepline();

#endif /* TOOLS_H */