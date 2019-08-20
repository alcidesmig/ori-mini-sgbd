#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "defines.h"
#include "tools.h"
#include "commands.h"

// Reconhece o comando
int parser(char *command) {
    char *parsing;

    char table_name[TBLNM_MAX];
    char field_name[FIELD_MAX];
    char value[VALUE_MAX];

    char type_name_arr[N_COLUMNS][TYPE_MAX];
    char field_name_arr[N_COLUMNS][FIELD_MAX];
    int iTypeField = 0;

    char value_arr[N_COLUMNS][VALUE_MAX];
    int iValue = 0;

    if (parsing = findl(command, CT, 0)) {
        if(sscanf(parsing, "%s %[^:]%*c%[^;]", table_name, type_name_arr[iTypeField], field_name_arr[iTypeField]) != 3) {
            printf("Erro ao criar tabela.\n");
            return 0;
        }

        toUpperCase(table_name);
        toUpperCase(type_name_arr[iTypeField]);

        iTypeField++;

        while (parsing = find(parsing, ";")) {
            if(sscanf(parsing, "%[^:]%*c%[^;]", type_name_arr[iTypeField], field_name_arr[iTypeField]) != 2) {
                printf("Erro ao criar tabela.\n");

                return 0;
            }
    
            toUpperCase(type_name_arr[iTypeField]);

            iTypeField++;
        }

        createTable(table_name, type_name_arr, field_name_arr, iTypeField);
    } else if (parsing = findl(command, RT, 0)) {
        if (sscanf(parsing, "%s", table_name) != 1) {
            printf("Erro ao remover tabela.\n");
            return 0;
        }

        toUpperCase(table_name);

        removeTable(table_name);
    } else if (parsing = findl(command, AT, 0)) {
        if (sscanf(parsing, "%s", table_name) != 1) {
            printf("Erro ao apresentar tabela.\n");
            return 0;
        }

        toUpperCase(table_name);

        apTable(table_name);
    } else if (parsing = findl(command, LT, 0)) {
        listTables();
    } else if (parsing = findl(command, IR, 0)) {
        if (sscanf(parsing, "%s %[^;]", table_name, value_arr[iValue]) != 2) {
            printf("Erro ao incluir registro.\n");
    
            return 0;
        }

        toUpperCase(table_name);

        iValue++;

        while (parsing = find(parsing, ";")) {
            if (sscanf(parsing, "%[^;]", value_arr[iValue]) != 1) {
                printf("Erro ao incluir registro.\n");
                
                return 0;
            }

            iValue++;
        }

        includeReg(table_name, value_arr, iValue);
    } else if (parsing = findl(command, BR, 0)) {
        char *temp = parsing;
        if (temp = findl(parsing, U, 1)) {
            if (sscanf(temp, "%s %[^:]%*c%s", table_name, field_name, value) != 3) {
                printf("Erro ao buscar registro.\n");
                return 0;
            }

            toUpperCase(table_name);

            busRegU(table_name, field_name, value);
        } else if (temp = findl(parsing, N, 1)) {
            if (sscanf(temp, "%s %[^:]%*c%s", table_name, field_name, value) != 3) {
                printf("Erro ao buscar registro.\n");
                return 0;
            }

            toUpperCase(table_name);

            busRegN(table_name, field_name, value);
        } else {
            printf("Parâmetro não reconhecido.\n");
            return 0;
        }
    } else if (parsing = findl(command, AR, 0)) {
        if (sscanf(parsing, "%s", table_name) != 1) {
            printf("Erro ao mostrar busca.\n");
            return 0;
        }

        toUpperCase(table_name);

        apReg(table_name);
    } else if (parsing = findl(command, RR, 0)) {
        if (sscanf(parsing, "%s", table_name) != 1) {
            printf("Erro ao deletar registros.\n");
            return 0;
        }

        toUpperCase(table_name);

        removeReg(table_name);
    } else if (parsing = findl(command, CI, 0)) {
        char *temp = parsing;
        if (temp = findl(parsing, A, 1)) {
            if (sscanf(temp, "%s %s", table_name, field_name) != 2) {
                printf("Erro ao criar índice.\n");
                return 0;
            }

            toUpperCase(table_name);

            createIndexA(table_name, field_name);
        } else if (temp = findl(parsing, H, 1)) {
            if (sscanf(temp, "%s %s", table_name, field_name) != 2) {
                printf("Erro ao criar índice.\n");
                return 0;
            }

            toUpperCase(table_name);

            createIndexH(table_name, field_name);
        } else {
            printf("Parâmetro não reconhecido.\n");
            return 0;
        }
    } else if (parsing = findl(command, RI, 0)) {
        if (sscanf(parsing, "%s %s", table_name, field_name) != 2) {
            printf("Erro ao remover índice.\n");
            return 0;
        }

        toUpperCase(table_name);

        removeIndex(table_name, field_name);
    } else if (parsing = findl(command, GI, 0)) {
        if (sscanf(parsing, "%s %s", table_name, field_name) != 2) {
            printf("Erro ao gerar índice.\n");
            return 0;
        }

        toUpperCase(table_name);
        
        genIndex(table_name, field_name);
    } else if (parsing = findl(command, EB, 0)) {
        return 1;
    } else {
        printf("Nenhum comando encontrado!\n");
    }
}