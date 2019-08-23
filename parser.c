#include "parser.h"

// Erro global de interpretação de comandos
int CMD_ERROR_CODE = NONE;

// Marca a posição corrente no buffer de comando
char *parsing;

// Buffers: Nome da tabela, nome do campo(chave) e valor
// Usados em comando com único parâmetro
TableName table_name;
Field field_name;
Value value;

// Buffer de tabela
TableWType table;
Row row;

// Identifica o comando
// command: String com a linha de comando em questão
// Lógica básica: Procura o comando e pula o ponteiro para após o mesmo, lê o nome da tabela, outro parâmetro até o ':' e outro até o ';' ou fim, repete
void parser(char * command) {
    table.cols = 0;

    // Começa o parsing
    if (parsing = findl(command, CT, 0)) {
        // Chama função para tratar dos espaços indesejados
        // fixingCommand(command);
        if(sscanf(parsing, "%s %[^:^;]%*c%[^;^\n]", table.name, table.types[table.cols], table.fields[table.cols]) == 3) {
            underlinaizer(table.fields[table.cols]);
            toUpperCase(table.name);
            toUpperCase(table.types[table.cols]);
            table.cols++;

            while (parsing = find(parsing, ";")) {
                if(sscanf(parsing, "%[^:^;]%*c%[^;^\n]", table.types[table.cols], table.fields[table.cols]) == 2) {
                    underlinaizer(table.fields[table.cols]);
                    toUpperCase(table.types[table.cols]);
                    table.cols++;
                } else {
                    CMD_ERROR_CODE = CT_WS_USC; return;
                }
            }

            createTable(table);
        } else {
            CMD_ERROR_CODE = CT_WS; return;
        }
    } else if (parsing = findl(command, RT, 0)) {
        if (sscanf(parsing, "%s", table.name) == 1) {
            toUpperCase(table.name);
            removeTable(table.name);
        } else {
            CMD_ERROR_CODE = RT_WS; return;
        }
    } else if (parsing = findl(command, AT, 0)) {
        if (sscanf(parsing, "%s", table.name) == 1) {
            toUpperCase(table.name);
            apTable(table.name);
        } else {
            CMD_ERROR_CODE = AT_WS; return;
        }
    } else if (parsing = findl(command, LT, 0)) {
        listTables();
    } else if (parsing = findl(command, IR, 0)) {
        if (sscanf(parsing, "%s %[^;^\n]", row.table_name, row.values[row.size]) == 2) {
            toUpperCase(row.table_name);

            row.size++;

            while (parsing = find(parsing, ";")) {
                if (sscanf(parsing, "%[^;^\n]", row.values[row.size]) == 1) {
                    row.size++;
                } else {
                    CMD_ERROR_CODE = IR_USC; return;
                }
            }

            includeReg(row);
        } else {
            CMD_ERROR_CODE = IR_WS; return;
        }
    } else if (parsing = findl(command, BR, 0)) {
        char *temp = parsing;
        char *aux_type = 0;

        if (temp = findl(stripStart(parsing), U, 0)) {
            aux_type = U;
        } else if (temp = findl(stripStart(parsing), N, 0)) {
            aux_type = N;
        } else {
            CMD_ERROR_CODE = BR_MP; return;
        }

        if (sscanf(temp, "%s %[^:]%*c%s", table_name, field_name, value) == 3) {
            toUpperCase(table_name);
            
            if (aux_type == U) {
                busRegU(table_name, field_name, value);
            } else if (aux_type == N) {
                busRegN(table_name, field_name, value);
            } else {
                CMD_ERROR_CODE = IN_ERROR;  return;
            }
        } else {
            CMD_ERROR_CODE = BR_WS; return;
        }
    } else if (parsing = findl(command, AR, 0)) {
        if (sscanf(parsing, "%s", table_name) == 1) {
            toUpperCase(table_name);
            apReg(table_name);
        } else {
            CMD_ERROR_CODE = AR_WS; return;
        }
    } else if (parsing = findl(command, RR, 0)) {
        if (sscanf(parsing, "%s", table_name) == 1) {
            toUpperCase(table_name);

            removeReg(table_name);
        } else {
            CMD_ERROR_CODE = RR_WS; return;
        }
    } else if (parsing = findl(command, CI, 0)) {
        char *temp = parsing;
        char *aux_type = 0;

        if (temp = findl(stripStart(parsing), A, 0)) {
            aux_type = A;
        } else if (temp = findl(stripStart(parsing), H, 0)) {
            aux_type = H;
        } else {
            CMD_ERROR_CODE = CI_MP; return;
        }

        if (sscanf(temp, "%s %s", table_name, field_name) == 2) {
            toUpperCase(table_name);
            
            if (aux_type == A) {
                createIndexA(table_name, field_name);
            } else if (aux_type == H) {
                createIndexH(table_name, field_name);
            } else {
                CMD_ERROR_CODE = IN_ERROR;  return;
            }
        } else {
            CMD_ERROR_CODE = CI_WS;  return;              
        }
    } else if (parsing = findl(command, RI, 0)) {
        if (sscanf(parsing, "%s %s", table_name, field_name) == 2) {
            toUpperCase(table_name);
            removeIndex(table_name, field_name);
        } else {
            CMD_ERROR_CODE = RI_WS; return;
        }
    } else if (parsing = findl(command, GI, 0)) {
        if (sscanf(parsing, "%s %s", table_name, field_name) == 2) {
            toUpperCase(table_name);
            genIndex(table_name, field_name);
        } else {
            CMD_ERROR_CODE = GI_WS; return;
        }
    } else if (parsing = findl(command, EB, 0)) {
        CMD_ERROR_CODE = EXIT; return;
    } else {
        CMD_ERROR_CODE = NO_CMD; return;
    }

    CMD_ERROR_CODE = NONE; return;
}