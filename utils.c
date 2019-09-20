#include "utils.h"

void *safe_malloc(size_t s) {
    void *p = malloc(s);
    if (p)
        return p;
    fprintf(stderr, "Malloc ERROR!\n");
    exit(1);
}

void toUpperCase(char *str) {
    int i = -1;
    while (str[++i] != '\0')
        if (str[i] >= 'a' && str[i] <= 'z') str[i] = str[i] - 32;
}

int replaceSpace(char *str, char c) {
    int flag = 0;

    for(int i = 0; str[i]; i++) {
        if (str[i] == ' ') {
            str[i] = c;
            flag = 1;
        }
    }

    return flag;
}