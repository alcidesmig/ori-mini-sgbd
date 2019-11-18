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
long int addToExFile(char *str, FILE *dataFile, FILE *emptyFile);
void removeFromExFile(long int pos, FILE *dataFile, FILE *emptyFile);
void removeEmptyBlock(FILE *emptyFile);
void insertEmptyBlock(FILE *emptyFile, EmptyBlock *emptyBlock);

#endif /* COMMANDS_TOOLS_H */
