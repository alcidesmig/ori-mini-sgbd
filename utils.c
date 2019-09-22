#include "utils.h"

void *mallocSafe(size_t s) {
    void *p = malloc(s);
    if (p)
        return p;
    fprintf(stderr, "Malloc ERROR!\n");
    exit(1);
}

void createFile(const char *path) {
    FILE *fp = fopen(path, "ab+");
    if (fp) {
        fclose(fp);
    } else {
        fprintf(stderr, "Erro ao abrir arquivo: %s!\n", path);
        exit(1);
    }
}

FILE *fopenSafe(const char *path, const char *mode) {
    FILE *fp = fopen(path, mode);
    if (fp) {
        return fp;
    } else {
        fprintf(stderr, "Erro ao abrir arquivo: %s!\n", path);
        exit(1);
    }
}

void removeFile(char *path) {
    if (remove(path)) {
        fprintf(stderr, "Erro ao remover arquivo: %s!\n", path);
        exit(1);
    }
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

char *glueString(int n_args, ...) {
    char **args = (char **)mallocSafe(n_args * sizeof(char*));
    int size = 0;

    va_list ap;
    va_start(ap, n_args);

    for (int i = 0; i < n_args; i++) {
        args[i] = va_arg(ap, char *);
        for (int j = 0; args[i][j]; j++) {
            size++;
        }
    }
    
    va_end(ap);
    
    size++;
    char *r = (char *)mallocSafe(size * sizeof(char));

    int k = 0;
    for (int i = 0; i < n_args; i++) {
        for (int j = 0; args[i][j]; j++) {
            r[k++] = args[i][j];
        }
    }
    r[k] = '\0';

    free(args);
    return r;
}