#ifndef TOOLS_H
#define TOOLS_H

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

// Printa mensagens de acordo com o erro
void errorHandler(int error);

#endif /* TOOLS_H */