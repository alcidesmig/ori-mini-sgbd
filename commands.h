#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "defines.h"
#include "utils.h"

extern const char TABLES_INDEX[];
extern const char TABLES_DIR[];

extern const char CT[];
extern const char RT[];
extern const char AT[];
extern const char LT[];
extern const char IR[];
extern const char BR[];
extern const char AR[];
extern const char RR[];
extern const char CI[];
extern const char RI[];
extern const char GI[];
extern const char EB[];

extern const char U[];
extern const char N[];
extern const char A[];
extern const char H[];

extern const char *commands[];

extern int CMD_LIMIT;
extern int PARAMETER_LIMIT;

extern FILE *tablesIndex;

void criarTabela(Table *table);
void removerTabela(Table *table);
void apresentarTabela(Table *table);
void listarTabela(Table *table);
void incluirRegistro(Row *row);
void buscarRegistros(Row *row);
void apresentarRegistros(Row *row);
void removerRegistros(Row *row);
void criarIndex(Selection *selection);
void removerIndex(Selection *selection);
void gerarIndex(Selection *selection);
void start();
void end();
int tableNameIsUnique(int qtTables, char *name, long int *marker);
void addTableName(int qtTables, char *name);

#endif /* COMMANDS_H */