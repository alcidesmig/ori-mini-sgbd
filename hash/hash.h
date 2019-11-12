#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../searchStructs.h"

#define NUM_BALDES 13           // deve ser proximo da quantidade de registros (valores atuais estao apenas para teste)
#define MAX_REGS_POR_BALDE 4    // deve ser o maximo que cabe em um bloco de FS
#define TAM_LIXO 16             // deve completar o balde para ficar do tamanho do bloco

typedef struct Item {
    int chave;
    long int valor;
} Item;

typedef struct Balde {
    int qtdRegsNoBalde;
    Item itens[MAX_REGS_POR_BALDE];
    char lixo[TAM_LIXO];
    long int posContinuacao;
} Balde;

void inicializaArquivoHash(char * filename);
void insereArquivoHash(char * filename, int chave, int valor);
void buscaEmArquivoHash(char * filename, int chave, int limit, ResultList **resultList);

#endif
