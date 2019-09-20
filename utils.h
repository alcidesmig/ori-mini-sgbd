#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>

void *safe_malloc(size_t s);
void toUpperCase(char *str);
int replaceSpace(char *str, char c);

#endif /* UTILS_H */