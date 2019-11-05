#ifndef COMMANDS_TOOLS_H
#define COMMANDS_TOOLS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "defines.h"
#include "utils.h"
#include "commandsDefines.h"
#include "btree/lista.h"
#include "btree/btree_tools.h"

void salvaBTrees(Noh * lista_btree);
void salvarBTree(TableName tableName);
void apagaBTree(TableName tableName);
BTree * encontraBTree(TableName tableName);
void carregaBTree(TableName tableName);
int haveIndexHash(TableName tableName);
int haveIndexTree(TableName tableName);
Type getFieldType(TableName tableName, Field field);
int fieldExistInTable(char *name, Field field);
int tableExists(int qtTables, char *name);
int tableNameIsUnique(int qtTables, char *name, long int *marker);
void addTableName(int qtTables, char *name);
long int addToExFile(char *str, FILE *dataFile, FILE *emptyFile);
void removeFromExFile(long int pos, FILE *dataFile, FILE *emptyFile);
void removeEmptyBlock(FILE *emptyFile);
void insertEmptyBlock(FILE *emptyFile, EmptyBlock *emptyBlock);

#endif /* COMMANDS_TOOLS_H */