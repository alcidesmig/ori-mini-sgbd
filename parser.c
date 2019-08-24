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

    char cmd[3];
    char parameter[2];
    int scaned = 0;

    sscanf(line, "%s %[^\n]", cmd, line);
    toUpperCase(cmd);

    if (!strcmp(cmd, CT)) {
        fixingCommandCT(line);
        if (sscanf(line, "%s %[^\n]", table.name, line) == 2) {
            toUpperCase(table.name);
            scaned = sscanf(line, "%[^:]%*c%[^;]%*c%[^\n]", table.types[table.cols], table.fields[table.cols], line);
            toUpperCase(table.types[table.cols]);
            table.cols++;
            if (scaned == 2 || scaned == 3) {
                while (scaned == 3) {
                    scaned = sscanf(line, "%[^:]%*c%[^;]%*c%[^\n]", table.types[table.cols], table.fields[table.cols], line);
                    toUpperCase(table.types[table.cols]);
                    table.cols++;
                }
                createTable(&table); return;
            }
        }
        raiseError(CT_WRONG_SINTAX);
    } else if (!strcmp(cmd, RT)) {
        if (sscanf(line, "%s %[^\n]", table.name, line) == 1) {
            toUpperCase(table.name);
            removeTable(table.name); return;
        }
        raiseError(RT_WRONG_SINTAX);
    } else if (!strcmp(cmd, AT)) {
        if (sscanf(line, "%s %[^\n]", table.name, line) == 1) {
            toUpperCase(table.name);
            apTable(table.name); return;
        }
        raiseError(AT_WRONG_SINTAX);
    } else if (!strcmp(cmd, LT)) {
        listTables(); return;
    } else if (!strcmp(cmd, IR)) {
        if (sscanf(line, "%s %[^\n]", row.table_name, line) == 2) {
            toUpperCase(row.table_name);
            scaned = sscanf(line, "%[^;]%*c%[^\n]", row.values[row.size], line);
            row.size++;
            if (scaned == 1 || scaned == 2) {
                table.cols++;
                while (scaned == 2) {
                    scaned = sscanf(line, "%[^;]%*c%[^\n]", row.values[row.size], line);
                    row.size++;
                }
                includeReg(&row); return;
            }
        }
        raiseError(IR_WRONG_SINTAX);
    } else if (!strcmp(cmd, BR)) {
        if (sscanf(line, "%s %[^\n]", parameter, line) == 2) {
            if (sscanf(line, "%s %[^\n]", table_name, line) == 2) {
                if (sscanf(line, "%[^:]%*c%s %[^\n]", field_name, value, line) == 2) {
                    toUpperCase(table_name);
                    if (!strcmp(parameter, U)) {
                        busRegU(table_name, field_name, value); return;
                    } else if (!strcmp(parameter, N)) {
                        busRegN(table_name, field_name, value); return;
                    } else {
                        raiseError(BR_WRONG_PARAMETER);
                    }
                }
            }
        }
        raiseError(BR_WRONG_SINTAX);
    } else if (!strcmp(cmd, AR)) {
        if (sscanf(line, "%s %[^\n]", table_name, line) == 1) {
            toUpperCase(table_name);
            apReg(table_name); return;
        }
        raiseError(AR_WRONG_SINTAX);
    } else if (!strcmp(cmd, RR)) {
        if (sscanf(line, "%s %[^\n]", table_name, line) == 1) {
            toUpperCase(table_name);
            removeReg(table_name); return;
        }
        raiseError(RR_WRONG_SINTAX);
    } else if (!strcmp(cmd, CI)) {
        if (sscanf(line, "%s %[^\n]", parameter, line) == 2) {
            if (sscanf(line, "%s %[^\n]", table_name, line) == 2) {
                if (sscanf(line, "%s %[^\n]", field_name, line) == 1) {
                    toUpperCase(table_name);
                    if (!strcmp(parameter, A)) {
                        createIndexA(table_name, field_name); return;
                    } else if (!strcmp(parameter, H)) {
                        createIndexH(table_name, field_name); return;
                    } else {
                        raiseError(CI_WRONG_PARAMETER);
                    }
                }
            }
        }
        raiseError(CI_WRONG_SINTAX);
    } else if (!strcmp(cmd, RI)) {
        if (sscanf(line, "%s %[^\n]", table_name, line) == 2) {
            if (sscanf(line, "%s %[^\n]", field_name, line) == 1) {
                toUpperCase(table_name);
                removeIndex(table_name, field_name); return;
            }
        }
        raiseError(RI_WRONG_SINTAX);
    } else if (!strcmp(cmd, GI)) {
        if (sscanf(line, "%s %[^\n]", table_name, line) == 2) {
            if (sscanf(line, "%s %[^\n]", field_name, line) == 1) {
                toUpperCase(table_name);
                genIndex(table_name, field_name); return;
            }
        }
        raiseError(GI_WRONG_SINTAX);
    } else if (!strcmp(cmd, EB)) {
        raiseError(EXIT);
    } else {
        raiseError(NO_COMMAND);
    }
}