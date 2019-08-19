#ifndef COMMANDS_H
#define COMMANDS_H

#include "defines.h"

// Remove a tabeta
void removeTable(char *table_name);

// Apresenta a tabeta
void apTable(char *table_name);

// Lista as tabetas
void listTable();

// Apresenta a pesquisa da tabela
void apReg(char *table_name);

// Busca registro em tabela, único
void busRegU(char *table_name, char *field_name, char *value);

// Busca registro em tabela, único
void busRegN(char *table_name, char *field_name, char *value);

// Cria um tabela
void createTable(char *table_name, char type_name_arr[N_COLUMNS][TYPE_MAX], char field_name_arr[N_COLUMNS][FIELD_MAX], int iTypeField);

// Inclui um registro em uma tabela
void includeReg(char *table_name, char value_arr[N_COLUMNS][VALUE_MAX], int iValue);

// Remove registros pesquisados
void removeReg(char *table_name);

// Cria índice para uma tabela, árvore
void createIndexA(char *table_name, char *field_name);

// Cria índice para uma tabela, heap
void createIndexH(char *table_name, char *field_name);

// Remove índice de uma tabela
void removeIndex(char *table_name, char *field_name);

// Gera índice para uma tabela
void genIndex(char *table_name, char *field_name);

#endif /* COMMANDS_H */