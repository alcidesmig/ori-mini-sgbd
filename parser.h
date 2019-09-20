#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "utils.h"
#include "defines.h"

char validateType(char *type);
ParsedData *parser(char *line);

#endif /* PARSER_H */