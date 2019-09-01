#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "defines.h"
#include "error.h"

int init();

void *safe_malloc(size_t size);

FILE *safe_fopen(char *name, char *mode);

char *safe_strcat(char *dest, char *src);

// Lê a quantidade de tabelas
// tables_index: Arquivo de index das tabelas
// return: Quantidade de tabelas
int read_qt_tables(FILE *tables_index);

// Escreve a quantidade de tabelas
// tables_index: Arquivo de index das tabelas
// qt_tables: Quantidade de tabelas
void write_qt_tables(FILE *tables_index, int qt_tables);

// Lê os nomes das tabelas
// tables_index: Arquivo de index das tabelas
// qt_tables: Quantidade de tabelas
// return: Vetor de TableName
TableName *read_tables_names(FILE *tables_index, int qt_tables);

// Escreve os nomes das tabelas
// tables_index: Arquivo de index das tabelas
// names: Lista dos nomes
// qt_tables: Quantidade de tabelas
void write_tables_names(FILE *tables_index, TableName *names, int qt_tables);

// Lê os metadados de uma tabela, se ela existir
// tableName: Nome da tabela a ser lida
// return: Ponteiro para TableWRep lida
TableWRep *read_table_metadata(TableName tableName);

// Escreve os metadados de uma tabela no seu arquivo
// tables_index: Arquivo de index das tabelas
// table: tabela a ser gravada
// index: Posição da tabela
void write_table_metadata(FILE *tables_index, TableWRep *table, int index);

void safe_remove(char *path);

void toUpperCase(char *str);

// return: Flag indicando se algum espaço foi substituido
int replaceSpace(char *str, char c);

// return: Flag indicando se algum char foi substituido
int glueChars(char *str, char c);

// Verifica a existência de uma tabela com o nome especificado
// name: Nome da tabela
// return: 1 se a tabela existe
int tableNameExists(TableName *names, char *name, int qt_tables);

// Converte uma TableWType para uma TableWRep
// return: 1 em sucesso
int convertToRep(TableWRep *tableR, TableWType *tableT);

// Converte uma TableWRep para uma TableWType
// return: 1 em sucesso
int convertToType(TableWType *tableT, TableWRep *tableR);

// Junta várias strings em uma
// return: Nova string
char *glueString(int n_args, ...);

void preline();

#endif /* TOOLS_H */