#include "parser.h"

// Buffers: Nome da tabela, nome do campo(chave) e valor
// Usados em comando com único parâmetro
TableName table_name;
Field field_name;
Value value;

// Buffer de tabela
TableWType table;
Row row;

// Identifica o comando
// line: String com a linha de comando em questão
void parser(char * line) {
    table.cols = 0;
    row.size = 0;

    // Buffer do comando
    char cmd[CMD_MAX];

    // Buffer do parâmetro do comando
    char parameter[PARAMETER_MAX];

    // Auxiliar
    int scaned = 0;

    // Lê o comando
    sscanf(line, CMD_SCANF, cmd, line);
    toUpperCase(cmd);

    // Identifica o comando
    if (!strcmp(cmd, CT)) {
        // Lê o nome da tabela
        if (sscanf(line, TBL_NAME_SCANF, table.name, line) == 2) {
            toUpperCase(table.name);

            // Lê um par de tipo de campo e nome de campo
            scaned = sscanf(line, TYPE_FIELD_SCANF, table.types[table.cols], table.fields[table.cols], line);
            toUpperCase(table.types[table.cols]);

            // Verifica por espaços nessas variáveis
            if (glueChars(table.types[table.cols], ' ')) printf("%s %s\n", msg_space_elim, table.types[table.cols]);
            if (replaceSpace(table.fields[table.cols], '_')) printf("%s %s\n", msg_space_field, table.fields[table.cols]);
            table.cols++;

            // Continua a leitura          
            if (scaned == 2 || scaned == 3) {
                // Se ainda há o que ler
                while (scaned == 3) {
                    // Lê um par de tipo de campo e nome de campo
                    scaned = sscanf(line, TYPE_FIELD_SCANF, table.types[table.cols], table.fields[table.cols], line);
                    toUpperCase(table.types[table.cols]);

                    // Verifica por espaços nessas variáveis
                    if (glueChars(table.types[table.cols], ' ')) printf("%s %s\n", msg_space_elim, table.types[table.cols]);
                    if (replaceSpace(table.fields[table.cols], '_')) printf("%s %s\n", msg_space_field, table.fields[table.cols]);
                    table.cols++;
                }
                createTable(&table); return;
            }
        }
        raiseError(WRONG_SINTAX);
    } else if (!strcmp(cmd, RT)) {
        // Lê o nome da tabela
        if (sscanf(line, TBL_NAME_SCANF, table.name, line) == 1) {
            toUpperCase(table.name);
            removeTable(table.name); return;
        }
        raiseError(WRONG_SINTAX);
    } else if (!strcmp(cmd, AT)) {
        // Lê o nome da tabela
        if (sscanf(line, TBL_NAME_SCANF, table.name, line) == 1) {
            toUpperCase(table.name);
            apTable(table.name); return;
        }
        raiseError(WRONG_SINTAX);
    } else if (!strcmp(cmd, LT)) {
        listTables(); return;
    } else if (!strcmp(cmd, IR)) {
        // Lê o nome da tabela
        if (sscanf(line, TBL_NAME_SCANF, row.table_name, line) == 2) {
            toUpperCase(row.table_name);

            // Lê o primeiro valor
            scaned = sscanf(line, VALUE_SCANF, row.values[row.size], line);
            row.size++;
            
            // Continua a leitura
            if (scaned == 1 || scaned == 2) {
                // Se ainda há o que ler
                while (scaned == 2) {
                    scaned = sscanf(line, VALUE_SCANF, row.values[row.size], line);
                    row.size++;
                }
                includeReg(&row); return;
            }
        }
        raiseError(WRONG_SINTAX);
    } else if (!strcmp(cmd, BR)) {
        if (sscanf(line, PARAMETER_SCANF, parameter, line) == 2) {
            if (sscanf(line, TBL_NAME_SCANF, table_name, line) == 2) {
                if (sscanf(line, FIELD_NAME_VALUE_SCANF, field_name, value) == 2) {
                    toUpperCase(table_name);
                    toUpperCase(parameter);
                    if (replaceSpace(field_name, '_')) printf("%s %s\n", msg_space_field, field_name);

                    if (!strcmp(parameter, U)) {
                        busReg(table_name, field_name, value, 1); return;
                    } else if (!strcmp(parameter, N)) {
                        busReg(table_name, field_name, value, 2147483647); return;
                    } else {
                        raiseError(WRONG_PARAMETER);
                    }
                }
            }
        }
        raiseError(WRONG_SINTAX);
    } else if (!strcmp(cmd, AR)) {
        if (sscanf(line, TBL_NAME_SCANF, table_name, line) == 1) {
            toUpperCase(table_name);
            apReg(table_name); return;
        }
        raiseError(WRONG_SINTAX);
    } else if (!strcmp(cmd, RR)) {
        if (sscanf(line, TBL_NAME_SCANF, table_name, line) == 1) {
            toUpperCase(table_name);
            removeReg(table_name); return;
        }
        raiseError(WRONG_SINTAX);
    } else if (!strcmp(cmd, CI)) {
        if (sscanf(line, PARAMETER_SCANF, parameter, line) == 2) {
            if (sscanf(line, TBL_NAME_SCANF, table_name, line) == 2) {
                if (sscanf(line, FIELD_NAME_SCANF, field_name) == 1) {
                    toUpperCase(table_name);
                    toUpperCase(parameter);
                    if (replaceSpace(field_name, '_')) printf("%s %s\n", msg_space_field, field_name);

                    if (!strcmp(parameter, A)) {
                        createIndexA(table_name, field_name); return;
                    } else if (!strcmp(parameter, H)) {
                        createIndexH(table_name, field_name); return;
                    } else {
                        raiseError(WRONG_PARAMETER);
                    }
                }
            }
        }
        raiseError(WRONG_SINTAX);
    } else if (!strcmp(cmd, RI)) {
        if (sscanf(line, TBL_NAME_SCANF, table_name, line) == 2) {
            if (sscanf(line, FIELD_NAME_SCANF, field_name) == 1) {
                toUpperCase(table_name);
                if (replaceSpace(field_name, '_')) printf("%s %s\n", msg_space_field, field_name);
                removeIndex(table_name, field_name); return;
            }
        }
        raiseError(WRONG_SINTAX);
    } else if (!strcmp(cmd, GI)) {
        if (sscanf(line, TBL_NAME_SCANF, table_name, line) == 2) {
            if (sscanf(line, FIELD_NAME_SCANF, field_name) == 1) {
                toUpperCase(table_name);
                if (replaceSpace(field_name, '_')) printf("%s %s\n", msg_space_field, field_name);
                genIndex(table_name, field_name); return;
            }
        }
        raiseError(WRONG_SINTAX);
    } else if (!strcmp(cmd, EB)) {
        raiseError(EXIT);
    } else {
        raiseError(NO_COMMAND);
    }
}