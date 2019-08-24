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

// Lê os nomes das tabelas
// tables_index: Arquivo de index das tabelas
// qt_tables: Quantidade de tabelas
TableName *read_tables_names(FILE *tables_index, int qt_tables);

// Aumenta a quantidade de tabelas em 1
// tables_index: Arquivo de index das tabelas
// qt_tables: Quantidade de tabelas
void increase_qt_tables(FILE *tables_index, int qt_tables);

// Diminui a quantidade de tabelas em 1
// tables_index: Arquivo de index das tabelas
// qt_tables: Quantidade de tabelas
void reduce_qt_tables(FILE *tables_index, int qt_tables);

// Escreve os metadados de uma tabela no seu arquivo
// tables_index: Arquivo de index das tabelas
// table: tabela a ser gravada
void write_table_metadata(FILE *tables_index, TableWRep *table);

// Lê os metadados de uma tabela, se ela existir
// tableName: Nome da tabela a ser lida
TableWRep *read_table_metadata(TableName tableName);

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