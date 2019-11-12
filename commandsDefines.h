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
	int size;
	long int pos;
	long int next;
} EmptyBlock;

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

extern int qtTables;

extern int CMD_LIMIT;
extern int PARAMETER_LIMIT;

extern FILE *tablesIndex;

extern ResultNode *resultTree;

#endif /* COMMANDS_DEFINES_H */