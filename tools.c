#include "tools.h"

const int ZERO = 0;

// Cria o arquivo de index
int init() {
    // Index de tabelas
    fclose(fopen(TABLES_INDEX, "ab"));

    return 1;
}

// Verifica a existência de uma tabela com o nome especificado
// name: Nome da tabela
int tableNameExists(TableName *names, char *name) {
    for (int i = 0; i < qt_tables; i++) {
        if(strcmp(names[i], name) == 0) {
            return 1;
        }
    }

    return 0;
}

// Converte uma TableWType para uma TableWRep
int convertToRep(TableWRep *tableR, TableWType *tableT) {
    strcpy((*tableR).name, (*tableT).name);
    (*tableR).cols = (*tableT).cols;
    
    int s = (*tableT).cols;

    for (int i = 0; i < s; i++) {
        if(strcmp((*tableT).types[i], STR) == 0)
            (*tableR).types[i] = STR_REP;
        else if(strcmp((*tableT).types[i], INT) == 0)
            (*tableR).types[i] = INT_REP;
        else if(strcmp((*tableT).types[i], FLT) == 0)
            (*tableR).types[i] = FLT_REP;
        else if(strcmp((*tableT).types[i], BIN) == 0)
            (*tableR).types[i] = BIN_REP;
        else
            return 0;

        strcpy((*tableR).fields[i], (*tableT).fields[i]);
    }

    return 1;
}

// Converte uma TableWRep para uma TableWType
void convertToType(TableWType *tableT, TableWRep *tableR) {
    strcpy((*tableT).name, (*tableR).name);
    (*tableT).cols = (*tableR).cols;
    
    int s = (*tableR).cols;

    for (int i = 0; i < s; i++) {
        if((*tableR).types[i] == STR_REP)
            strcpy((*tableT).types[i], STR);
        else if((*tableR).types[i] == INT_REP)
            strcpy((*tableT).types[i], INT);
        else if((*tableR).types[i] == FLT_REP)
            strcpy((*tableT).types[i], FLT);
        else if((*tableR).types[i] == BIN_REP)
            strcpy((*tableT).types[i], BIN);

        strcpy((*tableT).fields[i], (*tableR).fields[i]);
    }
}

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

int countSpaces(char * str){
    int cont = 0;
    for (int i = 0; i < strlen(str); i++) {
        if(*(str + i) == ' ') cont++;
    }
    return cont;
}

// Retira espaços indesejados no meio dos tipos e substitui os espaços dos campos por underline -> Para CT
int fixingCommandCT(char * command) {
    if(countSpaces(command) < 2) return 0;
    char * beginStruct = strstr((strstr(command, " ") + 1), " ") + 1;
    void bringBack(char * str, int qt) {
        for(int i = 0; i < strlen(str); i++) {
            str[i] = str[i+qt];
        }
    }
    int inType = 0;
    for(int i = 0; beginStruct[i] != '\0'; i++){
        if(beginStruct[i] == ':') inType = 1;
        else if(beginStruct[i] == ';') inType = 0;
        if(beginStruct[i] == ' '){
            if(!inType) {
                bringBack(&beginStruct[i], 1);
            } else {
                beginStruct[i] = '_';
            }
        }
    }
    return 1;
}

// Substitui espaços dos campos por underline
void underlinaizer(Field field) {
    int i = 0;
    while(field[i] != '\0') {
        if (field[i] == ' ') {
            field[i] = '_';
        }
        i++;
    }
}

// Printa mensagens de acordo com o erro
void errorHandler(int error) {
    switch (error) {
        case NONE:
            break;
        case IN_ERROR:
            printf("Erro interno.\n");
            break;
        case TODO:
            printf("\tTODO.\n");
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
            printf("DEFAULT %x\n", error);
            break;
    }
}

// Printa mensagens de acordo com o erro
void errorHandlerExec(int error) {
    switch (error) {
        case NONE:
            break;
        case IN_ERROR:
            printf("Erro interno.\n");
            break;
        case TODO:
            printf("\tTODO.\n");
            break;
        case CT_SUCCESS:
            printf("Tabela criada.\n");
            break;
        case CT_WRG_TYPE:
            printf("Tipo de dado não suportado.\n");
            break;
        case CT_TBL_EXT:
            printf("Uma tabela com o mesmo nome já existe.\n");
            break;
        case CT_FAILED:
            printf("Erro ao criar a tabela.\n");
            break;
        case LT_SUCCESS:
            break;
        case LT_FAILED:
            printf("Erro ao listar tabelas.\n");
            break;
        default:
            printf("DEFAULT %x\n", error);
            break;
    }
}

int prepline() {
    printf("SGDB>");

    return 1;
}