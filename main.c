#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 

// Comandos
#define CT "CT"
#define RT "RT"
#define AT "AT"
#define LT "LT"
#define IR "IR"
#define BR "BR"
#define AR "AR"
#define S "S"

// Parâmetros do BR
#define U "U"
#define N "N"

// Tipos
#define STR "STR"
#define BIN "BIN"
#define INT "INT"
#define FLT "FLT"

// Tamanho máximo
#define CMD_MAX 256
#define TBLNM_MAX 32
#define TYPE_MAX 4
#define FIELD_MAX 32
#define VALUE_MAX 32
#define N_COLUMNS 32

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

// Printa os comandos
void menu() {
    printf("Escolha uma das opções abaixo:\n");

    printf("Criar um arquivo em uma tabela:\n");
    printf("\'CT NOME_TABELA TIPO:NOME_CAMPO;...\'\'\n\n");

    printf("Apagar um arquivo referente a uma tabela:\n");
    printf("\'RT NOME_TABELA\'\n\n");

    printf("Apresentar metadados da tabela indicada:\n");
    printf("\'AT NOME_TABELA\'\n\n");

    printf("Listar os nomes de todas as tabelas existentes na base:\n");
    printf("\'LT\'\n\n");

    printf("Inserir um registro em uma tabela:\n");
    printf("\'IR NOME_TABELA VALOR;...\'\n\n");

    printf("Buscar um registro em uma tabela:\n");
    printf("\'BR PARÂMETRO NOME_TABELA NOME_CAMPO:VALOR\'\n\n");

    printf("Apresentar a última busca referente a uma tabela:\n");
    printf("\'AR NOME_TABELA\'\n\n");

    printf("Digite \'S\' para encerrar.\n");
}

// Remove a tabeta
void removeTabela(char *table_name) {
    printf("Removendo a tabela %s.\n", table_name);
    printf("TODO\n");
}

// Apresenta a tabeta
void apresentaTabela(char *table_name) {
    printf("Mostrando a tabela %s.\n", table_name);
    printf("TODO\n");
}

// Lista as tabetas
void listaTabelas() {
    printf("Listando tabelas.\n");
    printf("TODO\n");
}

// Apresenta a pesquisa da tabela
void apresentaRegistro(char *table_name) {
    printf("Mostrando a pesquisa de %s.\n", table_name);
    printf("TODO\n");
}

// Busca registro em tabela, único
void buscarRegistroU(char *table_name, char *field_name, char *value) {
    printf("Buscando %s igual à %s em %s.\n", table_name, field_name, value);
    printf("TODO\n");
}

// Busca registro em tabela, único
void buscarRegistroN(char *table_name, char *field_name, char *value) {
    printf("Buscando todos os %s igual à %s em %s.\n", table_name, field_name, value);
    printf("TODO\n");
}

// Cria um tabela
void criarTabela(char *table_name, char type_name_arr[N_COLUMNS][TYPE_MAX], char field_name_arr[N_COLUMNS][FIELD_MAX], int iTypeField) {
    printf("Criando tabela %s.\n", table_name);

    for (int i = 0; i < iTypeField; i++) {
        printf("%s %s\n", type_name_arr[i], field_name_arr[i]);
    }

    printf("TODO\n");
}

// Inclui um registro em uma tabela
void incluiRegistro(char *table_name) {
    printf("Novo registro na tabela %s.\n", table_name);
    printf("TODO\n");
}

// Procura em uma string e retorna NULL ou o ponteiro após a string procurada
char *find(char *haystack, const char *needle) {
    int i;
    for (i = 0; needle[i]; i++) {}

    char *r = strstr(haystack, needle);

    if (r) {
        return r + i;
    }

    return NULL;
}

void start() {
    char comando[CMD_MAX];
    char *parsing;

    char table_name[TBLNM_MAX];
    char field_name[FIELD_MAX];
    char value[VALUE_MAX];

    char type_name_arr[N_COLUMNS][TYPE_MAX];
    char field_name_arr[N_COLUMNS][FIELD_MAX];
    int iTypeField = 0;

    fgets(comando, CMD_MAX, stdin); // Lê o comando
    toUpperCase(comando); //Não diferenciação de letras maiúsculas e minúsculas

    // Identifica o comando
    if (parsing = find(comando, CT)) {
        if(sscanf(parsing, "%s %[^:]%*c%[^;]", table_name, type_name_arr[iTypeField], field_name_arr[iTypeField]) != 3) {
            printf("Erro ao criar tabela.\n");
            return;
        }

        iTypeField++;

        while (parsing = find(parsing, ";")) {
            if(sscanf(parsing, "%[^:]%*c%[^;]", type_name_arr[iTypeField], field_name_arr[iTypeField]) != 2) {
                printf("Erro ao criar tabela.\n");
                return;
            }

            iTypeField++;
        }

        criarTabela(table_name, type_name_arr, field_name_arr, iTypeField);
    } else if (parsing = find(comando, RT)) {
        if (sscanf(parsing, "%s", table_name) != 1) {
            printf("Erro ao remover tabela.\n");
            return;
        }

        removeTabela(table_name);
    } else if (parsing = find(comando, AT)) {
        if (sscanf(parsing, "%s", table_name) != 1) {
            printf("Erro ao apresentar tabela.\n");
            return;
        }
        apresentaTabela(table_name);
    } else if (parsing = find(comando, LT)) {
        listaTabelas();
    } else if (parsing = find(comando, IR)) {
        if (sscanf(parsing, "%s", table_name) != 1) {
            printf("Erro ao incluir registro.\n");
            return;
        }

        // TODO

        incluiRegistro(table_name);
    } else if (parsing = find(comando, BR)) {
        if (parsing = find(comando, U)) {
            if (sscanf(parsing, "%s %s:%s", table_name, field_name, value) != 3) {
                printf("Erro ao buscar registro.\n");
                return;
            }

            buscarRegistroU(table_name, field_name, value);
        } else if (parsing = find(comando, N)) {
            if (sscanf(parsing, "%s %s:%s", table_name, field_name, value) != 3) {
                printf("Erro ao buscar registro.\n");
                return;
            }

            buscarRegistroN(table_name, field_name, value);
        } else {
            printf("Parâmetro não reconhecido.\n");
            return;
        }
    } else if (parsing = find(comando, AR)) {
        if (sscanf(parsing, "%s", table_name) != 1) {
            printf("Erro ao mostrar busca.\n");
            return;
        }
        apresentaRegistro(table_name);
    } else if (parsing = find(comando, S)) {
        printf("Saindo.\n");
        return;
    } else {
        printf("Nenhum comando encontrado!\n");
    }

    start();
}

int main() {
    menu();
    start();

    return 0;
}