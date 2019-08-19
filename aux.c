#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aux.h"

//Função que recebe uma string e quebra ela em pedaços separados pelo splitter. Retorno: vetor de palavras
char ** split(char * str, char splitter) {
    int contSplitters = 0, contLetras = 0, contAux = 0, iSplitted = 0;
    char * aux = (char *) malloc(strlen(str) * sizeof (char));
    memset(aux, '\0', sizeof (aux));
    for (int i = 0; i < strlen(str); i++) { //Retira splitters repetidos
        if (str[i] != splitter ||
                (str[i] == splitter && str[i + 1] != splitter)) {
            aux[contAux++] = str[i];
        }
        if (str[i] == splitter && str[i + 1] != splitter) contSplitters;
    }
    char ** splitted = (char **) malloc((contSplitters + 1) * sizeof (char *));
    for (int i = 0; i < strlen(aux); i++) {
        if (aux[i] == splitter) {
            splitted[iSplitted++] = (char *) malloc((contLetras + 2) * sizeof (char));
            contLetras = 0;
        }
        contLetras++;
    }
    splitted[iSplitted++] = (char *) malloc((contLetras + 1) * sizeof (char));
    splitted[iSplitted] = NULL;
    iSplitted = 0;
    int cont = 0;
    for (int i = 0; i < strlen(aux); i++) {
        if (aux[i] == splitter) {
            splitted[iSplitted][cont] = '\0';
            iSplitted++;
            cont = 0;
        } else {
            splitted[iSplitted][cont++] = aux[i];
        }
    }
    return splitted;
}

// Função para transformar as letras de uma string em letras maíusculas
void toUpperCase(char * str) {
    int i = -1;
    while (str[++i] != '\0') {
        if (str[i] >= 'a' && str[i] <= 'z') str[i] = str[i] - 32;
    }
}

// Procura em uma string e retorna NULL ou o ponteiro após a string procurada
// Inclui o limit, número de chars sem matching
// limit == -1 default, equivalente a limite infinito
char *findl(char *haystack, const char *needle, int limit) {
    int i, j = 0, k = 0;
    
    for (i = 0; needle[i]; i++);

    while (haystack[j] != '\0' && k < i && ((j-k) <= limit || limit == -1)) {
        if (haystack[j] == needle[k] || haystack[j] == (needle[k] + 32))
            k++;
        j++;
    }

    if (k == i) return &haystack[j];

    return NULL;
}

char *find(char *haystack, const char *needle) {
    return findl(haystack, needle, -1);
}