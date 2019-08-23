#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "commands.h"

// Cria o arquivo de index
int init();

// Verifica a existência de uma tabela com o nome especificado
// name: Nome da tabela
int tableNameExists(TableName *names, char *name);

// Converte uma TableWType para uma TableWRep
int convertToRep(TableWRep *tableR, TableWType *tableT);

// Converte uma TableWRep para uma TableWType
void convertToType(TableWType *tableT,TableWRep *tableR);

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

// Conta a quantidade de espaços em uma string
int countSpaces(char * str);

// Retira espaços indesejados no meio dos tipos e substitui os espaços dos atributos por '_'
int fixingCommandCT(char * command);

// Printa mensagens de acordo com o erro
int errorHandler(Error error);

int ExecErrorHandler(Error error);

int PreErrorHandler(Error error);

int prepline();

#endif /* TOOLS_H */