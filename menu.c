#include "menu.h"

// Printa os comandos
void menu() {
    printf("Escolha uma das opções abaixo:\n");

    printf("Criar um arquivo em uma tabela:\n");
    printf("\'%sNOME_TABELA TIPO:NOME_CAMPO;...\'\n\n", CT);

    printf("Apagar um arquivo referente a uma tabela:\n");
    printf("\'%sNOME_TABELA\'\n\n", RT);

    printf("Apresentar metadados da tabela indicada:\n");
    printf("\'%sNOME_TABELA\'\n\n", AT);

    printf("Listar os nomes de todas as tabelas existentes na base:\n");
    printf("\'%s\'\n\n", LT);

    printf("Inserir um registro em uma tabela:\n");
    printf("\'%sNOME_TABELA VALOR;...\'\n\n", IR);

    printf("Buscar um registro em uma tabela:\n");
    printf("\'%sPARÂMETRO NOME_TABELA NOME_CAMPO:VALOR\'\n\n", BR);

    printf("Apresentar a última busca referente a uma tabela:\n");
    printf("\'%sNOME_TABELA\'\n\n", AR);

    printf("Remove os registros encontrados na última busca de uma tabela:\n");
    printf("\'%sNOME_TABELA\'\n\n", RR);

    printf("Apresentar a última busca referente a uma tabela:\n");
    printf("\'%sPARÂMETRO NOME_TABELA NOME_CAMPO\'\n\n", CI);

    printf("Apresentar a última busca referente a uma tabela:\n");
    printf("\'%sNOME_TABELA NOME_CAMPO\'\n\n", RI);

    printf("Apresentar a última busca referente a uma tabela:\n");
    printf("\'%sNOME_TABELA NOME_CAMPO\'\n\n", GI);

    printf("Digite \' %s\' para encerrar.\n", EB);
}