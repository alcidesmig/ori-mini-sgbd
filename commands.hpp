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
#include "commandsTools.hpp"
#include "searchStructs.h"
#include "btree/disktree.hpp"
#include "hash/hash.h"

void criarTabela(Table *table);
void removerTabela(Table *table);
void apresentarTabela(Table *table);
void listarTabela();
void incluirRegistro(Row *row);
void buscarRegistros(Selection *selection);
void apresentarRegistros(Selection *selection);
void removerRegistros(Selection *selection);
void criarIndex(Selection *selection);
void removerIndex(TableName tableName, Field field, int imprime, int all);
void gerarIndex(Selection *selection, char op);
void start();
void end();

#endif /* COMMANDS_H */