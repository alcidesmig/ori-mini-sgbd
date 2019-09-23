#ifndef COMMANDS_DEFINES_H
#define COMMANDS_DEFINES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "defines.h"
#include "utils.h"
#include "searchStructs.h"

#define NUM_COMMANDS 12

typedef struct {
	long int pos;
	int size;
} EmptyBlock;

typedef struct EmptyBlockList {
	struct EmptyBlockList *next;
	EmptyBlock *data;
} EmptyBlockList;

extern const char TABLES_INDEX[];
extern const char STRINGS_FILE[];
extern const char STRINGS_EMPTY_LIST[];
extern const char BINARIES_FILE[];
extern const char BINARIES_EMPTY_LIST[];
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

extern int qtTables;

extern int CMD_LIMIT;
extern int PARAMETER_LIMIT;

extern FILE *tablesIndex;
extern FILE *stringsFile;
extern FILE *stringsEmptyList;
extern FILE *binariesFile;
extern FILE *binariesEmptyList;

extern EmptyBlockList *stringEBlocks;
extern EmptyBlockList *binaryEBlocks;

extern ResultNode *resultTree;

#endif /* COMMANDS_DEFINES_H */