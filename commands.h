#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "tools.h"
#include "error.h"
#include "data_struct.h"

extern FILE *tables_index; // Variável global utilizada para manipular arquivos

extern int qt_tables; // Quantidade de tabelas

extern Result *result_list; // Lista de resultados

extern Dnode *search_dict; // Árvore com os resultados das pesquisas

// Cria tabela
// table: Struct com as informações para crira uma tabela
void createTable(TableWType *table);

// Remove tabela
// table_name: Nome da tabela
void removeTable(TableName table_name);

// Apresenta tabela
// table_name: Nome da tabela
void apTable(TableName table_name);

// Lista tabela
void listTables();

// Inclui registro na tabela
// row: Struct com os valores de um registro
void includeReg(Row *row);

// Busca registros na tabela
// table_name: Nome da tabela
// field_name: Nome do campo(chave)
// value: Nome do valor
void busReg(TableName table_name, Field field_name, Value value, int matchings);

// Apresenta registros pesquisados da tabela
// table_name: Nome da tabela
void apReg(TableName table_name);

// Remove registro da tabela
// table_name: Nome da tabela
void removeReg(TableName table_name);

// Cria index da tabela, árvore
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void createIndexA(TableName table_name, Field field_name);

// Cria index da tabela, heap
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void createIndexH(TableName table_name, Field field_name);

// Remove index da tabela
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void removeIndex(TableName table_name, Field field_name);

// Gera index
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void genIndex(TableName table_name, Field field_name);

#endif /* COMMANDS_H */