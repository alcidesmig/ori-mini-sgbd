#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "defines.h"

// Remove a tabeta
void removeTable(char *table_name) {
    printf("Removendo a tabela %s.\n", table_name);
    printf("TODO\n");
}

// Apresenta a tabeta
void apTable(char *table_name) {
    printf("Mostrando a tabela %s.\n", table_name);
    printf("TODO\n");
}

// Lista as tabetas
void listTables() {
    printf("Listando tabelas.\n");
    printf("TODO\n");
}

// Apresenta a pesquisa da tabela
void apReg(char *table_name) {
    printf("Mostrando a pesquisa de %s.\n", table_name);
    printf("TODO\n");
}

// Busca registro em tabela, único
void busRegU(char *table_name, char *field_name, char *value) {
    printf("Buscando %s igual à %s em %s.\n", table_name, field_name, value);
    printf("TODO\n");
}

// Busca registro em tabela, único
void busRegN(char *table_name, char *field_name, char *value) {
    printf("Buscando todos os %s igual à %s em %s.\n", table_name, field_name, value);
    printf("TODO\n");
}

// Cria um tabela
void createTable(char *table_name, char type_name_arr[N_COLUMNS][TYPE_MAX], char field_name_arr[N_COLUMNS][FIELD_MAX], int iTypeField) {
    printf("Criando tabela %s.\n", table_name);

    for (int i = 0; i < iTypeField; i++) {
        printf("%s %s\n", type_name_arr[i], field_name_arr[i]);
    }

    printf("TODO\n");
}

// Inclui um registro em uma tabela
void includeReg(char *table_name, char value_arr[N_COLUMNS][VALUE_MAX], int iValue) {
    printf("Novo registro na tabela %s.\n", table_name);

    for (int i = 0; i < iValue; i++) {
        printf("%s\n", value_arr[i]);
    }

    printf("TODO\n");
}

// Remove registros pesquisados
void removeReg(char *table_name) {
    printf("Registros removidos de %s.\n", table_name);
    
    printf("TODO\n");
}

// Cria índice para uma tabela, árvore
void createIndexA(char *table_name, char *field_name) {
    printf("Criado um índice com árvore para %s usando a coluna %s.\n", table_name, field_name);
    
    printf("TODO\n");
}

// Cria índice para uma tabela, heap
void createIndexH(char *table_name, char *field_name) {
    printf("Criado um índice com heap para %s usando a coluna %s.\n", table_name, field_name);
    
    printf("TODO\n");
}

// Remove índice de uma tabela
void removeIndex(char *table_name, char *field_name) {
    printf("Removendo o índice de %s referente a chave %s.\n", table_name, field_name);
    
    printf("TODO\n");
}

// Gera índice para uma tabela
void genIndex(char *table_name, char *field_name) {
    printf("Gerando um índice com %s para %s usando a coluna %s.\n", "-recuperar-", table_name, field_name);
    
    printf("TODO\n");
}