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
#include "commandsDefines.h"
#include "commandsTools.h"
#include "searchStructs.h"
#include "btree/btree_tools.h"

void criarTabela(Table *table);
void removerTabela(Table *table);
void apresentarTabela(Table *table);
void listarTabela();
void incluirRegistro(Row *row);
void buscarRegistros(Selection *selection);
void apresentarRegistros(Selection *selection);
void removerRegistros(Selection *selection);
void criarIndex(Selection *selection);
void removerIndex(TableName tableName, int imprime);
void gerarIndex(Selection *selection);
void start();
void end();

#endif /* COMMANDS_H */