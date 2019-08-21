#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "defines.h"
#include "tools.h"
#include "commands.h"

// Marca a posição corrente no buffer de comando
char *parsing;

// Buffers: Nome da tabela, nome do campo(chave) e valor
// Usados em comando com único parâmetro
char table_name[TBLNM_MAX];
char field_name[FIELD_MAX];
char value[VALUE_MAX];

// Buffers: Tipos, nomes do campo(chave) e valores
// Usados em comando com mutiplos parâmetros
char type_name_arr[N_COLUMNS][TYPE_MAX];
char field_name_arr[N_COLUMNS][FIELD_MAX];
char value_arr[N_COLUMNS][VALUE_MAX];

// Index dos vetores a cima
int index_arr = 0;

// Identifica o comando
// command: String com a linha de comando em questão
// Lógica básica: Procura o comando e pula o ponteiro para após o mesmo, lê o nome da tabela, outro parâmetro até o ':' e outro até o ';' ou fim, repete
int parser(char *command) {
    index_arr = 0;

    if (parsing = findl(command, CT, 0)) {
        if(sscanf(parsing, "%s %[^:]%*c%[^;]", table_name, type_name_arr[index_arr], field_name_arr[index_arr]) == 3) {
            toUpperCase(table_name);
            toUpperCase(type_name_arr[index_arr]);
            index_arr++;

            while (parsing = find(parsing, ";")) {
                if(sscanf(parsing, "%[^:]%*c%[^;]", type_name_arr[index_arr], field_name_arr[index_arr]) == 2) {
                    toUpperCase(type_name_arr[index_arr]);
                    index_arr++;
                } else {
                    return CT_WS_USC;
                }
            }

            createTable(table_name, type_name_arr, field_name_arr, index_arr);
        } else {
            return CT_WS;
        }
    } else if (parsing = findl(command, RT, 0)) {
        if (sscanf(parsing, "%s", table_name) == 1) {
            toUpperCase(table_name);
            removeTable(table_name);
        } else {
            return RT_WS;
        }
    } else if (parsing = findl(command, AT, 0)) {
        if (sscanf(parsing, "%s", table_name) == 1) {
            toUpperCase(table_name);
            apTable(table_name);
        } else {
            return AT_WS;
        }
    } else if (parsing = findl(command, LT, 0)) {
        listTables();
    } else if (parsing = findl(command, IR, 0)) {
        if (sscanf(parsing, "%s %[^;]", table_name, value_arr[index_arr]) == 2) {
            toUpperCase(table_name);

            index_arr++;

            while (parsing = find(parsing, ";")) {
                if (sscanf(parsing, "%[^;]", value_arr[index_arr]) == 1) {
                    index_arr++;
                } else {
                    return IR_USC;
                }
            }

            includeReg(table_name, value_arr, index_arr);
        } else {
            return IR_WS;
        }
    } else if (parsing = findl(command, BR, 0)) {
        char *temp = parsing;
        char *aux_type = 0;

        if (temp = findl(parsing, U, 1)) {
            aux_type = U;
        } else if (temp = findl(parsing, N, 1)) {
            aux_type = N;
        } else {
            return BR_MP;
        }

        if (sscanf(temp, "%s %[^:]%*c%s", table_name, field_name, value) == 3) {
            toUpperCase(table_name);
            
            if (aux_type == U) {
                busRegU(table_name, field_name, value);
            } else if (aux_type == N) {
                busRegN(table_name, field_name, value);
            } else {
                return IN_ERROR; 
            }
        } else {
            return BR_WS;
        }
    } else if (parsing = findl(command, AR, 0)) {
        if (sscanf(parsing, "%s", table_name) == 1) {
            toUpperCase(table_name);
            apReg(table_name);
        } else {
            return AR_WS;
        }
    } else if (parsing = findl(command, RR, 0)) {
        if (sscanf(parsing, "%s", table_name) == 1) {
            toUpperCase(table_name);

            removeReg(table_name);
        } else {
            return RR_WS;
        }
    } else if (parsing = findl(command, CI, 0)) {
        char *temp = parsing;
        char *aux_type = 0;

        if (temp = findl(parsing, A, 1)) {
            aux_type = A;
        } else if (temp = findl(parsing, H, 1)) {
            aux_type = H;
        } else {
            return CI_MP;
        }

        if (sscanf(temp, "%s %s", table_name, field_name) == 2) {
            toUpperCase(table_name);
            
            if (aux_type == A) {
                createIndexA(table_name, field_name);
            } else if (aux_type == H) {
                createIndexH(table_name, field_name);
            } else {
                return IN_ERROR; 
            }
        } else {
            return CI_WS;               
        }
    } else if (parsing = findl(command, RI, 0)) {
        if (sscanf(parsing, "%s %s", table_name, field_name) == 2) {
            toUpperCase(table_name);
            removeIndex(table_name, field_name);
        } else {
            return RI_WS;
        }
    } else if (parsing = findl(command, GI, 0)) {
        if (sscanf(parsing, "%s %s", table_name, field_name) == 2) {
            toUpperCase(table_name);
            genIndex(table_name, field_name);
        } else {
            return GI_WS;
        }
    } else if (parsing = findl(command, EB, 0)) {
        return EXIT;
    } else {
        return NO_CMD;
    }

    return NONE;
}