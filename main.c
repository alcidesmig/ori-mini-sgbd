#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Comandos
#define CT "CT"
#define RT "RT"
#define AT "AT"
#define LT "LT"
#define IR "IR"
#define BR "BR"
#define AR "AR"
#define RR "RR"
#define CI "CI"
#define RI "RI"
#define GI "GI"
#define EB "EB"

// Parâmetros do BR
#define U "U"
#define N "N"
#define A "A"
#define H "H"

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

// Arquivos
#define TABLES_FILE "tables.bin"

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
    printf("\'%s NOME_TABELA TIPO:NOME_CAMPO;...\'\n\n", CT);

    printf("Apagar um arquivo referente a uma tabela:\n");
    printf("\'%s NOME_TABELA\'\n\n", RT);

    printf("Apresentar metadados da tabela indicada:\n");
    printf("\'%s NOME_TABELA\'\n\n", AT);

    printf("Listar os nomes de todas as tabelas existentes na base:\n");
    printf("\'%s\'\n\n", LT);

    printf("Inserir um registro em uma tabela:\n");
    printf("\'%s NOME_TABELA VALOR;...\'\n\n", IR);

    printf("Buscar um registro em uma tabela:\n");
    printf("\'%s PARÂMETRO NOME_TABELA NOME_CAMPO:VALOR\'\n\n", BR);

    printf("Apresentar a última busca referente a uma tabela:\n");
    printf("\'%s NOME_TABELA\'\n\n", AR);

    printf("Remove os registros encontrados na última busca de uma tabela:\n");
    printf("\'%s NOME_TABELA\'\n\n", RR);

    printf("Apresentar a última busca referente a uma tabela:\n");
    printf("\'%s PARÂMETRO NOME_TABELA NOME_CAMPO\'\n\n", CI);

    printf("Apresentar a última busca referente a uma tabela:\n");
    printf("\'%s NOME_TABELA NOME_CAMPO\'\n\n", RI);

    printf("Apresentar a última busca referente a uma tabela:\n");
    printf("\'%s NOME_TABELA NOME_CAMPO\'\n\n", GI);

    printf("Digite \'%s\' para encerrar.\n", EB);
}

// Remove a tabeta
void removeTable(char *table_name) {
    printf("Removendo a tabela %s.\n", table_name);
    printf("TODO\n");
}

// Apresenta a tabeta
void apTable(char *table_name) {
    printf("Mostrando a tabela %s.\n", table_name);
    printf("TODO\n");
}

// Lista as tabetas
void listTable() {
    printf("Listando tabelas.\n");
    printf("TODO\n");
}

// Apresenta a pesquisa da tabela
void apReg(char *table_name) {
    printf("Mostrando a pesquisa de %s.\n", table_name);
    printf("TODO\n");
}

// Busca registro em tabela, único
void busRegU(char *table_name, char *field_name, char *value) {
    printf("Buscando %s igual à %s em %s.\n", table_name, field_name, value);
    printf("TODO\n");
}

// Busca registro em tabela, único
void busRegN(char *table_name, char *field_name, char *value) {
    printf("Buscando todos os %s igual à %s em %s.\n", table_name, field_name, value);
    printf("TODO\n");
}

// Cria um tabela
void createTable(char *table_name, char type_name_arr[N_COLUMNS][TYPE_MAX], char field_name_arr[N_COLUMNS][FIELD_MAX], int iTypeField) {
    printf("Criando tabela %s.\n", table_name);

    for (int i = 0; i < iTypeField; i++) {
        printf("%s %s\n", type_name_arr[i], field_name_arr[i]);
    }

    printf("TODO\n");
}

// Inclui um registro em uma tabela
void includeReg(char *table_name, char value_arr[N_COLUMNS][VALUE_MAX], int iValue) {
    printf("Novo registro na tabela %s.\n", table_name);

    for (int i = 0; i < iValue; i++) {
        printf("%s\n", value_arr[i]);
    }

    printf("TODO\n");
}

// Remove registros pesquisados
void removeReg(char *table_name) {
    printf("Registros removidos de %s.\n", table_name);
    
    printf("TODO\n");
}

// Cria índice para uma tabela, árvore
void createIndexA(char *table_name, char *field_name) {
    printf("Criado um índice com árvore para %s usando a coluna %s.\n", table_name, field_name);
    
    printf("TODO\n");
}

// Cria índice para uma tabela, heap
void createIndexH(char *table_name, char *field_name) {
    printf("Criado um índice com heap para %s usando a coluna %s.\n", table_name, field_name);
    
    printf("TODO\n");
}

// Remove índice de uma tabela
void removeIndex(char *table_name, char *field_name) {
    printf("Removendo o índice de %s referente a chave %s.\n", table_name, field_name);
    
    printf("TODO\n");
}

// Gera índice para uma tabela
void genIndex(char *table_name, char *field_name) {
    printf("Gerando um índice com %s para %s usando a coluna %s.\n", "-recuperar-", table_name, field_name);
    
    printf("TODO\n");
}

// Procura em uma string e retorna NULL ou o ponteiro após a string procurada
// Inclui o limit, número de chars sem matching
// limit == -1 default, equivalente a limite infinito
char *findl(char *haystack, const char *needle, int limit) {
    int i, j = 0, k = 0;
    
    for (i = 0; needle[i]; i++);

    while (haystack[j] != '\0' && k < i && ((j-k) <= limit || limit == -1)) {
        if (haystack[j] == needle[k])
            k++;
        j++;
    }

    if (k == i) return &haystack[j];

    return NULL;
}

