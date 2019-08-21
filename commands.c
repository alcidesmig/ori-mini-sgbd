#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "defines.h"

// Remove tabela
// table_name: Nome da tabela
void removeTable(char *table_name) {
    printf("Removendo a tabela %s.\n", table_name);
    printf("TODO\n");
}

// Apresenta tabela tabela
// table_name: Nome da tabela
void apTable(char *table_name) {
    printf("Mostrando a tabela %s.\n", table_name);
    printf("TODO\n");
}

// Lista tabela
void listTables() {
    printf("Listando tabelas.\n");
    printf("TODO\n");
}

// Apresenta registros pesquisados da tabela
// table_name: Nome da tabela
void apReg(char *table_name) {
    printf("Mostrando a pesquisa de %s.\n", table_name);
    printf("TODO\n");
}

// Busca registros na tabela, único
// table_name: Nome da tabela
// field_name: Nome do campo(chave)
// value: Nome do valor
void busRegU(char *table_name, char *field_name, char *value) {
    printf("Buscando %s igual à %s em %s.\n", table_name, field_name, value);
    printf("TODO\n");
}

// Busca registros na tabela, todos
// table_name: Nome da tabela
// field_name: Nome do campo(chave)
// value: Nome do valor
void busRegN(char *table_name, char *field_name, char *value) {
    printf("Buscando todos os %s igual à %s em %s.\n", table_name, field_name, value);
    printf("TODO\n");
}

// Cria tabela
// table_name: Nome da tabela
// type_name_arr: Vetor de tipos
// field_name_arr: Vetor de campos(chave)
// size_arr: Tamanho dos vetores
void createTable(char *table_name, char type_name_arr[N_COLUMNS][TYPE_MAX], char field_name_arr[N_COLUMNS][FIELD_MAX], int size_arr) {
    printf("Criando tabela %s.\n", table_name);

    for (int i = 0; i < size_arr; i++) {
        printf("%s %s\n", type_name_arr[i], field_name_arr[i]);
    }

    printf("TODO\n");
}

// Inclui registro na tabela
// table_name: Nome da tabela
// value_arr: Vetor de valores
// size_arr: Tamanho do vetores
void includeReg(char *table_name, char value_arr[N_COLUMNS][VALUE_MAX], int size_arr) {
    printf("Novo registro na tabela %s.\n", table_name);

    for (int i = 0; i < iValue; i++) {
        printf("%s\n", value_arr[i]);
    }

    printf("TODO\n");
}

// Remove registro da tabela
// table_name: Nome da tabela
void removeReg(char *table_name) {
    printf("Registros removidos de %s.\n", table_name);
    printf("TODO\n");
}

// Cria index da tabela, árvore
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void createIndexA(char *table_name, char *field_name) {
    printf("Criado um índice com árvore para %s usando a coluna %s.\n", table_name, field_name);
    printf("TODO\n");
}

// Cria index da tabela, heap
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void createIndexH(char *table_name, char *field_name) {
    printf("Criado um índice com heap para %s usando a coluna %s.\n", table_name, field_name);
    printf("TODO\n");
}

// Remove index da tabela
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void removeIndex(char *table_name, char *field_name) {
    printf("Removendo o índice de %s referente a chave %s.\n", table_name, field_name);
    printf("TODO\n");
}

// Gera index
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void genIndex(char *table_name, char *field_name) {
    printf("Gerando um índice com %s para %s usando a coluna %s.\n", "-recuperar-", table_name, field_name);
    printf("TODO\n");
}