#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

//Função para transformar as letras de uma string em letras maíusculas

void toUpperCase(char * str) {
    int i = -1;
    while (str[++i] != '\0') {
        if (str[i] >= 'a' && str[i] <= 'z') str[i] = str[i] - 32;
    }
}

//Lista os nomes das tabelas presentes na base

void listTable() {
    printf("Lista das tabelas presentes na base de dados: \n\n");
    printf("%s\n", p->name);
}

void readFile(table *p) {
    FILE *descritor;

    //abre o arquivo
    descritor = fopen("arquivo.bin", "rb");

    //verificacao de erro
    if (!descritor) {
        printf("Erro ao abrir o arquivo\n");
    } else {
        while (fread(p, sizeof (table), 1, descritor) > 0)
            listTable(p);
        fclose(descritor);
    }
}

void menu() {

    printf("Escolha uma das opções abaixo:\n");

    printf("Criar um arquivo em uma tabela: \n");
    printf("CT tabela campos \n\n");

    printf("Apagar um arquivo referente a uma tabela: \n");
    printf("RT tabela \n\n");

    printf("Apresentar metadados da tabela indicada: \n");
    printf("AT tabela \n\n");

    printf("Listar os nomes de todas as tabelas existentes na base: \n");
    printf("LT\n");

    printf("Digite 'S' para encerrar.\n");
}

void start() {
    char option[2],
            stop = '',
            nomeTabela[100]; //modificar para alocacao dinamica do tamanho da string 

    menu();
    scanf("%s", &option);

    toUpperCase(option); //Não diferenciação de letras maiúsculas e minúsculas

    switch (option) {
        case 'CT':
            printf("Criando arquivo...\n");
            do {
                //newTable(&p);
                //saveTable(&p);
                printf("Continuar: 1 | Parar: 0\n");
            } while (stop != 0 && stop == 1)
                break;

        case 'RT':
            printf("Digite o nome da tabela: \n",);
            fgets(nomeTabela);
            printf("Apagando o arquivo...\n");
            delTable(&p, nomeTabela);
            break;

        case 'LT'
            printf("Listando os nomes das tabelas... \n");
            readFile(&p);
            break;

        case 'S':
            break;

        default:
            printf("Opção inválida!\n");
            break;
    }

    int main() {
        start();
        return 0;
    }





