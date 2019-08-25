#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int read_qt_tables(FILE *tables_index);

// Escreve a quantidade de tabelas
// tables_index: Arquivo de index das tabelas
// qt_tables: Quantidade de tabelas
void write_qt_tables(FILE *tables_index, int qt_tables);

// Lê os nomes das tabelas
// tables_index: Arquivo de index das tabelas
// qt_tables: Quantidade de tabelas
TableName *read_tables_names(FILE *tables_index, int qt_tables);

// Escreve os nomes das tabelas
// tables_index: Arquivo de index das tabelas
// names: Lista dos nomes
// qt_tables: Quantidade de tabelas
void write_tables_names(FILE *tables_index, TableName *names, int qt_tables);

// Lê os metadados de uma tabela, se ela existir
// tableName: Nome da tabela a ser lida
TableWRep *read_table_metadata(TableName tableName);

// Escreve os metadados de uma tabela no seu arquivo
// tables_index: Arquivo de index das tabelas
// table: tabela a ser gravada
void write_table_metadata(FILE *tables_index, TableWRep *table, int index);

void safe_remove(char *path);

void toUpperCase(char *str);

int replaceSpace(char *str, char c);

int glueChars(char *str, char c);

// Verifica a existência de uma tabela com o nome especificado
// name: Nome da tabela
int tableNameExists(TableName *names, char *name, int qt_tables);

// Converte uma TableWType para uma TableWRep
int convertToRep(TableWRep *tableR, TableWType *tableT);

// Converte uma TableWRep para uma TableWType
int convertToType(TableWType *tableT, TableWRep *tableR);

void preline();

#endif /* TOOLS_H */