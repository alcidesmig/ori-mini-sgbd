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

#endif /* COMMANDS_H */