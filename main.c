#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main() {

    char option[2],
            stop = '',
            nomeTabela[100]; /* modificar para alocacao dinamica do tamanho da string */

    menu();
    scanf("%s", &option);

    switch (option) {
        case 'CT':
            printf("Criando arquivo...\n");
            do {
                newTable(&p);
                saveTable(&p);

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

        case 'S' || 's':
            return 1;
            break

        default:
            printf("Opção inválida!\n");
            break;
    }

    return 0;
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

//lista os nomes das tabelas presentes na base

void listTable() {
    printf("Lista das tabelas presentes na base de dados: \n\n");
    printf("%s\n", p->name);
}




