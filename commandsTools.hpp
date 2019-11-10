#ifndef COMMANDS_TOOLS_HPP
#define COMMANDS_TOOLS_HPP

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "defines.h"
#include "utils.h"
#include "commandsDefines.h"
#include "btree/disktree.hpp"

int tem_index_hash(TableName tableName, Field field);
int tem_index_tree(TableName tableName, Field field);
Type getFieldType(TableName tableName, Field field);
Table readTable(TableName name);
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
