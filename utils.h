#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void *mallocSafe(size_t s);
void createFile(const char *path);
FILE *fopenSafe(const char *path);
void toUpperCase(char *str);
int replaceSpace(char *str, char c);

#endif /* UTILS_H */