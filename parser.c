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

    char cmd[CMD_MAX];
    char parameter[PARAMETER_MAX];
    int scaned = 0;

    sscanf(line, CMD_SCANF, cmd, line);
    toUpperCase(cmd);

    if (!strcmp(cmd, CT)) {
        if (sscanf(line, TBL_NAME_SCANF, table.name, line) == 2) {
            toUpperCase(table.name);
            scaned = sscanf(line, TYPE_FIELD_SCANF, table.types[table.cols], table.fields[table.cols], line);
            toUpperCase(table.types[table.cols]);
            if (glueChars(table.types[table.cols], ' ')) printf("Espaços foram eliminados do tipo: %s\n", table.types[table.cols]);
            if (replaceSpace(table.fields[table.cols], '_')) printf("Espaços foram eliminados do campo: %s\n", table.fields[table.cols]);
            table.cols++;
            
            if (scaned == 2 || scaned == 3) {
                while (scaned == 3) {
                    scaned = sscanf(line, TYPE_FIELD_SCANF, table.types[table.cols], table.fields[table.cols], line);
                    toUpperCase(table.types[table.cols]);
                    if (glueChars(table.types[table.cols], ' ')) printf("Espaços foram eliminados do tipo: %s\n", table.types[table.cols]);
                    if (replaceSpace(table.fields[table.cols], '_')) printf("Espaços foram eliminados do campo: %s\n", table.fields[table.cols]);
                    table.cols++;
                }
                createTable(&table); return;
            }
        }
        raiseError(WRONG_SINTAX);
    } else if (!strcmp(cmd, RT)) {
        if (sscanf(line, TBL_NAME_SCANF, table.name, line) == 1) {
            toUpperCase(table.name);
            removeTable(table.name); return;
        }
        raiseError(WRONG_SINTAX);
    } else if (!strcmp(cmd, AT)) {
        if (sscanf(line, TBL_NAME_SCANF, table.name, line) == 1) {
            toUpperCase(table.name);
            apTable(table.name); return;
        }
        raiseError(WRONG_SINTAX);
    } else if (!strcmp(cmd, LT)) {
        listTables(); return;
    } else if (!strcmp(cmd, IR)) {
        if (sscanf(line, TBL_NAME_SCANF, row.table_name, line) == 2) {
            toUpperCase(row.table_name);
            scaned = sscanf(line, VALUE_SCANF, row.values[row.size], line);
            row.size++;
            
            if (scaned == 1 || scaned == 2) {
                table.cols++;
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
                    if (replaceSpace(field_name, '_')) printf("Espaços foram eliminados do campo: %s\n", field_name);

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
                    if (replaceSpace(field_name, '_')) printf("Espaços foram eliminados do campo: %s\n", field_name);

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
                if (replaceSpace(field_name, '_')) printf("Espaços foram eliminados do campo: %s\n", field_name);
                removeIndex(table_name, field_name); return;
            }
        }
        raiseError(WRONG_SINTAX);
    } else if (!strcmp(cmd, GI)) {
        if (sscanf(line, TBL_NAME_SCANF, table_name, line) == 2) {
            if (sscanf(line, FIELD_NAME_SCANF, field_name) == 1) {
                toUpperCase(table_name);
                if (replaceSpace(field_name, '_')) printf("Espaços foram eliminados do campo: %s\n", field_name);
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