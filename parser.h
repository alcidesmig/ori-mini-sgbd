#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "utils.h"
#include "tools.h"
#include "defines.h"

typedef struct {
    char *command;
    union {
        Table table;
        Row row;
        Selection selection;
    } data;
} ParsedData;

// Transforma uma linha de comando em uma struct com os dados
// line: linha de comando
// return: ponteiro para a struct com os dados
ParsedData *parser(char *line);

#endif /* PARSER_H */