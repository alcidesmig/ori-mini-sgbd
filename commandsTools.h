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
void loadEmptyList(FILE *fp, EmptyBlockList **list);
void saveEmptyList(FILE *fp, EmptyBlockList **list);
long int addToExFile(char *str, FILE *dataFile, EmptyBlockList **list);
void removeFromExFile(long int pos, FILE *dataFile, EmptyBlockList **list);
void addInOrderToEBList(EmptyBlockList **list, long int pos, int size);

#endif /* COMMANDS_TOOLS_H */