char *find(char *haystack, const char *needle) {
    return findl(haystack, needle, -1);
}

// Reconhece o comando
int parser(char *command) {
    char *parsing;

    char table_name[TBLNM_MAX];
    char field_name[FIELD_MAX];
    char value[VALUE_MAX];

    char type_name_arr[N_COLUMNS][TYPE_MAX];
    char field_name_arr[N_COLUMNS][FIELD_MAX];
    int iTypeField = 0;

    char value_arr[N_COLUMNS][VALUE_MAX];
    int iValue = 0;

    if (parsing = findl(command, CT, 0)) {
        if(sscanf(parsing, "%s %[^:]%*c%[^;]", table_name, type_name_arr[iTypeField], field_name_arr[iTypeField]) != 3) {
            printf("Erro ao criar tabela.\n");
            return 0;
        }

        iTypeField++;

        while (parsing = find(parsing, ";")) {
            if(sscanf(parsing, "%[^:]%*c%[^;]", type_name_arr[iTypeField], field_name_arr[iTypeField]) != 2) {
                printf("Erro ao criar tabela.\n");

                return 0;
            }

            iTypeField++;
        }

        createTable(table_name, type_name_arr, field_name_arr, iTypeField);
    } else if (parsing = findl(command, RT, 0)) {
        if (sscanf(parsing, "%s", table_name) != 1) {
            printf("Erro ao remover tabela.\n");
            return 0;
        }

        removeTable(table_name);
    } else if (parsing = findl(command, AT, 0)) {
        if (sscanf(parsing, "%s", table_name) != 1) {
            printf("Erro ao apresentar tabela.\n");
            return 0;
        }

        apTable(table_name);
    } else if (parsing = findl(command, LT, 0)) {
        listTable();
    } else if (parsing = findl(command, IR, 0)) {
        if (sscanf(parsing, "%s %[^;]", table_name, value_arr[iValue]) != 2) {
            printf("Erro ao incluir registro.\n");
    
            return 0;
        }

        iValue++;

        while (parsing = find(parsing, ";")) {
            if (sscanf(parsing, "%[^;]", value_arr[iValue]) != 1) {
                printf("Erro ao incluir registro.\n");
                
                return 0;
            }

            iValue++;
        }

        includeReg(table_name, value_arr, iValue);
    } else if (parsing = findl(command, BR, 0)) {
        char *temp = parsing;
        if (temp = findl(parsing, U, 1)) {
            if (sscanf(temp, "%s %[^:]%*c%s", table_name, field_name, value) != 3) {
                printf("Erro ao buscar registro.\n");
                return 0;
            }

            busRegU(table_name, field_name, value);
        } else if (temp = findl(parsing, N, 1)) {
            if (sscanf(temp, "%s %[^:]%*c%s", table_name, field_name, value) != 3) {
                printf("Erro ao buscar registro.\n");
                return 0;
            }

            busRegN(table_name, field_name, value);
        } else {
            printf("Parâmetro não reconhecido.\n");
            return 0;
        }
    } else if (parsing = findl(command, AR, 0)) {
        if (sscanf(parsing, "%s", table_name) != 1) {
            printf("Erro ao mostrar busca.\n");
            return 0;
        }

        apReg(table_name);
    } else if (parsing = findl(command, RR, 0)) {
        if (sscanf(parsing, "%s", table_name) != 1) {
            printf("Erro ao deletar registros.\n");
            return 0;
        }

        removeReg(table_name);
    } else if (parsing = findl(command, CI, 0)) {
        char *temp = parsing;
        if (temp = findl(parsing, A, 1)) {
            if (sscanf(temp, "%s %s", table_name, field_name) != 2) {
                printf("Erro ao criar índice.\n");
                return 0;
            }

            createIndexA(table_name, field_name);
        } else if (temp = findl(parsing, H, 1)) {
            if (sscanf(temp, "%s %s", table_name, field_name) != 2) {
                printf("Erro ao criar índice.\n");
                return 0;
            }

            createIndexH(table_name, field_name);
        } else {
            printf("Parâmetro não reconhecido.\n");
            return 0;
        }
    } else if (parsing = findl(command, RI, 0)) {
        if (sscanf(parsing, "%s %s", table_name, field_name) != 2) {
            printf("Erro ao remover índice.\n");
            return 0;
        }

        removeIndex(table_name, field_name);
    } else if (parsing = findl(command, GI, 0)) {
        if (sscanf(parsing, "%s %s", table_name, field_name) != 2) {
            printf("Erro ao gerar índice.\n");
            return 0;
        }

        genIndex(table_name, field_name);
    } else if (parsing = findl(command, EB, 0)) {
        return 1;
    } else {
        printf("Nenhum comando encontrado!\n");
    }
}

void commandLine() {
    char comando[CMD_MAX];

    fgets(comando, CMD_MAX, stdin); // Lê o comando
    toUpperCase(comando); // Não diferenciação de letras maiúsculas e minúsculas

    // Identifica o comando
    if (parser(comando)) {
        return;
    }

    commandLine();
}

int fromFile(char *file) {
    printf("Executando %s\n", file);

    FILE *fp = fopen(file, "r");

    if (fp == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return 1;
    }

    char comando[CMD_MAX];

    // Lê os comandos
    while (fgets(comando, CMD_MAX, fp)) {
        toUpperCase(comando); //Não diferenciação de letras maiúsculas e minúsculas
        
        parser(comando); // Identifica o comando
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        menu();
        commandLine();

        printf("Saindo.\n");
    } else {
        fromFile(argv[1]);

        printf("Saindo.\n");
    }

    return 0;
}