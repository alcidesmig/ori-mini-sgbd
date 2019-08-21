
#include "tools.h"

// Separa uma string usando os separadores.
// str: String
// splitter: separador
// Retorna vetor de strings
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

void toUpperCase(char * str) {
    int i = -1;

    while (str[++i] != '\0')
        if (str[i] >= 'a' && str[i] <= 'z') str[i] = str[i] - 32;
}

// Acha uma substring em string, pode haver limite de caracteres de matching
// haystack: string
// needle: substring
// limit: limite
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

// Acha uma substring em string
// haystack: string
// needle: substring
char *find(char *haystack, const char *needle) {
    return findl(haystack, needle, -1);
}

// Remove espaços no começo de um uma string
// Retorna ponteiro para a nova posição
char *stripStart(char *command) {
    int i = 0;

    while (command[i] == ' ')
        i++;

    return command + i;
}

// Printa mensagens de acordo com o erro
void errorHandler(int error) {
    switch (error) {
        case IN_ERROR:
            printf("Erro interno.\n");
            break;
        case NO_CMD:
            printf("Nenhum comando encontrado.\n");
            break;
        case CT_WS:
            printf("Sintax do comando \'%s\' errada.\n", CT);
            break;
        case CT_WS_USC:
            printf("Sintax do comando \'%s\' errada ou último \';\' sobrando.\n", CT);
            break;
        case RT_WS:
            printf("Sintax do comando \'%s\' errada.\n", RT);
            break;
        case AT_WS:
            printf("Sintax do comando \'%s\' errada.\n", AT);
            break;
        case IR_WS:
            printf("Sintax do comando \'%s\' errada.\n", IR);
            break;
        case IR_USC:
            printf("Último \';\' sobrando.\n");
            break;
        case BR_WS:
            printf("Sintax do comando \'%s\' errada.\n", BR);
            break;
        case BR_MP:
            printf("Parâmetro não encontrado.\n");
            break;
        case AR_WS:
            printf("Sintax do comando \'%s\' errada.\n", AR);
            break;
        case RR_WS:
            printf("Sintax do comando \'%s\' errada.\n", RR);
            break;
        case CI_WS:
            printf("Sintax do comando \'%s\' errada.\n", CI);
            break;
        case CI_MP:
            printf("Parâmetro não encontrado.\n");
            break;
        case RI_WS:
            printf("Sintax do comando \'%s\' errada.\n", RI);
            break;
        case GI_WS:
            printf("Sintax do comando \'%s\' errada.\n", GI);
            break;
        case EXIT:
            printf("Saindo.\n");
            break;
        default:
            printf("DEFAULT %d\n", error);
            break;
    }
}

int prepline() {
    printf("SGDB> ");

    return 1;
}