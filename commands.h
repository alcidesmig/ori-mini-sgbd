#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

// Erro global de execução de comandos
extern int EXEC_ERROR_CODE;

// Cria tabela
// table_name: Nome da tabela
// type_name_arr: Vetor de tipos
// field_name_arr: Vetor de campos(chave)
// size_arr: Tamanho dos vetores
void createTable(char *table_name, TypeArr type_name_arr, FieldArr field_name_arr, int size_arr);

// Remove tabela
// table_name: Nome da tabela
void removeTable(char *table_name);

// Apresenta tabela tabela
// table_name: Nome da tabela
void apTable(char *table_name);

// Lista tabela
void listTables();

// Inclui registro na tabela
// table_name: Nome da tabela
// value_arr: Vetor de valores
// size_arr: Tamanho do vetores
void includeReg(char *table_name, ValueArr value_arr, int size_arr);

// Busca registros na tabela, único
// table_name: Nome da tabela
// field_name: Nome do campo(chave)
// value: Nome do valor
void busRegU(char *table_name, char *field_name, char *value);

// Busca registros na tabela, todos
// table_name: Nome da tabela
// field_name: Nome do campo(chave)
// value: Nome do valor
void busRegN(char *table_name, char *field_name, char *value);

// Apresenta registros pesquisados da tabela
// table_name: Nome da tabela
void apReg(char *table_name);

// Remove registro da tabela
// table_name: Nome da tabela
void removeReg(char *table_name);

// Cria index da tabela, árvore
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void createIndexA(char *table_name, char *field_name);

// Cria index da tabela, heap
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void createIndexH(char *table_name, char *field_name);

// Remove index da tabela
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void removeIndex(char *table_name, char *field_name);

// Gera index
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void genIndex(char *table_name, char *field_name);

#endif /* COMMANDS_H */